
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXSCORE_VERSION 1

#include "sxscore.h"
#include "sound.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

MainSound *g_pManagerSound = 0;

#define SCORE_PRECOND(retval) if(!g_pManagerSound){LibReport(-1, "%s - sxsound is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SSCore_0GetVersion()
{
	return SXSCORE_VERSION;
}

SX_LIB_API void SSCore_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SSCore_0Create(const char *szName, HWND hWnd, bool isUnic)
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
				g_pManagerSound = new MainSound();
				g_pManagerSound->Init(hWnd);
			}
		}
		else
		{
			g_pManagerSound = new MainSound();
			g_pManagerSound->Init(hWnd);
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SSCore_AKill()
{
	mem_delete(g_pManagerSound);
}

//#############################################################################

SX_LIB_API void SSCore_Clear()
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->Clear();
}


SX_LIB_API void SSCore_Update(const float3 *pViewPos, const float3 *pViewDir)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->Update(pViewPos, pViewDir);
}

SX_LIB_API int SSCore_SndsPlayCountGet()
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundsPlayCountGet();
}

SX_LIB_API int SSCore_SndsLoadCountGet()
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundsLoadCountGet();
}

SX_LIB_API ID SSCore_SndCreate2d(const char *szFile, bool isLooping, UINT uiSizeStream)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->SoundCreate2d(szFile, isLooping, uiSizeStream);
}

SX_LIB_API ID SSCore_SndCreate3d(const char *szFile, bool isLooping, UINT uiSizeStream, float fDist, float fShiftPan)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->SoundCreate3d(szFile, isLooping, uiSizeStream, fDist, fShiftPan);
}

SX_LIB_API ID SSCore_SndCreate2dInst(const char *szFile, bool isLooping, UINT uiSizeStream)
{
	SCORE_PRECOND(-1);

	return  g_pManagerSound->SoundCreate2dInst(szFile, isLooping, uiSizeStream);
}

SX_LIB_API ID SSCore_SndCreate3dInst(const char *szFile, bool isLooping, UINT uiSizeStream, float fDist, float fShiftPan)
{
	SCORE_PRECOND(-1);

	return  g_pManagerSound->SoundCreate3dInst(szFile, isLooping, uiSizeStream, fDist, fShiftPan);
}

SX_LIB_API ID SSCore_SndFind2dInst(const char *szFile)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->SoundFind2dInst(szFile);
}

SX_LIB_API ID SSCore_SndFind3dInst(const char *szFile)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->SoundFind3dInst(szFile);
}

SX_LIB_API void SSCore_SndInstancePlay2d(ID idSound, int iVolume, int iPan)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundInstancePlay2d(idSound, iVolume, iPan);
}

SX_LIB_API void SSCore_SndInstancePlay3d(ID idSound, const float3 *pPos)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundInstancePlay3d(idSound, pPos);
}


SX_LIB_API bool SSCore_SndIsInit(ID idSound)
{
	SCORE_PRECOND(false);

	return g_pManagerSound->SoundIsInit(idSound);
}

SX_LIB_API void SSCore_SndDelete(ID idSound)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundDelete(idSound);
}


SX_LIB_API void SSCore_SndPlay(ID idSound, int isLooping)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundPlay(idSound, isLooping);
}

SX_LIB_API void SSCore_SndPause(ID idSound)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundPause(idSound);
}

SX_LIB_API void SSCore_SndStop(ID idSound)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundStop(idSound);
}


SX_LIB_API void SSCore_SndStateSet(ID idSound, SOUND_OBJSTATE state)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundStateSet(idSound, state);
}

SX_LIB_API SOUND_OBJSTATE SSCore_SndStateGet(ID idSound)
{
	SCORE_PRECOND(SOUND_OBJSTATE_STOP);

	return g_pManagerSound->SoundStateGet(idSound);
}

SX_LIB_API void SSCore_SndPosCurrSet(ID idSound, UINT uiPos, int iType)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundPosCurrSet(idSound, uiPos, iType);
}

SX_LIB_API UINT SSCore_SndPosCurrGet(ID idSound, int iType)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundPosCurrGet(idSound, iType);
}


SX_LIB_API void SSCore_SndVolumeSet(ID idSound, int iVolume, int iType)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundVolumeSet(idSound, iVolume, iType);
}

SX_LIB_API int SSCore_SndVolumeGet(ID idSound, int iType)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundVolumeGet(idSound, iType);
}


SX_LIB_API void SSCore_SndPanSet(ID idSound, int iValue, int iType)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundPanSet(idSound, iValue, iType);
}

SX_LIB_API int SSCore_SndPanGet(ID idSound, int iType)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundPanGet(idSound, iType);
}


SX_LIB_API void SSCore_SndFreqCurrSet(ID idSound, UINT fValue)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundFreqCurrSet(idSound, fValue);
}

SX_LIB_API UINT SSCore_SndFreqCurrGet(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundFreqCurrGet(idSound);
}

SX_LIB_API UINT SSCore_SndFreqOriginGet(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundFreqOriginGet(idSound);
}

SX_LIB_API void SSCore_SndPosWSet(ID idSound, const float3 *pPos)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundPosWSet(idSound, pPos);
}

SX_LIB_API void SSCore_SndPosWGet(ID idSound, float3 *pPos)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundPosWGet(idSound, pPos);
}


SX_LIB_API int SSCore_SndLengthSecGet(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundLengthSecGet(idSound);
}

SX_LIB_API UINT SSCore_SndBytesPerSecGet(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundBytesPerSecGet(idSound);
}

SX_LIB_API UINT SSCore_SndSizeGet(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundSizeGet(idSound);
}

SX_LIB_API void SSCore_SndFileGet(ID idSound, char *szPath)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundFileGet(idSound, szPath);
}


SX_LIB_API float SSCore_SndDistAudibleGet(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->SoundDistAudibleGet(idSound);
}

SX_LIB_API void SSCore_SndDistAudibleSet(ID idSound, float fValue)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->SoundDistAudibleSet(idSound, fValue);
}
