#include <windows.h>

#include "animated.h"

#include <gcore/sxgcore.h>

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

#define SA_PRECOND() if(!g_mgr){reportf(-1, "%s - sxanim is not init", gen_msg_location);return;}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

AnimationManager * g_mgr = NULL;

void Report(int level, const char* format, ...)
{
	va_list va;
	va_start(va, format);
	reportf(level, format, va);
	va_end(va);
}

SX_LIB_API void SXAnim_0Create()
{
	if(g_mgr)
	{
		reportf(-1, "%s - sxanim double init", gen_msg_location);
		return;
	}
	g_mgr = new AnimationManager(SGCore_GetDXDevice());
}
SX_LIB_API void SXAnim_0Kill()
{
	SA_PRECOND();
	mem_delete(g_mgr);
}

SX_LIB_API void SXAnim_Update(int thread)
{
	SA_PRECOND();
	g_mgr->Update(thread);
}
SX_LIB_API void SXAnim_Sync()
{
	SA_PRECOND();
	g_mgr->Sync();
}
SX_LIB_API void SXAnim_Render(ID for_id)
{
	SA_PRECOND();
	g_mgr->Render(for_id);
}

SX_LIB_API IAnimPlayer * SXAnim_CreatePlayer(const char * mdl)
{
	if(!g_mgr)
	{
		reportf(-1, "%s - sxanim is not init", gen_msg_location); return(NULL);
	}
	Animation * anim = new Animation(g_mgr);
	if(mdl)
	{
		anim->SetModel(mdl);
	}
	return(anim);
}

SX_LIB_API void SXAnim_ModelsComVisible(const ISXFrustum * frustum, const float3 * viewpos, ID id_arr)
{
	g_mgr->ComputeVis(frustum, viewpos, id_arr);
}

SX_LIB_API ID SXAnim_ModelsAddArrForCom()
{
	return(g_mgr->GetNextVisId());
}
SX_LIB_API void SXAnim_ModelsDelArrForCom(ID id_arr)
{
}
