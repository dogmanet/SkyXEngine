
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

CLights* ArrLights = 0;
CMaterials* ArrMaterials = 0;

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

SX_LIB_API void SML_0Create(const char *szName, bool isUnic)
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
				MLSet::MLInit();
				MLSet::ReCalcSize();
				ArrLights = new CLights();
				ArrMaterials = new CMaterials();
			}
		}
		else
		{
			MLSet::MLInit();
			MLSet::ReCalcSize();
			ArrLights = new CLights();
			ArrMaterials = new CMaterials();
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
	ArrLights->onLostDevice();
	ArrMaterials->onLostDevice();
}

SX_LIB_API void SML_OnResetDevice()
{
	ML_PRECOND(_VOID);

	MLSet::ReCalcSize();
	ArrLights->onResetDevice();
	ArrMaterials->onResetDevice();

	MLSet::IDsRenderTargets::HowAdaptedLum = 0;
}

SX_LIB_API void SML_Update(DWORD timeDelta)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	if (ArrMaterials)
		ArrMaterials->update(timeDelta);

	MLSet::RefMProjPlane = SMMatrixPerspectiveFovLH(*r_default_fov, float(*r_win_width) / float(*r_win_height), MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
	MLSet::RefMProjCube = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
}

SX_LIB_API void SML_LigthsSettLCoefSizeDepth(float fCoef)
{
	ML_PRECOND(_VOID);

	MLSet::CoefSizeDepthMapForLocal = fCoef;
	MLSet::ReCalcSize();
	SML_OnLostDevice();
	SML_OnResetDevice();
}

SX_LIB_API void SML_LigthsSettGCoefSizeDepth(float fCoef)
{
	ML_PRECOND(_VOID);

	MLSet::CoefSizeDepthMapForGlobal = fCoef;
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

	ArrLights->clear();
}

SX_LIB_API void SML_LigthsClearIDArr()
{
	ML_PRECOND(_VOID);

	ArrLights->clearIDArr();
}

SX_LIB_API int SML_LigthsGetCount()
{
	ML_PRECOND(-1);

	return ArrLights->getCountLights();
}

SX_LIB_API bool SML_LigthsGetExists(ID id)
{
	ML_PRECOND(false);

	return ArrLights->getExists(id);
}


SX_LIB_API ID SML_LigthsCreatePoint(const float3 *pCenter, float fDist, const float3 *pColor, bool isGlobal, bool isShadowed/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->createPoint(-1, pCenter, fDist, pColor, isGlobal, isShadowed/*, 0*/);
}

SX_LIB_API ID SML_LigthsCreateDirection(const float3 *pPos, float fDist, const float3 *pColor, const SMQuaternion *qOrient, float fTopRadius, float fAngle, bool isShadow/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->createDirection(-1, pPos, fDist, pColor, qOrient, fTopRadius, fAngle, isShadow/*, 0*/);
}

SX_LIB_API void SML_LigthsRender(ID id, DWORD timeDelta)
{
	ML_PRECOND(_VOID);
	ArrLights->render(id, timeDelta);
}

SX_LIB_API ID SML_LigthsGetGlobal()
{
	ML_PRECOND(-1);
	return ArrLights->getLightGlobal();
}

SX_LIB_API bool SML_LigthsGetCastGlobalShadow()
{
	ML_PRECOND(false);
	return ArrLights->getCastGlobalShadow();
}

SX_LIB_API void SML_LigthsSetCastGlobalShadow(bool isShadowed)
{
	ML_PRECOND(_VOID);
	return ArrLights->setCastGlobalShadow(isShadowed);
}

SX_LIB_API void SML_LigthsGetColor(ID id, float3 *pColor)
{
	ML_PRECOND(_VOID);
	ArrLights->getLightColor(id, pColor);
}

SX_LIB_API void SML_LigthsSetColor(ID id, const float3 *pColor)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightColor(id, pColor);
}

SX_LIB_API void SML_LigthsGetPos(ID id, float3 *pPos, bool isGreal)
{
	ML_PRECOND(_VOID);
	ArrLights->getLightPos(id, pPos, isGreal);
}

SX_LIB_API float SML_LigthsGetPower(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getLightPower(id);
}

SX_LIB_API float SML_LigthsGetDist(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getLightDist(id);
}

SX_LIB_API void SML_LigthsSetPos(ID id, const float3 *pPos, bool isGreal)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightPos(id, pPos, isGreal);
}

SX_LIB_API void SML_LigthsGetOrient(ID id, SMQuaternion *pQ)
{
	ML_PRECOND(_VOID);
	ArrLights->getLightOrient(id, pQ);
}

SX_LIB_API void SML_LigthsSetOrient(ID id, const SMQuaternion *pQ)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightOrient(id, pQ);
}

SX_LIB_API LTYPE_LIGHT SML_LigthsGetType(ID id)
{
	ML_PRECOND(LTYPE_LIGHT_NONE);
	return ArrLights->getLightType(id);
}

SX_LIB_API bool SML_LigthsComVisibleForFrustum(ID id, const IFrustum *pFrustum)
{
	ML_PRECOND(false);
	return ArrLights->comVisibleForFrustum(id, pFrustum);
}

SX_LIB_API bool SML_LigthsGetVisibleForFrustum(ID id)
{
	ML_PRECOND(false);
	return ArrLights->getVisibleForFrustum(id);
}

SX_LIB_API float SML_LigthsComDistFor(ID id, const float3 *pPos)
{
	ML_PRECOND(-1);
	return ArrLights->comDistFor(id, pPos);
}

SX_LIB_API float SML_LigthsGetDistFor(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getDistFor(id);
}

SX_LIB_API void SML_LigthsComVisibleFrustumDistFor(const IFrustum *pFrustum, const float3 *pPos)
{
	ML_PRECOND(_VOID);
	return ArrLights->comVisibleFrustumDistFor(pFrustum, pPos);
}

SX_LIB_API bool SML_LigthsGetEnable(ID id)
{
	ML_PRECOND(false);
	return ArrLights->getLightEnable(id);
}

SX_LIB_API void SML_LigthsSetEnable(ID id, bool isEnable)
{
	ML_PRECOND(_VOID);
	return ArrLights->setLightEnable(id, isEnable);
}

SX_LIB_API bool SML_LigthsGetShadowed(ID id)
{
	ML_PRECOND(false);
	return ArrLights->getLightShadowed(id);
}

SX_LIB_API bool SML_LigthsGet4Or3SplitsG(ID id)
{
	ML_PRECOND(0);
	return ArrLights->get4Or3Splits(id);
}

SX_LIB_API void SML_LigthsSet4Or3SplitsG(ID id, bool is4or3)
{
	ML_PRECOND(_VOID);

	ArrLights->set4Or3Splits(id, is4or3);
}

SX_LIB_API IDirect3DTexture9* SML_LigthsGetShadow()
{
	ML_PRECOND(0);
	return ArrLights->getShadow2();
}

SX_LIB_API void SML_LigthsShadowNull()
{
	ML_PRECOND(_VOID);
	return ArrLights->shadowNull();
}

SX_LIB_API void SML_LigthsShadowRenderBegin(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowRenderBegin(id);
}

SX_LIB_API void SML_LigthsShadowRenderEnd(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowRenderEnd(id);
}

SX_LIB_API void SML_LigthsShadowRenderPre(ID id, int iCube)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowRenderPre(id, iCube);
}

SX_LIB_API void SML_LigthsShadowRenderPost(ID id, int iCube)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowRenderPost(id, iCube);
}

SX_LIB_API IFrustum* SML_LigthsGetFrustum(ID id, int iHow)
{
	ML_PRECOND(0);
	return ArrLights->getLightFrustum(id, iHow);
}

SX_LIB_API void SML_LigthsUpdateGFrustums(ID id, int iSplit, const float3 *pPos, const float3 *pDir)
{
	ML_PRECOND(_VOID);
	return ArrLights->updateLightGFrustums(id, iSplit, pPos, pDir);
}

SX_LIB_API void SML_LigthsShadowSetShaderOfTypeMat(ID id, int iTypeMaterial, const float4x4 *pWorld)
{
	ML_PRECOND(_VOID);
	ArrLights->initShaderOfTypeMaterial(id, iTypeMaterial, pWorld);
}

SX_LIB_API void SML_LigthsShadowGen(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowGen2(id);
}

SX_LIB_API void SML_LigthsShadowSoft(bool isRandom, float fSize, bool isFirst)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowSoft(isRandom, fSize, isFirst);
}



SX_LIB_API ID SML_LigthsCreateCopy(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->createCopy(id);
}

SX_LIB_API void SML_LigthsDeleteLight(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->deleteLight(id);
}

SX_LIB_API char* SML_LigthsGetName(ID id)
{
	ML_PRECOND(0);
	return ArrLights->getLightName(id);
}

SX_LIB_API void SML_LigthsSetName(ID id, const char *szName)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightName(id, szName);
}


SX_LIB_API void SML_LigthsSetAngle(ID id, float fAngle)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightAngle(id, fAngle, true);
}

SX_LIB_API void SML_LigthsSetTopRadius(ID id, float fTopRadius)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightTopRadius(id, fTopRadius);
}

SX_LIB_API void SML_LigthsSetDist(ID id, float fRadiusHeight, bool isCreate)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightDist(id, fRadiusHeight, isCreate);
}


SX_LIB_API float SML_LigthsGetShadowBias(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getShadowBias(id);
}

SX_LIB_API void SML_LigthsSetShadowBias(ID id, float fBias)
{
	ML_PRECOND(_VOID);
	return ArrLights->setShadowBias(id, fBias);
}

SX_LIB_API void SML_LigthsSetShadowBlurPixel(ID id, float fBlurPixel)
{
	ML_PRECOND(_VOID);
	ArrLights->setShadowBlurPixel(id, fBlurPixel);
}

SX_LIB_API float SML_LigthsGetShadowBlurPixel(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getShadowBlurPixel(id);
}
SX_LIB_API void SML_LigthsSetShadowLocalNear(ID id, float fNear)
{
	ML_PRECOND(_VOID);
	ArrLights->setShadowLocalNear(id, fNear);
}

SX_LIB_API float SML_LigthsGetShadowLocalNear(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getShadowLocalNear(id);
}

SX_LIB_API void SML_LigthsSetShadowLocalFar(ID id, float fFar)
{
	ML_PRECOND(_VOID);
	ArrLights->setShadowLocalFar(id, fFar);
}

SX_LIB_API float SML_LigthsGetShadowLocalFar(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getShadowLocalFar(id);
}

SX_LIB_API void SML_LigthsSetCubeEdgeEnable(ID id, int iEdge, bool isEnable)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightCubeEdgeEnable(id, iEdge, isEnable);
}

SX_LIB_API bool SML_LigthsGetCubeEdgeEnable(ID id, int iEdge)
{
	ML_PRECOND(false);
	return ArrLights->getLightCubeEdgeEnable(id, iEdge);
}


SX_LIB_API float SML_LigthsGetAngle(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getLightAngle(id);
}

SX_LIB_API float SML_LigthsGetTopRadius(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getLightTopRadius(id);
}

SX_LIB_API ID SML_LigthsGetIDArr(ID id, ID idOwner, int iHow)
{
	ML_PRECOND(-1);
	return ArrLights->getLightIDArr(id, idOwner, iHow);
}

SX_LIB_API void SML_LigthsSetIDArr(ID id, ID idOwner, int iHow, ID idArr)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightIDArr(id, idOwner, iHow, idArr);
}

SX_LIB_API void SML_LigthsSetTypeShadowed(ID id, LTYPE_SHADOW type)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightTypeShadowed(id, type);
}

SX_LIB_API LTYPE_SHADOW SML_LigthsGetTypeShadowed(ID id)
{
	ML_PRECOND(LTYPE_SHADOW_NONE);
	return ArrLights->getLightTypeShadowed(id);
}

SX_LIB_API bool SML_LigthsCountUpdateUpdate(ID id, const float3 *pViewPos, int iHow)
{
	ML_PRECOND(false);
	return ArrLights->lightCountUpdateUpdate(id, pViewPos, iHow);
}

SX_LIB_API bool SML_LigthsCountUpdateAllowed(ID id, int iHow)
{
	ML_PRECOND(false);
	return ArrLights->lightCountUpdateAllowed(id, iHow);
}

SX_LIB_API void SML_LigthsCountUpdateNull(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->lightCountUpdateNull(id);
}

SX_LIB_API void SML_LigthsToneMappingCom(DWORD timeDelta, float fAdapted)
{
	ML_PRECOND(_VOID);
	ArrLights->toneMappingCom(timeDelta, fAdapted);
}

SX_LIB_API int SML_LigthsDelGetCount()
{
	ML_PRECOND(-1);
	return ArrLights->delGetCount();
}

SX_LIB_API LTYPE_LIGHT SML_LigthsDelGetType(ID idKey)
{
	ML_PRECOND(LTYPE_LIGHT_NONE);
	return ArrLights->delGetType(idKey);
}

SX_LIB_API void SML_LigthsDelDel(ID idKey)
{
	ML_PRECOND(_VOID);
	ArrLights->delDel(idKey);
}

SX_LIB_API ID SML_LigthsDelGetIDArr(ID idKey, ID idOwner, int iHow)
{
	ML_PRECOND(-1);
	return ArrLights->delGetIDArr(idKey, idOwner, iHow);
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

SX_LIB_API ID SML_MtlLoad(const char *szName, MTLTYPE_MODEL mtl_type)
{
	ML_PRECOND(-1);

	return ArrMaterials->mtlLoad(szName, mtl_type);
}

SX_LIB_API void SML_MtlSave(ID id)
{
	ML_PRECOND(_VOID);

	ArrMaterials->mtlSave(id);
}

SX_LIB_API MTLTYPE_MODEL SML_MtlGetTypeModel(ID id)
{
	ML_PRECOND(MTLTYPE_MODEL_STATIC);

	return ArrMaterials->getTypeModel(id);
}

SX_LIB_API UINT SML_MtlGetSort(ID id)
{
	ML_PRECOND(0);

	return ArrMaterials->mtlGetSort(id);
}

SX_LIB_API void SML_MtlSetTypeModel(ID id, MTLTYPE_MODEL type_model)
{
	ML_PRECOND(_VOID);

	ArrMaterials->setTypeModel(id, type_model);
}

SX_LIB_API long SML_MtlGetCount()
{
	ML_PRECOND(-1);

	return ArrMaterials->getCount();
}

SX_LIB_API void SML_MtlRender(ID id, const float4x4 *pWorld)
{
	ML_PRECOND(_VOID);
	ArrMaterials->render(id, pWorld);
}

SX_LIB_API void SML_MtlRenderStd(MTLTYPE_MODEL type, const float4x4 *pWorld, ID idSlot, ID idMtl)
{
	ML_PRECOND(_VOID);
	ArrMaterials->renderStd(type, pWorld, idSlot, idMtl);
}

SX_LIB_API void SML_MtlRenderLight(const float4_t *pColor, const float4x4 *pWorld)
{
	ML_PRECOND(_VOID);
	ArrMaterials->renderLight(pColor, pWorld);
}

SX_LIB_API void SML_MtlSetMainTexture(ID slot, ID id)
{
	ML_PRECOND(_VOID);
	ArrMaterials->setMainTexture(slot, id);
}

SX_LIB_API void SML_MtlSetIsIncrCountSurf(bool bf)
{
	ML_PRECOND(_VOID);
	ArrMaterials->setIsIncrCountSurf(bf);
}

SX_LIB_API bool SML_MtlGetIsIncrCountSurf()
{
	ML_PRECOND(false);
	return ArrMaterials->getIsIncrCountSurf();
}

SX_LIB_API void SML_MtlNullingCurrCountSurf()
{
	ML_PRECOND(_VOID);
	ArrMaterials->nullingCurrCountSurf();
}

SX_LIB_API int SML_MtlGetCurrCountSurf()
{
	ML_PRECOND(-1);
	return ArrMaterials->getCurrCountSurf();
}

SX_LIB_API void SML_MtlSetCurrCountSurf(int iCount)
{
	ML_PRECOND(_VOID);
	ArrMaterials->setCurrCountSurf(iCount);
}

SX_LIB_API void SML_MtlSetForceblyAlphaTest(bool isAlphaTest)
{
	ML_PRECOND(_VOID);
	ArrMaterials->setForceblyAlphaTest(isAlphaTest);
}

SX_LIB_API bool SML_MtlGetForceblyAlphaTest()
{
	ML_PRECOND(false);
	return ArrMaterials->getForceblyAlphaTest();
}


SX_LIB_API ID SML_MtlGetStdMtl(MTLTYPE_MODEL type_model)
{
	ML_PRECOND(-1);
	return ArrMaterials->getStdMtl(type_model);
}


SX_LIB_API void SML_MtlSetPhysicMaterial(ID id, MTLTYPE_PHYSIC type)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetPhysicMaterial(id, type);
}

SX_LIB_API MTLTYPE_PHYSIC SML_MtlGetPhysicMaterial(ID id)
{
	ML_PRECOND(MTLTYPE_PHYSIC_DEFAULT);
	return ArrMaterials->mtlGetPhysicMaterial(id);
}

SX_LIB_API bool SML_MtlGetUsingAlphaTest(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetUsingAlphaTest(id);
}

SX_LIB_API void SML_MtlSetUsingAlphaTest(ID id, bool isUsing)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUsingAlphaTest(id, isUsing);
}



SX_LIB_API void SML_MtlRefSetIDArr(ID id, ID idOwner, int iCube, ID idArr)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefSetIDArr(id, idOwner, iCube, idArr);
}

SX_LIB_API ID SML_MtlRefGetIDArr(ID id, ID idOwner, int iCube)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlRefGetIDArr(id, idOwner, iCube);
}

SX_LIB_API int SML_MtlDelRefGetCount()
{
	ML_PRECOND(-1);
	return ArrMaterials->delRefCount();
}

SX_LIB_API ID SML_MtlDelRefGetIDArr(ID idKey, ID idOwner, int iCube)
{
	ML_PRECOND(-1);
	return ArrMaterials->delRefGetIDArr(idKey, idOwner, iCube);
}

SX_LIB_API void SML_MtlDelRefClear()
{
	ML_PRECOND(_VOID);
	ArrMaterials->delRefAllDel();
}

SX_LIB_API void SML_MtlRefSetMinMax(ID id, const float3_t *pMin, const float3_t *pMax)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefSetMinMax(id, pMin, pMax);
}

SX_LIB_API void SML_MtlRefPreRenderPlane(ID id, D3DXPLANE *pPlane)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefPreRenderPlane(id, pPlane);
}

SX_LIB_API const IFrustum* SML_MtlRefGetfrustum(ID id, int iCube)
{
	ML_PRECOND(0);
	return ArrMaterials->mtlRefGetfrustum(id, iCube);
}

SX_LIB_API bool SML_MtlRefIsAllowedRender(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlRefIsAllowedRender(id);
}

SX_LIB_API void SML_MtlRefPostRenderPlane(ID id)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefPostRenderPlane(id);
}

SX_LIB_API IDirect3DTexture9* SML_MtlRefGetTexPlane(ID id)
{
	ML_PRECOND(0);
	return ArrMaterials->mtlRefPlaneGetTex(id);
}



SX_LIB_API void SML_MtlRefCubeBeginRender(ID id, const float3_t *pCenter)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefCubeBeginRender(id, pCenter);
}

SX_LIB_API void SML_MtlRefCubePreRender(ID id, int iCube, const float4x4 *pWorld)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefCubePreRender(id, iCube, pWorld);
}

SX_LIB_API void SML_MtlRefCubePostRender(ID id, int iCube)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefCubePostRender(id, iCube);
}

SX_LIB_API void SML_MtlRefCubeEndRender(ID id, const float3_t *pViewPos)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefCubeEndRender(id, pViewPos);
}

SX_LIB_API bool SML_MtlRefUpdateCountUpdate(ID id, const float3_t *pViewPos)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlRefUpdateCountUpdate(id, pViewPos);
}

SX_LIB_API void SML_MtlRefNullingCountUpdate(ID id)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlRefNullingCountUpdate(id);
}

SX_LIB_API IDirect3DCubeTexture9* SML_MtlRefCubeGetTex(ID id)
{
	ML_PRECOND(0);
	return ArrMaterials->refCubeGetTex(id);
}

//##########################################################################

SX_LIB_API void SML_MtlReloadMaterial(ID id, const char *szName)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlReLoad(id, szName);
}

SX_LIB_API void SML_MtlClear(bool isClearRefDel)
{
	ML_PRECOND(_VOID);
	ArrMaterials->clear(isClearRefDel);
}

SX_LIB_API void SML_MtlSetTexture(ID id, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetTexture(id, szTexture);
}

SX_LIB_API void SML_MtlGetTexture(ID id, char *szName)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetTexture(id, szName);
}

SX_LIB_API ID SML_MtlGetTextureID(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetTextureID(id);
}

SX_LIB_API void SML_MtlSetVS(ID id, const char *szPath)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetVS(id, szPath);
}

SX_LIB_API void SML_MtlGetVS(ID id, char *szName)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetVS(id, szName);
}

SX_LIB_API void SML_MtlSetPS(ID id, const char *szPath)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetPS(id, szPath);
}

SX_LIB_API void SML_MtlGetPS(ID id, char *szName)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetPS(id, szName);
}

SX_LIB_API float SML_MtlGetDurability(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetDurability(id);
}

SX_LIB_API void SML_MtlSetDurability(ID id, float fDurability)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetDurability(id, fDurability);
}

SX_LIB_API float SML_MtlGetHitChance(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetHitChance(id);
}

SX_LIB_API void SML_MtlSetHitChance(ID id, float fHitChance)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetHitChance(id, fHitChance);
}

SX_LIB_API float SML_MtlGetDensity(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetDensity(id);
}

SX_LIB_API void SML_MtlSetDensity(ID id, float fDensity)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetDensity(id, fDensity);
}

//##########################################################################

SX_LIB_API void SML_MtlSetLighting(ID id, bool isLighting)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetLighting(id, isLighting);
}

SX_LIB_API bool SML_MtlGetLighting(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetLighting(id);
}

SX_LIB_API void SML_MtlSetIsTextureLighting(ID id, bool isTexLighting)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetIsTextureLighting(id, isTexLighting);
}

SX_LIB_API bool SML_MtlGetIsTextureLighting(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetIsTextureLighting(id);
}

SX_LIB_API void SML_MtlSetTextureLighting(ID id, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetTextureLighting(id, szTexture);
}

SX_LIB_API void SML_MtlGetTextureLighting(ID id, char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetTextureLighting(id, szTexture);
}

SX_LIB_API void SML_MtlSetRoughness(ID id, float fRoughness)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetRoughness(id, fRoughness);
}

SX_LIB_API float SML_MtlGetRoughness(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetRoughness(id);
}

SX_LIB_API void SML_MtlSetThickness(ID id, float fThickness)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetThickness(id, fThickness);
}

SX_LIB_API float SML_MtlGetThickness(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetThickness(id);
}

SX_LIB_API void SML_MtlSetF0(ID id, float fF0)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetF0(id, fF0);
}

SX_LIB_API float SML_MtlGetF0(ID id)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetF0(id);
}

SX_LIB_API void SML_MtlSetTransparency(ID id, bool isTransparent)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetTransparency(id, isTransparent);
}

SX_LIB_API bool SML_MtlGetTransparency(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetTransparency(id);
}

SX_LIB_API void SML_MtlSetTypeReflection(ID id, MTLTYPE_REFLECT type)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetTypeReflection(id, type);
}

SX_LIB_API MTLTYPE_REFLECT SML_MtlGetTypeReflection(ID id)
{
	ML_PRECOND(MTLTYPE_REFLECT_NONE);
	return ArrMaterials->mtlGetTypeReflection(id);
}

//**************************************************************************

SX_LIB_API void SML_MtlSetMaskTex(ID id, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetMaskTex(id, szTexture);
}

SX_LIB_API void SML_MtlGetMaskTex(ID id, char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetMaskTex(id, szTexture);
}

SX_LIB_API void SML_MtlSetMRTex(ID id, int iChannel, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetMRTex(id, iChannel, szTexture);
}

SX_LIB_API void SML_MtlGetMRTex(ID id, int iChannel, char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetMRTex(id, iChannel, szTexture);
}

SX_LIB_API void SML_MtlSetDTex(ID id, int iChannel, const char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetDTex(id, iChannel, szTexture);
}

SX_LIB_API void SML_MtlGetDTex(ID id, int iChannel, char *szTexture)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlGetDTex(id, iChannel, szTexture);
}

SX_LIB_API void SML_MtlSetStdVS(ID id, MTL_SHADERSTD type, bool isSend)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetStdVS(id, type, isSend);
}

SX_LIB_API bool SML_MtlGetStdVS(ID id, MTL_SHADERSTD type)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetStdVS(id, type);
}

SX_LIB_API void SML_MtlSetStdPS(ID id, MTL_SHADERSTD type, bool isSend)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetStdPS(id, type, isSend);
}

SX_LIB_API bool SML_MtlGetStdPS(ID id, MTL_SHADERSTD type)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetStdPS(id, type);
}


SX_LIB_API void SML_MtlSetUserDataVS(ID id, int iComponent, float fValue)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUserDataVS(id, iComponent, fValue);
}

SX_LIB_API float SML_MtlGetUserDataVS(ID id, int iComponent)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetUserDataVS(id, iComponent);
}

SX_LIB_API void SML_MtlSetUserDataVS_InPS(ID id, bool isSendPS)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUserDataVS_InPS(id, isSendPS);
}

SX_LIB_API bool SML_MtlGetUserDataVS_InPS(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetUserDataVS_InPS(id);
}

SX_LIB_API void SML_MtlSetUserDataPS(ID id, int iComponent, float fValue)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUserDataPS(id, iComponent, fValue);
}

SX_LIB_API float SML_MtlGetUserDataPS(ID id, int iComponent)
{
	ML_PRECOND(-1);
	return ArrMaterials->mtlGetUserDataPS(id, iComponent);
}

SX_LIB_API void SML_MtlSetUserDataPS_InVS(ID id, bool isSendVS)
{
	ML_PRECOND(_VOID);
	ArrMaterials->mtlSetUserDataPS_InPS(id, isSendVS);
}

SX_LIB_API bool SML_MtlGetUserDataPS_InVS(ID id)
{
	ML_PRECOND(false);
	return ArrMaterials->mtlGetUserDataPS_InPS(id);
}
