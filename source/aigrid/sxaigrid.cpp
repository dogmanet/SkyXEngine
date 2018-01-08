
#include "sxaigrid.h"
#include "aigrid.h"

#define SXAIGRID_VERSION 1

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

bool QuadPhyNavigate(float3_t * pos)
{
	return false;
}

g_aiquad_phy_navigate AIQuadPhyNavigate = QuadPhyNavigate;

AIGrid* ObjAIGrid = 0;

#define AIG_PRECOND(retval) if(!ObjAIGrid){g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - sxaigrid is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SAIG_0GetVersion()
{
	return SXAIGRID_VERSION;
}

SX_LIB_API void SAIG_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SAIG_0Create(const char* name, bool use_graphics, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				g_fnReportf(-1, "%s - none unic name, sxaigrid", GEN_MSG_LOCATION);
			}
			else
			{
				ObjAIGrid = new AIGrid();
				if (use_graphics)
					ObjAIGrid->GraphicsInit();
			}
		}
		else
		{
			ObjAIGrid = new AIGrid();
			if (use_graphics)
				ObjAIGrid->GraphicsInit();
		}
	}
	else
		g_fnReportf(-1, "%s - not init argument [name], sxaigrid", GEN_MSG_LOCATION);
}

SX_LIB_API void SAIG_AKill()
{
	AIG_PRECOND(_VOID);

	mem_delete(ObjAIGrid);
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

	ObjAIGrid->Clear();
}

SX_LIB_API void SAIG_GridSave(const char* path)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridSave(path);
}

SX_LIB_API void SAIG_GridLoad(const char* path)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridLoad(path);
}

SX_LIB_API UINT SAIG_GridGetCountQuads()
{
	AIG_PRECOND(0);

	return ObjAIGrid->GridGetCountQuads();
}

SX_LIB_API void SAIG_BBCreate(const float3* center, const float3* param)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBCreate(center, param);
}

SX_LIB_API bool SAIG_BBIsCreated()
{
	AIG_PRECOND(false);

	return ObjAIGrid->BBIsCreated();
}

SX_LIB_API void SAIG_BBSetDimensions(const float3* dim)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBSetDimensions(dim);
}

SX_LIB_API void SAIG_BBGetDimensions(float3* dim)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBGetDimensions(dim);
}

SX_LIB_API void SAIG_BBSetPos(const float3* pos)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBSetPos(pos);
}

SX_LIB_API void SAIG_BBGetPos(float3* pos)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBGetPos(pos);
}

SX_LIB_API void SAIG_BBCreateFinish()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBCreateFinish();
}

SX_LIB_API bool SAIG_BBIsCreatedFinish()
{
	AIG_PRECOND(false);

	return ObjAIGrid->BBIsCreatedFinish();
}

SX_LIB_API AIQUAD_STATE SAIG_QuadGetState(ID id)
{
	AIG_PRECOND(AIQUAD_STATE::AIQUAD_STATE_FREE);

	return ObjAIGrid->QuadGetState(id);
}

SX_LIB_API void SAIG_QuadSetState(ID id, AIQUAD_STATE state)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSetState(id, state);
}

SX_LIB_API void SAIG_QuadSetStateWho(ID id, ID who)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSetStateWho(id, who);
}

SX_LIB_API ID SAIG_QuadGetStateWho(ID id)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->QuadGetStateWho(id);
}

SX_LIB_API bool SAIG_QuadIs2Neighbors(ID id, ID idn1, ID idn2)
{
	AIG_PRECOND(false);

	return ObjAIGrid->QuadIs2Neighbors(id, idn1, idn2);
}

SX_LIB_API ID SAIG_QuadGet(const float3* pos, bool isnear_or_permissible)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->QuadGet(pos, isnear_or_permissible);
}

SX_LIB_API bool SAIG_QuadGetPos(ID id, float3* pos)
{
	AIG_PRECOND(false);

	return ObjAIGrid->QuadGetPos(id, pos);
}

SX_LIB_API void SAIG_QuadSetPosY(ID id, float posy)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSetPosY(id, posy);
}

SX_LIB_API float SAIG_QuadGetPosY(ID id)
{
	AIG_PRECOND(0);

	return ObjAIGrid->QuadGetPosY(id);
}

SX_LIB_API ID SAIG_QuadAdd(const float3* pos)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->QuadAdd(pos);
}

SX_LIB_API bool SAIG_QuadDelete(ID id)
{
	AIG_PRECOND(false);

	return ObjAIGrid->QuadDelete(id);
}


SX_LIB_API void SAIG_GridClear()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridClear();
}

SX_LIB_API ID SAIG_GridTraceBeam(const float3* start, const float3* dir)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->GridTraceBeam(start, dir);
}

SX_LIB_API void SAIG_QuadSelect(ID id, bool consider_prev)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSelect(id, consider_prev);
}

SX_LIB_API void SAIG_QuadSelectedAddPosY(float posy)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSelectedAddPosY(posy);
}

SX_LIB_API void SAIG_QuadSelectedDelete()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSelectedDelete();
}

SX_LIB_API bool SAIG_QuadIsFree(ID id, int radius)
{
	AIG_PRECOND(false);

	return ObjAIGrid->QuadIsFree(id, radius);
}

SX_LIB_API ID SAIG_QuadGetNear(const float3* pos, bool isfree, int raius)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->QuadGetNear(pos, isfree, raius);
}

SX_LIB_API void SAIG_GridTestValidation()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridTestValidation();
}

SX_LIB_API UINT SAIG_GridGetCountSplits()
{
	AIG_PRECOND(0);

	return ObjAIGrid->GridGetCountSplits();
}

SX_LIB_API void SAIG_GridSetMarkSplits(bool mark)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridSetMarkSplits(mark);
}

SX_LIB_API bool SAIG_GridGetMarkSplits()
{
	AIG_PRECOND(false);

	return ObjAIGrid->GridGetMarkSplits();
}

SX_LIB_API void SAIG_GridGenerate()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridGenerate();
}

SX_LIB_API bool SAIG_GridFindPath(ID beginq, ID endq)
{
	AIG_PRECOND(false);

	return ObjAIGrid->GridFindPath(beginq, endq);
}

SX_LIB_API UINT SAIG_GridGetSizePath()
{
	AIG_PRECOND(0);

	return ObjAIGrid->GridGetSizePath();
}

SX_LIB_API bool SAIG_GridGetPath(ID * pmem, UINT count, bool reverse)
{
	AIG_PRECOND(false);

	return ObjAIGrid->GridGetPath(pmem, count, reverse);
}

SX_LIB_API void SAIG_GridSetColorArr(const ID * pmem, DWORD color, UINT count)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridSetColorArr(pmem, color, count);
}

SX_LIB_API void SAIG_GridSetNullColor()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridSetNullColor();
}


SX_LIB_API void SAIG_GraphPointGenerate()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GraphPointGenerate();
}

SX_LIB_API UINT SAIG_GraphPointGetCount()
{
	AIG_PRECOND(0);

	return ObjAIGrid->GraphPointGetCount();
}

SX_LIB_API void SAIG_GraphPointClear()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GraphPointClear();
}

SX_LIB_API void SAIG_GraphPointAdd(ID id)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GraphPointAdd(id);
}

SX_LIB_API void SAIG_GraphPointDelete(ID id)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GraphPointDelete(id);
}

SX_LIB_API ID SAIG_GraphPointGetNear(ID beginq, ID endq)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->GraphPointGetNear(beginq, endq);
}


SX_LIB_API void SAIG_RenderQuads(const ISXFrustum * frustum, const float3 * viewpos, float dist)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->RenderQuads(frustum, viewpos, dist);
}

SX_LIB_API void SAIG_RenderGraphPoints(const float3 * viewpos, float dist)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->RenderGraphPoints(viewpos, dist);
}

SX_LIB_API void SAIG_RenderBB()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->RenderBB();
}
