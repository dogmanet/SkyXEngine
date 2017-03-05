
#pragma once

#define _BUILD_STR_LIB
#define S4G_BUILD_DLL
#define S4G_DLL
#include "s4g\s4g.h"
#include "s4g\s4g_main.cpp"
#include "s4g\s4g_api.cpp"

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

