

#pragma once

namespace SkyXEngine
{
namespace Graphics
{
namespace ThreeD
{

Material::Material()
{
	PreShaderVS = -1;
	PreShaderPS = -1;
	IsRefraction=false;
	TransVSDataInPS=false;
	TransPSDataInVS=false;
	IsForwardRender = false;
	IDSelShaders = -1;
	PhysicsMaterial = 0;
}

Material::~Material()
{

}

void Material::SetMainTexture(DWORD slot)
{
		if(MainTexture != -1)
			Core::Data::Device->SetTexture(0,Core::Data::LoadedTextures->GetTexture(MainTexture));
		else
			Core::Data::Device->SetTexture(0,0);
}

void Material::Render(float4x4* world,DWORD timeDelta)
{
		//если есть то устанавливаем текстуру материала
		if(MainTexture != -1)
			Core::Data::Device->SetTexture(0,Core::Data::LoadedTextures->GetTexture(MainTexture));
		
		//если нет отражения то отправляем 0
		if(LightParam.TypeReflect == 0)
			Core::Data::Device->SetTexture(12,0);

		//если есть рефаркция, а она идет вторым проходом, то отправляем, иначе посылаем 0
		if(IsRefraction)
			Core::Data::Device->SetTexture(13,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSComLigth));
		else
			Core::Data::Device->SetTexture(13,0);

		if(MicroDetail.Mask != -1)
			Core::Data::Device->SetTexture(1,Core::Data::LoadedTextures->GetTexture(MicroDetail.Mask));

		for(int k=0;k<4;k++)
		{
				if(MicroDetail.ArrDeatail[k] != -1)
					Core::Data::Device->SetTexture(2+k,Core::Data::LoadedTextures->GetTexture(MicroDetail.ArrDeatail[k]));
				else
					Core::Data::Device->SetTexture(2+k,0);

				if(MicroDetail.ArrMicroDiffuse[k] != -1)
					Core::Data::Device->SetTexture(6+k,Core::Data::LoadedTextures->GetTexture(MicroDetail.ArrMicroDiffuse[k]));
				else
					Core::Data::Device->SetTexture(6+k,0);
		}

		//если есть текстура с параметрами освещения и установлено что берем параметры из текстуры, то отправляем текстуру с параметрами
		if(LightParam.ParamTex != -1 && LightParam.IsTextureParam)
		{
			Core::Data::Device->SetTexture(10,Core::Data::LoadedTextures->GetTexture(LightParam.ParamTex));
		}
		//иначе если берем параметры из ... редактора
		else if(!LightParam.IsTextureParam)
		{
			//блокируем текстуру 1х1 которая ест ьпараметры освещения, и запсиываем туда то что настрйоили
			D3DLOCKED_RECT LockedRect;
			IDirect3DTexture9* ParamLightModelTex = Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_ParamLight);
			ParamLightModelTex->LockRect(0,&LockedRect,0,0);
			DWORD *param = (DWORD*)LockedRect.pBits;
			//DWORD param = D3DCOLOR_ARGB(0,0,0,0);
			param[0] = D3DCOLOR_ARGB(DWORD(LightParam.MapSpecularValue*255.f),DWORD(LightParam.RoughnessValue*255.f),DWORD(LightParam.F0Value*255.f),DWORD(LightParam.SoftSpecularValue*255.f));
			ParamLightModelTex->UnlockRect(0);

			Core::Data::Device->SetTexture(10,Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_ParamLight));
		}
		else
			Core::Data::Device->SetTexture(10,Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_NullMaterial));


		if(PreShaderVS != -1)
			Core::Data::ShadersManager->Bind(0,PreShaderVS);

		if(PreShaderPS != -1)
			Core::Data::ShadersManager->Bind(1,PreShaderPS);

		if(VS.IsTransWorld || PS.IsTransWorld || VS.IsTransWorldView || PS.IsTransWorldView || VS.IsTransWorldViewProjection || PS.IsTransWorldViewProjection)
			worldtrans = SMMatrixTranspose(*world);

		if(VS.IsTransWorldView || PS.IsTransWorldView || VS.IsTransWorldViewProjection || PS.IsTransWorldViewProjection)
		{
			D3DXMATRIX tmpView;
			Core::Data::Device->GetTransform(D3DTS_VIEW,&tmpView);
			view = float4x4(tmpView);
			viewtrans = SMMatrixTranspose(view);

			D3DXMATRIX tmpProjection;

			Core::Data::Device->GetTransform(D3DTS_PROJECTION,&tmpProjection);
			proj = float4x4(tmpProjection);
			projtrans = SMMatrixTranspose(proj);
		}
			
		if(VS.IsTransWorld)
			Core::Data::ShadersManager->SetValueRegisterF(0,PreShaderVS,"World",&worldtrans);

		if(PS.IsTransWorld)
			Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"World",&worldtrans);

		if(VS.IsTransView)
			Core::Data::ShadersManager->SetValueRegisterF(0,PreShaderVS,"View",&viewtrans);

		if(PS.IsTransView)
			Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"View",&viewtrans);

		if(VS.IsTransProjection)
			Core::Data::ShadersManager->SetValueRegisterF(0,PreShaderVS,"Projection",&projtrans);

		if(PS.IsTransProjection)
			Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"Projection",&projtrans);


		if(VS.IsTransWorldView || PS.IsTransWorldView)
		{
			float4x4 wv = viewtrans * worldtrans;

				if(VS.IsTransWorldView)
					Core::Data::ShadersManager->SetValueRegisterF(0,PreShaderVS,"WorldView",&wv);

				if(PS.IsTransWorldView)
					Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"WorldView",&wv);
		}

		if(VS.IsTransWorldViewProjection || PS.IsTransWorldViewProjection)
		{
			D3DXMATRIX tmpVP;
			Core::Data::Device->GetTransform(D3DTS_WORLD1,&tmpVP);

			float4x4 wvp = *world * float4x4(tmpVP);
			wvp = SMMatrixTranspose(wvp);

				if(VS.IsTransWorldViewProjection)
					Core::Data::ShadersManager->SetValueRegisterF(0,PreShaderVS,"WorldViewProjection",&wvp);

				if(PS.IsTransWorldViewProjection)
					Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"WorldViewProjection",&wvp);
		}

		if(VS.IsTransPosCam || PS.IsTransPosCam)
		{
				if(VS.IsTransPosCam)
					Core::Data::ShadersManager->SetValueRegisterF(0,PreShaderVS,"PosCam",&Core::Data::ConstCurrentCameraPosition);

				if(PS.IsTransPosCam)
					Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"PosCam",&Core::Data::ConstCurrentCameraPosition);
		}


		if(VS.IsTransUserData)
			Core::Data::ShadersManager->SetValueRegisterF(0,PreShaderVS,"Param",&(VS.Param));

		if(TransVSDataInPS)
			Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"ParamVS",&(VS.Param));

		if(PS.IsTransUserData)
			Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"Param",&(PS.Param));

		if(TransPSDataInVS)
			Core::Data::ShadersManager->SetValueRegisterF(0,PreShaderVS,"ParamPS",&PS.Param);
								
		if(VS.IsTransTimeDelta)
			Core::Data::ShadersManager->SetValueRegisterF(0,PreShaderVS,"TimeDelta",&float2(Core::Data::CountTimeDelta,float(timeDelta) * 0.001f));

		if(PS.IsTransTimeDelta)
			Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"TimeDelta",&float2(Core::Data::CountTimeDelta,float(timeDelta) * 0.001f));

		if(RenderStates.IsCullBack)
			Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		else
			Core::Data::Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		if(RenderStates.IsAlphaTest)
		{
			Core::Data::Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			Core::Data::Device->SetRenderState(D3DRS_ALPHAREF, 1);
			Core::Data::Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		}
		else
			Core::Data::Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
							
		//почти во всех пиксельных шейдерах материалов есть данная NearFar, необходимая дял записи глубины
		if(PreShaderPS != -1)
			Core::Data::ShadersManager->SetValueRegisterF(1,PreShaderPS,"NearFar",&(Core::Data::NearFar));
}

//////////////////////

MaterialManager::MaterialManager()
{
	//СОЗДАЕМ СТАНДАРТНЫЕ МАТЕРИАЛЫ

	Material* tmpMtlDefoltGreen = new Material();

	sprintf(tmpMtlDefoltGreen->Name,"%s","mtrl_default_green_color");
	tmpMtlDefoltGreen->PreShaderVS = Core::Data::ShadersManager->GetID(0,"mtrl_default_green");
	tmpMtlDefoltGreen->PreShaderPS = Core::Data::ShadersManager->GetID(1,"mtrl_default_green_color");
	tmpMtlDefoltGreen->RenderStates.IsAlphaTest = true;
	tmpMtlDefoltGreen->RenderStates.IsCullBack = true;

	tmpMtlDefoltGreen->MainTexture = -1;//Core::Data::LoadedTextures->AddName(name);
	tmpMtlDefoltGreen->VS.IsTransWorldViewProjection = true;
					
	Arr.push_back(tmpMtlDefoltGreen);


	Material* tmpMtlDefoltGreenShadowDirectPSSM = new Material();

	sprintf(tmpMtlDefoltGreenShadowDirectPSSM->Name,"%s","mtrl_default_green_pssm_direct_shadow");
	tmpMtlDefoltGreenShadowDirectPSSM->PreShaderVS = Core::Data::ShadersManager->GetID(0,"mtrl_default_green");
	tmpMtlDefoltGreenShadowDirectPSSM->PreShaderPS = Core::Data::ShadersManager->GetID(1,"mtrl_default_green_pssm_direct_shadow");
	tmpMtlDefoltGreenShadowDirectPSSM->RenderStates.IsAlphaTest = true;
	tmpMtlDefoltGreenShadowDirectPSSM->RenderStates.IsCullBack = true;

	tmpMtlDefoltGreenShadowDirectPSSM->MainTexture = -1;//Core::Data::LoadedTextures->AddName(name);
	tmpMtlDefoltGreenShadowDirectPSSM->VS.IsTransWorldViewProjection = true;
					
	Arr.push_back(tmpMtlDefoltGreenShadowDirectPSSM);



	Material* tmpMtlDefoltGreenShadowCube = new Material();

	sprintf(tmpMtlDefoltGreenShadowCube->Name,"%s","mtrl_default_green_cube_shadow");
	tmpMtlDefoltGreenShadowCube->PreShaderVS = Core::Data::ShadersManager->GetID(0,"mtrl_default_green_cube_shadow");
	tmpMtlDefoltGreenShadowCube->PreShaderPS = Core::Data::ShadersManager->GetID(1,"mtrl_default_green_cube_shadow");
	tmpMtlDefoltGreenShadowCube->RenderStates.IsAlphaTest = true;
	tmpMtlDefoltGreenShadowCube->RenderStates.IsCullBack = true;

	tmpMtlDefoltGreenShadowCube->MainTexture = -1;//Core::Data::LoadedTextures->AddName(name);
	tmpMtlDefoltGreenShadowCube->VS.IsTransWorldViewProjection = true;
					
	Arr.push_back(tmpMtlDefoltGreenShadowCube);
	


	Material* tmpMtlBase = new Material();

	sprintf(tmpMtlBase->Name,"%s","mtrl_base");
	tmpMtlBase->PreShaderVS = Core::Data::ShadersManager->GetID(0,"mtrl_base");
	tmpMtlBase->PreShaderPS = Core::Data::ShadersManager->GetID(1,"mtrl_base");
	tmpMtlBase->VS.IsTransWorldViewProjection = true;
	tmpMtlBase->VS.IsTransWorld = true;
	tmpMtlBase->MainTexture = -1;

	Arr.push_back(tmpMtlBase);



	/*Material* tmpMtlSkinning = new Material();

 sprintf(tmpMtlSkinning->Name, "%s", "mtrl_base_skin");
 tmpMtlSkinning->PreShaderVS = Core::Data::ShadersManager->GetID(0, "mtrl_base_skin");
 tmpMtlSkinning->PreShaderPS = Core::Data::ShadersManager->GetID(1, "mtrl_base");
 tmpMtlSkinning->VS.IsTransWorldViewProjection = true;
 tmpMtlSkinning->VS.IsTransWorld = true;
 tmpMtlSkinning->MainTexture = -1;

 Arr.push_back(tmpMtlSkinning);*/
}

MaterialManager::~MaterialManager()
{
		for(DWORD i=0;i<Arr.size();i++)
		{
			nmem_delete(Arr[i]);
		}
	Arr.clear();
}

DWORD MaterialManager::Load(const char* name,int type)
{
	bool IsLoad = false;
		for(DWORD i=0;i<Arr.size();i++)
		{
				if(strcmp(Arr[i]->Name,name) == 0)
				{
					IsLoad = true;
					return i;
				}
		}

		if(!IsLoad)
		{
			char* ArrRGBA[4] = {"r","g","b","a"};
			

			char tmpNameMtl[256];
			char tmpVS[256];
			char tmpPS[256];

			char tmpMask[256];
			char tmpMicroDiff[4][256];
			char tmpDetail[4][256];

			char tmpParamLigth[256];

			char tmp_path[256];//папка
			char tmp_name[256];//само имя текстыр с расширением

			bool IsTruePath = false;
				//обрезаем имя текстуры и папку
				for(DWORD k=0;k<strlen(name);k++)
				{
						if(name[k] == '_')
						{
							memcpy(tmp_path,name,sizeof(char)*k);
							tmp_path[k] = 0;
							sprintf(tmp_name,"%s",name+k+1);
							IsTruePath = true;
							break;
						}
				}

			char path[1024];
			//char tmp_name[256];

				for(DWORD k=0;k<strlen(name);k++)
				{
						if(name[k] == '.')
						{
							sprintf(tmp_name,"%s",name);
							tmp_name[k] = 0;
							IsTruePath = true;
							break;
						}
				}

			sprintf(path,"%s%s\\%s.mtl",Core::Data::Pathes::Materials,tmp_path,tmp_name);
				if(AddFunc::FileExists(path))
				{
					Material* tmpMtl = new Material();
					Core::Loaders::LoaderConfig* config = new Core::Loaders::LoaderConfig(path);

						if(config->KeyExists(tmp_name,"texture"))
							tmpMtl->MainTexture = Core::Data::LoadedTextures->AddName(config->Get(tmp_name,"texture").c_str());
						else
						{
							tmpMtl->MainTexture = Core::Data::LoadedTextures->AddName(name);
						}

					sprintf(tmpMtl->Name,"%s",tmp_name);

					tmpVS[0] = 0;
						if(config->KeyExists(tmp_name,"vs"))
							sprintf(tmpVS,"%s",config->Get(tmp_name,"vs").c_str());

					tmpPS[0] = 0;
						if(config->KeyExists(tmp_name,"ps"))
							sprintf(tmpPS,"%s",config->Get(tmp_name,"ps").c_str());

					
						if(config->KeyExists(tmp_name,"is_forward"))
							tmpMtl->IsForwardRender = config->Get(tmp_name,"is_forward").ToBool();

						if(tmpMtl->IsForwardRender || strcmp("prop_lampa_g",tmp_name) == 0)
							int sfsdf=0;

						/*if(config->KeyExists(tmp_name,"is_forward"))
							
						else
							tmpMtl->IsForwardRender = false;*/

						if(config->KeyExists(tmp_name,"physmaterial"))
							tmpMtl->PhysicsMaterial = config->Get(tmp_name,"physmaterial").ToInt();
						else
							tmpMtl->PhysicsMaterial = 0;
					
						//Core::InLog("PhysicsMaterial = %d\n",tmpMtl->PhysicsMaterial);

						if(StrValidate(tmpVS))
							tmpMtl->PreShaderVS = Core::Data::ShadersManager->Load(0,tmpVS,"main",true);
						else
							tmpMtl->PreShaderVS = Core::Data::ShadersManager->GetID(0,"mtrl_base");

						if(StrValidate(tmpPS))
							tmpMtl->PreShaderPS = Core::Data::ShadersManager->Load(1,tmpPS,"main",true);
						else
							tmpMtl->PreShaderPS = Core::Data::ShadersManager->GetID(1,"mtrl_base");

					sprintf(tmpMicroDiff[0],"%s",config->Get(tmp_name,"mirco_diff_r").c_str());
						if(tmpMicroDiff[0][0] != '0' && tmpMicroDiff[0][0] != 0)
							tmpMtl->MicroDetail.ArrMicroDiffuse[0] = Core::Data::LoadedTextures->AddName(tmpMicroDiff[0]);
						else
							tmpMtl->MicroDetail.ArrMicroDiffuse[0] = -1;

					sprintf(tmpMicroDiff[1],"%s",config->Get(tmp_name,"mirco_diff_g").c_str());
						if(tmpMicroDiff[1][0] != '0' && tmpMicroDiff[1][0] != 0)
							tmpMtl->MicroDetail.ArrMicroDiffuse[1] = Core::Data::LoadedTextures->AddName(tmpMicroDiff[1]);
						else
							tmpMtl->MicroDetail.ArrMicroDiffuse[1] = -1;

					sprintf(tmpMicroDiff[2],"%s",config->Get(tmp_name,"mirco_diff_b").c_str());
						if(tmpMicroDiff[2][0] != '0' && tmpMicroDiff[2][0] != 0)
							tmpMtl->MicroDetail.ArrMicroDiffuse[2] = Core::Data::LoadedTextures->AddName(tmpMicroDiff[2]);
						else
							tmpMtl->MicroDetail.ArrMicroDiffuse[2] = -1;

					sprintf(tmpMicroDiff[3],"%s",config->Get(tmp_name,"mirco_diff_a").c_str());
						if(tmpMicroDiff[3][0] != '0' && tmpMicroDiff[3][0] != 0)
							tmpMtl->MicroDetail.ArrMicroDiffuse[3] = Core::Data::LoadedTextures->AddName(tmpMicroDiff[3]);
						else
							tmpMtl->MicroDetail.ArrMicroDiffuse[3] = -1;



					sprintf(tmpDetail[0],"%s",config->Get(tmp_name,"detail_r").c_str());
						if(tmpDetail[0][0] != '0' && tmpMicroDiff[0][0] != 0)
							tmpMtl->MicroDetail.ArrDeatail[0] = Core::Data::LoadedTextures->AddName(tmpDetail[0]);
						else
							tmpMtl->MicroDetail.ArrDeatail[0] = -1;

					sprintf(tmpDetail[1],"%s",config->Get(tmp_name,"detail_g").c_str());
						if(tmpDetail[1][0] != '0' && tmpMicroDiff[1][0] != 0)
							tmpMtl->MicroDetail.ArrDeatail[1] = Core::Data::LoadedTextures->AddName(tmpDetail[1]);
						else
							tmpMtl->MicroDetail.ArrDeatail[1] = -1;

					sprintf(tmpDetail[2],"%s",config->Get(tmp_name,"detail_b").c_str());
						if(tmpDetail[2][0] != '0' && tmpMicroDiff[2][0] != 0)
							tmpMtl->MicroDetail.ArrDeatail[2] = Core::Data::LoadedTextures->AddName(tmpDetail[2]);
						else
							tmpMtl->MicroDetail.ArrDeatail[2] = -1;

					sprintf(tmpDetail[3],"%s",config->Get(tmp_name,"detail_a").c_str());
						if(tmpDetail[3][0] != '0' && tmpMicroDiff[3][0] != 0)
							tmpMtl->MicroDetail.ArrDeatail[3] = Core::Data::LoadedTextures->AddName(tmpDetail[3]);
						else
							tmpMtl->MicroDetail.ArrDeatail[3] = -1;


					sprintf(tmpMask,"%s",config->Get(tmp_name,"mask").c_str());
						if(tmpMask[0] != '0' && tmpMask[0] != 0)
							tmpMtl->MicroDetail.Mask = Core::Data::LoadedTextures->AddName(tmpMask);
						else
							tmpMtl->MicroDetail.Mask = -1;


					sprintf(tmpParamLigth,"%s",config->Get(tmp_name,"param_ligth").c_str());
						if(tmpParamLigth[0] != '0' && tmpParamLigth[0] != 0)
							tmpMtl->LightParam.ParamTex = Core::Data::LoadedTextures->AddName(tmpParamLigth);
						else
							tmpMtl->LightParam.ParamTex = -1;

					tmpMtl->IsRefraction = config->Get(tmp_name,"refraction").ToBool();
					//tmpMtl->IsRefraction = true;

					tmpMtl->VS.IsTransWorld = config->Get(tmp_name,"vs_world").ToInt();
					tmpMtl->VS.IsTransView = config->Get(tmp_name,"vs_view").ToInt();
					tmpMtl->VS.IsTransProjection = config->Get(tmp_name,"vs_projection").ToInt();
					tmpMtl->VS.IsTransWorldView = config->Get(tmp_name,"vs_worldview").ToInt();
					tmpMtl->VS.IsTransWorldViewProjection = config->Get(tmp_name,"vs_worldviewprojection").ToInt();
					tmpMtl->VS.IsTransPosCam = config->Get(tmp_name,"vs_poscam").ToInt();
					tmpMtl->VS.IsTransTimeDelta = config->Get(tmp_name,"vs_timedelta").ToInt();
					tmpMtl->VS.IsTransUserData = config->Get(tmp_name,"vs_userdata").ToInt();
					tmpMtl->VS.Param.x = config->Get(tmp_name,"vs_userdata_value_x").ToDouble();
					tmpMtl->VS.Param.y = config->Get(tmp_name,"vs_userdata_value_y").ToDouble();
					tmpMtl->VS.Param.z = config->Get(tmp_name,"vs_userdata_value_z").ToDouble();
					tmpMtl->VS.Param.w = config->Get(tmp_name,"vs_userdata_value_w").ToDouble();
					tmpMtl->TransVSDataInPS = config->Get(tmp_name,"vs_userdata_trans_in_ps").ToBool();

					tmpMtl->PS.IsTransWorld = config->Get(tmp_name,"ps_world").ToInt();
					tmpMtl->PS.IsTransView = config->Get(tmp_name,"ps_view").ToInt();
					tmpMtl->PS.IsTransProjection = config->Get(tmp_name,"ps_projection").ToInt();
					tmpMtl->PS.IsTransWorldView = config->Get(tmp_name,"ps_worldview").ToInt();
					tmpMtl->PS.IsTransWorldViewProjection = config->Get(tmp_name,"ps_worldviewprojection").ToInt();
					tmpMtl->PS.IsTransPosCam = config->Get(tmp_name,"ps_poscam").ToInt();
					tmpMtl->PS.IsTransTimeDelta = config->Get(tmp_name,"ps_timedelta").ToInt();
					tmpMtl->PS.IsTransUserData = config->Get(tmp_name,"ps_userdata").ToInt();
					tmpMtl->PS.Param.x = config->Get(tmp_name,"ps_userdata_value_x").ToDouble();
					tmpMtl->PS.Param.y = config->Get(tmp_name,"ps_userdata_value_y").ToDouble();
					tmpMtl->PS.Param.z = config->Get(tmp_name,"ps_userdata_value_z").ToDouble();
					tmpMtl->PS.Param.w = config->Get(tmp_name,"ps_userdata_value_w").ToDouble();
					tmpMtl->TransPSDataInVS = config->Get(tmp_name,"ps_userdata_trans_in_vs").ToBool();
					

					tmpMtl->LightParam.TypeReflect = config->Get(tmp_name,"type_reflect").ToInt();

					tmpMtl->RenderStates.IsAlphaTest = config->Get(tmp_name,"alpha_test").ToBool();
					tmpMtl->RenderStates.IsCullBack = config->Get(tmp_name,"cullmode").ToBool();

					config->Release();
					Arr.push_back(tmpMtl);
					return Arr.size()-1;
					//Data::ArrMaterial->Add(tmpMtl);
				}
				else
				{
					//если такого материала не существует, то мы должны были задать примерный тип материала
					Material* tmpMtl = new Material();

						//обычная геометрия
						if(type == 0)
						{
							tmpMtl->PreShaderVS = Core::Data::ShadersManager->GetID(0,"mtrl_base");
							tmpMtl->PreShaderPS = Core::Data::ShadersManager->GetID(1,"mtrl_base");
							tmpMtl->VS.IsTransWorld = true;
						}
						//деревья
						else if(type == 1)
						{
							tmpMtl->PreShaderVS = Core::Data::ShadersManager->GetID(0,"mtrl_base_green_tree");
							tmpMtl->PreShaderPS = Core::Data::ShadersManager->GetID(1,"mtrl_base_green");
							//tmpMtl->RenderStates.IsAlphaTest = true;
						}
						//трава
						else if(type == 2)
						{
							tmpMtl->PreShaderVS = Core::Data::ShadersManager->GetID(0,"mtrl_base_green_grass");
							tmpMtl->PreShaderPS = Core::Data::ShadersManager->GetID(1,"mtrl_base_green");
							//tmpMtl->RenderStates.IsAlphaTest = true;
						}

						else if(type == 3)
						{
							tmpMtl->PreShaderVS = Core::Data::ShadersManager->GetID(0, "mtrl_base_skin");
							tmpMtl->PreShaderPS = Core::Data::ShadersManager->GetID(1, "mtrl_base");
							tmpMtl->VS.IsTransWorld = true;
						}

					tmpMtl->MainTexture = Core::Data::LoadedTextures->AddName(name);
					tmpMtl->VS.IsTransWorldViewProjection = true;

					//char path[1024];
					char tmp_name[256];

						for(DWORD k=0;k<strlen(name);k++)
						{
								if(name[k] == '.')
								{
									sprintf(tmp_name,"%s",name);
									tmp_name[k] = 0;
									IsTruePath = true;
									break;
								}
						}

					sprintf(tmpMtl->Name,"%s",tmp_name);
					
					Arr.push_back(tmpMtl);
					return Arr.size()-1;
				}
		}
}

bool MaterialManager::IsAlpha(DWORD id)
{
		if(id > 0 && id < Arr.size() && Arr[id])
			return Arr[id]->IsRefraction;
		else
			return false;
}

bool MaterialManager::IsForward(DWORD id)
{
		if(id > 0 && id < Arr.size() && Arr[id])
			return Arr[id]->IsForwardRender;
		else
			return false;
}

void MaterialManager::Save()
{
	char tmp_path[256];//папка
	char tmp_name[256];//само имя текстыр с расширением
	bool IsTruePath = false;
	char tmppath[1024];
	char* ArrRGBA[4] = {"r","g","b","a"};

	//fprintf(file,"%s","[level]\n");

		for(int i=4;i<Arr.size();i++)
		{
			SkyXEngine::Graphics::ThreeD::Material* mtrl = Arr[i];
			IsTruePath = false;
				//обрезаем имя текстуры и папку
				for(DWORD k=0;k<strlen(mtrl->Name);k++)
				{
						if(mtrl->Name[k] == '_')
						{
							memcpy(tmp_path,mtrl->Name,sizeof(char)*k);
							tmp_path[k] = 0;
							sprintf(tmp_name,"%s",mtrl->Name+k+1);
							IsTruePath = true;
							break;
						}
				}

				for(DWORD k=0;k<strlen(mtrl->Name);k++)
				{
						if(mtrl->Name[k] == '.')
						{
							sprintf(tmp_name,"%s",mtrl->Name);
							tmp_name[k] = 0;
							IsTruePath = true;
							break;
						}
				}

			tmppath[0] = 0;
			mkdir(SkyXEngine::Core::Data::Pathes::Materials);
			sprintf(tmppath,"%s%s\\",SkyXEngine::Core::Data::Pathes::Materials,tmp_path);
			mkdir(tmppath);
			sprintf(tmppath,"%s%s.mtl",tmppath,mtrl->Name);
			FILE* file = 0;
			file = fopen(tmppath,"w");

			fprintf(file,"[%s]\n",mtrl->Name);

			char tmpPathVS[1024];
			char tmpPathPS[1024];

			char tmpPathVSName[1024];
			char tmpPathPSName[1024];

			char tmpPathMaskName[1024];

			SkyXEngine::Core::Data::ShadersManager->GetPath(0,mtrl->PreShaderVS,tmpPathVSName);
			SkyXEngine::Core::Data::ShadersManager->GetPath(1,mtrl->PreShaderPS,tmpPathPSName);

			fprintf(file,"%s%d%s","physmaterial = ",mtrl->PhysicsMaterial,"\n");

			fprintf(file,"%s%s%s","vs = ",tmpPathVSName,"\n");
			fprintf(file,"%s%s%s","ps = ",tmpPathPSName,"\n");

			fprintf(file,"%s%d%s","cullmode = ",mtrl->RenderStates.IsCullBack,"\n");
			fprintf(file,"%s%d%s","alpha_test = ",mtrl->RenderStates.IsAlphaTest,"\n");

			char namebasetex[256];
			namebasetex[0] = '0';
			namebasetex[1] = '\0';
			SkyXEngine::Core::Data::LoadedTextures->GetName(mtrl->MainTexture,namebasetex);
			fprintf(file,"%s%s%s","texture = ",namebasetex,"\n");

				if(mtrl->MicroDetail.Mask == -1)
				{
					tmpPathMaskName[0] = '0';
					tmpPathMaskName[1] = 0;
				}
				else
					SkyXEngine::Core::Data::LoadedTextures->GetName(mtrl->MicroDetail.Mask,tmpPathMaskName);

			fprintf(file,"%s%s%s","mask = ",tmpPathMaskName,"\n");

				for(int k=0;k<4;k++)
				{
						if(mtrl->MicroDetail.ArrMicroDiffuse[k] == -1)
						{
							tmpPathMaskName[0] = '0';
							tmpPathMaskName[1] = 0;
						}
						else
							SkyXEngine::Core::Data::LoadedTextures->GetName(mtrl->MicroDetail.ArrMicroDiffuse[k],tmpPathMaskName);

					fprintf(file,"mirco_diff_%s = %s\n",ArrRGBA[k],tmpPathMaskName);
				}

				for(int k=0;k<4;k++)
				{
						if(mtrl->MicroDetail.ArrDeatail[k] == -1)
						{
							tmpPathMaskName[0] = '0';
							tmpPathMaskName[1] = 0;
						}
						else
							SkyXEngine::Core::Data::LoadedTextures->GetName(mtrl->MicroDetail.ArrDeatail[k],tmpPathMaskName);

					fprintf(file,"detail_%s = %s\n",ArrRGBA[k],tmpPathMaskName);
				}

				if(mtrl->LightParam.ParamTex == -1)
				{
					tmpPathMaskName[0] = '0';
					tmpPathMaskName[1] = 0;
				}
				else
					SkyXEngine::Core::Data::LoadedTextures->GetName(mtrl->LightParam.ParamTex,tmpPathMaskName);

			fprintf(file,"param_ligth = %s\n",tmpPathMaskName);

			fprintf(file,"refraction = %d\n",mtrl->IsRefraction);

			fprintf(file,"vs_world = %d\n",mtrl->VS.IsTransWorld);
			fprintf(file,"vs_view = %d\n",mtrl->VS.IsTransView);
			fprintf(file,"vs_projection = %d\n",mtrl->VS.IsTransProjection);
			fprintf(file,"vs_worldview = %d\n",mtrl->VS.IsTransWorldView);
			fprintf(file,"vs_worldviewprojection = %d\n",mtrl->VS.IsTransWorldViewProjection);

			fprintf(file,"vs_poscam = %d\n",mtrl->VS.IsTransPosCam);
			fprintf(file,"vs_timedelta = %d\n",mtrl->VS.IsTransTimeDelta);
			fprintf(file,"vs_userdata = %d\n",mtrl->VS.IsTransUserData);
			fprintf(file,"vs_userdata_value_x = %f\nvs_userdata_value_y = %f\nvs_userdata_value_z = %f\nvs_userdata_value_w = %f\n",mtrl->VS.Param.x,mtrl->VS.Param.y,mtrl->VS.Param.z,mtrl->VS.Param.w);
			fprintf(file,"vs_userdata_trans_in_ps = %d\n",mtrl->TransVSDataInPS);

			fprintf(file,"ps_world = %d\n",mtrl->PS.IsTransWorld);
			fprintf(file,"ps_view = %d\n",mtrl->PS.IsTransView);
			fprintf(file,"ps_projection = %d\n",mtrl->PS.IsTransProjection);
			fprintf(file,"ps_worldview = %d\n",mtrl->PS.IsTransWorldView);
			fprintf(file,"ps_worldviewprojection = %d\n",mtrl->PS.IsTransWorldViewProjection);

			fprintf(file,"ps_poscam = %d\n",mtrl->PS.IsTransPosCam);
			fprintf(file,"ps_timedelta = %d\n",mtrl->PS.IsTransTimeDelta);
			fprintf(file,"ps_userdata = %d\n",mtrl->PS.IsTransUserData);
			fprintf(file,"ps_userdata_value_x = %f\nps_userdata_value_y = %f\nps_userdata_value_z = %f\nps_userdata_value_w = %f\n",mtrl->PS.Param.x,mtrl->PS.Param.y,mtrl->PS.Param.z,mtrl->PS.Param.w);
			fprintf(file,"ps_userdata_trans_in_vs = %d\n",mtrl->TransPSDataInVS);


			fprintf(file,"type_reflect = %d\n",mtrl->LightParam.TypeReflect);

			fprintf(file,"\n");
			fclose(file);
		}
}

void MaterialManager::SetMainTexture(DWORD slot,DWORD id)
{
		if(id >= 0 && id < Arr.size() && Arr[id])
			Arr[id]->SetMainTexture(slot);
		else
		{
				if(id < 0 || id >= Arr.size())
					Core::InError("!!! selected id [%d] for material is failed",id);
				else if(!Arr[id])
					Core::InError("!!! selected material is failed, id [%d]",id);
		}
}

void MaterialManager::Render(DWORD id,float4x4* world,DWORD timeDelta)
{
		if(id >= 0 && id < Arr.size() && Arr[id])
			Arr[id]->Render(world,timeDelta);
		else
		{
				if(id < 0 || id >= Arr.size())
					Core::InError("!!! selected id [%d] for material is failed",id);
				else if(!Arr[id])
					Core::InError("!!! selected material is failed, id [%d]",id);
		}
}

Material* MaterialManager::GetMaterial(DWORD id)
{
		if(id >= 0 && id < Arr.size() && Arr[id])
			return Arr[id];
		else
		{
				if(id < 0 || id >= Arr.size())
					Core::InError("!!! selected id [%d] for material is failed",id);
				else if(!Arr[id])
					Core::InError("!!! selected material is failed, id [%d]",id);
		}
}

DWORD MaterialManager::GetID(const char* name)
{
		for(DWORD i=0;i<Arr.size();i++)
		{
				if(strcmp(Arr[i]->Name,name) == 0)
					return i;
		}
	return -1;
}

//////////////////////////

RenderReflection::RenderReflection()
{
	IsComNow = true;
	SurfaceReflect = 0;
	TextureReflect = 0;
	TextureCubeReflect = 0;
	SurfaceZBuffer = 0;
	BackBuffer = 0;
	LastSurfaceZBuffer = 0;

	CubeReflectSurface[0] = 0;
	CubeReflectSurface[1] = 0;
	CubeReflectSurface[2] = 0;
	CubeReflectSurface[3] = 0;
	CubeReflectSurface[4] = 0;
	CubeReflectSurface[5] = 0;
}

RenderReflection::~RenderReflection()
{
	nmem_delete(ReflectFrustum);

	ndx_release(BackBuffer);
	ndx_release(SurfaceZBuffer);
	ndx_release(LastSurfaceZBuffer);
	ndx_release(TextureReflect);
	ndx_release(SurfaceReflect);

		for(int i=0;i<6;i++)
			ndx_release(CubeReflectSurface[i]);

	ndx_release(TextureCubeReflect);
}

void RenderReflection::OnLostDevice()
{
	ndx_release(TextureReflect);
	ndx_release(TextureCubeReflect);
	ndx_release(SurfaceZBuffer);

	ndx_release(SurfaceReflect);

	ndx_release(CubeReflectSurface[0]);
	ndx_release(CubeReflectSurface[1]);
	ndx_release(CubeReflectSurface[2]);
	ndx_release(CubeReflectSurface[3]);
	ndx_release(CubeReflectSurface[4]);
	ndx_release(CubeReflectSurface[5]);
}

void RenderReflection::OnResetDevice()
{
		if(TypeRef == 0)
		{
			D3DXCreateTexture(Core::Data::Device,SkyXEngine::Core::Data::SizeTexReflect.x,SkyXEngine::Core::Data::SizeTexReflect.y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&TextureReflect);
			Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexReflect.x,Core::Data::SizeTexReflect.y,D3DFMT_D24S8,D3DMULTISAMPLE_NONE,0,TRUE,&SurfaceZBuffer,NULL);
		}
		else if(TypeRef == 1)
		{
			Core::Data::Device->CreateCubeTexture(Core::Data::SizeTexReflect.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureCubeReflect, NULL);
			Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexReflect.x,Core::Data::SizeTexReflect.x,D3DFMT_D24S8,D3DMULTISAMPLE_NONE,0,TRUE,&SurfaceZBuffer,NULL);
		}
}

void RenderReflection::Init(int howref)
{
	TypeRef = howref;
		if(TypeRef == 0)
		{
			D3DXCreateTexture(Core::Data::Device,Core::Data::SizeTexReflect.x,Core::Data::SizeTexReflect.y,1,D3DUSAGE_RENDERTARGET,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&TextureReflect);
			Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexReflect.x,Core::Data::SizeTexReflect.y,D3DFMT_D24S8,D3DMULTISAMPLE_NONE,0,TRUE,&SurfaceZBuffer,NULL);
		}
		else if(TypeRef == 1)
		{
			Core::Data::Device->CreateCubeTexture(Core::Data::SizeTexReflect.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &TextureCubeReflect, NULL);
			Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexReflect.x,Core::Data::SizeTexReflect.x,D3DFMT_D24S8,D3DMULTISAMPLE_NONE,0,TRUE,&SurfaceZBuffer,NULL);
		}

	ReflectFrustum = new Core::ControllMoving::Frustum();

	CubeReflectSurface[0] = 0;
	CubeReflectSurface[1] = 0;
	CubeReflectSurface[2] = 0;
	CubeReflectSurface[3] = 0;
	CubeReflectSurface[4] = 0;
	CubeReflectSurface[5] = 0;
}

void RenderReflection::PreRenderRefPlane(float4x4* world)
{
	D3DXMATRIX MatrixView;
	PositionReflect = SMVector3Transform(Position,*world);
	//PositionReflect = SkyXEngine::Core::Data::ConstCurrentCameraPosition;
	float4x4 viewmat;
	Core::Data::ObjCamera->GetViewMatrix(&viewmat);

	float4x4 tmpworld;
	tmpworld = *world;
	tmpworld._41 = tmpworld._42 = tmpworld._43 = tmpworld._14 = tmpworld._24 = tmpworld._34 = 0;
	tmpworld._44 = 1.f;
	
	D3DXMATRIX matReflect,matView;
	D3DXPLANE tmpplane;
	D3DXPlaneTransform(&tmpplane,&Plane,&(tmpworld.operator D3DXMATRIX()));
	D3DXMatrixReflect(&matReflect, &tmpplane);

	D3DXMatrixMultiply(&MatrixView, &matReflect, &(viewmat.operator D3DXMATRIX()) );
	
	MatrixView._12 = -MatrixView._12;
	MatrixView._22 = -MatrixView._22;
	MatrixView._32 = -MatrixView._32;
	MatrixView._42 = -MatrixView._42;

	Core::Data::Device->GetTransform(D3DTS_VIEW, &OldMatView);
	Core::Data::Device->GetTransform(D3DTS_PROJECTION, &OldMatProj);
	Core::Data::Device->GetTransform(D3DTS_WORLD1,&OldMatViewProj);

	float4x4 tmpproj = SMMatrixPerspectiveFovLH(Core::Data::Settings::ProjFov,Core::Data::Settings::ProjRatio,0.25,100);
	Core::Data::Device->SetTransform(D3DTS_PROJECTION, &(tmpproj.operator D3DXMATRIX()));
	Core::Data::Device->SetTransform(D3DTS_VIEW, &MatrixView);

	Core::Data::Device->SetTransform(D3DTS_WORLD1,&((float4x4(MatrixView) * tmpproj).operator D3DXMATRIX()));

	ReflectFrustum->Update(&float4x4(MatrixView),&(tmpproj));

	Core::Data::Device->GetRenderTarget(0, &BackBuffer);

	ndx_release(SurfaceReflect);
	TextureReflect->GetSurfaceLevel(0, &SurfaceReflect);
	Core::Data::Device->SetRenderTarget(0, SurfaceReflect);
	Core::Data::Device->GetDepthStencilSurface(&LastSurfaceZBuffer);
	Core::Data::Device->SetDepthStencilSurface(SurfaceZBuffer);
	Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0L);

	SkyXEngine::Core::Data::MaterialsManager->Render(SkyXEngine::Core::Data::EngineID::ID_MtrlBase,&SMMatrixIdentity(),0);
}

void RenderReflection::PostRenderRefPlane()
{
	Core::Data::Device->SetTransform(D3DTS_VIEW, &OldMatView);
	Core::Data::Device->SetTransform(D3DTS_PROJECTION, &OldMatProj);
	Core::Data::Device->SetTransform(D3DTS_WORLD1,&OldMatViewProj);
	Core::Data::Device->SetRenderTarget(0, BackBuffer);
	Core::Data::Device->SetDepthStencilSurface(LastSurfaceZBuffer);

		if(GetAsyncKeyState(VK_NUMPAD9))
		{
			char tmpstr[256];
			sprintf(tmpstr,"C:\\1\\reflectionreflection.png");
			D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_JPG, SurfaceReflect, NULL,0);
		}

	ndx_release(BackBuffer);
	ndx_release(LastSurfaceZBuffer);
}

inline IDirect3DTexture9* RenderReflection::GetRefPlaneTex()
{
	return TextureReflect;
}

////////////////////////

void RenderReflection::BeginRenderRefCube()
{
	Core::Data::Device->GetTransform(D3DTS_VIEW, &OldMatView);
	Core::Data::Device->GetTransform(D3DTS_PROJECTION, &OldMatProj);
	Core::Data::Device->GetTransform(D3DTS_WORLD1,&OldMatViewProj);
	Core::Data::Device->GetRenderTarget(0, &BackBuffer);
	Core::Data::Device->GetDepthStencilSurface(&LastSurfaceZBuffer);
	Core::Data::Device->SetDepthStencilSurface(SurfaceZBuffer);
}

void RenderReflection::PreRenderRefCube(int cube,float4x4* world)
{
	float3 tmpmax = SMVector3Transform(PosMax,*world);
	float3 tmpmin = SMVector3Transform(PosMin,*world);
	float znear = 0.25f;
		if(cube == 0 || cube == 1)
			znear = (PosMax.x - PosMin.x)*0.5f;
		else if(cube == 2 || cube == 3)
			znear = (PosMax.y - PosMin.y)*0.5f;
		else if(cube == 4 || cube == 5)
			znear = (PosMax.z - PosMin.z)*0.5f;

	D3DXMATRIX tmpproj;
	D3DXMatrixPerspectiveFovLH(&tmpproj,D3DX_PI / 2.f,1,znear+0.25,100);
	Core::Data::Device->SetTransform(D3DTS_PROJECTION, &tmpproj);

	/*D3DXMATRIXA16 mViewDir;
	float4x4 tmpview;
	Core::Data::ObjCamera->GetViewMatrix(&tmpview);*/
	/*tmpview._14 = tmpview._12 = tmpview._13 = 0.0f;
	tmpview._21 = tmpview._24 = tmpview._23 = 0.0f;
	tmpview._31 = tmpview._32 = tmpview._34 = 0.0f;
	tmpview._41 = tmpview._42 = tmpview._43 = 0.0f;*/

	PositionReflect = SMVector3Transform(Position,*world);
	//float3 tmppos = SMVector3Transform(Position,*world);
	MatrixView = SMMatrixLookAtLH(
		PositionReflect,
		//((Core::Data::OrientedCube[cube]*Core::Data::NearFar.y)+Position),
		((Core::Data::OrientedCube[cube]+PositionReflect)),
		Core::Data::UpVectorsCube[cube]);
	//MatrixView = tmpview * MatrixView;



	/*float4x4 viewmat;
	Core::Data::ObjCamera->GetViewMatrix(&viewmat);

	D3DXPLANE tmpplane2;
	D3DXPlaneFromPointNormal(&tmpplane2,&Position.operator D3DXVECTOR3(),&Core::Data::OrientedCube[cube].operator D3DXVECTOR3());
	
	D3DXMATRIX matReflect,matView,MatrixView2;
	D3DXPLANE tmpplane;
	D3DXPlaneTransform(&tmpplane,&tmpplane2,&(world->operator D3DXMATRIX()));
	D3DXMatrixReflect(&matReflect, &tmpplane);

	D3DXMatrixMultiply(&MatrixView2, &matReflect, &(viewmat.operator D3DXMATRIX()) );*/


	Core::Data::Device->SetTransform(D3DTS_VIEW, &MatrixView.operator D3DXMATRIX());

	Core::Data::Device->SetTransform(D3DTS_WORLD1,&((MatrixView * tmpproj).operator D3DXMATRIX()));

	ReflectFrustum->Update(&float4x4(MatrixView),&float4x4(tmpproj));

	ndx_release(CubeReflectSurface[cube]);
	
    TextureCubeReflect->GetCubeMapSurface( ( D3DCUBEMAP_FACES ) cube, 0, &CubeReflectSurface[cube] );
	Core::Data::Device->SetRenderTarget( 0, CubeReflectSurface[cube] );
	
	Core::Data::Device->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0,0,0,0), 1, 0L );
	//SkyXEngine::Core::Data::MaterialsManager->Render(SkyXEngine::Core::Data::EngineID::ID_MtrlBase,&SMMatrixIdentity(),0);
}

void RenderReflection::PostRenderRefCube(int cube)
{
		/*if(GetAsyncKeyState(VK_NUMPAD5))
		{
			char tmpstr[1024];
			sprintf(tmpstr,"C:\\1\\reflectioncube_%d.bmp",cube);
			D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_PNG, CubeReflectSurface[cube], NULL,0);
		}*/
}

void RenderReflection::EndRenderRefCube()
{
	Core::Data::Device->SetTransform(D3DTS_VIEW, &OldMatView);
	Core::Data::Device->SetTransform(D3DTS_PROJECTION, &OldMatProj);
	Core::Data::Device->SetTransform(D3DTS_WORLD1,&OldMatViewProj);
	Core::Data::Device->SetRenderTarget(0, BackBuffer);
	Core::Data::Device->SetDepthStencilSurface(LastSurfaceZBuffer);
	ndx_release(LastSurfaceZBuffer);
	ndx_release(BackBuffer);
	
}

inline IDirect3DCubeTexture9* RenderReflection::GetRefCubeTex()
{
	return TextureCubeReflect;
}

}
};
};