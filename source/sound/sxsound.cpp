

#include <sound\sxsound.h>
#include <sound\sxsounds_class.h>
SXMainSound* ObjectMainSound = 0;
IDirectSound8* GDeviceSound;
#include <sound\sxsounds_class.cpp>

#define SXSOUND_VERSION 1

report_func reportf = def_report;

long SSound_0GetVersion()
{
	return SXSOUND_VERSION;
}

void SSound_Dbg_Set(report_func rf)
{
	reportf = rf;
}

void SSound_0Create(const char* name, HWND hwnd, bool is_unic)
{
		if(name && strlen(name) > 1)
		{
				if(is_unic)
				{
					HANDLE hMutex = CreateMutex(NULL, FALSE, name);
						if(GetLastError() == ERROR_ALREADY_EXISTS)
						{
							CloseHandle(hMutex);
							reportf(-1, "%s - none unic name for system sound", gen_msg_location);
						}
						else
						{
							ObjectMainSound = new SXMainSound(name);
							int cerr = ObjectMainSound->Init(hwnd);
							if (cerr == SOUND_CORE_ERR_FAILED_INIT)
								reportf(-1, "%s - none init sound system", gen_msg_location);
							else if (cerr == SOUND_CORE_ERR_FAILED_CL)
								reportf(-1, "%s - none init cooperation level for sound system", gen_msg_location);
							else if (cerr == SOUND_CORE_ERR_FAILED_PRIM_BUF)
								reportf(-1, "%s - none init primary buffer for sound system", gen_msg_location);
							else if (cerr == SOUND_CORE_ERR_FAILED_SET_FORMAT)
								reportf(-1, "%s - none init format of primary buffer for sound system", gen_msg_location);
						}
				}
				else
				{
					ObjectMainSound = new SXMainSound(name);
					int cerr = ObjectMainSound->Init(hwnd);
					if (cerr == SOUND_CORE_ERR_FAILED_INIT)
						reportf(-1, "%s - none init sound system", gen_msg_location);
					else if (cerr == SOUND_CORE_ERR_FAILED_CL)
						reportf(-1, "%s - none init cooperation level for sound system", gen_msg_location);
					else if (cerr == SOUND_CORE_ERR_FAILED_PRIM_BUF)
						reportf(-1, "%s - none init primary buffer for sound system", gen_msg_location);
					else if (cerr == SOUND_CORE_ERR_FAILED_SET_FORMAT)
						reportf(-1, "%s - none init format of primary buffer for sound system", gen_msg_location);
				}
		}
		else
			reportf(-1, "%s - not init argument [name] for system sound", gen_msg_location);
}

DWORD SSound_Create2d(const char *file, bool looping, int streaming, DWORD size_stream)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = new SXSoundObject();
		tmpsnd->Load(file, looping, streaming, size_stream);
		if (tmpsnd)
			return tmpsnd->GetID();
}

DWORD SSound_Create3d(const char *file, bool looping, int streaming, DWORD size_stream, float max_dist, float damping, float shift_pan)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = new SXSoundObject();
		tmpsnd->Load(file, looping, streaming, size_stream, max_dist, damping, shift_pan);
		if (tmpsnd)
			return tmpsnd->GetID();
}

void SSound_Play(DWORD id)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
		if (tmpsnd)
			tmpsnd->Play();
		else
			reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

void SSound_Stop(DWORD id)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

		SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
		if (tmpsnd)
			tmpsnd->Stop();
		else
			reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

void SSound_SetVolume(DWORD id, long volume, int type)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
		if (tmpsnd)
			tmpsnd->SetVolume(volume, type);
		else
			reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

long SSound_GetVolume(DWORD id, int type)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

		SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
		if (tmpsnd)
			return tmpsnd->GetVolume(type);
		else
			reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

void SSound_SetCurrentPos(DWORD id, DWORD pos, int	type)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
		if (tmpsnd)
			tmpsnd->SetCurrentPos(pos, type);
		else
			reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

DWORD SSound_GetCurrentPos(DWORD id, int type)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
		if (tmpsnd)
			return tmpsnd->GetCurrentPos(type);
		else
			reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

void SSound_SetPan(DWORD id, long value, int type)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
		if (tmpsnd)
			return tmpsnd->SetPan(value, type);
		else
			reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

long SSound_GetPan(DWORD id, int type)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
		if(tmpsnd)
			return tmpsnd->GetPan(type);
		else
			reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

void SSound_SetFrequency(DWORD id, long value)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
		if (tmpsnd)
			tmpsnd->SetFrequency(value);
		else
			reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

DWORD SSound_GetFrequency(DWORD id)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->GetFrequency();
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

DWORD SSound_GetFrequencyOriginal(DWORD id)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->GetFrequencyOriginal();
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

bool SSound_Playing(DWORD id)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->Playing();
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

//эффекты накладывающиеся на звук
int SSound_EffectIsOn(DWORD id, int effect)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->EffectIsOn(effect);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

int SSound_EffectOnOff(DWORD id, int effect, int switch_)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->EffectOnOff(effect, switch_);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

int SSound_SetEffectGargle(DWORD id, DWORD RateHz, DWORD WaveShape)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->SetEffectGargle(RateHz, WaveShape);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}
int SSound_SetEffectChorus(DWORD id, float	WetDryMix, float Depth, float Feedback, float Frequency, long Waveform, float Delay, long Phase)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->SetEffectChorus(WetDryMix, Depth, Feedback, Frequency, Waveform, Delay, Phase);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

int SSound_SetEffectFlanger(DWORD id, float	WetDryMix, float Depth, float Feedback, float Frequency, long Waveform, float Delay, long Phase)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->SetEffectFlanger(WetDryMix, Depth, Feedback, Frequency, Waveform, Delay, Phase);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

int SSound_SetEffectEcho(DWORD id, float	WetDryMix, float Feedback, float LeftDelay, float RightDelay, long PanDelay)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->SetEffectEcho(WetDryMix, Feedback, LeftDelay, RightDelay, PanDelay);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

int SSound_SetEffectDistortion(DWORD id, float	Gain, float Edge, float PostEQCenterFrequency, float PostEQBandwidth, float PreLowpassCutoff)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->SetEffectDistortion(Gain, Edge, PostEQCenterFrequency, PostEQBandwidth, PreLowpassCutoff);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

int SSound_SetEffectCompressor(DWORD id, float	Gain, float Attack, float Release, float Threshold, float Ratio, float Predelay)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->SetEffectCompressor(Gain, Attack, Release, Threshold, Ratio, Predelay);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

int SSound_SetEffectParameq(DWORD id, float	Center, float Bandwidth, float Gain)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->SetEffectParameq(Center, Bandwidth, Gain);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

int SSound_SetEffectI3DL2Reverb(DWORD id,
	long Room, long RoomHF, float RoomRolloffFactor, float DecayTime, float DecayHFRatio, long Reflections,
	float ReflectionsDelay, long Reverb, float ReverbDelay, float Diffusion, float Density, float HFReference
	)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->SetEffectI3DL2Reverb(Room, RoomHF, RoomRolloffFactor, DecayTime, DecayHFRatio, Reflections,
		ReflectionsDelay, Reverb, ReverbDelay, Diffusion, Density, HFReference);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

int SSound_SetEffectWavesReverb(DWORD id, float InGain, float ReverbMix, float ReverbTime, float HighFreqRTRatio)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->SetEffectWavesReverb(InGain, ReverbMix, ReverbTime, HighFreqRTRatio);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

float SSound_GetCountSec(DWORD id)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->GetCountSec();
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

DWORD SSound_GetBytesPerSec(DWORD id)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->GetBytesPerSec();
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

DWORD SSound_GetSize(DWORD id)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->GetSize();
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

void SSound_GetPathFile(DWORD id, char* path)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		tmpsnd->GetPathFile(path);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

void SSound_AllUpdate(float3* campos, float3* camdir)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	ObjectMainSound->Update(campos, camdir);
}

/*float SSound_GetMinDist(DWORD id)
{
	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->GetMinDist();
}

void SSound_SetMinDist(DWORD id, float value)
{
	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		tmpsnd->SetMinDist(value);
}*/

float SSound_GetMaxDist(DWORD id)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		return tmpsnd->GetMaxDist();
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

void SSound_SetMaxDist(DWORD id, float value)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		tmpsnd->SetMaxDist(value);
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}

void SSound_SetPos(DWORD id, float3* pos)
{
	if (!ObjectMainSound)
		reportf(-1, "%s - sound system is not init", gen_msg_location);

	SXSoundObject* tmpsnd = ObjectMainSound->GetSoundObject(id);
	if (tmpsnd)
		tmpsnd->Position = *pos;
	else
		reportf(1, "%s - not found id for sound [%d]", gen_msg_location, id);
}