
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXMATERIAL_LIGTH_VERSION 1

#include "sxlight.h"
#include "ml_data.h"

#include "light.h"

//##########################################################################

SX_LIB_API long SLight_0GetVersion()
{
	return SXMATERIAL_LIGTH_VERSION;
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



