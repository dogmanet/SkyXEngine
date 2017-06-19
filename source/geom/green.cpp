
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
		{ 1, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		D3DDECL_END()
	};

	Green::DXDevice->CreateVertexDeclaration(InstanceGreen, &VertexDeclarationGreen);

	Green::DXDevice->CreateVertexBuffer(
		GREEN_MAX_ELEM_IN_DIP * sizeof(GreenDataVertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
		&TransVertBuf,
		0);

	IRSData* tmparr = new IRSData();
	ArrComFor.push_back(tmparr);
	IRSData* tmparr2 = new IRSData();
	ArrComFor.push_back(tmparr2);
}

Green::~Green()
{
	for (long i = 0; i < ArrModels.size(); ++i)
	{
		mem_delete(ArrModels[i]);
	}

	ArrModels.clear();

	mem_release(TransVertBuf);
	mem_release(VertexDeclarationGreen);

	while (ArrComFor.size() > 0)
	{
		mem_delete(ArrComFor[0]);
		ArrComFor.erase(0);
	}
}

Green::Model::Model()
{
	ArrLod[0] = ArrLod[1] = ArrLod[2] = 0;
	AllCountGreen = 0;
	SplitsIDs = 0;
	SplitsTree = 0;
	AllTrans = 0;
	ArrLod[0] = ArrLod[1] = ArrLod[2] = 0;
	NavigateMesh = 0;
}

Green::Model::NavMesh::NavMesh()
{
	arr_vertex = 0;
	arr_index = 0;
	count_vertex = 0;
	count_index = 0;
	arr_mtl = 0;
	pathname = "";
}

Green::Model::NavMesh::~NavMesh()
{
	mem_delete_a(arr_vertex);
	count_vertex = 0;
	mem_delete_a(arr_index);
	mem_delete_a(arr_mtl);
	count_index = 0;
}

Green::Model::~Model()
{
	mem_delete(SplitsTree);
	mem_delete(AllTrans);
	mem_delete(NavigateMesh);

	mem_del(ArrLod[0]);
	if (ArrLod[0] != ArrLod[1])
		mem_del(ArrLod[1]);

	if (ArrLod[0] != ArrLod[2] && ArrLod[1] != ArrLod[2])
		mem_del(ArrLod[2]);
}

Green::Segment::Segment()
{
	for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; ++i)
		Splits[i] = 0;
	
	Data = 0;
	CountAllGreen = 0;
	BoundVolumeSys = 0;
	BoundVolumeP = 0;
	Id = -1;
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
	model = 0;
}

Green::Lod::~Lod()
{
	idstex.clear();
	mem_release_del(model);
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
	
}

Green::IRSData::~IRSData()
{
	queue.clear();

	for (int i = 0; i < arr.size(); ++i)
	{
		mem_delete(arr[i]);
	}
	arr.clear();
}


void Green::OnLostDevice()
{
	mem_release_del(TransVertBuf);
}

void Green::OnResetDevice()
{
	Green::DXDevice->CreateVertexBuffer(
		GREEN_MAX_ELEM_IN_DIP * sizeof(GreenDataVertex),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
		&TransVertBuf,
		0);
}

void Green::PreSegmentation(Model* model, float3* min_level, float3* max_level)
{
	model->SplitsTree = new Segment();

	float3 tmpMin, tmpMax;
	float3 tmpMin2, tmpMax2;
	model->SplitsTree->BoundVolumeSys = SGCore_CrBound();
	SGCore_FCompBoundBox(model->ArrLod[0]->model->VertexBuffer, &(model->SplitsTree->BoundVolumeSys), model->ArrLod[0]->model->AllVertexCount, sizeof(vertex_static));

	model->SplitsTree->BoundVolumeSys->GetMinMax(&tmpMin2, &tmpMax2);
	model->BBMax = tmpMax2 * (1.f + GREEN_GEN_RAND_SCALE);
	model->BBMin = tmpMin2 * (1.f + GREEN_GEN_RAND_SCALE);
	tmpMin = *min_level;
	tmpMax = *max_level;

	tmpMax.x += model->BBMax.x;
	tmpMax.y += model->BBMax.y;
	tmpMax.z += model->BBMax.z;

	tmpMin.x += model->BBMin.x;
	tmpMin.y += model->BBMin.y;
	tmpMin.z += model->BBMin.z;

	float tmpX = tmpMax.x - tmpMin.x;
	float tmpY = tmpMax.y - tmpMin.y;
	float tmpZ = tmpMax.z - tmpMin.z;

	model->SplitsTree->BoundVolumeP = SGCore_CrBound();
	model->SplitsTree->BoundVolumeP->SetMinMax(&tmpMin, &tmpMax);

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

	model->SplitsTree->BoundVolumeSys->SetMinMax(&tmpMin, &tmpMax);

	model->SplitsTree->CountAllGreen = model->AllCountGreen;
	if (model->AllCountGreen > 0)
	{
		model->SplitsTree->Data = new GreenDataVertex[model->AllCountGreen];
		memcpy(model->SplitsTree->Data, model->AllTrans, sizeof(GreenDataVertex)* model->AllCountGreen);
	}

	if (model->SplitsTree->CountAllGreen > 0)
		model->SplitsTree->BFNonEnd = true;
	else
		model->SplitsTree->BFNonEnd = false;

	CycleSegmentation(model->SplitsTree, model);
}

void Green::CycleSegmentation(Segment* Split, Model* mesh)
{
	Array<Segment*> queue;
	long tmpcount = 0;
	queue.push_back(Split);
	float width = 0;
	float depth = 0;
	float3 min, max;

	while (queue.size())
	{
		queue[0]->BoundVolumeSys->GetMinMax(&min, &max);
		if ((max.x - min.x)*0.5f > GREEN_BB_MIN_X && (max.z - min.z)*0.5f > GREEN_BB_MIN_Z)
		{
			Segmentation(queue[0], mesh);
			for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; i++)
			{
				if (queue[0]->Splits[i])
					queue.push_back(queue[0]->Splits[i]);
			}
		}
		else
			queue[0]->BFNonEnd = false;

		queue.erase(0);
		++tmpcount;
	}
}

void Green::Segmentation(Segment* Split, Model* mesh)
{
	Array<DWORD> ArrPoly[4];

	for (int i = 0; i<4; i++)
		Split->Splits[i] = new Segment();

	ISXBound* ArrBound[4];
	for (int i = 0; i < 4; ++i)
		ArrBound[i] = SGCore_CrBound();

	SGCore_0ComBoundBoxArr4(Split->BoundVolumeSys, (ArrBound));

	float3 tmpmin, tmpmax;
	for (int i = 0; i<4; ++i)
	{
		Split->Splits[i]->BoundVolumeSys = ArrBound[i];
		Split->Splits[i]->BoundVolumeSys->GetMinMax(&tmpmin, &tmpmax);
		Split->Splits[i]->BoundVolumeP = SGCore_CrBound();
		Split->Splits[i]->BoundVolumeP->SetMinMax(&tmpmin, &tmpmax);
	}

	bool *tmp_arr_mesh_poly = 0;
	if (Split->CountAllGreen)
	{
		tmp_arr_mesh_poly = new bool[Split->CountAllGreen];
		for (int i = 0; i < Split->CountAllGreen; i++)
			tmp_arr_mesh_poly[i] = true;
	}

	DWORD tmpCountNonIn = 0;
	float3 tmpMin, tmpMax;
	for (WORD i = 0; i<4; ++i)
	{
		Split->Splits[i]->BoundVolumeSys->GetMinMax(&tmpMin, &tmpMax);
		//SGCore_FCreateBoundingBoxMesh(&tmpMin, &tmpMax, &(Split->Splits[i]->BoundBox));

		for (DWORD j = 0; j<Split->CountAllGreen; ++j)
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
			Split->Splits[i]->Data = new GreenDataVertex[Split->Splits[i]->CountAllGreen];
			for (DWORD k = 0; k < ArrPoly[i].size(); k++)
			{
				Split->Splits[i]->Data[k] = Split->Data[ArrPoly[i][k]];
			}

			AlignBound(mesh, Split->Splits[i]);

			ArrPoly[i].clear();
		}

		if (Split->Splits[i]->CountAllGreen > 0)
			Split->Splits[i]->BFNonEnd = true;
		else
		{
			Split->Splits[i]->BFNonEnd = false;
		}
	}

	mem_delete_a(Split->Data);
	mem_delete_a(tmp_arr_mesh_poly);
}

void Green::AlignBound(Model* model, Segment* split)
{
	if (split->CountAllGreen > 0)
	{
		float3 comMax = split->Data[0].Position;
		float3 comMin = split->Data[0].Position;

		for (int k = 0; k<split->CountAllGreen; ++k)
		{
			if (split->Data[k].Position.y > comMax.y)
				comMax.y = split->Data[k].Position.y;

			if (split->Data[k].Position.y < comMin.y)
				comMin.y = split->Data[k].Position.y;


			if (split->Data[k].Position.x > comMax.x)
				comMax.x = split->Data[k].Position.x;

			if (split->Data[k].Position.x < comMin.x)
				comMin.x = split->Data[k].Position.x;


			if (split->Data[k].Position.z > comMax.z)
				comMax.z = split->Data[k].Position.z;

			if (split->Data[k].Position.z < comMin.z)
				comMin.z = split->Data[k].Position.z;
		}

		float3 tmpMin, tmpMax;
		float scalecoef = 1.f + GREEN_GEN_RAND_SCALE;

		split->BoundVolumeSys->GetMinMax(&tmpMin, &tmpMax);
		tmpMax.y = comMax.y + model->BBMax.y * scalecoef;
		tmpMin.y = comMin.y + model->BBMin.y * scalecoef;

		split->BoundVolumeSys->SetMinMax(&tmpMin, &tmpMax);

		tmpMax.x = comMax.x + model->BBMax.x * scalecoef;
		tmpMax.y = comMax.y + model->BBMax.y * scalecoef;
		tmpMax.z = comMax.z + model->BBMax.z * scalecoef;

		tmpMin.x = comMin.x + model->BBMin.x * scalecoef;
		tmpMin.y = comMin.y + model->BBMin.y * scalecoef;
		tmpMin.z = comMin.z + model->BBMin.z * scalecoef;

		split->BoundVolumeP->SetMinMax(&tmpMin, &tmpMax);
	}
}

void Green::SetSplitID(Model* model)
{
	Array<Segment*, GREEN_DEFAULT_RESERVE_COM> queue;
	long tmpcount = 0;
	queue.push_back(model->SplitsTree);

	while (queue.size())
	{
		SetSplitID2(model, queue[0], &queue);

		queue.erase(0);
		++tmpcount;
	}
}

void Green::SetSplitID2(Model* model, Segment* Split, Array<Segment*, GREEN_DEFAULT_RESERVE_COM>* queue)
{
	if (Split)
	{
		Split->Id = model->SplitsIDs;
		model->SplitsArr[Split->Id] = Split;
		++(model->SplitsIDs);
		for (int i = 0; i < 4; i++)
		{
			if (Split->Splits[i])
				queue->push_back(Split->Splits[i]);
		}
	}
}

void Green::CPUFillingArrIndeces(ISXFrustum* frustum, float3* viewpos, ID id_arr)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr);
	
	DWORD tmpcount = 0;
	DWORD* tmpcountcom = 0;
	Segment** tmpsegments = 0;
	for (int i = 0; i < ArrModels.size(); ++i)
	{
		tmpcount = ArrComFor[id_arr]->arr[i]->Count;
		ArrComFor[id_arr]->arr[i]->CountCom = 0;
		tmpcountcom = &(ArrComFor[id_arr]->arr[i]->CountCom);
		tmpsegments = ArrComFor[id_arr]->arr[i]->Arr;
		
		ArrComFor[id_arr]->queue.clear();
		//long tmpcount = 0;
		ArrComFor[id_arr]->queue.push_back(ArrModels[i]->SplitsTree);

		while (ArrComFor[id_arr]->queue.size())
		{
			ComRecArrIndeces(frustum, tmpsegments, tmpcountcom, ArrComFor[id_arr]->queue[0], viewpos, &ArrComFor[id_arr]->queue, tmpcount);

			ArrComFor[id_arr]->queue.erase(0);
			//++tmpcount;
		}
	}

	int qwert = 0;
}

void Green::ComRecArrIndeces(ISXFrustum* frustum, Segment** arrsplits, DWORD *count, Segment* comsegment, float3* viewpos, Array<Segment*, GREEN_DEFAULT_RESERVE_COM>* queue, ID curr_splits_ids_render)
{
	float3 jcenter;
	float jradius;
	ID SortId[GREEN_COUNT_TYPE_SEGMENTATION];
	comsegment->BoundVolumeP->GetSphere(&jcenter, &jradius);

	if (comsegment->CountAllGreen > 0 && frustum->SphereInFrustum(&jcenter, jradius))
	{
			if (comsegment->BFNonEnd)
			{
				if (Green::UseSortFrontToBackSplits)
				{
					for (int q = 0; q<GREEN_COUNT_TYPE_SEGMENTATION; ++q)
					{
						SortId[q] = -1;
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
						if (SortId[(GREEN_COUNT_TYPE_SEGMENTATION - 1) - j] != -1 && comsegment->Splits[SortId[(GREEN_COUNT_TYPE_SEGMENTATION-1) - j]])
							queue->push_back(comsegment->Splits[SortId[(GREEN_COUNT_TYPE_SEGMENTATION-1) - j]]);
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
				if ((*count) < curr_splits_ids_render && comsegment->Data)
				{
					arrsplits[(*count)] = comsegment;
					comsegment->DistForCamera = 0;// SMVector3Length((jcenter - (*viewpos))) - jradius;
					(*count)++;
				}
			}
	}
}

void Green::GPURender2(DWORD timeDelta, float3* viewpos, ID nm, int lod, ID id_tex)
{
	//если есть что к отрисовке
	if (RTCountDrawObj)
	{
		Green::DXDevice->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | RTCountDrawObj));

		Green::DXDevice->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1));
		Green::DXDevice->SetStreamSource(1, TransVertBuf, 0, sizeof(GreenDataVertex));

		Green::DXDevice->SetStreamSource(0, ArrModels[nm]->ArrLod[lod]->model->VertexBuffer, 0, sizeof(vertex_static));
		Green::DXDevice->SetIndices(ArrModels[nm]->ArrLod[lod]->model->IndexBuffer);
		Green::DXDevice->SetVertexDeclaration(VertexDeclarationGreen);

		jCountIndex = 0;
		for (DWORD i = 0; i < ArrModels[nm]->ArrLod[lod]->model->SubsetCount; i++)
		{
			SGCore_MtlSet((id_tex > 0 ? id_tex : ArrModels[nm]->ArrLod[lod]->idstex[i]), 0);

			if (ArrModels[nm]->TypeGreen == GeomGreenType::ggt_grass)
					Green::DXDevice->SetVertexShaderConstantF(59, (float*)&float2_t(Green::BeginEndLessening, Green::DistLods.x), 1);
				else
					Green::DXDevice->SetVertexShaderConstantF(59, (float*)&float2_t(0,0), 1);

			Green::DXDevice->SetVertexShaderConstantF(60, (float*)viewpos, 1);
			Green::DXDevice->SetVertexShaderConstantF(61, (float*)&(ArrModels[nm]->ArrLod[lod]->model->BSphere), 1);
			Green::DXDevice->SetVertexShaderConstantF(62, (float*)&(ArrModels[nm]->ArrLod[lod]->model->BBMax), 1);
			Green::DXDevice->SetVertexShaderConstantF(63, (float*)&(ArrModels[nm]->ArrLod[lod]->model->BBMin), 1);

			SGCore_DIP(D3DPT_TRIANGLELIST, 0, 0, ArrModels[nm]->ArrLod[lod]->model->VertexCount[i], jCountIndex, ArrModels[nm]->ArrLod[lod]->model->IndexCount[i] / 3);
			Core_RIntSet(G_RI_INT_COUNT_POLY, Core_RIntGet(G_RI_INT_COUNT_POLY) + ((ArrModels[nm]->ArrLod[lod]->model->IndexCount[i] / 3) * RTCountDrawObj));
			jCountIndex += ArrModels[nm]->ArrLod[lod]->model->IndexCount[i];
		}

		Green::DXDevice->SetStreamSourceFreq(0, 1);
		Green::DXDevice->SetStreamSourceFreq(1, 1);
	}
}

void Green::GPURender(DWORD timeDelta, float3* viewpos, GeomGreenType type, ID id_arr)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	float3 jcenter;
	float jradius;

	for (int nm = 0; nm < ArrModels.size(); ++nm)
	{
		//если тип не указан
		if (type != GeomGreenType::ggtr_all && type != ArrModels[nm]->TypeGreen)
			continue;

		jarrsplits = ArrComFor[id_arr]->arr[nm]->Arr;
		jcount = ArrComFor[id_arr]->arr[nm]->CountCom;

		for (int lod = 0; lod < GREEN_COUNT_LOD; ++lod)
		{
			if (ArrModels[nm]->ArrLod[lod])
			{
				RTGPUArrVerteces = 0;
				TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
				RTCountDrawObj = 0;

				for (DWORD i = 0; i < jcount; i++)
				{
					if (RTCountDrawObj + jarrsplits[i]->CountAllGreen >= GREEN_MAX_ELEM_IN_DIP)
					{
						TransVertBuf->Unlock();
						GPURender2(timeDelta, viewpos, nm, lod, -1);
						TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
						RTCountDrawObj = 0;
					}

					jarrsplits[i]->BoundVolumeP->GetSphere(&jcenter, &jradius);
					jarrsplits[i]->DistForCamera = SMVector3Length((jcenter - (*viewpos))) - jradius;

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
						if (!(lod == 0 && ArrModels[nm]->TypeGreen == GeomGreenType::ggt_grass))
						{
							memcpy(RTGPUArrVerteces + (RTCountDrawObj),
								jarrsplits[i]->Data,
								jarrsplits[i]->CountAllGreen * sizeof(GreenDataVertex));

							RTCountDrawObj += jarrsplits[i]->CountAllGreen;
						}
						//иначе это трава, а ее по особенному рисуем
						else if (lod == 0 && ArrModels[nm]->TypeGreen == GeomGreenType::ggt_grass)
						{
							if (Green::CurrentFreqGrass >= 100)
							{
								memcpy(RTGPUArrVerteces + (RTCountDrawObj),
									jarrsplits[i]->Data,
									jarrsplits[i]->CountAllGreen * sizeof(GreenDataVertex));

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
										memcpy(RTGPUArrVerteces + RTCountDrawObj, jarrsplits[i]->Data + curCP, sizeof(GreenDataVertex));
										RTCountDrawObj += 1;
										lastCP = curCP;
									}
								}
							}
						}
					}
				}

				TransVertBuf->Unlock();

				GPURender2(timeDelta, viewpos, nm, lod, -1);
			}
		}
	}
}

void Green::GPURenderSingly(DWORD timeDelta, float3* viewpos, ID id, ID id_tex)
{
	if (ArrModels.size() <= id)
		return;

	float3 jcenter;
	float jradius;
	ID id_arr = 0;

	jarrsplits = ArrComFor[id_arr]->arr[id]->Arr;
	jcount = ArrComFor[id_arr]->arr[id]->CountCom;

	for (int lod = 0; lod < GREEN_COUNT_LOD; ++lod)
	{
		if (ArrModels[id]->ArrLod[lod])
		{
			RTGPUArrVerteces = 0;
			TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
			RTCountDrawObj = 0;

			for (DWORD i = 0; i < jcount; i++)
			{
				if (RTCountDrawObj + jarrsplits[i]->CountAllGreen >= GREEN_MAX_ELEM_IN_DIP)
				{
					TransVertBuf->Unlock();
					GPURender2(timeDelta, viewpos, id, lod, id_tex);
					TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
					RTCountDrawObj = 0;
				}

				jarrsplits[i]->BoundVolumeP->GetSphere(&jcenter, &jradius);
				jarrsplits[i]->DistForCamera = SMVector3Length((jcenter - (*viewpos))) - jradius;

				if (
					//распределение по дистанции есесно и по лодам
					(
					(lod == 0 && jarrsplits[i]->DistForCamera <= Green::DistLods.x) ||
					(lod == 1 && jarrsplits[i]->DistForCamera <= Green::DistLods.y && jarrsplits[i]->DistForCamera > Green::DistLods.x) ||
					(lod == 2 && jarrsplits[i]->DistForCamera > Green::DistLods.y) || !(ArrModels[id]->ArrLod[1])
					)
					)
				{
					if (!(lod == 0 && ArrModels[id]->TypeGreen == GeomGreenType::ggt_grass))
					{
						memcpy(RTGPUArrVerteces + (RTCountDrawObj),
							jarrsplits[i]->Data,
							jarrsplits[i]->CountAllGreen * sizeof(GreenDataVertex));

						RTCountDrawObj += jarrsplits[i]->CountAllGreen;
					}
					else
					{
						float percent = 0.1f;
						float newCount = ((float)jarrsplits[i]->CountAllGreen * percent);
						float step = float(jarrsplits[i]->CountAllGreen) / newCount;
						UINT lastCP = 0;
						for (float k = 0; k < jarrsplits[i]->CountAllGreen; k += step)
						{
							UINT curCP = (int)floor(k + 0.5);;
							if (curCP > lastCP)
							{
								memcpy(RTGPUArrVerteces + RTCountDrawObj, jarrsplits[i]->Data + curCP, sizeof(GreenDataVertex));
								RTCountDrawObj += 1;
								lastCP = curCP;
							}
						}
					}
				}
			}

			TransVertBuf->Unlock();

			GPURender2(timeDelta, viewpos, id, lod, id_tex);
		}
	}
}

void Green::GPURenderObject(DWORD timeDelta, float3* viewpos, ID id, ID split, ID idobj, ID id_tex)
{
	if (id < 0 || ArrModels.size() <= id || split < 0 || ArrModels[id]->SplitsArr.size() <= split || idobj < 0 || ArrModels[id]->SplitsArr[split]->CountAllGreen <= idobj)
		return;

	RTGPUArrVerteces = 0;
	TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
	memcpy(RTGPUArrVerteces, &(ArrModels[id]->SplitsArr[split]->Data[idobj]), sizeof(GreenDataVertex));
	TransVertBuf->Unlock();
	RTCountDrawObj = 1;

	GPURender2(timeDelta, viewpos, id, 0, id_tex);
}

ID Green::Init(StaticGeom* geom, const char* name,
	const char* path_mask, 
	float count_max, 
	const char* path, const char* lod1, const char* lod2, 
	const char* navmesh)
{
	if (geom->GetCountModel() > 0 && def_str_validate(path))
	{
		Model* tmpnewmpdel = new Model();
		sprintf(tmpnewmpdel->Name, name);
		tmpnewmpdel->MaskName = path_mask;
		tmpnewmpdel->ArrLod[0] = new Lod();
		tmpnewmpdel->ArrLod[1] = 0;
		tmpnewmpdel->ArrLod[2] = 0;

		if (!lod1 && !lod2)
			tmpnewmpdel->TypeGreen = GeomGreenType::ggt_grass;
		else
			tmpnewmpdel->TypeGreen = GeomGreenType::ggt_tree;

		char tmppath[1024];
		sprintf(tmppath, "%s%s", Green::StdPath, path);

		SGCore_StaticModelLoad(tmppath, &tmpnewmpdel->ArrLod[0]->model);
		tmpnewmpdel->ArrLod[0]->path = path;
		char tmppathtex[1024];
		for (int i = 0; i < tmpnewmpdel->ArrLod[0]->model->SubsetCount; ++i)
		{
			sprintf(tmppathtex, "%s.dds", tmpnewmpdel->ArrLod[0]->model->ArrTextures[i]);
			tmpnewmpdel->ArrLod[0]->idstex[i] = SGCore_MtlLoad(tmppathtex, (tmpnewmpdel->TypeGreen == GeomGreenType::ggt_tree ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
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
				SGCore_StaticModelLoad(tmppath, &tmpnewmpdel->ArrLod[1]->model);

				for (int i = 0; i < tmpnewmpdel->ArrLod[1]->model->SubsetCount; ++i)
				{
					sprintf(tmppathtex, "%s.dds", tmpnewmpdel->ArrLod[1]->model->ArrTextures[i]);
					tmpnewmpdel->ArrLod[1]->idstex[i] = SGCore_MtlLoad(tmppathtex, (tmpnewmpdel->TypeGreen == GeomGreenType::ggt_tree ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
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
				SGCore_StaticModelLoad(tmppath, &tmpnewmpdel->ArrLod[2]->model);

				for (int i = 0; i < tmpnewmpdel->ArrLod[2]->model->SubsetCount; ++i)
				{
					sprintf(tmppathtex, "%s.dds", tmpnewmpdel->ArrLod[1]->model->ArrTextures[i]);
					tmpnewmpdel->ArrLod[2]->idstex[i] = SGCore_MtlLoad(tmppathtex, (tmpnewmpdel->TypeGreen == GeomGreenType::ggt_tree ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
				}
			}
		}

		ISXBound* tmpbb = SGCore_CrBound();
		SGCore_FCompBoundBox(tmpnewmpdel->ArrLod[0]->model->VertexBuffer, &tmpbb, tmpnewmpdel->ArrLod[0]->model->AllVertexCount, sizeof(vertex_static));

		float3 mmax, mmin;
		tmpbb->GetMinMax(&mmin, &mmax);
		mem_release(tmpbb);

		float3 tmpmin, tmpmax;
		geom->GetMinMax(&tmpmin, &tmpmax);

		if (def_str_validate(path_mask))
		{
			ID IDTexMask = SGCore_LoadTexAddName(path_mask, LoadTexType::ltt_load);
			SGCore_LoadTexLoadTextures();

			GenByTex(geom, tmpnewmpdel, IDTexMask, &mmin, &mmax, count_max);
		}

		PreSegmentation(tmpnewmpdel, &tmpmin, &tmpmax);
		mem_delete_a(tmpnewmpdel->AllTrans);

		SetSplitID(tmpnewmpdel);
		
		ArrModels.push_back(tmpnewmpdel);
		if (def_str_validate(navmesh))
		{
			SetGreenNav(ArrModels.size() - 1, navmesh);
		}
		AddModelInArrCom(ArrModels.size() - 1);
		return ArrModels.size() - 1;
	}
	else
	{
		reportf(REPORT_MSG_LEVEL_WARRNING,"not found static geometry in level!!!");
	}

	return -1;
}

void Green::GenByTex(StaticGeom* geom, Model* model, ID idmask, float3* min, float3* max, float count_max)
{
	float CountMaxInPixel = count_max;

	float3 tmpmin, tmpmax;
	geom->GetMinMax(&tmpmin, &tmpmax);

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

					if (model->TypeGreen == GeomGreenType::ggt_tree)
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
						isintersect = geom->GetIntersectedRayY(&tmppos2);

					if (isintersect)
					{
						arrpos.push_back(tmppos2);

						++model->AllCountGreen;

						//если тип дерево, то на пиксель генерируем только одно дерево
						if (model->TypeGreen == GeomGreenType::ggt_tree)
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

	if (model->AllCountGreen <= 0)
		return;

	model->AllTrans = new GreenDataVertex[model->AllCountGreen];

	for (DWORD i = 0; i<model->AllCountGreen; i++)
	{
		model->AllTrans[i].Position = arrpos[i];
		model->AllTrans[i].TexCoord.x = 1.f + randf(0.f, GREEN_GEN_RAND_SCALE);
		model->AllTrans[i].TexCoord.y = randf(0.f, GREEN_GEN_RAND_ROTATE_Y);
		model->AllTrans[i].TexCoord.z = 0;// (float(rand() % 200) / 100.f) - 1.f;
		model->AllTrans[i].SinCosRot.x = sinf(model->AllTrans[i].TexCoord.y);
		model->AllTrans[i].SinCosRot.y = cosf(model->AllTrans[i].TexCoord.y);
	}
	arrpos.clear();
}

ID Green::GetIDSplit(ID id, float3* pos)
{
	if (id < 0 || ArrModels.size() <= id)
		return -1;

	Array<Segment*> queue;
	long tmpcount = 0;
	queue.push_back(ArrModels[id]->SplitsTree);
	float width = 0;
	float depth = 0;
	float3 min, max;
	ID tmpidsplit = -1;

	while (queue.size())
	{
		queue[0]->BoundVolumeSys->GetMinMax(&min, &max);
		if (max.x >= pos->x && max.z >= pos->z && min.x <= pos->x && min.z <= pos->z)
		{
			if (!(queue[0]->Splits[0]))
				tmpidsplit = queue[0]->Id;
			else
			{
				queue[0]->BFNonEnd = true;
				++(queue[0]->CountAllGreen);

				float3 tmpMin, tmpMax;
				float scalecoef = 1.f + GREEN_GEN_RAND_SCALE;

				queue[0]->BoundVolumeSys->GetMinMax(&tmpMin, &tmpMax);
				if (tmpMax.y < (pos->y + ArrModels[id]->BBMax.y * scalecoef))
					tmpMax.y = tmpMax.y + ArrModels[id]->BBMax.y * scalecoef;

				if (tmpMin.y >(pos->y + ArrModels[id]->BBMin.y * scalecoef))
					tmpMin.y = tmpMin.y + ArrModels[id]->BBMin.y * scalecoef;

				queue[0]->BoundVolumeSys->SetMinMax(&tmpMin, &tmpMax);

				if (tmpMax.x < (pos->x + ArrModels[id]->BBMax.x * scalecoef))
					tmpMax.x = tmpMax.x + ArrModels[id]->BBMax.x * scalecoef;

				if (tmpMax.z < (pos->z + ArrModels[id]->BBMax.z * scalecoef))
					tmpMax.z = tmpMax.z + ArrModels[id]->BBMax.z * scalecoef;

				if (tmpMin.x >(pos->x + ArrModels[id]->BBMin.x * scalecoef))
					tmpMin.x = tmpMin.x + ArrModels[id]->BBMin.x * scalecoef;

				if (tmpMin.z >(pos->z + ArrModels[id]->BBMin.z * scalecoef))
					tmpMin.z = tmpMin.z + ArrModels[id]->BBMin.z * scalecoef;

				queue[0]->BoundVolumeP->SetMinMax(&tmpMin, &tmpMax);
			}

			for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; i++)
			{
				if (queue[0]->Splits[i])
					queue.push_back(queue[0]->Splits[i]);
			}
		}

		queue.erase(0);
		++tmpcount;
	}

	return tmpidsplit;
}

ID Green::AddObject(ID id, float3* pos, GreenDataVertex* data, ID* idsplit)
{
	if (id < 0 || ArrModels.size() <= id)
		return -1;

	ID tmpidsplit = GetIDSplit(id, pos);

	if (tmpidsplit < 0)
		return -1;

	int oldlen = ArrModels[id]->SplitsArr[tmpidsplit]->CountAllGreen;
	GreenDataVertex* tmpdv = new GreenDataVertex[oldlen + 1];
	if (oldlen > 0)
		memcpy(tmpdv, ArrModels[id]->SplitsArr[tmpidsplit]->Data, oldlen * sizeof(GreenDataVertex));

	GreenDataVertex tmpdvobj;
	if (!data)
	{
		tmpdvobj.Position = *pos;
		tmpdvobj.TexCoord.x = 1.f + randf(0.f, GREEN_GEN_RAND_SCALE);
		tmpdvobj.TexCoord.y = randf(0.f, GREEN_GEN_RAND_ROTATE_Y);
		tmpdvobj.TexCoord.z = 0;// (float(rand() % 200) / 100.f) - 1.f;
		tmpdvobj.SinCosRot.x = sinf(tmpdvobj.TexCoord.y);
		tmpdvobj.SinCosRot.y = cosf(tmpdvobj.TexCoord.y);
	}
	else
	{
		tmpdvobj = *data;
		tmpdvobj.Position = *pos;
	}

	memcpy(tmpdv + oldlen, &tmpdvobj, sizeof(GreenDataVertex));
	mem_delete_a(ArrModels[id]->SplitsArr[tmpidsplit]->Data);
	ArrModels[id]->SplitsArr[tmpidsplit]->Data = tmpdv;
	++(ArrModels[id]->SplitsArr[tmpidsplit]->CountAllGreen);
	++(ArrModels[id]->AllCountGreen);
	int currlen = ArrModels[id]->SplitsArr[tmpidsplit]->CountAllGreen;
	AlignBound(ArrModels[id], ArrModels[id]->SplitsArr[tmpidsplit]);

	if (idsplit)
		*idsplit = tmpidsplit;

	return oldlen;
}

void Green::DelObject(ID id, ID idsplit, ID idobj)
{
	if (id < 0 || ArrModels.size() <= id || idsplit < 0 || ArrModels[id]->SplitsArr.size() <= idsplit || idobj < 0 || ArrModels[id]->SplitsArr[idsplit]->CountAllGreen <= idobj)
		return;

	int oldlen = ArrModels[id]->SplitsArr[idsplit]->CountAllGreen;
	GreenDataVertex* tmpdv = new GreenDataVertex[oldlen - 1];
	memcpy(tmpdv, ArrModels[id]->SplitsArr[idsplit]->Data, idobj * sizeof(GreenDataVertex));
	memcpy(tmpdv + idobj, ArrModels[id]->SplitsArr[idsplit]->Data + idobj + 1, ((oldlen - idobj) - 1)* sizeof(GreenDataVertex));
	mem_delete_a(ArrModels[id]->SplitsArr[idsplit]->Data);
	ArrModels[id]->SplitsArr[idsplit]->Data = tmpdv;
	--(ArrModels[id]->SplitsArr[idsplit]->CountAllGreen);
	--(ArrModels[id]->AllCountGreen);
	AlignBound(ArrModels[id], ArrModels[id]->SplitsArr[idsplit]);
}

void Green::GetPosObject(ID id, ID idsplit, ID idobj, float3_t* pos)
{
	if (!pos || id < 0 || ArrModels.size() <= id || idsplit < 0 || ArrModels[id]->SplitsArr.size() <= idsplit || idobj < 0 || ArrModels[id]->SplitsArr[idsplit]->CountAllGreen <= idobj)
		return;

	*pos = ArrModels[id]->SplitsArr[idsplit]->Data[idobj].Position;
}

void Green::SetPosObject(ID id, ID* idsplit, ID* idobj, float3_t* pos)
{
	if (!pos || !idsplit || !idobj || id < 0 || ArrModels.size() <= id || (*idsplit) < 0 || ArrModels[id]->SplitsArr.size() <= (*idsplit) || (*idobj) < 0 || ArrModels[id]->SplitsArr[(*idsplit)]->CountAllGreen <= (*idobj))
		return;

	if (GetIDSplit(id, &float3(*pos)) == (*idsplit))
		ArrModels[id]->SplitsArr[(*idsplit)]->Data[(*idobj)].Position = *pos;
	else
	{
		GreenDataVertex tmpdv = ArrModels[id]->SplitsArr[(*idsplit)]->Data[(*idobj)];
		DelObject(id, (*idsplit), (*idobj));
		(*idobj) = AddObject(id, &float3(*pos), &tmpdv, idsplit);
	}
}

void Green::Save(const char* path)
{
	FILE* file = fopen(path, "wb");

	int32_t countmodel = ArrModels.size();
	fwrite(&countmodel, sizeof(int32_t), 1, file);

	for (long i = 0; i < ArrModels.size(); ++i)
	{
		fwrite(&ArrModels[i]->TypeGreen, sizeof(int32_t), 1, file);

		int32_t tmpstrlen = strlen(ArrModels[i]->Name);
		fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
		fwrite(ArrModels[i]->Name, sizeof(char), tmpstrlen, file);

		tmpstrlen = ArrModels[i]->MaskName.length();
		fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
		fwrite(ArrModels[i]->MaskName.c_str(), sizeof(char), tmpstrlen, file);

		if (ArrModels[i]->NavigateMesh)
		{
			tmpstrlen = strlen(ArrModels[i]->NavigateMesh->pathname.c_str());
			fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
			fwrite(ArrModels[i]->NavigateMesh->pathname.c_str(), sizeof(char), tmpstrlen, file);
		}
		else
		{
			tmpstrlen = 0;
			fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
		}

		if (ArrModels[i]->TypeGreen == GeomGreenType::ggt_grass)
		{
			tmpstrlen = strlen(ArrModels[i]->ArrLod[0]->path.c_str());
			fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
			fwrite(ArrModels[i]->ArrLod[0]->path.c_str(), sizeof(char), tmpstrlen, file);
		}
		else
		{
			for (int k = 0; k < GREEN_COUNT_LOD; ++k)
			{
				tmpstrlen = ArrModels[i]->ArrLod[k]->path.length();
				fwrite(&tmpstrlen, sizeof(int32_t), 1, file);
				fwrite(ArrModels[i]->ArrLod[k]->path.c_str(), sizeof(char), tmpstrlen, file);
			}
		}
		
		fwrite(&ArrModels[i]->BBMin.x, sizeof(float), 1, file);
		fwrite(&ArrModels[i]->BBMin.y, sizeof(float), 1, file);
		fwrite(&ArrModels[i]->BBMin.z, sizeof(float), 1, file);

		fwrite(&ArrModels[i]->BBMax.x, sizeof(float), 1, file);
		fwrite(&ArrModels[i]->BBMax.y, sizeof(float), 1, file);
		fwrite(&ArrModels[i]->BBMax.z, sizeof(float), 1, file);

		fwrite(&ArrModels[i]->AllCountGreen, sizeof(uint32_t), 1, file);

		Array<Segment*> queue;
		long tmpcount = 0;
		queue.push_back(ArrModels[i]->SplitsTree);

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

	fwrite(&Split->CountAllGreen, sizeof(uint32_t), 1, file);

	if (Split->CountAllGreen > 0)
		int qwerty = 0;

	fwrite(&Split->BFNonEnd, sizeof(int8_t), 1, file);

	bool isexists = true;

	if (Split->Splits[0]/*!Split->Data && Split->CountAllGreen > 0*/)
	{
		fwrite(&isexists, sizeof(int8_t), 1, file);
		for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		{
			if (Split->Splits[i])
				queue->push_back(Split->Splits[i]);
		}
	}
	else
	{
		isexists = false;
		fwrite(&isexists, sizeof(int8_t), 1, file);
		if (Split->CountAllGreen > 0)
			fwrite(Split->Data, sizeof(GreenDataVertex), Split->CountAllGreen, file);
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

	int32_t countmodel;
	fread(&countmodel, sizeof(int32_t), 1, file);

	for (long i = 0; i < countmodel; ++i)
	{
		tmpstr[0][0] = tmpstr[1][0] = tmpstr[2][0] = 0;
		Model* tmpmodel = new Model();
		fread(&tmpmodel->TypeGreen, sizeof(int32_t), 1, file);

		int32_t tmpstrlen;// = strlen(ArrModels[i]->Name);
		fread(&tmpstrlen, sizeof(int32_t), 1, file);
		fread(tmpNameMask, sizeof(char), tmpstrlen, file);
		tmpNameMask[tmpstrlen] = 0;
		sprintf(tmpmodel->Name,"%s",tmpNameMask);

		//tmpstrlen = ArrModels[i]->MaskName.length();
		fread(&tmpstrlen, sizeof(int32_t), 1, file);
		fread(tmpNameMask, sizeof(char), tmpstrlen, file);
		tmpNameMask[tmpstrlen] = 0;
		tmpmodel->MaskName = tmpNameMask;

		fread(&tmpstrlen, sizeof(int32_t), 1, file);
		if (tmpstrlen > 0)
		{
			fread(tmpNameMask, sizeof(char), tmpstrlen, file);
			tmpNameMask[tmpstrlen] = 0;
			tmpmodel->NavigateMesh = new Model::NavMesh();
			tmpmodel->NavigateMesh->pathname = tmpNameMask;
		}

		if (tmpmodel->TypeGreen == GeomGreenType::ggt_grass)
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

		
		tmpmodel->ArrLod[0] = new Lod();
		tmpmodel->ArrLod[1] = 0;
		tmpmodel->ArrLod[2] = 0;

		sprintf(tmppath, "%s%s", Green::StdPath, tmpstr[0]);
		SGCore_StaticModelLoad(tmppath, &tmpmodel->ArrLod[0]->model);
		tmpmodel->ArrLod[0]->path = tmpstr[0];
		char tmppathtex[1024];
		for (int k = 0; k < tmpmodel->ArrLod[0]->model->SubsetCount; ++k)
		{
			sprintf(tmppathtex, "%s.dds", tmpmodel->ArrLod[0]->model->ArrTextures[k]);
			tmpmodel->ArrLod[0]->idstex[k] = SGCore_MtlLoad(tmppathtex, (tmpmodel->TypeGreen == GeomGreenType::ggt_tree ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
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
				SGCore_StaticModelLoad(tmppath, &tmpmodel->ArrLod[1]->model);

				for (int k = 0; k < tmpmodel->ArrLod[1]->model->SubsetCount; ++k)
				{
					sprintf(tmppathtex, "%s.dds", tmpmodel->ArrLod[1]->model->ArrTextures[k]);
					tmpmodel->ArrLod[1]->idstex[k] = SGCore_MtlLoad(tmppathtex, (tmpmodel->TypeGreen == GeomGreenType::ggt_tree ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
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
				SGCore_StaticModelLoad(tmppath, &tmpmodel->ArrLod[2]->model);

				for (int k = 0; k < tmpmodel->ArrLod[2]->model->SubsetCount; ++k)
				{
					sprintf(tmppathtex, "%s.dds", tmpmodel->ArrLod[1]->model->ArrTextures[k]);
					tmpmodel->ArrLod[2]->idstex[k] = SGCore_MtlLoad(tmppathtex, (tmpmodel->TypeGreen == GeomGreenType::ggt_tree ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
				}
			}
		}

		fread(&tmpmodel->BBMin.x, sizeof(float), 1, file);
		fread(&tmpmodel->BBMin.y, sizeof(float), 1, file);
		fread(&tmpmodel->BBMin.z, sizeof(float), 1, file);

		fread(&tmpmodel->BBMax.x, sizeof(float), 1, file);
		fread(&tmpmodel->BBMax.y, sizeof(float), 1, file);
		fread(&tmpmodel->BBMax.z, sizeof(float), 1, file);

		fread(&tmpmodel->AllCountGreen, sizeof(uint32_t), 1, file);

		Array<Segment**> queue;
		long tmpcount = 0;
		queue.push_back(&(tmpmodel->SplitsTree));

		while (queue.size())
		{
			LoadSplit(queue[0], file, &queue);

			if ((*queue[0])->Data)
				tmpmodel->AllCountGreen += (*queue[0])->CountAllGreen;
			queue.erase(0);
			++tmpcount;
		}

		SetSplitID(tmpmodel);

		ArrModels.push_back(tmpmodel);
		AddModelInArrCom(ArrModels.size() - 1);

		if (tmpmodel->NavigateMesh)
			SetGreenNav(ArrModels.size() - 1, tmpmodel->NavigateMesh->pathname.c_str());
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

	fread(&(*Split)->CountAllGreen, sizeof(uint32_t), 1, file);

	fread(&(*Split)->BFNonEnd, sizeof(int8_t), 1, file);

	bool isexists = false;
	fread(&isexists, sizeof(int8_t), 1, file);

	if (isexists)
	{
		for (int i = 0; i<GREEN_COUNT_TYPE_SEGMENTATION; i++)
		{
			queue->push_back(&((*Split)->Splits[i]));
		}
	}
	else
	{
		if ((*Split)->CountAllGreen > 0)
		{
			(*Split)->Data = new GreenDataVertex[(*Split)->CountAllGreen];

			fread((*Split)->Data, sizeof(GreenDataVertex)*(*Split)->CountAllGreen, 1, file);
		}
	}
}

ID Green::AddArrForCom()
{
	IRSData* ttmpdata = new IRSData();
	for (long i = 0; i < ArrModels.size(); ++i)
	{
		InfoRenderSegments* tmpirs = new InfoRenderSegments();
		tmpirs->Count = ArrModels[i]->SplitsIDs;
		tmpirs->Arr = new Segment*[ArrModels[i]->SplitsIDs];
		tmpirs->CountCom = 0;
		ttmpdata->arr.push_back(tmpirs);
	}

	ID id_arr = -1;
	for (long i = 0; i < ArrComFor.size(); ++i)
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

void Green::DelArrForCom(ID id_arr)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID(id_arr);

	mem_delete(ArrComFor[id_arr]);
}

void Green::AddModelInArrCom(ID id_model)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model);
	
	for (long i = 0; i < ArrComFor.size(); ++i)
	{
		InfoRenderSegments* tmpirs = new InfoRenderSegments();
		tmpirs->Count = ArrModels[id_model]->SplitsIDs;
		tmpirs->Arr = new Segment*[ArrModels[id_model]->SplitsIDs];
		tmpirs->CountCom = 0;
		ArrComFor[i]->arr.push_back(tmpirs);
	}
}

void Green::DelModelInArrCom(ID id_model)
{
	GREEN_PRECOND_ARRCOMFOR_ERR_ID_MODEL(id_model);

	for (int i = 0; i < ArrComFor.size(); ++i)
	{
		if (!(ArrComFor[i]))
			continue;

		mem_delete(ArrComFor[i]->arr[id_model]);
		ArrComFor[i]->arr.erase(id_model);
	}
}

ID Green::GetCountGreen()
{
	return ArrModels.size();
}

char* Green::GetGreenName(ID id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->Name;

	return 0;
}

inline long Green::GetGreenCountGen(ID id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->AllCountGreen;

	return -1;
}

inline long Green::GetGreenCountPoly(ID id)
{
	if (id < ArrModels.size() && ArrModels[id]->ArrLod[0])
		return ArrModels[id]->ArrLod[0]->model->AllIndexCount / 3;

	return -1;
}

inline int Green::GetGreenTypeCountGen(ID id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->TypeGreen;

	return -1;
}

inline const char* Green::GetGreenModel(ID id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->ArrLod[0]->path.c_str();

	return 0;
}

inline const char* Green::GetGreenLod1(ID id)
{
	if (id < ArrModels.size() && ArrModels[id]->ArrLod[1])
		return ArrModels[id]->ArrLod[1]->path.c_str();

	return 0;
}

inline const char* Green::GetGreenLod2(ID id)
{
	if (id < ArrModels.size() && ArrModels[id]->ArrLod[2])
		return ArrModels[id]->ArrLod[2]->path.c_str();

	return 0;
}

inline const char* Green::GetGreenMask(ID id)
{
	if (id < ArrModels.size())
		return ArrModels[id]->MaskName.c_str();

	return 0;
}

inline const char* Green::GetGreenNav(ID id)
{
	if (id < ArrModels.size() && ArrModels[id]->NavigateMesh)
		return ArrModels[id]->NavigateMesh->pathname.c_str();

	return 0;
}

void Green::DelGreen(ID id)
{
	if (id >= 0 && id < ArrModels.size())
	{
		DelModelInArrCom(id);
		mem_delete(ArrModels[id]);
		ArrModels.erase(id);
	}
}

void Green::Clear()
{
	while(ArrModels.size() > 0)
	{
		DelModelInArrCom(0);
		mem_delete(ArrModels[0]);
		ArrModels.erase(0);
	}

	while (ArrComFor.size() > 2)
	{
		mem_delete(ArrComFor[2]);
		ArrComFor.erase(2);
	}
}

void Green::SetGreenLod(ID id, int lod, const char* pathname)
{
	if (!(lod >= 0 && lod < GREEN_COUNT_LOD && id >= 0 && id < ArrModels.size()))
		return;

	bool isunic = true;

	for (int i = 0; i < GREEN_COUNT_LOD; ++i)
	{
		if (ArrModels[id]->ArrLod[i] == ArrModels[id]->ArrLod[lod])
			isunic = false;
	}

	if (isunic)
		mem_delete(ArrModels[id]->ArrLod[lod]);

	char tmppath[1024];
	ArrModels[id]->ArrLod[lod] = new Lod();
	ArrModels[id]->ArrLod[lod]->path = pathname;
	sprintf(tmppath, "%s%s", Green::StdPath, pathname);
	SGCore_StaticModelLoad(tmppath, &ArrModels[id]->ArrLod[lod]->model);

	for (int k = 0; k < ArrModels[id]->ArrLod[lod]->model->SubsetCount; ++k)
	{
		sprintf(tmppath, "%s.dds", ArrModels[id]->ArrLod[lod]->model->ArrTextures[k]);
		ArrModels[id]->ArrLod[lod]->idstex[k] = SGCore_MtlLoad(tmppath, (ArrModels[id]->TypeGreen == GeomGreenType::ggt_tree ? MTL_TYPE_TREE : MTL_TYPE_GRASS));
	}
}

void Green::SetGreenNav(ID id, const char* pathname)
{
	if (!(id >= 0 && id < ArrModels.size()))
		return;

	char tmpstr[1024];
	sprintf(tmpstr, "%s", pathname);

	mem_delete(ArrModels[id]->NavigateMesh);
	ArrModels[id]->NavigateMesh = new Model::NavMesh();

	char tmppath[1024];
	sprintf(tmppath, "%s%s", Green::StdPath, tmpstr);
	ArrModels[id]->NavigateMesh->pathname = tmpstr;

	ISXDataStaticModel* nmesh;
	SGCore_StaticModelLoad(tmppath, &nmesh);
	ArrModels[id]->NavigateMesh->count_vertex = nmesh->AllVertexCount;
	ArrModels[id]->NavigateMesh->count_index = nmesh->AllIndexCount;
	ArrModels[id]->NavigateMesh->arr_vertex = new float3_t[nmesh->AllVertexCount];
	vertex_static *pVert;
	nmesh->VertexBuffer->Lock(0, 0, (void **)&pVert, 0);
	for (long i = 0; i < nmesh->AllVertexCount; ++i)
	{
		ArrModels[id]->NavigateMesh->arr_vertex[i] = pVert[i].Pos;
	}
	nmesh->VertexBuffer->Unlock();
	
	ArrModels[id]->NavigateMesh->arr_index = new uint32_t[nmesh->AllIndexCount];
	ArrModels[id]->NavigateMesh->arr_mtl = new ID[nmesh->AllIndexCount];
	UINT* pInd;
	nmesh->IndexBuffer->Lock(0, 0, (void **)&pInd, 0);
	DWORD prebias = 0;
	long tmp_countindex = 0;
	char tmpnametex[SXGC_LOADTEX_MAX_SIZE_DIRNAME];
	for (int i = 0; i < nmesh->SubsetCount; ++i)
	{
		sprintf(tmpnametex, "%s.dds", nmesh->ArrTextures[i]);
		ID tmpidmtl = SGCore_MtlLoad(tmpnametex, MTL_TYPE_TREE);
		for (int k = 0; k < nmesh->IndexCount[i]; ++k)
		{
			ArrModels[id]->NavigateMesh->arr_index[tmp_countindex] = pInd[nmesh->StartIndex[i] + k] /*+ prebias*/;
			ArrModels[id]->NavigateMesh->arr_mtl[tmp_countindex] = tmpidmtl;
			++tmp_countindex;
		}
		prebias += nmesh->IndexCount[i];
	}
	nmesh->IndexBuffer->Unlock();
	mem_release_del(nmesh);
}

void Green::GetNavMeshAndTransform(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, ID*** arr_mtl, int32_t** arr_count_index, GreenDataVertex*** arr_transform, int32_t** arr_count_transform, int32_t* arr_count_green)
{
	if (ArrModels.size() <= 0)
		return;

	int count_green = 0;

	for (int id = 0; id < ArrModels.size(); ++id)
	{
		if (!(ArrModels[id]->NavigateMesh))
			continue;

		Array<Segment*> queue;
		long tmpcount = 0;
		queue.push_back(ArrModels[id]->SplitsTree);

		ArrModels[id]->AllCountGreen = 0;

		while (queue.size())
		{
			if (!(queue[0]->Splits[0]) && queue[0]->Data && queue[0]->CountAllGreen > 0)
			{
				int AllCountGreen = ArrModels[id]->AllCountGreen;
				int CountAllGreen = queue[0]->CountAllGreen;
				GreenDataVertex* tmpdv = new GreenDataVertex[ArrModels[id]->AllCountGreen + queue[0]->CountAllGreen];
				if (ArrModels[id]->AllTrans)
					memcpy(tmpdv, ArrModels[id]->AllTrans, sizeof(GreenDataVertex)* ArrModels[id]->AllCountGreen);
				memcpy(tmpdv + ArrModels[id]->AllCountGreen, queue[0]->Data, sizeof(GreenDataVertex)* queue[0]->CountAllGreen);
				mem_delete_a(ArrModels[id]->AllTrans);
				ArrModels[id]->AllTrans = tmpdv;
				ArrModels[id]->AllCountGreen += queue[0]->CountAllGreen;
			}

			for (int i = 0; i < GREEN_COUNT_TYPE_SEGMENTATION; ++i)
			{
				if (queue[0]->Splits[i])
					queue.push_back(queue[0]->Splits[i]);
			}

			queue.erase(0);
			++tmpcount;
		}

		if (ArrModels[id]->AllTrans)
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

	(*arr_transform) = new GreenDataVertex*[count_green];
	(*arr_count_transform) = new int32_t[count_green];

	int curr_model = 0;

	for (int i = 0; i < ArrModels.size(); ++i)
	{
		if (!(ArrModels[i]->NavigateMesh))
			continue;

		(*arr_vertex)[curr_model] = new float3_t[ArrModels[i]->NavigateMesh->count_vertex];
		memcpy((*arr_vertex)[curr_model], ArrModels[i]->NavigateMesh->arr_vertex, sizeof(float3_t)* ArrModels[i]->NavigateMesh->count_vertex);
		(*arr_count_vertex)[curr_model] = ArrModels[i]->NavigateMesh->count_vertex;

		(*arr_index)[curr_model] = new uint32_t[ArrModels[i]->NavigateMesh->count_index];
		memcpy((*arr_index)[curr_model], ArrModels[i]->NavigateMesh->arr_index, sizeof(uint32_t)* ArrModels[i]->NavigateMesh->count_index);
		(*arr_mtl)[curr_model] = new ID[ArrModels[i]->NavigateMesh->count_index];
		memcpy((*arr_mtl)[curr_model], ArrModels[i]->NavigateMesh->arr_mtl, sizeof(ID)* ArrModels[i]->NavigateMesh->count_index);
		(*arr_count_index)[curr_model] = ArrModels[i]->NavigateMesh->count_index;

		(*arr_transform)[curr_model] = new GreenDataVertex[ArrModels[i]->AllCountGreen];
		(*arr_count_transform)[curr_model] = ArrModels[i]->AllCountGreen;
		for (long k = 0; k < ArrModels[i]->AllCountGreen; ++k)
		{
			(*arr_transform)[curr_model][k] = ArrModels[i]->AllTrans[k];// SMMatrixScaling(float3(ArrModels[i]->AllTrans[k].TexCoord.x, ArrModels[i]->AllTrans[k].TexCoord.x, ArrModels[i]->AllTrans[k].TexCoord.x)) * SMMatrixRotationY(ArrModels[i]->AllTrans[k].TexCoord.y) * SMMatrixTranslation(ArrModels[i]->AllTrans[k].Position);
		}

		++curr_model;
	}

	for (int id = 0; id < ArrModels.size(); ++id)
	{
		mem_delete_a(ArrModels[id]->AllTrans);
	}
}

void Green::GetPartBeam(float3* pos, float3 * dir, Segment** arrsplits, DWORD *count, Segment* comsegment, ID curr_splits_ids_render)
{
	float3 center;
	float radius;
	comsegment->BoundVolumeP->GetSphere(&center, &radius);

	float distsqr = UTIL_DistancePointBeam2(center, *pos, *dir);
	if (comsegment->CountAllGreen > 0 && distsqr <= radius*radius)
	{
		if (comsegment->BFNonEnd)
		{
			for (int j = 0; j<GREEN_COUNT_TYPE_SEGMENTATION; ++j)
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

bool Green::TraceBeam(float3* start, float3* dir, float3* _res, ID* idgreen, ID* idsplits, ID* idobj, ID* idmtl)
{
	if (ArrModels.size() <= 0)
		return false;

	triangle tmptri;
	float dist;
	bool tmpiscom = true;
	float3 ip;
	float3 res;
	float3 il;
	res = (*start) + float3(10000.0f, 10000.0f, 10000.0f);
	il = (*dir) * 10000.0f;
	bool found = false;
	InfoRenderSegments* irs = 0;
	Model* model = 0;
	float4x4 mat;

	for (int id = 0; id < ArrModels.size(); ++id)
	{
		model = ArrModels[id];
		irs = ArrComFor[1]->arr[id];
		irs->CountCom = 0;

		GetPartBeam(start, dir, irs->Arr, &(irs->CountCom), model->SplitsTree, irs->Count);

		vertex_static* pVertData = 0;
		if (FAILED(model->ArrLod[0]->model->VertexBuffer->Lock(0, 0, (void**)&pVertData, 0)))
			continue;

		DWORD* pIndData = 0;
		if (FAILED(model->ArrLod[0]->model->IndexBuffer->Lock(0, 0, (void**)&pIndData, 0)))
			continue;

		for (int k = 0; k < irs->CountCom; ++k)
		{
			for (int key = 0; key < irs->Arr[k]->CountAllGreen; ++key)
			{
				UINT tmpcountind = 0;
				for (int g = 0; g < model->ArrLod[0]->model->SubsetCount; ++g)
				{
					for (int poly = 0; poly < model->ArrLod[0]->model->IndexCount[g] / 3; ++poly)
					{
						float tmpscale = irs->Arr[k]->Data[key].TexCoord.x;
						mat = SMMatrixScaling(tmpscale, tmpscale, tmpscale) * SMMatrixRotationY(irs->Arr[k]->Data[key].TexCoord.y) * SMMatrixTranslation(irs->Arr[k]->Data[key].Position);

						tmptri.a = SMVector3Transform(pVertData[pIndData[poly]].Pos, mat);
						tmptri.b = SMVector3Transform(pVertData[pIndData[poly + 1]].Pos, mat);
						tmptri.c = SMVector3Transform(pVertData[pIndData[poly + 2]].Pos, mat);

						if (tmptri.IntersectLine((*start), il, &ip))
						{
							if (SMVector3Length2((*start) - res) > SMVector3Length2((*start) - ip))
							{
								res = ip;
								found = true;

								if (idgreen)
									*idgreen = id;

								if (idsplits)
									*idsplits = irs->Arr[k]->Id;

								if (idobj)
									*idobj = key;

								if (idmtl)
									*idmtl = model->ArrLod[0]->idstex[g];
							}
						}
					}

					tmpcountind += model->ArrLod[0]->model->SubsetCount;
				}
			}
		}

		model->ArrLod[0]->model->VertexBuffer->Unlock();
		model->ArrLod[0]->model->IndexBuffer->Unlock();
	}

	if (found && _res)
		*_res = res;

	return found;
}

void Green::GetPartBB(float3* bbmin, float3 * bbmax, Segment** arrsplits, DWORD *count, Segment* comsegment, ID curr_splits_ids_render)
{
	float3 min,max;
	float radius;
	comsegment->BoundVolumeP->GetMinMax(&min, &max);

	if (comsegment->CountAllGreen > 0 && UTIL_InretsectBox(bbmin, bbmax, &min, &max))
	{
		if (comsegment->BFNonEnd)
		{
			for (int j = 0; j<GREEN_COUNT_TYPE_SEGMENTATION; ++j)
			{
				if (comsegment->Splits[j])
					GetPartBB(bbmin, bbmax, arrsplits, count, comsegment->Splits[j], curr_splits_ids_render);
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

bool Green::GetOccurencessLeafGrass(float3* bbmin, float3* bbmax, int physic_mtl)
{
	if (ArrModels.size() <= 0)
		return false;

	bool isfound = false;
	InfoRenderSegments* irs = 0;
	Model* model = 0;
	float3 p1, p2, p3, min, max;
	float4x4 mat;

	for (int id = 0; id < ArrModels.size() && !isfound; ++id)
	{
		model = ArrModels[id];
		irs = ArrComFor[1]->arr[id];
		irs->CountCom = 0;

		GetPartBB(bbmin, bbmax, irs->Arr, &(irs->CountCom), model->SplitsTree, irs->Count);

		vertex_static* pVertData = 0;
		if (FAILED(model->ArrLod[0]->model->VertexBuffer->Lock(0, 0, (void**)&pVertData, 0)))
			continue;

		DWORD* pIndData = 0;
		if (FAILED(model->ArrLod[0]->model->IndexBuffer->Lock(0, 0, (void**)&pIndData, 0)))
			continue;

		for (int g = 0; g < model->ArrLod[0]->model->SubsetCount && !isfound; ++g)
		{
			int pt = SGCore_MtlGetPhysicType(model->ArrLod[0]->idstex[g]);
			if (SGCore_MtlGetPhysicType(model->ArrLod[0]->idstex[g]) != physic_mtl)
				continue;

			for (int k = 0; k < irs->CountCom && !isfound; ++k)
			{
				for (int key = 0; key < irs->Arr[k]->CountAllGreen && !isfound; ++key)
				{


					for (int poly = 0; poly < model->ArrLod[0]->model->IndexCount[g] / 3 && !isfound; ++poly)
					{
						float tmpscale = irs->Arr[k]->Data[key].TexCoord.x;
						mat = SMMatrixScaling(tmpscale, tmpscale, tmpscale) * SMMatrixRotationY(irs->Arr[k]->Data[key].TexCoord.y) * SMMatrixTranslation(irs->Arr[k]->Data[key].Position);

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

						if (UTIL_InretsectBox(bbmin, bbmax, &min, &max))
						{
							isfound = true;
							break;
						}
					}
				}
			}
		}

		model->ArrLod[0]->model->VertexBuffer->Unlock();
		model->ArrLod[0]->model->IndexBuffer->Unlock();
	}

	return isfound;
}