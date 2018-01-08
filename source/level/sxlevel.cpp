
#define SXLEVEL_VERSION 1

#include "sxlevel.h"

#include "level.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

//##########################################################################

CLevel* g_pLevel = 0;

#define SL_PRECOND(ret) if(!g_pLevel){g_fnReportf(-1, "%s - sxlevel is not init", GEN_MSG_LOCATION);return ret;}

//##########################################################################

SX_LIB_API long SLevel_0GetVersion()
{
	return SXLEVEL_VERSION;
}

SX_LIB_API void SLevel_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SLevel_0Create(const char *szName, bool isUnic)
{
	if (szName && strlen(szName) > 1)
	{
		if (isUnic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, szName);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				g_fnReportf(-1, "%s - none unic name, sxlevel", GEN_MSG_LOCATION);
			}
			else
			{
				g_pLevel = new CLevel();
			}
		}
		else
		{
			g_pLevel = new CLevel();
		}
	}
	else
		g_fnReportf(-1, "%s - not init argument [name], sxlevel", GEN_MSG_LOCATION);
}

SX_LIB_API void SLevel_AKill()
{
	SL_PRECOND(_VOID);

	mem_delete(g_pLevel);
}

//##########################################################################

SX_LIB_API void SLevel_Clear()
{
	SL_PRECOND(_VOID);
	g_pLevel->clear();
}

SX_LIB_API void SLevel_Load(const char *szName, bool isGame)
{
	SL_PRECOND(_VOID);
	g_pLevel->load(szName, isGame);
}

SX_LIB_API void SLevel_Save(const char *szName)
{
	SL_PRECOND(_VOID);
	g_pLevel->save(szName);
}

SX_LIB_API const char* SLevel_GetName(char *szName)
{
	SL_PRECOND(0);
	return g_pLevel->getName(szName);
}


SX_LIB_API void SLevel_LoadParticles()
{
	SL_PRECOND(_VOID);
	g_pLevel->loadParticles();
}


SX_LIB_API void SLevel_SaveParticles()
{
	SL_PRECOND(_VOID);
	g_pLevel->saveParticles();
}

//##########################################################################

SX_LIB_API void SLevel_AmbientSndAdd(const char* path)
{
	SL_PRECOND(_VOID);
	g_pLevel->ambientSndAdd(path);
}

SX_LIB_API void SLevel_AmbientSndGet(ID id, char* path)
{
	SL_PRECOND(_VOID);
	g_pLevel->ambientSndGet(id, path);
}

SX_LIB_API UINT SLevel_AmbientSndGetCount()
{
	SL_PRECOND(0);
	return g_pLevel->ambientSndGetCount();
}

SX_LIB_API void SLevel_AmbientSndClear()
{
	SL_PRECOND(_VOID);
	g_pLevel->ambientSndClear();
}


SX_LIB_API void SLevel_AmbientSndPlay()
{
	SL_PRECOND(_VOID);
	g_pLevel->ambientSndPlay();
}

SX_LIB_API void SLevel_AmbientSndUpdate()
{
	SL_PRECOND(_VOID);
	g_pLevel->ambientSndUpdate();
}

SX_LIB_API void SLevel_AmbientSndPause()
{
	SL_PRECOND(_VOID);
	g_pLevel->ambientSndPause();
}

SX_LIB_API bool SLevel_AmbientSndGetPlaying()
{
	SL_PRECOND(false);
	return g_pLevel->ambientSndGetPlaying();
}

//#############################################################################

SX_LIB_API void SLevel_WeatherLoad(const char* path)
{
	SL_PRECOND(_VOID);
	g_pLevel->weatherLoad(path);
}

SX_LIB_API void SLevel_WeatherUpdate()
{
	SL_PRECOND(_VOID);
	g_pLevel->weatherUpdate();
}

SX_LIB_API float SLevel_WeatherGetCurrRainDensity()
{
	SL_PRECOND(0.f);
	return g_pLevel->weatherGetCurrRainDensity();
}

SX_LIB_API void SLevel_WeatherSndPlay()
{
	SL_PRECOND(_VOID);
	g_pLevel->weatherSndPlay();
}

SX_LIB_API void SLevel_WeatherSndPause()
{
	SL_PRECOND(_VOID);
	g_pLevel->weatherSndPause();
}

SX_LIB_API bool SLevel_WeatherSndIsPlaying()
{
	SL_PRECOND(false);
	return g_pLevel->weatherSndGetPlaying();
}