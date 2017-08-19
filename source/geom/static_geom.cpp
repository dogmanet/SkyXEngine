
#include <geom\\static_geom.h>

StaticGeom::Segment::Segment()
{
	for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
	{
		Splits[i] = 0;
	}

	ArrPoly = 0;
	CountPoly = 0;
	NumberGroup = 0;
	NumberGroupModel = 0;
	CountSubSet = 0;
	CountAllPoly = 0;
	BoundVolumeSys = 0;
	BoundVolumeP = 0;
	ID = -1;
	SID = -1;
	BFNonEnd = false;
}

StaticGeom::Segment::~Segment()
{
	for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; i++)
	{
		mem_delete(Splits[i]);
	}
	mem_delete_a(NumberGroup);
	mem_delete_a(NumberGroupModel);
	mem_delete_a(CountPoly);
	
	if (ArrPoly)
	{
		for (DWORD i = 0; i < CountSubSet; i++)
		{
			mem_delete_a(ArrPoly[i]);
		}
	}

	mem_delete_a(ArrPoly);
	mem_release_del(BoundVolumeSys);
	mem_release_del(BoundVolumeP);
	//ndx_release(BoundBox);
}

StaticGeom::Group::Group()
{
	name = "";
	idtex = -1;
	SortGroup = 0;
	AllCountVertex = AllCountIndex = 0;
}

/*StaticGeom::Group::VertexBuff::VertexBuff()
{
	arr = 0;
	count = 0;
}

StaticGeom::Group::VertexBuff::~VertexBuff()
{
	mem_delete_a(arr);
	count = 0;
}*/

StaticGeom::Group::~Group()
{
	idtex = -1;
	AllCountVertex = AllCountIndex = 0;
	CountVertex.clear();
	CountIndex.clear();

	for (long i = 0; i < VertexBuffer.size(); ++i)
	{
		mem_release_del(VertexBuffer[i]);
		//mem_delete(VertexBufferOrigin[i]);
	}
}

StaticGeom::Model::Lod::Lod()
{
	PathName[0] = 0;
	model = 0;
	BoundVolume = SGCore_CrBound();
}

StaticGeom::Model::Lod::~Lod()
{
	mem_release_del(model);
	mem_release_del(BoundVolume);
}

StaticGeom::Model::Model()
{
	SplitsIDs = SplitsIDsRender = 0;
	ArrSplits = 0;
	Position = float3(0,0,0);
	Rotation = float3(0,0,0);
	Scale = float3(1,1,1);

	OldPosition = Position;
	OldRotation = Rotation;
	OldScale = Scale;

	CountPoly = 0;

	Name[0] = 0;
	PathName[0] = 0;
	IsRenderLod = false;
}

StaticGeom::Model::~Model()
{
	SplitsIDs = SplitsIDsRender = 0;
	SubSet.clear();
	mem_delete(ArrSplits);
}

StaticGeom::InfoRenderSegments::InfoRenderSegments()
{
	Arr = 0; Count = 0; CountCom = 0;
}

StaticGeom::InfoRenderSegments::~InfoRenderSegments()
{
	mem_delete_a(Arr);
}

StaticGeom::IRSData::IRSData()
{

}

StaticGeom::IRSData::~IRSData()
{
	queue.clear();
	for (int i = 0; i < arr.size(); ++i)
	{
		mem_delete(arr[i]);
	}
	arr.clear();
}

StaticGeom::StaticGeom()
{
	SizeRenderIndexBuffer = 0;
	RenderIndexBuffer = 0;
	RTCPUArrIndicesPtrs = 0;

	WorldMat = SMMatrixIdentity();

	ArrMeshVertex = 0;
	ArrMeshIndex = 0;
	jvevyn = D3DXVECTOR3(0.0f, -1000.0f, 0.0f);
	jvevyp = D3DXVECTOR3(0.0f, 1000.0f, 0.0f);

	BoundVolume = SGCore_CrBound();

	/*D3DVERTEXELEMENT9 layoutstatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	StaticGeom::DXDevice->CreateVertexDeclaration(layoutstatic, &VertexDeclarationStatic);*/

	IRSData* tmparr = new IRSData();
	ArrComFor.push_back(tmparr);
	IRSData* tmparr2 = new IRSData();
	ArrComFor.push_back(tmparr2);
}

StaticGeom::~StaticGeom()
{
	for (long i = 0; i < ArrComFor.size(); ++i)
	{
		mem_delete(ArrComFor[i]);
	}

	ArrComFor.clear();
	mem_delete_a(ArrMeshVertex);
	mem_delete_a(ArrMeshIndex);
	mem_release(RenderIndexBuffer);
	//mem_release(VertexDeclarationStatic);
	mem_release(BoundVolume);

	DelArrIndexPtr();

	for (long i = 0; i < AllModels.size(); ++i)
	{
		mem_delete(AllModels[i]);
	}
	AllModels.clear();

	for (long i = 0; i < AllGroups.size(); ++i)
	{
		mem_delete(AllGroups[i]);
	}
	AllGroups.clear();
}

void StaticGeom::OnLostDevice()
{
	mem_release_del(RenderIndexBuffer);
}

void StaticGeom::OnResetDevice()
{
	StaticGeom::DXDevice->CreateIndexBuffer(sizeof(uint32_t)* SizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &RenderIndexBuffer, 0);
}

inline long StaticGeom::GetCountModel()
{
	return AllModels.size();
}

ID StaticGeom::AddModel(const char* path, const char* lod1, const char* name)
{
	reportf(0, "load geometry: '%s'.. ", path);
	DelArrIndexPtr();

	ISXDataStaticModel* model = 0;
	
	char tmppath[1024];
	sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path);
	SGCore_StaticModelLoad(tmppath, &model);

	Model* tmpmodel = new Model();
	ID tmpidmodel = AllModels.size() - 1;
	if (!def_str_validate(name))
		sprintf(tmpmodel->Name, "");
	else
		sprintf(tmpmodel->Name, "%s", name);

	sprintf(tmpmodel->PathName, "%s", path);

	bool isunictex = true;	//уникальна ли подгруппа в модели? (то есть, нет ли данной подгруппы в списке уже загруженных)

	vertex_static* pVert;
	UINT* pInd;

	if (FAILED(model->VertexBuffer->Lock(0, 0, (void**)&pVert, 0)))
		return -1;

	if (FAILED(model->IndexBuffer->Lock(0, 0, (void**)&pInd, 0)))
		return -1;

	for (int i = 0; i < model->SubsetCount; ++i)
	{
		tmpmodel->CountPoly += model->IndexCount[i] / 3;

		char tmptex[1024];
		sprintf(tmptex, "%s.dds", model->ArrTextures[i]);
		tmpmodel->IDTex[i] = SGCore_MtlLoad(tmptex, MTL_TYPE_GEOM);

		

		//если количество полигонов в подгруппе модели больше разрешенного
		if (model->IndexCount[i] / 3 > GEOM_MAX_POLY_IN_GROUP)
		{
			reportf(-1, "[GEOM] %s count polygons %d in group over default '%d'", gen_msg_location, model->IndexCount[i] / 3, GEOM_MAX_POLY_IN_GROUP);
			return -1;
		}

		isunictex = true;

		for (int k = 0; k < AllGroups.size(); ++k)
		{
			//сравниваем подгруппы только по имени, если имена одинаковые значит это одна и та же подгруппа
			if (stricmp(AllGroups[k]->name.c_str(), model->ArrTextures[i]) == 0)
			{
				isunictex = false;

				Model::GDataBuff gdb;

				gdb.idgroup = k;

				IDirect3DVertexBuffer9* vb;

				vertex_static * pData;
				vertex_static * pData2;

				//если последний незаполненный буфер не вмещает в себ¤
				if (AllGroups[k]->CountIndex[AllGroups[k]->CountIndex.size() - 1] / 3 + model->IndexCount[i] / 3 > GEOM_MAX_POLY_IN_GROUP)
				{
					//создаем новый
					gdb.VertexStart = 0;
					gdb.VertexCount = model->VertexCount[i];

					StaticGeom::DXDevice->CreateVertexBuffer(sizeof(vertex_static)* model->VertexCount[i], NULL, NULL, D3DPOOL_MANAGED, &vb, 0);
					
					/*Group::VertexBuff* vborigin = new Group::VertexBuff();
					vborigin->count = model->VertexCount[i];
					vborigin->arr = new float3_t[vborigin->count];
					for (long j = 0; j < model->VertexCount[i]; ++j)
					{
						vborigin->arr[j] = (model->ArrVertBuf + model->StartVertex[i] + j)->Pos;
					}
					AllGroups[k]->VertexBufferOrigin.push_back(vborigin);*/
					
					AllGroups[k]->AllCountVertex += model->VertexCount[i];
					AllGroups[k]->CountVertex.push_back(model->VertexCount[i]);
					//копируем данные в новый вершинный буфер
					if (!FAILED(vb->Lock(0, 0, (void**)&pData, 0)))
					{
						memcpy(pData, pVert + model->StartVertex[i], sizeof(vertex_static)* model->VertexCount[i]);
						vb->Unlock();
					}

					AllGroups[k]->VertexBuffer.push_back(vb);


					gdb.IndexStart = 0;
					gdb.IndexCount = model->IndexCount[i];
					AllGroups[k]->AllCountIndex += model->IndexCount[i];
					AllGroups[k]->CountIndex.push_back(model->IndexCount[i]);

					gdb.idbuff = AllGroups[k]->VertexBuffer.size()-1;

					AllGroups[k]->ArrModels[gdb.idbuff].push_back(tmpmodel);

					if (SizeRenderIndexBuffer < model->IndexCount[i])
					{
						mem_release(RenderIndexBuffer);
						SizeRenderIndexBuffer = model->IndexCount[i];
						StaticGeom::DXDevice->CreateIndexBuffer(sizeof(uint32_t)* SizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &RenderIndexBuffer, 0);
					}
				}
				//иначе если влезет в буфер
				else
				{
					//впихиваем в буфер
					gdb.idbuff = AllGroups[k]->VertexBuffer.size() - 1;
					Array<Model*>& tmparrmodel = AllGroups[k]->ArrModels[gdb.idbuff];
					AllGroups[k]->ArrModels[gdb.idbuff].push_back(tmpmodel);
					long lastvbnum = AllGroups[k]->VertexBuffer.size() - 1;

					StaticGeom::DXDevice->CreateVertexBuffer(sizeof(vertex_static)* (AllGroups[k]->CountVertex[lastvbnum] + model->VertexCount[i]), NULL, NULL, D3DPOOL_MANAGED, &vb, 0);
					
					/*Group::VertexBuff* vborigin = new Group::VertexBuff();
					vborigin->count = (AllGroups[k]->CountVertex[lastvbnum] + model->VertexCount[i]);
					vborigin->arr = new float3_t[vborigin->count];
					memcpy(vborigin->arr, AllGroups[k]->VertexBufferOrigin[lastvbnum]->arr, sizeof(float3_t) * AllGroups[k]->CountVertex[lastvbnum]);
					for (long j = 0; j < model->VertexCount[i]; ++j)
					{
						vborigin->arr[AllGroups[k]->CountVertex[lastvbnum]+j] = (model->ArrVertBuf + model->StartVertex[i] + j)->Pos;
					}
					mem_delete(AllGroups[k]->VertexBufferOrigin[lastvbnum]);
					AllGroups[k]->VertexBufferOrigin[lastvbnum] = vborigin;*/

					gdb.VertexStart = AllGroups[k]->CountVertex[lastvbnum];
					gdb.VertexCount = model->VertexCount[i];
					
					if (!FAILED(vb->Lock(0, 0, (void**)&pData, 0)))
					{
						if (!FAILED(AllGroups[k]->VertexBuffer[lastvbnum]->Lock(0, 0, (void**)&pData2, 0)))
						{
							memcpy(pData, pData2, sizeof(vertex_static)* AllGroups[k]->CountVertex[lastvbnum]);
							AllGroups[k]->VertexBuffer[lastvbnum]->Unlock();
						}

						memcpy(pData + AllGroups[k]->CountVertex[lastvbnum], pVert + model->StartVertex[i], sizeof(vertex_static)* (model->VertexCount[i]));
						vb->Unlock();
					}

					AllGroups[k]->AllCountVertex += model->VertexCount[i];
					AllGroups[k]->CountVertex[lastvbnum] += model->VertexCount[i];
					mem_release(AllGroups[k]->VertexBuffer[lastvbnum]);
					AllGroups[k]->VertexBuffer[lastvbnum] = vb;

					gdb.IndexStart = AllGroups[k]->CountIndex[lastvbnum];
					gdb.IndexCount = model->IndexCount[i];

					AllGroups[k]->AllCountIndex += model->IndexCount[i];
					AllGroups[k]->CountIndex[lastvbnum] += model->IndexCount[i];

					if (SizeRenderIndexBuffer < (AllGroups[k]->CountIndex[lastvbnum] + model->IndexCount[i]))
					{
						mem_release(RenderIndexBuffer);
						SizeRenderIndexBuffer = (AllGroups[k]->CountIndex[lastvbnum] + model->IndexCount[i]);
						StaticGeom::DXDevice->CreateIndexBuffer(sizeof(uint32_t)* SizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &RenderIndexBuffer, 0);
					}
				}

				tmpmodel->SubSet.push_back(gdb);
			}
		}

		if (isunictex)
		{
			Group* ngroup = new Group();
			Model::GDataBuff gdb;

			ngroup->ArrModels[0].push_back(tmpmodel);

			gdb.idbuff = 0;
			gdb.idgroup = AllGroups.size();
			gdb.VertexStart = 0;
			gdb.VertexCount = model->VertexCount[i];
			gdb.IndexStart = 0;
			gdb.IndexCount = model->IndexCount[i];

			tmpmodel->SubSet.push_back(gdb);

			IDirect3DVertexBuffer9* vb;
			StaticGeom::DXDevice->CreateVertexBuffer(sizeof(vertex_static)* model->VertexCount[i], NULL, NULL, D3DPOOL_MANAGED, &vb, 0);
			
			/*Group::VertexBuff* vborigin = new Group::VertexBuff();
			vborigin->count = model->VertexCount[i];
			vborigin->arr = new float3_t[vborigin->count];

			for (long k = 0; k < model->VertexCount[i]; ++k)
			{
				vborigin->arr[k] = (model->ArrVertBuf + model->StartVertex[i] + k)->Pos;
			}*/

			ngroup->AllCountVertex = model->VertexCount[i];
			ngroup->CountVertex.push_back(model->VertexCount[i]);
			vertex_static * pData;
			if (!FAILED(vb->Lock(0, 0, (void**)&pData, 0)))
			{
				memcpy(pData, pVert + model->StartVertex[i], sizeof(vertex_static)* model->VertexCount[i]);
				vb->Unlock();
			}

			ngroup->AllCountIndex = model->IndexCount[i];
			ngroup->CountIndex.push_back(model->IndexCount[i]);
			
			ngroup->VertexBuffer.push_back(vb);
			//ngroup->VertexBufferOrigin.push_back(vborigin);
			
			if (SizeRenderIndexBuffer < model->IndexCount[i])
			{
				mem_release(RenderIndexBuffer);
				SizeRenderIndexBuffer = model->IndexCount[i];
				StaticGeom::DXDevice->CreateIndexBuffer(sizeof(uint32_t)* SizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &RenderIndexBuffer, 0);
			}

			//char tmptex[1024];
			//sprintf(tmptex, "%s.dds", model->ArrTextures[i]);

			ngroup->idtex = tmpmodel->IDTex[i];// SGCore_LoadMtl(tmptex, MTL_GEOM);
			ngroup->SortGroup = SGCore_MtlGetSort(ngroup->idtex);
			ngroup->IsRenderSingly = SGCore_MtlGroupRenderIsSingly(ngroup->idtex);
			ngroup->name = model->ArrTextures[i];
			AllGroups.push_back(ngroup);
		}


		vertex_static* pData;
		model->VertexBuffer->Lock(0, 0, (void**)&pData, 0);

		float3_t tmppos = pData[model->StartVertex[i]].Pos;
		float3 tmpMax = tmppos;
		float3 tmpMin = tmppos;
		for (DWORD k = 0; k<model->VertexCount[i]; k++)
		{
			tmppos = pData[model->StartVertex[i] + k].Pos;

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
		model->IndexBuffer->Lock(0, 0, (void **)&indeces, 0);

		float3 tmpMM = SMVectorLerp(tmpMax, tmpMin, 0.5f);

		D3DXPlaneFromPoints(&tmpmodel->SubSet[i].Plane,
				&D3DXVECTOR3(pData[indeces[model->StartIndex[i] + 0]].Pos.x, pData[indeces[model->StartIndex[i] + 0]].Pos.y, pData[indeces[model->StartIndex[i] + 0]].Pos.z),
				&D3DXVECTOR3(pData[indeces[model->StartIndex[i] + 1]].Pos.x, pData[indeces[model->StartIndex[i] + 1]].Pos.y, pData[indeces[model->StartIndex[i] + 1]].Pos.z),
				&D3DXVECTOR3(pData[indeces[model->StartIndex[i] + 2]].Pos.x, pData[indeces[model->StartIndex[i] + 2]].Pos.y, pData[indeces[model->StartIndex[i] + 2]].Pos.z));
		model->VertexBuffer->Unlock();
		model->IndexBuffer->Unlock();

		tmpmodel->SubSet[i].Center = (float3_t)((tmpMax + tmpMin) * 0.5);
		tmpmodel->SubSet[i].Min = tmpMin;
		tmpmodel->SubSet[i].Max = tmpMax;
	}

	model->VertexBuffer->Unlock();
	model->IndexBuffer->Unlock();

	AllModels.push_back(tmpmodel);

	SetModelLodPath(AllModels.size() - 1, lod1);
	
	PreSegmentation(tmpmodel, model);

	SetSplitID(tmpmodel->ArrSplits, &tmpmodel->SplitsIDs, &tmpmodel->SplitsIDsRender);

	AddModelInArrCom(AllModels.size() - 1);

	InitArrIndexPtr();

	float3 jmin, jmax, jmin2, jmax2;

	AllModels[AllModels.size()-1]->ArrSplits->BoundVolumeP->GetMinMax(&jmin, &jmax);
	BoundVolume->GetMinMax(&jmin2, &jmax2);

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

	BoundVolume->SetMinMax(&jmin2, &jmax2);
	
	mem_release_del(model);

	mem_delete_a(ArrMeshVertex);
	mem_delete_a(ArrMeshIndex);

	reportf(0, " completed\n", path);
	return AllModels.size() - 1;
}

void StaticGeom::DelModel(ID id)
{
	if (id >= AllModels.size())
		return;

	Model* tmpmodel = AllModels[id];

	//ПРЕЖДЕ НАДО ОЧИСТИТЬ БУФЕРЫ КОТОРЫЕ ЗАПОЛНЯЮТСЯ ИНДЕКСНЫМИ ДАННЫМИ, ИБО ПОСЛЕ УДАЛЕНИЯ МОДЕЛИ ДАННЫЕ ДЛЯ ОЧИТСКИ БУДУТ УЖЕ НЕКОРРЕТНЫЕ
	DelArrIndexPtr();

	vertex_static* pData, *pData2, *pData3;
	ID idgroup;
	ID idbuff;

	for (long i = 0; i < tmpmodel->SubSet.size(); ++i)
	{
		idgroup = tmpmodel->SubSet[i].idgroup;
		idbuff = tmpmodel->SubSet[i].idbuff;

		long tmpcountvertexi = AllGroups[idgroup]->CountVertex[idbuff];
		long tmpcountvertex = tmpmodel->SubSet[i].VertexCount;
		long tmpcountstartvertex = tmpmodel->SubSet[i].VertexStart;

		//если количество удаляемых вершин меньше чем общее количество вершин в буфере подгруппы
		if (AllGroups[idgroup]->CountVertex[idbuff] > tmpmodel->SubSet[i].VertexCount)
		{
			//производим удаление, путем пересоздания есесно
			IDirect3DVertexBuffer9* vb;
			StaticGeom::DXDevice->CreateVertexBuffer(sizeof(vertex_static)* (AllGroups[idgroup]->CountVertex[idbuff] - tmpmodel->SubSet[i].VertexCount), NULL, NULL, D3DPOOL_MANAGED, &vb, 0);

			/*Group::VertexBuff* vborigin = new Group::VertexBuff();
			vborigin->count = (AllGroups[idgroup]->CountVertex[idbuff] - tmpmodel->SubSet[i].VertexCount);
			vborigin->arr = new float3_t[vborigin->count];*/

			vb->Lock(0, 0, (void**)&pData, 0);
			AllGroups[idgroup]->VertexBuffer[idbuff]->Lock(0, 0, (void**)&pData2, 0);
			long tmpvertexstart = tmpmodel->SubSet[i].VertexStart;
			memcpy(pData, pData2, sizeof(vertex_static)* tmpmodel->SubSet[i].VertexStart);
			//memcpy(vborigin->arr, AllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr, sizeof(float3_t)* tmpmodel->SubSet[i].VertexStart);
			if (AllGroups[idgroup]->CountVertex[idbuff] - (tmpmodel->SubSet[i].VertexStart + tmpmodel->SubSet[i].VertexCount) > 0)
			{
				memcpy(pData + tmpmodel->SubSet[i].VertexStart, pData2 + tmpmodel->SubSet[i].VertexStart + tmpmodel->SubSet[i].VertexCount, sizeof(vertex_static)* (AllGroups[idgroup]->CountVertex[idbuff] - (tmpmodel->SubSet[i].VertexStart + tmpmodel->SubSet[i].VertexCount)));
				//memcpy(vborigin->arr + tmpmodel->SubSet[i].VertexStart, AllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr + tmpmodel->SubSet[i].VertexStart + tmpmodel->SubSet[i].VertexCount, sizeof(float3_t)* (AllGroups[idgroup]->CountVertex[idbuff] - (tmpmodel->SubSet[i].VertexStart + tmpmodel->SubSet[i].VertexCount)));

			}

			vb->Unlock();
			AllGroups[idgroup]->VertexBuffer[idbuff]->Unlock();

			//обновляем данные и производим замену
			AllGroups[idgroup]->AllCountVertex -= tmpmodel->SubSet[i].VertexCount;
			AllGroups[idgroup]->CountVertex[idbuff] -= tmpmodel->SubSet[i].VertexCount;

			AllGroups[idgroup]->AllCountIndex -= tmpmodel->SubSet[i].IndexCount;
			AllGroups[idgroup]->CountIndex[idbuff] -= tmpmodel->SubSet[i].IndexCount;

			mem_release(AllGroups[idgroup]->VertexBuffer[idbuff]);
			AllGroups[idgroup]->VertexBuffer[idbuff] = vb;

			/*mem_delete(AllGroups[idgroup]->VertexBufferOrigin[idbuff]);
			AllGroups[idgroup]->VertexBufferOrigin[idbuff] = vborigin;*/
		}
		
		//если количество моделей в массиве больше одной
		if (AllGroups[idgroup]->ArrModels[idbuff].size() > 1)
		{
			//находим удаляемую модель в массиве моделей в буфере данной подгруппы
			long tmpidingroup = -1;	//id модели в текущем массиве, в подгруппе
			for (int k = 0; k < AllGroups[idgroup]->ArrModels[idbuff].size(); ++k)
			{
				Model* tmptmpmodel = AllGroups[idgroup]->ArrModels[idbuff][k];
				if (AllGroups[idgroup]->ArrModels[idbuff][k] == tmpmodel)
				{
					tmpidingroup = k;
					break;
				}
			}

			//если модель не была найдена в массиве моделей
			if (tmpidingroup == -1)
			{
				//то это очень плохо
				reportf(REPORT_MSG_LEVEL_ERROR, "[GEOM] %s - when deleting the model, it was found in an array of patterns in subgroups of the buffer, it is very bad", gen_msg_location);
			}

			//обновляем данные в подгруппе модели о стартовых позициях
			for (int k = tmpidingroup + 1; k < AllGroups[idgroup]->ArrModels[idbuff].size(); ++k)
			{
				Model* tmptmpmodel = AllGroups[idgroup]->ArrModels[idbuff][k];
				for (int j = 0; j < AllGroups[idgroup]->ArrModels[idbuff][k]->SubSet.size(); ++j)
				{
					if (tmptmpmodel->SubSet[j].idgroup == idgroup)
					{
						tmptmpmodel->SubSet[j].IndexStart -= tmpmodel->SubSet[i].IndexCount;
						tmptmpmodel->SubSet[j].VertexStart -= tmpmodel->SubSet[i].VertexCount;

						break;
					}
				}

				//обновляем также все индексы во всех сплитах
				Array<Segment*, GEOM_DEFAULT_RESERVE_COM> queue;
				queue.push_back(AllGroups[idgroup]->ArrModels[idbuff][k]->ArrSplits);

				while (queue.size())
				{
					if (queue[0]->BFNonEnd)
					{
						for (int j = 0; j < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
						{
							if (queue[0]->Splits[j])
								queue.push_back(queue[0]->Splits[j]);
						}
					}
					else
					{
						for (int j = 0; j < queue[0]->CountSubSet; ++j)
						{
							if (queue[0]->NumberGroup[j] == idgroup )
							{
								for (int q = 0; q < queue[0]->CountPoly[j]*3; ++q)
								{
									queue[0]->ArrPoly[j][q] -= tmpmodel->SubSet[i].VertexCount;
								}
							}
						}
					}

					queue.erase(0);
				}
			}

			//удал¤ем текущую удал¤емую модель из массива моделей буфера текущей подгруппы
			AllGroups[idgroup]->ArrModels[idbuff].erase(tmpidingroup);
		}
		//если в буфере подгруппы всего одна модель, то значит эта та модель которую удаляем
		else if (AllGroups[idgroup]->ArrModels[idbuff].size() == 1)
			AllGroups[idgroup]->ArrModels[idbuff].erase(0);
		
		//если количество удалемых вершин больше либо равно количеству имеющихся в буфере подгруппы
		if (tmpcountvertexi <= tmpcountvertex)
		{
			//то теряется смысл данного буфера, и надо его удалить

			Group* tmpgroup = AllGroups[idgroup];

			tmpgroup->CountVertex.erase(idbuff);
			tmpgroup->CountIndex.erase(idbuff);
			mem_release(tmpgroup->VertexBuffer[idbuff]);
			//IDirect3DVertexBuffer9* vb = AllGroups[idgroup]->VertexBuffer[idbuff];
			tmpgroup->VertexBuffer.erase(idbuff);
			tmpgroup->ArrModels.erase(idbuff);
			//Group::VertexBuff* tmpduff = AllGroups[idgroup]->VertexBufferOrigin[idbuff];
			/*mem_delete(tmpgroup->VertexBufferOrigin[idbuff]);
			tmpgroup->VertexBufferOrigin.erase(idbuff);*/
		}

		//если подгруппа пустая, то обрабатываем все значения что больше идентификатора группы
		if (AllGroups[idgroup]->CountVertex.size() <= 0)
		{
			//Group* tmpgroup = AllGroups[idgroup];
			
			//то есть декрементируем все идентфиикаторы подгрупп которые больше чем текущая удаляемая подгруппа
			//декремент произодим и у текущей удаляемой модели, ибо все будет смещено, а значит и в этой моделе должно быть все смещенно для корретных просчетов
			for (int k = 0; k < AllModels.size(); ++k)
			{
				Model* tmptmpmodel = AllModels[k];

				for (long j = 0; j < tmptmpmodel->SubSet.size(); ++j)
				{
					if (tmptmpmodel->SubSet[j].idgroup > idgroup)
						--(tmptmpmodel->SubSet[j].idgroup);
				}

				//также проходимся и по всем сплитам чтобы и там произвести декремент
				Array<Segment*, GEOM_DEFAULT_RESERVE_COM> queue;
				queue.push_back(tmptmpmodel->ArrSplits);

				while (queue.size())
				{
					if (queue[0]->BFNonEnd)
					{
						for (int j = 0; j < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
						{
							if (queue[0]->Splits[j])
								queue.push_back(queue[0]->Splits[j]);
						}
					}
					else
					{
						for (int j = 0; j < queue[0]->CountSubSet; ++j)
						{
							if (queue[0]->NumberGroup[j] > idgroup)
								--(queue[0]->NumberGroup[j]);
						}
					}

					queue.erase(0);
				}
			}

			//удаляем подгруппу, за ненадобностью
			mem_delete(AllGroups[idgroup]);
			AllGroups.erase(idgroup);
		}
		//если подгруппа не пустая то может быть тогда можно где-то совместить буферы?
		else
		{
			for (long k = 0; k < AllGroups[idgroup]->VertexBuffer.size(); ++k)
			{
				//можно ли совместить буферы
				if (idbuff != k && AllGroups[idgroup]->CountIndex[idbuff] / 3 + AllGroups[idgroup]->CountIndex[k] / 3 <= GEOM_MAX_POLY_IN_GROUP)
				{
					//СОВМЕЩАЕМ ТЕКУЩИЙ ОБРАБАТЫВАЕМЫЙ БУФЕР (idbuff) (ИЗ КОТОРОГО МОДЕЛЬ БЫЛА УДАЛЕНА С БУФЕРОМ В КОТОРЙ ОН ВМЕЩАЕТСЯ (k)
					//idbuff => k

					//производим совмещение данных в буферах
					//{
					IDirect3DVertexBuffer9* vb;
					StaticGeom::DXDevice->CreateVertexBuffer(sizeof(vertex_static)* (AllGroups[idgroup]->CountVertex[idbuff] + AllGroups[idgroup]->CountVertex[k]), NULL, NULL, D3DPOOL_MANAGED, &vb, 0);

					/*Group::VertexBuff* vborigin = new Group::VertexBuff();
					vborigin->count = (AllGroups[idgroup]->CountVertex[idbuff] + AllGroups[idgroup]->CountVertex[k]);
					vborigin->arr = new float3_t[vborigin->count];*/

					vb->Lock(0, 0, (void**)&pData, 0);
					AllGroups[idgroup]->VertexBuffer[idbuff]->Lock(0, 0, (void**)&pData2, 0);
					AllGroups[idgroup]->VertexBuffer[k]->Lock(0, 0, (void**)&pData3, 0);

					memcpy(pData, pData3, sizeof(vertex_static)* AllGroups[idgroup]->CountVertex[k]);
					//memcpy(vborigin->arr, AllGroups[idgroup]->VertexBufferOrigin[k]->arr, sizeof(float3_t)* AllGroups[idgroup]->CountVertex[k]);

					memcpy(pData + AllGroups[idgroup]->CountVertex[k], pData2, sizeof(vertex_static)* AllGroups[idgroup]->CountVertex[idbuff]);
					//memcpy(vborigin->arr + AllGroups[idgroup]->CountVertex[k], AllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr, sizeof(float3_t)* AllGroups[idgroup]->CountVertex[idbuff]);

					vb->Unlock();
					AllGroups[idgroup]->VertexBuffer[idbuff]->Unlock();
					AllGroups[idgroup]->VertexBuffer[k]->Unlock();
					//}

					//обновляем данные во всех моделях которые есть в текущем буфере
					for (long m = 0; m < AllGroups[idgroup]->ArrModels[idbuff].size(); ++m)
					{
						long tmpsizearrmodls = AllGroups[idgroup]->ArrModels[idbuff].size();
						Model* tmptmpmodel = AllGroups[idgroup]->ArrModels[idbuff][m];
						//обновляем данные в структуре инфе о перемещенном буфере
						for (long j = 0; j < tmptmpmodel->SubSet.size(); ++j)
						{
							if (tmptmpmodel->SubSet[j].idgroup == idgroup)
							{
								//а именно, текущий буфер
								if (idbuff > k)
									tmptmpmodel->SubSet[j].idbuff = k;
								else
									tmptmpmodel->SubSet[j].idbuff = k - 1;

								//и стартовые позиции
								tmptmpmodel->SubSet[j].IndexStart += AllGroups[idgroup]->CountIndex[k];
								tmptmpmodel->SubSet[j].VertexStart += AllGroups[idgroup]->CountVertex[k];
								break;
							}
						}

						//обновляем все индексы модели текущей подгруппы перемещенного буфера
						//{
						Array<Segment*, GEOM_DEFAULT_RESERVE_COM> queue;
						queue.push_back(tmptmpmodel->ArrSplits);

						while (queue.size())
						{
							if (queue[0]->BFNonEnd)
							{
								for (int j = 0; j < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
								{
									if (queue[0]->Splits[j])
										queue.push_back(queue[0]->Splits[j]);
								}
							}
							else
							{
								for (int j = 0; j < queue[0]->CountSubSet; ++j)
								{
									if (queue[0]->NumberGroup[j] == idgroup)
									{
										for (int q = 0; q < queue[0]->CountPoly[j] * 3; ++q)
										{
											queue[0]->ArrPoly[j][q] += AllGroups[idgroup]->CountVertex[k];
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
					for (long j = idbuff+1; j < AllGroups[idgroup]->ArrModels.size(); ++j)
					{
						for (long q = 0; q < AllGroups[idgroup]->ArrModels[j].size(); ++q)
						{
							for (long g = 0; g < AllGroups[idgroup]->ArrModels[j][q]->SubSet.size(); ++g)
							{
								if (AllGroups[idgroup]->ArrModels[j][q]->SubSet[g].idgroup == idgroup && AllGroups[idgroup]->ArrModels[j][q]->SubSet[g].idbuff > idbuff)
								{
									--(AllGroups[idgroup]->ArrModels[j][q]->SubSet[g].idbuff);
								}
							}
						}
					}
					//}

					//обновляем информацию в k буфере, по ходу удаляем информацию в idbuff буфере
					//{
					AllGroups[idgroup]->CountVertex[k] += AllGroups[idgroup]->CountVertex[idbuff];
					AllGroups[idgroup]->CountIndex[k] += AllGroups[idgroup]->CountIndex[idbuff];

					mem_release(AllGroups[idgroup]->VertexBuffer[k]);
					mem_release(AllGroups[idgroup]->VertexBuffer[idbuff]);
					AllGroups[idgroup]->VertexBuffer[k] = vb;

					/*mem_delete(AllGroups[idgroup]->VertexBufferOrigin[k]);
					mem_delete(AllGroups[idgroup]->VertexBufferOrigin[idbuff]);
					AllGroups[idgroup]->VertexBufferOrigin[k] = vborigin;*/

					//перемещаем все модели из idbuff буфера в тот с которым совмещаем
					for (long m = 0; m < AllGroups[idgroup]->ArrModels[idbuff].size(); ++m)
						AllGroups[idgroup]->ArrModels[k].push_back(AllGroups[idgroup]->ArrModels[idbuff][m]);

					AllGroups[idgroup]->CountVertex.erase(idbuff);
					AllGroups[idgroup]->CountIndex.erase(idbuff);

					AllGroups[idgroup]->ArrModels.erase(idbuff);
					
					AllGroups[idgroup]->VertexBuffer.erase(idbuff);
					//AllGroups[idgroup]->VertexBufferOrigin.erase(idbuff);
					//}
					
					//ПРЕРЫВАЕМ ЦИКЛ, ТАК КАК УЖЕ ОБЪЕДИНИЛИ И ЭТОГО ХВАТИТ
					break;
				}
			}
		}
	}

	//удаляем модель из просчетов, как объект, и из массива тоже
	DelModelInArrCom(id);
	mem_delete(AllModels[id]);
	AllModels.erase(id);

	//реинициализируем буферы для индексных данных рендера
	InitArrIndexPtr();
}

void StaticGeom::SetSplitID(Segment* Split, ID* SplitsIDs, ID* SplitsIDsRender)
{
	Array<Segment*> queue;
	long tmpcount = 0;
	queue.push_back(Split);

	while (queue.size())
	{
		if (queue[0] && queue[0]->BFNonEnd)
		{
			queue[0]->ID = (*SplitsIDs);
			++(*SplitsIDs);
			for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; i++)
				queue.push_back(queue[0]->Splits[i]);
		}
		else if (queue[0])
		{
			queue[0]->ID = (*SplitsIDs);
			queue[0]->SID = (*SplitsIDsRender);
			++(*SplitsIDs);
			++(*SplitsIDsRender);
		}
		
		queue.erase(0);
		++tmpcount;
	}
}

void StaticGeom::DelArrIndexPtr()
{
	if (RTCPUArrIndicesPtrs)
	{
		for (long i = 0; i<AllGroups.size(); i++)
		{
			for (long k = 0; k < AllGroups[i]->VertexBuffer.size(); ++k)
			{
				mem_delete_a(RTCPUArrIndicesPtrs[i][k]);
				mem_delete_a(RTCountDrawPolyModel[i][k]);
				mem_delete_a(RTBeginIndexModel[i][k]);
			}
			mem_delete_a(RTCountDrawPoly[i]);
			mem_delete_a(RTCountDrawPolyModel[i]);
			mem_delete_a(RTBeginIndexModel[i]);
			mem_delete_a(RTCPUArrIndicesPtrs[i]);
		}

		mem_delete_a(RTCountDrawPoly);
		mem_delete_a(RTCountDrawPolyModel);
		mem_delete_a(RTBeginIndexModel);
		mem_delete_a(RTCPUArrIndicesPtrs);
	}
}

void StaticGeom::InitArrIndexPtr()
{
	RTCPUArrIndicesPtrs = new uint32_t**[AllGroups.size()];
	RTCountDrawPoly = new uint32_t*[AllGroups.size()];
	RTCountDrawPolyModel = new uint32_t**[AllGroups.size()];
	RTBeginIndexModel = new uint32_t**[AllGroups.size()];

	for (DWORD i = 0; i<AllGroups.size(); i++)
	{
		RTCountDrawPoly[i] = new uint32_t[AllGroups[i]->VertexBuffer.size()];
		RTCPUArrIndicesPtrs[i] = new uint32_t*[AllGroups[i]->VertexBuffer.size()];
		RTCountDrawPolyModel[i] = new uint32_t*[AllGroups[i]->VertexBuffer.size()];
		RTBeginIndexModel[i] = new uint32_t*[AllGroups[i]->VertexBuffer.size()];

			for (long k = 0; k < AllGroups[i]->VertexBuffer.size(); ++k)
			{
				RTCPUArrIndicesPtrs[i][k] = new uint32_t[AllGroups[i]->CountIndex[k]];
				RTCountDrawPolyModel[i][k] = new uint32_t[AllGroups[i]->ArrModels[k].size()];
				RTBeginIndexModel[i][k] = new uint32_t[AllGroups[i]->ArrModels[k].size()];
			}
	}
}


void StaticGeom::CPUFillingArrIndeces(ISXFrustum* frustum, float3* viewpos, ID id_arr)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	DWORD tmpcount = 0;
	DWORD* tmpcountcom = 0;
	Segment** tmpsegments = 0;
	float jradius;
	float3 jcenter;

	for(int i = 0, l = AllModels.size(); i < l; ++i)
	{
		if (id_arr == 0)
		{
			AllModels[i]->ArrSplits->BoundVolumeP->GetSphere(&jcenter, &jradius);
			AllModels[i]->ArrSplits->DistForCamera = SMVector3Length((jcenter - (*viewpos))) - jradius;
		}

		if (AllModels[i]->ArrSplits->DistForCamera >= StaticGeom::DistForLod && AllModels[i]->Lod0.model)
			AllModels[i]->IsRenderLod = true;
		else
		{
			AllModels[i]->IsRenderLod = false;
			tmpcount = ArrComFor[id_arr]->arr[i]->Count;
			ArrComFor[id_arr]->arr[i]->CountCom = 0;
			tmpcountcom = &(ArrComFor[id_arr]->arr[i]->CountCom);
			tmpsegments = ArrComFor[id_arr]->arr[i]->Arr;

			ArrComFor[id_arr]->queue.push_back(AllModels[i]->ArrSplits);

			while (ArrComFor[id_arr]->queue.size())
			{
				ComRecArrIndeces(frustum, tmpsegments, tmpcountcom, ArrComFor[id_arr]->queue[0], viewpos, &ArrComFor[id_arr]->queue, tmpcount);

				ArrComFor[id_arr]->queue.erase(0);
				++tmpcount;
			}
		}

		int qwert = 0;
	}
}

void StaticGeom::ComRecArrIndeces(ISXFrustum* frustum, Segment** arrsplits, DWORD *count, Segment* comsegment, float3* viewpos, Array<Segment*, GEOM_DEFAULT_RESERVE_COM>* queue, ID curr_splits_ids_render)
{
	float jradius;
	float3 jcenter;
	comsegment->BoundVolumeP->GetSphere(&jcenter, &jradius);

	if (comsegment->CountAllPoly > 0 && (frustum->SphereInFrustum(&jcenter, jradius)))
	{
			if (comsegment->BFNonEnd )
			{
				if (StaticGeom::UseSortFrontToBackSplits)
				{
					long SortId[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];
					float DistFor[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];

					for (int q = 0; q<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++q)
					{
						SortId[q] = -1;
						if (comsegment->Splits[q])
						{
							comsegment->Splits[q]->BoundVolumeP->GetSphere(&jcenter, &jradius);
							DistFor[q] = SMVector3Length2((jcenter - (*viewpos))) - jradius*jradius;
						}
					}

					float pl1, pl2;
					int tmpCountGreater = 0;
					for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
					{
						if (comsegment->Splits[i])
						{
							tmpCountGreater = 0;
							pl1 = DistFor[i];

							for (int k = 0; k<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++k)
							{
								if (comsegment->Splits[k])
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
						if (SortId[(GEOM_COUNT_TYPE_SEGMENTATION_OCTO - 1) - j] != -1 && comsegment->Splits[SortId[(GEOM_COUNT_TYPE_SEGMENTATION_OCTO - 1) - j]])
							queue->push_back(comsegment->Splits[SortId[(GEOM_COUNT_TYPE_SEGMENTATION_OCTO - 1) - j]]);
					}
				}
				else
				{
					for (int j = 0; j<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
					{
						if (comsegment->Splits[j])
							queue->push_back(comsegment->Splits[j]);
					}
				}
			}
			else
			{
				if ((*count) < curr_splits_ids_render)
				{
					arrsplits[(*count)] = comsegment;
					comsegment->DistForCamera = SMVector3Length((jcenter - (*viewpos))) - jradius;
					(*count)++;
				}
				/*else
				{
					int qwert = 0;
				}*/
			}
	}
}

bool StaticGeom::SortExistsForRender(int sort, ID id_arr)
{
	//валиден ли id_arr?
	STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	Segment** jarrsplits;
	ID jidbuff;
	ID jnumgroup;

	//проходимся по всем моделям
	for (int i = 0, l = AllModels.size(); i < l; ++i)
	{
		if (ArrComFor[id_arr]->arr[i]->CountCom > 0)
		{
			for (int j = 0, jl = ArrComFor[id_arr]->arr[i]->CountCom; j<jl; j++)
			{
				jarrsplits = ArrComFor[id_arr]->arr[i]->Arr;
				for (int k = 0; k<jarrsplits[j]->CountSubSet; ++k)
				{
					jidbuff = AllModels[i]->SubSet[jarrsplits[j]->NumberGroupModel[k]].idbuff;
					jnumgroup = jarrsplits[j]->NumberGroup[k];

					if (SGCore_MtlGetSort(AllGroups[jnumgroup]->idtex) == sort)
						return true;
				}
			}
		}
	}

	return false;
}

void StaticGeom::GPURender(DWORD timeDelta, int sort_mtl, ID id_arr, ID exclude_model_id, ID exclude_group_id, bool is_sorted)
{
	//валиден ли id_arr?
	STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	Segment** jarrsplits;
	ID jidbuff;
	ID jnumgroup;
	
	//обнуляем все данные об отрисованном в прошлый раз
	for (int i = 0; i < AllGroups.size(); i++)
	{
		if (AllGroups[i])
		{
			for (int k = 0; k < AllGroups[i]->VertexBuffer.size(); ++k)
			{
				RTCountDrawPoly[i][k] = 0;

				for (int j = 0; j < AllGroups[i]->ArrModels[k].size(); ++j)
				{
					RTCountDrawPolyModel[i][k][j] = 0;
					RTBeginIndexModel[i][k][j] = -1;
				}
			}
		}
	}
	
	//проходимся по всем моделям
	for(int i = 0, l = AllModels.size(); i < l; ++i)
	{
		if (exclude_model_id == i && exclude_group_id < 0)
			continue;

		if (AllModels[i]->IsRenderLod)
		{
			StaticGeom::DXDevice->SetStreamSource(0, AllModels[i]->Lod0.model->VertexBuffer, 0, sizeof(vertex_static));
			StaticGeom::DXDevice->SetIndices(AllModels[i]->Lod0.model->IndexBuffer);
			StaticGeom::DXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());
			for(int k = 0, kl = AllModels[i]->Lod0.model->SubsetCount; k < kl; ++k)
			{
				if (AllModels[i]->Lod0.SortGroup == sort_mtl || sort_mtl == -1)
				{
					SGCore_MtlSet(AllModels[i]->Lod0.IDsTexs[k], 0);
					SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, AllModels[i]->Lod0.model->VertexCount[k], AllModels[i]->Lod0.model->StartIndex[k], AllModels[i]->Lod0.model->IndexCount[k] / 3);
					Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + AllModels[i]->Lod0.model->IndexCount[k] / 3);
				}
			}
		}
		else if (ArrComFor[id_arr]->arr[i]->CountCom > 0)
		{
			for(DWORD j = 0, jl = ArrComFor[id_arr]->arr[i]->CountCom; j<jl; j++)
			{
				jarrsplits = ArrComFor[id_arr]->arr[i]->Arr;
				for (DWORD k = 0; k<jarrsplits[j]->CountSubSet; ++k)
				{
					jidbuff = AllModels[i]->SubSet[jarrsplits[j]->NumberGroupModel[k]].idbuff;
					jnumgroup = jarrsplits[j]->NumberGroup[k];

					if (exclude_model_id >= 0 && exclude_group_id == jarrsplits[j]->NumberGroupModel[k])
						continue;

					ID tmpcurrmodel = -1;

					for(int q = 0, ql = AllGroups[jnumgroup]->ArrModels[jidbuff].size(); q<ql; ++q)
					{
						if (AllGroups[jnumgroup]->ArrModels[jidbuff][q] == AllModels[i])
						{
							tmpcurrmodel = q;
							break;
						}
					}

					if (tmpcurrmodel == -1)
					{
						int qwert = 0;
					}

					int currsort = AllGroups[jnumgroup]->SortGroup;

					if (AllGroups[jnumgroup]->SortGroup == sort_mtl || sort_mtl == -1)
					{
						if (
							jarrsplits[j]->CountPoly[k] > 0 &&	//если количество полигонов больше 0
							RTCountDrawPoly[jnumgroup][jidbuff] + jarrsplits[j]->CountPoly[k] <= AllGroups[jnumgroup]->CountIndex[jidbuff] / 3 	//если количество записанных полигонов в данную подгруппу меньше либо равно общему количеству полигонов которое содержит данна¤ подгруппа
							)
						{
							memcpy(RTCPUArrIndicesPtrs[jnumgroup][jidbuff] + (RTCountDrawPoly[jnumgroup][jidbuff] * 3),
								jarrsplits[j]->ArrPoly[k],
								jarrsplits[j]->CountPoly[k] * sizeof(uint32_t)* 3);

							if (RTBeginIndexModel[jnumgroup][jidbuff][tmpcurrmodel] == -1)
							{
								RTBeginIndexModel[jnumgroup][jidbuff][tmpcurrmodel] = RTCountDrawPoly[jnumgroup][jidbuff];
							}

							RTCountDrawPoly[jnumgroup][jidbuff] += jarrsplits[j]->CountPoly[k];
							RTCountDrawPolyModel[jnumgroup][jidbuff][tmpcurrmodel] += jarrsplits[j]->CountPoly[k];
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
		for(int i = 0, l = AllGroups.size(); i < l; ++i)
		{
			Group* tmpgroup = AllGroups[i];
			if (!tmpgroup || tmpgroup->CountVertex.size() <= 0 || !(tmpgroup->SortGroup == sort_mtl || sort_mtl == -1))
				continue;

			//проходимся по всем буферам
			for(int k = 0, kl = tmpgroup->VertexBuffer.size(); k<kl; ++k)
			{
				//если есть что к отрисовке
				if (RTCountDrawPoly[i][k] > 0)
				{
					RenderIndexBuffer->Lock(0, 0, (void**)&(RTGPUArrIndicesPtrs2), D3DLOCK_DISCARD);
					memcpy(RTGPUArrIndicesPtrs2, RTCPUArrIndicesPtrs[i][k], RTCountDrawPoly[i][k] * 3 * sizeof(uint32_t));
					RenderIndexBuffer->Unlock();

					StaticGeom::DXDevice->SetStreamSource(0, tmpgroup->VertexBuffer[k], 0, sizeof(vertex_static));
					StaticGeom::DXDevice->SetIndices(RenderIndexBuffer);
					StaticGeom::DXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());

					//если на данную подгруппу назначен раздельный рендер
					if (tmpgroup->IsRenderSingly)
					{
						long tmpprevcountindex = 0;

						for(int j = 0, jl = tmpgroup->ArrModels[k].size(); j < jl; ++j)
						{
							if (RTCountDrawPolyModel[i][k][j] > 0)
							{
								//ищем id материала подгруппы
								//общий id использовать нельзя так как не зря стоит раздельный рендер
								for (int q = 0; q < tmpgroup->ArrModels[k][j]->IDTex.size(); ++q)
								{
									if (i == tmpgroup->ArrModels[k][j]->SubSet[q].idgroup)
									{
										SGCore_MtlSet(tmpgroup->ArrModels[k][j]->IDTex[q], 0);
										break;
									}
								}
								
								SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, tmpgroup->CountVertex[k], tmpprevcountindex * 3, RTCountDrawPolyModel[i][k][j]);
								tmpprevcountindex += RTCountDrawPolyModel[i][k][j];
								Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + RTCountDrawPolyModel[i][k][j]);
							}
						}
					}
					else
					{
						SGCore_MtlSet(tmpgroup->idtex, 0);
						SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, tmpgroup->CountVertex[k], 0, RTCountDrawPoly[i][k]);
						Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + RTCountDrawPoly[i][k]);
					}
				}
			}
		}
	}
	//иначе рисуем по сортированному списку
	else
	{
		for (int i = 0; i < DistGroup.size(); ++i)
		{
			InfoGroup* tmpig = DistGroup[i];
			Group* tmpgroup = AllGroups[tmpig->g_group];

			if (!tmpgroup || tmpgroup->CountVertex.size() <= 0 || !(tmpgroup->SortGroup == sort_mtl || sort_mtl == -1))
				continue;

			if (exclude_model_id == tmpig->model && (exclude_group_id < 0 || exclude_group_id == tmpig->group))
				continue;

			ID id_buff = AllModels[tmpig->model]->SubSet[tmpig->group].idbuff;

			if (RTCountDrawPoly[tmpig->g_group][id_buff] > 0)
			{
				ID id_model;
				for (int j = 0; j < tmpgroup->ArrModels[id_buff].size(); ++j)
				{
					if (tmpgroup->ArrModels[id_buff][j] == AllModels[tmpig->model])
					{
						id_model = j;
						break;
					}
				}

				RenderIndexBuffer->Lock(0, 0, (void**)&(RTGPUArrIndicesPtrs2), D3DLOCK_DISCARD);
				memcpy(RTGPUArrIndicesPtrs2, RTCPUArrIndicesPtrs[tmpig->g_group][id_buff], RTCountDrawPoly[tmpig->g_group][id_buff]/*RTCountDrawPolyModel[tmpig->g_group][id_buff][id_model]*/ * 3 * sizeof(uint32_t));
				RenderIndexBuffer->Unlock();

				StaticGeom::DXDevice->SetStreamSource(0, tmpgroup->VertexBuffer[id_buff], 0, sizeof(vertex_static));
				StaticGeom::DXDevice->SetIndices(RenderIndexBuffer);
				StaticGeom::DXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());



				long tmpprevcountindex = AllModels[tmpig->model]->SubSet[tmpig->group].IndexStart/3;
				

				if (RTCountDrawPolyModel[tmpig->g_group][id_buff][id_model] > 0)
				{
					//ищем id материала подгруппы
					for (int q = 0; q < tmpgroup->ArrModels[id_buff][id_model]->IDTex.size(); ++q)
					{
						if (tmpig->g_group == tmpgroup->ArrModels[id_buff][id_model]->SubSet[q].idgroup)
						{
							SGCore_MtlSet(tmpgroup->ArrModels[id_buff][id_model]->IDTex[q], 0);
							break;
						}
					}
					SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, tmpgroup->CountVertex[id_buff], RTBeginIndexModel[tmpig->g_group][id_buff][id_model] * 3, RTCountDrawPolyModel[tmpig->g_group][id_buff][id_model]);
					tmpprevcountindex += RTCountDrawPolyModel[tmpig->g_group][id_buff][id_model];
					Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + RTCountDrawPolyModel[tmpig->g_group][id_buff][id_model]);
				}
			}
		}
	}
}

void StaticGeom::GPURenderSingly(DWORD timeDelta, ID id, ID id_tex)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id, _VOID);
	Segment** jarrsplits;
	long jidbuff;
	long jnumgroup;

	//обнуляем все данные об отрисованном в прошлый раз
	for (int i = 0; i < AllGroups.size(); i++)
	{
		if (AllGroups[i])
		{
			for (int k = 0; k < AllGroups[i]->VertexBuffer.size(); ++k)
			{
				RTCountDrawPoly[i][k] = 0;

				for (int j = 0; j < AllGroups[i]->ArrModels[k].size(); ++j)
				{
					RTCountDrawPolyModel[i][k][j] = 0;
					RTBeginIndexModel[i][k][j] = -1;
				}
			}
		}
	}


	if (AllModels[id]->IsRenderLod)
	{
		StaticGeom::DXDevice->SetStreamSource(0, AllModels[id]->Lod0.model->VertexBuffer, 0, sizeof(vertex_static));
		StaticGeom::DXDevice->SetIndices(AllModels[id]->Lod0.model->IndexBuffer);
		StaticGeom::DXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());
		for (int k = 0, kl = AllModels[id]->Lod0.model->SubsetCount; k < kl; ++k)
		{
			SGCore_MtlSet((id_tex > 0 ? id_tex : AllModels[id]->Lod0.IDsTexs[k]), 0);
			SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, AllModels[id]->Lod0.model->VertexCount[k], AllModels[id]->Lod0.model->StartIndex[k], AllModels[id]->Lod0.model->IndexCount[k] / 3);
			Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + AllModels[id]->Lod0.model->IndexCount[k] / 3);
		}
	}
	else if (ArrComFor[0]->arr[id]->CountCom > 0)
	{
		for (DWORD j = 0, jl = ArrComFor[0]->arr[id]->CountCom; j < jl; j++)
		{
			jarrsplits = ArrComFor[0]->arr[id]->Arr;
			for (DWORD k = 0; k < jarrsplits[j]->CountSubSet; ++k)
			{
				jidbuff = AllModels[id]->SubSet[jarrsplits[j]->NumberGroupModel[k]].idbuff;
				jnumgroup = jarrsplits[j]->NumberGroup[k];

				ID tmpcurrmodel = -1;

				for (int q = 0, ql = AllGroups[jnumgroup]->ArrModels[jidbuff].size(); q < ql; ++q)
				{
					if (AllGroups[jnumgroup]->ArrModels[jidbuff][q] == AllModels[id])
					{
						tmpcurrmodel = q;
						break;
					}
				}

				int currsort = AllGroups[jnumgroup]->SortGroup;

				
					if (
						jarrsplits[j]->CountPoly[k] > 0 &&	//если количество полигонов больше 0
						RTCountDrawPoly[jnumgroup][jidbuff] + jarrsplits[j]->CountPoly[k] <= AllGroups[jnumgroup]->CountIndex[jidbuff] / 3 	//если количество записанных полигонов в данную подгруппу меньше либо равно общему количеству полигонов которое содержит данна¤ подгруппа
						)
					{
						memcpy(RTCPUArrIndicesPtrs[jnumgroup][jidbuff] + (RTCountDrawPoly[jnumgroup][jidbuff] * 3),
							jarrsplits[j]->ArrPoly[k],
							jarrsplits[j]->CountPoly[k] * sizeof(uint32_t)* 3);

						if (RTBeginIndexModel[jnumgroup][jidbuff][tmpcurrmodel] == -1)
						{
							RTBeginIndexModel[jnumgroup][jidbuff][tmpcurrmodel] = RTCountDrawPoly[jnumgroup][jidbuff];
						}

						RTCountDrawPoly[jnumgroup][jidbuff] += jarrsplits[j]->CountPoly[k];
						RTCountDrawPolyModel[jnumgroup][jidbuff][tmpcurrmodel] += jarrsplits[j]->CountPoly[k];
					}
			}
		}
	}


	uint32_t* RTGPUArrIndicesPtrs2;
	
	for (int i = 0; i < AllModels[id]->SubSet.size(); ++i)
	{
		ID idgroup = AllModels[id]->SubSet[i].idgroup;
		Group* tmpgroup = AllGroups[AllModels[id]->SubSet[i].idgroup];
		ID id_buff = AllModels[id]->SubSet[i].idbuff;

		ID id_model;
		for (int j = 0; j < tmpgroup->ArrModels[id_buff].size(); ++j)
		{
			if (tmpgroup->ArrModels[id_buff][j] == AllModels[id])
			{
				id_model = j;
				break;
			}
		}

		RenderIndexBuffer->Lock(0, 0, (void**)&(RTGPUArrIndicesPtrs2), D3DLOCK_DISCARD);
		memcpy(RTGPUArrIndicesPtrs2, RTCPUArrIndicesPtrs[idgroup][id_buff], RTCountDrawPoly[idgroup][id_buff]/*RTCountDrawPolyModel[tmpig->g_group][id_buff][id_model]*/ * 3 * sizeof(uint32_t));
		RenderIndexBuffer->Unlock();

		StaticGeom::DXDevice->SetStreamSource(0, tmpgroup->VertexBuffer[id_buff], 0, sizeof(vertex_static));
		StaticGeom::DXDevice->SetIndices(RenderIndexBuffer);
		StaticGeom::DXDevice->SetVertexDeclaration(SGCore_StaticModelGetDecl());

		SGCore_MtlSet((id_tex > 0 ? id_tex : tmpgroup->idtex), 0);

		SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, tmpgroup->CountVertex[id_buff], 0, RTCountDrawPoly[idgroup][id_buff]);
		Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + RTCountDrawPoly[idgroup][id_buff]);
	}
}

void StaticGeom::PreSegmentation(Model* mesh, ISXDataStaticModel* model)
{
	vertex_static *CreateV;
	mem_delete_a(ArrMeshVertex);
	mem_delete_a(ArrMeshIndex);
	CountVertex = model->AllVertexCount;
	//копируем все вершины для текущих расчетов
	ArrMeshVertex = new float3[model->AllVertexCount];

	model->VertexBuffer->Lock(0, 0, (void **)&CreateV, 0);
	for (DWORD i = 0; i<model->AllVertexCount; i++)
	{
		ArrMeshVertex[i] = (CreateV[i].Pos.operator float3());
	}
	model->VertexBuffer->Unlock();

	ArrMeshIndex = new UINT[model->AllIndexCount];
	UINT* pInd;
	model->IndexBuffer->Lock(0, 0, (void **)&pInd, 0);
	memcpy(ArrMeshIndex, pInd, sizeof(UINT)* model->AllIndexCount);
	model->IndexBuffer->Unlock();

	///////

	mesh->ArrSplits = new Segment();
	mesh->ArrSplits->CountAllPoly = model->AllIndexCount/3;
	reportf(REPORT_MSG_LEVEL_NOTICE, "poly: %d, ", mesh->ArrSplits->CountAllPoly);

	mesh->ArrSplits->BoundVolumeP = SGCore_CrBound();
	mesh->ArrSplits->BoundVolumeP->CalcBound(model->VertexBuffer, model->AllVertexCount, sizeof(vertex_static));

	float3 tmpMin, tmpMax;
	mesh->ArrSplits->BoundVolumeP->GetMinMax(&tmpMin, &tmpMax);

	int CountSplitsSys = 0;
	int CountPolyInSegment = GEOM_MIN_COUNT_POLY;

	float3 dimensions = tmpMax - tmpMin;	//габариты
#if 1
	float allvolume = dimensions.x * dimensions.y * dimensions.z;
	const float minVol = GEOM_MIN_ALLVOLUME_FOR_SEGMENTATION;
	const float minLen = GEOM_MIN_LENGTH_FOR_SEGMENTATION;
	const float minOctoHeight = GEOM_MIN_HEIGHT_FOR_SEGMENTATION;
	if ((allvolume >= minVol || dimensions.x >= minLen || dimensions.y >= minOctoHeight || dimensions.z >= minLen) && (model->AllIndexCount >= GEOM_MIN_POLYGONS_FOR_SEGMENTATION * 3 || allvolume >= GEOM_FORCE_ALLVOLUME_FOR_SEGMENTATION))
	{
		if(dimensions.y >= minOctoHeight)
		{
			CountSplitsSys = GEOM_COUNT_TYPE_SEGMENTATION_OCTO;
			reportf(REPORT_MSG_LEVEL_NOTICE, " div: octo, ");
		}
		else
		{
			CountSplitsSys = GEOM_COUNT_TYPE_SEGMENTATION_QUAD;
			reportf(REPORT_MSG_LEVEL_NOTICE, " div: quad, ");

		}
			
		float4_t tmpmin(dimensions.x / minLen, dimensions.y / minOctoHeight, dimensions.z / minLen, (float)model->AllIndexCount / (float)GEOM_MIN_POLYGONS_FOR_SEGMENTATION / 3.0f);
		tmpmin.x = min(min(min(tmpmin.x, tmpmin.y), min(tmpmin.z, tmpmin.w)), (allvolume / minVol)) * 10.0;

		CountPolyInSegment = (float)model->AllIndexCount / 3.0 / tmpmin.x / CountSplitsSys;

		if (CountPolyInSegment < GEOM_MIN_COUNT_POLY)
		{
			CountPolyInSegment = GEOM_MIN_COUNT_POLY;
		}

		if (CountPolyInSegment > GEOM_MAX_COUNT_POLY)
		{
			CountPolyInSegment = GEOM_MAX_COUNT_POLY;
		}

		reportf(REPORT_MSG_LEVEL_NOTICE, "poly in split: %d, ", CountPolyInSegment);
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
				reportf(REPORT_MSG_LEVEL_NOTICE, " div: octo, ");
			}
			else
			{
				CountSplitsSys = STATIC_COUNT_TYPE_SEGMENTATION_QUAD;
				reportf(REPORT_MSG_LEVEL_NOTICE, " div: quad, ");
			}

			//определяем коэфициент интерполяции для определния минимального количества полигонов в сплите
			float tmpcoef = float(allvolume * 3) / (float)(model->AllIndexCount / 3);
			if(tmpcoef > 1.f)
				tmpcoef = 1;
			else if(tmpcoef < 0.f)
				tmpcoef = 0;
			CountPolyInSegment = lerpf(STATIC_MIN_COUNT_POLY, STATIC_MAX_COUNT_POLY, tmpcoef);
			reportf(REPORT_MSG_LEVEL_NOTICE, "poly in split: %d, ", CountPolyInSegment);
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

	mesh->ArrSplits->BoundVolumeSys = SGCore_CrBound();
	mesh->ArrSplits->BoundVolumeSys->SetMinMax(&tmpMin, &tmpMax);
	//}}

	mesh->ArrSplits->CountSubSet = model->SubsetCount;
	mesh->ArrSplits->NumberGroup = new uint32_t[model->SubsetCount];
	mesh->ArrSplits->NumberGroupModel = new uint32_t[model->SubsetCount];
	mesh->ArrSplits->CountPoly = new uint32_t[model->SubsetCount];

	/////

	//заполняем массив с номаерами подгрупп и массив с количествами полигонов
	for (DWORD i = 0; i<model->SubsetCount; i++)
	{
		mesh->ArrSplits->NumberGroupModel[i] = i;
		mesh->ArrSplits->NumberGroup[i] = i;
		mesh->ArrSplits->CountPoly[i] = model->IndexCount[i] / 3;
	}

	//создаем массивы с полигонами, каждый полигон принадлежит своей подгруппе
	mesh->ArrSplits->ArrPoly = new uint32_t*[mesh->ArrSplits->CountSubSet];
	for (DWORD i = 0; i<mesh->ArrSplits->CountSubSet; i++)
	{
		mesh->ArrSplits->ArrPoly[i] = new uint32_t[mesh->ArrSplits->CountPoly[i] * 3];
	}

	//заполняем массивы с полигонами
	for (DWORD i = 0; i<model->SubsetCount; i++)
	{
		memcpy(mesh->ArrSplits->ArrPoly[i], ArrMeshIndex + model->StartIndex[i], model->IndexCount[i] * sizeof(uint32_t));
	}

	//если количество полигонов и текущая установка деления позволяют делить дальше
	if (mesh->ArrSplits->CountAllPoly >= CountPolyInSegment && CountSplitsSys != 0)
	{
		mesh->ArrSplits->BFNonEnd = true;
		CycleSegmentation(mesh->ArrSplits, mesh, model, CountSplitsSys, CountPolyInSegment);
	}
	else
	{
		//оптимизация для Post TnL кэша
		//{{
		Array<DWORD> tmpVert;
		bool isunic = true;
		for (DWORD q = 0; q<mesh->ArrSplits->CountSubSet; q++)
		{
			tmpVert.clear();
			tmpVert.push_back(mesh->ArrSplits->ArrPoly[q][0]);
			for (DWORD k = 0; k<mesh->ArrSplits->CountPoly[q] * 3; k++)
			{
				isunic = true;
				for (DWORD j = 0; j<tmpVert.size() && isunic; j++)
				{
					if (mesh->ArrSplits->ArrPoly[q][k] == tmpVert[j])
					{
						isunic = false;
					}
				}

				if (isunic)
					tmpVert.push_back(mesh->ArrSplits->ArrPoly[q][k]);
			}

			SGCore_OptimizeIndecesInSubsetUint32(mesh->ArrSplits->ArrPoly[q], mesh->ArrSplits->CountPoly[q], tmpVert.size());
		}
		//}}

		//иначе нельзя делить дальше
		//приводим индексы куска к глобальному виду уровня
		for (DWORD i = 0; i<mesh->ArrSplits->CountSubSet; i++)
		{
			for (DWORD k = 0; k<mesh->ArrSplits->CountPoly[i] * 3; k++)
			{
				mesh->ArrSplits->ArrPoly[i][k] -= model->StartVertex[mesh->ArrSplits->NumberGroup[i]];
				mesh->ArrSplits->ArrPoly[i][k] += mesh->SubSet[mesh->ArrSplits->NumberGroup[i]].VertexStart;
			}
			mesh->ArrSplits->NumberGroup[i] = mesh->SubSet[mesh->ArrSplits->NumberGroup[i]].idgroup;
		}

		mesh->ArrSplits->BFNonEnd = false;
	}
}

void StaticGeom::CycleSegmentation(Segment *Split, Model* mesh, ISXDataStaticModel* model, long CountSplitsSys, long CountPolyInSegment)
{
	Array<Segment*> queue;
	long tmpcount = 0;
	queue.push_back(Split);

	while (queue.size())
	{
		if (queue[0]->BFNonEnd)
		{
			Segmentation(queue[0], mesh, model, CountSplitsSys, CountPolyInSegment, &queue);

			for (int i = 0; i < CountSplitsSys; i++)
			{
				if (queue[0]->Splits[i])
					queue.push_back(queue[0]->Splits[i]);
			}
		}

		queue.erase(0);
		++tmpcount;
	}
}


void StaticGeom::Segmentation(Segment* Split, Model* mesh, ISXDataStaticModel* model, long CountSplitsSys, long CountPolyInSegment, Array<Segment*> * queue)
{
	Array<DWORD> ArrPoly[8];
	Array<DWORD> ArrGroup[8];
	Array<DWORD> ArrSerialNum[8];

	for (int i = 0; i<CountSplitsSys; i++)
	{
		Split->Splits[i] = new Segment();
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
		SGCore_0ComBoundBoxArr4(Split->BoundVolumeSys, (ArrBound));
		for (int i = 0; i<CountSplitsSys; i++)
		{
			Split->Splits[i]->BoundVolumeSys = ArrBound[i];
			Split->Splits[i]->BoundVolumeSys->GetMinMax(&tmpmin, &tmpmax);
			Split->Splits[i]->BoundVolumeP = SGCore_CrBound();
			Split->Splits[i]->BoundVolumeP->SetMinMax(&tmpmin, &tmpmax);
		}
	}
	else if (CountSplitsSys == GEOM_COUNT_TYPE_SEGMENTATION_OCTO)
	{
		ISXBound* ArrBound[GEOM_COUNT_TYPE_SEGMENTATION_OCTO];
		for (int i = 0; i < GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
			ArrBound[i] = SGCore_CrBound();

		float3 tmpmin, tmpmax;
		SGCore_0ComBoundBoxArr8(Split->BoundVolumeSys, ArrBound);

		for (int i = 0; i<CountSplitsSys; i++)
		{
			Split->Splits[i]->BoundVolumeSys = ArrBound[i];
			Split->Splits[i]->BoundVolumeSys->GetMinMax(&tmpmin, &tmpmax);
			Split->Splits[i]->BoundVolumeP = SGCore_CrBound();
			Split->Splits[i]->BoundVolumeP->SetMinMax(&tmpmin, &tmpmax);
		}
	}
	//}}

	bool *tmp_arr_mesh_poly = new bool[Split->CountAllPoly];
	for (DWORD i = 0; i<Split->CountAllPoly; i++)
		tmp_arr_mesh_poly[i] = true;

	DWORD CountInPoly = 0;
	float3 tmpMin, tmpMax;
	for (int i = 0; i<CountSplitsSys; i++)
	{
		Split->Splits[i]->BoundVolumeSys->GetMinMax(&tmpMin, &tmpMax);
		//SGCore_FCreateBoundingBoxMesh(&tmpMin, &tmpMax, &(Split->Splits[i]->BoundBox));
		DWORD tmpNumCurrentPoly = 0;
		for (DWORD j = 0; j<Split->CountSubSet; j++)
		{
			for (DWORD k = 0; k<Split->CountPoly[j] * 3; k += 3)
			{
				if (SGCore_0InPosPoints2D(&tmpMin, &tmpMax,
					&ArrMeshVertex[Split->ArrPoly[j][k]],
					&ArrMeshVertex[Split->ArrPoly[j][k + 1]],
					&ArrMeshVertex[Split->ArrPoly[j][k + 2]]
					)
					&& tmp_arr_mesh_poly[tmpNumCurrentPoly]
					)
				{
					ArrPoly[i].push_back((Split->ArrPoly[j][k]));
					ArrPoly[i].push_back((Split->ArrPoly[j][k + 1]));
					ArrPoly[i].push_back((Split->ArrPoly[j][k + 2]));
					ArrGroup[i].push_back((Split->NumberGroup[j]));

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
	if (CountInPoly < Split->CountAllPoly)
	{
		DWORD tmpNumCurrentPolyDec = 0;
		DWORD tmpNumCurrentPoly = 0;
		float3 pos0, pos1, pos2, CenterPoly, Length, SphereCenter;
		float ArrLength[8], ShereRadius;

		for (DWORD j = 0; j<Split->CountSubSet; j++)
		{
			for (DWORD k = 0; k<Split->CountPoly[j] * 3; k += 3)
			{
				if (tmp_arr_mesh_poly[tmpNumCurrentPoly])
				{
					pos0 = ArrMeshVertex[Split->ArrPoly[j][k]];
					pos1 = ArrMeshVertex[Split->ArrPoly[j][k + 1]];
					pos2 = ArrMeshVertex[Split->ArrPoly[j][k + 2]];

					CenterPoly.x = CenterPoly.y = CenterPoly.z = 0;

					CenterPoly.x = (pos0.x + pos1.x + pos2.x) / 3.0;
					CenterPoly.y = (pos0.y + pos1.y + pos2.y) / 3.0;
					CenterPoly.z = (pos0.z + pos1.z + pos2.z) / 3.0;

					//memset(ArrLength,0,sizeof(float) * 8);

					for (int q = 0; q<CountSplitsSys; q++)
					{
						Split->Splits[q]->BoundVolumeSys->GetSphere(&SphereCenter, &ShereRadius);
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

					ArrPoly[Key].push_back((Split->ArrPoly[j][k]));
					ArrPoly[Key].push_back((Split->ArrPoly[j][k + 1]));
					ArrPoly[Key].push_back((Split->ArrPoly[j][k + 2]));
					ArrGroup[Key].push_back((Split->NumberGroup[j]));
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
			Split->Splits[i]->CountSubSet = 0;

			//предварительно записываем одну подгруппу в массив tmpNumSubSet
			tmpNumSubSet.push_back(ArrGroup[i][0]);
			Split->Splits[i]->CountSubSet++;
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
					Split->Splits[i]->CountSubSet++;
				}
			}

			//
			Split->Splits[i]->NumberGroup = new uint32_t[Split->Splits[i]->CountSubSet];
			Split->Splits[i]->NumberGroupModel = new uint32_t[Split->Splits[i]->CountSubSet];

			for (DWORD k = 0; k<Split->Splits[i]->CountSubSet; k++)
			{
				Split->Splits[i]->NumberGroup[k] = (tmpNumSubSet[k]);
				Split->Splits[i]->NumberGroupModel[k] = (tmpNumSubSet[k]);
			}

			tmpNumSubSet.clear();

			Split->Splits[i]->CountPoly = new uint32_t[Split->Splits[i]->CountSubSet];

			//обнуление данных GlobalMesh->SplitMeshes[i]->CountPoly
			for (DWORD j = 0; j<Split->Splits[i]->CountSubSet; j++)
			{
				Split->Splits[i]->CountPoly[j] = 0;
			}
			Split->Splits[i]->CountAllPoly = 0;

			//вычисляем сколько полигонов в каждой подгруппе в данном сплите
			for (DWORD k = 0; k<ArrPoly[i].size(); k += 3)
			{
				for (DWORD j = 0; j<Split->Splits[i]->CountSubSet; j++)
				{
					if (Split->Splits[i]->NumberGroup[j] == (ArrGroup[i][k / 3]))
					{
						Split->Splits[i]->CountPoly[j]++;
						Split->Splits[i]->CountAllPoly++;
					}
				}
			}

			Split->Splits[i]->ArrPoly = new uint32_t*[Split->Splits[i]->CountSubSet];
			
			for (DWORD j = 0; j<Split->Splits[i]->CountSubSet; j++)
			{
				Split->Splits[i]->ArrPoly[j] = new uint32_t[Split->Splits[i]->CountPoly[j] * 3];
			}
			uint32_t* tmpCountPG = new uint32_t[Split->Splits[i]->CountSubSet];

			for (DWORD k = 0; k<Split->Splits[i]->CountSubSet; k++)
			{
				tmpCountPG[k] = 0;
			}

			for (DWORD k = 0; k<ArrPoly[i].size(); k += 3)
			{
				for (DWORD j = 0; j<Split->Splits[i]->CountSubSet; j++)
				{

					if (Split->Splits[i]->NumberGroup[j] == (ArrGroup[i][k / 3]))
					{
						Split->Splits[i]->ArrPoly[j][tmpCountPG[j]] = (ArrPoly[i][k]);
						Split->Splits[i]->ArrPoly[j][tmpCountPG[j] + 1] = (ArrPoly[i][k + 1]);
						Split->Splits[i]->ArrPoly[j][tmpCountPG[j] + 2] = (ArrPoly[i][k + 2]);

						tmpCountPG[j] += 3;
					}
				}
			}

			ArrPoly[i].clear();
			ArrGroup[i].clear();

			mem_delete_a(tmpCountPG);

			EditVolume(mesh, Split->Splits[i]);
			if (Split->Splits[i]->CountAllPoly > 0 && Split->Splits[i]->CountAllPoly > CountPolyInSegment)
			{
				Split->Splits[i]->BFNonEnd = true;
			}
			else
			{
				Split->Splits[i]->BFNonEnd = false;
				//EditVolume(mesh, Split->Splits[i]);

				//оптимизация для Post TnL кэша
				//{{
				Array<uint32_t> tmpVert;
				bool isunic = true;
				for (DWORD q = 0; q<Split->Splits[i]->CountSubSet; q++)
				{
					tmpVert.clear();
					tmpVert.push_back(Split->Splits[i]->ArrPoly[q][0]);
					for (DWORD k = 0; k<Split->Splits[i]->CountPoly[q] * 3; k++)
					{
						isunic = true;
						for (DWORD j = 0; j<tmpVert.size() && isunic; j++)
						{
							if (Split->Splits[i]->ArrPoly[q][k] == tmpVert[j])
							{
								isunic = false;
							}
						}

						if (isunic)
							tmpVert.push_back(Split->Splits[i]->ArrPoly[q][k]);
					}

					SGCore_OptimizeIndecesInSubsetUint32(Split->Splits[i]->ArrPoly[q], Split->Splits[i]->CountPoly[q], tmpVert.size());
				}
				//}}
				

				for (DWORD q = 0; q<Split->Splits[i]->CountSubSet; q++)
				{
					for (DWORD k = 0; k<Split->Splits[i]->CountPoly[q] * 3; k++)
					{
						Split->Splits[i]->ArrPoly[q][k] -= model->StartVertex[Split->Splits[i]->NumberGroup[q]];
						Split->Splits[i]->ArrPoly[q][k] += mesh->SubSet[Split->Splits[i]->NumberGroup[q]].VertexStart;
					}

					Split->Splits[i]->NumberGroup[q] = mesh->SubSet[Split->Splits[i]->NumberGroup[q]].idgroup;
				}
			}
		}
		else
		{
			Split->Splits[i]->CountAllPoly = 0;
			Split->Splits[i]->BFNonEnd = false;

			mem_delete(Split->Splits[i]);
		}
	}

	mem_delete_a(Split->NumberGroup);
	
	mem_delete_a(Split->CountPoly);

	for (DWORD k = 0; k<Split->CountSubSet; k++)
	{
		mem_delete_a(Split->ArrPoly[k]);
	}

	mem_delete_a(Split->ArrPoly);
	mem_delete_a(tmp_arr_mesh_poly);
}

void StaticGeom::EditVolume(Model* mesh, Segment* Split)
{
	float3 Max, Min;
	Max = ArrMeshVertex[(Split->ArrPoly[0][0])];
	Min = ArrMeshVertex[(Split->ArrPoly[0][0])];

	float3 Max2, Min2;
	Split->BoundVolumeSys->GetMinMax(&Min2, &Max2);

	float3 tmpf;
	for (DWORD i = 0; i<Split->CountSubSet; i++)
	{
		for (DWORD k = 0; k<Split->CountPoly[i] * 3; k++)
		{
			tmpf = ArrMeshVertex[(Split->ArrPoly[i][k])];
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

	//Split->BoundVolumeP->SetSphere(&Center, &Radius);
	Split->BoundVolumeP->SetMinMax(&Min, &Max);
}

void StaticGeom::Save(const char* path)
{
	FILE* file = fopen(path, "wb");
	
	//записываем количество подгрупп
	int32_t countgroup = AllGroups.size();
	fwrite(&countgroup, sizeof(int32_t), 1, file);

	vertex_static * pData;

	for (long i = 0; i < AllGroups.size(); ++i)
	{
		int32_t tmpstrlen = AllGroups[i]->name.length();
		fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
		fwrite(AllGroups[i]->name.c_str(), sizeof(char), tmpstrlen, file);

		//записываем количество буферов в подгруппе
		int32_t countbuffingroup = AllGroups[i]->VertexBuffer.size();
		fwrite(&countbuffingroup, sizeof(int32_t), 1, file);

		for (long k = 0; k < AllGroups[i]->CountVertex.size(); ++k)
		{
			//записываем количество моделей, которые используют данную подгруппу
			int32_t countusingmodels = AllGroups[i]->ArrModels[k].size();
			fwrite(&countusingmodels, sizeof(int32_t), 1, file);
			int32_t tmpcountusingmodels = 0;
			for (int32_t j = 0; j < AllGroups[i]->ArrModels[k].size(); ++j)
			{
				for (int32_t q = 0; q < AllModels.size(); ++q)
				{
					if (AllModels[q] == AllGroups[i]->ArrModels[k][j])
					{
						fwrite(&(q), sizeof(int32_t), 1, file);
						++tmpcountusingmodels;
						break;
					}
				}
			}

			//записываем количество вершин в буфере подгруппы
			fwrite(&AllGroups[i]->CountVertex[k], sizeof(int32_t), 1, file);

			//записываем количество индексов в буфере подгруппы
			fwrite(&AllGroups[i]->CountIndex[k], sizeof(int32_t), 1, file);

			//записываем весь вершинный буфер подгруппы
			AllGroups[i]->VertexBuffer[k]->Lock(0, 0, (void**)&pData, 0);
			fwrite(pData, sizeof(vertex_static), AllGroups[i]->CountVertex[k], file);
			AllGroups[i]->VertexBuffer[k]->Unlock();
		}
	}

	//записываем количество моделей
	int32_t countmodels = AllModels.size();
	fwrite(&countmodels, sizeof(int32_t), 1, file);

	for (long i = 0; i < AllModels.size(); ++i)
	{
		int32_t countsubset = AllModels[i]->SubSet.size();
		fwrite(&countsubset, sizeof(int32_t), 1, file);

		int32_t countlenstr = strlen(AllModels[i]->Name);
		fwrite(&countlenstr, sizeof(int32_t), 1, file);
		fwrite(&AllModels[i]->Name, sizeof(char), countlenstr, file);

		countlenstr = strlen(AllModels[i]->PathName);
		fwrite(&countlenstr, sizeof(int32_t), 1, file);
		fwrite(&AllModels[i]->PathName, sizeof(char), countlenstr, file);

		
		fwrite(&AllModels[i]->CountPoly, sizeof(int32_t), 1, file);
		
		fwrite(&AllModels[i]->Position.x, sizeof(float), 1, file);
		fwrite(&AllModels[i]->Position.y, sizeof(float), 1, file);
		fwrite(&AllModels[i]->Position.z, sizeof(float), 1, file);

		fwrite(&AllModels[i]->Rotation.x, sizeof(float), 1, file);
		fwrite(&AllModels[i]->Rotation.y, sizeof(float), 1, file);
		fwrite(&AllModels[i]->Rotation.z, sizeof(float), 1, file);

		fwrite(&AllModels[i]->Scale.x, sizeof(float), 1, file);
		fwrite(&AllModels[i]->Scale.y, sizeof(float), 1, file);
		fwrite(&AllModels[i]->Scale.z, sizeof(float), 1, file);

		if (AllModels[i]->Lod0.model)
		{
			countlenstr = strlen(AllModels[i]->Lod0.PathName);
			fwrite(&countlenstr, sizeof(int32_t), 1, file);
			fwrite(AllModels[i]->Lod0.PathName, sizeof(char), countlenstr, file);
		}
		else
		{
			countlenstr = 0;
			fwrite(&countlenstr, sizeof(int32_t), 1, file);
		}

		for (long k = 0; k < AllModels[i]->SubSet.size(); ++k)
		{
			fwrite(&(AllModels[i]->SubSet[k]), sizeof(Model::GDataBuff), 1, file);
		}

		Array<Segment*> queue;
		long tmpcount = 0;
		queue.push_back(AllModels[i]->ArrSplits);

		while (queue.size())
		{
			SaveSplit(queue[0], file, &queue);

			queue.erase(0);
			++tmpcount;
		}
	}

	fclose(file);
}

void StaticGeom::SaveSplit(Segment* Split, FILE* file, Array<Segment*> * queue)
{
	float3 jmin, jmax;
	Split->BoundVolumeSys->GetMinMax(&jmin, &jmax);
	fwrite(&jmin.x, sizeof(float), 1, file);
	fwrite(&jmin.y, sizeof(float), 1, file);
	fwrite(&jmin.z, sizeof(float), 1, file);

	fwrite(&jmax.x, sizeof(float), 1, file);
	fwrite(&jmax.y, sizeof(float), 1, file);
	fwrite(&jmax.z, sizeof(float), 1, file);

	Split->BoundVolumeP->GetMinMax(&jmin, &jmax);
	fwrite(&jmin.x, sizeof(float), 1, file);
	fwrite(&jmin.y, sizeof(float), 1, file);
	fwrite(&jmin.z, sizeof(float), 1, file);

	fwrite(&jmax.x, sizeof(float), 1, file);
	fwrite(&jmax.y, sizeof(float), 1, file);
	fwrite(&jmax.z, sizeof(float), 1, file);

	fwrite(&Split->CountAllPoly, sizeof(uint32_t), 1, file);

	fwrite(&Split->BFNonEnd, sizeof(int8_t), 1, file);

	if (Split->BFNonEnd)
	{
		bool isexists = true;
		for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; i++)
		{
			if (Split->Splits[i])
			{
				isexists = true;
				fwrite(&isexists, sizeof(int8_t), 1, file);
				queue->push_back(Split->Splits[i]);
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
		fwrite(&Split->CountSubSet, sizeof(uint32_t), 1, file);
		fwrite(Split->NumberGroupModel, sizeof(uint32_t)*Split->CountSubSet, 1, file);
		fwrite(Split->NumberGroup, sizeof(uint32_t)*Split->CountSubSet, 1, file);
		fwrite(Split->CountPoly, sizeof(uint32_t)*Split->CountSubSet, 1, file);

		for (long i = 0; i < Split->CountSubSet; ++i)
		{
			fwrite(Split->ArrPoly[i], sizeof(uint32_t)*Split->CountPoly[i] * 3, 1, file);
		}
	}
}


void StaticGeom::Load(const char* path)
{
	DelArrIndexPtr();

	FILE* file = fopen(path, "rb");

	int32_t countgroup = -1;
	fread(&countgroup, sizeof(int32_t), 1, file);
	Array<Array<Array<int32_t>>> tmpArrIdsModels;
	vertex_static * pData;
	long tmpbigersizebuff = 0;

	for (long i = 0; i < countgroup; ++i)
	{
		Group* group = new Group();
		tmpbigersizebuff = 0;
		int32_t tmpstrlen;
		char texname[1024];
		fread(&tmpstrlen, sizeof(int32_t), 1, file);
		fread(texname, sizeof(char), tmpstrlen, file);
		texname[tmpstrlen] = 0;
		group->name = texname;
		
		group->AllCountVertex = 0;
		group->AllCountIndex = 0;
		
		//записываем количество буферов в подгруппе
		int32_t countbuffingroup = -1;
		fread(&countbuffingroup, sizeof(int32_t), 1, file);
		
		for (long k = 0; k < countbuffingroup; ++k)
		{
			//записываем количество моделей, которые используют данную подгруппу
			int32_t countusingmodels;
			fread(&countusingmodels, sizeof(int32_t), 1, file);
			int32_t tmpnummodel;
			for (long j = 0; j < countusingmodels; ++j)
			{
				fread(&tmpnummodel, sizeof(int32_t), 1, file);
				tmpArrIdsModels[i][k].push_back(tmpnummodel);
			}

			//записываем количество вершин в буфере подгруппы
			fread(&group->CountVertex[k], sizeof(int32_t), 1, file);
			group->AllCountVertex += group->CountVertex[k];
			
			//записываем количество индексов в буфере подгруппы
			fread(&group->CountIndex[k], sizeof(int32_t), 1, file);
			group->AllCountIndex += group->CountIndex[k];

			if (tmpbigersizebuff < group->CountIndex[k])
				tmpbigersizebuff = group->CountIndex[k];

			IDirect3DVertexBuffer9* vb;
			StaticGeom::DXDevice->CreateVertexBuffer(sizeof(vertex_static)* group->CountVertex[k], NULL, NULL, D3DPOOL_MANAGED, &vb, 0);

			//записываем весь вершинный буфер подгруппы
			vb->Lock(0, 0, (void**)&pData, 0);
			fread(pData, sizeof(vertex_static), group->CountVertex[k], file);
		
			/*Group::VertexBuff* vborigin = new Group::VertexBuff();
			vborigin->count = group->CountVertex[k];
			vborigin->arr = new float3_t[vborigin->count];

			for (long j = 0; j < vborigin->count; ++j)
			{
				vborigin->arr[j] = pData[j].Pos;
			}*/

			//UpdateArrMeshVertex2(group->CountVertex[k], pData);
			vb->Unlock();

			group->VertexBuffer.push_back(vb);
			//group->VertexBufferOrigin.push_back(vborigin);
		}
		
		char tmptex[1024];
		sprintf(tmptex, "%s.dds", group->name.c_str());

		group->idtex = SGCore_MtlLoad(tmptex, MTL_TYPE_GEOM);
		group->IsRenderSingly = SGCore_MtlGroupRenderIsSingly(group->idtex);
		group->SortGroup = SGCore_MtlGetSort(group->idtex);
		AllGroups.push_back(group);
		
		if (RenderIndexBuffer == 0 || SizeRenderIndexBuffer < tmpbigersizebuff)
		{
			mem_release(RenderIndexBuffer);
			SizeRenderIndexBuffer = tmpbigersizebuff;
			StaticGeom::DXDevice->CreateIndexBuffer(sizeof(uint32_t)* SizeRenderIndexBuffer, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/, &RenderIndexBuffer, 0);
		}
	}

	//записываем количество моделей
	int32_t countmodels = AllModels.size();
	fread(&countmodels, sizeof(int32_t), 1, file);

	for (long i = 0; i < countmodels; ++i)
	{
		AllModels.push_back(0);
		AllModels[i] = new Model();

		int32_t countsubset;
		fread(&countsubset, sizeof(int32_t), 1, file);

		int32_t countlenstr = 0;
		fread(&countlenstr, sizeof(int32_t), 1, file);
		fread(&AllModels[i]->Name, sizeof(char), countlenstr, file);
		AllModels[i]->Name[countlenstr] = 0;

		countlenstr = 0;
		fread(&countlenstr, sizeof(int32_t), 1, file);
		fread(&AllModels[i]->PathName, sizeof(char), countlenstr, file);
		AllModels[i]->PathName[countlenstr] = 0;

		fread(&AllModels[i]->CountPoly, sizeof(int32_t), 1, file);

		fread(&AllModels[i]->Position.x, sizeof(float), 1, file);
		fread(&AllModels[i]->Position.y, sizeof(float), 1, file);
		fread(&AllModels[i]->Position.z, sizeof(float), 1, file);

		fread(&AllModels[i]->Rotation.x, sizeof(float), 1, file);
		fread(&AllModels[i]->Rotation.y, sizeof(float), 1, file);
		fread(&AllModels[i]->Rotation.z, sizeof(float), 1, file);

		fread(&AllModels[i]->Scale.x, sizeof(float), 1, file);
		fread(&AllModels[i]->Scale.y, sizeof(float), 1, file);
		fread(&AllModels[i]->Scale.z, sizeof(float), 1, file);

		AllModels[i]->OldPosition = AllModels[i]->Position;
		AllModels[i]->OldRotation = AllModels[i]->Rotation;
		AllModels[i]->OldScale = AllModels[i]->Scale;

		WorldMat = SMMatrixScaling(AllModels[i]->Scale) * SMMatrixRotationX(AllModels[i]->Rotation.x) * SMMatrixRotationY(AllModels[i]->Rotation.y) * SMMatrixRotationZ(AllModels[i]->Rotation.z) * SMMatrixTranslation(AllModels[i]->Position);

		fread(&countlenstr, sizeof(int32_t), 1, file);

		if (countlenstr > 0)
		{
			fread(AllModels[i]->Lod0.PathName, sizeof(char), countlenstr, file);
			AllModels[i]->Lod0.PathName[countlenstr] = 0;
			SetModelLodPath(i, AllModels[i]->Lod0.PathName);
		}

		for (long k = 0; k < countsubset; ++k)
		{
			Model::GDataBuff gdb;
			fread(&gdb, sizeof(Model::GDataBuff), 1, file);
			AllModels[i]->SubSet.push_back(gdb);

			char tmptex[1024];
			sprintf(tmptex, "%s.dds", AllGroups[gdb.idgroup]->name.c_str());
			AllModels[i]->IDTex[k] = SGCore_MtlLoad(tmptex, MTL_TYPE_GEOM);
		}

		Array<Segment**> queue;
		long tmpcount = 0;
		queue.push_back(&(AllModels[i]->ArrSplits));

		while (queue.size())
		{
			LoadSplit(queue[0], file, &queue);

			queue.erase(0);
			++tmpcount;
		}

		Segment* tmpsegment = AllModels[i]->ArrSplits;
		SetSplitID(AllModels[i]->ArrSplits, &(AllModels[i]->SplitsIDs), (&AllModels[i]->SplitsIDsRender));

		AddModelInArrCom(i);

		float3 jmin, jmax, jmin2, jmax2;

		AllModels[i]->ArrSplits->BoundVolumeP->GetMinMax(&jmin, &jmax);
		BoundVolume->GetMinMax(&jmin2, &jmax2);

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

		BoundVolume->SetMinMax(&jmin2, &jmax2);
	}

	for (long i = 0; i < AllGroups.size(); ++i)
	{
		for (long k = 0; k < tmpArrIdsModels[i].size(); ++k)
		{
			for (long j = 0; j < tmpArrIdsModels[i][k].size(); ++j)
			{
				AllGroups[i]->ArrModels[k][j] = AllModels[tmpArrIdsModels[i][k][j]];
			}
		}
	}

	InitArrIndexPtr();

	fclose(file);
}

void StaticGeom::LoadSplit(Segment** Split, FILE* file, Array<Segment**> * queue)
{
	float3 jmin, jmax;
	(*Split) = new Segment();

	fread(&jmin.x, sizeof(float), 1, file);
	fread(&jmin.y, sizeof(float), 1, file);
	fread(&jmin.z, sizeof(float), 1, file);

	fread(&jmax.x, sizeof(float), 1, file);
	fread(&jmax.y, sizeof(float), 1, file);
	fread(&jmax.z, sizeof(float), 1, file);

	(*Split)->BoundVolumeSys = SGCore_CrBound();
	(*Split)->BoundVolumeSys->SetMinMax(&jmin, &jmax);


	fread(&jmin.x, sizeof(float), 1, file);
	fread(&jmin.y, sizeof(float), 1, file);
	fread(&jmin.z, sizeof(float), 1, file);

	fread(&jmax.x, sizeof(float), 1, file);
	fread(&jmax.y, sizeof(float), 1, file);
	fread(&jmax.z, sizeof(float), 1, file);

	(*Split)->BoundVolumeP = SGCore_CrBound();
	(*Split)->BoundVolumeP->SetMinMax(&jmin, &jmax);

	fread(&(*Split)->CountAllPoly, sizeof(uint32_t), 1, file);

	fread(&(*Split)->BFNonEnd, sizeof(int8_t), 1, file);

	if ((*Split)->BFNonEnd)
	{
		bool isexists = false;

		for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; i++)
		{
			fread(&isexists, sizeof(int8_t), 1, file);
			if (isexists)
				queue->push_back(&((*Split)->Splits[i]));

			isexists = false;
		}
	}
	else
	{
		fread(&(*Split)->CountSubSet, sizeof(uint32_t), 1, file);

		(*Split)->NumberGroupModel = new uint32_t[(*Split)->CountSubSet];
		(*Split)->NumberGroup = new uint32_t[(*Split)->CountSubSet];
		(*Split)->CountPoly = new uint32_t[(*Split)->CountSubSet];

		fread((*Split)->NumberGroupModel, sizeof(uint32_t)*(*Split)->CountSubSet, 1, file);
		fread((*Split)->NumberGroup, sizeof(uint32_t)*(*Split)->CountSubSet, 1, file);
		fread((*Split)->CountPoly, sizeof(uint32_t)*(*Split)->CountSubSet, 1, file);

		(*Split)->ArrPoly = new uint32_t*[(*Split)->CountSubSet];

		for (long i = 0; i < (*Split)->CountSubSet; ++i)
		{
			(*Split)->ArrPoly[i] = new uint32_t[(*Split)->CountPoly[i] * 3];
			fread((*Split)->ArrPoly[i], sizeof(uint32_t)*(*Split)->CountPoly[i] * 3, 1, file);
		}
	}
}

void StaticGeom::SortGroup(float3* viewpos, int sort_mtl)
{
	float tmpradius = 0;
	float tmpsidt = 0;
	float3 tmpcenter = float3(0,0,0);
	Model* tmpmodel;
	Array<float> arr_dist;

	long tmp_index_gd = 0;

	float tmpdist = 999999;

	for (int i = 0; i < AllModels.size(); ++i)
	{
		tmpmodel = AllModels[i];
		for (int k = 0; k < tmpmodel->SubSet.size(); ++k)
		{
			//if (sort_mtl == AllGroups[tmpmodel->SubSet[k].idgroup]->SortGroup)
			
			tmpcenter = (tmpmodel->SubSet[k].Min + tmpmodel->SubSet[k].Max) * 0.5f;
			tmpradius = SMVector3Length(tmpcenter - tmpmodel->SubSet[k].Max);
			//tmpmodel->GroupDist[k] = SMVector3Length((tmpcenter - (*viewpos))) - tmpradius;
			
			if (DistGroup.size() <= tmp_index_gd || !DistGroup[tmp_index_gd])
				DistGroup[tmp_index_gd] = new InfoGroup();

			DistGroup[tmp_index_gd]->count = 0;
			DistGroup[tmp_index_gd]->dist = SMVector3Length((tmpcenter - (*viewpos))) - tmpradius;
			DistGroup[tmp_index_gd]->group = k;
			DistGroup[tmp_index_gd]->g_group = tmpmodel->SubSet[k].idgroup;
			DistGroup[tmp_index_gd]->model = i;

			++tmp_index_gd;
		}
	}

	for (int i = 0; i < DistGroup.size(); ++i)
	{
		InfoGroup* tmpig = DistGroup[i];
		
		int pos = i;
		for (int k = i+1; k < DistGroup.size(); ++k)
		{
			InfoGroup* tmpig2 = DistGroup[k];
			int qwert = 0;

			if (tmpig->dist > tmpig2->dist)
			{
				pos = k;
				tmpig = tmpig2;

				//++(tmpig->count);
			}
		}

		DistGroup[pos] = DistGroup[i];
		DistGroup[i] = tmpig;

		int qwert = 0;
	}
}


void StaticGeom::GetIntersectedRayY2(float3* pos, Segment** arrsplits, DWORD *count, Segment* comsegment, ID curr_splits_ids_render)
{
	float3 jmin, jmax;
	comsegment->BoundVolumeP->GetMinMax(&jmin, &jmax);

	if (comsegment->CountAllPoly > 0 && pos->x >= jmin.x && pos->z >= jmin.z && pos->x <= jmax.x && pos->z <= jmax.z)
	{
		if (comsegment->BFNonEnd)
		{
			for (int j = 0; j<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; j++)
			{
				if (comsegment->Splits[j])
					GetIntersectedRayY2(pos, arrsplits, count, comsegment->Splits[j], curr_splits_ids_render);
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
}

bool StaticGeom::GetIntersectedRayY(float3* pos)
{
	jpos = (D3DXVECTOR3)*pos;
	BOOL Hit;
	float u, v, Dist, CountDist;
	Dist = -1000;
	float tmpy = -1000;
	CountDist = -1000;
	bool is_find = false;

	for (int id = 0; id < AllModels.size(); ++id)
	{
		ArrComFor[1]->arr[id]->CountCom = 0;

		GetIntersectedRayY2(pos, ArrComFor[1]->arr[id]->Arr, &(ArrComFor[1]->arr[id]->CountCom), AllModels[id]->ArrSplits, ArrComFor[1]->arr[id]->Count);

		for (DWORD k = 0; k<ArrComFor[1]->arr[id]->CountCom; ++k)
		{
			for (DWORD group = 0; group<ArrComFor[1]->arr[id]->Arr[k]->CountSubSet; group++)
			{
				if (AllGroups[ArrComFor[1]->arr[id]->Arr[k]->NumberGroup[group]]->SortGroup > 0)
					continue;

				ID idbuff = AllModels[id]->SubSet[ArrComFor[1]->arr[id]->Arr[k]->NumberGroupModel[group]].idbuff;
				ID idgroup = ArrComFor[1]->arr[id]->Arr[k]->NumberGroup[group];

				vertex_static* pData = 0;
				if (FAILED(AllGroups[idgroup]->VertexBuffer[idbuff]->Lock(0, 0, (void**)&pData, 0)))
					continue;

				for (DWORD numpoly = 0; numpoly<ArrComFor[1]->arr[id]->Arr[k]->CountPoly[group] * 3; numpoly += 3)
				{
					Dist = -1000.f;
					//находим все 3 вершины
					/*jvec0 = ArrMeshVertex2[ArrComFor[1].arr[id]->Arr[k]->ArrPoly[group][numpoly]];
					jvec1 = ArrMeshVertex2[ArrComFor[1].arr[id]->Arr[k]->ArrPoly[group][numpoly + 1]];
					jvec2 = ArrMeshVertex2[ArrComFor[1].arr[id]->Arr[k]->ArrPoly[group][numpoly + 2]];*/
					
					//jvec0 = AllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr[ArrComFor[1]->arr[id]->Arr[k]->ArrPoly[group][numpoly]];
					//jvec1 = AllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr[ArrComFor[1]->arr[id]->Arr[k]->ArrPoly[group][numpoly + 1]];
					//jvec2 = AllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr[ArrComFor[1]->arr[id]->Arr[k]->ArrPoly[group][numpoly + 2]];

					jvec0 = pData[ArrComFor[1]->arr[id]->Arr[k]->ArrPoly[group][numpoly]].Pos;
					jvec1 = pData[ArrComFor[1]->arr[id]->Arr[k]->ArrPoly[group][numpoly + 1]].Pos;
					jvec2 = pData[ArrComFor[1]->arr[id]->Arr[k]->ArrPoly[group][numpoly + 2]].Pos;

					Hit = D3DXIntersectTri(&(jvec0.operator D3DXVECTOR3()),
						&(jvec1.operator D3DXVECTOR3()),
						&(jvec2.operator D3DXVECTOR3()),
						&(jpos),
						&jvevyn, &u, &v, &Dist);

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
							&jvevyp, &u, &v, &Dist);
						if (Hit)
						{
							pos->y += Dist * 1000;
							tmpy = pos->y;
							is_find = true;
						}
					}
				}

				AllGroups[idgroup]->VertexBuffer[idbuff]->Unlock();
			}
		}
	}
	pos->y = tmpy;
	return is_find;
}

inline void StaticGeom::GetMinMax(float3* min, float3* max)
{
	BoundVolume->GetMinMax(min, max);
}

ID StaticGeom::AddArrForCom()
{
	IRSData* ttmpdata = new IRSData();
	
	for (int i = 0; i < AllModels.size(); ++i)
	{
		InfoRenderSegments* tmpirs = new InfoRenderSegments();
		tmpirs->Count = AllModels[i]->SplitsIDsRender;
		tmpirs->Arr = new Segment*[AllModels[i]->SplitsIDsRender];
		tmpirs->CountCom = 0;
		ttmpdata->arr.push_back(tmpirs);
	}

	ID id_arr = -1;
	for (int i = 0; i < ArrComFor.size(); ++i)
	{
		if (ArrComFor[i] == 0)
		{
			ArrComFor[i] = ttmpdata;
			id_arr = i;
			break;
		}
	}

	if (id_arr == -1)
	{
		ArrComFor.push_back(ttmpdata);
		id_arr = ArrComFor.size() - 1;
	}

	return id_arr;
}

void StaticGeom::DelArrForCom(ID id_arr)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	mem_delete(ArrComFor[id_arr]);
}

void StaticGeom::AddModelInArrCom(ID id_model)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model, _VOID);

	for (long i = 0; i < ArrComFor.size(); ++i)
	{
		InfoRenderSegments* tmpirs = new InfoRenderSegments();
		tmpirs->Count = AllModels[id_model]->SplitsIDsRender;
		tmpirs->Arr = new Segment*[AllModels[id_model]->SplitsIDsRender];
		tmpirs->CountCom = 0;
		ArrComFor[i]->arr.push_back(tmpirs);
	}
}

void StaticGeom::DelModelInArrCom(ID id_model)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model, _VOID);

	for (int i = 0; i < ArrComFor.size(); ++i)
	{
		if (!(ArrComFor[i]))
			continue;

		mem_delete(ArrComFor[i]->arr[id_model]);
		ArrComFor[i]->arr.erase(id_model);
	}
}

///

char* StaticGeom::GetModelName(ID id)
{
	if (id < AllModels.size())
		return AllModels[id]->Name;

	return 0;
}

const char* StaticGeom::GetModelPathName(ID id)
{
	if (id < AllModels.size())
		return AllModels[id]->PathName;

	return 0;
}

long StaticGeom::GetModelCountPoly(ID id)
{
	if (id < AllModels.size())
		return AllModels[id]->CountPoly;

	return -1;
}

void StaticGeom::GetModelMinMax(ID id, float3* min, float3* max)
{
	if (id >= AllModels.size())
		return;

	AllModels[id]->ArrSplits->BoundVolumeP->GetMinMax(min, max);
}


float3* StaticGeom::GetModelPosition(ID id)
{
	if (id < AllModels.size())
		return &AllModels[id]->Position;

	return 0;
}

float3* StaticGeom::GetModelRotation(ID id)
{
	if (id < AllModels.size())
		return &AllModels[id]->Rotation;

	return 0;
}

float3* StaticGeom::GetModelScale(ID id)
{
	if (id < AllModels.size())
		return &AllModels[id]->Scale;

	return 0;
}

const char* StaticGeom::GetModelLodPath(ID id)
{
	if (id < AllModels.size())
		return AllModels[id]->Lod0.PathName;

	return 0;
}

void StaticGeom::SetModelLodPath(ID id, const char* path)
{
	if (id < AllModels.size() && def_str_validate(path))
	{
		mem_delete(AllModels[id]->Lod0.model);
		AllModels[id]->Lod0.IDsTexs.clear();
		//AllModels[id]->Lod0.model = new DataStaticModel();
		char tmppath[1024];
		sprintf(tmppath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), path);
		SGCore_StaticModelLoad(tmppath, &(AllModels[id]->Lod0.model));

		char tmptex[1024];
		ID tmpidmat;
		for (int i = 0; i < AllModels[id]->Lod0.model->SubsetCount; ++i)
		{
			sprintf(tmptex, "%s.dds", AllModels[id]->Lod0.model->ArrTextures[i]);
			tmpidmat = SGCore_MtlLoad(tmptex, MTL_TYPE_GEOM);
			AllModels[id]->Lod0.IDsTexs.push_back(tmpidmat);
			AllModels[id]->Lod0.SortGroup = SGCore_MtlGetSort(tmpidmat);
		}

		sprintf(AllModels[id]->Lod0.PathName,"%s",path);
		
		AllModels[id]->Lod0.BoundVolume->CalcBound(AllModels[id]->Lod0.model->VertexBuffer, AllModels[id]->Lod0.model->AllVertexCount, sizeof(vertex_static));
		
		ApplyTransformLod(id);
	}
}

void StaticGeom::ApplyTransform(ID id)
{
	if (id >= AllModels.size())
		return;
	Model* tmpmodel = AllModels[id];

	float3 tmppos = tmpmodel->Position - tmpmodel->OldPosition;
	float3 tmprot = tmpmodel->Rotation - tmpmodel->OldRotation;
	float3 tmpscale = tmpmodel->Scale / tmpmodel->OldScale;

	float3 jmin, jmax;
	tmpmodel->ArrSplits->BoundVolumeP->GetMinMax(&jmin, &jmax);
	float3 jcentererpos = (jmax + jmin)*0.5f;
	
	RotationMat = SMMatrixRotationX(tmprot.x) * SMMatrixRotationY(tmprot.y) * SMMatrixRotationZ(tmprot.z);
	WorldMat = SMMatrixScaling(tmpscale) * RotationMat * SMMatrixTranslation(tmppos);


	ApplyTransformLod(id);

	vertex_static* pData;
	float3_t* pData2;
	long idgroup;
	long idbuff;

	long vertexstart, vertexcount;
	
	for (long i = 0; i < tmpmodel->SubSet.size(); ++i)
	{
		tmpmodel->SubSet[i].Center = SMVector3Transform(tmpmodel->SubSet[i].Center, WorldMat);
		D3DXPlaneTransform(&tmpmodel->SubSet[i].Plane, &tmpmodel->SubSet[i].Plane, &(WorldMat.operator D3DXMATRIX()));


		tmpmodel->SubSet[i].Min.x -= jcentererpos.x;
		tmpmodel->SubSet[i].Min.y -= jcentererpos.y;
		tmpmodel->SubSet[i].Min.z -= jcentererpos.z;

		tmpmodel->SubSet[i].Min = SMVector3Transform(tmpmodel->SubSet[i].Min, WorldMat);

		tmpmodel->SubSet[i].Min.x += jcentererpos.x;
		tmpmodel->SubSet[i].Min.y += jcentererpos.y;
		tmpmodel->SubSet[i].Min.z += jcentererpos.z;


		tmpmodel->SubSet[i].Max.x -= jcentererpos.x;
		tmpmodel->SubSet[i].Max.y -= jcentererpos.y;
		tmpmodel->SubSet[i].Max.z -= jcentererpos.z;

		tmpmodel->SubSet[i].Max = SMVector3Transform(tmpmodel->SubSet[i].Max, WorldMat);

		tmpmodel->SubSet[i].Max.x += jcentererpos.x;
		tmpmodel->SubSet[i].Max.y += jcentererpos.y;
		tmpmodel->SubSet[i].Max.z += jcentererpos.z;



		idgroup = tmpmodel->SubSet[i].idgroup;
		idbuff = tmpmodel->SubSet[i].idbuff;
		//pData2 = AllGroups[idgroup]->VertexBufferOrigin[idbuff]->arr;
		if (!FAILED(AllGroups[idgroup]->VertexBuffer[idbuff]->Lock(0, 0, (void**)&pData, 0)))
		{
			vertexstart = tmpmodel->SubSet[i].VertexStart;
			vertexcount = tmpmodel->SubSet[i].VertexCount;

			for (int k = 0; k < vertexcount; ++k)
			{
				pData[vertexstart + k].Pos.x -= jcentererpos.x;
				pData[vertexstart + k].Pos.y -= jcentererpos.y;
				pData[vertexstart + k].Pos.z -= jcentererpos.z;

				pData[vertexstart + k].Pos = SMVector3Transform(pData[vertexstart + k].Pos, WorldMat);
				pData[vertexstart + k].Norm = SMVector3Transform(pData[vertexstart + k].Norm, RotationMat);

				pData[vertexstart + k].Pos.x += jcentererpos.x;
				pData[vertexstart + k].Pos.y += jcentererpos.y;
				pData[vertexstart + k].Pos.z += jcentererpos.z;
			}
			AllGroups[idgroup]->VertexBuffer[idbuff]->Unlock();
		}
	}

	Array<Segment*, GEOM_DEFAULT_RESERVE_COM> queue;
	long tmpcount = 0;
	queue.push_back(tmpmodel->ArrSplits);

	float3 tmpmin, tmpmax;

	while (queue.size())
	{
		queue[0]->BoundVolumeSys->GetMinMax(&tmpmin, &tmpmax);
		tmpmin -= jcentererpos;
		tmpmax -= jcentererpos;

		tmpmin = SMVector3Transform(tmpmin, WorldMat);
		tmpmax = SMVector3Transform(tmpmax, WorldMat);

		tmpmin += jcentererpos;
		tmpmax += jcentererpos;
		queue[0]->BoundVolumeSys->SetMinMax(&tmpmin, &tmpmax);

		queue[0]->BoundVolumeP->GetMinMax(&tmpmin, &tmpmax);
		tmpmin -= jcentererpos;
		tmpmax -= jcentererpos;

		tmpmin = SMVector3Transform(tmpmin, WorldMat);
		tmpmax = SMVector3Transform(tmpmax, WorldMat);

		tmpmin += jcentererpos;
		tmpmax += jcentererpos;
		queue[0]->BoundVolumeP->SetMinMax(&tmpmin, &tmpmax);

		if (queue[0]->BFNonEnd)
		{
			for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
			{
				if (queue[0]->Splits[i])
					queue.push_back(queue[0]->Splits[i]);
			}
		}

		queue.erase(0);
		++tmpcount;
	}

	tmpmodel->OldPosition = tmpmodel->Position;
	tmpmodel->OldRotation = tmpmodel->Rotation;
	tmpmodel->OldScale = tmpmodel->Scale;
}

void StaticGeom::ApplyTransformLod(ID id)
{
	if (id >= AllModels.size() || AllModels[id]->Lod0.model == 0)
		return;

	Model* tmpmodel = AllModels[id];
	vertex_static* pData;

	float3 jmin, jmax;
	AllModels[id]->Lod0.BoundVolume->GetMinMax(&jmin, &jmax);
	float3 jcentererpos = (jmax + jmin)*0.5f;

	tmpmodel->Lod0.model->VertexBuffer->Lock(0, 0, (void**)&pData, 0);

	for (long i = 0; i < tmpmodel->Lod0.model->AllVertexCount; ++i)
	{
		pData[i].Pos.x -= jcentererpos.x;
		pData[i].Pos.y -= jcentererpos.y;
		pData[i].Pos.z -= jcentererpos.z;
		pData[i].Pos = SMVector3Transform(pData[i].Pos, WorldMat);
		pData[i].Pos.x += jcentererpos.x;
		pData[i].Pos.y += jcentererpos.y;
		pData[i].Pos.z += jcentererpos.z;
	}

	tmpmodel->Lod0.model->VertexBuffer->Unlock();

	
	jmin -= jcentererpos;
	jmax -= jcentererpos;

	jmin = SMVector3Transform(jmin, WorldMat);
	jmax = SMVector3Transform(jmax, WorldMat);

	jmin += jcentererpos;
	jmax += jcentererpos;
	tmpmodel->Lod0.BoundVolume->SetMinMax(&jmin, &jmax);
}

void StaticGeom::Clear()
{
	DelArrIndexPtr();
	for (long i = 0; i < AllGroups.size(); ++i)
	{
		mem_delete(AllGroups[i]);
	}
	AllGroups.clear();

	while (AllModels.size() > 0)
	{
		DelModelInArrCom(0);
		mem_delete(AllModels[0]);
		AllModels.erase(0);
	}
	AllModels.clear();

	while (ArrComFor.size() > 2)
	{
		mem_delete(ArrComFor[2]);
		ArrComFor.erase(2);
	}


	mem_delete_a(ArrMeshVertex);
	mem_delete_a(ArrMeshIndex);
	CountVertex = 0;

	float3 jmin(0, 0, 0), jmax(0, 0, 0);
	BoundVolume->SetMinMax(&jmin,&jmax);
}

void StaticGeom::GetArrBuffsGeom(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, ID*** arr_mtl, int32_t** arr_count_index, int32_t* count_models)
{
	(*count_models) = AllModels.size();
	(*arr_vertex) = new float3_t*[AllModels.size()];
	(*arr_index) = new uint32_t*[AllModels.size()];
	(*arr_mtl) = new ID*[AllModels.size()];
	(*arr_count_vertex) = new int32_t[AllModels.size()];
	(*arr_count_index) = new int32_t[AllModels.size()];

	vertex_static* pData;

	for (long i = 0; i < AllModels.size(); ++i)
	{
		long sizevertex = 0;
		long sizeindex = 0;
		for (long k = 0; k < AllModels[i]->SubSet.size(); ++k)
		{
			sizevertex += AllModels[i]->SubSet[k].VertexCount;
			sizeindex += AllModels[i]->SubSet[k].IndexCount;
		}

		(*arr_vertex)[i] = new float3_t[sizevertex];
		(*arr_index)[i] = new uint32_t[sizeindex];
		(*arr_mtl)[i] = new ID[sizeindex];

		(*arr_count_vertex)[i] = sizevertex;
		(*arr_count_index)[i] = sizeindex;

		Array<long,1024> tmpbiasindex;

		long curr_size_vertex = 0;
		long tmp_pre_bias = 0;
		for (long k = 0; k < AllModels[i]->SubSet.size(); ++k)
		{
			AllGroups[AllModels[i]->SubSet[k].idgroup]->VertexBuffer[AllModels[i]->SubSet[k].idbuff]->Lock(0, 0, (void**)&pData, 0);

			for (long j = 0; j < AllModels[i]->SubSet[k].VertexCount; ++j)
			{
				(*arr_vertex)[i][curr_size_vertex] = (float3_t)pData[AllModels[i]->SubSet[k].VertexStart + j].Pos;
				++curr_size_vertex;
			}

			tmpbiasindex[k] = 0;
			tmpbiasindex[k] = (tmpbiasindex[k] - AllModels[i]->SubSet[k].VertexStart) + tmp_pre_bias;
			tmp_pre_bias += AllModels[i]->SubSet[k].VertexCount;

			AllGroups[AllModels[i]->SubSet[k].idgroup]->VertexBuffer[AllModels[i]->SubSet[k].idbuff]->Unlock();
		}

		Array<Segment*, GEOM_DEFAULT_RESERVE_COM> queue;
		long tmpcount = 0;
		queue.push_back(AllModels[i]->ArrSplits);
		long curr_size_index = 0;
		while (queue.size())
		{
			if (queue[0]->BFNonEnd)
			{
				for (int i = 0; i<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++i)
				{
					if (queue[0]->Splits[i])
						queue.push_back(queue[0]->Splits[i]);
				}
			}
			else
			{
				Segment* split = queue[0];
				for (long k = 0; k < split->CountSubSet; ++k)
				{
					for (long j = 0; j < split->CountPoly[k]*3; ++j)
					{
						(*arr_index)[i][curr_size_index] = split->ArrPoly[k][j] + tmpbiasindex[split->NumberGroupModel[k]];
						(*arr_mtl)[i][curr_size_index] = AllGroups[split->NumberGroup[k]]->idtex;
						++curr_size_index;
					}
				}
			}

			queue.erase(0);
			++tmpcount;
		}
	}
}

ID StaticGeom::GetModelCountGroups(ID id)
{
	STATIC_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id, -1);

	return AllModels[id]->SubSet.size();
}

ID StaticGeom::GetModelGroupIDMat(ID id, ID group)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group , - 1);

	return AllModels[id]->IDTex[group];
}

void StaticGeom::GetModelGroupCenter(ID id, ID group, float3_t* center)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group, _VOID);

	*center = AllModels[id]->SubSet[group].Center;
}

void StaticGeom::GetModelGroupMin(ID id, ID group, float3_t* min)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group, _VOID);

	*min = AllModels[id]->SubSet[group].Min;
}

void StaticGeom::GetModelGroupMax(ID id, ID group, float3_t* max)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group, _VOID);

	*max = AllModels[id]->SubSet[group].Max;
}

void StaticGeom::GetModelGroupPlane(ID id, ID group, D3DXPLANE* plane)
{
	STATIC_PRECOND_ERR_ID_GROUP(id, group, _VOID);

	*plane = AllModels[id]->SubSet[group].Plane;
}

void StaticGeom::GetPartBeam(float3* pos, float3 * dir, Segment** arrsplits, DWORD *count, Segment* comsegment, ID curr_splits_ids_render)
{
	float3 center;
	float radius;
	comsegment->BoundVolumeP->GetSphere(&center, &radius);

	float distsqr = SGCore_0DistancePointBeam2(center, *pos, *dir);
	if (comsegment->CountAllPoly > 0 && distsqr <= radius*radius)
	{
		if (comsegment->BFNonEnd)
		{
			for (int j = 0; j<GEOM_COUNT_TYPE_SEGMENTATION_OCTO; ++j)
			{
				if (comsegment->Splits[j])
					GetPartBeam(pos, dir, arrsplits, count, comsegment->Splits[j], curr_splits_ids_render);
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

bool StaticGeom::TraceBeam(float3* start, float3* dir, float3* _res, ID* idmodel, ID* idmtl)
{
	if (AllModels.size() <= 0)
		return false;

	SXTriangle tmptri;
	float dist;
	bool tmpiscom = true;
	float3 ip;
	float3 res;
	float3 il;
	res = (*start) + float3(10000.0f, 10000.0f, 10000.0f);
	il = (*dir) * 10000.0f;
	bool found = false;

	for (int id = 0; id < AllModels.size(); ++id)
	{
		ArrComFor[1]->arr[id]->CountCom = 0;

		GetPartBeam(start, dir, ArrComFor[1]->arr[id]->Arr, &(ArrComFor[1]->arr[id]->CountCom), AllModels[id]->ArrSplits, ArrComFor[1]->arr[id]->Count);

		for (DWORD k = 0; k<ArrComFor[1]->arr[id]->CountCom; ++k)
		{
			for (DWORD group = 0; group<ArrComFor[1]->arr[id]->Arr[k]->CountSubSet; group++)
			{
				ID idbuff = AllModels[id]->SubSet[ArrComFor[1]->arr[id]->Arr[k]->NumberGroupModel[group]].idbuff;
				ID idgroup = ArrComFor[1]->arr[id]->Arr[k]->NumberGroup[group];

				vertex_static* pData = 0;
				if (FAILED(AllGroups[idgroup]->VertexBuffer[idbuff]->Lock(0, 0, (void**)&pData, 0)))
					continue;

				for (DWORD numpoly = 0; numpoly<ArrComFor[1]->arr[id]->Arr[k]->CountPoly[group] * 3; numpoly += 3)
				{
					tmptri.a = pData[ArrComFor[1]->arr[id]->Arr[k]->ArrPoly[group][numpoly]].Pos;
					tmptri.b = pData[ArrComFor[1]->arr[id]->Arr[k]->ArrPoly[group][numpoly + 1]].Pos;
					tmptri.c = pData[ArrComFor[1]->arr[id]->Arr[k]->ArrPoly[group][numpoly + 2]].Pos;

					if (tmptri.IntersectLine((*start), il, &ip))
					{
						if (SMVector3Length2((*start) - res) > SMVector3Length2((*start) - ip))
						{
							res = ip;
							found = true;

							if (idmodel)
								*idmodel = id;

							if (idmtl)
								*idmtl = AllGroups[idgroup]->idtex;
						}
					}
				}

				AllGroups[idgroup]->VertexBuffer[idbuff]->Unlock();
			}
		}
	}

	if (found && _res)
		*_res = res;

	return found;
}
