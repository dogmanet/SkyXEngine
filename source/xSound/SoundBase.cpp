
#include "SoundBase.h"

//##########################################################################

SOUND_DTYPE XMETHODCALLTYPE CSoundBase::getType()
{
	return m_dtype;
}

void XMETHODCALLTYPE CSoundBase::setVolume(float fVolume)
{
	if (!m_pAB)
		return;

	m_pAB->setVolume(fVolume);
}

float XMETHODCALLTYPE CSoundBase::getVolume() const
{
	if (!m_pAB)
		return 0.f;

	return m_pAB->getVolume();
}

void XMETHODCALLTYPE CSoundBase::setPan(float fPan)
{
	if (!m_pAB || m_dtype == SOUND_DTYPE_3D)
		return;

	m_pAB->setPan(fPan);
}

float XMETHODCALLTYPE CSoundBase::getPan() const
{
	if (!m_pAB || m_dtype == SOUND_DTYPE_3D)
		return 0.f;

	return m_pAB->getPan();
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
