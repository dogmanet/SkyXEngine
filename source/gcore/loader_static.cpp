
struct DataStaticModel : public ISXDataStaticModel
{
	DataStaticModel()
	{
		VertexBuffer = 0;
		IndexBuffer = 0;
		ArrVertBuf = 0;
		ArrIndBuf = 0;

		SubsetCount = 0;
		ArrTextures = 0;
		StartIndex = 0;
		IndexCount = 0;
		StartVertex = 0;
		VertexCount = 0;
		AllIndexCount = 0;
		AllVertexCount = 0;
	}

	void Release(){ mem_del(this); }

	ISXDataStaticModel* GetCopy()
	{
		ISXDataStaticModel* nm = new DataStaticModel();
		nm->ArrTextures = new char*[SubsetCount];
		for (DWORD i = 0; i < SubsetCount; i++)
		{
			nm->ArrTextures[i] = new char[strlen(ArrTextures[i]) + 1];
			sprintf(nm->ArrTextures[i], "%s", ArrTextures[i]);
		}
		nm->SubsetCount = SubsetCount;
		nm->StartIndex = new UINT[SubsetCount];
		memcpy(nm->StartIndex, StartIndex, sizeof(UINT)*SubsetCount);
		nm->IndexCount = new UINT[SubsetCount];
		memcpy(nm->IndexCount, IndexCount, sizeof(UINT)*SubsetCount);
		nm->StartVertex = new UINT[SubsetCount];
		memcpy(nm->StartVertex, StartVertex, sizeof(UINT)*SubsetCount);
		nm->VertexCount = new UINT[SubsetCount];
		memcpy(nm->VertexCount, VertexCount, sizeof(UINT)*SubsetCount);

		DWORD tmpvert = 0;
		DWORD tmpind = 0;
		for (DWORD i = 0; i < SubsetCount; i++)
		{
			tmpvert += nm->VertexCount[i];
			tmpind += nm->IndexCount[i];
		}

		DXDevice->CreateVertexBuffer(sizeof(vertex_static)* tmpvert, NULL, NULL, D3DPOOL_MANAGED, &nm->VertexBuffer, 0);
		nm->ArrVertBuf = new vertex_static[tmpvert];
		vertex_static * dstData, *srcData;
		nm->VertexBuffer->Lock(0, 0, (void**)&dstData, 0);
		VertexBuffer->Lock(0, 0, (void**)&srcData, 0);

		memcpy(dstData, srcData, sizeof(vertex_static)* tmpvert);
		memcpy(nm->ArrVertBuf, srcData, sizeof(vertex_static)* tmpvert);

		nm->VertexBuffer->Unlock();
		VertexBuffer->Unlock();


		DXDevice->CreateIndexBuffer(sizeof(UINT)* tmpind, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &nm->IndexBuffer, 0);
		nm->ArrIndBuf = new DWORD[tmpind];
		nm->IndexBuffer->Lock(0, 0, (void**)&dstData, 0);
		IndexBuffer->Lock(0, 0, (void**)&srcData, 0);

		memcpy(dstData, srcData, sizeof(UINT)* tmpind);
		memcpy(nm->ArrIndBuf, srcData, sizeof(UINT)* tmpind);

		nm->IndexBuffer->Unlock();
		IndexBuffer->Unlock();

		return nm;
	}

	DataStaticModel::~DataStaticModel()
	{
		mem_release_del(VertexBuffer);
		mem_release_del(IndexBuffer);

		for (DWORD i = 0; i < SubsetCount; ++i)
		{
			mem_delete_a(ArrTextures[i]);
		}

		mem_delete_a(ArrVertBuf);
		mem_delete_a(ArrIndBuf);

		mem_delete_a(ArrTextures);
		mem_delete_a(StartIndex);
		mem_delete_a(IndexCount);
		mem_delete_a(StartVertex);
		mem_delete_a(VertexCount);
	}
};


///////////

ISXDataStaticModel* SGCore_CrDSModel()
{
	return new DataStaticModel();
}

void SGCore_LoadStaticModel(const char * file, ISXDataStaticModel** data)
{
	if (!data)
	{
		return;
	}

	(*data) = new DataStaticModel();
	FILE * pf = fopen(file, "rb");
	if (!pf)
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "unable to open model file '%s'\n", file);
		return;
	}

	ModelHeader header;

	fread(&header, sizeof(ModelHeader), 1, pf);

	if (header.Magick != SX_MODEL_MAGICK)
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "unsupported file type '%s'\n", file);
		fclose(pf);
		return;
	}

	if (!(header.iVersion == SX_MODEL_VERSION_OLD || header.iVersion == SX_MODEL_VERSION))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "unsupported file '%s' version %d'\n", header.iVersion, file);
		fclose(pf);
		return;
	}

	if (header.iVersion == SX_MODEL_VERSION_OLD)
	{
		(*data)->ArrTextures = NULL;
		Array<String> tex;
		if (header.iMaterialsOffset)
		{
			fseek(pf, header.iMaterialsOffset, SEEK_SET);
			(*data)->ArrTextures = new char*[header.iMaterialCount];
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
			(*data)->SubsetCount = lh.iSubMeshCount;
			(*data)->StartIndex = new UINT[lh.iSubMeshCount];
			(*data)->IndexCount = new UINT[lh.iSubMeshCount];
			(*data)->StartVertex = new UINT[lh.iSubMeshCount];
			(*data)->VertexCount = new UINT[lh.iSubMeshCount];
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

				(*data)->ArrTextures[i] = new char[tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1];
				memcpy((*data)->ArrTextures[i], tex[lh.pSubLODmeshes[i].iMaterialID].c_str(), (tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1) * sizeof(char));
			}

			UINT iStartIndex = 0;
			UINT iStartVertex = 0;

			for (int i = 0; i < lh.iSubMeshCount; i++)
			{
				(*data)->StartIndex[i] = iStartIndex;
				(*data)->StartVertex[i] = iStartVertex;
				(*data)->IndexCount[i] = lh.pSubLODmeshes[i].iIndexCount;
				(*data)->VertexCount[i] = lh.pSubLODmeshes[i].iVectexCount;
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

			(*data)->AllVertexCount = iStartVertex;
			DXDevice->CreateVertexBuffer(sizeof(vertex_static)* iStartVertex, NULL, NULL, D3DPOOL_MANAGED, &(*data)->VertexBuffer, 0);
			(*data)->ArrVertBuf = new vertex_static[iStartVertex];
			vertex_static * pData;
			if (!FAILED((*data)->VertexBuffer->Lock(0, sizeof(vertex_static)* iStartVertex, (void**)&pData, 0)))
			{
				for (DWORD i = 0; i < lh.iSubMeshCount; i++)
				{
					for (DWORD k = 0; k < lh.pSubLODmeshes[i].iVectexCount; k++)
					{
						memcpy(pData + lh.pSubLODmeshes[i].iStartVertex + k, pVertices + lh.pSubLODmeshes[i].iStartVertex + k, sizeof(vertex_static));
						memcpy((*data)->ArrVertBuf + lh.pSubLODmeshes[i].iStartVertex + k, pVertices + lh.pSubLODmeshes[i].iStartVertex + k, sizeof(vertex_static));
					}
				}
				(*data)->VertexBuffer->Unlock();
			}

			(*data)->AllIndexCount = iStartIndex;
			DWORD tmpCountIndecex = 0;
			DXDevice->CreateIndexBuffer(sizeof(UINT)* iStartIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &(*data)->IndexBuffer, 0);
			(*data)->ArrIndBuf = new DWORD[iStartIndex];
			if (!FAILED((*data)->IndexBuffer->Lock(0, sizeof(UINT)* iStartIndex, (void**)&pData, 0)))
			{
				memcpy(pData, pIndices, sizeof(UINT)* iStartIndex);
				memcpy((*data)->ArrIndBuf, pIndices, sizeof(UINT)* iStartIndex);
				(*data)->IndexBuffer->Unlock();
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
		(*data)->ArrTextures = NULL;
		Array<String> tex;
		if (header.iMaterialsOffset)
		{
			fseek(pf, header.iMaterialsOffset, SEEK_SET);
			(*data)->ArrTextures = new char*[header.iMaterialCount];
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

		(*data)->SubsetCount = m_pLods[0].iSubMeshCount;
		(*data)->StartIndex = new UINT[m_pLods[0].iSubMeshCount];
		(*data)->IndexCount = new UINT[m_pLods[0].iSubMeshCount];
		(*data)->StartVertex = new UINT[m_pLods[0].iSubMeshCount];
		(*data)->VertexCount = new UINT[m_pLods[0].iSubMeshCount];

		UINT iStartIndex = 0;
		UINT iStartVertex = 0;
		ModelLoD lh = m_pLods[0];

		for (int i = 0; i < lh.iSubMeshCount; i++)
		{
			(*data)->StartIndex[i] = iStartIndex;
			(*data)->StartVertex[i] = iStartVertex;
			(*data)->IndexCount[i] = lh.pSubLODmeshes[i].iIndexCount;
			(*data)->VertexCount[i] = lh.pSubLODmeshes[i].iVectexCount;
			lh.pSubLODmeshes[i].iStartIndex = iStartIndex;
			lh.pSubLODmeshes[i].iStartVertex = iStartVertex;

			for (int j = 0; j < lh.pSubLODmeshes[i].iIndexCount; j++)
			{
				lh.pSubLODmeshes[i].pIndices[j] += iStartVertex;
			}

			iStartIndex += lh.pSubLODmeshes[i].iIndexCount;
			iStartVertex += lh.pSubLODmeshes[i].iVectexCount;

			(*data)->ArrTextures[i] = new char[tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1];
			memcpy((*data)->ArrTextures[i], tex[lh.pSubLODmeshes[i].iMaterialID].c_str(), (tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1) * sizeof(char));
		}

		UINT * pIndices = new UINT[iStartIndex];
		vertex_static * pVertices = new vertex_static[iStartVertex];

		for (int i = 0; i < lh.iSubMeshCount; i++)
		{
			memcpy(pIndices + lh.pSubLODmeshes[i].iStartIndex, lh.pSubLODmeshes[i].pIndices, sizeof(UINT)* lh.pSubLODmeshes[i].iIndexCount);
			memcpy(pVertices + lh.pSubLODmeshes[i].iStartVertex, lh.pSubLODmeshes[i].pVertices, sizeof(vertex_static)* lh.pSubLODmeshes[i].iVectexCount);
		}

		(*data)->AllVertexCount = iStartVertex;
		DXDevice->CreateVertexBuffer(sizeof(vertex_static)* iStartVertex, NULL, NULL, D3DPOOL_MANAGED, &(*data)->VertexBuffer, 0);
		(*data)->ArrVertBuf = new vertex_static[iStartVertex];
		vertex_static * pData;
		if (!FAILED((*data)->VertexBuffer->Lock(0, sizeof(vertex_static)* iStartVertex, (void**)&pData, 0)))
		{
			memcpy(pData, pVertices, sizeof(vertex_static)* iStartVertex);
			memcpy((*data)->ArrVertBuf, pVertices, sizeof(vertex_static)* iStartVertex);

			(*data)->VertexBuffer->Unlock();
		}

		(*data)->AllIndexCount = iStartIndex;
		DXDevice->CreateIndexBuffer(sizeof(UINT)* iStartIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &(*data)->IndexBuffer, 0);
		(*data)->ArrIndBuf = new DWORD[iStartIndex];
		if (!FAILED((*data)->IndexBuffer->Lock(0, sizeof(UINT)* iStartIndex, (void**)&pData, 0)))
		{
			memcpy(pData, pIndices, sizeof(UINT)* iStartIndex);
			memcpy((*data)->ArrIndBuf, pIndices, sizeof(UINT)* iStartIndex);
			(*data)->IndexBuffer->Unlock();
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
	tmpmodel->SubsetCount = 0;
	tmpmodel->ArrTextures = new char*[(*data)->SubsetCount];
	memset(tmpmodel->ArrTextures, 0, (*data)->SubsetCount * sizeof(char));

	tmpmodel->ArrVertBuf = new vertex_static[(*data)->AllVertexCount];
	tmpmodel->ArrIndBuf = new DWORD[(*data)->AllIndexCount];

	tmpmodel->StartIndex = new UINT[(*data)->SubsetCount];
	tmpmodel->StartVertex = new UINT[(*data)->SubsetCount];
	tmpmodel->IndexCount = new UINT[(*data)->SubsetCount];
	tmpmodel->VertexCount = new UINT[(*data)->SubsetCount];

	long startindex = 0;
	long startvertex = 0;
	long countindex = 0;
	long countvertex = 0;
	//long countsubset = 0;

	for (long i = 0; i < (*data)->SubsetCount; ++i)
	{
		if ((*data)->ArrTextures[i][0] == 0)
			continue;

		tmpmodel->ArrTextures[tmpmodel->SubsetCount] = new char[strlen((*data)->ArrTextures[i]) + 1];
		strcpy(tmpmodel->ArrTextures[tmpmodel->SubsetCount], (*data)->ArrTextures[i]);

		startvertex = countvertex;
		startindex = countindex;

		tmpmodel->StartIndex[tmpmodel->SubsetCount] = startindex;
		tmpmodel->StartVertex[tmpmodel->SubsetCount] = startvertex;

		memcpy(tmpmodel->ArrVertBuf + startvertex, (*data)->ArrVertBuf + (*data)->StartVertex[i], sizeof(vertex_static)* (*data)->VertexCount[i]);
		//memcpy(tmpmodel->ArrIndBuf + startindex, (*data)->ArrIndBuf + (*data)->StartIndex[i], sizeof(DWORD)* (*data)->IndexCount[i]);

		for (long j = 0; j < (*data)->IndexCount[i]; ++j)
		{
			tmpmodel->ArrIndBuf[countindex + j] = countvertex + ((*data)->ArrIndBuf[(*data)->StartIndex[i] + j] - (*data)->StartVertex[i]);
		}

		countvertex += (*data)->VertexCount[i];
		countindex += (*data)->IndexCount[i];

		for (long k = i + 1; k < (*data)->SubsetCount; ++k)
		{
			if (strcmp((*data)->ArrTextures[i], (*data)->ArrTextures[k]) == 0)
			{
				(*data)->ArrTextures[k][0] = 0;
				memcpy(tmpmodel->ArrVertBuf + countvertex, (*data)->ArrVertBuf + (*data)->StartVertex[k], sizeof(vertex_static)* (*data)->VertexCount[k]);

				//memcpy(tmpmodel->ArrIndBuf + countindex, (*data)->ArrIndBuf + (*data)->StartIndex[k], sizeof(DWORD)* (*data)->IndexCount[k]);

				for (long j = 0; j < (*data)->IndexCount[k]; ++j)
				{
					tmpmodel->ArrIndBuf[countindex + j] = countvertex + ((*data)->ArrIndBuf[(*data)->StartIndex[k] + j] - (*data)->StartVertex[k]);
				}

				countvertex += (*data)->VertexCount[k];
				countindex += (*data)->IndexCount[k];
			}
		}

		tmpmodel->VertexCount[tmpmodel->SubsetCount] = countvertex - startvertex;
		tmpmodel->IndexCount[tmpmodel->SubsetCount] = countindex - startindex;

		++tmpmodel->SubsetCount;
	}

	tmpmodel->AllIndexCount = countindex;
	tmpmodel->AllVertexCount = countvertex;

	DXDevice->CreateVertexBuffer(sizeof(vertex_static)* countvertex, NULL, NULL, D3DPOOL_MANAGED, &tmpmodel->VertexBuffer, 0);

	vertex_static * pData;
	if (!FAILED(tmpmodel->VertexBuffer->Lock(0, sizeof(vertex_static)* countvertex, (void**)&pData, 0)))
	{
		memcpy(pData, tmpmodel->ArrVertBuf, sizeof(vertex_static)* countvertex);
		tmpmodel->VertexBuffer->Unlock();
	}

	DXDevice->CreateIndexBuffer(sizeof(UINT)* countindex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &tmpmodel->IndexBuffer, 0);

	if (!FAILED(tmpmodel->IndexBuffer->Lock(0, sizeof(UINT)* countindex, (void**)&pData, 0)))
	{
		memcpy(pData, tmpmodel->ArrIndBuf, sizeof(UINT)* countindex);
		tmpmodel->IndexBuffer->Unlock();
	}

	mem_release_del((*data));

	(*data) = tmpmodel;
	//}

	//Core::InLog("mesh is loaded [%s]\n", file);
};

void SGCore_SaveStaticModel(const char * file, DataStaticModel** data)
{
	ModelHeader hdr;
	memset(&hdr, 0, sizeof(hdr));

	FILE * pF = fopen(file, "wb");

	if (!pF)
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "unable to open model file '%s'\n", file);
		return;
	}

	hdr.Magick = SX_MODEL_MAGICK;
	hdr.iBoneCount = 1;
	hdr.iFlags = MODEL_FLAG_COMPILED | MODEL_FLAG_STATIC;

	hdr.iMaterialCount = (*data)->SubsetCount;
	hdr.iSkinCount = 1;
	hdr.iVersion = SX_MODEL_VERSION_OLD;

	hdr.iMaterialsOffset = sizeof(hdr);

	UINT iMaterialsSize = 0;
	for (UINT i = 0; i < hdr.iMaterialCount; i++)
	{
		iMaterialsSize += strlen((*data)->ArrTextures[i]) + 1;
	}

	hdr.iBonesOffset = 0;

	hdr.iLODoffset = hdr.iMaterialsOffset + iMaterialsSize;

	UINT iLodSize = 0;
	iLodSize += sizeof(int);
	for (int i = 0; i < hdr.iMaterialCount; i++)
	{
		iLodSize += sizeof(vertex_animated_ex)* (*data)->VertexCount[i];
		iLodSize += sizeof(UINT)* (*data)->IndexCount[i];
		iLodSize += sizeof(int)* 3;
	}

	hdr.iAnimationsOffset = 0;
	hdr.iSecondHeaderOffset = 0;

	fwrite(&hdr, sizeof(hdr), 1, pF);
	for (UINT i = 0; i < hdr.iMaterialCount; i++)
	{
		fwrite((*data)->ArrTextures[i], 1, strlen((*data)->ArrTextures[i]) + 1, pF);
	}
	int smc = hdr.iMaterialCount;
	fwrite(&smc, sizeof(int), 1, pF);
	vertex_animated_ex * pVB;
	UINT * pIB;
	(*data)->VertexBuffer->Lock(0, 0, (void**)&pVB, 0);
	(*data)->IndexBuffer->Lock(0, 0, (void**)&pIB, 0);
	UINT iVC = 0;
	UINT iIC = 0;
	for (int i = 0; i < hdr.iMaterialCount; i++)
	{
		fwrite(&i, sizeof(int), 1, pF);
		fwrite(&(*data)->VertexCount[i], sizeof(int), 1, pF);
		fwrite(&(*data)->IndexCount[i], sizeof(int), 1, pF);

		fwrite(pVB + iVC, sizeof(vertex_animated_ex), (*data)->VertexCount[i], pF);
		fwrite(pIB + iIC, sizeof(UINT), (*data)->IndexCount[i], pF);
		iVC += (*data)->VertexCount[i];
		iIC += (*data)->IndexCount[i];
	}
	(*data)->VertexBuffer->Unlock();
	(*data)->IndexBuffer->Unlock();
	fclose(pF);
}

void SGCore_ConvertX2DSE(const char* pathx, const char* pathdse)
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
		reportf(REPORT_MSG_LEVEL_ERROR, "failed loaded X mesh '%s'\n", pathx);
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


	HRESULT hr = pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT/* | D3DXMESHOPT_VERTEXCACHE*/, adjacencyInfo, optimizedAdjacencyInfo, 0, 0);

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
}