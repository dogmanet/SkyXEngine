
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxmtrl.h"

#include "MaterialSystem.h"


CMaterialSystem *g_pMaterialSystem = NULL;

//##########################################################################

DECLARE_PROFILER_INTERNAL();

SX_LIB_API void SMtrl_0Create(const char *szName, bool isUnic, bool isServerMode)
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
		/*if(!isServerMode)
		{
			mtrl_data::Init();
		}*/

		INIT_OUTPUT_STREAM(Core_GetIXCore());
		INIT_PROFILER_INTERNAL();

		g_pMaterialSystem = new CMaterialSystem();
		Core_GetIXCore()->getPluginManager()->registerInterface(IXMATERIALSYSTEM_GUID, g_pMaterialSystem);
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SMtrl_AKill()
{
	mem_delete(g_pMaterialSystem);
}


SX_LIB_API void SMtrl_Update(DWORD timeDelta)
{
	XPROFILE_FUNCTION();
	g_pMaterialSystem->update(0.016f);
}
