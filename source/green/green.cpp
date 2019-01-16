
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "green.h"

CGreen::CGreen()
{
	if(CGreen::m_pDXDevice)
	{
	GXVERTEXELEMENT oInstanceGreen[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		{0, 20, GXDECLTYPE_FLOAT3, GXDECLUSAGE_NORMAL},
		{0, 32, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TANGENT},
		{0, 44, GXDECLTYPE_FLOAT3, GXDECLUSAGE_BINORMAL},
		{1, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD1},
		{1, 12, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD2},
		{1, 24, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD3},
		GXDECL_END()
	};

	m_pVertexDeclarationGreen = CGreen::m_pDXDevice->createVertexDeclaration(oInstanceGreen);

	createInstVB();

	CVisCaclObj *pVisCaclObj1 = new CVisCaclObj();
	m_aVisCaclObj.push_back(pVisCaclObj1);

	CVisCaclObj *pVisCaclObj2 = new CVisCaclObj();
	m_aVisCaclObj.push_back(pVisCaclObj2);

	m_iCurrCountDrawObj = 0;
}
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

	while (m_aVisCaclObj.size() > 0)
	{
		mem_delete(m_aVisCaclObj[0]);
		m_aVisCaclObj.erase(0);
	}
}

//**************************************************************************

CGreen::CModel::CModel()
{
	m_aLods[0] = m_aLods[1] = m_aLods[2] = 0;
	m_uiCountObj = 0;
	m_idCountSplits = 0;
	m_pSplitsTree = 0;
	m_pPhysMesh = 0;
	m_pDataLod0 = 0;
	m_szName[0] = 0;

	m_vMax = float3(0,0,0);
	m_vMin = float3(0, 0, 0);
}

CGreen::CModel::~CModel()
{
	mem_delete(m_pSplitsTree);
	mem_delete(m_pPhysMesh);
	mem_delete(m_pDataLod0);

	mem_del(m_aLods[0]);
	if (m_aLods[0] != m_aLods[1])
		mem_del(m_aLods[1]);

	if (m_aLods[0] != m_aLods[2] && m_aLods[1] != m_aLods[2])
		mem_del(m_aLods[2]);
}

//**************************************************************************

CGreen::CModel::CPhysMesh::CPhysMesh()
{
	m_sPathName = "";
}

CGreen::CModel::CPhysMesh::~CPhysMesh()
{

}

//**************************************************************************

CGreen::CSegment::CSegment()
{
	for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; ++i)
		m_aSplits[i] = 0;
	
	m_pArrIDs = 0;
	m_iCountObj = 0;
	m_pBoundVolumeSys = 0;
	m_pBoundVolumeP = 0;
	m_fDistForCamera = 0;
	m_id = -1;
	m_isFinite = true;
}

CGreen::CSegment::~CSegment()
{
	for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		mem_delete(m_aSplits[i]);

	mem_release_del(m_pBoundVolumeSys);
	mem_release_del(m_pBoundVolumeP);
}

//**************************************************************************

CGreen::CLod::CLod()
{
	m_pModel = 0;
}

CGreen::CLod::~CLod()
{
	m_aIDsTextures.clear();
	mem_release_del(m_pModel);
}

//**************************************************************************

CGreen::CVisInfo::CVisInfo()
{
	m_ppSegments = 0; m_iCount = 0; m_iCountCom = 0;
}

CGreen::CVisInfo::~CVisInfo()
{
	mem_delete_a(m_ppSegments);
}

//**************************************************************************

CGreen::CVisCaclObj::CVisCaclObj()
{
	
}

CGreen::CVisCaclObj::~CVisCaclObj()
{
	m_aQueue.clear();

	for (int i = 0; i < m_aVisibleInfo.size(); ++i)
	{
		mem_delete(m_aVisibleInfo[i]);
	}
	m_aVisibleInfo.clear();
}

//##########################################################################

void CGreen::onLostDevice()
{
	mem_release_del(m_pTransVertBuf);
}

void CGreen::onResetDevice()
{
	createInstVB();
}

void CGreen::createInstVB()
{
	CGreen::m_pDXDevice->CreateVertexBuffer(
		GREEN_MAX_ELEM_IN_DIP * sizeof(CGreenDataVertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&m_pTransVertBuf,
		0);
}

//##########################################################################

void CGreen::preSegmentation(CModel *pGreen, const float3 *pLevelMin, const float3 *pLevelMax)
{
	pGreen->m_pSplitsTree = new CSegment();

	float3 vMin, vMax;

	//просчет ограничивающего бокса модели
	pGreen->m_pSplitsTree->m_pBoundVolumeSys = SGCore_CrBound();
	SGCore_FCompBoundBox(pGreen->m_aLods[0]->m_pModel->m_pVertexBuffer, &(pGreen->m_pSplitsTree->m_pBoundVolumeSys), pGreen->m_aLods[0]->m_pModel->m_uiAllVertexCount, sizeof(vertex_static_ex));

	pGreen->m_pSplitsTree->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);
	pGreen->m_vMax = vMax * (1.f + GREEN_GEN_RAND_SCALE);
	pGreen->m_vMin = vMin * (1.f + GREEN_GEN_RAND_SCALE);

	//расчет ограничивающего бокса всей растительности
	vMin = *pLevelMin;
	vMax = *pLevelMax;

	vMax.x += pGreen->m_vMax.x;
	vMax.y += pGreen->m_vMax.y;
	vMax.z += pGreen->m_vMax.z;

	vMin.x += pGreen->m_vMin.x;
	vMin.y += pGreen->m_vMin.y;
	vMin.z += pGreen->m_vMin.z;

	float fX = vMax.x - vMin.x;
	float fY = vMax.y - vMin.y;
	float fZ = vMax.z - vMin.z;

	pGreen->m_pSplitsTree->m_pBoundVolumeP = SGCore_CrBound();
	pGreen->m_pSplitsTree->m_pBoundVolumeP->setMinMax(&vMin, &vMax);

	//выравниваем по квадрату
	if (fX > fZ)
	{
		fZ = fX - fZ;
		fZ /= 2.f;
		vMax.z += fZ;
		vMin.z -= fZ;
	}
	else if (fZ > fX)
	{
		fX = fZ - fX;
		fX /= 2.f;
		vMax.x += fX;
		vMin.x -= fX;
	}

	pGreen->m_pSplitsTree->m_pBoundVolumeSys->setMinMax(&vMin, &vMax);


	//заполняем внутренний массив сплита идентификаторов объектов
	pGreen->m_pSplitsTree->m_iCountObj = pGreen->m_uiCountObj;
	if (pGreen->m_uiCountObj > 0)
	{
		pGreen->m_pSplitsTree->m_pArrIDs = new ID[pGreen->m_uiCountObj];

		for (int i = 0; i<pGreen->m_uiCountObj; ++i)
		{
			pGreen->m_pSplitsTree->m_pArrIDs[i] = i;
		}
	}

	pGreen->m_pSplitsTree->m_isFinite = false;

	cycleSegmentation(pGreen->m_pSplitsTree, pGreen);
}

void CGreen::cycleSegmentation(CSegment *pSplit, CModel *pGreen)
{
	Array<CSegment*> aQueue;
	int iCount = 0;
	aQueue.push_back(pSplit);

	float3 vMin, vMax;

	while (aQueue.size() > iCount)
	{
		CSegment *pCurrSegment = aQueue[iCount];
		aQueue[iCount]->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);

		//если размеры текущего сплита все еще больше установленного предела, значит делим дальше
		if ((vMax.x - vMin.x)*0.5f > GREEN_BB_MIN_X && (vMax.z - vMin.z)*0.5f > GREEN_BB_MIN_Z)
		{
			segmentation(pCurrSegment, pGreen);
			for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; i++)
			{
				if (pCurrSegment->m_aSplits[i])
					aQueue.push_back(pCurrSegment->m_aSplits[i]);
			}

			pCurrSegment->m_isFinite = false;
		}
		else
			pCurrSegment->m_isFinite = true;

		//aQueue.erase(0);
		++iCount;
	}
}

void CGreen::segmentation(CSegment *pSplit, CModel *pGreen)
{
	Array<ID> aObects[4];

	for (int i = 0; i<4; i++)
		pSplit->m_aSplits[i] = new CSegment();

	ISXBound *aBounds[4];
	for (int i = 0; i < 4; ++i)
		aBounds[i] = SGCore_CrBound();

	SGCore_0ComBoundBoxArr4(pSplit->m_pBoundVolumeSys, aBounds);

	//инициализируем ограничивающие объемы
	float3 vMin, vMax;
	for (int i = 0; i<4; ++i)
	{
		pSplit->m_aSplits[i]->m_pBoundVolumeSys = aBounds[i];
		pSplit->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);
		pSplit->m_aSplits[i]->m_pBoundVolumeP = SGCore_CrBound();
		pSplit->m_aSplits[i]->m_pBoundVolumeP->setMinMax(&vMin, &vMax);
	}

	//инициализируем массив занятости объектов
	bool *aBusyObj = 0;
	if (pSplit->m_iCountObj)
	{
		aBusyObj = new bool[pSplit->m_iCountObj];
		for (int i = 0; i < pSplit->m_iCountObj; i++)
			aBusyObj[i] = false;
	}

	//распределяем объекты по сплитам
	for (int i = 0; i<4; ++i)
	{
		pSplit->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);
		//SGCore_FCreateBoundingBoxMesh(&vMin, &vMax, &(pSplit->m_aSplits[i]->BoundBox));

		for (int j = 0; j<pSplit->m_iCountObj; ++j)
		{
			//если позиция проверяемого объекта находиться в пределах ограничивающего бокса
			if (
				(int(vMax.x * 1000) >= int(pGreen->m_pAllTrans[pSplit->m_pArrIDs[j]].m_vPosition.x * 1000) && int(vMin.x * 1000) <= int(pGreen->m_pAllTrans[pSplit->m_pArrIDs[j]].m_vPosition.x * 1000))
				&&
				(int(vMax.z * 1000) >= int(pGreen->m_pAllTrans[pSplit->m_pArrIDs[j]].m_vPosition.z * 1000) && int(vMin.z * 1000) <= int(pGreen->m_pAllTrans[pSplit->m_pArrIDs[j]].m_vPosition.z * 1000))
				&&
				!(aBusyObj[j])
				)
			{
				//записываем объект в массив
				aObects[i].push_back(j);

				//помечаем объект занятым
				aBusyObj[j] = true;
			}
		}
	}

	//записываем данные распределения объектов по сплитам
	for (int i = 0; i<4; ++i)
	{
		pSplit->m_aSplits[i]->m_iCountObj = aObects[i].size();

		if (pSplit->m_aSplits[i]->m_iCountObj > 0)
		{
			pSplit->m_aSplits[i]->m_pArrIDs = new ID[pSplit->m_aSplits[i]->m_iCountObj];
			for (int k = 0; k < aObects[i].size(); ++k)
			{
				pSplit->m_aSplits[i]->m_pArrIDs[k] = pSplit->m_pArrIDs[aObects[i][k]];
			}

			alignBound(pGreen, pSplit->m_aSplits[i]);

			aObects[i].clear();
		}

		pSplit->m_aSplits[i]->m_isFinite = false;
		}

	mem_delete_a(pSplit->m_pArrIDs);
	mem_delete_a(aBusyObj);
}

void CGreen::alignBound(CModel *pGreen, CSegment *pSplit)
{
	if (pSplit->m_iCountObj > 0)
	{
		float3 vMax = pGreen->m_pAllTrans[pSplit->m_pArrIDs[0]].m_vPosition;
		float3 vMin = pGreen->m_pAllTrans[pSplit->m_pArrIDs[0]].m_vPosition;

		for (int k = 0; k<pSplit->m_iCountObj; ++k)
		{
			if (pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.y > vMax.y)
				vMax.y = pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.y;

			if (pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.y < vMin.y)
				vMin.y = pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.y;


			if (pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.x > vMax.x)
				vMax.x = pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.x;

			if (pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.x < vMin.x)
				vMin.x = pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.x;


			if (pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.z > vMax.z)
				vMax.z = pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.z;

			if (pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.z < vMin.z)
				vMin.z = pGreen->m_pAllTrans[pSplit->m_pArrIDs[k]].m_vPosition.z;
		}

		float3 vTransformMin, vTransformMax;
		float fScale = 1.f + GREEN_GEN_RAND_SCALE;

		pSplit->m_pBoundVolumeSys->getMinMax(&vTransformMin, &vTransformMax);
		vTransformMax.y = vMax.y + pGreen->m_vMax.y * fScale;
		vTransformMin.y = vMin.y + pGreen->m_vMin.y * fScale;

		pSplit->m_pBoundVolumeSys->setMinMax(&vTransformMin, &vTransformMax);

		vTransformMax.x = vMax.x + pGreen->m_vMax.x * fScale;
		vTransformMax.y = vMax.y + pGreen->m_vMax.y * fScale;
		vTransformMax.z = vMax.z + pGreen->m_vMax.z * fScale;

		vTransformMin.x = vMin.x + pGreen->m_vMin.x * fScale;
		vTransformMin.y = vMin.y + pGreen->m_vMin.y * fScale;
		vTransformMin.z = vMin.z + pGreen->m_vMin.z * fScale;

		pSplit->m_pBoundVolumeP->setMinMax(&vTransformMin, &vTransformMax);
	}
}

void CGreen::setSplitID(CModel *pGreen)
{
	Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> aQueue;
	int iCount = 0;
	aQueue.push_back(pGreen->m_pSplitsTree);

	while (aQueue.size() > iCount)
	{
		setSplitID2(pGreen, aQueue[iCount], &aQueue);

		++iCount;
	}
}

void CGreen::setSplitID2(CModel *pGreen, CSegment *pSplit, Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> *pQueue)
{
	if (pSplit)
	{
		pSplit->m_id = pGreen->m_idCountSplits;
		pGreen->m_aSplitsArr[pSplit->m_id] = pSplit;
		++(pGreen->m_idCountSplits);

		for (int i = 0; i < 4; i++)
		{
			if (pSplit->m_aSplits[i])
				pQueue->push_back(pSplit->m_aSplits[i]);
		}
	}
}

void CGreen::computeBB(const CGreenDataVertex &oTrans, CBoundBox &oBox)
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

//##########################################################################

void CGreen::comArrIndeces(const IFrustum *pFrustum, const float3 *pViewPos, ID idVisCaclObj)
{
	GREEN_PRECOND_IDVISCALCOBJ_ERR(idVisCaclObj);

	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

	int iCountIteration = 0;
	int iCount = 0;
	int *pCountCom = 0;
	CSegment **ppSegments = 0;

	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		CModel *pGreen = m_aGreens[i];
		CVisCaclObj *pCurrVisCalcObj = m_aVisCaclObj[idVisCaclObj];

		//если массив видимости меньше чем количество объектов, то расширяем его
		if (pCurrVisCalcObj->m_aVisibleInfo[i]->m_aVisible.size() < pGreen->m_pAllTrans.size())
			pCurrVisCalcObj->m_aVisibleInfo[i]->m_aVisible.resize(pGreen->m_pAllTrans.size());

		//обнуляем массив видимости
		memset(&(pCurrVisCalcObj->m_aVisibleInfo[i]->m_aVisible[0]), 0, sizeof(bool)* pGreen->m_pAllTrans.size());

		iCount = pCurrVisCalcObj->m_aVisibleInfo[i]->m_iCount;
		pCurrVisCalcObj->m_aVisibleInfo[i]->m_iCountCom = 0;
		pCountCom = &(pCurrVisCalcObj->m_aVisibleInfo[i]->m_iCountCom);
		ppSegments = pCurrVisCalcObj->m_aVisibleInfo[i]->m_ppSegments;
		
		pCurrVisCalcObj->m_aQueue.clearFast();
		
		pCurrVisCalcObj->m_aQueue.push_back(pGreen->m_pSplitsTree);

		while (pCurrVisCalcObj->m_aQueue.size() > iCountIteration)
		{
			comRecArrIndeces(i, idVisCaclObj, pFrustum, ppSegments, pCountCom, pCurrVisCalcObj->m_aQueue[iCountIteration], pViewPos, &pCurrVisCalcObj->m_aQueue, iCount);

			++iCountIteration;
		}
	}
}

void CGreen::sort4SplitsFront2Back(const CSegment *pParentSplit, const float3 *pViewPos, ID aSortId[GREEN_COUNT_TYPE_SEGMENTATION])
{
	float3 vSphereCenter;
	float fSphereRadius;

	for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; ++i)
	{
		aSortId[i] = -1;
		if (pParentSplit->m_aSplits[i])
		{
			pParentSplit->m_aSplits[i]->m_pBoundVolumeP->getSphere(&vSphereCenter, &fSphereRadius);
			pParentSplit->m_aSplits[i]->m_fDistForCamera = SMVector3Length2((vSphereCenter - (*pViewPos))) - fSphereRadius*fSphereRadius;
		}
	}

	float fDist1, fDist2;
	int iCountGreater = 0;

	for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; ++i)
	{
		if (!pParentSplit->m_aSplits[i])
			continue;

		iCountGreater = 0;
		fDist1 = pParentSplit->m_aSplits[i]->m_fDistForCamera;

		for (int k = 0; k<GREEN_COUNT_TYPE_SEGMENTATION; ++k)
		{
			if (pParentSplit->m_aSplits[k])
			{
				fDist2 = pParentSplit->m_aSplits[k]->m_fDistForCamera;

				if (i != k && fDist2 >= fDist1)
					++iCountGreater;
			}
		}

		while (true)
		{
			if (aSortId[iCountGreater] == -1)
			{
				aSortId[iCountGreater] = i;
				break;
			}
			else
				--iCountGreater;
		}
	}
}

void CGreen::comRecArrIndeces(ID idGreen, ID idVisCaclObj, const IFrustum *pFrustum, CSegment **ppSplits, int *iCountCom, CSegment *pCurrSplit, const float3 *pViewPos, Array<CSegment*, GREEN_DEFAULT_RESERVE_COM> *pQueue, int iAllCount)
{
	if (idGreen < 0 || m_aGreens.size() < idGreen)
		return;

	CModel *pGreen = m_aGreens[idGreen];
	float3 vSphereCenter;
	float fSphereRadius;

	ID aSortId[GREEN_COUNT_TYPE_SEGMENTATION];

	for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; ++i)
		aSortId[i] = i;

	pCurrSplit->m_pBoundVolumeP->getSphere(&vSphereCenter, &fSphereRadius);

	// если объекты в сплите есть и сфера видна фрустуму
	if (/*pCurrSplit->m_iCountObj > 0 && */pFrustum->sphereInFrustum(&vSphereCenter, fSphereRadius))
	{
		// если не конечный сплит
		if (!(pCurrSplit->m_isFinite))
		{
			// если включена сортировка сплитов то сортируем
			if (CGreen::m_isUseSortFrontToBackSplits)
				sort4SplitsFront2Back(pCurrSplit, pViewPos, aSortId);

			for (int j = 0; j<GREEN_COUNT_TYPE_SEGMENTATION; ++j)
			{
				if (aSortId[(GREEN_COUNT_TYPE_SEGMENTATION - 1) - j] != -1 && pCurrSplit->m_aSplits[aSortId[(GREEN_COUNT_TYPE_SEGMENTATION - 1) - j]])
					pQueue->push_back(pCurrSplit->m_aSplits[aSortId[(GREEN_COUNT_TYPE_SEGMENTATION - 1) - j]]);
			}
		}
		else
		{
			if ((*iCountCom) <= iAllCount && pCurrSplit->m_pArrIDs)
			{
				// количество видимых объектов
				int iCountVisible = 1;

				// если текущий массив просчетов "для камеры" и тип растительности "деревья"
				if (pGreen->m_typeGreen == GREEN_TYPE_TREE /*GetAsyncKeyState('Y')*/)
				{
					iCountVisible = 0;
					float3 vMin, vMax;

					// проходимся по массиву объектов
					for (int i = 0; i < pCurrSplit->m_iCountObj; ++i)
					{
						vMin = pGreen->m_aTransW[pCurrSplit->m_pArrIDs[i]].m_vMin;
						vMax = pGreen->m_aTransW[pCurrSplit->m_pArrIDs[i]].m_vMax;

						// сначала фрустум куллинг бокса
						bool isVisible = pFrustum->boxInFrustum(&vMin, &vMax);

						// если считаем для камеры то делаем occlusion culling test 
						if (isVisible && idVisCaclObj == 0)
							isVisible = SGCore_OC_IsVisible(&vMax, &vMin);

						if (isVisible)
							++iCountVisible;

						// помечаем объект как видимый
						m_aVisCaclObj[idVisCaclObj]->m_aVisibleInfo[idGreen]->m_aVisible[pCurrSplit->m_pArrIDs[i]] = isVisible;
					}
				}

				if (iCountVisible > 0)
				{
					ppSplits[(*iCountCom)] = pCurrSplit;
					pCurrSplit->m_fDistForCamera = 0;
					++(*iCountCom);
				}
			}
		}
	}
}

//##########################################################################

void CGreen::render2(DWORD timeDelta, const float3 *pViewPos, ID idGreen, int iLod, ID idTex)
{
	if (idGreen < 0 || m_aGreens.size() < idGreen)
		return;

	CModel *pGreen = m_aGreens[idGreen];
	CLod *pLod = pGreen->m_aLods[iLod];

	//если есть что к отрисовке
	if (m_iCurrCountDrawObj)
	{
		CGreen::m_pDXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | m_iCurrCountDrawObj));

		CGreen::m_pDXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		CGreen::m_pDXDevice->SetStreamSource(1, m_pTransVertBuf, 0, sizeof(CGreenDataVertex));

		CGreen::m_pDXDevice->SetStreamSource(0, pLod->m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static_ex));
		CGreen::m_pDXDevice->SetIndices(pLod->m_pModel->m_pIndexBuffer);
		CGreen::m_pDXDevice->SetVertexDeclaration(m_pVertexDeclarationGreen);

		int iCountIndex = 0;
		for (int i = 0; i < pLod->m_pModel->m_uiSubsetCount; i++)
		{
			SGCore_MtlSet((idTex > 0 ? idTex : pLod->m_aIDsTextures[i]), 0);

			if (m_aGreens[idGreen]->m_typeGreen == GREEN_TYPE_GRASS)
				CGreen::m_pDXDevice->SetVertexShaderConstantF(59, (float*)&float2_t(CGreen::m_fDistGrassLessening, CGreen::m_vDistLods.x), 1);
			else
				CGreen::m_pDXDevice->SetVertexShaderConstantF(59, (float*)&float2_t(0,0), 1);

			CGreen::m_pDXDevice->SetVertexShaderConstantF(60, (float*)pViewPos, 1);
			CGreen::m_pDXDevice->SetVertexShaderConstantF(61, (float*)&(pLod->m_pModel->m_vBSphere), 1);
			CGreen::m_pDXDevice->SetVertexShaderConstantF(62, (float*)&(pLod->m_pModel->m_vBBMax), 1);
			CGreen::m_pDXDevice->SetVertexShaderConstantF(63, (float*)&(pLod->m_pModel->m_vBBMin), 1);

			SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, pLod->m_pModel->m_pVertexCount[i], iCountIndex, pLod->m_pModel->m_pIndexCount[i] / 3);
			Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ((pLod->m_pModel->m_pIndexCount[i] / 3) * m_iCurrCountDrawObj));
			iCountIndex += pLod->m_pModel->m_pIndexCount[i];
		}

		CGreen::m_pDXDevice->SetStreamSourceFreq(0, 1);
		CGreen::m_pDXDevice->SetStreamSourceFreq(1, 1);
	}
}

void CGreen::render(DWORD timeDelta, const float3 *pViewPos, GREEN_TYPE type, ID idVisCaclObj)
{
	GREEN_PRECOND_IDVISCALCOBJ_ERR(idVisCaclObj);

	float3 vSphereCenter;
	float fSphereRadius;

	//циклом проходимся по каждому виду растительности
	for (int idGreen = 0; idGreen < m_aGreens.size(); ++idGreen)
	{
		//если тип не указан
		if (type != GREEN_TYPE_ALL && type != m_aGreens[idGreen]->m_typeGreen)
			continue;

		renderSingly(timeDelta, pViewPos, idGreen, -1, idVisCaclObj);
	}
}

void CGreen::renderSingly(DWORD timeDelta, const float3 *pViewPos, ID idGreen, ID idTex, ID idVisCaclObj)
{
	GREEN_PRECOND_IDVISCALCOBJ_ERR(idVisCaclObj);

	if (m_aGreens.size() <= idGreen)
		return;

	float3 vSphereCenter;
	float fSphereRadius;

	CModel *pGreen = m_aGreens[idGreen];
	CVisCaclObj *pCurrVisCalcObj = m_aVisCaclObj[idVisCaclObj];

	CSegment **ppArrSplits = pCurrVisCalcObj->m_aVisibleInfo[idGreen]->m_ppSegments;
	int iCountSplits = pCurrVisCalcObj->m_aVisibleInfo[idGreen]->m_iCountCom;

	// проходимся по всем лодам
	for (int iLod = 0; iLod < GREEN_COUNT_LOD; ++iLod)
	{
		if (pGreen->m_aLods[iLod])
		{
			CGreenDataVertex *pVerteces = 0;
			m_pTransVertBuf->Lock(0, 0, (void**)&pVerteces, D3DLOCK_DISCARD);

			// количество объектов к отрисовке
			m_iCurrCountDrawObj = 0;

			//проходимся по сплитам (которые к отрисовке)
			for (int i = 0; i < iCountSplits; ++i)
			{
				CSegment *pCurrSegment = ppArrSplits[i];

				// если количество объектов для отрисовки превысело лимит (либо превысит при текущей итерации), значит рисуем, а потом опять считаем
				if (m_iCurrCountDrawObj + pCurrSegment->m_iCountObj >= GREEN_MAX_ELEM_IN_DIP)
				{
					m_pTransVertBuf->Unlock();
					render2(timeDelta, pViewPos, idGreen, iLod, idTex);
					m_pTransVertBuf->Lock(0, 0, (void**)&pVerteces, D3DLOCK_DISCARD);
					m_iCurrCountDrawObj = 0;
				}

				pCurrSegment->m_pBoundVolumeP->getSphere(&vSphereCenter, &fSphereRadius);
				pCurrSegment->m_fDistForCamera = SMVector3Length((vSphereCenter - (*pViewPos))) - fSphereRadius;

				if (
					//распределение по дистанции есесно и по лодам
					(
					(iLod == 0 && pCurrSegment->m_fDistForCamera <= CGreen::m_vDistLods.x) ||
					(iLod == 1 && pCurrSegment->m_fDistForCamera <= CGreen::m_vDistLods.y && pCurrSegment->m_fDistForCamera > CGreen::m_vDistLods.x) ||
					(iLod == 2 && pCurrSegment->m_fDistForCamera > CGreen::m_vDistLods.y)
					)
					)
				{
					//если это не трава
					if (!(iLod == 0 && pGreen->m_typeGreen == GREEN_TYPE_GRASS))
					{
						//проходимся по всем объектам сплита
						for (int k = 0; k < pCurrSegment->m_iCountObj; ++k)
						{
							if (pCurrVisCalcObj->m_aVisibleInfo[idGreen]->m_aVisible[pCurrSegment->m_pArrIDs[k]])
							{
								pVerteces[m_iCurrCountDrawObj] = pGreen->m_pAllTrans[pCurrSegment->m_pArrIDs[k]];
								++m_iCurrCountDrawObj;
							}
						}
					}
					//иначе это трава, а ее по особенному рисуем
					else if (iLod == 0 && pGreen->m_typeGreen == GREEN_TYPE_GRASS)
					{
						// если плотность 100% то просто все копируем
						if (CGreen::m_iRenderFreqGrass >= 100)
						{
							for (int k = 0; k < pCurrSegment->m_iCountObj; ++k)
							{
								pVerteces[m_iCurrCountDrawObj] = pGreen->m_pAllTrans[pCurrSegment->m_pArrIDs[k]];
								++m_iCurrCountDrawObj;
							}
						}
						//иначе записываем объекты на отрисовку в зависимости от процента частоты
						else
						{
							float fPercent = float(CGreen::m_iRenderFreqGrass) / 100.f;
							float fNewCount = ((float)pCurrSegment->m_iCountObj * fPercent);
							float fStep = float(pCurrSegment->m_iCountObj) / fNewCount;
							int iLastCP = 0;
							for (float k = 0; k < pCurrSegment->m_iCountObj; k += fStep)
							{
								int iCurrCP = (int)floor(k + 0.5);
								if (iCurrCP > iLastCP)
								{
									pVerteces[m_iCurrCountDrawObj] = pGreen->m_pAllTrans[pCurrSegment->m_pArrIDs[iCurrCP]];
									m_iCurrCountDrawObj += 1;
									iLastCP = iCurrCP;
								}
							}
						}
					}
				}
			}

			m_pTransVertBuf->Unlock();

			render2(timeDelta, pViewPos, idGreen, iLod, idTex);
		}
	}
}

void CGreen::renderObject(DWORD timeDelta, const float3 *pViewPos, ID idGreen, ID idSplit, ID idObj, ID idTex)
{
	if (idGreen < 0 || m_aGreens.size() <= idGreen || idSplit < 0 || m_aGreens[idGreen]->m_aSplitsArr.size() <= idSplit || idObj < 0 || m_aGreens[idGreen]->m_aSplitsArr[idSplit]->m_iCountObj <= idObj)
		return;

	CGreenDataVertex *pVerteces = 0;
	m_pTransVertBuf->Lock(0, 0, (void**)&pVerteces, D3DLOCK_DISCARD);
	pVerteces[0] = m_aGreens[idGreen]->m_pAllTrans[m_aGreens[idGreen]->m_aSplitsArr[idSplit]->m_pArrIDs[idObj]];
	m_pTransVertBuf->Unlock();
	m_iCurrCountDrawObj = 1;

	render2(timeDelta, pViewPos, idGreen, 0, idTex);
}

//##########################################################################

void CGreen::loadModelForLod(CModel *pGreen, int iLod, const char *szPathModel)
{
	if (pGreen->m_aLods[iLod] == 0)
		pGreen->m_aLods[iLod] = new CLod();
	char szFullPath[1024];
	sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), szPathModel);

	SGCore_StaticModelLoad(szFullPath, &pGreen->m_aLods[iLod]->m_pModel);
	pGreen->m_aLods[iLod]->m_sPath = szPathModel;

	char szTexture[SXGC_LOADTEX_MAX_SIZE_DIRNAME];

	for (int i = 0; i < pGreen->m_aLods[iLod]->m_pModel->m_uiSubsetCount; ++i)
	{
		sprintf(szTexture, "%s.dds", pGreen->m_aLods[iLod]->m_pModel->m_ppTextures[i]);
		pGreen->m_aLods[iLod]->m_aIDsTextures[i] = SGCore_MtlLoad(szTexture, (pGreen->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
	}
}

ID CGreen::generate(const char *szName,
	const char *szTexMask, 
	bool shouldAveragedRGB,
	float fDensity, 
	const char *szPathModel, const char *szPathLod1, const char *szPathLod2,
	const char *szPathNavMesh)
{
	if (!STR_VALIDATE(szPathModel))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "not initialized path for model!!!");
		return -1;
	}

	//! есть ли статическая геометрия на уровне
	if (SGeom_GetCountModels() > 0)
	{
		CModel *pGreen = new CModel();
		sprintf(pGreen->m_szName, szName);
		pGreen->m_szMaskName = szTexMask;

		//! если нет лодов значит тип "трава"
		if (!szPathLod1 && !szPathLod2)
			pGreen->m_typeGreen = GREEN_TYPE_GRASS;
		else
			pGreen->m_typeGreen = GREEN_TYPE_TREE;

		loadModelForLod(pGreen, 0, szPathModel);

		initGreenDataLod0(pGreen);

		if (STR_VALIDATE(szPathLod1))
		{
			if (stricmp(szPathModel, szPathLod1) == 0)
				pGreen->m_aLods[1] = pGreen->m_aLods[0];
			else
			{
				loadModelForLod(pGreen, 1, szPathLod1);
			}
		}

		if (STR_VALIDATE(szPathLod2))
		{
			if (stricmp(szPathModel, szPathLod2) == 0)
				pGreen->m_aLods[2] = pGreen->m_aLods[0];
			else if (stricmp(szPathLod1, szPathLod2) == 0)
				pGreen->m_aLods[2] = pGreen->m_aLods[1];
			else
			{
				loadModelForLod(pGreen, 2, szPathLod2);
			}
		}

		ISXBound *pBound = SGCore_CrBound();
		SGCore_FCompBoundBox(pGreen->m_aLods[0]->m_pModel->m_pVertexBuffer, &pBound, pGreen->m_aLods[0]->m_pModel->m_uiAllVertexCount, sizeof(vertex_static_ex));

		float3 vGreenMax, vGreenMin;
		pBound->getMinMax(&vGreenMin, &vGreenMax);
		mem_release(pBound);

		float3 vLevelMin, vLevelMax;
		SGeom_GetMinMax(&vLevelMin, &vLevelMax);

		if (STR_VALIDATE(szTexMask))
		{
			ID idTexMask = SGCore_LoadTexAddName(szTexMask, LOAD_TEXTURE_TYPE_LOAD);
			SGCore_LoadTexAllLoad();

			if (!genByTex(pGreen, idTexMask, shouldAveragedRGB, &vGreenMin, &vGreenMax, fDensity))
			{
				mem_delete(pGreen);
				return -1;
			}
		}

		preSegmentation(pGreen, &vLevelMin, &vLevelMax);

		setSplitID(pGreen);
		
		m_aGreens.push_back(pGreen);

		if (STR_VALIDATE(szPathNavMesh))
			setGreenNav(m_aGreens.size() - 1, szPathNavMesh);

		addModelInVisCaclObj(m_aGreens.size() - 1);
		return m_aGreens.size() - 1;
	}
	else
	{
		LibReport(REPORT_MSG_LEVEL_WARNING,"not found static geometry in level!!!");
	}

	return -1;
}

bool CGreen::genByTex(CModel *pGreen, ID idMask, bool shouldAveragedRGB, float3 *pGreenMin, float3 *pGreenMax, float fDensity)
{
	float3 vLeveMin, vLeveMax;
	SGeom_GetMinMax(&vLeveMin, &vLeveMax);
	float3 vPosMin = vLeveMin;
	float3 vPosMax = vLeveMax;

	float r2d = 0;

	if ((pGreenMax->x - pGreenMin->x)*0.5f > (pGreenMax->z - pGreenMin->z)*0.5f)
		r2d = (pGreenMax->x - pGreenMin->x)*0.5f;
	else
		r2d = (pGreenMax->z - pGreenMin->z)*0.5f;

	float fWidthLand = vLeveMax.x - vLeveMin.x;
	float fHeightLand = vLeveMax.z - vLeveMin.z;

	D3DSURFACE_DESC desc;
	SGCore_LoadTexGetTex(idMask)->GetLevelDesc(0, &desc);

	if (desc.Format != D3DFMT_A8R8G8B8)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "unresolved format of mask!!!");
		return false;
	}

	D3DLOCKED_RECT oLockedRect;

	SGCore_LoadTexGetTex(idMask)->LockRect(0, &oLockedRect, 0, 0);
	DWORD *pColor = (DWORD*)oLockedRect.pBits;
	DWORD dwColor;

	// количество неуданчых генераций
	int iCountFailGreen = 0;

	// массив сгенерированных позиций
	Array<float3_t, GREEN_DEFAULT_RESERVE_GEN> aPos;

	DWORD MaxAlpha = 0;
	DWORD dwAlpha;
	float fAlpha;
	float fPosInLandX;
	float fPosInLandY;

	//сколько метров на уровне занимает один пиксель из маски
	float fPixelInLandX = fWidthLand / float(desc.Width);
	float fPixelInLandY = fHeightLand / float(desc.Height);

	float3 vGenPos;
	bool isIntersect = false;

	for (int x = 0; x < desc.Width; ++x)
	{
		for (int y = 0; y < desc.Height; ++y)
		{
			dwColor = pColor[y*desc.Width + x];
			if (shouldAveragedRGB)
				dwAlpha = (GetRValue(dwColor) + GetGValue(dwColor) + GetBValue(dwColor)) / 3;
			else
				dwAlpha = (dwColor >> 24);

			fAlpha = 1.f / 255.f * (float)dwAlpha;
			if (dwAlpha > 0)
			{
				//позиция пикселя на ландшафте
				fPosInLandX = lerpf(vLeveMin.x, vLeveMax.x, float(x + 1) / float(desc.Width));
				fPosInLandY = lerpf(vLeveMax.z, vLeveMin.z, float(y + 1) / float(desc.Height));

				//расчет позиций объектов на квадратный метр
				int iCount = int(floor(float(float(dwAlpha)*fDensity) / 255.f)) * fPixelInLandX * fPixelInLandY;
				for (int i = 0; i<iCount; ++i)
				{
					isIntersect = true;

					vGenPos.x = (fPosInLandX - fPixelInLandX * 0.5f) + randf(0.0, fPixelInLandX);
					vGenPos.z = (fPosInLandY - fPixelInLandY * 0.5f) + randf(0.0, fPixelInLandY);
					vGenPos.y = vLeveMax.y;

					// если тип "дерево"
					if (pGreen->m_typeGreen == GREEN_TYPE_TREE)
					{
						// проходим по всему массиву сгенерированных объектов и ищем пересечения с текущей позицией, если есть значит запрещаем генерировать в этом месте
						for (int k = 0; k < aPos.size(); ++k)
						{
							if (SMVector3Length2(vGenPos - float3(aPos[k].x, vGenPos.y, aPos[k].z)) < r2d*r2d)
							{
								dwAlpha = 0;
								isIntersect = false;
								break;
							}
						}
					}

					// если разрешено определение пересечения - определяем
					if (isIntersect)
					{
						vPosMax.x = vPosMin.x = vGenPos.x;
						vPosMax.z = vPosMin.z = vGenPos.z;
						isIntersect = g_fnIntersect(&vPosMax, &vPosMin, &vGenPos);
					}

					// если есть пересечение
					if (isIntersect)
					{
						aPos.push_back(vGenPos);

						//если тип дерево, то на пиксель генерируем только одно дерево
						if (pGreen->m_typeGreen == GREEN_TYPE_TREE)
							break;
					}
					else
					{
						++iCountFailGreen;
					}
				}
			}
		}
	}

	SGCore_LoadTexGetTex(idMask)->UnlockRect(0);

	if (aPos.size() <= 0)
		return false;

	CGreenDataVertex oGreenData;
	pGreen->m_uiCountObj = 0;

	for (int i = 0; i<aPos.size(); ++i)
	{
		genDataObject(&oGreenData, &(aPos[i]));
		addNewObject2Global(pGreen, &oGreenData);
	}
	aPos.clear();

	return true;
}

void CGreen::genDataObject(CGreenDataVertex *pGreenData, const float3_t *pPos)
{
	pGreenData->m_vPosition = *pPos;
	pGreenData->m_vTexCoord.x = 1.f + randf(0.f, GREEN_GEN_RAND_SCALE);
	pGreenData->m_vTexCoord.y = randf(0.f, GREEN_GEN_RAND_ROTATE_Y);
	pGreenData->m_vTexCoord.z = 0;
	pGreenData->m_vSinCosRot.x = sinf(pGreenData->m_vTexCoord.y);
	pGreenData->m_vSinCosRot.y = cosf(pGreenData->m_vTexCoord.y);
}

//##########################################################################

ID CGreen::getIDsplit(ID idGreen, const float3 *pPos)
{
	if (idGreen < 0 || m_aGreens.size() <= idGreen)
		return -1;

	Array<CSegment*> aQueue;
	int iCount = 0;
	aQueue.push_back(m_aGreens[idGreen]->m_pSplitsTree);
	float3 vSplitMin, vSplitMax;
	ID idSplit = -1;

	while (aQueue.size() > iCount)
	{
		aQueue[iCount]->m_pBoundVolumeSys->getMinMax(&vSplitMin, &vSplitMax);
		if (vSplitMax.x >= pPos->x && vSplitMax.z >= pPos->z && vSplitMin.x <= pPos->x && vSplitMin.z <= pPos->z)
		{
			if (!(aQueue[iCount]->m_aSplits[0]))
				idSplit = aQueue[iCount]->m_id;

			for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; i++)
			{
				if (aQueue[iCount]->m_aSplits[i])
					aQueue.push_back(aQueue[iCount]->m_aSplits[i]);
			}
		}

		++iCount;
	}

	return idSplit;
}

//##########################################################################

ID CGreen::addObject(ID id, float3 *pPos, CGreenDataVertex *pGreenData, ID *pIdSplit)
{
	if(id < 0 || (ID)m_aGreens.size() <= id)
		return -1;

	ID idSplit = getIDsplit(id, pPos);

	if (idSplit < 0)
		return -1;

	CModel *pCurrModel = m_aGreens[id];
	CSegment *pCurrSegment = m_aGreens[id]->m_aSplitsArr[idSplit];

	int iOldLen = pCurrSegment->m_iCountObj;
	
	//создаем новый массив с новым размером, и если в прошлом массиве были данные, то копируем их в новый
	ID *aIDs = new ID[iOldLen + 1];
	if (iOldLen > 0)
		memcpy(aIDs, pCurrSegment->m_pArrIDs, iOldLen * sizeof(ID));

	//создаем новый объект растительности, если исходный объект для копирования не передан, значит создаем новый, иначе меняем позицию
	CGreenDataVertex oGreenData;
	if (!pGreenData)
		genDataObject(&oGreenData, &float3_t(pPos->x, pPos->y, pPos->z));
	else
	{
		oGreenData = *pGreenData;
		oGreenData.m_vPosition = *pPos;
	}

	ID idGlobalNew = addNewObject2Global(pCurrModel, &oGreenData);

	//запись нового объекта (его глобального id) в массив сплита, замена старого массива сплита новым
	aIDs[iOldLen] = idGlobalNew;
	mem_delete_a(pCurrSegment->m_pArrIDs);
	pCurrSegment->m_pArrIDs = aIDs;

	++(pCurrSegment->m_iCountObj);

	alignBound(pCurrModel, pCurrSegment);

	if (pIdSplit)
		*pIdSplit = idSplit;

	return iOldLen;
	}

ID CGreen::addNewObject2Global(CModel *pGreen, CGreenDataVertex *pObject)
	{
	//определяем новую позицию объекта в глобальном массиве текущей растительности, если есть объекты к удалению, то берем самый последний, иначе выбираем новую позицию
	ID idGlobalNew = -1;
	if (pGreen->m_aDeleteObj.size() > 0)
	{
		idGlobalNew = pGreen->m_aDeleteObj[pGreen->m_aDeleteObj.size() - 1];
		pGreen->m_aDeleteObj.erase(pGreen->m_aDeleteObj.size() - 1);
	}
	else
		idGlobalNew = pGreen->m_pAllTrans.size();

	pGreen->m_pAllTrans[idGlobalNew] = *pObject;

	//просчет ограничивающего объема объекта и запись в глобальный массив объемов
	CBoundBox oBB;
	oBB.m_vMin = pGreen->m_vMin;
	oBB.m_vMax = pGreen->m_vMax;
	computeBB(*pObject, oBB);
	pGreen->m_aTransW[idGlobalNew] = oBB;

	++(pGreen->m_uiCountObj);
	
	return idGlobalNew;
}

void CGreen::deleteObject(ID id, ID idSplit, ID idObj)
{
	if (id < 0 || m_aGreens.size() <= id || idSplit < 0 || m_aGreens[id]->m_aSplitsArr.size() <= idSplit || idObj < 0 || m_aGreens[id]->m_aSplitsArr[idSplit]->m_iCountObj <= idObj)
		return;

	CModel *pCurrGreen = m_aGreens[id];
	CSegment *pCurrSegment = pCurrGreen->m_aSplitsArr[idSplit];

	int iOldLen = pCurrSegment->m_iCountObj;

	// помещаем удаляемый объект в массив на удаление
	pCurrGreen->m_aDeleteObj.push_back(pCurrSegment->m_pArrIDs[idObj]);

	// создаем новый массив и копируем его данные, старый массив удаляем и заменяем новым
	ID *aIDs = new ID[iOldLen - 1];
	memcpy(aIDs, pCurrSegment->m_pArrIDs, idObj * sizeof(ID));
	memcpy(aIDs + idObj, pCurrSegment->m_pArrIDs + idObj + 1, ((iOldLen - idObj) - 1)* sizeof(ID));
	mem_delete_a(pCurrSegment->m_pArrIDs);
	pCurrSegment->m_pArrIDs = aIDs;

	//декрементируем данные глобального переменной количества этой растительности и локальной переменной текущего сплита
	--(pCurrSegment->m_iCountObj);
	--(pCurrGreen->m_uiCountObj);

	alignBound(pCurrGreen, pCurrSegment);
}

void CGreen::getPosObject(ID id, ID idSplit, ID idObj, float3_t *pPos)
{
	if (!pPos || id < 0 || m_aGreens.size() <= id || idSplit < 0 || m_aGreens[id]->m_aSplitsArr.size() <= idSplit || idObj < 0 || m_aGreens[id]->m_aSplitsArr[idSplit]->m_iCountObj <= idObj)
		return;

	//id объекта в глобальном списке текущей растительности
	ID idObj2G = m_aGreens[id]->m_aSplitsArr[idSplit]->m_pArrIDs[idObj];

	*pPos = m_aGreens[id]->m_pAllTrans[idObj2G].m_vPosition;
}

void CGreen::setPosObject(ID id, ID *pIdSplit, ID *pIdObj, const float3_t *pPos)
{
	if (!pPos || !pIdSplit || !pIdObj || id < 0 || m_aGreens.size() <= id || (*pIdSplit) < 0 || m_aGreens[id]->m_aSplitsArr.size() <= (*pIdSplit) || (*pIdObj) < 0 || m_aGreens[id]->m_aSplitsArr[(*pIdSplit)]->m_iCountObj <= (*pIdObj))
		return;

	CModel *pCurrGreen = m_aGreens[id];

	//id объекта в глобальном списке текущей растительности
	ID idObj2G = pCurrGreen->m_aSplitsArr[(*pIdSplit)]->m_pArrIDs[(*pIdObj)];

	// если сплит позиции равен текущему сплиту  объекта, тогда просто перемещаем внутри, меняя только позицию
	if (getIDsplit(id, &float3(*pPos)) == (*pIdSplit))
		pCurrGreen->m_pAllTrans[idObj2G].m_vPosition = *pPos;
	else
	{
		CGreenDataVertex oGreenData = pCurrGreen->m_pAllTrans[idObj2G];
		deleteObject(id, (*pIdSplit), (*pIdObj));
		(*pIdObj) = addObject(id, &float3(*pPos), &oGreenData, pIdSplit);
	}
}

//##########################################################################

bool DelDelCompareFunc(ID id1, ID id2)
{
	return (id1 < id2);
}

void CGreen::deleteDeleted()
{
	// проходим по всему массиву растительности
	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		CModel *pGreen = m_aGreens[i];

		// если нет объектов для удаления то пропускаем
		if (pGreen->m_aDeleteObj.size() == 0)
			continue;

		// сортируем массив по возрастанию идентификаторов
		pGreen->m_aDeleteObj.quickSort(DelDelCompareFunc);

		// удаляем из общих массивов удаленные данные
		for (int k = 0; k < pGreen->m_aDeleteObj.size(); ++k)
		{
			int iKey = (pGreen->m_aDeleteObj.size() - k) - 1;
			pGreen->m_pAllTrans.erase(pGreen->m_aDeleteObj[iKey]);
			pGreen->m_aTransW.erase(pGreen->m_aDeleteObj[iKey]);
		}


		// проходимся по каждому сплиту и все сдвигаем

		Array<CSegment*> aQueue;
		int iCountComSplit = 0;
		aQueue.push_back(pGreen->m_pSplitsTree);

		while (aQueue.size() > iCountComSplit)
		{
			CSegment *pCurrSegment = aQueue[iCountComSplit];
			// данные хранятся только в конечных сплитах, поэтому вычленяем их
			if (!(pCurrSegment->m_isFinite))
			{
				for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
				{
					if (pCurrSegment->m_aSplits[i])
						aQueue.push_back(pCurrSegment->m_aSplits[i]);
				}
			}
			else
			{
				if (pCurrSegment->m_iCountObj <= 0)
					continue;

				// вычисляем новый разер (количество объектов в сплите) на основании количества удаляемых из него id
				int iNewSize = pCurrSegment->m_iCountObj;
				for (int k = 0; k < pCurrSegment->m_iCountObj; ++k)
				{
					for (int j = 0; j < pGreen->m_aDeleteObj.size(); ++j)
					{
						if (pCurrSegment->m_pArrIDs[k] == pGreen->m_aDeleteObj[j])
							--iNewSize;
					}
				}

				// если в сплите остаются объекты, но есть объекты к удалению
				if (iNewSize > 0 && iNewSize < pCurrSegment->m_iCountObj)
				{
					ID *pArrIDs = new ID[iNewSize];

					for (int k = 0; k < iNewSize; ++k)
						pArrIDs[k] = -1;

					int iCurrKey = 0;

					// проходим по массиву идентификаторов в сплите
					for (int k = 0; k < pCurrSegment->m_iCountObj; ++k)
					{
						// создаем переменную и записываем в нее текущий индентификатор
						ID idNewID = pCurrSegment->m_pArrIDs[k];

						// проходимся по массиву удаляемых объектов
						for (int j = 0; j < pGreen->m_aDeleteObj.size(); ++j)
						{
							// если текущий идентификатор объекта в сплите не равен удаляемому идентификатору
							if (pCurrSegment->m_pArrIDs[k] != pGreen->m_aDeleteObj[j])
							{
								// если текущий идентификатор объекта в сплите больше удаляемого идентификатора то просто декрементим его
								if (pCurrSegment->m_pArrIDs[k] > pGreen->m_aDeleteObj[j])
								{
									--idNewID;

									if (idNewID < 0)
										int qwerty = 0;
								}
							}
							// иначе это удаляемый объект
							else
							{
								idNewID = -1;
								break;
							}
						}

						// если новый идентификатор объекта валиден то записываем его
						if (idNewID > -1)
						{
							pArrIDs[iCurrKey] = idNewID;
							++iCurrKey;
						}
					}

					/*for (int k = 0; k < iNewSize; ++k)
					{
						if (pArrIDs[k] < 0)
							int qwerty = 0;
					}*/

					pCurrSegment->m_iCountObj = iNewSize;
					mem_delete_a(pCurrSegment->m_pArrIDs);
					pCurrSegment->m_pArrIDs = pArrIDs;
				}
				// иначе если в сплите не остается объектов то удаляем инфу об объектах в сплите
				else if (iNewSize <= 0)
				{
					pCurrSegment->m_iCountObj = 0;
					mem_delete_a(pCurrSegment->m_pArrIDs);
				}				
			}

			++iCountComSplit;
		}

		// очищаем массив данных для удаления
		pGreen->m_aDeleteObj.clear();
	}
}

void CGreen::save(const char *szPath)
{
	deleteDeleted();

	FILE *pFile = fopen(szPath, "wb");

	fwrite(SX_GREEN_MAGIC_WORD, sizeof(char)* strlen(SX_GREEN_MAGIC_WORD), 1, pFile);
	uint32_t uiFmtVersion = SX_GREEN_FILE_FORMAT_VERSION;
	fwrite(&uiFmtVersion, sizeof(uint32_t), 1, pFile);

	int32_t iCountModel = m_aGreens.size();
	fwrite(&iCountModel, sizeof(int32_t), 1, pFile);

	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		CModel *pGreen = m_aGreens[i];

		fwrite(&pGreen->m_typeGreen, sizeof(int32_t), 1, pFile);

		int32_t iStrlen = strlen(pGreen->m_szName);
		fwrite(&iStrlen, sizeof(int32_t), 1, pFile);
		fwrite(pGreen->m_szName, sizeof(char), iStrlen, pFile);

		iStrlen = pGreen->m_szMaskName.length();
		fwrite(&iStrlen, sizeof(int32_t), 1, pFile);
		fwrite(pGreen->m_szMaskName.c_str(), sizeof(char), iStrlen, pFile);

		if (pGreen->m_pPhysMesh)
		{
			iStrlen = strlen(pGreen->m_pPhysMesh->m_sPathName.c_str());
			fwrite(&iStrlen, sizeof(int32_t), 1, pFile);
			fwrite(pGreen->m_pPhysMesh->m_sPathName.c_str(), sizeof(char), iStrlen, pFile);
		}
		else
		{
			iStrlen = 0;
			fwrite(&iStrlen, sizeof(int32_t), 1, pFile);
		}

		if (pGreen->m_typeGreen == GREEN_TYPE_GRASS)
		{
			iStrlen = strlen(pGreen->m_aLods[0]->m_sPath.c_str());
			fwrite(&iStrlen, sizeof(int32_t), 1, pFile);
			fwrite(pGreen->m_aLods[0]->m_sPath.c_str(), sizeof(char), iStrlen, pFile);
		}
		else
		{
			for (int k = 0; k < GREEN_COUNT_LOD; ++k)
			{
				iStrlen = pGreen->m_aLods[k]->m_sPath.length();
				fwrite(&iStrlen, sizeof(int32_t), 1, pFile);
				fwrite(pGreen->m_aLods[k]->m_sPath.c_str(), sizeof(char), iStrlen, pFile);
			}
		}
		
		fwrite(&pGreen->m_vMin.x, sizeof(float), 1, pFile);
		fwrite(&pGreen->m_vMin.y, sizeof(float), 1, pFile);
		fwrite(&pGreen->m_vMin.z, sizeof(float), 1, pFile);

		fwrite(&pGreen->m_vMax.x, sizeof(float), 1, pFile);
		fwrite(&pGreen->m_vMax.y, sizeof(float), 1, pFile);
		fwrite(&pGreen->m_vMax.z, sizeof(float), 1, pFile);

		fwrite(&pGreen->m_uiCountObj, sizeof(uint32_t), 1, pFile);
		fwrite(&(pGreen->m_pAllTrans[0]), sizeof(CGreenDataVertex), pGreen->m_uiCountObj, pFile);

		uint32_t uiCountBytesAllSplits = 0;
		Array<CSegment*> aQueue;
		uint32_t uiCountSplits = 0;
		aQueue.push_back(pGreen->m_pSplitsTree);

		while (aQueue.size() > uiCountSplits)
		{
			uiCountBytesAllSplits += getCountBytes4SaveSplit(aQueue[uiCountSplits], pFile, &aQueue);
			++uiCountSplits;
		}

		fwrite(&uiCountBytesAllSplits, sizeof(uint32_t), 1, pFile);
		fwrite(&uiCountSplits, sizeof(uint32_t), 1, pFile);

		aQueue.clearFast();
		uiCountSplits = 0;
		aQueue.push_back(pGreen->m_pSplitsTree);

		while (aQueue.size() > uiCountSplits)
		{
			saveSplit(aQueue[uiCountSplits], pFile, &aQueue);
			++uiCountSplits;
		}
	}

	fclose(pFile);
}

void CGreen::saveSplit(const CSegment *pSplit, FILE *pFile, Array<CSegment*> *pQueue)
{
	float3 vMin, vMax;
	pSplit->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);

	fwrite(&vMin.x, sizeof(float), 1, pFile);
	fwrite(&vMin.y, sizeof(float), 1, pFile);
	fwrite(&vMin.z, sizeof(float), 1, pFile);

	fwrite(&vMax.x, sizeof(float), 1, pFile);
	fwrite(&vMax.y, sizeof(float), 1, pFile);
	fwrite(&vMax.z, sizeof(float), 1, pFile);

	pSplit->m_pBoundVolumeP->getMinMax(&vMin, &vMax);
	fwrite(&vMin.x, sizeof(float), 1, pFile);
	fwrite(&vMin.y, sizeof(float), 1, pFile);
	fwrite(&vMin.z, sizeof(float), 1, pFile);

	fwrite(&vMax.x, sizeof(float), 1, pFile);
	fwrite(&vMax.y, sizeof(float), 1, pFile);
	fwrite(&vMax.z, sizeof(float), 1, pFile);

	fwrite(&pSplit->m_iCountObj, sizeof(uint32_t), 1, pFile);

	fwrite(&pSplit->m_isFinite, sizeof(int8_t), 1, pFile);

	if (!(pSplit->m_isFinite))
	{
		for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		{
			if (pSplit->m_aSplits[i])
				pQueue->push_back(pSplit->m_aSplits[i]);
		}
	}
	else
	{
		if (pSplit->m_iCountObj > 0)
			fwrite(pSplit->m_pArrIDs, sizeof(ID), pSplit->m_iCountObj, pFile);
	}
}

uint32_t CGreen::getCountBytes4SaveSplit(const CSegment *pSplit, FILE *pFile, Array<CSegment*> *pQueue)
{
	uint32_t uiCountBytes = (sizeof(float3_t)* 4) + sizeof(uint32_t)+sizeof(int8_t);

	if (!(pSplit->m_isFinite))
	{
		for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		{
			if (pSplit->m_aSplits[i])
				pQueue->push_back(pSplit->m_aSplits[i]);
		}
	}
	else
	{
		uiCountBytes += sizeof(ID)* pSplit->m_iCountObj;
	}

	return uiCountBytes;
}

//**************************************************************************

bool CGreen::load(const char *szPath)
{
	FILE *pFile = fopen(szPath, "rb");

	uint64_t ui64Magic;
	fread(&ui64Magic, sizeof(uint64_t), 1, pFile);

	if (ui64Magic != SX_GREEN_MAGIC_NUM)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "file [%s] is not green\n", szPath);
		fclose(pFile);
		return false;
	}

	uint32_t uiFmtVersion;
	fread(&uiFmtVersion, sizeof(uint32_t), 1, pFile);

	if (uiFmtVersion != SX_GREEN_FILE_FORMAT_VERSION)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "file [%s] have unduported version %d\n", szPath, uiFmtVersion);
		fclose(pFile);
		return false;
	}

	char aStr[3][1024];
	aStr[0][0] = 0;
	aStr[1][0] = 0;
	aStr[2][0] = 0;
	char szPath2[1024];
	char szStr[1024];

	int32_t iCountModel;
	fread(&iCountModel, sizeof(int32_t), 1, pFile);

	for (int i = 0; i < iCountModel; ++i)
	{
		aStr[0][0] = aStr[1][0] = aStr[2][0] = 0;
		
		CModel *pGreen = new CModel();
		fread(&pGreen->m_typeGreen, sizeof(int32_t), 1, pFile);

		int32_t iStrlen;
		fread(&iStrlen, sizeof(int32_t), 1, pFile);
		fread(szStr, sizeof(char), iStrlen, pFile);
		szStr[iStrlen] = 0;
		sprintf(pGreen->m_szName, "%s", szStr);

		fread(&iStrlen, sizeof(int32_t), 1, pFile);
		fread(szStr, sizeof(char), iStrlen, pFile);
		szStr[iStrlen] = 0;
		pGreen->m_szMaskName = szStr;

		fread(&iStrlen, sizeof(int32_t), 1, pFile);
		if (iStrlen > 0)
		{
			fread(szStr, sizeof(char), iStrlen, pFile);
			szStr[iStrlen] = 0;
			setGreenNav2(pGreen, szStr);
			/*pGreen->m_pPhysMesh = new CModel::CPhysMesh();
			pGreen->m_pPhysMesh->m_sPathName = szStr;*/
		}

		if (pGreen->m_typeGreen == GREEN_TYPE_GRASS)
		{
			fread(&iStrlen, sizeof(int32_t), 1, pFile);
			fread(aStr[0], sizeof(char), iStrlen, pFile);
			aStr[0][iStrlen] = 0;
		}
		else
		{
			for (int k = 0; k < GREEN_COUNT_LOD; ++k)
			{
				fread(&iStrlen, sizeof(int32_t), 1, pFile);
				fread(aStr[k], sizeof(char), iStrlen, pFile);
				aStr[k][iStrlen] = 0;
			}
		}

		pGreen->m_aLods[0] = new CLod();
		pGreen->m_aLods[1] = 0;
		pGreen->m_aLods[2] = 0;

		sprintf(szPath2, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), aStr[0]);
		SGCore_StaticModelLoad(szPath2, &pGreen->m_aLods[0]->m_pModel);
		pGreen->m_aLods[0]->m_sPath = aStr[0];
		
		for (int k = 0; k < pGreen->m_aLods[0]->m_pModel->m_uiSubsetCount; ++k)
		{
			sprintf(szPath2, "%s.dds", pGreen->m_aLods[0]->m_pModel->m_ppTextures[k]);
			pGreen->m_aLods[0]->m_aIDsTextures[k] = SGCore_MtlLoad(szPath2, (pGreen->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
		}

		initGreenDataLod0(pGreen);

		for (int k = 1; k < GREEN_COUNT_LOD; ++k)
		{
			if (aStr[k][0])
			{
				if (stricmp(aStr[k-1], aStr[k]) == 0)
					pGreen->m_aLods[k] = pGreen->m_aLods[k-1];
				else
				{
					pGreen->m_aLods[k] = new CLod();
					pGreen->m_aLods[k]->m_sPath = aStr[k];
					sprintf(szPath2, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), aStr[k]);
					SGCore_StaticModelLoad(szPath2, &pGreen->m_aLods[1]->m_pModel);

					for (int j = 0; j < pGreen->m_aLods[k]->m_pModel->m_uiSubsetCount; ++j)
					{
						sprintf(szPath2, "%s.dds", pGreen->m_aLods[k]->m_pModel->m_ppTextures[j]);
						pGreen->m_aLods[k]->m_aIDsTextures[j] = SGCore_MtlLoad(szPath2, (pGreen->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
					}
				}
			}
		}

		fread(&pGreen->m_vMin.x, sizeof(float), 1, pFile);
		fread(&pGreen->m_vMin.y, sizeof(float), 1, pFile);
		fread(&pGreen->m_vMin.z, sizeof(float), 1, pFile);

		fread(&pGreen->m_vMax.x, sizeof(float), 1, pFile);
		fread(&pGreen->m_vMax.y, sizeof(float), 1, pFile);
		fread(&pGreen->m_vMax.z, sizeof(float), 1, pFile);


		CGreenDataVertex oDataVertex;

		int iCountObject = 0;
		fread(&iCountObject, sizeof(uint32_t), 1, pFile);

		for (int k = 0; k < iCountObject; ++k)
		{
			fread(&oDataVertex, sizeof(CGreenDataVertex), 1, pFile);
			addNewObject2Global(pGreen, &oDataVertex);
		}

		uint32_t uiCountBytesAllSplits;
		uint32_t uiCountSplits;

		fread(&uiCountBytesAllSplits, sizeof(uint32_t), 1, pFile);
		fread(&uiCountSplits, sizeof(uint32_t), 1, pFile);

		long lCurrPos = ftell(pFile);
		fseek(pFile, 0, SEEK_END);
		long lLastSize = ftell(pFile) - lCurrPos;

		if (lLastSize < uiCountBytesAllSplits)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "file [%s] damaged, lacks %d bytes \n", szPath, uiCountBytesAllSplits - lLastSize);
			fclose(pFile);
			return false;
		}

		fseek(pFile, lCurrPos, SEEK_SET);

		Array<CSegment**> aQueue;
		int iCount = 0;
		aQueue.push_back(&(pGreen->m_pSplitsTree));

		while (aQueue.size() > iCount)
		{
			loadSplit(aQueue[iCount], pFile, &aQueue);
			++iCount;
		}

		setSplitID(pGreen);

		m_aGreens.push_back(pGreen);

		addModelInVisCaclObj(m_aGreens.size() - 1);
	}

	fclose(pFile);

	return true;
}

void CGreen::loadSplit(CSegment **ppSplit, FILE *pFile, Array<CSegment**> *pQueue)
{
	(*ppSplit) = new CSegment();
	float3 vMin, vMax;
	fread(&vMin.x, sizeof(float), 1, pFile);
	fread(&vMin.y, sizeof(float), 1, pFile);
	fread(&vMin.z, sizeof(float), 1, pFile);

	fread(&vMax.x, sizeof(float), 1, pFile);
	fread(&vMax.y, sizeof(float), 1, pFile);
	fread(&vMax.z, sizeof(float), 1, pFile);

	(*ppSplit)->m_pBoundVolumeSys = SGCore_CrBound();
	(*ppSplit)->m_pBoundVolumeSys->setMinMax(&vMin, &vMax);


	fread(&vMin.x, sizeof(float), 1, pFile);
	fread(&vMin.y, sizeof(float), 1, pFile);
	fread(&vMin.z, sizeof(float), 1, pFile);

	fread(&vMax.x, sizeof(float), 1, pFile);
	fread(&vMax.y, sizeof(float), 1, pFile);
	fread(&vMax.z, sizeof(float), 1, pFile);

	(*ppSplit)->m_pBoundVolumeP = SGCore_CrBound();
	(*ppSplit)->m_pBoundVolumeP->setMinMax(&vMin, &vMax);

	fread(&(*ppSplit)->m_iCountObj, sizeof(uint32_t), 1, pFile);

	fread(&(*ppSplit)->m_isFinite, sizeof(int8_t), 1, pFile);

	if (!((*ppSplit)->m_isFinite))
	{
		for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		{
			pQueue->push_back(&((*ppSplit)->m_aSplits[i]));
		}
	}
	else
	{
		if ((*ppSplit)->m_iCountObj > 0)
		{
			(*ppSplit)->m_pArrIDs = new ID[(*ppSplit)->m_iCountObj];
			fread((*ppSplit)->m_pArrIDs, sizeof(ID)*(*ppSplit)->m_iCountObj, 1, pFile);
		}
	}
}

//##########################################################################

ID CGreen::addVisCaclObj()
{
	CVisCaclObj *pVisCaclObj = new CVisCaclObj();

	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		CModel *pGreen = m_aGreens[i];

		CVisInfo *pVisInfo = new CVisInfo();
		pVisInfo->m_iCount = pGreen->m_idCountSplits;
		pVisInfo->m_ppSegments = new CSegment*[pGreen->m_idCountSplits];
		pVisInfo->m_iCountCom = 0;
		
		pVisInfo->m_aVisible.reserve(pGreen->m_uiCountObj);
		for (int k = 0; k < pGreen->m_uiCountObj; ++k)
			pVisInfo->m_aVisible[k] = false;

		pVisCaclObj->m_aVisibleInfo.push_back(pVisInfo);
	}

	ID idVisCaclObj = -1;
	for (int i = 0; i < m_aVisCaclObj.size(); ++i)
	{
		if (m_aVisCaclObj[i] == 0)
		{
			m_aVisCaclObj[i] = pVisCaclObj;
			idVisCaclObj = i;
			break;
		}
	}

	if (idVisCaclObj == -1)
	{
		m_aVisCaclObj.push_back(pVisCaclObj);
		idVisCaclObj = m_aVisCaclObj.size() - 1;
	}

	return idVisCaclObj;
}

bool CGreen::existsVisCaclObj(ID idVisCaclObj)
{
	return (m_aVisCaclObj.size() > idVisCaclObj);
}

void CGreen::deleteVisCaclObj(ID idVisCaclObj)
{
	GREEN_PRECOND_IDVISCALCOBJ_ERR(idVisCaclObj);

	mem_delete(m_aVisCaclObj[idVisCaclObj]);
}

//**************************************************************************

void CGreen::addModelInVisCaclObj(ID idGreen)
{
	GREEN_PRECOND_IDGREEN_ERR(idGreen);
	
	CModel *Green = m_aGreens[idGreen];
	
	for (int i = 0; i < m_aVisCaclObj.size(); ++i)
	{
		CVisCaclObj *pVisCalcObj = m_aVisCaclObj[i];

		CVisInfo *pVisInfo = new CVisInfo();
		pVisInfo->m_iCount = Green->m_idCountSplits;
		pVisInfo->m_ppSegments = new CSegment*[Green->m_idCountSplits];
		pVisInfo->m_iCountCom = 0;
		pVisCalcObj->m_aVisibleInfo[idGreen] = pVisInfo;
		pVisCalcObj->m_aVisibleInfo[idGreen]->m_aVisible.resize(Green->m_pAllTrans.size());
	}
}

void CGreen::deleteModelInVisCaclObj(ID idGreen)
{
	GREEN_PRECOND_IDGREEN_ERR(idGreen);

	for (int i = 0; i < m_aVisCaclObj.size(); ++i)
	{
		if (!(m_aVisCaclObj[i]))
			continue;

		mem_delete(m_aVisCaclObj[i]->m_aVisibleInfo[idGreen]);
		m_aVisCaclObj[i]->m_aVisibleInfo.erase(idGreen);
	}
}

//##########################################################################

ID CGreen::getCountGreen()
{
	return m_aGreens.size();
}

char* CGreen::getGreenName(ID idGreen)
{
	if (idGreen < m_aGreens.size())
		return m_aGreens[idGreen]->m_szName;

	return 0;
}

int CGreen::getGreenCountGen(ID idGreen)
{
	if (idGreen < m_aGreens.size())
		return m_aGreens[idGreen]->m_uiCountObj;

	return -1;
}

int CGreen::getGreenCountPoly(ID idGreen)
{
	if (idGreen < m_aGreens.size() && m_aGreens[idGreen]->m_aLods[0])
		return m_aGreens[idGreen]->m_aLods[0]->m_pModel->m_uiAllIndexCount / 3;

	return -1;
}

int CGreen::getGreenTypeCountGen(ID idGreen)
{
	if (idGreen < m_aGreens.size())
		return m_aGreens[idGreen]->m_typeGreen;

	return -1;
}

const char* CGreen::getGreenModel(ID idGreen)
{
	if (idGreen < m_aGreens.size())
		return m_aGreens[idGreen]->m_aLods[0]->m_sPath.c_str();

	return 0;
}

const char* CGreen::getGreenLod1(ID idGreen)
{
	if (idGreen < m_aGreens.size() && m_aGreens[idGreen]->m_aLods[1])
		return m_aGreens[idGreen]->m_aLods[1]->m_sPath.c_str();

	return 0;
}

const char* CGreen::getGreenLod2(ID idGreen)
{
	if (idGreen < m_aGreens.size() && m_aGreens[idGreen]->m_aLods[2])
		return m_aGreens[idGreen]->m_aLods[2]->m_sPath.c_str();

	return 0;
}

const char* CGreen::getGreenMask(ID id)
{
	if (id < m_aGreens.size())
		return m_aGreens[id]->m_szMaskName.c_str();

	return 0;
}

const char* CGreen::getGreenNav(ID idGreen)
{
	if (idGreen < m_aGreens.size() && m_aGreens[idGreen]->m_pPhysMesh)
		return m_aGreens[idGreen]->m_pPhysMesh->m_sPathName.c_str();

	return 0;
}

void CGreen::setGreenLod(ID idGreen, int iLod, const char *szPathName)
{
	if (!(iLod >= 0 && iLod < GREEN_COUNT_LOD && idGreen >= 0 && idGreen < m_aGreens.size()))
		return;

	CModel *pGreen = m_aGreens[idGreen];
	CLod *pCurrLod = pGreen->m_aLods[iLod];

	bool isUnic = true;

	for (int i = 0; i < GREEN_COUNT_LOD; ++i)
	{
		if (pGreen->m_aLods[i] == pCurrLod)
			isUnic = false;
	}

	if (isUnic)
		mem_delete(pCurrLod);

	char szFullPath[1024];
	pCurrLod = new CLod();
	pCurrLod->m_sPath = szPathName;
	sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), szPathName);
	SGCore_StaticModelLoad(szFullPath, &pCurrLod->m_pModel);

	for (int k = 0; k < pCurrLod->m_pModel->m_uiSubsetCount; ++k)
	{
		sprintf(szFullPath, "%s.dds", pCurrLod->m_pModel->m_ppTextures[k]);
		pCurrLod->m_aIDsTextures[k] = SGCore_MtlLoad(szFullPath, (pGreen->m_typeGreen == GREEN_TYPE_TREE ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
	}
}

void CGreen::setGreenNav2(CModel *pGreen, const char *szPathName)
{
	mem_delete(pGreen->m_pPhysMesh);
	pGreen->m_pPhysMesh = new CModel::CPhysMesh();
	pGreen->m_pPhysMesh->m_sPathName = szPathName;

	char szFullPath[1024];
	sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), szPathName);

	ISXDataStaticModel *pStatiModel = 0;
	SGCore_StaticModelLoad(szFullPath, &pStatiModel);

	//pGreen->m_pPhysMesh->m_iCountVertex = pStatiModel->m_uiAllVertexCount;
	//pGreen->m_pPhysMesh->m_iCountIndex = pStatiModel->m_uiAllIndexCount;
	//pGreen->m_pPhysMesh->m_iSizeArrMtl = pStatiModel->m_uiAllIndexCount;

	//pGreen->m_pPhysMesh->m_pArrVertex = new float3_t[pStatiModel->m_uiAllVertexCount];
	pGreen->m_pPhysMesh->m_aVertex.resize(pStatiModel->m_uiAllVertexCount);
	vertex_static_ex *pVert;
	pStatiModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVert, 0);
	for (int i = 0; i < pStatiModel->m_uiAllVertexCount; ++i)
	{
		pGreen->m_pPhysMesh->m_aVertex[i] = pVert[i].Pos;
	}
	pStatiModel->m_pVertexBuffer->Unlock();

	//pGreen->m_pPhysMesh->m_pArrIndex = new uint32_t[pStatiModel->m_uiAllIndexCount];
	//pGreen->m_pPhysMesh->m_pArrMtl = new ID[pStatiModel->m_uiAllIndexCount];

	pGreen->m_pPhysMesh->m_aIndex.resize(pStatiModel->m_uiAllIndexCount);
	pGreen->m_pPhysMesh->m_aMtrl.resize(pStatiModel->m_uiAllIndexCount);

	UINT *pInd;
	pStatiModel->m_pIndexBuffer->Lock(0, 0, (void **)&pInd, 0);
	
	UINT uiPrebias = 0;
	int iCountIndex = 0;
	char szTexName[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	for (int i = 0; i < pStatiModel->m_uiSubsetCount; ++i)
	{
		sprintf(szTexName, "%s.dds", pStatiModel->m_ppTextures[i]);
		ID idMtrl = SGCore_MtlLoad(szTexName, MTL_TYPE_TREE);
		for (int k = 0; k < pStatiModel->m_pIndexCount[i]; ++k)
		{
			pGreen->m_pPhysMesh->m_aIndex[iCountIndex] = pInd[pStatiModel->m_pStartIndex[i] + k] /*+ uiPrebias*/;
			pGreen->m_pPhysMesh->m_aMtrl[iCountIndex] = idMtrl;
			++iCountIndex;
		}
		uiPrebias += pStatiModel->m_pIndexCount[i];
	}
	pStatiModel->m_pIndexBuffer->Unlock();
	mem_release_del(pStatiModel);
}

void CGreen::initGreenDataLod0(CModel *pGreen)
{
	mem_delete(pGreen->m_pDataLod0);
	pGreen->m_pDataLod0 = new CModel::CPhysMesh();
	pGreen->m_pDataLod0->m_sPathName = "";

	pGreen->m_pDataLod0->m_aVertex.resize(pGreen->m_aLods[0]->m_pModel->m_uiAllVertexCount);
	vertex_static_ex *pVert;
	pGreen->m_aLods[0]->m_pModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVert, 0);
	for (int i = 0; i < pGreen->m_aLods[0]->m_pModel->m_uiAllVertexCount; ++i)
	{
		pGreen->m_pDataLod0->m_aVertex[i] = pVert[i].Pos;
	}
	pGreen->m_aLods[0]->m_pModel->m_pVertexBuffer->Unlock();

	pGreen->m_pDataLod0->m_aIndex.resize(pGreen->m_aLods[0]->m_pModel->m_uiAllIndexCount);
	pGreen->m_pDataLod0->m_aMtrl.resize(pGreen->m_aLods[0]->m_pModel->m_uiSubsetCount);
	pGreen->m_pDataLod0->m_aIndexCount.resize(pGreen->m_aLods[0]->m_pModel->m_uiSubsetCount);

	UINT *pInd;
	pGreen->m_aLods[0]->m_pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pInd, 0);

	UINT uiPrebias = 0;
	int iCountIndex = 0;
	char szTexName[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	for (int i = 0; i < pGreen->m_aLods[0]->m_pModel->m_uiSubsetCount; ++i)
	{
		sprintf(szTexName, "%s.dds", pGreen->m_aLods[0]->m_pModel->m_ppTextures[i]);
		ID idMtrl = SGCore_MtlLoad(szTexName, MTL_TYPE_TREE);
		pGreen->m_pDataLod0->m_aMtrl[i] = idMtrl;
		pGreen->m_pDataLod0->m_aIndexCount[i] = pGreen->m_aLods[0]->m_pModel->m_pIndexCount[i];
		for (int k = 0; k < pGreen->m_aLods[0]->m_pModel->m_pIndexCount[i]; ++k)
		{
			pGreen->m_pDataLod0->m_aIndex[iCountIndex] = pInd[pGreen->m_aLods[0]->m_pModel->m_pStartIndex[i] + k] /*+ uiPrebias*/;
			++iCountIndex;
		}
		uiPrebias += pGreen->m_aLods[0]->m_pModel->m_pIndexCount[i];
	}
	pGreen->m_aLods[0]->m_pModel->m_pIndexBuffer->Unlock();
}

void CGreen::setGreenNav(ID idGreen, const char *szPathName)
{
	if (!(idGreen >= 0 && idGreen < m_aGreens.size()))
		return;

	setGreenNav2(m_aGreens[idGreen], szPathName);
}

//##########################################################################

void CGreen::deleteGreen(ID idGreen)
{
	if (idGreen >= 0 && idGreen < m_aGreens.size())
	{
		deleteModelInVisCaclObj(idGreen);
		mem_delete(m_aGreens[idGreen]);
		m_aGreens.erase(idGreen);
	}
}

//##########################################################################

void CGreen::clear()
{
	while(m_aGreens.size() > 0)
	{
		deleteModelInVisCaclObj(0);
		mem_delete(m_aGreens[0]);
		m_aGreens.erase(0);
	}

	while (m_aVisCaclObj.size() > 2)
	{
		mem_delete(m_aVisCaclObj[2]);
		m_aVisCaclObj.erase(2);
	}
}

//##########################################################################

void CGreen::getNavMeshAndTransform(float3_t ***pppArrVertex, int32_t **ppArrCountVertex, uint32_t ***pppArrIndex, ID ***pppArrMtrl, int32_t **ppArrCountIndex, CGreenDataVertex ***pppArrTransform, int32_t **ppArrCountTransform, int32_t *pCountGreen)
{
	if (m_aGreens.size() <= 0)
		return;

	int iCountGreen = 0;

	for (int id = 0; id < m_aGreens.size(); ++id)
	{
		if (!(m_aGreens[id]->m_pPhysMesh))
			continue;

		if (m_aGreens[id]->m_pAllTrans.size() > 0)
			++iCountGreen;
	}

	(*pCountGreen) = iCountGreen;

	if (iCountGreen <= 0)
		return;

	(*pppArrVertex) = new float3_t*[iCountGreen];
	(*ppArrCountVertex) = new int32_t[iCountGreen];

	(*pppArrIndex) = new uint32_t*[iCountGreen];
	(*pppArrMtrl) = new ID*[iCountGreen];
	(*ppArrCountIndex) = new int32_t[iCountGreen];

	(*pppArrTransform) = new CGreenDataVertex*[iCountGreen];
	(*ppArrCountTransform) = new int32_t[iCountGreen];

	int iCurrGreen = 0;

	for (int i = 0; i < m_aGreens.size(); ++i)
	{
		if (!(m_aGreens[i]->m_pPhysMesh))
			continue;

		CModel *pGreen = m_aGreens[i];

		(*pppArrVertex)[iCurrGreen] = new float3_t[pGreen->m_pPhysMesh->m_aVertex.size()];
		memcpy((*pppArrVertex)[iCurrGreen], &(pGreen->m_pPhysMesh->m_aVertex[0]), sizeof(float3_t)* pGreen->m_pPhysMesh->m_aVertex.size());
		(*ppArrCountVertex)[iCurrGreen] = pGreen->m_pPhysMesh->m_aVertex.size();

		(*pppArrIndex)[iCurrGreen] = new uint32_t[pGreen->m_pPhysMesh->m_aIndex.size()];
		memcpy((*pppArrIndex)[iCurrGreen], &(pGreen->m_pPhysMesh->m_aIndex[0]), sizeof(uint32_t)* pGreen->m_pPhysMesh->m_aIndex.size());
		(*pppArrMtrl)[iCurrGreen] = new ID[pGreen->m_pPhysMesh->m_aIndex.size()];
		memcpy((*pppArrMtrl)[iCurrGreen], &(pGreen->m_pPhysMesh->m_aMtrl[0]), sizeof(ID)* pGreen->m_pPhysMesh->m_aIndex.size());
		(*ppArrCountIndex)[iCurrGreen] = pGreen->m_pPhysMesh->m_aIndex.size();

		(*pppArrTransform)[iCurrGreen] = new CGreenDataVertex[pGreen->m_uiCountObj];
		(*ppArrCountTransform)[iCurrGreen] = pGreen->m_uiCountObj;
		for (int k = 0; k < pGreen->m_uiCountObj; ++k)
		{
			(*pppArrTransform)[iCurrGreen][k] = pGreen->m_pAllTrans[k];// SMMatrixScaling(float3(pGreen->m_pAllTrans[k].TexCoord.x, pGreen->m_pAllTrans[k].TexCoord.x, pGreen->m_pAllTrans[k].TexCoord.x)) * SMMatrixRotationY(pGreen->m_pAllTrans[k].TexCoord.y) * SMMatrixTranslation(pGreen->m_pAllTrans[k].Position);
		}

		++iCurrGreen;
	}
}

//##########################################################################

void CGreen::getPartBeam(const float3 *pPos, const float3 *pDir, CSegment **ppArrSplits, int *pCount, CSegment *pCurrSplit, int iCountCom)
{
	Array<CSegment*> aQueue;
	int iCount = 0;
	aQueue.push_back(pCurrSplit);

	float3 vSphereCenter;
	float fSphereRadius;

	while (aQueue.size() > iCount)
	{
		aQueue[iCount]->m_pBoundVolumeP->getSphere(&vSphereCenter, &fSphereRadius);

		float fDistSqr = SGCore_0DistancePointBeam2(vSphereCenter, *pPos, *pDir);
		if (aQueue[iCount]->m_iCountObj > 0 && fDistSqr <= fSphereRadius*fSphereRadius)
		{
			if (!(aQueue[iCount]->m_isFinite))
			{
				for (int j = 0; j<GREEN_COUNT_TYPE_SEGMENTATION; ++j)
				{
					if (aQueue[iCount]->m_aSplits[j])
						aQueue.push_back(aQueue[iCount]->m_aSplits[j]);
				}
			}
			else
			{
				if ((*pCount) < iCountCom)
				{
					ppArrSplits[(*pCount)] = aQueue[iCount];

					++(*pCount);
				}
			}
		}

		//aQueue.erase(0);
		++iCount;
	}
}

bool CGreen::traceBeam(const float3 *pStart, const float3 *pDir, float3 *pResult, ID *pIdGreen, ID *pIdSplits, ID *pIdObj, ID *pIdMtrl)
{
	DWORD ttime = GetTickCount();
	if (m_aGreens.size() <= 0)
		return false;

	CTriangle oTri;
	float3 ip;
	float3 vResult = (*pStart) + float3(10000.0f, 10000.0f, 10000.0f);
	float3 il = (*pDir) * 10000.0f;
	bool isFound = false;
	CVisInfo *pVisInfo = 0;
	CModel *pGreen = 0;
	float4x4 mTransformation;

	for (int id = 0; id < m_aGreens.size(); ++id)
	{
		pGreen = m_aGreens[id];
		pVisInfo = m_aVisCaclObj[1]->m_aVisibleInfo[id];
		pVisInfo->m_iCountCom = 0;

		DWORD ttime2 = GetTickCount();
		getPartBeam(pStart, pDir, pVisInfo->m_ppSegments, &(pVisInfo->m_iCountCom), pGreen->m_pSplitsTree, pVisInfo->m_iCount);

		ttime2 = GetTickCount() - ttime2;

		for (int k = 0; k < pVisInfo->m_iCountCom; ++k)
		{
			for (int key = 0; key < pVisInfo->m_ppSegments[k]->m_iCountObj; ++key)
			{
				CGreenDataVertex oDataVertex = m_aGreens[id]->m_pAllTrans[pVisInfo->m_ppSegments[k]->m_pArrIDs[key]];

				float fScale = oDataVertex.m_vTexCoord.x;
				mTransformation = SMMatrixScaling(fScale, fScale, fScale) * SMMatrixRotationY(oDataVertex.m_vTexCoord.y) * SMMatrixTranslation(oDataVertex.m_vPosition);

				for (int g = 0; g < pGreen->m_pDataLod0->m_aIndexCount.size(); ++g)
				{
					for (int poly = 0; poly < pGreen->m_pDataLod0->m_aIndexCount[g] / 3; ++poly)
					{
						oTri.m_vA = SMVector3Transform(pGreen->m_pDataLod0->m_aVertex[pGreen->m_pDataLod0->m_aIndex[poly]], mTransformation);
						oTri.m_vB = SMVector3Transform(pGreen->m_pDataLod0->m_aVertex[pGreen->m_pDataLod0->m_aIndex[poly + 1]], mTransformation);
						oTri.m_vC = SMVector3Transform(pGreen->m_pDataLod0->m_aVertex[pGreen->m_pDataLod0->m_aIndex[poly + 2]], mTransformation);

						if (oTri.IntersectLine((*pStart), il, &ip))
						{
							if (SMVector3Length2((*pStart) - vResult) > SMVector3Length2((*pStart) - ip))
							{
								vResult = ip;
								isFound = true;

								if (pIdGreen)
									*pIdGreen = id;

								if (pIdSplits)
									*pIdSplits = pVisInfo->m_ppSegments[k]->m_id;

								if (pIdObj)
									*pIdObj = key;

								if (pIdMtrl)
									*pIdMtrl = pGreen->m_aLods[0]->m_aIDsTextures[g];
							}
						}
					}
				}
			}
		}
	}

	if (isFound && pResult)
		*pResult = vResult;

	ttime = GetTickCount() - ttime;

	return isFound;
}

//##########################################################################

void CGreen::getPartBB(const float3 *pMin, const float3 *pMax, CSegment **ppArrSplits, int *pCount, CSegment *pCurrSplit, int iCountCom)
{
	Array<CSegment*> aQueue;
	int iCount = 0;
	aQueue.push_back(pCurrSplit);

	float3 vMin, vMax;

	while (aQueue.size() > iCount)
	{
		aQueue[iCount]->m_pBoundVolumeP->getMinMax(&vMin, &vMax);

		if (aQueue[iCount]->m_iCountObj > 0 && SGCore_0InretsectBox(pMin, pMax, &vMin, &vMax))
		{
			if (!(aQueue[iCount]->m_isFinite))
			{
				for (int j = 0; j<GREEN_COUNT_TYPE_SEGMENTATION; ++j)
				{
					if (aQueue[iCount]->m_aSplits[j])
						aQueue.push_back(aQueue[iCount]->m_aSplits[j]);
				}
			}
			else
			{
				if ((*pCount) < iCountCom)
				{
					ppArrSplits[(*pCount)] = aQueue[iCount];

					++(*pCount);
				}
			}
		}

		//aQueue.erase(0);
		++iCount;
	}
}

bool CGreen::getOccurencessLeafGrass(const float3 *pMin, const float3 *pMax, int iPhysicMtl)
{
	if (m_aGreens.size() <= 0)
		return false;

	bool isFound = false;
	CVisInfo *pVisInfo = 0;
	CModel *pGreen = 0;
	float3 vP1, vP2, vP3, vMin, vMax;
	float4x4 mTransformation;
	CGreenDataVertex oDataVertex;

	for (int id = 0; id < m_aGreens.size() && !isFound; ++id)
	{
		pGreen = m_aGreens[id];
		pVisInfo = m_aVisCaclObj[1]->m_aVisibleInfo[id];
		pVisInfo->m_iCountCom = 0;

		getPartBB(pMin, pMax, pVisInfo->m_ppSegments, &(pVisInfo->m_iCountCom), pGreen->m_pSplitsTree, pVisInfo->m_iCount);

		for (int g = 0; g < pGreen->m_pDataLod0->m_aIndexCount.size() && !isFound; ++g)
		{
			int iPM = SGCore_MtlGetPhysicType(pGreen->m_pDataLod0->m_aMtrl[g]);
			if (SGCore_MtlGetPhysicType(pGreen->m_pDataLod0->m_aMtrl[g]) != iPhysicMtl)
				continue;

			for (int k = 0; k < pVisInfo->m_iCountCom && !isFound; ++k)
			{
				for (int key = 0; key < pVisInfo->m_ppSegments[k]->m_iCountObj && !isFound; ++key)
				{
					oDataVertex = pGreen->m_pAllTrans[pVisInfo->m_ppSegments[k]->m_pArrIDs[key]];
					float fScale = oDataVertex.m_vTexCoord.x;
					mTransformation = SMMatrixScaling(fScale, fScale, fScale) * SMMatrixRotationY(oDataVertex.m_vTexCoord.y) * SMMatrixTranslation(oDataVertex.m_vPosition);

					vMin = pGreen->m_vMin;
					vMax = pGreen->m_vMax;

					vMin = SMVector3Transform(vMin, mTransformation);
					vMax = SMVector3Transform(vMax, mTransformation);

					if (SGCore_0InretsectBox(pMin, pMax, &vMin, &vMax))
					{
						//isFound = true;
						//break;
					
						for (int poly = 0; poly < pGreen->m_pDataLod0->m_aIndexCount[g] / 3 && !isFound; ++poly)
						{
							//oDataVertex = m_aGreens[id]->m_pAllTrans[pVisInfo->m_ppSegments[k]->m_pArrIDs[key]];

							//float fScale = oDataVertex.m_vTexCoord.x;
							//mTransformation = SMMatrixScaling(fScale, fScale, fScale) * SMMatrixRotationY(oDataVertex.m_vTexCoord.y) * SMMatrixTranslation(oDataVertex.m_vPosition);

							vP1 = SMVector3Transform(pGreen->m_pDataLod0->m_aVertex[pGreen->m_pDataLod0->m_aIndex[poly]], mTransformation);
							vP2 = SMVector3Transform(pGreen->m_pDataLod0->m_aVertex[pGreen->m_pDataLod0->m_aIndex[poly + 1]], mTransformation);
							vP3 = SMVector3Transform(pGreen->m_pDataLod0->m_aVertex[pGreen->m_pDataLod0->m_aIndex[poly + 2]], mTransformation);

							vMin = vP1;
							vMax = vP1;

							if (vMin.x > vP2.x)
								vMin.x = vP2.x;
							if (vMin.y > vP2.y)
								vMin.y = vP2.y;
							if (vMin.z > vP2.z)
								vMin.z = vP2.z;

							if (vMin.x > vP3.x)
								vMin.x = vP3.x;
							if (vMin.y > vP3.y)
								vMin.y = vP3.y;
							if (vMin.z > vP3.z)
								vMin.z = vP3.z;

							if (vMax.x < vP2.x)
								vMax.x = vP2.x;
							if (vMax.y < vP2.y)
								vMax.y = vP2.y;
							if (vMax.z < vP2.z)
								vMax.z = vP2.z;

							if (vMax.x < vP3.x)
								vMax.x = vP3.x;
							if (vMax.y < vP3.y)
								vMax.y = vP3.y;
							if (vMax.z < vP3.z)
								vMax.z = vP3.z;

							if (SGCore_0InretsectBox(pMin, pMax, &vMin, &vMax))
							{
								isFound = true;
								break;
							}
						}
					}
				}
			}
		}
	}

	return isFound;
}