
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXMATERIAL_LIGTH_VERSION 1

#include "sxmtrl.h"
#include "ml_data.h"

#include "material.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

CMaterials* ArrMaterials = 0;

#define ML_PRECOND(retval) if(!ArrMaterials){LibReport(-1, "%s - sxmtrl is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SMtrl_0GetVersion()
{
	return SXMATERIAL_LIGTH_VERSION;
}

SX_LIB_API void SMtrl_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SMtrl_0Create(const char *szName, bool isUnic)
{
	if (szName && strlen(szName) > 1)
	{
		if (isUnic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, szName);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
			}
			else
			{
				mtrl_data::Init();
				ArrMaterials = new CMaterials();
			}
		}
		else
		{
			mtrl_data::Init();
			ArrMaterials = new CMaterials();
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SMtrl_AKill()
{
	mem_delete(ArrMaterials);
}

SX_LIB_API bool SMtrl_AisInit()
{
	return (ArrMaterials ? true : false);
}

SX_LIB_API void SMtrl_OnLostDevice()
{
	ML_PRECOND(_VOID);
	ArrMaterials->onLostDevice();
}

SX_LIB_API void SMtrl_OnResetDevice()
{
	ML_PRECOND(_VOID);

	ArrMaterials->onResetDevice();

	//MLSet::rt_id::HowAdaptedLum = 0;
}

SX_LIB_API void SMtrl_Update(DWORD timeDelta)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	if (ArrMaterials)
		ArrMaterials->update(timeDelta);

	mtrl_data::mRefProjPlane = SMMatrixPerspectiveFovLH(*r_default_fov, float(*r_win_width) / float(*r_win_height), MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
	mtrl_data::mRefProjCube = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
}

//#############################################################################

SX_LIB_API ID SMtrl_MtlLoad(const char *szName, MTLTYPE_MODEL mtl_type)
{
	ML_PRECOND(-1);

	return ArrMaterials->mtlLoad(szName, mtl_type);
}

SX_LIB_API void SMtrl_MtlSave(ID id)
{
	ML_PRECOND(_VOID);

	ArrMaterials->mtlSave(id);
}

SX_LIB_API MTLTYPE_MODEL SMtrl_MtlGetTypeModel(ID id)
{
	ML_PRECOND(MTLTYPE_MODEL_STATIC);

	return ArrMaterials->getTypeModel(id);
}

SX_LIB_API ID SMtrl_MtlGetLightMtrl()
{
	ML_PRECOND(-1);

	return ArrMaterials->getLightMtrl();
}

/*SX_LIB_API UINT SMtrl_MtlGetSort(ID id)
{
	ML_PRECOND(0);

	return ArrMaterials->mtlGetSort(id);
}*/

SX_LIB_API bool SMtrl_MtlIsTransparency(ID id)
{
	ML_PRECOND(false);

	return ArrMaterials->mtlGetTransparency(id);
}

SX_LIB_API void SMtrl_MtlSetTypeModel(ID id, MTLTYPE_MODEL type_model)
{
	ML_PRECOND(_VOID);

	ArrMaterials->setTypeModel(id, type_model);
}

SX_LIB_API long SMtrl_MtlGetCount()
{
	ML_PRECOND(-1);

	return ArrMaterials->getCount();
}

SX_LIB_API void SMtrl_MtlRender(ID id, const float4x4 *pWorld, const float4 *pColor)
{
	ML_PRECOND(_VOID);
	ArrMaterials->render(id, pWorld, pColor);
}

SX_LIB_API void SMtrl_MtlRenderStd(MTLTYPE_MODEL type, const float4x4 *pWorld, ID idSlot, ID idMtl)
{
	ML_PRECOND(_VOID);
	ArrMaterials->renderStd(type, pWorld, idSlot, idMtl);
}

SX_LIB_API void SMtrl_MtlRenderLight(const float4_t *pColor, const float4x4 *pWorld)
{
	ML_PRECOND(_VOID);
	ArrMaterials->renderLight(pColor, pWorld);
}

SX_LIB_API void SMtrl_MtlSetMainTexture(ID slot, ID id)
{
	ML_PRECOND(_VOID);
	ArrMaterials->setMainTexture(slot, id);
}

SX_LIB_API void SMtrl_MtlSetIsIncrCountSurf(bool bf)
{
	ML_PRECOND(_VOID);
	ArrMaterials->setIsIncrCountSurf(bf);
}

SX_LIB_API bool SMtrl_MtlGetIsIncrCountSurf()
{
	ML_PRECOND(false);
	return ArrMaterials->getIsIncrCountSurf();
}

SX_LIB_API void SMtrl_MtlNullingCurrCountSurf()
{
	ML_PRECOND(_VOID);
	ArrMaterials->nullingCurrCountSurf();
}

SX_LIB_API int SMtrl_MtlGetCurrCountSurf()
{
	ML_PRECOND(-1);
	return ArrMaterials->getCurrCountSurf();
}

SX_LIB_API void SMtrl_MtlSetCurrCountSurf(int iCount)
{
	ML_PRECOND(_VOID);
	ArrMaterials->setCurrCountSurf(iCount);
}

SX_LIB_API void SMtrl_MtlSetForceblyAlphaTest(bool isAlphaTest)
{
	ML_PRECOND(_VOID);
	ArrMaterials->setForceblyAlphaTest(isAlphaTest);
}

SX_LIB_API bool SMtrl_MtlGetForceblyAlphaTest()
{
	ML_PRECOND(false);
	return ArrMaterials->getForceblyAlphaTest();
}


SX_LIB_API ID SMtrl_MtlGetStdMtl(MTLTYPE_MODEL type_model)
{
	ML_PRECOND(-1);
	return ArrMaterials->getStdMtl(type_model);
}


SX_LIB_API void SMtrl_MtlSetPhysicMaterial(ID id, MTLTYPE_PHYSIC type)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetPhysicMaterial(id, type);
}

SX_LIB_API MTLTYPE_PHYSIC SMtrl_MtlGetPhysicMaterial(ID id)
{
	ML_PRECOND(MTLTYPE_PHYSIC_DEFAULT);
	return ArrMaterials->mtlGetPhysicMaterial(id);
}

SX_LIB_API bool SMtrl_MtlGetUsingAlphaTest(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetUsingAlphaTest(id);
}

SX_LIB_API void SMtrl_MtlSetUsingAlphaTest(ID id, bool isUsing)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUsingAlphaTest(id, isUsing);
}



SX_LIB_API void SMtrl_RefSetIDArr(ID id, ID idOwner, int iCube, ID idArr)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefSetIDArr(id, idOwner, iCube, idArr);
}

SX_LIB_API ID SMtrl_RefGetIDArr(ID id, ID idOwner, int iCube)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlRefGetIDArr(id, idOwner, iCube);
}

SX_LIB_API int SMtrl_RefDelGetCount()
{
	ML_PRECOND(-1);
	return ArrMaterials->delRefCount();
}

SX_LIB_API ID SMtrl_RefDelGetIDArr(ID idKey, ID idOwner, int iCube)
{
	ML_PRECOND(-1);
	return ArrMaterials->delRefGetIDArr(idKey, idOwner, iCube);
}

SX_LIB_API void SMtrl_RefDelClear()
{
	ML_PRECOND(_VOID);
	ArrMaterials->delRefAllDel();
}

SX_LIB_API void SMtrl_RefSetMinMax(ID id, const float3_t *pMin, const float3_t *pMax)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefSetMinMax(id, pMin, pMax);
}

SX_LIB_API void SMtrl_RefPreRenderPlane(ID id, D3DXPLANE *pPlane)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefPreRenderPlane(id, pPlane);
}

SX_LIB_API const IFrustum* SMtrl_RefGetfrustum(ID id, int iCube)
{
	ML_PRECOND(0);
	return ArrMaterials->mtlRefGetfrustum(id, iCube);
}

SX_LIB_API bool SMtrl_RefIsAllowedRender(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlRefIsAllowedRender(id);
}

SX_LIB_API void SMtrl_RefPostRenderPlane(ID id)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefPostRenderPlane(id);
}

SX_LIB_API IDirect3DTexture9* SMtrl_RefGetTexPlane(ID id)
{
	ML_PRECOND(0);
	return ArrMaterials->mtlRefPlaneGetTex(id);
}



SX_LIB_API void SMtrl_RefCubeBeginRender(ID id, const float3_t *pCenter)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefCubeBeginRender(id, pCenter);
}

SX_LIB_API void SMtrl_RefCubePreRender(ID id, int iCube, const float4x4 *pWorld)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefCubePreRender(id, iCube, pWorld);
}

SX_LIB_API void SMtrl_RefCubePostRender(ID id, int iCube)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefCubePostRender(id, iCube);
}

SX_LIB_API void SMtrl_RefCubeEndRender(ID id, const float3_t *pViewPos)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefCubeEndRender(id, pViewPos);
}

SX_LIB_API bool SMtrl_RefUpdateCountUpdate(ID id, const float3_t *pViewPos)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlRefUpdateCountUpdate(id, pViewPos);
}

SX_LIB_API void SMtrl_RefNullingCountUpdate(ID id)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefNullingCountUpdate(id);
}

SX_LIB_API IDirect3DCubeTexture9* SMtrl_RefCubeGetTex(ID id)
{
	ML_PRECOND(0);
	return ArrMaterials->refCubeGetTex(id);
}

//##########################################################################

SX_LIB_API void SMtrl_MtlReloadMaterial(ID id, const char *szName)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlReLoad(id, szName);
}

SX_LIB_API void SMtrl_MtlClear(bool isClearRefDel)
{
	ML_PRECOND(_VOID);
	ArrMaterials->clear(isClearRefDel);
}

SX_LIB_API void SMtrl_MtlSetTexture(ID id, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetTexture(id, szTexture);
}

SX_LIB_API void SMtrl_MtlGetTexture(ID id, char *szName)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetTexture(id, szName);
}

SX_LIB_API ID SMtrl_MtlGetTextureID(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetTextureID(id);
}


SX_LIB_API void SMtrl_MtlSetUseDestColor(ID id, bool useDestColor)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUseDestColor(id, useDestColor);
}

SX_LIB_API bool SMtrl_MtlGetUseDestColor(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetUseDestColor(id);
}


SX_LIB_API void SMtrl_MtlSetVS(ID id, const char *szPath)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetVS(id, szPath);
}

SX_LIB_API void SMtrl_MtlGetVS(ID id, char *szName)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetVS(id, szName);
}

SX_LIB_API void SMtrl_MtlSetPS(ID id, const char *szPath)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetPS(id, szPath);
}

SX_LIB_API void SMtrl_MtlGetPS(ID id, char *szName)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetPS(id, szName);
}

SX_LIB_API float SMtrl_MtlGetDurability(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetDurability(id);
}

SX_LIB_API void SMtrl_MtlSetDurability(ID id, float fDurability)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetDurability(id, fDurability);
}

SX_LIB_API float SMtrl_MtlGetHitChance(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetHitChance(id);
}

SX_LIB_API void SMtrl_MtlSetHitChance(ID id, float fHitChance)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetHitChance(id, fHitChance);
}

SX_LIB_API float SMtrl_MtlGetDensity(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetDensity(id);
}

SX_LIB_API void SMtrl_MtlSetDensity(ID id, float fDensity)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetDensity(id, fDensity);
}

//##########################################################################

SX_LIB_API void SMtrl_MtlSetLighting(ID id, bool isLighting)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetLighting(id, isLighting);
}

SX_LIB_API bool SMtrl_MtlGetLighting(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetLighting(id);
}

SX_LIB_API void SMtrl_MtlSetIsTextureLighting(ID id, bool isTexLighting)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetIsTextureLighting(id, isTexLighting);
}

SX_LIB_API bool SMtrl_MtlGetIsTextureLighting(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetIsTextureLighting(id);
}

SX_LIB_API void SMtrl_MtlSetTextureLighting(ID id, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetTextureLighting(id, szTexture);
}

SX_LIB_API void SMtrl_MtlGetTextureLighting(ID id, char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetTextureLighting(id, szTexture);
}

SX_LIB_API void SMtrl_MtlSetRoughness(ID id, float fRoughness)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetRoughness(id, fRoughness);
}

SX_LIB_API float SMtrl_MtlGetRoughness(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetRoughness(id);
}

SX_LIB_API void SMtrl_MtlSetThickness(ID id, float fThickness)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetThickness(id, fThickness);
}

SX_LIB_API float SMtrl_MtlGetThickness(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetThickness(id);
}

SX_LIB_API void SMtrl_MtlSetF0(ID id, float fF0)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetF0(id, fF0);
}

SX_LIB_API float SMtrl_MtlGetF0(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetF0(id);
}

SX_LIB_API void SMtrl_MtlSetTransparency(ID id, bool isTransparent)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetTransparency(id, isTransparent);
}

SX_LIB_API bool SMtrl_MtlGetTransparency(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetTransparency(id);
}

SX_LIB_API void SMtrl_MtlSetTypeReflection(ID id, MTLTYPE_REFLECT type)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetTypeReflection(id, type);
}

SX_LIB_API MTLTYPE_REFLECT SMtrl_MtlGetTypeReflection(ID id)
{
	ML_PRECOND(MTLTYPE_REFLECT_NONE);
	return ArrMaterials->mtlGetTypeReflection(id);
}

//**************************************************************************

SX_LIB_API void SMtrl_MtlSetMaskTex(ID id, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetMaskTex(id, szTexture);
}

SX_LIB_API void SMtrl_MtlGetMaskTex(ID id, char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetMaskTex(id, szTexture);
}

SX_LIB_API void SMtrl_MtlSetMRTex(ID id, int iChannel, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetMRTex(id, iChannel, szTexture);
}

SX_LIB_API void SMtrl_MtlGetMRTex(ID id, int iChannel, char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetMRTex(id, iChannel, szTexture);
}

SX_LIB_API void SMtrl_MtlSetDTex(ID id, int iChannel, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetDTex(id, iChannel, szTexture);
}

SX_LIB_API void SMtrl_MtlGetDTex(ID id, int iChannel, char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetDTex(id, iChannel, szTexture);
}

SX_LIB_API void SMtrl_MtlSetStdVS(ID id, MTL_SHADERSTD type, bool isSend)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetStdVS(id, type, isSend);
}

SX_LIB_API bool SMtrl_MtlGetStdVS(ID id, MTL_SHADERSTD type)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetStdVS(id, type);
}

SX_LIB_API void SMtrl_MtlSetStdPS(ID id, MTL_SHADERSTD type, bool isSend)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetStdPS(id, type, isSend);
}

SX_LIB_API bool SMtrl_MtlGetStdPS(ID id, MTL_SHADERSTD type)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetStdPS(id, type);
}


SX_LIB_API void SMtrl_MtlSetUserDataVS(ID id, int iComponent, float fValue)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUserDataVS(id, iComponent, fValue);
}

SX_LIB_API float SMtrl_MtlGetUserDataVS(ID id, int iComponent)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetUserDataVS(id, iComponent);
}

SX_LIB_API void SMtrl_MtlSetUserDataVS_InPS(ID id, bool isSendPS)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUserDataVS_InPS(id, isSendPS);
}

SX_LIB_API bool SMtrl_MtlGetUserDataVS_InPS(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetUserDataVS_InPS(id);
}

SX_LIB_API void SMtrl_MtlSetUserDataPS(ID id, int iComponent, float fValue)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUserDataPS(id, iComponent, fValue);
}

SX_LIB_API float SMtrl_MtlGetUserDataPS(ID id, int iComponent)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetUserDataPS(id, iComponent);
}

SX_LIB_API void SMtrl_MtlSetUserDataPS_InVS(ID id, bool isSendVS)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUserDataPS_InPS(id, isSendVS);
}

SX_LIB_API bool SMtrl_MtlGetUserDataPS_InVS(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetUserDataPS_InPS(id);
}
