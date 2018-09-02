
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "green.h"

CGreen::CGreen()
{
	//для растиетльности (с хардварным инстансингом)
	//{
	D3DVERTEXELEMENT9 InstanceGreen[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 1, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 1, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 1, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		D3DDECL_END()
	};

	CGreen::m_pDXDevice->CreateVertexDeclaration(InstanceGreen, &m_pVertexDeclarationGreen);

	CGreen::m_pDXDevice->CreateVertexBuffer(
		GREEN_MAX_ELEM_IN_DIP * sizeof(CGreenDataVertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
		&m_pTransVertBuf,
		0);

	CIRSData* tmparr = new CIRSData();
	m_aArrComFor.push_back(tmparr);
	CIRSData* tmparr2 = new CIRSData();
	m_aArrComFor.push_back(tmparr2);

	m_iCurrCountDrawObj = 0;
}

CGreen::~CGreen()
{
	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		mem_delete(m_aGreens[i]);
	}

	m_aGreens.clear();

	mem_release(m_pTransVertBuf);
	mem_release(m_pVertexDeclarationGreen);

	while (m_aArrComFor.size() > 0)
	{
		mem_delete(m_aArrComFor[0]);
		m_aArrComFor.erase(0);
	}
}

CGreen::CModel::CModel()
{
	m_aLods[0] = m_aLods[1] = m_aLods[2] = 0;
	m_uiCountObj = 0;
	m_idCountSplits = 0;
	m_pSplitsTree = 0;
	//m_pAllTrans = 0;
	m_pPhysMesh = 0;
	m_szName[0] = 0;

	m_vMax = float3(0,0,0);
	m_vMin = float3(0, 0, 0);
}

CGreen::CModel::CPhysMesh::CPhysMesh()
{
	m_pArrVertex = 0;
	m_pArrIndex = 0;
	m_iCountVertex = 0;
	m_iCountIndex = 0;
	m_pArrMtl = 0;
	m_sPathName = "";
}

CGreen::CModel::CPhysMesh::~CPhysMesh()
{
	mem_delete_a(m_pArrVertex);
	m_iCountVertex = 0;
	mem_delete_a(m_pArrIndex);
	mem_delete_a(m_pArrMtl);
	m_iCountIndex = 0;
}

CGreen::CModel::~CModel()
{
	mem_delete(m_pSplitsTree);
	//mem_delete(m_pAllTrans);
	mem_delete(m_pPhysMesh);

	mem_del(m_aLods[0]);
	if (m_aLods[0] != m_aLods[1])
		mem_del(m_aLods[1]);

	if (m_aLods[0] != m_aLods[2] && m_aLods[1] != m_aLods[2])
		mem_del(m_aLods[2]);
}

CGreen::CSegment::CSegment()
{
	for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; ++i)
		m_aSplits[i] = 0;
	
	m_pArrIDs = 0;
	m_pObjData = 0;
	m_iCountObj = 0;
	m_pBoundVolumeSys = 0;
	m_pBoundVolumeP = 0;
	m_fDistForCamera = 0;
	m_id = -1;
	m_idNonEnd = false;
}

CGreen::CSegment::~CSegment()
{
	for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		mem_delete(m_aSplits[i]);
	//mem_delete_a(m_pObjData);

	mem_release_del(m_pBoundVolumeSys);
	mem_release_del(m_pBoundVolumeP);
}

CGreen::CLod::CLod()
{
	m_pModel = 0;
}

CGreen::CLod::~CLod()
{
	m_aIDsTextures.clear();
	mem_release_del(m_pModel);
}

CGreen::CInfoRenderSegments::CInfoRenderSegments()
{
	m_ppSegments = 0; m_iCount = 0; m_iCountCom = 0;
}

CGreen::CInfoRenderSegments::~CInfoRenderSegments()
{
	mem_delete_a(m_ppSegments);
}

CGreen::CIRSData::CIRSData()
{
	
}

CGreen::CIRSData::~CIRSData()
{
	m_aQueue.clear();

	for (int i = 0; i < m_aIRS.size(); ++i)
	{
		mem_delete(m_aIRS[i]);
	}
	m_aIRS.clear();
}


void CGreen::onLostDevice()
{
	mem_release_del(m_pTransVertBuf);
}

void CGreen::onResetDevice()
{
	CGreen::m_pDXDevice->CreateVertexBuffer(
		GREEN_MAX_ELEM_IN_DIP * sizeof(CGreenDataVertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
		&m_pTransVertBuf,
		0);
}

void CGreen::preSegmentation(CModel* model, float3* min_level, float3* max_level)
{
	model->m_pSplitsTree = new CSegment();

	float3 tmpMin, tmpMax;
	float3 tmpMin2, tmpMax2;
	model->m_pSplitsTree->m_pBoundVolumeSys = SGCore_CrBound();
	SGCore_FCompBoundBox(model->m_aLods[0]->m_pModel->m_pVertexBuffer, &(model->m_pSplitsTree->m_pBoundVolumeSys), model->m_aLods[0]->m_pModel->m_uiAllVertexCount, sizeof(vertex_static));

	model->m_pSplitsTree->m_pBoundVolumeSys->getMinMax(&tmpMin2, &tmpMax2);
	model->m_vMax = tmpMax2 * (1.f + GREEN_GEN_RAND_SCALE);
	model->m_vMin = tmpMin2 * (1.f + GREEN_GEN_RAND_SCALE);
	tmpMin = *min_level;
	tmpMax = *max_level;

	tmpMax.x += model->m_vMax.x;
	tmpMax.y += model->m_vMax.y;
	tmpMax.z += model->m_vMax.z;

	tmpMin.x += model->m_vMin.x;
	tmpMin.y += model->m_vMin.y;
	tmpMin.z += model->m_vMin.z;

	float tmpX = tmpMax.x - tmpMin.x;
	float tmpY = tmpMax.y - tmpMin.y;
	float tmpZ = tmpMax.z - tmpMin.z;

	model->m_pSplitsTree->m_pBoundVolumeP = SGCore_CrBound();
	model->m_pSplitsTree->m_pBoundVolumeP->setMinMax(&tmpMin, &tmpMax);

	//выравниваем по квадрату
	if (tmpX > tmpZ)
	{
		tmpZ = tmpX - tmpZ;
		tmpZ /= 2.f;
		tmpMax.z += tmpZ;
		tmpMin.z -= tmpZ;
	}
	else if (tmpZ > tmpX)
	{
		tmpX = tmpZ - tmpX;
		tmpX /= 2.f;
		tmpMax.x += tmpX;
		tmpMin.x -= tmpX;
	}

	model->m_pSplitsTree->m_pBoundVolumeSys->setMinMax(&tmpMin, &tmpMax);

	model->m_pSplitsTree->m_iCountObj = model->m_uiCountObj;
	if (model->m_uiCountObj > 0)
	{
		//model->m_pSplitsTree->m_pObjData = new CGreenDataVertex[model->m_uiCountObj];
		//memcpy(model->m_pSplitsTree->m_pObjData, &(model->m_pAllTrans[0]), sizeof(CGreenDataVertex)* model->m_uiCountObj);

		model->m_pSplitsTree->m_pArrIDs = new ID[model->m_uiCountObj];

		for (int i = 0; i<model->m_uiCountObj; ++i)
		{
			model->m_pSplitsTree->m_pArrIDs[i] = i;
		}
	}

	if (model->m_pSplitsTree->m_iCountObj > 0)
		model->m_pSplitsTree->m_idNonEnd = true;
	else
		model->m_pSplitsTree->m_idNonEnd = false;

	cycleSegmentation(model->m_pSplitsTree, model);
}

void CGreen::cycleSegmentation(CSegment* Split, CModel* mesh)
{
	Array<CSegment*> queue;
	int tmpcount = 0;
	queue.push_back(Split);
	float width = 0;
	float depth = 0;
	float3 min, max;

	while (queue.size())
	{
		queue[0]->m_pBoundVolumeSys->getMinMax(&min, &max);
		if ((max.x - min.x)*0.5f > GREEN_BB_MIN_X && (max.z - min.z)*0.5f > GREEN_BB_MIN_Z)
		{
			segmentation(queue[0], mesh);
			for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; i++)
			{
				if (queue[0]->m_aSplits[i])
					queue.push_back(queue[0]->m_aSplits[i]);
			}
		}
		else
			queue[0]->m_idNonEnd = false;

		queue.erase(0);
		++tmpcount;
	}
}

void CGreen::segmentation(CSegment* Split, CModel* mesh)
{
	Array<DWORD> ArrPoly[4];

	for (int i = 0; i<4; i++)
		Split->m_aSplits[i] = new CSegment();

	ISXBound* ArrBound[4];
	for (int i = 0; i < 4; ++i)
		ArrBound[i] = SGCore_CrBound();

	SGCore_0ComBoundBoxArr4(Split->m_pBoundVolumeSys, (ArrBound));

	float3 tmpmin, tmpmax;
	for (int i = 0; i<4; ++i)
	{
		Split->m_aSplits[i]->m_pBoundVolumeSys = ArrBound[i];
		Split->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&tmpmin, &tmpmax);
		Split->m_aSplits[i]->m_pBoundVolumeP = SGCore_CrBound();
		Split->m_aSplits[i]->m_pBoundVolumeP->setMinMax(&tmpmin, &tmpmax);
	}

	bool *tmp_arr_mesh_poly = 0;
	if (Split->m_iCountObj)
	{
		tmp_arr_mesh_poly = new bool[Split->m_iCountObj];
		for (int i = 0; i < Split->m_iCountObj; i++)
			tmp_arr_mesh_poly[i] = true;
	}

	DWORD tmpCountNonIn = 0;
	float3 tmpMin, tmpMax;
	for (WORD i = 0; i<4; ++i)
	{
		Split->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&tmpMin, &tmpMax);
		//SGCore_FCreateBoundingBoxMesh(&tmpMin, &tmpMax, &(Split->m_aSplits[i]->BoundBox));

		for (DWORD j = 0; j<Split->m_iCountObj; ++j)
		{
			//если позици¤ провер¤емого полигона находитьс¤ в пределах ограничивающего паралелепипеда
			if (
				/*(int(tmpMax.x * 1000) >= int(Split->m_pObjData[j].m_vPosition.x * 1000) && int(tmpMin.x * 1000) <= int(Split->m_pObjData[j].m_vPosition.x * 1000))
				&&
				(int(tmpMax.z * 1000) >= int(Split->m_pObjData[j].m_vPosition.z * 1000) && int(tmpMin.z * 1000) <= int(Split->m_pObjData[j].m_vPosition.z * 1000))
				&&
				tmp_arr_mesh_poly[j]*/

				(int(tmpMax.x * 1000) >= int(mesh->m_pAllTrans[Split->m_pArrIDs[j]].m_vPosition.x * 1000) && int(tmpMin.x * 1000) <= int(mesh->m_pAllTrans[Split->m_pArrIDs[j]].m_vPosition.x * 1000))
				&&
				(int(tmpMax.z * 1000) >= int(mesh->m_pAllTrans[Split->m_pArrIDs[j]].m_vPosition.z * 1000) && int(tmpMin.z * 1000) <= int(mesh->m_pAllTrans[Split->m_pArrIDs[j]].m_vPosition.z * 1000))
				&&
				tmp_arr_mesh_poly[j]
				)
			{
				//записываем вершины в массив
				ArrPoly[i].push_back(j);
				//полигон использваон, т.е. зан¤т
				tmp_arr_mesh_poly[j] = false;
				tmpCountNonIn++;
			}
		}
	}

	for (int i = 0; i<4; i++)
	{
		Split->m_aSplits[i]->m_iCountObj = ArrPoly[i].size();

		if (Split->m_aSplits[i]->m_iCountObj > 0)
		{
			/*Split->m_aSplits[i]->m_pObjData = new CGreenDataVertex[Split->m_aSplits[i]->m_iCountObj];
			for (DWORD k = 0; k < ArrPoly[i].size(); k++)
			{
				Split->m_aSplits[i]->m_pObjData[k] = Split->m_pObjData[ArrPoly[i][k]];
			}*/

			Split->m_aSplits[i]->m_pArrIDs = new ID[Split->m_aSplits[i]->m_iCountObj];
			for (DWORD k = 0; k < ArrPoly[i].size(); k++)
			{
				Split->m_aSplits[i]->m_pArrIDs[k] = Split->m_pArrIDs[ArrPoly[i][k]];
			}

			alignBound(mesh, Split->m_aSplits[i]);

			ArrPoly[i].clear();
		}

		if (Split->m_aSplits[i]->m_iCountObj > 0)
			Split->m_aSplits[i]->m_idNonEnd = true;
		else
		{
			Split->m_aSplits[i]->m_idNonEnd = false;
		}
	}

	mem_delete_a(Split->m_pArrIDs);
	mem_delete_a(tmp_arr_mesh_poly);
}

void CGreen::alignBound(CModel* model, CSegment* split)
{
	if (split->m_iCountObj > 0)
	{
		/*float3 comMax = split->m_pObjData[0].m_vPosition;
		float3 comMin = split->m_pObjData[0].m_vPosition;

		for (int k = 0; k<split->m_iCountObj; ++k)
		{
			if (split->m_pObjData[k].m_vPosition.y > comMax.y)
				comMax.y = split->m_pObjData[k].m_vPosition.y;

			if (split->m_pObjData[k].m_vPosition.y < comMin.y)
				comMin.y = split->m_pObjData[k].m_vPosition.y;


			if (split->m_pObjData[k].m_vPosition.x > comMax.x)
				comMax.x = split->m_pObjData[k].m_vPosition.x;

			if (split->m_pObjData[k].m_vPosition.x < comMin.x)
				comMin.x = split->m_pObjData[k].m_vPosition.x;


			if (split->m_pObjData[k].m_vPosition.z > comMax.z)
				comMax.z = split->m_pObjData[k].m_vPosition.z;

			if (split->m_pObjData[k].m_vPosition.z < comMin.z)
				comMin.z = split->m_pObjData[k].m_vPosition.z;
		}*/

		float3 comMax = model->m_pAllTrans[split->m_pArrIDs[0]].m_vPosition;
		float3 comMin = model->m_pAllTrans[split->m_pArrIDs[0]].m_vPosition;

		for (int k = 0; k<split->m_iCountObj; ++k)
		{
			if (model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.y > comMax.y)
				comMax.y = model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.y;

			if (model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.y < comMin.y)
				comMin.y = model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.y;


			if (model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.x > comMax.x)
				comMax.x = model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.x;

			if (model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.x < comMin.x)
				comMin.x = model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.x;


			if (model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.z > comMax.z)
				comMax.z = model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.z;

			if (model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.z < comMin.z)
				comMin.z = model->m_pAllTrans[split->m_pArrIDs[k]].m_vPosition.z;
		}

		float3 tmpMin, tmpMax;
		float scalecoef = 1.f + GREEN_GEN_RAND_SCALE;

		split->m_pBoundVolumeSys->getMinMax(&tmpMin, &tmpMax);
		tmpMax.y = comMax.y + model->m_vMax.y * scalecoef;
		tmpMin.y = comMin.y + model->m_vMin.y * scalecoef;

		split->m_pBoundVolumeSys->setMinMax(&tmpMin, &tmpMax);

		tmpMax.x = comMax.x + model->m_vMax.x * scalecoef;
		tmpMax.y = comMax.y + model->m_vMax.y * scalecoef;
		tmpMax.z = comMax.z + model->m_vMax.z * scalecoef;

		tmpMin.x = comMin.x + model->m_vMin.x * scalecoef;
		tmpMin.y = comMin.y + model->m_vMin.y * scalecoef;
		tmpMin.z = comMin.z + model->m_vMin.z * scalecoef;

		split->m_pBoundVolumeP->setMinMax(&tmpMin, &tmpMax);
	}
}

void CGreen::setSplitID(CModel* model)
{
	Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> queue;
	int tmpcount = 0;
	queue.push_back(model->m_pSplitsTree);

	//model->m_pAllTrans = new CGreenDataVertex[model->m_uiCountObj];

	while (queue.size())
	{
		setSplitID2(model, queue[0], &queue);

		/*if (queue[0]->m_iCountObj > 0 && queue[0]->m_pObjData)
		{
			for (int i = 0; i < queue[0]->m_iCountObj; ++i)
			{
				queue[0]->m_pArrIDs[i] = model->m_pAllTrans.size();
				model->m_pAllTrans.push_back(queue[0]->m_pObjData[i]);
			}
		}*/

		queue.erase(0);
		++tmpcount;
	}
}

void CGreen::setSplitID2(CModel* model, CSegment* Split, Array<CSegment*, GREEN_DEFAULT_RESERVE_COM>* queue)
{
	if (Split)
	{
		Split->m_id = model->m_idCountSplits;
		model->m_aSplitsArr[Split->m_id] = Split;
		++(model->m_idCountSplits);
		for (int i = 0; i < 4; i++)
		{
			if (Split->m_aSplits[i])
				queue->push_back(Split->m_aSplits[i]);
		}
	}
}

void CGreen::ComputeBB(const CGreenDataVertex &oTrans, CBoundBox &oBox)
{
	float3_t vMin, vMax;
	vMin = oBox.m_vMin;
	vMax = oBox.m_vMax;

	float3 aPoints[8];

	aPoints[0] = float3(vMax.x, vMax.y, vMax.z);
	aPoints[1] = float3(vMax.x, vMax.y, vMin.z);
	aPoints[2] = float3(vMax.x, vMin.y, vMax.z);
	aPoints[3] = float3(vMin.x, vMax.y, vMax.z);
	aPoints[4] = float3(vMax.x, vMin.y, vMin.z);
	aPoints[5] = float3(vMin.x, vMin.y, vMax.z);
	aPoints[6] = float3(vMin.x, vMax.y, vMin.z);
	aPoints[7] = float3(vMin.x, vMin.y, vMin.z);

	for (int k = 0; k < 8; ++k)
	{
		float3 vNewPos;
		vNewPos.x = aPoints[k].x * oTrans.m_vSinCosRot.y + aPoints[k].z * oTrans.m_vSinCosRot.x;
		vNewPos.z = -aPoints[k].x * oTrans.m_vSinCosRot.x + aPoints[k].z * oTrans.m_vSinCosRot.y;
		vNewPos.y = aPoints[k].y;
		aPoints[k] = vNewPos;

		aPoints[k] *= oTrans.m_vTexCoord.x;

		aPoints[k] += oTrans.m_vPosition;
	}

	vMin = aPoints[0];
	vMax = aPoints[0];

	for (int k = 0; k < 8; ++k)
	{
		if (aPoints[k].x > vMax.x)
			vMax.x = aPoints[k].x;

		if (aPoints[k].y > vMax.y)
			vMax.y = aPoints[k].y;

		if (aPoints[k].z > vMax.z)
			vMax.z = aPoints[k].z;


		if (aPoints[k].x < vMin.x)
			vMin.x = aPoints[k].x;

		if (aPoints[k].y < vMin.y)
			vMin.y = aPoints[k].y;

		if (aPoints[k].z < vMin.z)
			vMin.z = aPoints[k].z;
	}

	oBox.m_vMin = vMin;
	oBox.m_vMax = vMax;
}

void CGreen::ComputeBBtrans(ID idGreen)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID_MODEL(idGreen);

	CModel *pGreen = m_aGreens[idGreen];

	pGreen->m_aTransW.clear();

	float3 vMin, vMax;
	float4 aPoints[8];
	float2_t vSinCos;
	float3 vPosition;
	float fMultiplier;
	

	pGreen->m_aTransW.reserve(pGreen->m_pAllTrans.size());
	for (int i = 0, il = pGreen->m_pAllTrans.size(); i < il; ++i)
	{
		vMin = pGreen->m_vMin;
		vMax = pGreen->m_vMax;
		vSinCos = pGreen->m_pAllTrans[i].m_vSinCosRot;
		vPosition = pGreen->m_pAllTrans[i].m_vPosition;
		fMultiplier = pGreen->m_pAllTrans[i].m_vTexCoord.x;
		
		aPoints[0] = float4(vMax.x, vMax.y, vMax.z, 1.0f);
		aPoints[1] = float4(vMax.x, vMax.y, vMin.z, 1.0f);
		aPoints[2] = float4(vMax.x, vMin.y, vMax.z, 1.0f);
		aPoints[3] = float4(vMin.x, vMax.y, vMax.z, 1.0f);
		aPoints[4] = float4(vMax.x, vMin.y, vMin.z, 1.0f);
		aPoints[5] = float4(vMin.x, vMin.y, vMax.z, 1.0f);
		aPoints[6] = float4(vMin.x, vMax.y, vMin.z, 1.0f);
		aPoints[7] = float4(vMin.x, vMin.y, vMin.z, 1.0f);
		
		for (int k = 0; k < 8; ++k)
		{
			float3 vNewPos;
			vNewPos.x = aPoints[k].x * vSinCos.y + aPoints[k].z * vSinCos.x;
			vNewPos.z = -aPoints[k].x * vSinCos.x + aPoints[k].z * vSinCos.y;
			vNewPos.y = aPoints[k].y;
			aPoints[k] = vNewPos;

			aPoints[k] *= fMultiplier;

			aPoints[k] += vPosition;
		}

		vMin = aPoints[0];
		vMax = aPoints[0];

		for (int k = 0; k < 8; ++k)
		{
			if (aPoints[k].x > vMax.x)
				vMax.x = aPoints[k].x;

			if (aPoints[k].y > vMax.y)
				vMax.y = aPoints[k].y;

			if (aPoints[k].z > vMax.z)
				vMax.z = aPoints[k].z;


			if (aPoints[k].x < vMin.x)
				vMin.x = aPoints[k].x;

			if (aPoints[k].y < vMin.y)
				vMin.y = aPoints[k].y;

			if (aPoints[k].z < vMin.z)
				vMin.z = aPoints[k].z;
		}

		
		CBoundBox oBB;

		oBB.m_vMin = vMin;
		oBB.m_vMax = vMax;
		//pGreen->m_aTransW[i] = oBB;
		pGreen->m_aTransW.push_back(oBB);
	}
}

void CGreen::comArrIndeces(const IFrustum* frustum, const float3* viewpos, ID id_arr)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

	int tmpcount = 0;
	int* tmpcountcom = 0;
	CSegment** tmpsegments = 0;
	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		if(m_aArrComFor[id_arr]->m_aVisible[i].size() < m_aGreens[i]->m_pAllTrans.size())
		{
			m_aArrComFor[id_arr]->m_aVisible[i].resize(m_aGreens[i]->m_pAllTrans.size());
		}
		assert(m_aArrComFor[id_arr]->m_aVisible[i].size() >= m_aGreens[i]->m_pAllTrans.size());
		memset(&(m_aArrComFor[id_arr]->m_aVisible[i][0]), 0, sizeof(bool)* m_aGreens[i]->m_pAllTrans.size());
		tmpcount = m_aArrComFor[id_arr]->m_aIRS[i]->m_iCount;
		m_aArrComFor[id_arr]->m_aIRS[i]->m_iCountCom = 0;
		tmpcountcom = &(m_aArrComFor[id_arr]->m_aIRS[i]->m_iCountCom);
		tmpsegments = m_aArrComFor[id_arr]->m_aIRS[i]->m_ppSegments;
		
		m_aArrComFor[id_arr]->m_aQueue.clearFast();
		//int tmpcount = 0;
		m_aArrComFor[id_arr]->m_aQueue.push_back(m_aGreens[i]->m_pSplitsTree);

		while (m_aArrComFor[id_arr]->m_aQueue.size())
		{
			comRecArrIndeces(i, id_arr, frustum, tmpsegments, tmpcountcom, m_aArrComFor[id_arr]->m_aQueue[0], viewpos, &m_aArrComFor[id_arr]->m_aQueue, tmpcount);

			m_aArrComFor[id_arr]->m_aQueue.erase(0);
			//++tmpcount;
		}
	}

	int qwert = 0;
}

void CGreen::comRecArrIndeces(ID idGreen, ID idArr, const IFrustum* frustum, CSegment** arrsplits, int *count, CSegment* comsegment, const float3* viewpos, Array<CSegment*, GREEN_DEFAULT_RESERVE_COM>* queue, ID curr_splits_ids_render)
{
	float3 jcenter;
	float jradius;
	ID SortId[GREEN_COUNT_TYPE_SEGMENTATION];
	comsegment->m_pBoundVolumeP->getSphere(&jcenter, &jradius);

	if (comsegment->m_iCountObj > 0 && frustum->sphereInFrustum(&jcenter, jradius))
	{
			if (comsegment->m_idNonEnd)
			{
				if (CGreen::m_isUseSortFrontToBackSplits)
				{
					for (int q = 0; q<GREEN_COUNT_TYPE_SEGMENTATION; ++q)
					{
						SortId[q] = -1;
						if (comsegment->m_aSplits[q])
						{
							comsegment->m_aSplits[q]->m_pBoundVolumeP->getSphere(&jcenter, &jradius);
							comsegment->m_aSplits[q]->m_fDistForCamera = SMVector3Length2((jcenter - (*viewpos))) - jradius*jradius;
						}
					}

					float pl1, pl2;
					int tmpCountGreater = 0;
					for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; ++i)
					{
						if (comsegment->m_aSplits[i])
						{
							tmpCountGreater = 0;
							pl1 = comsegment->m_aSplits[i]->m_fDistForCamera;

							for (int k = 0; k<GREEN_COUNT_TYPE_SEGMENTATION; ++k)
							{
								if (comsegment->m_aSplits[k])
								{
									pl2 = comsegment->m_aSplits[k]->m_fDistForCamera;

									if (i != k && pl2 >= pl1)
										++tmpCountGreater;
								}
							}

							bool tmpisnend = true;
							while (tmpisnend)
							{
								if (SortId[tmpCountGreater] == -1)
								{
									SortId[tmpCountGreater] = i;
									tmpisnend = false;
								}
								else
									--tmpCountGreater;
							}
						}
					}

					for (int j = 0; j<GREEN_COUNT_TYPE_SEGMENTATION; ++j)
					{
						if (SortId[(GREEN_COUNT_TYPE_SEGMENTATION - 1) - j] != -1 && comsegment->m_aSplits[SortId[(GREEN_COUNT_TYPE_SEGMENTATION-1) - j]])
							queue->push_back(comsegment->m_aSplits[SortId[(GREEN_COUNT_TYPE_SEGMENTATION-1) - j]]);
					}
				}
				else
				{
					for (int j = 0; j < GREEN_COUNT_TYPE_SEGMENTATION; ++j)
					{
						if (comsegment->m_aSplits[j])
							queue->push_back(comsegment->m_aSplits[j]);
					}
				}
			}
			else
			{
				if ((*count) < curr_splits_ids_render && /*comsegment->m_pObjData*/comsegment->m_pArrIDs)
				{
					int iCountVisible = 1;
					
					//if (GetAsyncKeyState('Y'))
					/*{
						iCountVisible = 0;
						float3 vMin, vMax;


						for (int i = 0; i < comsegment->m_iCountObj; ++i)
						{
							vMin = m_aGreens[idGreen]->m_aTransW[comsegment->m_pArrIDs[i]].m_vMin;
							vMax = m_aGreens[idGreen]->m_aTransW[comsegment->m_pArrIDs[i]].m_vMax;

							bool isVisible = frustum->boxInFrustum(&vMin, &vMax);

							if (isVisible && idArr == 0)
								isVisible = SGCore_OC_IsVisible(&vMax, &vMin);
							else if (idArr == 0)
								int qwerty = 0;

							if (isVisible)
								++iCountVisible;

							m_aArrComFor[idArr]->m_aVisible[idGreen][comsegment->m_pArrIDs[i]] = isVisible;
						}
					}*/

					if (iCountVisible > 0)
					{
						arrsplits[(*count)] = comsegment;
						comsegment->m_fDistForCamera = 0;// SMVector3Length((jcenter - (*viewpos))) - jradius;
						(*count)++;
					}
				}
			}
	}
}

void CGreen::render2(DWORD timeDelta, const float3* viewpos, ID nm, int lod, ID id_tex)
{
	//если есть что к отрисовке
	if (m_iCurrCountDrawObj)
	{
		CGreen::m_pDXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | m_iCurrCountDrawObj));

		CGreen::m_pDXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		CGreen::m_pDXDevice->SetStreamSource(1, m_pTransVertBuf, 0, sizeof(CGreenDataVertex));

		CGreen::m_pDXDevice->SetStreamSource(0, m_aGreens[nm]->m_aLods[lod]->m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static));
		CGreen::m_pDXDevice->SetIndices(m_aGreens[nm]->m_aLods[lod]->m_pModel->m_pIndexBuffer);
		CGreen::m_pDXDevice->SetVertexDeclaration(m_pVertexDeclarationGreen);

		int iCountIndex = 0;
		for (DWORD i = 0; i < m_aGreens[nm]->m_aLods[lod]->m_pModel->m_uiSubsetCount; i++)
		{
			SGCore_MtlSet((id_tex > 0 ? id_tex : m_aGreens[nm]->m_aLods[lod]->m_aIDsTextures[i]), 0);

			if (m_aGreens[nm]->m_typeGreen == GREEN_TYPE_GRASS)
				CGreen::m_pDXDevice->SetVertexShaderConstantF(59, (float*)&float2_t(CGreen::m_fDistGrassLessening, CGreen::m_vDistLods.x), 1);
			else
				CGreen::m_pDXDevice->SetVertexShaderConstantF(59, (float*)&float2_t(0,0), 1);

			CGreen::m_pDXDevice->SetVertexShaderConstantF(60, (float*)viewpos, 1);
			CGreen::m_pDXDevice->SetVertexShaderConstantF(61, (float*)&(m_aGreens[nm]->m_aLods[lod]->m_pModel->m_vBSphere), 1);
			CGreen::m_pDXDevice->SetVertexShaderConstantF(62, (float*)&(m_aGreens[nm]->m_aLods[lod]->m_pModel->m_vBBMax), 1);
			CGreen::m_pDXDevice->SetVertexShaderConstantF(63, (float*)&(m_aGreens[nm]->m_aLods[lod]->m_pModel->m_vBBMin), 1);

			SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aGreens[nm]->m_aLods[lod]->m_pModel->m_pVertexCount[i], iCountIndex, m_aGreens[nm]->m_aLods[lod]->m_pModel->m_pIndexCount[i] / 3);
			Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ((m_aGreens[nm]->m_aLods[lod]->m_pModel->m_pIndexCount[i] / 3) * m_iCurrCountDrawObj));
			iCountIndex += m_aGreens[nm]->m_aLods[lod]->m_pModel->m_pIndexCount[i];
		}

		CGreen::m_pDXDevice->SetStreamSourceFreq(0, 1);
		CGreen::m_pDXDevice->SetStreamSourceFreq(1, 1);
	}
}

void CGreen::render(DWORD timeDelta, const float3* viewpos, GREEN_TYPE type, ID id_arr)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	float3 jcenter;
	float jradius;

	for (int nm = 0; nm < m_aGreens.size(); ++nm)
	{
		//если тип не указан
		if (type != GREEN_TYPE_ALL && type != m_aGreens[nm]->m_typeGreen)
			continue;

		CSegment **ppArrSplits = m_aArrComFor[id_arr]->m_aIRS[nm]->m_ppSegments;
		int iCount = m_aArrComFor[id_arr]->m_aIRS[nm]->m_iCountCom;

		for (int lod = 0; lod < GREEN_COUNT_LOD; ++lod)
		{
			if (m_aGreens[nm]->m_aLods[lod])
			{
				CGreenDataVertex* RTGPUArrVerteces = 0;
				m_pTransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
				m_iCurrCountDrawObj = 0;

				for (DWORD i = 0; i < iCount; i++)
				{
					if (m_iCurrCountDrawObj + ppArrSplits[i]->m_iCountObj >= GREEN_MAX_ELEM_IN_DIP)
					{
						m_pTransVertBuf->Unlock();
						render2(timeDelta, viewpos, nm, lod, -1);
						m_pTransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
						m_iCurrCountDrawObj = 0;
					}

					ppArrSplits[i]->m_pBoundVolumeP->getSphere(&jcenter, &jradius);
					ppArrSplits[i]->m_fDistForCamera = SMVector3Length((jcenter - (*viewpos))) - jradius;

					if (
						//распределение по дистанции есесно и по лодам
						(
						(lod == 0 && ppArrSplits[i]->m_fDistForCamera <= CGreen::m_vDistLods.x) ||
						(lod == 1 && ppArrSplits[i]->m_fDistForCamera <= CGreen::m_vDistLods.y && ppArrSplits[i]->m_fDistForCamera > CGreen::m_vDistLods.x) ||
						(lod == 2 && ppArrSplits[i]->m_fDistForCamera > CGreen::m_vDistLods.y)
						)
						)
					{
						//если это не трава
						if (!(lod == 0 && m_aGreens[nm]->m_typeGreen == GREEN_TYPE_GRASS))
						{
							/*memcpy(RTGPUArrVerteces + (m_iCurrCountDrawObj),
								ppArrSplits[i]->m_pObjData,
								ppArrSplits[i]->m_iCountObj * sizeof(CGreenDataVertex));

							m_iCurrCountDrawObj += ppArrSplits[i]->m_iCountObj;*/

							for (int k = 0; k < ppArrSplits[i]->m_iCountObj; ++k)
							{
								if (m_aArrComFor[id_arr]->m_aVisible[nm][ppArrSplits[i]->m_pArrIDs[k]])
								{
									//RTGPUArrVerteces[m_iCurrCountDrawObj] = ppArrSplits[i]->m_pObjData[k];
									RTGPUArrVerteces[m_iCurrCountDrawObj] = m_aGreens[nm]->m_pAllTrans[ppArrSplits[i]->m_pArrIDs[k]];
									++m_iCurrCountDrawObj;
								}
							}
						}
						//иначе это трава, а ее по особенному рисуем
						else if (lod == 0 && m_aGreens[nm]->m_typeGreen == GREEN_TYPE_GRASS)
						{
							if (CGreen::m_iRenderFreqGrass >= 100)
							{
								/*memcpy(RTGPUArrVerteces + (m_iCurrCountDrawObj),
									ppArrSplits[i]->m_pObjData,
									ppArrSplits[i]->m_iCountObj * sizeof(CGreenDataVertex));*/

								for (int k = 0; k < ppArrSplits[i]->m_iCountObj; ++k)
								{
									RTGPUArrVerteces[m_iCurrCountDrawObj] = m_aGreens[nm]->m_pAllTrans[ppArrSplits[i]->m_pArrIDs[k]];
									++m_iCurrCountDrawObj;
								}

								//m_iCurrCountDrawObj += ppArrSplits[i]->m_iCountObj;
							}
							else
							{
								float percent = float(CGreen::m_iRenderFreqGrass) / 100.f;
								float newCount = ((float)ppArrSplits[i]->m_iCountObj * percent);
								float step = float(ppArrSplits[i]->m_iCountObj) / newCount;
								UINT lastCP = 0;
								for (float k = 0; k < ppArrSplits[i]->m_iCountObj; k += step)
								{
									UINT curCP = (int)floor(k + 0.5);
									if (curCP > lastCP)
									{
										//memcpy(RTGPUArrVerteces + m_iCurrCountDrawObj, ppArrSplits[i]->m_pObjData + curCP, sizeof(CGreenDataVertex));
										RTGPUArrVerteces[m_iCurrCountDrawObj] = m_aGreens[nm]->m_pAllTrans[ppArrSplits[i]->m_pArrIDs[curCP]];
										m_iCurrCountDrawObj += 1;
										lastCP = curCP;
									}
								}
							}
						}
					}
				}

				m_pTransVertBuf->Unlock();

				render2(timeDelta, viewpos, nm, lod, -1);
			}
		}
	}
}

void CGreen::renderSingly(DWORD timeDelta, const  float3* viewpos, ID id, ID id_tex)
{
	if (m_aGreens.size() <= id)
		return;

	float3 jcenter;
	float jradius;
	ID id_arr = 0;

	CSegment **ppArrSplits = m_aArrComFor[id_arr]->m_aIRS[id]->m_ppSegments;
	int iCount = m_aArrComFor[id_arr]->m_aIRS[id]->m_iCountCom;

	for (int lod = 0; lod < GREEN_COUNT_LOD; ++lod)
	{
		if (m_aGreens[id]->m_aLods[lod])
		{
			CGreenDataVertex* RTGPUArrVerteces = 0;
			m_pTransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
			m_iCurrCountDrawObj = 0;

			for (int i = 0; i < iCount; i++)
			{
				if (m_iCurrCountDrawObj + ppArrSplits[i]->m_iCountObj >= GREEN_MAX_ELEM_IN_DIP)
				{
					m_pTransVertBuf->Unlock();
					render2(timeDelta, viewpos, id, lod, id_tex);
					m_pTransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
					m_iCurrCountDrawObj = 0;
				}

				ppArrSplits[i]->m_pBoundVolumeP->getSphere(&jcenter, &jradius);
				ppArrSplits[i]->m_fDistForCamera = SMVector3Length((jcenter - (*viewpos))) - jradius;

				if (
					//распределение по дистанции есесно и по лодам
					(
					(lod == 0 && ppArrSplits[i]->m_fDistForCamera <= CGreen::m_vDistLods.x) ||
					(lod == 1 && ppArrSplits[i]->m_fDistForCamera <= CGreen::m_vDistLods.y && ppArrSplits[i]->m_fDistForCamera > CGreen::m_vDistLods.x) ||
					(lod == 2 && ppArrSplits[i]->m_fDistForCamera > CGreen::m_vDistLods.y) || !(m_aGreens[id]->m_aLods[1])
					)
					)
				{
					if (!(lod == 0 && m_aGreens[id]->m_typeGreen == GREEN_TYPE_GRASS))
					{
						/*memcpy(RTGPUArrVerteces + (m_iCurrCountDrawObj),
							ppArrSplits[i]->m_pObjData,
							ppArrSplits[i]->m_iCountObj * sizeof(CGreenDataVertex));

						m_iCurrCountDrawObj += ppArrSplits[i]->m_iCountObj;*/

						for (int k = 0; k < ppArrSplits[i]->m_iCountObj; ++k)
						{
							if (m_aArrComFor[id_arr]->m_aVisible[id][ppArrSplits[i]->m_pArrIDs[k]])
							{
								//RTGPUArrVerteces[m_iCurrCountDrawObj] = ppArrSplits[i]->m_pObjData[k];
								RTGPUArrVerteces[m_iCurrCountDrawObj] = m_aGreens[id]->m_pAllTrans[ppArrSplits[i]->m_pArrIDs[k]];
								++m_iCurrCountDrawObj;
							}
						}
					}
					else
					{
						float percent = 0.1f;
						float newCount = ((float)ppArrSplits[i]->m_iCountObj * percent);
						float step = float(ppArrSplits[i]->m_iCountObj) / newCount;
						UINT lastCP = 0;
						for (float k = 0; k < ppArrSplits[i]->m_iCountObj; k += step)
						{
							UINT curCP = (int)floor(k + 0.5);;
							if (curCP > lastCP)
							{
								//memcpy(RTGPUArrVerteces + m_iCurrCountDrawObj, ppArrSplits[i]->m_pObjData + curCP, sizeof(CGreenDataVertex));
								RTGPUArrVerteces[m_iCurrCountDrawObj] = m_aGreens[id]->m_pAllTrans[ppArrSplits[i]->m_pArrIDs[curCP]];
								m_iCurrCountDrawObj += 1;
								lastCP = curCP;
							}
						}
					}
				}
			}

			m_pTransVertBuf->Unlock();

			render2(timeDelta, viewpos, id, lod, id_tex);
		}
	}
}

void CGreen::renderObject(DWORD timeDelta, const float3* viewpos, ID id, ID split, ID idobj, ID id_tex)
{
	if (id < 0 || m_aGreens.size() <= id || split < 0 || m_aGreens[id]->m_aSplitsArr.size() <= split || idobj < 0 || m_aGreens[id]->m_aSplitsArr[split]->m_iCountObj <= idobj)
		return;

	CGreenDataVertex* RTGPUArrVerteces = 0;
	m_pTransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
	//memcpy(RTGPUArrVerteces, &(m_aGreens[id]->m_aSplitsArr[split]->m_pObjData[idobj]), sizeof(CGreenDataVertex));
	RTGPUArrVerteces[0] = m_aGreens[id]->m_pAllTrans[m_aGreens[id]->m_aSplitsArr[split]->m_pArrIDs[idobj]];
	m_pTransVertBuf->Unlock();
	m_iCurrCountDrawObj = 1;

	render2(timeDelta, viewpos, id, 0, id_tex);
}

ID CGreen::init(const char* name,
	const char* path_mask, 
	float count_max, 
	const char* path, const char* lod1, const char* lod2, 
	const char* navmesh)
{
	if (SGeom_ModelsGetCount() > 0 && STR_VALIDATE(path))
	{
		CModel* tmpnewmpdel = new CModel();
		sprintf(tmpnewmpdel->m_szName, name);
		tmpnewmpdel->m_szMaskName = path_mask;
		tmpnewmpdel->m_aLods[0] = new CLod();
		tmpnewmpdel->m_aLods[1] = 0;
		tmpnewmpdel->m_aLods[2] = 0;

		if (!lod1 && !lod2)
			tmpnewmpdel->m_typeGreen = GREEN_TYPE_GRASS;
		else
			tmpnewmpdel->m_typeGreen = GREEN_TYPE_TREE;

		char tmppath[1024];
		sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path);

		SGCore_StaticModelLoad(tmppath, &tmpnewmpdel->m_aLods[0]->m_pModel);
		tmpnewmpdel->m_aLods[0]->m_sPath = path;
		char tmppathtex[1024];
		for (int i = 0; i < tmpnewmpdel->m_aLods[0]->m_pModel->m_uiSubsetCount; ++i)
		{
			sprintf(tmppathtex, "%s.dds", tmpnewmpdel->m_aLods[0]->m_pModel->m_ppTextures[i]);
			tmpnewmpdel->m_aLods[0]->m_aIDsTextures[i] = SGCore_MtlLoad(tmppathtex, (tmpnewmpdel->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
		}

		if (STR_VALIDATE(lod1))
		{
			if (stricmp(path, lod1) == 0)
				tmpnewmpdel->m_aLods[1] = tmpnewmpdel->m_aLods[0];
			else
			{
				tmpnewmpdel->m_aLods[1] = new CLod();
				sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), lod1);
				tmpnewmpdel->m_aLods[1]->m_sPath = lod1;
				SGCore_StaticModelLoad(tmppath, &tmpnewmpdel->m_aLods[1]->m_pModel);

				for (int i = 0; i < tmpnewmpdel->m_aLods[1]->m_pModel->m_uiSubsetCount; ++i)
				{
					sprintf(tmppathtex, "%s.dds", tmpnewmpdel->m_aLods[1]->m_pModel->m_ppTextures[i]);
					tmpnewmpdel->m_aLods[1]->m_aIDsTextures[i] = SGCore_MtlLoad(tmppathtex, (tmpnewmpdel->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
				}
			}
		}

		if (STR_VALIDATE(lod2))
		{
			if (stricmp(path, lod2) == 0)
				tmpnewmpdel->m_aLods[2] = tmpnewmpdel->m_aLods[0];
			else if (stricmp(lod1, lod2) == 0)
				tmpnewmpdel->m_aLods[2] = tmpnewmpdel->m_aLods[1];
			else
			{
				tmpnewmpdel->m_aLods[2] = new CLod();
				sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), lod2);
				tmpnewmpdel->m_aLods[2]->m_sPath = lod2;
				SGCore_StaticModelLoad(tmppath, &tmpnewmpdel->m_aLods[2]->m_pModel);

				for (int i = 0; i < tmpnewmpdel->m_aLods[2]->m_pModel->m_uiSubsetCount; ++i)
				{
					sprintf(tmppathtex, "%s.dds", tmpnewmpdel->m_aLods[1]->m_pModel->m_ppTextures[i]);
					tmpnewmpdel->m_aLods[2]->m_aIDsTextures[i] = SGCore_MtlLoad(tmppathtex, (tmpnewmpdel->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
				}
			}
		}

		ISXBound* tmpbb = SGCore_CrBound();
		SGCore_FCompBoundBox(tmpnewmpdel->m_aLods[0]->m_pModel->m_pVertexBuffer, &tmpbb, tmpnewmpdel->m_aLods[0]->m_pModel->m_uiAllVertexCount, sizeof(vertex_static));

		float3 mmax, mmin;
		tmpbb->getMinMax(&mmin, &mmax);
		mem_release(tmpbb);

		float3 tmpmin, tmpmax;
		SGeom_ModelsGetMinMax(&tmpmin, &tmpmax);

		if (STR_VALIDATE(path_mask))
		{
			ID IDTexMask = SGCore_LoadTexAddName(path_mask, LOAD_TEXTURE_TYPE_LOAD);
			SGCore_LoadTexAllLoad();

			genByTex(tmpnewmpdel, IDTexMask, &mmin, &mmax, count_max);
		}

		preSegmentation(tmpnewmpdel, &tmpmin, &tmpmax);
		//mem_delete_a(tmpnewmpdel->m_pAllTrans);

		setSplitID(tmpnewmpdel);
		
		m_aGreens.push_back(tmpnewmpdel);

		ComputeBBtrans(m_aGreens.size() - 1);

		if (STR_VALIDATE(navmesh))
		{
			setGreenNav(m_aGreens.size() - 1, navmesh);
		}
		addModelInArrCom(m_aGreens.size() - 1);
		return m_aGreens.size() - 1;
	}
	else
	{
		LibReport(REPORT_MSG_LEVEL_WARNING,"not found static geometry in level!!!");
	}

	return -1;
}

void CGreen::genByTex(CModel* model, ID idmask, float3* min, float3* max, float count_max)
{
	float CountMaxInPixel = count_max;

	float3 tmpmin, tmpmax;
	SGeom_ModelsGetMinMax(&tmpmin, &tmpmax);

	float r2d = 0;

	if ((max->x - min->x)*0.5f > (max->z - min->z)*0.5f)
		r2d = (max->x - min->x)*0.5f;
	else
		r2d = (max->z - min->z)*0.5f;

	float WidthLand = tmpmax.x - tmpmin.x;
	float HeightLand = tmpmax.z - tmpmin.z;

	D3DSURFACE_DESC desc;
	SGCore_LoadTexGetTex(idmask)->GetLevelDesc(0, &desc);

	D3DLOCKED_RECT LockedRect;

	SGCore_LoadTexGetTex(idmask)->LockRect(0, &LockedRect, 0, 0);
	DWORD* tmpColor = (DWORD*)LockedRect.pBits;

	DWORD tmpUnAllCountGreen = 0;
	Array<float3_t, GREEN_DEFAULT_RESERVE_GEN> arrpos;

	DWORD MaxAlpha = 0;
	DWORD alpha;
	float AlphaColor;
	float PosInLandX;
	float PosInLandY;

	float3_t tmp2;

	float OneEdX;
	float OneEdY;

	float3 tmppos2;
	bool isintersect;

	float3 vGeomMin, vGeomMax;
	SGeom_ModelsGetMinMax(&vGeomMin, &vGeomMax);
	float3 vPosMin = vGeomMin;

	for (DWORD x = 0; x<desc.Width; ++x)
	{
		for (DWORD y = 0; y<desc.Height; ++y)
		{
			alpha = (tmpColor[y*desc.Width + x] >> 24);
			AlphaColor = 1.f / 255.f * (float)alpha;
			if (alpha > 0)
			{
				//позици¤ пиксел¤ на ландшафте
				PosInLandX = lerpf(tmpmin.x, tmpmax.x, float(x + 1) / float(desc.Width));
				PosInLandY = lerpf(tmpmax.z, tmpmin.z, float(y + 1) / float(desc.Height));

				tmp2 = float3_t(PosInLandX, 0, PosInLandY);

				OneEdX = WidthLand / float(desc.Width);
				OneEdY = HeightLand / float(desc.Height);

				//int tmpcountgreen = lerp(0,10,(float(alpha)*CountMaxInPixel)/255.f);//int(floor(float(float(alpha)*CountMaxInPixel*100)/2550.f));
				//расчет позиций объектов на квадратный метр
				for (int i = 0; i<int(floor(float(float(alpha)*CountMaxInPixel * 100) / 2550.f)); ++i)
				{
					tmppos2 = float3(tmp2.x, 100, tmp2.z);
					isintersect = true;

					tmppos2.x = (tmp2.x - OneEdX*0.5f) + randf(0.0, OneEdX);
					tmppos2.z = (tmp2.z - OneEdY*0.5f) + randf(0.0, OneEdY);

					if (model->m_typeGreen == GREEN_TYPE_TREE)
					{
						for (int k = 0; k < arrpos.size(); ++k)
						{
							if (SMVector3Length2(tmppos2 - float3(arrpos[k].x, 100, arrpos[k].z)) < r2d*r2d)
							{
								alpha = 0;
								isintersect = false;
								break;
							}
						}
					}

					if (isintersect)
					{
						vPosMin.x = tmppos2.x;
						vPosMin.z = tmppos2.z;
						isintersect = g_fnIntersect(&tmppos2, &vPosMin, &tmppos2);
					}

					if (isintersect)
					{
						arrpos.push_back(tmppos2);

						++model->m_uiCountObj;

						//если тип дерево, то на пиксель генерируем только одно дерево
						if (model->m_typeGreen == GREEN_TYPE_TREE)
							break;
					}
					else
					{
						++tmpUnAllCountGreen;
					}
				}
			}
		}
	}

	SGCore_LoadTexGetTex(idmask)->UnlockRect(0);

	if (model->m_uiCountObj <= 0)
		return;

	//model->m_pAllTrans = new CGreenDataVertex[model->m_uiCountObj];
	model->m_pAllTrans.reserve(model->m_uiCountObj);

	for (DWORD i = 0; i<model->m_uiCountObj; i++)
	{
		model->m_pAllTrans[i].m_vPosition = arrpos[i];
		model->m_pAllTrans[i].m_vTexCoord.x = 1.f + randf(0.f, GREEN_GEN_RAND_SCALE);
		model->m_pAllTrans[i].m_vTexCoord.y = randf(0.f, GREEN_GEN_RAND_ROTATE_Y);
		model->m_pAllTrans[i].m_vTexCoord.z = 0;// (float(rand() % 200) / 100.f) - 1.f;
		model->m_pAllTrans[i].m_vSinCosRot.x = sinf(model->m_pAllTrans[i].m_vTexCoord.y);
		model->m_pAllTrans[i].m_vSinCosRot.y = cosf(model->m_pAllTrans[i].m_vTexCoord.y);
	}
	arrpos.clear();
}

ID CGreen::getIDSplit(ID id, float3* pos)
{
	if (id < 0 || m_aGreens.size() <= id)
		return -1;

	Array<CSegment*> queue;
	int tmpcount = 0;
	queue.push_back(m_aGreens[id]->m_pSplitsTree);
	float width = 0;
	float depth = 0;
	float3 min, max;
	ID tmpidsplit = -1;

	while (queue.size())
	{
		queue[0]->m_pBoundVolumeSys->getMinMax(&min, &max);
		if (max.x >= pos->x && max.z >= pos->z && min.x <= pos->x && min.z <= pos->z)
		{
			if (!(queue[0]->m_aSplits[0]))
				tmpidsplit = queue[0]->m_id;
			else
			{
				queue[0]->m_idNonEnd = true;
				++(queue[0]->m_iCountObj);

				float3 tmpMin, tmpMax;
				float scalecoef = 1.f + GREEN_GEN_RAND_SCALE;

				queue[0]->m_pBoundVolumeSys->getMinMax(&tmpMin, &tmpMax);
				if (tmpMax.y < (pos->y + m_aGreens[id]->m_vMax.y * scalecoef))
					tmpMax.y = tmpMax.y + m_aGreens[id]->m_vMax.y * scalecoef;

				if (tmpMin.y >(pos->y + m_aGreens[id]->m_vMin.y * scalecoef))
					tmpMin.y = tmpMin.y + m_aGreens[id]->m_vMin.y * scalecoef;

				queue[0]->m_pBoundVolumeSys->setMinMax(&tmpMin, &tmpMax);

				if (tmpMax.x < (pos->x + m_aGreens[id]->m_vMax.x * scalecoef))
					tmpMax.x = tmpMax.x + m_aGreens[id]->m_vMax.x * scalecoef;

				if (tmpMax.z < (pos->z + m_aGreens[id]->m_vMax.z * scalecoef))
					tmpMax.z = tmpMax.z + m_aGreens[id]->m_vMax.z * scalecoef;

				if (tmpMin.x >(pos->x + m_aGreens[id]->m_vMin.x * scalecoef))
					tmpMin.x = tmpMin.x + m_aGreens[id]->m_vMin.x * scalecoef;

				if (tmpMin.z >(pos->z + m_aGreens[id]->m_vMin.z * scalecoef))
					tmpMin.z = tmpMin.z + m_aGreens[id]->m_vMin.z * scalecoef;

				queue[0]->m_pBoundVolumeP->setMinMax(&tmpMin, &tmpMax);
			}

			for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; i++)
			{
				if (queue[0]->m_aSplits[i])
					queue.push_back(queue[0]->m_aSplits[i]);
			}
		}

		queue.erase(0);
		++tmpcount;
	}

	return tmpidsplit;
}

ID CGreen::addObject(ID id, float3* pos, CGreenDataVertex* data, ID* idsplit)
{
	if(id < 0 || (ID)m_aGreens.size() <= id)
		return -1;

	ID tmpidsplit = getIDSplit(id, pos);

	if (tmpidsplit < 0)
		return -1;

	int oldlen = m_aGreens[id]->m_aSplitsArr[tmpidsplit]->m_iCountObj;
	/*CGreenDataVertex* tmpdv = new CGreenDataVertex[oldlen + 1];
	if (oldlen > 0)
		memcpy(tmpdv, m_aGreens[id]->m_aSplitsArr[tmpidsplit]->m_pObjData, oldlen * sizeof(CGreenDataVertex));*/
	
	ID* tmpdv = new ID[oldlen + 1];
	if (oldlen > 0)
		memcpy(tmpdv, m_aGreens[id]->m_aSplitsArr[tmpidsplit]->m_pArrIDs, oldlen * sizeof(ID));


	CGreenDataVertex tmpdvobj;
	if (!data)
	{
		tmpdvobj.m_vPosition = *pos;
		tmpdvobj.m_vTexCoord.x = 1.f + randf(0.f, GREEN_GEN_RAND_SCALE);
		tmpdvobj.m_vTexCoord.y = randf(0.f, GREEN_GEN_RAND_ROTATE_Y);
		tmpdvobj.m_vTexCoord.z = 0;// (float(rand() % 200) / 100.f) - 1.f;
		tmpdvobj.m_vSinCosRot.x = sinf(tmpdvobj.m_vTexCoord.y);
		tmpdvobj.m_vSinCosRot.y = cosf(tmpdvobj.m_vTexCoord.y);
	}
	else
	{
		tmpdvobj = *data;
		tmpdvobj.m_vPosition = *pos;
	}

	/*memcpy(tmpdv + oldlen, &tmpdvobj, sizeof(CGreenDataVertex));
	mem_delete_a(m_aGreens[id]->m_aSplitsArr[tmpidsplit]->m_pObjData);
	m_aGreens[id]->m_aSplitsArr[tmpidsplit]->m_pObjData = tmpdv;*/

	ID idNew = -1;

	if (m_aGreens[id]->m_aDeleteObj.size() > 0)
	{
		idNew = m_aGreens[id]->m_aDeleteObj[m_aGreens[id]->m_aDeleteObj.size() - 1];
		m_aGreens[id]->m_aDeleteObj.erase(m_aGreens[id]->m_aDeleteObj.size() - 1);
	}
	else
	{
		idNew = m_aGreens[id]->m_pAllTrans.size();
	}

	m_aGreens[id]->m_pAllTrans[idNew] = tmpdvobj;

	CBoundBox oBB;
	oBB.m_vMin = m_aGreens[id]->m_vMin;
	oBB.m_vMax = m_aGreens[id]->m_vMax;

	ComputeBB(tmpdvobj, oBB);
	
	m_aGreens[id]->m_aTransW[idNew] = oBB;

	tmpdv[oldlen] = idNew;
	mem_delete_a(m_aGreens[id]->m_aSplitsArr[tmpidsplit]->m_pArrIDs);
	m_aGreens[id]->m_aSplitsArr[tmpidsplit]->m_pArrIDs = tmpdv;
	
	++(m_aGreens[id]->m_aSplitsArr[tmpidsplit]->m_iCountObj);
	++(m_aGreens[id]->m_uiCountObj);
	int currlen = m_aGreens[id]->m_aSplitsArr[tmpidsplit]->m_iCountObj;
	alignBound(m_aGreens[id], m_aGreens[id]->m_aSplitsArr[tmpidsplit]);

	if (idsplit)
		*idsplit = tmpidsplit;

	return oldlen;
}

void CGreen::deleteObject(ID id, ID idsplit, ID idobj)
{
	if (id < 0 || m_aGreens.size() <= id || idsplit < 0 || m_aGreens[id]->m_aSplitsArr.size() <= idsplit || idobj < 0 || m_aGreens[id]->m_aSplitsArr[idsplit]->m_iCountObj <= idobj)
		return;

	int oldlen = m_aGreens[id]->m_aSplitsArr[idsplit]->m_iCountObj;

	/*CGreenDataVertex* tmpdv = new CGreenDataVertex[oldlen - 1];
	memcpy(tmpdv, m_aGreens[id]->m_aSplitsArr[idsplit]->m_pObjData, idobj * sizeof(CGreenDataVertex));
	memcpy(tmpdv + idobj, m_aGreens[id]->m_aSplitsArr[idsplit]->m_pObjData + idobj + 1, ((oldlen - idobj) - 1)* sizeof(CGreenDataVertex));
	mem_delete_a(m_aGreens[id]->m_aSplitsArr[idsplit]->m_pObjData);
	m_aGreens[id]->m_aSplitsArr[idsplit]->m_pObjData = tmpdv;*/

	m_aGreens[id]->m_aDeleteObj.push_back(m_aGreens[id]->m_aSplitsArr[idsplit]->m_pArrIDs[idobj]);

	ID* tmpdv = new ID[oldlen - 1];
	memcpy(tmpdv, m_aGreens[id]->m_aSplitsArr[idsplit]->m_pArrIDs, idobj * sizeof(ID));
	memcpy(tmpdv + idobj, m_aGreens[id]->m_aSplitsArr[idsplit]->m_pArrIDs + idobj + 1, ((oldlen - idobj) - 1)* sizeof(ID));
	mem_delete_a(m_aGreens[id]->m_aSplitsArr[idsplit]->m_pArrIDs);
	m_aGreens[id]->m_aSplitsArr[idsplit]->m_pArrIDs = tmpdv;

	--(m_aGreens[id]->m_aSplitsArr[idsplit]->m_iCountObj);
	--(m_aGreens[id]->m_uiCountObj);
	alignBound(m_aGreens[id], m_aGreens[id]->m_aSplitsArr[idsplit]);
}

void CGreen::getPosObject(ID id, ID idsplit, ID idobj, float3_t* pos)
{
	if (!pos || id < 0 || m_aGreens.size() <= id || idsplit < 0 || m_aGreens[id]->m_aSplitsArr.size() <= idsplit || idobj < 0 || m_aGreens[id]->m_aSplitsArr[idsplit]->m_iCountObj <= idobj)
		return;

	//*pos = m_aGreens[id]->m_aSplitsArr[idsplit]->m_pObjData[idobj].m_vPosition;
	*pos = m_aGreens[id]->m_pAllTrans[m_aGreens[id]->m_aSplitsArr[idsplit]->m_pArrIDs[idobj]].m_vPosition;
}

void CGreen::setPosObject(ID id, ID* idsplit, ID* idobj, const float3_t* pos)
{
	if (!pos || !idsplit || !idobj || id < 0 || m_aGreens.size() <= id || (*idsplit) < 0 || m_aGreens[id]->m_aSplitsArr.size() <= (*idsplit) || (*idobj) < 0 || m_aGreens[id]->m_aSplitsArr[(*idsplit)]->m_iCountObj <= (*idobj))
		return;

	if (getIDSplit(id, &float3(*pos)) == (*idsplit))
		//m_aGreens[id]->m_aSplitsArr[(*idsplit)]->m_pObjData[(*idobj)].m_vPosition = *pos;
		m_aGreens[id]->m_pAllTrans[m_aGreens[id]->m_aSplitsArr[(*idsplit)]->m_pArrIDs[(*idobj)]].m_vPosition = *pos;
	else
	{
		//CGreenDataVertex tmpdv = m_aGreens[id]->m_aSplitsArr[(*idsplit)]->m_pObjData[(*idobj)];
		CGreenDataVertex tmpdv = m_aGreens[id]->m_pAllTrans[m_aGreens[id]->m_aSplitsArr[(*idsplit)]->m_pArrIDs[(*idobj)]];
		deleteObject(id, (*idsplit), (*idobj));
		(*idobj) = addObject(id, &float3(*pos), &tmpdv, idsplit);
	}
}

bool CompareFunc(ID id1, ID id2)
{
	return (id1 < id2);
}

void CGreen::save(const char* path)
{
	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		if (m_aGreens[i]->m_aDeleteObj.size() == 0)
			continue;

		m_aGreens[i]->m_aDeleteObj.quickSort(CompareFunc);

		for (int k = 0; k < m_aGreens[i]->m_aDeleteObj.size(); ++k)
		{
			int iKey = (m_aGreens[i]->m_aDeleteObj.size() - k) - 1;
			m_aGreens[i]->m_pAllTrans.erase(m_aGreens[i]->m_aDeleteObj[iKey]);
			m_aGreens[i]->m_aTransW.erase(m_aGreens[i]->m_aDeleteObj[iKey]);
		}

		Array<CSegment*> queue;
		int tmpcount = 0;
		queue.push_back(m_aGreens[i]->m_pSplitsTree);

		while (queue.size())
		{
			if (queue[0]->m_idNonEnd)
			{
				for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
				{
					if (queue[0]->m_aSplits[i])
						queue.push_back(queue[0]->m_aSplits[i]);
				}
			}
			else
			{
				if (queue[0]->m_iCountObj > 0)
				{
					int iNewSize = queue[0]->m_iCountObj;
					for (int k = 0; k < queue[0]->m_iCountObj; ++k)
					{
						for (int j = 0; j < m_aGreens[i]->m_aDeleteObj.size(); ++j)
						{
							if (queue[0]->m_pArrIDs[k] == m_aGreens[i]->m_aDeleteObj[j])
								--iNewSize;
						}
					}

					if (iNewSize > 0)
					{
						ID *pArrIDs = new ID[iNewSize];

						for (int k = 0; k < iNewSize; ++k)
						{
							pArrIDs[k] = -1;
						}
						
						int iCurrKey = -1;
						for (int k = 0; k < queue[0]->m_iCountObj; ++k)
						{
							for (int j = 0; j < m_aGreens[i]->m_aDeleteObj.size(); ++j)
							{
								if (queue[0]->m_pArrIDs[k] != m_aGreens[i]->m_aDeleteObj[j])
								{
									if (pArrIDs[k] == -1)
									{
										++iCurrKey;
										pArrIDs[iCurrKey] = queue[0]->m_pArrIDs[k];
									}

									if (queue[0]->m_pArrIDs[k] > m_aGreens[i]->m_aDeleteObj[j])
									{
										--pArrIDs[iCurrKey];

										if (pArrIDs[iCurrKey] < 0)
											int qwerty = 0;
									}
								}
							}
						}

						for (int k = 0; k < iNewSize; ++k)
						{
							if (pArrIDs[k] < 0)
								int qwerty = 0;
						}

						queue[0]->m_iCountObj = iNewSize;
						mem_delete_a(queue[0]->m_pArrIDs);
						queue[0]->m_pArrIDs = pArrIDs;
					}
					else
					{
						queue[0]->m_iCountObj = iNewSize;
						mem_delete_a(queue[0]->m_pArrIDs);
					}
				}
			}

			queue.erase(0);
			++tmpcount;
		}

		m_aGreens[i]->m_aDeleteObj.clear();
	}


	FILE* file = fopen(path, "wb");

	int32_t countmodel = m_aGreens.size();
	fwrite(&countmodel, sizeof(int32_t), 1, file);

	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		fwrite(&m_aGreens[i]->m_typeGreen, sizeof(int32_t), 1, file);

		int32_t tmpstrlen = strlen(m_aGreens[i]->m_szName);
		fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
		fwrite(m_aGreens[i]->m_szName, sizeof(char), tmpstrlen, file);

		tmpstrlen = m_aGreens[i]->m_szMaskName.length();
		fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
		fwrite(m_aGreens[i]->m_szMaskName.c_str(), sizeof(char), tmpstrlen, file);

		if (m_aGreens[i]->m_pPhysMesh)
		{
			tmpstrlen = strlen(m_aGreens[i]->m_pPhysMesh->m_sPathName.c_str());
			fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
			fwrite(m_aGreens[i]->m_pPhysMesh->m_sPathName.c_str(), sizeof(char), tmpstrlen, file);
		}
		else
		{
			tmpstrlen = 0;
			fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
		}

		if (m_aGreens[i]->m_typeGreen == GREEN_TYPE_GRASS)
		{
			tmpstrlen = strlen(m_aGreens[i]->m_aLods[0]->m_sPath.c_str());
			fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
			fwrite(m_aGreens[i]->m_aLods[0]->m_sPath.c_str(), sizeof(char), tmpstrlen, file);
		}
		else
		{
			for (int k = 0; k < GREEN_COUNT_LOD; ++k)
			{
				tmpstrlen = m_aGreens[i]->m_aLods[k]->m_sPath.length();
				fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
				fwrite(m_aGreens[i]->m_aLods[k]->m_sPath.c_str(), sizeof(char), tmpstrlen, file);
			}
		}
		
		fwrite(&m_aGreens[i]->m_vMin.x, sizeof(float), 1, file);
		fwrite(&m_aGreens[i]->m_vMin.y, sizeof(float), 1, file);
		fwrite(&m_aGreens[i]->m_vMin.z, sizeof(float), 1, file);

		fwrite(&m_aGreens[i]->m_vMax.x, sizeof(float), 1, file);
		fwrite(&m_aGreens[i]->m_vMax.y, sizeof(float), 1, file);
		fwrite(&m_aGreens[i]->m_vMax.z, sizeof(float), 1, file);

		fwrite(&m_aGreens[i]->m_uiCountObj, sizeof(uint32_t), 1, file);
		fwrite(&(m_aGreens[i]->m_pAllTrans[0]), sizeof(CGreenDataVertex), m_aGreens[i]->m_uiCountObj, file);

		Array<CSegment*> queue;
		int tmpcount = 0;
		queue.push_back(m_aGreens[i]->m_pSplitsTree);

		while (queue.size())
		{
			saveSplit(queue[0], file, &queue);

			queue.erase(0);
			++tmpcount;
		}
	}

	fclose(file);
}

void CGreen::saveSplit(CSegment* Split, FILE* file, Array<CSegment*> * queue)
{
	float3 jmin, jmax;
	Split->m_pBoundVolumeSys->getMinMax(&jmin, &jmax);
	fwrite(&jmin.x, sizeof(float), 1, file);
	fwrite(&jmin.y, sizeof(float), 1, file);
	fwrite(&jmin.z, sizeof(float), 1, file);

	fwrite(&jmax.x, sizeof(float), 1, file);
	fwrite(&jmax.y, sizeof(float), 1, file);
	fwrite(&jmax.z, sizeof(float), 1, file);

	Split->m_pBoundVolumeP->getMinMax(&jmin, &jmax);
	fwrite(&jmin.x, sizeof(float), 1, file);
	fwrite(&jmin.y, sizeof(float), 1, file);
	fwrite(&jmin.z, sizeof(float), 1, file);

	fwrite(&jmax.x, sizeof(float), 1, file);
	fwrite(&jmax.y, sizeof(float), 1, file);
	fwrite(&jmax.z, sizeof(float), 1, file);

	fwrite(&Split->m_iCountObj, sizeof(uint32_t), 1, file);

	if (Split->m_iCountObj > 0)
		int qwerty = 0;

	fwrite(&Split->m_idNonEnd, sizeof(int8_t), 1, file);

	bool isexists = true;

	if (Split->m_aSplits[0]/*!Split->m_pObjData && Split->m_iCountObj > 0*/)
	{
		fwrite(&isexists, sizeof(int8_t), 1, file);
		for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		{
			if (Split->m_aSplits[i])
				queue->push_back(Split->m_aSplits[i]);
		}
	}
	else
	{
		isexists = false;
		fwrite(&isexists, sizeof(int8_t), 1, file);
		if (Split->m_iCountObj > 0)
			//fwrite(Split->m_pObjData, sizeof(CGreenDataVertex), Split->m_iCountObj, file);
			fwrite(Split->m_pArrIDs, sizeof(ID), Split->m_iCountObj, file);
	}
}

void CGreen::load(const char* path)
{
	FILE* file = fopen(path, "rb");

	char tmpstr[3][1024];
	tmpstr[0][0] = 0;
	tmpstr[1][0] = 0;
	tmpstr[2][0] = 0;
	char tmppath[1024];
	char tmpNameMask[1024];

	int32_t countmodel;
	fread(&countmodel, sizeof(int32_t), 1, file);

	for (int i = 0; i < countmodel; ++i)
	{
		tmpstr[0][0] = tmpstr[1][0] = tmpstr[2][0] = 0;
		CModel* tmpmodel = new CModel();
		fread(&tmpmodel->m_typeGreen, sizeof(int32_t), 1, file);

		int32_t tmpstrlen;// = strlen(m_aGreens[i]->Name);
		fread(&tmpstrlen, sizeof(int32_t), 1, file);
		fread(tmpNameMask, sizeof(char), tmpstrlen, file);
		tmpNameMask[tmpstrlen] = 0;
		sprintf(tmpmodel->m_szName, "%s", tmpNameMask);

		//tmpstrlen = m_aGreens[i]->m_szMaskName.length();
		fread(&tmpstrlen, sizeof(int32_t), 1, file);
		fread(tmpNameMask, sizeof(char), tmpstrlen, file);
		tmpNameMask[tmpstrlen] = 0;
		tmpmodel->m_szMaskName = tmpNameMask;

		fread(&tmpstrlen, sizeof(int32_t), 1, file);
		if (tmpstrlen > 0)
		{
			fread(tmpNameMask, sizeof(char), tmpstrlen, file);
			tmpNameMask[tmpstrlen] = 0;
			tmpmodel->m_pPhysMesh = new CModel::CPhysMesh();
			tmpmodel->m_pPhysMesh->m_sPathName = tmpNameMask;
		}

		if (tmpmodel->m_typeGreen == GREEN_TYPE_GRASS)
		{
			fread(&tmpstrlen, sizeof(int32_t), 1, file);
			fread(tmpstr[0], sizeof(char), tmpstrlen, file);
			tmpstr[0][tmpstrlen] = 0;
		}
		else
		{
			for (int k = 0; k < GREEN_COUNT_LOD; ++k)
			{
				fread(&tmpstrlen, sizeof(int32_t), 1, file);
				fread(tmpstr[k], sizeof(char), tmpstrlen, file);
				tmpstr[k][tmpstrlen] = 0;
			}
		}

		
		tmpmodel->m_aLods[0] = new CLod();
		tmpmodel->m_aLods[1] = 0;
		tmpmodel->m_aLods[2] = 0;

		sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), tmpstr[0]);
		SGCore_StaticModelLoad(tmppath, &tmpmodel->m_aLods[0]->m_pModel);
		tmpmodel->m_aLods[0]->m_sPath = tmpstr[0];
		char tmppathtex[1024];
		for (int k = 0; k < tmpmodel->m_aLods[0]->m_pModel->m_uiSubsetCount; ++k)
		{
			sprintf(tmppathtex, "%s.dds", tmpmodel->m_aLods[0]->m_pModel->m_ppTextures[k]);
			tmpmodel->m_aLods[0]->m_aIDsTextures[k] = SGCore_MtlLoad(tmppathtex, (tmpmodel->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
		}

		if (tmpstr[1][0])
		{
			if (stricmp(tmpstr[0], tmpstr[1]) == 0)
				tmpmodel->m_aLods[1] = tmpmodel->m_aLods[0];
			else
			{
				tmpmodel->m_aLods[1] = new CLod();
				tmpmodel->m_aLods[1]->m_sPath = tmpstr[1];
				sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), tmpstr[1]);
				SGCore_StaticModelLoad(tmppath, &tmpmodel->m_aLods[1]->m_pModel);

				for (int k = 0; k < tmpmodel->m_aLods[1]->m_pModel->m_uiSubsetCount; ++k)
				{
					sprintf(tmppathtex, "%s.dds", tmpmodel->m_aLods[1]->m_pModel->m_ppTextures[k]);
					tmpmodel->m_aLods[1]->m_aIDsTextures[k] = SGCore_MtlLoad(tmppathtex, (tmpmodel->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
				}
			}
		}

		if (tmpstr[2][0])
		{
			if (stricmp(tmpstr[0], tmpstr[2]) == 0)
				tmpmodel->m_aLods[2] = tmpmodel->m_aLods[0];
			else if (stricmp(tmpstr[1], tmpstr[2]) == 0)
				tmpmodel->m_aLods[2] = tmpmodel->m_aLods[1];
			else
			{
				tmpmodel->m_aLods[2] = new CLod();
				tmpmodel->m_aLods[2]->m_sPath = tmpstr[2];
				sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), tmpstr[2]);
				SGCore_StaticModelLoad(tmppath, &tmpmodel->m_aLods[2]->m_pModel);

				for (int k = 0; k < tmpmodel->m_aLods[2]->m_pModel->m_uiSubsetCount; ++k)
				{
					sprintf(tmppathtex, "%s.dds", tmpmodel->m_aLods[1]->m_pModel->m_ppTextures[k]);
					tmpmodel->m_aLods[2]->m_aIDsTextures[k] = SGCore_MtlLoad(tmppathtex, (tmpmodel->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
				}
			}
		}

		fread(&tmpmodel->m_vMin.x, sizeof(float), 1, file);
		fread(&tmpmodel->m_vMin.y, sizeof(float), 1, file);
		fread(&tmpmodel->m_vMin.z, sizeof(float), 1, file);

		fread(&tmpmodel->m_vMax.x, sizeof(float), 1, file);
		fread(&tmpmodel->m_vMax.y, sizeof(float), 1, file);
		fread(&tmpmodel->m_vMax.z, sizeof(float), 1, file);

		fread(&tmpmodel->m_uiCountObj, sizeof(uint32_t), 1, file);
		tmpmodel->m_pAllTrans.resize(tmpmodel->m_uiCountObj);
		fread(&(tmpmodel->m_pAllTrans[0]), sizeof(CGreenDataVertex), tmpmodel->m_uiCountObj, file);

		Array<CSegment**> queue;
		int tmpcount = 0;
		queue.push_back(&(tmpmodel->m_pSplitsTree));

		while (queue.size())
		{
			loadSplit(queue[0], file, &queue);

			/*if (/*(*queue[0])->m_pObjData*//*(*queue[0])->m_pArrIDs)
				tmpmodel->m_uiCountObj += (*queue[0])->m_iCountObj;*/
			queue.erase(0);
			++tmpcount;
		}

		setSplitID(tmpmodel);

		m_aGreens.push_back(tmpmodel);

		ComputeBBtrans(m_aGreens.size() - 1);
		
		addModelInArrCom(m_aGreens.size() - 1);

		if (tmpmodel->m_pPhysMesh)
			setGreenNav(m_aGreens.size() - 1, tmpmodel->m_pPhysMesh->m_sPathName.c_str());
	}

	fclose(file);

	//save(path);
}

void CGreen::loadSplit(CSegment** Split, FILE* file, Array<CSegment**> * queue)
{
	(*Split) = new CSegment();
	float3 jmin, jmax;
	fread(&jmin.x, sizeof(float), 1, file);
	fread(&jmin.y, sizeof(float), 1, file);
	fread(&jmin.z, sizeof(float), 1, file);

	fread(&jmax.x, sizeof(float), 1, file);
	fread(&jmax.y, sizeof(float), 1, file);
	fread(&jmax.z, sizeof(float), 1, file);

	(*Split)->m_pBoundVolumeSys = SGCore_CrBound();
	(*Split)->m_pBoundVolumeSys->setMinMax(&jmin, &jmax);


	fread(&jmin.x, sizeof(float), 1, file);
	fread(&jmin.y, sizeof(float), 1, file);
	fread(&jmin.z, sizeof(float), 1, file);

	fread(&jmax.x, sizeof(float), 1, file);
	fread(&jmax.y, sizeof(float), 1, file);
	fread(&jmax.z, sizeof(float), 1, file);

	(*Split)->m_pBoundVolumeP = SGCore_CrBound();
	(*Split)->m_pBoundVolumeP->setMinMax(&jmin, &jmax);

	fread(&(*Split)->m_iCountObj, sizeof(uint32_t), 1, file);

	fread(&(*Split)->m_idNonEnd, sizeof(int8_t), 1, file);

	bool isexists = false;
	fread(&isexists, sizeof(int8_t), 1, file);

	if (isexists)
	{
		for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		{
			queue->push_back(&((*Split)->m_aSplits[i]));
		}
	}
	else
	{
		if ((*Split)->m_iCountObj > 0)
		{
			//(*Split)->m_pObjData = new CGreenDataVertex[(*Split)->m_iCountObj];
			(*Split)->m_pArrIDs = new ID[(*Split)->m_iCountObj];

			/*for (int i = 0; i < (*Split)->m_iCountObj; ++i)
				(*Split)->m_pArrIDs[i] = -1;*/

			//fread((*Split)->m_pObjData, sizeof(CGreenDataVertex)*(*Split)->m_iCountObj, 1, file);
			fread((*Split)->m_pArrIDs, sizeof(ID)*(*Split)->m_iCountObj, 1, file);
		}
	}
}

ID CGreen::addArrForCom()
{
	CIRSData* ttmpdata = new CIRSData();
	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		CInfoRenderSegments* tmpirs = new CInfoRenderSegments();
		tmpirs->m_iCount = m_aGreens[i]->m_idCountSplits;
		tmpirs->m_ppSegments = new CSegment*[m_aGreens[i]->m_idCountSplits];
		tmpirs->m_iCountCom = 0;
		ttmpdata->m_aIRS.push_back(tmpirs);

		Array<bool> aVisible;
		aVisible.reserve(m_aGreens[i]->m_uiCountObj);
		for (int k = 0; k < m_aGreens[i]->m_uiCountObj; ++k)
			aVisible[k] = false;

		ttmpdata->m_aVisible.push_back(aVisible);
	}

	ID id_arr = -1;
	for (int i = 0; i < m_aArrComFor.size(); ++i)
	{
		if (m_aArrComFor[i] == 0)
		{
			m_aArrComFor[i] = ttmpdata;
			id_arr = i;
			break;
		}
	}

	if (id_arr == -1)
	{
		m_aArrComFor.push_back(ttmpdata);
		id_arr = m_aArrComFor.size() - 1;
	}

	return id_arr;
}

bool CGreen::existsArrForCom(ID id)
{
	return (m_aArrComFor.size() > id);
}

void CGreen::deleteArrForCom(ID id_arr)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	mem_delete(m_aArrComFor[id_arr]);
}

void CGreen::addModelInArrCom(ID id_model)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model);
	
	for (int i = 0; i < m_aArrComFor.size(); ++i)
	{
		CInfoRenderSegments* tmpirs = new CInfoRenderSegments();
		tmpirs->m_iCount = m_aGreens[id_model]->m_idCountSplits;
		tmpirs->m_ppSegments = new CSegment*[m_aGreens[id_model]->m_idCountSplits];
		tmpirs->m_iCountCom = 0;
		m_aArrComFor[i]->m_aIRS.push_back(tmpirs);
		m_aArrComFor[i]->m_aVisible[id_model].resize(m_aGreens[id_model]->m_pAllTrans.size());
	}
}

void CGreen::deleteModelInArrCom(ID id_model)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model);

	for (int i = 0; i < m_aArrComFor.size(); ++i)
	{
		if (!(m_aArrComFor[i]))
			continue;

		mem_delete(m_aArrComFor[i]->m_aIRS[id_model]);
		m_aArrComFor[i]->m_aIRS.erase(id_model);
	}
}

ID CGreen::getCountGreen()
{
	return m_aGreens.size();
}

char* CGreen::getGreenName(ID id)
{
	if (id < m_aGreens.size())
		return m_aGreens[id]->m_szName;

	return 0;
}

int CGreen::getGreenCountGen(ID id)
{
	if (id < m_aGreens.size())
		return m_aGreens[id]->m_uiCountObj;

	return -1;
}

int CGreen::getGreenCountPoly(ID id)
{
	if (id < m_aGreens.size() && m_aGreens[id]->m_aLods[0])
		return m_aGreens[id]->m_aLods[0]->m_pModel->m_uiAllIndexCount / 3;

	return -1;
}

int CGreen::getGreenTypeCountGen(ID id)
{
	if (id < m_aGreens.size())
		return m_aGreens[id]->m_typeGreen;

	return -1;
}

const char* CGreen::getGreenModel(ID id)
{
	if (id < m_aGreens.size())
		return m_aGreens[id]->m_aLods[0]->m_sPath.c_str();

	return 0;
}

const char* CGreen::getGreenLod1(ID id)
{
	if (id < m_aGreens.size() && m_aGreens[id]->m_aLods[1])
		return m_aGreens[id]->m_aLods[1]->m_sPath.c_str();

	return 0;
}

const char* CGreen::getGreenLod2(ID id)
{
	if (id < m_aGreens.size() && m_aGreens[id]->m_aLods[2])
		return m_aGreens[id]->m_aLods[2]->m_sPath.c_str();

	return 0;
}

const char* CGreen::getGreenMask(ID id)
{
	if (id < m_aGreens.size())
		return m_aGreens[id]->m_szMaskName.c_str();

	return 0;
}

const char* CGreen::getGreenNav(ID id)
{
	if (id < m_aGreens.size() && m_aGreens[id]->m_pPhysMesh)
		return m_aGreens[id]->m_pPhysMesh->m_sPathName.c_str();

	return 0;
}

void CGreen::deleteGreen(ID id)
{
	if (id >= 0 && id < m_aGreens.size())
	{
		deleteModelInArrCom(id);
		mem_delete(m_aGreens[id]);
		m_aGreens.erase(id);
	}
}

void CGreen::clear()
{
	while(m_aGreens.size() > 0)
	{
		deleteModelInArrCom(0);
		mem_delete(m_aGreens[0]);
		m_aGreens.erase(0);
	}

	while (m_aArrComFor.size() > 2)
	{
		mem_delete(m_aArrComFor[2]);
		m_aArrComFor.erase(2);
	}
}

void CGreen::setGreenLod(ID id, int lod, const char* pathname)
{
	if (!(lod >= 0 && lod < GREEN_COUNT_LOD && id >= 0 && id < m_aGreens.size()))
		return;

	bool isunic = true;

	for (int i = 0; i < GREEN_COUNT_LOD; ++i)
	{
		if (m_aGreens[id]->m_aLods[i] == m_aGreens[id]->m_aLods[lod])
			isunic = false;
	}

	if (isunic)
		mem_delete(m_aGreens[id]->m_aLods[lod]);

	char tmppath[1024];
	m_aGreens[id]->m_aLods[lod] = new CLod();
	m_aGreens[id]->m_aLods[lod]->m_sPath = pathname;
	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), pathname);
	SGCore_StaticModelLoad(tmppath, &m_aGreens[id]->m_aLods[lod]->m_pModel);

	for (int k = 0; k < m_aGreens[id]->m_aLods[lod]->m_pModel->m_uiSubsetCount; ++k)
	{
		sprintf(tmppath, "%s.dds", m_aGreens[id]->m_aLods[lod]->m_pModel->m_ppTextures[k]);
		m_aGreens[id]->m_aLods[lod]->m_aIDsTextures[k] = SGCore_MtlLoad(tmppath, (m_aGreens[id]->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
	}
}

void CGreen::setGreenNav(ID id, const char* pathname)
{
	if (!(id >= 0 && id < m_aGreens.size()))
		return;

	char tmpstr[1024];
	sprintf(tmpstr, "%s", pathname);

	mem_delete(m_aGreens[id]->m_pPhysMesh);
	m_aGreens[id]->m_pPhysMesh = new CModel::CPhysMesh();

	char tmppath[1024];
	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), tmpstr);
	m_aGreens[id]->m_pPhysMesh->m_sPathName = tmpstr;

	ISXDataStaticModel* nmesh;
	SGCore_StaticModelLoad(tmppath, &nmesh);
	m_aGreens[id]->m_pPhysMesh->m_iCountVertex = nmesh->m_uiAllVertexCount;
	m_aGreens[id]->m_pPhysMesh->m_iCountIndex = nmesh->m_uiAllIndexCount;
	m_aGreens[id]->m_pPhysMesh->m_pArrVertex = new float3_t[nmesh->m_uiAllVertexCount];
	vertex_static *pVert;
	nmesh->m_pVertexBuffer->Lock(0, 0, (void **)&pVert, 0);
	for (int i = 0; i < nmesh->m_uiAllVertexCount; ++i)
	{
		m_aGreens[id]->m_pPhysMesh->m_pArrVertex[i] = pVert[i].Pos;
	}
	nmesh->m_pVertexBuffer->Unlock();
	
	m_aGreens[id]->m_pPhysMesh->m_pArrIndex = new uint32_t[nmesh->m_uiAllIndexCount];
	m_aGreens[id]->m_pPhysMesh->m_pArrMtl = new ID[nmesh->m_uiAllIndexCount];
	UINT* pInd;
	nmesh->m_pIndexBuffer->Lock(0, 0, (void **)&pInd, 0);
	DWORD prebias = 0;
	int tmp_countindex = 0;
	char tmpnametex[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	for (int i = 0; i < nmesh->m_uiSubsetCount; ++i)
	{
		sprintf(tmpnametex, "%s.dds", nmesh->m_ppTextures[i]);
		ID tmpidmtl = SGCore_MtlLoad(tmpnametex, MTL_TYPE_TREE);
		for (int k = 0; k < nmesh->m_pIndexCount[i]; ++k)
		{
			m_aGreens[id]->m_pPhysMesh->m_pArrIndex[tmp_countindex] = pInd[nmesh->m_pStartIndex[i] + k] /*+ prebias*/;
			m_aGreens[id]->m_pPhysMesh->m_pArrMtl[tmp_countindex] = tmpidmtl;
			++tmp_countindex;
		}
		prebias += nmesh->m_pIndexCount[i];
	}
	nmesh->m_pIndexBuffer->Unlock();
	mem_release_del(nmesh);
}

void CGreen::getNavMeshAndTransform(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, ID*** arr_mtl, int32_t** arr_count_index, CGreenDataVertex*** arr_transform, int32_t** arr_count_transform, int32_t* arr_count_green)
{
	if (m_aGreens.size() <= 0)
		return;

	int count_green = 0;

	for (int id = 0; id < m_aGreens.size(); ++id)
	{
		if (!(m_aGreens[id]->m_pPhysMesh))
			continue;

		/*Array<CSegment*> queue;
		int tmpcount = 0;
		queue.push_back(m_aGreens[id]->m_pSplitsTree);

		m_aGreens[id]->m_uiCountObj = 0;

		while (queue.size())
		{
			if (!(queue[0]->m_aSplits[0]) && /*queue[0]->m_pObjData*/ /*queue[0]->m_pArrIDs && queue[0]->m_iCountObj > 0)
			{
				int m_uiCountObj = m_aGreens[id]->m_uiCountObj;
				int CountAllGreen = queue[0]->m_iCountObj;
				/*CGreenDataVertex* tmpdv = new CGreenDataVertex[m_aGreens[id]->m_uiCountObj + queue[0]->m_iCountObj];
				if (m_aGreens[id]->m_pAllTrans)
					memcpy(tmpdv, m_aGreens[id]->m_pAllTrans, sizeof(CGreenDataVertex)* m_aGreens[id]->m_uiCountObj);
				memcpy(tmpdv + m_aGreens[id]->m_uiCountObj, queue[0]->m_pObjData, sizeof(CGreenDataVertex)* queue[0]->m_iCountObj);
				mem_delete_a(m_aGreens[id]->m_pAllTrans);
				m_aGreens[id]->m_pAllTrans = tmpdv;*/

				/*for (int i = 0; i < queue[0]->m_iCountObj; ++i)
				{
					m_aGreens[id]->m_pAllTrans.push_back(queue[0]->m_pObjData[i]);
				}

				m_aGreens[id]->m_uiCountObj += queue[0]->m_iCountObj;
			}

			for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; ++i)
			{
				if (queue[0]->m_aSplits[i])
					queue.push_back(queue[0]->m_aSplits[i]);
			}

			queue.erase(0);
			++tmpcount;
		}*/

		if (m_aGreens[id]->m_pAllTrans.size() > 0)
			++count_green;
	}

	(*arr_count_green) = count_green;

	if (count_green <= 0)
		return;

	(*arr_vertex) = new float3_t*[count_green];
	(*arr_count_vertex) = new int32_t[count_green];

	(*arr_index) = new uint32_t*[count_green];
	(*arr_mtl) = new ID*[count_green];
	(*arr_count_index) = new int32_t[count_green];

	(*arr_transform) = new CGreenDataVertex*[count_green];
	(*arr_count_transform) = new int32_t[count_green];

	int curr_model = 0;

	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		if (!(m_aGreens[i]->m_pPhysMesh))
			continue;

		(*arr_vertex)[curr_model] = new float3_t[m_aGreens[i]->m_pPhysMesh->m_iCountVertex];
		memcpy((*arr_vertex)[curr_model], m_aGreens[i]->m_pPhysMesh->m_pArrVertex, sizeof(float3_t)* m_aGreens[i]->m_pPhysMesh->m_iCountVertex);
		(*arr_count_vertex)[curr_model] = m_aGreens[i]->m_pPhysMesh->m_iCountVertex;

		(*arr_index)[curr_model] = new uint32_t[m_aGreens[i]->m_pPhysMesh->m_iCountIndex];
		memcpy((*arr_index)[curr_model], m_aGreens[i]->m_pPhysMesh->m_pArrIndex, sizeof(uint32_t)* m_aGreens[i]->m_pPhysMesh->m_iCountIndex);
		(*arr_mtl)[curr_model] = new ID[m_aGreens[i]->m_pPhysMesh->m_iCountIndex];
		memcpy((*arr_mtl)[curr_model], m_aGreens[i]->m_pPhysMesh->m_pArrMtl, sizeof(ID)* m_aGreens[i]->m_pPhysMesh->m_iCountIndex);
		(*arr_count_index)[curr_model] = m_aGreens[i]->m_pPhysMesh->m_iCountIndex;

		(*arr_transform)[curr_model] = new CGreenDataVertex[m_aGreens[i]->m_uiCountObj];
		(*arr_count_transform)[curr_model] = m_aGreens[i]->m_uiCountObj;
		for (int k = 0; k < m_aGreens[i]->m_uiCountObj; ++k)
		{
			(*arr_transform)[curr_model][k] = m_aGreens[i]->m_pAllTrans[k];// SMMatrixScaling(float3(m_aGreens[i]->m_pAllTrans[k].TexCoord.x, m_aGreens[i]->m_pAllTrans[k].TexCoord.x, m_aGreens[i]->m_pAllTrans[k].TexCoord.x)) * SMMatrixRotationY(m_aGreens[i]->m_pAllTrans[k].TexCoord.y) * SMMatrixTranslation(m_aGreens[i]->m_pAllTrans[k].Position);
		}

		++curr_model;
	}

	/*for (int id = 0; id < m_aGreens.size(); ++id)
	{
		mem_delete_a(m_aGreens[id]->m_pAllTrans);
	}*/
}

void CGreen::getPartBeam(const float3* pos, const float3 * dir, CSegment** arrsplits, int *count, CSegment* comsegment, ID curr_splits_ids_render)
{
	float3 center;
	float radius;
	comsegment->m_pBoundVolumeP->getSphere(&center, &radius);

	float distsqr = SGCore_0DistancePointBeam2(center, *pos, *dir);
	if (comsegment->m_iCountObj > 0 && distsqr <= radius*radius)
	{
		if (comsegment->m_idNonEnd)
		{
			for (int j = 0; j<GREEN_COUNT_TYPE_SEGMENTATION; ++j)
			{
				if (comsegment->m_aSplits[j])
					getPartBeam(pos, dir, arrsplits, count, comsegment->m_aSplits[j], curr_splits_ids_render);
			}
		}
		else
		{
			if ((*count) < curr_splits_ids_render)
			{
				arrsplits[(*count)] = comsegment;

				++(*count);
			}
		}
	}
}

bool CGreen::traceBeam(const float3* start, const float3* dir, float3* _res, ID* idgreen, ID* idsplits, ID* idobj, ID* idmtl)
{
	if (m_aGreens.size() <= 0)
		return false;

	CTriangle tmptri;
	bool tmpiscom = true;
	float3 ip;
	float3 res;
	float3 il;
	res = (*start) + float3(10000.0f, 10000.0f, 10000.0f);
	il = (*dir) * 10000.0f;
	bool found = false;
	CInfoRenderSegments* irs = 0;
	CModel* model = 0;
	float4x4 mat;

	for (int id = 0; id < m_aGreens.size(); ++id)
	{
		model = m_aGreens[id];
		irs = m_aArrComFor[1]->m_aIRS[id];
		irs->m_iCountCom = 0;

		getPartBeam(start, dir, irs->m_ppSegments, &(irs->m_iCountCom), model->m_pSplitsTree, irs->m_iCount);

		vertex_static* pVertData = 0;
		if (FAILED(model->m_aLods[0]->m_pModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertData, 0)))
			continue;

		DWORD* pIndData = 0;
		if (FAILED(model->m_aLods[0]->m_pModel->m_pIndexBuffer->Lock(0, 0, (void**)&pIndData, 0)))
			continue;

		for (int k = 0; k < irs->m_iCountCom; ++k)
		{
			for (int key = 0; key < irs->m_ppSegments[k]->m_iCountObj; ++key)
			{
				UINT tmpcountind = 0;
				for (int g = 0; g < model->m_aLods[0]->m_pModel->m_uiSubsetCount; ++g)
				{
					for (int poly = 0; poly < model->m_aLods[0]->m_pModel->m_pIndexCount[g] / 3; ++poly)
					{
						/*float tmpscale = irs->m_ppSegments[k]->m_pObjData[key].m_vTexCoord.x;
						mat = SMMatrixScaling(tmpscale, tmpscale, tmpscale) * SMMatrixRotationY(irs->m_ppSegments[k]->m_pObjData[key].m_vTexCoord.y) * SMMatrixTranslation(irs->m_ppSegments[k]->m_pObjData[key].m_vPosition);
						*/
						CGreenDataVertex oDataVertex = m_aGreens[id]->m_pAllTrans[irs->m_ppSegments[k]->m_pArrIDs[key]];

						float tmpscale = oDataVertex.m_vTexCoord.x;
						mat = SMMatrixScaling(tmpscale, tmpscale, tmpscale) * SMMatrixRotationY(oDataVertex.m_vTexCoord.y) * SMMatrixTranslation(oDataVertex.m_vPosition);

						tmptri.m_vA = SMVector3Transform(pVertData[pIndData[poly]].Pos, mat);
						tmptri.m_vB = SMVector3Transform(pVertData[pIndData[poly + 1]].Pos, mat);
						tmptri.m_vC = SMVector3Transform(pVertData[pIndData[poly + 2]].Pos, mat);

						if (tmptri.IntersectLine((*start), il, &ip))
						{
							if (SMVector3Length2((*start) - res) > SMVector3Length2((*start) - ip))
							{
								res = ip;
								found = true;

								if (idgreen)
									*idgreen = id;

								if (idsplits)
									*idsplits = irs->m_ppSegments[k]->m_id;

								if (idobj)
									*idobj = key;

								if (idmtl)
									*idmtl = model->m_aLods[0]->m_aIDsTextures[g];
							}
						}
					}

					tmpcountind += model->m_aLods[0]->m_pModel->m_uiSubsetCount;
				}
			}
		}

		model->m_aLods[0]->m_pModel->m_pVertexBuffer->Unlock();
		model->m_aLods[0]->m_pModel->m_pIndexBuffer->Unlock();
	}

	if (found && _res)
		*_res = res;

	return found;
}

void CGreen::getPartBB(float3* bbmin, float3 * bbmax, CSegment** arrsplits, int *count, CSegment* comsegment, ID curr_splits_ids_render)
{
	float3 min,max;
	comsegment->m_pBoundVolumeP->getMinMax(&min, &max);

	if (comsegment->m_iCountObj > 0 && SGCore_0InretsectBox(bbmin, bbmax, &min, &max))
	{
		if (comsegment->m_idNonEnd)
		{
			for (int j = 0; j<GREEN_COUNT_TYPE_SEGMENTATION; ++j)
			{
				if (comsegment->m_aSplits[j])
					getPartBB(bbmin, bbmax, arrsplits, count, comsegment->m_aSplits[j], curr_splits_ids_render);
			}
		}
		else
		{
			if ((*count) < curr_splits_ids_render)
			{
				arrsplits[(*count)] = comsegment;

				++(*count);
			}
		}
	}
}

bool CGreen::getOccurencessLeafGrass(float3* bbmin, float3* bbmax, int physic_mtl)
{
	if (m_aGreens.size() <= 0)
		return false;

	bool isfound = false;
	CInfoRenderSegments* irs = 0;
	CModel* model = 0;
	float3 p1, p2, p3, min, max;
	float4x4 mat;

	for (int id = 0; id < m_aGreens.size() && !isfound; ++id)
	{
		model = m_aGreens[id];
		irs = m_aArrComFor[1]->m_aIRS[id];
		irs->m_iCountCom = 0;

		getPartBB(bbmin, bbmax, irs->m_ppSegments, &(irs->m_iCountCom), model->m_pSplitsTree, irs->m_iCount);

		vertex_static* pVertData = 0;
		if (FAILED(model->m_aLods[0]->m_pModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertData, 0)))
			continue;

		DWORD* pIndData = 0;
		if (FAILED(model->m_aLods[0]->m_pModel->m_pIndexBuffer->Lock(0, 0, (void**)&pIndData, 0)))
			continue;

		for (int g = 0; g < model->m_aLods[0]->m_pModel->m_uiSubsetCount && !isfound; ++g)
		{
			int pt = SGCore_MtlGetPhysicType(model->m_aLods[0]->m_aIDsTextures[g]);
			if (SGCore_MtlGetPhysicType(model->m_aLods[0]->m_aIDsTextures[g]) != physic_mtl)
				continue;

			for (int k = 0; k < irs->m_iCountCom && !isfound; ++k)
			{
				for (int key = 0; key < irs->m_ppSegments[k]->m_iCountObj && !isfound; ++key)
				{


					for (int poly = 0; poly < model->m_aLods[0]->m_pModel->m_pIndexCount[g] / 3 && !isfound; ++poly)
					{
						/*float tmpscale = irs->m_ppSegments[k]->m_pObjData[key].m_vTexCoord.x;
						mat = SMMatrixScaling(tmpscale, tmpscale, tmpscale) * SMMatrixRotationY(irs->m_ppSegments[k]->m_pObjData[key].m_vTexCoord.y) * SMMatrixTranslation(irs->m_ppSegments[k]->m_pObjData[key].m_vPosition);
						*/

						CGreenDataVertex oDataVertex = m_aGreens[id]->m_pAllTrans[irs->m_ppSegments[k]->m_pArrIDs[key]];

						float tmpscale = oDataVertex.m_vTexCoord.x;
						mat = SMMatrixScaling(tmpscale, tmpscale, tmpscale) * SMMatrixRotationY(oDataVertex.m_vTexCoord.y) * SMMatrixTranslation(oDataVertex.m_vPosition);

						p1 = SMVector3Transform(pVertData[pIndData[poly]].Pos, mat);
						p2 = SMVector3Transform(pVertData[pIndData[poly + 1]].Pos, mat);
						p3 = SMVector3Transform(pVertData[pIndData[poly + 2]].Pos, mat);

						min = p1;
						max = p1;

						if (min.x > p2.x)
							min.x = p2.x;
						if (min.y > p2.y)
							min.y = p2.y;
						if (min.z > p2.z)
							min.z = p2.z;

						if (min.x > p3.x)
							min.x = p3.x;
						if (min.y > p3.y)
							min.y = p3.y;
						if (min.z > p3.z)
							min.z = p3.z;

						if (max.x < p2.x)
							max.x = p2.x;
						if (max.y < p2.y)
							max.y = p2.y;
						if (max.z < p2.z)
							max.z = p2.z;

						if (max.x < p3.x)
							max.x = p3.x;
						if (max.y < p3.y)
							max.y = p3.y;
						if (max.z < p3.z)
							max.z = p3.z;

						if (SGCore_0InretsectBox(bbmin, bbmax, &min, &max))
						{
							isfound = true;
							break;
						}
					}
				}
			}
		}

		model->m_aLods[0]->m_pModel->m_pVertexBuffer->Unlock();
		model->m_aLods[0]->m_pModel->m_pIndexBuffer->Unlock();
	}

	return isfound;
}