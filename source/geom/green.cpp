
#include <geom\\green.h>

Green::Green()
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
		D3DDECL_END()
	};

	Green::DXDevice->CreateVertexDeclaration(InstanceGreen, &VertexDeclarationGreen);

	Green::DXDevice->CreateVertexBuffer(
		GREEN_MAX_ELEM_IN_DIP * sizeof(float)* 6,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
		&TransVertBuf,
		0);

	IRSData tmparr;
	ArrComFor.push_back(tmparr);
}

Green::~Green()
{
	ArrComFor.clear();

	for (long i = 0; i < ArrModels.size(); ++i)
	{
		mem_delete(ArrModels[i]);
	}

	ArrModels.clear();
	mem_release(TransVertBuf);
	mem_release(VertexDeclarationGreen);
}

Green::Model::Model()
{
	ArrLod[0] = ArrLod[1] = ArrLod[2] = 0;
	AllCountGreen = 0;
	SplitsIDs = 0;
	SplitsIDsRender = 0;
	ArrSplits = 0;
	AllTrans = 0;
	ArrLod[0] = ArrLod[1] = ArrLod[2] = 0;
}

Green::Model::~Model()
{
	mem_delete(ArrSplits);
	mem_delete(AllTrans);
	mem_delete(ArrLod[0]);
	mem_delete(ArrLod[1]);
	mem_delete(ArrLod[2]);
}

Green::Segment::Segment()
{
	for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; ++i)
	{
		Splits[i] = 0;
		SortId[i] = -1;
	}

	Data = 0;
	CountAllGreen = 0;
	BoundVolumeSys = 0;
	BoundVolumeP = 0;
	ID = -1;
	SID = -1;
	BFNonEnd = false;
}

Green::Segment::~Segment()
{
	for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		mem_delete(Splits[i]);
	mem_delete_a(Data);

	mem_release_del(BoundVolumeSys);
	mem_release_del(BoundVolumeP);
}

Green::Lod::Lod()
{

}

Green::Lod::~Lod()
{
	idstex.clear();
}

Green::InfoRenderSegments::InfoRenderSegments()
{
	Arr = 0; Count = 0; CountCom = 0;
}

Green::InfoRenderSegments::~InfoRenderSegments()
{
	mem_delete_a(Arr);
}

Green::IRSData::IRSData()
{
	queue.clear();
	arr.clear();
}

Green::IRSData::~IRSData()
{

}


void Green::OnLostDevice()
{
	mem_release(TransVertBuf);
}

void Green::OnResetDevice()
{
	Green::DXDevice->CreateVertexBuffer(
		GREEN_MAX_ELEM_IN_DIP * sizeof(float)* 6,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
		&TransVertBuf,
		0);
}

void Green::Load(const char* path, const char* lod1, const char* lod2, const char* path_bin_mask, DWORD count_object_in_split)
{
	/*if (!path)
		return;*/

	Model* tmpnewmpdel = new Model();
	tmpnewmpdel->ArrLod[0] = new Lod();
	tmpnewmpdel->ArrLod[1] = new Lod();
	tmpnewmpdel->ArrLod[2] = new Lod();

	SGCore_LoadStaticModel(path, &tmpnewmpdel->ArrLod[0]->model);

	char tmppathtex[1024];
	for (int i = 0; i < tmpnewmpdel->ArrLod[0]->model.SubsetCount; ++i)
	{
		sprintf(tmppathtex, "%s.dds", tmpnewmpdel->ArrLod[0]->model.ArrTextures[i]);
		tmpnewmpdel->ArrLod[0]->idstex[i] = SGCore_LoadTexAddName(tmppathtex);
	}

	SGCore_LoadStaticModel(lod1, &tmpnewmpdel->ArrLod[1]->model);

	for (int i = 0; i < tmpnewmpdel->ArrLod[1]->model.SubsetCount; ++i)
	{
		sprintf(tmppathtex, "%s.dds", tmpnewmpdel->ArrLod[1]->model.ArrTextures[i]);
		tmpnewmpdel->ArrLod[1]->idstex[i] = SGCore_LoadTexAddName(tmppathtex);
	}

	SGCore_LoadStaticModel(lod2, &tmpnewmpdel->ArrLod[2]->model);

	for (int i = 0; i < tmpnewmpdel->ArrLod[2]->model.SubsetCount; ++i)
	{
		sprintf(tmppathtex, "%s.dds", tmpnewmpdel->ArrLod[1]->model.ArrTextures[i]);
		tmpnewmpdel->ArrLod[2]->idstex[i] = SGCore_LoadTexAddName(tmppathtex);
	}


	FILE* file = 0;
	file = fopen(path_bin_mask, "rb");

	char HeaderFile[13];

	fread(HeaderFile, 13, 1, file);

	if (strcmp(HeaderFile, "sxgreentrans") == 0)
	{
		fread(&tmpnewmpdel->AllCountGreen, sizeof(DWORD), 1, file);
		tmpnewmpdel->AllTrans = new DataVertex[tmpnewmpdel->AllCountGreen];
		fread(tmpnewmpdel->AllTrans, sizeof(DataVertex), tmpnewmpdel->AllCountGreen, file);
	}
	//CountGrassInVert = mpnewmpdel->AllCountGreen + 1;
	fclose(file);

	PreSegmentation(tmpnewmpdel, count_object_in_split);
	//currSplitsIDs = 0;
	//currSplitsIDsRender = 0;
	SetSplitID(tmpnewmpdel->ArrSplits, &tmpnewmpdel->SplitsIDs, &tmpnewmpdel->SplitsIDsRender);
	//tmpnewmpdel->SplitsIDs = currSplitsIDs;
	//tmpnewmpdel->SplitsIDsRender = currSplitsIDsRender;

	/*InfoRenderSegments* tmpirs = new InfoRenderSegments();
	tmpirs->Count = tmpnewmpdel->SplitsIDsRender;
	tmpirs->Arr = new Segment*[tmpnewmpdel->SplitsIDsRender];
	tmpirs->CountCom = 0;*/

	ArrModels.push_back(tmpnewmpdel);
	AddModelInArrCom(ArrModels.size()-1);
}

void Green::PreSegmentation(Model* model, DWORD count_object_in_split)
{
	model->ArrSplits = new Segment();

	float3 tmpMin, tmpMax;
	float3 tmpMin2, tmpMax2;
	model->ArrSplits->BoundVolumeSys = SGCore_CrBound();
	SGCore_FCompBoundBox(model->ArrLod[0]->model.VertexBuffer, &(model->ArrSplits->BoundVolumeSys), model->ArrLod[0]->model.AllVertexCount, sizeof(vertex_static));

	model->ArrSplits->BoundVolumeSys->GetMinMax(&tmpMin2, &tmpMax2);
	model->BBMax = tmpMax2 * 2.f;
	model->BBMin = tmpMin2 * 2.f;
	tmpMin = model->AllTrans[0].Position;
	tmpMax = model->AllTrans[0].Position;

	for (DWORD i = 0; i<model->AllCountGreen; i++)
	{
		if (model->AllTrans[i].Position.x > tmpMax.x)
			tmpMax.x = model->AllTrans[i].Position.x;

		if (model->AllTrans[i].Position.y > tmpMax.y)
			tmpMax.y = model->AllTrans[i].Position.y;

		if (model->AllTrans[i].Position.z > tmpMax.z)
			tmpMax.z = model->AllTrans[i].Position.z;


		if (model->AllTrans[i].Position.x < tmpMin.x)
			tmpMin.x = model->AllTrans[i].Position.x;

		if (model->AllTrans[i].Position.y < tmpMin.y)
			tmpMin.y = model->AllTrans[i].Position.y;

		if (model->AllTrans[i].Position.z < tmpMin.z)
			tmpMin.z = model->AllTrans[i].Position.z;
	}

	tmpMax.x += model->BBMax.x;
	tmpMax.y += model->BBMax.y;
	tmpMax.z += model->BBMax.z;

	tmpMin.x += model->BBMin.x;
	tmpMin.y += model->BBMin.y;
	tmpMin.z += model->BBMin.z;

	float tmpX = tmpMax.x - tmpMin.x;
	float tmpY = tmpMax.y - tmpMin.y;
	float tmpZ = tmpMax.z - tmpMin.z;

	model->ArrSplits->BoundVolumeP = SGCore_CrBound();
	model->ArrSplits->BoundVolumeP->SetMinMax(&tmpMin, &tmpMax);
	//SkyXEngine::Core::WorkModel::CreateBoundingBoxMesh(&tmpMin, &tmpMax, &(model->ArrSplits->BoundBox));

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

	model->ArrSplits->BoundVolumeSys->SetMinMax(&tmpMin, &tmpMax);

	model->ArrSplits->CountAllGreen = model->AllCountGreen;
	model->ArrSplits->Data = new DataVertex[model->AllCountGreen];
	memcpy(model->ArrSplits->Data, model->AllTrans, sizeof(DataVertex)* model->AllCountGreen);

	if (model->ArrSplits->CountAllGreen > 0 && model->ArrSplits->CountAllGreen > count_object_in_split)
	{
		model->ArrSplits->BFNonEnd = true;
		CycleSegmentation(model->ArrSplits, model, count_object_in_split);
	}
	else
	{
		model->ArrSplits->BFNonEnd = false;
	}
}

void Green::CycleSegmentation(Segment* Split, Model* mesh, DWORD count_object_in_split)
{
	Array<Segment*> queue;
	long tmpcount = 0;
	queue.push_back(Split);

	while (queue.size())
	{
		if (queue[0]->BFNonEnd)
		{
			Segmentation(queue[0], mesh, count_object_in_split);

			for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; i++)
			{
				if (queue[0]->Splits[i])
					queue.push_back(queue[0]->Splits[i]);
			}
		}

		queue.erase(0);
		++tmpcount;
	}
}

void Green::Segmentation(Segment* Split, Model* mesh, DWORD count_object_in_split)
{
	Array<DWORD> ArrPoly[4];

	for (int i = 0; i<4; i++)
		Split->Splits[i] = new Segment();

	ISXBound* ArrBound[4];
	for (int i = 0; i < 4; ++i)
		ArrBound[i] = SGCore_CrBound();

	SGCore_0ComBoundBoxArr4(Split->BoundVolumeSys, (ArrBound));

	for (int i = 0; i<4; i++)
	{
		Split->Splits[i]->BoundVolumeSys = ArrBound[i];
		Split->Splits[i]->BoundVolumeP = ArrBound[i];
	}

	bool *tmp_arr_mesh_poly = new bool[Split->CountAllGreen];
	for (DWORD i = 0; i<Split->CountAllGreen; i++)
		tmp_arr_mesh_poly[i] = true;

	DWORD tmpCountNonIn = 0;
	float3 tmpMin, tmpMax;
	for (WORD i = 0; i<4; i++)
	{
		Split->Splits[i]->BoundVolumeSys->GetMinMax(&tmpMin, &tmpMax);
		//SGCore_FCreateBoundingBoxMesh(&tmpMin, &tmpMax, &(Split->Splits[i]->BoundBox));

		for (DWORD j = 0; j<Split->CountAllGreen; j++)
		{
			//если позици¤ провер¤емого полигона находитьс¤ в пределах ограничивающего паралелепипеда
			if (
				(long(tmpMax.x * 1000) >= long(Split->Data[j].Position.x * 1000) && long(tmpMin.x * 1000) <= long(Split->Data[j].Position.x * 1000))
				&&
				(long(tmpMax.z * 1000) >= long(Split->Data[j].Position.z * 1000) && long(tmpMin.z * 1000) <= long(Split->Data[j].Position.z * 1000))
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
		Split->Splits[i]->CountAllGreen = ArrPoly[i].size();

		if (Split->Splits[i]->CountAllGreen > 0)
		{
			Split->Splits[i]->Data = new DataVertex[Split->Splits[i]->CountAllGreen];

			float3 comMax = Split->Data[ArrPoly[i][0]].Position;
			float3 comMin = Split->Data[ArrPoly[i][0]].Position;

			for (DWORD k = 0; k<ArrPoly[i].size(); k++)
			{
				Split->Splits[i]->Data[k] = Split->Data[ArrPoly[i][k]];

				if (Split->Data[ArrPoly[i][k]].Position.y > comMax.y)
					comMax.y = Split->Data[ArrPoly[i][k]].Position.y;

				if (Split->Data[ArrPoly[i][k]].Position.y < comMin.y)
					comMin.y = Split->Data[ArrPoly[i][k]].Position.y;


				if (Split->Data[ArrPoly[i][k]].Position.x > comMax.x)
					comMax.x = Split->Data[ArrPoly[i][k]].Position.x;

				if (Split->Data[ArrPoly[i][k]].Position.x < comMin.x)
					comMin.x = Split->Data[ArrPoly[i][k]].Position.x;


				if (Split->Data[ArrPoly[i][k]].Position.z > comMax.z)
					comMax.z = Split->Data[ArrPoly[i][k]].Position.z;

				if (Split->Data[ArrPoly[i][k]].Position.z < comMin.z)
					comMin.z = Split->Data[ArrPoly[i][k]].Position.z;
			}

			Split->Splits[i]->BoundVolumeSys->GetMinMax(&tmpMin, &tmpMax);
			tmpMax.y = comMax.y + mesh->BBMax.y;
			tmpMin.y = comMin.y + mesh->BBMin.y;

			Split->Splits[i]->BoundVolumeSys->SetMinMax(&tmpMin, &tmpMax);

			tmpMax.x = comMax.x + mesh->BBMax.x;
			tmpMax.y = comMax.y + mesh->BBMax.y;
			tmpMax.z = comMax.z + mesh->BBMax.z;

			tmpMin.x = comMin.x + mesh->BBMin.x;
			tmpMin.y = comMin.y + mesh->BBMin.y;
			tmpMin.z = comMin.z + mesh->BBMin.z;

			Split->Splits[i]->BoundVolumeP->SetMinMax(&tmpMin, &tmpMax);

			ArrPoly[i].clear();
		}

		if (Split->Splits[i]->CountAllGreen > 0 && Split->Splits[i]->CountAllGreen > count_object_in_split)
			Split->Splits[i]->BFNonEnd = true;
		else if (Split->Splits[i]->CountAllGreen <= 0)
		{
			mem_delete(Split->Splits[i]);
		}
		else
			Split->Splits[i]->BFNonEnd = false;
	}

	mem_delete_a(Split->Data);
	mem_delete_a(tmp_arr_mesh_poly);
}

void Green::SetSplitID(Segment* Split, long* SplitsIDs, long* SplitsIDsRender)
{
	Array<Segment*, GREEN_DEFAULT_RESERVE_COM> queue;
	long tmpcount = 0;
	queue.push_back(Split);

	while (queue.size())
	{
		SetSplitID2(queue[0], SplitsIDs, SplitsIDsRender, &queue);

		queue.erase(0);
		++tmpcount;
	}
}

void Green::SetSplitID2(Segment* Split, long* SplitsIDs, long* SplitsIDsRender, Array<Segment*, GREEN_DEFAULT_RESERVE_COM>* queue)
{
	if (Split && Split->BFNonEnd)
	{
		Split->ID = (*SplitsIDs);
		++(*SplitsIDs);
		for (int i = 0; i<4; i++)
			queue->push_back(Split->Splits[i]);
	}
	else if (Split)
	{
		Split->ID = (*SplitsIDs);
		++(*SplitsIDs);
		++(*SplitsIDsRender);
	}
}

void Green::CPUFillingArrIndeces(ISXFrustum* frustum, float3* viewpos, long id_arr)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr);
	
	DWORD tmpcount = 0;
	DWORD* tmpcountcom = 0;
	Segment** tmpsegments = 0;
	for (int i = 0; i < ArrModels.size(); ++i)
	{
		tmpcount = ArrComFor[id_arr].arr[i]->Count;
		ArrComFor[id_arr].arr[i]->CountCom = 0;
		tmpcountcom = &(ArrComFor[id_arr].arr[i]->CountCom);
		tmpsegments = ArrComFor[id_arr].arr[i]->Arr;
		
		ArrComFor[id_arr].queue.clear();
		long tmpcount = 0;
		ArrComFor[id_arr].queue.push_back(ArrModels[i]->ArrSplits);

		while (ArrComFor[id_arr].queue.size())
		{
			ComRecArrIndeces(frustum, tmpsegments, tmpcountcom, ArrComFor[id_arr].queue[0], viewpos, &ArrComFor[id_arr].queue, tmpcount);

			ArrComFor[id_arr].queue.erase(0);
			++tmpcount;
		}
	}
}

void Green::ComRecArrIndeces(ISXFrustum* frustum, Segment** arrsplits, DWORD *count, Segment* comsegment, float3* viewpos, Array<Segment*, GREEN_DEFAULT_RESERVE_COM>* queue, DWORD curr_splits_ids_render)
{
	float3 jcenter;
	float jradius;
	comsegment->BoundVolumeP->GetSphere(&jcenter, &jradius);

	if (comsegment->CountAllGreen > 0 && frustum->SphereInFrustum(&jcenter, jradius))
	{
			if (comsegment->BFNonEnd)
			{
				if (Green::UseSortFrontToBackSplits)
				{
					for (int q = 0; q<GREEN_COUNT_TYPE_SEGMENTATION; ++q)
					{
						comsegment->SortId[q] = -1;
						if (comsegment->Splits[q])
						{
							comsegment->Splits[q]->BoundVolumeP->GetSphere(&jcenter, &jradius);
							comsegment->Splits[q]->DistForCamera = SMVector3Length2((jcenter - (*viewpos))) - jradius*jradius;
						}
					}

					float pl1, pl2;
					int tmpCountGreater = 0;
					for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; ++i)
					{
						if (comsegment->Splits[i])
						{
							tmpCountGreater = 0;
							pl1 = comsegment->Splits[i]->DistForCamera;

							for (int k = 0; k<GREEN_COUNT_TYPE_SEGMENTATION; ++k)
							{
								if (comsegment->Splits[k])
								{
									pl2 = comsegment->Splits[k]->DistForCamera;

									if (i != k && pl2 >= pl1)
										++tmpCountGreater;
								}
							}

							bool tmpisnend = true;
							while (tmpisnend)
							{
								if (comsegment->SortId[tmpCountGreater] == -1)
								{
									comsegment->SortId[tmpCountGreater] = i;
									tmpisnend = false;
								}
								else
									--tmpCountGreater;
							}
						}
					}

					for (int j = 0; j<GREEN_COUNT_TYPE_SEGMENTATION; ++j)
					{
						if (comsegment->SortId[(GREEN_COUNT_TYPE_SEGMENTATION - 1) - j] != -1 && comsegment->Splits[comsegment->SortId[(GREEN_COUNT_TYPE_SEGMENTATION-1) - j]])
							queue->push_back(comsegment->Splits[comsegment->SortId[(GREEN_COUNT_TYPE_SEGMENTATION-1) - j]]);
					}
				}
				else
				{
					for (int j = 0; j < GREEN_COUNT_TYPE_SEGMENTATION; ++j)
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
			}
	}
}

void Green::GPURender2(DWORD timeDelta,long nm, int lod)
{
	//если есть что к отрисовке
	if (RTCountDrawObj)
	{
		Green::DXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | RTCountDrawObj));

		Green::DXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		Green::DXDevice->SetStreamSource(1, TransVertBuf, 0, sizeof(DataVertex));

		Green::DXDevice->SetStreamSource(0, ArrModels[nm]->ArrLod[lod]->model.VertexBuffer, 0, sizeof(vertex_static));
		Green::DXDevice->SetIndices(ArrModels[nm]->ArrLod[lod]->model.IndexBuffer);
		Green::DXDevice->SetVertexDeclaration(VertexDeclarationGreen);


		Green::DXDevice->GetTransform(D3DTS_WORLD2, &mat);

		if (ArrModels[nm]->TypeGreen == GREEN_TYPE_TREE)
		{
			SGCore_ShaderBind(0, GData::IDShaderVSRenderGreenTree);
			SGCore_ShaderBind(1, GData::IDShaderPSRenderGreenTree);

			SGCore_ShaderSetVRF(0, GData::IDShaderVSRenderGreenTree, "WorldViewProjection", &mat);
		}
		else
		{
			SGCore_ShaderBind(0, GData::IDShaderVSRenderGreenGrass);
			SGCore_ShaderBind(1, GData::IDShaderPSRenderGreenTree);

			SGCore_ShaderSetVRF(0, GData::IDShaderVSRenderGreenGrass, "WorldViewProjection", &mat);
			SGCore_ShaderSetVRF(0, GData::IDShaderVSRenderGreenGrass, "DistBeginEndLessening", &float2_t(Green::BeginEndLessening, Green::DistLods.x));
		}

		jCountIndex = 0;
		for (DWORD i = 0; i < ArrModels[nm]->ArrLod[lod]->model.SubsetCount; i++)
		{


			Green::DXDevice->SetTexture(0, SGCore_GetTex(ArrModels[nm]->ArrLod[lod]->idstex[i]));
			Green::DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, ArrModels[nm]->ArrLod[lod]->model.VertexCount[i], jCountIndex, ArrModels[nm]->ArrLod[lod]->model.IndexCount[i] / 3);
			jCountIndex += ArrModels[nm]->ArrLod[lod]->model.IndexCount[i];
		}

		SGCore_ShaderUnBind();

		Green::DXDevice->SetStreamSourceFreq(0, 1);
		Green::DXDevice->SetStreamSourceFreq(1, 1);
	}
}

void Green::GPURender(DWORD timeDelta,long id_arr)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	for (int nm = 0; nm < ArrModels.size(); ++nm)
	{
		jarrsplits = ArrComFor[id_arr].arr[nm]->Arr;
		jcount = ArrComFor[id_arr].arr[nm]->CountCom;

		for (int lod = 0; lod < GREEN_COUNT_LOD; ++lod)
		{
			if (ArrModels[nm]->ArrLod[lod])
			{
				RTGPUArrVerteces = 0;
				TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
				RTCountDrawObj = 0;

				for (DWORD i = 0; i < jcount; i++)
				{
					if (jarrsplits[i]->CountAllGreen >= GREEN_MAX_ELEM_IN_DIP)
					{
						//error
					}
					if (RTCountDrawObj + jarrsplits[i]->CountAllGreen >= GREEN_MAX_ELEM_IN_DIP)
					{
						TransVertBuf->Unlock();
						GPURender2(timeDelta, nm, lod);
						TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
						RTCountDrawObj = 0;
					}

					if (
						//распределение по дистанции есесно и по лодам
						(
						(lod == 0 && jarrsplits[i]->DistForCamera <= Green::DistLods.x) ||
						(lod == 1 && jarrsplits[i]->DistForCamera <= Green::DistLods.y && jarrsplits[i]->DistForCamera > Green::DistLods.x) ||
						(lod == 2 && jarrsplits[i]->DistForCamera > Green::DistLods.y)
						)
						)
					{
						//если это не трава
						if (!(lod == 0 && ArrModels[nm]->TypeGreen == GREEN_TYPE_TREE))
						{
							memcpy(RTGPUArrVerteces + (RTCountDrawObj),
								jarrsplits[i]->Data,
								jarrsplits[i]->CountAllGreen * sizeof(DataVertex));

							RTCountDrawObj += jarrsplits[i]->CountAllGreen;
						}
						//иначе это трава, а ее по особенному рисуем
						else
						{
							if (Green::CurrentFreqGrass == 100)
							{
								memcpy(RTGPUArrVerteces + (RTCountDrawObj),
									jarrsplits[i]->Data,
									jarrsplits[i]->CountAllGreen * sizeof(DataVertex));

								RTCountDrawObj += jarrsplits[i]->CountAllGreen;
							}
							else
							{
								float percent = float(Green::CurrentFreqGrass) / 100.f;
								float newCount = ((float)jarrsplits[i]->CountAllGreen * percent);
								float step = float(jarrsplits[i]->CountAllGreen) / newCount;
								UINT lastCP = 0;
								for (float k = 0; k < jarrsplits[i]->CountAllGreen; k += step)
								{
									UINT curCP = (int)floor(k + 0.5);;
									if (curCP > lastCP)
									{
										memcpy(RTGPUArrVerteces + RTCountDrawObj, jarrsplits[i]->Data + curCP, sizeof(DataVertex));
										RTCountDrawObj += 1;
										lastCP = curCP;
									}
								}
							}
						}
					}
				}

				TransVertBuf->Unlock();

				GPURender2(timeDelta, nm, lod);
			}
		}
	}
}

void Green::Init(StaticGeom* geom, const char* name,
	const char* path_mask, 
	float count_max, 
	const char* path, const char* lod1, const char* lod2, 
	DWORD count_object_in_split)
{
	if (geom->GetCountModel() > 0 && def_str_validate(path))
	{
		Model* tmpnewmpdel = new Model();
		sprintf(tmpnewmpdel->Name, name);
		tmpnewmpdel->MaskName = path_mask;
		tmpnewmpdel->ArrLod[0] = new Lod();
		tmpnewmpdel->ArrLod[1] = 0;
		tmpnewmpdel->ArrLod[2] = 0;

		char tmppath[1024];
		sprintf(tmppath, "%s%s", Green::StdPath, path);

		SGCore_LoadStaticModel(tmppath, &tmpnewmpdel->ArrLod[0]->model);
		tmpnewmpdel->ArrLod[0]->path = path;
		char tmppathtex[1024];
		for (int i = 0; i < tmpnewmpdel->ArrLod[0]->model.SubsetCount; ++i)
		{
			sprintf(tmppathtex, "%s.dds", tmpnewmpdel->ArrLod[0]->model.ArrTextures[i]);
			tmpnewmpdel->ArrLod[0]->idstex[i] = SGCore_LoadTexAddName(tmppathtex);
		}

		if (def_str_validate(lod1))
		{
			if (stricmp(path, lod1) == 0)
				tmpnewmpdel->ArrLod[1] = tmpnewmpdel->ArrLod[0];
			else
			{
				tmpnewmpdel->ArrLod[1] = new Lod();
				sprintf(tmppath, "%s%s", Green::StdPath, lod1);
				tmpnewmpdel->ArrLod[1]->path = lod1;
				SGCore_LoadStaticModel(tmppath, &tmpnewmpdel->ArrLod[1]->model);

				for (int i = 0; i < tmpnewmpdel->ArrLod[1]->model.SubsetCount; ++i)
				{
					sprintf(tmppathtex, "%s.dds", tmpnewmpdel->ArrLod[1]->model.ArrTextures[i]);
					tmpnewmpdel->ArrLod[1]->idstex[i] = SGCore_LoadTexAddName(tmppathtex);
				}
			}
		}

		if (def_str_validate(lod2))
		{
			if (stricmp(path, lod2) == 0)
				tmpnewmpdel->ArrLod[2] = tmpnewmpdel->ArrLod[0];
			else if (stricmp(lod1, lod2) == 0)
				tmpnewmpdel->ArrLod[2] = tmpnewmpdel->ArrLod[1];
			else
			{
				tmpnewmpdel->ArrLod[2] = new Lod();
				sprintf(tmppath, "%s%s", Green::StdPath, lod2);
				tmpnewmpdel->ArrLod[2]->path = lod2;
				SGCore_LoadStaticModel(tmppath, &tmpnewmpdel->ArrLod[2]->model);

				for (int i = 0; i < tmpnewmpdel->ArrLod[2]->model.SubsetCount; ++i)
				{
					sprintf(tmppathtex, "%s.dds", tmpnewmpdel->ArrLod[1]->model.ArrTextures[i]);
					tmpnewmpdel->ArrLod[2]->idstex[i] = SGCore_LoadTexAddName(tmppathtex);
				}
			}
		}

		if (!lod1 && !lod2)
			tmpnewmpdel->TypeGreen = GREEN_TYPE_GRASS;
		else
			tmpnewmpdel->TypeGreen = GREEN_TYPE_TREE;

		float3 tmpmin, tmpmax;
		geom->GetMinMax(&tmpmin, &tmpmax);

		DWORD IDTexMask = SGCore_LoadTexAddName(path_mask);
		SGCore_LoadTexLoadTextures();

		float CountMaxInPixel = count_max;

		float WidthLand = tmpmax.x - tmpmin.x;
		float HeightLand = tmpmax.z - tmpmin.z;

		D3DSURFACE_DESC desc;
		SGCore_GetTex(IDTexMask)->GetLevelDesc(0, &desc);

		D3DLOCKED_RECT LockedRect;

		SGCore_GetTex(IDTexMask)->LockRect(0, &LockedRect, 0, 0);
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

						isintersect = geom->GetIntersectedRayY(&tmppos2);
						if (isintersect)
						{
							arrpos.push_back(tmppos2);

							++tmpnewmpdel->AllCountGreen;
						}
						else
						{
							++tmpUnAllCountGreen;
						}
					}
				}
			}
		}

		SGCore_GetTex(IDTexMask)->UnlockRect(0);

		tmpnewmpdel->AllTrans = new DataVertex[tmpnewmpdel->AllCountGreen];

		for (DWORD i = 0; i<tmpnewmpdel->AllCountGreen; i++)
		{
			tmpnewmpdel->AllTrans[i].Position = arrpos[i];
			tmpnewmpdel->AllTrans[i].TexCoord.x = 1.f + (float(rand() % 100) / 100.f);
			tmpnewmpdel->AllTrans[i].TexCoord.y = D3DXToRadian(float(rand() % 360));
			tmpnewmpdel->AllTrans[i].TexCoord.z = (float(rand() % 200) / 100.f) - 1.f;
		}
		arrpos.clear();


		PreSegmentation(tmpnewmpdel, count_object_in_split);
		//currSplitsIDs = 0;
		//currSplitsIDsRender = 0;
		SetSplitID(tmpnewmpdel->ArrSplits, &tmpnewmpdel->SplitsIDs, &tmpnewmpdel->SplitsIDsRender);
		//tmpnewmpdel->SplitsIDs = currSplitsIDs;
		//tmpnewmpdel->SplitsIDsRender = currSplitsIDsRender;

		ArrModels.push_back(tmpnewmpdel);
		AddModelInArrCom(ArrModels.size() - 1);
	}
	else
	{
		reportf(REPORT_MSG_LEVEL_WARRNING,"not found static geometry in level!!!");
	}
}

void Green::Save(const char* path)
{
	FILE* file = fopen(path, "wb");

	long countmodel = ArrModels.size();
	fwrite(&countmodel, sizeof(long), 1, file);

	for (long i = 0; i < ArrModels.size(); ++i)
	{
		fwrite(&ArrModels[i]->TypeGreen, sizeof(int), 1, file);

		long tmpstrlen = strlen(ArrModels[i]->Name);
		fwrite(&tmpstrlen, sizeof(long), 1, file);
		fwrite(ArrModels[i]->Name, sizeof(char), tmpstrlen, file);

		tmpstrlen = ArrModels[i]->MaskName.length();
		fwrite(&tmpstrlen, sizeof(long), 1, file);
		fwrite(ArrModels[i]->MaskName.c_str(), sizeof(char), tmpstrlen, file);

		if (ArrModels[i]->TypeGreen == GREEN_TYPE_GRASS)
		{
			tmpstrlen = strlen(ArrModels[i]->ArrLod[0]->path.c_str());
			fwrite(&tmpstrlen, sizeof(long), 1, file);
			fwrite(ArrModels[i]->ArrLod[0]->path.c_str(), sizeof(char), tmpstrlen, file);
		}
		else
		{
			for (int k = 0; k < GREEN_COUNT_LOD; ++k)
			{
				tmpstrlen = ArrModels[i]->ArrLod[k]->path.length();
				fwrite(&tmpstrlen, sizeof(long), 1, file);
				fwrite(ArrModels[i]->ArrLod[k]->path.c_str(), sizeof(char), tmpstrlen, file);
			}
		}
		
		fwrite(&ArrModels[i]->BBMin.x, sizeof(float), 1, file);
		fwrite(&ArrModels[i]->BBMin.y, sizeof(float), 1, file);
		fwrite(&ArrModels[i]->BBMin.z, sizeof(float), 1, file);

		fwrite(&ArrModels[i]->BBMax.x, sizeof(float), 1, file);
		fwrite(&ArrModels[i]->BBMax.y, sizeof(float), 1, file);
		fwrite(&ArrModels[i]->BBMax.z, sizeof(float), 1, file);

		fwrite(&ArrModels[i]->AllCountGreen, sizeof(DWORD), 1, file);
		fwrite(&ArrModels[i]->AllTrans, sizeof(DataVertex), ArrModels[i]->AllCountGreen, file);

		Array<Segment*> queue;
		long tmpcount = 0;
		queue.push_back(ArrModels[i]->ArrSplits);

		while (queue.size())
		{
			SaveSplit(queue[0], file, &queue);

			queue.erase(0);
			++tmpcount;
		}
	}

	fclose(file);
}

void Green::SaveSplit(Segment* Split, FILE* file, Array<Segment*> * queue)
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

	fwrite(&Split->CountAllGreen, sizeof(DWORD), 1, file);

	fwrite(&Split->BFNonEnd, sizeof(bool), 1, file);

	if (Split->BFNonEnd)
	{
		bool isexists = true;
		for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		{
			if (Split->Splits[i])
			{
				isexists = true;
				fwrite(&isexists, sizeof(bool), 1, file);
				queue->push_back(Split->Splits[i]);
			}
			else
			{
				isexists = false;
				fwrite(&isexists, sizeof(bool), 1, file);
			}

			isexists = true;
		}
	}
	else
	{
		fwrite(Split->Data, sizeof(DataVertex), Split->CountAllGreen, file);
	}
}

void Green::Load(const char* path)
{
	FILE* file = fopen(path, "rb");

	char tmpstr[3][1024];
	tmpstr[0][0] = 0;
	tmpstr[1][0] = 0;
	tmpstr[2][0] = 0;
	char tmppath[1024];
	char tmpNameMask[1024];

	long countmodel;
	fread(&countmodel, sizeof(long), 1, file);

	for (long i = 0; i < countmodel; ++i)
	{
		tmpstr[0][0] = tmpstr[1][0] = tmpstr[2][0] = 0;
		Model* tmpmodel = new Model();
		fread(&tmpmodel->TypeGreen, sizeof(int), 1, file);

		long tmpstrlen;// = strlen(ArrModels[i]->Name);
		fread(&tmpstrlen, sizeof(long), 1, file);
		fread(tmpNameMask, sizeof(char), tmpstrlen, file);
		tmpNameMask[tmpstrlen] = 0;
		sprintf(tmpmodel->Name,"%s",tmpNameMask);

		//tmpstrlen = ArrModels[i]->MaskName.length();
		fread(&tmpstrlen, sizeof(long), 1, file);
		fread(tmpNameMask, sizeof(char), tmpstrlen, file);
		tmpNameMask[tmpstrlen] = 0;
		tmpmodel->MaskName = tmpNameMask;

		if (tmpmodel->TypeGreen == GREEN_TYPE_GRASS)
		{
			//sprintf(tmpstr[0], "%s", Green::StdPath);
			//long tmpstrlen;
			fread(&tmpstrlen, sizeof(long), 1, file);
			fread(tmpstr[0], sizeof(char), tmpstrlen, file);
			tmpstr[0][tmpstrlen] = 0;
		}
		else
		{
			for (int k = 0; k < GREEN_COUNT_LOD; ++k)
			{
				//sprintf(tmpstr[k], "%s", Green::StdPath);
				//long tmpstrlen;
				fread(&tmpstrlen, sizeof(long), 1, file);
				fread(tmpstr[k], sizeof(char), tmpstrlen, file);
				tmpstr[k][tmpstrlen] = 0;
			}
		}

		
		tmpmodel->ArrLod[0] = new Lod();
		tmpmodel->ArrLod[1] = 0;
		tmpmodel->ArrLod[2] = 0;

		sprintf(tmppath, "%s%s", Green::StdPath, tmpstr[0]);
		SGCore_LoadStaticModel(tmppath, &tmpmodel->ArrLod[0]->model);
		tmpmodel->ArrLod[0]->path = tmpstr[0];
		char tmppathtex[1024];
		for (int k = 0; k < tmpmodel->ArrLod[0]->model.SubsetCount; ++k)
		{
			sprintf(tmppathtex, "%s.dds", tmpmodel->ArrLod[0]->model.ArrTextures[k]);
			tmpmodel->ArrLod[0]->idstex[k] = SGCore_LoadTexAddName(tmppathtex);
		}

		if (tmpstr[1][0])
		{
			if (stricmp(tmpstr[0], tmpstr[1]) == 0)
				tmpmodel->ArrLod[1] = tmpmodel->ArrLod[0];
			else
			{
				tmpmodel->ArrLod[1] = new Lod();
				tmpmodel->ArrLod[1]->path = tmpstr[1];
				sprintf(tmppath, "%s%s", Green::StdPath, tmpstr[1]);
				SGCore_LoadStaticModel(tmpstr[1], &tmpmodel->ArrLod[1]->model);

				for (int k = 0; k < tmpmodel->ArrLod[1]->model.SubsetCount; ++k)
				{
					sprintf(tmppathtex, "%s.dds", tmpmodel->ArrLod[1]->model.ArrTextures[k]);
					tmpmodel->ArrLod[1]->idstex[k] = SGCore_LoadTexAddName(tmppathtex);
				}
			}
		}

		if (tmpstr[2][0])
		{
			if (stricmp(tmpstr[0], tmpstr[2]) == 0)
				tmpmodel->ArrLod[2] = tmpmodel->ArrLod[0];
			else if (stricmp(tmpstr[1], tmpstr[2]) == 0)
				tmpmodel->ArrLod[2] = tmpmodel->ArrLod[1];
			else
			{
				tmpmodel->ArrLod[2] = new Lod();
				tmpmodel->ArrLod[2]->path = tmpstr[2];
				sprintf(tmppath, "%s%s", Green::StdPath, tmpstr[2]);
				SGCore_LoadStaticModel(tmppath, &tmpmodel->ArrLod[2]->model);

				for (int k = 0; k < tmpmodel->ArrLod[2]->model.SubsetCount; ++k)
				{
					sprintf(tmppathtex, "%s.dds", tmpmodel->ArrLod[1]->model.ArrTextures[k]);
					tmpmodel->ArrLod[2]->idstex[k] = SGCore_LoadTexAddName(tmppathtex);
				}
			}
		}

		fread(&tmpmodel->BBMin.x, sizeof(float), 1, file);
		fread(&tmpmodel->BBMin.y, sizeof(float), 1, file);
		fread(&tmpmodel->BBMin.z, sizeof(float), 1, file);

		fread(&tmpmodel->BBMax.x, sizeof(float), 1, file);
		fread(&tmpmodel->BBMax.y, sizeof(float), 1, file);
		fread(&tmpmodel->BBMax.z, sizeof(float), 1, file);

		fread(&tmpmodel->AllCountGreen, sizeof(DWORD), 1, file);
		tmpmodel->AllTrans = new DataVertex[tmpmodel->AllCountGreen];
		fread(tmpmodel->AllTrans, sizeof(DataVertex), tmpmodel->AllCountGreen, file);

		Array<Segment**> queue;
		long tmpcount = 0;
		queue.push_back(&(tmpmodel->ArrSplits));

		while (queue.size())
		{
			LoadSplit(queue[0], file, &queue);

			queue.erase(0);
			++tmpcount;
		}


		//currSplitsIDs = 0;
		//currSplitsIDsRender = 0;
		SetSplitID(tmpmodel->ArrSplits, &tmpmodel->SplitsIDs, &tmpmodel->SplitsIDsRender);
		//tmpmodel->SplitsIDs = currSplitsIDs;
		//tmpmodel->SplitsIDsRender = currSplitsIDsRender;

		ArrModels.push_back(tmpmodel);
		AddModelInArrCom(ArrModels.size() - 1);
		/*InfoRenderSegments* tmpirs = new InfoRenderSegments();
		tmpirs->Count = tmpmodel->SplitsIDsRender;
		tmpirs->Arr = new Segment*[tmpmodel->SplitsIDsRender];
		tmpirs->CountCom = 0;

		ArrComForCamera.push_back(tmpirs);*/
	}

	fclose(file);
}

void Green::LoadSplit(Segment** Split, FILE* file, Array<Segment**> * queue)
{
	(*Split) = new Segment();
	float3 jmin, jmax;
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

	fread(&(*Split)->CountAllGreen, sizeof(DWORD), 1, file);

	fread(&(*Split)->BFNonEnd, sizeof(bool), 1, file);

	if ((*Split)->BFNonEnd)
	{
		bool isexists = false;

		for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		{
			fread(&isexists, sizeof(bool), 1, file);
			if (isexists)
				queue->push_back(&((*Split)->Splits[i]));

			isexists = false;
		}
	}
	else
	{
		(*Split)->Data = new DataVertex[(*Split)->CountAllGreen];
		
		fread((*Split)->Data, sizeof(DataVertex)*(*Split)->CountAllGreen, 1, file);
	}
}

long Green::AddArrForCom()
{
	IRSData ttmpdata;
	for (long i = 0; i < ArrModels.size(); ++i)
	{
		InfoRenderSegments* tmpirs = new InfoRenderSegments();
		tmpirs->Count = ArrModels[i]->SplitsIDsRender;
		tmpirs->Arr = new Segment*[ArrModels[i]->SplitsIDsRender];
		tmpirs->CountCom = 0;
		ttmpdata.arr.push_back(tmpirs);
	}

	ArrComFor.push_back(ttmpdata);

	return ArrComFor.size() - 1;
}

void Green::DelArrForCom(long id_arr)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	for (long i = 0; i < ArrModels.size(); ++i)
	{
		mem_delete_a(ArrComFor[id_arr].arr[i]->Arr);
	}

	ArrComFor.erase(id_arr);
}

void Green::AddModelInArrCom(long id_model)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model);
	
	for (long i = 0; i < ArrComFor.size(); ++i)
	{
		InfoRenderSegments* tmpirs = new InfoRenderSegments();
		tmpirs->Count = ArrModels[id_model]->SplitsIDsRender;
		tmpirs->Arr = new Segment*[ArrModels[id_model]->SplitsIDsRender];
		tmpirs->CountCom = 0;
		ArrComFor[i].arr.push_back(tmpirs);
	}
}

void Green::DelModelInArrCom(long id_model)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model);

	for (long i = 0; i < ArrComFor.size(); ++i)
	{
		mem_delete_a(ArrComFor[i].arr[id_model]->Arr);
		ArrComFor[i].arr.erase(id_model);
	}
}

long Green::GetCountGreen()
{
	return ArrModels.size();
}

char* Green::GetGreenName(long id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->Name;

	return 0;
}

inline long Green::GetGreenCountGen(long id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->AllCountGreen;

	return -1;
}

inline int Green::GetGreenTypeCountGen(long id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->TypeGreen;

	return -1;
}

inline const char* Green::GetGreenModel(long id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->ArrLod[0]->path.c_str();

	return 0;
}

inline const char* Green::GetGreenLod1(long id)
{
	if (id < ArrModels.size() && ArrModels[id]->ArrLod[1])
		return ArrModels[id]->ArrLod[1]->path.c_str();

	return 0;
}

inline const char* Green::GetGreenLod2(long id)
{
	if (id < ArrModels.size() && ArrModels[id]->ArrLod[2])
		return ArrModels[id]->ArrLod[2]->path.c_str();

	return 0;
}

inline const char* Green::GetGreenMask(long id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->MaskName.c_str();

	return 0;
}