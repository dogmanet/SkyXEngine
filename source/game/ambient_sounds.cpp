
#include <game/ambient_sounds.h>

AmbientSounds::AmbientSounds()
{
	playing = false;
	PlayingLast = 0;
}

AmbientSounds::~AmbientSounds()
{
	Clear();
}

void AmbientSounds::Add(const char* path)
{
	ID id = SSCore_SndCreate2d(path);
	if (id >= 0)
		ArrIDsSnds.push_back(id);
}

void AmbientSounds::Get(ID id, char* path)
{
	if (id >= 0 && id < ArrIDsSnds.size())
	{
		SSCore_SndFileGet(ArrIDsSnds[id], path);
	}
}

UINT AmbientSounds::GetCount()
{
	return ArrIDsSnds.size();
}

void AmbientSounds::Clear()
{
	for (int i = 0; i < ArrIDsSnds.size(); ++i)
	{
		SSCore_SndDelete(ArrIDsSnds[i]);
	}

	ArrIDsSnds.clear();
}

void AmbientSounds::Play()
{
	if (PlayingLast >= 0 && PlayingLast < ArrIDsSnds.size())
	{
		SSCore_SndVolumeSet(ArrIDsSnds[PlayingLast], 100);
		SSCore_SndPlay(ArrIDsSnds[PlayingLast]);
		playing = true;
	}
}

void AmbientSounds::Update()
{
	if (!playing)
		return;

	static const float * ambient_snd_volume = GET_PCVAR_FLOAT("ambient_snd_volume");
	static float ambient_snd_volume_old = 1.f;

	if (SSCore_SndStateGet(ArrIDsSnds[PlayingLast]) != SoundObjState::sos_play)
	{
		if (PlayingLast + 1 < ArrIDsSnds.size())
			++PlayingLast;
		else
			PlayingLast = 0;

		SSCore_SndVolumeSet(ArrIDsSnds[PlayingLast], ambient_snd_volume_old*100.f);
		SSCore_SndPlay(ArrIDsSnds[PlayingLast]);
	}

	if (ambient_snd_volume && ambient_snd_volume_old != (*ambient_snd_volume))
	{
		ambient_snd_volume_old = *ambient_snd_volume;
		if (PlayingLast >= 0)
			SSCore_SndVolumeSet(ArrIDsSnds[PlayingLast], ambient_snd_volume_old*100.f);
	}
}

void AmbientSounds::Pause()
{
	if (!playing)
		return;

	SSCore_SndPause(ArrIDsSnds[PlayingLast]);

	playing = false;
}

bool AmbientSounds::IsPlaying()
{
	return playing;
}