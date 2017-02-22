
#define SXMATERIAL_LIGTH_VERSION 1

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

#include <mtl_ligth\\ml_data.cpp>
#include <mtl_ligth\material_ligth.h>
#include <mtl_ligth\\light.cpp>
#include <mtl_ligth\\material.cpp>

Lights* ArrLights = 0;
Materials* ArrMaterials = 0;
Reflection* Reflect = 0;

#define ML_PRECOND(retval) if(!ArrLights){reportf(-1, "%s - sxmaterial_ligth is not init", gen_msg_location); return retval;}

long SML_0GetVersion()
{
	return SXMATERIAL_LIGTH_VERSION;
}

void SML_Dbg_Set(report_func rf)
{
	reportf = rf;
}

void SML_0Create(const char* name, IDirect3DDevice9* device, const char* std_path_material, const char* std_path_mesh, float2_t* winsize, float projfov, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				reportf(-1, "%s - none unic name, sxmaterial_ligth", gen_msg_location);
			}
			else
			{
				MLInit(device, std_path_material, std_path_mesh, winsize, projfov);
				MLSet::ReCalcSize();
				ArrLights = new Lights();
				ArrMaterials = new Materials();
				Reflect = new Reflection();
				Reflect->Init(0);
			}
		}
		else
		{
			MLInit(device, std_path_material, std_path_mesh, winsize, projfov);
			MLSet::ReCalcSize();
			ArrLights = new Lights();
			ArrMaterials = new Materials();
			Reflect = new Reflection();
			Reflect->Init(0);
		}
	}
	else
		reportf(-1, "%s - not init argument [name], sxmaterial_ligth", gen_msg_location);
}

void SML_0Kill()
{
	mem_delete(ArrLights)
	mem_delete(ArrMaterials);
}

void SML_OnLostDevice()
{
	ML_PRECOND();
	ArrLights->OnLostDevice();
	Reflect->OnLostDevice();
}

void SML_OnResetDevice(int width, int heigth, float projfov)
{
	ML_PRECOND();
	MLSet::WinSize.x = width;
	MLSet::WinSize.y = heigth;
	MLSet::ProjFov = projfov;
	MLSet::ProjRatio = MLSet::WinSize.x / MLSet::WinSize.y;
	MLSet::ReCalcSize();
	ArrLights->OnResetDevice();
	Reflect->OnResetDevice();

	//MLSet::IDsRenderTargets::ToneMaps.clear();
	//MLSet::IDsRenderTargets::SurfToneMap.clear();
	int tmpcount = 0;
	while (true)
	{
		int tmpsize = 1 << (2 * tmpcount);
		if (tmpsize >= MLSet::WinSize.x*0.25 || tmpsize > MLSet::WinSize.y*0.25)
			break;
		if (MLSet::IDsRenderTargets::CountArrToneMaps <= tmpcount)
		{
			MLSet::IDsRenderTargets::ToneMaps[tmpcount] = SGCore_RTAdd(tmpsize, tmpsize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "", 0);
			MLSet::IDsRenderTargets::SurfToneMap[tmpcount] = 0;
		}
		++tmpcount;
	}
	MLSet::IDsRenderTargets::CountArrToneMaps = tmpcount;
	MLSet::IDsRenderTargets::HowAdaptedLum = 0;




	/*
	//этот код вызывал first chance exeption long at memory
	LPDIRECT3DSURFACE9 TmpSurf, BackBuf;

	SGCore_RTGetTexture(MLSet::IDsRenderTargets::LigthComScaled)->GetSurfaceLevel(0, &TmpSurf);

	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, TmpSurf);

	SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, MLSet::IDsShaders::PS::ScreenOut);

	MLSet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(MLSet::IDsTexs::NullingTex));
	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();

	mem_release(TmpSurf);



	SGCore_RTGetTexture(MLSet::IDsRenderTargets::AdaptLumCurr)->GetSurfaceLevel(0, &TmpSurf);

	MLSet::DXDevice->SetRenderTarget(0, TmpSurf);

	SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, MLSet::IDsShaders::PS::ScreenOut);

	MLSet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(MLSet::IDsTexs::NullingTex));
	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();

	mem_release(TmpSurf);



	SGCore_RTGetTexture(MLSet::IDsRenderTargets::AdaptLumLast)->GetSurfaceLevel(0, &TmpSurf);

	MLSet::DXDevice->SetRenderTarget(0, TmpSurf);

	SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, MLSet::IDsShaders::PS::ScreenOut);

	MLSet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(MLSet::IDsTexs::NullingTex));
	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();

	mem_release(TmpSurf);


	for (int i = 0; i < MLSet::IDsRenderTargets::CountArrToneMaps; ++i)
	{
		SGCore_RTGetTexture(MLSet::IDsRenderTargets::ToneMaps[i])->GetSurfaceLevel(0, &TmpSurf);

		MLSet::DXDevice->SetRenderTarget(0, TmpSurf);

		SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(1, MLSet::IDsShaders::PS::ScreenOut);

		MLSet::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(MLSet::IDsTexs::NullingTex));
		SGCore_ScreenQuadDraw();

		MLSet::DXDevice->SetVertexShader(0);
		MLSet::DXDevice->SetPixelShader(0);

		SGCore_ShaderUnBind();

		mem_release(TmpSurf);
	}

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);*/
}

void SML_LigthsSettLCoefSizeDepth(float coef)
{
	ML_PRECOND();

	MLSet::CoefSizeDepthMapForLocal = coef;
	MLSet::ReCalcSize();
	SML_OnLostDevice();
	SML_OnResetDevice(MLSet::WinSize.x, MLSet::WinSize.y, MLSet::ProjFov);
}

void SML_LigthsSettGCoefSizeDepth(float coef)
{
	ML_PRECOND();

	MLSet::CoefSizeDepthMapForGlobal = coef;
	MLSet::ReCalcSize();
	SML_OnLostDevice();
	SML_OnResetDevice(MLSet::WinSize.x, MLSet::WinSize.y, MLSet::ProjFov);
	
}

float SML_LigthsGettLCoefSizeDepth()
{
	ML_PRECOND(-1);

	return MLSet::CoefSizeDepthMapForLocal;
}

float SML_LigthsGettGCoefSizeDepth()
{
	ML_PRECOND(-1);
	return MLSet::CoefSizeDepthMapForGlobal;
}


void SML_LigthsUpdate(float3* const_cam_pos, float4x4* camview)
{
	ML_PRECOND();

	MLSet::ConstCurrCamPos = *const_cam_pos;
	MLSet::MCamView = *camview;
}


void SML_LigthsClear()
{
	ML_PRECOND();

	ArrLights->Clear();
}

long SML_LigthsGetCount()
{
	ML_PRECOND(-1);

	return ArrLights->GetCountLights();
}

long SML_LigthsGetIDOfKey(long key)
{
	ML_PRECOND(-1);

	return ArrLights->GetIdOfKey(key);
}

void SML_LigthsSave(const char* path)
{
	ML_PRECOND();
	ArrLights->Save(path);
}

void SML_LigthsLoad(const char* path)
{
	ML_PRECOND();
	ArrLights->Load(path);
}

ID SML_LigthsCreatePoint(float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreatePoint(-1,center, power, dist, color, isglobal, is_shadowed, 0);
}

ID SML_LigthsCreateDirection(float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreateDirection(-1,pos, power, dist, color, dir, top_radius, angle, is_shadow, 0);
}

ID SML_LigthsReCreatePoint(ID id, float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreatePoint(id, center, power, dist, color, isglobal, is_shadowed, 0);
}

ID SML_LigthsReCreateDirection(ID id, float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreateDirection(id, pos, power, dist, color, dir, top_radius, angle, is_shadow, 0);
}

void SML_LigthsLoadSource(ID id, const char* path)
{
	ML_PRECOND();
	ArrLights->LoadLightMeshSource(id, path);
}

void SML_LigthsBindToGroupSource(ID id, int group)
{
	ML_PRECOND();
	ArrLights->BindLightToGroup(id, group);
}

long SML_LigthsGetCountGroupSource(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightCountGroupMesh(id);
}

const char* SML_LigthsGetNameGroupSource(ID id, int group)
{
	ML_PRECOND(0);
	return ArrLights->GetLightNameGroupMesh(id, group);
}

const char* SML_LigthsGetPathNameSource(ID id)
{
	ML_PRECOND(0);
	return ArrLights->GetLightPathSource(id);
}

long SML_LigthsGetBindedGroupSource(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightBindedGroupSource(id);
}

bool SML_LigthsExistsSource(ID id)
{
	ML_PRECOND(0);
	return (ArrLights->GetLightCountGroupMesh(id) > -1);
}

void SML_LigthsRenderSource(ID id, bool render_bind_group, DWORD timeDelta)
{
	ML_PRECOND();
	if (id < 0)
		ArrLights->RenderSourceAll(render_bind_group, timeDelta);
	else
		ArrLights->RenderSource(id, render_bind_group, timeDelta);
}

void SML_LigthsRenderAllExceptGroupSource(ID id, DWORD timeDelta)
{
	ML_PRECOND();
	ArrLights->RenderSourceAllExceptGroup(id, timeDelta);
}

void SML_LigthsRender(ID id, DWORD timeDelta)
{
	ML_PRECOND();
	ArrLights->Render(id, timeDelta);
}

void SML_LigthsGetColor(ID id, float3* color)
{
	ML_PRECOND();
	ArrLights->GetLightColor(id, color);
}

void SML_LigthsSetColor(ID id, float3* vec)
{
	ML_PRECOND();
	ArrLights->SetLightColor(id, vec);
}
/*
void SML_LigthsGetPosW(long id, float4* pos)
{
	ML_PRECOND();
	ArrLights->GetLightPosW(id, pos);
}*/

void SML_LigthsGetPos(ID id, float3* vec, bool for_mesh, bool greal)
{
	ML_PRECOND();
	ArrLights->GetLightPos(id, vec, for_mesh, greal);
}

void SML_LigthsSetPower(ID id, float power)
{
	ML_PRECOND();
	ArrLights->SetLightPower(id, power);
}

float SML_LigthsGetPower(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightPower(id);
}

float SML_LigthsGetPowerDiv(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightPowerDiv(id);
}

float SML_LigthsGetDist(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightDist(id);
}

void SML_LigthsSetPos(ID id, float3* vec, bool greal)
{
	ML_PRECOND();
	ArrLights->SetLightPos(id, vec, greal);
}

void SML_LigthsGetRot(ID id, float3* vec, bool rot_mesh)
{
	ML_PRECOND();
	ArrLights->GetLightRot(id, vec, rot_mesh);
}

void SML_LigthsSetRot(ID id, float3* vec, bool rot_mesh)
{
	ML_PRECOND();
	ArrLights->SetLightRot(id, vec, rot_mesh);
}

void SML_LigthsGetDir(ID id, float3* vec, bool rot_mesh)
{
	ML_PRECOND();
	ArrLights->GetLightDir(id, vec, rot_mesh);
}

void SML_LigthsSetDir(ID id, float3* vec, bool rot_mesh)
{
	ML_PRECOND();
	ArrLights->SetLightDir(id, vec, rot_mesh);
}

int SML_LigthsGetType(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetType(id);
}

bool SML_LigthsComVisibleForFrustum(ID id, ISXFrustum* frustum)
{
	ML_PRECOND(-1);
	return ArrLights->ComVisibleForFrustum(id, frustum);
}

bool SML_LigthsGetVisibleForFrustum(ID id)
{
	ML_PRECOND(false);
	return ArrLights->GetVisibleForFrustum(id);
}

float SML_LigthsComDistFor(ID id, float3* vec)
{
	ML_PRECOND(-1);
	return ArrLights->ComDistFor(id, vec);
}

float SML_LigthsGetDistFor(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetDistFor(id);
}

void SML_LigthsComVisibleFrustumDistFor(ISXFrustum* frustum, float3* vec)
{
	ML_PRECOND();
	return ArrLights->ComVisibleFrustumDistFor(frustum, vec);
}

int SML_LigthsIsEnable(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->IsEnable(id);
}

void SML_LigthsSetEnable(ID id, bool val)
{
	ML_PRECOND();
	return ArrLights->SetEnable(id, val);
}

bool SML_LigthsIsShadow(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->IsShadow(id);
}

bool SML_LigthsGet4Or3SplitsG(ID id)
{
	ML_PRECOND(0);
}

void SML_LigthsSet4Or3SplitsG(ID id, bool bf4or3)
{
	ML_PRECOND();
}

IDirect3DTexture9* SML_LigthsGetShadow()
{
	ML_PRECOND(0);
	return ArrLights->GetShadow2();
}

void SML_LigthsNullingShadow()
{
	ML_PRECOND();
	return ArrLights->NullingShadow();
}

void SML_LigthsInRenderBegin(ID id)
{
	ML_PRECOND();
	ArrLights->InRenderBegin(id);
}

void SML_LigthsInRenderEnd(ID id)
{
	ML_PRECOND();
	ArrLights->InRenderEnd(id);
}

void SML_LigthsInRenderPre(ID id, int cube)
{
	ML_PRECOND();
	ArrLights->InRenderPre(id, cube);
}

void SML_LigthsInRenderPost(ID id, int cube)
{
	ML_PRECOND();
	ArrLights->InRenderPost(id, cube);
}

ISXFrustum* SML_LigthsGetFrustum(ID id, int how)
{
	ML_PRECOND(0);
	return ArrLights->GetFrustum(id, how);
}

void SML_LigthsUpdateFrustumsG(ID id, int split, float3* pos, float3* dir)
{
	ML_PRECOND();
	return ArrLights->UpdateFrustumsG(id, split, pos,dir);
}

void SML_LigthsShadowSetShaderOfTypeMat(ID id, int typemat, float4x4* wmat)
{
	ML_PRECOND();
	ArrLights->InitShaderOfTypeMaterial(id, typemat, wmat);
}

void SML_LigthsGenShadow(ID id)
{
	ML_PRECOND();
	ArrLights->GenShadow2(id);
}

void SML_LigthsSoftShadow(bool randomsam, float size, bool isfirst)
{
	ML_PRECOND();
	ArrLights->SoftShadow(randomsam, size, isfirst);
}



ID SML_LigthsCreateCopy(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->CreateCopy(id);
}

void SML_LigthsDeleteLight(ID id)
{
	ML_PRECOND();
	ArrLights->DeleteLight(id);
}

char* SML_LigthsGetName(ID id)
{
	ML_PRECOND(0);
	return ArrLights->GetLightName(id);
}

void SML_LigthsSetName(ID id, const char* name)
{
	ML_PRECOND();
	ArrLights->SetLightName(id, name);
}


void SML_LigthsSetAngle(ID id, float angle)
{
	ML_PRECOND();
	ArrLights->ChangeAngle(id, angle, true);
}

void SML_LigthsSetTopRadius(ID id, float top_radius)
{
	ML_PRECOND();
	ArrLights->ChangeTopRadius(id, top_radius);
}

void SML_LigthsSetDist(ID id, float radius_height, bool is_create)
{
	ML_PRECOND();
	ArrLights->ChangeRadiusHeight(id, radius_height, is_create);
}

void SML_LigthsSetShadow(ID id, bool is_shadow)
{
	ML_PRECOND();
	ArrLights->ChangeShadow(id, is_shadow);
}


void SML_LigthsSetBlurPixel(ID id, float blur_pixel)
{
	ML_PRECOND();
	ArrLights->SetBlurPixel(id, blur_pixel);
}
float SML_LigthsGetBlurPixel(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetBlurPixel(id);
}
void SML_LigthsSetShadowLocalNear(ID id, float slnear)
{
	ML_PRECOND();
	ArrLights->SetShadowLocalNear(id, slnear);
}
float SML_LigthsGetShadowLocalNear(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowLocalNear(id);
}
void SML_LigthsSetShadowLocalFar(ID id, float slfar)
{
	ML_PRECOND();
	ArrLights->SetShadowLocalFar(id, slfar);
}
float SML_LigthsGetShadowLocalFar(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowLocalFar(id);
}
void SML_LigthsSetEnableCubeEdge(ID id, int edge, bool enable)
{
	ML_PRECOND();
	ArrLights->SetEnableCubeEdge(id, edge, enable);
}
bool SML_LigthsGetEnableCubeEdge(ID id, int edge)
{
	ML_PRECOND(false);
	return ArrLights->GetEnableCubeEdge(id, edge);
}


float SML_LigthsGetAngle(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightAngle(id);
}

float SML_LigthsGetTopRadius(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightTopRadius(id);
}

float SML_LigthsGetBias(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightBias(id);
}

void SML_LigthsSetBias(ID id, float bias)
{
	ML_PRECOND();
	return ArrLights->SetLightBias(id, bias);
}

ID SML_LigthsGetIDArr(ID id, ID inid, int how)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightIDArr(id, inid, how);
}

void SML_LigthsSetIDArr(ID id, ID inid, int how, ID id_arr)
{
	ML_PRECOND();
	ArrLights->SetLightIDArr(id, inid, how, id_arr);
}

void SML_LigthsSetTypeShadowed(ID id, int type)
{
	ML_PRECOND();
	ArrLights->SetLightTypeShadowed(id, type);
}

int SML_LigthsGetTypeShadowed(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightTypeShadowed(id);
}

bool SML_LigthsUpdateCountUpdate(ID id, float3* viewpos, int ghow)
{
	ML_PRECOND(false);
	return ArrLights->UpdateLightCountUpdate(id, viewpos, ghow);
}

bool SML_LigthsAllowedRender(ID id, int ghow)
{
	ML_PRECOND(false);
	return ArrLights->AllowedRenderLight(id, ghow);
}

void SML_LigthsSetNullCountUpdate(ID id)
{
	ML_PRECOND();
	ArrLights->SetNullLightCountUpdate(id);
}

void SML_LigthsComHDR(DWORD timeDelta, float factor_adapted)
{
	ML_PRECOND();
	ArrLights->ComHDR(timeDelta, factor_adapted);
}

long SML_LigthsDelGetCount()
{
	ML_PRECOND(-1);
	return ArrLights->DelGetCount();
}

int SML_LigthsDelGetType(long key)
{
	ML_PRECOND(-1);
	return ArrLights->DelGetType(key);
}

void SML_LigthsDelDel(long key)
{
	ML_PRECOND();
	ArrLights->DelDel(key);
}

ID SML_LigthsDelGetIDArr(long key, ID inid, int how)
{
	ML_PRECOND(-1);
	return ArrLights->DelGetIDArr(key, inid, how);
}

////////

ID SML_DSGetRT_ID(int type)
{
	ML_PRECOND(-1);
	if (type == DS_RT_COLOR)
		return MLSet::IDsRenderTargets::ColorScene;
	else if (type == DS_RT_NORMAL)
		return MLSet::IDsRenderTargets::NormalScene;
	else if (type == DS_RT_PARAM)
		return MLSet::IDsRenderTargets::ParamsScene;
	else if (type == DS_RT_DEPTH)
		return MLSet::IDsRenderTargets::DepthScene;

	else if (type == DS_RT_COLOR2)
		return MLSet::IDsRenderTargets::ColorScene2;
	else if (type == DS_RT_NORMAL2)
		return MLSet::IDsRenderTargets::NormalScene2;
	else if (type == DS_RT_PARAM2)
		return MLSet::IDsRenderTargets::ParamsScene2;
	else if (type == DS_RT_DEPTH2)
		return MLSet::IDsRenderTargets::DepthScene2;

	else if (type == DS_RT_AMBIENT_DIFF)
		return MLSet::IDsRenderTargets::LightAmbientDiff;
	else if (type == DS_RT_SPECULAR)
		return MLSet::IDsRenderTargets::LightSpecular;

	else if (type == DS_RT_AMBIENT_DIFF2)
		return MLSet::IDsRenderTargets::LightAmbientDiff2;
	else if (type == DS_RT_SPECULAR2)
		return MLSet::IDsRenderTargets::LightSpecular2;

	else if (type == DS_RT_SCENE_LIGHT_COM)
		return MLSet::IDsRenderTargets::LigthCom;

	else if (type == DS_RT_SCENE_LIGHT_COM2)
		return MLSet::IDsRenderTargets::LigthCom2;

	else if (type == DS_RT_ADAPTED_LUM_CURR)
		return MLSet::IDsRenderTargets::GetCurrAdaptedLum();

	return -1;
}

IDirect3DTexture9* SML_DSGetRT(int type)
{
	ML_PRECOND(0);

	ID tmpid = SML_DSGetRT_ID(type);
	if (tmpid >= 0)
		return SGCore_RTGetTexture(SML_DSGetRT_ID(type));
	else
		return 0;
}

////////

ID SML_MtlLoad(const char* name, int mtl_type)
{
	ML_PRECOND(-1);

	return ArrMaterials->Load(name, mtl_type);
}

int SML_MtlGetType(ID id)
{
	ML_PRECOND(-1);

	return ArrMaterials->GetType(id);
}

int SML_MtlIsRefraction(ID id)
{
	ML_PRECOND(-1);

	return ArrMaterials->IsRefraction(id);
}

int SML_MtlTypeReflection(ID id)
{
	ML_PRECOND(-1);

	return ArrMaterials->TypeReflection(id);
}

void SML_MtlUpdate(DWORD timeDelta)
{
	ML_PRECOND();

	ArrMaterials->Update(timeDelta);
}

long SML_MtlGetCount()
{
	ML_PRECOND(-1);

	return ArrMaterials->GetCount();
}

void SML_MtlRender(ID id, float4x4* world)
{
	ML_PRECOND();
	ArrMaterials->Render(id, world);
}

void SML_MtlRenderLight(float4_t* color, float4x4* world)
{
	ML_PRECOND();
	ArrMaterials->RenderLight(color, world);
}

void SML_MtlSetMainTexture(ID slot, ID id)
{
	ML_PRECOND();
	ArrMaterials->SetMainTexture(slot, id);
}




void SML_MtlRefSetPlane(D3DXPLANE* plane)
{
	Reflect->Plane = *plane;
}

void SML_MtlRefSetCenter(float3_t* center)
{
	Reflect->Position = *center;
}

void SML_MtlRefPreRenderPlane(float4x4* world)
{
	Reflect->PreRenderRefPlane(world);
}

ISXFrustum* SML_MtlRefGetfrustum()
{
	return Reflect->ReflectFrustum[0];
}

void SML_MtlRefPostRenderPlane()
{
	Reflect->PostRenderRefPlane();
}

IDirect3DTexture9* SML_MtlRefGetTexPlane()
{
	return Reflect->GetRefPlaneTex();
}
