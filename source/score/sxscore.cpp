
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SXSCORE_VERSION 1

#include "sxscore.h"
#include "sound.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

MainSound* MSound = 0;

#define SCORE_PRECOND(retval) if(!MSound){g_fnReportf(-1, "%s - sxsound is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SSCore_0GetVersion()
{
	return SXSCORE_VERSION;
}

SX_LIB_API void SSCore_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SSCore_0Create(const char* name, HWND hwnd, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				g_fnReportf(-1, "%s - none unic name, sxsound", GEN_MSG_LOCATION);
			}
			else
			{
				MSound = new MainSound();
				MSound->Init(hwnd);
			}
		}
		else
		{
			MSound = new MainSound();
			MSound->Init(hwnd);
		}
	}
	else
		g_fnReportf(-1, "%s - not init argument [name], sxsound", GEN_MSG_LOCATION);
}

SX_LIB_API void SSCore_AKill()
{
	mem_delete(MSound);
}

//#############################################################################

SX_LIB_API void SSCore_Clear()
{
	SCORE_PRECOND(_VOID);

	MSound->Clear();
}


SX_LIB_API void SSCore_Update(const float3* viewpos, const float3* viewdir)
{
	SCORE_PRECOND(_VOID);

	MSound->Update(viewpos, viewdir);
}

SX_LIB_API int SSCore_SndsPlayCountGet()
{
	SCORE_PRECOND(0);

	return MSound->SoundsPlayCountGet();
}

SX_LIB_API int SSCore_SndsLoadCountGet()
{
	SCORE_PRECOND(0);

	return MSound->SoundsLoadCountGet();
}

SX_LIB_API ID SSCore_SndCreate2d(const char *file, bool looping, DWORD size_stream)
{
	SCORE_PRECOND(-1);

	return MSound->SoundCreate2d(file, looping, size_stream);
}

SX_LIB_API ID SSCore_SndCreate3d(const char *file, bool looping, DWORD size_stream, float dist, float shift_pan)
{
	SCORE_PRECOND(-1);

	return MSound->SoundCreate3d(file, looping, size_stream, dist, shift_pan);
}

SX_LIB_API ID SSCore_SndCreate2dInst(const char *file, bool looping, DWORD size_stream)
{
	SCORE_PRECOND(-1);

	return  MSound->SoundCreate2dInst(file, looping, size_stream);
}

SX_LIB_API ID SSCore_SndCreate3dInst(const char *file, bool looping, DWORD size_stream, float dist, float shift_pan)
{
	SCORE_PRECOND(-1);

	return  MSound->SoundCreate3dInst(file, looping, size_stream, dist, shift_pan);
}

SX_LIB_API ID SSCore_SndFind2dInst(const char *file)
{
	SCORE_PRECOND(-1);

	return MSound->SoundFind2dInst(file);
}

SX_LIB_API ID SSCore_SndFind3dInst(const char *file)
{
	SCORE_PRECOND(-1);

	return MSound->SoundFind3dInst(file);
}

SX_LIB_API void SSCore_SndInstancePlay2d(ID id, int volume, int pan)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundInstancePlay2d(id, volume, pan);
}

SX_LIB_API void SSCore_SndInstancePlay3d(ID id, const float3* pos)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundInstancePlay3d(id, pos);
}


SX_LIB_API bool SSCore_SndIsInit(ID id)
{
	SCORE_PRECOND(false);

	return MSound->SoundIsInit(id);
}

SX_LIB_API void SSCore_SndDelete(ID id)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundDelete(id);
}


SX_LIB_API void SSCore_SndPlay(ID id, int looping)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundPlay(id, looping);
}

SX_LIB_API void SSCore_SndPause(ID id)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundPause(id);
}

SX_LIB_API void SSCore_SndStop(ID id)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundStop(id);
}


SX_LIB_API void SSCore_SndStateSet(ID id, SOUND_OBJSTATE state)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundStateSet(id, state);
}

SX_LIB_API SOUND_OBJSTATE SSCore_SndStateGet(ID id)
{
	SCORE_PRECOND(SOUND_OBJSTATE_STOP);

	return MSound->SoundStateGet(id);
}

SX_LIB_API void SSCore_SndPosCurrSet(ID id, DWORD pos, int type)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundPosCurrSet(id, pos, type);
}

SX_LIB_API DWORD SSCore_SndPosCurrGet(ID id, int type)
{
	SCORE_PRECOND(0);

	return MSound->SoundPosCurrGet(id, type);
}


SX_LIB_API void SSCore_SndVolumeSet(ID id, long volume, int type)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundVolumeSet(id, volume, type);
}

SX_LIB_API long SSCore_SndVolumeGet(ID id, int type)
{
	SCORE_PRECOND(0);

	return MSound->SoundVolumeGet(id, type);
}


SX_LIB_API void SSCore_SndPanSet(ID id, long value, int type)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundPanSet(id, value, type);
}

SX_LIB_API long SSCore_SndPanGet(ID id, int type)
{
	SCORE_PRECOND(0);

	return MSound->SoundPanGet(id, type);
}


SX_LIB_API void SSCore_SndFreqCurrSet(ID id, DWORD value)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundFreqCurrSet(id, value);
}

SX_LIB_API DWORD SSCore_SndFreqCurrGet(ID id)
{
	SCORE_PRECOND(0);

	return MSound->SoundFreqCurrGet(id);
}

SX_LIB_API DWORD SSCore_SndFreqOriginGet(ID id)
{
	SCORE_PRECOND(0);

	return MSound->SoundFreqOriginGet(id);
}

SX_LIB_API void SSCore_SndPosWSet(ID id, const float3* pos)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundPosWSet(id, pos);
}

SX_LIB_API void SSCore_SndPosWGet(ID id, float3* pos)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundPosWGet(id, pos);
}


SX_LIB_API int SSCore_SndLengthSecGet(ID id)
{
	SCORE_PRECOND(0);

	return MSound->SoundLengthSecGet(id);
}

SX_LIB_API DWORD SSCore_SndBytesPerSecGet(ID id)
{
	SCORE_PRECOND(0);

	return MSound->SoundBytesPerSecGet(id);
}

SX_LIB_API DWORD SSCore_SndSizeGet(ID id)
{
	SCORE_PRECOND(0);

	return MSound->SoundSizeGet(id);
}

SX_LIB_API void SSCore_SndFileGet(ID id, char* path)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundFileGet(id, path);
}


SX_LIB_API float SSCore_SndDistAudibleGet(ID id)
{
	SCORE_PRECOND(0);

	return MSound->SoundDistAudibleGet(id);
}

SX_LIB_API void SSCore_SndDistAudibleSet(ID id, float value)
{
	SCORE_PRECOND(_VOID);

	MSound->SoundDistAudibleSet(id, value);
}
