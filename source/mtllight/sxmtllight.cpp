
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXMATERIAL_LIGTH_VERSION 1

#include "sxmtllight.h"
#include "ml_data.h"

#include "light.h"
#include "material.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

Lights* ArrLights = 0;
Materials* ArrMaterials = 0;

#define ML_PRECOND(retval) if(!ArrLights){LibReport(-1, "%s - sxmtlligth is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SML_0GetVersion()
{
	return SXMATERIAL_LIGTH_VERSION;
}

SX_LIB_API void SML_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SML_0Create(const char* name, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
			}
			else
			{
				MLSet::MLInit();
				MLSet::ReCalcSize();
				ArrLights = new Lights();
				ArrMaterials = new Materials();
			}
		}
		else
		{
			MLSet::MLInit();
			MLSet::ReCalcSize();
			ArrLights = new Lights();
			ArrMaterials = new Materials();
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SML_AKill()
{
	mem_delete(ArrLights)
	mem_delete(ArrMaterials);
}

SX_LIB_API void SML_OnLostDevice()
{
	ML_PRECOND(_VOID);
	ArrLights->OnLostDevice();
	ArrMaterials->OnLostDevice();
}

SX_LIB_API void SML_OnResetDevice()
{
	ML_PRECOND(_VOID);
	/*MLSet::WinSize.x = width;
	MLSet::WinSize.y = heigth;
	MLSet::ProjFov = projfov;
	MLSet::ProjRatio = MLSet::WinSize.x / MLSet::WinSize.y;*/
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

SX_LIB_API void SML_Update(DWORD timeDelta)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	if (ArrMaterials)
		ArrMaterials->Update(timeDelta);

	MLSet::RefMProjPlane = SMMatrixPerspectiveFovLH(*r_default_fov, float(*r_win_width) / float(*r_win_height), MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
	MLSet::RefMProjCube = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
}

SX_LIB_API void SML_LigthsSettLCoefSizeDepth(float coef)
{
	ML_PRECOND(_VOID);

	MLSet::CoefSizeDepthMapForLocal = coef;
	MLSet::ReCalcSize();
	SML_OnLostDevice();
	SML_OnResetDevice();
}

SX_LIB_API void SML_LigthsSettGCoefSizeDepth(float coef)
{
	ML_PRECOND(_VOID);

	MLSet::CoefSizeDepthMapForGlobal = coef;
	MLSet::ReCalcSize();
	SML_OnLostDevice();
	SML_OnResetDevice();
	
}

SX_LIB_API float SML_LigthsGettLCoefSizeDepth()
{
	ML_PRECOND(-1);

	return MLSet::CoefSizeDepthMapForLocal;
}

SX_LIB_API float SML_LigthsGettGCoefSizeDepth()
{
	ML_PRECOND(-1);
	return MLSet::CoefSizeDepthMapForGlobal;
}

//#############################################################################

SX_LIB_API void SML_LigthsClear()
{
	ML_PRECOND(_VOID);

	ArrLights->Clear();
}

SX_LIB_API void SML_LigthsClearIDArr()
{
	ML_PRECOND(_VOID);

	ArrLights->ClearIDArr();
}

SX_LIB_API long SML_LigthsGetCount()
{
	ML_PRECOND(-1);

	return ArrLights->GetCountLights();
}

SX_LIB_API bool SML_LigthsGetExists(ID id)
{
	ML_PRECOND(false);

	return ArrLights->getExists(id);
}


SX_LIB_API ID SML_LigthsCreatePoint(const float3* center, float dist, const float3* color, bool isglobal, bool is_shadowed/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreatePoint(-1,center, dist, color, isglobal, is_shadowed/*, 0*/);
}

SX_LIB_API ID SML_LigthsCreateDirection(const float3* pos, float dist, const float3* color, const SMQuaternion* orient, float top_radius, float angle, bool is_shadow/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->CreateDirection(-1, pos, dist, color, orient, top_radius, angle, is_shadow/*, 0*/);
}

SX_LIB_API void SML_LigthsRender(ID id, DWORD timeDelta)
{
	ML_PRECOND(_VOID);
	ArrLights->Render(id, timeDelta);
}

SX_LIB_API ID SML_LigthsGetGlobal()
{
	ML_PRECOND(-1);
	return ArrLights->GetLightGlobal();
}

SX_LIB_API bool SML_LigthsGetCastGlobalShadow()
{
	ML_PRECOND(false);
	return ArrLights->GetCastGlobalShadow();
}

SX_LIB_API void SML_LigthsSetCastGlobalShadow(bool isShadowed)
{
	ML_PRECOND(_VOID);
	return ArrLights->SetCastGlobalShadow(isShadowed);
}

SX_LIB_API void SML_LigthsGetColor(ID id, float3* color)
{
	ML_PRECOND(_VOID);
	ArrLights->GetLightColor(id, color);
}

SX_LIB_API void SML_LigthsSetColor(ID id, const float3* vec)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightColor(id, vec);
}

SX_LIB_API void SML_LigthsGetPos(ID id, float3* vec, bool greal)
{
	ML_PRECOND(_VOID);
	ArrLights->GetLightPos(id, vec, greal);
}

SX_LIB_API float SML_LigthsGetPower(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightPower(id);
}

SX_LIB_API float SML_LigthsGetDist(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightDist(id);
}

SX_LIB_API void SML_LigthsSetPos(ID id, const float3* vec, bool greal)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightPos(id, vec, greal);
}

SX_LIB_API void SML_LigthsGetOrient(ID id, SMQuaternion* q)
{
	ML_PRECOND(_VOID);
	ArrLights->GetLightOrient(id, q);
}

SX_LIB_API void SML_LigthsSetOrient(ID id, const SMQuaternion* q)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightOrient(id, q);
}

SX_LIB_API LTYPE_LIGHT SML_LigthsGetType(ID id)
{
	ML_PRECOND(LTYPE_LIGHT_NONE);
	return ArrLights->GetLightType(id);
}

SX_LIB_API bool SML_LigthsComVisibleForFrustum(ID id, const ISXFrustum* frustum)
{
	ML_PRECOND(-1);
	return ArrLights->ComVisibleForFrustum(id, frustum);
}

SX_LIB_API bool SML_LigthsGetVisibleForFrustum(ID id)
{
	ML_PRECOND(false);
	return ArrLights->GetVisibleForFrustum(id);
}

SX_LIB_API float SML_LigthsComDistFor(ID id, const float3* vec)
{
	ML_PRECOND(-1);
	return ArrLights->ComDistFor(id, vec);
}

SX_LIB_API float SML_LigthsGetDistFor(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetDistFor(id);
}

SX_LIB_API void SML_LigthsComVisibleFrustumDistFor(const ISXFrustum* frustum, const float3* vec)
{
	ML_PRECOND(_VOID);
	return ArrLights->ComVisibleFrustumDistFor(frustum, vec);
}

SX_LIB_API bool SML_LigthsGetEnable(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightEnable(id);
}

SX_LIB_API void SML_LigthsSetEnable(ID id, bool val)
{
	ML_PRECOND(_VOID);
	return ArrLights->SetLightEnable(id, val);
}

SX_LIB_API bool SML_LigthsGetShadowed(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightShadowed(id);
}

SX_LIB_API bool SML_LigthsGet4Or3SplitsG(ID id)
{
	ML_PRECOND(0);
	return ArrLights->Get4Or3Splits(id);
}

SX_LIB_API void SML_LigthsSet4Or3SplitsG(ID id, bool bf4or3)
{
	ML_PRECOND(_VOID);

	ArrLights->Set4Or3Splits(id, bf4or3);
}

SX_LIB_API IDirect3DTexture9* SML_LigthsGetShadow()
{
	ML_PRECOND(0);
	return ArrLights->GetShadow2();
}

SX_LIB_API void SML_LigthsShadowNull()
{
	ML_PRECOND(_VOID);
	return ArrLights->ShadowNull();
}

SX_LIB_API void SML_LigthsShadowRenderBegin(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->ShadowRenderBegin(id);
}

SX_LIB_API void SML_LigthsShadowRenderEnd(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->ShadowRenderEnd(id);
}

SX_LIB_API void SML_LigthsShadowRenderPre(ID id, int cube)
{
	ML_PRECOND(_VOID);
	ArrLights->ShadowRenderPre(id, cube);
}

SX_LIB_API void SML_LigthsShadowRenderPost(ID id, int cube)
{
	ML_PRECOND(_VOID);
	ArrLights->ShadowRenderPost(id, cube);
}

SX_LIB_API ISXFrustum* SML_LigthsGetFrustum(ID id, int how)
{
	ML_PRECOND(0);
	return ArrLights->GetLightFrustum(id, how);
}

SX_LIB_API void SML_LigthsUpdateGFrustums(ID id, int split, const float3* pos, const float3* dir)
{
	ML_PRECOND(_VOID);
	return ArrLights->UpdateLightGFrustums(id, split, pos, dir);
}

SX_LIB_API void SML_LigthsShadowSetShaderOfTypeMat(ID id, int typemat, float4x4* wmat)
{
	ML_PRECOND(_VOID);
	ArrLights->InitShaderOfTypeMaterial(id, typemat, wmat);
}

SX_LIB_API void SML_LigthsShadowGen(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->ShadowGen2(id);
}

SX_LIB_API void SML_LigthsShadowSoft(bool randomsam, float size, bool isfirst)
{
	ML_PRECOND(_VOID);
	ArrLights->ShadowSoft(randomsam, size, isfirst);
}



SX_LIB_API ID SML_LigthsCreateCopy(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->CreateCopy(id);
}

SX_LIB_API void SML_LigthsDeleteLight(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->DeleteLight(id);
}

SX_LIB_API char* SML_LigthsGetName(ID id)
{
	ML_PRECOND(0);
	return ArrLights->GetLightName(id);
}

SX_LIB_API void SML_LigthsSetName(ID id, const char* name)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightName(id, name);
}


SX_LIB_API void SML_LigthsSetAngle(ID id, float angle)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightAngle(id, angle, true);
}

SX_LIB_API void SML_LigthsSetTopRadius(ID id, float top_radius)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightTopRadius(id, top_radius);
}

SX_LIB_API void SML_LigthsSetDist(ID id, float radius_height, bool is_create)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightDist(id, radius_height, is_create);
}


SX_LIB_API float SML_LigthsGetShadowBias(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowBias(id);
}

SX_LIB_API void SML_LigthsSetShadowBias(ID id, float bias)
{
	ML_PRECOND(_VOID);
	return ArrLights->SetShadowBias(id, bias);
}

SX_LIB_API void SML_LigthsSetShadowBlurPixel(ID id, float blur_pixel)
{
	ML_PRECOND(_VOID);
	ArrLights->SetShadowBlurPixel(id, blur_pixel);
}

SX_LIB_API float SML_LigthsGetShadowBlurPixel(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowBlurPixel(id);
}
SX_LIB_API void SML_LigthsSetShadowLocalNear(ID id, float slnear)
{
	ML_PRECOND(_VOID);
	ArrLights->SetShadowLocalNear(id, slnear);
}

SX_LIB_API float SML_LigthsGetShadowLocalNear(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowLocalNear(id);
}

SX_LIB_API void SML_LigthsSetShadowLocalFar(ID id, float slfar)
{
	ML_PRECOND(_VOID);
	ArrLights->SetShadowLocalFar(id, slfar);
}

SX_LIB_API float SML_LigthsGetShadowLocalFar(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetShadowLocalFar(id);
}

SX_LIB_API void SML_LigthsSetCubeEdgeEnable(ID id, int edge, bool enable)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightCubeEdgeEnable(id, edge, enable);
}

SX_LIB_API bool SML_LigthsGetCubeEdgeEnable(ID id, int edge)
{
	ML_PRECOND(false);
	return ArrLights->GetLightCubeEdgeEnable(id, edge);
}


SX_LIB_API float SML_LigthsGetAngle(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightAngle(id);
}

SX_LIB_API float SML_LigthsGetTopRadius(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightTopRadius(id);
}

SX_LIB_API ID SML_LigthsGetIDArr(ID id, ID inid, int how)
{
	ML_PRECOND(-1);
	return ArrLights->GetLightIDArr(id, inid, how);
}

SX_LIB_API void SML_LigthsSetIDArr(ID id, ID inid, int how, ID id_arr)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightIDArr(id, inid, how, id_arr);
}

SX_LIB_API void SML_LigthsSetTypeShadowed(ID id, LTYPE_SHADOW type)
{
	ML_PRECOND(_VOID);
	ArrLights->SetLightTypeShadowed(id, type);
}

SX_LIB_API LTYPE_SHADOW SML_LigthsGetTypeShadowed(ID id)
{
	ML_PRECOND(LTYPE_SHADOW_NONE);
	return ArrLights->GetLightTypeShadowed(id);
}

SX_LIB_API bool SML_LigthsCountUpdateUpdate(ID id, const float3* viewpos, int ghow)
{
	ML_PRECOND(false);
	return ArrLights->LightCountUpdateUpdate(id, viewpos, ghow);
}

SX_LIB_API bool SML_LigthsCountUpdateAllowed(ID id, int ghow)
{
	ML_PRECOND(false);
	return ArrLights->LightCountUpdateAllowed(id, ghow);
}

SX_LIB_API void SML_LigthsCountUpdateNull(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->LightCountUpdateNull(id);
}

SX_LIB_API void SML_LigthsToneMappingCom(DWORD timeDelta, float factor_adapted)
{
	ML_PRECOND(_VOID);
	ArrLights->ToneMappingCom(timeDelta, factor_adapted);
}

SX_LIB_API int SML_LigthsDelGetCount()
{
	ML_PRECOND(-1);
	return ArrLights->DelGetCount();
}

SX_LIB_API LTYPE_LIGHT SML_LigthsDelGetType(ID key)
{
	ML_PRECOND(LTYPE_LIGHT_NONE);
	return ArrLights->DelGetType(key);
}

SX_LIB_API void SML_LigthsDelDel(ID key)
{
	ML_PRECOND(_VOID);
	ArrLights->DelDel(key);
}

SX_LIB_API ID SML_LigthsDelGetIDArr(ID key, ID inid, int how)
{
	ML_PRECOND(-1);
	return ArrLights->DelGetIDArr(key, inid, how);
}

//#############################################################################

SX_LIB_API ID SML_DSGetRT_ID(DS_RT type)
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
	else if (type == DS_RT_DEPTH0)
		return MLSet::IDsRenderTargets::DepthScene0;
	else if (type == DS_RT_DEPTH1)
		return MLSet::IDsRenderTargets::DepthScene1;

	else if (type == DS_RT_AMBIENTDIFF)
		return MLSet::IDsRenderTargets::LightAmbientDiff;
	else if (type == DS_RT_SPECULAR)
		return MLSet::IDsRenderTargets::LightSpecular;

	else if (type == DS_RT_SCENELIGHT)
		return MLSet::IDsRenderTargets::LigthCom;
	else if (type == DS_RT_SCENELIGHT2)
		return MLSet::IDsRenderTargets::LigthCom2;

	else if (type == DS_RT_ADAPTEDLUM)
		return MLSet::IDsRenderTargets::GetCurrAdaptedLum();

	return -1;
}

SX_LIB_API IDirect3DTexture9* SML_DSGetRT(DS_RT type)
{
	ML_PRECOND(0);

	ID tmpid = SML_DSGetRT_ID(type);
	if (tmpid >= 0)
		return SGCore_RTGetTexture(tmpid);
	else
		return 0;
}

//#############################################################################

SX_LIB_API ID SML_MtlLoad(const char* name, MTLTYPE_MODEL mtl_type)
{
	ML_PRECOND(-1);

	return ArrMaterials->MtlLoad(name, mtl_type);
}

SX_LIB_API void SML_MtlSave(ID id)
{
	ML_PRECOND(_VOID);

	ArrMaterials->MtlSave(id);
}

SX_LIB_API MTLTYPE_MODEL SML_MtlGetTypeModel(ID id)
{
	ML_PRECOND(MTLTYPE_MODEL_STATIC);

	return ArrMaterials->GetTypeModel(id);
}

SX_LIB_API void SML_MtlSetTypeModel(ID id, MTLTYPE_MODEL type_model)
{
	ML_PRECOND(_VOID);

	ArrMaterials->SetTypeModel(id, type_model);
}

SX_LIB_API long SML_MtlGetCount()
{
	ML_PRECOND(-1);

	return ArrMaterials->GetCount();
}

SX_LIB_API void SML_MtlRender(ID id, float4x4* world)
{
	ML_PRECOND(_VOID);
	ArrMaterials->Render(id, world);
}

SX_LIB_API void SML_MtlRenderStd(MTLTYPE_MODEL type, float4x4* world, ID slot, ID id_mtl)
{
	ML_PRECOND(_VOID);
	ArrMaterials->RenderStd(type, world, slot, id_mtl);
}

SX_LIB_API void SML_MtlRenderLight(float4_t* color, float4x4* world)
{
	ML_PRECOND(_VOID);
	ArrMaterials->RenderLight(color, world);
}

SX_LIB_API void SML_MtlSetMainTexture(ID slot, ID id)
{
	ML_PRECOND(_VOID);
	ArrMaterials->SetMainTexture(slot, id);
}

SX_LIB_API void SML_MtlSetIsIncrCountSurf(bool bf)
{
	ML_PRECOND(_VOID);
	ArrMaterials->SetIsIncrCountSurf(bf);
}

SX_LIB_API bool SML_MtlGetIsIncrCountSurf()
{
	ML_PRECOND(false);
	return ArrMaterials->GetIsIncrCountSurf();
}

SX_LIB_API void SML_MtlNullingCurrCountSurf()
{
	ML_PRECOND(_VOID);
	ArrMaterials->NullingCurrCountSurf();
}

SX_LIB_API int SML_MtlGetCurrCountSurf()
{
	ML_PRECOND(-1);
	return ArrMaterials->GetCurrCountSurf();
}

SX_LIB_API void SML_MtlSetCurrCountSurf(int count)
{
	ML_PRECOND(_VOID);
	ArrMaterials->SetCurrCountSurf(count);
}

SX_LIB_API void SML_MtlSetForceblyAlphaTest(bool isat)
{
	ML_PRECOND(_VOID);
	ArrMaterials->SetForceblyAlphaTest(isat);
}

SX_LIB_API bool SML_MtlGetForceblyAlphaTest()
{
	ML_PRECOND(false);
	return ArrMaterials->GetForceblyAlphaTest();
}


SX_LIB_API ID SML_MtlGetStdMtl(MTLTYPE_MODEL type_model)
{
	ML_PRECOND(-1);
	return ArrMaterials->GetStdMtl(type_model);
}


SX_LIB_API void SML_MtlSetPhysicMaterial(ID id, MTLTYPE_PHYSIC type)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetPhysicMaterial(id, type);
}

SX_LIB_API MTLTYPE_PHYSIC SML_MtlGetPhysicMaterial(ID id)
{
	ML_PRECOND(MTLTYPE_PHYSIC_DEFAULT);
	return ArrMaterials->MtlGetPhysicMaterial(id);
}

SX_LIB_API bool SML_MtlGetUsingAlphaTest(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetUsingAlphaTest(id);
}

SX_LIB_API void SML_MtlSetUsingAlphaTest(ID id, bool is_using)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetUsingAlphaTest(id, is_using);
}



SX_LIB_API void SML_MtlRefSetIDArr(ID id, ID inid, int cube, ID idarr)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlRefSetIDArr(id, inid, cube, idarr);
}

SX_LIB_API ID SML_MtlRefGetIDArr(ID id, ID inid, int cube)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlRefGetIDArr(id, inid, cube);
}

SX_LIB_API int SML_MtlDelRefGetCount()
{
	ML_PRECOND(-1);
	return ArrMaterials->DelRefCount();
}

SX_LIB_API ID SML_MtlDelRefGetIDArr(ID key, ID inid, int cube)
{
	ML_PRECOND(-1);
	return ArrMaterials->DelRefGetIDArr(key, inid, cube);
}

SX_LIB_API void SML_MtlDelRefClear()
{
	ML_PRECOND(_VOID);
	ArrMaterials->DelRefAllDel();
}

SX_LIB_API void SML_MtlRefSetMinMax(ID id, float3_t* min, float3_t* max)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlRefSetMinMax(id, min, max);
}

SX_LIB_API void SML_MtlRefPreRenderPlane(ID id, D3DXPLANE* plane)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlRefPreRenderPlane(id, plane);
}

SX_LIB_API const ISXFrustum* SML_MtlRefGetfrustum(ID id, int cube)
{
	ML_PRECOND(0);
	return ArrMaterials->MtlRefGetfrustum(id, cube);
}

SX_LIB_API bool SML_MtlRefIsAllowedRender(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlRefIsAllowedRender(id);
}

SX_LIB_API void SML_MtlRefPostRenderPlane(ID id)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlRefPostRenderPlane(id);
}

SX_LIB_API IDirect3DTexture9* SML_MtlRefGetTexPlane(ID id)
{
	ML_PRECOND(0);
	return ArrMaterials->MtlRefPlaneGetTex(id);
}



SX_LIB_API void SML_MtlRefCubeBeginRender(ID id, float3_t* center)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlRefCubeBeginRender(id, center);
}

SX_LIB_API void SML_MtlRefCubePreRender(ID id, int cube, float4x4* world)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlRefCubePreRender(id, cube, world);
}

SX_LIB_API void SML_MtlRefCubePostRender(ID id, int cube)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlRefCubePostRender(id, cube);
}

SX_LIB_API void SML_MtlRefCubeEndRender(ID id, float3_t* viewpos)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlRefCubeEndRender(id, viewpos);
}

SX_LIB_API bool SML_MtlRefUpdateCountUpdate(ID id, float3_t* viewpos)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlRefUpdateCountUpdate(id, viewpos);
}

SX_LIB_API void SML_MtlRefNullingCountUpdate(ID id)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlRefNullingCountUpdate(id);
}

SX_LIB_API IDirect3DCubeTexture9* SML_MtlRefCubeGetTex(ID id)
{
	ML_PRECOND(0);
	return ArrMaterials->RefCubeGetTex(id);
}

//##########################################################################

SX_LIB_API void SML_MtlReloadMaterial(ID id, const char* name)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlReLoad(id, name);
}

SX_LIB_API void SML_MtlClear(bool clear_ref_del)
{
	ML_PRECOND(_VOID);
	ArrMaterials->Clear(clear_ref_del);
}

SX_LIB_API void SML_MtlSetTexture(ID id, const char* path_tex)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetTexture(id, path_tex);
}

SX_LIB_API void SML_MtlGetTexture(ID id, char* name)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlGetTexture(id, name);
}

SX_LIB_API ID SML_MtlGetTextureID(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetTextureID(id);
}

SX_LIB_API void SML_MtlSetVS(ID id, const char* path_vs)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetVS(id, path_vs);
}

SX_LIB_API void SML_MtlGetVS(ID id, char* name)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlGetVS(id, name);
}

SX_LIB_API void SML_MtlSetPS(ID id, const char* path_ps)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetPS(id, path_ps);
}

SX_LIB_API void SML_MtlGetPS(ID id, char* name)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlGetPS(id, name);
}

SX_LIB_API float SML_MtlGetPenetration(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetPenetration(id);
}

SX_LIB_API void SML_MtlSetPenetration(ID id, float penetration)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetPenetration(id, penetration);
}

SX_LIB_API float SML_MtlGetHitChance(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetHitChance(id);
}

SX_LIB_API void SML_MtlSetHitChance(ID id, float fHitChance)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetHitChance(id, fHitChance);
}

SX_LIB_API float SML_MtlGetDensity(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetDensity(id);
}

SX_LIB_API void SML_MtlSetDensity(ID id, float fDensity)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetDensity(id, fDensity);
}

//##########################################################################

SX_LIB_API void SML_MtlSetLighting(ID id, bool is_lighting)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetLighting(id, is_lighting);
}

SX_LIB_API bool SML_MtlGetLighting(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetLighting(id);
}

SX_LIB_API void SML_MtlSetIsTextureLighting(ID id, bool is_tex_lighting)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetIsTextureLighting(id, is_tex_lighting);
}

SX_LIB_API bool SML_MtlGetIsTextureLighting(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetIsTextureLighting(id);
}

SX_LIB_API void SML_MtlSetTextureLighting(ID id, const char* path_tex)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetTextureLighting(id, path_tex);
}

SX_LIB_API void SML_MtlGetTextureLighting(ID id, char* path_tex)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlGetTextureLighting(id, path_tex);
}

SX_LIB_API void SML_MtlSetRoughness(ID id, float roughness)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetRoughness(id, roughness);
}

SX_LIB_API float SML_MtlGetRoughness(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetRoughness(id);
}

SX_LIB_API void SML_MtlSetThickness(ID id, float thickness)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetThickness(id, thickness);
}

SX_LIB_API float SML_MtlGetThickness(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetThickness(id);
}

SX_LIB_API void SML_MtlSetF0(ID id, float f0)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetF0(id, f0);
}

SX_LIB_API float SML_MtlGetF0(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetF0(id);
}

SX_LIB_API void SML_MtlSetTypeTransparency(ID id, MTLTYPE_TRANSPARENCY type)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetTypeTransparency(id, type);
}

SX_LIB_API MTLTYPE_TRANSPARENCY SML_MtlGetTypeTransparency(ID id)
{
	ML_PRECOND(MTLTYPE_TRANSPARENCY_NONE);
	return ArrMaterials->MtlGetTypeTransparency(id);
}

SX_LIB_API void SML_MtlSetTypeReflection(ID id, MTLTYPE_REFLECT type)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetTypeReflection(id, type);
}

SX_LIB_API MTLTYPE_REFLECT SML_MtlGetTypeReflection(ID id)
{
	ML_PRECOND(MTLTYPE_REFLECT_NONE);
	return ArrMaterials->MtlGetTypeReflection(id);
}

//**************************************************************************

SX_LIB_API void SML_MtlSetMaskTex(ID id, const char* path_tex)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetMaskTex(id, path_tex);
}

SX_LIB_API void SML_MtlGetMaskTex(ID id, char* path_tex)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlGetMaskTex(id, path_tex);
}

SX_LIB_API void SML_MtlSetMRTex(ID id, int channel, const char* path_tex)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetMRTex(id, channel, path_tex);
}

SX_LIB_API void SML_MtlGetMRTex(ID id, int channel, char* path_tex)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlGetMRTex(id, channel, path_tex);
}

SX_LIB_API void SML_MtlSetDTex(ID id, int channel, const char* path_tex)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetDTex(id, channel, path_tex);
}

SX_LIB_API void SML_MtlGetDTex(ID id, int channel, char* path_tex)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlGetDTex(id, channel, path_tex);
}

SX_LIB_API void SML_MtlSetSTDVS(ID id, MTL_SHADERSTD type, bool is_send)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetSTDVS(id, type, is_send);
}

SX_LIB_API bool SML_MtlGetSTDVS(ID id, MTL_SHADERSTD type)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetSTDVS(id, type);
}

SX_LIB_API void SML_MtlSetSTDPS(ID id, MTL_SHADERSTD type, bool is_send)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetSTDPS(id, type, is_send);
}

SX_LIB_API bool SML_MtlGetSTDPS(ID id, MTL_SHADERSTD type)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetSTDPS(id, type);
}


SX_LIB_API void SML_MtlSetUDVS(ID id, int component, float val)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetUDVS(id, component, val);
}

SX_LIB_API float SML_MtlGetUDVS(ID id, int component)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetUDVS(id, component);
}

SX_LIB_API void SML_MtlSetUDVS_InPS(ID id, bool is_send_ps)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetUDVS_InPS(id, is_send_ps);
}

SX_LIB_API bool SML_MtlGetUDVS_InPS(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetUDVS_InPS(id);
}

SX_LIB_API void SML_MtlSetUDPS(ID id, int component, float val)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetUDPS(id, component, val);
}

SX_LIB_API float SML_MtlGetUDPS(ID id, int component)
{
	ML_PRECOND(-1);
	return ArrMaterials->MtlGetUDPS(id, component);
}

SX_LIB_API void SML_MtlSetUDPS_InVS(ID id, bool is_send_vs)
{
	ML_PRECOND(_VOID);
	ArrMaterials->MtlSetUDPS_InPS(id, is_send_vs);
}

SX_LIB_API bool SML_MtlGetUDPS_InVS(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->MtlGetUDPS_InPS(id);
}
