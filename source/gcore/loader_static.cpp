
/*
DataStaticModel::DataStaticModel(){}

DataStaticModel::DataStaticModel(DataStaticModel& dsm)
{
	ArrTextures = new char*[dsm.SubsetCount];
	for (DWORD i = 0; i<dsm.SubsetCount; i++)
	{
		ArrTextures[i] = new char[strlen(dsm.ArrTextures[i]) + 1];
		sprintf(ArrTextures[i], "%s", dsm.ArrTextures[i]);
	}
	SubsetCount = dsm.SubsetCount;
	StartIndex = new UINT[dsm.SubsetCount];
	memcpy(StartIndex, dsm.StartIndex, sizeof(UINT)*dsm.SubsetCount);
	IndexCount = new UINT[dsm.SubsetCount];
	memcpy(IndexCount, dsm.IndexCount, sizeof(UINT)*dsm.SubsetCount);
	StartVertex = new UINT[dsm.SubsetCount];
	memcpy(StartVertex, dsm.StartVertex, sizeof(UINT)*dsm.SubsetCount);
	VertexCount = new UINT[dsm.SubsetCount];
	memcpy(VertexCount, dsm.VertexCount, sizeof(UINT)*dsm.SubsetCount);

	DWORD tmpvert = 0;
	DWORD tmpind = 0;
	for (DWORD i = 0; i<dsm.SubsetCount; i++)
	{
		tmpvert += VertexCount[i];
		tmpind += IndexCount[i];
	}

	DXDevice->CreateVertexBuffer(sizeof(vertex_static)* tmpvert, NULL, NULL, D3DPOOL_MANAGED, &VertexBuffer, 0);
	ArrVertBuf = new vertex_static[tmpvert];
	vertex_static * dstData, *srcData;
	VertexBuffer->Lock(0, 0, (void**)&dstData, 0);
	dsm.VertexBuffer->Lock(0, 0, (void**)&dstData, 0);

	memcpy(dstData, srcData, sizeof(vertex_static)* tmpvert);
	memcpy(ArrVertBuf, srcData, sizeof(vertex_static)* tmpvert);

	VertexBuffer->Unlock();
	dsm.VertexBuffer->Unlock();


	DXDevice->CreateIndexBuffer(sizeof(UINT)* tmpind, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &IndexBuffer, 0);
	ArrIndBuf = new DWORD[tmpind];
	IndexBuffer->Lock(0, 0, (void**)&dstData, 0);
	dsm.IndexBuffer->Lock(0, 0, (void**)&srcData, 0);

	memcpy(dstData, srcData, sizeof(UINT)* tmpind);
	memcpy(ArrIndBuf, srcData, sizeof(UINT)* tmpind);

	IndexBuffer->Unlock();
	dsm.IndexBuffer->Unlock();
}

DataStaticModel::~DataStaticModel()
{
	mem_release_del(VertexBuffer);
	mem_release_del(IndexBuffer);

	for (DWORD i = 0; i < SubsetCount; ++i)
	{
		mem_delete_a(ArrTextures[i]);
	}

	mem_delete_a(ArrTextures);
	mem_delete_a(StartIndex);
	mem_delete_a(IndexCount);
	mem_delete_a(StartVertex);
	mem_delete_a(VertexCount);
}
*/

///////////

void SGCore_LoadStaticModel(const char * file, DataStaticModel * data)
{
	if (!data)
	{
		return;
	}
	FILE * pf = fopen(file, "rb");
	if (!pf)
	{
		//Core::InError("Unable to open model file \"%s\"\n", file);
		return;
	}

	ModelHeader header;

	fread(&header, sizeof(ModelHeader), 1, pf);

	if (header.Magick != SX_MODEL_MAGICK)
	{
		//Core::InError("Unsupported file type \"%s\"\n", file);
		fclose(pf);
		return;
	}

	if (!(header.iVersion == SX_MODEL_VERSION_OLD || header.iVersion == SX_MODEL_VERSION))
	{
		//DSconsole::write("[Error]: Unsupported file version %d \"%s\"\n", header.iVersion, (file + ".dse").c_str());
		//Core::InError("Unsupported file version %d \"%s\"\n", header.iVersion, file);
		fclose(pf);
		return;
	}

	if (header.iVersion == SX_MODEL_VERSION_OLD)
	{
		data->ArrTextures = NULL;
		Array<String> tex;
		if (header.iMaterialsOffset)
		{
			fseek(pf, header.iMaterialsOffset, SEEK_SET);
			data->ArrTextures = new char*[header.iMaterialCount];
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
			data->SubsetCount = lh.iSubMeshCount;
			data->StartIndex = new UINT[lh.iSubMeshCount];
			data->IndexCount = new UINT[lh.iSubMeshCount];
			data->StartVertex = new UINT[lh.iSubMeshCount];
			data->VertexCount = new UINT[lh.iSubMeshCount];
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

				data->ArrTextures[i] = new char[tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1];
				memcpy(data->ArrTextures[i], tex[lh.pSubLODmeshes[i].iMaterialID].c_str(), (tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1) * sizeof(char));
			}

			UINT iStartIndex = 0;
			UINT iStartVertex = 0;
			/*for(int i = 0; i < lh.iSubMeshCount; i++)
			{
			data->StartIndex[i] = iStartIndex;
			data->StartVertex[i] = iStartVertex;
			data->IndexCount[i] = lh.pSubLODmeshes[i].iIndexCount;
			data->VertexCount[i] = lh.pSubLODmeshes[i].iVectexCount;
			lh.pSubLODmeshes[i].iStartIndex = iStartIndex;
			lh.pSubLODmeshes[i].iStartVertex = iStartVertex;

			iStartIndex += lh.pSubLODmeshes[i].iIndexCount;
			iStartVertex += lh.pSubLODmeshes[i].iVectexCount;
			}*/

			for (int i = 0; i < lh.iSubMeshCount; i++)
			{
				data->StartIndex[i] = iStartIndex;
				data->StartVertex[i] = iStartVertex;
				data->IndexCount[i] = lh.pSubLODmeshes[i].iIndexCount;
				data->VertexCount[i] = lh.pSubLODmeshes[i].iVectexCount;
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

			data->AllVertexCount = iStartVertex;
			DXDevice->CreateVertexBuffer(sizeof(vertex_static)* iStartVertex, NULL, NULL, D3DPOOL_MANAGED, &data->VertexBuffer, 0);
			data->ArrVertBuf = new vertex_static[iStartVertex];
			vertex_static * pData;
			if (!FAILED(data->VertexBuffer->Lock(0, sizeof(vertex_static)* iStartVertex, (void**)&pData, 0)))
			{
				//memcpy(pData, pVertices, sizeof(vertex) * iStartVertex);
				for (DWORD i = 0; i<lh.iSubMeshCount; i++)
				{
					for (DWORD k = 0; k<lh.pSubLODmeshes[i].iVectexCount; k++)
					{
						//pData[lh.pSubLODmeshes[i].iStartVertex + k] = pVertices + k
						memcpy(pData + lh.pSubLODmeshes[i].iStartVertex + k, pVertices + lh.pSubLODmeshes[i].iStartVertex + k, sizeof(vertex_static));
						memcpy(data->ArrVertBuf + lh.pSubLODmeshes[i].iStartVertex + k, pVertices + lh.pSubLODmeshes[i].iStartVertex + k, sizeof(vertex_static));
					}
				}
				data->VertexBuffer->Unlock();
			}

			data->AllIndexCount = iStartIndex;
			DWORD tmpCountIndecex = 0;
			DXDevice->CreateIndexBuffer(sizeof(UINT)* iStartIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &data->IndexBuffer, 0);
			data->ArrIndBuf = new DWORD[iStartIndex];
			if (!FAILED(data->IndexBuffer->Lock(0, sizeof(UINT)* iStartIndex, (void**)&pData, 0)))
			{
				memcpy(pData, pIndices, sizeof(UINT)* iStartIndex);
				memcpy(data->ArrIndBuf, pIndices, sizeof(UINT)* iStartIndex);
				data->IndexBuffer->Unlock();
			}

			mem_delete_a(pVertices);
			mem_delete_a(pIndices);
		}
	}
	else if (header.iVersion == SX_MODEL_VERSION)
	{
		data->ArrTextures = NULL;
		Array<String> tex;
		if (header.iMaterialsOffset)
		{
			fseek(pf, header.iMaterialsOffset, SEEK_SET);
			data->ArrTextures = new char*[header.iMaterialCount];
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

		data->SubsetCount = m_pLods[0].iSubMeshCount;
		data->StartIndex = new UINT[m_pLods[0].iSubMeshCount];
		data->IndexCount = new UINT[m_pLods[0].iSubMeshCount];
		data->StartVertex = new UINT[m_pLods[0].iSubMeshCount];
		data->VertexCount = new UINT[m_pLods[0].iSubMeshCount];

		UINT iStartIndex = 0;
		UINT iStartVertex = 0;
		ModelLoD lh = m_pLods[0];

		/*for(int i = 0; i < lh.iSubMeshCount; i++)
		{
		data->StartIndex[i] = iStartIndex;
		data->StartVertex[i] = iStartVertex;
		data->IndexCount[i] = lh.pSubLODmeshes[i].iIndexCount;
		data->VertexCount[i] = lh.pSubLODmeshes[i].iVectexCount;
		lh.pSubLODmeshes[i].iStartIndex = iStartIndex;
		lh.pSubLODmeshes[i].iStartVertex = iStartVertex;

		iStartIndex += lh.pSubLODmeshes[i].iIndexCount;
		iStartVertex += lh.pSubLODmeshes[i].iVectexCount;

		data->ArrTextures[i] = new char[tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1];
		memcpy(data->ArrTextures[i], tex[lh.pSubLODmeshes[i].iMaterialID].c_str(), (tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1) * sizeof(char));
		}*/

		for (int i = 0; i < lh.iSubMeshCount; i++)
		{
			data->StartIndex[i] = iStartIndex;
			data->StartVertex[i] = iStartVertex;
			data->IndexCount[i] = lh.pSubLODmeshes[i].iIndexCount;
			data->VertexCount[i] = lh.pSubLODmeshes[i].iVectexCount;
			lh.pSubLODmeshes[i].iStartIndex = iStartIndex;
			lh.pSubLODmeshes[i].iStartVertex = iStartVertex;

			for (int j = 0; j < lh.pSubLODmeshes[i].iIndexCount; j++)
			{
				lh.pSubLODmeshes[i].pIndices[j] += iStartVertex;
			}

			iStartIndex += lh.pSubLODmeshes[i].iIndexCount;
			iStartVertex += lh.pSubLODmeshes[i].iVectexCount;

			data->ArrTextures[i] = new char[tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1];
			memcpy(data->ArrTextures[i], tex[lh.pSubLODmeshes[i].iMaterialID].c_str(), (tex[lh.pSubLODmeshes[i].iMaterialID].length() + 1) * sizeof(char));
		}

		UINT * pIndices = new UINT[iStartIndex];
		vertex_static * pVertices = new vertex_static[iStartVertex];
		//SXmodelSubLODmesh * pSM;
		for (int i = 0; i < lh.iSubMeshCount; i++)
		{
			//pSM = &lh.pSubLODmeshes[i];
			memcpy(pIndices + lh.pSubLODmeshes[i].iStartIndex, lh.pSubLODmeshes[i].pIndices, sizeof(UINT)* lh.pSubLODmeshes[i].iIndexCount);
			memcpy(pVertices + lh.pSubLODmeshes[i].iStartVertex, lh.pSubLODmeshes[i].pVertices, sizeof(vertex_static)* lh.pSubLODmeshes[i].iVectexCount);
		}

		data->AllVertexCount = iStartVertex;
		DXDevice->CreateVertexBuffer(sizeof(vertex_static)* iStartVertex, NULL, NULL, D3DPOOL_MANAGED, &data->VertexBuffer, 0);
		data->ArrVertBuf = new vertex_static[iStartVertex];
		vertex_static * pData;
		if (!FAILED(data->VertexBuffer->Lock(0, sizeof(vertex_static)* iStartVertex, (void**)&pData, 0)))
		{
			memcpy(pData, pVertices, sizeof(vertex_static)* iStartVertex);
			memcpy(data->ArrVertBuf, pVertices, sizeof(vertex_static)* iStartVertex);

			/*Core::InLog("------------%d\n",iStartVertex);
			for(int i=0;i<iStartVertex;i++)
			{
			Core::InLog("%dpos = %f | %f | %f\n",i,pVertices[i].Pos.x,pVertices[i].Pos.y,pVertices[i].Pos.z);
			}
			Core::InLog("------------%d\n",lh.iSubMeshCount);*/
			data->VertexBuffer->Unlock();
		}
		
		data->AllIndexCount = iStartIndex;
		DXDevice->CreateIndexBuffer(sizeof(UINT)* iStartIndex, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &data->IndexBuffer, 0);
		data->ArrIndBuf = new DWORD[iStartIndex];
		if (!FAILED(data->IndexBuffer->Lock(0, sizeof(UINT)* iStartIndex, (void**)&pData, 0)))
		{
			memcpy(pData, pIndices, sizeof(UINT)* iStartIndex);
			memcpy(data->ArrIndBuf, pIndices, sizeof(UINT)* iStartIndex);
			data->IndexBuffer->Unlock();
		}

		/*Core::InLog("------------%d\n",iStartIndex);
		for(int i=0;i<iStartIndex;i+=3)
		{
		Core::InLog("%dind = %d | %d | %d\n",i,pIndices[i],pIndices[i+1],pIndices[i+2]);
		}
		Core::InLog("------------%d\n",lh.iSubMeshCount);*/

		mem_delete_a(pVertices);
		mem_delete_a(pIndices);


	}

	fclose(pf);

	//Core::InLog("mesh is loaded [%s]\n", file);
	//return(true);
};

void SGCore_SaveStaticModel(const char * file, DataStaticModel * data)
{
	ModelHeader hdr;
	memset(&hdr, 0, sizeof(hdr));

	FILE * pF = fopen(file, "wb");

	if (!pF)
	{
		//DSconsole::write("[Warning]: Unable to open model file \"%s\"\n", (file + ".mc").c_str());
		//Core::InError("Unable to open model file \"%s\"\n", file);
		return;
	}

	hdr.Magick = SX_MODEL_MAGICK;
	hdr.iBoneCount = 1;
	hdr.iFlags = MODEL_FLAG_COMPILED | MODEL_FLAG_STATIC;

	hdr.iMaterialCount = data->SubsetCount;
	hdr.iSkinCount = 1;
	hdr.iVersion = SX_MODEL_VERSION_OLD;

	hdr.iMaterialsOffset = sizeof(hdr);

	UINT iMaterialsSize = 0;
	for (UINT i = 0; i < hdr.iMaterialCount; i++)
	{
		iMaterialsSize += strlen(data->ArrTextures[i]) + 1;
	}

	hdr.iBonesOffset = 0;

	hdr.iLODoffset = hdr.iMaterialsOffset + iMaterialsSize;

	UINT iLodSize = 0;
	iLodSize += sizeof(int);
	for (int i = 0; i < hdr.iMaterialCount; i++)
	{
		iLodSize += sizeof(vertex_animated_ex)* data->VertexCount[i];
		iLodSize += sizeof(UINT)* data->IndexCount[i];
		iLodSize += sizeof(int)* 3;
	}

	hdr.iAnimationsOffset = 0;
	hdr.iSecondHeaderOffset = 0;

	fwrite(&hdr, sizeof(hdr), 1, pF);
	for (UINT i = 0; i < hdr.iMaterialCount; i++)
	{
		fwrite(data->ArrTextures[i], 1, strlen(data->ArrTextures[i]) + 1, pF);
	}
	int smc = hdr.iMaterialCount;
	fwrite(&smc, sizeof(int), 1, pF);
	vertex_animated_ex * pVB;
	UINT * pIB;
	data->VertexBuffer->Lock(0, 0, (void**)&pVB, 0);
	data->IndexBuffer->Lock(0, 0, (void**)&pIB, 0);
	UINT iVC = 0;
	UINT iIC = 0;
	for (int i = 0; i < hdr.iMaterialCount; i++)
	{
		fwrite(&i, sizeof(int), 1, pF);
		fwrite(&data->VertexCount[i], sizeof(int), 1, pF);
		fwrite(&data->IndexCount[i], sizeof(int), 1, pF);

		fwrite(pVB + iVC, sizeof(vertex_animated_ex), data->VertexCount[i], pF);
		fwrite(pIB + iIC, sizeof(UINT), data->IndexCount[i], pF);
		iVC += data->VertexCount[i];
		iIC += data->IndexCount[i];
	}
	data->VertexBuffer->Unlock();
	data->IndexBuffer->Unlock();
	fclose(pF);
}
