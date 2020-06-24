
#include "SoundBase.h"

//##########################################################################

SOUND_DTYPE XMETHODCALLTYPE CSoundBase::getType() const
{
	return m_dtype;
}

void XMETHODCALLTYPE CSoundBase::setType(SOUND_DTYPE dtype)
{
	m_dtype = dtype;
}

const char* XMETHODCALLTYPE CSoundBase::getName() const
{
	return m_sName.c_str();
}

void XMETHODCALLTYPE CSoundBase::setVolume(float fVolume)
{
	m_fVolume = fVolume;
}

float XMETHODCALLTYPE CSoundBase::getVolume() const
{
	return m_fVolume;
}

void XMETHODCALLTYPE CSoundBase::setPan(float fPan)
{
	m_fPan = fPan;
}

float XMETHODCALLTYPE CSoundBase::getPan() const
{
	return m_fPan;
}

const float3& XMETHODCALLTYPE CSoundBase::getWorldPos() const
{
	return m_vWorldPos;
}

void XMETHODCALLTYPE CSoundBase::setWorldPos(const float3 &vPos)
{
	m_vWorldPos = vPos;
}

float XMETHODCALLTYPE CSoundBase::getDistance() const
{
	return m_fDist;
}

void XMETHODCALLTYPE CSoundBase::setDistance(float fDist)
{
	m_fDist = fDist;
}
