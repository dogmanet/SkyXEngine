
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "sxphysics.h"
#include "PhyWorld.h"

#include <core/sxcore.h>

//..\..\bin\BulletDynamics_vs2010_debug.lib;..\..\bin\BulletCollision_vs2010_debug.lib;..\..\bin\LinearMath_vs2010_debug.lib;


#if defined(_DEBUG)
#	pragma comment(lib, "sxcore_d.lib")
#	pragma comment(lib, "BulletDynamics_vs2010_debug.lib")
#	pragma comment(lib, "BulletCollision_vs2010_debug.lib")
#	pragma comment(lib, "LinearMath_vs2010_debug.lib")
#else
#	pragma comment(lib, "sxcore.lib")
#	pragma comment(lib, "BulletDynamics_vs2010.lib")
#	pragma comment(lib, "BulletCollision_vs2010.lib")
#	pragma comment(lib, "LinearMath_vs2010.lib")
#endif

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

PhyWorld * g_pWorld = NULL;

#define SP_PRECOND(ret) if(!g_pWorld){reportf(-1, "%s - sxphysics is not init", gen_msg_location);return ret;}

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

void Report(int level, const char* format, ...)
{
	va_list va;
	va_start(va, format);
	reportf(level, format, va);
	va_end(va);
}

SX_LIB_API void SXPhysics_0Create()
{
	if(g_pWorld)
	{
		reportf(-1, "%s - sxphysics double init", gen_msg_location);
		return;
	}
	Core_SetOutPtr();

	g_pWorld = new PhyWorld();
}
SX_LIB_API void SXPhysics_0Kill()
{
	SP_PRECOND(_VOID);
	mem_delete(g_pWorld);
}

SX_LIB_API void SXPhysics_Update(int thread)
{
	SP_PRECOND(_VOID);
	g_pWorld->Update(thread);
}
SX_LIB_API void SXPhysics_UpdateSetThreadNum(int num)
{
	SP_PRECOND(_VOID);
	g_pWorld->SetThreadNum(num);
}
SX_LIB_API void SXPhysics_Sync()
{
	SP_PRECOND(_VOID);
	g_pWorld->Sync();
}

SX_LIB_API void SXPhysics_Dbg_Set(report_func rf)
{
	reportf = rf;
}
