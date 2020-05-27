
#include "SoundPlayer.h"

#include "SoundLoader.h"
#include "SoundLayer.h"

//##########################################################################

CSoundPlayer::~CSoundPlayer()
{
	mem_release_del(m_pAB);

	mem_delete(m_pCodecTarget);
	mem_delete(m_pStream);
}

//**************************************************************************

CSoundPlayer* CSoundPlayer::newInstance()
{
	if (m_pStream)
		return NULL;

	CSoundPlayer *pPlayer = new CSoundPlayer();

	pPlayer->m_pAB = m_pAB->newInstance();
	pPlayer->m_uLengthBytes = this->m_uLengthBytes;
	pPlayer->m_fLengthMls = this->m_fLengthMls;
	pPlayer->m_loop = this->m_loop;
	pPlayer->m_dtype = this->m_dtype;
	pPlayer->m_state = SOUND_STATE_STOP;
	pPlayer->m_sName = this->m_sName;
	pPlayer->m_pLayer = this->m_pLayer;
	return pPlayer;
}

//**************************************************************************

bool CSoundPlayer::create(CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget)
{
	if (!pCodecTarget || !pLayer)
		return false;

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

	setTime(0);

	return true;
}

//##########################################################################

void XMETHODCALLTYPE CSoundPlayer::play()
{
	if (!m_pLayer->isPlaying())
		return;

	m_pAB->play(true);
	m_state = SOUND_STATE_PLAY;
}

//**************************************************************************

void XMETHODCALLTYPE CSoundPlayer::resume()
{
	if (!m_pLayer->isPlaying() || m_state != SOUND_STATE_PAUSE)
		return;

	m_pAB->play(true);
}

//**************************************************************************

void XMETHODCALLTYPE CSoundPlayer::pause()
{
	m_pAB->play(false);
	m_state = SOUND_STATE_PAUSE;
}

//**************************************************************************

void XMETHODCALLTYPE CSoundPlayer::stop()
{
	m_pAB->play(false);
	setTime(0.f);
	m_state = SOUND_STATE_STOP;
}

//**************************************************************************

bool XMETHODCALLTYPE CSoundPlayer::isPlaying() const
{
	return m_pAB->isPlaying();
}

//##########################################################################

void XMETHODCALLTYPE CSoundPlayer::setLoop(SOUND_LOOP loop)
{
	m_loop = loop;
	if (!m_pStream)
		m_pAB->setLoop((AB_LOOP)m_loop);
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

//**************************************************************************

float XMETHODCALLTYPE CSoundPlayer::getLength() const
{
	return double(m_fLengthMls) / 1000.f;
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
	m_pStream->uNumLoader = uParts * uCountParts + uCurrPart;

	uint32_t uPosLoader = uPos - (uCurrPart * m_pStream->uPartSize);
	m_pCodecTarget->setPos(uPosLoader);
	for (int i = 0; i < STREAM_DATA_COUNT_PARTS; ++i)
	{
		if (i >= uCurrPart)
		{
			size_t sizeRead = m_pCodecTarget->decode(m_pCodecTarget->getPos(), m_pStream->oData.uSize, (void**)&(m_pStream->oData.pData));
			if (sizeRead <= 0)
				continue;

			BYTE *pData = 0;
			uint32_t uLocked = m_pAB->lock(AB_LOCK_WRITE, (void**)&pData, m_pStream->aParts[i].uStart, m_pStream->aParts[i].uEnd - m_pStream->aParts[i].uStart);
			memcpy(pData, m_pStream->oData.pData, sizeRead);
			m_pAB->unlock();

			m_pStream->aParts[i].isLoaded = true;
			++(m_pStream->uNumLoader);
		}
		else
		{
			m_pStream->aParts[i].isPlay = false;
			m_pStream->aParts[i].isLoaded = false;
		}
	}

	int qwerty = 0;
}

//**************************************************************************

uint32_t CSoundPlayer::getPosBytes() const
{
	uint32_t uPos = m_pAB->getPos();

	if (m_pStream)
		uPos = (uint64_t(uPos) + uint64_t(m_pStream->uPartSize) * uint64_t(m_pStream->uNumLoader));

	return uPos;
}

//##########################################################################

uint32_t uMax = 0;

void CSoundPlayer::update()
{
	if (!m_pStream)
		return;

	uint32_t uGPosBytes = getPosBytes();
	uint32_t uGPosBytes2 = m_pCodecTarget->getPos();

	if (uMax < uGPosBytes)
	{
		uMax = uGPosBytes;
		//printf("uMax = %d\n", uMax);
	}

	if (uGPosBytes >= m_uLengthBytes)
	{
		if (m_loop == AB_LOOP_NONE)
			m_pAB->play(false);
		else
		{
			m_pCodecTarget->setPos(0);
			m_pStream->uNumLoader = 0;
		}
	}

	if (!isPlaying())
		return;

	uint32_t uPos = m_pAB->getPos();

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
				if (sizeRead == 0)
					continue;

				BYTE *pData = 0;
				uint32_t uLocked = m_pAB->lock(AB_LOCK_WRITE, (void**)&pData, m_pStream->aParts[i].uStart, m_pStream->aParts[i].uEnd - m_pStream->aParts[i].uStart);
				memset(pData, 0, uLocked);
				memcpy(pData, m_pStream->oData.pData, sizeRead);
				m_pAB->unlock();

				m_pStream->aParts[i].isLoaded = true;
				++(m_pStream->uNumLoader);
			}
		}
	}
}
