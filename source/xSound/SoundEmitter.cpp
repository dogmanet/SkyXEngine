
#include "SoundEmitter.h"

#include "SoundLoader.h"
#include "SoundLayer.h"

//##########################################################################

CSoundEmitter::~CSoundEmitter()
{
	for (int i = 0, il = m_aInstances.size(); i<il; ++i)
	{
		mem_release(m_aInstances[i]);
	}

	mem_release(m_pAB);
}

//**************************************************************************

void XMETHODCALLTYPE CSoundEmitter::play()
{
	if (!(m_pAB->isPlaying()))
	{
		m_pAB->play(true);
		return;
	}

	for (int i = 0, il = m_aInstances.size(); i<il; ++i)
	{
		if (!(m_aInstances[i]->isPlaying()))
		{
			m_aInstances[i]->play(true);
			return;
		}
	}

	IAudioBuffer *pInst = m_pAB->newInstance();
	pInst->play(true);
	m_aInstances.push_back(pInst);
}

//**************************************************************************

bool CSoundEmitter::create(CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget)
{
	if (!pCodecTarget || !pLayer)
		return false;

	AudioRawDesc oDesc;
	pCodecTarget->getDesc(&oDesc);
	m_pLayer = pLayer;

	m_pAB = pLayer->createAudioBuffer(AB_TYPE_SECOND, &oDesc);
	BYTE *pData = new BYTE[oDesc.uSize];
	pCodecTarget->decode(0, oDesc.uSize, (void**)&pData);

	BYTE *pData2;
	m_pAB->lock(AB_LOCK_WRITE, (void**)&pData2);
	memcpy(pData2, pData, oDesc.uSize);
	m_pAB->unlock();

	mem_delete_a(pData);
	mem_release(pCodecTarget);

	m_pAB->setLoop(AB_LOOP_NONE);

	return true;
}
