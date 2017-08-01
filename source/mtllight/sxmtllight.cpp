
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SXMATERIAL_LIGTH_VERSION 1

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif
#include <common\\string_api.cpp>

#include <mtllight\sxmtllight.h>
#include <mtllight\\ml_data.cpp>

#include <mtllight\\light.cpp>
#include <mtllight\\material.cpp>

Lights* ArrLights = 0;
Materials* ArrMaterials = 0;

#define ML_PRECOND(retval) if(!ArrLights){reportf(-1, "%s - sxmtlligth is not init", gen_msg_location); return retval;}

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

void SML_AKill()
{
	mem_delete(ArrLights)
	mem_delete(ArrMaterials);
}

void SML_OnLostDevice()
{
	ML_PRECOND();
	ArrLights->OnLostDevice();
	ArrMaterials->OnLostDevice();
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
	ArrMaterials->OnResetDevice();

	//MLSet::IDsRenderTargets::ToneMaps.clear();
	//MLSet::IDsRenderTargets::SurfToneMap.clear();
	/*int tmpcount = 0;
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
	MLSet::IDsRenderTargets::CountArrToneMaps = tmpcount;*/
	MLSet::IDsRenderTargets::HowAdaptedLum = 0;
}

void SML_Update(DWORD timeDelta, float2_t* winsize, float2_t* nearfar, float3* poscam, float4x4* mview, float projfov)
{
	MLSet::WinSize = *winsize;
	MLSet::NearFar = *nearfar;
	MLSet::ConstCurrCamPos = *poscam;
	MLSet::MCamView = *mview;
	MLSet::ProjFov = projfov;
	MLSet::ProjRatio = MLSet::WinSize.x / MLSet::WinSize.y;

	if (ArrMaterials)
		ArrMaterials->Update(timeDelta);
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

//#############################################################################

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

ID SML_LigthsGetIDByKey(ID key)
{
	ML_PRECOND(-1);

	return ArrLights->GetIdByKey(key);
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

ID SML_LigthsCreatePoint(const float3* center, float dist, const float3* color, bool isglobal, bool is_shadowed/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreatePoint(-1,center, dist, color, isglobal, is_shadowed, 0);
}

ID SML_LigthsCreateDirection(const float3* pos, float dist, const float3* color, const float3* dir, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreateDirection(-1,pos, dist, color, dir, top_radius, angle, is_shadow, 0);
}

void SML_LigthsRender(ID id, DWORD timeDelta)
{
	ML_PRECOND();
	ArrLights->Render(id, timeDelta);
}

ID SML_LigthsGetGlobal()
{
	ML_PRECOND(-1);
	return ArrLights->GetLightGlobal();
}

void SML_LigthsGetColor(ID id, float3* color)
{
	ML_PRECOND();
	ArrLights->GetLightColor(id, color);
}

void SML_LigthsSetColor(ID id, const float3* vec)
{
	ML_PRECOND();
	ArrLights->SetLightColor(id, vec);
}

void SML_LigthsGetPos(ID id, float3* vec, bool greal)
{
	ML_PRECOND();
	ArrLights->GetLightPos(id, vec, greal);
}

float SML_LigthsGetPower(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightPower(id);
}

float SML_LigthsGetDist(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightDist(id);
}

void SML_LigthsSetPos(ID id, const float3* vec, bool greal)
{
	ML_PRECOND();
	ArrLights->SetLightPos(id, vec, greal);
}

void SML_LigthsGetRot(ID id, float3* vec)
{
	ML_PRECOND();
	ArrLights->GetLightRot(id, vec);
}

void SML_LigthsSetRot(ID id, const float3* vec)
{
	ML_PRECOND();
	ArrLights->SetLightRot(id, vec);
}

void SML_LigthsGetDir(ID id, float3* vec)
{
	ML_PRECOND();
	ArrLights->GetLightDir(id, vec);
}

void SML_LigthsSetDir(ID id, const float3* vec)
{
	ML_PRECOND();
	ArrLights->SetLightDir(id, vec);
}

LightsTypeLight SML_LigthsGetType(ID id)
{
	ML_PRECOND(LightsTypeLight::ltl_none);
	return ArrLights->GetLightType(id);
}

bool SML_LigthsComVisibleForFrustum(ID id, const ISXFrustum* frustum)
{
	ML_PRECOND(-1);
	return ArrLights->ComVisibleForFrustum(id, frustum);
}

bool SML_LigthsGetVisibleForFrustum(ID id)
{
	ML_PRECOND(false);
	return ArrLights->GetVisibleForFrustum(id);
}

float SML_LigthsComDistFor(ID id, const float3* vec)
{
	ML_PRECOND(-1);
	return ArrLights->ComDistFor(id, vec);
}

float SML_LigthsGetDistFor(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetDistFor(id);
}

void SML_LigthsComVisibleFrustumDistFor(const ISXFrustum* frustum, const float3* vec)
{
	ML_PRECOND();
	return ArrLights->ComVisibleFrustumDistFor(frustum, vec);
}

bool SML_LigthsGetEnable(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightEnable(id);
}

void SML_LigthsSetEnable(ID id, bool val)
{
	ML_PRECOND();
	return ArrLights->SetLightEnable(id, val);
}

bool SML_LigthsGetShadowed(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightShadowed(id);
}

bool SML_LigthsGet4Or3SplitsG(ID id)
{
	ML_PRECOND(0);
	return ArrLights->Get4Or3Splits(id);
}

void SML_LigthsSet4Or3SplitsG(ID id, bool bf4or3)
{
	ML_PRECOND();

	ArrLights->Set4Or3Splits(id, bf4or3);
}

IDirect3DTexture9* SML_LigthsGetShadow()
{
	ML_PRECOND(0);
	return ArrLights->GetShadow2();
}

void SML_LigthsShadowNull()
{
	ML_PRECOND();
	return ArrLights->ShadowNull();
}

void SML_LigthsShadowRenderBegin(ID id)
{
	ML_PRECOND();
	ArrLights->ShadowRenderBegin(id);
}

void SML_LigthsShadowRenderEnd(ID id)
{
	ML_PRECOND();
	ArrLights->ShadowRenderEnd(id);
}

void SML_LigthsShadowRenderPre(ID id, int cube)
{
	ML_PRECOND();
	ArrLights->ShadowRenderPre(id, cube);
}

void SML_LigthsShadowRenderPost(ID id, int cube)
{
	ML_PRECOND();
	ArrLights->ShadowRenderPost(id, cube);
}

ISXFrustum* SML_LigthsGetFrustum(ID id, int how)
{
	ML_PRECOND(0);
	return ArrLights->GetLightFrustum(id, how);
}

void SML_LigthsUpdateGFrustums(ID id, int split, const float3* pos, const float3* dir)
{
	ML_PRECOND();
	return ArrLights->UpdateLightGFrustums(id, split, pos, dir);
}

void SML_LigthsShadowSetShaderOfTypeMat(ID id, int typemat, float4x4* wmat)
{
	ML_PRECOND();
	ArrLights->InitShaderOfTypeMaterial(id, typemat, wmat);
}

void SML_LigthsShadowGen(ID id)
{
	ML_PRECOND();
	ArrLights->ShadowGen2(id);
}

void SML_LigthsShadowSoft(bool randomsam, float size, bool isfirst)
{
	ML_PRECOND();
	ArrLights->ShadowSoft(randomsam, size, isfirst);
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
	ArrLights->SetLightAngle(id, angle, true);
}

void SML_LigthsSetTopRadius(ID id, float top_radius)
{
	ML_PRECOND();
	ArrLights->SetLightTopRadius(id, top_radius);
}

void SML_LigthsSetDist(ID id, float radius_height, bool is_create)
{
	ML_PRECOND();
	ArrLights->SetLightDist(id, radius_height, is_create);
}


float SML_LigthsGetShadowBias(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowBias(id);
}

void SML_LigthsSetShadowBias(ID id, float bias)
{
	ML_PRECOND();
	return ArrLights->SetShadowBias(id, bias);
}

void SML_LigthsSetShadowBlurPixel(ID id, float blur_pixel)
{
	ML_PRECOND();
	ArrLights->SetShadowBlurPixel(id, blur_pixel);
}
float SML_LigthsGetShadowBlurPixel(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowBlurPixel(id);
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
void SML_LigthsSetCubeEdgeEnable(ID id, int edge, bool enable)
{
	ML_PRECOND();
	ArrLights->SetLightCubeEdgeEnable(id, edge, enable);
}
bool SML_LigthsGetCubeEdgeEnable(ID id, int edge)
{
	ML_PRECOND(false);
	return ArrLights->GetLightCubeEdgeEnable(id, edge);
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

void SML_LigthsSetTypeShadowed(ID id, LightsTypeShadow type)
{
	ML_PRECOND();
	ArrLights->SetLightTypeShadowed(id, type);
}

LightsTypeShadow SML_LigthsGetTypeShadowed(ID id)
{
	ML_PRECOND(LightsTypeShadow::lts_none);
	return ArrLights->GetLightTypeShadowed(id);
}

bool SML_LigthsCountUpdateUpdate(ID id, const float3* viewpos, int ghow)
{
	ML_PRECOND(false);
	return ArrLights->LightCountUpdateUpdate(id, viewpos, ghow);
}

bool SML_LigthsCountUpdateAllowed(ID id, int ghow)
{
	ML_PRECOND(false);
	return ArrLights->LightCountUpdateAllowed(id, ghow);
}

void SML_LigthsCountUpdateNull(ID id)
{
	ML_PRECOND();
	ArrLights->LightCountUpdateNull(id);
}

void SML_LigthsToneMappingCom(DWORD timeDelta, float factor_adapted)
{
	ML_PRECOND();
	ArrLights->ToneMappingCom(timeDelta, factor_adapted);
}

int SML_LigthsDelGetCount()
{
	ML_PRECOND(-1);
	return ArrLights->DelGetCount();
}

LightsTypeLight SML_LigthsDelGetType(ID key)
{
	ML_PRECOND(LightsTypeLight::ltl_none);
	return ArrLights->DelGetType(key);
}

void SML_LigthsDelDel(ID key)
{
	ML_PRECOND();
	ArrLights->DelDel(key);
}

ID SML_LigthsDelGetIDArr(ID key, ID inid, int how)
{
	ML_PRECOND(-1);
	return ArrLights->DelGetIDArr(key, inid, how);
}

//#############################################################################

ID SML_DSGetRT_ID(DS_RT type)
{
	ML_PRECOND(-1);
	if (type == DS_RT::ds_rt_color)
		return MLSet::IDsRenderTargets::ColorScene;
	else if (type == DS_RT::ds_rt_normal)
		return MLSet::IDsRenderTargets::NormalScene;
	else if (type == DS_RT::ds_rt_param)
		return MLSet::IDsRenderTargets::ParamsScene;
	else if (type == DS_RT::ds_rt_depth)
		return MLSet::IDsRenderTargets::DepthScene;
	else if (type == DS_RT::ds_rt_depth0)
		return MLSet::IDsRenderTargets::DepthScene0;
	else if (type == DS_RT::ds_rt_depth1)
		return MLSet::IDsRenderTargets::DepthScene1;

	else if (type == DS_RT::ds_rt_ambient_diff)
		return MLSet::IDsRenderTargets::LightAmbientDiff;
	else if (type == DS_RT::ds_rt_specular)
		return MLSet::IDsRenderTargets::LightSpecular;

	else if (type == DS_RT::ds_rt_scene_light_com)
		return MLSet::IDsRenderTargets::LigthCom;
	else if (type == DS_RT::ds_rt_scene_light_com2)
		return MLSet::IDsRenderTargets::LigthCom2;

	else if (type == DS_RT::ds_rt_adapted_lum_curr)
		return MLSet::IDsRenderTargets::GetCurrAdaptedLum();

	return -1;
}

IDirect3DTexture9* SML_DSGetRT(DS_RT type)
{
	ML_PRECOND(0);

	ID tmpid = SML_DSGetRT_ID(type);
	if (tmpid >= 0)
		return SGCore_RTGetTexture(tmpid);
	else
		return 0;
}

//#############################################################################

ID SML_MtlLoad(const char* name, MtlTypeModel mtl_type)
{
	ML_PRECOND(-1);

	return ArrMaterials->MtlLoad(name, mtl_type);
}

void SML_MtlSave(ID id)
{
	ML_PRECOND();

	ArrMaterials->MtlSave(id);
}

MtlTypeModel SML_MtlGetTypeModel(ID id)
{
	ML_PRECOND(MtlTypeModel::tms_static);

	return ArrMaterials->GetTypeModel(id);
}

void SML_MtlSetTypeModel(ID id, MtlTypeModel type_model)
{
	ML_PRECOND();

	ArrMaterials->SetTypeModel(id, type_model);
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

void SML_MtlRenderStd(MtlTypeModel type, float4x4* world, ID slot, ID id_mtl)
{
	ML_PRECOND();
	ArrMaterials->RenderStd(type, world, slot, id_mtl);
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

void SML_MtlSetIsIncrCountSurf(bool bf)
{
	ML_PRECOND();
	ArrMaterials->SetIsIncrCountSurf(bf);
}

bool SML_MtlGetIsIncrCountSurf()
{
	ML_PRECOND(false);
	return ArrMaterials->GetIsIncrCountSurf();
}

void SML_MtlNullingCurrCountSurf()
{
	ML_PRECOND();
	ArrMaterials->NullingCurrCountSurf();
}

int SML_MtlGetCurrCountSurf()
{
	ML_PRECOND(-1);
	return ArrMaterials->GetCurrCountSurf();
}

void SML_MtlSetCurrCountSurf(int count)
{
	ML_PRECOND();
	ArrMaterials->SetCurrCountSurf(count);
}

void SML_MtlSetForceblyAlphaTest(bool isat)
{
	ML_PRECOND();
	ArrMaterials->SetForceblyAlphaTest(isat);
}

bool SML_MtlGetForceblyAlphaTest()
{
	ML_PRECOND(false);
	return ArrMaterials->GetForceblyAlphaTest();
}


ID SML_MtlGetStdMtl(MtlTypeModel type_model)
{
	ML_PRECOND(-1);
	return ArrMaterials->GetStdMtl(type_model);
}


void SML_MtlSetPhysicMaterial(ID id, MtlPhysicType type)
{
	ML_PRECOND();
	ArrMaterials->MtlSetPhysicMaterial(id, type);
}

MtlPhysicType SML_MtlGetPhysicMaterial(ID id)
{
	ML_PRECOND((MtlPhysicType )-1);
	return ArrMaterials->MtlGetPhysicMaterial(id);
}

bool SML_MtlGetUsingAlphaTest(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetUsingAlphaTest(id);
}

void SML_MtlSetUsingAlphaTest(ID id, bool is_using)
{
	ML_PRECOND();
	ArrMaterials->MtlSetUsingAlphaTest(id, is_using);
}



void SML_MtlRefSetIDArr(ID id, ID inid, int cube, ID idarr)
{
	ML_PRECOND();
	ArrMaterials->MtlRefSetIDArr(id, inid, cube, idarr);
}

ID SML_MtlRefGetIDArr(ID id, ID inid, int cube)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlRefGetIDArr(id, inid, cube);
}

int SML_MtlDelRefGetCount()
{
	ML_PRECOND(-1);
	return ArrMaterials->DelRefCount();
}

ID SML_MtlDelRefGetIDArr(ID key, ID inid, int cube)
{
	ML_PRECOND(-1);
	return ArrMaterials->DelRefGetIDArr(key, inid, cube);
}

void SML_MtlDelRefClear()
{
	ML_PRECOND();
	ArrMaterials->DelRefAllDel();
}

void SML_MtlRefSetMinMax(ID id, float3_t* min, float3_t* max)
{
	ML_PRECOND();
	ArrMaterials->MtlRefSetMinMax(id, min, max);
}

void SML_MtlRefPreRenderPlane(ID id, D3DXPLANE* plane)
{
	ML_PRECOND();
	ArrMaterials->MtlRefPreRenderPlane(id, plane);
}

ISXFrustum* SML_MtlRefGetfrustum(ID id, int cube)
{
	ML_PRECOND(0);
	return ArrMaterials->MtlRefGetfrustum(id, cube);
}

bool SML_MtlRefIsAllowedRender(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlRefIsAllowedRender(id);
}

void SML_MtlRefPostRenderPlane(ID id)
{
	ML_PRECOND();
	ArrMaterials->MtlRefPostRenderPlane(id);
}

IDirect3DTexture9* SML_MtlRefGetTexPlane(ID id)
{
	ML_PRECOND(0);
	return ArrMaterials->MtlRefPlaneGetTex(id);
}



void SML_MtlRefCubeBeginRender(ID id, float3_t* center)
{
	ML_PRECOND();
	ArrMaterials->MtlRefCubeBeginRender(id, center);
}

void SML_MtlRefCubePreRender(ID id, int cube, float4x4* world)
{
	ML_PRECOND();
	ArrMaterials->MtlRefCubePreRender(id, cube, world);
}

void SML_MtlRefCubePostRender(ID id, int cube)
{
	ML_PRECOND();
	ArrMaterials->MtlRefCubePostRender(id, cube);
}

void SML_MtlRefCubeEndRender(ID id, float3_t* viewpos)
{
	ML_PRECOND();
	ArrMaterials->MtlRefCubeEndRender(id, viewpos);
}

bool SML_MtlRefUpdateCountUpdate(ID id, float3_t* viewpos)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlRefUpdateCountUpdate(id, viewpos);
}

void SML_MtlRefNullingCountUpdate(ID id)
{
	ML_PRECOND();
	ArrMaterials->MtlRefNullingCountUpdate(id);
}

IDirect3DCubeTexture9* SML_MtlRefCubeGetTex(ID id)
{
	ML_PRECOND(0);
	return ArrMaterials->RefCubeGetTex(id);
}

////

void SML_MtlReloadMaterial(ID id, const char* name)
{
	ML_PRECOND();
	ArrMaterials->MtlReLoad(id, name);
}

void SML_MtlClear(bool clear_ref_del)
{
	ML_PRECOND();
	ArrMaterials->Clear(clear_ref_del);
}

void SML_MtlSetTexture(ID id, const char* path_tex)
{
	ML_PRECOND();
	ArrMaterials->MtlSetTexture(id, path_tex);
}

void SML_MtlGetTexture(ID id, char* name)
{
	ML_PRECOND();
	ArrMaterials->MtlGetTexture(id, name);
}

ID SML_MtlGetTextureID(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetTextureID(id);
}

void SML_MtlSetVS(ID id, const char* path_vs)
{
	ML_PRECOND();
	ArrMaterials->MtlSetVS(id, path_vs);
}

void SML_MtlGetVS(ID id, char* name)
{
	ML_PRECOND();
	ArrMaterials->MtlGetVS(id, name);
}

void SML_MtlSetPS(ID id, const char* path_ps)
{
	ML_PRECOND();
	ArrMaterials->MtlSetPS(id, path_ps);
}

void SML_MtlGetPS(ID id, char* name)
{
	ML_PRECOND();
	ArrMaterials->MtlGetPS(id, name);
}

float SML_MtlGetPenetration(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetPenetration(id);
}

void SML_MtlSetPenetration(ID id, float penetration)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetPenetration(id, penetration);
}




void SML_MtlSetLighting(ID id, bool is_lighting)
{
	ML_PRECOND();
	ArrMaterials->MtlSetLighting(id, is_lighting);
}

bool SML_MtlGetLighting(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetLighting(id);
}


void SML_MtlSetIsTextureLighting(ID id, bool is_tex_lighting)
{
	ML_PRECOND();
	ArrMaterials->MtlSetIsTextureLighting(id, is_tex_lighting);
}

bool SML_MtlGetIsTextureLighting(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetIsTextureLighting(id);
}


void SML_MtlSetTextureLighting(ID id, const char* path_tex)
{
	ML_PRECOND();
	ArrMaterials->MtlSetTextureLighting(id, path_tex);
}

void SML_MtlGetTextureLighting(ID id, char* path_tex)
{
	ML_PRECOND();
	ArrMaterials->MtlGetTextureLighting(id, path_tex);
}


void SML_MtlSetRoughness(ID id, float roughness)
{
	ML_PRECOND();
	ArrMaterials->MtlSetRoughness(id, roughness);
}

float SML_MtlGetRoughness(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetRoughness(id);
}


void SML_MtlSetThickness(ID id, float thickness)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetThickness(id, thickness);
}

float SML_MtlGetThickness(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetThickness(id);
}


void SML_MtlSetF0(ID id, float f0)
{
	ML_PRECOND();
	ArrMaterials->MtlSetF0(id, f0);
}

float SML_MtlGetF0(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetF0(id);
}


void SML_MtlSetTypeTransparency(ID id, MtlTypeTransparency type)
{
	ML_PRECOND();
	ArrMaterials->MtlSetTypeTransparency(id, type);
}

MtlTypeTransparency SML_MtlGetTypeTransparency(ID id)
{
	ML_PRECOND(MtlTypeTransparency::mtt_none);
	return ArrMaterials->MtlGetTypeTransparency(id);
}


void SML_MtlSetTypeReflection(ID id, MtlTypeReflect type)
{
	ML_PRECOND();
	ArrMaterials->MtlSetTypeReflection(id, type);
}

MtlTypeReflect SML_MtlGetTypeReflection(ID id)
{
	ML_PRECOND(MtlTypeReflect::mtr_none);
	return ArrMaterials->MtlGetTypeReflection(id);
}





void SML_MtlSetMaskTex(ID id, const char* path_tex)
{
	ML_PRECOND();
	ArrMaterials->MtlSetMaskTex(id, path_tex);
}

void SML_MtlGetMaskTex(ID id, char* path_tex)
{
	ML_PRECOND();
	ArrMaterials->MtlGetMaskTex(id, path_tex);
}


void SML_MtlSetMRTex(ID id, int channel, const char* path_tex)
{
	ML_PRECOND();
	ArrMaterials->MtlSetMRTex(id, channel, path_tex);
}

void SML_MtlGetMRTex(ID id, int channel, char* path_tex)
{
	ML_PRECOND();
	ArrMaterials->MtlGetMRTex(id, channel, path_tex);
}


void SML_MtlSetDTex(ID id, int channel, const char* path_tex)
{
	ML_PRECOND();
	ArrMaterials->MtlSetDTex(id, channel, path_tex);
}

void SML_MtlGetDTex(ID id, int channel, char* path_tex)
{
	ML_PRECOND();
	ArrMaterials->MtlGetDTex(id, channel, path_tex);
}


void SML_MtlSetSTDVS(ID id, MtlTransShaderStd type, bool is_send)
{
	ML_PRECOND();
	ArrMaterials->MtlSetSTDVS(id, type, is_send);
}

bool SML_MtlGetSTDVS(ID id, MtlTransShaderStd type)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetSTDVS(id, type);
}

void SML_MtlSetSTDPS(ID id, MtlTransShaderStd type, bool is_send)
{
	ML_PRECOND();
	ArrMaterials->MtlSetSTDPS(id, type, is_send);
}

bool SML_MtlGetSTDPS(ID id, MtlTransShaderStd type)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetSTDPS(id, type);
}



void SML_MtlSetUDVS(ID id, int component, float val)
{
	ML_PRECOND();
	ArrMaterials->MtlSetUDVS(id, component, val);
}

float SML_MtlGetUDVS(ID id, int component)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetUDVS(id, component);
}

void SML_MtlSetUDVS_InPS(ID id, bool is_send_ps)
{
	ML_PRECOND();
	ArrMaterials->MtlSetUDVS_InPS(id, is_send_ps);
}

bool SML_MtlGetUDVS_InPS(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetUDVS_InPS(id);
}

void SML_MtlSetUDPS(ID id, int component, float val)
{
	ML_PRECOND();
	ArrMaterials->MtlSetUDPS(id, component, val);
}

float SML_MtlGetUDPS(ID id, int component)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetUDPS(id, component);
}

void SML_MtlSetUDPS_InVS(ID id, bool is_send_vs)
{
	ML_PRECOND();
	ArrMaterials->MtlSetUDPS_InPS(id, is_send_vs);
}

bool SML_MtlGetUDPS_InVS(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetUDPS_InPS(id);
}
