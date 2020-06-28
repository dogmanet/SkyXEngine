
#include "SoundBase.h"

#include "SoundLayer.h"

//##########################################################################

SOUND_TYPE XMETHODCALLTYPE CSoundBase::getType() const
{
	return m_type;
}

SOUND_SPACE XMETHODCALLTYPE CSoundBase::getSpace() const
{
	return m_space;
}

void XMETHODCALLTYPE CSoundBase::setSpace(SOUND_SPACE space)
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_SPACE;
	oMsg.arg.space = space;
	if (m_type == SOUND_TYPE_EMITTER)
		oMsg.pEmitter = (CSoundEmitter*)this;
	else
		oMsg.pPlayer = (CSoundPlayer*)this;
	m_pLayer->addMessage(oMsg);
}

const char* XMETHODCALLTYPE CSoundBase::getName() const
{
	return m_sName.c_str();
}

void XMETHODCALLTYPE CSoundBase::setVolume(float fVolume)
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_VOLUME;
	oMsg.arg.f = fVolume;
	if (m_type == SOUND_TYPE_EMITTER)
		oMsg.pEmitter = (CSoundEmitter*)this;
	else
		oMsg.pPlayer = (CSoundPlayer*)this;
	m_pLayer->addMessage(oMsg);
}

float XMETHODCALLTYPE CSoundBase::getVolume() const
{
	return m_fVolume;
}

void XMETHODCALLTYPE CSoundBase::setPan(float fPan)
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_PAN;
	oMsg.arg.f = fPan;
	if (m_type == SOUND_TYPE_EMITTER)
		oMsg.pEmitter = (CSoundEmitter*)this;
	else
		oMsg.pPlayer = (CSoundPlayer*)this;
	m_pLayer->addMessage(oMsg);
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
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_POS;
	oMsg.arg.vector[0] = vPos.x;
	oMsg.arg.vector[1] = vPos.y;
	oMsg.arg.vector[2] = vPos.z;
	if (m_type == SOUND_TYPE_EMITTER)
		oMsg.pEmitter = (CSoundEmitter*)this;
	else
		oMsg.pPlayer = (CSoundPlayer*)this;
	m_pLayer->addMessage(oMsg);
}

float XMETHODCALLTYPE CSoundBase::getDistance() const
{
	return m_fDist;
}

void XMETHODCALLTYPE CSoundBase::setDistance(float fDist)
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_DIST;
	oMsg.arg.f = fDist;
	if (m_type == SOUND_TYPE_EMITTER)
		oMsg.pEmitter = (CSoundEmitter*)this;
	else
		oMsg.pPlayer = (CSoundPlayer*)this;
	m_pLayer->addMessage(oMsg);
}

//##########################################################################

void CSoundBase::_setSpace(SOUND_SPACE space)
{
	m_space = space;
}

void CSoundBase::_setVolume(float fVolume)
{
	m_fVolume = fVolume;
}

void CSoundBase::_setPan(float fPan)
{
	m_fPan = fPan;
}

void CSoundBase::_setWorldPos(const float3 &vPos)
{
	m_vWorldPos = vPos;
}

void CSoundBase::_setDistance(float fDist)
{
	m_fDist = fDist;
}
