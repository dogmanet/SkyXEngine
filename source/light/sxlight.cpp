
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXMATERIAL_LIGTH_VERSION 1

#include "sxlight.h"
#include "ml_data.h"

#include "light.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

CLights* ArrLights = 0;

#define ML_PRECOND(retval) if(!ArrLights){LibReport(-1, "%s - sxmtlligth is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SLight_0GetVersion()
{
	return SXMATERIAL_LIGTH_VERSION;
}

SX_LIB_API void SLight_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SLight_0Create(const char *szName, bool isUnic)
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
				light_data::Init();
				light_data::ReCalcSize();
				ArrLights = new CLights();
			}
		}
		else
		{
			light_data::Init();
			light_data::ReCalcSize();
			ArrLights = new CLights();
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SLight_AKill()
{
	mem_delete(ArrLights)
}

SX_LIB_API void SLight_OnLostDevice()
{
	ML_PRECOND(_VOID);
	ArrLights->onLostDevice();
}

SX_LIB_API void SLight_OnResetDevice()
{
	ML_PRECOND(_VOID);

	light_data::ReCalcSize();
	ArrLights->onResetDevice();
}



SX_LIB_API void SLight_SettLCoefSizeDepth(float fCoef)
{
	ML_PRECOND(_VOID);

	light_data::fCoefSizeDepthMapForLocal = fCoef;
	light_data::ReCalcSize();
	SLight_OnLostDevice();
	SLight_OnResetDevice();
}

SX_LIB_API void SLight_SettGCoefSizeDepth(float fCoef)
{
	ML_PRECOND(_VOID);

	light_data::fCoefSizeDepthMapForGlobal = fCoef;
	light_data::ReCalcSize();
	SLight_OnLostDevice();
	SLight_OnResetDevice();
	
}

SX_LIB_API float SLight_GettLCoefSizeDepth()
{
	ML_PRECOND(-1);

	return light_data::fCoefSizeDepthMapForLocal;
}

SX_LIB_API float SLight_GettGCoefSizeDepth()
{
	ML_PRECOND(-1);
	return light_data::fCoefSizeDepthMapForGlobal;
}

//#############################################################################

SX_LIB_API void SLight_Clear()
{
	ML_PRECOND(_VOID);

	ArrLights->clear();
}

SX_LIB_API void SLight_ClearIDArr()
{
	ML_PRECOND(_VOID);

	ArrLights->clearIDArr();
}

SX_LIB_API int SLight_GetCount()
{
	ML_PRECOND(-1);

	return ArrLights->getCountLights();
}

SX_LIB_API bool SLight_GetExists(ID id)
{
	ML_PRECOND(false);

	return ArrLights->getExists(id);
}


SX_LIB_API ID SLight_CreatePoint(const float3 *pCenter, float fDist, const float3 *pColor, bool isGlobal, bool isShadowed/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->createPoint(-1, pCenter, fDist, pColor, isGlobal, isShadowed/*, 0*/);
}

SX_LIB_API ID SLight_CreateDirection(const float3 *pPos, float fDist, const float3 *pColor, const SMQuaternion *qOrient, float fTopRadius, float fAngle, bool isShadow/*, const char* bound_volume*/)
{
	ML_PRECOND(-1);
	return ArrLights->createDirection(-1, pPos, fDist, pColor, qOrient, fTopRadius, fAngle, isShadow/*, 0*/);
}

SX_LIB_API void SLight_Render(ID id, DWORD timeDelta)
{
	ML_PRECOND(_VOID);
	ArrLights->render(id, timeDelta);
}

SX_LIB_API ID SLight_GetGlobal()
{
	ML_PRECOND(-1);
	return ArrLights->getLightGlobal();
}

SX_LIB_API bool SLight_GetCastGlobalShadow()
{
	ML_PRECOND(false);
	return ArrLights->getCastGlobalShadow();
}

SX_LIB_API void SLight_SetCastGlobalShadow(bool isShadowed)
{
	ML_PRECOND(_VOID);
	return ArrLights->setCastGlobalShadow(isShadowed);
}

SX_LIB_API void SLight_GetColor(ID id, float3 *pColor)
{
	ML_PRECOND(_VOID);
	ArrLights->getLightColor(id, pColor);
}

SX_LIB_API void SLight_SetColor(ID id, const float3 *pColor)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightColor(id, pColor);
}

SX_LIB_API void SLight_GetPos(ID id, float3 *pPos, bool isGreal)
{
	ML_PRECOND(_VOID);
	ArrLights->getLightPos(id, pPos, isGreal);
}

SX_LIB_API float SLight_GetPower(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getLightPower(id);
}

SX_LIB_API float SLight_GetDist(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getLightDist(id);
}

SX_LIB_API void SLight_SetPos(ID id, const float3 *pPos, bool isGreal)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightPos(id, pPos, isGreal);
}

SX_LIB_API void SLight_GetOrient(ID id, SMQuaternion *pQ)
{
	ML_PRECOND(_VOID);
	ArrLights->getLightOrient(id, pQ);
}

SX_LIB_API void SLight_SetOrient(ID id, const SMQuaternion *pQ)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightOrient(id, pQ);
}

SX_LIB_API LTYPE_LIGHT SLight_GetType(ID id)
{
	ML_PRECOND(LTYPE_LIGHT_NONE);
	return ArrLights->getLightType(id);
}

SX_LIB_API bool SLight_ComVisibleForFrustum(ID id, const IFrustum *pFrustum)
{
	ML_PRECOND(false);
	return ArrLights->comVisibleForFrustum(id, pFrustum);
}

SX_LIB_API bool SLight_GetVisibleForFrustum(ID id)
{
	ML_PRECOND(false);
	return ArrLights->getVisibleForFrustum(id);
}

SX_LIB_API float SLight_ComDistFor(ID id, const float3 *pPos)
{
	ML_PRECOND(-1);
	return ArrLights->comDistFor(id, pPos);
}

SX_LIB_API float SLight_GetDistFor(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getDistFor(id);
}

SX_LIB_API void SLight_ComVisibleFrustumDistFor(const IFrustum *pFrustum, const float3 *pPos)
{
	ML_PRECOND(_VOID);
	return ArrLights->comVisibleFrustumDistFor(pFrustum, pPos);
}

SX_LIB_API bool SLight_GetEnable(ID id)
{
	ML_PRECOND(false);
	return ArrLights->getLightEnable(id);
}

SX_LIB_API void SLight_SetEnable(ID id, bool isEnable)
{
	ML_PRECOND(_VOID);
	return ArrLights->setLightEnable(id, isEnable);
}

SX_LIB_API bool SLight_GetShadowed(ID id)
{
	ML_PRECOND(false);
	return ArrLights->getLightShadowed(id);
}

SX_LIB_API bool SLight_Get4Or3SplitsG(ID id)
{
	ML_PRECOND(0);
	return ArrLights->get4Or3Splits(id);
}

SX_LIB_API void SLight_Set4Or3SplitsG(ID id, bool is4or3)
{
	ML_PRECOND(_VOID);

	ArrLights->set4Or3Splits(id, is4or3);
}

SX_LIB_API IDirect3DTexture9* SLight_GetShadow()
{
	ML_PRECOND(0);
	return ArrLights->getShadow2();
}

SX_LIB_API void SLight_ShadowNull()
{
	ML_PRECOND(_VOID);
	return ArrLights->shadowNull();
}

SX_LIB_API void SLight_ShadowRenderBegin(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowRenderBegin(id);
}

SX_LIB_API void SLight_ShadowRenderEnd(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowRenderEnd(id);
}

SX_LIB_API void SLight_ShadowRenderPre(ID id, int iCube)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowRenderPre(id, iCube);
}

SX_LIB_API void SLight_ShadowRenderPost(ID id, int iCube)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowRenderPost(id, iCube);
}

SX_LIB_API IFrustum* SLight_GetFrustum(ID id, int iHow)
{
	ML_PRECOND(0);
	return ArrLights->getLightFrustum(id, iHow);
}

SX_LIB_API void SLight_UpdateGFrustums(ID id, int iSplit, const float3 *pPos, const float3 *pDir)
{
	ML_PRECOND(_VOID);
	return ArrLights->updateLightGFrustums(id, iSplit, pPos, pDir);
}

SX_LIB_API void SLight_ShadowSetShaderOfTypeMat(ID id, int iTypeMaterial, const float4x4 *pWorld)
{
	ML_PRECOND(_VOID);
	ArrLights->initShaderOfTypeMaterial(id, iTypeMaterial, pWorld);
}

SX_LIB_API void SLight_ShadowGen(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowGen2(id);
}

SX_LIB_API void SLight_ShadowSoft(bool isRandom, float fSize, bool isFirst)
{
	ML_PRECOND(_VOID);
	ArrLights->shadowSoft(isRandom, fSize, isFirst);
}



SX_LIB_API ID SLight_CreateCopy(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->createCopy(id);
}

SX_LIB_API void SLight_DeleteLight(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->deleteLight(id);
}

SX_LIB_API char* SLight_GetName(ID id)
{
	ML_PRECOND(0);
	return ArrLights->getLightName(id);
}

SX_LIB_API void SLight_SetName(ID id, const char *szName)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightName(id, szName);
}


SX_LIB_API void SLight_SetAngle(ID id, float fAngle)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightAngle(id, fAngle, true);
}

SX_LIB_API void SLight_SetTopRadius(ID id, float fTopRadius)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightTopRadius(id, fTopRadius);
}

SX_LIB_API void SLight_SetDist(ID id, float fRadiusHeight, bool isCreate)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightDist(id, fRadiusHeight, isCreate);
}


SX_LIB_API float SLight_GetShadowBias(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getShadowBias(id);
}

SX_LIB_API void SLight_SetShadowBias(ID id, float fBias)
{
	ML_PRECOND(_VOID);
	return ArrLights->setShadowBias(id, fBias);
}

SX_LIB_API void SLight_SetShadowBlurPixel(ID id, float fBlurPixel)
{
	ML_PRECOND(_VOID);
	ArrLights->setShadowBlurPixel(id, fBlurPixel);
}

SX_LIB_API float SLight_GetShadowBlurPixel(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getShadowBlurPixel(id);
}
SX_LIB_API void SLight_SetShadowLocalNear(ID id, float fNear)
{
	ML_PRECOND(_VOID);
	ArrLights->setShadowLocalNear(id, fNear);
}

SX_LIB_API float SLight_GetShadowLocalNear(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getShadowLocalNear(id);
}

SX_LIB_API void SLight_SetShadowLocalFar(ID id, float fFar)
{
	ML_PRECOND(_VOID);
	ArrLights->setShadowLocalFar(id, fFar);
}

SX_LIB_API float SLight_GetShadowLocalFar(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getShadowLocalFar(id);
}

SX_LIB_API void SLight_SetCubeEdgeEnable(ID id, int iEdge, bool isEnable)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightCubeEdgeEnable(id, iEdge, isEnable);
}

SX_LIB_API bool SLight_GetCubeEdgeEnable(ID id, int iEdge)
{
	ML_PRECOND(false);
	return ArrLights->getLightCubeEdgeEnable(id, iEdge);
}


SX_LIB_API float SLight_GetAngle(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getLightAngle(id);
}

SX_LIB_API float SLight_GetTopRadius(ID id)
{
	ML_PRECOND(-1);
	return ArrLights->getLightTopRadius(id);
}

SX_LIB_API ID SLight_GetIDArr(ID id, ID idOwner, int iHow)
{
	ML_PRECOND(-1);
	return ArrLights->getLightIDArr(id, idOwner, iHow);
}

SX_LIB_API void SLight_SetIDArr(ID id, ID idOwner, int iHow, ID idArr)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightIDArr(id, idOwner, iHow, idArr);
}

SX_LIB_API void SLight_SetTypeShadowed(ID id, LTYPE_SHADOW type)
{
	ML_PRECOND(_VOID);
	ArrLights->setLightTypeShadowed(id, type);
}

SX_LIB_API LTYPE_SHADOW SLight_GetTypeShadowed(ID id)
{
	ML_PRECOND(LTYPE_SHADOW_NONE);
	return ArrLights->getLightTypeShadowed(id);
}

SX_LIB_API bool SLight_CountUpdateUpdate(ID id, const float3 *pViewPos, int iHow)
{
	ML_PRECOND(false);
	return ArrLights->lightCountUpdateUpdate(id, pViewPos, iHow);
}

SX_LIB_API bool SLight_CountUpdateAllowed(ID id, int iHow)
{
	ML_PRECOND(false);
	return ArrLights->lightCountUpdateAllowed(id, iHow);
}

SX_LIB_API void SLight_CountUpdateNull(ID id)
{
	ML_PRECOND(_VOID);
	ArrLights->lightCountUpdateNull(id);
}

/*SX_LIB_API void SLight_ToneMappingCom(DWORD timeDelta, float fAdapted)
{
	ML_PRECOND(_VOID);
	ArrLights->toneMappingCom(timeDelta, fAdapted);
}*/

SX_LIB_API int SLight_DelGetCount()
{
	ML_PRECOND(-1);
	return ArrLights->delGetCount();
}

SX_LIB_API LTYPE_LIGHT SLight_DelGetType(ID idKey)
{
	ML_PRECOND(LTYPE_LIGHT_NONE);
	return ArrLights->delGetType(idKey);
}

SX_LIB_API void SLight_DelDel(ID idKey)
{
	ML_PRECOND(_VOID);
	ArrLights->delDel(idKey);
}

SX_LIB_API ID SLight_DelGetIDArr(ID idKey, ID idOwner, int iHow)
{
	ML_PRECOND(-1);
	return ArrLights->delGetIDArr(idKey, idOwner, iHow);
}
