
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXGEOM_VERSION 1

#include "sxgeom.h"
#include "models.h"

//##########################################################################

IGXContext *g_pDXDevice = 0;

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

CModels *g_pModels = 0;

#define GEOM_PRECOND(retval) if(!(g_pModels)){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxgeom is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SGeom_0GetVersion()
{
	return SXGEOM_VERSION;
}

SX_LIB_API void SGeom_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SGeom_0Create(const char *szName, bool isUnic, bool isServerMode)
{
	if(!isServerMode)
	{
		g_pDXDevice = SGCore_GetDXDevice();
	}

	if (szName && strlen(szName) > 1)
	{
		if (isUnic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, szName);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
				return;
			}
		}
		g_pModels = new CModels(isServerMode);
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SGeom_AKill()
{
	mem_delete(g_pModels);
}

SX_LIB_API void SGeom_OnLostDevice()
{
	GEOM_PRECOND(_VOID);

	g_pModels->onLostDevice();
}

SX_LIB_API void SGeom_OnResetDevice()
{
	GEOM_PRECOND(_VOID);

	g_pModels->onResetDevice();
}

//##########################################################################

SX_LIB_API void SGeom_Clear()
{
	GEOM_PRECOND(_VOID);
	g_pModels->clear();
}

SX_LIB_API void SGeom_Save(const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pModels->save(szPath);
}

SX_LIB_API void SGeom_Load(const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pModels->load(szPath);
}

SX_LIB_API int SGeom_GetCountModels()
{
	GEOM_PRECOND(-1);
	return g_pModels->getCountModel();
}

SX_LIB_API void SGeom_ComVisible(const IFrustum *pFrustum, const float3 *pViewPos, ID idVisCalcObj)
{
	GEOM_PRECOND(_VOID);
	g_pModels->comVisible(pFrustum, pViewPos, idVisCalcObj);
}

SX_LIB_API bool SGeom_TransparencyExistsForRender(ID idVisCalcObj)
{
	GEOM_PRECOND(false);
	return g_pModels->existsTransparency4Render(idVisCalcObj);
}

SX_LIB_API void SGeom_RenderSingly(DWORD timeDelta, ID idModel, ID idMtrl, const float3 *pPos)
{
	GEOM_PRECOND(_VOID);

	g_pModels->renderSingly(timeDelta, idModel, idMtrl, pPos);
}

SX_LIB_API void SGeom_Render(DWORD timeDelta, GEOM_RENDER_TYPE type, ID idVisCalcObj, ID idExcludeModel, ID idExcludeGroup)
{
	GEOM_PRECOND(_VOID);
	g_pModels->render(timeDelta, type, idVisCalcObj);
}

SX_LIB_API ID SGeom_ModelAdd(const char *szPath, const char *szName, const char *szLod, const char *szPhys, bool needSegmentation)
{
	GEOM_PRECOND(-1);
	return g_pModels->addModel(szPath, szName, szLod, szPhys, needSegmentation);
}

SX_LIB_API ID SGeom_ModelCopy(ID idModel)
{
	GEOM_PRECOND(-1);
	return g_pModels->copy(idModel);
}

SX_LIB_API void SGeom_ModelDelete(ID idModel)
{
	GEOM_PRECOND(_VOID);
	g_pModels->deleteModel(idModel);
}

SX_LIB_API void SGeom_GetMinMax(float3 *pMin, float3 *pMax)
{
	GEOM_PRECOND(_VOID);
	g_pModels->getMinMax(pMin, pMax);
}

//**************************************************************************

SX_LIB_API ID SGeom_VisCaclObjAdd()
{
	GEOM_PRECOND(-1);
	return g_pModels->addVisCaclObj();
}

SX_LIB_API bool SGeom_VisCaclObjExists(ID idVisCaclObj)
{
	GEOM_PRECOND(false);
	return g_pModels->existsVisCaclObj(idVisCaclObj);
}

SX_LIB_API void SGeom_VisCaclObjDelete(ID idVisCaclObj)
{
	GEOM_PRECOND(_VOID);
	g_pModels->deleteVisCaclObj(idVisCaclObj);
}

//**************************************************************************

SX_LIB_API void SGeom_ModelGetMinMax(ID idModel, float3 *pMin, float3 *pMax)
{
	GEOM_PRECOND(_VOID);
	g_pModels->modelGetMinMax(idModel, pMin, pMax);
}

SX_LIB_API const char* SGeom_ModelGetName(ID idModel, char *szName)
{
	GEOM_PRECOND(0);
	return g_pModels->modelGetName(idModel, szName);
}

SX_LIB_API void SGeom_ModelSetName(ID idModel, const char *szName)
{
	GEOM_PRECOND(_VOID);
	g_pModels->modelSetName(idModel, szName);
}

SX_LIB_API const char* SGeom_ModelGetPath4Model(ID idModel)
{
	GEOM_PRECOND(0);
	return g_pModels->modelGetPath4Model(idModel);
}

SX_LIB_API int SGeom_ModelGetCountPoly(ID idModel)
{
	GEOM_PRECOND(-1);
	return g_pModels->modelGetCountPoly(idModel);
}

SX_LIB_API const float3* SGeom_ModelGetPosition(ID idModel, float3 *pPos)
{
	GEOM_PRECOND(0);
	return g_pModels->modelGetPosition(idModel, pPos);
}

SX_LIB_API void SGeom_ModelSetPosition(ID idModel, const float3 *pPos)
{
	GEOM_PRECOND(_VOID);

	g_pModels->modelSetPosition(idModel, pPos);
}

SX_LIB_API const float3* SGeom_ModelGetRotation(ID idModel, float3 *pRot)
{
	GEOM_PRECOND(0);
	return g_pModels->modelGetRotation(idModel, pRot);
}

SX_LIB_API void SGeom_ModelSetRotation(ID idModel, const float3 *pRot)
{
	GEOM_PRECOND(_VOID);

	g_pModels->modelSetRotation(idModel, pRot);
}

SX_LIB_API const float3* SGeom_ModelGetScale(ID idModel, float3 *pScale)
{
	GEOM_PRECOND(0);
	return g_pModels->modelGetScale(idModel, pScale);
}

SX_LIB_API void SGeom_ModelSetScale(ID idModel, const float3 *pScale)
{
	GEOM_PRECOND(_VOID);

	g_pModels->modelSetScale(idModel, pScale);
}


SX_LIB_API const char* SGeom_ModelGetPath4Lod(ID idModel)
{
	GEOM_PRECOND(0);
	return g_pModels->modelGetPath4Lod(idModel);
}

SX_LIB_API void SGeom_ModelSetPath4Lod(ID idModel, const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pModels->modelSetLod(idModel, szPath);
}

SX_LIB_API const char* SGeom_ModelGetPath4Physics(ID idModel)
{
	GEOM_PRECOND(0);
	return g_pModels->modelGetPath4Physics(idModel);
}

SX_LIB_API void SGeom_ModelSetPath4Physics(ID idModel, const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pModels->modelSetPhysics(idModel, szPath);
}


SX_LIB_API ID SGeom_ModelGetCountGroups(ID idModel)
{
	GEOM_PRECOND(-1);
	return g_pModels->modelGetCountGroups(idModel);
}

SX_LIB_API ID SGeom_ModelGetGroupMtrlID(ID idModel, ID idGroup)
{
	GEOM_PRECOND(-1);
	return g_pModels->modelGetGroupMtrlID(idModel, idGroup);
}

SX_LIB_API void SGeom_ModelGetGroupCenter(ID idModel, ID idGroup, float3_t *pCenter)
{
	GEOM_PRECOND(_VOID);
	g_pModels->modelGetGroupCenter(idModel, idGroup, pCenter);
}

SX_LIB_API void SGeom_ModelGetGroupPlane(ID idModel, ID idGroup, D3DXPLANE *pPlane)
{
	GEOM_PRECOND(_VOID);
	g_pModels->modelGetGroupPlane(idModel, idGroup, pPlane);
}

SX_LIB_API void SGeom_ModelGetGroupMinMax(ID idModel, ID idGroup, float3_t *pMin, float3_t *pMax)
{
	GEOM_PRECOND(_VOID);
	g_pModels->modelGetGroupMinMax(idModel, idGroup, pMin, pMax);
}

SX_LIB_API bool SGeom_ModelGetSegmentation(ID idModel)
{
	GEOM_PRECOND(false);
	return g_pModels->modelGerSegmentation(idModel);
}

//**************************************************************************

SX_LIB_API void SGeom_SortTransparent(const float3 *pViewPos)
{
	GEOM_PRECOND(_VOID);
	g_pModels->sortTransparency(pViewPos);
}

//**************************************************************************

SX_LIB_API void SGeom_GetArrBuffsGeom(float3_t ***pppArrVertex, int32_t **ppArrCountVertex, uint32_t ***pppArrIndex, ID ***pppArrMtl, int32_t **ppArrCountIndex, int32_t *pCountModels)
{
	GEOM_PRECOND(_VOID);
	g_pModels->getArrBuffsGeom(pppArrVertex, ppArrCountVertex, pppArrIndex, pppArrMtl, ppArrCountIndex, pCountModels);
}

SX_LIB_API void SGeom_ClearArrBuffsGeom(float3_t **ppArrVertex, int32_t *pArrCountVertex, uint32_t **ppArrIndex, ID **ppArrMtl, int32_t *pArrCountIndex, int32_t iCountModels)
{
	for (int i = 0; i < iCountModels; ++i)
	{
		mem_delete_a(ppArrVertex[i]);
		mem_delete_a(ppArrIndex[i]);
		mem_delete_a(ppArrMtl[i]);
	}
	mem_delete_a(ppArrVertex);
	mem_delete_a(pArrCountVertex);
	mem_delete_a(ppArrIndex);
	mem_delete_a(ppArrMtl);
	mem_delete_a(pArrCountIndex);
}

SX_LIB_API bool SGeom_TraceBeam(const float3 *pStart, const float3 *pDir, float3 *pResult, ID *pIDmodel, ID *pIDmtl)
{
	GEOM_PRECOND(false);
	return g_pModels->traceBeam(pStart, pDir, pResult, pIDmodel, pIDmtl);
}
