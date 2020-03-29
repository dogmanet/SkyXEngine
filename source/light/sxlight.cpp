
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXMATERIAL_LIGTH_VERSION 1

#include "sxlight.h"
#include "ml_data.h"

#include "light.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

#define ML_PRECOND(retval)
//if(!ArrLights){LibReport(-1, "%s - sxmtlligth is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SLight_0GetVersion()
{
	return SXMATERIAL_LIGTH_VERSION;
}

SX_LIB_API void SLight_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SLight_0Create(const char *szName, bool isUnic)
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
		light_data::Init();
	}
	else
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
	}
}

SX_LIB_API void SLight_AKill()
{
}



