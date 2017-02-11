
#include <material_ligth\\light.h>

Lights::Lights()
{
	D3DVERTEXELEMENT9 layoutstatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	MLSet::DXDevice->CreateVertexDeclaration(layoutstatic, &VertexDeclarationStatic);
	HowShadow = 0;
	ShadowMap = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap", 1);
	ShadowMap2 = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "shadowmap2", 1);
	
	MLSet::IDsRenderTargets::ColorScene = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_color", 1);
	MLSet::IDsRenderTargets::NormalScene = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A2B10G10R10, D3DPOOL_DEFAULT, "ds_normal", 1);
	MLSet::IDsRenderTargets::ParamsScene = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, "ds_param", 1);
	MLSet::IDsRenderTargets::DepthScene = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_depth", 1);

	MLSet::IDsRenderTargets::LightAmbientDiff = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_ambient", 1);
	MLSet::IDsRenderTargets::LightSpecular = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_specdiff", 1);

	for (int i = 0; i < 4; i++)
	{
		int iSampleLen = 1 << (2 * i);

		MLSet::IDsRenderTargets::ToneMaps[i] = SGCore_RTAdd(iSampleLen, iSampleLen, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "", 0);
	}

	MLSet::IDsRenderTargets::AdaptLumCurr = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "", 0);
	MLSet::IDsRenderTargets::AdaptLumLast = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "", 0);

	MLSet::IDsRenderTargets::LigthCom = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A2B10G10R10, D3DPOOL_DEFAULT, "ds_lightcom", 1);
	
	MLSet::IDsRenderTargets::LigthComScaled = SGCore_RTAdd(MLSet::WinSize.x*0.25f, MLSet::WinSize.y*0.25f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A2B10G10R10, D3DPOOL_DEFAULT, "ds_lightcomscaled", 0.25);
}

Lights::~Lights()
{
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		mem_delete(ArrLights[i]);
	}

	ArrLights.clear();

	SGCore_RTDelete(ShadowMap);
	SGCore_RTDelete(ShadowMap2);
}

void Lights::Save(const char* path)
{
	FILE* file = 0;
	file = fopen(path, "w");

	if (!file)
	{
		reportf(-1, "%s - light: failed open file '%s'", gen_msg_location, path);
		return;
	}

	float3 tmppos;
	float tmppower;
	float tmpdist;
	float3 tmprot;
	float3 tmpcolor;
	fprintf(file, "%s", "[light]\n");
	fprintf(file, "%s%d%s", "count = ", ArrLights.size(), "\n");
	fprintf(file, "%s", "\n");
	long tmpnumll = 0;


	for (DWORD i = 0; i<ArrLights.size(); i++)
	{
		//if (SkyXEngine::Core::Data::Level::LightManager->Arr[i]->IDEffect == -1)
		//{
		GetLightPos(i, &tmppos,false,true);
		tmppower = ArrLights[i]->Power;
		tmpdist = ArrLights[i]->Dist;
		tmprot = ArrLights[i]->Rotation;
		tmpcolor = ArrLights[i]->Color;

		char* tmpname = ArrLights[i]->Name;

		fprintf(file, "%s%d%s", "[light_", i, "]\n");

		fprintf(file, "%s%s%s", "name = ", tmpname, "\n");
		fprintf(file, "%s%d%s", "enable = ", ArrLights[i]->IsEnable, "\n");

		fprintf(file, "%s%d%s", "type = ", ArrLights[i]->TypeLight, "\n");

		fprintf(file, "%s%d%s", "color_r = ", DWORD(tmpcolor.x * 255), "\n");
		fprintf(file, "%s%d%s", "color_g = ", DWORD(tmpcolor.y * 255), "\n");
		fprintf(file, "%s%d%s", "color_b = ", DWORD(tmpcolor.z * 255), "\n");
		
		fprintf(file, "%s%d%s", "is_shadowed = ", ArrLights[i]->IsShadow, "\n");

		fprintf(file, "%s%f%s", "pos_x = ", tmppos.x, "\n");
		fprintf(file, "%s%f%s", "pos_y = ", tmppos.y, "\n");
		fprintf(file, "%s%f%s", "pos_z = ", tmppos.z, "\n");

		fprintf(file, "%s%f%s", "power = ", tmppower, "\n");

		if (ArrLights[i]->TypeLight != LIGHTS_TYPE_GLOBAL)
		{
			fprintf(file, "%s%f%s", "shadow_bias = ", GetLightBias(i), "\n");
			fprintf(file, "%s%f%s", "dist = ", tmpdist, "\n");
			fprintf(file, "%s%f%s", "dist_for_end_shadow = ", GetShadowLocalFar(i), "\n");

			if (ArrLights[i]->Source)
			{
				fprintf(file, "source = %s\n", GetLightPathSource(i));
				fprintf(file, "source_bind_group = %d\n", GetLightBindedGroupSource(i));

				float3 rotsource;
				GetLightRot(i, &rotsource, true);

				fprintf(file, "source_rot_x = %f\n", rotsource.x);
				fprintf(file, "source_rot_y = %f\n", rotsource.y);
				fprintf(file, "source_rot_z = %f\n", rotsource.z);
			}

				if (ArrLights[i]->TypeLight == LIGHTS_TYPE_DIRECTION)
				{
					fprintf(file, "%s%f%s", "rot_x = ", tmprot.x, "\n");
					fprintf(file, "%s%f%s", "rot_y = ", tmprot.y, "\n");
					fprintf(file, "%s%f%s", "rot_z = ", tmprot.z, "\n");
					fprintf(file, "%s%f%s", "angle = ", GetLightAngle(i), "\n");
					fprintf(file, "%s%f%s", "top_radius = ", GetLightTopRadius(i), "\n");
				}
				else
				{
					fprintf(file, "%s%d%s", "enable_edge_0 = ", GetEnableCubeEdge(i, 0), "\n");
					fprintf(file, "%s%d%s", "enable_edge_1 = ", GetEnableCubeEdge(i, 1), "\n");
					fprintf(file, "%s%d%s", "enable_edge_2 = ", GetEnableCubeEdge(i, 2), "\n");
					fprintf(file, "%s%d%s", "enable_edge_3 = ", GetEnableCubeEdge(i, 3), "\n");
					fprintf(file, "%s%d%s", "enable_edge_4 = ", GetEnableCubeEdge(i, 4), "\n");
					fprintf(file, "%s%d%s", "enable_edge_5 = ", GetEnableCubeEdge(i, 5), "\n");
				}
			}
			tmpnumll++;
			fprintf(file, "%s", "\n");

		//}
	}

	fclose(file);
}

void Lights::Load(const char* path)
{
	ISXLConfig* config = Core_OpLConfig(path);

	if (!config->SectionExists("light", true))
	{
		reportf(-1, "%s - light: not found section 'light', file '%s'", gen_msg_location, path);
		return;
	}

	if (!config->KeyExists("light", "count", true))
	{
		reportf(-1, "%s - light: not found key 'count', file '%s', section 'light'", gen_msg_location);
		return;
	}

	long CountLight = String(config->GetKey("light", "count")).ToInt();

	char text_SectionLight[CONFIG_SECTION_MAX_LEN];
	char textsource[1024];
	long tmpsourcebindgroup;
	char tmpname[OBJECT_NAME_MAX_LEN];
	bool tmpenable;
	bool tmpshadow;
	float tmpshadowbias;
	bool tmpglobal;
	float distforendshadow;
	float3 tmppos;
	float tmppower;
	float tmpdist;
	int dir_or_rot;
	float3 tmprot;
	int tmptype;
	float3 tmpcolor;

	float tmptopradius;
	float tmpangle;

	bool tmp_enable_edge[6];

	for (int i = 0; i<CountLight; i++)
	{
		sprintf(text_SectionLight, "%s%d", "light_", i);

		if (!config->SectionExists(text_SectionLight, true))
		{
			reportf(-1, "%s - light: not found section '%s', file '%s', section ''", gen_msg_location, path, text_SectionLight);
			return;
		}

		/*if (config->KeyExists(text_SectionLight, "enable", true))
		{
			reportf(-1, "%s - light: not found key 'enable', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
			return;
		}*/

		tmpenable = String(config->GetKey(text_SectionLight, "enable")).ToBool();

		if (config->KeyExists(text_SectionLight, "dist_for_end_shadow"))
			distforendshadow = String(config->GetKey(text_SectionLight, "dist_for_end_shadow")).ToDouble();
		else
			distforendshadow = 0;

		if (config->KeyExists(text_SectionLight, "name"))
			sprintf(tmpname, "%s", config->GetKey(text_SectionLight, "name"));
		else
			sprintf(tmpname, "%s", "__noname");

		if (config->KeyExists(text_SectionLight, "source"))
			sprintf(textsource,"%s",config->GetKey(text_SectionLight, "source"));
		else
			textsource[0] = '0';

		if (textsource[0] != 0 && config->KeyExists(text_SectionLight, "source_bind_group"))
			tmpsourcebindgroup = String(config->GetKey(text_SectionLight, "source_bind_group")).ToInt();
		else
			tmpsourcebindgroup = 0;

		if (config->KeyExists(text_SectionLight, "pos_x"))
			tmppos.x = String(config->GetKey(text_SectionLight, "pos_x")).ToDouble();
		else
			tmppos.x = 0;

		if (config->KeyExists(text_SectionLight, "pos_y"))
			tmppos.y = String(config->GetKey(text_SectionLight, "pos_y")).ToDouble();
		else
			tmppos.y = 0;

		if (config->KeyExists(text_SectionLight, "pos_x"))
			tmppos.z = String(config->GetKey(text_SectionLight, "pos_z")).ToDouble();
		else
			tmppos.z = 0;

		if (config->KeyExists(text_SectionLight, "type"))
			tmptype = String(config->GetKey(text_SectionLight, "type")).ToInt();
		else
		{
			reportf(-1, "%s - light: not found key 'type', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
			return;
		}

		if (config->KeyExists(text_SectionLight, "is_shadowed"))
			tmpshadow = String(config->GetKey(text_SectionLight, "is_shadowed")).ToBool();
		else
		{
			reportf(-1, "%s - light: not found key 'is_shadowed', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
			return;
		}

		if (config->KeyExists(text_SectionLight, "shadow_bias"))
			tmpshadowbias = String(config->GetKey(text_SectionLight, "shadow_bias")).ToDouble();
		else
			tmpshadowbias = 0;

		if (config->KeyExists(text_SectionLight, "power"))
			tmppower = String(config->GetKey(text_SectionLight, "power")).ToDouble();
		else
		{
			reportf(-1, "%s - light: not found key 'power', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
			return;
		}


		if (config->KeyExists(text_SectionLight, "color_r"))
			tmpcolor.x = String(config->GetKey(text_SectionLight, "color_r")).ToDouble() / 255.f;
		else
		{
			reportf(-1, "%s - light: not found key 'color_r', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
			return;
		}
		
		if (config->KeyExists(text_SectionLight, "color_g"))
			tmpcolor.y = String(config->GetKey(text_SectionLight, "color_g")).ToDouble() / 255.f;
		else
		{
			reportf(-1, "%s - light: not found key 'color_g', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
			return;
		}

		if (config->KeyExists(text_SectionLight, "color_b"))
			tmpcolor.z = String(config->GetKey(text_SectionLight, "color_b")).ToDouble() / 255.f;
		else
		{
			reportf(-1, "%s - light: not found key 'color_b', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
			return;
		}

		if (config->KeyExists(text_SectionLight, "dist"))
			tmpdist = String(config->GetKey(text_SectionLight, "dist")).ToDouble();
		else if (tmptype != LIGHTS_TYPE_GLOBAL)
		{
			reportf(-1, "%s - light: not found key 'dist', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
			return;
		}


		//если источник точечный
		if (tmptype == LIGHTS_TYPE_POINT)
		{
			if (config->KeyExists(text_SectionLight, "enable_edge_0"))
				tmp_enable_edge[0] = String(config->GetKey(text_SectionLight, "enable_edge_0")).ToBool();
			else
				tmp_enable_edge[0] = true;

			if (config->KeyExists(text_SectionLight, "enable_edge_1"))
				tmp_enable_edge[1] = String(config->GetKey(text_SectionLight, "enable_edge_1")).ToBool();
			else
				tmp_enable_edge[1] = '1';

			if (config->KeyExists(text_SectionLight, "enable_edge_2"))
				tmp_enable_edge[2] = String(config->GetKey(text_SectionLight, "enable_edge_2")).ToBool();
			else
				tmp_enable_edge[2] = true;

			if (config->KeyExists(text_SectionLight, "enable_edge_3"))
				tmp_enable_edge[3] = String(config->GetKey(text_SectionLight, "enable_edge_3")).ToBool();
			else
				tmp_enable_edge[3] = true;

			if (config->KeyExists(text_SectionLight, "enable_edge_4"))
				tmp_enable_edge[4] = String(config->GetKey(text_SectionLight, "enable_edge_4")).ToBool();
			else
				tmp_enable_edge[4] = true;

			if (config->KeyExists(text_SectionLight, "enable_edge_5"))
				tmp_enable_edge[5] = String(config->GetKey(text_SectionLight, "enable_edge_5")).ToBool();
			else
				tmp_enable_edge[5] = true;

			CreatePoint(-1,
				&tmppos,
				tmppower,
				tmpdist,
				&tmpcolor,
				false,
				tmpshadow,0);

			SetEnableCubeEdge(ArrLights.size() - 1, 0, tmp_enable_edge[0]);
			SetEnableCubeEdge(ArrLights.size() - 1, 1, tmp_enable_edge[1]);
			SetEnableCubeEdge(ArrLights.size() - 1, 2, tmp_enable_edge[2]);
			SetEnableCubeEdge(ArrLights.size() - 1, 3, tmp_enable_edge[3]);
			SetEnableCubeEdge(ArrLights.size() - 1, 4, tmp_enable_edge[4]);
			SetEnableCubeEdge(ArrLights.size() - 1, 5, tmp_enable_edge[5]);
		}
		//если направленный
		else if (tmptype == LIGHTS_TYPE_DIRECTION)
		{
			if (config->KeyExists(text_SectionLight, "dir_x"))
			{
				dir_or_rot = 1;
				tmprot.x = String(config->GetKey(text_SectionLight, "dir_x")).ToDouble();
				tmprot.y = String(config->GetKey(text_SectionLight, "dir_y")).ToDouble();
				tmprot.z = String(config->GetKey(text_SectionLight, "dir_z")).ToDouble();
			}
			else if (config->KeyExists(text_SectionLight, "rot_x"))
			{
				dir_or_rot = 0;
				tmprot.x = String(config->GetKey(text_SectionLight, "rot_x")).ToDouble();
				tmprot.y = String(config->GetKey(text_SectionLight, "rot_y")).ToDouble();
				tmprot.z = String(config->GetKey(text_SectionLight, "rot_z")).ToDouble();
			}
			else
			{
				dir_or_rot = 1;
				tmprot.x = 0;
				tmprot.y = -1;
				tmprot.z = 0;
			}

			if (config->KeyExists(text_SectionLight, "top_radius"))
				tmptopradius = String(config->GetKey(text_SectionLight, "top_radius")).ToDouble();
			else
			{
				reportf(-1, "%s - light: not found key 'top_radius', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
				return;
			}

			if (config->KeyExists(text_SectionLight, "angle"))
				tmpangle = String(config->GetKey(text_SectionLight, "angle")).ToDouble();
			else
			{
				reportf(-1, "%s - light: not found key 'angle', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
				return;
			}

			CreateDirection(-1,
				&tmppos,
				tmppower,
				tmpdist,
				&tmpcolor,
				&tmprot,
				tmptopradius,
				tmpangle,
				tmpshadow,0);

			
		}
		//если глобальный
		else if (tmptype == LIGHTS_TYPE_GLOBAL)
		{
			tmppos.z = 0;
			CreatePoint(-1,
				&tmppos,
				LIGHTS_GLOBAL_MAX_POWER,
				LIGHTS_GLOBAL_STD_RADIUS,
				&tmpcolor,
				true,
				tmpshadow,0);
			
		}
		else
		{
			reportf(-1, "%s - light: undefinde type light '%d', file '%s', section '%s'", gen_msg_location, tmptype, path, text_SectionLight);
			return;
		}

		if (tmptype != LIGHTS_TYPE_GLOBAL)
		{
			SetLightBias(ArrLights.size() - 1, tmpshadowbias);
			
			if (distforendshadow > 0)
				SetShadowLocalFar(ArrLights.size() - 1, distforendshadow);

			if (def_str_validate(textsource))
			{
				LoadLightMeshSource(ArrLights.size() - 1, textsource);
				BindLightToGroup(ArrLights.size() - 1, tmpsourcebindgroup);

				float3 rotsource;

				if (config->KeyExists(text_SectionLight, "source_rot_x"))
					rotsource.x = String(config->GetKey(text_SectionLight, "source_rot_x")).ToDouble();
				else
				{
					reportf(-1, "%s - light: not found key 'source_rot_x', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
					return;
				}

				if (config->KeyExists(text_SectionLight, "source_rot_y"))
					rotsource.x = String(config->GetKey(text_SectionLight, "source_rot_y")).ToDouble();
				else
				{
					reportf(-1, "%s - light: not found key 'source_rot_y', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
					return;
				}

				if (config->KeyExists(text_SectionLight, "source_rot_z"))
					rotsource.x = String(config->GetKey(text_SectionLight, "source_rot_z")).ToDouble();
				else
				{
					reportf(-1, "%s - light: not found key 'source_rot_z', file '%s', section '%s'", gen_msg_location, path, text_SectionLight);
					return;
				}

				SetLightRot(ArrLights.size() - 1, &rotsource, true);
			}

			if (tmptype == LIGHTS_TYPE_DIRECTION)
			{
				if (dir_or_rot == 1)
					SetLightDir(ArrLights.size() - 1, &tmprot,false);
				else
					SetLightRot(ArrLights.size() - 1, &tmprot, false);
			}
		}

		SetEnable(ArrLights.size() - 1, tmpenable);
		SetLightName(ArrLights.size() - 1, tmpname);
	}

	config->Release();
}

long Lights::CreateCopy(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		Light* tmplight = ArrLights[id];
		Light* tmplight2 = new Light();

		tmplight2->Angle = tmplight->Angle;
		tmplight2->BoundVolume = SGCore_CrBound();
		IDirect3DVertexBuffer9* vertexbuf;
		tmplight->Mesh->GetVertexBuffer(&vertexbuf);
		tmplight->BoundVolume->CalcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
		mem_release(vertexbuf);
		tmplight2->Power = tmplight->Power;
		tmplight2->Dist = tmplight->Dist;
		tmplight2->Color = tmplight->Color;
		tmplight2->Direction = tmplight->Direction;
		tmplight2->DistFor = -1;
		tmplight2->GAngleX = tmplight->GAngleX;
		tmplight2->GAngleY = tmplight->GAngleY;
		tmplight2->IsEnable = tmplight->IsEnable;
		tmplight2->IsGlobal = tmplight->IsGlobal;
		tmplight2->IsShadow = tmplight->IsShadow;
		tmplight2->IsVisible = tmplight->IsVisible;
		tmplight2->IsVisibleFor = false;
		tmplight2->MatRot = tmplight->MatRot;
		sprintf(tmplight2->Name, "%s", tmplight->Name);
		tmplight2->Position = tmplight->Position;
		tmplight2->Rotation = tmplight->Rotation;
		tmplight2->TopBottomRadius = tmplight->TopBottomRadius;
		tmplight2->TypeLight = tmplight->TypeLight;
		tmplight2->WorldMat = tmplight->WorldMat;

		tmplight2->TypeShadowed = tmplight->TypeShadowed;
		tmplight2->CountUpdate = tmplight->CountUpdate;
		tmplight2->MatRotL = tmplight2->MatRotL;

		if (tmplight->Source)
		{
			tmplight2->Source = new Light::MeshSource();
			sprintf(tmplight2->Source->Path, "%s", tmplight->Source->Path);

			tmplight2->Source->Mesh = tmplight->Source->Mesh->GetCopy();
			tmplight2->Source->WorldMat;
			tmplight2->Source->WorldMatLocal;
			tmplight2->Source->MatRot;
			tmplight2->Source->Position;
			tmplight2->Source->Rotation;
			tmplight2->Source->LocalPos;
			tmplight2->Source->IdGroupBind;

			tmplight2->Source->ArrTex = new DWORD[tmplight2->Source->Mesh->SubsetCount];
			memcpy(tmplight2->Source->ArrTex, tmplight->Source->ArrTex, sizeof(DWORD)* tmplight2->Source->Mesh->SubsetCount);
		}

		if (tmplight->ShadowPSSM)
		{
			tmplight2->ShadowPSSM = new PSSM();
			tmplight2->ShadowPSSM->Init();
			tmplight2->GAngleX = tmplight->GAngleX;
			tmplight2->GAngleY = tmplight->GAngleY;
			tmplight2->ShadowPSSM->SetPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			memcpy((tmplight2->ShadowPSSM->NearFar), (tmplight->ShadowPSSM->NearFar), sizeof(float2)* 5);
			memcpy((tmplight2->ShadowPSSM->IsUpdate), (tmplight->ShadowPSSM->IsUpdate), sizeof(int) * 5);
		}
		else
			tmplight2->ShadowPSSM = 0;

		if (tmplight->ShadowSM)
		{
			tmplight2->ShadowSM = new ShadowMapTech();
			tmplight2->ShadowSM->Init();
			tmplight2->ShadowSM->SetPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			tmplight2->ShadowSM->SetDirection(&tmplight->Direction);
			
			tmplight2->ShadowSM->SetBias(tmplight->ShadowSM->GetBias());
			tmplight2->ShadowSM->SetBlurPixel(tmplight->ShadowSM->GetBlurPixel());
			float3 anf;
			tmplight2->ShadowSM->GetAngleNearFar(&anf);
			tmplight2->ShadowSM->SetAngleNearFar(&anf);
		}
		else
			tmplight2->ShadowSM = 0;

		if (tmplight->ShadowCube)
		{
			tmplight2->ShadowCube = new ShadowMapCubeTech();
			tmplight2->ShadowCube->Init();
			tmplight2->ShadowCube->SetPosition(&float3(tmplight->Position.x, tmplight->Position.y, tmplight->Position.z));
			tmplight2->ShadowCube->SetNearFar(&float2(0.1f, tmplight->Dist));
			tmplight2->ShadowCube->SetNear(tmplight2->ShadowCube->GetNear());
			tmplight2->ShadowCube->SetFar(tmplight2->ShadowCube->GetFar());
			tmplight2->ShadowCube->SetBias(tmplight->ShadowCube->GetBias());
			tmplight2->ShadowCube->SetBlurPixel(tmplight->ShadowCube->GetBlurPixel());

			for (int i = 0; i < 6; ++i)
			{
				tmplight2->ShadowCube->SetEnableCubeEdge(i, tmplight2->ShadowCube->GetEnableCubeEdge(i));
			}
		}
		else
			tmplight2->ShadowCube = 0;

		tmplight->Mesh->CloneMeshFVF(tmplight->Mesh->GetOptions(), tmplight->Mesh->GetFVF(), MLSet::DXDevice, &(tmplight2->Mesh));
	}
	return -1;
}

Lights::Light::Light()
{
	TypeLight = LIGHTS_TYPE_NONE;
	Name[0] = 0;
	
	TypeShadowed = LIGHTS_TYPE_SHADOWED_STATIC;
	CountUpdate = 0;

	IsVisible = IsEnable = IsShadow = IsGlobal = false;

	Mesh = 0;
	Power = Dist = 0;
	GAngleX = GAngleY = 0;

	Angle = 0;

	BoundVolume = 0;
	WorldMat;
	MatRot;

	IsVisibleFor = false;
	DistFor = -1;

	ShadowPSSM = 0;
	ShadowSM = 0;
	ShadowCube = 0;

	Source = 0;
}

Lights::Light::~Light()
{
	mem_release_del(Mesh);
	mem_delete(Source);
	mem_release_del(BoundVolume);
	
	mem_delete(ShadowPSSM);
	mem_delete(ShadowSM);
	mem_delete(ShadowCube);
}

Lights::Light::MeshSource::MeshSource()
{
	Path[0]=0;
	Mesh = 0;
	IdGroupBind=-1;
	ArrTex = 0;
}

Lights::Light::MeshSource::~MeshSource()
{
	mem_release_del(Mesh);
	mem_delete_a(ArrTex);
}

void Lights::OnLostDevice()
{
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i]->ShadowSM)
			ArrLights[i]->ShadowSM->OnLostDevice();
		else if (ArrLights[i]->ShadowCube)
			ArrLights[i]->ShadowCube->OnLostDevice();
		else if (ArrLights[i]->ShadowPSSM)
			ArrLights[i]->ShadowPSSM->OnLostDevice();
	}
}

void Lights::OnResetDevice()
{
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i]->ShadowSM)
			ArrLights[i]->ShadowSM->OnResetDevice();
		else if (ArrLights[i]->ShadowCube)
			ArrLights[i]->ShadowCube->OnResetDevice();
		else if (ArrLights[i]->ShadowPSSM)
			ArrLights[i]->ShadowPSSM->OnResetDevice();

		SetNullLightCountUpdate(i);
	}
}

inline long Lights::AddLight(Light* obj)
{
	long idadd = -1;
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		if (ArrLights[i] == 0)
		{
			ArrLights[i] = obj;
			idadd = i;
			break;
		}
	}

	if (idadd == -1)
	{
		ArrLights.push_back(obj);
		idadd = ArrLights.size() - 1;
	}

	return idadd;
}

inline long Lights::GetCountLights()
{
	return ArrLights.size();
}

void Lights::Clear()
{
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		mem_delete(ArrLights[i]);
	}

	ArrLights.clear();
}

void Lights::DeleteLight(long id)
{
	LIGHTS_PRE_COND_ID(id);

	mem_delete(ArrLights[id]);
	ArrLights.erase(id);
}

char* Lights::GetLightName(long id)
{
	LIGHTS_PRE_COND_ID(id, 0);
	return ArrLights[id]->Name;
}

void Lights::SetLightName(long id, const char* name)
{
	LIGHTS_PRE_COND_ID(id);
	sprintf(ArrLights[id]->Name,"%s", name);
}

long Lights::CreatePoint(long id, float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed, const char* bound_volume)
{
	Light* tmplight = 0;// new Light();

	if (id == -1)
		tmplight = new Light();
	else
	{
		mem_delete(ArrLights[id]);
		ArrLights[id] = new Light();
		tmplight = ArrLights[id];
	}

	tmplight->IsGlobal = isglobal;

	if (isglobal)
	{
		tmplight->Dist = LIGHTS_GLOBAL_STD_RADIUS;
		tmplight->Power = LIGHTS_GLOBAL_MAX_POWER;
	}
	else
	{
		tmplight->Dist = dist;
		tmplight->Power = power;
	}

	tmplight->TypeLight = (isglobal ? LIGHTS_TYPE_GLOBAL : LIGHTS_TYPE_POINT);
		if(StrValidate(bound_volume))
		{
			
		}
		else
		{
			D3DXCreateSphere(MLSet::DXDevice, tmplight->Dist, 20, 20, &tmplight->Mesh, 0);
		}

		if (isglobal && is_shadowed)
		{
			tmplight->ShadowPSSM = new PSSM();
			tmplight->ShadowPSSM->Init();
			tmplight->IsShadow = true;
			tmplight->ShadowPSSM->SetPosition(&float3(center->x, center->y, center->z));
		}
		else if (is_shadowed)
		{
			tmplight->ShadowCube = new ShadowMapCubeTech();
			tmplight->ShadowCube->Init();
			tmplight->ShadowCube->SetPosition(&float3(center->x, center->y, center->z));
			tmplight->ShadowCube->SetNearFar(&float2(LIGHTS_LOCAL_STD_NEAR, tmplight->Dist));
			tmplight->IsShadow = true;

		}
		else
			tmplight->IsShadow = false;

	long tmpid = id;

	if (id == -1)
		tmpid = AddLight(tmplight);

	SetLightPos(tmpid, &float3(center->x, center->y, center->z), true);

	tmplight->Color = *color;
	tmplight->IsEnable = true;
	tmplight->BoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->Mesh->GetVertexBuffer(&vertexbuf);
	tmplight->BoundVolume->CalcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);

	return tmpid;
}

long Lights::CreateDirection(long id, float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow, const char* bound_volume)
{
	Light* tmplight = 0;

	if (id == -1)
		tmplight = new Light();
	else
	{
		mem_delete(ArrLights[id]);
		ArrLights[id] = new Light();
		tmplight = ArrLights[id];
	}

	tmplight->IsGlobal = false;
	tmplight->TypeLight = LIGHTS_TYPE_DIRECTION;
	tmplight->Angle = angle;
	float bottom_radius = top_radius + dist * tanf(angle / 2.f);
	tmplight->TopBottomRadius.x = top_radius;
	tmplight->TopBottomRadius.y = bottom_radius;

	if (StrValidate(bound_volume))
	{
		
	}
	else
		SGCore_FCreateCone(top_radius, bottom_radius, dist, &tmplight->Mesh, 32);

	tmplight->TopBottomRadius = float2_t(top_radius, bottom_radius);
	tmplight->Color = *color;
	tmplight->Direction = *dir;
	float3 f(0, -1, 0);
	float3 a = SMVector3Cross(f, *dir);
	float ang = acosf(SMVector3Dot(f, *dir));
	tmplight->MatRot = SMMatrixRotationAxis(a, ang);

	tmplight->Power = power;
	tmplight->Dist = dist;

	tmplight->Rotation = SMMatrixToEuler(tmplight->MatRot);
	tmplight->IsEnable = true;
	float4x4 mpos = SMMatrixTranslation(*pos);
	tmplight->WorldMat = tmplight->MatRot * mpos;

	tmplight->Position = float3(pos->x, pos->y, pos->z);
	//tmplight->Direction = *dir;
	
	if (is_shadow)
	{
		tmplight->ShadowSM = new ShadowMapTech();
		tmplight->ShadowSM->Init();
		tmplight->ShadowSM->SetPosition(&float3(pos->x, pos->y, pos->z));
		tmplight->ShadowSM->SetDirection(dir);
		tmplight->ShadowSM->SetAngleNearFar(&float3(angle, LIGHTS_LOCAL_STD_NEAR, dist));
	}
	tmplight->IsShadow = is_shadow;

	tmplight->BoundVolume = SGCore_CrBound();
	IDirect3DVertexBuffer9* vertexbuf;
	tmplight->Mesh->GetVertexBuffer(&vertexbuf);
	tmplight->BoundVolume->CalcBound(vertexbuf, tmplight->Mesh->GetNumVertices(), tmplight->Mesh->GetNumBytesPerVertex());
	mem_release(vertexbuf);

	long tmpid = id;

	if (id == -1)
		tmpid = AddLight(tmplight);
	
	return tmpid;
}

void Lights::LoadLightMeshSource(long id, const char* path)
{
	LIGHTS_PRE_COND_ID(id);

	if (!def_str_validate(path))
		return;

	mem_delete(ArrLights[id]->Source);

	ArrLights[id]->Source = new Light::MeshSource();

	char fullpath[1024];
	sprintf(fullpath, "%s%s", MLSet::StdPathMesh, path);
	SGCore_LoadStaticModel(fullpath, &(ArrLights[id]->Source->Mesh));

	if (ArrLights[id]->Source->Mesh)
	{
		sprintf(ArrLights[id]->Source->Path, "%s", path);

		char tmpnametex[1024];
		ArrLights[id]->Source->ArrTex = new DWORD[ArrLights[id]->Source->Mesh->SubsetCount];
		for (int i = 0; i < ArrLights[id]->Source->Mesh->SubsetCount; ++i)
		{
			sprintf(tmpnametex, "%s.dds", ArrLights[id]->Source->Mesh->ArrTextures[i]);
			ArrLights[id]->Source->ArrTex[i] = FuncLoadMaterial(tmpnametex);
		}
	}

	//ArrLights[id]->Source->Position = ArrLights[id]->Position;
	//ArrLights[id]->Source->WorldMat = SMMatrixTranslation(ArrLights[id]->Source->Position);

	BindLightToGroup(id, 0);
}

void Lights::BindLightToGroup(long id, int group)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrLights[id]->Source->Mesh->SubsetCount <= group)
		return;

	vertex_static* pData;
	ArrLights[id]->Source->Mesh->VertexBuffer->Lock(0, 0, (void**)&pData, 0);

	float3 tmpmin, tmpmax;
	tmpmin = pData[0].Pos;
	tmpmax = pData[0].Pos;
	long tmpindv = 0;
	for (long i = 0; i < ArrLights[id]->Source->Mesh->VertexCount[group]; ++i)
	{
		tmpindv = ArrLights[id]->Source->Mesh->StartVertex[group] + i;

		if (tmpmin.x > pData[tmpindv].Pos.x)
			tmpmin.x = pData[tmpindv].Pos.x;
		else if (tmpmin.y > pData[tmpindv].Pos.y)
			tmpmin.y = pData[tmpindv].Pos.y;
		else if (tmpmin.z > pData[tmpindv].Pos.z)
			tmpmin.z = pData[tmpindv].Pos.z;

		if (tmpmax.x < pData[tmpindv].Pos.x)
			tmpmax.x = pData[tmpindv].Pos.x;
		else if (tmpmax.y < pData[tmpindv].Pos.y)
			tmpmax.y = pData[tmpindv].Pos.y;
		else if (tmpmin.z < pData[tmpindv].Pos.z)
			tmpmax.z = pData[tmpindv].Pos.z;
	}

	ArrLights[id]->Source->Mesh->VertexBuffer->Unlock();

	ArrLights[id]->Source->LocalPos = ((tmpmin + tmpmax) * 0.5f);
	ArrLights[id]->Source->Position = ArrLights[id]->Position - ArrLights[id]->Source->LocalPos;
	ArrLights[id]->Source->WorldMatLocal = SMMatrixTranslation(ArrLights[id]->Source->Position);
	SetLightRot(id, &float3(0, 0, 0), true);
	ArrLights[id]->Source->IdGroupBind = group;
	SetNullLightCountUpdate(id);
}

long Lights::GetLightCountGroupMesh(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (!(ArrLights[id]->Source && ArrLights[id]->Source->Mesh))
		return -1;

	return ArrLights[id]->Source->Mesh->SubsetCount;
}

const char* Lights::GetLightNameGroupMesh(long id,int group)
{
	LIGHTS_PRE_COND_ID(id, 0);

	if (ArrLights[id]->Source && ArrLights[id]->Source->Mesh->SubsetCount <= group)
		return 0;

	return ArrLights[id]->Source->Mesh->ArrTextures[group];
}

const char* Lights::GetLightPathSource(long id)
{
	LIGHTS_PRE_COND_ID(id, 0);

	if (!(ArrLights[id]->Source))
		return 0;

	return ArrLights[id]->Source->Path;
}

long Lights::GetLightBindedGroupSource(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->Source)
		return 0;

	return ArrLights[id]->Source->IdGroupBind;
}

void Lights::RenderSource(long id, bool render_bind_group, DWORD timeDelta)
{
	LIGHTS_PRE_COND_ID(id);

		if (!(ArrLights[id]->Source && ArrLights[id]->Source->Mesh))
			return;

		float4x4 tmpwmat = ArrLights[id]->Source->WorldMat;
		MLSet::DXDevice->SetTransform(D3DTS_WORLD, &((D3DXMATRIX)ArrLights[id]->Source->WorldMat));
		D3DXMATRIX tmpmat;
		MLSet::DXDevice->GetTransform(D3DTS_WORLD1, &(tmpmat));
		MLSet::DXDevice->SetVertexShaderConstantF(0, (float*)&(SMMatrixTranspose(ArrLights[id]->Source->WorldMat * float4x4(tmpmat))), 4);

		MLSet::DXDevice->SetStreamSource(0, ArrLights[id]->Source->Mesh->VertexBuffer, 0, sizeof(vertex_static));
		MLSet::DXDevice->SetIndices(ArrLights[id]->Source->Mesh->IndexBuffer);
		MLSet::DXDevice->SetVertexDeclaration(VertexDeclarationStatic);
		
		for (int i = 0; i < ArrLights[id]->Source->Mesh->SubsetCount; ++i)
		{
			if (!render_bind_group && i == ArrLights[id]->Source->IdGroupBind)
				continue;
			FuncSetMaterial(ArrLights[id]->Source->ArrTex[i], &(ArrLights[id]->Source->WorldMat));
			FuncDIP(MLSet::DXDevice,D3DPT_TRIANGLELIST, 0, 0, ArrLights[id]->Source->Mesh->VertexCount[i], ArrLights[id]->Source->Mesh->StartIndex[i], ArrLights[id]->Source->Mesh->IndexCount[i]);
		}
}

void Lights::RenderSourceAll(bool render_bind_group, DWORD timeDelta)
{
	for (long id = 0; id < ArrLights.size();++id)
	{
		if (!(ArrLights[id]->Source && ArrLights[id]->Source->Mesh))
			return;

		RenderSource(id, render_bind_group, timeDelta);
	}
}

void Lights::RenderSourceAllExceptGroup(long id, DWORD timeDelta)
{
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		if (!(ArrLights[i]->Source && ArrLights[i]->Source->Mesh))
			return;

		RenderSource(i, (i == id ? false : true), timeDelta);
	}
}


void Lights::Render(long id,DWORD timeDelta)
{
	LIGHTS_PRE_COND_ID(id);

	MLSet::DXDevice->SetTransform(D3DTS_WORLD, &(ArrLights[id]->WorldMat.operator D3DXMATRIX()));
	ArrLights[id]->Mesh->DrawSubset(0);
}

void Lights::GetLightColor(long id,float3* vec)
{
	LIGHTS_PRE_COND_ID(id);

	(*vec) = ArrLights[id]->Color;
}

void Lights::SetLightColor(long id, float3* vec)
{
	LIGHTS_PRE_COND_ID(id);

	ArrLights[id]->Color = *vec;
	SetNullLightCountUpdate(id);
}

void Lights::GetLightPos(long id, float3* vec, bool for_mesh, bool greal)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrLights[id]->IsGlobal && greal)
		{
			vec->x = ArrLights[id]->GAngleX;
			vec->y = ArrLights[id]->GAngleY;
			vec->z = 0;
		}
		else
		{
			if (!for_mesh)
			{
				vec->x = ArrLights[id]->Position.x;
				vec->y = ArrLights[id]->Position.y;
				vec->z = ArrLights[id]->Position.z;
			}
			else if (ArrLights[id]->Source && ArrLights[id]->Source->IdGroupBind > -1)
			{
				vec->x = ArrLights[id]->Source->Position.x;// +ArrLights[id]->Source->LocalPos.x;
				vec->y = ArrLights[id]->Source->Position.y;// +ArrLights[id]->Source->LocalPos.y;
				vec->z = ArrLights[id]->Source->Position.z;// +ArrLights[id]->Source->LocalPos.z;
			}
		}
}

float Lights::GetLightPower(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return ArrLights[id]->Power;
}

float Lights::GetLightPowerDiv(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return sqrtf(ArrLights[id]->Power / LIGHTS_GLOBAL_MAX_POWER);
}

void Lights::SetLightPower(long id, float power)
{
	LIGHTS_PRE_COND_ID(id);

	ArrLights[id]->Power = power;
	SetNullLightCountUpdate(id);
}

float Lights::GetLightDist(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return ArrLights[id]->Dist;
}

void Lights::SetLightPos(long id, float3* vec, bool greal)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrLights[id]->IsGlobal)
		{
			ArrLights[id]->GAngleX = vec->x;
			ArrLights[id]->GAngleY = vec->y;

			if (ArrLights[id]->GAngleX > 360 || ArrLights[id]->GAngleX < 0)
				ArrLights[id]->GAngleX = 0;

			if (ArrLights[id]->GAngleY > 360 || ArrLights[id]->GAngleY < 0)
				ArrLights[id]->GAngleY = 0;


			float4x4 mat = SMMatrixRotationZ(-D3DXToRadian(ArrLights[id]->GAngleX)) * SMMatrixRotationY(D3DXToRadian(ArrLights[id]->GAngleY));
			ArrLights[id]->Position = SMVector3Transform(float3(-1, 0, 0), mat);


			ArrLights[id]->Position.x *= LIGHTS_POS_G_MAX;
			ArrLights[id]->Position.y *= LIGHTS_POS_G_MAX;
			ArrLights[id]->Position.z *= LIGHTS_POS_G_MAX;

			ArrLights[id]->WorldMat = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
			if (ArrLights[id]->ShadowPSSM)
				ArrLights[id]->ShadowPSSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
		}
		else
		{
			/*if (for_mesh)
			{
				if (ArrLights[id]->Source && ArrLights[id]->Source->IdGroupBind > -1)
				{
					ArrLights[id]->Source->Position.x = (*vec).x;
					ArrLights[id]->Source->Position.y = (*vec).y;
					ArrLights[id]->Source->Position.z = (*vec).z;

					ArrLights[id]->Source->Position = (*vec) - ArrLights[id]->Source->LocalPos;

					float determ = 0;
					ArrLights[id]->Source->WorldMatLocal = ArrLights[id]->Source->MatRot * SMMatrixTranslation(ArrLights[id]->Source->Position);
					ArrLights[id]->Source->WorldMat = ArrLights[id]->WorldMat * ArrLights[id]->Source->WorldMatLocal;


					ArrLights[id]->Position.x = (*vec).x - ArrLights[id]->Source->LocalPos.x;
					ArrLights[id]->Position.y = (*vec).y - ArrLights[id]->Source->LocalPos.y;
					ArrLights[id]->Position.z = (*vec).z - ArrLights[id]->Source->LocalPos.z;

					float4x4 mpos = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
					ArrLights[id]->WorldMat = ArrLights[id]->MatRot * mpos;
				}
			}
			else
			{*/
				ArrLights[id]->Position.x = (*vec).x;
				ArrLights[id]->Position.y = (*vec).y;
				ArrLights[id]->Position.z = (*vec).z;

				float4x4 mpos = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
				ArrLights[id]->WorldMat = ArrLights[id]->MatRot * mpos;

				if (ArrLights[id]->Source && ArrLights[id]->Source->IdGroupBind > -1)
				{
					float determ = 0;
					ArrLights[id]->Source->Position = ArrLights[id]->Position - ArrLights[id]->Source->LocalPos;
					ArrLights[id]->Source->WorldMatLocal = SMMatrixTranslation(ArrLights[id]->Source->Position);
					ArrLights[id]->Source->WorldMat = ArrLights[id]->Source->MatRot * ArrLights[id]->Source->WorldMatLocal;
				}
			//}

			if (ArrLights[id]->ShadowSM)
			{
				ArrLights[id]->ShadowSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}

			if (ArrLights[id]->ShadowCube)
			{
				ArrLights[id]->WorldMat = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
				ArrLights[id]->ShadowCube->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
			}
		}

		SetNullLightCountUpdate(id);
}


void Lights::GetLightRot(long id, float3* vec, bool rot_mesh)
{
	LIGHTS_PRE_COND_ID(id);

	if (!rot_mesh)
		*vec = ArrLights[id]->Rotation;
	else if (ArrLights[id]->Source)
		*vec = ArrLights[id]->Source->Rotation;
}

void Lights::SetLightRot(long id, float3* vec, bool rot_mesh)
{
	LIGHTS_PRE_COND_ID(id);

		float3 tmpdir = SMEulerToVec((*vec));

		float3 f(0, -1, 0);
		float3 a = SMVector3Cross(f, tmpdir);
		float ang = acosf(SMVector3Dot(f, tmpdir));
		float4x4 tmpmatrot = SMMatrixRotationAxis(a, ang);

		Light* tmpl = ArrLights[id];
		float determ = 0;

		if (rot_mesh && ArrLights[id]->Source && ArrLights[id]->Source->IdGroupBind > -1)
		{
			tmpl->Source->Rotation = (*vec);
			tmpl->Source->MatRot = tmpmatrot;
			tmpl->MatRot = tmpl->Source->MatRot * tmpl->MatRotL;
			ArrLights[id]->Rotation = SMMatrixToEuler(tmpl->MatRot);
			float4x4 mpos = SMMatrixTranslation(tmpl->Position.x, tmpl->Position.y, tmpl->Position.z);
			tmpl->WorldMat = tmpl->MatRot * mpos;
			tmpl->Direction = SMEulerToVec(ArrLights[id]->Rotation);
			tmpl->Source->WorldMat = tmpl->Source->MatRot * tmpl->Source->WorldMatLocal;
		}
		else if (ArrLights[id]->Source && ArrLights[id]->Source->IdGroupBind > -1)
		{
			ArrLights[id]->Rotation = *vec;
			ArrLights[id]->Direction = tmpdir;

			tmpl->MatRot = tmpmatrot;
			tmpl->MatRotL = SMMatrixInverse(&determ, tmpl->Source->MatRot) * tmpl->MatRot;
			tmpl->MatRot = tmpl->Source->MatRot * tmpl->MatRotL;
			float4x4 mpos = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
			ArrLights[id]->WorldMat = tmpmatrot * mpos;
		}
		else
		{
			ArrLights[id]->Rotation = *vec;
			ArrLights[id]->Direction = tmpdir;

			tmpl->MatRot = tmpmatrot;
			//tmpl->MatRotL = SMMatrixIdentity() * tmpl->MatRot;
			//tmpl->MatRot = SMMatrixIdentity() * tmpl->MatRotL;
			float4x4 mpos = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
			ArrLights[id]->WorldMat = tmpl->MatRot * mpos;
		}
		

		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetDirection(&ArrLights[id]->Direction);

		SetNullLightCountUpdate(id);
}

void Lights::GetLightDir(long id, float3* vec, bool rot_mesh)
{
	LIGHTS_PRE_COND_ID(id);

	if (rot_mesh && ArrLights[id]->Source && ArrLights[id]->Source->IdGroupBind > -1)
		*vec = ArrLights[id]->Source->Direction;
	else
		*vec = ArrLights[id]->Direction;
}

void Lights::SetLightDir(long id, float3* vec, bool rot_mesh)
{
	LIGHTS_PRE_COND_ID(id);

		float3 tmpdir = *vec;

		float3 f(0, -1, 0);
		float3 a = SMVector3Cross(f, tmpdir);
		float ang = acosf(SMVector3Dot(f, tmpdir));
		//ArrLights[id]->MatRot = SMMatrixRotationAxis(a, ang);

		//ArrLights[id]->Rotation = SMMatrixToEuler(ArrLights[id]->MatRot);
		SetLightRot(id, &SMMatrixToEuler(SMMatrixRotationAxis(a, ang)), rot_mesh);
		/*float4x4 mpos = SMMatrixTranslation(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z);
		ArrLights[id]->WorldMat = ArrLights[id]->MatRot * mpos;

		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetDirection(&ArrLights[id]->Direction);

		SetNullLightCountUpdate(id);*/
}

void Lights::SetLightBias(long id, float val)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetBias(val);
		else if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SetBias(val);

		SetNullLightCountUpdate(id);
}

float Lights::GetLightBias(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->ShadowSM->GetBias();
	else if (ArrLights[id]->ShadowCube)
		return ArrLights[id]->ShadowCube->GetBias();
}

float Lights::GetLightTopRadius(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->TopBottomRadius.x;
}

float Lights::GetLightAngle(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->Angle;
}



bool Lights::ComVisibleForFrustum(long id, ISXFrustum* frustum)
{
	LIGHTS_PRE_COND_ID(id, false);

	float3 tmpcenter;
	float tmpradius;
	ArrLights[id]->BoundVolume->GetSphere(&tmpcenter, &tmpradius);
	tmpcenter = SMVector3Transform(tmpcenter, ArrLights[id]->WorldMat);

	return frustum->SphereInFrustum(&tmpcenter, tmpradius);
}

bool Lights::GetVisibleForFrustum(long id)
{
	LIGHTS_PRE_COND_ID(id, false);

	return ArrLights[id]->IsVisibleFor;
}

float Lights::ComDistFor(long id, float3* vec)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return SMVector3Distance((float3)ArrLights[id]->Position, *vec);
}

void Lights::ComVisibleFrustumDistFor(ISXFrustum* frustum, float3* vec)
{
	for (long i = 0; i < ArrLights.size(); ++i)
	{
		ArrLights[i]->DistFor = SMVector3Distance((float3)ArrLights[i]->Position, *vec);
		ArrLights[i]->IsVisibleFor = ComVisibleForFrustum(i, frustum);
	}
}

float Lights::GetDistFor(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	return ArrLights[id]->DistFor;
}

IDirect3DTexture9* Lights::GetShadow2()
{
	return SGCore_RTGetTexture(ShadowMap);
}

bool Lights::IsEnable(long id)
{
	LIGHTS_PRE_COND_ID(id, false);

	return ArrLights[id]->IsEnable;
}

void Lights::SetEnable(long id, bool val)
{
	LIGHTS_PRE_COND_ID(id);

	ArrLights[id]->IsEnable = val;
	SetNullLightCountUpdate(id);
}

bool Lights::IsShadow(long id)
{
	LIGHTS_PRE_COND_ID(id, false);

	return ArrLights[id]->IsShadow;
}

int Lights::GetType(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowPSSM)
		return LIGHTS_TYPE_GLOBAL;
	else if (ArrLights[id]->ShadowSM)
		return LIGHTS_TYPE_DIRECTION;
	else if (ArrLights[id]->ShadowCube)
		return LIGHTS_TYPE_POINT;
}

void Lights::InRenderBegin(long id)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->Begin();
	else if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->Begin();
	else if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->Begin();
}

void Lights::InRenderEnd(long id)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->End();
	else if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->End();
	else if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->End();
}

void Lights::InRenderPre(long id, int cube)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->Pre(cube);
	else if (ArrLights[id]->ShadowPSSM && cube >= 0 && cube < 5)
		ArrLights[id]->ShadowPSSM->PreRender(cube);
}

void Lights::InRenderPost(long id, int cube)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->Post(cube);
}

ISXFrustum* Lights::GetFrustum(long id, int how)
{
	LIGHTS_PRE_COND_ID(id, 0);

		if (ArrLights[id]->ShadowSM)
		{
			if (how == 0)
				return ArrLights[id]->ShadowSM->Frustum;
		}
		else if (ArrLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				return ArrLights[id]->ShadowCube->Frustums[how];
		}
		else if (ArrLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return ArrLights[id]->ShadowPSSM->Frustums[how];
		}
}

ISXFrustum* Lights::GetFrustumG(long id, int split)
{
	LIGHTS_PRE_COND_ID(id, 0);

	if (ArrLights[id]->ShadowPSSM && split >= 0 && split < 4 && ArrLights[id]->ShadowPSSM->Frustums[split])
		return ArrLights[id]->ShadowPSSM->Frustums[split];
}

void Lights::UpdateFrustumsG(long id, int split, float3* pos, float3* dir)
{
	LIGHTS_PRE_COND_ID(id);

	ArrLights[id]->ShadowPSSM->UpdateFrustums(split, pos, dir);
}

void Lights::GenShadow2(long id)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->GenShadow2(SGCore_RTGetTexture(ShadowMap), sqrtf(ArrLights[id]->Power / LIGHTS_GLOBAL_MAX_POWER), &(ArrLights[id]->Position));
	else if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->GenShadow2(SGCore_RTGetTexture(ShadowMap), sqrtf(ArrLights[id]->Power / LIGHTS_GLOBAL_MAX_POWER), &(ArrLights[id]->Position));
	else if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->GenShadow2(SGCore_RTGetTexture(ShadowMap), sqrtf(ArrLights[id]->Power / LIGHTS_GLOBAL_MAX_POWER), &(ArrLights[id]->Position));
}

void Lights::NullingShadow()
{
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	SGCore_RTGetTexture(ShadowMap)->GetSurfaceLevel(0, &RenderSurf);
	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, RenderSurf);

	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

void Lights::ChangeAngle(long id, float angle, bool is_create)
{
	LIGHTS_PRE_COND_ID(id);

		ArrLights[id]->Angle = angle;
		if (ArrLights[id]->TypeLight == LIGHTS_TYPE_DIRECTION && is_create)
		{
			mem_release_del(ArrLights[id]->Mesh);
			ArrLights[id]->TopBottomRadius.y = ArrLights[id]->TopBottomRadius.x + ArrLights[id]->Dist * tanf(angle / 2.f);
			ArrLights[id]->Angle = angle;
			SGCore_FCreateCone(ArrLights[id]->TopBottomRadius.x, ArrLights[id]->TopBottomRadius.y, ArrLights[id]->Dist, &ArrLights[id]->Mesh, 32);
			//ArrLights[id]->PathVolume[0] = 0;
		}

		if (ArrLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrLights[id]->BoundVolume->CalcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release(vertexbuf);
		}

		if (ArrLights[id]->TypeLight == LIGHTS_TYPE_DIRECTION && ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetAngleNearFar(&float3(angle, 0.1, ArrLights[id]->Dist));

		SetNullLightCountUpdate(id);
}

void Lights::ChangeTopRadius(long id, float top_radius)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrLights[id]->TypeLight == 1)
		{
			mem_release_del(ArrLights[id]->Mesh);
			ArrLights[id]->TopBottomRadius.x = top_radius;
			SGCore_FCreateCone(top_radius, ArrLights[id]->TopBottomRadius.y, ArrLights[id]->Dist, &ArrLights[id]->Mesh, 32);
			//ArrLights[id]->PathVolume[0] = 0;
		}

		if (ArrLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrLights[id]->BoundVolume->CalcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release_del(vertexbuf);
		}

		SetNullLightCountUpdate(id);
}

void Lights::ChangeRadiusHeight(long id, float radius_height, bool is_create)
{
	LIGHTS_PRE_COND_ID(id);

		ArrLights[id]->Dist = radius_height;
		if (ArrLights[id]->TypeLight == LIGHTS_TYPE_DIRECTION && is_create)
		{
			ArrLights[id]->TopBottomRadius.y = ArrLights[id]->TopBottomRadius.x + radius_height * tanf(ArrLights[id]->Angle / 2.f);
			mem_release_del(ArrLights[id]->Mesh);
			SGCore_FCreateCone(ArrLights[id]->TopBottomRadius.x, ArrLights[id]->TopBottomRadius.y, radius_height, &ArrLights[id]->Mesh, 32);
			//PathVolume[0] = 0;
		}

		if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetAngleNearFar(&float3(ArrLights[id]->Angle, 0.1, ArrLights[id]->Dist));

		if (ArrLights[id]->TypeLight == LIGHTS_TYPE_POINT && is_create)
		{
			mem_release_del(ArrLights[id]->Mesh);
			D3DXCreateSphere(MLSet::DXDevice, radius_height, 20, 20, &ArrLights[id]->Mesh, 0);
			//PathVolume[0] = 0;
		}

		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SetNearFar(&float2(0.1, ArrLights[id]->Dist));

		if (ArrLights[id]->Mesh)
		{
			IDirect3DVertexBuffer9* vertexbuf;
			ArrLights[id]->Mesh->GetVertexBuffer(&vertexbuf);
			ArrLights[id]->BoundVolume->CalcBound(vertexbuf, ArrLights[id]->Mesh->GetNumVertices(), ArrLights[id]->Mesh->GetNumBytesPerVertex());
			mem_release_del(vertexbuf);
		}

		SetNullLightCountUpdate(id);
}

void Lights::ChangeShadow(long id, bool is_shadow)
{
	LIGHTS_PRE_COND_ID(id);

		ArrLights[id]->IsShadow = is_shadow;
		if (ArrLights[id]->TypeLight == LIGHTS_TYPE_DIRECTION)
		{
			if (!ArrLights[id]->ShadowSM && ArrLights[id]->IsShadow)
			{
				ArrLights[id]->ShadowSM = new ShadowMapTech();
				ArrLights[id]->ShadowSM->Init();
				ArrLights[id]->ShadowSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
				ArrLights[id]->ShadowSM->SetDirection(&ArrLights[id]->Direction);
				ArrLights[id]->ShadowSM->SetAngleNearFar(&float3(ArrLights[id]->Angle, 0.1, ArrLights[id]->Dist));
			}
		}
		else if (ArrLights[id]->TypeLight == LIGHTS_TYPE_GLOBAL)
		{
			if (ArrLights[id]->IsGlobal)
			{
				if (!ArrLights[id]->ShadowPSSM && ArrLights[id]->IsShadow)
				{
					ArrLights[id]->ShadowPSSM = new PSSM();
					ArrLights[id]->ShadowPSSM->Init();
					ArrLights[id]->ShadowPSSM->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
				}
			}
		}
		else if (ArrLights[id]->TypeLight == LIGHTS_TYPE_POINT)
		{
			if (!ArrLights[id]->ShadowCube && ArrLights[id]->IsShadow)
			{
				ArrLights[id]->ShadowCube = new ShadowMapCubeTech();
				ArrLights[id]->ShadowCube->Init();
				ArrLights[id]->ShadowCube->SetPosition(&float3(ArrLights[id]->Position.x, ArrLights[id]->Position.y, ArrLights[id]->Position.z));
				ArrLights[id]->ShadowCube->SetNearFar(&float2(0.1f, ArrLights[id]->Dist));
			}
		}
		SetNullLightCountUpdate(id);
}

void Lights::SetBlurPixel(long id, float blur_pixel)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrLights[id]->ShadowCube)
			ArrLights[id]->ShadowCube->SetBlurPixel(blur_pixel);
		else if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetBlurPixel(blur_pixel);
		else if (ArrLights[id]->ShadowPSSM)
			ArrLights[id]->ShadowPSSM->SetBlurPixel(blur_pixel);

		SetNullLightCountUpdate(id);
}

float Lights::GetBlurPixel(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (ArrLights[id]->ShadowCube)
			return ArrLights[id]->ShadowCube->GetBlurPixel();
		else if (ArrLights[id]->ShadowSM)
			return ArrLights[id]->ShadowSM->GetBlurPixel();
		else if (ArrLights[id]->ShadowPSSM)
			return ArrLights[id]->ShadowPSSM->GetBlurPixel();
}

void Lights::SetShadowLocalNear(long id, float slnear)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->SetNear(slnear);
	else if (ArrLights[id]->ShadowSM)
		ArrLights[id]->ShadowSM->SetNear(slnear);

	SetNullLightCountUpdate(id);
}

float Lights::GetShadowLocalNear(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	if (ArrLights[id]->ShadowCube)
		return ArrLights[id]->ShadowCube->GetNear();
	else if (ArrLights[id]->ShadowSM)
		return ArrLights[id]->ShadowSM->GetNear();
}

void Lights::SetShadowLocalFar(long id, float slfar)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrLights[id]->ShadowCube)
		{
			float2 tmpnf;
			ArrLights[id]->ShadowCube->GetNearFar(&tmpnf);
			tmpnf.y = slfar;
			ArrLights[id]->ShadowCube->SetNearFar(&tmpnf);
		}
		else if (ArrLights[id]->ShadowSM)
			ArrLights[id]->ShadowSM->SetFar(slfar);


		SetNullLightCountUpdate(id);
}

float Lights::GetShadowLocalFar(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (ArrLights[id]->ShadowCube)
		{
			float2 tmpnf;
			ArrLights[id]->ShadowCube->GetNearFar(&tmpnf);
			return tmpnf.y;
		}
		else if (ArrLights[id]->ShadowSM)
			return ArrLights[id]->ShadowSM->GetFar();
		else
			return ArrLights[id]->Dist;
}

void Lights::SetEnableCubeEdge(long id, int edge, bool enable)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrLights[id]->ShadowCube)
		ArrLights[id]->ShadowCube->SetEnableCubeEdge(edge, enable);

	SetNullLightCountUpdate(id);
}

bool Lights::GetEnableCubeEdge(long id, int edge)
{
	LIGHTS_PRE_COND_ID(id, false);

	if (ArrLights[id]->ShadowCube)
		return ArrLights[id]->ShadowCube->GetEnableCubeEdge(edge);
}

long Lights::GetLightIDArr(long id, long inid, int how)
{
	LIGHTS_PRE_COND_ID(id, -1);

		if (ArrLights[id]->ShadowSM)
		{
			if (how == 0)
				return ArrLights[id]->ShadowSM->GetIDArr(inid);
		}
		else if (ArrLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				return ArrLights[id]->ShadowCube->GetIDArr(inid, how);
		}
		else if (ArrLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				return ArrLights[id]->ShadowPSSM->GetIDArr(inid, how);
		}
}

void Lights::SetLightIDArr(long id, long inid, int how, long id_arr)
{
	LIGHTS_PRE_COND_ID(id);

		if (ArrLights[id]->ShadowSM)
		{
			if (how == 0)
				ArrLights[id]->ShadowSM->SetIDArr(inid, id_arr);
		}
		else if (ArrLights[id]->ShadowCube)
		{
			if (how >= 0 && how < 6)
				ArrLights[id]->ShadowCube->SetIDArr(inid, how, id_arr);
		}
		else if (ArrLights[id]->ShadowPSSM)
		{
			if (how >= 0 && how < 5)
				ArrLights[id]->ShadowPSSM->SetIDArr(inid, how, id_arr);
		}
}

void Lights::SetLightTypeShadowed(long id, int type)
{
	LIGHTS_PRE_COND_ID(id);

	ArrLights[id]->TypeShadowed = type;

	SetNullLightCountUpdate(id);
}

int Lights::GetLightTypeShadowed(long id)
{
	LIGHTS_PRE_COND_ID(id, -1);

	ArrLights[id]->CountUpdate = 0;
	return ArrLights[id]->TypeShadowed;
}


bool Lights::UpdateLightCountUpdate(long id, float3* viewpos, int ghow)
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			Light* tmpl = ArrLights[id];
			if (tmpl->ShadowPSSM)
			{
				if (ghow >= 0 && ghow < 4)
				{
					if (tmpl->ShadowPSSM->IsUpdate[ghow] >= ghow*LIGHTS_UPDATE_PSSM_SPLIT)
						tmpl->ShadowPSSM->IsUpdate[ghow] = -1;
				}
				else
				{
					if (tmpl->ShadowPSSM->IsUpdate[ghow] >= LIGHTS_UPDATE_PSSM_SPLIT_5)
						tmpl->ShadowPSSM->IsUpdate[ghow] = -1;
					
				}

				++(tmpl->ShadowPSSM->IsUpdate[ghow]);
				
				return (tmpl->ShadowPSSM->IsUpdate[ghow] == 0);
			}
		}

		else if (ArrLights[id]->TypeShadowed == LIGHTS_TYPE_SHADOWED_STATIC)
		{
			if (ArrLights[id]->CountUpdate < LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC)
			{
				++(ArrLights[id]->CountUpdate);
				return true;
			}
			else
				return false;
		}
		else
		{
			Light* tmpl = ArrLights[id];
			float dist = SMVector3Distance(tmpl->Position, (*viewpos));
			if (dist < LIGHTS_UPDATE_L0_DIST)
				tmpl->CountUpdate = -1;
			else if (dist < LIGHTS_UPDATE_L1_DIST && dist > LIGHTS_UPDATE_L0_DIST)
			{
				if (tmpl->CountUpdate > 1)
					tmpl->CountUpdate = -1;
			}
			else if (dist < LIGHTS_UPDATE_L2_DIST && dist > LIGHTS_UPDATE_L1_DIST)
			{
				if (tmpl->CountUpdate > 2)
					tmpl->CountUpdate = -1;
			}
			else if (dist > LIGHTS_UPDATE_L2_DIST)
			{
				tmpl->CountUpdate = 4;
			}

			++(tmpl->CountUpdate);

			return (tmpl->CountUpdate == 0);
		}
}

bool Lights::AllowedRenderLight(long id, int ghow)
{
	LIGHTS_PRE_COND_ID(id, false);

		if (ghow >= 0 && ghow < 5)
		{
			if (ArrLights[id]->ShadowPSSM)
			{
				return (ArrLights[id]->ShadowPSSM->IsUpdate[ghow] == 0);
			}
		}
		else if (ArrLights[id]->TypeShadowed == LIGHTS_TYPE_SHADOWED_STATIC)
		{
			if (ArrLights[id]->CountUpdate >= LIGHTS_UPDATE_MAX_COUNT_FOR_STATIC)
				return false;
		}
		else
		{
			return (ArrLights[id]->CountUpdate == 0);
		}
}

void Lights::SetNullLightCountUpdate(long id)
{
	if (id >= 0 && id < ArrLights.size())
	{
		ArrLights[id]->CountUpdate = 0;
	}
	else if (id < 0)
	{
		for (long i = 0; i < ArrLights.size(); ++i)
		{
			ArrLights[i]->CountUpdate = 0;
		}
	}
}

void Lights::SoftShadow(bool randomsam, float size, bool isfirst)
{
	if (isfirst)
		HowShadow = 0;

	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(HowShadow == 0)
			SGCore_RTGetTexture(ShadowMap2)->GetSurfaceLevel(0, &RenderSurf);
		else
			SGCore_RTGetTexture(ShadowMap)->GetSurfaceLevel(0, &RenderSurf);

	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0,RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);

	MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::DepthScene));
	
		if(HowShadow == 0)
			MLSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(ShadowMap));
		else
			MLSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(ShadowMap2));
	
	SGCore_ShaderBind(0,MLSet::IDsShaders::VS::PPQuadRender);

		if(randomsam)
		{
			SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
			SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);
			MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise);
		}
		else
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::PPBlurDepthBased);
	
		if(randomsam)
		{
			SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise, "PixelSize", &float2_t(size / MLSet::WinSize.x, size / MLSet::WinSize.x));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise,"NearFar",&MLSet::NearFar);
		}
		else
		{
			SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::PPBlurDepthBased, "PixelSize", &float2_t(size / MLSet::WinSize.x, size / MLSet::WinSize.x));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBased,"NearFar",&MLSet::NearFar);
		}

	
	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	MLSet::DXDevice->SetRenderTarget(0,BackBuf);
	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);

		if(HowShadow == 1)
			HowShadow = 0;
		else
			HowShadow = 1;
}

void Lights::ComHDR(DWORD timeDelta, float factor_adapted)
{
	MLSet::GetArrDownScale4x4(MLSet::WinSize.x, MLSet::WinSize.y, MLSet::HDRSampleOffsets);

	LPDIRECT3DSURFACE9 SurfSceneScale, BackBuf;

	SGCore_RTGetTexture(MLSet::IDsRenderTargets::LigthComScaled)->GetSurfaceLevel(0, &SurfSceneScale);

	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, SurfSceneScale);

	SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, MLSet::IDsShaders::PS::SampleLumIterative);
	SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::SampleLumIterative, "ArrOffsets", &(MLSet::HDRSampleOffsets));

	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::LigthCom));
	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();

	mem_release(SurfSceneScale);


	long CurrTexture = HDR_NUM_TONEMAP_TEXTURES - 1;

	PDIRECT3DSURFACE9 SurfToneMap[HDR_NUM_TONEMAP_TEXTURES] = { 0 };

	for (int i = 0; i < HDR_NUM_TONEMAP_TEXTURES; i++)
	{
		SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[i])->GetSurfaceLevel(0, &SurfToneMap[i]);
	}

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[CurrTexture])->GetLevelDesc(0, &desc);

	MLSet::GetArrDownScale4x4(desc.Width, desc.Height, MLSet::HDRSampleOffsets);

	MLSet::DXDevice->SetRenderTarget(0, SurfToneMap[CurrTexture]);
	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::LigthComScaled));
	MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	MLSet::DXDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	MLSet::DXDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, MLSet::IDsShaders::PS::SampleLumInit);
	SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::SampleLumInit, "ArrOffsets", &(MLSet::HDRSampleOffsets));

	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();
	mem_release(SurfToneMap[CurrTexture]);

	--CurrTexture;

	while (CurrTexture >= 0)
	{
		SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[CurrTexture + 1])->GetLevelDesc(0, &desc);
		MLSet::GetArrDownScale4x4(desc.Width, desc.Height, MLSet::HDRSampleOffsets);

		MLSet::DXDevice->SetRenderTarget(0, SurfToneMap[CurrTexture]);
		MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

		SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(1, MLSet::IDsShaders::PS::SampleLumIterative);
		SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::SampleLumIterative, "ArrOffsets", &(MLSet::HDRSampleOffsets));

		MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[CurrTexture + 1]));
		SGCore_ScreenQuadDraw();

		MLSet::DXDevice->SetVertexShader(0);
		MLSet::DXDevice->SetPixelShader(0);

		SGCore_ShaderUnBind();
		CurrTexture--;
	}

	for (int i = 0; i < HDR_NUM_TONEMAP_TEXTURES; i++)
	{
		mem_release(SurfToneMap[i]);
	}

	MLSet::IDsRenderTargets::IncrAdaptedLum();
	LPDIRECT3DSURFACE9 SurfAdaptedLum = NULL;
	SGCore_RTGetTexture(MLSet::IDsRenderTargets::GetCurrAdaptedLum())->GetSurfaceLevel(0, &SurfAdaptedLum);

	MLSet::DXDevice->SetRenderTarget(0, SurfAdaptedLum);
	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::GetLastAdaptedLum()));
	MLSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[0]));
	MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	MLSet::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	MLSet::DXDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	MLSet::DXDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, MLSet::IDsShaders::PS::CalcAdaptedLum);

	float ElapsedTime = float(timeDelta) * 0.001f * factor_adapted;
	SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::CalcAdaptedLum, "ElapsedTime", &(ElapsedTime));

	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();
	mem_release(SurfAdaptedLum);

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);
}

void Lights::Set4Or3Splits(long id, bool is4)
{
	LIGHTS_PRE_COND_ID(id);

	if (ArrLights[id]->ShadowPSSM)
		ArrLights[id]->ShadowPSSM->Set4Or3Splits(is4);
}

bool Lights::Get4Or3Splits(long id)
{
	LIGHTS_PRE_COND_ID(id, false);

	if (ArrLights[id]->ShadowPSSM)
		return ArrLights[id]->ShadowPSSM->Get4Or3Splits();

	return false;
}