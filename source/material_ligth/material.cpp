
#include <material_ligth\\material.h>

Materials::Materials()
{

}

Materials::~Materials()
{

}

Materials::Material::Material()
{

}

Materials::Material::~Material()
{

}

Materials::Material::MaterialDataShader::MaterialDataShader()
{
	IsTransWorld = false;
	IsTransView = false;
	IsTransProjection = false;
	IsTransWorldView = false;
	IsTransWorldViewProjection = false;
	IsTransPosCam = false;
	IsTransTimeDelta = false;
	IsTransUserData = false;
	Param = float4(0, 0, 0, 0);
};

Materials::Material::MaterialDataShader::~MaterialDataShader()
{

};

Materials::Material::MaterialLightParam::MaterialLightParam()
{
	ParamTex = -1;
	ParamTex2 = -1;

	RoughnessValue = 0.f;
	F0Value = 0.f;
	SoftSpecularValue = 0.f;
	MapSpecularValue = 0.f;

	TypeReflect = 0;
	IDParamLight = -1;
	IsTextureParam = true;
}

Materials::Material::MaterialLightParam::~MaterialLightParam()
{

}

Materials::Material::MaterialRenderStates::MaterialRenderStates()
{
	IsCullBack = false;
	IsAlphaTest = false;
}

Materials::Material::MaterialRenderStates::~MaterialRenderStates()
{

}

Materials::Material::MaterialMaskPM::MaterialMaskPM()
{
	Mask = -1;
	ArrDeatail[0] = -1;
	ArrDeatail[1] = -1;
	ArrDeatail[2] = -1;
	ArrDeatail[3] = -1;

	ArrMicroDiffuse[0] = -1;
	ArrMicroDiffuse[1] = -1;
	ArrMicroDiffuse[2] = -1;
	ArrMicroDiffuse[3] = -1;
}

Materials::Material::MaterialMaskPM::~MaterialMaskPM()
{

}

/////

long Materials::Load(const char* name, int type)
{
	bool IsLoad = false;
	for (long i = 0; i<ArrMaterials.size(); i++)
	{
		if (strcmp(ArrMaterials[i]->Name, name) == 0)
		{
			IsLoad = true;
			return i;
		}
	}

	if (!IsLoad)
	{
		char* ArrRGBA[4] = { "r", "g", "b", "a" };


		char tmpNameMtl[256];
		char tmpVS[256];
		char tmpPS[256];

		char tmpMask[256];
		char tmpMicroDiff[4][256];
		char tmpDetail[4][256];

		char tmpParamLigth[256];

		char tmp_path[256];//папка
		char tmp_name[256];//само им¤ текстыр с расширением

		bool IsTruePath = false;
		//обрезаем им¤ текстуры и папку
		for (long k = 0; k< strlen(name); k++)
		{
			if (name[k] == '_')
			{
				memcpy(tmp_path, name, sizeof(char)*k);
				tmp_path[k] = 0;
				sprintf(tmp_name, "%s", name + k + 1);
				IsTruePath = true;
				break;
			}
		}

		char path[1024];
		//char tmp_name[256];

		for (DWORD k = 0; k<strlen(name); k++)
		{
			if (name[k] == '.')
			{
				sprintf(tmp_name, "%s", name);
				tmp_name[k] = 0;
				IsTruePath = true;
				break;
			}
		}

		sprintf(path, "%s%s\\%s.mtl", MLSet::StdPathMaterial, tmp_path, tmp_name);
		if (Core_0FileExists(path))
		{
			Material* tmpMtl = new Material();
			ISXLConfig* config = Core_OpLConfig(path);

			if (config->KeyExists(tmp_name, "texture"))
				tmpMtl->MainTexture = SGCore_LoadTexAddName(config->GetKey(tmp_name, "texture"));
			else
			{
				tmpMtl->MainTexture = SGCore_LoadTexAddName(name);
			}

			sprintf(tmpMtl->Name, "%s", tmp_name);

			tmpVS[0] = 0;
			if (config->KeyExists(tmp_name, "vs"))
				sprintf(tmpVS, "%s", config->GetKey(tmp_name, "vs"));

			tmpPS[0] = 0;
			if (config->KeyExists(tmp_name, "ps"))
				sprintf(tmpPS, "%s", config->GetKey(tmp_name, "ps"));


			if (config->KeyExists(tmp_name, "is_forward"))
				tmpMtl->IsForwardRender = String(config->GetKey(tmp_name, "is_forward")).ToBool();

			if (tmpMtl->IsForwardRender || strcmp("prop_lampa_g", tmp_name) == 0)
				int sfsdf = 0;

			/*if(config->KeyExists(tmp_name,"is_forward"))

			else
			tmpMtl->IsForwardRender = false;*/

			if (config->KeyExists(tmp_name, "physmaterial"))
				tmpMtl->PhysicsMaterial = String(config->GetKey(tmp_name, "physmaterial")).ToInt();
			else
				tmpMtl->PhysicsMaterial = 0;

			//Core::InLog("PhysicsMaterial = %d\n",tmpMtl->PhysicsMaterial);

			if (def_str_validate(tmpVS))
				tmpMtl->PreShaderVS = SGCore_ShaderLoad(0, tmpVS, "main", true);
			else
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrl_base");

			if (def_str_validate(tmpPS))
				tmpMtl->PreShaderPS = SGCore_ShaderLoad(1, tmpPS, "main", true);
			else
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrl_base");

			sprintf(tmpMicroDiff[0], "%s", config->GetKey(tmp_name, "mirco_diff_r"));

			if (tmpMicroDiff[0][0] != '0' && tmpMicroDiff[0][0] != 0)
				tmpMtl->MicroDetail.ArrMicroDiffuse[0] = SGCore_LoadTexAddName(tmpMicroDiff[0]);
			else
				tmpMtl->MicroDetail.ArrMicroDiffuse[0] = -1;

			sprintf(tmpMicroDiff[1], "%s", config->GetKey(tmp_name, "mirco_diff_g"));
			if (tmpMicroDiff[1][0] != '0' && tmpMicroDiff[1][0] != 0)
				tmpMtl->MicroDetail.ArrMicroDiffuse[1] = SGCore_LoadTexAddName(tmpMicroDiff[1]);
			else
				tmpMtl->MicroDetail.ArrMicroDiffuse[1] = -1;

			sprintf(tmpMicroDiff[2], "%s", config->GetKey(tmp_name, "mirco_diff_b"));
			if (tmpMicroDiff[2][0] != '0' && tmpMicroDiff[2][0] != 0)
				tmpMtl->MicroDetail.ArrMicroDiffuse[2] = SGCore_LoadTexAddName(tmpMicroDiff[2]);
			else
				tmpMtl->MicroDetail.ArrMicroDiffuse[2] = -1;

			sprintf(tmpMicroDiff[3], "%s", config->GetKey(tmp_name, "mirco_diff_a"));
			if (tmpMicroDiff[3][0] != '0' && tmpMicroDiff[3][0] != 0)
				tmpMtl->MicroDetail.ArrMicroDiffuse[3] = SGCore_LoadTexAddName(tmpMicroDiff[3]);
			else
				tmpMtl->MicroDetail.ArrMicroDiffuse[3] = -1;



			sprintf(tmpDetail[0], "%s", config->GetKey(tmp_name, "detail_r"));
			if (tmpDetail[0][0] != '0' && tmpMicroDiff[0][0] != 0)
				tmpMtl->MicroDetail.ArrDeatail[0] = SGCore_LoadTexAddName(tmpDetail[0]);
			else
				tmpMtl->MicroDetail.ArrDeatail[0] = -1;

			sprintf(tmpDetail[1], "%s", config->GetKey(tmp_name, "detail_g"));
			if (tmpDetail[1][0] != '0' && tmpMicroDiff[1][0] != 0)
				tmpMtl->MicroDetail.ArrDeatail[1] = SGCore_LoadTexAddName(tmpDetail[1]);
			else
				tmpMtl->MicroDetail.ArrDeatail[1] = -1;

			sprintf(tmpDetail[2], "%s", config->GetKey(tmp_name, "detail_b"));
			if (tmpDetail[2][0] != '0' && tmpMicroDiff[2][0] != 0)
				tmpMtl->MicroDetail.ArrDeatail[2] = SGCore_LoadTexAddName(tmpDetail[2]);
			else
				tmpMtl->MicroDetail.ArrDeatail[2] = -1;

			sprintf(tmpDetail[3], "%s", config->GetKey(tmp_name, "detail_a"));
			if (tmpDetail[3][0] != '0' && tmpMicroDiff[3][0] != 0)
				tmpMtl->MicroDetail.ArrDeatail[3] = SGCore_LoadTexAddName(tmpDetail[3]);
			else
				tmpMtl->MicroDetail.ArrDeatail[3] = -1;


			sprintf(tmpMask, "%s", config->GetKey(tmp_name, "mask"));
			if (tmpMask[0] != '0' && tmpMask[0] != 0)
				tmpMtl->MicroDetail.Mask = SGCore_LoadTexAddName(tmpMask);
			else
				tmpMtl->MicroDetail.Mask = -1;


			sprintf(tmpParamLigth, "%s", config->GetKey(tmp_name, "param_ligth"));
			if (tmpParamLigth[0] != '0' && tmpParamLigth[0] != 0)
				tmpMtl->LightParam.ParamTex = SGCore_LoadTexAddName(tmpParamLigth);
			else
				tmpMtl->LightParam.ParamTex = -1;

			tmpMtl->IsRefraction = String(config->GetKey(tmp_name, "refraction")).ToBool();
			//tmpMtl->IsRefraction = true;

			tmpMtl->VS.IsTransWorld = String(config->GetKey(tmp_name, "vs_world")).ToInt();
			tmpMtl->VS.IsTransView = String(config->GetKey(tmp_name, "vs_view")).ToInt();
			tmpMtl->VS.IsTransProjection = String(config->GetKey(tmp_name, "vs_projection")).ToInt();
			tmpMtl->VS.IsTransWorldView = String(config->GetKey(tmp_name, "vs_worldview")).ToInt();
			tmpMtl->VS.IsTransWorldViewProjection = String(config->GetKey(tmp_name, "vs_worldviewprojection")).ToInt();
			tmpMtl->VS.IsTransPosCam = String(config->GetKey(tmp_name, "vs_poscam")).ToInt();
			tmpMtl->VS.IsTransTimeDelta = String(config->GetKey(tmp_name, "vs_timedelta")).ToInt();
			tmpMtl->VS.IsTransUserData = String(config->GetKey(tmp_name, "vs_userdata")).ToInt();
			tmpMtl->VS.Param.x = String(config->GetKey(tmp_name, "vs_userdata_value_x")).ToDouble();
			tmpMtl->VS.Param.y = String(config->GetKey(tmp_name, "vs_userdata_value_y")).ToDouble();
			tmpMtl->VS.Param.z = String(config->GetKey(tmp_name, "vs_userdata_value_z")).ToDouble();
			tmpMtl->VS.Param.w = String(config->GetKey(tmp_name, "vs_userdata_value_w")).ToDouble();
			tmpMtl->TransVSDataInPS = String(config->GetKey(tmp_name, "vs_userdata_trans_in_ps")).ToBool();

			tmpMtl->PS.IsTransWorld = String(config->GetKey(tmp_name, "ps_world")).ToInt();
			tmpMtl->PS.IsTransView = String(config->GetKey(tmp_name, "ps_view")).ToInt();
			tmpMtl->PS.IsTransProjection = String(config->GetKey(tmp_name, "ps_projection")).ToInt();
			tmpMtl->PS.IsTransWorldView = String(config->GetKey(tmp_name, "ps_worldview")).ToInt();
			tmpMtl->PS.IsTransWorldViewProjection = String(config->GetKey(tmp_name, "ps_worldviewprojection")).ToInt();
			tmpMtl->PS.IsTransPosCam = String(config->GetKey(tmp_name, "ps_poscam")).ToInt();
			tmpMtl->PS.IsTransTimeDelta = String(config->GetKey(tmp_name, "ps_timedelta")).ToInt();
			tmpMtl->PS.IsTransUserData = String(config->GetKey(tmp_name, "ps_userdata")).ToInt();
			tmpMtl->PS.Param.x = String(config->GetKey(tmp_name, "ps_userdata_value_x")).ToDouble();
			tmpMtl->PS.Param.y = String(config->GetKey(tmp_name, "ps_userdata_value_y")).ToDouble();
			tmpMtl->PS.Param.z = String(config->GetKey(tmp_name, "ps_userdata_value_z")).ToDouble();
			tmpMtl->PS.Param.w = String(config->GetKey(tmp_name, "ps_userdata_value_w")).ToDouble();
			tmpMtl->TransPSDataInVS = String(config->GetKey(tmp_name, "ps_userdata_trans_in_vs")).ToBool();


			tmpMtl->LightParam.TypeReflect = String(config->GetKey(tmp_name, "type_reflect")).ToInt();

			tmpMtl->RenderStates.IsAlphaTest = String(config->GetKey(tmp_name, "alpha_test")).ToBool();
			tmpMtl->RenderStates.IsCullBack = String(config->GetKey(tmp_name, "cullmode")).ToBool();

			config->Release();
			ArrMaterials.push_back(tmpMtl);
			return ArrMaterials.size() - 1;
			//Data::ArrMaterial->Add(tmpMtl);
		}
		else
		{
			//если такого материала не существует, то мы должны были задать примерный тип материала
			Material* tmpMtl = new Material();

			//обычна¤ геометри¤
			if (type == 0)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrl_base");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrl_base");
				tmpMtl->VS.IsTransWorld = true;
			}
			//деревь¤
			else if (type == 1)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrl_base_green_tree");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrl_base_green");
				//tmpMtl->RenderStates.IsAlphaTest = true;
			}
			//трава
			else if (type == 2)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrl_base_green_grass");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrl_base_green");
				//tmpMtl->RenderStates.IsAlphaTest = true;
			}
			//анимационная модель
			else if (type == 3)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrl_base_skin");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrl_base");
				tmpMtl->VS.IsTransWorld = true;
			}
			//источник света
			else if (type == 4)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrl_base");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrl_base_ligth");
				tmpMtl->VS.IsTransWorld = true;
				tmpMtl->IsForwardRender = true;
				tmpMtl->PS.IsTransUserData = true;
				tmpMtl->PS.Param = float4(0, 0, 0, 0);
			}

			tmpMtl->MainTexture = SGCore_LoadTexAddName(name);
			tmpMtl->VS.IsTransWorldViewProjection = true;

			//char path[1024];
			char tmp_name[256];

			for (DWORD k = 0; k<strlen(name); k++)
			{
				if (name[k] == '.')
				{
					sprintf(tmp_name, "%s", name);
					tmp_name[k] = 0;
					IsTruePath = true;
					break;
				}
			}

			sprintf(tmpMtl->Name, "%s", tmp_name);

			ArrMaterials.push_back(tmpMtl);
			return ArrMaterials.size() - 1;
		}
	}
}

void Materials::Save()
{

}

void Materials::SetMainTexture(DWORD slot, long id)
{
	if (id >= 0 && id < ArrMaterials.size() && ArrMaterials[id]->MainTexture != -1)
		MLSet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(ArrMaterials[id]->MainTexture));
	else
		MLSet::DXDevice->SetTexture(0, 0);
}

long Materials::GetID(const char* name)
{
	for (long i = 0; i<ArrMaterials.size(); i++)
	{
		if (strcmp(ArrMaterials[i]->Name, name) == 0)
			return i;
	}
	return -1;
}

inline long Materials::GetCount()
{
	return ArrMaterials.size();
}

void Materials::Render(long id, float4x4* world)
{
	if (!(id >= 0 && id < ArrMaterials.size()))
		return;

	if (!world)
		world = &(SMMatrixIdentity());

	Material* tmpmaterial = ArrMaterials[id];

	//если есть то устанавливаем текстуру материала
	if (tmpmaterial->MainTexture != -1)
		MLSet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(tmpmaterial->MainTexture));

	//если нет отражени¤ то отправл¤ем 0
	if (tmpmaterial->LightParam.TypeReflect == 0)
		MLSet::DXDevice->SetTexture(12, 0);

	//если есть рефаркци¤, а она идет вторым проходом, то отправл¤ем, иначе посылаем 0
	if (tmpmaterial->IsRefraction)
		MLSet::DXDevice->SetTexture(13, SGCore_RTGetTexture(/*Core::Data::EngineID::Tex_DSComLigth*/0));
	else
		MLSet::DXDevice->SetTexture(13, 0);

	if (tmpmaterial->MicroDetail.Mask != -1)
		MLSet::DXDevice->SetTexture(1, SGCore_LoadTexGetTex(tmpmaterial->MicroDetail.Mask));

	for (int k = 0; k<4; k++)
	{
		if (tmpmaterial->MicroDetail.ArrDeatail[k] != -1)
			MLSet::DXDevice->SetTexture(2 + k, SGCore_LoadTexGetTex(tmpmaterial->MicroDetail.ArrDeatail[k]));
		else
			MLSet::DXDevice->SetTexture(2 + k, 0);

		if (tmpmaterial->MicroDetail.ArrMicroDiffuse[k] != -1)
			MLSet::DXDevice->SetTexture(6 + k, SGCore_LoadTexGetTex(tmpmaterial->MicroDetail.ArrMicroDiffuse[k]));
		else
			MLSet::DXDevice->SetTexture(6 + k, 0);
	}

	//если есть текстура с параметрами освещени¤ и установлено что берем параметры из текстуры, то отправл¤ем текстуру с параметрами
	if (tmpmaterial->LightParam.ParamTex != -1 && tmpmaterial->LightParam.IsTextureParam)
	{
		if (SSInput_GetKeyState(DIK_P))
		{
			SGCore_LoadTexUpdate(tmpmaterial->MainTexture);
			SGCore_LoadTexUpdate(tmpmaterial->LightParam.ParamTex);
		}

		MLSet::DXDevice->SetTexture(10, SGCore_LoadTexGetTex(tmpmaterial->LightParam.ParamTex));
	}
	//иначе если берем параметры из ... редактора
	else if (!tmpmaterial->LightParam.IsTextureParam)
	{
		//блокируем текстуру 1х1 котора¤ ест ьпараметры освещени¤, и запсиываем туда то что настрйоили
		D3DLOCKED_RECT LockedRect;
		IDirect3DTexture9* ParamLightModelTex = SGCore_LoadTexGetTex(MLSet::IDsTexs::ParamLight);
		ParamLightModelTex->LockRect(0, &LockedRect, 0, 0);
		DWORD *param = (DWORD*)LockedRect.pBits;
		//DWORD param = D3DCOLOR_ARGB(0,0,0,0);
		param[0] = D3DCOLOR_ARGB(DWORD(tmpmaterial->LightParam.MapSpecularValue*255.f), DWORD(tmpmaterial->LightParam.RoughnessValue*255.f), DWORD(tmpmaterial->LightParam.F0Value*255.f), DWORD(tmpmaterial->LightParam.SoftSpecularValue*255.f));
		ParamLightModelTex->UnlockRect(0);

		MLSet::DXDevice->SetTexture(10, SGCore_LoadTexGetTex(MLSet::IDsTexs::ParamLight));
	}
	else
		MLSet::DXDevice->SetTexture(10, SGCore_LoadTexGetTex(MLSet::IDsTexs::NullMaterial));


	if (tmpmaterial->PreShaderVS != -1)
		SGCore_ShaderBind(0, tmpmaterial->PreShaderVS);

	if (tmpmaterial->PreShaderPS != -1)
		SGCore_ShaderBind(1, tmpmaterial->PreShaderPS);

	if (tmpmaterial->VS.IsTransWorld || tmpmaterial->PS.IsTransWorld || tmpmaterial->VS.IsTransWorldView || tmpmaterial->PS.IsTransWorldView || tmpmaterial->VS.IsTransWorldViewProjection || tmpmaterial->PS.IsTransWorldViewProjection)
		worldtrans = SMMatrixTranspose(*world);

	if (tmpmaterial->VS.IsTransWorldView || tmpmaterial->PS.IsTransWorldView || tmpmaterial->VS.IsTransWorldViewProjection || tmpmaterial->PS.IsTransWorldViewProjection)
	{
		D3DXMATRIX tmpView;
		MLSet::DXDevice->GetTransform(D3DTS_VIEW, &tmpView);
		view = float4x4(tmpView);
		viewtrans = SMMatrixTranspose(view);

		D3DXMATRIX tmpProjection;

		MLSet::DXDevice->GetTransform(D3DTS_PROJECTION, &tmpProjection);
		proj = float4x4(tmpProjection);
		projtrans = SMMatrixTranspose(proj);
	}

	if (tmpmaterial->VS.IsTransWorld)
		SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "World", &worldtrans);

	if (tmpmaterial->PS.IsTransWorld)
		SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "World", &worldtrans);

	if (tmpmaterial->VS.IsTransView)
		SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "View", &viewtrans);

	if (tmpmaterial->PS.IsTransView)
		SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "View", &viewtrans);

	if (tmpmaterial->VS.IsTransProjection)
		SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "Projection", &projtrans);

	if (tmpmaterial->PS.IsTransProjection)
		SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "Projection", &projtrans);


	if (tmpmaterial->VS.IsTransWorldView || tmpmaterial->PS.IsTransWorldView)
	{
		float4x4 wv = viewtrans * worldtrans;

		if (tmpmaterial->VS.IsTransWorldView)
			SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "WorldView", &wv);

		if (tmpmaterial->PS.IsTransWorldView)
			SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "WorldView", &wv);
	}

	if (tmpmaterial->VS.IsTransWorldViewProjection || tmpmaterial->PS.IsTransWorldViewProjection)
	{
		D3DXMATRIX tmpVP;
		MLSet::DXDevice->GetTransform(D3DTS_WORLD1, &tmpVP);

		float4x4 wvp = *world * float4x4(tmpVP);
		wvp = SMMatrixTranspose(wvp);

		if (tmpmaterial->VS.IsTransWorldViewProjection)
			SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "WorldViewProjection", &wvp);

		if (tmpmaterial->PS.IsTransWorldViewProjection)
			SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "WorldViewProjection", &wvp);
	}

	if (tmpmaterial->VS.IsTransPosCam || tmpmaterial->PS.IsTransPosCam)
	{
		if (tmpmaterial->VS.IsTransPosCam)
			SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "PosCam", &MLSet::ConstCurrCamPos);

		if (tmpmaterial->PS.IsTransPosCam)
			SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "PosCam", &MLSet::ConstCurrCamPos);
	}


	if (tmpmaterial->VS.IsTransUserData)
		SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "Param", &(tmpmaterial->VS.Param));

	if (tmpmaterial->TransVSDataInPS)
		SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "ParamVS", &(tmpmaterial->VS.Param));

	if (tmpmaterial->PS.IsTransUserData)
		SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "Param", &(tmpmaterial->PS.Param));

	if (tmpmaterial->TransPSDataInVS)
		SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "ParamPS", &(tmpmaterial->PS.Param));

	if (tmpmaterial->VS.IsTransTimeDelta)
		SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "TimeDelta", &float2(MLSet::CountTimeDelta, /*float(timeDelta)*/0 * 0.001f));

	if (tmpmaterial->PS.IsTransTimeDelta)
		SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "TimeDelta", &float2(MLSet::CountTimeDelta, /*float(timeDelta)*/0 * 0.001f));

	if (tmpmaterial->RenderStates.IsCullBack)
		MLSet::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	else
		MLSet::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (tmpmaterial->RenderStates.IsAlphaTest)
	{
		MLSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		MLSet::DXDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		MLSet::DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	else
		MLSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//почти во всех пиксельных шейдерах материалов есть данна¤ NearFar, необходима¤ д¤л записи глубины
	if (tmpmaterial->PreShaderPS != -1)
		SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "NearFar", &(MLSet::NearFar));
}