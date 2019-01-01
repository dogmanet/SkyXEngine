
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "models.h"

CModels::CModels(bool isServerMode)
{
	//объект расчетов видимости для наблюдателя
	m_aVisInfo.push_back(new CVisInfo());

	//объект расчетов видимости для трассировки луча
	m_aVisInfo.push_back(new CVisInfo());

	m_pCurrArrMeshVertex = 0;
	m_pCurrArrMeshIndex = 0;

	m_isServerMode = isServerMode;
}

CModels::~CModels()
{
	for (int i = 0, il = m_aModels.size(); i < il; ++i)
	{
		mem_delete(m_aModels[i]);
	}
}


void CModels::onResetDevice()
{
	for(int i = 0, l = m_aModels.size(); i < l; ++i)
	{
		g_pDXDevice->CreateIndexBuffer(
			m_aModels[i]->m_pModel->m_uiAllIndexCount * sizeof(UINT),
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX32,
			D3DPOOL_DEFAULT,
			&(m_aModels[i]->m_pVisibleIndexBuffer),
			0);

		m_aModels[i]->m_pModel->syncBuffers();
	}
	for(int i = 0, l = m_aTransparency.size(); i < l; ++i)
	{
		m_aTransparency[i]->syncBuffers();
	}
}

void CModels::onLostDevice()
{
	for(int i = 0, l = m_aModels.size(); i < l; ++i)
	{
		mem_release_del(m_aModels[i]->m_pVisibleIndexBuffer);

		mem_release_del(m_aModels[i]->m_pModel->m_pIndexBuffer);
		mem_release_del(m_aModels[i]->m_pModel->m_pVertexBuffer);
	}
	for(int i = 0, l = m_aTransparency.size(); i < l; ++i)
	{
		mem_release_del(m_aTransparency[i]->m_pIndexBuffer);
		mem_release_del(m_aTransparency[i]->m_pVertexBuffer);
	}
}

//**************************************************************************

CModels::CPhysMesh::CPhysMesh()
{
	m_sPath = "";
}

CModels::CPhysMesh::~CPhysMesh()
{

}

//**************************************************************************

CModels::CLod::CLod()
{
	m_pModel = 0;
}

CModels::CLod::~CLod()
{
	m_aIDsTextures.clear();
	mem_delete(m_pModel);
}

//**************************************************************************

CModels::CSegment::CSegment()
{
	for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
	{
		m_aSplits[i] = 0;
	}

	m_ppArrPoly = 0;
	m_pParent = 0;
	m_pCountPoly = 0;
	m_pNumberGroup = 0;
	m_uiCountSubSet = 0;
	m_uiCountAllPoly = 0;
	m_pBoundVolumeSys = 0;
	m_pBoundVolumeP = 0;
	m_ID = -1;
	m_SID = -1;
	m_idDepth = 0;
	m_isFinite = true;
	//m_pBoundBox = 0;
}

CModels::CSegment::~CSegment()
{
	for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; i++)
	{
		mem_delete(m_aSplits[i]);
	}
	mem_delete_a(m_pNumberGroup);
	mem_delete_a(m_pCountPoly);

	if (m_ppArrPoly)
	{
		for (int i = 0; i < m_uiCountSubSet; ++i)
		{
			mem_delete_a(m_ppArrPoly[i]);
		}
	}

	mem_delete_a(m_ppArrPoly);
	mem_release_del(m_pBoundVolumeSys);
	mem_release_del(m_pBoundVolumeP);
}

//**************************************************************************

CModels::CTransparencyModel::CTransparencyModel()
{
	m_idModel = -1;
	m_isVisible4Observer = false;
	m_fDist4Observer = 0;
	m_sTex = "";
	m_idTex = -1;
	m_iCountVertex = 0;
	m_iCountIndex = 0;
	m_pVertexBuffer = 0;
	m_pIndexBuffer = 0;
	m_pBoundVolume = SGCore_CrBound();
}

CModels::CTransparencyModel::~CTransparencyModel()
{
	mem_release(m_pVertexBuffer);
	mem_release(m_pIndexBuffer);
	mem_release(m_pBoundVolume);
	mem_delete_a(m_pIndices);
	mem_delete_a(m_pVertices);
}

void CModels::CTransparencyModel::syncBuffers(bool bRecreate)
{
	if(bRecreate)
	{
		mem_release_del(m_pVertexBuffer);
		mem_release_del(m_pIndexBuffer);
	}

	if(!m_pVertexBuffer)
	{
		DX_CALL(g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static_ex)* m_iCountVertex, D3DUSAGE_WRITEONLY, NULL, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0));
	}

	vertex_static_ex *pVertex;
	if(SUCCEEDED(DX_CALL(m_pVertexBuffer->Lock(0, 0, (void **)&pVertex, 0))))
	{
		memcpy(pVertex, m_pVertices, sizeof(vertex_static_ex)* m_iCountVertex);
		m_pVertexBuffer->Unlock();
	}

	if(!m_pIndexBuffer)
	{
		DX_CALL(g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* m_iCountIndex, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &m_pIndexBuffer, 0));
	}

	UINT *pIndex;
	if(SUCCEEDED(DX_CALL(m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0))))
	{
		memcpy(pIndex, m_pIndices, sizeof(uint32_t)* m_iCountIndex);
		m_pIndexBuffer->Unlock();
	}
}

//**************************************************************************

CModels::CGroupInfo::CGroupInfo()
{

}

CModels::CGroupInfo::~CGroupInfo()
{

}

//**************************************************************************

CModels::CModel::CModel()
{
	m_sName = "";
	m_sPath = "";

	m_isRenderLod = false;
	m_fDist4Observer = 0;

	m_pPhysics = 0;

	m_pLod = 0;
	m_pModel = 0;
	m_pArrSplits = 0;
	m_idCountSplits = 0;
	m_idCountSplitsRender = 0;
	m_pBoundVolume = SGCore_CrBound();
	m_pVisibleIndexBuffer = 0;
}

CModels::CModel::~CModel()
{
	mem_release(m_pModel);
	mem_release(m_pBoundVolume);

	for (int i = 0, il = m_aGroupInfo.size(); i < il; ++i)
	{
		mem_delete(m_aGroupInfo[i]);
	}
}

//**************************************************************************

CModels::CVisInfoModel::CVisInfoModel()
{
	m_isVisible = false;
	m_ppSegments = 0;
	m_iCount = 0;
	m_iCountCom = 0;
}

CModels::CVisInfoModel::~CVisInfoModel()
{
	mem_delete(m_ppSegments);
}


CModels::CVisInfo::CVisInfo()
{

}

CModels::CVisInfo::~CVisInfo()
{
	for (int i = 0, il = m_aVisible4Model.size(); i < il; ++i)
	{
		mem_delete(m_aVisible4Model[i]);
	}
}

//##########################################################################

void CModels::save(const char *szPath)
{
	FILE *pFile = fopen(szPath, "wb");

	if (pFile == NULL)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "failed to open (save) file static geometry [%s]\n", szPath);
		return;
	}

	//сохраняем магическое число
	fwrite(SX_GEOM_MAGIC_WORD, sizeof(char)* strlen(SX_GEOM_MAGIC_WORD), 1, pFile);

	//сохраняем версию формата
	uint32_t uiFmtVersion = SX_GEOM_FILE_FORMAT_VERSION;
	fwrite(&uiFmtVersion, sizeof(uint32_t), 1, pFile);

	long lPositionSize = ftell(pFile);
	uint32_t uiSizeFile = 0;
	fwrite(&uiSizeFile, sizeof(uint32_t), 1, pFile);

	//сохраняем количество моделей
	int32_t iCountModel = m_aModels.size();
	fwrite(&iCountModel, sizeof(int32_t), 1, pFile);

	for (int i = 0; i < m_aModels.size(); ++i)
	{
		//записываем данные - размер блока, после записи модели данные обновляются
		long lPosModel = ftell(pFile);
		uint32_t uiSizeBlock = lPosModel;
		fwrite(&uiSizeBlock, sizeof(uint32_t), 1, pFile);

		//сохраняем размер имени и имя
		int32_t iStrLen = m_aModels[i]->m_sName.length();
		fwrite(&iStrLen, sizeof(int32_t), 1, pFile);
		fwrite(m_aModels[i]->m_sName.c_str(), sizeof(char), iStrLen, pFile);

		//сохраняем трансформации
		float3 vPosition, vRotation, vScale;
		m_aModels[i]->m_pBoundVolume->getPosition(&vPosition);
		m_aModels[i]->m_pBoundVolume->getRotation(&vRotation);
		m_aModels[i]->m_pBoundVolume->getScale(&vScale);

		fwrite(&vPosition, sizeof(float3_t), 1, pFile);
		fwrite(&vRotation, sizeof(float3_t), 1, pFile);
		fwrite(&vScale, sizeof(float3_t), 1, pFile);

		//сохранение данных модели для рендера
		//{
		//сохраняем размер пути до модели и путь до модели
		iStrLen = m_aModels[i]->m_sPath.length();
		fwrite(&iStrLen, sizeof(int32_t), 1, pFile);
		fwrite(m_aModels[i]->m_sPath.c_str(), sizeof(char), iStrLen, pFile);

		//количество подгрупп в основной модели
		uint32_t iCountSubset = m_aModels[i]->m_pModel->m_uiSubsetCount;
		fwrite(&(m_aModels[i]->m_pModel->m_uiSubsetCount), sizeof(uint32_t), 1, pFile);

		//если есть подгруппы, может не быть если загрузить модель у которой все материалы пп
		if (iCountSubset > 0)
		{
			//общее количество вершин
			uint32_t iAllCountVertex = m_aModels[i]->m_pModel->m_uiAllVertexCount;
			fwrite(&iAllCountVertex, sizeof(uint32_t), 1, pFile);

			//общее количество индексов
			uint32_t iAllCountIndex = m_aModels[i]->m_pModel->m_uiAllIndexCount;
			fwrite(&iAllCountIndex, sizeof(uint32_t), 1, pFile);

			//сохранение массива имен текстур без расширения в виде: int32_t - количество символов, array char - строка
			for (int k = 0; k < iCountSubset; ++k)
			{
				iStrLen = strlen(m_aModels[i]->m_pModel->m_ppTextures[k]);
				fwrite(&iStrLen, sizeof(int32_t), 1, pFile);
				fwrite(m_aModels[i]->m_pModel->m_ppTextures[k], sizeof(char)* iStrLen, 1, pFile);
			}

			//сохранение массива стартовых индексов
			fwrite(m_aModels[i]->m_pModel->m_pStartIndex, sizeof(uint32_t), iCountSubset, pFile);

			//сохранение массива количества индексов
			fwrite(m_aModels[i]->m_pModel->m_pIndexCount, sizeof(uint32_t), iCountSubset, pFile);

			//сохранение массива стартовых вершин
			fwrite(m_aModels[i]->m_pModel->m_pStartVertex, sizeof(uint32_t), iCountSubset, pFile);

			//сохранение массива количества вершин
			fwrite(m_aModels[i]->m_pModel->m_pVertexCount, sizeof(uint32_t), iCountSubset, pFile);


			//сохраняем данные вершинного и индексного буферов
			//UINT *pIndex;
			//m_aModels[i]->m_pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0);
			//vertex_static_ex *pVertex;
			//m_aModels[i]->m_pModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVertex, 0);

			//fwrite(pVertex, sizeof(vertex_static_ex), iAllCountVertex, pFile);
			//fwrite(pIndex, sizeof(uint32_t), iAllCountIndex, pFile);

			//m_aModels[i]->m_pModel->m_pVertexBuffer->Unlock();
			//m_aModels[i]->m_pModel->m_pIndexBuffer->Unlock();

			fwrite(m_aModels[i]->m_pModel->m_pVertices, sizeof(vertex_static_ex), iAllCountVertex, pFile);
			fwrite(m_aModels[i]->m_pModel->m_pIndices, sizeof(uint32_t), iAllCountIndex, pFile);
		}
		//}

		//сохранение данных лода
		//{

		//сохранение значения "есть ли лод"
		uint8_t ui8ExistsLod = (m_aModels[i]->m_pLod != 0);
		fwrite(&ui8ExistsLod, sizeof(uint8_t), 1, pFile);

		if (ui8ExistsLod)
		{
			//сохраняем размер пути до модели и путь до модели
			iStrLen = m_aModels[i]->m_pLod->m_sPath.length();
			fwrite(&iStrLen, sizeof(int32_t), 1, pFile);
			fwrite(m_aModels[i]->m_pLod->m_sPath.c_str(), sizeof(char), iStrLen, pFile);

			//количество подгрупп в модели
			int32_t iCountSubsetLod = m_aModels[i]->m_pLod->m_pModel->m_uiSubsetCount;
			fwrite(&(m_aModels[i]->m_pLod->m_pModel->m_uiSubsetCount), sizeof(uint32_t), 1, pFile);

			//общее количество вершин
			uint32_t iAllCountVertexLod = m_aModels[i]->m_pLod->m_pModel->m_uiAllVertexCount;
			fwrite(&iAllCountVertexLod, sizeof(uint32_t), 1, pFile);

			//общее количество индексов
			uint32_t iAllCountIndexLod = m_aModels[i]->m_pLod->m_pModel->m_uiAllIndexCount;
			fwrite(&iAllCountIndexLod, sizeof(uint32_t), 1, pFile);

			//сохранение массива имен текстур без расширеня в виде: int32_t - количество символов, array char - строка
			for (int k = 0; k < iCountSubsetLod; ++k)
			{
				iStrLen = strlen(m_aModels[i]->m_pLod->m_pModel->m_ppTextures[k]);
				fwrite(&iStrLen, sizeof(int32_t), 1, pFile);
				fwrite(m_aModels[i]->m_pLod->m_pModel->m_ppTextures[k], sizeof(char)* iStrLen, 1, pFile);
			}

			fwrite(m_aModels[i]->m_pLod->m_pModel->m_pStartIndex, sizeof(uint32_t), iCountSubsetLod, pFile);
			fwrite(m_aModels[i]->m_pLod->m_pModel->m_pIndexCount, sizeof(uint32_t), iCountSubsetLod, pFile);
			fwrite(m_aModels[i]->m_pLod->m_pModel->m_pStartVertex, sizeof(uint32_t), iCountSubsetLod, pFile);
			fwrite(m_aModels[i]->m_pLod->m_pModel->m_pVertexCount, sizeof(uint32_t), iCountSubsetLod, pFile);

			//сохраняем данные вершинного и индексного буферов
			//UINT *pIndexLod;
			//m_aModels[i]->m_pLod->m_pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndexLod, 0);
			//vertex_static_ex *pVertexLod;
			//m_aModels[i]->m_pLod->m_pModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVertexLod, 0);

			//fwrite(pVertexLod, sizeof(vertex_static_ex), iAllCountVertexLod, pFile);
			fwrite(m_aModels[i]->m_pLod->m_pModel->m_pVertices, sizeof(vertex_static_ex), iAllCountVertexLod, pFile);
			//fwrite(pIndexLod, sizeof(uint32_t), iAllCountIndexLod, pFile);
			fwrite(m_aModels[i]->m_pLod->m_pModel->m_pIndices, sizeof(uint32_t), iAllCountIndexLod, pFile);

			//m_aModels[i]->m_pLod->m_pModel->m_pVertexBuffer->Unlock();
			//m_aModels[i]->m_pLod->m_pModel->m_pIndexBuffer->Unlock();
		}

		//}


		//сохранение данных физической модели
		//{

		//сохранение значения "есть ли физическая модель"
		int8_t i8ExistsPhysics = (m_aModels[i]->m_pPhysics != 0);
		fwrite(&i8ExistsPhysics, sizeof(int8_t), 1, pFile);

		if (i8ExistsPhysics)
		{
			iStrLen = m_aModels[i]->m_pPhysics->m_sPath.length();
			fwrite(&iStrLen, sizeof(int32_t), 1, pFile);
			fwrite(m_aModels[i]->m_pPhysics->m_sPath.c_str(), sizeof(char)* iStrLen, 1, pFile);

			uint32_t uiSizeArr = m_aModels[i]->m_pPhysics->m_aVertex.size();
			fwrite(&uiSizeArr, sizeof(int32_t), 1, pFile);
			fwrite(&(m_aModels[i]->m_pPhysics->m_aVertex[0]), sizeof(float3_t), m_aModels[i]->m_pPhysics->m_aVertex.size(), pFile);

			uiSizeArr = m_aModels[i]->m_pPhysics->m_aIndex.size();
			fwrite(&uiSizeArr, sizeof(int32_t), 1, pFile);
			fwrite(&(m_aModels[i]->m_pPhysics->m_aIndex[0]), sizeof(uint32_t), m_aModels[i]->m_pPhysics->m_aIndex.size(), pFile);

			uiSizeArr = m_aModels[i]->m_pPhysics->m_aNumMtrl.size();
			fwrite(&uiSizeArr, sizeof(int32_t), 1, pFile);
			fwrite(&(m_aModels[i]->m_pPhysics->m_aNumMtrl[0]), sizeof(ID), m_aModels[i]->m_pPhysics->m_aNumMtrl.size(), pFile);

			uiSizeArr = m_aModels[i]->m_pPhysics->m_aStringMtrl.size();
			fwrite(&uiSizeArr, sizeof(uint32_t), 1, pFile);

			for (int k = 0; k < uiSizeArr; ++k)
			{
				iStrLen = m_aModels[i]->m_pPhysics->m_aStringMtrl[k].length();
				fwrite(&iStrLen, sizeof(uint32_t), 1, pFile);
				fwrite(m_aModels[i]->m_pPhysics->m_aStringMtrl[k].c_str(), sizeof(char)* iStrLen, 1, pFile);
			}
		}

		//}


		//сохранение пп моделей
		//{

		uint32_t uiCountPtansparency = m_aModels[i]->m_aTransparency.size();
		fwrite(&uiCountPtansparency, sizeof(uint32_t), 1, pFile);
		if (uiCountPtansparency > 0)
			fwrite(&(m_aModels[i]->m_aTransparency[0]), sizeof(ID), uiCountPtansparency, pFile);

		//}


		//сохранение сегментов модели, если есть
		//{
		int8_t i8ExistsSegments= (m_aModels[i]->m_pArrSplits != 0);
		fwrite(&i8ExistsSegments, sizeof(int8_t), 1, pFile);

		if (i8ExistsSegments)
		{
			Array<CSegment*> aQueue;
			int iCountSplits = 0;
			aQueue.push_back(m_aModels[i]->m_pArrSplits);

			while (aQueue.size() > iCountSplits)
			{
				CSegment *pSplit = aQueue[iCountSplits];
				float3 vMin, vMax;
				pSplit->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);
				fwrite(&vMin, sizeof(float3_t), 1, pFile);
				fwrite(&vMax, sizeof(float3_t), 1, pFile);

				pSplit->m_pBoundVolumeP->getMinMax(&vMin, &vMax);
				fwrite(&vMin, sizeof(float3_t), 1, pFile);
				fwrite(&vMax, sizeof(float3_t), 1, pFile);

				fwrite(&pSplit->m_idDepth, sizeof(ID), 1, pFile);

				fwrite(&pSplit->m_uiCountAllPoly, sizeof(uint32_t), 1, pFile);

				int8_t iIsFinite = pSplit->m_isFinite;
				fwrite(&iIsFinite, sizeof(int8_t), 1, pFile);

				if (!(pSplit->m_isFinite))
				{
					int8_t isExists = 0;
					for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
					{
						if (pSplit->m_aSplits[i])
						{
							isExists = 1;
							fwrite(&isExists, sizeof(int8_t), 1, pFile);
							aQueue.push_back(pSplit->m_aSplits[i]);
						}
						else
						{
							isExists = 0;
							fwrite(&isExists, sizeof(int8_t), 1, pFile);
						}
					}
				}
				else
				{
					fwrite(&(pSplit->m_uiCountSubSet), sizeof(uint32_t), 1, pFile);
					fwrite(pSplit->m_pNumberGroup, sizeof(uint32_t)*pSplit->m_uiCountSubSet, 1, pFile);
					fwrite(pSplit->m_pCountPoly, sizeof(uint32_t)*pSplit->m_uiCountSubSet, 1, pFile);

					for (int i = 0; i < pSplit->m_uiCountSubSet; ++i)
					{
						fwrite(pSplit->m_ppArrPoly[i], sizeof(uint32_t)*pSplit->m_pCountPoly[i] * 3, 1, pFile);
					}
				}


				++iCountSplits;
			}
		}

		//запоминаем текущую позицию в файле, перемещаемся к началу записи текущей модели, записываем размер блока, перемещаемся в конец для дальнейшей записи
		long lCurrPos = ftell(pFile);
		uiSizeBlock = lCurrPos - uiSizeBlock;
		fseek(pFile, lPosModel, SEEK_SET);
		fwrite(&uiSizeBlock, sizeof(uint32_t), 1, pFile);
		fseek(pFile, lCurrPos, SEEK_SET);

		//}
	}

	uint32_t uiCountTransparency = m_aTransparency.size();
	fwrite(&uiCountTransparency, sizeof(uint32_t), 1, pFile);

	//сохранение массива пп моделей
	for (int i = 0; i < m_aTransparency.size(); ++i)
	{
		//записываем данные - размер блока, после записи модели данные обновляются
		long lPosModel = ftell(pFile);
		uint32_t uiSizeBlock = lPosModel;
		fwrite(&uiSizeBlock, sizeof(uint32_t), 1, pFile);

		fwrite(&(m_aTransparency[i]->m_idModel), sizeof(ID), 1, pFile);

		int32_t iStrLen = m_aTransparency[i]->m_sTex.length();
		fwrite(&iStrLen, sizeof(int32_t), 1, pFile);
		fwrite(m_aTransparency[i]->m_sTex.c_str(), sizeof(char)* iStrLen, 1, pFile);

		fwrite(&(m_aTransparency[i]->m_iCountVertex), sizeof(int32_t), 1, pFile);
		fwrite(&(m_aTransparency[i]->m_iCountIndex), sizeof(int32_t), 1, pFile);

		//сохраняем данные вершинного и индексного буферов
		UINT *pIndexTrancparency = m_aTransparency[i]->m_pIndices;
		//m_aTransparency[i]->m_pIndexBuffer->Lock(0, 0, (void **)&pIndexTrancparency, 0);
		vertex_static_ex *pVertexTrancparency = m_aTransparency[i]->m_pVertices;
		//m_aTransparency[i]->m_pVertexBuffer->Lock(0, 0, (void **)&pVertexTrancparency, 0);

		fwrite(pVertexTrancparency, sizeof(vertex_static_ex), m_aTransparency[i]->m_iCountVertex, pFile);
		fwrite(pIndexTrancparency, sizeof(uint32_t), m_aTransparency[i]->m_iCountIndex, pFile);

		//m_aTransparency[i]->m_pVertexBuffer->Unlock();
		//m_aTransparency[i]->m_pIndexBuffer->Unlock();

		//запоминаем текущую позицию в файле, перемещаемся к началу записи текущей модели, записываем размер блока, перемещаемся в конец для дальнейшей записи
		long lCurrPos = ftell(pFile);
		uiSizeBlock = lCurrPos - uiSizeBlock;
		fseek(pFile, lPosModel, SEEK_SET);
		fwrite(&uiSizeBlock, sizeof(uint32_t), 1, pFile);
		fseek(pFile, lCurrPos, SEEK_SET);
	}

	uiSizeFile = ftell(pFile);
	fseek(pFile, lPositionSize, SEEK_SET);
	fwrite(&uiSizeFile, sizeof(uint32_t), 1, pFile);

	fclose(pFile);
}

void CModels::load(const char *szPath)
{
	FILE *pFile = fopen(szPath, "rb");

	if (pFile == NULL)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "failed to open (load) file static geometry [%s]\n", szPath);
		return;
	}

	//первичные проверки
	//{
	//считывание и проверка магического слова
	uint64_t ui64Magic;
	fread(&ui64Magic, sizeof(uint64_t), 1, pFile);

	if (ui64Magic != SX_GEOM_MAGIC_NUM)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "file static geometry [%s] is not static geometry\n", szPath);
		fclose(pFile);
		return;
	}

	//считывание и проверка поддерживаемой версии
	uint32_t uiFmtVersion;
	fread(&uiFmtVersion, sizeof(uint32_t), 1, pFile);

	if (uiFmtVersion != SX_GEOM_FILE_FORMAT_VERSION)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "file static geometry [%s] have unduported version %d\n", szPath, uiFmtVersion);
		fclose(pFile);
		return;
	}

	//считывание и проверка размера файла
	uint32_t uiSizeFile;
	fread(&uiSizeFile, sizeof(uint32_t), 1, pFile);
	long lCurrPos = ftell(pFile);
	fseek(pFile, 0, SEEK_END);

	if (uiSizeFile != ftell(pFile))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "file static geometry [%s] damaged, whole file %d\n", szPath);
		fclose(pFile);
		return;
	}

	fseek(pFile, lCurrPos, SEEK_SET);
	//}

	int32_t iCountModel = 0;
	fread(&iCountModel, sizeof(int32_t), 1, pFile);
	
	for (int i = 0; i < iCountModel; ++i)
	{
		uint32_t uiSizeBlock = 0;
		fread(&uiSizeBlock, sizeof(uint32_t), 1, pFile);

		if (uiSizeBlock >= uiSizeFile)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "file static geometry [%s] damaged, block size (model) %d\n", szPath);
			fclose(pFile);
			return;
		}

		CModel *pModel = new CModel();
		int32_t iStrLen = 0;
		char szStr[1024];

		fread(&iStrLen, sizeof(int32_t), 1, pFile);
		fread(szStr, sizeof(char), iStrLen, pFile);
		szStr[iStrLen] = 0;
		pModel->m_sName = szStr;

		//считываем трансформации
		//{
		float3 vPosition, vRotation, vScale;
		fread(&(vPosition.x), sizeof(float), 1, pFile);
		fread(&(vPosition.y), sizeof(float), 1, pFile);
		fread(&(vPosition.z), sizeof(float), 1, pFile);

		fread(&(vRotation.x), sizeof(float), 1, pFile);
		fread(&(vRotation.y), sizeof(float), 1, pFile);
		fread(&(vRotation.z), sizeof(float), 1, pFile);

		fread(&(vScale.x), sizeof(float), 1, pFile);
		fread(&(vScale.y), sizeof(float), 1, pFile);
		fread(&(vScale.z), sizeof(float), 1, pFile);

		pModel->m_pBoundVolume->setPosition(&vPosition);
		pModel->m_pBoundVolume->setRotation(&vRotation);
		pModel->m_pBoundVolume->setScale(&vScale);
		//}

		fread(&iStrLen, sizeof(int32_t), 1, pFile);
		fread(szStr, sizeof(char), iStrLen, pFile);
		szStr[iStrLen] = 0;
		pModel->m_sPath = szStr;

		//создаем статическую модель и считываем для нее данные
		//{
		pModel->m_pModel = SGCore_StaticModelCr();
		fread(&(pModel->m_pModel->m_uiSubsetCount), sizeof(uint32_t), 1, pFile);

		//если модель не пустая (есть не пп подгруппы)
		if (pModel->m_pModel->m_uiSubsetCount)
		{
			fread(&(pModel->m_pModel->m_uiAllVertexCount), sizeof(uint32_t), 1, pFile);
			fread(&(pModel->m_pModel->m_uiAllIndexCount), sizeof(uint32_t), 1, pFile);

			pModel->m_pModel->m_ppTextures = new char*[pModel->m_pModel->m_uiSubsetCount];

			pModel->m_pModel->m_pStartIndex = new uint32_t[pModel->m_pModel->m_uiSubsetCount];
			pModel->m_pModel->m_pIndexCount = new uint32_t[pModel->m_pModel->m_uiSubsetCount];

			pModel->m_pModel->m_pStartVertex = new uint32_t[pModel->m_pModel->m_uiSubsetCount];
			pModel->m_pModel->m_pVertexCount = new uint32_t[pModel->m_pModel->m_uiSubsetCount];

			for (int k = 0; k < pModel->m_pModel->m_uiSubsetCount; ++k)
			{
				fread(&iStrLen, sizeof(int32_t), 1, pFile);
				fread(szStr, sizeof(char), iStrLen, pFile);
				pModel->m_pModel->m_ppTextures[k] = new char[iStrLen + 1];
				szStr[iStrLen] = 0;
				strcpy(pModel->m_pModel->m_ppTextures[k], szStr);
				sprintf(szStr, "%s.dds", pModel->m_pModel->m_ppTextures[k]);
				pModel->m_aIDsTextures.push_back(m_isServerMode ? -1 : SGCore_MtlLoad(szStr, MTL_TYPE_GEOM));
			}

			fread(pModel->m_pModel->m_pStartIndex, sizeof(uint32_t), pModel->m_pModel->m_uiSubsetCount, pFile);
			fread(pModel->m_pModel->m_pIndexCount, sizeof(uint32_t), pModel->m_pModel->m_uiSubsetCount, pFile);
			fread(pModel->m_pModel->m_pStartVertex, sizeof(uint32_t), pModel->m_pModel->m_uiSubsetCount, pFile);
			fread(pModel->m_pModel->m_pVertexCount, sizeof(uint32_t), pModel->m_pModel->m_uiSubsetCount, pFile);
			
			pModel->m_pModel->m_pVertices = new vertex_static_ex[pModel->m_pModel->m_uiAllVertexCount];
			pModel->m_pModel->m_pIndices = new UINT[pModel->m_pModel->m_uiAllIndexCount];

			fread(pModel->m_pModel->m_pVertices, sizeof(vertex_static_ex), pModel->m_pModel->m_uiAllVertexCount, pFile);
			fread(pModel->m_pModel->m_pIndices, sizeof(uint32_t), pModel->m_pModel->m_uiAllIndexCount, pFile);

			//создаем баунд для модели
			pModel->m_pBoundVolume->calcBound(pModel->m_pModel->m_pVertices, pModel->m_pModel->m_uiAllVertexCount, sizeof(vertex_static_ex));

			if(!m_isServerMode)
			{
				pModel->m_pModel->syncBuffers(true);
			}
		}
		//}

		//считываем лод, если есть
		//{
		uint8_t ui8ExistsLod = 0;
		fread(&ui8ExistsLod, sizeof(uint8_t), 1, pFile);

		if (ui8ExistsLod)
		{
			pModel->m_pLod = new CLod();

			fread(&iStrLen, sizeof(int32_t), 1, pFile);
			fread(szStr, sizeof(char), iStrLen, pFile);
			szStr[iStrLen] = 0;
			pModel->m_pLod->m_sPath = szStr;

			pModel->m_pLod->m_pModel = SGCore_StaticModelCr();
			fread(&(pModel->m_pLod->m_pModel->m_uiSubsetCount), sizeof(uint32_t), 1, pFile);
			fread(&(pModel->m_pLod->m_pModel->m_uiAllVertexCount), sizeof(uint32_t), 1, pFile);
			fread(&(pModel->m_pLod->m_pModel->m_uiAllIndexCount), sizeof(uint32_t), 1, pFile);

			pModel->m_pLod->m_pModel->m_ppTextures = new char*[pModel->m_pLod->m_pModel->m_uiSubsetCount];

			pModel->m_pLod->m_pModel->m_pStartIndex = new uint32_t[pModel->m_pLod->m_pModel->m_uiSubsetCount];
			pModel->m_pLod->m_pModel->m_pIndexCount = new uint32_t[pModel->m_pLod->m_pModel->m_uiSubsetCount];

			pModel->m_pLod->m_pModel->m_pStartVertex = new uint32_t[pModel->m_pLod->m_pModel->m_uiSubsetCount];
			pModel->m_pLod->m_pModel->m_pVertexCount = new uint32_t[pModel->m_pLod->m_pModel->m_uiSubsetCount];

			for (int k = 0; k < pModel->m_pLod->m_pModel->m_uiSubsetCount; ++k)
			{
				fread(&iStrLen, sizeof(int32_t), 1, pFile);
				fread(szStr, sizeof(char), iStrLen, pFile);
				pModel->m_pLod->m_pModel->m_ppTextures[k] = new char[iStrLen + 1];
				szStr[iStrLen] = 0;
				strcpy(pModel->m_pLod->m_pModel->m_ppTextures[k], szStr);
				sprintf(szStr, "%s.dds", pModel->m_pLod->m_pModel->m_ppTextures[k]);
				pModel->m_pLod->m_aIDsTextures.push_back(m_isServerMode ? -1 : SGCore_MtlLoad(szStr, MTL_TYPE_GEOM));
			}

			fread(pModel->m_pLod->m_pModel->m_pStartIndex, sizeof(uint32_t), pModel->m_pLod->m_pModel->m_uiSubsetCount, pFile);
			fread(pModel->m_pLod->m_pModel->m_pIndexCount, sizeof(uint32_t), pModel->m_pLod->m_pModel->m_uiSubsetCount, pFile);
			fread(pModel->m_pLod->m_pModel->m_pStartVertex, sizeof(uint32_t), pModel->m_pLod->m_pModel->m_uiSubsetCount, pFile);
			fread(pModel->m_pLod->m_pModel->m_pVertexCount, sizeof(uint32_t), pModel->m_pLod->m_pModel->m_uiSubsetCount, pFile);

			/*
			g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static_ex)* pModel->m_pLod->m_pModel->m_uiAllVertexCount, NULL, NULL, D3DPOOL_MANAGED, &(pModel->m_pLod->m_pModel->m_pVertexBuffer), 0);
			g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* pModel->m_pLod->m_pModel->m_uiAllIndexCount, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &(pModel->m_pLod->m_pModel->m_pIndexBuffer), 0);

			UINT *pIndexLod;
			pModel->m_pLod->m_pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndexLod, 0);
			vertex_static_ex *pVertexLod;
			pModel->m_pLod->m_pModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVertexLod, 0);

			fread(pVertexLod, sizeof(vertex_static_ex), pModel->m_pLod->m_pModel->m_uiAllVertexCount, pFile);
			fread(pIndexLod, sizeof(uint32_t), pModel->m_pLod->m_pModel->m_uiAllIndexCount, pFile);

			pModel->m_pLod->m_pModel->m_pVertexBuffer->Unlock();
			pModel->m_pLod->m_pModel->m_pIndexBuffer->Unlock();
			*/

			pModel->m_pLod->m_pModel->m_pVertices = new vertex_static_ex[pModel->m_pLod->m_pModel->m_uiAllVertexCount];
			pModel->m_pLod->m_pModel->m_pIndices = new UINT[pModel->m_pLod->m_pModel->m_uiAllIndexCount];

			fread(pModel->m_pLod->m_pModel->m_pVertices, sizeof(vertex_static_ex), pModel->m_pLod->m_pModel->m_uiAllVertexCount, pFile);
			fread(pModel->m_pLod->m_pModel->m_pIndices, sizeof(uint32_t), pModel->m_pLod->m_pModel->m_uiAllIndexCount, pFile);

			if(!m_isServerMode)
			{
				pModel->m_pLod->m_pModel->syncBuffers(true);
			}
		}
		//}

		//считываем физическую модель, если есть
		//{
		uint8_t ui8ExistsPhysics = 0;
		fread(&ui8ExistsPhysics, sizeof(uint8_t), 1, pFile);

		if (ui8ExistsPhysics)
		{
			pModel->m_pPhysics = new CPhysMesh();

			fread(&iStrLen, sizeof(int32_t), 1, pFile);
			fread(szStr, sizeof(char), iStrLen, pFile);
			pModel->m_pPhysics->m_sPath = szStr;

			//загрузка размера массива вершин и вершин
			uint32_t uiSizeArr = 0;
			fread(&uiSizeArr, sizeof(int32_t), 1, pFile);
			pModel->m_pPhysics->m_aVertex.resize(uiSizeArr);
			fread(&(pModel->m_pPhysics->m_aVertex[0]), sizeof(float3_t), pModel->m_pPhysics->m_aVertex.size(), pFile);

			//загрузка размера массива индексов и индексов
			fread(&uiSizeArr, sizeof(int32_t), 1, pFile);
			pModel->m_pPhysics->m_aIndex.resize(uiSizeArr);
			fread(&(pModel->m_pPhysics->m_aIndex[0]), sizeof(uint32_t), pModel->m_pPhysics->m_aIndex.size(), pFile);

			//загрузка размеров массива с номерами материалов (для каждого индекса номер из локального массива) и номеров
			fread(&uiSizeArr, sizeof(int32_t), 1, pFile);
			pModel->m_pPhysics->m_aNumMtrl.resize(uiSizeArr);
			fread(&(pModel->m_pPhysics->m_aNumMtrl[0]), sizeof(uint32_t), pModel->m_pPhysics->m_aNumMtrl.size(), pFile);

			//загрузка массива с именами текстур (для материалов) и создание массива материалов для каждой подгруппы
			Array<ID> aIDs;
			fread(&uiSizeArr, sizeof(int32_t), 1, pFile);
			for (int k = 0; k < uiSizeArr; ++k)
			{
				fread(&iStrLen, sizeof(int32_t), 1, pFile);
				fread(szStr, sizeof(char), iStrLen, pFile);
				szStr[iStrLen] = 0;
				pModel->m_pPhysics->m_aStringMtrl.push_back(szStr);
				aIDs.push_back(SGCore_MtlLoad(szStr, MTL_TYPE_GEOM));
			}

			//назначение каждому индексу id материала
			pModel->m_pPhysics->m_aMtrl.resize(pModel->m_pPhysics->m_aIndex.size());
			for (int k = 0; k < pModel->m_pPhysics->m_aMtrl.size(); ++k)
			{
				pModel->m_pPhysics->m_aMtrl[k] = aIDs[k];
			}
		}
		//}

		//считывание идентификаторов пп моделей для текущей модели
		//{
		uint32_t uiCountTransparency = 0;
		fread(&uiCountTransparency, sizeof(uint32_t), 1, pFile);
		if (uiCountTransparency > 0)
		{
			pModel->m_aTransparency.resize(uiCountTransparency);
			fread(&(pModel->m_aTransparency[0]), sizeof(ID), uiCountTransparency, pFile);
		}
		//}

		//считывание сегментов модели, если есть
		//{
		int8_t i8ExistsSegments = 0;
		fread(&i8ExistsSegments, sizeof(int8_t), 1, pFile);

		if (i8ExistsSegments)
		{
			Array<CSegment**> aQueue;
			int iCountSplits = 0;
			aQueue.push_back(&(pModel->m_pArrSplits));

			while (aQueue.size() > iCountSplits)
			{
				float3 vMin, vMax;
				CSegment **ppSplit = aQueue[iCountSplits];
				(*ppSplit) = new CSegment();

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

				fread(&(*ppSplit)->m_idDepth, sizeof(ID), 1, pFile);
				fread(&(*ppSplit)->m_uiCountAllPoly, sizeof(uint32_t), 1, pFile);
				fread(&(*ppSplit)->m_isFinite, sizeof(int8_t), 1, pFile);

				if (!(*ppSplit)->m_isFinite)
				{
					int8_t iExists = false;

					for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
					{
						fread(&iExists, sizeof(int8_t), 1, pFile);
						if (iExists)
							aQueue.push_back(&((*ppSplit)->m_aSplits[i]));

						iExists = false;
					}
				}
				else
				{
					fread(&(*ppSplit)->m_uiCountSubSet, sizeof(uint32_t), 1, pFile);

					(*ppSplit)->m_pNumberGroup = new uint32_t[(*ppSplit)->m_uiCountSubSet];
					(*ppSplit)->m_pCountPoly = new uint32_t[(*ppSplit)->m_uiCountSubSet];

					fread((*ppSplit)->m_pNumberGroup, sizeof(uint32_t)*(*ppSplit)->m_uiCountSubSet, 1, pFile);
					fread((*ppSplit)->m_pCountPoly, sizeof(uint32_t)*(*ppSplit)->m_uiCountSubSet, 1, pFile);

					(*ppSplit)->m_ppArrPoly = new uint32_t*[(*ppSplit)->m_uiCountSubSet];

					for (int k = 0; k < (*ppSplit)->m_uiCountSubSet; ++k)
					{
						(*ppSplit)->m_ppArrPoly[k] = new uint32_t[(*ppSplit)->m_pCountPoly[k] * 3];
						fread((*ppSplit)->m_ppArrPoly[k], sizeof(uint32_t)*(*ppSplit)->m_pCountPoly[k] * 3, 1, pFile);
					}
				}

				++iCountSplits;
			}

			if (pModel->m_pArrSplits)
			{
				setSplitID(pModel->m_pArrSplits, &(pModel->m_idCountSplits), &(pModel->m_idCountSplitsRender));

				createExternalData4SegmentModel(pModel);
			}
		}
		//}

		modelGenGroupInfo(pModel);
		m_aModels.push_back(pModel);
		addModelInVisCalcObj(m_aModels.size() - 1);
	}


	uint32_t uiCountTransparency = 0;
	fread(&uiCountTransparency, sizeof(uint32_t), 1, pFile);

	for (int i = 0; i < uiCountTransparency; ++i)
	{
		uint32_t uiSizeBlock = 0;
		fread(&uiSizeBlock, sizeof(uint32_t), 1, pFile);

		if (uiSizeBlock >= uiSizeFile)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "file static geometry [%s] damaged, block size (transparency) %d\n", szPath);
			fclose(pFile);
			return;
		}

		CTransparencyModel *pTransparency = new CTransparencyModel();
		fread(&(pTransparency->m_idModel), sizeof(ID), 1, pFile);
		int32_t iStrLen = 0;
		char szStr[1024];
		fread(&iStrLen, sizeof(int32_t), 1, pFile);
		fread(szStr, sizeof(char), iStrLen, pFile);
		szStr[iStrLen] = 0;
		pTransparency->m_sTex = szStr;
		pTransparency->m_sTex += ".dds";
		pTransparency->m_idTex = SGCore_MtlLoad((pTransparency->m_sTex + ".dds").c_str(), MTL_TYPE_GEOM);

		fread(&(pTransparency->m_iCountVertex), sizeof(int32_t), 1, pFile);
		fread(&(pTransparency->m_iCountIndex), sizeof(int32_t), 1, pFile);

		pTransparency->m_pVertices = new vertex_static_ex[pTransparency->m_iCountVertex];
		pTransparency->m_pIndices = new UINT[pTransparency->m_iCountIndex];
		//g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static_ex)* pTransparency->m_iCountVertex, NULL, NULL, D3DPOOL_MANAGED, &(pTransparency->m_pVertexBuffer), 0);
		//g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* pTransparency->m_iCountIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &(pTransparency->m_pIndexBuffer), 0);

		UINT *pIndexTransparency = pTransparency->m_pIndices;
		//pTransparency->m_pIndexBuffer->Lock(0, 0, (void **)&pIndexTransparency, 0);
		vertex_static_ex *pVertexTransparency = pTransparency->m_pVertices;
		//pTransparency->m_pVertexBuffer->Lock(0, 0, (void **)&pVertexTransparency, 0);

		fread(pVertexTransparency, sizeof(vertex_static_ex), pTransparency->m_iCountVertex, pFile);
		fread(pIndexTransparency, sizeof(uint32_t), pTransparency->m_iCountIndex, pFile);

		pTransparency->syncBuffers();

		//pTransparency->m_pVertexBuffer->Unlock();
		//pTransparency->m_pIndexBuffer->Unlock();

		CModel *pModel = m_aModels[pTransparency->m_idModel];
		pTransparency->m_pBoundVolume->setPosition(pModel->m_pBoundVolume->getPosition());
		pTransparency->m_pBoundVolume->setRotation(pModel->m_pBoundVolume->getRotation());
		pTransparency->m_pBoundVolume->setScale(pModel->m_pBoundVolume->getScale());
		pTransparency->m_pBoundVolume->calcBound(pTransparency->m_pVertices, pTransparency->m_iCountVertex, sizeof(vertex_static_ex));

		modelGenGroupInfo4Transparency(pTransparency);

		m_aTransparency.push_back(pTransparency);
	}

	fclose(pFile);
}

//##########################################################################

ID CModels::createTransparencyModel(ID idTex, const char *szTex, const vertex_static_ex *pArrVertex, int iCountVertex, const UINT *pArrIndex, int iCountIndex)
{
	CTransparencyModel *pTransparency = new CTransparencyModel();
	pTransparency->m_iCountVertex = iCountVertex;
	pTransparency->m_iCountIndex = iCountIndex;
	pTransparency->m_idTex = idTex;
	pTransparency->m_sTex = szTex;

	pTransparency->m_pVertices = new vertex_static_ex[iCountVertex];
	pTransparency->m_pIndices = new UINT[iCountIndex];

	//g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static_ex)* iCountVertex, NULL, NULL, D3DPOOL_MANAGED, &(pTransparency->m_pVertexBuffer), 0);
	//g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* iCountIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &(pTransparency->m_pIndexBuffer), 0);

	UINT *pIndexNew = pTransparency->m_pIndices;
	//pTransparency->m_pIndexBuffer->Lock(0, 0, (void **)&pIndexNew, 0);
	vertex_static_ex *pVertexNew = pTransparency->m_pVertices;
	//pTransparency->m_pVertexBuffer->Lock(0, 0, (void **)&pVertexNew, 0);

	memcpy(pVertexNew, pArrVertex, sizeof(vertex_static_ex)* iCountVertex);
	memcpy(pIndexNew, pArrIndex, sizeof(UINT)* iCountIndex);

	//pTransparency->m_pVertexBuffer->Unlock();
	//pTransparency->m_pIndexBuffer->Unlock();

	pTransparency->syncBuffers();

	modelGenGroupInfo4Transparency(pTransparency);

	m_aTransparency.push_back(pTransparency);

	return m_aTransparency.size() - 1;
}

ID CModels::addModel(const char *szPath, const char *szName, const char *szLod, const char *szPhys, bool needSegmentation)
{
	//если путь до модели и путь до лода идентичны, то обнуляем путь до лода
	if (szLod && stricmp(szPath, szLod) == 0)
		szLod = 0;

	//если путь до модели и путь до физической модели идентичны, то обнуляем путь до физической модели
	if (szPhys && stricmp(szPath, szPhys) == 0)
		szPhys = 0;

	CModel *pModel = new CModel();
	pModel->m_sName = szName;
	pModel->m_sPath = szPath;

	String sFullPath = String(Core_RStringGet(G_RI_STRING_PATH_GS_MESHES)) + pModel->m_sPath;

	SGCore_StaticModelLoad(sFullPath.c_str(), &(pModel->m_pModel));

	/*UINT *pIndex;
	pModel->m_pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0);
	UINT *pNewIndeces = new UINT[pModel->m_pModel->m_uiAllIndexCount];
	int iCountIndex = 0;
	for (int g = 0; g < pModel->m_pModel->m_uiSubsetCount; ++g)
	{
		for (int i = 0; i < pModel->m_pModel->m_pIndexCount[g]; ++i)
		{
			pNewIndeces[iCountIndex] = pIndex[pModel->m_pModel->m_pStartIndex[g] + i] + pModel->m_pModel->m_pStartVertex[g];
			++iCountIndex;
		}
	}
	memcpy(pIndex, pNewIndeces, pModel->m_pModel->m_uiAllIndexCount * sizeof(UINT));
	pModel->m_pModel->m_pIndexBuffer->Unlock();*/

	//загружает материалы
	for (int i = 0; i < pModel->m_pModel->m_uiSubsetCount; ++i)
	{
		sFullPath = String(pModel->m_pModel->m_ppTextures[i]) + ".dds";
		pModel->m_aIDsTextures[i] = SGCore_MtlLoad(sFullPath.c_str(), MTL_TYPE_GEOM);
	}


	//создаем пп модели, если в текущей загружаемой моделе есть такие подгруппы
	//{

	int iCountVertexNew = 0;
	int iCountIndexNew = 0;
	int iCountSubsetNew = 0;

	UINT *pIndex = pModel->m_pModel->m_pIndices;
	//pModel->m_pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0);
	vertex_static_ex *pVertex = pModel->m_pModel->m_pVertices;
	//pModel->m_pModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVertex, 0);

	for (int g = 0; g < pModel->m_pModel->m_uiSubsetCount; ++g)
	{
		//если подгруппа не полупрозрачная то пропускаем
		if (!SGCore_MtlIsTransparency(pModel->m_aIDsTextures[g]))
		{
			++iCountSubsetNew;
			iCountIndexNew += pModel->m_pModel->m_pIndexCount[g];
			iCountVertexNew += pModel->m_pModel->m_pVertexCount[g];
			continue;
		}

		char* szTexName = pModel->m_pModel->m_ppTextures[g];

		//массивы индексов пп моделей
		Array<Array<UINT>> aIndex;

		//массивы оригинальных индексов 
		Array<Array<UINT>> aIndexInBuffer;

		//массивы пп вершин
		Array<Array<vertex_static_ex>> aVertex;

		//количество пп моделей в текущей подгруппе, или текущий номер модели в aIndex aIndexInBuffer aVertex
		int iCountSubModels = 0;

		//массив занятых индексов
		Array<bool> aIndexBusy;
		aIndexBusy.resize(pModel->m_pModel->m_pIndexCount[g]);
		memset(&(aIndexBusy[0]), 0, sizeof(bool)*aIndexBusy.size());

		//количество занятых индексов
		int iCountIndexBusy = 0;

		//при первой проверке индексов нам надо делать запись, эта переменная указыает на то, является ли текущий полигон первым в анализе
		bool isFirstVerifable = true;

		//количество занятых индексов с прошлой итерации
		int iCountIndexBusyOld = 0;

		//если количество занятых индексов дошло до размеров массива занятых индексов, значит мы исчерпали все индексы
		while (aIndexBusy.size() > iCountIndexBusy)
		{
			//если количество занятых индексов обнулено, значит считаем данные для новой пп модели
			if (iCountIndexBusyOld == 0)
			{
				iCountIndexBusyOld = iCountIndexBusy;
				isFirstVerifable = true;
			}

			//проходимся по индексам, по трем сразу (по полигонам)
			for (int i = 0; i < pModel->m_pModel->m_pIndexCount[g]; i += 3)
			{
				//если индекс занят, то пропускаем
				if (aIndexBusy[i])
					continue;

				//считаем количество уникальных индексов
				//{
				int iCountNonUnic = 0;

				for (int j = 0; j < 3; ++j)
				{
					for (int k = 0, kl = aIndexInBuffer[iCountSubModels].size(); k < kl; ++k)
					{
						if (aIndexInBuffer[iCountSubModels][k] == (pIndex[pModel->m_pModel->m_pStartIndex[g] + i + j]))
						{
							++iCountNonUnic;
							break;
						}
					}
				}
				//}

				//если все индексы уникальные и сейчас не первый анализ, то пропускаем итерацию
				if (iCountNonUnic == 0 && !isFirstVerifable)
					continue;

				//анализируем 3 индекса
				for (int j = 0; j < 3; ++j)
				{
					int iKey = -1;

					//ищем, возможно текущий индекс уже записывался в данные новой модели
					for (int k = 0, kl = aIndexInBuffer[iCountSubModels].size(); k < kl; ++k)
					{
						if (aIndexInBuffer[iCountSubModels][k] == (pIndex[pModel->m_pModel->m_pStartIndex[g] + i + j]))
						{
							iKey = k;
							break;
						}
					}

					//если индекс уникальный
					if (iKey == -1)
					{
						aVertex[iCountSubModels].push_back(pVertex[pIndex[pModel->m_pModel->m_pStartIndex[g] + i + j]]);
					}

					UINT uiSubMinus = 0;

					if (iCountSubModels > 0)
					{
						for (int k = 0; k < aVertex.size()-1; ++k)
							uiSubMinus += aVertex[k].size();
					}

					aIndex[iCountSubModels].push_back(pIndex[pModel->m_pModel->m_pStartIndex[g] + i + j] - pModel->m_pModel->m_pStartVertex[g] - uiSubMinus);
					aIndexInBuffer[iCountSubModels].push_back(pIndex[pModel->m_pModel->m_pStartIndex[g] + i + j]);
				}

				//сообщаем что проанализированные индексы уже заняты
				aIndexBusy[i] = aIndexBusy[i+1] = aIndexBusy[i+2] = true;
				iCountIndexBusy += 3;

				//сообщаем что начало анализа прошло
				isFirstVerifable = false;
			}

			//если предыдущее и текущее значения количества занятых полигонов равны, значит непрерывная пп модель закончилась
			if (iCountIndexBusyOld == iCountIndexBusy)
			{
				iCountIndexBusyOld = 0;
				++iCountSubModels;
			}
			//иначе за эту итерацию добавилось еще данных, равняем количество занятых полигонов
			else
				iCountIndexBusyOld = iCountIndexBusy;
		}

		for (int i = 0; i < aVertex.size(); ++i)
			pModel->m_aTransparency.push_back(createTransparencyModel(pModel->m_aIDsTextures[g], pModel->m_pModel->m_ppTextures[g], & (aVertex[i][0]), aVertex[i].size(), &(aIndex[i][0]), aIndex[i].size()));

		int qwerty = 0;
	}
	//}

	//если новое количество подгрупп (количество не пп подгрупп) не равно текущему количеству подгрупп, тогда вычленяем пп модели
	if (iCountSubsetNew != pModel->m_pModel->m_uiSubsetCount)
	{
		//новые данные для объекта модели
		char **ppTextures = 0;
		UINT *pStartIndex = 0;
		UINT *pIndexCount = 0;
		UINT *pStartVertex = 0;
		UINT *pVertexCount = 0;
		Array<ID> aIDsTextures;
		//IDirect3DVertexBuffer9 *pVertexBuffer = 0;
		//IDirect3DIndexBuffer9 *pIndexBuffer = 0;
		UINT *pIndexNew = 0;
		vertex_static_ex *pVertexNew = 0;

		//если есть подгруппы, то есть есть не пп данные, то вырежем из модели пп подгруппы
		if (iCountSubsetNew > 0)
		{
			ppTextures = new char*[iCountSubsetNew];
			pStartIndex = new UINT[iCountSubsetNew];
			pIndexCount = new UINT[iCountSubsetNew];
			pStartVertex = new UINT[iCountSubsetNew];
			pVertexCount = new UINT[iCountSubsetNew];

			pVertexNew = new vertex_static_ex[iCountVertexNew];
			pIndexNew = new UINT[iCountIndexNew];
			//g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static_ex)* iCountVertexNew, NULL, NULL, D3DPOOL_MANAGED, &pVertexBuffer, 0);
			//g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* iCountIndexNew, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &pIndexBuffer, 0);

			//UINT *pIndexNew;
			//pIndexBuffer->Lock(0, 0, (void **)&pIndexNew, 0);
			//vertex_static_ex *pVertexNew;
			//pVertexBuffer->Lock(0, 0, (void **)&pVertexNew, 0);

			int iCountVertex = 0;
			int iCountVertexMinus = 0;
			int iCountIndex = 0;
			int iCountIndexMinus = 0;

			//текущий номер записываемой подгруппы
			int iNumGroup = 0;

			for (int g = 0; g < pModel->m_pModel->m_uiSubsetCount; ++g)
			{
				//если подгруппа полупрозрачная то пропускаем
				if (SGCore_MtlIsTransparency(pModel->m_aIDsTextures[g]))
				{
					iCountIndexMinus += pModel->m_pModel->m_pIndexCount[g];
					iCountVertexMinus += pModel->m_pModel->m_pVertexCount[g];
					continue;
				}

				//заполняем массив id материалов и массив имен текстур
				aIDsTextures.push_back(pModel->m_aIDsTextures[g]);
				ppTextures[iNumGroup] = new char[strlen(pModel->m_pModel->m_ppTextures[g]) + 1];
				sprintf(ppTextures[iNumGroup], pModel->m_pModel->m_ppTextures[g]);

				//копируем вершины в новый буфер
				memcpy(pVertexNew + iCountVertex, pVertex + pModel->m_pModel->m_pStartVertex[g], sizeof(vertex_static_ex)* pModel->m_pModel->m_pVertexCount[g]);

				//обновляем данные стартовых позиций и количества
				pStartIndex[iNumGroup] = iCountIndex;
				pIndexCount[iNumGroup] = pModel->m_pModel->m_pIndexCount[g];
				pStartVertex[iNumGroup] = iCountVertex;
				pVertexCount[iNumGroup] = pModel->m_pModel->m_pVertexCount[g];

				iCountVertex += pModel->m_pModel->m_pVertexCount[g];

				for (int i = 0; i < pModel->m_pModel->m_pIndexCount[g]; ++i)
				{
					pIndexNew[iCountIndex] = pIndex[pModel->m_pModel->m_pStartIndex[g] + i] - iCountVertexMinus;
					++iCountIndex;
				}

				++iNumGroup;
			}

			//pModel->m_pModel->m_pIndexBuffer->Unlock();
			//pModel->m_pModel->m_pVertexBuffer->Unlock();
		}

		//удаление старых данных модели
		//{
		mem_delete_a(pModel->m_pModel->m_pIndices);
		mem_delete_a(pModel->m_pModel->m_pVertices);
		mem_release(pModel->m_pModel->m_pIndexBuffer);
		mem_release(pModel->m_pModel->m_pVertexBuffer);

		for (int i = 0; i < pModel->m_pModel->m_uiSubsetCount; ++i)
		{
			mem_delete_a(pModel->m_pModel->m_ppTextures[i]);
		}

		mem_delete_a(pModel->m_pModel->m_ppTextures);
		mem_delete_a(pModel->m_pModel->m_pStartIndex);
		mem_delete_a(pModel->m_pModel->m_pIndexCount);
		mem_delete_a(pModel->m_pModel->m_pStartVertex);
		mem_delete_a(pModel->m_pModel->m_pVertexCount);
		//}

		//установка новых данных модели
		//{
		pModel->m_aIDsTextures = aIDsTextures;
		pModel->m_pModel->m_pIndices = pIndexNew;
		pModel->m_pModel->m_pVertices = pVertexNew;
		pModel->m_pModel->m_ppTextures = ppTextures;
		pModel->m_pModel->m_pStartIndex = pStartIndex;
		pModel->m_pModel->m_pIndexCount = pIndexCount;
		pModel->m_pModel->m_pStartVertex = pStartVertex;
		pModel->m_pModel->m_pVertexCount = pVertexCount;
		pModel->m_pModel->m_uiSubsetCount = iCountSubsetNew;
		pModel->m_pModel->m_uiAllVertexCount = iCountVertexNew;
		pModel->m_pModel->m_uiAllIndexCount = iCountIndexNew;

		pModel->m_pModel->syncBuffers(true);
		//}
	}

	if (iCountSubsetNew > 0)
		pModel->m_pBoundVolume->calcBound(pModel->m_pModel->m_pVertices, pModel->m_pModel->m_uiAllVertexCount, sizeof(vertex_static_ex));

	//если для модели нужно деление, то делим
	if (pModel->m_pModel->m_uiSubsetCount > 0 && needSegmentation)
	{
		segmentation(pModel);
		setSplitID(pModel->m_pArrSplits, &(pModel->m_idCountSplits), &(pModel->m_idCountSplitsRender));

		createExternalData4SegmentModel(pModel);
	}

	modelGenGroupInfo(pModel);

	m_aModels.push_back(pModel);

	if (szLod)
		modelSetLod(m_aModels.size() - 1, szLod);

	if (szPhys)
		modelSetPhysics2(pModel, szPhys);

	addModelInVisCalcObj(m_aModels.size() - 1);

	reSetIdModel4Transparency(m_aModels.size() - 1);

	return m_aModels.size() - 1;
}

void CModels::deleteModel(ID idModel)
{
	// удаление всех пп моделей удаляемой модели
	for (int i = 0, il = m_aModels[idModel]->m_aTransparency.size(); i < il; ++i)
	{
		mem_delete(m_aTransparency[m_aModels[idModel]->m_aTransparency[i]]);
	}

	// перекомпоновка массива пп моделей
	reCalcIdTransparency();

	// декремент id моделей у всех пп моделей, родитель которых, выше чем удаляемая модель
	for (int i = 0, il = m_aTransparency.size(); i < il; ++i)
	{
		if (m_aTransparency[i]->m_idModel > idModel)
			--m_aTransparency[i]->m_idModel;
	}

	// удаление модели из памяти и массива
	mem_delete(m_aModels[idModel]);
	m_aModels.erase(idModel);

	// очистка объекта просчетов видимости от удаленной модели
	for (int i = 0; i < m_aVisInfo.size(); ++i)
	{
		mem_delete(m_aVisInfo[i]->m_aVisible4Model[idModel]);
		m_aVisInfo[i]->m_aVisible4Model.erase(idModel);
	}
}

void CModels::clear()
{
	while(m_aModels.size() > 0)
	{
		//удаляем каждую модель, так как модель удаляется из массива erase то можно за последний id брать (размер массива - 1)
		deleteModel((m_aModels.size() - 1));
	}
}

int CModels::getCountModel()
{
	return m_aModels.size();
}

void CModels::getMinMax(float3 *pMin, float3 *pMax)
{
	if (m_aModels.size() <= 0)
		return;

	float3 vCountMin, vCountMax, vMin, vMax;
	modelGetMinMax(0, &vCountMin, &vCountMax);
	
	for (int i = 1, il = m_aModels.size(); i < il; ++i)
	{
		modelGetMinMax(i, &vMin, &vMax);

		if (vMin.x < vCountMin.x)
			vCountMin.x = vMin.x;

		if (vMin.y < vCountMin.y)
			vCountMin.y = vMin.y;

		if (vMin.z < vCountMin.z)
			vCountMin.z = vMin.z;


		if (vMax.x > vCountMax.x)
			vCountMax.x = vMax.x;

		if (vMax.y > vCountMax.y)
			vCountMax.y = vMax.y;

		if (vMax.z > vCountMax.z)
			vCountMax.z = vMax.z;
	}

	*pMin = vCountMin;
	*pMax = vCountMax;
}

ID CModels::copy(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, -1);

	CModel *pModel = m_aModels[idModel];
	CModel *pCopy = new CModel();

	int iPos = StrFindI(pModel->m_sName.c_str(), GEOM_COPY_POSTFIX);
	if (iPos >= 0)
	{
		if (pModel->m_sName.length() == iPos + strlen(GEOM_COPY_POSTFIX))
			pCopy->m_sName = pModel->m_sName + "1";
		else
		{
			String sName = StrSubstr(pModel->m_sName.c_str(), 0, iPos + strlen(GEOM_COPY_POSTFIX));
			String sNum = StrSubstr(pModel->m_sName.c_str(), iPos + strlen(GEOM_COPY_POSTFIX));
			pCopy->m_sName = sName + (sNum.toInt() + 1);
		}
	}
	else
		pCopy->m_sName = pModel->m_sName + GEOM_COPY_POSTFIX;

	pCopy->m_sPath = pModel->m_sPath;

	pCopy->m_pModel = pModel->m_pModel->getCopy();

	if (pModel->m_pLod)
	{
		pCopy->m_pLod = new CLod();
		pCopy->m_pLod->m_sPath = pModel->m_pLod->m_sPath;
		pCopy->m_pLod->m_aIDsTextures = pModel->m_pLod->m_aIDsTextures;
		pCopy->m_pLod->m_pModel = pModel->m_pLod->m_pModel->getCopy();
	}

	if (pCopy->m_pPhysics)
	{
		pCopy->m_pPhysics = new CPhysMesh();
		pCopy->m_pPhysics->m_sPath = pModel->m_pPhysics->m_sPath;
		pCopy->m_pPhysics->m_aVertex = pModel->m_pPhysics->m_aVertex;
		pCopy->m_pPhysics->m_aIndex = pModel->m_pPhysics->m_aIndex;
		pCopy->m_pPhysics->m_aStringMtrl = pModel->m_pPhysics->m_aStringMtrl;
		pCopy->m_pPhysics->m_aNumMtrl = pModel->m_pPhysics->m_aNumMtrl;
		pCopy->m_pPhysics->m_aMtrl = pModel->m_pPhysics->m_aMtrl;
	}

	pCopy->m_aIDsTextures = pModel->m_aIDsTextures;

	if (pModel->m_aGroupInfo.size() > 0)
	{
		pCopy->m_aGroupInfo.resize(pModel->m_aGroupInfo.size());
		for (int i = 0; i < pModel->m_aGroupInfo.size(); ++i)
		{
			pCopy->m_aGroupInfo[i] = new CGroupInfo();
			memcpy(pCopy->m_aGroupInfo[i], pModel->m_aGroupInfo[i], sizeof(CGroupInfo));
		}
	}

	float3 vMin, vMax;
	pModel->m_pBoundVolume->getMinMax(&vMin, &vMax);
	pCopy->m_pBoundVolume->setPosition(pModel->m_pBoundVolume->getPosition());
	pCopy->m_pBoundVolume->setRotation(pModel->m_pBoundVolume->getRotation());
	pCopy->m_pBoundVolume->setScale(pModel->m_pBoundVolume->getScale());
	pCopy->m_pBoundVolume->setMinMax(&vMin, &vMax);

	m_aModels.push_back(pCopy);
	ID idModelCopy = m_aModels.size() - 1;


	for (int i = 0, il = pModel->m_aTransparency.size(); i < il; ++i)
	{
		CTransparencyModel *pTransparencyModel = m_aTransparency[pModel->m_aTransparency[i]];
		CTransparencyModel *pTransparencyModelCopy = new CTransparencyModel();
		pTransparencyModelCopy->m_idModel = idModelCopy;
		pTransparencyModelCopy->m_iCountIndex = pTransparencyModel->m_iCountIndex;
		pTransparencyModelCopy->m_iCountVertex = pTransparencyModel->m_iCountVertex;
		pTransparencyModelCopy->m_idTex = pTransparencyModel->m_idTex;
		pTransparencyModelCopy->m_sTex = pTransparencyModel->m_sTex;
		pTransparencyModelCopy->m_oGroupInfo = pTransparencyModel->m_oGroupInfo;

		pTransparencyModel->m_pBoundVolume->getMinMax(&vMin, &vMax);
		pTransparencyModelCopy->m_pBoundVolume->setPosition(pTransparencyModel->m_pBoundVolume->getPosition());
		pTransparencyModelCopy->m_pBoundVolume->setRotation(pTransparencyModel->m_pBoundVolume->getRotation());
		pTransparencyModelCopy->m_pBoundVolume->setScale(pTransparencyModel->m_pBoundVolume->getScale());
		pTransparencyModelCopy->m_pBoundVolume->setMinMax(&vMin, &vMax);


		pTransparencyModelCopy->m_pVertices = new vertex_static_ex[pTransparencyModelCopy->m_iCountVertex];
		//g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static_ex)* pTransparencyModelCopy->m_iCountVertex, NULL, NULL, D3DPOOL_MANAGED, &(pTransparencyModelCopy->m_pVertexBuffer), 0);
		vertex_static_ex *pVertex = pTransparencyModel->m_pVertices, *pVertexCopy = pTransparencyModelCopy->m_pVertices;
		//pTransparencyModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);
		//pTransparencyModelCopy->m_pVertexBuffer->Lock(0, 0, (void**)&pVertexCopy, 0);

		memcpy(pVertexCopy, pVertex, sizeof(vertex_static_ex)* pTransparencyModelCopy->m_iCountVertex);

		//pTransparencyModel->m_pVertexBuffer->Unlock();
		//pTransparencyModelCopy->m_pVertexBuffer->Unlock();

		pTransparencyModelCopy->m_pIndices = new UINT[pTransparencyModelCopy->m_iCountIndex];
		//g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* pTransparencyModelCopy->m_iCountIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &(pTransparencyModelCopy->m_pIndexBuffer), 0);
		UINT *pIndex = pTransparencyModel->m_pIndices, *pIndexCopy = pTransparencyModelCopy->m_pIndices;
		//pTransparencyModel->m_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);
		//pTransparencyModelCopy->m_pIndexBuffer->Lock(0, 0, (void**)&pIndexCopy, 0);

		memcpy(pIndexCopy, pIndex, sizeof(UINT)* pTransparencyModelCopy->m_iCountIndex);

		//pTransparencyModel->m_pIndexBuffer->Unlock();
		//pTransparencyModelCopy->m_pIndexBuffer->Unlock();

		pTransparencyModelCopy->syncBuffers();

		m_aTransparency.push_back(pTransparencyModelCopy);
		pCopy->m_aTransparency.push_back(m_aTransparency.size() - 1);
	}

	if (pModel->m_pArrSplits)
	{
		Array<CSegment*> aQueue;
		int iCount = 0;

		CSegment *pSegmentOrigin = pModel->m_pArrSplits;
		CSegment *pSegmentCopy = new CSegment();
		pSegmentCopy->m_pParent = 0;
		pSegmentCopy->m_uiCountSubSet = pSegmentOrigin->m_uiCountSubSet;
		pSegmentCopy->m_uiCountAllPoly = pSegmentOrigin->m_uiCountAllPoly;
		pSegmentCopy->m_ID = pSegmentOrigin->m_ID;
		pSegmentCopy->m_SID = pSegmentOrigin->m_SID;
		pSegmentCopy->m_idDepth = pSegmentOrigin->m_idDepth;
		pSegmentCopy->m_isFinite = pSegmentOrigin->m_isFinite;

		pSegmentCopy->m_pBoundVolumeP = SGCore_CrBound();
		pSegmentOrigin->m_pBoundVolumeP->getMinMax(&vMin, &vMax);
		pSegmentCopy->m_pBoundVolumeP->setPosition(pSegmentOrigin->m_pBoundVolumeP->getPosition());
		pSegmentCopy->m_pBoundVolumeP->setRotation(pSegmentOrigin->m_pBoundVolumeP->getRotation());
		pSegmentCopy->m_pBoundVolumeP->setScale(pSegmentOrigin->m_pBoundVolumeP->getScale());
		pSegmentCopy->m_pBoundVolumeP->setMinMax(&vMin, &vMax);

		pSegmentCopy->m_pBoundVolumeSys = SGCore_CrBound();
		pSegmentOrigin->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);
		pSegmentCopy->m_pBoundVolumeSys->setPosition(pSegmentOrigin->m_pBoundVolumeSys->getPosition());
		pSegmentCopy->m_pBoundVolumeSys->setRotation(pSegmentOrigin->m_pBoundVolumeSys->getRotation());
		pSegmentCopy->m_pBoundVolumeSys->setScale(pSegmentOrigin->m_pBoundVolumeSys->getScale());
		pSegmentCopy->m_pBoundVolumeSys->setMinMax(&vMin, &vMax);

		pSegmentCopy->m_pNumberGroup = 0;
		pSegmentCopy->m_pCountPoly = 0;
		pSegmentCopy->m_ppArrPoly = 0;

		if (pSegmentCopy->m_uiCountAllPoly > 0 && pSegmentCopy->m_isFinite)
		{
			pSegmentCopy->m_pNumberGroup = new uint32_t[pSegmentCopy->m_uiCountSubSet];
			memcpy(pSegmentCopy->m_pNumberGroup, pSegmentOrigin->m_pNumberGroup, sizeof(uint32_t)* pSegmentCopy->m_uiCountSubSet);

			pSegmentCopy->m_pCountPoly = new uint32_t[pSegmentCopy->m_uiCountSubSet];
			memcpy(pSegmentCopy->m_pCountPoly, pSegmentOrigin->m_pCountPoly, sizeof(uint32_t)* pSegmentCopy->m_uiCountSubSet);

			pSegmentCopy->m_ppArrPoly = new uint32_t*[pSegmentCopy->m_uiCountSubSet];

			for (int i = 0; i < pSegmentCopy->m_uiCountSubSet; ++i)
			{
				pSegmentCopy->m_ppArrPoly[i] = new uint32_t[pSegmentCopy->m_pCountPoly[i]*3];
				memcpy(pSegmentCopy->m_ppArrPoly[i], pSegmentOrigin->m_ppArrPoly[i], sizeof(uint32_t)* pSegmentCopy->m_pCountPoly[i]*3);
			}
		}

		for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
		{
			pSegmentCopy->m_aSplits[i] = pSegmentOrigin->m_aSplits[i];
		}

		pCopy->m_pArrSplits = pSegmentCopy;
		aQueue.push_back(pCopy->m_pArrSplits);


		while (aQueue.size() > iCount)
		{
			CSegment *pSegment = aQueue[iCount];

			for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
			{
				if (pSegment->m_aSplits[i])
				{
					CSegment *pSegmentOrigin = pSegment->m_aSplits[i];
					CSegment *pSegmentCopy = new CSegment();
					pSegmentCopy->m_pParent = pSegment;
					pSegmentCopy->m_uiCountSubSet = pSegmentOrigin->m_uiCountSubSet;
					pSegmentCopy->m_uiCountAllPoly = pSegmentOrigin->m_uiCountAllPoly;
					pSegmentCopy->m_ID = pSegmentOrigin->m_ID;
					pSegmentCopy->m_SID = pSegmentOrigin->m_SID;
					pSegmentCopy->m_idDepth = pSegmentOrigin->m_idDepth;
					pSegmentCopy->m_isFinite = pSegmentOrigin->m_isFinite;

					pSegmentCopy->m_pBoundVolumeP = SGCore_CrBound();
					pSegmentOrigin->m_pBoundVolumeP->getMinMax(&vMin, &vMax);
					pSegmentCopy->m_pBoundVolumeP->setPosition(pSegmentOrigin->m_pBoundVolumeP->getPosition());
					pSegmentCopy->m_pBoundVolumeP->setRotation(pSegmentOrigin->m_pBoundVolumeP->getRotation());
					pSegmentCopy->m_pBoundVolumeP->setScale(pSegmentOrigin->m_pBoundVolumeP->getScale());
					pSegmentCopy->m_pBoundVolumeP->setMinMax(&vMin, &vMax);

					pSegmentCopy->m_pBoundVolumeSys = SGCore_CrBound();
					pSegmentOrigin->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);
					pSegmentCopy->m_pBoundVolumeSys->setPosition(pSegmentOrigin->m_pBoundVolumeSys->getPosition());
					pSegmentCopy->m_pBoundVolumeSys->setRotation(pSegmentOrigin->m_pBoundVolumeSys->getRotation());
					pSegmentCopy->m_pBoundVolumeSys->setScale(pSegmentOrigin->m_pBoundVolumeSys->getScale());
					pSegmentCopy->m_pBoundVolumeSys->setMinMax(&vMin, &vMax);

					pSegmentCopy->m_pNumberGroup = 0;
					pSegmentCopy->m_pCountPoly = 0;
					pSegmentCopy->m_ppArrPoly = 0;

					if (pSegmentCopy->m_uiCountAllPoly > 0 && pSegmentCopy->m_isFinite)
					{
						pSegmentCopy->m_pNumberGroup = new uint32_t[pSegmentCopy->m_uiCountSubSet];
						memcpy(pSegmentCopy->m_pNumberGroup, pSegmentOrigin->m_pNumberGroup, sizeof(uint32_t)* pSegmentCopy->m_uiCountSubSet);

						pSegmentCopy->m_pCountPoly = new uint32_t[pSegmentCopy->m_uiCountSubSet];
						memcpy(pSegmentCopy->m_pCountPoly, pSegmentOrigin->m_pCountPoly, sizeof(uint32_t)* pSegmentCopy->m_uiCountSubSet);

						pSegmentCopy->m_ppArrPoly = new uint32_t*[pSegmentCopy->m_uiCountSubSet];

						for (int k = 0; k < pSegmentCopy->m_uiCountSubSet; ++k)
						{
							pSegmentCopy->m_ppArrPoly[k] = new uint32_t[pSegmentCopy->m_pCountPoly[k]*3];
							memcpy(pSegmentCopy->m_ppArrPoly[k], pSegmentOrigin->m_ppArrPoly[k], sizeof(uint32_t)* pSegmentCopy->m_pCountPoly[k]*3);
						}
					}

					for (int k = 0; k < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++k)
					{
						pSegmentCopy->m_aSplits[k] = pSegmentOrigin->m_aSplits[k];
					}

					pSegment->m_aSplits[i] = pSegmentCopy;
					aQueue.push_back(pSegmentCopy);
				}
			}

			++iCount;
		}

		pCopy->m_idCountSplits = pModel->m_idCountSplits;
		pCopy->m_idCountSplitsRender = pModel->m_idCountSplitsRender;

		createExternalData4SegmentModel(pCopy);
	}


	addModelInVisCalcObj(idModelCopy);

	reSetIdModel4Transparency(idModelCopy);

	return idModelCopy;
}

//##########################################################################

ID CModels::addVisCaclObj()
{
	ID idNewVisCaclObj = -1;

	for (int i = 0; i < m_aVisInfo.size(); ++i)
	{
		if (!(m_aVisInfo[i]))
			idNewVisCaclObj = i;
	}

	CVisInfo *pVisInfo = new CVisInfo();
	for (int i = 0; i < m_aModels.size(); ++i)
	{
		pVisInfo->m_aVisible4Model[i] = new CVisInfoModel();
		if (m_aModels[i]->m_pArrSplits)
		{
			pVisInfo->m_aVisible4Model[i]->m_ppSegments = new CSegment*[m_aModels[i]->m_idCountSplitsRender];
			pVisInfo->m_aVisible4Model[i]->m_iCountCom = 0;
			pVisInfo->m_aVisible4Model[i]->m_iCount = m_aModels[i]->m_idCountSplitsRender;
		}
		else
		{
			pVisInfo->m_aVisible4Model[i]->m_ppSegments = 0;
			pVisInfo->m_aVisible4Model[i]->m_iCountCom = 0;
			pVisInfo->m_aVisible4Model[i]->m_iCount = 0;
		}

		pVisInfo->m_aVisible4Model[i]->m_isVisible = false;
	}

	if (idNewVisCaclObj == -1)
	{
		m_aVisInfo.push_back(pVisInfo);
		idNewVisCaclObj = m_aVisInfo.size() - 1;
	}
	else
		m_aVisInfo[idNewVisCaclObj] = pVisInfo;

	return idNewVisCaclObj;
}

bool CModels::existsVisCaclObj(ID idVisCaclObj)
{
	return (idVisCaclObj >= 0 && m_aVisInfo.size() > idVisCaclObj && m_aVisInfo[idVisCaclObj]);
}

void CModels::deleteVisCaclObj(ID idVisCaclObj)
{
	if (existsVisCaclObj(idVisCaclObj))
	{
		mem_delete(m_aVisInfo[idVisCaclObj]);
	}
}

void CModels::addModelInVisCalcObj(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	for (int i = 0; i < m_aVisInfo.size(); ++i)
	{
		CVisInfo *pVisCalcObj = m_aVisInfo[i];

		if (pVisCalcObj)
		{
			if (pVisCalcObj->m_aVisible4Model.size() > idModel && pVisCalcObj->m_aVisible4Model[idModel])
			{
				mem_delete(pVisCalcObj->m_aVisible4Model[idModel]);
			}

			pVisCalcObj->m_aVisible4Model[idModel] = new CVisInfoModel();
			pVisCalcObj->m_aVisible4Model[idModel]->m_ppSegments = new CSegment*[m_aModels[idModel]->m_idCountSplitsRender];
			pVisCalcObj->m_aVisible4Model[idModel]->m_iCountCom = 0;
			pVisCalcObj->m_aVisible4Model[idModel]->m_iCount = m_aModels[idModel]->m_idCountSplitsRender;
			pVisCalcObj->m_aVisible4Model[idModel]->m_isVisible = false;
		}
	}
}

void CModels::deleteModelOutVisCalcObj(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	for (int i = 0; i < m_aVisInfo.size(); ++i)
	{
		if (m_aVisInfo[i])
		{
			if (m_aVisInfo[i]->m_aVisible4Model[idModel])
			{
				mem_delete(m_aVisInfo[i]->m_aVisible4Model[idModel]);
			}
		}
	}
}

//##########################################################################

void CModels::modelSetPosition(ID idModel, const float3 *pPos)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	m_aModels[idModel]->m_pBoundVolume->setPosition(pPos);

	modelComBound(idModel);
}

const float3* CModels::modelGetPosition(ID idModel, float3 *pPos)
{
	STATIC_PRECOND_MODEL_ID(idModel, 0);

	return m_aModels[idModel]->m_pBoundVolume->getPosition(pPos);
}


void CModels::modelSetRotation(ID idModel, const float3 *pRot)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	m_aModels[idModel]->m_pBoundVolume->setRotation(pRot);

	modelComBound(idModel);
}

const float3* CModels::modelGetRotation(ID idModel, float3 *pRot)
{
	STATIC_PRECOND_MODEL_ID(idModel, 0);

	return m_aModels[idModel]->m_pBoundVolume->getRotation(pRot);
}


void CModels::modelSetScale(ID idModel, const float3 *pScale)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	m_aModels[idModel]->m_pBoundVolume->setScale(pScale);
	
	modelComBound(idModel);
}

const float3* CModels::modelGetScale(ID idModel, float3 *pScale)
{
	STATIC_PRECOND_MODEL_ID(idModel, 0);

	return m_aModels[idModel]->m_pBoundVolume->getScale(pScale);
}

void CModels::modelComBound(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	CModel *pModel = m_aModels[idModel];

	if (pModel->m_pModel->m_uiSubsetCount > 0)
		pModel->m_pBoundVolume->calcBound(pModel->m_pModel->m_pVertices, pModel->m_pModel->m_uiAllVertexCount, sizeof(vertex_static_ex));
	
	if (pModel->m_pArrSplits)
		modelComBound4Segments(idModel);

	if (pModel->m_aTransparency.size() > 0)
		modelComBoundArrTransparency(idModel);
}

void CModels::modelComBoundArrTransparency(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	CModel *pModel = m_aModels[idModel];

	float3 vMinModel, vMaxModel, vMin, vMax;
	pModel->m_pBoundVolume->getMinMax(&vMinModel, &vMaxModel);

	for (int i = 0; i < pModel->m_aTransparency.size(); ++i)
	{
		CModels::CTransparencyModel *pTransparency = m_aTransparency[pModel->m_aTransparency[i]];
		pTransparency->m_pBoundVolume->setPosition(pModel->m_pBoundVolume->getPosition());
		pTransparency->m_pBoundVolume->setRotation(pModel->m_pBoundVolume->getRotation());
		pTransparency->m_pBoundVolume->setScale(pModel->m_pBoundVolume->getScale());
		pTransparency->m_pBoundVolume->calcBound(pTransparency->m_pVertices, pTransparency->m_iCountVertex, sizeof(vertex_static_ex));
		pTransparency->m_pBoundVolume->getMinMax(&vMin, &vMax);

		if (vMin.x < vMinModel.x)
			vMinModel.x = vMin.x;

		if (vMin.y < vMinModel.y)
			vMinModel.y = vMin.y;

		if (vMin.z < vMinModel.z)
			vMinModel.z = vMin.z;

		if (vMax.x > vMaxModel.x)
			vMaxModel.x = vMax.x;

		if (vMax.y > vMaxModel.y)
			vMaxModel.y = vMax.y;

		if (vMax.z > vMaxModel.z)
			vMaxModel.z = vMax.z;
	}

	pModel->m_pBoundVolume->setMinMax(&vMinModel, &vMaxModel);
}

void CModels::modelComBound4Segments(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	CModel *pModel = m_aModels[idModel];

	if (!(pModel->m_pArrSplits))
		return;

	//копируем данные из дх буферов
	//{
	vertex_static_ex *pVertex;
	mem_delete(m_pCurrArrMeshVertex);
	mem_delete(m_pCurrArrMeshIndex);

	m_pCurrArrMeshVertex = new float3[pModel->m_pModel->m_uiAllVertexCount];

	pVertex = pModel->m_pModel->m_pVertices;
	//pModel->m_pModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVertex, 0);
	for (int i = 0; i<pModel->m_pModel->m_uiAllVertexCount; ++i)
		m_pCurrArrMeshVertex[i] = (float3)pVertex[i].Pos;
	//pModel->m_pModel->m_pVertexBuffer->Unlock();

	m_pCurrArrMeshIndex = new UINT[pModel->m_pModel->m_uiAllIndexCount];
	UINT *pIndex = pModel->m_pModel->m_pIndices;
	//pModel->m_pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0);
	memcpy(m_pCurrArrMeshIndex, pIndex, sizeof(UINT)* pModel->m_pModel->m_uiAllIndexCount);
	//pModel->m_pModel->m_pIndexBuffer->Unlock();
	//}

	//массив массивов сплитов по глубинам arr[0] - массив всех сплитов с глубиной 1, arr[1] - массив всех сплитов с глубиной 2
	Array<Array<CSegment*>> aSegmentDepth;

	//трансформируем баунды всех конечных потомков
	//{
	Array<CSegment*> aQueue;
	int iCount = 0;
	int iMaxDepth = 0;
	aQueue.push_back(pModel->m_pArrSplits);

	while (aQueue.size() > iCount)
	{
		CSegment *pSegment = aQueue[iCount];

		if (!(pSegment->m_isFinite))
		{
			for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
			{
				if (pSegment->m_aSplits[i])
					aQueue.push_back(pSegment->m_aSplits[i]);
			}
		}
		else
		{
			//устанавливаем баундам сегментов новые данные трансформации
			pSegment->m_pBoundVolumeP->setPosition(pModel->m_pBoundVolume->getPosition());
			pSegment->m_pBoundVolumeP->setRotation(pModel->m_pBoundVolume->getRotation());
			pSegment->m_pBoundVolumeP->setScale(pModel->m_pBoundVolume->getScale());
			//pSegment->m_pBoundVolumeP->calcWorldAndTrans();
			pSegment->m_pBoundVolumeP->calcBoundIndex(pModel->m_pModel->m_pVertices, pSegment->m_ppArrPoly, pSegment->m_pCountPoly, pSegment->m_uiCountSubSet, sizeof(vertex_static_ex));

			if (iCount >= 5)
				int qwerty = 0;
			editVolume(pModel, pSegment);

			if (iMaxDepth < pSegment->m_idDepth)
				iMaxDepth = pSegment->m_idDepth;
		}

		aSegmentDepth[pSegment->m_idDepth - 1].push_back(pSegment);

		++iCount;
	}
	//}

	//трансформируем баунды всех родителей
	//{
	float3 vMax, vMin, vCountMax, vCountMin;

	for (int i = 1, il = aSegmentDepth.size(); i < il; ++i)
	{
		//начинаем с максимальной глубины вложенности - 1, чтобы идти от потомков к родителям
		int iCurrDepth = (aSegmentDepth.size() - i) - 1;

		for (int k = 0, kl = aSegmentDepth[iCurrDepth].size(); k < kl; ++k)
		{
			//если сегмент конечный то ничего с ним не делаем
			if (aSegmentDepth[iCurrDepth][k]->m_isFinite)
				continue;

			//сначала инициализируем экстремумы
			for (int j = 0; j < 4; ++j)
			{
				if (aSegmentDepth[iCurrDepth][k]->m_aSplits[j])
				{
					aSegmentDepth[iCurrDepth][k]->m_aSplits[j]->m_pBoundVolumeP->getMinMax(&vCountMin, &vCountMax);
					break;
				}
			}

			//теперь считаем экстремумы на основании данных потомков
			for (int j = 0; j < 4; ++j)
			{
				if (aSegmentDepth[iCurrDepth][k]->m_aSplits[j])
				{
					aSegmentDepth[iCurrDepth][k]->m_aSplits[j]->m_pBoundVolumeP->getMinMax(&vMin, &vMax);

					if (vCountMin.x > vMin.x)
						vCountMin.x = vMin.x;

					if (vCountMin.y > vMin.y)
						vCountMin.y = vMin.y;

					if (vCountMin.z > vMin.z)
						vCountMin.z = vMin.z;

					if (vCountMax.x < vMax.x)
						vCountMax.x = vMax.x;

					if (vCountMax.y < vMax.y)
						vCountMax.y = vMax.y;

					if (vCountMax.z < vMax.z)
						vCountMax.z = vMax.z;
				}
			}

			//теперь устаналвиваем в оба баунда, потому что системный больше не нужен
			aSegmentDepth[iCurrDepth][k]->m_pBoundVolumeP->setMinMax(&vCountMin, &vCountMax);
			aSegmentDepth[iCurrDepth][k]->m_pBoundVolumeSys->setMinMax(&vCountMin, &vCountMax);
		}
	}
	//}

	int qwerty = 0;
}

//##########################################################################

void CModels::createExternalData4SegmentModel(CModel *pModel)
{
	if (!pModel || !(pModel->m_pArrSplits))
		return;

	pModel->m_pCountDrawPoly = new UINT[pModel->m_pModel->m_uiSubsetCount];
	memset(pModel->m_pCountDrawPoly, 0, sizeof(UINT)* pModel->m_pModel->m_uiSubsetCount);

	pModel->m_pVisibleIndeces = new UINT*[pModel->m_pModel->m_uiSubsetCount];

	for (int i = 0; i<pModel->m_pModel->m_uiSubsetCount; ++i)
	{
		pModel->m_pVisibleIndeces[i] = new UINT[pModel->m_pModel->m_pIndexCount[i]];
	}

	g_pDXDevice->CreateIndexBuffer(
		pModel->m_pModel->m_uiAllIndexCount * sizeof(UINT),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&(pModel->m_pVisibleIndexBuffer),
		0);
}

void CModels::segmentation(CModel *pModel)
{
	//копируем данные из дх буферов
	//{
	vertex_static_ex *pVertex;
	mem_delete_a(m_pCurrArrMeshVertex);
	mem_delete_a(m_pCurrArrMeshIndex);
	
	m_pCurrArrMeshVertex = new float3[pModel->m_pModel->m_uiAllVertexCount];

	//pModel->m_pModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVertex, 0);
	pVertex = pModel->m_pModel->m_pVertices;
	for (int i = 0; i<pModel->m_pModel->m_uiAllVertexCount; ++i)
	{
		m_pCurrArrMeshVertex[i] = (float3)pVertex[i].Pos;
	}
	//pModel->m_pModel->m_pVertexBuffer->Unlock();

	m_pCurrArrMeshIndex = new UINT[pModel->m_pModel->m_uiAllIndexCount];
	UINT *pIndex = pModel->m_pModel->m_pIndices;
	//pModel->m_pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0);
	memcpy(m_pCurrArrMeshIndex, pIndex, sizeof(UINT)* pModel->m_pModel->m_uiAllIndexCount);
	//pModel->m_pModel->m_pIndexBuffer->Unlock();
	//}

	//**********************************************************************

	pModel->m_pArrSplits = new CSegment();
	pModel->m_pArrSplits->m_idDepth = 1;
	pModel->m_pArrSplits->m_pParent = 0;
	pModel->m_pArrSplits->m_uiCountAllPoly = pModel->m_pModel->m_uiAllIndexCount / 3;
	LibReport(REPORT_MSG_LEVEL_NOTICE, "poly: %d, ", pModel->m_pArrSplits->m_uiCountAllPoly);

	pModel->m_pArrSplits->m_pBoundVolumeP = SGCore_CrBound();
	pModel->m_pArrSplits->m_pBoundVolumeP->calcBound(pModel->m_pModel->m_pVertices, pModel->m_pModel->m_uiAllVertexCount, sizeof(vertex_static_ex));

	float3 vMin, vMax;
	pModel->m_pArrSplits->m_pBoundVolumeP->getMinMax(&vMin, &vMax);

	// определение типа деления и количества полигонов в сплите
	//{
	int iCountSplitsSys = 0;
	int iCountPolyInSegment = SX_GEOM_MIN_COUNT_POLY;

	// габариты
	float3 vDimensions = vMax - vMin;

	// общий объем
	float fGeneralVolume = vDimensions.x * vDimensions.y * vDimensions.z;
	const float minVol = SX_GEOM_MIN_ALLVOLUME_FOR_SEGMENTATION;
	const float minLen = SX_GEOM_MIN_LENGTH_FOR_SEGMENTATION;
	const float minOctoHeight = SX_GEOM_MIN_HEIGHT_FOR_SEGMENTATION;
	if ((fGeneralVolume >= minVol || vDimensions.x >= minLen || vDimensions.y >= minOctoHeight || vDimensions.z >= minLen) && (pModel->m_pModel->m_uiAllIndexCount >= SX_GEOM_MIN_POLYGONS_FOR_SEGMENTATION * 3 || fGeneralVolume >= SX_GEOM_FORCE_ALLVOLUME_FOR_SEGMENTATION))
	{
		if (vDimensions.y >= minOctoHeight)
		{
			iCountSplitsSys = GEOM_COUNT_TYPE_SEGMENTATION_OCTO;
			LibReport(REPORT_MSG_LEVEL_NOTICE, " div: octo, ");
		}
		else
		{
			iCountSplitsSys = GEOM_COUNT_TYPE_SEGMENTATION_QUAD;
			LibReport(REPORT_MSG_LEVEL_NOTICE, " div: quad, ");
		}

		float4_t tmpmin(vDimensions.x / minLen, vDimensions.y / minOctoHeight, vDimensions.z / minLen, (float)pModel->m_pModel->m_uiAllIndexCount / (float)SX_GEOM_MIN_POLYGONS_FOR_SEGMENTATION / 3.0f);
		tmpmin.x = min(min(min(tmpmin.x, tmpmin.y), min(tmpmin.z, tmpmin.w)), (fGeneralVolume / minVol)) * 10.0f;

		iCountPolyInSegment = (float)pModel->m_pModel->m_uiAllIndexCount / 3.0f / tmpmin.x / iCountSplitsSys;

		if (iCountPolyInSegment < SX_GEOM_MIN_COUNT_POLY)
			iCountPolyInSegment = SX_GEOM_MIN_COUNT_POLY;

		if (iCountPolyInSegment > SX_GEOM_MAX_COUNT_POLY)
			iCountPolyInSegment = SX_GEOM_MAX_COUNT_POLY;

		LibReport(REPORT_MSG_LEVEL_NOTICE, "poly in split: %d, ", iCountPolyInSegment);
	}
	//}

	//**********************************************************************

	float fDimensionX = vDimensions.x;
	float fDimensionY = vDimensions.y;
	float fDimensionZ = vDimensions.z;

	// выравниваем для равномерного делени
	//{
	if (iCountSplitsSys == GEOM_COUNT_TYPE_SEGMENTATION_OCTO)
	{
		if (fDimensionX > fDimensionY && fDimensionX > fDimensionZ)
		{
			fDimensionY = fDimensionX - fDimensionY;
			fDimensionY /= 2.f;
			vMax.y += fDimensionY;
			vMin.y -= fDimensionY;

			fDimensionZ = fDimensionX - fDimensionZ;
			fDimensionZ /= 2.f;
			vMax.z += fDimensionZ;
			vMin.z -= fDimensionZ;
		}
		else if (fDimensionY > fDimensionX && fDimensionY > fDimensionZ)
		{
			fDimensionX = fDimensionY - fDimensionX;
			fDimensionX /= 2.f;
			vMax.x += fDimensionX;
			vMin.x -= fDimensionX;

			fDimensionZ = fDimensionY - fDimensionZ;
			fDimensionZ /= 2.f;
			vMax.z += fDimensionZ;
			vMin.z -= fDimensionZ;
		}
		else if (fDimensionZ > fDimensionX && fDimensionZ > fDimensionY)
		{
			fDimensionX = fDimensionZ - fDimensionX;
			fDimensionX /= 2.f;
			vMax.x += fDimensionX;
			vMin.x -= fDimensionX;

			fDimensionY = fDimensionZ - fDimensionY;
			fDimensionY /= 2.f;
			vMax.y += fDimensionY;
			vMin.y -= fDimensionY;
		}
	}
	else if (iCountSplitsSys == GEOM_COUNT_TYPE_SEGMENTATION_QUAD)
	{
		if (fDimensionX > fDimensionZ)
		{
			fDimensionZ = fDimensionX - fDimensionZ;
			fDimensionZ /= 2.f;
			vMax.z += fDimensionZ;
			vMin.z -= fDimensionZ;
		}
		else if (fDimensionZ > fDimensionX)
		{
			fDimensionX = fDimensionZ - fDimensionX;
			fDimensionX /= 2.f;
			vMax.x += fDimensionX;
			vMin.x -= fDimensionX;
		}
	}

	pModel->m_pArrSplits->m_pBoundVolumeSys = SGCore_CrBound();
	pModel->m_pArrSplits->m_pBoundVolumeSys->setMinMax(&vMin, &vMax);
	//}
	//**********************************************************************

	pModel->m_pArrSplits->m_uiCountSubSet = pModel->m_pModel->m_uiSubsetCount;
	pModel->m_pArrSplits->m_pNumberGroup = new uint32_t[pModel->m_pModel->m_uiSubsetCount];
	pModel->m_pArrSplits->m_pCountPoly = new uint32_t[pModel->m_pModel->m_uiSubsetCount];

	//заполняем массив с номерами подгрупп и массив с количествами полигонов
	for (int i = 0; i<pModel->m_pModel->m_uiSubsetCount; ++i)
	{
		pModel->m_pArrSplits->m_pNumberGroup[i] = i;
		pModel->m_pArrSplits->m_pCountPoly[i] = pModel->m_pModel->m_pIndexCount[i] / 3;
	}

	//создаем массивы с полигонами, каждый полигон принадлежит своей подгруппе
	pModel->m_pArrSplits->m_ppArrPoly = new uint32_t*[pModel->m_pArrSplits->m_uiCountSubSet];
	for (int i = 0; i<pModel->m_pArrSplits->m_uiCountSubSet; ++i)
	{
		pModel->m_pArrSplits->m_ppArrPoly[i] = new uint32_t[pModel->m_pArrSplits->m_pCountPoly[i] * 3];
	}

	//заполняем массивы с полигонами
	for (int i = 0; i<pModel->m_pModel->m_uiSubsetCount; ++i)
	{
		memcpy(pModel->m_pArrSplits->m_ppArrPoly[i], m_pCurrArrMeshIndex + pModel->m_pModel->m_pStartIndex[i], pModel->m_pModel->m_pIndexCount[i] * sizeof(uint32_t));
	}

	//если количество полигонов и текущая установка деления позволяют делить дальше
	if (pModel->m_pArrSplits->m_uiCountAllPoly >= (UINT)iCountPolyInSegment && iCountSplitsSys != 0)
	{
		pModel->m_pArrSplits->m_isFinite = false;
		segmentationCycle(pModel->m_pArrSplits, pModel, iCountSplitsSys, iCountPolyInSegment);
	}
	else
	{
		//оптимизация для Post TnL кэша
		//{{
		Array<UINT> aIndeces;
		bool isUnic = true;
		for (DWORD q = 0; q<pModel->m_pArrSplits->m_uiCountSubSet; q++)
		{
			aIndeces.clear();
			aIndeces.push_back(pModel->m_pArrSplits->m_ppArrPoly[q][0]);
			for (int k = 0; k<pModel->m_pArrSplits->m_pCountPoly[q] * 3; ++k)
			{
				isUnic = true;
				for (int j = 0; j<aIndeces.size() && isUnic; ++j)
				{
					if (pModel->m_pArrSplits->m_ppArrPoly[q][k] == aIndeces[j])
						isUnic = false;
				}

				if (isUnic)
					aIndeces.push_back(pModel->m_pArrSplits->m_ppArrPoly[q][k]);
			}

			SGCore_OptimizeIndecesInSubsetUint32(pModel->m_pArrSplits->m_ppArrPoly[q], pModel->m_pArrSplits->m_pCountPoly[q], aIndeces.size());
		}
		//}}

		pModel->m_pArrSplits->m_isFinite = true;
	}
}

void CModels::segmentationCycle(CSegment *pSplit, CModel *pModel, int iCountSplitsSys, int iCountPolyInSegment)
{
	Array<CSegment*> aQueue;
	int iCount = 0;
	aQueue.push_back(pSplit);

	while (aQueue.size() > iCount)
	{
		CSegment *pSegment = aQueue[iCount];

		if (!(pSegment->m_isFinite))
		{
			segmentation2(pSegment, pModel, iCountSplitsSys, iCountPolyInSegment, &aQueue);

			for (int i = 0; i < iCountSplitsSys; i++)
			{
				if (pSegment->m_aSplits[i])
					aQueue.push_back(pSegment->m_aSplits[i]);
			}
		}

		++iCount;
	}
}


void CModels::segmentation2(CSegment *pSplit, CModel *pModel, int iCountSplitsSys, int iCountPolyInSegment, Array<CSegment*> *pQueue)
{
	//полигоны сегментов, точнее индексы в порядке полигонов (счет по 3 индекса)
	Array<UINT> aSegmentPoly[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];

	//подгруппы сегментов, на каждый полигон записывается подгруппа
	Array<UINT> aSegmentGroup[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];


	for (int i = 0; i<iCountSplitsSys; ++i)
		pSplit->m_aSplits[i] = new CSegment();

	//получаем ограничивающие объемы для деления и облегаемые
	//{{
	if (iCountSplitsSys == GEOM_COUNT_TYPE_SEGMENTATION_QUAD)
	{
		ISXBound *aBounds[GEOM_COUNT_TYPE_SEGMENTATION_QUAD];
		for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_QUAD; ++i)
			aBounds[i] = SGCore_CrBound();

		float3 vMin, vMax;
		SGCore_0ComBoundBoxArr4(pSplit->m_pBoundVolumeSys, (aBounds));
		
		for (int i = 0; i<iCountSplitsSys; i++)
		{
			pSplit->m_aSplits[i]->m_pBoundVolumeSys = aBounds[i];
			pSplit->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);
			pSplit->m_aSplits[i]->m_pBoundVolumeP = SGCore_CrBound();
			pSplit->m_aSplits[i]->m_pBoundVolumeP->setMinMax(&vMin, &vMax);
		}
	}
	else if (iCountSplitsSys == GEOM_COUNT_TYPE_SEGMENTATION_OCTO)
	{
		ISXBound *aBounds[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];
		for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
			aBounds[i] = SGCore_CrBound();

		float3 vMin, vMax;
		SGCore_0ComBoundBoxArr8(pSplit->m_pBoundVolumeSys, aBounds);

		for (int i = 0; i<iCountSplitsSys; i++)
		{
			pSplit->m_aSplits[i]->m_pBoundVolumeSys = aBounds[i];
			pSplit->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);
			pSplit->m_aSplits[i]->m_pBoundVolumeP = SGCore_CrBound();
			pSplit->m_aSplits[i]->m_pBoundVolumeP->setMinMax(&vMin, &vMax);
		}
	}
	//}}

	//массив свободности каждого полигона
	bool *aFreePoly = new bool[pSplit->m_uiCountAllPoly];
	for (int i = 0; i<pSplit->m_uiCountAllPoly; ++i)
		aFreePoly[i] = true;

	//распределяем полигоны по сплитам
	//{
	int iCountComPoly = 0;
	float3 vMin, vMax;
	for (int i = 0; i<iCountSplitsSys; i++)
	{
		//pSplit->m_aSplits[i]->m_pBoundVolumeP->getMinMax(&tmpMin, &tmpMax);
		//SGCore_FCreateBoundingBoxMesh(&tmpMin, &tmpMax, &(pSplit->m_aSplits[i]->m_pBoundBox));
		
		pSplit->m_aSplits[i]->m_pBoundVolumeSys->getMinMax(&vMin, &vMax);
		int iNumCurrentPoly = 0;
		for (int j = 0; j<pSplit->m_uiCountSubSet; ++j)
		{
			for (int k = 0; k<pSplit->m_pCountPoly[j] * 3; k += 3)
			{
				if (SGCore_0InPosPoints2D(&vMin, &vMax,
					&m_pCurrArrMeshVertex[pSplit->m_ppArrPoly[j][k]],
					&m_pCurrArrMeshVertex[pSplit->m_ppArrPoly[j][k + 1]],
					&m_pCurrArrMeshVertex[pSplit->m_ppArrPoly[j][k + 2]]
					)
					&& aFreePoly[iNumCurrentPoly]
					)
				{
					aSegmentPoly[i].push_back((pSplit->m_ppArrPoly[j][k]));
					aSegmentPoly[i].push_back((pSplit->m_ppArrPoly[j][k + 1]));
					aSegmentPoly[i].push_back((pSplit->m_ppArrPoly[j][k + 2]));
					aSegmentGroup[i].push_back((pSplit->m_pNumberGroup[j]));

					aFreePoly[iNumCurrentPoly] = false;
					++iCountComPoly;
				}
				++iNumCurrentPoly;
			}
		}
	}
	//}

	// если вдруг какой-то из полигонов никуда не попал, то путем несложных расчетов определяем его в один из сплитов
	if (iCountComPoly < pSplit->m_uiCountAllPoly)
	{
		int iNumCurrentPoly = 0;
		float3 vVertex0, vVertex1, vVertex2, vCenterPoly, vLength, vSphereCenter;
		float aLength[8], fShereRadius;

		for (int j = 0; j<pSplit->m_uiCountSubSet; ++j)
		{
			for (int k = 0; k<pSplit->m_pCountPoly[j] * 3; k += 3)
			{
				if (aFreePoly[iNumCurrentPoly])
				{
					vVertex0 = m_pCurrArrMeshVertex[pSplit->m_ppArrPoly[j][k]];
					vVertex1 = m_pCurrArrMeshVertex[pSplit->m_ppArrPoly[j][k + 1]];
					vVertex2 = m_pCurrArrMeshVertex[pSplit->m_ppArrPoly[j][k + 2]];

					vCenterPoly = (vVertex0 + vVertex1 + vVertex2) / 3.0f;

					for (int q = 0; q<iCountSplitsSys; ++q)
					{
						pSplit->m_aSplits[q]->m_pBoundVolumeSys->getSphere(&vSphereCenter, &fShereRadius);
						vLength = -vSphereCenter + vCenterPoly;
						aLength[q] = SMVector3Length(vLength);
					}

					int iKey = 0;

					for (int q = 0; q<iCountSplitsSys; ++q)
					{
						bool isSmall = true;
						for (int n = 0; n<iCountSplitsSys; ++n)
						{
							if (aLength[q] > aLength[n])
							{
								isSmall = false;
								//break;
							}
						}

						if (isSmall)
						{
							iKey = q;
							//break;
						}
					}

					aSegmentPoly[iKey].push_back((pSplit->m_ppArrPoly[j][k]));
					aSegmentPoly[iKey].push_back((pSplit->m_ppArrPoly[j][k + 1]));
					aSegmentPoly[iKey].push_back((pSplit->m_ppArrPoly[j][k + 2]));
					aSegmentGroup[iKey].push_back((pSplit->m_pNumberGroup[j]));

					aFreePoly[iNumCurrentPoly] = false;
				}
				++iNumCurrentPoly;
			}
		}
	}

	// проход по всем плитам и правка данных
	for (int i = 0; i<iCountSplitsSys; i++)
	{
		if (aSegmentPoly[i].size() > 0)
		{
			pSplit->m_aSplits[i]->m_idDepth = pSplit->m_idDepth + 1;
			pSplit->m_aSplits[i]->m_pParent = pSplit;

			//подсчет количества уникальных подгрупп
			//{
			Array<UINT> aUnicSubSet;
			pSplit->m_aSplits[i]->m_uiCountSubSet = 0;

			//предварительно записываем одну подгруппу в массив aUnicSubSet
			aUnicSubSet.push_back(aSegmentGroup[i][0]);
			pSplit->m_aSplits[i]->m_uiCountSubSet++;

			//циклом проходимся по aSegmentGroup и выбираем оттуда все первые уникальные значения
			for (int k = 0; k<aSegmentGroup[i].size(); ++k)
			{
				bool isRecorded = false;
				for (int j = 0; j<aUnicSubSet.size() && !isRecorded; ++j)
				{
					if (aUnicSubSet[j] == aSegmentGroup[i][k])
						isRecorded = true;
				}

				if (!isRecorded)
				{
					aUnicSubSet.push_back(aSegmentGroup[i][k]);
					pSplit->m_aSplits[i]->m_uiCountSubSet++;
				}
			}
			//}

			//
			pSplit->m_aSplits[i]->m_pNumberGroup = new uint32_t[pSplit->m_aSplits[i]->m_uiCountSubSet];
			
			//записываем уникальные номера подгрупп
			for (int k = 0; k<pSplit->m_aSplits[i]->m_uiCountSubSet; ++k)
				pSplit->m_aSplits[i]->m_pNumberGroup[k] = (aUnicSubSet[k]);

			aUnicSubSet.clear();


			pSplit->m_aSplits[i]->m_pCountPoly = new uint32_t[pSplit->m_aSplits[i]->m_uiCountSubSet];

			//обнуление данных по количеству полигонов в сплите
			for (int j = 0; j<pSplit->m_aSplits[i]->m_uiCountSubSet; ++j)
				pSplit->m_aSplits[i]->m_pCountPoly[j] = 0;

			pSplit->m_aSplits[i]->m_uiCountAllPoly = 0;

			//вычисляем сколько полигонов в каждой подгруппе в данном сплите
			for (int k = 0; k<aSegmentPoly[i].size(); k += 3)
			{
				for (int j = 0; j<pSplit->m_aSplits[i]->m_uiCountSubSet; ++j)
				{
					if (pSplit->m_aSplits[i]->m_pNumberGroup[j] == (aSegmentGroup[i][k / 3]))
					{
						pSplit->m_aSplits[i]->m_pCountPoly[j]++;
						pSplit->m_aSplits[i]->m_uiCountAllPoly++;
					}
				}
			}

			pSplit->m_aSplits[i]->m_ppArrPoly = new uint32_t*[pSplit->m_aSplits[i]->m_uiCountSubSet];

			for (int j = 0; j<pSplit->m_aSplits[i]->m_uiCountSubSet; ++j)
				pSplit->m_aSplits[i]->m_ppArrPoly[j] = new uint32_t[pSplit->m_aSplits[i]->m_pCountPoly[j] * 3];

			uint32_t* tmpCountPG = new uint32_t[pSplit->m_aSplits[i]->m_uiCountSubSet];

			for (int k = 0; k<pSplit->m_aSplits[i]->m_uiCountSubSet; ++k)
			{
				tmpCountPG[k] = 0;
			}

			for (int k = 0; k<aSegmentPoly[i].size(); k += 3)
			{
				for (int j = 0; j<pSplit->m_aSplits[i]->m_uiCountSubSet; ++j)
				{
					if (pSplit->m_aSplits[i]->m_pNumberGroup[j] == (aSegmentGroup[i][k / 3]))
					{
						pSplit->m_aSplits[i]->m_ppArrPoly[j][tmpCountPG[j]] = (aSegmentPoly[i][k]);
						pSplit->m_aSplits[i]->m_ppArrPoly[j][tmpCountPG[j] + 1] = (aSegmentPoly[i][k + 1]);
						pSplit->m_aSplits[i]->m_ppArrPoly[j][tmpCountPG[j] + 2] = (aSegmentPoly[i][k + 2]);

						tmpCountPG[j] += 3;
					}
				}
			}

			aSegmentPoly[i].clear();
			aSegmentGroup[i].clear();

			mem_delete_a(tmpCountPG);

			editVolume(pModel, pSplit->m_aSplits[i]);

			if (pSplit->m_aSplits[i]->m_uiCountAllPoly > 0 && pSplit->m_aSplits[i]->m_uiCountAllPoly > (UINT)iCountPolyInSegment)
				pSplit->m_aSplits[i]->m_isFinite = false;
			else
			{
				pSplit->m_aSplits[i]->m_isFinite = true;
				
				//оптимизация для Post TnL кэша (13.11.2018 - подробностей не помню, но вроде полезная штука :) )
				//{
				Array<uint32_t> aIndeces;
				bool isUnic = true;
				for (int q = 0; q<pSplit->m_aSplits[i]->m_uiCountSubSet; ++q)
				{
					aIndeces.clear();
					aIndeces.push_back(pSplit->m_aSplits[i]->m_ppArrPoly[q][0]);
					for (int k = 0; k<pSplit->m_aSplits[i]->m_pCountPoly[q] * 3; ++k)
					{
						isUnic = true;
						for (int j = 0; j<aIndeces.size() && isUnic; ++j)
						{
							if (pSplit->m_aSplits[i]->m_ppArrPoly[q][k] == aIndeces[j])
								isUnic = false;						}

						if (isUnic)
							aIndeces.push_back(pSplit->m_aSplits[i]->m_ppArrPoly[q][k]);
					}

					SGCore_OptimizeIndecesInSubsetUint32(pSplit->m_aSplits[i]->m_ppArrPoly[q], pSplit->m_aSplits[i]->m_pCountPoly[q], aIndeces.size());
				}
				//}
			}
		}
		else
		{
			pSplit->m_aSplits[i]->m_uiCountAllPoly = 0;
			pSplit->m_aSplits[i]->m_isFinite = true;

			mem_delete(pSplit->m_aSplits[i]);
		}
	}

	mem_delete_a(pSplit->m_pNumberGroup);
	mem_delete_a(pSplit->m_pCountPoly);

	for (int k = 0; k<pSplit->m_uiCountSubSet; ++k)
	{
		mem_delete_a(pSplit->m_ppArrPoly[k]);
	}

	mem_delete_a(pSplit->m_ppArrPoly);
	mem_delete_a(aFreePoly);
}

void CModels::editVolume(CModel *pModel, CSegment *pSplit)
{
	if (!pModel || !pSplit)
		return;

	float3 vMax, vMin;
	vMax = m_pCurrArrMeshVertex[(pSplit->m_ppArrPoly[0][0])];
	vMin = m_pCurrArrMeshVertex[(pSplit->m_ppArrPoly[0][0])];

	/*float3 vMax2, vMin2;
	pSplit->m_pBoundVolumeSys->getMinMax(&vMin2, &vMax2);*/

	float3 vCurr;
	for (UINT i = 0; i<pSplit->m_uiCountSubSet; ++i)
	{
		for (UINT k = 0; k<pSplit->m_pCountPoly[i] * 3; ++k)
		{
			vCurr = m_pCurrArrMeshVertex[(pSplit->m_ppArrPoly[i][k])] * (* (pSplit->m_pBoundVolumeP->calcWorld()));
			if (vCurr.x > vMax.x)
				vMax.x = vCurr.x;
			if (vCurr.y > vMax.y)
				vMax.y = vCurr.y;
			if (vCurr.z > vMax.z)
				vMax.z = vCurr.z;

			if (vCurr.x < vMin.x)
				vMin.x = vCurr.x;
			if (vCurr.y < vMin.y)
				vMin.y = vCurr.y;
			if (vCurr.z < vMin.z)
				vMin.z = vCurr.z;
		}
	}

	pSplit->m_pBoundVolumeP->setMinMax(&vMin, &vMax);
}

void CModels::setSplitID(CSegment *pSplit, ID *pSplitsIDs, ID *pSplitsIDsRender)
{
	Array<CSegment*> aQueue;
	int iCount = 0;
	aQueue.push_back(pSplit);

	while (aQueue.size() > iCount)
	{
		if (aQueue[iCount] && !(aQueue[iCount]->m_isFinite))
		{
			aQueue[iCount]->m_ID = (*pSplitsIDs);
			++(*pSplitsIDs);
			for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
			{
				if (aQueue[iCount]->m_aSplits[i])
					aQueue[iCount]->m_aSplits[i]->m_pParent = aQueue[iCount];

				aQueue.push_back(aQueue[iCount]->m_aSplits[i]);
			}
		}
		else if (aQueue[iCount])
		{
			aQueue[iCount]->m_ID = (*pSplitsIDs);
			aQueue[iCount]->m_SID = (*pSplitsIDsRender);
			++(*pSplitsIDs);
			++(*pSplitsIDsRender);
		}

		++iCount;
	}
}

//##########################################################################

void CModels::comVisible(const IFrustum *pFrustum, const float3 *pViewPos, ID idVisCalcObj)
{
	STATIC_PRECOND_VISCALCOBJ_ID(idVisCalcObj, _VOID);

	CVisInfo *pVisCalcObj = m_aVisInfo[idVisCalcObj];

	float3 vSphereCenter;
	float fSphereRadius;

	for (int i = 0; i < m_aModels.size(); ++i)
	{
		CModel *pModel = m_aModels[i];

		if (pModel->m_aTransparency.size() > 0)
			comVisibleTransparency(pFrustum, pViewPos, idVisCalcObj);

		//если в модели нет подгрупп тогда пропускаем
		if (pModel->m_pModel->m_uiSubsetCount == 0)
			continue;

		//frustum culling
		pModel->m_pBoundVolume->getSphere(&vSphereCenter, &fSphereRadius);
		pVisCalcObj->m_aVisible4Model[i]->m_isVisible = pFrustum->sphereInFrustum(&vSphereCenter, fSphereRadius);

		//если объект расчетов видимости для наблюдателя
		if (idVisCalcObj == SX_GEOM_DEFAULT_VISCALCOBJ)
		{
			//считаем расстояние от объекта до наблюдателя
			pModel->m_fDist4Observer = SMVector3Length((vSphereCenter - (*pViewPos))) - fSphereRadius;

			//если модель видна то делаем occlusion culling
			if (pVisCalcObj->m_aVisible4Model[i])
			{
				float3 vMin, vMax;
				pModel->m_pBoundVolume->getMinMax(&vMin, &vMax);
				pVisCalcObj->m_aVisible4Model[i]->m_isVisible = SGCore_OC_IsVisible(&vMax, &vMin);
			}
		}

		if (pVisCalcObj->m_aVisible4Model[i]->m_isVisible)
		{
			//если расстояние от наблюдателя до модели допускает рисование лода и лод есть - рисуем лод
			if (pModel->m_fDist4Observer >= SX_GEOM_DIST4LOD && pModel->m_pLod && pModel->m_pLod->m_pModel)
				pModel->m_isRenderLod = true;
			else
			{
				pModel->m_isRenderLod = false;

				//если есть сегменты, то считаем какие сегменты рисовать
				if (pModel->m_pArrSplits)
				{
					int iCountIteration = 0;
					int iCountSplits = pVisCalcObj->m_aVisible4Model[i]->m_iCount;
					pVisCalcObj->m_aVisible4Model[i]->m_iCountCom = 0;
					int *iCountCom = &(pVisCalcObj->m_aVisible4Model[i]->m_iCountCom);
					CSegment **ppSegments = pVisCalcObj->m_aVisible4Model[i]->m_ppSegments;

					Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> aQueue;
					aQueue.push_back(pModel->m_pArrSplits);

					while (aQueue.size() > iCountIteration)
					{
						comVisibleSegment(idVisCalcObj, pFrustum, ppSegments, iCountCom, aQueue[iCountIteration], pViewPos, &aQueue, iCountSplits);

						++iCountIteration;
					}
				}
			}
		}
	}
}

void CModels::comVisibleSegment(ID idVisCalcObj, const IFrustum *pFrustum, CSegment **ppSplits, int *iCount, CSegment *pSegment, const float3 *pViewPos, Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> *pQueue, int iCountSplitsRender)
{
	STATIC_PRECOND_VISCALCOBJ_ID(idVisCalcObj, _VOID);

	float fSphereRadius;
	float3 vSphereCenter;
	pSegment->m_pBoundVolumeP->getSphere(&vSphereCenter, &fSphereRadius);

	if (pSegment->m_uiCountAllPoly > 0 && (pFrustum->sphereInFrustum(&vSphereCenter, fSphereRadius)))
	{
		if (!(pSegment->m_isFinite))
		{
			for (int j = 0; j<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
			{
				if (pSegment->m_aSplits[j])
					pQueue->push_back(pSegment->m_aSplits[j]);
			}
		}
		else
		{
			if ((*iCount) < iCountSplitsRender)
			{
				bool isVisible = true;

				if (idVisCalcObj == SX_GEOM_DEFAULT_VISCALCOBJ)
				{
					float3 vMin, vMax;
					pSegment->m_pBoundVolumeP->getMinMax(&vMin, &vMax);
					isVisible = SGCore_OC_IsVisible(&vMax, &vMin);
				}

				if (isVisible)
				{
					ppSplits[(*iCount)] = pSegment;
					pSegment->m_fDistForCamera = SMVector3Length((vSphereCenter - (*pViewPos))) - fSphereRadius;
					++(*iCount);
				}
			}
		}
	}
}

void CModels::comVisibleTransparency(const IFrustum *pFrustum, const float3 *pViewPos, ID idVisCalcObj)
{
	STATIC_PRECOND_VISCALCOBJ_ID(idVisCalcObj, _VOID);

	CVisInfo *pVisCalcObj = m_aVisInfo[idVisCalcObj];

	float fSphereRadius;
	float3 vSphereCenter;

	float3 vMin, vMax;

	for (int i = 0; i < m_aTransparency.size(); ++i)
	{
		CTransparencyModel *pTransparency = m_aTransparency[i];
		
		if (!pTransparency)
			continue;

		pTransparency->m_pBoundVolume->getSphere(&vSphereCenter, &fSphereRadius);
		pTransparency->m_pBoundVolume->getMinMax(&vMin, &vMax);
		pVisCalcObj->m_aVisible4Transparency[i] = pFrustum->boxInFrustum(&vMin, &vMax);

		if (idVisCalcObj == SX_GEOM_DEFAULT_VISCALCOBJ && m_aVisInfo[idVisCalcObj]->m_aVisible4Transparency[i])
		{
			pVisCalcObj->m_aVisible4Transparency[i] = SGCore_OC_IsVisible(&vMax, &vMin);
			pTransparency->m_isVisible4Observer = pVisCalcObj->m_aVisible4Transparency[i];
			pTransparency->m_fDist4Observer = SMVector3Length((vSphereCenter - (*pViewPos))) - fSphereRadius;
		}
	}
}

//##########################################################################

void CModels::renderSingly(DWORD timeDelta, ID idModel, ID idMtrl, const float3 *pPos)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	CModel *pModel = m_aModels[idModel];

	if (pModel->m_isRenderLod && pModel->m_pLod)
		renderLod(timeDelta, idModel, idMtrl, pPos);
	else
	{
		if (pModel->m_pModel->m_uiSubsetCount > 0)
			renderObject(timeDelta, idModel, idMtrl, pPos);
	}
}

void CModels::render(DWORD timeDelta, GEOM_RENDER_TYPE type, ID idVisCalcObj)
{
	STATIC_PRECOND_VISCALCOBJ_ID(idVisCalcObj, _VOID);

	if (type == GEOM_RENDER_TYPE_ALL || type == GEOM_RENDER_TYPE_OPAQUE)
	{
		for (int i = 0; i < m_aModels.size(); ++i)
		{
			if (m_aVisInfo[idVisCalcObj]->m_aVisible4Model[i])
			{
				if (m_aModels[i]->m_isRenderLod && m_aModels[i]->m_pLod)
				{
					renderLod(timeDelta, i);
				}
				else
				{
					if (m_aModels[i]->m_pModel->m_uiSubsetCount == 0)
						continue;

					if (m_aModels[i]->m_pArrSplits)
						renderSegmets(timeDelta, i, -1, idVisCalcObj);
					else
						renderObject(timeDelta, i);
				}
			}
		}
	}

	if (type == GEOM_RENDER_TYPE_ALL || type == GEOM_RENDER_TYPE_TRANSPARENCY)
	{
		if (idVisCalcObj != SX_GEOM_DEFAULT_VISCALCOBJ)
		{
			for (int k = 0, kl = m_aTransparency.size(); k < kl; ++k)
			{
				if (m_aTransparency[k] && m_aVisInfo[idVisCalcObj]->m_aVisible4Transparency[k])
					renderTransparency(timeDelta, m_aTransparency[k]);
			}
		}
		else if (idVisCalcObj == SX_GEOM_DEFAULT_VISCALCOBJ)
		{
			for (int k = 0, kl = m_aSortTransparency.size(); k < kl; ++k)
			{
				if (m_aSortTransparency[k] && m_aSortTransparency[k]->m_isVisible4Observer)
					renderTransparency(timeDelta, m_aSortTransparency[k]);
			}
		}
	}
}

void CModels::renderLod(DWORD timeDelta, ID idModel, ID idTex, const float3 *pPos)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	g_pDXDevice->SetStreamSource(0, m_aModels[idModel]->m_pLod->m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static_ex));

	g_pDXDevice->SetIndices(m_aModels[idModel]->m_pLod->m_pModel->m_pIndexBuffer);
	g_pDXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());

	int iCountIndex = 0;

	float4x4 mWorld;

	if (pPos)
	{
		const float3 *pScale = m_aModels[idModel]->m_pBoundVolume->getScale();
		const float3 *pRot = m_aModels[idModel]->m_pBoundVolume->getRotation();
		mWorld = SMMatrixScaling(*pScale) * SMMatrixRotationX(pRot->x) * SMMatrixRotationY(pRot->y) * SMMatrixRotationZ(pRot->z) * SMMatrixTranslation(*pPos);
	}
	else
		mWorld = *(m_aModels[idModel]->m_pBoundVolume->calcWorld());

	for (int g = 0; g < m_aModels[idModel]->m_pLod->m_pModel->m_uiSubsetCount; g++)
	{
		SGCore_MtlSet((idTex > 0 ? idTex : m_aModels[idModel]->m_pLod->m_aIDsTextures[g]), &mWorld);

		SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aModels[idModel]->m_pLod->m_pModel->m_pVertexCount[g], iCountIndex, m_aModels[idModel]->m_pLod->m_pModel->m_pIndexCount[g] / 3);
		Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ((m_aModels[idModel]->m_pLod->m_pModel->m_pIndexCount[g] / 3)));
		iCountIndex += m_aModels[idModel]->m_pLod->m_pModel->m_pIndexCount[g];
	}
}

void CModels::renderObject(DWORD timeDelta, ID idModel, ID idTex, const float3 *pPos)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	//если нет подгрупп у модели - не отрисовываем
	if (m_aModels[idModel]->m_pModel->m_uiSubsetCount == 0)
		return;

	g_pDXDevice->SetStreamSource(0, m_aModels[idModel]->m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static_ex));

	g_pDXDevice->SetIndices(m_aModels[idModel]->m_pModel->m_pIndexBuffer);
	g_pDXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());

	int iCountIndex = 0;

	float4x4 mWorld;

	if (pPos)
	{
		const float3 *pScale = m_aModels[idModel]->m_pBoundVolume->getScale();
		const float3 *pRot = m_aModels[idModel]->m_pBoundVolume->getRotation();
		mWorld = SMMatrixScaling(*pScale) * SMMatrixRotationX(pRot->x) * SMMatrixRotationY(pRot->y) * SMMatrixRotationZ(pRot->z) * SMMatrixTranslation(*pPos);
	}
	else
		mWorld = *(m_aModels[idModel]->m_pBoundVolume->calcWorld());

	for (int g = 0; g < m_aModels[idModel]->m_pModel->m_uiSubsetCount; g++)
	{
		SGCore_MtlSet((idTex >= 0 ? idTex : m_aModels[idModel]->m_aIDsTextures[g]), &mWorld);

		SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, m_aModels[idModel]->m_pModel->m_pVertexCount[g], iCountIndex, m_aModels[idModel]->m_pModel->m_pIndexCount[g] / 3);
		Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ((m_aModels[idModel]->m_pModel->m_pIndexCount[g] / 3)));
		iCountIndex += m_aModels[idModel]->m_pModel->m_pIndexCount[g];
	}
}

void CModels::renderSegmets(DWORD timeDelta, ID idModel, ID idTex, ID idVisCalcObj)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);
	STATIC_PRECOND_VISCALCOBJ_ID(idVisCalcObj, _VOID);

	CModel *pModel = m_aModels[idModel];

	//если нет подгрупп у модели - не отрисовываем
	if (pModel->m_pModel->m_uiSubsetCount == 0)
		return;

	//если количество видимых сплитов, для объекта расчетов видимости, нулевое (то есть нечего рисовать), то не обрабатываем
	if (m_aVisInfo[idVisCalcObj]->m_aVisible4Model[idModel]->m_iCountCom <= 0)
		return;

	//обнуляем массив количества рисуемых полгонов для всех подгрупп
	memset(pModel->m_pCountDrawPoly, 0, sizeof(UINT)* pModel->m_pModel->m_uiSubsetCount);
	
	for (int j = 0, jl = m_aVisInfo[idVisCalcObj]->m_aVisible4Model[idModel]->m_iCountCom; j<jl; j++)
	{
		CSegment **ppSegments = m_aVisInfo[idVisCalcObj]->m_aVisible4Model[idModel]->m_ppSegments;
		for (int g = 0; g<ppSegments[j]->m_uiCountSubSet; ++g)
		{
			int iGroupModel = ppSegments[j]->m_pNumberGroup[g];
			if (
				ppSegments[j]->m_pCountPoly[g] > 0 &&	//если количество полигонов больше 0
				//если количество записанных полигонов в данную подгруппу меньше либо равно общему количеству полигонов которое содержит данна¤ подгруппа
				pModel->m_pCountDrawPoly[iGroupModel] + ppSegments[j]->m_pCountPoly[g] <= pModel->m_pModel->m_pIndexCount[iGroupModel] / 3
				)
			{
				memcpy(pModel->m_pVisibleIndeces[iGroupModel] + pModel->m_pCountDrawPoly[iGroupModel]*3,
					ppSegments[j]->m_ppArrPoly[g],
					ppSegments[j]->m_pCountPoly[g] * sizeof(uint32_t)* 3);

				pModel->m_pCountDrawPoly[iGroupModel] += ppSegments[j]->m_pCountPoly[g];
			}
		}
	}

	//записываем в индексный буфер все видимые индексы
	//{
	UINT *pIndices;
	int iCountCopyPoly = 0;
	pModel->m_pVisibleIndexBuffer->Lock(0, 0, (void**)&(pIndices), D3DLOCK_DISCARD);
	
	for (int i = 0; i<pModel->m_pModel->m_uiSubsetCount; ++i)
	{
		if (pModel->m_pCountDrawPoly[i] > 0)
		{
			memcpy(pIndices + iCountCopyPoly, pModel->m_pVisibleIndeces[i], pModel->m_pCountDrawPoly[i] * 3 * sizeof(UINT));
			iCountCopyPoly += pModel->m_pCountDrawPoly[i] * 3;
		}
	}

	pModel->m_pVisibleIndexBuffer->Unlock();
	//}

	g_pDXDevice->SetStreamSource(0, pModel->m_pModel->m_pVertexBuffer, 0, sizeof(vertex_static_ex));
	g_pDXDevice->SetIndices(pModel->m_pVisibleIndexBuffer);
	g_pDXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());

	int iCountIndex = 0;

	for (int g = 0; g < pModel->m_pModel->m_uiSubsetCount; g++)
	{
		if (pModel->m_pCountDrawPoly[g] > 0)
		{
			SGCore_MtlSet((idTex > 0 ? idTex : pModel->m_aIDsTextures[g]), (float4x4*)pModel->m_pBoundVolume->calcWorld());

			SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, pModel->m_pModel->m_pVertexCount[g], iCountIndex, pModel->m_pCountDrawPoly[g]);
			Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + (pModel->m_pCountDrawPoly[g]));
			iCountIndex += pModel->m_pCountDrawPoly[g] * 3;
		}
	}
}

void CModels::renderTransparency(DWORD timeDelta, CTransparencyModel *pTransparencyModel)
{
	if (!pTransparencyModel)
		return;

	g_pDXDevice->SetStreamSource(0, pTransparencyModel->m_pVertexBuffer, 0, sizeof(vertex_static_ex));

	g_pDXDevice->SetIndices(pTransparencyModel->m_pIndexBuffer);
	g_pDXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());

	SGCore_MtlSet(pTransparencyModel->m_idTex, (float4x4*)pTransparencyModel->m_pBoundVolume->calcWorld());

	SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, pTransparencyModel->m_iCountVertex, 0, pTransparencyModel->m_iCountIndex / 3);
	Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ((pTransparencyModel->m_iCountIndex / 3)));
}

//##########################################################################

bool SortTransparencyCompareFunc(CModels::CTransparencyModel *pM1, CModels::CTransparencyModel *pM2)
{
	return (pM1->m_fDist4Observer < pM1->m_fDist4Observer);
}

void CModels::sortTransparency(const float3 *pViewPos)
{
	m_aSortTransparency.clearFast();
	for (int i = 0; i < m_aTransparency.size(); ++i)
	{
		if (m_aTransparency[i])
			m_aSortTransparency.push_back(m_aTransparency[i]);
	}

	m_aSortTransparency.quickSort(SortTransparencyCompareFunc);
	/*for (int i = 0; i < m_aSortTransparency.size(); ++i)
	{
		LibReport(0, "%f\n", m_aSortTransparency[i]->m_fDist4Observer);
	}
	int qwerty = 0;*/
}

bool CModels::existsTransparency4Render(ID idVisCalcObj)
{
	STATIC_PRECOND_VISCALCOBJ_ID(idVisCalcObj, false);

	for (int k = 0, kl = m_aSortTransparency.size(); k < kl; ++k)
	{
		if (m_aSortTransparency[k] && m_aSortTransparency[k]->m_isVisible4Observer)
			return true;
	}

	return false;
}

void CModels::reCalcIdTransparency()
{
	for (int i = 0; i < m_aTransparency.size(); ++i)
	{
		int iLastKey = (m_aTransparency.size() - i) - 1;

		for (int k = 0; k < iLastKey; ++k)
		{
			if (!m_aTransparency[k])
			{
				m_aTransparency[k] = m_aTransparency[iLastKey];

				for (int j = 0, jl = m_aModels[m_aTransparency[iLastKey]->m_idModel]->m_aTransparency.size(); j < jl; ++j)
				{
					if (m_aModels[m_aTransparency[iLastKey]->m_idModel]->m_aTransparency[j] == iLastKey)
					{
						m_aModels[m_aTransparency[iLastKey]->m_idModel]->m_aTransparency[j] = k;
						break;
					}
				}

				m_aTransparency.erase(iLastKey);
			}
		}
	}
}

void CModels::reSetIdModel4Transparency(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	for (int i = 0, il = m_aModels[idModel]->m_aTransparency.size(); i < il; ++i)
	{
		m_aTransparency[m_aModels[idModel]->m_aTransparency[i]]->m_idModel = idModel;
	}
}

//##########################################################################

void CModels::modelSetLod(ID idModel, const char *szPath)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	if (!szPath)
		return;

	CModel *pModel = m_aModels[idModel];

	mem_delete(pModel->m_pLod);
	pModel->m_pLod = new CLod();

	char szFullPath4Model[1024];
	sprintf(szFullPath4Model, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), szPath);
	SGCore_StaticModelLoad(szFullPath4Model, &(pModel->m_pLod->m_pModel));

	char szTex[1024];
	ID idMtrl = -1;
	for (int i = 0; i < pModel->m_pLod->m_pModel->m_uiSubsetCount; ++i)
	{
		sprintf(szTex, "%s.dds", pModel->m_pLod->m_pModel->m_ppTextures[i]);
		idMtrl = SGCore_MtlLoad(szTex, MTL_TYPE_GEOM);
		pModel->m_pLod->m_aIDsTextures.push_back(idMtrl);
	}

	pModel->m_pLod->m_sPath = szPath;
}

void CModels::modelSetPhysics(ID idModel, const char *szPath)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	modelSetPhysics2(m_aModels[idModel], szPath);
}

void CModels::modelSetPhysics2(CModel *pModel, const char *szPath)
{
	if (!pModel || !szPath)
		return;

	mem_delete(pModel->m_pPhysics);
	pModel->m_pPhysics = new CPhysMesh();
	pModel->m_pPhysics->m_sPath = szPath;

	char szFullPath[1024];
	sprintf(szFullPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), szPath);

	ISXDataStaticModel *pStatiModel = 0;
	SGCore_StaticModelLoad(szFullPath, &pStatiModel);

	pModel->m_pPhysics->m_aVertex.resize(pStatiModel->m_uiAllVertexCount);
	vertex_static_ex *pVert = pStatiModel->m_pVertices;
	//pStatiModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVert, 0);
	for (int i = 0; i < pStatiModel->m_uiAllVertexCount; ++i)
	{
		pModel->m_pPhysics->m_aVertex[i] = pVert[i].Pos;
	}
	//pStatiModel->m_pVertexBuffer->Unlock();

	pModel->m_pPhysics->m_aIndex.resize(pStatiModel->m_uiAllIndexCount);
	pModel->m_pPhysics->m_aMtrl.resize(pStatiModel->m_uiAllIndexCount);

	UINT *pInd = pStatiModel->m_pIndices;
	//pStatiModel->m_pIndexBuffer->Lock(0, 0, (void **)&pInd, 0);

	UINT uiPrebias = 0;
	int iCountIndex = 0;
	char szTexName[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	for (int i = 0; i < pStatiModel->m_uiSubsetCount; ++i)
	{
		sprintf(szTexName, "%s.dds", pStatiModel->m_ppTextures[i]);
		pModel->m_pPhysics->m_aStringMtrl.push_back(szTexName);
		ID idMtrl = SGCore_MtlLoad(szTexName, MTL_TYPE_GEOM);
		for (int k = 0; k < pStatiModel->m_pIndexCount[i]; ++k)
		{
			pModel->m_pPhysics->m_aIndex[iCountIndex] = pInd[pStatiModel->m_pStartIndex[i] + k] /*+ uiPrebias*/;
			pModel->m_pPhysics->m_aMtrl[iCountIndex] = idMtrl;
			pModel->m_pPhysics->m_aNumMtrl[iCountIndex] = i;
			++iCountIndex;
		}
		uiPrebias += pStatiModel->m_pIndexCount[i];
	}
	//pStatiModel->m_pIndexBuffer->Unlock();
	mem_release_del(pStatiModel);
}

void CModels::modelGenGroupInfo(CModel *pModel)
{
	if (!pModel || pModel->m_pModel->m_uiSubsetCount <= 0)
		return;

	vertex_static_ex *pVertex = pModel->m_pModel->m_pVertices;
	//pModel->m_pModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);

	UINT *pIndex = pModel->m_pModel->m_pIndices;
	//pModel->m_pModel->m_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	for (int i = 0; i < pModel->m_pModel->m_uiSubsetCount; ++i)
	{
		float3 vMin, vMax;
		float3_t vPos = pVertex[pModel->m_pModel->m_pStartVertex[i]].Pos;

		for (int k = 0; k<pModel->m_pModel->m_pVertexCount[i]; k++)
		{
			vPos = pVertex[pModel->m_pModel->m_pStartVertex[i] + k].Pos;

			if (vPos.x > vMax.x)
				vMax.x = vPos.x;

			if (vPos.y > vMax.y)
				vMax.y = vPos.y;

			if (vPos.z > vMax.z)
				vMax.z = vPos.z;


			if (vPos.x < vMin.x)
				vMin.x = vPos.x;

			if (vPos.y < vMin.y)
				vMin.y = vPos.y;

			if (vPos.z < vMin.z)
				vMin.z = vPos.z;
		}

		pModel->m_aGroupInfo[i] = new CGroupInfo();
		D3DXPlaneFromPoints(&(pModel->m_aGroupInfo[i]->m_oPlane),
			&D3DXVECTOR3(pVertex[pIndex[pModel->m_pModel->m_pStartIndex[i] + 0]].Pos.x, pVertex[pIndex[pModel->m_pModel->m_pStartIndex[i] + 0]].Pos.y, pVertex[pIndex[pModel->m_pModel->m_pStartIndex[i] + 0]].Pos.z),
			&D3DXVECTOR3(pVertex[pIndex[pModel->m_pModel->m_pStartIndex[i] + 1]].Pos.x, pVertex[pIndex[pModel->m_pModel->m_pStartIndex[i] + 1]].Pos.y, pVertex[pIndex[pModel->m_pModel->m_pStartIndex[i] + 1]].Pos.z),
			&D3DXVECTOR3(pVertex[pIndex[pModel->m_pModel->m_pStartIndex[i] + 2]].Pos.x, pVertex[pIndex[pModel->m_pModel->m_pStartIndex[i] + 2]].Pos.y, pVertex[pIndex[pModel->m_pModel->m_pStartIndex[i] + 2]].Pos.z));

		pModel->m_aGroupInfo[i]->m_vCenter = (float3_t)((vMax + vMin) * 0.5);
		pModel->m_aGroupInfo[i]->m_vMin = vMin;
		pModel->m_aGroupInfo[i]->m_vMax = vMax;
	}

	//pModel->m_pModel->m_pVertexBuffer->Unlock();
	//pModel->m_pModel->m_pIndexBuffer->Unlock();
}

void CModels::modelGenGroupInfo4Transparency(CTransparencyModel *pModel)
{
	if (!pModel)
		return;

	vertex_static_ex* pVertex = pModel->m_pVertices;
	//pModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);

	UINT *pIndex = pModel->m_pIndices;
	//pModel->m_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	float3 vMin, vMax;
	float3_t vPos = pVertex[0].Pos;

	for (int k = 0; k<pModel->m_iCountVertex; k++)
	{
		vPos = pVertex[k].Pos;

		if (vPos.x > vMax.x)
			vMax.x = vPos.x;

		if (vPos.y > vMax.y)
			vMax.y = vPos.y;

		if (vPos.z > vMax.z)
			vMax.z = vPos.z;


		if (vPos.x < vMin.x)
			vMin.x = vPos.x;

		if (vPos.y < vMin.y)
			vMin.y = vPos.y;

		if (vPos.z < vMin.z)
			vMin.z = vPos.z;
	}

	D3DXPlaneFromPoints(&(pModel->m_oGroupInfo.m_oPlane),
		&D3DXVECTOR3(pVertex[pIndex[0]].Pos.x, pVertex[pIndex[0]].Pos.y, pVertex[pIndex[0]].Pos.z),
		&D3DXVECTOR3(pVertex[pIndex[1]].Pos.x, pVertex[pIndex[1]].Pos.y, pVertex[pIndex[1]].Pos.z),
		&D3DXVECTOR3(pVertex[pIndex[2]].Pos.x, pVertex[pIndex[2]].Pos.y, pVertex[pIndex[2]].Pos.z));

	pModel->m_oGroupInfo.m_vCenter = (float3_t)((vMax + vMin) * 0.5);
	pModel->m_oGroupInfo.m_vMin = vMin;
	pModel->m_oGroupInfo.m_vMax = vMax;

	//pModel->m_pVertexBuffer->Unlock();
	//pModel->m_pIndexBuffer->Unlock();
}

void CModels::modelGetMinMax(ID idModel, float3 *pMin, float3 *pMax)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	CModel *pModel = m_aModels[idModel];

	pModel->m_pBoundVolume->getMinMax(pMin, pMax);

	if (pModel->m_aTransparency.size() <= 0)
		return;

	float3 vMin, vMax;

	for (int i = 0, il = pModel->m_aTransparency.size(); i<il; ++i)
	{
		m_aTransparency[pModel->m_aTransparency[i]]->m_pBoundVolume->getMinMax(&vMin, &vMax);

		if (vMax.x > pMax->x)
			pMax->x = vMax.x;

		if (vMax.y > pMax->y)
			pMax->y = vMax.y;

		if (vMax.z > pMax->z)
			pMax->z = vMax.z;


		if (vMin.x < pMin->x)
			pMin->x = vMin.x;

		if (vMin.y < pMin->y)
			pMin->y = vMin.y;

		if (vMin.z < pMin->z)
			pMin->z = vMin.z;
	}
}

const char* CModels::modelGetName(ID idModel, char *szName)
{
	STATIC_PRECOND_MODEL_ID(idModel, 0);

	if (szName)
		strcpy(szName, m_aModels[idModel]->m_sName.c_str());

	return m_aModels[idModel]->m_sName.c_str();
}

void CModels::modelSetName(ID idModel, const char *szName)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	m_aModels[idModel]->m_sName = szName;
}

const char* CModels::modelGetPath4Model(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, 0);

	return m_aModels[idModel]->m_sPath.c_str();
}

int CModels::modelGetCountPoly(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, 0);

	return m_aModels[idModel]->m_pModel->m_uiAllIndexCount / 3;
}

const char* CModels::modelGetPath4Lod(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, 0);

	if (m_aModels[idModel]->m_pLod)
		return m_aModels[idModel]->m_pLod->m_sPath.c_str();
	else
		return 0;
}

const char* CModels::modelGetPath4Physics(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, 0);

	if (m_aModels[idModel]->m_pPhysics)
		return m_aModels[idModel]->m_pPhysics->m_sPath.c_str();
	else
		return 0;
}


int CModels::modelGetCountGroups(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, 0);

	return m_aModels[idModel]->m_pModel->m_uiSubsetCount + m_aModels[idModel]->m_aTransparency.size();
}

ID CModels::modelGetGroupMtrlID(ID idModel, ID idGroup)
{
	STATIC_PRECOND_MODEL_ID(idModel, -1);

	if (idGroup >= 0)
	{
		//если номер подгруппы в пределах количества подгрупп самой модели
		if (m_aModels[idModel]->m_aIDsTextures.size() > idGroup)
			return m_aModels[idModel]->m_aIDsTextures[idGroup];
		//если номер подгруппы в пределах суммы количества подгрупп модели и всех ее пп моделей
		else if (m_aModels[idModel]->m_aIDsTextures.size() + m_aModels[idModel]->m_aTransparency.size() > idGroup)
		{
			int iKey = idGroup - m_aModels[idModel]->m_aIDsTextures.size();
			return m_aTransparency[m_aModels[idModel]->m_aTransparency[iKey]]->m_idTex;
		}
		else
		{
			STATIC_ERROR_MODEL_GROUP_ID(idModel, idGroup, -1);
		}
	}
	else
	{
		STATIC_ERROR_MODEL_GROUP_ID(idModel, idGroup, -1);
	}
}

void CModels::modelGetGroupMinMax(ID idModel, ID idGroup, float3_t *pMin, float3_t *pMax)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	if (idGroup >= 0)
	{
		if (m_aModels[idModel]->m_aGroupInfo.size() > idGroup)
		{
			*pMin = m_aModels[idModel]->m_aGroupInfo[idGroup]->m_vMin;
			*pMax = m_aModels[idModel]->m_aGroupInfo[idGroup]->m_vMax;
		}
		else if (m_aModels[idModel]->m_aGroupInfo.size() + m_aModels[idModel]->m_aTransparency.size() > idGroup)
		{
			int iKey = idGroup - m_aModels[idModel]->m_aIDsTextures.size();
			*pMin = m_aTransparency[m_aModels[idModel]->m_aTransparency[iKey]]->m_oGroupInfo.m_vMin;
			*pMax = m_aTransparency[m_aModels[idModel]->m_aTransparency[iKey]]->m_oGroupInfo.m_vMax;
		}
		else
		{
			STATIC_ERROR_MODEL_GROUP_ID(idModel, idGroup, _VOID);
		}
	}
}

void CModels::modelGetGroupPlane(ID idModel, ID idGroup, D3DXPLANE *pPlane)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	if (idGroup >= 0)
	{
		if (m_aModels[idModel]->m_aGroupInfo.size() > idGroup)
			*pPlane = m_aModels[idModel]->m_aGroupInfo[idGroup]->m_oPlane;
		else if (m_aModels[idModel]->m_aGroupInfo.size() + m_aModels[idModel]->m_aTransparency.size() > idGroup)
		{
			int iKey = idGroup - m_aModels[idModel]->m_aIDsTextures.size();
			*pPlane = m_aTransparency[m_aModels[idModel]->m_aTransparency[iKey]]->m_oGroupInfo.m_oPlane;
		}
		else
		{
			STATIC_ERROR_MODEL_GROUP_ID(idModel, idGroup, _VOID);
		}
	}
}

void CModels::modelGetGroupCenter(ID idModel, ID idGroup, float3_t *pCenter)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	if (idGroup >= 0)
	{
		if (m_aModels[idModel]->m_aGroupInfo.size() > idGroup)
			*pCenter = m_aModels[idModel]->m_aGroupInfo[idGroup]->m_vMin;
		else if (m_aModels[idModel]->m_aGroupInfo.size() + m_aModels[idModel]->m_aTransparency.size() > idGroup)
		{
			int iKey = idGroup - m_aModels[idModel]->m_aIDsTextures.size();
			*pCenter = m_aTransparency[m_aModels[idModel]->m_aTransparency[iKey]]->m_oGroupInfo.m_vCenter;
		}
		else
		{
			STATIC_ERROR_MODEL_GROUP_ID(idModel, idGroup, _VOID);
		}
	}
}

bool CModels::modelGerSegmentation(ID idModel)
{
	STATIC_PRECOND_MODEL_ID(idModel, false);

	return (m_aModels[idModel]->m_pArrSplits != NULL);
}

//##########################################################################

void CModels::getArrBuffsGeom(float3_t ***pppArrVertex, int32_t	**ppArrCountVertex, uint32_t ***pppArrIndex, ID ***pppArrMtl, int32_t **ppArrCountIndex, int32_t *pCountModels)
{
	//инициализация массивов
	(*pCountModels) = m_aModels.size();
	(*pppArrVertex) = new float3_t*[m_aModels.size()];
	(*pppArrIndex) = new uint32_t*[m_aModels.size()];
	(*pppArrMtl) = new ID*[m_aModels.size()];
	(*ppArrCountVertex) = new int32_t[m_aModels.size()];
	(*ppArrCountIndex) = new int32_t[m_aModels.size()];

	//проход по всему списку моделей
	for (int i = 0; i < m_aModels.size(); ++i)
	{
		float4x4 mWorld = *(m_aModels[i]->m_pBoundVolume->calcWorld());
		//общее количество вершин/индексов (в дальнейшем может быть увеличено, т.е. + данные ппп)
		int iCountVertex = m_aModels[i]->m_pModel->m_uiAllVertexCount;
		int iCountIndex = m_aModels[i]->m_pModel->m_uiAllIndexCount;

		//порядковый номер вершины/индекса
		int iCurrVertex = 0;
		int iCurrIndex = 0;

		//если есть физическая модель, то берем данные из нее, а данные ппп не учитываем, потому что их данные должны быть в физической модели
		if (m_aModels[i]->m_pPhysics)
		{
			iCountVertex = m_aModels[i]->m_pPhysics->m_aVertex.size();
			iCountIndex = m_aModels[i]->m_pPhysics->m_aIndex.size();

			(*pppArrVertex)[i] = new float3_t[iCountVertex];
			(*pppArrIndex)[i] = new uint32_t[iCountIndex];
			(*pppArrMtl)[i] = new ID[iCountIndex];
			(*ppArrCountVertex)[i] = iCountVertex;
			(*ppArrCountIndex)[i] = iCountIndex;

			for (int k = 0; k < iCountVertex; ++k)
				(*pppArrVertex)[i][k] = SMVector3Transform(m_aModels[i]->m_pPhysics->m_aVertex[k], mWorld);

			for (int k = 0; k < iCountIndex; ++k)
				(*pppArrIndex)[i][k] = m_aModels[i]->m_pPhysics->m_aIndex[k];

			for (int k = 0; k < iCountIndex; ++k)
				(*pppArrMtl)[i][k] = m_aModels[i]->m_pPhysics->m_aMtrl[k];

			iCurrVertex = iCountVertex;
			iCurrIndex = iCountIndex;
		}
		else
		{
			//иначе берем данные из самой модели
			iCountVertex = m_aModels[i]->m_pModel->m_uiAllVertexCount;
			iCountIndex = m_aModels[i]->m_pModel->m_uiAllIndexCount;

			//прибавляем количество вершин/индексов из массива ппп моделей (если есть)
			for (int k = 0; k < m_aModels[i]->m_aTransparency.size(); ++k)
			{
				iCountVertex += m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_iCountVertex;
				iCountIndex += m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_iCountIndex;
			}

			(*pppArrVertex)[i] = new float3_t[iCountVertex];
			(*pppArrIndex)[i] = new uint32_t[iCountIndex];
			(*pppArrMtl)[i] = new ID[iCountIndex];

			(*ppArrCountVertex)[i] = iCountVertex;
			(*ppArrCountIndex)[i] = iCountIndex;

			//блокируем индексный и вершинный буферы
			UINT *pIndex = m_aModels[i]->m_pModel->m_pIndices;
			//m_aModels[i]->m_pModel->m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0);
			vertex_static_ex *pVertex = m_aModels[i]->m_pModel->m_pVertices;
			//m_aModels[i]->m_pModel->m_pVertexBuffer->Lock(0, 0, (void **)&pVertex, 0);

			//записываем позиции вершин
			for (int k = 0; k < m_aModels[i]->m_pModel->m_uiAllVertexCount; ++k)
				(*pppArrVertex)[i][k] = SMVector3Transform(pVertex[k].Pos, mWorld);

			//записываем индексы и id материала для каждого индекса
			for (int g = 0; g < m_aModels[i]->m_pModel->m_uiSubsetCount; ++g)
			{
				for (int k = 0; k < m_aModels[i]->m_pModel->m_pIndexCount[g]; ++k)
				{
					(*pppArrIndex)[i][iCurrIndex] = pIndex[m_aModels[i]->m_pModel->m_pStartIndex[g] + k];
					(*pppArrMtl)[i][iCurrIndex] = m_aModels[i]->m_aIDsTextures[g];
					++iCurrIndex;
				}
			}

			//m_aModels[i]->m_pModel->m_pVertexBuffer->Unlock();
			//m_aModels[i]->m_pModel->m_pIndexBuffer->Unlock();

			//текущей вершиной устанавливаем скопированное количество вершин
			iCurrVertex = m_aModels[i]->m_pModel->m_uiAllVertexCount;

			//количество вершин для расчета индексов
			int iCurrVertex2 = iCurrVertex;

			//проходимся по массиву с ппп
			for (int k = 0; k < m_aModels[i]->m_aTransparency.size(); ++k)
			{
				//блокируем индексный и вершинный буферы
				UINT *pIndex = m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_pIndices;
				//m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_pIndexBuffer->Lock(0, 0, (void **)&pIndex, 0);
				vertex_static_ex *pVertex = m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_pVertices;
				//m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_pVertexBuffer->Lock(0, 0, (void **)&pVertex, 0);

				//записываем позиции вершин
				for (int j = 0; j < m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_iCountVertex; ++j)
				{
					(*pppArrVertex)[i][iCurrVertex] = SMVector3Transform(pVertex[j].Pos, mWorld);
					++iCurrVertex;
				}

				//записываем индексы и id материала для каждого индекса
				for (int j = 0; j < m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_iCountIndex; ++j)
				{
					//текущий индекс рассчитывается как индекс ппп + количество вершин основной модели
					(*pppArrIndex)[i][iCurrIndex] = pIndex[j] + iCurrVertex2;
					(*pppArrMtl)[i][iCurrIndex] = m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_idTex;
					++iCurrIndex;
				}

				iCurrVertex2 = iCurrVertex;

				//m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_pVertexBuffer->Unlock();
				//m_aTransparency[m_aModels[i]->m_aTransparency[k]]->m_pIndexBuffer->Unlock();
			}
		}
	}
}

//##########################################################################

void CModels::getPartBeam(ID idModel, ID idVisCalcObj, const float3 *pStart, const float3 *pDir)
{
	STATIC_PRECOND_MODEL_ID(idModel, _VOID);

	CModel *pModel = m_aModels[idModel];

	float3 vCenter;
	float fRadius;

	int iCountIteration = 0;
	int iCountSplits = m_aVisInfo[idVisCalcObj]->m_aVisible4Model[idModel]->m_iCount;
	m_aVisInfo[idVisCalcObj]->m_aVisible4Model[idModel]->m_iCountCom = 0;
	int *iCountCom = &(m_aVisInfo[idVisCalcObj]->m_aVisible4Model[idModel]->m_iCountCom);
	CSegment **ppSegments = m_aVisInfo[idVisCalcObj]->m_aVisible4Model[idModel]->m_ppSegments;

	Array<CSegment*, GEOM_DEFAULT_RESERVE_COM> aQueue;
	aQueue.push_back(pModel->m_pArrSplits);

	while (aQueue.size() > iCountIteration)
	{
		CSegment *pSegment = aQueue[iCountIteration];

		pSegment->m_pBoundVolumeP->getSphere(&vCenter, &fRadius);

		float fDistSqr = SGCore_0DistancePointBeam2(vCenter, *pStart, *pDir);
		if (pSegment->m_uiCountAllPoly > 0 && fDistSqr <= fRadius*fRadius)
		{
			if (!(pSegment->m_isFinite))
			{
				for (int j = 0; j<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
				{
					if (pSegment->m_aSplits[j])
						aQueue.push_back(pSegment->m_aSplits[j]);
				}
			}
			else
			{
				if ((*iCountCom) < iCountSplits)
				{
					ppSegments[(*iCountCom)] = pSegment;

					++(*iCountCom);
				}
			}
		}

		++iCountIteration;
	}
}

bool CModels::traceBeam(const float3 *pStart, const float3 *pDir, float3 *pResult, ID *idModel, ID *idMtrl)
{
	if (m_aModels.size() <= 0)
		return false;

	CTriangle oTriangle;
	bool tmpiscom = true;
	float3 ip;
	float3 vResult;
	float3 il;
	vResult = (*pStart) + float3(10000.0f, 10000.0f, 10000.0f);
	il = (*pDir) * 10000.0f;
	bool isFound = false;

	float3 vMin, vMax;

	for (int id = 0; id < m_aModels.size(); ++id)
	{
		CModel *pModel = m_aModels[id];
		pModel->m_pBoundVolume->getMinMax(&vMin, &vMax);

		if (!SMBoxRayIntersection(vMin, vMax, *pStart, *pDir))
			continue;

		float4x4 mWorld = *(pModel->m_pBoundVolume->calcWorld());
		if (pModel->m_pArrSplits)
		{
			getPartBeam(id, SX_GEOM_TRACEBEAM_VISCALCOBJ, pStart, pDir);

			vertex_static_ex* pVertex = pModel->m_pModel->m_pVertices;
			//pModel->m_pModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);

			UINT *pIndex = pModel->m_pModel->m_pIndices;
			//pModel->m_pModel->m_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

			for (int k = 0; k<m_aVisInfo[1]->m_aVisible4Model[id]->m_iCountCom; ++k)
			{
				for (int g = 0; g<m_aVisInfo[1]->m_aVisible4Model[id]->m_ppSegments[k]->m_uiCountSubSet; ++g)
				{
					ID idGroup = m_aVisInfo[1]->m_aVisible4Model[id]->m_ppSegments[k]->m_pNumberGroup[g];

					for (int p = 0; p<m_aVisInfo[1]->m_aVisible4Model[id]->m_ppSegments[k]->m_pCountPoly[g] * 3; p += 3)
					{
						oTriangle.m_vA = SMVector3Transform(pVertex[m_aVisInfo[1]->m_aVisible4Model[id]->m_ppSegments[k]->m_ppArrPoly[g][p]].Pos, mWorld);
						oTriangle.m_vB = SMVector3Transform(pVertex[m_aVisInfo[1]->m_aVisible4Model[id]->m_ppSegments[k]->m_ppArrPoly[g][p + 1]].Pos, mWorld);
						oTriangle.m_vC = SMVector3Transform(pVertex[m_aVisInfo[1]->m_aVisible4Model[id]->m_ppSegments[k]->m_ppArrPoly[g][p + 2]].Pos, mWorld);

						if (oTriangle.IntersectLine((*pStart), il, &ip))
						{
							if (SMVector3Length2((*pStart) - vResult) > SMVector3Length2((*pStart) - ip))
							{
								vResult = ip;
								isFound = true;

								if (idModel)
									*idModel = id;

								if (idMtrl)
									*idMtrl = pModel->m_aIDsTextures[idGroup];
							}
						}
					}
				}
			}

			//pModel->m_pModel->m_pVertexBuffer->Unlock();
			//pModel->m_pModel->m_pIndexBuffer->Unlock();
		}
		else
		{
			vertex_static_ex* pVertex = pModel->m_pModel->m_pVertices;
			//pModel->m_pModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertex, D3DLOCK_READONLY);

			UINT *pIndex = pModel->m_pModel->m_pIndices;
			//pModel->m_pModel->m_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_READONLY);

			for (int g = 0; g < pModel->m_pModel->m_uiSubsetCount; ++g)
			{
				for (int k = 0; k<pModel->m_pModel->m_pIndexCount[g]; k+=3)
				{
					oTriangle.m_vA = SMVector3Transform(pVertex[pIndex[pModel->m_pModel->m_pStartIndex[g] + k]].Pos, mWorld);
					oTriangle.m_vB = SMVector3Transform(pVertex[pIndex[pModel->m_pModel->m_pStartIndex[g] + k + 1]].Pos, mWorld);
					oTriangle.m_vC = SMVector3Transform(pVertex[pIndex[pModel->m_pModel->m_pStartIndex[g] + k + 2]].Pos, mWorld);

					if (oTriangle.IntersectLine((*pStart), il, &ip))
					{
						if (SMVector3Length2((*pStart) - vResult) > SMVector3Length2((*pStart) - ip))
						{
							vResult = ip;
							isFound = true;

							if (idModel)
								*idModel = id;

							if (idMtrl)
								*idMtrl = m_aModels[id]->m_aIDsTextures[g];
						}
					}
				}
			}

			//pModel->m_pModel->m_pVertexBuffer->Unlock();
			//pModel->m_pModel->m_pIndexBuffer->Unlock();
		}

		vertex_static_ex* pVertex = 0;
		UINT *pIndex = 0;

		for (int i = 0; i < pModel->m_aTransparency.size(); ++i)
		{
			CTransparencyModel *pTransparencyModel = m_aTransparency[pModel->m_aTransparency[i]];

			pVertex = pTransparencyModel->m_pVertices;
			pIndex = pTransparencyModel->m_pIndices;
			//pTransparencyModel->m_pVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);
			//pTransparencyModel->m_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

			for (int k = 0; k<pTransparencyModel->m_iCountIndex; k += 3)
			{
				oTriangle.m_vA = pVertex[pIndex[k]].Pos;
				oTriangle.m_vB = pVertex[pIndex[k + 1]].Pos;
				oTriangle.m_vC = pVertex[pIndex[k + 2]].Pos;

				if (oTriangle.IntersectLine((*pStart), il, &ip))
				{
					if (SMVector3Length2((*pStart) - vResult) > SMVector3Length2((*pStart) - ip))
					{
						vResult = ip;
						isFound = true;

						if (idModel)
							*idModel = id;

						if (idMtrl)
							*idMtrl = pTransparencyModel->m_idTex;
					}
				}
			}

			//pTransparencyModel->m_pVertexBuffer->Unlock();
			//pTransparencyModel->m_pIndexBuffer->Unlock();
		}
	}

	if (isFound && pResult)
		*pResult = vResult;

	return isFound;
}