
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "static_geom.h"

CStaticGeom::CSegment::CSegment()
{
	for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
	{
		m_aSplits[i] = 0;
	}

	m_ppArrPoly = 0;
	m_pCountPoly = 0;
	m_pNumberGroup = 0;
	m_pNumberGroupModel = 0;
	m_uiCountSubSet = 0;
	m_uiCountAllPoly = 0;
	m_pBoundVolumeSys = 0;
	m_pBoundVolumeP = 0;
	m_ID = -1;
	m_SID = -1;
	m_isNonEnd = false;
	m_pBoundBox = 0;
}

CStaticGeom::CSegment::~CSegment()
{
	for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; i++)
	{
		mem_delete(m_aSplits[i]);
	}
	mem_delete_a(m_pNumberGroup);
	mem_delete_a(m_pNumberGroupModel);
	mem_delete_a(m_pCountPoly);
	
	if (m_ppArrPoly)
	{
		for (DWORD i = 0; i < m_uiCountSubSet; i++)
		{
			mem_delete_a(m_ppArrPoly[i]);
		}
	}

	mem_delete_a(m_ppArrPoly);
	mem_release_del(m_pBoundVolumeSys);
	mem_release_del(m_pBoundVolumeP);
	//ndx_release(BoundBox);
}

CStaticGeom::CGroup::CGroup()
{
	m_sName = "";
	m_idTexture = -1;
	m_iSortGroup = 0;
	m_iCountVertex = m_iCountIndex = 0;
}

/*CStaticGeom::Group::VertexBuff::VertexBuff()
{
	arr = 0;
	count = 0;
}

CStaticGeom::Group::VertexBuff::~VertexBuff()
{
	mem_delete_a(arr);
	count = 0;
}*/

CStaticGeom::CGroup::~CGroup()
{
	m_idTexture = -1;
	m_iCountVertex = m_iCountIndex = 0;
	m_aCountVertex.clear();
	m_aCountIndex.clear();

	for(int i = 0, l = m_aVertexBuffers.size(); i < l; ++i)
	{
		mem_release_del(m_aVertexBuffers[i]);
		//mem_delete(VertexBufferOrigin[i]);
	}
}

CStaticGeom::CModel::CLod::CLod()
{
	m_szPathName[0] = 0;
	m_pModel = 0;
	m_pBoundVolume = SGCore_CrBound();
}

CStaticGeom::CModel::CLod::~CLod()
{
	mem_release_del(m_pModel);
	mem_release_del(m_pBoundVolume);
}

CStaticGeom::CModel::CModel()
{
	m_idCountSplits = m_idCountSplitsRender = 0;
	m_pArrSplits = 0;
	m_vPosition = float3(0, 0, 0);
	m_vRotation = float3(0, 0, 0);
	m_vScale = float3(1, 1, 1);

	m_vOldPosition = m_vPosition;
	m_vOldRotation = m_vRotation;
	m_vOldScale = m_vScale;

	m_uiCountPoly = 0;

	m_szName[0] = 0;
	m_szPathName[0] = 0;
	m_isRenderLod = false;
}

CStaticGeom::CModel::~CModel()
{
	m_idCountSplits = m_idCountSplitsRender = 0;
	m_aSubSets.clear();
	mem_delete(m_pArrSplits);
}

CStaticGeom::CInfoRenderSegments::CInfoRenderSegments()
{
	m_ppSegments = 0; m_iCount = 0; m_iCountCom = 0;
}

CStaticGeom::CInfoRenderSegments::~CInfoRenderSegments()
{
	mem_delete_a(m_ppSegments);
}

CStaticGeom::CIRSData::CIRSData()
{

}

CStaticGeom::CIRSData::~CIRSData()
{
	m_aQueue.clear();
	for (int i = 0; i < m_aIRS.size(); ++i)
	{
		mem_delete(m_aIRS[i]);
	}
	m_aIRS.clear();
}

CStaticGeom::CStaticGeom()
{
	m_iSizeRenderIndexBuffer = 0;
	m_pRenderIndexBuffer = 0;

	m_pppRTArrIndices = 0;
	m_ppRTCountDrawPoly = 0;
	m_pppRTCountDrawPolyModel = 0;
	m_pppRTBeginIndexModel = 0;

	m_mWorld = SMMatrixIdentity();

	m_pCurrArrMeshVertex = 0;
	m_pCurrArrMeshIndex = 0;

	m_pBoundVolume = SGCore_CrBound();

	/*D3DVERTEXELEMENT9 layoutstatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	CStaticGeom::m_pDXDevice->CreateVertexDeclaration(layoutstatic, &VertexDeclarationStatic);*/

	CIRSData* tmparr = new CIRSData();
	m_aArrComFor.push_back(tmparr);
	CIRSData* tmparr2 = new CIRSData();
	m_aArrComFor.push_back(tmparr2);
}

CStaticGeom::~CStaticGeom()
{
	for(int i = 0, l = m_aArrComFor.size(); i < l; ++i)
	{
		mem_delete(m_aArrComFor[i]);
	}

	m_aArrComFor.clear();
	mem_delete_a(m_pCurrArrMeshVertex);
	mem_delete_a(m_pCurrArrMeshIndex);
	mem_release(m_pRenderIndexBuffer);
	//mem_release(VertexDeclarationStatic);
	mem_release(m_pBoundVolume);

	deleteArrIndexPtr();

	for(int i = 0, l = m_aAllModels.size(); i < l; ++i)
	{
		mem_delete(m_aAllModels[i]);
	}
	m_aAllModels.clear();

	for(int i = 0, l = m_aAllGroups.size(); i < l; ++i)
	{
		mem_delete(m_aAllGroups[i]);
	}
	m_aAllGroups.clear();
}

void CStaticGeom::onLostDevice()
{
	mem_release_del(m_pRenderIndexBuffer);
}

void CStaticGeom::onResetDevice()
{
	CStaticGeom::m_pDXDevice->CreateIndexBuffer(sizeof(uint32_t)* m_iSizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &m_pRenderIndexBuffer, 0);
}

int CStaticGeom::getCountModel()
{
	return m_aAllModels.size();
}

ID CStaticGeom::addModel(const char* path, const char* lod1, const char* name)
{
	LibReport(0, "load geometry: '%s'.. ", path);
	deleteArrIndexPtr();

	ISXDataStaticModel* model = 0;
	
	char tmppath[1024];
	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path);
	SGCore_StaticModelLoad(tmppath, &model);

	CModel* tmpmodel = new CModel();
	ID tmpidmodel = m_aAllModels.size() - 1;
	if (!STR_VALIDATE(name))
		sprintf(tmpmodel->m_szName, "");
	else
		sprintf(tmpmodel->m_szName, "%s", name);

	sprintf(tmpmodel->m_szPathName, "%s", path);

	bool isunictex = true;	//уникальна ли подгруппа в модели? (то есть, нет ли данной подгруппы в списке уже загруженных)

	vertex_static* pVert;
	UINT* pInd;

	if (FAILED(model->m_pVertexBuffer->Lock(0, 0, (void**)&pVert, 0)))
		return -1;

	if (FAILED(model->m_pIndexBuffer->Lock(0, 0, (void**)&pInd, 0)))
		return -1;

	for (UINT i = 0; i < model->m_uiSubsetCount; ++i)
	{
		tmpmodel->m_uiCountPoly += model->m_pIndexCount[i] / 3;

		char tmptex[1024];
		sprintf(tmptex, "%s.dds", model->m_ppTextures[i]);
		tmpmodel->m_aIDsTexures[i] = SGCore_MtlLoad(tmptex, MTL_TYPE_GEOM);

		

		//если количество полигонов в подгруппе модели больше разрешенного
		if (model->m_pIndexCount[i] / 3 > GEOM_MAX_POLY_IN_GROUP)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s count polygons %d in group over default '%d'", GEN_MSG_LOCATION, model->m_pIndexCount[i] / 3, GEOM_MAX_POLY_IN_GROUP);
			return -1;
		}

		isunictex = true;

		for (UINT k = 0; k < m_aAllGroups.size(); ++k)
		{
			//сравниваем подгруппы только по имени, если имена одинаковые значит это одна и та же подгруппа
			if (stricmp(m_aAllGroups[k]->m_sName.c_str(), model->m_ppTextures[i]) == 0)
			{
				isunictex = false;

				CModel::CSubSet gdb;

				gdb.m_idGroup = k;

				IDirect3DVertexBuffer9* vb;

				vertex_static * pData;
				vertex_static * pData2;

				//если последний незаполненный буфер не вмещает в себ¤
				if (m_aAllGroups[k]->m_aCountIndex[m_aAllGroups[k]->m_aCountIndex.size() - 1] / 3 + model->m_pIndexCount[i] / 3 > GEOM_MAX_POLY_IN_GROUP)
				{
					//создаем новый
					gdb.m_idVertexStart = 0;
					gdb.m_uiVertexCount = model->m_pVertexCount[i];

					CStaticGeom::m_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* model->m_pVertexCount[i], NULL, NULL, D3DPOOL_MANAGED, &vb, 0);
					
					/*Group::VertexBuff* vborigin = new Group::VertexBuff();
					vborigin->count = model->VertexCount[i];
					vborigin->arr = new float3_t[vborigin->count];
					for (int j = 0; j < model->VertexCount[i]; ++j)
					{
						vborigin->arr[j] = (model->ArrVertBuf + model->StartVertex[i] + j)->Pos;
					}
					m_aAllGroups[k]->VertexBufferOrigin.push_back(vborigin);*/
					
					m_aAllGroups[k]->m_iCountVertex += model->m_pVertexCount[i];
					m_aAllGroups[k]->m_aCountVertex.push_back(model->m_pVertexCount[i]);
					//копируем данные в новый вершинный буфер
					if (!FAILED(vb->Lock(0, 0, (void**)&pData, 0)))
					{
						memcpy(pData, pVert + model->m_pStartVertex[i], sizeof(vertex_static)* model->m_pVertexCount[i]);
						vb->Unlock();
					}

					m_aAllGroups[k]->m_aVertexBuffers.push_back(vb);


					gdb.m_idIndexStart = 0;
					gdb.m_uiIndexCount = model->m_pIndexCount[i];
					m_aAllGroups[k]->m_iCountIndex += model->m_pIndexCount[i];
					m_aAllGroups[k]->m_aCountIndex.push_back(model->m_pIndexCount[i]);

					gdb.m_idBuff = m_aAllGroups[k]->m_aVertexBuffers.size() - 1;

					m_aAllGroups[k]->m_aModels[gdb.m_idBuff].push_back(tmpmodel);

					if (m_iSizeRenderIndexBuffer < (int)model->m_pIndexCount[i])
					{
						mem_release(m_pRenderIndexBuffer);
						m_iSizeRenderIndexBuffer = model->m_pIndexCount[i];
						CStaticGeom::m_pDXDevice->CreateIndexBuffer(sizeof(uint32_t)* m_iSizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &m_pRenderIndexBuffer, 0);
					}
				}
				//иначе если влезет в буфер
				else
				{
					//впихиваем в буфер
					gdb.m_idBuff = m_aAllGroups[k]->m_aVertexBuffers.size() - 1;
					Array<CModel*>& tmparrmodel = m_aAllGroups[k]->m_aModels[gdb.m_idBuff];
					m_aAllGroups[k]->m_aModels[gdb.m_idBuff].push_back(tmpmodel);
					int lastvbnum = m_aAllGroups[k]->m_aVertexBuffers.size() - 1;

					CStaticGeom::m_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* (m_aAllGroups[k]->m_aCountVertex[lastvbnum] + model->m_pVertexCount[i]), NULL, NULL, D3DPOOL_MANAGED, &vb, 0);
					
					/*Group::VertexBuff* vborigin = new Group::VertexBuff();
					vborigin->count = (m_aAllGroups[k]->CountVertex[lastvbnum] + model->VertexCount[i]);
					vborigin->arr = new float3_t[vborigin->count];
					memcpy(vborigin->arr, m_aAllGroups[k]->VertexBufferOrigin[lastvbnum]->arr, sizeof(float3_t) * m_aAllGroups[k]->CountVertex[lastvbnum]);
					for (int j = 0; j < model->VertexCount[i]; ++j)
					{
						vborigin->arr[m_aAllGroups[k]->CountVertex[lastvbnum]+j] = (model->ArrVertBuf + model->StartVertex[i] + j)->Pos;
					}
					mem_delete(m_aAllGroups[k]->VertexBufferOrigin[lastvbnum]);
					m_aAllGroups[k]->VertexBufferOrigin[lastvbnum] = vborigin;*/

					gdb.m_idVertexStart = m_aAllGroups[k]->m_aCountVertex[lastvbnum];
					gdb.m_uiVertexCount = model->m_pVertexCount[i];
					
					if (!FAILED(vb->Lock(0, 0, (void**)&pData, 0)))
					{
						if (!FAILED(m_aAllGroups[k]->m_aVertexBuffers[lastvbnum]->Lock(0, 0, (void**)&pData2, 0)))
						{
							memcpy(pData, pData2, sizeof(vertex_static)* m_aAllGroups[k]->m_aCountVertex[lastvbnum]);
							m_aAllGroups[k]->m_aVertexBuffers[lastvbnum]->Unlock();
						}

						memcpy(pData + m_aAllGroups[k]->m_aCountVertex[lastvbnum], pVert + model->m_pStartVertex[i], sizeof(vertex_static)* (model->m_pVertexCount[i]));
						vb->Unlock();
					}

					m_aAllGroups[k]->m_iCountVertex += model->m_pVertexCount[i];
					m_aAllGroups[k]->m_aCountVertex[lastvbnum] += model->m_pVertexCount[i];
					mem_release(m_aAllGroups[k]->m_aVertexBuffers[lastvbnum]);
					m_aAllGroups[k]->m_aVertexBuffers[lastvbnum] = vb;

					gdb.m_idIndexStart = m_aAllGroups[k]->m_aCountIndex[lastvbnum];
					gdb.m_uiIndexCount = model->m_pIndexCount[i];

					m_aAllGroups[k]->m_iCountIndex += model->m_pIndexCount[i];
					m_aAllGroups[k]->m_aCountIndex[lastvbnum] += model->m_pIndexCount[i];

					if (m_iSizeRenderIndexBuffer < (int)(m_aAllGroups[k]->m_aCountIndex[lastvbnum] + model->m_pIndexCount[i]))
					{
						mem_release(m_pRenderIndexBuffer);
						m_iSizeRenderIndexBuffer = (m_aAllGroups[k]->m_aCountIndex[lastvbnum] + model->m_pIndexCount[i]);
						CStaticGeom::m_pDXDevice->CreateIndexBuffer(sizeof(uint32_t)* m_iSizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &m_pRenderIndexBuffer, 0);
					}
				}

				tmpmodel->m_aSubSets.push_back(gdb);
			}
		}

		if (isunictex)
		{
			CGroup* ngroup = new CGroup();
			CModel::CSubSet gdb;

			ngroup->m_aModels[0].push_back(tmpmodel);

			gdb.m_idBuff = 0;
			gdb.m_idGroup = m_aAllGroups.size();
			gdb.m_idVertexStart = 0;
			gdb.m_uiVertexCount = model->m_pVertexCount[i];
			gdb.m_idIndexStart = 0;
			gdb.m_uiIndexCount = model->m_pIndexCount[i];

			tmpmodel->m_aSubSets.push_back(gdb);

			IDirect3DVertexBuffer9* vb;
			CStaticGeom::m_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* model->m_pVertexCount[i], NULL, NULL, D3DPOOL_MANAGED, &vb, 0);
			
			/*Group::VertexBuff* vborigin = new Group::VertexBuff();
			vborigin->count = model->VertexCount[i];
			vborigin->arr = new float3_t[vborigin->count];

			for (int k = 0; k < model->VertexCount[i]; ++k)
			{
				vborigin->arr[k] = (model->ArrVertBuf + model->StartVertex[i] + k)->Pos;
			}*/

			ngroup->m_iCountVertex = model->m_pVertexCount[i];
			ngroup->m_aCountVertex.push_back(model->m_pVertexCount[i]);
			vertex_static * pData;
			if (!FAILED(vb->Lock(0, 0, (void**)&pData, 0)))
			{
				memcpy(pData, pVert + model->m_pStartVertex[i], sizeof(vertex_static)* model->m_pVertexCount[i]);
				vb->Unlock();
			}

			ngroup->m_iCountIndex = model->m_pIndexCount[i];
			ngroup->m_aCountIndex.push_back(model->m_pIndexCount[i]);
			
			ngroup->m_aVertexBuffers.push_back(vb);
			//ngroup->VertexBufferOrigin.push_back(vborigin);
			
			if (m_iSizeRenderIndexBuffer < (int)model->m_pIndexCount[i])
			{
				mem_release(m_pRenderIndexBuffer);
				m_iSizeRenderIndexBuffer = model->m_pIndexCount[i];
				CStaticGeom::m_pDXDevice->CreateIndexBuffer(sizeof(uint32_t)* m_iSizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &m_pRenderIndexBuffer, 0);
			}

			//char tmptex[1024];
			//sprintf(tmptex, "%s.dds", model->ArrTextures[i]);

			ngroup->m_idTexture = tmpmodel->m_aIDsTexures[i];// SGCore_LoadMtl(tmptex, MTL_GEOM);
			ngroup->m_iSortGroup = SGCore_MtlGetSort(ngroup->m_idTexture);
			ngroup->m_isRenderSingly = SGCore_MtlGroupRenderIsSingly(ngroup->m_idTexture);
			ngroup->m_sName = model->m_ppTextures[i];
			m_aAllGroups.push_back(ngroup);
		}


		vertex_static* pData;
		model->m_pVertexBuffer->Lock(0, 0, (void**)&pData, 0);

		float3_t tmppos = pData[model->m_pStartVertex[i]].Pos;
		float3 tmpMax = tmppos;
		float3 tmpMin = tmppos;
		for (DWORD k = 0; k<model->m_pVertexCount[i]; k++)
		{
			tmppos = pData[model->m_pStartVertex[i] + k].Pos;

			if (tmppos.x > tmpMax.x)
				tmpMax.x = tmppos.x;

			if (tmppos.y > tmpMax.y)
				tmpMax.y = tmppos.y;

			if (tmppos.z > tmpMax.z)
				tmpMax.z = tmppos.z;


			if (tmppos.x < tmpMin.x)
				tmpMin.x = tmppos.x;

			if (tmppos.y < tmpMin.y)
				tmpMin.y = tmppos.y;

			if (tmppos.z < tmpMin.z)
				tmpMin.z = tmppos.z;
		}

		DWORD* indeces;
		model->m_pIndexBuffer->Lock(0, 0, (void **)&indeces, 0);

		float3 tmpMM = SMVectorLerp(tmpMax, tmpMin, 0.5f);

		D3DXPlaneFromPoints(&tmpmodel->m_aSubSets[i].m_oPlane,
			&D3DXVECTOR3(pData[indeces[model->m_pStartIndex[i] + 0]].Pos.x, pData[indeces[model->m_pStartIndex[i] + 0]].Pos.y, pData[indeces[model->m_pStartIndex[i] + 0]].Pos.z),
			&D3DXVECTOR3(pData[indeces[model->m_pStartIndex[i] + 1]].Pos.x, pData[indeces[model->m_pStartIndex[i] + 1]].Pos.y, pData[indeces[model->m_pStartIndex[i] + 1]].Pos.z),
			&D3DXVECTOR3(pData[indeces[model->m_pStartIndex[i] + 2]].Pos.x, pData[indeces[model->m_pStartIndex[i] + 2]].Pos.y, pData[indeces[model->m_pStartIndex[i] + 2]].Pos.z));
		model->m_pVertexBuffer->Unlock();
		model->m_pIndexBuffer->Unlock();

		tmpmodel->m_aSubSets[i].m_vCenter = (float3_t)((tmpMax + tmpMin) * 0.5);
		tmpmodel->m_aSubSets[i].m_vMin = tmpMin;
		tmpmodel->m_aSubSets[i].m_vMax = tmpMax;
	}

	model->m_pVertexBuffer->Unlock();
	model->m_pIndexBuffer->Unlock();

	m_aAllModels.push_back(tmpmodel);

	setModelLodPath(m_aAllModels.size() - 1, lod1);
	
	preSegmentation(tmpmodel, model);

	setSplitID(tmpmodel->m_pArrSplits, &tmpmodel->m_idCountSplits, &tmpmodel->m_idCountSplitsRender);

	addModelInArrCom(m_aAllModels.size() - 1);

	initArrIndexPtr();

	float3 jmin, jmax, jmin2, jmax2;

	m_aAllModels[m_aAllModels.size()-1]->m_pArrSplits->m_pBoundVolumeP->getMinMax(&jmin, &jmax);
	m_pBoundVolume->getMinMax(&jmin2, &jmax2);

	if (jmin2.x > jmin.x)
		jmin2.x = jmin.x;
	if (jmin2.y > jmin.y)
		jmin2.y = jmin.y;
	if (jmin2.z > jmin.z)
		jmin2.z = jmin.z;

	if (jmax2.x < jmax.x)
		jmax2.x = jmax.x;
	if (jmax2.y < jmax.y)
		jmax2.y = jmax.y;
	if (jmax2.z < jmax.z)
		jmax2.z = jmax.z;

	m_pBoundVolume->setMinMax(&jmin2, &jmax2);
	
	mem_release_del(model);

	mem_delete_a(m_pCurrArrMeshVertex);
	mem_delete_a(m_pCurrArrMeshIndex);

	LibReport(0, " completed\n", path);
	return m_aAllModels.size() - 1;
}

void CStaticGeom::deleteModel(ID id)
{
	if(id >= (int)m_aAllModels.size())
		return;

	CModel* tmpmodel = m_aAllModels[id];

	//ПРЕЖДЕ НАДО ОЧИСТИТЬ БУФЕРЫ КОТОРЫЕ ЗАПОЛНЯЮТСЯ ИНДЕКСНЫМИ ДАННЫМИ, ИБО ПОСЛЕ УДАЛЕНИЯ МОДЕЛИ ДАННЫЕ ДЛЯ ОЧИТСКИ БУДУТ УЖЕ НЕКОРРЕТНЫЕ
	deleteArrIndexPtr();

	vertex_static* pData, *pData2, *pData3;
	ID idgroup;
	ID idbuff;

	for (UINT i = 0; i < tmpmodel->m_aSubSets.size(); ++i)
	{
		idgroup = tmpmodel->m_aSubSets[i].m_idGroup;
		idbuff = tmpmodel->m_aSubSets[i].m_idBuff;

		int tmpcountvertexi = m_aAllGroups[idgroup]->m_aCountVertex[idbuff];
		int tmpcountvertex = tmpmodel->m_aSubSets[i].m_uiVertexCount;
		int tmpcountstartvertex = tmpmodel->m_aSubSets[i].m_idVertexStart;

		//если количество удаляемых вершин меньше чем общее количество вершин в буфере подгруппы
		if (m_aAllGroups[idgroup]->m_aCountVertex[idbuff] > tmpmodel->m_aSubSets[i].m_uiVertexCount)
		{
			//производим удаление, путем пересоздания есесно
			IDirect3DVertexBuffer9* vb;
			CStaticGeom::m_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* (m_aAllGroups[idgroup]->m_aCountVertex[idbuff] - tmpmodel->m_aSubSets[i].m_uiVertexCount), NULL, NULL, D3DPOOL_MANAGED, &vb, 0);

			/*Group::VertexBuff* vborigin = new Group::VertexBuff();
			vborigin->count = (m_aAllGroups[idgroup]->CountVertex[idbuff] - tmpmodel->m_aSubSets[i].VertexCount);
			vborigin->arr = new float3_t[vborigin->count];*/

			vb->Lock(0, 0, (void**)&pData, 0);
			m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Lock(0, 0, (void**)&pData2, 0);
			int tmpvertexstart = tmpmodel->m_aSubSets[i].m_idVertexStart;
			memcpy(pData, pData2, sizeof(vertex_static)* tmpmodel->m_aSubSets[i].m_idVertexStart);
			//memcpy(vborigin->arr, m_aAllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr, sizeof(float3_t)* tmpmodel->m_aSubSets[i].VertexStart);
			if (m_aAllGroups[idgroup]->m_aCountVertex[idbuff] - (tmpmodel->m_aSubSets[i].m_idVertexStart + tmpmodel->m_aSubSets[i].m_uiVertexCount) > 0)
			{
				memcpy(pData + tmpmodel->m_aSubSets[i].m_idVertexStart, pData2 + tmpmodel->m_aSubSets[i].m_idVertexStart + tmpmodel->m_aSubSets[i].m_uiVertexCount, sizeof(vertex_static)* (m_aAllGroups[idgroup]->m_aCountVertex[idbuff] - (tmpmodel->m_aSubSets[i].m_idVertexStart + tmpmodel->m_aSubSets[i].m_uiVertexCount)));
				//memcpy(vborigin->arr + tmpmodel->m_aSubSets[i].VertexStart, m_aAllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr + tmpmodel->m_aSubSets[i].VertexStart + tmpmodel->m_aSubSets[i].VertexCount, sizeof(float3_t)* (m_aAllGroups[idgroup]->CountVertex[idbuff] - (tmpmodel->m_aSubSets[i].VertexStart + tmpmodel->m_aSubSets[i].VertexCount)));

			}

			vb->Unlock();
			m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Unlock();

			//обновляем данные и производим замену
			m_aAllGroups[idgroup]->m_iCountVertex -= tmpmodel->m_aSubSets[i].m_uiVertexCount;
			m_aAllGroups[idgroup]->m_aCountVertex[idbuff] -= tmpmodel->m_aSubSets[i].m_uiVertexCount;

			m_aAllGroups[idgroup]->m_iCountIndex -= tmpmodel->m_aSubSets[i].m_uiIndexCount;
			m_aAllGroups[idgroup]->m_aCountIndex[idbuff] -= tmpmodel->m_aSubSets[i].m_uiIndexCount;

			mem_release(m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]);
			m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff] = vb;

			/*mem_delete(m_aAllGroups[idgroup]->VertexBufferOrigin[idbuff]);
			m_aAllGroups[idgroup]->VertexBufferOrigin[idbuff] = vborigin;*/
		}
		
		//если количество моделей в массиве больше одной
		if (m_aAllGroups[idgroup]->m_aModels[idbuff].size() > 1)
		{
			//находим удаляемую модель в массиве моделей в буфере данной подгруппы
			int tmpidingroup = -1;	//id модели в текущем массиве, в подгруппе
			for (UINT k = 0; k < m_aAllGroups[idgroup]->m_aModels[idbuff].size(); ++k)
			{
				CModel* tmptmpmodel = m_aAllGroups[idgroup]->m_aModels[idbuff][k];
				if (m_aAllGroups[idgroup]->m_aModels[idbuff][k] == tmpmodel)
				{
					tmpidingroup = k;
					break;
				}
			}

			//если модель не была найдена в массиве моделей
			if (tmpidingroup == -1)
			{
				//то это очень плохо
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - when deleting the model, it was found in an array of patterns in subgroups of the buffer, it is very bad", GEN_MSG_LOCATION);
			}

			//обновляем данные в подгруппе модели о стартовых позициях
			for (UINT k = tmpidingroup + 1; k < m_aAllGroups[idgroup]->m_aModels[idbuff].size(); ++k)
			{
				CModel* tmptmpmodel = m_aAllGroups[idgroup]->m_aModels[idbuff][k];
				for (UINT j = 0; j < m_aAllGroups[idgroup]->m_aModels[idbuff][k]->m_aSubSets.size(); ++j)
				{
					if (tmptmpmodel->m_aSubSets[j].m_idGroup == idgroup)
					{
						tmptmpmodel->m_aSubSets[j].m_idIndexStart -= tmpmodel->m_aSubSets[i].m_uiIndexCount;
						tmptmpmodel->m_aSubSets[j].m_idVertexStart -= tmpmodel->m_aSubSets[i].m_uiVertexCount;

						break;
					}
				}

				//обновляем также все индексы во всех сплитах
				Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> queue;
				queue.push_back(m_aAllGroups[idgroup]->m_aModels[idbuff][k]->m_pArrSplits);

				while (queue.size())
				{
					if (queue[0]->m_isNonEnd)
					{
						for (int j = 0; j < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
						{
							if (queue[0]->m_aSplits[j])
								queue.push_back(queue[0]->m_aSplits[j]);
						}
					}
					else
					{
						for (UINT j = 0; j < queue[0]->m_uiCountSubSet; ++j)
						{
							if (queue[0]->m_pNumberGroup[j] == idgroup )
							{
								for (UINT q = 0; q < queue[0]->m_pCountPoly[j]*3; ++q)
								{
									queue[0]->m_ppArrPoly[j][q] -= tmpmodel->m_aSubSets[i].m_uiVertexCount;
								}
							}
						}
					}

					queue.erase(0);
				}
			}

			//удал¤ем текущую удал¤емую модель из массива моделей буфера текущей подгруппы
			m_aAllGroups[idgroup]->m_aModels[idbuff].erase(tmpidingroup);
		}
		//если в буфере подгруппы всего одна модель, то значит эта та модель которую удаляем
		else if (m_aAllGroups[idgroup]->m_aModels[idbuff].size() == 1)
			m_aAllGroups[idgroup]->m_aModels[idbuff].erase(0);
		
		//если количество удалемых вершин больше либо равно количеству имеющихся в буфере подгруппы
		if (tmpcountvertexi <= tmpcountvertex)
		{
			//то теряется смысл данного буфера, и надо его удалить

			CGroup* tmpgroup = m_aAllGroups[idgroup];

			tmpgroup->m_aCountVertex.erase(idbuff);
			tmpgroup->m_aCountIndex.erase(idbuff);
			mem_release(tmpgroup->m_aVertexBuffers[idbuff]);
			//IDirect3DVertexBuffer9* vb = m_aAllGroups[idgroup]->VertexBuffer[idbuff];
			tmpgroup->m_aVertexBuffers.erase(idbuff);
			tmpgroup->m_aModels.erase(idbuff);
			//Group::VertexBuff* tmpduff = m_aAllGroups[idgroup]->VertexBufferOrigin[idbuff];
			/*mem_delete(tmpgroup->VertexBufferOrigin[idbuff]);
			tmpgroup->VertexBufferOrigin.erase(idbuff);*/
		}

		//если подгруппа пустая, то обрабатываем все значения что больше идентификатора группы
		if (m_aAllGroups[idgroup]->m_aCountVertex.size() <= 0)
		{
			//Group* tmpgroup = m_aAllGroups[idgroup];
			
			//то есть декрементируем все идентфиикаторы подгрупп которые больше чем текущая удаляемая подгруппа
			//декремент произодим и у текущей удаляемой модели, ибо все будет смещено, а значит и в этой моделе должно быть все смещенно для корретных просчетов
			for (UINT k = 0; k < m_aAllModels.size(); ++k)
			{
				CModel* tmptmpmodel = m_aAllModels[k];

				for(UINT j = 0; j < tmptmpmodel->m_aSubSets.size(); ++j)
				{
					if (tmptmpmodel->m_aSubSets[j].m_idGroup > idgroup)
						--(tmptmpmodel->m_aSubSets[j].m_idGroup);
				}

				//также проходимся и по всем сплитам чтобы и там произвести декремент
				Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> queue;
				queue.push_back(tmptmpmodel->m_pArrSplits);

				while (queue.size())
				{
					if (queue[0]->m_isNonEnd)
					{
						for (int j = 0; j < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
						{
							if (queue[0]->m_aSplits[j])
								queue.push_back(queue[0]->m_aSplits[j]);
						}
					}
					else
					{
						for(UINT j = 0; j < queue[0]->m_uiCountSubSet; ++j)
						{
							if ((int)queue[0]->m_pNumberGroup[j] > idgroup)
								--(queue[0]->m_pNumberGroup[j]);
						}
					}

					queue.erase(0);
				}
			}

			//удаляем подгруппу, за ненадобностью
			mem_delete(m_aAllGroups[idgroup]);
			m_aAllGroups.erase(idgroup);
		}
		//если подгруппа не пустая то может быть тогда можно где-то совместить буферы?
		else
		{
			for (UINT k = 0; k < m_aAllGroups[idgroup]->m_aVertexBuffers.size(); ++k)
			{
				//можно ли совместить буферы
				if (idbuff != k && m_aAllGroups[idgroup]->m_aCountIndex[idbuff] / 3 + m_aAllGroups[idgroup]->m_aCountIndex[k] / 3 <= GEOM_MAX_POLY_IN_GROUP)
				{
					//СОВМЕЩАЕМ ТЕКУЩИЙ ОБРАБАТЫВАЕМЫЙ БУФЕР (idbuff) (ИЗ КОТОРОГО МОДЕЛЬ БЫЛА УДАЛЕНА С БУФЕРОМ В КОТОРЙ ОН ВМЕЩАЕТСЯ (k)
					//idbuff => k

					//производим совмещение данных в буферах
					//{
					IDirect3DVertexBuffer9* vb;
					CStaticGeom::m_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* (m_aAllGroups[idgroup]->m_aCountVertex[idbuff] + m_aAllGroups[idgroup]->m_aCountVertex[k]), NULL, NULL, D3DPOOL_MANAGED, &vb, 0);

					/*Group::VertexBuff* vborigin = new Group::VertexBuff();
					vborigin->count = (m_aAllGroups[idgroup]->CountVertex[idbuff] + m_aAllGroups[idgroup]->CountVertex[k]);
					vborigin->arr = new float3_t[vborigin->count];*/

					vb->Lock(0, 0, (void**)&pData, 0);
					m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Lock(0, 0, (void**)&pData2, 0);
					m_aAllGroups[idgroup]->m_aVertexBuffers[k]->Lock(0, 0, (void**)&pData3, 0);

					memcpy(pData, pData3, sizeof(vertex_static)* m_aAllGroups[idgroup]->m_aCountVertex[k]);
					//memcpy(vborigin->arr, m_aAllGroups[idgroup]->VertexBufferOrigin[k]->arr, sizeof(float3_t)* m_aAllGroups[idgroup]->CountVertex[k]);

					memcpy(pData + m_aAllGroups[idgroup]->m_aCountVertex[k], pData2, sizeof(vertex_static)* m_aAllGroups[idgroup]->m_aCountVertex[idbuff]);
					//memcpy(vborigin->arr + m_aAllGroups[idgroup]->CountVertex[k], m_aAllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr, sizeof(float3_t)* m_aAllGroups[idgroup]->CountVertex[idbuff]);

					vb->Unlock();
					m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Unlock();
					m_aAllGroups[idgroup]->m_aVertexBuffers[k]->Unlock();
					//}

					//обновляем данные во всех моделях которые есть в текущем буфере
					for (UINT m = 0; m < m_aAllGroups[idgroup]->m_aModels[idbuff].size(); ++m)
					{
						int tmpsizearrmodls = m_aAllGroups[idgroup]->m_aModels[idbuff].size();
						CModel* tmptmpmodel = m_aAllGroups[idgroup]->m_aModels[idbuff][m];
						//обновляем данные в структуре инфе о перемещенном буфере
						for(UINT j = 0; j < tmptmpmodel->m_aSubSets.size(); ++j)
						{
							if (tmptmpmodel->m_aSubSets[j].m_idGroup == idgroup)
							{
								//а именно, текущий буфер
								if (idbuff > k)
									tmptmpmodel->m_aSubSets[j].m_idBuff = k;
								else
									tmptmpmodel->m_aSubSets[j].m_idBuff = k - 1;

								//и стартовые позиции
								tmptmpmodel->m_aSubSets[j].m_idIndexStart += m_aAllGroups[idgroup]->m_aCountIndex[k];
								tmptmpmodel->m_aSubSets[j].m_idVertexStart += m_aAllGroups[idgroup]->m_aCountVertex[k];
								break;
							}
						}

						//обновляем все индексы модели текущей подгруппы перемещенного буфера
						//{
						Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> queue;
						queue.push_back(tmptmpmodel->m_pArrSplits);

						while (queue.size())
						{
							if (queue[0]->m_isNonEnd)
							{
								for (int j = 0; j < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
								{
									if (queue[0]->m_aSplits[j])
										queue.push_back(queue[0]->m_aSplits[j]);
								}
							}
							else
							{
								for(UINT j = 0; j < queue[0]->m_uiCountSubSet; ++j)
								{
									if (queue[0]->m_pNumberGroup[j] == idgroup)
									{
										for(UINT q = 0; q < queue[0]->m_pCountPoly[j] * 3; ++q)
										{
											queue[0]->m_ppArrPoly[j][q] += m_aAllGroups[idgroup]->m_aCountVertex[k];
										}
									}
								}
							}

							queue.erase(0);
						}
						//}
					}

					//обновляем информацию во всех моделях у которых такая же подгруппа, но которые находились в буферах "выше"
					//сообщаем что они стали ниже на единицу
					//{
					for(UINT j = idbuff + 1; j < m_aAllGroups[idgroup]->m_aModels.size(); ++j)
					{
						for(UINT q = 0; q < m_aAllGroups[idgroup]->m_aModels[j].size(); ++q)
						{
							for(UINT g = 0; g < m_aAllGroups[idgroup]->m_aModels[j][q]->m_aSubSets.size(); ++g)
							{
								if (m_aAllGroups[idgroup]->m_aModels[j][q]->m_aSubSets[g].m_idGroup == idgroup && m_aAllGroups[idgroup]->m_aModels[j][q]->m_aSubSets[g].m_idBuff > idbuff)
								{
									--(m_aAllGroups[idgroup]->m_aModels[j][q]->m_aSubSets[g].m_idBuff);
								}
							}
						}
					}
					//}

					//обновляем информацию в k буфере, по ходу удаляем информацию в idbuff буфере
					//{
					m_aAllGroups[idgroup]->m_aCountVertex[k] += m_aAllGroups[idgroup]->m_aCountVertex[idbuff];
					m_aAllGroups[idgroup]->m_aCountIndex[k] += m_aAllGroups[idgroup]->m_aCountIndex[idbuff];

					mem_release(m_aAllGroups[idgroup]->m_aVertexBuffers[k]);
					mem_release(m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]);
					m_aAllGroups[idgroup]->m_aVertexBuffers[k] = vb;

					/*mem_delete(m_aAllGroups[idgroup]->VertexBufferOrigin[k]);
					mem_delete(m_aAllGroups[idgroup]->VertexBufferOrigin[idbuff]);
					m_aAllGroups[idgroup]->VertexBufferOrigin[k] = vborigin;*/

					//перемещаем все модели из idbuff буфера в тот с которым совмещаем
					for(UINT m = 0; m < m_aAllGroups[idgroup]->m_aModels[idbuff].size(); ++m)
						m_aAllGroups[idgroup]->m_aModels[k].push_back(m_aAllGroups[idgroup]->m_aModels[idbuff][m]);

					m_aAllGroups[idgroup]->m_aCountVertex.erase(idbuff);
					m_aAllGroups[idgroup]->m_aCountIndex.erase(idbuff);

					m_aAllGroups[idgroup]->m_aModels.erase(idbuff);
					
					m_aAllGroups[idgroup]->m_aVertexBuffers.erase(idbuff);
					//m_aAllGroups[idgroup]->VertexBufferOrigin.erase(idbuff);
					//}
					
					//ПРЕРЫВАЕМ ЦИКЛ, ТАК КАК УЖЕ ОБЪЕДИНИЛИ И ЭТОГО ХВАТИТ
					break;
				}
			}
		}
	}

	//удаляем модель из просчетов, как объект, и из массива тоже
	deleteModelInArrCom(id);
	mem_delete(m_aAllModels[id]);
	m_aAllModels.erase(id);

	//реинициализируем буферы для индексных данных рендера
	initArrIndexPtr();
}

void CStaticGeom::setSplitID(CSegment* Split, ID* SplitsIDs, ID* SplitsIDsRender)
{
	Array<CSegment*> queue;
	int tmpcount = 0;
	queue.push_back(Split);

	while (queue.size())
	{
		if (queue[0] && queue[0]->m_isNonEnd)
		{
			queue[0]->m_ID = (*SplitsIDs);
			++(*SplitsIDs);
			for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; i++)
				queue.push_back(queue[0]->m_aSplits[i]);
		}
		else if (queue[0])
		{
			queue[0]->m_ID = (*SplitsIDs);
			queue[0]->m_SID = (*SplitsIDsRender);
			++(*SplitsIDs);
			++(*SplitsIDsRender);

			float3 tmpMin, tmpMax;

			queue[0]->m_pBoundVolumeP->getMinMax(&tmpMin, &tmpMax);
			SGCore_FCreateBoundingBoxMesh(&tmpMin, &tmpMax, &(queue[0]->m_pBoundBox));
		}
		
		queue.erase(0);
		++tmpcount;
	}
}

void CStaticGeom::deleteArrIndexPtr()
{
	if (m_pppRTArrIndices)
	{
		for(UINT i = 0, l = m_aAllGroups.size(); i < l; ++i)
		{
			for(UINT k = 0, lk = m_aAllGroups[i]->m_aVertexBuffers.size(); k < lk; ++k)
			{
				mem_delete_a(m_pppRTArrIndices[i][k]);
				mem_delete_a(m_pppRTCountDrawPolyModel[i][k]);
				mem_delete_a(m_pppRTBeginIndexModel[i][k]);
			}
			mem_delete_a(m_ppRTCountDrawPoly[i]);
			mem_delete_a(m_pppRTCountDrawPolyModel[i]);
			mem_delete_a(m_pppRTBeginIndexModel[i]);
			mem_delete_a(m_pppRTArrIndices[i]);
		}

		mem_delete_a(m_ppRTCountDrawPoly);
		mem_delete_a(m_pppRTCountDrawPolyModel);
		mem_delete_a(m_pppRTBeginIndexModel);
		mem_delete_a(m_pppRTArrIndices);
	}
}

void CStaticGeom::initArrIndexPtr()
{
	m_pppRTArrIndices = new uint32_t**[m_aAllGroups.size()];
	m_ppRTCountDrawPoly = new uint32_t*[m_aAllGroups.size()];
	m_pppRTCountDrawPolyModel = new uint32_t**[m_aAllGroups.size()];
	m_pppRTBeginIndexModel = new uint32_t**[m_aAllGroups.size()];

	for(UINT i = 0, l = m_aAllGroups.size(); i<l; i++)
	{
		m_ppRTCountDrawPoly[i] = new uint32_t[m_aAllGroups[i]->m_aVertexBuffers.size()];
		m_pppRTArrIndices[i] = new uint32_t*[m_aAllGroups[i]->m_aVertexBuffers.size()];
		m_pppRTCountDrawPolyModel[i] = new uint32_t*[m_aAllGroups[i]->m_aVertexBuffers.size()];
		m_pppRTBeginIndexModel[i] = new uint32_t*[m_aAllGroups[i]->m_aVertexBuffers.size()];

		for(UINT k = 0, kl = m_aAllGroups[i]->m_aVertexBuffers.size(); k < kl; ++k)
		{
			m_pppRTArrIndices[i][k] = new uint32_t[m_aAllGroups[i]->m_aCountIndex[k]];
			m_pppRTCountDrawPolyModel[i][k] = new uint32_t[m_aAllGroups[i]->m_aModels[k].size()];
			m_pppRTBeginIndexModel[i][k] = new uint32_t[m_aAllGroups[i]->m_aModels[k].size()];
		}
	}
}


void CStaticGeom::comArrIndeces(const IFrustum* frustum, const float3* viewpos, ID id_arr)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

	int tmpcount = 0;
	int* tmpcountcom = 0;
	CSegment** tmpsegments = 0;
	float jradius;
	float3 jcenter;

	for(UINT i = 0, l = m_aAllModels.size(); i < l; ++i)
	{
		if (id_arr == 0)
		{
			m_aAllModels[i]->m_pArrSplits->m_pBoundVolumeP->getSphere(&jcenter, &jradius);
			m_aAllModels[i]->m_pArrSplits->m_fDistForCamera = SMVector3Length((jcenter - (*viewpos))) - jradius;

			float3 vMin, vMax;
			m_aAllModels[i]->m_pArrSplits->m_pBoundVolumeP->getMinMax(&vMin, &vMax);
			if (!SGCore_OC_IsVisible(&vMax, &vMin))
				continue;
		}


		if (m_aAllModels[i]->m_pArrSplits->m_fDistForCamera >= CStaticGeom::m_fDistForLod && m_aAllModels[i]->m_oLod0.m_pModel)
			m_aAllModels[i]->m_isRenderLod = true;
		else
		{
			m_aAllModels[i]->m_isRenderLod = false;
			tmpcount = m_aArrComFor[id_arr]->m_aIRS[i]->m_iCount;
			m_aArrComFor[id_arr]->m_aIRS[i]->m_iCountCom = 0;
			tmpcountcom = &(m_aArrComFor[id_arr]->m_aIRS[i]->m_iCountCom);
			tmpsegments = m_aArrComFor[id_arr]->m_aIRS[i]->m_ppSegments;

			m_aArrComFor[id_arr]->m_aQueue.push_back(m_aAllModels[i]->m_pArrSplits);

			while (m_aArrComFor[id_arr]->m_aQueue.size())
			{
				comRecArrIndeces(id_arr, frustum, tmpsegments, tmpcountcom, m_aArrComFor[id_arr]->m_aQueue[0], viewpos, &m_aArrComFor[id_arr]->m_aQueue, tmpcount);

				m_aArrComFor[id_arr]->m_aQueue.erase(0);
				++tmpcount;
			}

			m_aArrComFor[id_arr]->m_aQueue.clear();
		}

		int qwert = 0;
	}
}

void CStaticGeom::comRecArrIndeces(ID idArr, const IFrustum* frustum, CSegment** arrsplits, int *count, CSegment* comsegment, const float3* viewpos, Array<CSegment*, GEOM_DEFAULT_RESERVE_COM>* queue, ID curr_splits_ids_render)
{
	float jradius;
	float3 jcenter;
	comsegment->m_pBoundVolumeP->getSphere(&jcenter, &jradius);

	if (comsegment->m_uiCountAllPoly > 0 && (frustum->sphereInFrustum(&jcenter, jradius)))
	{
			if (comsegment->m_isNonEnd )
			{
				if (CStaticGeom::m_isUseSortFrontToBackSplits)
				{
					int SortId[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];
					float DistFor[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];

					for (int q = 0; q<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++q)
					{
						SortId[q] = -1;
						if (comsegment->m_aSplits[q])
						{
							comsegment->m_aSplits[q]->m_pBoundVolumeP->getSphere(&jcenter, &jradius);
							DistFor[q] = SMVector3Length2((jcenter - (*viewpos))) - jradius*jradius;
						}
					}

					float pl1, pl2;
					int tmpCountGreater = 0;
					for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
					{
						if (comsegment->m_aSplits[i])
						{
							tmpCountGreater = 0;
							pl1 = DistFor[i];

							for (int k = 0; k<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++k)
							{
								if (comsegment->m_aSplits[k])
								{
									pl2 = DistFor[k];

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

					for (int j = 0; j<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
					{
						if (SortId[(GEOM_COUNT_TYPE_SEGMENTATION_OCTO - 1) - j] != -1 && comsegment->m_aSplits[SortId[(GEOM_COUNT_TYPE_SEGMENTATION_OCTO - 1) - j]])
							queue->push_back(comsegment->m_aSplits[SortId[(GEOM_COUNT_TYPE_SEGMENTATION_OCTO - 1) - j]]);
					}
				}
				else
				{
					for (int j = 0; j<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
					{
						if (comsegment->m_aSplits[j])
							queue->push_back(comsegment->m_aSplits[j]);
					}
				}
			}
			else
			{
				if ((*count) < curr_splits_ids_render)
				{
					bool isVisible = true;
					
					if (idArr == 0)
					{
						float3 vMin, vMax;
						comsegment->m_pBoundVolumeP->getMinMax(&vMin, &vMax);
						isVisible = SGCore_OC_IsVisible(&vMax, &vMin);
					}
					
					if (isVisible)
					{
						arrsplits[(*count)] = comsegment;
						comsegment->m_fDistForCamera = SMVector3Length((jcenter - (*viewpos))) - jradius;
						(*count)++;
					}
					else
						int qwerty = 0;
				}
				/*else
				{
					int qwert = 0;
				}*/
			}
	}
}

bool CStaticGeom::sortExistsForRender(int sort, ID id_arr)
{
	//валиден ли id_arr?
	STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	CSegment** jarrsplits;
	ID jidbuff;
	ID jnumgroup;

	//проходимся по всем моделям
	for (UINT i = 0, l = m_aAllModels.size(); i < l; ++i)
	{
		if (m_aArrComFor[id_arr]->m_aIRS[i]->m_iCountCom > 0)
		{
			for(UINT j = 0, jl = m_aArrComFor[id_arr]->m_aIRS[i]->m_iCountCom; j<jl; j++)
			{
				jarrsplits = m_aArrComFor[id_arr]->m_aIRS[i]->m_ppSegments;
				for(UINT k = 0; k<jarrsplits[j]->m_uiCountSubSet; ++k)
				{
					jidbuff = m_aAllModels[i]->m_aSubSets[jarrsplits[j]->m_pNumberGroupModel[k]].m_idBuff;
					jnumgroup = jarrsplits[j]->m_pNumberGroup[k];

					if (SGCore_MtlGetSort(m_aAllGroups[jnumgroup]->m_idTexture) & sort)
						return true;
				}
			}
		}
	}

	return false;
}

void CStaticGeom::render(DWORD timeDelta, int sort_mtl, ID id_arr, ID exclude_model_id, ID exclude_group_id, bool is_sorted)
{
	//валиден ли id_arr?
	STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	CSegment** jarrsplits;
	ID jidbuff;
	ID jnumgroup;
	
	//обнуляем все данные об отрисованном в прошлый раз
	for(UINT i = 0, l = m_aAllGroups.size(); i < l; ++i)
	{
		if (m_aAllGroups[i])
		{
			for(UINT k = 0, kl = m_aAllGroups[i]->m_aVertexBuffers.size(); k < kl; ++k)
			{
				m_ppRTCountDrawPoly[i][k] = 0;

				for(UINT j = 0, jl = m_aAllGroups[i]->m_aModels[k].size(); j < jl; ++j)
				{
					m_pppRTCountDrawPolyModel[i][k][j] = 0;
					m_pppRTBeginIndexModel[i][k][j] = -1;
				}
			}
		}
	}
	
	//проходимся по всем моделям
	for(int i = 0, l = m_aAllModels.size(); i < l; ++i)
	{
		if (exclude_model_id == i && exclude_group_id < 0)
			continue;

		if (m_aAllModels[i]->m_isRenderLod)
		{
			CStaticGeom::m_pDXDevice->SetStreamSource(0, m_aAllModels[i]->m_oLod0.m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static));
			CStaticGeom::m_pDXDevice->SetIndices(m_aAllModels[i]->m_oLod0.m_pModel->m_pIndexBuffer);
			CStaticGeom::m_pDXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());
			for (int k = 0, kl = m_aAllModels[i]->m_oLod0.m_pModel->m_uiSubsetCount; k < kl; ++k)
			{
				if ((m_aAllModels[i]->m_oLod0.m_iSortGroup & sort_mtl) || sort_mtl == -1)
				{
					SGCore_MtlSet(m_aAllModels[i]->m_oLod0.m_aIDsTextures[k], 0);
					SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aAllModels[i]->m_oLod0.m_pModel->m_pVertexCount[k], m_aAllModels[i]->m_oLod0.m_pModel->m_pStartIndex[k], m_aAllModels[i]->m_oLod0.m_pModel->m_pIndexCount[k] / 3);
					Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_aAllModels[i]->m_oLod0.m_pModel->m_pIndexCount[k] / 3);
				}
			}
		}
		else if (m_aArrComFor[id_arr]->m_aIRS[i]->m_iCountCom > 0)
		{
			for (DWORD j = 0, jl = m_aArrComFor[id_arr]->m_aIRS[i]->m_iCountCom; j<jl; j++)
			{
				jarrsplits = m_aArrComFor[id_arr]->m_aIRS[i]->m_ppSegments;
				for (DWORD k = 0; k<jarrsplits[j]->m_uiCountSubSet; ++k)
				{
					jidbuff = m_aAllModels[i]->m_aSubSets[jarrsplits[j]->m_pNumberGroupModel[k]].m_idBuff;
					jnumgroup = jarrsplits[j]->m_pNumberGroup[k];

					if (exclude_model_id >= 0 && exclude_group_id == jarrsplits[j]->m_pNumberGroupModel[k])
						continue;

					ID tmpcurrmodel = -1;

					for(int q = 0, ql = m_aAllGroups[jnumgroup]->m_aModels[jidbuff].size(); q<ql; ++q)
					{
						if (m_aAllGroups[jnumgroup]->m_aModels[jidbuff][q] == m_aAllModels[i])
						{
							tmpcurrmodel = q;
							break;
						}
					}

					if (tmpcurrmodel == -1)
					{
						int qwert = 0;
					}

					int currsort = m_aAllGroups[jnumgroup]->m_iSortGroup;

					if ((m_aAllGroups[jnumgroup]->m_iSortGroup & sort_mtl) || sort_mtl == -1)
					{
						if (
							jarrsplits[j]->m_pCountPoly[k] > 0 &&	//если количество полигонов больше 0
							m_ppRTCountDrawPoly[jnumgroup][jidbuff] + jarrsplits[j]->m_pCountPoly[k] <= (UINT)m_aAllGroups[jnumgroup]->m_aCountIndex[jidbuff] / 3 	//если количество записанных полигонов в данную подгруппу меньше либо равно общему количеству полигонов которое содержит данна¤ подгруппа
							)
						{
							memcpy(m_pppRTArrIndices[jnumgroup][jidbuff] + (m_ppRTCountDrawPoly[jnumgroup][jidbuff] * 3),
								jarrsplits[j]->m_ppArrPoly[k],
								jarrsplits[j]->m_pCountPoly[k] * sizeof(uint32_t)* 3);

							if (m_pppRTBeginIndexModel[jnumgroup][jidbuff][tmpcurrmodel] == -1)
							{
								m_pppRTBeginIndexModel[jnumgroup][jidbuff][tmpcurrmodel] = m_ppRTCountDrawPoly[jnumgroup][jidbuff];
							}

							m_ppRTCountDrawPoly[jnumgroup][jidbuff] += jarrsplits[j]->m_pCountPoly[k];
							m_pppRTCountDrawPolyModel[jnumgroup][jidbuff][tmpcurrmodel] += jarrsplits[j]->m_pCountPoly[k];

							/*if (jarrsplits[j]->m_pBoundBox)
							{
								CStaticGeom::m_pDXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
								//CStaticGeom::m_pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
								CStaticGeom::m_pDXDevice->SetTexture(0, 0);
								jarrsplits[j]->m_pBoundBox->DrawSubset(0);
								//CStaticGeom::m_pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
								CStaticGeom::m_pDXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
							}*/
						}
					}
				}
			}
		}
	}

	
	uint32_t* RTGPUArrIndicesPtrs2;

	//если производим рендер не по отсортированному списку
	if (!is_sorted)
	{
		//проходимся по всем подгруппам
		for(int i = 0, l = m_aAllGroups.size(); i < l; ++i)
		{
			CGroup* tmpgroup = m_aAllGroups[i];
			if (!tmpgroup || tmpgroup->m_aCountVertex.size() <= 0 || !((tmpgroup->m_iSortGroup & sort_mtl) || sort_mtl == -1))
				continue;

			//проходимся по всем буферам
			for (int k = 0, kl = tmpgroup->m_aVertexBuffers.size(); k<kl; ++k)
			{
				//если есть что к отрисовке
				if (m_ppRTCountDrawPoly[i][k] > 0)
				{
					m_pRenderIndexBuffer->Lock(0, 0, (void**)&(RTGPUArrIndicesPtrs2), D3DLOCK_DISCARD);
					memcpy(RTGPUArrIndicesPtrs2, m_pppRTArrIndices[i][k], m_ppRTCountDrawPoly[i][k] * 3 * sizeof(uint32_t));
					m_pRenderIndexBuffer->Unlock();

					CStaticGeom::m_pDXDevice->SetStreamSource(0, tmpgroup->m_aVertexBuffers[k], 0, sizeof(vertex_static));
					CStaticGeom::m_pDXDevice->SetIndices(m_pRenderIndexBuffer);
					CStaticGeom::m_pDXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());

					//если на данную подгруппу назначен раздельный рендер
					if (tmpgroup->m_isRenderSingly)
					{
						int tmpprevcountindex = 0;

						for(UINT j = 0, jl = tmpgroup->m_aModels[k].size(); j < jl; ++j)
						{
							if (m_pppRTCountDrawPolyModel[i][k][j] > 0)
							{
								//ищем id материала подгруппы
								//общий id использовать нельзя так как не зря стоит раздельный рендер
								for(UINT q = 0; q < tmpgroup->m_aModels[k][j]->m_aIDsTexures.size(); ++q)
								{
									if (i == tmpgroup->m_aModels[k][j]->m_aSubSets[q].m_idGroup)
									{
										SGCore_MtlSet(tmpgroup->m_aModels[k][j]->m_aIDsTexures[q], 0);
										break;
									}
								}
								
								SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, tmpgroup->m_aCountVertex[k], tmpprevcountindex * 3, m_pppRTCountDrawPolyModel[i][k][j]);
								tmpprevcountindex += m_pppRTCountDrawPolyModel[i][k][j];
								Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_pppRTCountDrawPolyModel[i][k][j]);
							}
						}
					}
					else
					{
						SGCore_MtlSet(tmpgroup->m_idTexture, 0);
						SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, tmpgroup->m_aCountVertex[k], 0, m_ppRTCountDrawPoly[i][k]);
						Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_ppRTCountDrawPoly[i][k]);
					}
				}
			}
		}
	}
	//иначе рисуем по сортированному списку
	else
	{
		for(UINT i = 0, l = m_aDistGroup.size(); i < l; ++i)
		{
			CInfoGroup* tmpig = m_aDistGroup[i];
			CGroup* tmpgroup = m_aAllGroups[tmpig->m_idGlobalGroup];

			if (!tmpgroup || tmpgroup->m_aCountVertex.size() <= 0 || !((tmpgroup->m_iSortGroup & sort_mtl) || sort_mtl == -1))
				continue;

			if (exclude_model_id == tmpig->m_idModel && (exclude_group_id < 0 || exclude_group_id == tmpig->m_idGroup))
				continue;

			ID id_buff = m_aAllModels[tmpig->m_idModel]->m_aSubSets[tmpig->m_idGroup].m_idBuff;

			if (m_ppRTCountDrawPoly[tmpig->m_idGlobalGroup][id_buff] > 0)
			{
				ID id_model;
				for(UINT j = 0, jl = tmpgroup->m_aModels[id_buff].size(); j < jl; ++j)
				{
					if (tmpgroup->m_aModels[id_buff][j] == m_aAllModels[tmpig->m_idModel])
					{
						id_model = j;
						break;
					}
				}

				m_pRenderIndexBuffer->Lock(0, 0, (void**)&(RTGPUArrIndicesPtrs2), D3DLOCK_DISCARD);
				memcpy(RTGPUArrIndicesPtrs2, m_pppRTArrIndices[tmpig->m_idGlobalGroup][id_buff], m_ppRTCountDrawPoly[tmpig->m_idGlobalGroup][id_buff]/*m_pppRTCountDrawPolyModel[tmpig->m_idGlobalGroup][id_buff][id_model]*/ * 3 * sizeof(uint32_t));
				m_pRenderIndexBuffer->Unlock();

				CStaticGeom::m_pDXDevice->SetStreamSource(0, tmpgroup->m_aVertexBuffers[id_buff], 0, sizeof(vertex_static));
				CStaticGeom::m_pDXDevice->SetIndices(m_pRenderIndexBuffer);
				CStaticGeom::m_pDXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());



				int tmpprevcountindex = m_aAllModels[tmpig->m_idModel]->m_aSubSets[tmpig->m_idGroup].m_idIndexStart / 3;
				

				if (m_pppRTCountDrawPolyModel[tmpig->m_idGlobalGroup][id_buff][id_model] > 0)
				{
					//ищем id материала подгруппы
					for(UINT q = 0, ql = tmpgroup->m_aModels[id_buff][id_model]->m_aIDsTexures.size(); q < ql; ++q)
					{
						if (tmpig->m_idGlobalGroup == tmpgroup->m_aModels[id_buff][id_model]->m_aSubSets[q].m_idGroup)
						{
							SGCore_MtlSet(tmpgroup->m_aModels[id_buff][id_model]->m_aIDsTexures[q], 0);
							break;
						}
					}
					SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, tmpgroup->m_aCountVertex[id_buff], m_pppRTBeginIndexModel[tmpig->m_idGlobalGroup][id_buff][id_model] * 3, m_pppRTCountDrawPolyModel[tmpig->m_idGlobalGroup][id_buff][id_model]);
					tmpprevcountindex += m_pppRTCountDrawPolyModel[tmpig->m_idGlobalGroup][id_buff][id_model];
					Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_pppRTCountDrawPolyModel[tmpig->m_idGlobalGroup][id_buff][id_model]);
				}
			}
		}
	}
}

void CStaticGeom::renderSingly(DWORD timeDelta, ID id, ID id_tex)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id, _VOID);
	CSegment** jarrsplits;
	int jidbuff;
	int jnumgroup;

	//обнуляем все данные об отрисованном в прошлый раз
	for(UINT i = 0, l = m_aAllGroups.size(); i < l; ++i)
	{
		if (m_aAllGroups[i])
		{
			for(UINT k = 0, kl = m_aAllGroups[i]->m_aVertexBuffers.size(); k < kl; ++k)
			{
				m_ppRTCountDrawPoly[i][k] = 0;

				for(UINT j = 0, jl = m_aAllGroups[i]->m_aModels[k].size(); j < jl; ++j)
				{
					m_pppRTCountDrawPolyModel[i][k][j] = 0;
					m_pppRTBeginIndexModel[i][k][j] = -1;
				}
			}
		}
	}


	if (m_aAllModels[id]->m_isRenderLod)
	{
		CStaticGeom::m_pDXDevice->SetStreamSource(0, m_aAllModels[id]->m_oLod0.m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static));
		CStaticGeom::m_pDXDevice->SetIndices(m_aAllModels[id]->m_oLod0.m_pModel->m_pIndexBuffer);
		CStaticGeom::m_pDXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());
		for (int k = 0, kl = m_aAllModels[id]->m_oLod0.m_pModel->m_uiSubsetCount; k < kl; ++k)
		{
			SGCore_MtlSet((id_tex > 0 ? id_tex : m_aAllModels[id]->m_oLod0.m_aIDsTextures[k]), 0);
			SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aAllModels[id]->m_oLod0.m_pModel->m_pVertexCount[k], m_aAllModels[id]->m_oLod0.m_pModel->m_pStartIndex[k], m_aAllModels[id]->m_oLod0.m_pModel->m_pIndexCount[k] / 3);
			Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_aAllModels[id]->m_oLod0.m_pModel->m_pIndexCount[k] / 3);
		}
	}
	else if (m_aArrComFor[0]->m_aIRS[id]->m_iCountCom > 0)
	{
		for (DWORD j = 0, jl = m_aArrComFor[0]->m_aIRS[id]->m_iCountCom; j < jl; j++)
		{
			jarrsplits = m_aArrComFor[0]->m_aIRS[id]->m_ppSegments;
			for (DWORD k = 0; k < jarrsplits[j]->m_uiCountSubSet; ++k)
			{
				jidbuff = m_aAllModels[id]->m_aSubSets[jarrsplits[j]->m_pNumberGroupModel[k]].m_idBuff;
				jnumgroup = jarrsplits[j]->m_pNumberGroup[k];

				ID tmpcurrmodel = -1;

				for (int q = 0, ql = m_aAllGroups[jnumgroup]->m_aModels[jidbuff].size(); q < ql; ++q)
				{
					if (m_aAllGroups[jnumgroup]->m_aModels[jidbuff][q] == m_aAllModels[id])
					{
						tmpcurrmodel = q;
						break;
					}
				}

				int currsort = m_aAllGroups[jnumgroup]->m_iSortGroup;

				
					if (
						jarrsplits[j]->m_pCountPoly[k] > 0 &&	//если количество полигонов больше 0
						m_ppRTCountDrawPoly[jnumgroup][jidbuff] + jarrsplits[j]->m_pCountPoly[k] <= (UINT)m_aAllGroups[jnumgroup]->m_aCountIndex[jidbuff] / 3 	//если количество записанных полигонов в данную подгруппу меньше либо равно общему количеству полигонов которое содержит данна¤ подгруппа
						)
					{
						memcpy(m_pppRTArrIndices[jnumgroup][jidbuff] + (m_ppRTCountDrawPoly[jnumgroup][jidbuff] * 3),
							jarrsplits[j]->m_ppArrPoly[k],
							jarrsplits[j]->m_pCountPoly[k] * sizeof(uint32_t)* 3);

						if (m_pppRTBeginIndexModel[jnumgroup][jidbuff][tmpcurrmodel] == -1)
						{
							m_pppRTBeginIndexModel[jnumgroup][jidbuff][tmpcurrmodel] = m_ppRTCountDrawPoly[jnumgroup][jidbuff];
						}

						m_ppRTCountDrawPoly[jnumgroup][jidbuff] += jarrsplits[j]->m_pCountPoly[k];
						m_pppRTCountDrawPolyModel[jnumgroup][jidbuff][tmpcurrmodel] += jarrsplits[j]->m_pCountPoly[k];
					}
			}
		}
	}


	uint32_t* RTGPUArrIndicesPtrs2;
	
	for(UINT i = 0, l = m_aAllModels[id]->m_aSubSets.size(); i < l; ++i)
	{
		ID idgroup = m_aAllModels[id]->m_aSubSets[i].m_idGroup;
		CGroup* tmpgroup = m_aAllGroups[m_aAllModels[id]->m_aSubSets[i].m_idGroup];
		ID id_buff = m_aAllModels[id]->m_aSubSets[i].m_idBuff;

		ID id_model;
		for(UINT j = 0, jl = tmpgroup->m_aModels[id_buff].size(); j < jl; ++j)
		{
			if (tmpgroup->m_aModels[id_buff][j] == m_aAllModels[id])
			{
				id_model = j;
				break;
			}
		}

		m_pRenderIndexBuffer->Lock(0, 0, (void**)&(RTGPUArrIndicesPtrs2), D3DLOCK_DISCARD);
		memcpy(RTGPUArrIndicesPtrs2, m_pppRTArrIndices[idgroup][id_buff], m_ppRTCountDrawPoly[idgroup][id_buff]/*m_pppRTCountDrawPolyModel[tmpig->m_idGlobalGroup][id_buff][id_model]*/ * 3 * sizeof(uint32_t));
		m_pRenderIndexBuffer->Unlock();

		CStaticGeom::m_pDXDevice->SetStreamSource(0, tmpgroup->m_aVertexBuffers[id_buff], 0, sizeof(vertex_static));
		CStaticGeom::m_pDXDevice->SetIndices(m_pRenderIndexBuffer);
		CStaticGeom::m_pDXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());

		SGCore_MtlSet((id_tex > 0 ? id_tex : tmpgroup->m_idTexture), 0);

		SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, tmpgroup->m_aCountVertex[id_buff], 0, m_ppRTCountDrawPoly[idgroup][id_buff]);
		Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + m_ppRTCountDrawPoly[idgroup][id_buff]);
	}
}

void CStaticGeom::preSegmentation(CModel* mesh, ISXDataStaticModel* model)
{
	vertex_static *CreateV;
	mem_delete_a(m_pCurrArrMeshVertex);
	mem_delete_a(m_pCurrArrMeshIndex);
	//CountVertex = model->m_uiAllVertexCount;
	//копируем все вершины для текущих расчетов
	m_pCurrArrMeshVertex = new float3[model->m_uiAllVertexCount];

	model->m_pVertexBuffer->Lock(0, 0, (void **)&CreateV, 0);
	for (DWORD i = 0; i<model->m_uiAllVertexCount; i++)
	{
		m_pCurrArrMeshVertex[i] = (CreateV[i].Pos.operator float3());
	}
	model->m_pVertexBuffer->Unlock();

	m_pCurrArrMeshIndex = new UINT[model->m_uiAllIndexCount];
	UINT* pInd;
	model->m_pIndexBuffer->Lock(0, 0, (void **)&pInd, 0);
	memcpy(m_pCurrArrMeshIndex, pInd, sizeof(UINT)* model->m_uiAllIndexCount);
	model->m_pIndexBuffer->Unlock();

	///////

	mesh->m_pArrSplits = new CSegment();
	mesh->m_pArrSplits->m_uiCountAllPoly = model->m_uiAllIndexCount / 3;
	LibReport(REPORT_MSG_LEVEL_NOTICE, "poly: %d, ", mesh->m_pArrSplits->m_uiCountAllPoly);

	mesh->m_pArrSplits->m_pBoundVolumeP = SGCore_CrBound();
	mesh->m_pArrSplits->m_pBoundVolumeP->calcBound(model->m_pVertexBuffer, model->m_uiAllVertexCount, sizeof(vertex_static));

	float3 tmpMin, tmpMax;
	mesh->m_pArrSplits->m_pBoundVolumeP->getMinMax(&tmpMin, &tmpMax);

	int CountSplitsSys = 0;
	int CountPolyInSegment = GEOM_MIN_COUNT_POLY;

	float3 dimensions = tmpMax - tmpMin;	//габариты
#if 1
	float allvolume = dimensions.x * dimensions.y * dimensions.z;
	const float minVol = GEOM_MIN_ALLVOLUME_FOR_SEGMENTATION;
	const float minLen = GEOM_MIN_LENGTH_FOR_SEGMENTATION;
	const float minOctoHeight = GEOM_MIN_HEIGHT_FOR_SEGMENTATION;
	if ((allvolume >= minVol || dimensions.x >= minLen || dimensions.y >= minOctoHeight || dimensions.z >= minLen) && (model->m_uiAllIndexCount >= GEOM_MIN_POLYGONS_FOR_SEGMENTATION * 3 || allvolume >= GEOM_FORCE_ALLVOLUME_FOR_SEGMENTATION))
	{
		if(dimensions.y >= minOctoHeight)
		{
			CountSplitsSys = GEOM_COUNT_TYPE_SEGMENTATION_OCTO;
			LibReport(REPORT_MSG_LEVEL_NOTICE, " div: octo, ");
		}
		else
		{
			CountSplitsSys = GEOM_COUNT_TYPE_SEGMENTATION_QUAD;
			LibReport(REPORT_MSG_LEVEL_NOTICE, " div: quad, ");

		}
			
		float4_t tmpmin(dimensions.x / minLen, dimensions.y / minOctoHeight, dimensions.z / minLen, (float)model->m_uiAllIndexCount / (float)GEOM_MIN_POLYGONS_FOR_SEGMENTATION / 3.0f);
		tmpmin.x = min(min(min(tmpmin.x, tmpmin.y), min(tmpmin.z, tmpmin.w)), (allvolume / minVol)) * 10.0f;

		CountPolyInSegment = (float)model->m_uiAllIndexCount / 3.0f / tmpmin.x / CountSplitsSys;

		if (CountPolyInSegment < GEOM_MIN_COUNT_POLY)
		{
			CountPolyInSegment = GEOM_MIN_COUNT_POLY;
		}

		if (CountPolyInSegment > GEOM_MAX_COUNT_POLY)
		{
			CountPolyInSegment = GEOM_MAX_COUNT_POLY;
		}

		LibReport(REPORT_MSG_LEVEL_NOTICE, "poly in split: %d, ", CountPolyInSegment);
	}

#else
		float allvolume = (dimensions.x + dimensions.y + dimensions.z) * 0.33f;	//общий средний объем

		//если общий средний объем больше либо равно чем минимальный подлжеащий делению и количество полигонов больше либо равно чем минимум подлжеащий делению
		if(allvolume >= STATIC_MIN_ALLVOLUME_FOR_SEGMENTATION && model->AllIndexCount / 3 >= STATIC_MIN_POLYGONS_FOR_SEGMENTATION)
		{
			//если разница сторон меньше чем минимально допустимое
			if(
				(dimensions.x / dimensions.y >= STATIC_DIFFERENCE_SIDES_FOR_OCTO && dimensions.x / dimensions.y <= 1 + STATIC_DIFFERENCE_SIDES_FOR_OCTO) ||
				(dimensions.z / dimensions.y >= STATIC_DIFFERENCE_SIDES_FOR_OCTO && dimensions.z / dimensions.y <= 1 + STATIC_DIFFERENCE_SIDES_FOR_OCTO)
				)
			{
				//делим как octo дерево
				CountSplitsSys = GEOM_COUNT_TYPE_SEGMENTATION_OCTO;
				LibReport(REPORT_MSG_LEVEL_NOTICE, " div: octo, ");
			}
			else
			{
				CountSplitsSys = STATIC_COUNT_TYPE_SEGMENTATION_QUAD;
				LibReport(REPORT_MSG_LEVEL_NOTICE, " div: quad, ");
			}

			//определяем коэфициент интерполяции для определния минимального количества полигонов в сплите
			float tmpcoef = float(allvolume * 3) / (float)(model->AllIndexCount / 3);
			if(tmpcoef > 1.f)
				tmpcoef = 1;
			else if(tmpcoef < 0.f)
				tmpcoef = 0;
			CountPolyInSegment = lerpf(STATIC_MIN_COUNT_POLY, STATIC_MAX_COUNT_POLY, tmpcoef);
			LibReport(REPORT_MSG_LEVEL_NOTICE, "poly in split: %d, ", CountPolyInSegment);
		}
#endif

	float tmpX = dimensions.x;
	float tmpY = dimensions.y;
	float tmpZ = dimensions.z;

	//выравниваем для равномерного делени
	//{
	if (CountSplitsSys == GEOM_COUNT_TYPE_SEGMENTATION_OCTO)
	{
		if (tmpX > tmpY && tmpX > tmpZ)
		{
			tmpY = tmpX - tmpY;
			tmpY /= 2.f;
			tmpMax.y += tmpY;
			tmpMin.y -= tmpY;

			tmpZ = tmpX - tmpZ;
			tmpZ /= 2.f;
			tmpMax.z += tmpZ;
			tmpMin.z -= tmpZ;
		}
		else if (tmpY > tmpX && tmpY > tmpZ)
		{
			tmpX = tmpY - tmpX;
			tmpX /= 2.f;
			tmpMax.x += tmpX;
			tmpMin.x -= tmpX;

			tmpZ = tmpY - tmpZ;
			tmpZ /= 2.f;
			tmpMax.z += tmpZ;
			tmpMin.z -= tmpZ;
		}
		else if (tmpZ > tmpX && tmpZ > tmpY)
		{
			tmpX = tmpZ - tmpX;
			tmpX /= 2.f;
			tmpMax.x += tmpX;
			tmpMin.x -= tmpX;

			tmpY = tmpZ - tmpY;
			tmpY /= 2.f;
			tmpMax.y += tmpY;
			tmpMin.y -= tmpY;
		}
	}
	else if (CountSplitsSys == GEOM_COUNT_TYPE_SEGMENTATION_QUAD)
	{
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
	}

	mesh->m_pArrSplits->m_pBoundVolumeSys = SGCore_CrBound();
	mesh->m_pArrSplits->m_pBoundVolumeSys->setMinMax(&tmpMin, &tmpMax);
	//}}

	mesh->m_pArrSplits->m_uiCountSubSet = model->m_uiSubsetCount;
	mesh->m_pArrSplits->m_pNumberGroup = new uint32_t[model->m_uiSubsetCount];
	mesh->m_pArrSplits->m_pNumberGroupModel = new uint32_t[model->m_uiSubsetCount];
	mesh->m_pArrSplits->m_pCountPoly = new uint32_t[model->m_uiSubsetCount];

	/////

	//заполняем массив с номаерами подгрупп и массив с количествами полигонов
	for (DWORD i = 0; i<model->m_uiSubsetCount; i++)
	{
		mesh->m_pArrSplits->m_pNumberGroupModel[i] = i;
		mesh->m_pArrSplits->m_pNumberGroup[i] = i;
		mesh->m_pArrSplits->m_pCountPoly[i] = model->m_pIndexCount[i] / 3;
	}

	//создаем массивы с полигонами, каждый полигон принадлежит своей подгруппе
	mesh->m_pArrSplits->m_ppArrPoly = new uint32_t*[mesh->m_pArrSplits->m_uiCountSubSet];
	for (DWORD i = 0; i<mesh->m_pArrSplits->m_uiCountSubSet; i++)
	{
		mesh->m_pArrSplits->m_ppArrPoly[i] = new uint32_t[mesh->m_pArrSplits->m_pCountPoly[i] * 3];
	}

	//заполняем массивы с полигонами
	for (DWORD i = 0; i<model->m_uiSubsetCount; i++)
	{
		memcpy(mesh->m_pArrSplits->m_ppArrPoly[i], m_pCurrArrMeshIndex + model->m_pStartIndex[i], model->m_pIndexCount[i] * sizeof(uint32_t));
	}

	//если количество полигонов и текущая установка деления позволяют делить дальше
	if (mesh->m_pArrSplits->m_uiCountAllPoly >= (UINT)CountPolyInSegment && CountSplitsSys != 0)
	{
		mesh->m_pArrSplits->m_isNonEnd = true;
		cycleSegmentation(mesh->m_pArrSplits, mesh, model, CountSplitsSys, CountPolyInSegment);
	}
	else
	{
		//оптимизация для Post TnL кэша
		//{{
		Array<DWORD> tmpVert;
		bool isunic = true;
		for (DWORD q = 0; q<mesh->m_pArrSplits->m_uiCountSubSet; q++)
		{
			tmpVert.clear();
			tmpVert.push_back(mesh->m_pArrSplits->m_ppArrPoly[q][0]);
			for (DWORD k = 0; k<mesh->m_pArrSplits->m_pCountPoly[q] * 3; k++)
			{
				isunic = true;
				for (DWORD j = 0; j<tmpVert.size() && isunic; j++)
				{
					if (mesh->m_pArrSplits->m_ppArrPoly[q][k] == tmpVert[j])
					{
						isunic = false;
					}
				}

				if (isunic)
					tmpVert.push_back(mesh->m_pArrSplits->m_ppArrPoly[q][k]);
			}

			SGCore_OptimizeIndecesInSubsetUint32(mesh->m_pArrSplits->m_ppArrPoly[q], mesh->m_pArrSplits->m_pCountPoly[q], tmpVert.size());
		}
		//}}

		//иначе нельзя делить дальше
		//приводим индексы куска к глобальному виду уровня
		for (DWORD i = 0; i<mesh->m_pArrSplits->m_uiCountSubSet; i++)
		{
			for (DWORD k = 0; k<mesh->m_pArrSplits->m_pCountPoly[i] * 3; k++)
			{
				mesh->m_pArrSplits->m_ppArrPoly[i][k] -= model->m_pStartVertex[mesh->m_pArrSplits->m_pNumberGroup[i]];
				mesh->m_pArrSplits->m_ppArrPoly[i][k] += mesh->m_aSubSets[mesh->m_pArrSplits->m_pNumberGroup[i]].m_idVertexStart;
			}
			mesh->m_pArrSplits->m_pNumberGroup[i] = mesh->m_aSubSets[mesh->m_pArrSplits->m_pNumberGroup[i]].m_idGroup;
		}

		mesh->m_pArrSplits->m_isNonEnd = false;
	}
}

void CStaticGeom::cycleSegmentation(CSegment *Split, CModel* mesh, ISXDataStaticModel* model, int CountSplitsSys, int CountPolyInSegment)
{
	Array<CSegment*> queue;
	int tmpcount = 0;
	queue.push_back(Split);

	while (queue.size())
	{
		if (queue[0]->m_isNonEnd)
		{
			segmentation(queue[0], mesh, model, CountSplitsSys, CountPolyInSegment, &queue);

			for (int i = 0; i < CountSplitsSys; i++)
			{
				if (queue[0]->m_aSplits[i])
					queue.push_back(queue[0]->m_aSplits[i]);
			}
		}

		queue.erase(0);
		++tmpcount;
	}
}


void CStaticGeom::segmentation(CSegment* Split, CModel* mesh, ISXDataStaticModel* model, int CountSplitsSys, int CountPolyInSegment, Array<CSegment*> * queue)
{
	Array<DWORD> ArrPoly[8];
	Array<DWORD> ArrGroup[8];
	Array<DWORD> ArrSerialNum[8];

	for (int i = 0; i<CountSplitsSys; i++)
	{
		Split->m_aSplits[i] = new CSegment();
	}

	//получаем ограничивающие для деления и облегаемые объемы
	//{{
	if (CountSplitsSys == GEOM_COUNT_TYPE_SEGMENTATION_QUAD)
	{
		ISXBound* ArrBound[GEOM_COUNT_TYPE_SEGMENTATION_QUAD];
		for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_QUAD; ++i)
		{
			ArrBound[i] = SGCore_CrBound();
		}
		float3 tmpmin,tmpmax;
		SGCore_0ComBoundBoxArr4(Split->m_pBoundVolumeSys, (ArrBound));
		for (int i = 0; i<CountSplitsSys; i++)
		{
			Split->m_aSplits[i]->m_pBoundVolumeSys = ArrBound[i];
			Split->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&tmpmin, &tmpmax);
			Split->m_aSplits[i]->m_pBoundVolumeP = SGCore_CrBound();
			Split->m_aSplits[i]->m_pBoundVolumeP->setMinMax(&tmpmin, &tmpmax);
		}
	}
	else if (CountSplitsSys == GEOM_COUNT_TYPE_SEGMENTATION_OCTO)
	{
		ISXBound* ArrBound[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];
		for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
			ArrBound[i] = SGCore_CrBound();

		float3 tmpmin, tmpmax;
		SGCore_0ComBoundBoxArr8(Split->m_pBoundVolumeSys, ArrBound);

		for (int i = 0; i<CountSplitsSys; i++)
		{
			Split->m_aSplits[i]->m_pBoundVolumeSys = ArrBound[i];
			Split->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&tmpmin, &tmpmax);
			Split->m_aSplits[i]->m_pBoundVolumeP = SGCore_CrBound();
			Split->m_aSplits[i]->m_pBoundVolumeP->setMinMax(&tmpmin, &tmpmax);
		}
	}
	//}}

	bool *tmp_arr_mesh_poly = new bool[Split->m_uiCountAllPoly];
	for (DWORD i = 0; i<Split->m_uiCountAllPoly; i++)
		tmp_arr_mesh_poly[i] = true;

	DWORD CountInPoly = 0;
	float3 tmpMin, tmpMax;
	for (int i = 0; i<CountSplitsSys; i++)
	{
		//Split->m_aSplits[i]->m_pBoundVolumeP->getMinMax(&tmpMin, &tmpMax);
		//SGCore_FCreateBoundingBoxMesh(&tmpMin, &tmpMax, &(Split->m_aSplits[i]->m_pBoundBox));
		Split->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&tmpMin, &tmpMax);
		DWORD tmpNumCurrentPoly = 0;
		for (DWORD j = 0; j<Split->m_uiCountSubSet; j++)
		{
			for (DWORD k = 0; k<Split->m_pCountPoly[j] * 3; k += 3)
			{
				if (SGCore_0InPosPoints2D(&tmpMin, &tmpMax,
					&m_pCurrArrMeshVertex[Split->m_ppArrPoly[j][k]],
					&m_pCurrArrMeshVertex[Split->m_ppArrPoly[j][k + 1]],
					&m_pCurrArrMeshVertex[Split->m_ppArrPoly[j][k + 2]]
					)
					&& tmp_arr_mesh_poly[tmpNumCurrentPoly]
					)
				{
					ArrPoly[i].push_back((Split->m_ppArrPoly[j][k]));
					ArrPoly[i].push_back((Split->m_ppArrPoly[j][k + 1]));
					ArrPoly[i].push_back((Split->m_ppArrPoly[j][k + 2]));
					ArrGroup[i].push_back((Split->m_pNumberGroup[j]));

					tmp_arr_mesh_poly[tmpNumCurrentPoly] = false;
					CountInPoly++;
				}
				tmpNumCurrentPoly++;
			}
		}
	}

	//ИНОГДА ТУТ БЫЛО ЧЕТО НЕПОНЯТНОЕ
	//ПОСЛЕ ТОГО КАК ГРАФИКА КАКИМ ТО МАГИЧЕСКИМ ОБРАЗОМ СТАЛА ЛУЧШЕ (КОСЯКИ С КУСТАМИ БЫЛИ ПРИ КОМПИЛЯЦИИ С /mt)
	//ПРОПАЛИ НЕКОТОЫРЕ УЧАСТКИ ОДНОГО ЗДАНИЯ, ПОСЛЕ ТОГО КАК УБРАЛ ЭТОТ КОД ВСЕ СТАЛО НОРМАЛЬНО
	if (CountInPoly < Split->m_uiCountAllPoly)
	{
		DWORD tmpNumCurrentPolyDec = 0;
		DWORD tmpNumCurrentPoly = 0;
		float3 pos0, pos1, pos2, CenterPoly, Length, SphereCenter;
		float ArrLength[8], ShereRadius;

		for (DWORD j = 0; j<Split->m_uiCountSubSet; j++)
		{
			for (DWORD k = 0; k<Split->m_pCountPoly[j] * 3; k += 3)
			{
				if (tmp_arr_mesh_poly[tmpNumCurrentPoly])
				{
					pos0 = m_pCurrArrMeshVertex[Split->m_ppArrPoly[j][k]];
					pos1 = m_pCurrArrMeshVertex[Split->m_ppArrPoly[j][k + 1]];
					pos2 = m_pCurrArrMeshVertex[Split->m_ppArrPoly[j][k + 2]];

					CenterPoly.x = CenterPoly.y = CenterPoly.z = 0;

					CenterPoly.x = (pos0.x + pos1.x + pos2.x) / 3.0f;
					CenterPoly.y = (pos0.y + pos1.y + pos2.y) / 3.0f;
					CenterPoly.z = (pos0.z + pos1.z + pos2.z) / 3.0f;

					//memset(ArrLength,0,sizeof(float) * 8);

					for (int q = 0; q<CountSplitsSys; q++)
					{
						Split->m_aSplits[q]->m_pBoundVolumeSys->getSphere(&SphereCenter, &ShereRadius);
						Length = -SphereCenter + CenterPoly;
						ArrLength[q] = SMVector3Length(Length);
					}

					WORD Key = 0;

					for (int q = 0; q<CountSplitsSys; q++)
					{
						bool IsSmall = true;
						for (int n = 0; n<CountSplitsSys; n++)
						{
							if (ArrLength[q] > ArrLength[n])
							{
								IsSmall = false;
								//break;
							}
						}

						if (IsSmall)
						{
							Key = q;
							//break;
						}
					}

					ArrPoly[Key].push_back((Split->m_ppArrPoly[j][k]));
					ArrPoly[Key].push_back((Split->m_ppArrPoly[j][k + 1]));
					ArrPoly[Key].push_back((Split->m_ppArrPoly[j][k + 2]));
					ArrGroup[Key].push_back((Split->m_pNumberGroup[j]));
					//ArrSerialNum[Key].push_back((Split->ArrSerialNum[j][k / 3]));

					tmp_arr_mesh_poly[tmpNumCurrentPolyDec] = false;
					//tmpNumCurrentPolyDec++;
				}
				tmpNumCurrentPoly++;
			}
		}
	}



	for (int i = 0; i<CountSplitsSys; i++)
	{
		if (ArrPoly[i].size() > 0)
		{
			Array<DWORD> tmpNumSubSet;
			Split->m_aSplits[i]->m_uiCountSubSet = 0;

			//предварительно записываем одну подгруппу в массив tmpNumSubSet
			tmpNumSubSet.push_back(ArrGroup[i][0]);
			Split->m_aSplits[i]->m_uiCountSubSet++;
			//вычисляем сколько всего у нас подгрупп
			for (DWORD k = 0; k<ArrGroup[i].size(); k++)
			{
				bool tmpIsThereGroup = false;
				for (DWORD j = 0; j<tmpNumSubSet.size() && !tmpIsThereGroup; j++)
				{
					if (tmpNumSubSet[j] == ArrGroup[i][k])
						tmpIsThereGroup = true;
				}

				if (!tmpIsThereGroup)
				{
					tmpNumSubSet.push_back(ArrGroup[i][k]);
					Split->m_aSplits[i]->m_uiCountSubSet++;
				}
			}

			//
			Split->m_aSplits[i]->m_pNumberGroup = new uint32_t[Split->m_aSplits[i]->m_uiCountSubSet];
			Split->m_aSplits[i]->m_pNumberGroupModel = new uint32_t[Split->m_aSplits[i]->m_uiCountSubSet];

			for (DWORD k = 0; k<Split->m_aSplits[i]->m_uiCountSubSet; k++)
			{
				Split->m_aSplits[i]->m_pNumberGroup[k] = (tmpNumSubSet[k]);
				Split->m_aSplits[i]->m_pNumberGroupModel[k] = (tmpNumSubSet[k]);
			}

			tmpNumSubSet.clear();

			Split->m_aSplits[i]->m_pCountPoly = new uint32_t[Split->m_aSplits[i]->m_uiCountSubSet];

			//обнуление данных GlobalMesh->SplitMeshes[i]->m_pCountPoly
			for (DWORD j = 0; j<Split->m_aSplits[i]->m_uiCountSubSet; j++)
			{
				Split->m_aSplits[i]->m_pCountPoly[j] = 0;
			}
			Split->m_aSplits[i]->m_uiCountAllPoly = 0;

			//вычисляем сколько полигонов в каждой подгруппе в данном сплите
			for (DWORD k = 0; k<ArrPoly[i].size(); k += 3)
			{
				for (DWORD j = 0; j<Split->m_aSplits[i]->m_uiCountSubSet; j++)
				{
					if (Split->m_aSplits[i]->m_pNumberGroup[j] == (ArrGroup[i][k / 3]))
					{
						Split->m_aSplits[i]->m_pCountPoly[j]++;
						Split->m_aSplits[i]->m_uiCountAllPoly++;
					}
				}
			}

			Split->m_aSplits[i]->m_ppArrPoly = new uint32_t*[Split->m_aSplits[i]->m_uiCountSubSet];
			
			for (DWORD j = 0; j<Split->m_aSplits[i]->m_uiCountSubSet; j++)
			{
				Split->m_aSplits[i]->m_ppArrPoly[j] = new uint32_t[Split->m_aSplits[i]->m_pCountPoly[j] * 3];
			}
			uint32_t* tmpCountPG = new uint32_t[Split->m_aSplits[i]->m_uiCountSubSet];

			for (DWORD k = 0; k<Split->m_aSplits[i]->m_uiCountSubSet; k++)
			{
				tmpCountPG[k] = 0;
			}

			for (DWORD k = 0; k<ArrPoly[i].size(); k += 3)
			{
				for (DWORD j = 0; j<Split->m_aSplits[i]->m_uiCountSubSet; j++)
				{

					if (Split->m_aSplits[i]->m_pNumberGroup[j] == (ArrGroup[i][k / 3]))
					{
						Split->m_aSplits[i]->m_ppArrPoly[j][tmpCountPG[j]] = (ArrPoly[i][k]);
						Split->m_aSplits[i]->m_ppArrPoly[j][tmpCountPG[j] + 1] = (ArrPoly[i][k + 1]);
						Split->m_aSplits[i]->m_ppArrPoly[j][tmpCountPG[j] + 2] = (ArrPoly[i][k + 2]);

						tmpCountPG[j] += 3;
					}
				}
			}

			ArrPoly[i].clear();
			ArrGroup[i].clear();

			mem_delete_a(tmpCountPG);

			editVolume(mesh, Split->m_aSplits[i]);
			if (Split->m_aSplits[i]->m_uiCountAllPoly > 0 && Split->m_aSplits[i]->m_uiCountAllPoly > (UINT)CountPolyInSegment)
			{
				Split->m_aSplits[i]->m_isNonEnd = true;
			}
			else
			{
				Split->m_aSplits[i]->m_isNonEnd = false;
				//EditVolume(mesh, Split->m_aSplits[i]);

				//оптимизация для Post TnL кэша
				//{{
				Array<uint32_t> tmpVert;
				bool isunic = true;
				for (DWORD q = 0; q<Split->m_aSplits[i]->m_uiCountSubSet; q++)
				{
					tmpVert.clear();
					tmpVert.push_back(Split->m_aSplits[i]->m_ppArrPoly[q][0]);
					for (DWORD k = 0; k<Split->m_aSplits[i]->m_pCountPoly[q] * 3; k++)
					{
						isunic = true;
						for (DWORD j = 0; j<tmpVert.size() && isunic; j++)
						{
							if (Split->m_aSplits[i]->m_ppArrPoly[q][k] == tmpVert[j])
							{
								isunic = false;
							}
						}

						if (isunic)
							tmpVert.push_back(Split->m_aSplits[i]->m_ppArrPoly[q][k]);
					}

					SGCore_OptimizeIndecesInSubsetUint32(Split->m_aSplits[i]->m_ppArrPoly[q], Split->m_aSplits[i]->m_pCountPoly[q], tmpVert.size());
				}
				//}}
				

				for (DWORD q = 0; q<Split->m_aSplits[i]->m_uiCountSubSet; q++)
				{
					for (DWORD k = 0; k<Split->m_aSplits[i]->m_pCountPoly[q] * 3; k++)
					{
						Split->m_aSplits[i]->m_ppArrPoly[q][k] -= model->m_pStartVertex[Split->m_aSplits[i]->m_pNumberGroup[q]];
						Split->m_aSplits[i]->m_ppArrPoly[q][k] += mesh->m_aSubSets[Split->m_aSplits[i]->m_pNumberGroup[q]].m_idVertexStart;
					}

					Split->m_aSplits[i]->m_pNumberGroup[q] = mesh->m_aSubSets[Split->m_aSplits[i]->m_pNumberGroup[q]].m_idGroup;
				}
			}
		}
		else
		{
			Split->m_aSplits[i]->m_uiCountAllPoly = 0;
			Split->m_aSplits[i]->m_isNonEnd = false;

			mem_delete(Split->m_aSplits[i]);
		}
	}

	mem_delete_a(Split->m_pNumberGroup);
	
	mem_delete_a(Split->m_pCountPoly);

	for (DWORD k = 0; k<Split->m_uiCountSubSet; k++)
	{
		mem_delete_a(Split->m_ppArrPoly[k]);
	}

	mem_delete_a(Split->m_ppArrPoly);
	mem_delete_a(tmp_arr_mesh_poly);
}

void CStaticGeom::editVolume(CModel* mesh, CSegment* Split)
{
	float3 Max, Min;
	Max = m_pCurrArrMeshVertex[(Split->m_ppArrPoly[0][0])];
	Min = m_pCurrArrMeshVertex[(Split->m_ppArrPoly[0][0])];

	float3 Max2, Min2;
	Split->m_pBoundVolumeSys->getMinMax(&Min2, &Max2);

	float3 tmpf;
	for(UINT i = 0; i<Split->m_uiCountSubSet; ++i)
	{
		for(UINT k = 0; k<Split->m_pCountPoly[i] * 3; ++k)
		{
			tmpf = m_pCurrArrMeshVertex[(Split->m_ppArrPoly[i][k])];
			if (tmpf.x > Max.x)
				Max.x = tmpf.x;
			if (tmpf.y > Max.y)
				Max.y = tmpf.y;
			if (tmpf.z > Max.z)
				Max.z = tmpf.z;

			if (tmpf.x < Min.x)
				Min.x = tmpf.x;
			if (tmpf.y < Min.y)
				Min.y = tmpf.y;
			if (tmpf.z < Min.z)
				Min.z = tmpf.z;
		}
	}

	//float3 Center = (Min + Max) * 0.5f;
	//float Radius = SMVector3Length(Center - Max);

	//Split->m_pBoundVolumeP->SetSphere(&Center, &Radius);
	Split->m_pBoundVolumeP->setMinMax(&Min, &Max);
}

void CStaticGeom::save(const char* path)
{
	FILE* file = fopen(path, "wb");
	
	//записываем количество подгрупп
	int32_t countgroup = m_aAllGroups.size();
	fwrite(&countgroup, sizeof(int32_t), 1, file);

	vertex_static * pData;

	for(UINT i = 0; i < m_aAllGroups.size(); ++i)
	{
		int32_t tmpstrlen = m_aAllGroups[i]->m_sName.length();
		fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
		fwrite(m_aAllGroups[i]->m_sName.c_str(), sizeof(char), tmpstrlen, file);

		//записываем количество буферов в подгруппе
		int32_t countbuffingroup = m_aAllGroups[i]->m_aVertexBuffers.size();
		fwrite(&countbuffingroup, sizeof(int32_t), 1, file);

		for(UINT k = 0; k < m_aAllGroups[i]->m_aCountVertex.size(); ++k)
		{
			//записываем количество моделей, которые используют данную подгруппу
			int32_t countusingmodels = m_aAllGroups[i]->m_aModels[k].size();
			fwrite(&countusingmodels, sizeof(int32_t), 1, file);
			int32_t tmpcountusingmodels = 0;
			for(UINT j = 0; j < m_aAllGroups[i]->m_aModels[k].size(); ++j)
			{
				for(UINT q = 0; q < m_aAllModels.size(); ++q)
				{
					if (m_aAllModels[q] == m_aAllGroups[i]->m_aModels[k][j])
					{
						fwrite(&(q), sizeof(int32_t), 1, file);
						++tmpcountusingmodels;
						break;
					}
				}
			}

			//записываем количество вершин в буфере подгруппы
			fwrite(&m_aAllGroups[i]->m_aCountVertex[k], sizeof(int32_t), 1, file);

			//записываем количество индексов в буфере подгруппы
			fwrite(&m_aAllGroups[i]->m_aCountIndex[k], sizeof(int32_t), 1, file);

			//записываем весь вершинный буфер подгруппы
			m_aAllGroups[i]->m_aVertexBuffers[k]->Lock(0, 0, (void**)&pData, 0);
			fwrite(pData, sizeof(vertex_static), m_aAllGroups[i]->m_aCountVertex[k], file);
			m_aAllGroups[i]->m_aVertexBuffers[k]->Unlock();
		}
	}

	//записываем количество моделей
	int32_t countmodels = m_aAllModels.size();
	fwrite(&countmodels, sizeof(int32_t), 1, file);

	for(UINT i = 0; i < m_aAllModels.size(); ++i)
	{
		int32_t countsubset = m_aAllModels[i]->m_aSubSets.size();
		fwrite(&countsubset, sizeof(int32_t), 1, file);

		int32_t countlenstr = strlen(m_aAllModels[i]->m_szName);
		fwrite(&countlenstr, sizeof(int32_t), 1, file);
		fwrite(&m_aAllModels[i]->m_szName, sizeof(char), countlenstr, file);

		countlenstr = strlen(m_aAllModels[i]->m_szPathName);
		fwrite(&countlenstr, sizeof(int32_t), 1, file);
		fwrite(&m_aAllModels[i]->m_szPathName, sizeof(char), countlenstr, file);

		
		fwrite(&m_aAllModels[i]->m_uiCountPoly, sizeof(int32_t), 1, file);
		
		fwrite(&m_aAllModels[i]->m_vPosition.x, sizeof(float), 1, file);
		fwrite(&m_aAllModels[i]->m_vPosition.y, sizeof(float), 1, file);
		fwrite(&m_aAllModels[i]->m_vPosition.z, sizeof(float), 1, file);

		fwrite(&m_aAllModels[i]->m_vRotation.x, sizeof(float), 1, file);
		fwrite(&m_aAllModels[i]->m_vRotation.y, sizeof(float), 1, file);
		fwrite(&m_aAllModels[i]->m_vRotation.z, sizeof(float), 1, file);

		fwrite(&m_aAllModels[i]->m_vScale.x, sizeof(float), 1, file);
		fwrite(&m_aAllModels[i]->m_vScale.y, sizeof(float), 1, file);
		fwrite(&m_aAllModels[i]->m_vScale.z, sizeof(float), 1, file);

		if (m_aAllModels[i]->m_oLod0.m_pModel)
		{
			countlenstr = strlen(m_aAllModels[i]->m_oLod0.m_szPathName);
			fwrite(&countlenstr, sizeof(int32_t), 1, file);
			fwrite(m_aAllModels[i]->m_oLod0.m_szPathName, sizeof(char), countlenstr, file);
		}
		else
		{
			countlenstr = 0;
			fwrite(&countlenstr, sizeof(int32_t), 1, file);
		}

		for(UINT k = 0; k < m_aAllModels[i]->m_aSubSets.size(); ++k)
		{
			fwrite(&(m_aAllModels[i]->m_aSubSets[k]), sizeof(CModel::CSubSet), 1, file);
		}

		Array<CSegment*> queue;
		int tmpcount = 0;
		queue.push_back(m_aAllModels[i]->m_pArrSplits);

		while (queue.size())
		{
			saveSplit(queue[0], file, &queue);

			queue.erase(0);
			++tmpcount;
		}
	}

	fclose(file);
}

void CStaticGeom::saveSplit(CSegment* Split, FILE* file, Array<CSegment*> * queue)
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

	fwrite(&Split->m_uiCountAllPoly, sizeof(uint32_t), 1, file);

	fwrite(&Split->m_isNonEnd, sizeof(int8_t), 1, file);

	if (Split->m_isNonEnd)
	{
		bool isexists = true;
		for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; i++)
		{
			if (Split->m_aSplits[i])
			{
				isexists = true;
				fwrite(&isexists, sizeof(int8_t), 1, file);
				queue->push_back(Split->m_aSplits[i]);
			}
			else
			{
				isexists = false;
				fwrite(&isexists, sizeof(int8_t), 1, file);
			}

			isexists = true;
		}
	}
	else
	{
		fwrite(&Split->m_uiCountSubSet, sizeof(uint32_t), 1, file);
		fwrite(Split->m_pNumberGroupModel, sizeof(uint32_t)*Split->m_uiCountSubSet, 1, file);
		fwrite(Split->m_pNumberGroup, sizeof(uint32_t)*Split->m_uiCountSubSet, 1, file);
		fwrite(Split->m_pCountPoly, sizeof(uint32_t)*Split->m_uiCountSubSet, 1, file);

		for(UINT i = 0; i < Split->m_uiCountSubSet; ++i)
		{
			fwrite(Split->m_ppArrPoly[i], sizeof(uint32_t)*Split->m_pCountPoly[i] * 3, 1, file);
		}
	}
}


void CStaticGeom::load(const char* path)
{
	deleteArrIndexPtr();

	FILE* file = fopen(path, "rb");

	int32_t countgroup = -1;
	fread(&countgroup, sizeof(int32_t), 1, file);
	Array<Array<Array<int32_t>>> tmpArrIdsModels;
	vertex_static * pData;
	int tmpbigersizebuff = 0;

	for (int i = 0; i < countgroup; ++i)
	{
		CGroup* group = new CGroup();
		tmpbigersizebuff = 0;
		int32_t tmpstrlen;
		char texname[1024];
		fread(&tmpstrlen, sizeof(int32_t), 1, file);
		fread(texname, sizeof(char), tmpstrlen, file);
		texname[tmpstrlen] = 0;
		group->m_sName = texname;
		
		group->m_iCountVertex = 0;
		group->m_iCountIndex = 0;
		
		//записываем количество буферов в подгруппе
		int32_t countbuffingroup = -1;
		fread(&countbuffingroup, sizeof(int32_t), 1, file);
		
		for (int k = 0; k < countbuffingroup; ++k)
		{
			//записываем количество моделей, которые используют данную подгруппу
			int32_t countusingmodels;
			fread(&countusingmodels, sizeof(int32_t), 1, file);
			int32_t tmpnummodel;
			for (int j = 0; j < countusingmodels; ++j)
			{
				fread(&tmpnummodel, sizeof(int32_t), 1, file);
				tmpArrIdsModels[i][k].push_back(tmpnummodel);
			}

			//записываем количество вершин в буфере подгруппы
			fread(&group->m_aCountVertex[k], sizeof(int32_t), 1, file);
			group->m_iCountVertex += group->m_aCountVertex[k];
			
			//записываем количество индексов в буфере подгруппы
			fread(&group->m_aCountIndex[k], sizeof(int32_t), 1, file);
			group->m_iCountIndex += group->m_aCountIndex[k];

			if (tmpbigersizebuff < group->m_aCountIndex[k])
				tmpbigersizebuff = group->m_aCountIndex[k];

			IDirect3DVertexBuffer9* vb;
			CStaticGeom::m_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* group->m_aCountVertex[k], NULL, NULL, D3DPOOL_MANAGED, &vb, 0);

			//записываем весь вершинный буфер подгруппы
			vb->Lock(0, 0, (void**)&pData, 0);
			fread(pData, sizeof(vertex_static), group->m_aCountVertex[k], file);
		
			/*Group::VertexBuff* vborigin = new Group::VertexBuff();
			vborigin->count = group->CountVertex[k];
			vborigin->arr = new float3_t[vborigin->count];

			for (int j = 0; j < vborigin->count; ++j)
			{
				vborigin->arr[j] = pData[j].Pos;
			}*/

			//UpdateArrMeshVertex2(group->CountVertex[k], pData);
			vb->Unlock();

			group->m_aVertexBuffers.push_back(vb);
			//group->VertexBufferOrigin.push_back(vborigin);
		}
		
		char tmptex[1024];
		sprintf(tmptex, "%s.dds", group->m_sName.c_str());

		group->m_idTexture = SGCore_MtlLoad(tmptex, MTL_TYPE_GEOM);
		group->m_isRenderSingly = SGCore_MtlGroupRenderIsSingly(group->m_idTexture);
		group->m_iSortGroup = SGCore_MtlGetSort(group->m_idTexture);
		m_aAllGroups.push_back(group);
		
		if (m_pRenderIndexBuffer == 0 || m_iSizeRenderIndexBuffer < tmpbigersizebuff)
		{
			mem_release(m_pRenderIndexBuffer);
			m_iSizeRenderIndexBuffer = tmpbigersizebuff;
			CStaticGeom::m_pDXDevice->CreateIndexBuffer(sizeof(uint32_t)* m_iSizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &m_pRenderIndexBuffer, 0);
		}
	}

	//записываем количество моделей
	int32_t countmodels = m_aAllModels.size();
	fread(&countmodels, sizeof(int32_t), 1, file);

	for (int i = 0; i < countmodels; ++i)
	{
		m_aAllModels.push_back(0);
		m_aAllModels[i] = new CModel();

		int32_t countsubset;
		fread(&countsubset, sizeof(int32_t), 1, file);

		int32_t countlenstr = 0;
		fread(&countlenstr, sizeof(int32_t), 1, file);
		fread(&m_aAllModels[i]->m_szName, sizeof(char), countlenstr, file);
		m_aAllModels[i]->m_szName[countlenstr] = 0;

		countlenstr = 0;
		fread(&countlenstr, sizeof(int32_t), 1, file);
		fread(&m_aAllModels[i]->m_szPathName, sizeof(char), countlenstr, file);
		m_aAllModels[i]->m_szPathName[countlenstr] = 0;

		fread(&m_aAllModels[i]->m_uiCountPoly, sizeof(int32_t), 1, file);

		fread(&m_aAllModels[i]->m_vPosition.x, sizeof(float), 1, file);
		fread(&m_aAllModels[i]->m_vPosition.y, sizeof(float), 1, file);
		fread(&m_aAllModels[i]->m_vPosition.z, sizeof(float), 1, file);

		fread(&m_aAllModels[i]->m_vRotation.x, sizeof(float), 1, file);
		fread(&m_aAllModels[i]->m_vRotation.y, sizeof(float), 1, file);
		fread(&m_aAllModels[i]->m_vRotation.z, sizeof(float), 1, file);

		fread(&m_aAllModels[i]->m_vScale.x, sizeof(float), 1, file);
		fread(&m_aAllModels[i]->m_vScale.y, sizeof(float), 1, file);
		fread(&m_aAllModels[i]->m_vScale.z, sizeof(float), 1, file);

		m_aAllModels[i]->m_vOldPosition = m_aAllModels[i]->m_vPosition;
		m_aAllModels[i]->m_vOldRotation = m_aAllModels[i]->m_vRotation;
		m_aAllModels[i]->m_vOldScale = m_aAllModels[i]->m_vScale;

		m_mWorld = SMMatrixScaling(m_aAllModels[i]->m_vScale) * SMMatrixRotationX(m_aAllModels[i]->m_vRotation.x) * SMMatrixRotationY(m_aAllModels[i]->m_vRotation.y) * SMMatrixRotationZ(m_aAllModels[i]->m_vRotation.z) * SMMatrixTranslation(m_aAllModels[i]->m_vPosition);

		fread(&countlenstr, sizeof(int32_t), 1, file);

		if (countlenstr > 0)
		{
			fread(m_aAllModels[i]->m_oLod0.m_szPathName, sizeof(char), countlenstr, file);
			m_aAllModels[i]->m_oLod0.m_szPathName[countlenstr] = 0;
			setModelLodPath(i, m_aAllModels[i]->m_oLod0.m_szPathName);
		}

		for (int k = 0; k < countsubset; ++k)
		{
			CModel::CSubSet gdb;
			fread(&gdb, sizeof(CModel::CSubSet), 1, file);
			m_aAllModels[i]->m_aSubSets.push_back(gdb);

			char tmptex[1024];
			sprintf(tmptex, "%s.dds", m_aAllGroups[gdb.m_idGroup]->m_sName.c_str());
			m_aAllModels[i]->m_aIDsTexures[k] = SGCore_MtlLoad(tmptex, MTL_TYPE_GEOM);
		}

		Array<CSegment**> queue;
		int tmpcount = 0;
		queue.push_back(&(m_aAllModels[i]->m_pArrSplits));

		while (queue.size())
		{
			loadSplit(queue[0], file, &queue);

			queue.erase(0);
			++tmpcount;
		}

		CSegment* tmpsegment = m_aAllModels[i]->m_pArrSplits;
		setSplitID(m_aAllModels[i]->m_pArrSplits, &(m_aAllModels[i]->m_idCountSplits), (&m_aAllModels[i]->m_idCountSplitsRender));

		addModelInArrCom(i);

		float3 jmin, jmax, jmin2, jmax2;

		m_aAllModels[i]->m_pArrSplits->m_pBoundVolumeP->getMinMax(&jmin, &jmax);
		m_pBoundVolume->getMinMax(&jmin2, &jmax2);

		if (jmin2.x > jmin.x)
			jmin2.x = jmin.x;
		if (jmin2.y > jmin.y)
			jmin2.y = jmin.y;
		if (jmin2.z > jmin.z)
			jmin2.z = jmin.z;

		if (jmax2.x < jmax.x)
			jmax2.x = jmax.x;
		if (jmax2.y < jmax.y)
			jmax2.y = jmax.y;
		if (jmax2.z < jmax.z)
			jmax2.z = jmax.z;

		m_pBoundVolume->setMinMax(&jmin2, &jmax2);
	}

	for(UINT i = 0; i < m_aAllGroups.size(); ++i)
	{
		for(UINT k = 0; k < tmpArrIdsModels[i].size(); ++k)
		{
			for(UINT j = 0; j < tmpArrIdsModels[i][k].size(); ++j)
			{
				m_aAllGroups[i]->m_aModels[k][j] = m_aAllModels[tmpArrIdsModels[i][k][j]];
			}
		}
	}

	initArrIndexPtr();

	fclose(file);
}

void CStaticGeom::loadSplit(CSegment** Split, FILE* file, Array<CSegment**> * queue)
{
	float3 jmin, jmax;
	(*Split) = new CSegment();

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

	fread(&(*Split)->m_uiCountAllPoly, sizeof(uint32_t), 1, file);

	fread(&(*Split)->m_isNonEnd, sizeof(int8_t), 1, file);

	if ((*Split)->m_isNonEnd)
	{
		bool isexists = false;

		for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; i++)
		{
			fread(&isexists, sizeof(int8_t), 1, file);
			if (isexists)
				queue->push_back(&((*Split)->m_aSplits[i]));

			isexists = false;
		}
	}
	else
	{
		fread(&(*Split)->m_uiCountSubSet, sizeof(uint32_t), 1, file);

		(*Split)->m_pNumberGroupModel = new uint32_t[(*Split)->m_uiCountSubSet];
		(*Split)->m_pNumberGroup = new uint32_t[(*Split)->m_uiCountSubSet];
		(*Split)->m_pCountPoly = new uint32_t[(*Split)->m_uiCountSubSet];

		fread((*Split)->m_pNumberGroupModel, sizeof(uint32_t)*(*Split)->m_uiCountSubSet, 1, file);
		fread((*Split)->m_pNumberGroup, sizeof(uint32_t)*(*Split)->m_uiCountSubSet, 1, file);
		fread((*Split)->m_pCountPoly, sizeof(uint32_t)*(*Split)->m_uiCountSubSet, 1, file);

		(*Split)->m_ppArrPoly = new uint32_t*[(*Split)->m_uiCountSubSet];

		for(UINT i = 0; i < (*Split)->m_uiCountSubSet; ++i)
		{
			(*Split)->m_ppArrPoly[i] = new uint32_t[(*Split)->m_pCountPoly[i] * 3];
			fread((*Split)->m_ppArrPoly[i], sizeof(uint32_t)*(*Split)->m_pCountPoly[i] * 3, 1, file);
		}
	}
}

void CStaticGeom::sortGroup(const float3* viewpos, int sort_mtl)
{
	float tmpradius = 0;
	float tmpsidt = 0;
	float3 tmpcenter = float3(0,0,0);
	CModel* tmpmodel;
	Array<float> arr_dist;

	UINT tmp_index_gd = 0;

	float tmpdist = 999999;

	for(UINT i = 0, l = m_aAllModels.size(); i < l; ++i)
	{
		tmpmodel = m_aAllModels[i];
		for(UINT k = 0, kl = tmpmodel->m_aSubSets.size(); k < kl; ++k)
		{
			//if (sort_mtl == m_aAllGroups[tmpmodel->m_aSubSets[k].idgroup]->SortGroup)
			
			tmpcenter = (tmpmodel->m_aSubSets[k].m_vMin + tmpmodel->m_aSubSets[k].m_vMax) * 0.5f;
			tmpradius = SMVector3Length(tmpcenter - tmpmodel->m_aSubSets[k].m_vMax);
			//tmpmodel->GroupDist[k] = SMVector3Length((tmpcenter - (*viewpos))) - tmpradius;
			
			if (m_aDistGroup.size() <= tmp_index_gd || !m_aDistGroup[tmp_index_gd])
				m_aDistGroup[tmp_index_gd] = new CInfoGroup();

			//m_aDistGroup[tmp_index_gd]->m_iCount = 0;
			m_aDistGroup[tmp_index_gd]->m_fDist = SMVector3Length((tmpcenter - (*viewpos))) - tmpradius;
			m_aDistGroup[tmp_index_gd]->m_idGroup = k;
			m_aDistGroup[tmp_index_gd]->m_idGlobalGroup = tmpmodel->m_aSubSets[k].m_idGroup;
			m_aDistGroup[tmp_index_gd]->m_idModel = i;

			++tmp_index_gd;
		}
	}

	for(int i = 0, l = m_aDistGroup.size(); i < l; ++i)
	{
		CInfoGroup* tmpig = m_aDistGroup[i];
		
		int pos = i;
		for(int k = i + 1, kl = l; k < kl; ++k)
		{
			CInfoGroup* tmpig2 = m_aDistGroup[k];
			int qwert = 0;

			if (tmpig->m_fDist > tmpig2->m_fDist)
			{
				pos = k;
				tmpig = tmpig2;

				//++(tmpig->count);
			}
		}

		m_aDistGroup[pos] = m_aDistGroup[i];
		m_aDistGroup[i] = tmpig;

		int qwert = 0;
	}
}


/*void CStaticGeom::getIntersectedRayY2(float3* pos, CSegment** arrsplits, int *count, CSegment* comsegment, ID curr_splits_ids_render)
{
	float3 jmin, jmax;
	comsegment->m_pBoundVolumeP->getMinMax(&jmin, &jmax);

	if (comsegment->m_uiCountAllPoly > 0 && pos->x >= jmin.x && pos->z >= jmin.z && pos->x <= jmax.x && pos->z <= jmax.z)
	{
		if (comsegment->m_isNonEnd)
		{
			for (int j = 0; j<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; j++)
			{
				if (comsegment->m_aSplits[j])
					getIntersectedRayY2(pos, arrsplits, count, comsegment->m_aSplits[j], curr_splits_ids_render);
			}
		}
		else
		{
			if ((*count) < curr_splits_ids_render)
			{
				arrsplits[(*count)] = comsegment;

				(*count)++;
			}
		}
	}
}*/

/*bool CStaticGeom::getIntersectedRayY(float3* pos)
{
	D3DXVECTOR3 jpos;
	float3 jvec0, jvec1, jvec2;

	jpos = (D3DXVECTOR3)*pos;
	BOOL Hit;
	float u, v, Dist, CountDist;
	Dist = -1000;
	float tmpy = -1000;
	CountDist = -1000;
	bool is_find = false;

	for(UINT id = 0, idl = m_aAllModels.size(); id < idl; ++id)
	{
		m_aArrComFor[1]->m_aIRS[id]->m_iCountCom = 0;

		getIntersectedRayY2(pos, m_aArrComFor[1]->m_aIRS[id]->m_ppSegments, &(m_aArrComFor[1]->m_aIRS[id]->m_iCountCom), m_aAllModels[id]->m_pArrSplits, m_aArrComFor[1]->m_aIRS[id]->m_iCount);

		for(DWORD k = 0, kl = m_aArrComFor[1]->m_aIRS[id]->m_iCountCom; k<kl; ++k)
		{
			for (DWORD group = 0; group<m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_uiCountSubSet; group++)
			{
				//if (m_aAllGroups[m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_pNumberGroup[group]]->m_iSortGroup > 0)
					//continue;

				ID idbuff = m_aAllModels[id]->m_aSubSets[m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_pNumberGroupModel[group]].m_idBuff;
				ID idgroup = m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_pNumberGroup[group];

				vertex_static* pData = 0;
				if (FAILED(m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Lock(0, 0, (void**)&pData, 0)))
					continue;

				for (DWORD numpoly = 0; numpoly<m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_pCountPoly[group] * 3; numpoly += 3)
				{
					Dist = -1000.f;
					//находим все 3 вершины
					
					jvec0 = pData[m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_ppArrPoly[group][numpoly]].Pos;
					jvec1 = pData[m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_ppArrPoly[group][numpoly + 1]].Pos;
					jvec2 = pData[m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_ppArrPoly[group][numpoly + 2]].Pos;

					Hit = D3DXIntersectTri(&(jvec0.operator D3DXVECTOR3()),
						&(jvec1.operator D3DXVECTOR3()),
						&(jvec2.operator D3DXVECTOR3()),
						&(jpos),
						&gс_vYmin, &u, &v, &Dist);

					if (Hit && tmpy < (pos->y - Dist * 1000))
					{
						tmpy = pos->y - Dist * 1000;
						is_find = true;
					}
					else
					{
						Hit = D3DXIntersectTri(&(jvec0.operator D3DXVECTOR3()),
							&(jvec1.operator D3DXVECTOR3()),
							&(jvec2.operator D3DXVECTOR3()),
							&(jpos),
							&gс_vYmax, &u, &v, &Dist);
						if (Hit)
						{
							pos->y += Dist * 1000;
							tmpy = pos->y;
							is_find = true;
						}
					}
				}

				m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Unlock();
			}
		}
	}
	pos->y = tmpy;
	return is_find;
}*/

void CStaticGeom::getMinMax(float3* min, float3* max)
{
	m_pBoundVolume->getMinMax(min, max);
}

ID CStaticGeom::addArrForCom()
{
	CIRSData* ttmpdata = new CIRSData();
	
	for(UINT i = 0, l = m_aAllModels.size(); i < l; ++i)
	{
		CInfoRenderSegments* tmpirs = new CInfoRenderSegments();
		tmpirs->m_iCount = m_aAllModels[i]->m_idCountSplitsRender;
		tmpirs->m_ppSegments = new CSegment*[m_aAllModels[i]->m_idCountSplitsRender];
		tmpirs->m_iCountCom = 0;
		ttmpdata->m_aIRS.push_back(tmpirs);
	}

	ID id_arr = -1;
	for(UINT i = 0, l = m_aArrComFor.size(); i < l; ++i)
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

bool CStaticGeom::existsArrForCom(ID id)
{
	return((ID)m_aArrComFor.size() > id);
}

void CStaticGeom::deleteArrForCom(ID id_arr)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	mem_delete(m_aArrComFor[id_arr]);
}

void CStaticGeom::addModelInArrCom(ID id_model)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model, _VOID);

	for(UINT i = 0, l = m_aArrComFor.size(); i < l; ++i)
	{
		CInfoRenderSegments* tmpirs = new CInfoRenderSegments();
		tmpirs->m_iCount = m_aAllModels[id_model]->m_idCountSplitsRender;
		tmpirs->m_ppSegments = new CSegment*[m_aAllModels[id_model]->m_idCountSplitsRender];
		tmpirs->m_iCountCom = 0;
		m_aArrComFor[i]->m_aIRS.push_back(tmpirs);
	}
}

void CStaticGeom::deleteModelInArrCom(ID id_model)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model, _VOID);

	for(UINT i = 0, l = m_aArrComFor.size(); i < l; ++i)
	{
		if (!(m_aArrComFor[i]))
			continue;

		mem_delete(m_aArrComFor[i]->m_aIRS[id_model]);
		m_aArrComFor[i]->m_aIRS.erase(id_model);
	}
}

///

char* CStaticGeom::getModelName(ID id)
{
	if (id < (ID)m_aAllModels.size())
		return m_aAllModels[id]->m_szName;

	return 0;
}

const char* CStaticGeom::getModelPathName(ID id)
{
	if (id < (ID)m_aAllModels.size())
		return m_aAllModels[id]->m_szPathName;

	return 0;
}

int CStaticGeom::getModelCountPoly(ID id)
{
	if (id < (ID)m_aAllModels.size())
		return m_aAllModels[id]->m_uiCountPoly;

	return -1;
}

void CStaticGeom::getModelMinMax(ID id, float3* min, float3* max)
{
	if(id >= (ID)m_aAllModels.size())
		return;

	m_aAllModels[id]->m_pArrSplits->m_pBoundVolumeP->getMinMax(min, max);
}


float3* CStaticGeom::getModelPosition(ID id)
{
	if(id < (ID)m_aAllModels.size())
		return &m_aAllModels[id]->m_vPosition;

	return 0;
}

float3* CStaticGeom::getModelRotation(ID id)
{
	if(id < (ID)m_aAllModels.size())
		return &m_aAllModels[id]->m_vRotation;

	return 0;
}

float3* CStaticGeom::getModelScale(ID id)
{
	if(id < (ID)m_aAllModels.size())
		return &m_aAllModels[id]->m_vScale;

	return 0;
}

const char* CStaticGeom::getModelLodPath(ID id)
{
	if(id < (ID)m_aAllModels.size())
		return m_aAllModels[id]->m_oLod0.m_szPathName;

	return 0;
}

void CStaticGeom::setModelLodPath(ID id, const char* path)
{
	if(id < (ID)m_aAllModels.size() && STR_VALIDATE(path))
	{
		mem_delete(m_aAllModels[id]->m_oLod0.m_pModel);
		m_aAllModels[id]->m_oLod0.m_aIDsTextures.clear();
		//m_aAllModels[id]->m_oLod0.model = new DataStaticModel();
		char tmppath[1024];
		sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path);
		SGCore_StaticModelLoad(tmppath, &(m_aAllModels[id]->m_oLod0.m_pModel));

		char tmptex[1024];
		ID tmpidmat;
		for (int i = 0; i < m_aAllModels[id]->m_oLod0.m_pModel->m_uiSubsetCount; ++i)
		{
			sprintf(tmptex, "%s.dds", m_aAllModels[id]->m_oLod0.m_pModel->m_ppTextures[i]);
			tmpidmat = SGCore_MtlLoad(tmptex, MTL_TYPE_GEOM);
			m_aAllModels[id]->m_oLod0.m_aIDsTextures.push_back(tmpidmat);
			m_aAllModels[id]->m_oLod0.m_iSortGroup = SGCore_MtlGetSort(tmpidmat);
		}

		sprintf(m_aAllModels[id]->m_oLod0.m_szPathName,"%s",path);
		
		m_aAllModels[id]->m_oLod0.m_pBoundVolume->calcBound(m_aAllModels[id]->m_oLod0.m_pModel->m_pVertexBuffer, m_aAllModels[id]->m_oLod0.m_pModel->m_uiAllVertexCount, sizeof(vertex_static));
		
		applyTransformLod(id);
	}
}

void CStaticGeom::applyTransform(ID id)
{
	if(id >= (ID)m_aAllModels.size())
		return;
	CModel* tmpmodel = m_aAllModels[id];

	float3 tmppos = tmpmodel->m_vPosition - tmpmodel->m_vOldPosition;
	float3 tmprot = tmpmodel->m_vRotation - tmpmodel->m_vOldRotation;
	float3 tmpscale = tmpmodel->m_vScale / tmpmodel->m_vOldScale;

	float3 jmin, jmax;
	tmpmodel->m_pArrSplits->m_pBoundVolumeP->getMinMax(&jmin, &jmax);
	float3 jcentererpos = (jmax + jmin)*0.5f;
	
	m_mRotation = SMMatrixRotationX(tmprot.x) * SMMatrixRotationY(tmprot.y) * SMMatrixRotationZ(tmprot.z);
	m_mWorld = SMMatrixScaling(tmpscale) * m_mRotation * SMMatrixTranslation(tmppos);


	applyTransformLod(id);

	vertex_static* pData;
	int idgroup;
	int idbuff;

	int vertexstart, vertexcount;
	
	for (int i = 0; i < tmpmodel->m_aSubSets.size(); ++i)
	{
		tmpmodel->m_aSubSets[i].m_vCenter = SMVector3Transform(tmpmodel->m_aSubSets[i].m_vCenter, m_mWorld);
		D3DXPlaneTransform(&tmpmodel->m_aSubSets[i].m_oPlane, &tmpmodel->m_aSubSets[i].m_oPlane, &(m_mWorld.operator D3DXMATRIX()));


		tmpmodel->m_aSubSets[i].m_vMin.x -= jcentererpos.x;
		tmpmodel->m_aSubSets[i].m_vMin.y -= jcentererpos.y;
		tmpmodel->m_aSubSets[i].m_vMin.z -= jcentererpos.z;

		tmpmodel->m_aSubSets[i].m_vMin = SMVector3Transform(tmpmodel->m_aSubSets[i].m_vMin, m_mWorld);

		tmpmodel->m_aSubSets[i].m_vMin.x += jcentererpos.x;
		tmpmodel->m_aSubSets[i].m_vMin.y += jcentererpos.y;
		tmpmodel->m_aSubSets[i].m_vMin.z += jcentererpos.z;


		tmpmodel->m_aSubSets[i].m_vMax.x -= jcentererpos.x;
		tmpmodel->m_aSubSets[i].m_vMax.y -= jcentererpos.y;
		tmpmodel->m_aSubSets[i].m_vMax.z -= jcentererpos.z;

		tmpmodel->m_aSubSets[i].m_vMax = SMVector3Transform(tmpmodel->m_aSubSets[i].m_vMax, m_mWorld);

		tmpmodel->m_aSubSets[i].m_vMax.x += jcentererpos.x;
		tmpmodel->m_aSubSets[i].m_vMax.y += jcentererpos.y;
		tmpmodel->m_aSubSets[i].m_vMax.z += jcentererpos.z;



		idgroup = tmpmodel->m_aSubSets[i].m_idGroup;
		idbuff = tmpmodel->m_aSubSets[i].m_idBuff;
		//pData2 = m_aAllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr;
		if (!FAILED(m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Lock(0, 0, (void**)&pData, 0)))
		{
			vertexstart = tmpmodel->m_aSubSets[i].m_idVertexStart;
			vertexcount = tmpmodel->m_aSubSets[i].m_uiVertexCount;

			for (int k = 0; k < vertexcount; ++k)
			{
				pData[vertexstart + k].Pos.x -= jcentererpos.x;
				pData[vertexstart + k].Pos.y -= jcentererpos.y;
				pData[vertexstart + k].Pos.z -= jcentererpos.z;

				pData[vertexstart + k].Pos = SMVector3Transform(pData[vertexstart + k].Pos, m_mWorld);
				pData[vertexstart + k].Norm = SMVector3Transform(pData[vertexstart + k].Norm, m_mRotation);

				pData[vertexstart + k].Pos.x += jcentererpos.x;
				pData[vertexstart + k].Pos.y += jcentererpos.y;
				pData[vertexstart + k].Pos.z += jcentererpos.z;
			}
			m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Unlock();
		}
	}

	Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> queue;
	int tmpcount = 0;
	queue.push_back(tmpmodel->m_pArrSplits);

	float3 tmpmin, tmpmax;

	while (queue.size())
	{
		queue[0]->m_pBoundVolumeSys->getMinMax(&tmpmin, &tmpmax);
		tmpmin -= jcentererpos;
		tmpmax -= jcentererpos;

		tmpmin = SMVector3Transform(tmpmin, m_mWorld);
		tmpmax = SMVector3Transform(tmpmax, m_mWorld);

		tmpmin += jcentererpos;
		tmpmax += jcentererpos;
		queue[0]->m_pBoundVolumeSys->setMinMax(&tmpmin, &tmpmax);

		queue[0]->m_pBoundVolumeP->getMinMax(&tmpmin, &tmpmax);
		tmpmin -= jcentererpos;
		tmpmax -= jcentererpos;

		tmpmin = SMVector3Transform(tmpmin, m_mWorld);
		tmpmax = SMVector3Transform(tmpmax, m_mWorld);

		tmpmin += jcentererpos;
		tmpmax += jcentererpos;
		queue[0]->m_pBoundVolumeP->setMinMax(&tmpmin, &tmpmax);

		if (queue[0]->m_isNonEnd)
		{
			for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
			{
				if (queue[0]->m_aSplits[i])
					queue.push_back(queue[0]->m_aSplits[i]);
			}
		}

		queue.erase(0);
		++tmpcount;
	}

	tmpmodel->m_vOldPosition = tmpmodel->m_vPosition;
	tmpmodel->m_vOldRotation = tmpmodel->m_vRotation;
	tmpmodel->m_vOldScale = tmpmodel->m_vScale;
}

void CStaticGeom::applyTransformLod(ID id)
{
	if (id >= m_aAllModels.size() || m_aAllModels[id]->m_oLod0.m_pModel == 0)
		return;

	CModel* tmpmodel = m_aAllModels[id];
	vertex_static* pData;

	float3 jmin, jmax;
	m_aAllModels[id]->m_oLod0.m_pBoundVolume->getMinMax(&jmin, &jmax);
	float3 jcentererpos = (jmax + jmin)*0.5f;

	tmpmodel->m_oLod0.m_pModel->m_pVertexBuffer->Lock(0, 0, (void**)&pData, 0);

	for (int i = 0; i < tmpmodel->m_oLod0.m_pModel->m_uiAllVertexCount; ++i)
	{
		pData[i].Pos.x -= jcentererpos.x;
		pData[i].Pos.y -= jcentererpos.y;
		pData[i].Pos.z -= jcentererpos.z;
		pData[i].Pos = SMVector3Transform(pData[i].Pos, m_mWorld);
		pData[i].Pos.x += jcentererpos.x;
		pData[i].Pos.y += jcentererpos.y;
		pData[i].Pos.z += jcentererpos.z;
	}

	tmpmodel->m_oLod0.m_pModel->m_pVertexBuffer->Unlock();

	
	jmin -= jcentererpos;
	jmax -= jcentererpos;

	jmin = SMVector3Transform(jmin, m_mWorld);
	jmax = SMVector3Transform(jmax, m_mWorld);

	jmin += jcentererpos;
	jmax += jcentererpos;
	tmpmodel->m_oLod0.m_pBoundVolume->setMinMax(&jmin, &jmax);
}

void CStaticGeom::clear()
{
	deleteArrIndexPtr();
	for (int i = 0; i < m_aAllGroups.size(); ++i)
	{
		mem_delete(m_aAllGroups[i]);
	}
	m_aAllGroups.clear();

	while (m_aAllModels.size() > 0)
	{
		deleteModelInArrCom(0);
		mem_delete(m_aAllModels[0]);
		m_aAllModels.erase(0);
	}
	m_aAllModels.clear();

	while (m_aArrComFor.size() > 2)
	{
		mem_delete(m_aArrComFor[2]);
		m_aArrComFor.erase(2);
	}


	mem_delete_a(m_pCurrArrMeshVertex);
	mem_delete_a(m_pCurrArrMeshIndex);
	//CountVertex = 0;

	float3 jmin(0, 0, 0), jmax(0, 0, 0);
	m_pBoundVolume->setMinMax(&jmin, &jmax);
}

void CStaticGeom::getArrBuffsGeom(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, ID*** arr_mtl, int32_t** arr_count_index, int32_t* count_models)
{
	(*count_models) = m_aAllModels.size();
	(*arr_vertex) = new float3_t*[m_aAllModels.size()];
	(*arr_index) = new uint32_t*[m_aAllModels.size()];
	(*arr_mtl) = new ID*[m_aAllModels.size()];
	(*arr_count_vertex) = new int32_t[m_aAllModels.size()];
	(*arr_count_index) = new int32_t[m_aAllModels.size()];

	vertex_static* pData;

	for (int i = 0; i < m_aAllModels.size(); ++i)
	{
		int sizevertex = 0;
		int sizeindex = 0;
		for (int k = 0; k < m_aAllModels[i]->m_aSubSets.size(); ++k)
		{
			sizevertex += m_aAllModels[i]->m_aSubSets[k].m_uiVertexCount;
			sizeindex += m_aAllModels[i]->m_aSubSets[k].m_uiIndexCount;
		}

		(*arr_vertex)[i] = new float3_t[sizevertex];
		(*arr_index)[i] = new uint32_t[sizeindex];
		(*arr_mtl)[i] = new ID[sizeindex];

		(*arr_count_vertex)[i] = sizevertex;
		(*arr_count_index)[i] = sizeindex;

		Array<int,1024> tmpbiasindex;

		int curr_size_vertex = 0;
		int tmp_pre_bias = 0;
		for (int k = 0; k < m_aAllModels[i]->m_aSubSets.size(); ++k)
		{
			m_aAllGroups[m_aAllModels[i]->m_aSubSets[k].m_idGroup]->m_aVertexBuffers[m_aAllModels[i]->m_aSubSets[k].m_idBuff]->Lock(0, 0, (void**)&pData, 0);

			for (int j = 0; j < m_aAllModels[i]->m_aSubSets[k].m_uiVertexCount; ++j)
			{
				(*arr_vertex)[i][curr_size_vertex] = (float3_t)pData[m_aAllModels[i]->m_aSubSets[k].m_idVertexStart + j].Pos;
				++curr_size_vertex;
			}

			tmpbiasindex[k] = 0;
			tmpbiasindex[k] = (tmpbiasindex[k] - m_aAllModels[i]->m_aSubSets[k].m_idVertexStart) + tmp_pre_bias;
			tmp_pre_bias += m_aAllModels[i]->m_aSubSets[k].m_uiVertexCount;

			m_aAllGroups[m_aAllModels[i]->m_aSubSets[k].m_idGroup]->m_aVertexBuffers[m_aAllModels[i]->m_aSubSets[k].m_idBuff]->Unlock();
		}

		Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> queue;
		int tmpcount = 0;
		queue.push_back(m_aAllModels[i]->m_pArrSplits);
		int curr_size_index = 0;
		while (queue.size())
		{
			if (queue[0]->m_isNonEnd)
			{
				for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
				{
					if (queue[0]->m_aSplits[i])
						queue.push_back(queue[0]->m_aSplits[i]);
				}
			}
			else
			{
				CSegment* split = queue[0];
				for (int k = 0; k < split->m_uiCountSubSet; ++k)
				{
					for (int j = 0; j < split->m_pCountPoly[k]*3; ++j)
					{
						(*arr_index)[i][curr_size_index] = split->m_ppArrPoly[k][j] + tmpbiasindex[split->m_pNumberGroupModel[k]];
						(*arr_mtl)[i][curr_size_index] = m_aAllGroups[split->m_pNumberGroup[k]]->m_idTexture;
						++curr_size_index;
					}
				}
			}

			queue.erase(0);
			++tmpcount;
		}
	}
}

ID CStaticGeom::getModelCountGroups(ID id)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id, -1);

	return m_aAllModels[id]->m_aSubSets.size();
}

ID CStaticGeom::getModelGroupIDMat(ID id, ID group)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group , - 1);

	return m_aAllModels[id]->m_aIDsTexures[group];
}

void CStaticGeom::getModelGroupCenter(ID id, ID group, float3_t* center)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group, _VOID);

	*center = m_aAllModels[id]->m_aSubSets[group].m_vCenter;
}

void CStaticGeom::getModelGroupMin(ID id, ID group, float3_t* min)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group, _VOID);

	*min = m_aAllModels[id]->m_aSubSets[group].m_vMin;
}

void CStaticGeom::getModelGroupMax(ID id, ID group, float3_t* max)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group, _VOID);

	*max = m_aAllModels[id]->m_aSubSets[group].m_vMax;
}

void CStaticGeom::getModelGroupPlane(ID id, ID group, D3DXPLANE* plane)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group, _VOID);

	*plane = m_aAllModels[id]->m_aSubSets[group].m_oPlane;
}

void CStaticGeom::getPartBeam(const float3* pos, const float3 * dir, CSegment** arrsplits, int *count, CSegment* comsegment, ID curr_splits_ids_render)
{
	float3 center;
	float radius;
	comsegment->m_pBoundVolumeP->getSphere(&center, &radius);

	float distsqr = SGCore_0DistancePointBeam2(center, *pos, *dir);
	if (comsegment->m_uiCountAllPoly > 0 && distsqr <= radius*radius)
	{
		if (comsegment->m_isNonEnd)
		{
			for (int j = 0; j<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
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

bool CStaticGeom::traceBeam(const float3* start, const float3* dir, float3* _res, ID* idmodel, ID* idmtl)
{
	if (m_aAllModels.size() <= 0)
		return false;

	CTriangle tmptri;
	bool tmpiscom = true;
	float3 ip;
	float3 res;
	float3 il;
	res = (*start) + float3(10000.0f, 10000.0f, 10000.0f);
	il = (*dir) * 10000.0f;
	bool found = false;

	for (int id = 0; id < m_aAllModels.size(); ++id)
	{
		m_aArrComFor[1]->m_aIRS[id]->m_iCountCom = 0;

		getPartBeam(start, dir, m_aArrComFor[1]->m_aIRS[id]->m_ppSegments, &(m_aArrComFor[1]->m_aIRS[id]->m_iCountCom), m_aAllModels[id]->m_pArrSplits, m_aArrComFor[1]->m_aIRS[id]->m_iCount);

		for (DWORD k = 0; k<m_aArrComFor[1]->m_aIRS[id]->m_iCountCom; ++k)
		{
			for (DWORD group = 0; group<m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_uiCountSubSet; group++)
			{
				ID idbuff = m_aAllModels[id]->m_aSubSets[m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_pNumberGroupModel[group]].m_idBuff;
				ID idgroup = m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_pNumberGroup[group];

				vertex_static* pData = 0;
				if (FAILED(m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Lock(0, 0, (void**)&pData, 0)))
					continue;

				for (DWORD numpoly = 0; numpoly<m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_pCountPoly[group] * 3; numpoly += 3)
				{
					tmptri.m_vA = pData[m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_ppArrPoly[group][numpoly]].Pos;
					tmptri.m_vB = pData[m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_ppArrPoly[group][numpoly + 1]].Pos;
					tmptri.m_vC = pData[m_aArrComFor[1]->m_aIRS[id]->m_ppSegments[k]->m_ppArrPoly[group][numpoly + 2]].Pos;

					if (tmptri.IntersectLine((*start), il, &ip))
					{
						if (SMVector3Length2((*start) - res) > SMVector3Length2((*start) - ip))
						{
							res = ip;
							found = true;

							if (idmodel)
								*idmodel = id;

							if (idmtl)
								*idmtl = m_aAllGroups[idgroup]->m_idTexture;
						}
					}
				}

				m_aAllGroups[idgroup]->m_aVertexBuffers[idbuff]->Unlock();
			}
		}
	}

	if (found && _res)
		*_res = res;

	return found;
}
