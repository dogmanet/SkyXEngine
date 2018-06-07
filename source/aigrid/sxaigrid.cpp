
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxaigrid.h"
#include "aigrid.h"

#define SXAIGRID_VERSION 1

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

bool QuadPhyNavigate(float3_t *pPos)
{
	return false;
}

g_aiquad_phy_navigate AIQuadPhyNavigate = QuadPhyNavigate;

AIGrid* g_pAIGrid = 0;

#define AIG_PRECOND(retval) if(!g_pAIGrid){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxaigrid is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SAIG_0GetVersion()
{
	return SXAIGRID_VERSION;
}

SX_LIB_API void SAIG_Dbg_Set(report_func fnReport)
{
	g_fnReportf = fnReport;
}

SX_LIB_API void SAIG_0Create(const char *szName, bool useGraphics, bool isUnic)
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
				g_pAIGrid = new AIGrid();
				if (useGraphics)
					g_pAIGrid->GraphicsInit();
			}
		}
		else
		{
			g_pAIGrid = new AIGrid();
			if (useGraphics)
				g_pAIGrid->GraphicsInit();
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SAIG_AKill()
{
	AIG_PRECOND(_VOID);

	mem_delete(g_pAIGrid);
}

//#############################################################################

SX_LIB_API void SAIG_SetFunc_QuadPhyNavigate(g_aiquad_phy_navigate func)
{
	AIG_PRECOND(_VOID);

	AIQuadPhyNavigate = func;
}

SX_LIB_API void SAIG_Clear()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->Clear();
}

SX_LIB_API bool SAIG_ExistsQuads()
{
	AIG_PRECOND(false);

	return g_pAIGrid->existsQuads();
}

SX_LIB_API void SAIG_GridSave(const char *szPath)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GridSave(szPath);
}

SX_LIB_API void SAIG_GridLoad(const char* szPath)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GridLoad(szPath);
}

SX_LIB_API UINT SAIG_GridGetCountQuads()
{
	AIG_PRECOND(0);

	return g_pAIGrid->GridGetCountQuads();
}

SX_LIB_API void SAIG_BBCreate(const float3 *pCenter, const float3 *pParam)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->BBCreate(pCenter, pParam);
}

SX_LIB_API bool SAIG_BBIsCreated()
{
	AIG_PRECOND(false);

	return g_pAIGrid->BBIsCreated();
}

SX_LIB_API void SAIG_BBSetDimensions(const float3 *pDimensions)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->BBSetDimensions(pDimensions);
}

SX_LIB_API void SAIG_BBGetDimensions(float3 *pDimensions)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->BBGetDimensions(pDimensions);
}

SX_LIB_API void SAIG_BBSetPos(const float3 *pPos)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->BBSetPos(pPos);
}

SX_LIB_API void SAIG_BBGetPos(float3 *pPos)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->BBGetPos(pPos);
}

SX_LIB_API void SAIG_BBCreateFinish()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->BBCreateFinish();
}

SX_LIB_API bool SAIG_BBIsCreatedFinish()
{
	AIG_PRECOND(false);

	return g_pAIGrid->BBIsCreatedFinish();
}

SX_LIB_API AIQUAD_STATE SAIG_QuadGetState(ID idQuad)
{
	AIG_PRECOND(AIQUAD_STATE::AIQUAD_STATE_FREE);

	return g_pAIGrid->QuadGetState(idQuad);
}

SX_LIB_API void SAIG_QuadSetState(ID idQuad, AIQUAD_STATE state)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->QuadSetState(idQuad, state);
}

SX_LIB_API void SAIG_QuadSetStateWho(ID idQuad, ID idWho)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->QuadSetStateWho(idQuad, idWho);
}

SX_LIB_API ID SAIG_QuadGetStateWho(ID idQuad)
{
	AIG_PRECOND(-1);

	return g_pAIGrid->QuadGetStateWho(idQuad);
}

SX_LIB_API bool SAIG_QuadIs2Neighbors(ID idQuad, ID idQuad1, ID idQuad2)
{
	AIG_PRECOND(false);

	return g_pAIGrid->QuadIs2Neighbors(idQuad, idQuad1, idQuad2);
}

SX_LIB_API ID SAIG_QuadGet(const float3 *pPos, bool isNearOrPermissible)
{
	AIG_PRECOND(-1);

	return g_pAIGrid->QuadGet(pPos, isNearOrPermissible);
}

SX_LIB_API bool SAIG_QuadGetPos(ID idQuad, float3 *pPos)
{
	AIG_PRECOND(false);

	return g_pAIGrid->QuadGetPos(idQuad, pPos);
}

SX_LIB_API void SAIG_QuadSetPosY(ID idQuad, float fPosY)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->QuadSetPosY(idQuad, fPosY);
}

SX_LIB_API float SAIG_QuadGetPosY(ID idQuad)
{
	AIG_PRECOND(0);

	return g_pAIGrid->QuadGetPosY(idQuad);
}

SX_LIB_API ID SAIG_QuadAdd(const float3 *pPos)
{
	AIG_PRECOND(-1);

	return g_pAIGrid->QuadAdd(pPos);
}

SX_LIB_API bool SAIG_QuadDelete(ID idQuad)
{
	AIG_PRECOND(false);

	return g_pAIGrid->QuadDelete(idQuad);
}


SX_LIB_API void SAIG_GridClear()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GridClear();
}

SX_LIB_API ID SAIG_GridTraceBeam(const float3 *pStart, const float3 *pDir)
{
	AIG_PRECOND(-1);

	return g_pAIGrid->GridTraceBeam(pStart, pDir);
}

SX_LIB_API void SAIG_QuadSelect(ID pStart, bool useConsiderPrev)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->QuadSelect(pStart, useConsiderPrev);
}

SX_LIB_API void SAIG_QuadSelectedAddPosY(float fPosY)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->QuadSelectedAddPosY(fPosY);
}

SX_LIB_API void SAIG_QuadSelectedDelete()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->QuadSelectedDelete();
}

SX_LIB_API bool SAIG_QuadIsFree(ID idQuad, int fRadius)
{
	AIG_PRECOND(false);

	return g_pAIGrid->QuadIsFree(idQuad, fRadius);
}

SX_LIB_API ID SAIG_QuadGetNear(const float3 *pPos, bool isFree, int iRadius)
{
	AIG_PRECOND(-1);

	return g_pAIGrid->QuadGetNear(pPos, isFree, iRadius);
}

SX_LIB_API void SAIG_GridTestValidation()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GridTestValidation();
}

SX_LIB_API UINT SAIG_GridGetCountSplits()
{
	AIG_PRECOND(0);

	return g_pAIGrid->GridGetCountSplits();
}

SX_LIB_API void SAIG_GridSetMarkSplits(bool isMark)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GridSetMarkSplits(isMark);
}

SX_LIB_API bool SAIG_GridGetMarkSplits()
{
	AIG_PRECOND(false);

	return g_pAIGrid->GridGetMarkSplits();
}

SX_LIB_API void SAIG_GridGenerate()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GridGenerate();
}

SX_LIB_API void SAIG_GridQueryFindPathUpdate(UINT uiLimitMls)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->gridQueryFindPathUpdate(uiLimitMls);
}

SX_LIB_API ID SAIG_GridQueryFindPath(ID idBegin, ID idEnd)
{
	AIG_PRECOND(-1);

	return g_pAIGrid->gridQueryFindPath(idBegin, idEnd);
}

SX_LIB_API bool SAIG_GridCancelQueryFindPath(ID idQuery)
{
	AIG_PRECOND(false);

	return g_pAIGrid->gridCancelQueryFindPath(idQuery);
}

SX_LIB_API int SAIG_GridGetSizePath(ID idQueueObject)
{
	AIG_PRECOND(0);

	return g_pAIGrid->gridGetSizePath(idQueueObject);
}

SX_LIB_API bool SAIG_GridGetPath(ID idQueueObject, ID *pMem, UINT uiCount, bool isReverse)
{
	AIG_PRECOND(false);

	return g_pAIGrid->gridGetPath(idQueueObject, pMem, uiCount, isReverse);
}

SX_LIB_API void SAIG_GridSetColorArr(const ID *pMem, DWORD dwColor, UINT uiCount)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GridSetColorArr(pMem, dwColor, uiCount);
}

SX_LIB_API void SAIG_GridSetNullColor()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GridSetNullColor();
}


SX_LIB_API void SAIG_GraphPointGenerate()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GraphPointGenerate();
}

SX_LIB_API UINT SAIG_GraphPointGetCount()
{
	AIG_PRECOND(0);

	return g_pAIGrid->GraphPointGetCount();
}

SX_LIB_API void SAIG_GraphPointClear()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GraphPointClear();
}

SX_LIB_API void SAIG_GraphPointAdd(ID idQuad)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GraphPointAdd(idQuad);
}

SX_LIB_API void SAIG_GraphPointDelete(ID idQuad)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->GraphPointDelete(idQuad);
}

SX_LIB_API ID SAIG_GraphPointGetNear(ID idBegin, ID idEnd)
{
	AIG_PRECOND(-1);

	return g_pAIGrid->GraphPointGetNear(idBegin, idEnd);
}


SX_LIB_API void SAIG_RenderQuads(const ISXFrustum *pFrustum, const float3 *pViewPos, float fDist)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->RenderQuads(pFrustum, pViewPos, fDist);
}

SX_LIB_API void SAIG_RenderGraphPoints(const float3 *pViewPos, float fDist)
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->RenderGraphPoints(pViewPos, fDist);
}

SX_LIB_API void SAIG_RenderBB()
{
	AIG_PRECOND(_VOID);

	g_pAIGrid->RenderBB();
}
