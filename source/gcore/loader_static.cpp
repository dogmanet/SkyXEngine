
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "loader_static.h"

ISXDataStaticModel* SGCore_StaticModelCr()
{
	return new DataStaticModel();
}

IDirect3DVertexDeclaration9* SGCore_StaticModelGetDecl()
{
	return g_pStaticVertexDecl;
}

bool SGCore_DSE_IsDSE(const char *szPath)
{
	FILE * pf = fopen(szPath, "rb");
	if (!pf)
		return false;

	ModelHeader header;

	fread(&header, sizeof(ModelHeader), 1, pf);
	fclose(pf);

	if (header.Magick != SX_MODEL_MAGICK)
		return false;

	if (header.iVersion != SX_MODEL_VERSION)
		return false;

	return true;
}

SX_LIB_API bool SGCore_DSE_IsModel(const char *szPath)
{
	FILE * pf = fopen(szPath, "rb");
	if (!pf)
		return false;

	ModelHeader header;

	fread(&header, sizeof(ModelHeader), 1, pf);
	fclose(pf);

	if (header.Magick != SX_MODEL_MAGICK)
		return false;

	if (header.iVersion != SX_MODEL_VERSION)
		return false;

	return !(header.iFlags & MODEL_FLAG_SOURCE_ANIMATION);
}


SX_LIB_API bool SGCore_DSE_IsAnimation(const char *szPath)
{
	FILE * pf = fopen(szPath, "rb");
	if (!pf)
		return false;

	ModelHeader header;

	fread(&header, sizeof(ModelHeader), 1, pf);
	fclose(pf);

	if (header.Magick != SX_MODEL_MAGICK)
		return false;

	if (header.iVersion != SX_MODEL_VERSION)
		return false;

	return (header.iFlags & MODEL_FLAG_SOURCE_ANIMATION);
}

SX_LIB_API bool SGCore_DSEgetInfo(const char *szPath, CDSEinfo *pInfo)
{
	FILE * pf = fopen(szPath, "rb");
	if (!pf)
		return false;

	ModelHeader header;

	fread(&header, sizeof(ModelHeader), 1, pf);

	if (header.Magick != SX_MODEL_MAGICK)
	{
		fclose(pf);
		return false;
	}

	if (header.iVersion != SX_MODEL_VERSION)
	{
		fclose(pf);
		return false;
	}

	pInfo->iVersion = header.iVersion;
	pInfo->iCountSkin = header.iSkinCount;
	pInfo->iCountSubsets = header.iMaterialCount;
	pInfo->iCountBone = header.iBoneCount;
	pInfo->iCountAnimation = header.iAnimationCount;

	fseek(pf, header.iSecondHeaderOffset, SEEK_SET);

	ModelHeader2 header2;
	fread(&header2, sizeof(ModelHeader2), 1, pf);

	pInfo->iCountHitbox = header2.iHitboxCount;

	//ÂÒÎË Ù‡ÈÎ ˝ÚÓ ‡ÌËÏ‡ˆËˇ
	if (header.iFlags & MODEL_FLAG_SOURCE_ANIMATION)
	{
		fclose(pf);
		pInfo->type = DSE_TYPE_ANIMATION;
		return true;
	}


	if (header.iFlags & MODEL_FLAG_STATIC)
		pInfo->type = DSE_TYPE_STATIC;
	else if (header.iFlags & MODEL_FLAG_COMPILED || header.iFlags & MODEL_FLAG_SOURCE_MESH)
		pInfo->type = DSE_TYPE_ANIM_MESH;
	else
	{
		// ˜ÚÓ ÚÓ„‰‡?
	}


	
	if (header.iMaterialsOffset)
	{
		fseek(pf, header.iMaterialsOffset, SEEK_SET);
		fseek(pf, header.iMaterialCount * MODEL_MAX_NAME, SEEK_CUR);
	}

	ModelLoD *m_pLods;
	if (header.iLODcount && header.iLODoffset)
	{

		fseek(pf, header.iLODoffset, SEEK_SET);

		m_pLods = new ModelLoD[header.iLODcount];

		for (int i = 0; i < header.iLODcount; i++)
		{
			fread(&m_pLods[i], MODEL_LOD_STRUCT_SIZE, 1, pf);
			int iVC = 0;
			int iIC = 0;
			float3 vMin, vMax;

			m_pLods[i].pSubLODmeshes = new ModelLoDSubset[m_pLods[i].iSubMeshCount];
			for (int j = 0; j < m_pLods[i].iSubMeshCount; j++)
			{

				fread(&m_pLods[i].pSubLODmeshes[j].iMaterialID, sizeof(int), 1, pf);
				fread(&m_pLods[i].pSubLODmeshes[j].iVectexCount, sizeof(int), 1, pf);
				fread(&m_pLods[i].pSubLODmeshes[j].iIndexCount, sizeof(int), 1, pf);

				if(header.iFlags & MODEL_FLAG_STATIC)
				{
					if(header.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM)
					{
						m_pLods[i].pSubLODmeshes[j].pVertices = new vertex_static_ex[m_pLods[i].pSubLODmeshes[j].iVectexCount];
					}
					else
					{
						m_pLods[i].pSubLODmeshes[j].pVertices = new vertex_static_ex[m_pLods[i].pSubLODmeshes[j].iVectexCount];
					}
				}
				else
				{
					if(header.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM)
					{
						m_pLods[i].pSubLODmeshes[j].pVertices = new vertex_animated_ex[m_pLods[i].pSubLODmeshes[j].iVectexCount];
					}
					else
					{
						m_pLods[i].pSubLODmeshes[j].pVertices = new vertex_animated[m_pLods[i].pSubLODmeshes[j].iVectexCount];
					}
				}

				m_pLods[i].pSubLODmeshes[j].pIndices = new UINT[m_pLods[i].pSubLODmeshes[j].iIndexCount];


				int iSizeVertex;

				if(header.iFlags & MODEL_FLAG_STATIC)
				{
					iSizeVertex = (header.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM) ? sizeof(vertex_static_ex) : sizeof(vertex_static);
					fread(m_pLods[i].pSubLODmeshes[j].pVertices, iSizeVertex, m_pLods[i].pSubLODmeshes[j].iVectexCount, pf);
				}
				else
				{
					iSizeVertex = (header.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM) ? sizeof(vertex_animated_ex) : sizeof(vertex_animated);
					fread(m_pLods[i].pSubLODmeshes[j].pVertices, iSizeVertex, m_pLods[i].pSubLODmeshes[j].iVectexCount, pf);
				}

				fread(m_pLods[i].pSubLODmeshes[j].pIndices, sizeof(UINT), m_pLods[i].pSubLODmeshes[j].iIndexCount, pf);

				iVC += m_pLods[i].pSubLODmeshes[j].iVectexCount;
				iIC += m_pLods[i].pSubLODmeshes[j].iIndexCount;


				if (j == 0)
				{
					vMin = *(float3_t*)(m_pLods[i].pSubLODmeshes[j].pVertices);
					vMax = *(float3_t*)(m_pLods[i].pSubLODmeshes[j].pVertices);
				}

				float3_t vPos;

				for (int k = 0; k < m_pLods[i].pSubLODmeshes[j].iVectexCount; ++k)
				{
					vPos = *(float3_t*)(m_pLods[i].pSubLODmeshes[j].pVertices + k*iSizeVertex);
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
			}

			if (i == 0)
			{
				pInfo->iCountVertex = iVC;
				pInfo->iCountIndex = iIC;
				pInfo->vDimensions = float3_t(vMax - vMin);
				pInfo->vCenter = float3_t((vMax + vMin) * 0.5f);
			}
		}
	}

	for (int i = 0; i < header.iLODcount; i++)
	{
		for (int j = 0; j < m_pLods[i].iSubMeshCount; j++)
		{
			mem_delete_a(m_pLods[i].pSubLODmeshes[j].pVertices);
			mem_delete_a(m_pLods[i].pSubLODmeshes[j].pIndices);
		}
		mem_delete_a(m_pLods[i].pSubLODmeshes);
	}
	mem_delete(m_pLods);

	fclose(pf);

	return true;
}

void SGCore_StaticModelLoad(const char * file, ISXDataStaticModel** data)
{
	if(!data)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - Òan not initialize a null pointer 'data', load model '%s'\n", GEN_MSG_LOCATION, file);
		return;
	}

	(*data) = new DataStaticModel();
	FILE * pf = fopen(file, "rb");
	if(!pf)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unable to open model file '%s'\n", GEN_MSG_LOCATION, file);
		return;
	}

	ModelHeader header;

	fread(&header, sizeof(ModelHeader), 1, pf);

	if(header.Magick != SX_MODEL_MAGICK)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unsupported file type '%s'\n", GEN_MSG_LOCATION, file);
		fclose(pf);
		return;
	}

	if(header.iVersion != SX_MODEL_VERSION)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unsupported file '%s' version %d'\n", GEN_MSG_LOCATION, header.iVersion, file);
		fclose(pf);
		return;
	}


	(*data)->m_ppTextures = NULL;
	Array<String> tex;
	if(header.iMaterialsOffset)
	{
		fseek(pf, header.iMaterialsOffset, SEEK_SET);
		(*data)->m_ppTextures = new char*[header.iMaterialCount];
		for(int i = 0; i < header.iMaterialCount; i++)
		{
			char c[MODEL_MAX_NAME];
			tex[i] = "";
			fread(c, 1, MODEL_MAX_NAME, pf);
			tex[i] = c;
			UINT pos = tex[i].find_last_of('/');
			tex[i] = tex[i].substr(pos + 1);
		}
	}
	ModelLoD *m_pLods;
	if(header.iLODcount && header.iLODoffset)
	{

		fseek(pf, header.iLODoffset, SEEK_SET);

		m_pLods = new ModelLoD[header.iLODcount];

		for(int i = 0; i < header.iLODcount; i++)
		{
			fread(&m_pLods[i], MODEL_LOD_STRUCT_SIZE, 1, pf);
			int iVC = 0;
			m_pLods[i].pSubLODmeshes = new ModelLoDSubset[m_pLods[i].iSubMeshCount];
			for(int j = 0; j < m_pLods[i].iSubMeshCount; j++)
			{

				fread(&m_pLods[i].pSubLODmeshes[j].iMaterialID, sizeof(int), 1, pf);
				fread(&m_pLods[i].pSubLODmeshes[j].iVectexCount, sizeof(int), 1, pf);
				fread(&m_pLods[i].pSubLODmeshes[j].iIndexCount, sizeof(int), 1, pf);
				m_pLods[i].pSubLODmeshes[j].pVertices = new vertex_static_ex[m_pLods[i].pSubLODmeshes[j].iVectexCount];
				if(header.iFlags & MODEL_FLAG_HAS_TANGENT_BINORM)
				{
					fread(m_pLods[i].pSubLODmeshes[j].pVertices, sizeof(vertex_static_ex), m_pLods[i].pSubLODmeshes[j].iVectexCount, pf);
				}
				else
				{
					vertex_static *pSource = new vertex_static[m_pLods[i].pSubLODmeshes[j].iVectexCount];
					fread(pSource, sizeof(vertex_static), m_pLods[i].pSubLODmeshes[j].iVectexCount, pf);
					vertex_static_ex *pTarget = (vertex_static_ex*)m_pLods[i].pSubLODmeshes[j].pVertices;
					for(int vi = 0; vi < m_pLods[i].pSubLODmeshes[j].iVectexCount; ++vi)
					{
						memcpy(&(pTarget[vi]), &(pSource[vi]), sizeof(vertex_static));
					}
					delete[] pSource;
				}
				m_pLods[i].pSubLODmeshes[j].pIndices = new UINT[m_pLods[i].pSubLODmeshes[j].iIndexCount];
				fread(m_pLods[i].pSubLODmeshes[j].pIndices, sizeof(UINT), m_pLods[i].pSubLODmeshes[j].iIndexCount, pf);

				iVC += m_pLods[i].pSubLODmeshes[j].iVectexCount;
			}
		}
	}

	(*data)->m_uiSubsetCount = m_pLods[0].iSubMeshCount;
	(*data)->m_pStartIndex = new UINT[m_pLods[0].iSubMeshCount];
	(*data)->m_pIndexCount = new UINT[m_pLods[0].iSubMeshCount];
	(*data)->m_pStartVertex = new UINT[m_pLods[0].iSubMeshCount];
	(*data)->m_pVertexCount = new UINT[m_pLods[0].iSubMeshCount];

	UINT iStartIndex = 0;
	UINT iStartVertex = 0;
	ModelLoD lh = m_pLods[0];

	for(int i = 0; i < lh.iSubMeshCount; i++)
	{
		(*data)->m_pStartIndex[i] = iStartIndex;
		(*data)->m_pStartVertex[i] = iStartVertex;
		(*data)->m_pIndexCount[i] = lh.pSubLODmeshes[i].iIndexCount;
		(*data)->m_pVertexCount[i] = lh.pSubLODmeshes[i].iVectexCount;
		lh.pSubLODmeshes[i].iStartIndex = iStartIndex;
		lh.pSubLODmeshes[i].iStartVertex = iStartVertex;

		for(int j = 0; j < lh.pSubLODmeshes[i].iIndexCount; j++)
		{
			lh.pSubLODmeshes[i].pIndices[j] += iStartVertex;
		}

		iStartIndex += lh.pSubLODmeshes[i].iIndexCount;
		iStartVertex += lh.pSubLODmeshes[i].iVectexCount;

		(*data)->m_ppTextures[i] = new char[tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1];
		memcpy((*data)->m_ppTextures[i], tex[lh.pSubLODmeshes[i].iMaterialID].c_str(), (tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1) * sizeof(char));
	}

	UINT * pIndices = new UINT[iStartIndex];
	vertex_static_ex * pVertices = new vertex_static_ex[iStartVertex];

	for(int i = 0; i < lh.iSubMeshCount; i++)
	{
		memcpy(pIndices + lh.pSubLODmeshes[i].iStartIndex, lh.pSubLODmeshes[i].pIndices, sizeof(UINT)* lh.pSubLODmeshes[i].iIndexCount);
		memcpy(pVertices + lh.pSubLODmeshes[i].iStartVertex, lh.pSubLODmeshes[i].pVertices, sizeof(vertex_static_ex)* lh.pSubLODmeshes[i].iVectexCount);
	}

	(*data)->m_uiAllVertexCount = iStartVertex;
	(*data)->m_pVertices = pVertices;
	//g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static_ex)* iStartVertex, NULL, NULL, D3DPOOL_MANAGED, &(*data)->m_pVertexBuffer, 0);
	////(*data)->ArrVertBuf = new vertex_static_ex[iStartVertex];
	//vertex_static_ex * pData;
	//if(!FAILED((*data)->m_pVertexBuffer->Lock(0, sizeof(vertex_static_ex)* iStartVertex, (void**)&pData, 0)))
	//{
	//	memcpy(pData, pVertices, sizeof(vertex_static_ex)* iStartVertex);
	//	//memcpy((*data)->ArrVertBuf, pVertices, sizeof(vertex_static_ex)* iStartVertex);

	//	(*data)->m_pVertexBuffer->Unlock();
	//}

	(*data)->m_uiAllIndexCount = iStartIndex;
	(*data)->m_pIndices = pIndices;
	//g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* iStartIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &(*data)->m_pIndexBuffer, 0);
	////(*data)->ArrIndBuf = new UINT[iStartIndex];
	//if(!FAILED((*data)->m_pIndexBuffer->Lock(0, sizeof(UINT)* iStartIndex, (void**)&pData, 0)))
	//{
	//	memcpy(pData, pIndices, sizeof(UINT)* iStartIndex);
	//	//memcpy((*data)->ArrIndBuf, pIndices, sizeof(UINT)* iStartIndex);
	//	(*data)->m_pIndexBuffer->Unlock();
	//}

	//(*data)->syncBuffers();

	for(int i = 0; i < header.iLODcount; i++)
	{
		for(int j = 0; j < m_pLods[i].iSubMeshCount; j++)
		{
			mem_delete_a(m_pLods[i].pSubLODmeshes[j].pVertices);
			mem_delete_a(m_pLods[i].pSubLODmeshes[j].pIndices);
		}
		mem_delete_a(m_pLods[i].pSubLODmeshes);
	}
	mem_delete(m_pLods);

	//mem_delete_a(pVertices);
	//mem_delete_a(pIndices);

	fclose(pf);

	//”Õ»‘» ¿÷»ﬂ œŒƒ√–”œœ
	//{{
	DataStaticModel* tmpmodel = new DataStaticModel();
	tmpmodel->m_uiSubsetCount = 0;
	tmpmodel->m_ppTextures = new char*[(*data)->m_uiSubsetCount];
	memset(tmpmodel->m_ppTextures, 0, (*data)->m_uiSubsetCount * sizeof(char));

	vertex_static_ex* ArrVertBuf = new vertex_static_ex[(*data)->m_uiAllVertexCount];
	UINT* ArrIndBuf = new UINT[(*data)->m_uiAllIndexCount];

	tmpmodel->m_pStartIndex = new UINT[(*data)->m_uiSubsetCount];
	tmpmodel->m_pStartVertex = new UINT[(*data)->m_uiSubsetCount];
	tmpmodel->m_pIndexCount = new UINT[(*data)->m_uiSubsetCount];
	tmpmodel->m_pVertexCount = new UINT[(*data)->m_uiSubsetCount];

	long startindex = 0;
	long startvertex = 0;
	long countindex = 0;
	long countvertex = 0;
	//long countsubset = 0;

	vertex_static_ex* pVert;
	UINT* pInd;
	pVert = (*data)->m_pVertices;
	//(*data)->m_pVertexBuffer->Lock(0, 0, (void**)&pVert, 0);

	pInd = (*data)->m_pIndices;
	//(*data)->m_pIndexBuffer->Lock(0, 0, (void**)&pInd, 0);

	for(long i = 0; i < (*data)->m_uiSubsetCount; ++i)
	{
		if((*data)->m_ppTextures[i][0] == 0)
			continue;

		tmpmodel->m_ppTextures[tmpmodel->m_uiSubsetCount] = new char[strlen((*data)->m_ppTextures[i]) + 1];
		strcpy(tmpmodel->m_ppTextures[tmpmodel->m_uiSubsetCount], (*data)->m_ppTextures[i]);

		startvertex = countvertex;
		startindex = countindex;

		tmpmodel->m_pStartIndex[tmpmodel->m_uiSubsetCount] = startindex;
		tmpmodel->m_pStartVertex[tmpmodel->m_uiSubsetCount] = startvertex;

		memcpy(ArrVertBuf + startvertex, pVert + (*data)->m_pStartVertex[i], sizeof(vertex_static_ex)* (*data)->m_pVertexCount[i]);
		memcpy(ArrIndBuf + startindex, pInd + (*data)->m_pStartIndex[i], sizeof(UINT)* (*data)->m_pIndexCount[i]);

		for(long j = 0; j < (*data)->m_pIndexCount[i]; ++j)
		{
			ArrIndBuf[countindex + j] = countvertex + (pInd[(*data)->m_pStartIndex[i] + j] - (*data)->m_pStartVertex[i]);
		}

		countvertex += (*data)->m_pVertexCount[i];
		countindex += (*data)->m_pIndexCount[i];

		for(long k = i + 1; k < (*data)->m_uiSubsetCount; ++k)
		{
			if(strcmp((*data)->m_ppTextures[i], (*data)->m_ppTextures[k]) == 0)
			{
				(*data)->m_ppTextures[k][0] = 0;
				memcpy(ArrVertBuf + countvertex, pVert + (*data)->m_pStartVertex[k], sizeof(vertex_static_ex)* (*data)->m_pVertexCount[k]);

				memcpy(ArrIndBuf + countindex, pInd + (*data)->m_pStartIndex[k], sizeof(UINT)* (*data)->m_pIndexCount[k]);

				for(long j = 0; j < (*data)->m_pIndexCount[k]; ++j)
				{
					ArrIndBuf[countindex + j] = countvertex + (pInd[(*data)->m_pStartIndex[k] + j] - (*data)->m_pStartVertex[k]);
				}

				countvertex += (*data)->m_pVertexCount[k];
				countindex += (*data)->m_pIndexCount[k];
			}
		}

		tmpmodel->m_pVertexCount[tmpmodel->m_uiSubsetCount] = countvertex - startvertex;
		tmpmodel->m_pIndexCount[tmpmodel->m_uiSubsetCount] = countindex - startindex;

		++tmpmodel->m_uiSubsetCount;
	}

	//(*data)->m_pVertexBuffer->Unlock();
	//(*data)->m_pIndexBuffer->Unlock();

	tmpmodel->m_uiAllIndexCount = countindex;
	tmpmodel->m_uiAllVertexCount = countvertex;

	tmpmodel->m_pVertices = new vertex_static_ex[countvertex];
	//g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static_ex)* countvertex, NULL, NULL, D3DPOOL_MANAGED, &tmpmodel->m_pVertexBuffer, 0);

	//vertex_static_ex * pData;
	//if(!FAILED(tmpmodel->m_pVertexBuffer->Lock(0, sizeof(vertex_static_ex)* countvertex, (void**)&pData, 0)))
	{
		memcpy(tmpmodel->m_pVertices, ArrVertBuf, sizeof(vertex_static_ex)* countvertex);

		float3_t tmppos = tmpmodel->m_pVertices[0].Pos;
		tmpmodel->m_vBBMax = tmppos;
		tmpmodel->m_vBBMin = tmppos;
		float3_t pos;

		for(long i = 0; i<countvertex; i++)
		{
			pos = tmpmodel->m_pVertices[i].Pos;

			if(pos.x > tmpmodel->m_vBBMax.x)
				tmpmodel->m_vBBMax.x = pos.x;

			if(pos.y > tmpmodel->m_vBBMax.y)
				tmpmodel->m_vBBMax.y = pos.y;

			if(pos.z > tmpmodel->m_vBBMax.z)
				tmpmodel->m_vBBMax.z = pos.z;


			if(pos.x < tmpmodel->m_vBBMin.x)
				tmpmodel->m_vBBMin.x = pos.x;

			if(pos.y < tmpmodel->m_vBBMin.y)
				tmpmodel->m_vBBMin.y = pos.y;

			if(pos.z < tmpmodel->m_vBBMin.z)
				tmpmodel->m_vBBMin.z = pos.z;
		}

		float3 Center = (tmpmodel->m_vBBMin + tmpmodel->m_vBBMax) * 0.5f;
		tmpmodel->m_vBSphere.x = Center.x;
		tmpmodel->m_vBSphere.y = Center.y;
		tmpmodel->m_vBSphere.z = Center.z;
		tmpmodel->m_vBSphere.w = SMVector3Length(Center - tmpmodel->m_vBBMax);

		//tmpmodel->m_pVertexBuffer->Unlock();
	}

	tmpmodel->m_pIndices = ArrIndBuf;
	//g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* countindex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &tmpmodel->m_pIndexBuffer, 0);

	/*if(!FAILED(tmpmodel->m_pIndexBuffer->Lock(0, sizeof(UINT)* countindex, (void**)&pData, 0)))
	{
		memcpy(pData, ArrIndBuf, sizeof(UINT)* countindex);
		tmpmodel->m_pIndexBuffer->Unlock();
	}*/

	mem_release_del((*data));
	mem_delete_a(ArrVertBuf);
	//mem_delete_a(ArrIndBuf);

	tmpmodel->syncBuffers();

	(*data) = tmpmodel;
	//}

	//Core::InLog("mesh is loaded [%s]\n", file);
}

/*void SGCore_StaticModelSave(const char * file, DataStaticModel** data)
{
	ModelHeader hdr;
	memset(&hdr, 0, sizeof(hdr));

	FILE * pF = fopen(file, "wb");

	if (!pF)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "unable to open model file '%s'\n", file);
		return;
	}

	hdr.Magick = SX_MODEL_MAGICK;
	hdr.iBoneCount = 1;
	hdr.iFlags = MODEL_FLAG_COMPILED | MODEL_FLAG_STATIC;

	hdr.iMaterialCount = (*data)->m_uiSubsetCount;
	hdr.iSkinCount = 1;
	hdr.iVersion = SX_MODEL_VERSION_OLD;

	hdr.iMaterialsOffset = sizeof(hdr);

	UINT iMaterialsSize = 0;
	for (UINT i = 0; i < hdr.iMaterialCount; i++)
	{
		iMaterialsSize += strlen((*data)->m_ppTextures[i]) + 1;
	}

	hdr.iBonesOffset = 0;

	hdr.iLODoffset = hdr.iMaterialsOffset + iMaterialsSize;

	UINT iLodSize = 0;
	iLodSize += sizeof(int);
	for (int i = 0; i < hdr.iMaterialCount; i++)
	{
		iLodSize += sizeof(vertex_animated_ex)* (*data)->m_pVertexCount[i];
		iLodSize += sizeof(UINT)* (*data)->m_pIndexCount[i];
		iLodSize += sizeof(int)* 3;
	}

	hdr.iAnimationsOffset = 0;
	hdr.iSecondHeaderOffset = 0;

	fwrite(&hdr, sizeof(hdr), 1, pF);
	for (UINT i = 0; i < hdr.iMaterialCount; i++)
	{
		fwrite((*data)->m_ppTextures[i], 1, strlen((*data)->m_ppTextures[i]) + 1, pF);
	}
	int smc = hdr.iMaterialCount;
	fwrite(&smc, sizeof(int), 1, pF);
	vertex_animated_ex * pVB;
	UINT * pIB;
	(*data)->m_pVertexBuffer->Lock(0, 0, (void**)&pVB, 0);
	(*data)->m_pIndexBuffer->Lock(0, 0, (void**)&pIB, 0);
	UINT iVC = 0;
	UINT iIC = 0;
	for (int i = 0; i < hdr.iMaterialCount; i++)
	{
		fwrite(&i, sizeof(int), 1, pF);
		fwrite(&(*data)->m_pVertexCount[i], sizeof(int), 1, pF);
		fwrite(&(*data)->m_pIndexCount[i], sizeof(int), 1, pF);

		fwrite(pVB + iVC, sizeof(vertex_animated_ex), (*data)->m_pVertexCount[i], pF);
		fwrite(pIB + iIC, sizeof(UINT), (*data)->m_pIndexCount[i], pF);
		iVC += (*data)->m_pVertexCount[i];
		iIC += (*data)->m_pIndexCount[i];
	}
	(*data)->m_pVertexBuffer->Unlock();
	(*data)->m_pIndexBuffer->Unlock();
	fclose(pF);
}
*/