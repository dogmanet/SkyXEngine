
#include <mtl_ligth\\material.h>
#include <mtl_ligth\\reflection.cpp>

Materials::Materials()
{
	CountTimeDelta = 0;
	SGCore_ShaderLoad(0, "mtrlgeom_base.vs", "mtrlgeom_base", true);
	SGCore_ShaderLoad(1, "mtrlgeom_base.ps", "mtrlgeom_base", true);

	SGCore_ShaderLoad(0, "mtrlgreen_base_tree.vs", "mtrlgreen_base_tree", true);
	SGCore_ShaderLoad(0, "mtrlgreen_base_grass.vs", "mtrlgreen_base_grass", true);
	SGCore_ShaderLoad(1, "mtrlgreen_base.ps", "mtrlgreen_base", true);

	SGCore_ShaderLoad(1, "mtrlgeom_light.ps", "mtrlgeom_light",true);


	Material* tmpMtlDefaultLight = new Material();

	sprintf(tmpMtlDefaultLight->Name, "%s", "mtrldef_light");
	tmpMtlDefaultLight->PreShaderVS = SGCore_ShaderGetID(0, "mtrlgeom_base");
	tmpMtlDefaultLight->PreShaderPS = SGCore_ShaderGetID(1, "mtrlgeom_light");

	tmpMtlDefaultLight->MainTexture = -1;
	tmpMtlDefaultLight->VS.IsTransWorldViewProjection = true;

	ArrMaterials.push_back(tmpMtlDefaultLight);
	AddName(tmpMtlDefaultLight->Name, ArrMaterials.size() - 1);
	MtrlDefLight = ArrMaterials.size() - 1;

	/*Material* tmpMtlDefaultGreen = new Material();

	sprintf(tmpMtlDefaultGreen->Name, "%s", "mtrl_default_green_color");
	tmpMtlDefaultGreen->PreShaderVS = SGCore_ShaderGetID(0, "mtrl_default_green");
	tmpMtlDefaultGreen->PreShaderPS = SGCore_ShaderGetID(1, "mtrl_default_green_color");

	tmpMtlDefoltGreen->MainTexture = -1;
	tmpMtlDefoltGreen->VS.IsTransWorldViewProjection = true;

	Arr.push_back(tmpMtlDefoltGreen);


	Material* tmpMtlBase = new Material();

	sprintf(tmpMtlBase->Name, "%s", "mtrl_base");
	tmpMtlBase->PreShaderVS = SGCore_ShaderGetID(0, "mtrl_base");
	tmpMtlBase->PreShaderPS = SGCore_ShaderGetID(1, "mtrl_base");
	tmpMtlBase->VS.IsTransWorldViewProjection = true;
	tmpMtlBase->MainTexture = -1;

	Arr.push_back(tmpMtlBase);*/
}

Materials::~Materials()
{
	for (long i = 0; i < ArrHMtls.size(); ++i)
	{
		mem_delete(ArrHMtls[i]);
	}

	for (long i = 0; i < ArrMaterials.size(); ++i)
	{
		mem_delete(ArrMaterials[i]);
	}
}

Materials::Material::Material()
{
	Name[0] = 0;
	TransVSDataInPS = false;
	TransPSDataInVS = false;

	IsForwardRender = false;
	IsRefraction = false;
	PhysicsMaterial = -1;

	Type = MTL_GEOM;

	Reflect = 0;
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

int Materials::GetType(ID id)
{
	MTL_PRE_COND_ID(id, -1);
	return ArrMaterials[id]->Type;
}

bool Materials::IsRefraction(ID id)
{
	MTL_PRE_COND_ID(id, -1);
	return ArrMaterials[id]->IsRefraction;
}

int Materials::TypeReflection(ID id)
{
	MTL_PRE_COND_ID(id, -1);
	return ArrMaterials[id]->LightParam.TypeReflect;
}

ID Materials::IsExists(const char* name)
{
	char tmp_path[MTL_MAX_SIZE_DIR];//папка
	char tmp_name[MTL_MAX_SIZE_NAME];//само имя текстыр с расширением
	long id = -1;
	bool IsTruePath = false;
	//обрезаем имя текстуры и папку
	for (int i = 0; i<strlen(name); i++)
	{
		if (name[i] == '_')
		{
			memcpy(tmp_path, name, sizeof(char)*i);
			tmp_path[i] = 0;
			sprintf(tmp_name, "%s", name + i + 1);
			IsTruePath = true;
			break;
		}
	}

	if (!IsTruePath)
	{
		return -2;
		//reportf(-1, "%s - wrong texture name [%s]!!!", gen_msg_location, name);
	}

	long tmpkey = -1;	//переменная в которой храним ключ от массива в который записываем
	for (long i = 0; i<ArrHMtls.size(); ++i)
	{
		if (strcmp(ArrHMtls[i]->Path, tmp_path) == 0)
		{
			tmpkey = i;
			break;
		}
	}

	//если мы не нашли совпадений значит путь уникален ...
	if (tmpkey == -1)
	{
		return -1;
	}

	//првоеряем записано ли уже имя текстуры
	for (int i = 0; i<ArrHMtls[tmpkey]->ArrNames.size(); i++)
	{
		if (strcmp(ArrHMtls[tmpkey]->ArrNames[i].c_str(), tmp_name) == 0)
		{
			id = ArrHMtls[tmpkey]->ArrID[i];
			break;
		}
	}

	return id;
}

void Materials::AddName(const char* name, ID id)
{
	char tmp_path[MTL_MAX_SIZE_DIR];//папка
	char tmp_name[MTL_MAX_SIZE_NAME];//само имя текстыр с расширением

	bool IsTruePath = false;
	//обрезаем имя текстуры и папку
	for (int i = 0; i<strlen(name); i++)
	{
		if (name[i] == '_')
		{
			memcpy(tmp_path, name, sizeof(char)*i);
			tmp_path[i] = 0;
			sprintf(tmp_name, "%s", name + i + 1);
			IsTruePath = true;
			break;
		}
	}

	long tmpkey = -1;	//переменная в которой храним ключ от массива в который записываем
	for (long i = 0; i<ArrHMtls.size(); ++i)
	{
		if (strcmp(ArrHMtls[i]->Path, tmp_path) == 0)
		{
			tmpkey = i;
			break;
		}
	}

	//если мы не нашли совпадений значит путь уникален ...
	if (tmpkey == -1)
	{
		tmpkey = ArrHMtls.size();
		TLPath* tmpntlp = new TLPath();
		ArrHMtls.push_back(tmpntlp);
	}
	sprintf(ArrHMtls[tmpkey]->Path, "%s", tmp_path);

	ArrHMtls[tmpkey]->ArrNames.push_back(tmp_name);
	ArrHMtls[tmpkey]->ArrID.push_back(id);
}

ID Materials::Load(const char* name, int type)
{
	long IsLoad = IsExists(name);

	if (IsLoad >= 0)
		return IsLoad;
	else
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

			if (config->KeyExists(tmp_name, "type"))
				tmpMtl->Type = String(config->GetKey(tmp_name, "type")).ToInt();
			else
				tmpMtl->Type = MTL_GEOM;

			if (config->KeyExists(tmp_name, "physmaterial"))
				tmpMtl->PhysicsMaterial = String(config->GetKey(tmp_name, "physmaterial")).ToInt();
			else
				tmpMtl->PhysicsMaterial = 0;

			//Core::InLog("PhysicsMaterial = %d\n",tmpMtl->PhysicsMaterial);

			if (def_str_validate(tmpVS))
				tmpMtl->PreShaderVS = SGCore_ShaderLoad(0, tmpVS, "main", true);
			else
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrlgeom_base");

			if (def_str_validate(tmpPS))
				tmpMtl->PreShaderPS = SGCore_ShaderLoad(1, tmpPS, "main", true);
			else
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrlgeom_base");

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

			if (tmpMtl->LightParam.TypeReflect == 1)
			{
				/*tmpMtl->Reflect = new Reflection();
				tmpMtl->Reflect->Init(0);*/
			}

			tmpMtl->RenderStates.IsAlphaTest = String(config->GetKey(tmp_name, "alpha_test")).ToBool();
			tmpMtl->RenderStates.IsCullBack = String(config->GetKey(tmp_name, "cullmode")).ToBool();

			config->Release();
			ArrMaterials.push_back(tmpMtl);
		}
		else
		{
			//если такого материала не существует, то мы должны были задать примерный тип материала
			Material* tmpMtl = new Material();
			tmpMtl->Type = type;
			//обычна¤ геометри¤
			if (type == MTL_GEOM)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrlgeom_base");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrlgeom_base");
				tmpMtl->VS.IsTransWorld = true;
			}
			//деревь¤
			else if (type == MTL_TREE)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrlgreen_base_tree");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrlgreen_base");
				//tmpMtl->RenderStates.IsAlphaTest = true;
			}
			//трава
			else if (type == MTL_GRASS)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrlgreen_base_grass");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrlgreen_base");
				//tmpMtl->RenderStates.IsAlphaTest = true;
			}
			//анимационная модель
			else if (type == MTL_ANIM)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrlskin_base");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrlskin_base");
				tmpMtl->VS.IsTransWorld = true;
			}
			//источник света
			/*else if (type == MTL_LIGHT)
			{
				tmpMtl->PreShaderVS = SGCore_ShaderGetID(0, "mtrlgeom_base");
				tmpMtl->PreShaderPS = SGCore_ShaderGetID(1, "mtrlgeom_light");
				tmpMtl->IsForwardRender = true;
				tmpMtl->PS.IsTransUserData = true;
				tmpMtl->PS.Param = float4(0, 0, 0, 0);
			}*/

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
		}

		AddName(name, ArrMaterials.size() - 1);
		return ArrMaterials.size() - 1;
	}
}

void Materials::Save()
{

}

void Materials::Update(DWORD timeDelta)
{
	CurrTimeDelta = timeDelta;
	CountTimeDelta += timeDelta;
}

void Materials::SetMainTexture(ID slot, ID id)
{
	if (id >= 0 && id < ArrMaterials.size() && ArrMaterials[id]->MainTexture != -1)
		MLSet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(ArrMaterials[id]->MainTexture));
	else
		MLSet::DXDevice->SetTexture(0, 0);
}

ID Materials::GetID(const char* name)
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

void Materials::Render(ID id, float4x4* world)
{
	MTL_PRE_COND_ID(id);

	if (!world)
		world = &(SMMatrixIdentity());

	Material* tmpmaterial = ArrMaterials[id];

	//если есть то устанавливаем текстуру материала
	if (tmpmaterial->MainTexture != -1)
		MLSet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(tmpmaterial->MainTexture));

	//если нет отражени¤ то отправл¤ем 0
	if (tmpmaterial->LightParam.TypeReflect == 0)
		MLSet::DXDevice->SetTexture(12, 0);
	else
		MLSet::DXDevice->SetTexture(12, SML_MtlRefGetTexPlane());

	MLSet::DXDevice->SetTexture(14, SGCore_RTGetTexture(MLSet::IDsRenderTargets::DepthScene));

	//если есть рефаркци¤, а она идет вторым проходом, то отправл¤ем, иначе посылаем 0
	if (tmpmaterial->IsRefraction)
		MLSet::DXDevice->SetTexture(13, SGCore_RTGetTexture(/*Core::Data::EngineID::Tex_DSComLigth*/MLSet::IDsRenderTargets::LigthCom2));
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
		SGCore_ShaderSetVRF(0, tmpmaterial->PreShaderVS, "TimeDelta", &float2(CountTimeDelta, float(CurrTimeDelta) * 0.001f));

	if (tmpmaterial->PS.IsTransTimeDelta)
		SGCore_ShaderSetVRF(1, tmpmaterial->PreShaderPS, "TimeDelta", &float2(CountTimeDelta, float(CurrTimeDelta) * 0.001f));

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

void Materials::RenderLight(float4_t* color, float4x4* world)
{
	MTL_PRE_COND_ID(MtrlDefLight);

	ArrMaterials[MtrlDefLight]->PS.Param = *color;
	Render(MtrlDefLight, world);
	ArrMaterials[MtrlDefLight]->PS.Param = float4(0, 0, 0, 0);
}