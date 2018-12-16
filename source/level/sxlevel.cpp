
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXLEVEL_VERSION 1

#include "sxlevel.h"

#include "level.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

//##########################################################################

CLevel* g_pLevel = 0;

#define SL_PRECOND(ret) if(!g_pLevel){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxlevel is not init", GEN_MSG_LOCATION);return ret;}

//##########################################################################

SX_LIB_API long SLevel_0GetVersion()
{
	return SXLEVEL_VERSION;
}

SX_LIB_API void SLevel_Dbg_Set(report_func fnFunc)
{
	g_fnReportf = fnFunc;
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
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
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
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
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

SX_LIB_API void SLevel_AmbientSndAdd(const char *szPath)
{
	SL_PRECOND(_VOID);
	g_pLevel->ambientSndAdd(szPath);
}

SX_LIB_API void SLevel_AmbientSndGet(ID id, char *szPath)
{
	SL_PRECOND(_VOID);
	g_pLevel->ambientSndGet(id, szPath);
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

SX_LIB_API void SLevel_WeatherLoad(const char *szPath)
{
	SL_PRECOND(_VOID);
	g_pLevel->weatherLoad(szPath);
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

//#############################################################################

SX_LIB_API BOOL SLevel_EnumLevels(CLevelInfo *pInfo)
{
	WIN32_FIND_DATA fd;
	bool bFound = false;
	if(!pInfo->m_hFind)
	{
		if((pInfo->m_hFind = ::FindFirstFile((String(Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS)) + "*").c_str(), &fd)) != INVALID_HANDLE_VALUE)
		{
			bFound = true;
		}
	}
	else
	{
		if(::FindNextFile(pInfo->m_hFind, &fd))
		{
			bFound = true;
		}
	}

	if(bFound)
	{
		while(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, "..")))
		{
			bFound = false;
			if(::FindNextFile(pInfo->m_hFind, &fd))
			{
				if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, ".."))
				{
					bFound = true;
					break;
				}
			}
			else
			{
				break;
			}
		}
	}

	if(!bFound)
	{
		if(INVALID_HANDLE_VALUE != pInfo->m_hFind)
		{
			::FindClose(pInfo->m_hFind);
		}
		return(FALSE);
	}
	
	strncpy(pInfo->m_szName, fd.cFileName, MAX_LEVEL_STRING - 1);

	{
		char szFullPath[1024];
		sprintf(szFullPath, "%s%s/%s.lvl", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), pInfo->m_szName, pInfo->m_szName);

		ISXConfig *pConfig = Core_OpConfig(szFullPath);
		if (pConfig->keyExists("level", "local_name"))
		{
			strncpy(pInfo->m_szLocalName, pConfig->getKey("level", "local_name"), MAX_LEVEL_STRING - 1);
		}
		else
		{
			strncpy(pInfo->m_szLocalName, fd.cFileName, MAX_LEVEL_STRING - 1);
		}
		mem_release(pConfig);

		sprintf(szFullPath, "%s%s/preview.bmp", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), pInfo->m_szName);
		pInfo->m_bHasPreview = FileExistsFile(szFullPath);
	}

	return(TRUE);
}
