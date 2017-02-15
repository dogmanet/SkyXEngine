
#define SXMATERIAL_LIGTH_VERSION 1

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

#include <material_ligth\\ml_data.cpp>
#include <material_ligth\material_ligth.h>
#include <material_ligth\\light.cpp>
#include <material_ligth\\material.cpp>

Lights* ArrLights = 0;
Materials* ArrMaterials = 0;

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
			}
		}
		else
		{
			MLInit(device, std_path_material, std_path_mesh, winsize, projfov);
			MLSet::ReCalcSize();
			ArrLights = new Lights();
			ArrMaterials = new Materials();
		}
	}
	else
		reportf(-1, "%s - not init argument [name], sxmaterial_ligth", gen_msg_location);
}

void SML_OnLostDevice()
{
	ML_PRECOND();
	ArrLights->OnLostDevice();
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

long SML_LigthsCreatePoint(float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreatePoint(-1,center, power, dist, color, isglobal, is_shadowed, 0);
}

long SML_LigthsCreateDirection(float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreateDirection(-1,pos, power, dist, color, dir, top_radius, angle, is_shadow, 0);
}

long SML_LigthsReCreatePoint(long id, float3* center, float power, float dist, float3* color, bool isglobal, bool is_shadowed/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreatePoint(id, center, power, dist, color, isglobal, is_shadowed, 0);
}

long SML_LigthsReCreateDirection(long id, float3* pos, float power, float dist, float3* color, float3* dir, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreateDirection(id, pos, power, dist, color, dir, top_radius, angle, is_shadow, 0);
}

void SML_LigthsLoadSource(long id, const char* path)
{
	ML_PRECOND();
	ArrLights->LoadLightMeshSource(id, path);
}

void SML_LigthsBindToGroupSource(long id, int group)
{
	ML_PRECOND();
	ArrLights->BindLightToGroup(id, group);
}

long SML_LigthsGetCountGroupSource(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightCountGroupMesh(id);
}

const char* SML_LigthsGetNameGroupSource(long id, int group)
{
	ML_PRECOND(0);
	return ArrLights->GetLightNameGroupMesh(id, group);
}

const char* SML_LigthsGetPathNameSource(long id)
{
	ML_PRECOND(0);
	return ArrLights->GetLightPathSource(id);
}

long SML_LigthsGetBindedGroupSource(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightBindedGroupSource(id);
}

bool SML_LigthsExistsSource(long id)
{
	ML_PRECOND(0);
	return (ArrLights->GetLightCountGroupMesh(id) > -1);
}

void SML_LigthsRenderSource(long id, bool render_bind_group, DWORD timeDelta)
{
	ML_PRECOND();
	if (id < 0)
		ArrLights->RenderSourceAll(render_bind_group, timeDelta);
	else
		ArrLights->RenderSource(id, render_bind_group, timeDelta);
}

void SML_LigthsRenderAllExceptGroupSource(long id, DWORD timeDelta)
{
	ML_PRECOND();
	ArrLights->RenderSourceAllExceptGroup(id, timeDelta);
}

void SML_LigthsRender(long id, DWORD timeDelta)
{
	ML_PRECOND();
	ArrLights->Render(id, timeDelta);
}

void SML_LigthsGetColor(long id, float3* color)
{
	ML_PRECOND();
	ArrLights->GetLightColor(id, color);
}

void SML_LigthsSetColor(long id, float3* vec)
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

void SML_LigthsGetPos(long id, float3* vec, bool for_mesh, bool greal)
{
	ML_PRECOND();
	ArrLights->GetLightPos(id, vec, for_mesh, greal);
}

void SML_LigthsSetPower(long id, float power)
{
	ML_PRECOND();
	ArrLights->SetLightPower(id, power);
}

float SML_LigthsGetPower(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightPower(id);
}

float SML_LigthsGetPowerDiv(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightPowerDiv(id);
}

float SML_LigthsGetDist(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightDist(id);
}

void SML_LigthsSetPos(long id, float3* vec, bool greal)
{
	ML_PRECOND();
	ArrLights->SetLightPos(id, vec, greal);
}

void SML_LigthsGetRot(long id, float3* vec, bool rot_mesh)
{
	ML_PRECOND();
	ArrLights->GetLightRot(id, vec, rot_mesh);
}

void SML_LigthsSetRot(long id, float3* vec, bool rot_mesh)
{
	ML_PRECOND();
	ArrLights->SetLightRot(id, vec, rot_mesh);
}

void SML_LigthsGetDir(long id, float3* vec, bool rot_mesh)
{
	ML_PRECOND();
	ArrLights->GetLightDir(id, vec, rot_mesh);
}

void SML_LigthsSetDir(long id, float3* vec, bool rot_mesh)
{
	ML_PRECOND();
	ArrLights->SetLightDir(id, vec, rot_mesh);
}

int SML_LigthsGetType(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetType(id);
}

bool SML_LigthsComVisibleForFrustum(long id, ISXFrustum* frustum)
{
	ML_PRECOND(-1);
	return ArrLights->ComVisibleForFrustum(id, frustum);
}

bool SML_LigthsGetVisibleForFrustum(long id)
{
	ML_PRECOND(false);
	return ArrLights->GetVisibleForFrustum(id);
}

float SML_LigthsComDistFor(long id, float3* vec)
{
	ML_PRECOND(-1);
	return ArrLights->ComDistFor(id, vec);
}

float SML_LigthsGetDistFor(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetDistFor(id);
}

void SML_LigthsComVisibleFrustumDistFor(ISXFrustum* frustum, float3* vec)
{
	ML_PRECOND();
	return ArrLights->ComVisibleFrustumDistFor(frustum, vec);
}

int SML_LigthsIsEnable(long id)
{
	ML_PRECOND(-1);
	return ArrLights->IsEnable(id);
}

void SML_LigthsSetEnable(long id, bool val)
{
	ML_PRECOND();
	return ArrLights->SetEnable(id, val);
}

bool SML_LigthsIsShadow(long id)
{
	ML_PRECOND(-1);
	return ArrLights->IsShadow(id);
}

bool SML_LigthsGet4Or3SplitsG(long id)
{
	ML_PRECOND(0);
}

void SML_LigthsSet4Or3SplitsG(long id, bool bf4or3)
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

void SML_LigthsInRenderBegin(long id)
{
	ML_PRECOND();
	ArrLights->InRenderBegin(id);
}

void SML_LigthsInRenderEnd(long id)
{
	ML_PRECOND();
	ArrLights->InRenderEnd(id);
}

void SML_LigthsInRenderPre(long id, int cube)
{
	ML_PRECOND();
	ArrLights->InRenderPre(id, cube);
}

void SML_LigthsInRenderPost(long id, int cube)
{
	ML_PRECOND();
	ArrLights->InRenderPost(id, cube);
}

ISXFrustum* SML_LigthsGetFrustum(long id, int how)
{
	ML_PRECOND(0);
	return ArrLights->GetFrustum(id, how);
}

void SML_LigthsUpdateFrustumsG(long id, int split, float3* pos, float3* dir)
{
	ML_PRECOND();
	return ArrLights->UpdateFrustumsG(id, split, pos,dir);
}

void SML_LigthsShadowSetShaderOfTypeMat(long id, int typemat, float4x4* wmat)
{
	ML_PRECOND();
	ArrLights->InitShaderOfTypeMaterial(id, typemat, wmat);
}

void SML_LigthsGenShadow(long id)
{
	ML_PRECOND();
	ArrLights->GenShadow2(id);
}

void SML_LigthsSoftShadow(bool randomsam, float size, bool isfirst)
{
	ML_PRECOND();
	ArrLights->SoftShadow(randomsam, size, isfirst);
}



long SML_LigthsCreateCopy(long id)
{
	ML_PRECOND(-1);
	return ArrLights->CreateCopy(id);
}

void SML_LigthsDeleteLight(long id)
{
	ML_PRECOND();
	ArrLights->DeleteLight(id);
}

char* SML_LigthsGetName(long id)
{
	ML_PRECOND(0);
	return ArrLights->GetLightName(id);
}

void SML_LigthsSetName(long id, const char* name)
{
	ML_PRECOND();
	ArrLights->SetLightName(id, name);
}


void SML_LigthsSetAngle(long id, float angle)
{
	ML_PRECOND();
	ArrLights->ChangeAngle(id, angle, true);
}

void SML_LigthsSetTopRadius(long id, float top_radius)
{
	ML_PRECOND();
	ArrLights->ChangeTopRadius(id, top_radius);
}

void SML_LigthsSetDist(long id, float radius_height, bool is_create)
{
	ML_PRECOND();
	ArrLights->ChangeRadiusHeight(id, radius_height, is_create);
}

void SML_LigthsSetShadow(long id, bool is_shadow)
{
	ML_PRECOND();
	ArrLights->ChangeShadow(id, is_shadow);
}


void SML_LigthsSetBlurPixel(long id, float blur_pixel)
{
	ML_PRECOND();
	ArrLights->SetBlurPixel(id, blur_pixel);
}
float SML_LigthsGetBlurPixel(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetBlurPixel(id);
}
void SML_LigthsSetShadowLocalNear(long id, float slnear)
{
	ML_PRECOND();
	ArrLights->SetShadowLocalNear(id, slnear);
}
float SML_LigthsGetShadowLocalNear(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowLocalNear(id);
}
void SML_LigthsSetShadowLocalFar(long id, float slfar)
{
	ML_PRECOND();
	ArrLights->SetShadowLocalFar(id, slfar);
}
float SML_LigthsGetShadowLocalFar(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowLocalFar(id);
}
void SML_LigthsSetEnableCubeEdge(long id, int edge, bool enable)
{
	ML_PRECOND();
	ArrLights->SetEnableCubeEdge(id, edge, enable);
}
bool SML_LigthsGetEnableCubeEdge(long id, int edge)
{
	ML_PRECOND(false);
	return ArrLights->GetEnableCubeEdge(id, edge);
}


float SML_LigthsGetAngle(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightAngle(id);
}

float SML_LigthsGetTopRadius(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightTopRadius(id);
}

float SML_LigthsGetBias(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightBias(id);
}

void SML_LigthsSetBias(long id, float bias)
{
	ML_PRECOND();
	return ArrLights->SetLightBias(id, bias);
}

long SML_LigthsGetIDArr(long id, long inid, int how)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightIDArr(id, inid, how);
}

void SML_LigthsSetIDArr(long id, long inid, int how, long id_arr)
{
	ML_PRECOND();
	ArrLights->SetLightIDArr(id, inid, how, id_arr);
}

void SML_LigthsSetTypeShadowed(long id, int type)
{
	ML_PRECOND();
	ArrLights->SetLightTypeShadowed(id, type);
}

int SML_LigthsGetTypeShadowed(long id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightTypeShadowed(id);
}

bool SML_LigthsUpdateCountUpdate(long id, float3* viewpos, int ghow)
{
	ML_PRECOND(false);
	return ArrLights->UpdateLightCountUpdate(id, viewpos, ghow);
}

bool SML_LigthsAllowedRender(long id, int ghow)
{
	ML_PRECOND(false);
	return ArrLights->AllowedRenderLight(id, ghow);
}

void SML_LigthsSetNullCountUpdate(long id)
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

long SML_LigthsDelGetIDArr(long key, long inid, int how)
{
	ML_PRECOND(-1);
	return ArrLights->DelGetIDArr(key, inid, how);
}

////////

DWORD SML_DSGetRT_ID(int type)
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
	else if (type == DS_RT_AMBIENT_DIFF)
		return MLSet::IDsRenderTargets::LightAmbientDiff;
	else if (type == DS_RT_SPECULAR)
		return MLSet::IDsRenderTargets::LightSpecular;
	else if (type == DS_RT_SCENE_LIGHT_COM)
		return MLSet::IDsRenderTargets::LigthCom;
	else if (type == DS_RT_ADAPTED_LUM_CURR)
		return MLSet::IDsRenderTargets::GetCurrAdaptedLum();

	return -1;
}

IDirect3DTexture9* SML_DSGetRT(int type)
{
	ML_PRECOND(0);
	if (type == DS_RT_COLOR)
		return SGCore_RTGetTexture(MLSet::IDsRenderTargets::ColorScene);
	else if (type == DS_RT_NORMAL)
		return SGCore_RTGetTexture(MLSet::IDsRenderTargets::NormalScene);
	else if (type == DS_RT_PARAM)
		return SGCore_RTGetTexture(MLSet::IDsRenderTargets::ParamsScene);
	else if (type == DS_RT_DEPTH)
		return SGCore_RTGetTexture(MLSet::IDsRenderTargets::DepthScene);
	else if (type == DS_RT_AMBIENT_DIFF)
		return SGCore_RTGetTexture(MLSet::IDsRenderTargets::LightAmbientDiff);
	else if (type == DS_RT_SPECULAR)
		return SGCore_RTGetTexture(MLSet::IDsRenderTargets::LightSpecular);
	else if (type == DS_RT_SCENE_LIGHT_COM)
		return SGCore_RTGetTexture(MLSet::IDsRenderTargets::LigthCom);
	else if (type == DS_RT_ADAPTED_LUM_CURR)
		return SGCore_RTGetTexture(MLSet::IDsRenderTargets::GetCurrAdaptedLum());

	return 0;
}

////////

long SML_MtlLoad(const char* name, int mtl_type)
{
	ML_PRECOND(-1);

	return ArrMaterials->Load(name, mtl_type);
}

int SML_MtlGetType(long id)
{
	ML_PRECOND(-1);

	return ArrMaterials->GetType(id);
}

long SML_MtlGetCount()
{
	ML_PRECOND(-1);

	return ArrMaterials->GetCount();
}

void SML_MtlRender(long id, float4x4* world)
{
	ML_PRECOND();
	ArrMaterials->Render(id, world);
}

void SML_MtlSetMainTexture(DWORD slot, long id)
{
	ML_PRECOND();
	ArrMaterials->SetMainTexture(slot, id);
}