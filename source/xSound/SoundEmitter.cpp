
#include "SoundEmitter.h"

#include "SoundLoader.h"
#include "SoundLayer.h"

//##########################################################################

CSoundEmitter::~CSoundEmitter()
{
	for (int i = 0, il = m_aInstances.size(); i<il; ++i)
	{
		m_aInstances[i]->Release();
	}

	m_pAB->Release();
}

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

bool CSoundEmitter::create(const char *szPath, CSoundLayer *pLayer, CSoundSystem *pSoundSystem)
{
	if (!pSoundSystem || !pLayer)
		return false;

	CSoundLoader *pLoader = new CSoundLoader();
	pLoader->init(pSoundSystem);
	m_pSoundSystem = pSoundSystem;
	AudioRawDesc oDesc;
	pLoader->load(szPath, &oDesc);
	m_pLayer = pLayer;

	m_pAB = pLayer->createAudioBuffer(AB_TYPE_SECOND, &oDesc);
	BYTE *pData = new BYTE[oDesc.uSize];
	pLoader->getPCM((void**)&pData, oDesc.uSize, 0);

	BYTE *pData2;
	m_pAB->lock(AB_LOCK_WRITE, (void**)&pData2);
	memcpy(pData2, pData, oDesc.uSize);
	m_pAB->unlock();

	mem_delete_a(pData);
	mem_delete(pLoader);

	m_pAB->setLoop(AB_LOOP_NONE);

	return true;
}
