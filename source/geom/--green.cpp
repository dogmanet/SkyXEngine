
#pragma once

namespace SkyXEngine
{
namespace Graphics
{
namespace ThreeD
{

Green::Green()
{
	DataLod[0] = 0;
	DataLod[1] = 0;
	DataLod[2] = 0;

	sprintf(Paths[0],"%s","0");
	sprintf(Paths[1],"%s","0");
	sprintf(Paths[2],"%s","0");

	ArrIDTex = 0;
	/*ArrIDTex[0] = 0;
	ArrIDTex[1] = 0;
	ArrIDTex[2] = 0;*/

	ArrMaterials = 0;
	/*ArrMaterials[0] = 0;
	ArrMaterials[1] = 0;
	ArrMaterials[2] = 0;*/

	AllCountPoly[0] = 0;
	AllCountPoly[1] = 0;
	AllCountPoly[2] = 0;

	AllCountVertex[0] = 0;
	AllCountVertex[1] = 0;
	AllCountVertex[2] = 0;

	AllCountGreen = 0;
	AllTrans = 0;

	CountGrassInVert = 50000;

	SplitsIDs = 0;
	SplitsIDsRender = 0;

	IDMaterailFreeRenderGreenColor = Core::Data::MaterialsManager->GetID("mtrl_default_green_color");
	IDMaterailFreeRenderGReenShadowCube = Core::Data::MaterialsManager->GetID("mtrl_default_green_cube_shadow");
	IDMaterailFreeRenderGreenShadowDirectPSSM = Core::Data::MaterialsManager->GetID("mtrl_default_green_pssm_direct_shadow");
}

Green::~Green()
{
	nmem_delete_a(AllTrans);

	nmem_delete(DataLod[0]);
	nmem_delete(DataLod[1]);
	nmem_delete(DataLod[2]);

	nmem_delete(ArrSegments);
	ndx_release(TransVertBuf);

	nmem_delete_a(ArrMaterials)
	/*nmem_delete_a(ArrMaterials[0]);
	nmem_delete_a(ArrMaterials[1]);
	nmem_delete_a(ArrMaterials[2]);*/

	nmem_delete_a(ArrIDTex);
	/*nmem_delete_a(ArrIDTex[0]);
	nmem_delete_a(ArrIDTex[1]);
	nmem_delete_a(ArrIDTex[2]);*/
}

void Green::OnLostDevice()
{
	ndx_release(TransVertBuf);
}

void Green::OnResetDevice()
{
	Core::Data::Device->CreateVertexBuffer(
											CountGrassInVert * sizeof(float)*6,
											D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
											0,
											D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
											&TransVertBuf,
											0);
}

void Green::Init(DWORD id_model,const char* path_mask,float count_max)
{
		if(id_model >= 0 && id_model < SkyXEngine::Core::Data::Level::ArrStaticModel->GetCount())
		{
			IDMeshIn = id_model;
			float3 tmpmin,tmpmax;
			SkyXEngine::Core::Data::Level::ArrStaticModel->Arr[id_model]->ArrSegments->BoundVolumeP.GetMinMax(&tmpmin,&tmpmax);

			IDTexMask = SkyXEngine::Core::Data::LoadedTextures->AddName(path_mask);
			SkyXEngine::Core::Data::LoadedTextures->LoadTextures();

			CountMaxInPixel = count_max;

			float WidthLand = tmpmax.x - tmpmin.x;
			float HeightLand = tmpmax.z - tmpmin.z;

			D3DSURFACE_DESC desc;
			SkyXEngine::Core::Data::LoadedTextures->GetTexture(IDTexMask)->GetLevelDesc(0,&desc);

			D3DLOCKED_RECT LockedRect;

			SkyXEngine::Core::Data::LoadedTextures->GetTexture(IDTexMask)->LockRect(0,&LockedRect,0,0);
			DWORD* tmpColor = (DWORD*)LockedRect.pBits;

			AllCountGreen = 0;
			DWORD tmpUnAllCountGreen = 0;
			std::vector<float3_t> arrpos;
			//printf("\tStart cycle for compute position ...\n");
			DWORD MaxAlpha = 0;
				for(DWORD x=0;x<desc.Width;x++)
				{
						for(DWORD y=0;y<desc.Height;y++)
						{
							DWORD alpha = (tmpColor[y*desc.Width + x] >> 24);
							float AlphaColor = 1.f/255.f * (float)alpha;
								if(alpha > 0)
								{
									//позиция пикселя на ландшафте
									float PosInLandX = SMLerpf(tmpmin.x,tmpmax.x,float(x+1)/float(desc.Width));//WidthLand * ((float(x)/float(desc.Width))-0.5f);
									float PosInLandY = SMLerpf(tmpmax.z,tmpmin.z,float(y+1)/float(desc.Height));//HeightLand * ((float(y)/float(desc.Height))-0.5f);

									float3_t tmp2 = float3(PosInLandX,0,PosInLandY);

									float OneEdX = WidthLand / float(desc.Width);
									float OneEdY = HeightLand / float(desc.Height);
								
									//int tmpcountgreen = lerp(0,10,(float(alpha)*CountMaxInPixel)/255.f);//int(floor(float(float(alpha)*CountMaxInPixel*100)/2550.f));
										//расчет позиций объектов на квадратный метр
										for(int i=0;i<int(floor(float(float(alpha)*CountMaxInPixel*100)/2550.f));i++)
										{
											float3 tmp(tmp2.x,0,tmp2.z);
											bool isintersect = true;

											tmp.x = (tmp2.x - OneEdX*0.5f) + SMRndf(0.0,OneEdX*0.5f);//(float(rand()%OneEdX*0.5f);
											tmp.z = (tmp2.z - OneEdY*0.5f) + SMRndf(0.0,OneEdY*0.5f);//(float(rand()%OneEdY*0.5f);

											isintersect = SkyXEngine::Core::Data::Level::ArrStaticModel->GetIntersectedRayY(id_model,&tmp);
												if(isintersect)
												{
													arrpos.push_back(tmp);
													
													AllCountGreen++;
												}
												else
												{
													tmpUnAllCountGreen++;
												}
										}
								}
						}
				}

			SkyXEngine::Core::Data::LoadedTextures->GetTexture(IDTexMask)->UnlockRect(0);

			//printf("\tCompute position is completed, start rewrite ...\n");
			nmem_delete_a(AllTrans);

			AllTrans = new GreenDataVertex[AllCountGreen];

				for(DWORD i=0;i<AllCountGreen;i++)
				{
					AllTrans[i].Position = arrpos[i];
					AllTrans[i].TexCoord.x = 1.f + (float(rand()%100)/100.f);
					AllTrans[i].TexCoord.y = D3DXToRadian(float(rand()%360));
					AllTrans[i].TexCoord.z = (float(rand()%200)/100.f) - 1.f;
				}
			arrpos.clear();
			//printf("{green} Ok, mask is writed, AllCountGreen = %d\n",AllCountGreen);
		}
}

void Green::LoadOne(const char* path)
{
	/*int lod = 0;  
	nmem_delete(DataLod[lod]);
	DataLod[lod] = new Core::Loaders::DataStaticModel;

	LoadStaticModel(path, DataLod[lod]);

		for(DWORD i=0;i<DataLod[lod]->SubsetCount;i++)
		{
			DWORD* indeces;
			DataLod[lod]->IndexBuffer->Lock(0, 0, (void**)&indeces, 0);

			Core::OptimizeMesh::OptimizeIndecesInSubsetDword(indeces+DataLod[lod]->StartIndex[i],DataLod[lod]->IndexCount[i]/3,DataLod[lod]->VertexCount[i]);

			DataLod[lod]->IndexBuffer->Unlock();
		}

	nmem_delete_a(ArrIDTex[lod]);
	ArrIDTex[lod] = new DWORD[DataLod[lod]->SubsetCount];

	nmem_delete_a(ArrMaterials[lod]);
	ArrMaterials[lod] = new DWORD[DataLod[lod]->SubsetCount];

	AllCountPoly[lod] = 0;
	AllCountVertex[lod] = 0;
		for(DWORD i=0;i<DataLod[lod]->SubsetCount;i++)
		{
			char tmptex[256];
			sprintf(tmptex,"%s.dds",DataLod[lod]->ArrTextures[i]);
			ArrIDTex[lod][i] = SkyXEngine::Core::Data::LoadedTextures->AddName(tmptex);
			AllCountPoly[lod] += DataLod[lod]->IndexCount[i] / 3;
			AllCountVertex[lod] += DataLod[lod]->VertexCount[i];

			ArrMaterials[lod][i] = SkyXEngine::Core::Data::MaterialsManager->Load(tmptex,1);
		}
	sprintf(Paths[lod],"%s",path);*/

	Load(path,0);
	Load(path,1);
	Load(path,2);

	nmem_delete_a(AllTrans);
	AllTrans = new GreenDataVertex[1];
	AllTrans[0].Position = float3_t(0,0,0);
	AllTrans[0].TexCoord.x = 1.f + (float(rand()%100)/100.f);
	AllTrans[0].TexCoord.y = D3DXToRadian(float(rand()%360));
	AllTrans[0].TexCoord.z = (float(rand()%200)/100.f) - 1.f;

	AllCountGreen = 1;
	CountGrassInVert = AllCountGreen+1;

	Segmentation(1);
	IsGrass = false;
}

void Green::Load(const char* path,int lod)
{
		if(lod < 3 && lod >= 0 && StrValidate(path))
		{
			//ищем загружена ли эта модель уже в один из лодов
			//если да то копируем данные из нее в нужный лод
			bool IsExist = false;
				for(int i=0;i<3;i++)
				{
						if(lod != i && strcmp(path,Paths[lod]) == 0)
						{
							memcpy(DataLod[lod],DataLod[i],sizeof(Graphics::ThreeD::DataStaticModel));
							//memcpy(ArrIDTex[lod],ArrIDTex[i],sizeof(DWORD) * DataLod[i]->SubsetCount);

							AllCountPoly[lod] = AllCountPoly[i];
							AllCountVertex[lod] = AllCountVertex[i];
							sprintf(Paths[lod],"%s",path);
							IsExist = true;
							break;
						}
				}
				
				//если не существует то грузим
				if(!IsExist)
				{
					nmem_delete(DataLod[lod]);
					DataLod[lod] = new Graphics::ThreeD::DataStaticModel;

					LoadStaticModel(path, DataLod[lod]);

						for(DWORD i=0;i<DataLod[lod]->SubsetCount;i++)
						{
							DWORD* indeces;
							DataLod[lod]->IndexBuffer->Lock(0, 0, (void**)&indeces, 0);

							Core::OptimizeMesh::OptimizeIndecesInSubsetDword(indeces+DataLod[lod]->StartIndex[i],DataLod[lod]->IndexCount[i]/3,DataLod[lod]->VertexCount[i]);

							DataLod[lod]->IndexBuffer->Unlock();
						}

						if(lod == 0)
						{
							nmem_delete_a(ArrIDTex);
							ArrIDTex = new DWORD[DataLod[lod]->SubsetCount];

							nmem_delete_a(ArrMaterials);
							ArrMaterials = new DWORD[DataLod[lod]->SubsetCount];
						}

					AllCountPoly[lod] = 0;
					AllCountVertex[lod] = 0;
						for(DWORD i=0;i<DataLod[lod]->SubsetCount;i++)
						{
								if(lod == 0)
								{
									char tmptex[256];
									sprintf(tmptex,"%s.dds",DataLod[lod]->ArrTextures[i]);
									ArrIDTex[i] = SkyXEngine::Core::Data::LoadedTextures->AddName(tmptex);
									ArrMaterials[i] = SkyXEngine::Core::Data::MaterialsManager->Load(tmptex,1);
								}
							AllCountPoly[lod] += DataLod[lod]->IndexCount[i] / 3;
							AllCountVertex[lod] += DataLod[lod]->VertexCount[i];

							
						}
					sprintf(Paths[lod],"%s",path);
				}
		}
}

void Green::LoadBinMask(const char* path)
{
	FILE* file = 0;
	file = fopen(path,"rb");

	char HeaderFile[13];

	//fscanf(file,"%s",HeaderFile);
	fread(HeaderFile,13,1,file);

		if(strcmp(HeaderFile,"sxgreentrans") == 0)
		{
			fread(&AllCountGreen,sizeof(DWORD),1,file);
			AllTrans = new GreenDataVertex[AllCountGreen];
			fread(AllTrans,sizeof(GreenDataVertex),AllCountGreen,file);
		}
	CountGrassInVert = AllCountGreen+1;
	fclose(file);
}

void Green::Segmentation(DWORD count_object_in_split)
{
		//если загружен только первый лод то значит ето трава
		if(DataLod[0] != 0 && (!DataLod[1] && !DataLod[2]))
			IsGrass = true;
		else
			IsGrass = false;

	CountObjectInSegment = count_object_in_split;
	Core::Data::Device->CreateVertexBuffer(
											CountGrassInVert * sizeof(float)*6,
											D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
											0,
											D3DPOOL_DEFAULT/*D3DPOOL_MANAGED*/,
											&TransVertBuf,
											0);
	
	ArrSegments = new Segment();

	float3 tmpMin,tmpMax;
	float3 tmpMin2,tmpMax2;
	Core::WorkModel::ComputeBoundingBox(DataLod[0]->VertexBuffer,&(ArrSegments->BoundVolumeSys),AllCountVertex[0],sizeof(Graphics::ThreeD::vertex_static));

	ArrSegments->BoundVolumeSys.GetMinMax(&tmpMin2,&tmpMax2); 
	BBMax = tmpMax2 * 2.f;
	BBMin = tmpMin2 * 2.f;
	tmpMin = AllTrans[0].Position;
	tmpMax = AllTrans[0].Position;

		for(DWORD i=0;i<AllCountGreen;i++)
		{
				if(AllTrans[i].Position.x > tmpMax.x)
					tmpMax.x = AllTrans[i].Position.x;

				if(AllTrans[i].Position.y > tmpMax.y)
					tmpMax.y = AllTrans[i].Position.y;

				if(AllTrans[i].Position.z > tmpMax.z)
					tmpMax.z = AllTrans[i].Position.z;


				if(AllTrans[i].Position.x < tmpMin.x)
					tmpMin.x = AllTrans[i].Position.x;

				if(AllTrans[i].Position.y < tmpMin.y)
					tmpMin.y = AllTrans[i].Position.y;

				if(AllTrans[i].Position.z < tmpMin.z)
					tmpMin.z = AllTrans[i].Position.z;
		}

	tmpMax.x += BBMax.x;
	tmpMax.y += BBMax.y;
	tmpMax.z += BBMax.z;

	tmpMin.x += BBMin.x;
	tmpMin.y += BBMin.y;
	tmpMin.z += BBMin.z;

	float tmpX = tmpMax.x - tmpMin.x;
	float tmpY = tmpMax.y - tmpMin.y;
	float tmpZ = tmpMax.z - tmpMin.z;

	ArrSegments->BoundVolumeP.SetMinMax(&tmpMin,&tmpMax);
	SkyXEngine::Core::WorkModel::CreateBoundingBoxMesh(&tmpMin,&tmpMax,&(ArrSegments->BoundBox));

		//выравниваем по квадрату
		if(tmpX > tmpZ)
		{
			tmpZ = tmpX - tmpZ;
			tmpZ /= 2.f;
			tmpMax.z += tmpZ;
			tmpMin.z -= tmpZ;
		}
		else if(tmpZ > tmpX)
		{
			tmpX = tmpZ - tmpX;
			tmpX /= 2.f;
			tmpMax.x += tmpX;
			tmpMin.x -= tmpX;
		}

	ArrSegments->BoundVolumeSys.SetMinMax(&tmpMin,&tmpMax);
	
	ArrSegments->CountAllPoly = AllCountGreen;
	ArrSegments->GreenData = new GreenDataVertex[AllCountGreen];
	memcpy(ArrSegments->GreenData,AllTrans,sizeof(GreenDataVertex) * AllCountGreen);

		if(ArrSegments->CountAllPoly > 0 && ArrSegments->CountAllPoly > CountObjectInSegment)
		{
			ArrSegments->BFNonEnd = true;
			CycleSegmentation(ArrSegments);
		}
		else
		{
			ArrSegments->BFNonEnd = false;
		}

	SplitsIDs = 0;
	SplitsIDsRender = 0;
	SetSplitID(ArrSegments);
}

void Green::CycleSegmentation(Segment* SplitingMesh)
{
		if(SplitingMesh->BFNonEnd)
		{
			SecondSeparated(SplitingMesh);
				for(int i=0;i<4;i++)
				{
						if(SplitingMesh->SplitMeshes[i])
							CycleSegmentation(SplitingMesh->SplitMeshes[i]);
				}
		}
}

void Green::SecondSeparated(Segment* SplitingMesh)
{
	Array<DWORD> ArrPoly[4];
	
		for(int i=0;i<4;i++)
			SplitingMesh->SplitMeshes[i] = new Segment();

	SkyXEngine::Graphics::ThreeD::Bound ArrBound[4];

	Core::WorkModel::ComputeBoundingBoxArr4(&(SplitingMesh->BoundVolumeSys),(ArrBound));
		for(int i=0;i<4;i++)
		{
			SplitingMesh->SplitMeshes[i]->BoundVolumeSys = ArrBound[i];
			SplitingMesh->SplitMeshes[i]->BoundVolumeP = ArrBound[i];
		}

	bool *tmp_arr_mesh_poly = new bool[SplitingMesh->CountAllPoly];
		for(DWORD i=0;i<SplitingMesh->CountAllPoly;i++)
			tmp_arr_mesh_poly[i] = true;

	DWORD tmpCountNonIn = 0;
	float3 tmpMin,tmpMax;
		for(WORD i=0;i<4;i++)
		{
			SplitingMesh->SplitMeshes[i]->BoundVolumeSys.GetMinMax(&tmpMin,&tmpMax);
			SkyXEngine::Core::WorkModel::CreateBoundingBoxMesh(&tmpMin,&tmpMax,&(SplitingMesh->SplitMeshes[i]->BoundBox));

				for(DWORD j=0;j<SplitingMesh->CountAllPoly;j++)
				{
						//если позиция проверяемого полигона находиться в пределах ограничивающего паралелепипеда
						if(
							(long(tmpMax.x * 1000) >= long(SplitingMesh->GreenData[j].Position.x * 1000) && long(tmpMin.x * 1000) <= long(SplitingMesh->GreenData[j].Position.x * 1000))
							&& 
							(long(tmpMax.z * 1000) >= long(SplitingMesh->GreenData[j].Position.z * 1000) && long(tmpMin.z * 1000) <= long(SplitingMesh->GreenData[j].Position.z * 1000))
							 &&
							tmp_arr_mesh_poly[j]
						  )
						{
							//записываем вершины в массив
							ArrPoly[i].push_back(j);
							//полигон использваон, т.е. занят
							tmp_arr_mesh_poly[j] = false;
							tmpCountNonIn++;
						}
				}
		}


		for(int i=0;i<4;i++)
		{
			SplitingMesh->SplitMeshes[i]->CountAllPoly = ArrPoly[i].size();

				if(SplitingMesh->SplitMeshes[i]->CountAllPoly > 0)
				{
					SplitingMesh->SplitMeshes[i]->GreenData = new GreenDataVertex[SplitingMesh->SplitMeshes[i]->CountAllPoly];

					float3 comMax = SplitingMesh->GreenData[ArrPoly[i][0]].Position;
					float3 comMin = SplitingMesh->GreenData[ArrPoly[i][0]].Position;

						for(DWORD k=0;k<ArrPoly[i].size();k++)
						{
								SplitingMesh->SplitMeshes[i]->GreenData[k] = SplitingMesh->GreenData[ArrPoly[i][k]];

									if(SplitingMesh->GreenData[ArrPoly[i][k]].Position.y > comMax.y)
										comMax.y = SplitingMesh->GreenData[ArrPoly[i][k]].Position.y;

									if(SplitingMesh->GreenData[ArrPoly[i][k]].Position.y < comMin.y)
										comMin.y = SplitingMesh->GreenData[ArrPoly[i][k]].Position.y;


									if(SplitingMesh->GreenData[ArrPoly[i][k]].Position.x > comMax.x)
										comMax.x = SplitingMesh->GreenData[ArrPoly[i][k]].Position.x;

									if(SplitingMesh->GreenData[ArrPoly[i][k]].Position.x < comMin.x)
										comMin.x = SplitingMesh->GreenData[ArrPoly[i][k]].Position.x;


									if(SplitingMesh->GreenData[ArrPoly[i][k]].Position.z > comMax.z)
										comMax.z = SplitingMesh->GreenData[ArrPoly[i][k]].Position.z;

									if(SplitingMesh->GreenData[ArrPoly[i][k]].Position.z < comMin.z)
										comMin.z = SplitingMesh->GreenData[ArrPoly[i][k]].Position.z;
						}

					SplitingMesh->SplitMeshes[i]->BoundVolumeSys.GetMinMax(&tmpMin,&tmpMax);
					tmpMax.y = comMax.y + BBMax.y;
					tmpMin.y = comMin.y + BBMin.y;

					SplitingMesh->SplitMeshes[i]->BoundVolumeSys.SetMinMax(&tmpMin,&tmpMax);

					tmpMax.x = comMax.x + BBMax.x;
					tmpMax.y = comMax.y + BBMax.y;
					tmpMax.z = comMax.z + BBMax.z;

					tmpMin.x = comMin.x + BBMin.x;
					tmpMin.y = comMin.y + BBMin.y;
					tmpMin.z = comMin.z + BBMin.z;

					SplitingMesh->SplitMeshes[i]->BoundVolumeP.SetMinMax(&tmpMin,&tmpMax);

					ArrPoly[i].clear();
				}

				if(SplitingMesh->SplitMeshes[i]->CountAllPoly > 0 && SplitingMesh->SplitMeshes[i]->CountAllPoly > CountObjectInSegment)
					SplitingMesh->SplitMeshes[i]->BFNonEnd = true;
				else if(SplitingMesh->SplitMeshes[i]->CountAllPoly <= 0)
				{
					nmem_delete(SplitingMesh->SplitMeshes[i]);
				}
				else
					SplitingMesh->SplitMeshes[i]->BFNonEnd = false;
		}

	nmem_delete_a(SplitingMesh->GreenData);
	nmem_delete_a(tmp_arr_mesh_poly);
}

void Green::SetSplitID(Segment* SplitingMesh)
{
		if(SplitingMesh && SplitingMesh->BFNonEnd)
		{
			SplitingMesh->ID = SplitsIDs;
			SplitsIDs++;
				for(int i=0;i<4;i++)
					SetSplitID(SplitingMesh->SplitMeshes[i]);
		}
		else if(SplitingMesh)
		{
				//if(SplitingMesh->CountAllPoly > 0)
				//{
					SplitingMesh->ID = SplitsIDs;
					SplitsIDs++;
					SplitsIDsRender++;
				//}
				//else
					//SplitingMesh = 0;//nmem_delete(SplitingMesh);
		}
}

void Green::CPUFillingArrIndeces(Core::ControllMoving::Frustum* frustum,Segment** arrsplits,DWORD *count,bool is_camera)
{
	ComRecArrIndeces(frustum,arrsplits,count,ArrSegments,is_camera);
}

void Green::ComRecArrIndeces(Core::ControllMoving::Frustum* frustum,Segment** arrsplits,DWORD *count,Segment* comsegment,bool is_camera)
{
	comsegment->BoundVolumeP.GetSphere(&jcenter,&jradius);
		if(comsegment->CountAllPoly > 0 && frustum->SphereInFrustum(&jcenter,jradius) )
		{
			//comsegment->BoundVolumeP.GetMinMax(&jmin,&jmax);
				//if(frustum->BoxInFrustum(&jmin,&jmax))
				{
				if(comsegment->BFNonEnd)
				{
						if(Core::Data::Settings::IsUseSortFrontToBack)
						{
								for(int q=0;q<4;q++)
								{
									comsegment->SortId[q] = -1;
										if(comsegment->SplitMeshes[q])
										{
											comsegment->SplitMeshes[q]->BoundVolumeP.GetSphere(&jcenter,&jradius);
											comsegment->SplitMeshes[q]->DistForCamera = SMVector3Length2((jcenter - Core::Data::ConstCurrentCameraPosition)) - jradius*jradius;
										}
								}

							float pl1,pl2;
							DWORD tmpCountGreater = 0;
								for(DWORD i=0;i<4;i++)
								{
										if(comsegment->SplitMeshes[i])
										{
											tmpCountGreater = 0;
											pl1 = comsegment->SplitMeshes[i]->DistForCamera;

												for(DWORD k=0;k<4;k++)
												{
														if(comsegment->SplitMeshes[k])
														{
															pl2 = comsegment->SplitMeshes[k]->DistForCamera;

																if(i != k && pl2 >= pl1)
																	tmpCountGreater++;
														}
												}

											bool tmpisnend = true;
												while(tmpisnend)
												{
														if(comsegment->SortId[tmpCountGreater] == -1)
														{
															comsegment->SortId[tmpCountGreater] = i;
															tmpisnend = false;
														}
														else
															tmpCountGreater--;
												}
										}
								}

								for(int j=0;j<4;j++)
								{
										if(comsegment->SortId[3-j] != -1 && comsegment->SplitMeshes[comsegment->SortId[3-j]])
											ComRecArrIndeces(frustum,arrsplits,count,comsegment->SplitMeshes[comsegment->SortId[3-j]],is_camera);
								}
						}
						else
						{
								for(int j=0;j<4;j++)
								{
										if(comsegment->SplitMeshes[j])
											ComRecArrIndeces(frustum,arrsplits,count,comsegment->SplitMeshes[j],is_camera);
								}
						}
				}
				else
				{
						if(is_camera)
							comsegment->DistForCamera = SMVector3Length((jcenter - Core::Data::ConstCurrentCameraPosition)) - jradius;

						if(comsegment->DistForCamera < 0)
							comsegment->DistForCamera = 0;

						if((*count) < SplitsIDsRender)
						{
							arrsplits[(*count)] = comsegment;

								if(is_camera)
									arrsplits[(*count)]->IsRenderForCamera = true;
								else
									arrsplits[(*count)]->IsRenderFor = true;
								
							(*count)++;
						}
				}
				}
		}
}


void Green::GPURender(Segment** arrsplits,DWORD count,int lod,bool ismaterial,bool is_camera,int how_shader,float3* lightpos,bool is_selected)
{
		if(lod == 0 || IsOrder)
			IsOrder = true;

	//!!!ОПТИМИЗИРОВАТЬ ПО RTCountDrawObj И ПРОЧИМ КОЛИЧЕСТВАМ
		if(lod < 3 && DataLod[lod])
		{
			GreenDataVertex* RTGPUArrVerteces;
			TransVertBuf->Lock(0, 0, (void**)&RTGPUArrVerteces, D3DLOCK_DISCARD);
			RTCountDrawObj = 0;

				for(DWORD i=0;i<count;i++)
				{
						if((is_camera ? arrsplits[i]->IsRenderForCamera : arrsplits[i]->IsRenderFor) &&	//если рисуем для камеры и указано что рисуем либо не для камеры и тоже указано
							(IsOrder && (	//распределение по дистанции есесн ои по лодам
											(lod == 0 && arrsplits[i]->DistForCamera < Core::Data::Settings::DistFor1SplitGreen) || 
											(lod == 1 && arrsplits[i]->DistForCamera < Core::Data::Settings::DistFor2SplitGreen && arrsplits[i]->DistForCamera > Core::Data::Settings::DistFor1SplitGreen) || 
											(lod == 2 && arrsplits[i]->DistForCamera > Core::Data::Settings::DistFor2SplitGreen)
										) || !IsOrder
							) &&
							RTCountDrawObj + arrsplits[i]->CountAllPoly < CountGrassInVert)	//если мы заполнили не весь масиив
						{
								//если это не трава
								if(!(lod == 0 && DataLod[lod+1] == 0 && DataLod[lod+2] == 0))
								{
									memcpy(RTGPUArrVerteces+(RTCountDrawObj),
									arrsplits[i]->GreenData,
									arrsplits[i]->CountAllPoly * sizeof(GreenDataVertex));
																	
									RTCountDrawObj += arrsplits[i]->CountAllPoly;
								}
								//иначе это трава, а ее по особенному рисуем
								else
								{
										if(Core::Data::Settings::CurrentFreqGrass == 100)
										{
											memcpy(RTGPUArrVerteces+(RTCountDrawObj),
											arrsplits[i]->GreenData,
											arrsplits[i]->CountAllPoly * sizeof(GreenDataVertex));
																					
											RTCountDrawObj += arrsplits[i]->CountAllPoly;
										}
										else
										{
											float percent = float(Core::Data::Settings::CurrentFreqGrass)/100.f; //10%
											float newCount = ((float)arrsplits[i]->CountAllPoly * percent);
											float step = float(arrsplits[i]->CountAllPoly) / newCount;
											UINT lastCP = 0;
												for(float k = 0; k < arrsplits[i]->CountAllPoly; k += step)
												{
													UINT curCP = (int)floor(k + 0.5);;
														if(curCP > lastCP)
														{
															memcpy(RTGPUArrVerteces+RTCountDrawObj,arrsplits[i]->GreenData+curCP,sizeof(GreenDataVertex));
															RTCountDrawObj += 1;
															lastCP = curCP;
														}
												}
										}
								}
						}
				}

			
			TransVertBuf->Unlock();

				//если есть что к отрисовке
				if(RTCountDrawObj)
				{
					Core::Data::Device->SetStreamSourceFreq(0, (D3DSTREAMSOURCE_INDEXEDDATA | RTCountDrawObj) );
					
					Core::Data::Device->SetStreamSourceFreq(1, (D3DSTREAMSOURCE_INSTANCEDATA | 1) );
					Core::Data::Device->SetStreamSource(1, TransVertBuf, 0,  sizeof(GreenDataVertex) );

					SkyXEngine::Core::Data::Device->SetStreamSource(0, DataLod[lod]->VertexBuffer, 0, sizeof(Graphics::ThreeD::vertex_static));
					SkyXEngine::Core::Data::Device->SetIndices(DataLod[lod]->IndexBuffer);
					SkyXEngine::Core::Data::Device->SetVertexDeclaration(SkyXEngine::Graphics::ThreeD::VertexDeclarationGreen);
								
					DWORD tmpCountIndex = 0;
						for(DWORD i=0;i<DataLod[lod]->SubsetCount;i++)
						{
								if((
									(SkyXEngine::Core::Data::Settings::IsComEditors && //если обработка редакторная
										(SkyXEngine::Core::Data::Settings::EditorsTypeRenderSubset == 0 ||	//если рисуем все подгруппы
										(SkyXEngine::Core::Data::Settings::EditorsTypeRenderSubset == 1 &&	//если рисуем выделенную подгруппу 
										i == SkyXEngine::Core::Data::Settings::EditorsSelectedSubset) ||	//и если текущая подгруппа есть выделенная
										(SkyXEngine::Core::Data::Settings::EditorsTypeRenderSubset == 2 &&	//если рисуем все кроме выделенной подгруппы 
										i != SkyXEngine::Core::Data::Settings::EditorsSelectedSubset))) ||	//и если текущая подгруппа не есть выделенная
									!SkyXEngine::Core::Data::Settings::IsComEditors	//если обработка не редакторная
									))
								{
										if(ismaterial)
										{
											SkyXEngine::Core::Data::MaterialsManager->Render(this->ArrMaterials[i],&(SMMatrixIdentity()),0);
												if(/*this->IsGrass && */Core::Data::MaterialsManager->GetMaterial(this->ArrMaterials[i])->PreShaderVS != -1)
													Core::Data::ShadersManager->SetValueRegisterF(
																									0,
																									Core::Data::MaterialsManager->GetMaterial(this->ArrMaterials[i])->PreShaderVS,
																									"DistBeginEndLessening",
																									&float2(Core::Data::Settings::DistBeginLesseningGrass,Core::Data::Settings::DistFor1SplitGreen));
											//DistBeginEndLessening
										}
										else
										{
												if(how_shader == 0)
													SkyXEngine::Core::Data::MaterialsManager->Render(IDMaterailFreeRenderGreenColor,&(SMMatrixIdentity()),0);
												else if(how_shader == 1)
													SkyXEngine::Core::Data::MaterialsManager->Render(IDMaterailFreeRenderGreenShadowDirectPSSM,&(SMMatrixIdentity()),0);
												else if(how_shader == 2)
													SkyXEngine::Core::Data::MaterialsManager->Render(IDMaterailFreeRenderGReenShadowCube,&(SMMatrixIdentity()),0);
											SkyXEngine::Core::Data::MaterialsManager->SetMainTexture(0,this->ArrMaterials[i]);
										}
									
										if(is_selected)
											SkyXEngine::Core::Data::Device->SetTexture(0,SkyXEngine::Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_FreeSelectedMesh));

									SkyXEngine::Core::Data::Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, DataLod[lod]->VertexCount[i], tmpCountIndex, DataLod[lod]->IndexCount[i] / 3);
									tmpCountIndex += DataLod[lod]->IndexCount[i];
									SkyXEngine::Core::Data::Statistics::CountPolyGreen += (DataLod[lod]->IndexCount[i] / 3) * RTCountDrawObj;
									SkyXEngine::Core::Data::Statistics::CountDips += 1;
								}
						}

					Core::Data::ShadersManager->UnBind();

					Core::Data::Device->SetStreamSourceFreq(0,1);
					Core::Data::Device->SetStreamSourceFreq(1,1);
				}

				//если сейчас рисовали 0 лод и первый есть либо рисовали первый и вторйо есть
				if((lod == 0 && DataLod[1]) || (lod == 1 && DataLod[2]))
				{
					lod++;
					GPURender(arrsplits,count,lod,ismaterial,is_camera);
				}

					//если второй лод то прекращаем отрисовку
					if(lod == 2)
						IsOrder = false;

				/*if(1)
				{
					Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
					Core::Data::Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

					Core::Data::ShadersManager->UnBind();
					Core::Data::Device->SetTexture(0,0);
					ArrSegments->BoundBox->DrawSubset(0);
					/*ArrSegments->SplitMeshes[0]->BoundBox->DrawSubset(0);
					ArrSegments->SplitMeshes[1]->BoundBox->DrawSubset(0);
					ArrSegments->SplitMeshes[2]->BoundBox->DrawSubset(0);
					ArrSegments->SplitMeshes[3]->BoundBox->DrawSubset(0);*/

					/*Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					Core::Data::Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				}*/
		}
		else
			Core::InLog("!!! invalide number lod of the green");
}

////////////////////////

ManagerGreen::ManagerGreen()
{
	IsSelectedMesh = false;
}

ManagerGreen::~ManagerGreen()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			nmem_delete(Arr[i]);
			nmem_delete_a(ArrComForCamera[i]->Arr);
			nmem_delete(ArrComForCamera[i]);
		}

		for(DWORD i=0;i<ArrFrustums.size();i++)
		{
				for(DWORD k=0;k<Arr.size();k++)
				{
					nmem_delete(ArrFrustums[i][k]);
				}
		}

	Arr.clear();
	ArrComForCamera.clear();
	ArrFrustums.clear();
}

void ManagerGreen::OnLostDevice()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			Arr[i]->OnLostDevice();
		}
}

void ManagerGreen::OnResetDevice()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			Arr[i]->OnResetDevice();
		}
}

void ManagerGreen::AddOne(const char* path)
{
	Green* tmpGreen = new Green();
	tmpGreen->LoadOne(path);
	/*tmpGreen->Load(lod1,1);
	tmpGreen->Load(lod2,2);
	tmpGreen->Init(id_model,path_mask,count_max);*/
	//tmpGreen->Segmentation(1);

	Arr.push_back(tmpGreen);

	InfoRenderSegments* tmpIRS = new InfoRenderSegments();
	tmpIRS->Count = tmpGreen->GetCountSplits();
	tmpIRS->Arr = new Segment*[tmpGreen->GetCountSplits()];
	tmpIRS->CountCom = 0;

	ArrComForCamera.push_back(tmpIRS);

		for(DWORD i=0;i<ArrFrustums.size();i++)
		{
			InfoRenderSegments* tmpIRS2 = new InfoRenderSegments();
			tmpIRS2->Count = tmpGreen->GetCountSplitsRender();
			tmpIRS2->Arr = new Segment*[tmpGreen->GetCountSplitsRender()];
			tmpIRS2->CountCom = 0;

			ArrFrustums[i].push_back(tmpIRS2);
		}
}

void ManagerGreen::Gen(DWORD id_model,float count_max,const char* path,DWORD count_object_in_segment,const char* lod1,const char* lod2,const char* path_mask)
{
	Green* tmpGreen = new Green();
	tmpGreen->Load(path,0);
	tmpGreen->Load(lod1,1);
	tmpGreen->Load(lod2,2);
	tmpGreen->Init(id_model,path_mask,count_max);
	tmpGreen->Segmentation(count_object_in_segment);

	Arr.push_back(tmpGreen);

	InfoRenderSegments* tmpIRS = new InfoRenderSegments();
	tmpIRS->Count = tmpGreen->GetCountSplits();
	tmpIRS->Arr = new Segment*[tmpGreen->GetCountSplits()];
	tmpIRS->CountCom = 0;

	ArrComForCamera.push_back(tmpIRS);

		for(DWORD i=0;i<ArrFrustums.size();i++)
		{
			InfoRenderSegments* tmpIRS2 = new InfoRenderSegments();
			tmpIRS2->Count = tmpGreen->GetCountSplitsRender();
			tmpIRS2->Arr = new Segment*[tmpGreen->GetCountSplitsRender()];
			tmpIRS2->CountCom = 0;

			ArrFrustums[i].push_back(tmpIRS2);
		}
}

void ManagerGreen::Add(const char* path,DWORD count_object_in_segment,const char* lod1,const char* lod2,const char* path_bin_mask)
{
	Green* tmpGreen = new Green();
	tmpGreen->Load(path,0);
	tmpGreen->Load(lod1,1);
	tmpGreen->Load(lod2,2);
	tmpGreen->LoadBinMask(path_bin_mask);
	tmpGreen->Segmentation(count_object_in_segment);

	Arr.push_back(tmpGreen);

	InfoRenderSegments* tmpIRS = new InfoRenderSegments();
	tmpIRS->Count = tmpGreen->GetCountSplitsRender();
	tmpIRS->Arr = new Segment*[tmpGreen->GetCountSplitsRender()];
	tmpIRS->CountCom = 0;

	ArrComForCamera.push_back(tmpIRS);

		for(DWORD i=0;i<ArrFrustums.size();i++)
		{
			InfoRenderSegments* tmpIRS2 = new InfoRenderSegments();
			tmpIRS2->Count = tmpGreen->GetCountSplitsRender();
			tmpIRS2->Arr = new Segment*[tmpGreen->GetCountSplitsRender()];
			tmpIRS2->CountCom = 0;

			ArrFrustums[i].push_back(tmpIRS2);
		}
}

DWORD ManagerGreen::AddFrustum()
{
	UINT id = ArrFrustums.size();
	Array<InfoRenderSegments*> tmpArr;
	ArrFrustums.push_back(tmpArr);
	return id;
}

void ManagerGreen::AllComForCamera(float3* campos)
{
	float4x4 mat = Core::Data::View * Core::Data::Projection;
	float3 sizemapdepth = float3(Core::Data::WidthHeight.x * Core::Data::CoefSizeOC,Core::Data::WidthHeight.y * Core::Data::CoefSizeOC,Core::Data::NearFar.y);
	float tmpdepth;
	int tmpkey;
	float tmpmaxdepth = 0;
	DWORD sizewarrdepth = Core::Data::WidthHeight.x * Core::Data::CoefSizeOC;
	DWORD sizeharrdepth = Core::Data::WidthHeight.y * Core::Data::CoefSizeOC;
	PosBBScreen res;
		for(DWORD i=0;i<Arr.size();i++)
		{
			ArrComForCamera[i]->CountCom = 0;
			Arr[i]->CPUFillingArrIndeces(Core::Data::ObjCamera->ObjFrustum,ArrComForCamera[i]->Arr,&(ArrComForCamera[i]->CountCom),true);
			DWORD tmptmp = ArrComForCamera[i]->CountCom;
				for(DWORD k=0;k<ArrComForCamera[i]->CountCom;k++)
				{
					ArrComForCamera[i]->Arr[k]->BoundVolumeP.GetPosBBScreen(&res,campos,&sizemapdepth,&mat);
					ArrComForCamera[i]->Arr[k]->IsRenderForCamera = false;
						
					tmpdepth = Core::Data::CurrentMaxDepth2;
						if(tmpdepth > res.mindepth*Core::Data::NearFar.y)
						{
								if(res.x < 0 && res.x + res.width > 0)
								{
									res.width = res.width + res.x;
									res.x = 0;
								}

								if(res.y < 0 && res.y + res.height > 0)
								{
									res.height = res.height + res.y;
									res.y = 0;
								}

								if(res.IsIn)
									ArrComForCamera[i]->Arr[k]->IsRenderForCamera = true;
								else if(res.maxdepth > 0.f )
								{
										for(int x=res.x;x<res.width+res.x;x++)
										{
												for(int y=res.y;y<res.height+res.y;y++)
												{
													tmpkey = ((y) * sizewarrdepth) + (x);
													tmpdepth = Core::Data::ArrDepthOC[tmpkey];
																						
														if(tmpdepth >= res.mindepth)
														{
															x=res.width+res.x;
															y=res.height+res.y;
															ArrComForCamera[i]->Arr[k]->IsRenderForCamera = true;
														}
												}
										}
								}
						}
				}
		}
}

void ManagerGreen::AllCom(Core::ControllMoving::Frustum* frustum,DWORD id,bool comgrass)
{
		if(id >= 0 && id < ArrFrustums.size())
		{
				for(DWORD i=0;i<Arr.size();i++)
				{
					ArrFrustums[id][i]->CountCom = 0;
						if((comgrass && Arr[i]->IsGrass) || !(Arr[i]->IsGrass))
							Arr[i]->CPUFillingArrIndeces(frustum,ArrFrustums[id][i]->Arr,&(ArrFrustums[id][i]->CountCom),false);
				}
		}
}

void ManagerGreen::RenderForCamera(DWORD timeDelta)
{

		for(DWORD i=0;i<ArrComForCamera.size();i++)
		{
				if(/*Arr[i]->ArrSegments->IsRenderForCamera*/ArrComForCamera[i]->CountCom > 0)
					Arr[i]->GPURender(ArrComForCamera[i]->Arr,ArrComForCamera[i]->CountCom,0,true,true,0,0,(NumSelModel == i && SkyXEngine::Core::Data::Settings::EditorsIsSelectedGreen ? true : false));
		}
}

void ManagerGreen::AllRender(DWORD id,DWORD timeDelta,bool ismaterial,bool comgrass,int lod,int how_shader,float3* lightpos)
{
		if(id >= 0 && id < ArrFrustums.size())
		{
				for(DWORD i=0;i<Arr.size();i++)
				{
					Segment** tmpsegment = ArrFrustums[id][i]->Arr;
					DWORD tmpcount = ArrFrustums[id][i]->CountCom;

						if(ArrFrustums[id][i]->CountCom)
						{
								if((comgrass && Arr[i]->IsGrass) || !(Arr[i]->IsGrass))
								{
									Arr[i]->GPURender(
										ArrFrustums[id][i]->Arr,
										ArrFrustums[id][i]->CountCom,
										lod,ismaterial,false,how_shader,lightpos);
								}
						}
				}
		}
}

inline DWORD ManagerGreen::GetCount()
{
	return Arr.size();
}

void ManagerGreen::Delete(DWORD id)
{
		if(id >= 0 && id < Arr.size())
		{
			nmem_delete(Arr[id]);
			Arr.erase(id);

			nmem_delete(ArrComForCamera[id]);
			ArrComForCamera.erase(id);

				for(DWORD i=0;i<ArrFrustums.size();i++)
				{
					nmem_delete(ArrFrustums[i][id]);
					ArrFrustums[i].erase(id);
				}
		}
}

void ManagerGreen::Clear()
{
		for(int i=0;i<Arr.size();i++)
		{
			this->Delete(0);
		}
}

DWORD ManagerGreen::GetAllCountObjectGen()
{
	DWORD AllCount = 0;

		for(int i=0;i<Arr.size();i++)
			AllCount += Arr[i]->AllCountGreen;

	return AllCount;
}

DWORD ManagerGreen::GetAllCountSplits()
{
	DWORD AllCount = 0;

		for(int i=0;i<Arr.size();i++)
			AllCount += Arr[i]->SplitsIDs;

	return AllCount;
}

DWORD ManagerGreen::GetAllCountRenderSplits()
{
	DWORD AllCount = 0;

		for(int i=0;i<Arr.size();i++)
			AllCount += Arr[i]->SplitsIDsRender;

	return AllCount;
}

inline DWORD ManagerGreen::GetSelected()
{
	return NumSelModel;
}

inline void ManagerGreen::SetSelected(DWORD id)
{
	NumSelModel = id;
	SkyXEngine::Core::Data::Level::ArrStaticModel->SetSelected(SkyXEngine::Core::Data::Level::ArrStaticModel->GetSelected());
}

inline bool ManagerGreen::GetSelectedMesh()
{
	return IsSelectedMesh;
}

inline void ManagerGreen::SetSelectedMesh(bool is_sel)
{
		/*if(IsSelectedMesh)
			SkyXEngine::Core::Data::EngineID::Tex_SelectedMesh = this->GetTexMask(NumSelModel);*/
	IsSelectedMesh = is_sel;
}

inline DWORD ManagerGreen::GetTexMask(DWORD id)
{
		if(id >= 0 && id < Arr.size())
			return Arr[id]->IDTexMask;
		else
			return -1;
}

inline DWORD ManagerGreen::GetMeshIn(DWORD id)
{
		if(id >= 0 && id < Arr.size())
			return Arr[id]->IDMeshIn;
		else
			return -1;
}

inline void ManagerGreen::GetLodPath(DWORD id,char* text,int type)
{
		if(id >= 0 && id < Arr.size() && type >= 0 && type < 3)
			sprintf(text,"%s",Arr[id]->Paths[type]);
}

inline DWORD ManagerGreen::GetCountObjectInSplit(DWORD id)
{
		if(id >= 0 && id < Arr.size())
			return Arr[id]->CountObjectInSegment;
}

inline DWORD ManagerGreen::IsLods(DWORD id)
{
		if(id >= 0 && id < Arr.size())
		{
				if(!Arr[id]->DataLod[1] && !Arr[id]->DataLod[2])
					return false;
				else
					return true;
		}
	return false;
}

inline DWORD ManagerGreen::GetCountGreen(DWORD id)
{
		if(id >= 0 && id < Arr.size())
			return Arr[id]->AllCountGreen;
}

inline void ManagerGreen::GetPosTrans(DWORD id,GreenDataVertex* data)
{
		if(id >= 0 && id < Arr.size())
		{
			memcpy(data,Arr[id]->AllTrans,sizeof(GreenDataVertex)*Arr[id]->AllCountGreen);
		}
}

};
};
};