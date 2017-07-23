
#include <managed_render/level_ambient_sounds.h>

LevelAmbientSounds::LevelAmbientSounds()
{
	playing = false;
	PlayingLast = 0;
}

LevelAmbientSounds::~LevelAmbientSounds()
{
	Clear();
}

void LevelAmbientSounds::Add(const char* path)
{
	ID id = SSCore_SndCreate2d(path);
	if (id >= 0)
		ArrIDsSnds.push_back(id);
}

void LevelAmbientSounds::Get(ID id, char* path)
{
	if (id >= 0 && id < ArrIDsSnds.size())
	{
		SSCore_SndFileGet(ArrIDsSnds[id], path);
	}
}

UINT LevelAmbientSounds::GetCount()
{
	return ArrIDsSnds.size();
}

void LevelAmbientSounds::Clear()
{
	for (int i = 0; i < ArrIDsSnds.size(); ++i)
	{
		SSCore_SndDelete(ArrIDsSnds[i]);
	}

	ArrIDsSnds.clear();
}

void LevelAmbientSounds::Play()
{
	if (PlayingLast >= 0 && PlayingLast < ArrIDsSnds.size())
	{
		SSCore_SndVolumeSet(ArrIDsSnds[PlayingLast], 100);
		SSCore_SndPlay(ArrIDsSnds[PlayingLast]);
		playing = true;
	}
}

void LevelAmbientSounds::Update()
{
	if (!playing)
		return;

	if (SSCore_SndStateGet(ArrIDsSnds[PlayingLast]) != SoundObjState::sos_play)
	{
		if (PlayingLast + 1 < ArrIDsSnds.size())
			++PlayingLast;
		else
			PlayingLast = 0;

		SSCore_SndVolumeSet(ArrIDsSnds[PlayingLast], 100);
		SSCore_SndPlay(ArrIDsSnds[PlayingLast]);
	}
}

void LevelAmbientSounds::Pause()
{
	if (!playing)
		return;

	SSCore_SndPause(ArrIDsSnds[PlayingLast]);

	playing = false;
}

bool LevelAmbientSounds::IsPlaying()
{
	return playing;
}