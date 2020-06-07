
#include "SoundEmitter.h"

#include "SoundLoader.h"
#include "SoundLayer.h"

//##########################################################################

CSoundEmitter::~CSoundEmitter()
{
	m_pLayer->delSndEmitter(this);

}

//**************************************************************************

CSoundEmitter* CSoundEmitter::newInstance()
{
	CSoundEmitter *pEmitter = new CSoundEmitter();

	pEmitter->m_aInstances[0].pAB = m_aInstances[0].pAB->newInstance();
	pEmitter->m_dtype = this->m_dtype;
	pEmitter->m_state = SOUND_STATE_STOP;
	pEmitter->m_sName = this->m_sName;
	pEmitter->m_pLayer = this->m_pLayer;
	pEmitter->m_fDist = this->m_fDist;
	pEmitter->m_vWorldPos = this->m_vWorldPos;
	return pEmitter;
}

//**************************************************************************

void XMETHODCALLTYPE CSoundEmitter::play()
{
	//если родительский слой не проигрывается, тогда не запускаем проигрывание
	if (!m_pLayer->isPlaying())
		return;

	//проход по массиву инстансов звука, если есть первый попавшийся не проигрываемые тогда его проигрываем
	for (int i = 0, il = m_aInstances.size(); i<il; ++i)
	{
		if (!(m_aInstances[i].pAB->isPlaying()))
		{
			m_aInstances[i].pAB->setVolume(m_fVolume);
			m_aInstances[i].pAB->play(true);
			return;
		}
	}

	//если пришли сюда, значит нет свободных инстансов, создаем новый и проигрыаем
	IAudioBuffer *pInst = m_aInstances[0].pAB->newInstance();
	pInst->setVolume(m_fVolume);
	pInst->play(true);
	m_aInstances.push_back(pInst);
}

//**************************************************************************

void CSoundEmitter::resume()
{
	//если инстансы проигрывались тогда включаем проигрывание
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if(m_aInstances[i].isPlaying)
			m_aInstances[i].pAB->play(true);
	}
}

void CSoundEmitter::pause()
{
	//записываем состояния инстансов и останавливаем
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		m_aInstances[i].isPlaying = m_aInstances[i].pAB->isPlaying();
		m_aInstances[i].pAB->play(false);
	}
}

//**************************************************************************

bool CSoundEmitter::create(CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget)
{
	if (!pCodecTarget || !pLayer)
		return false;

	AudioRawDesc oDesc;
	pCodecTarget->getDesc(&oDesc);
	m_pLayer = pLayer;

	IAudioBuffer *pAB = pLayer->createAudioBuffer(AB_TYPE_SECOND, &oDesc);
	BYTE *pData = new BYTE[oDesc.uSize];
	pCodecTarget->decode(0, oDesc.uSize, (void**)&pData);

	BYTE *pData2;
	pAB->lock(AB_LOCK_WRITE, (void**)&pData2);
	memcpy(pData2, pData, oDesc.uSize);
	pAB->unlock();

	mem_delete_a(pData);
	mem_release(pCodecTarget);

	pAB->setLoop(AB_LOOP_NONE);

	m_aInstances.push_back(Instance(pAB));

	return true;
}
