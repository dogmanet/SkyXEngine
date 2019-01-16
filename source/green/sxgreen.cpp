
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXGEOM_VERSION 1

#include "sxgreen.h"
#include "green.h"

//##########################################################################

int CGreen::m_iRenderFreqGrass = 100;
float2_t CGreen::m_vDistLods = float2_t(50, 100);
float CGreen::m_fDistGrassLessening = 30;
bool CGreen::m_isUseSortFrontToBackSplits = false;
IGXContext* CGreen::m_pDXDevice = 0;

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

CGreen *g_pGreen = 0;

#define GEOM_PRECOND(retval) if(!g_pGreen){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxgreen is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SGreen_0GetVersion()
{
	return SXGEOM_VERSION;
}

SX_LIB_API void SGreen_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SGreen_0Create(const char *szName, bool isUnic, bool isServerMode)
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
				return;
			}
		}
		if(!isServerMode)
		{
			CGreen::m_pDXDevice = SGCore_GetDXDevice();
		}
		g_pGreen = new CGreen();
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SGreen_AKill()
{
	mem_delete(g_pGreen);
}

SX_LIB_API void SGreen_OnLostDevice()
{
	GEOM_PRECOND(_VOID);

	g_pGreen->onLostDevice();
}

SX_LIB_API void SGreen_OnResetDevice()
{
	GEOM_PRECOND(_VOID);

	g_pGreen->onResetDevice();
}

//##########################################################################

SX_LIB_API float SGreen_0SettGetDistLods1()
{
	return CGreen::m_vDistLods.x;
}

SX_LIB_API float SGreen_0SettGetDistLods2()
{
	return CGreen::m_vDistLods.y;
}

SX_LIB_API int SGreen_0SettGetFreqGrass()
{
	return CGreen::m_iRenderFreqGrass;
}

SX_LIB_API float SGreen_0SettGetBeginEndLessening()
{
	return CGreen::m_fDistGrassLessening;
}

SX_LIB_API bool SGreen_0SettGetSortFrontToBackSplits()
{
	return CGreen::m_isUseSortFrontToBackSplits;
}


SX_LIB_API void SGreen_0SettSetDistLods1(float val)
{
	CGreen::m_vDistLods.x = val;
}

SX_LIB_API void SGreen_0SettSetDistLods2(float val)
{
	CGreen::m_vDistLods.y = val;
}

SX_LIB_API void SGreen_0SettSetFreqGrass(int val)
{
	CGreen::m_iRenderFreqGrass = val;
}

SX_LIB_API void SGreen_0SettSetBeginEndLessening(float val)
{
	CGreen::m_fDistGrassLessening = val;
}

SX_LIB_API void SGreen_0SettSetSortFrontToBackSplits(bool val)
{
	CGreen::m_isUseSortFrontToBackSplits = val;
}

//##########################################################################

bool StdFuncIntersect(const float3 *pStart, const float3 *pEnd, float3 *pResult)
{
	return false;
}

g_phy_intersect g_fnIntersect = StdFuncIntersect;

SX_LIB_API void SGreen_SetFuncIntersect(g_phy_intersect fnFunc)
{
	g_fnIntersect = fnFunc;
}

SX_LIB_API ID SGreen_AddGreen(
	const char *szName,
	const char *szPathMask,
	bool shouldAveragedRGB,
	float fCountMax,
	const char *szPathLod0, const char *szPathLod1, const char *szPathLod2,
	const char *szNavMesh)
{
	GEOM_PRECOND(-1);
	return g_pGreen->generate(szName, szPathMask, shouldAveragedRGB, fCountMax, szPathLod0, szPathLod1, szPathLod2, szNavMesh);
}

SX_LIB_API ID SGreen_AddObject(ID idGreen, float3 *pPos, ID *pIDsplit)
{
	GEOM_PRECOND(-1);
	return g_pGreen->addObject(idGreen, pPos, 0, pIDsplit);
}

SX_LIB_API void SGreen_DelObject(ID idGreen, ID idSplit, ID idObj)
{
	GEOM_PRECOND(_VOID);
	return g_pGreen->deleteObject(idGreen, idSplit, idObj);
}

SX_LIB_API void SGreen_GetPosObject(ID idGreen, ID idSplit, ID idObj, float3_t *pPos)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->getPosObject(idGreen, idSplit, idObj, pPos);
}

SX_LIB_API void SGreen_SetPosObject(ID idGreen, ID *pIDsplit, ID *pIDobj, float3_t *pPos)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->setPosObject(idGreen, pIDsplit, pIDobj, pPos);
}

SX_LIB_API void SGreen_DelGreen(ID idGreen)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->deleteGreen(idGreen);
}

SX_LIB_API ID SGreen_AddVisCaclObj()
{
	GEOM_PRECOND(-1);
	return g_pGreen->addVisCaclObj();
}

SX_LIB_API bool SGreen_ExistsVisCaclObj(ID idVisCaclObj)
{
	GEOM_PRECOND(false);
	return g_pGreen->existsVisCaclObj(idVisCaclObj);
}

SX_LIB_API void SGreen_DelVisCaclObj(ID idVisCaclObj)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->deleteVisCaclObj(idVisCaclObj);
}

SX_LIB_API void SGreen_Save(const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->save(szPath);
}

SX_LIB_API void SGreen_Load(const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->load(szPath);
}

SX_LIB_API void SGreen_Clear()
{
	GEOM_PRECOND(_VOID);
	g_pGreen->clear();
}

SX_LIB_API void SGreen_ComVisible(const IFrustum* frustum, const float3 *pViewPos, ID idVisCaclObj)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->comArrIndeces(frustum, pViewPos, idVisCaclObj);
}

SX_LIB_API void SGreen_Render(DWORD timeDelta, const float3 *pViewPos, GREEN_TYPE type, ID idVisCaclObj)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->render(timeDelta, pViewPos, type, idVisCaclObj);
}

SX_LIB_API void SGreen_RenderSingly(DWORD timeDelta, const float3 *pViewPos, ID idGreen, ID idTexture)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->renderSingly(timeDelta, pViewPos, idGreen, idTexture);
}

SX_LIB_API void SGreen_RenderObject(DWORD timeDelta, const float3 *pViewPos, ID idGreen, ID idSplit, ID idObj, ID idTexture)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->renderObject(timeDelta, pViewPos, idGreen, idSplit, idObj, idTexture);
}


SX_LIB_API int SGreen_GetCount()
{
	GEOM_PRECOND(-1);
	return g_pGreen->getCountGreen();
}

SX_LIB_API char* SGreen_MGetName(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenName(idGreen);
}

SX_LIB_API int SGreen_MGetCountGen(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenCountGen(idGreen);
}

SX_LIB_API int SGreen_MGetCountPoly(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenCountPoly(idGreen);
}

SX_LIB_API int SGreen_MGetTypeCountGen(ID idGreen)
{
	GEOM_PRECOND(-1);
	return g_pGreen->getGreenTypeCountGen(idGreen);
}

SX_LIB_API const char* SGreen_MGetModel(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenModel(idGreen);
}

SX_LIB_API const char* SGreen_MGetLod1(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenLod1(idGreen);
}

SX_LIB_API const char* SGreen_MGetLod2(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenLod2(idGreen);
}

SX_LIB_API const char* SGreen_MGetMask(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenMask(idGreen);
}

SX_LIB_API const char* SGreen_MGetNav(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenNav(idGreen);
}

SX_LIB_API void SGreen_MSetLod(ID idGreen, int iLod, const char *szPathName)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->setGreenLod(idGreen, iLod, szPathName);
}

SX_LIB_API void SGreen_MSetNav(ID idGreen, const char *szPathName)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->setGreenNav(idGreen, szPathName);
}


SX_LIB_API void SGreen_GetNavMeshAndTransform(float3_t ***pppArrVertex, int32_t **ppArrVountVertex, uint32_t ***pppArrIndex, ID ***pppArrMtl, int32_t **ppArrCountIndex, CGreenDataVertex ***pppArrTransform, int32_t **ppArrCountTransform, int32_t *pArrCountGreen)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->getNavMeshAndTransform(pppArrVertex, ppArrVountVertex, pppArrIndex, pppArrMtl, ppArrCountIndex, pppArrTransform, ppArrCountTransform, pArrCountGreen);
}

SX_LIB_API void SGreen_ClearNavMeshAndTransform(float3_t **ppArrVertex, int32_t *pArrCountVertex, uint32_t **ppArrIndex, ID **ppArrMtl, int32_t *pArrCountIndex, CGreenDataVertex **ppArrTransform, int32_t *pArrCountTransform, int32_t iCountGreen)
{
	GEOM_PRECOND(_VOID);
	
	for (int i = 0; i < iCountGreen; ++i)
	{
		mem_delete_a(ppArrVertex[i]);
		mem_delete_a(ppArrIndex[i]);
		mem_delete_a(ppArrMtl[i]);
		mem_delete_a(ppArrTransform[i]);
	}
	mem_delete_a(ppArrVertex);
	mem_delete_a(pArrCountVertex);
	mem_delete_a(ppArrIndex);
	mem_delete_a(ppArrMtl);
	mem_delete_a(ppArrTransform);
	mem_delete_a(pArrCountTransform);
	mem_delete_a(pArrCountIndex);
}

SX_LIB_API bool SGreen_TraceBeam(const float3 *pStart, const float3 *pDir, float3 *pResult, ID *pIDgreen, ID *pIDsplits, ID *pIDobj, ID *pIDmtl)
{
	GEOM_PRECOND(false);
	return g_pGreen->traceBeam(pStart, pDir, pResult, pIDgreen, pIDsplits, pIDobj, pIDmtl);
}

SX_LIB_API bool SGreen_GetOccurencessLeafGrass(const float3 *pMin, const float3 *pMax, int iPhysicMtl)
{
	GEOM_PRECOND(false);
	return g_pGreen->getOccurencessLeafGrass(pMin, pMax, iPhysicMtl);
}
