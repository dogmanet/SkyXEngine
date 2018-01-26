
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXGEOM_VERSION 1

#include "sxgeom.h"

#include "static_geom.h"
#include "green.h"

//##########################################################################

bool CStaticGeom::m_isUseSortFrontToBackSplits = true;
//bool CStaticGeom::m_isUseSortFrontToBackModels = true;
IDirect3DDevice9* CStaticGeom::m_pDXDevice = 0;
float CStaticGeom::m_fDistForLod = 200.f;

int CGreen::m_iRenderFreqGrass = 100;
float2_t CGreen::m_vDistLods = float2_t(50, 100);
float CGreen::m_fDistGrassLessening = 30;
bool CGreen::m_isUseSortFrontToBackSplits = false;
IDirect3DDevice9* CGreen::m_pDXDevice = 0;

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

CStaticGeom *g_pGeometry = 0;
CGreen *g_pGreen = 0;

#define GEOM_PRECOND(retval) if(!g_pGeometry || !g_pGreen){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxgeom is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SGeom_0GetVersion()
{
	return SXGEOM_VERSION;
}

SX_LIB_API void SGeom_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SGeom_0Create(const char *szName, bool isUnic)
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
				CStaticGeom::m_pDXDevice = SGCore_GetDXDevice();
				CGreen::m_pDXDevice = SGCore_GetDXDevice();
				g_pGeometry = new CStaticGeom();
				g_pGreen = new CGreen();
			}
		}
		else
		{
			CStaticGeom::m_pDXDevice = SGCore_GetDXDevice();
			CGreen::m_pDXDevice = SGCore_GetDXDevice();
			g_pGeometry = new CStaticGeom();
			g_pGreen = new CGreen();
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SGeom_AKill()
{
	mem_delete(g_pGeometry);
	mem_delete(g_pGreen);
}

SX_LIB_API void SGeom_OnLostDevice()
{
	GEOM_PRECOND(_VOID);

	g_pGreen->onLostDevice();
	g_pGeometry->onLostDevice();
}

SX_LIB_API void SGeom_OnResetDevice()
{
	GEOM_PRECOND(_VOID);

	g_pGreen->onResetDevice();
	g_pGeometry->onResetDevice();
}

//##########################################################################

SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackModels(bool val)
{

}

SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackModels()
{
	return false;
}


SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackSplits(bool val)
{
	CStaticGeom::m_isUseSortFrontToBackSplits = val;
}

SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackSplits()
{
	return CStaticGeom::m_isUseSortFrontToBackSplits;
}


SX_LIB_API float SGeom_0SettGreenGetDistLods1()
{
	return CGreen::m_vDistLods.x;
}

SX_LIB_API float SGeom_0SettGreenGetDistLods2()
{
	return CGreen::m_vDistLods.y;
}

SX_LIB_API int SGeom_0SettGreenGetFreqGrass()
{
	return CGreen::m_iRenderFreqGrass;
}

SX_LIB_API float SGeom_0SettGreenGetBeginEndLessening()
{
	return CGreen::m_fDistGrassLessening;
}

SX_LIB_API bool SGeom_0SettGreenGetSortFrontToBackSplits()
{
	return CGreen::m_isUseSortFrontToBackSplits;
}


SX_LIB_API void SGeom_0SettGreenSetDistLods1(float val)
{
	CGreen::m_vDistLods.x = val;
}

SX_LIB_API void SGeom_0SettGreenSetDistLods2(float val)
{
	CGreen::m_vDistLods.y = val;
}

SX_LIB_API void SGeom_0SettGreenSetFreqGrass(int val)
{
	CGreen::m_iRenderFreqGrass = val;
}

SX_LIB_API void SGeom_0SettGreenSetBeginEndLessening(float val)
{
	CGreen::m_fDistGrassLessening = val;
}

SX_LIB_API void SGeom_0SettGreenSetSortFrontToBackSplits(bool val)
{
	CGreen::m_isUseSortFrontToBackSplits = val;
}

//##########################################################################

SX_LIB_API void SGeom_ModelsClear()
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->clear();
}

SX_LIB_API void SGeom_ModelsSave(const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->save(szPath);
}

SX_LIB_API void SGeom_ModelsLoad(const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->load(szPath);
}

SX_LIB_API int SGeom_ModelsGetCount()
{
	GEOM_PRECOND(-1);
	return g_pGeometry->getCountModel();
}

SX_LIB_API void SGeom_ModelsComVisible(const ISXFrustum *pFrustum, const float3 *pViewPos, ID idArr)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->comArrIndeces(pFrustum, pViewPos, idArr);
}

SX_LIB_API bool SGeom_ModelsSortExistsForRender(int iSort, ID idArr)
{
	GEOM_PRECOND(false);
	return g_pGeometry->sortExistsForRender(iSort, idArr);
}

SX_LIB_API void SGeom_ModelsRender(DWORD timeDelta, int iSortMtl, ID idArr, bool isSorted, ID idExcludeModel, ID idExcludeGroup)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->render(timeDelta, iSortMtl, idArr, idExcludeModel, idExcludeGroup, isSorted);
}

SX_LIB_API void SGeom_ModelsRenderSingly(DWORD timeDelta, ID idModel, ID idTexture)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->renderSingly(timeDelta, idModel, idTexture);
}

SX_LIB_API ID SGeom_ModelsAddModel(const char *szPath, const char *szLod1, const char *szName)
{
	GEOM_PRECOND(-1);
	return g_pGeometry->addModel(szPath, szLod1, szName);
}

SX_LIB_API void SGeom_ModelsDelModel(ID idModel)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->deleteModel(idModel);
}

SX_LIB_API void SGeom_ModelsGetMinMax(float3 *pMin, float3 *pMax)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->getMinMax(pMin, pMax);
}

SX_LIB_API ID SGeom_ModelsAddArrForCom()
{
	GEOM_PRECOND(-1);
	return g_pGeometry->addArrForCom();
}

SX_LIB_API bool SGeom_ModelsExistsArrForCom(ID idModel)
{
	GEOM_PRECOND(false);
	return g_pGeometry->existsArrForCom(idModel);
}

SX_LIB_API void SGeom_ModelsDelArrForCom(ID idArr)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->deleteArrForCom(idArr);
}


SX_LIB_API void SGeom_ModelsMGetMinMax(ID idModel, float3 *pMin, float3 *pMax)
{
	GEOM_PRECOND(_VOID);
	return g_pGeometry->getModelMinMax(idModel, pMin, pMax);
}

SX_LIB_API char* SGeom_ModelsMGetName(ID idModel)
{
	GEOM_PRECOND(0);
	return g_pGeometry->getModelName(idModel);
}

SX_LIB_API const char* SGeom_ModelsMGetPathName(ID idModel)
{
	GEOM_PRECOND(0);
	return g_pGeometry->getModelPathName(idModel);
}

SX_LIB_API int SGeom_ModelsMGetCountPoly(ID idModel)
{
	GEOM_PRECOND(-1);
	return g_pGeometry->getModelCountPoly(idModel);
}


SX_LIB_API float3* SGeom_ModelsMGetPosition(ID idModel)
{
	GEOM_PRECOND(0);
	return g_pGeometry->getModelPosition(idModel);
}

SX_LIB_API float3* SGeom_ModelsMGetRotation(ID idModel)
{
	GEOM_PRECOND(0);
	return g_pGeometry->getModelRotation(idModel);
}

SX_LIB_API float3* SGeom_ModelsMGetScale(ID idModel)
{
	GEOM_PRECOND(0);
	return g_pGeometry->getModelScale(idModel);
}


SX_LIB_API const char* SGeom_ModelsMGetLodPath(ID idModel)
{
	GEOM_PRECOND(0);
	return g_pGeometry->getModelLodPath(idModel);
}

SX_LIB_API void SGeom_ModelsMSetLodPath(ID idModel, const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->setModelLodPath(idModel, szPath);
}


SX_LIB_API void SGeom_ModelsMApplyTransform(ID idModel)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->applyTransform(idModel);
}


SX_LIB_API void SGeom_ModelsMSortGroups(const float3 *pViewPos, int iSortMtl)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->sortGroup(pViewPos, iSortMtl);
}


SX_LIB_API ID SGeom_ModelsMGetCountGroups(ID idModel)
{
	GEOM_PRECOND(-1);
	return g_pGeometry->getModelCountGroups(idModel);
}

SX_LIB_API ID SGeom_ModelsMGetGroupIDMat(ID idModel, ID idGroup)
{
	GEOM_PRECOND(-1);
	return g_pGeometry->getModelGroupIDMat(idModel, idGroup);
}

SX_LIB_API void SGeom_ModelsMGetGroupCenter(ID idModel, ID idGroup, float3_t *pCenter)
{
	GEOM_PRECOND(_VOID);
	return g_pGeometry->getModelGroupCenter(idModel, idGroup, pCenter);
}

SX_LIB_API void SGeom_ModelsMGetGroupMin(ID idModel, ID idGroup, float3_t *pMin)
{
	GEOM_PRECOND(_VOID);
	return g_pGeometry->getModelGroupMin(idModel, idGroup, pMin);
}

SX_LIB_API void SGeom_ModelsMGetGroupMax(ID idModel, ID idGroup, float3_t *pMax)
{
	GEOM_PRECOND(_VOID);
	return g_pGeometry->getModelGroupMax(idModel, idGroup, pMax);
}

SX_LIB_API void SGeom_ModelsMGetGroupPlane(ID idModel, ID idGroup, D3DXPLANE *pPlane)
{
	GEOM_PRECOND(_VOID);
	return g_pGeometry->getModelGroupPlane(idModel, idGroup, pPlane);
}



SX_LIB_API void SGeom_ModelsGetArrBuffsGeom(float3_t ***pppArrVertex, int32_t **ppArrCountVertex, uint32_t ***pppArrIndex, ID ***pppArrMtl, int32_t **ppArrCountIndex, int32_t *pCountModels)
{
	GEOM_PRECOND(_VOID);
	g_pGeometry->getArrBuffsGeom(pppArrVertex, ppArrCountVertex, pppArrIndex, pppArrMtl, ppArrCountIndex, pCountModels);
}

SX_LIB_API void SGeom_ModelsClearArrBuffsGeom(float3_t **ppArrVertex, int32_t *pArrCountVertex, uint32_t **ppArrIndex, ID **ppArrMtl, int32_t *pArrCountIndex, int32_t iCountModels)
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

SX_LIB_API bool SGeom_ModelsTraceBeam(const float3 *pStart, const float3 *pDir, float3 *pResult, ID *pIDmodel, ID *pIDmtl)
{
	GEOM_PRECOND(false);
	return g_pGeometry->traceBeam(pStart, pDir, pResult, pIDmodel, pIDmtl);
}

//##########################################################################

SX_LIB_API ID SGeom_GreenAddGreen(
	const char *szName,
	const char *szPathMask,
	float fCountMax,
	const char *szPathLod0, const char *szPathLod1, const char *szPathLod2,
	const char *szNavMesh)
{
	GEOM_PRECOND(-1);
	return g_pGreen->init(g_pGeometry, szName, szPathMask, fCountMax, szPathLod0, szPathLod1, szPathLod2, szNavMesh);
}

SX_LIB_API ID SGeom_GreenAddObject(ID idGreen, float3 *pPos, ID *pIDsplit)
{
	GEOM_PRECOND(-1);
	return g_pGreen->addObject(idGreen, pPos, 0, pIDsplit);
}

SX_LIB_API void SGeom_GreenDelObject(ID idGreen, ID idSplit, ID idObj)
{
	GEOM_PRECOND(_VOID);
	return g_pGreen->deleteObject(idGreen, idSplit, idObj);
}

SX_LIB_API void SGeom_GreenGetPosObject(ID idGreen, ID idSplit, ID idObj, float3_t *pPos)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->getPosObject(idGreen, idSplit, idObj, pPos);
}

SX_LIB_API void SGeom_GreenSetPosObject(ID idGreen, ID *pIDsplit, ID *pIDobj, float3_t *pPos)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->setPosObject(idGreen, pIDsplit, pIDobj, pPos);
}

SX_LIB_API void SGeom_GreenDelGreen(ID idGreen)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->deleteGreen(idGreen);
}

SX_LIB_API ID SGeom_GreenAddArrForCom()
{
	GEOM_PRECOND(-1);
	return g_pGreen->addArrForCom();
}

SX_LIB_API bool SGeom_GreenExistsArrForCom(ID idGreen)
{
	GEOM_PRECOND(false);
	return g_pGreen->existsArrForCom(idGreen);
}

SX_LIB_API void SGeom_GreenDelArrForCom(ID idArr)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->deleteArrForCom(idArr);
}

SX_LIB_API void SGeom_GreenSave(const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->save(szPath);
}

SX_LIB_API void SGeom_GreenLoad(const char *szPath)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->load(szPath);
}

SX_LIB_API void SGeom_GreenClear()
{
	GEOM_PRECOND(_VOID);
	g_pGreen->clear();
}

SX_LIB_API void SGeom_GreenComVisible(const ISXFrustum* frustum, float3 *pViewPos, ID idArr)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->comArrIndeces(frustum, pViewPos, idArr);
}

SX_LIB_API void SGeom_GreenRender(DWORD timeDelta, float3 *pViewPos, GREEN_TYPE type, ID idArr)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->render(timeDelta, pViewPos, type, idArr);
}

SX_LIB_API void SGeom_GreenRenderSingly(DWORD timeDelta, float3 *pViewPos, ID idGreen, ID idTexture)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->renderSingly(timeDelta, pViewPos, idGreen, idTexture);
}

SX_LIB_API void SGeom_GreenRenderObject(DWORD timeDelta, float3 *pViewPos, ID idGreen, ID idSplit, ID idObj, ID idTexture)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->renderObject(timeDelta, pViewPos, idGreen, idSplit, idObj, idTexture);
}


SX_LIB_API int SGeom_GreenGetCount()
{
	GEOM_PRECOND(-1);
	return g_pGreen->getCountGreen();
}

SX_LIB_API char* SGeom_GreenMGetName(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenName(idGreen);
}

SX_LIB_API int SGeom_GreenMGetCountGen(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenCountGen(idGreen);
}

SX_LIB_API int SGeom_GreenMGetCountPoly(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenCountPoly(idGreen);
}

SX_LIB_API int SGeom_GreenMGetTypeCountGen(ID idGreen)
{
	GEOM_PRECOND(-1);
	return g_pGreen->getGreenTypeCountGen(idGreen);
}

SX_LIB_API const char* SGeom_GreenMGetModel(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenModel(idGreen);
}

SX_LIB_API const char* SGeom_GreenMGetLod1(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenLod1(idGreen);
}

SX_LIB_API const char* SGeom_GreenMGetLod2(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenLod2(idGreen);
}

SX_LIB_API const char* SGeom_GreenMGetMask(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenMask(idGreen);
}

SX_LIB_API const char* SGeom_GreenMGetNav(ID idGreen)
{
	GEOM_PRECOND(0);
	return g_pGreen->getGreenNav(idGreen);
}

SX_LIB_API void SGeom_GreenMSetLod(ID idGreen, int iLod, const char *szPathName)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->setGreenLod(idGreen, iLod, szPathName);
}

SX_LIB_API void SGeom_GreenMSetNav(ID idGreen, const char *szPathName)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->setGreenNav(idGreen, szPathName);
}


SX_LIB_API void SGeom_GreenGetNavMeshAndTransform(float3_t ***pppArrVertex, int32_t **ppArrVountVertex, uint32_t ***pppArrIndex, ID ***pppArrMtl, int32_t **ppArrCountIndex, CGreenDataVertex ***pppArrTransform, int32_t **ppArrCountTransform, int32_t *pArrCountGreen)
{
	GEOM_PRECOND(_VOID);
	g_pGreen->getNavMeshAndTransform(pppArrVertex, ppArrVountVertex, pppArrIndex, pppArrMtl, ppArrCountIndex, pppArrTransform, ppArrCountTransform, pArrCountGreen);
}

SX_LIB_API void SGeom_GreenClearNavMeshAndTransform(float3_t **ppArrVertex, int32_t *pArrCountVertex, uint32_t **ppArrIndex, ID **ppArrMtl, int32_t *pArrCountIndex, CGreenDataVertex **ppArrTransform, int32_t *pArrCountTransform, int32_t iCountGreen)
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

SX_LIB_API bool SGeom_GreenTraceBeam(float3 *pStart, float3 *pDir, float3 *pResult, ID *pIDgreen, ID *pIDsplits, ID *pIDobj, ID *pIDmtl)
{
	GEOM_PRECOND(false);
	return g_pGreen->traceBeam(pStart, pDir, pResult, pIDgreen, pIDsplits, pIDobj, pIDmtl);
}

SX_LIB_API bool SGeom_GreenGetOccurencessLeafGrass(float3 *pMin, float3 *pMax, int iPhysicMtl)
{
	GEOM_PRECOND(false);
	return g_pGreen->getOccurencessLeafGrass(pMin, pMax, iPhysicMtl);
}
