
#include "SoundPlayer.h"

#include "SoundLoader.h"
#include "SoundLayer.h"

//##########################################################################

CSoundPlayer::~CSoundPlayer()
{
	mem_release(m_pAB);

	mem_delete(m_pCodecTarget);
	mem_delete(m_pStream);

	mem_release(m_pLayer);
}

//**************************************************************************

void XMETHODCALLTYPE CSoundPlayer::FinalRelease()
{ 
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_DELETE;
	oMsg.pPlayer = this;
	oMsg.pOwner = m_pLayer;
	m_pLayer->addMessage(oMsg);
}

//**************************************************************************

bool CSoundPlayer::canInstance() const
{
	return (!m_pStream);
}

//**************************************************************************

CSoundBase* CSoundPlayer::newInstance()
{
	if (m_pStream)
		return NULL;

	CSoundPlayer *pPlayer = new CSoundPlayer();

	pPlayer->m_pAB = m_pAB->newInstance();
	pPlayer->m_uLengthBytes = this->m_uLengthBytes;
	pPlayer->m_fLengthMls = this->m_fLengthMls;
	pPlayer->m_loop = this->m_loop;
	pPlayer->m_space = this->m_space;
	pPlayer->m_state = SOUND_STATE_STOP;
	pPlayer->m_sName = this->m_sName;
	pPlayer->m_pLayer = this->m_pLayer;
	pPlayer->m_fDist = this->m_fDist;
	pPlayer->m_vWorldPos = this->m_vWorldPos;
	pPlayer->m_sName = this->m_sName;
	return pPlayer;
}

//**************************************************************************

bool CSoundPlayer::create(const char* szName, CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget, SOUND_SPACE space)
{
	if (!pCodecTarget || !pLayer)
		return false;

	m_sName = szName;
	m_space = space;
	m_pLayer = pLayer;
	m_pCodecTarget = pCodecTarget;
	AudioRawDesc oDesc;
	pCodecTarget->getDesc(&oDesc);
	m_uLengthBytes = oDesc.uSize;
	m_fLengthMls = (uint32_t)(double(oDesc.uSize * 1000) / double(oDesc.uBytesPerSec));
	
	uint32_t uChunkSize = m_pLayer->getStreamChunkSize(&oDesc);


	if (oDesc.uSize / uChunkSize > STREAM_COUNT_MIN_PARTS)
	{
		oDesc.uSize = uChunkSize * 4;
		m_pAB = m_pLayer->createAudioBuffer(AB_TYPE_SECOND, &oDesc);
		m_pStream = new CStreamData();
		m_pStream->uPartSize = oDesc.uSize / STREAM_DATA_COUNT_PARTS;
		m_pStream->oData.uSize = m_pStream->uPartSize;
		m_pStream->oData.pData = new BYTE[m_pStream->oData.uSize];
		for (int i = 0; i < STREAM_DATA_COUNT_PARTS; ++i)
		{
			m_pStream->aParts[i].uStart = i * m_pStream->uPartSize;
			m_pStream->aParts[i].uEnd = m_pStream->aParts[i].uStart + m_pStream->uPartSize;
		}
		m_pAB->setLoop(AB_LOOP_SIMPLE);
	}
	else
	{
		m_pAB = m_pLayer->createAudioBuffer(AB_TYPE_SECOND, &oDesc);
		BYTE *pData = new BYTE[oDesc.uSize];
		m_pCodecTarget->decode(0, oDesc.uSize, (void**)&pData);

		BYTE *pData2;
		m_pAB->lock(AB_LOCK_WRITE, (void**)&pData2);
		memcpy(pData2, pData, oDesc.uSize);
		m_pAB->unlock();

		mem_delete_a(pData);
	}

	m_pAB->play(false);
	_setTime(0);
	pLayer->AddRef();

	return true;
}

//##########################################################################

void XMETHODCALLTYPE CSoundPlayer::play()
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_PLAY;
	oMsg.arg.state = SOUND_STATE_PLAY;
	oMsg.pPlayer = this;
	m_pLayer->addMessage(oMsg);
}

//**************************************************************************

void XMETHODCALLTYPE CSoundPlayer::resume()
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_RESUME;
	oMsg.pPlayer = this;
	m_pLayer->addMessage(oMsg);
}

//**************************************************************************

void XMETHODCALLTYPE CSoundPlayer::pause()
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_PLAY;
	oMsg.arg.state = SOUND_STATE_PAUSE;
	oMsg.pPlayer = this;
	m_pLayer->addMessage(oMsg);
}

//**************************************************************************

void XMETHODCALLTYPE CSoundPlayer::stop()
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_PLAY;
	oMsg.arg.state = SOUND_STATE_STOP;
	oMsg.pPlayer = this;
	m_pLayer->addMessage(oMsg);
}

//**************************************************************************

bool XMETHODCALLTYPE CSoundPlayer::isPlaying() const
{
	return m_pAB->isPlaying();
}

//##########################################################################

void XMETHODCALLTYPE CSoundPlayer::setLoop(SOUND_LOOP loop)
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_LOOP;
	oMsg.arg.loop = loop;
	oMsg.pPlayer = this;
	m_pLayer->addMessage(oMsg);
}

//**************************************************************************

SOUND_LOOP XMETHODCALLTYPE CSoundPlayer::getLoop() const
{
	return m_loop;
}

//##########################################################################

float XMETHODCALLTYPE CSoundPlayer::getTime() const
{
	uint32_t uPos = getPosBytes();

	if (m_pStream)
	{
		AudioRawDesc oDesc;
		m_pAB->getDesc(&oDesc);
		uPos = (uint64_t(uPos) * uint64_t(1000)) / uint64_t(oDesc.uBytesPerSec);
	}

	return float(uPos) / 1000.f;
}

//**************************************************************************

void XMETHODCALLTYPE CSoundPlayer::setTime(float fTime)
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_TIME;
	oMsg.arg.f = fTime;
	m_pLayer->addMessage(oMsg);
}

//**************************************************************************

float XMETHODCALLTYPE CSoundPlayer::getLength() const
{
	return float(m_fLengthMls) / 1000.f;
}

//##########################################################################

void CSoundPlayer::setPosStream(uint32_t uPos)
{
	uint32_t uCountParts = STREAM_DATA_COUNT_PARTS;
	uint32_t uParts = uPos / (m_pStream->uPartSize * uCountParts);
	if (uParts * m_pStream->uPartSize * uCountParts > uPos)
		--uParts;

	uint32_t uPosAB = (uPos - (uParts * m_pStream->uPartSize * uCountParts));
	uint32_t uCurrPart = uPosAB / m_pStream->uPartSize;
	m_pStream->uCountPlayed = uParts * uCountParts + uCurrPart;

	uint32_t uPosLoader = uPos - (uCurrPart * m_pStream->uPartSize);
	m_pCodecTarget->setPos(uPosLoader);
	for (uint32_t i = 0; i < STREAM_DATA_COUNT_PARTS; ++i)
	{
		if (i >= uCurrPart)
		{
			size_t sizeRead = m_pCodecTarget->decode(m_pCodecTarget->getPos(), m_pStream->oData.uSize, (void**)&(m_pStream->oData.pData));
			if (sizeRead <= 0)
				continue;

			BYTE *pData = 0;
			size_t sizeLocked = m_pAB->lock(AB_LOCK_WRITE, (void**)&pData, m_pStream->aParts[i].uStart, m_pStream->aParts[i].uEnd - m_pStream->aParts[i].uStart);
			memcpy(pData, m_pStream->oData.pData, sizeRead);
			m_pAB->unlock();

			m_pStream->aParts[i].isLoaded = true;
		}
		else
		{
			m_pStream->aParts[i].isPlay = false;
			m_pStream->aParts[i].isLoaded = false;
		}
	}

	m_pAB->setPos(0);
	int qwerty = 0;
}

//**************************************************************************

uint32_t CSoundPlayer::getPosBytes() const
{
	size_t uPos = m_pAB->getPos();

	if (m_pStream)
		uPos = (uint64_t(uPos) + uint64_t(m_pStream->uPartSize) * uint64_t(m_pStream->uCountPlayed));

	return uPos;
}

//##########################################################################

void CSoundPlayer::update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp)
{
	m_vListenerPos = vListenerPos;
	m_vListenerDir = vListenerDir;
	m_vListenerUp = vListenerUp;


	if (!m_pLayer->isPlaying())
		return;

	if (m_space == SOUND_SPACE_3D)
		Com3D(m_pAB, m_fDist, m_fVolume, m_vWorldPos, vListenerPos, vListenerDir, vListenerUp);
	else
		int qwerty = 0;

	if (!m_pStream)
		return;

	uint32_t uGPosBytes = getPosBytes();
	//uint32_t uGPosBytes2 = m_pCodecTarget->getPos();
	float fPos = getTime();

	if (uGPosBytes >= m_uLengthBytes)
	{
		if (m_loop == AB_LOOP_NONE)
			m_pAB->play(false);
		else
			_setTime(0.f);
	}

	if (!isPlaying())
		return;

	size_t uPos = m_pAB->getPos();

	for (int i = 0; i < STREAM_DATA_COUNT_PARTS; ++i)
	{
		if (m_pStream->aParts[i].uStart <= uPos && uPos <= m_pStream->aParts[i].uEnd)
			m_pStream->aParts[i].isPlay = true;
		else
		{
			if (m_pStream->aParts[i].isPlay)
			{
				m_pStream->aParts[i].isPlay = false;
				m_pStream->aParts[i].isLoaded = false;

				size_t sizeRead = m_pCodecTarget->decode(m_pCodecTarget->getPos(), m_pStream->oData.uSize, (void**)&(m_pStream->oData.pData));
				++(m_pStream->uCountPlayed);
				if (sizeRead == 0)
 					continue;

				BYTE *pData = 0;
				size_t sizeLocked = m_pAB->lock(AB_LOCK_WRITE, (void**)&pData, m_pStream->aParts[i].uStart, m_pStream->aParts[i].uEnd - m_pStream->aParts[i].uStart);
				memset(pData, 0, sizeLocked);
				memcpy(pData, m_pStream->oData.pData, sizeRead);
				m_pAB->unlock();

				m_pStream->aParts[i].isLoaded = true;
			}
		}
	}
}

//##########################################################################
//##########################################################################

void CSoundPlayer::_setSpace(SOUND_SPACE space)
{
	m_pAB->setPan(m_fPan);
	m_pAB->setVolume(m_fVolume);
	m_space = space;
}

//**************************************************************************

void CSoundPlayer::_play()
{
	if (!m_pLayer->isPlaying())
	{
		m_state = SOUND_STATE_PAUSE;
		return;
	}

	if (m_space == SOUND_SPACE_3D)
		Com3D(m_pAB, m_fDist, m_fVolume, m_vWorldPos, m_vListenerPos, m_vListenerDir, m_vListenerUp);
	m_pAB->play(true);
	m_state = SOUND_STATE_PLAY;
}

//**************************************************************************

void CSoundPlayer::_resume()
{
	if (!m_pLayer->isPlaying() || m_state != SOUND_STATE_PAUSE)
		return;

	m_pAB->play(true);
}

//**************************************************************************

void CSoundPlayer::_pause()
{
	m_pAB->play(false);
	m_state = SOUND_STATE_PAUSE;
}

//**************************************************************************

void CSoundPlayer::_stop()
{
	m_pAB->play(false);
	_setTime(0.f);
	m_state = SOUND_STATE_STOP;
}

//**************************************************************************

void CSoundPlayer::_setLoop(SOUND_LOOP loop)
{
	m_loop = loop;
	if (!m_pStream)
		m_pAB->setLoop((AB_LOOP)m_loop);
}

//**************************************************************************

void CSoundPlayer::_setTime(float fTime)
{
	AudioRawDesc oDesc;
	m_pAB->getDesc(&oDesc);
	uint32_t uPosBytes = (uint64_t(fTime * 1000.f) * uint64_t(oDesc.uBytesPerSec)) / uint64_t(1000);

	if (m_uLengthBytes < uPosBytes)
		return;

	if (m_pStream)
		setPosStream(uPosBytes);
	else
		m_pAB->setPos(uPosBytes);
}
