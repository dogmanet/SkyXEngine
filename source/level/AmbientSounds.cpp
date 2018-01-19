
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "AmbientSounds.h"

CAmbientSounds::CAmbientSounds()
{
	m_isPlaying = false;
	m_iPlayingLast = 0;
}

CAmbientSounds::~CAmbientSounds()
{
	clear();
}

void CAmbientSounds::add(const char *szPath)
{
	ID id = SSCore_SndCreate2d(szPath);
	if (id >= 0)
		m_aIDSnds.push_back(id);
}

void CAmbientSounds::get(ID id, char *szPath)
{
	if (id >= 0 && id < m_aIDSnds.size())
	{
		SSCore_SndFileGet(m_aIDSnds[id], szPath);
	}
}

UINT CAmbientSounds::getCount()
{
	return m_aIDSnds.size();
}

void CAmbientSounds::clear()
{
	for (int i = 0; i < m_aIDSnds.size(); ++i)
	{
		SSCore_SndDelete(m_aIDSnds[i]);
	}

	m_aIDSnds.clear();
}

void CAmbientSounds::play()
{
	if (m_iPlayingLast >= 0 && m_iPlayingLast < m_aIDSnds.size())
	{
		SSCore_SndVolumeSet(m_aIDSnds[m_iPlayingLast], 100);
		SSCore_SndPlay(m_aIDSnds[m_iPlayingLast]);
		m_isPlaying = true;
	}
}

void CAmbientSounds::update()
{
	if (!m_isPlaying)
		return;

	static const float * env_ambient_snd_volume = GET_PCVAR_FLOAT("env_ambient_snd_volume");
	static float env_ambient_snd_volume_old = 1.f;

	if (SSCore_SndStateGet(m_aIDSnds[m_iPlayingLast]) != SOUND_OBJSTATE_PLAY)
	{
		if (m_iPlayingLast + 1 < m_aIDSnds.size())
			++m_iPlayingLast;
		else
			m_iPlayingLast = 0;

		SSCore_SndVolumeSet(m_aIDSnds[m_iPlayingLast], env_ambient_snd_volume_old*100.f);
		SSCore_SndPlay(m_aIDSnds[m_iPlayingLast]);
	}

	if (env_ambient_snd_volume && env_ambient_snd_volume_old != (*env_ambient_snd_volume))
	{
		env_ambient_snd_volume_old = *env_ambient_snd_volume;
		if (m_iPlayingLast >= 0)
			SSCore_SndVolumeSet(m_aIDSnds[m_iPlayingLast], env_ambient_snd_volume_old*100.f);
	}
}

void CAmbientSounds::pause()
{
	if (!m_isPlaying)
		return;

	SSCore_SndPause(m_aIDSnds[m_iPlayingLast]);

	m_isPlaying = false;
}

bool CAmbientSounds::getPlaying()
{
	return m_isPlaying;
}