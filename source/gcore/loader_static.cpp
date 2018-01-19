
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

void SGCore_StaticModelLoad(const char * file, ISXDataStaticModel** data)
{
	if (!data)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - сan not initialize a null pointer 'data', load model '%s'\n", GEN_MSG_LOCATION, file);
		return;
	}

	(*data) = new DataStaticModel();
	FILE * pf = fopen(file, "rb");
	if (!pf)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unable to open model file '%s'\n", GEN_MSG_LOCATION, file);
		return;
	}

	ModelHeader header;

	fread(&header, sizeof(ModelHeader), 1, pf);

	if (header.Magick != SX_MODEL_MAGICK)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unsupported file type '%s'\n", GEN_MSG_LOCATION, file);
		fclose(pf);
		return;
	}

	if (!(header.iVersion == SX_MODEL_VERSION_OLD || header.iVersion == SX_MODEL_VERSION))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unsupported file '%s' version %d'\n", GEN_MSG_LOCATION, header.iVersion, file);
		fclose(pf);
		return;
	}

	if (header.iVersion == SX_MODEL_VERSION_OLD)
	{
		(*data)->m_ppTextures = NULL;
		Array<String> tex;
		if (header.iMaterialsOffset)
		{
			fseek(pf, header.iMaterialsOffset, SEEK_SET);
			(*data)->m_ppTextures = new char*[header.iMaterialCount];
			for (int i = 0; i < header.iMaterialCount; i++)
			{
				char c;
				tex[i] = "";
				while (c = getc(pf))
				{
					tex[i] += c;
				}
				UINT pos = tex[i].find_last_of('/');
				tex[i] = tex[i].substr(pos + 1);
			}
		}

		if (header.iFlags & (MODEL_FLAG_SOURCE_MESH | MODEL_FLAG_COMPILED))
		{
			ModelLoD lh;

			fread(&lh.iSubMeshCount, sizeof(int), 1, pf);
			lh.pSubLODmeshes = new ModelLoDSubset[lh.iSubMeshCount];
			(*data)->m_uiSubsetCount = lh.iSubMeshCount;
			(*data)->m_pStartIndex = new UINT[lh.iSubMeshCount];
			(*data)->m_pIndexCount = new UINT[lh.iSubMeshCount];
			(*data)->m_pStartVertex = new UINT[lh.iSubMeshCount];
			(*data)->m_pVertexCount = new UINT[lh.iSubMeshCount];
			int iVC = 0;
			for (int i = 0; i < lh.iSubMeshCount; i++)
			{
				fread(&lh.pSubLODmeshes[i].iMaterialID, sizeof(int), 1, pf);
				fread(&lh.pSubLODmeshes[i].iVectexCount, sizeof(int), 1, pf);
				fread(&lh.pSubLODmeshes[i].iIndexCount, sizeof(int), 1, pf);
				lh.pSubLODmeshes[i].pVertices = new vertex_animated_ex[lh.pSubLODmeshes[i].iVectexCount];
				lh.pSubLODmeshes[i].pIndices = new UINT[lh.pSubLODmeshes[i].iIndexCount];
				fread(lh.pSubLODmeshes[i].pVertices, sizeof(vertex_animated_ex), lh.pSubLODmeshes[i].iVectexCount, pf);
				fread(lh.pSubLODmeshes[i].pIndices, sizeof(UINT), lh.pSubLODmeshes[i].iIndexCount, pf);

				iVC += lh.pSubLODmeshes[i].iVectexCount;

				(*data)->m_ppTextures[i] = new char[tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1];
				memcpy((*data)->m_ppTextures[i], tex[lh.pSubLODmeshes[i].iMaterialID].c_str(), (tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1) * sizeof(char));
			}

			UINT iStartIndex = 0;
			UINT iStartVertex = 0;

			for (int i = 0; i < lh.iSubMeshCount; i++)
			{
				(*data)->m_pStartIndex[i] = iStartIndex;
				(*data)->m_pStartVertex[i] = iStartVertex;
				(*data)->m_pIndexCount[i] = lh.pSubLODmeshes[i].iIndexCount;
				(*data)->m_pVertexCount[i] = lh.pSubLODmeshes[i].iVectexCount;
				lh.pSubLODmeshes[i].iStartIndex = iStartIndex;
				lh.pSubLODmeshes[i].iStartVertex = iStartVertex;

				for (int j = 0; j < lh.pSubLODmeshes[i].iIndexCount; j++)
				{
					lh.pSubLODmeshes[i].pIndices[j] += iStartIndex;
					//Core::InLog("%d | %d\n",i,lh.pSubLODmeshes[i].pIndices[j]);
				}

				iStartIndex += lh.pSubLODmeshes[i].iIndexCount;
				iStartVertex += lh.pSubLODmeshes[i].iVectexCount;
			}

			UINT * pIndices = new UINT[iStartIndex];
			vertex_animated_ex * pVertices = new vertex_animated_ex[iStartVertex];
			ModelLoDSubset * pSM;
			for (int i = 0; i < lh.iSubMeshCount; i++)
			{
				pSM = &lh.pSubLODmeshes[i];
				memcpy(pIndices + lh.pSubLODmeshes[i].iStartIndex, lh.pSubLODmeshes[i].pIndices, sizeof(UINT)* lh.pSubLODmeshes[i].iIndexCount);
				memcpy(pVertices + lh.pSubLODmeshes[i].iStartVertex, lh.pSubLODmeshes[i].pVertices, sizeof(vertex_animated_ex)* lh.pSubLODmeshes[i].iVectexCount);
			}

			(*data)->m_uiAllVertexCount = iStartVertex;
			g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* iStartVertex, NULL, NULL, D3DPOOL_MANAGED, &(*data)->m_pVertexBuffer, 0);
			//(*data)->ArrVertBuf = new vertex_static[iStartVertex];
			vertex_static * pData;
			if (!FAILED((*data)->m_pVertexBuffer->Lock(0, sizeof(vertex_static)* iStartVertex, (void**)&pData, 0)))
			{
				for (DWORD i = 0; i < lh.iSubMeshCount; i++)
				{
					for (DWORD k = 0; k < lh.pSubLODmeshes[i].iVectexCount; k++)
					{
						memcpy(pData + lh.pSubLODmeshes[i].iStartVertex + k, pVertices + lh.pSubLODmeshes[i].iStartVertex + k, sizeof(vertex_static));
						//memcpy((*data)->ArrVertBuf + lh.pSubLODmeshes[i].iStartVertex + k, pVertices + lh.pSubLODmeshes[i].iStartVertex + k, sizeof(vertex_static));
					}
				}
				(*data)->m_pVertexBuffer->Unlock();
			}

			(*data)->m_uiAllIndexCount = iStartIndex;
			DWORD tmpCountIndecex = 0;
			g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* iStartIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &(*data)->m_pIndexBuffer, 0);
			//(*data)->ArrIndBuf = new UINT[iStartIndex];
			if (!FAILED((*data)->m_pIndexBuffer->Lock(0, sizeof(UINT)* iStartIndex, (void**)&pData, 0)))
			{
				memcpy(pData, pIndices, sizeof(UINT)* iStartIndex);
				//memcpy((*data)->ArrIndBuf, pIndices, sizeof(UINT)* iStartIndex);
				(*data)->m_pIndexBuffer->Unlock();
			}

			for (int i = 0; i < lh.iSubMeshCount; i++)
			{
				mem_delete_a(lh.pSubLODmeshes[i].pVertices);
				mem_delete_a(lh.pSubLODmeshes[i].pIndices);
			}

			mem_delete_a(lh.pSubLODmeshes);
			mem_delete_a(pVertices);
			mem_delete_a(pIndices);
		}
	}
	else if (header.iVersion == SX_MODEL_VERSION)
	{
		(*data)->m_ppTextures = NULL;
		Array<String> tex;
		if (header.iMaterialsOffset)
		{
			fseek(pf, header.iMaterialsOffset, SEEK_SET);
			(*data)->m_ppTextures = new char*[header.iMaterialCount];
			for (int i = 0; i < header.iMaterialCount; i++)
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
		if (header.iLODcount && header.iLODoffset)
		{

			fseek(pf, header.iLODoffset, SEEK_SET);

			m_pLods = new ModelLoD[header.iLODcount];

			for (int i = 0; i < header.iLODcount; i++)
			{
				fread(&m_pLods[i], MODEL_LOD_STRUCT_SIZE, 1, pf);
				int iVC = 0;
				m_pLods[i].pSubLODmeshes = new ModelLoDSubset[m_pLods[i].iSubMeshCount];
				for (int j = 0; j < m_pLods[i].iSubMeshCount; j++)
				{

					fread(&m_pLods[i].pSubLODmeshes[j].iMaterialID, sizeof(int), 1, pf);
					fread(&m_pLods[i].pSubLODmeshes[j].iVectexCount, sizeof(int), 1, pf);
					fread(&m_pLods[i].pSubLODmeshes[j].iIndexCount, sizeof(int), 1, pf);
					m_pLods[i].pSubLODmeshes[j].pVertices = new vertex_static[m_pLods[i].pSubLODmeshes[j].iVectexCount];
					m_pLods[i].pSubLODmeshes[j].pIndices = new UINT[m_pLods[i].pSubLODmeshes[j].iIndexCount];
					fread(m_pLods[i].pSubLODmeshes[j].pVertices, sizeof(vertex_static), m_pLods[i].pSubLODmeshes[j].iVectexCount, pf);
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

		for (int i = 0; i < lh.iSubMeshCount; i++)
		{
			(*data)->m_pStartIndex[i] = iStartIndex;
			(*data)->m_pStartVertex[i] = iStartVertex;
			(*data)->m_pIndexCount[i] = lh.pSubLODmeshes[i].iIndexCount;
			(*data)->m_pVertexCount[i] = lh.pSubLODmeshes[i].iVectexCount;
			lh.pSubLODmeshes[i].iStartIndex = iStartIndex;
			lh.pSubLODmeshes[i].iStartVertex = iStartVertex;

			for (int j = 0; j < lh.pSubLODmeshes[i].iIndexCount; j++)
			{
				lh.pSubLODmeshes[i].pIndices[j] += iStartVertex;
			}

			iStartIndex += lh.pSubLODmeshes[i].iIndexCount;
			iStartVertex += lh.pSubLODmeshes[i].iVectexCount;

			(*data)->m_ppTextures[i] = new char[tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1];
			memcpy((*data)->m_ppTextures[i], tex[lh.pSubLODmeshes[i].iMaterialID].c_str(), (tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1) * sizeof(char));
		}

		UINT * pIndices = new UINT[iStartIndex];
		vertex_static * pVertices = new vertex_static[iStartVertex];

		for (int i = 0; i < lh.iSubMeshCount; i++)
		{
			memcpy(pIndices + lh.pSubLODmeshes[i].iStartIndex, lh.pSubLODmeshes[i].pIndices, sizeof(UINT)* lh.pSubLODmeshes[i].iIndexCount);
			memcpy(pVertices + lh.pSubLODmeshes[i].iStartVertex, lh.pSubLODmeshes[i].pVertices, sizeof(vertex_static)* lh.pSubLODmeshes[i].iVectexCount);
		}

		(*data)->m_uiAllVertexCount = iStartVertex;
		g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* iStartVertex, NULL, NULL, D3DPOOL_MANAGED, &(*data)->m_pVertexBuffer, 0);
		//(*data)->ArrVertBuf = new vertex_static[iStartVertex];
		vertex_static * pData;
		if (!FAILED((*data)->m_pVertexBuffer->Lock(0, sizeof(vertex_static)* iStartVertex, (void**)&pData, 0)))
		{
			memcpy(pData, pVertices, sizeof(vertex_static)* iStartVertex);
			//memcpy((*data)->ArrVertBuf, pVertices, sizeof(vertex_static)* iStartVertex);

			(*data)->m_pVertexBuffer->Unlock();
		}

		(*data)->m_uiAllIndexCount = iStartIndex;
		g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* iStartIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &(*data)->m_pIndexBuffer, 0);
		//(*data)->ArrIndBuf = new UINT[iStartIndex];
		if (!FAILED((*data)->m_pIndexBuffer->Lock(0, sizeof(UINT)* iStartIndex, (void**)&pData, 0)))
		{
			memcpy(pData, pIndices, sizeof(UINT)* iStartIndex);
			//memcpy((*data)->ArrIndBuf, pIndices, sizeof(UINT)* iStartIndex);
			(*data)->m_pIndexBuffer->Unlock();
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

		mem_delete_a(pVertices);
		mem_delete_a(pIndices);
	}

	fclose(pf);

	//УНИФИКАЦИЯ ПОДГРУПП
	//{{
	DataStaticModel* tmpmodel = new DataStaticModel();
	tmpmodel->m_uiSubsetCount = 0;
	tmpmodel->m_ppTextures = new char*[(*data)->m_uiSubsetCount];
	memset(tmpmodel->m_ppTextures, 0, (*data)->m_uiSubsetCount * sizeof(char));

	vertex_static* ArrVertBuf = new vertex_static[(*data)->m_uiAllVertexCount];
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

	vertex_static* pVert;
	UINT* pInd;

	(*data)->m_pVertexBuffer->Lock(0, 0, (void**)&pVert, 0);

	(*data)->m_pIndexBuffer->Lock(0, 0, (void**)&pInd, 0);

	for (long i = 0; i < (*data)->m_uiSubsetCount; ++i)
	{
		if ((*data)->m_ppTextures[i][0] == 0)
			continue;

		tmpmodel->m_ppTextures[tmpmodel->m_uiSubsetCount] = new char[strlen((*data)->m_ppTextures[i]) + 1];
		strcpy(tmpmodel->m_ppTextures[tmpmodel->m_uiSubsetCount], (*data)->m_ppTextures[i]);

		startvertex = countvertex;
		startindex = countindex;

		tmpmodel->m_pStartIndex[tmpmodel->m_uiSubsetCount] = startindex;
		tmpmodel->m_pStartVertex[tmpmodel->m_uiSubsetCount] = startvertex;

		memcpy(ArrVertBuf + startvertex, pVert + (*data)->m_pStartVertex[i], sizeof(vertex_static)* (*data)->m_pVertexCount[i]);
		memcpy(ArrIndBuf + startindex, pInd + (*data)->m_pStartIndex[i], sizeof(UINT)* (*data)->m_pIndexCount[i]);

		for (long j = 0; j < (*data)->m_pIndexCount[i]; ++j)
		{
			ArrIndBuf[countindex + j] = countvertex + (pInd[(*data)->m_pStartIndex[i] + j] - (*data)->m_pStartVertex[i]);
		}

		countvertex += (*data)->m_pVertexCount[i];
		countindex += (*data)->m_pIndexCount[i];

		for (long k = i + 1; k < (*data)->m_uiSubsetCount; ++k)
		{
			if (strcmp((*data)->m_ppTextures[i], (*data)->m_ppTextures[k]) == 0)
			{
				(*data)->m_ppTextures[k][0] = 0;
				memcpy(ArrVertBuf + countvertex, pVert + (*data)->m_pStartVertex[k], sizeof(vertex_static)* (*data)->m_pVertexCount[k]);

				memcpy(ArrIndBuf + countindex, pInd + (*data)->m_pStartIndex[k], sizeof(UINT)* (*data)->m_pIndexCount[k]);

				for (long j = 0; j < (*data)->m_pIndexCount[k]; ++j)
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

	(*data)->m_pVertexBuffer->Unlock();
	(*data)->m_pIndexBuffer->Unlock();

	tmpmodel->m_uiAllIndexCount = countindex;
	tmpmodel->m_uiAllVertexCount = countvertex;

	g_pDXDevice->CreateVertexBuffer(sizeof(vertex_static)* countvertex, NULL, NULL, D3DPOOL_MANAGED, &tmpmodel->m_pVertexBuffer, 0);

	vertex_static * pData;
	if (!FAILED(tmpmodel->m_pVertexBuffer->Lock(0, sizeof(vertex_static)* countvertex, (void**)&pData, 0)))
	{
		memcpy(pData, ArrVertBuf, sizeof(vertex_static)* countvertex);

		float3_t tmppos = pData[0].Pos;
		tmpmodel->m_vBBMax = tmppos;
		tmpmodel->m_vBBMin = tmppos;
		float3_t pos;

		for (long i = 0; i<countvertex; i++)
		{
			pos = pData[i].Pos;

			if (pos.x > tmpmodel->m_vBBMax.x)
				tmpmodel->m_vBBMax.x = pos.x;

			if (pos.y > tmpmodel->m_vBBMax.y)
				tmpmodel->m_vBBMax.y = pos.y;

			if (pos.z > tmpmodel->m_vBBMax.z)
				tmpmodel->m_vBBMax.z = pos.z;


			if (pos.x < tmpmodel->m_vBBMin.x)
				tmpmodel->m_vBBMin.x = pos.x;

			if (pos.y < tmpmodel->m_vBBMin.y)
				tmpmodel->m_vBBMin.y = pos.y;

			if (pos.z < tmpmodel->m_vBBMin.z)
				tmpmodel->m_vBBMin.z = pos.z;
		}

		float3 Center = (tmpmodel->m_vBBMin + tmpmodel->m_vBBMax) * 0.5f;
		tmpmodel->m_vBSphere.x = Center.x;
		tmpmodel->m_vBSphere.y = Center.y;
		tmpmodel->m_vBSphere.z = Center.z;
		tmpmodel->m_vBSphere.w = SMVector3Length(Center - tmpmodel->m_vBBMax);

		tmpmodel->m_pVertexBuffer->Unlock();
	}

	g_pDXDevice->CreateIndexBuffer(sizeof(UINT)* countindex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &tmpmodel->m_pIndexBuffer, 0);

	if (!FAILED(tmpmodel->m_pIndexBuffer->Lock(0, sizeof(UINT)* countindex, (void**)&pData, 0)))
	{
		memcpy(pData, ArrIndBuf, sizeof(UINT)* countindex);
		tmpmodel->m_pIndexBuffer->Unlock();
	}

	mem_release_del((*data));
	mem_delete_a(ArrVertBuf);
	mem_delete_a(ArrIndBuf);

	(*data) = tmpmodel;
	//}

	//Core::InLog("mesh is loaded [%s]\n", file);
};

void SGCore_StaticModelSave(const char * file, DataStaticModel** data)
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

/*void SGCore_ConvertX2DSE(const char* pathx, const char* pathdse)
{
	ID3DXMesh* Mesh;
	ID3DXBuffer* Mtrl;
	ID3DXBuffer *Adj;
	DWORD CountMtrl;
	if (FAILED(D3DXLoadMeshFromX(
		pathx,
		D3DXMESH_32BIT,
		DXDevice,
		&Adj,
		&Mtrl,
		0,
		&CountMtrl,
		&Mesh))
		)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "failed loaded X mesh '%s'\n", pathx);
		return;
	}

	D3DVERTEXELEMENT9 layoutstaticdynamic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	ID3DXMesh* pMesh = 0;
	Mesh->CloneMesh(Mesh->GetOptions(), layoutstaticdynamic, DXDevice, &pMesh);

	Mesh->Release();

	D3DXComputeNormals(pMesh, NULL);


	DWORD* adjacencyInfo = new DWORD[pMesh->GetNumFaces() * 3];
	pMesh->GenerateAdjacency(0.0f, adjacencyInfo);

	// Массив для хранения информации о смежности граней
	// оптимизированной сетки
	DWORD* optimizedAdjacencyInfo = new DWORD[pMesh->GetNumFaces() * 3];


	HRESULT hr = pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT, adjacencyInfo, optimizedAdjacencyInfo, 0, 0);

	DWORD numSubsets = 0;
	pMesh->GetAttributeTable(0, &numSubsets);

	D3DXATTRIBUTERANGE* tableAtt = new D3DXATTRIBUTERANGE[numSubsets];
	pMesh->GetAttributeTable(tableAtt, &numSubsets);
	Mesh = pMesh;

	DWORD numSubsets22 = 0;
	Mesh->GetAttributeTable(0, &numSubsets22);

	D3DXATTRIBUTERANGE* table = new D3DXATTRIBUTERANGE[numSubsets22];
	Mesh->GetAttributeTable(table, &numSubsets22);




	DataStaticModel* DataModel = new DataStaticModel();

	DataModel->SubsetCount = CountMtrl;
	DataModel->StartIndex = new UINT[DataModel->SubsetCount];
	DataModel->IndexCount = new UINT[DataModel->SubsetCount];
	DataModel->StartVertex = new UINT[DataModel->SubsetCount];
	DataModel->VertexCount = new UINT[DataModel->SubsetCount];
	DataModel->ArrTextures = new char*[DataModel->SubsetCount];



	D3DXMATERIAL* mtrls = (D3DXMATERIAL*)Mtrl->GetBufferPointer();
	int NumP = 0;
	for (DWORD i = 0; i<DataModel->SubsetCount; i++)
	{
		for (int k = strlen(mtrls[i].pTextureFilename); k >= 0; k--)
		{
			if (mtrls[i].pTextureFilename[k] == '.')
			{
				NumP = k;
				break;
			}
		}
		DataModel->ArrTextures[i] = new char[NumP + 1];
		//sprintf(DataModel->ArrTextures[i],"%s",NumP);
		memcpy(DataModel->ArrTextures[i], mtrls[i].pTextureFilename, sizeof(char)* NumP);
		DataModel->ArrTextures[i][NumP] = 0;

		DataModel->StartIndex[i] = tableAtt[i].FaceStart * 3;
		DataModel->IndexCount[i] = tableAtt[i].FaceCount * 3;
		DataModel->StartVertex[i] = tableAtt[i].VertexStart;
		DataModel->VertexCount[i] = tableAtt[i].VertexCount;
	}

	DXDevice->CreateVertexBuffer(sizeof(vertex_static)* Mesh->GetNumVertices(), 0, 0, D3DPOOL_DEFAULT, &(DataModel->VertexBuffer), 0);

	void * pData;
	void * srcpData;
	DataModel->VertexBuffer->Lock(0, 0, (void**)&pData, 0);
	Mesh->LockVertexBuffer(0, (void**)&srcpData);

	memcpy(pData, srcpData, sizeof(vertex_static)* Mesh->GetNumVertices());

	DataModel->VertexBuffer->Unlock();
	Mesh->UnlockVertexBuffer();


	DXDevice->CreateIndexBuffer(sizeof(UINT)* Mesh->GetNumFaces() * 3, NULL, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &DataModel->IndexBuffer, 0);

	UINT* tmpIndD, *tmpIndS;
	Mesh->LockIndexBuffer(0, (void**)&tmpIndS);
	DataModel->IndexBuffer->Lock(0, 0, (void**)&tmpIndD, 0);
	//memcpy(pData,srcpData,sizeof(DWORD) * Mesh->GetNumFaces() * 3);
	DWORD tmpCountCPoly = 0;
	for (DWORD i = 0; i<DataModel->SubsetCount; i++)
	{
		for (DWORD k = 0; k<DataModel->IndexCount[i]; k++)
		{
			tmpIndD[tmpCountCPoly] = tmpIndS[tmpCountCPoly] - DataModel->StartIndex[i];
			tmpCountCPoly++;
		}
	}

	DataModel->IndexBuffer->Unlock();


	SGCore_SaveStaticModel(pathdse, &DataModel);

	mem_release(Mesh);
	mem_release(DataModel);
}*/