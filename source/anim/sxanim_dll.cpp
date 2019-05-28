
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "animated.h"
#include "Renderable.h"
#include "Updatable.h"

#include <gcore/sxgcore.h>

#include "AnimatedModelProvider.h"
#include "DynamicModelProvider.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

#define SA_PRECOND(ret) if(!g_mgr){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxanim is not init", GEN_MSG_LOCATION);return ret;}

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
CRenderable *g_pRenderable = NULL;
CUpdatable *g_pUpdatable = NULL;
CAnimatedModelProvider *g_pAnimatedModelProvider = NULL;
CDynamicModelProvider *g_pDynamicModelProvider = NULL;

SX_LIB_API void SXAnim_0Create(bool m_isServerMode)
{
	if(g_mgr)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxanim double init", GEN_MSG_LOCATION);
		return;
	}
	Core_SetOutPtr();
	g_mgr = new AnimationManager(m_isServerMode ? NULL : SGCore_GetDXDevice());
	g_pRenderable = new CRenderable();
	g_pUpdatable = new CUpdatable();
	g_pAnimatedModelProvider = new CAnimatedModelProvider(Core_GetIXCore());
	g_pDynamicModelProvider = new CDynamicModelProvider(Core_GetIXCore());

	Core_GetIXCore()->getPluginManager()->registerInterface(IXRENDERABLE_GUID, g_pRenderable);
	Core_GetIXCore()->getPluginManager()->registerInterface(IXUPDATABLE_GUID, g_pUpdatable);
	Core_GetIXCore()->getPluginManager()->registerInterface(IXANIMATEDMODELPROVIDER_GUID, g_pAnimatedModelProvider);
	Core_GetIXCore()->getPluginManager()->registerInterface(IXDYNAMICMODELPROVIDER_GUID, g_pDynamicModelProvider);

}
SX_LIB_API void SXAnim_AKill()
{
	SA_PRECOND(_VOID);
	mem_delete(g_mgr);
	mem_delete(g_pRenderable);
	mem_delete(g_pAnimatedModelProvider);
	mem_delete(g_pDynamicModelProvider);
}

SX_LIB_API IAnimPlayer * SXAnim_CreatePlayer(const char * mdl)
{
	SA_PRECOND(NULL);
	Animation * anim = new Animation(g_mgr);
	if(mdl)
	{
		anim->setModel(mdl);
	}
	return(anim);
}