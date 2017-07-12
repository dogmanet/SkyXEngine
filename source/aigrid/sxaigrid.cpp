
#include <aigrid/sxaigrid.h>

#define SXAIGRID_VERSION 1

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

bool QuadPhyNavigate(float3_t * pos)
{
	return false;
}

g_aiquad_phy_navigate AIQuadPhyNavigate = QuadPhyNavigate;

#include <aigrid/aigrid.cpp>

AIGrid* ObjAIGrid = 0;

#define AIG_PRECOND(retval) if(!ObjAIGrid){reportf(REPORT_MSG_LEVEL_ERROR, "%s - sxaigrid is not init", gen_msg_location); return retval;}

long SAIG_0GetVersion()
{
	return SXAIGRID_VERSION;
}

void SAIG_Dbg_Set(report_func rf)
{
	reportf = rf;
}

void SAIG_0Create(const char* name, bool use_graphics, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				reportf(-1, "%s - none unic name, sxaigrid", gen_msg_location);
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
		reportf(-1, "%s - not init argument [name], sxaigrid", gen_msg_location);
}

void SAIG_AKill()
{
	AIG_PRECOND(_VOID);

	mem_delete(ObjAIGrid);
}

//#############################################################################

void SAIG_SetFunc_QuadPhyNavigate(g_aiquad_phy_navigate func)
{
	AIG_PRECOND(_VOID);

	AIQuadPhyNavigate = func;
}

void SAIG_Clear()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->Clear();
}

void SAIG_GridSave(const char* path)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridSave(path);
}

void SAIG_GridLoad(const char* path)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridLoad(path);
}

UINT SAIG_GridGetCountQuads()
{
	AIG_PRECOND(0);

	return ObjAIGrid->GridGetCountQuads();
}

void SAIG_BBCreate(const float3* center, const float3* param)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBCreate(center, param);
}

bool SAIG_BBIsCreated()
{
	AIG_PRECOND(false);

	return ObjAIGrid->BBIsCreated();
}

void SAIG_BBSetDimensions(const float3* dim)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBSetDimensions(dim);
}

void SAIG_BBGetDimensions(float3* dim)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBGetDimensions(dim);
}

void SAIG_BBSetPos(const float3* pos)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBSetPos(pos);
}

void SAIG_BBGetPos(float3* pos)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBGetPos(pos);
}

void SAIG_BBCreateFinish()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->BBCreateFinish();
}

bool SAIG_BBIsCreatedFinish()
{
	AIG_PRECOND(false);

	return ObjAIGrid->BBIsCreatedFinish();
}

AIQUAD_STATE SAIG_QuadGetState(ID id)
{
	AIG_PRECOND(AIQUAD_STATE::AIQUAD_STATE_FREE);

	return ObjAIGrid->QuadGetState(id);
}

void SAIG_QuadSetState(ID id, AIQUAD_STATE state)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSetState(id, state);
}

ID SAIG_QuadGet(const float3* pos, bool isnear_or_permissible)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->QuadGet(pos, isnear_or_permissible);
}

void SAIG_QuadSetPosY(ID id, float posy)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSetPosY(id, posy);
}

float SAIG_QuadGetPosY(ID id)
{
	AIG_PRECOND(0);

	return ObjAIGrid->QuadGetPosY(id);
}

ID SAIG_QuadAdd(const float3* pos)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->QuadAdd(pos);
}

bool SAIG_QuadDelete(ID id)
{
	AIG_PRECOND(false);

	return ObjAIGrid->QuadDelete(id);
}


void SAIG_GridClear()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridClear();
}

ID SAIG_GridTraceBeam(const float3* start, const float3* dir)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->GridTraceBeam(start, dir);
}

void SAIG_QuadSelect(ID id, bool consider_prev)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSelect(id, consider_prev);
}

void SAIG_QuadSelectedAddPosY(float posy)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSelectedAddPosY(posy);
}

void SAIG_QuadSelectedDelete()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->QuadSelectedDelete();
}

bool SAIG_QuadIsFree(ID id, int radius)
{
	AIG_PRECOND(false);

	return ObjAIGrid->QuadIsFree(id, radius);
}

ID SAIG_QuadGetNear(const float3* pos, bool isfree, int raius)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->QuadGetNear(pos, isfree, raius);
}

void SAIG_GridTestValidation()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridTestValidation();
}

UINT SAIG_GridGetCountSplits()
{
	AIG_PRECOND(0);

	return ObjAIGrid->GridGetCountSplits();
}

void SAIG_GridSetMarkSplits(bool mark)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridSetMarkSplits(mark);
}

bool SAIG_GridGetMarkSplits()
{
	AIG_PRECOND(false);

	return ObjAIGrid->GridGetMarkSplits();
}

void SAIG_GridGenerate()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridGenerate();
}

int SAIG_GridFindPath(ID beginq, ID endq)
{
	AIG_PRECOND(false);

	return ObjAIGrid->GridFindPath(beginq, endq);
}

UINT SAIG_GridGetSizePath()
{
	AIG_PRECOND(0);

	return ObjAIGrid->GridGetSizePath();
}

bool SAIG_GridGetPath(ID * pmem, UINT count)
{
	AIG_PRECOND(false);

	return ObjAIGrid->GridGetPath(pmem, count);
}

void SAIG_GridSetColorArr(const ID * pmem, DWORD color, UINT count)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridSetColorArr(pmem, color, count);
}

void SAIG_GridSetNullColor()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GridSetNullColor();
}


void SAIG_GraphPointGenerate()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GraphPointGenerate();
}

UINT SAIG_GraphPointGetCount()
{
	AIG_PRECOND(0);

	return ObjAIGrid->GraphPointGetCount();
}

void SAIG_GraphPointClear()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GraphPointClear();
}

void SAIG_GraphPointAdd(ID id)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GraphPointAdd(id);
}

void SAIG_GraphPointDelete(ID id)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->GraphPointDelete(id);
}

ID SAIG_GraphPointGetNear(ID beginq, ID endq)
{
	AIG_PRECOND(-1);

	return ObjAIGrid->GraphPointGetNear(beginq, endq);
}


void SAIG_RenderQuads(const ISXFrustum * frustum, const float3 * viewpos, float dist)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->RenderQuads(frustum, viewpos, dist);
}

void SAIG_RenderGraphPoints(const float3 * viewpos, float dist)
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->RenderGraphPoints(viewpos, dist);
}

void SAIG_RenderBB()
{
	AIG_PRECOND(_VOID);

	ObjAIGrid->RenderBB();
}
