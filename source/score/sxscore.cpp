
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

CSoundManager *g_pManagerSound = 0;

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
				g_pManagerSound = new CSoundManager();
				g_pManagerSound->init(hWnd);
			}
		}
		else
		{
			g_pManagerSound = new CSoundManager();
			g_pManagerSound->init(hWnd);
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

	g_pManagerSound->clear();
}


SX_LIB_API void SSCore_Update(const float3 *pViewPos, const float3 *pViewDir)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->update(pViewPos, pViewDir);
}

SX_LIB_API void SSCore_SetMainVolume(float fVolume)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->setMainVolume(fVolume);
}

SX_LIB_API int SSCore_SndsGetCountPlay()
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundsGetPlayCount();
}

SX_LIB_API int SSCore_SndsGetCountLoad()
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundsGetLoadCount();
}

//##########################################################################

SX_LIB_API void SSCore_ChannelAdd(ID idChannel, bool isPlaying)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->channelAdd(idChannel, isPlaying);
}

SX_LIB_API bool SSCore_ChannelExists(ID idChannel)
{
	SCORE_PRECOND(false);

	return g_pManagerSound->channelExists(idChannel);
}

SX_LIB_API int SSCore_ChannelGetSndCount(ID idChannel)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->channelGetSndCount(idChannel);
}

SX_LIB_API void SSCore_ChannelPlay(ID idChannel)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->channelPlay(idChannel);
}

SX_LIB_API void SSCore_ChannelPlayOnly(ID idChannel)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->channelPlayOnly(idChannel);
}

SX_LIB_API bool SSCore_ChannelPlaying(ID idChannel)
{
	SCORE_PRECOND(false);

	return g_pManagerSound->channelPlaying(idChannel);
}

SX_LIB_API void SSCore_ChannelStop(ID idChannel)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->channelStop(idChannel);
}

//##########################################################################

SX_LIB_API ID SSCore_SndCreate2d(const char *szFile, ID idChannel, UINT uiSizeStream)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->soundCreate2d(szFile, idChannel, uiSizeStream);
}

SX_LIB_API ID SSCore_SndCreate3d(const char *szFile, ID idChannel, UINT uiSizeStream, float fDist)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->soundCreate3d(szFile, idChannel, uiSizeStream, fDist);
}

SX_LIB_API ID SSCore_SndCreate2dInst(const char *szFile, ID idChannel)
{
	SCORE_PRECOND(-1);

	return  g_pManagerSound->soundCreate2dInst(szFile, idChannel);
}

SX_LIB_API ID SSCore_SndCreate3dInst(const char *szFile, ID idChannel, float fDist)
{
	SCORE_PRECOND(-1);

	return  g_pManagerSound->soundCreate3dInst(szFile, idChannel, fDist);
}

SX_LIB_API ID SSCore_SndFind2dInst(const char *szFile, ID idChannel)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->soundFind2dInst(szFile, idChannel);
}

SX_LIB_API ID SSCore_SndFind3dInst(const char *szFile, ID idChannel)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->soundFind3dInst(szFile, idChannel);
}


SX_LIB_API ID SSCore_SndInstancePlay(ID idSound, bool isBlocked, bool isLooping, UINT *pArrDelay, int iSizeArrDelay, const float3 *pPos, float fVolume, float fPan)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->soundInstancePlay(idSound, isBlocked, isLooping, pArrDelay, iSizeArrDelay, pPos, fVolume, fPan);
}

/*SX_LIB_API ID SSCore_SndInstancePlay2d(ID idSound, bool isBlocked, bool isLooping, int iVolume, int iPan)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->soundInstancePlay(idSound, isBlocked, isLooping, 0, 0, 0, iVolume, iPan);
}

SX_LIB_API ID SSCore_SndInstancePlayDelay2d(ID idSound, bool isBlocked, bool isLooping, UINT *pArrDelay, int iSizeArrDelay, int iVolume, int iPan)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->soundInstancePlay(idSound, isBlocked, isLooping, pArrDelay, iSizeArrDelay, 0, iVolume, iPan);
}

SX_LIB_API ID SSCore_SndInstancePlay3d(ID idSound, bool isBlocked, bool isLooping, const float3 *pPos)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->soundInstancePlay(idSound, isBlocked, isLooping, 0, 0, pPos);
}

SX_LIB_API ID SSCore_SndInstancePlayDelay3d(ID idSound, bool isBlocked, bool isLooping, UINT *pArrDelay, int iSizeArrDelay, const float3 *pPos)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->soundInstancePlay(idSound, isBlocked, isLooping, pArrDelay, iSizeArrDelay, pPos);
}*/


SX_LIB_API bool SSCore_SndInstancePlaying(ID idSound, ID idInstance)
{
	SCORE_PRECOND(false);

	return g_pManagerSound->soundInstancePlaying(idSound, idInstance);
}

SX_LIB_API void SSCore_SndInstanceStop(ID idSound, ID idInstance)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundInstanceStop(idSound, idInstance);
}

SX_LIB_API void SSCore_SndInstanceFree(ID idSound, ID idInstance)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundInstanceStop(idSound, idInstance);
}


SX_LIB_API bool SSCore_SndIsInit(ID idSound)
{
	SCORE_PRECOND(false);

	return g_pManagerSound->soundIsInit(idSound);
}

SX_LIB_API void SSCore_SndDelete(ID idSound)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundDelete(idSound);
}


SX_LIB_API void SSCore_SndPlay(ID idSound, bool isLooping, UINT *pArrDelay, int iSizeArrDelay)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundPlay(idSound, isLooping, pArrDelay, iSizeArrDelay);
}

/*SX_LIB_API void SSCore_SndPlayDelay(ID idSound, bool isLooping, UINT *pArrDelay, int iSizeArrDelay)
{
	SCORE_PRECOND(_VOID);*/

	/*Array<UINT> aValues;
	
	UINT uiCurrVal = 0;
	va_list va;
	va_start(va, isLooping);

	uiCurrVal = va_arg(va, UINT);
	while (uiCurrVal > 0)
	{
		aValues.push_back(uiCurrVal);
		uiCurrVal = va_arg(va, UINT);
	}

	va_end(va);*/

	/*g_pManagerSound->soundPlayDelay(idSound, isLooping, pArrDelay, iSizeArrDelay);
}*/

SX_LIB_API void SSCore_SndPause(ID idSound)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundPause(idSound);
}

SX_LIB_API void SSCore_SndStop(ID idSound)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundStop(idSound);
}


SX_LIB_API void SSCore_SndSetState(ID idSound, SOUND_OBJSTATE state)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundSetState(idSound, state);
}

SX_LIB_API SOUND_OBJSTATE SSCore_SndGetState(ID idSound)
{
	SCORE_PRECOND(SOUND_OBJSTATE_STOP);

	return g_pManagerSound->soundGetState(idSound);
}

SX_LIB_API void SSCore_SndSetPosPlay(ID idSound, UINT uiPosMls)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundSetPosPlay(idSound, uiPosMls);
}

SX_LIB_API UINT SSCore_SndGetPosPlay(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundGetPosPlay(idSound);
}


SX_LIB_API void SSCore_SndSetVolume(ID idSound, float fVolume)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundSetVolume(idSound, fVolume);
}

SX_LIB_API float SSCore_SndGetVolume(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundGetVolume(idSound);
}


SX_LIB_API void SSCore_SndSetPan(ID idSound, float fValue)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundSetPan(idSound, fValue);
}

SX_LIB_API float SSCore_SndGetPan(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundGetPan(idSound);
}


SX_LIB_API void SSCore_SndSetCurrFreq(ID idSound, UINT fValue)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundSetFreqCurr(idSound, fValue);
}

SX_LIB_API UINT SSCore_SndGetCurrFreq(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundGetFreqCurr(idSound);
}

SX_LIB_API UINT SSCore_SndGetOriginFreq(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundGetFreqOrigin(idSound);
}

SX_LIB_API void SSCore_SndSetPosWorld(ID idSound, const float3 *pPos)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundSetPosWorld(idSound, pPos);
}

SX_LIB_API void SSCore_SndGetPosWorld(ID idSound, float3 *pPos)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundGetPosWorld(idSound, pPos);
}


SX_LIB_API int SSCore_SndGetLengthSec(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundGetLengthSec(idSound);
}

SX_LIB_API UINT SSCore_SndGetBytesPerSec(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundGetBytesPerSec(idSound);
}

SX_LIB_API UINT SSCore_SndGetSize(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundGetSize(idSound);
}

SX_LIB_API void SSCore_SndGetFile(ID idSound, char *szPath)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundGetFile(idSound, szPath);
}


SX_LIB_API float SSCore_SndGetDistAudible(ID idSound)
{
	SCORE_PRECOND(0);

	return g_pManagerSound->soundGetDistAudible(idSound);
}

SX_LIB_API void SSCore_SndSetDistAudible(ID idSound, float fValue)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->soundSetDistAudible(idSound, fValue);
}

//##########################################################################

SX_LIB_API ID SSCore_SndkitCreate(const char *szName, ID idChannel, bool is3D, float fDistAudible)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->sndkitCreate(szName, idChannel, is3D, fDistAudible);
}

SX_LIB_API ID SSCore_SndkitCreateFromList(const char *szName, ID idChannel, bool is3D, float fDistAudible, float fVolume, const char *szFileOrList, ...)
{
	SCORE_PRECOND(-1);

	Array<String> aStrings;
	// если в первой строке есть запятые тогда будем парсить строку
	if (strstr(szFileOrList, ","))
	{
		aStrings = StrExplode(szFileOrList, ",");
	}
	// иначе считываем все строчные аргументы
	else if (szFileOrList)
	{
		const char *szStrCurr = 0;
		va_list va;
		va_start(va, szFileOrList);

		szStrCurr = szFileOrList;
		while (szStrCurr)
		{
			aStrings.push_back(szStrCurr);
			szStrCurr = va_arg(va, const char *);
		}

		va_end(va);
	}

	return g_pManagerSound->sndkitCreateFromList(szName, idChannel, aStrings, is3D, fDistAudible, fVolume);
}

SX_LIB_API void SSCore_SndkitAddSound(ID idSndKit, const char *szFile, float fDistAudible, float fVolume, UINT *pArrDelay, int iSizeArrDelay)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->sndkitAddSound(idSndKit, szFile, fDistAudible, fVolume, pArrDelay, iSizeArrDelay);
}

SX_LIB_API uint64_t SSCore_SndkitPlay(ID idSndKit, uint64_t id2, const float3 *pPos, float fVolume, float fPan)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->sndkitPlay(idSndKit, id2, pPos, fVolume, fPan);
}

SX_LIB_API void SSCore_SndkitStop(ID idSndKit, uint64_t id2)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->sndkitStop(idSndKit, id2);
}

SX_LIB_API ID SSCore_SndkitGetID(const char *szName)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->sndkitGetID(szName);
}

SX_LIB_API ID SSCore_SndkitGetChannel(ID idSndKit)
{
	SCORE_PRECOND(-1);

	return g_pManagerSound->sndkitGetChannel(idSndKit);
}

SX_LIB_API void SSCore_SndkitGetName(ID idSndKit, char *szName)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->sndkitGetName(idSndKit, szName);
}

SX_LIB_API void SSCore_SndkitDelete(ID idSndKit)
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->sndkitDelete(idSndKit);
}

SX_LIB_API void SSCore_SndkitDeleteAll()
{
	SCORE_PRECOND(_VOID);

	g_pManagerSound->sndkitDeleteAll();
}