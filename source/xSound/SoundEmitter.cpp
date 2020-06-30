
#include "SoundEmitter.h"

#include "SoundLoader.h"
#include "SoundLayer.h"

//##########################################################################

CSoundEmitter::~CSoundEmitter()
{
	for (auto i = 0u, il = m_aInstances.size(); i < il; ++i)
	{
		mem_delete(m_aInstances[i]);
	}

	mem_release(m_pLayer);
}

void XMETHODCALLTYPE CSoundEmitter::FinalRelease()
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_DELETE;
	oMsg.pEmitter = this;
	oMsg.pOwner = m_pLayer;
	m_pLayer->addMessage(oMsg);
}

//##########################################################################

bool CSoundEmitter::canInstance() const
{
	return true;
}

//**************************************************************************

CSoundBase* CSoundEmitter::newInstance()
{
	CSoundEmitter *pEmitter = new CSoundEmitter();

	pEmitter->m_aInstances[0]->pAB = m_aInstances[0]->pAB->newInstance();
	pEmitter->m_space = this->m_space;
	pEmitter->m_state = SOUND_STATE_STOP;
	pEmitter->m_sName = this->m_sName;
	pEmitter->m_pLayer = this->m_pLayer;
	pEmitter->m_fDist = this->m_fDist;
	pEmitter->m_vWorldPos = this->m_vWorldPos;
	return pEmitter;
}

//##########################################################################

void XMETHODCALLTYPE CSoundEmitter::play()
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_PLAY;
	oMsg.arg.state = SOUND_STATE_PLAY;
	oMsg.pEmitter = this;
	m_pLayer->addMessage(oMsg);
}

//**************************************************************************

void CSoundEmitter::resume()
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_RESUME;
	oMsg.pEmitter = this;
	m_pLayer->addMessage(oMsg);
}

void CSoundEmitter::pause()
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_PLAY;
	oMsg.arg.state = SOUND_STATE_PAUSE;
	oMsg.pEmitter = this;
	m_pLayer->addMessage(oMsg);
}

//**************************************************************************

bool CSoundEmitter::create(const char* szName, CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget, SOUND_SPACE space)
{
	if (!pCodecTarget || !pLayer)
		return false;

	m_sName = szName;
	m_space = space;
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

	m_aInstances.push_back(new Instance(pAB));
	pLayer->AddRef();

	return true;
}

//##########################################################################

void CSoundEmitter::update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp)
{
	m_vListenerPos = vListenerPos;
	m_vListenerDir = vListenerDir;
	m_vListenerUp = vListenerUp;

	if (m_space == SOUND_SPACE_2D)
		return;

	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if (m_aInstances[i]->pAB->isPlaying())
		{
			Com3D(m_aInstances[i]->pAB, m_fDist, m_fVolume, m_vWorldPos, vListenerPos, vListenerDir, vListenerUp);
		}
	}
}

//##########################################################################

void CSoundEmitter::_play()
{
	//если родительский слой не проигрывается, тогда не запускаем проигрывание
	if (!m_pLayer->isPlaying())
		return;

	IAudioBuffer *pAB = NULL;

	//проход по массиву инстансов звука, если есть первый попавшийся не проигрываемые тогда его проигрываем
	for (int i = 0, il = m_aInstances.size(); i<il; ++i)
	{
		if (!(m_aInstances[i]->pAB->isPlaying()))
		{
			pAB = m_aInstances[i]->pAB;
			break;
		}
	}

	if (!pAB)
	{
		//если пришли сюда, значит нет свободных инстансов, создаем новый и проигрыаем
		IAudioBuffer *pInst = m_aInstances[0]->pAB->newInstance();
		pAB = pInst;
		m_aInstances.push_back(new Instance(pInst));
	}

	Com3D(pAB, m_fDist, m_fVolume, m_vWorldPos, m_vListenerPos, m_vListenerDir, m_vListenerUp);
	pAB->play(true);
}

//**************************************************************************

void CSoundEmitter::_resume()
{
	//если инстансы проигрывались тогда включаем проигрывание
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if (m_aInstances[i]->isPlaying)
			m_aInstances[i]->pAB->play(true);
	}
}

//**************************************************************************

void CSoundEmitter::_pause()
{
	//записываем состояния инстансов и останавливаем
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		m_aInstances[i]->isPlaying = m_aInstances[i]->pAB->isPlaying();
		m_aInstances[i]->pAB->play(false);
	}
}

//**************************************************************************

void CSoundEmitter::_setSpace(SOUND_SPACE space)
{
	m_space = space;
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if (m_aInstances[i]->pAB->isPlaying())
		{
			m_aInstances[i]->pAB->setPan(m_fPan);
			m_aInstances[i]->pAB->setVolume(m_fVolume);
		}
	}
}

//**************************************************************************

void CSoundEmitter::_setVolume(float fVolume)
{
	m_fVolume = fVolume;
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if (m_aInstances[i]->pAB->isPlaying())
			m_aInstances[i]->pAB->setVolume(m_fVolume);
	}
}

//**************************************************************************

void CSoundEmitter::_setPan(float fPan)
{
	m_fPan = fPan;
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if (m_aInstances[i]->pAB->isPlaying())
			m_aInstances[i]->pAB->setPan(m_fPan);
	}
}

//**************************************************************************

void CSoundEmitter::_setWorldPos(const float3 &vPos)
{
	m_vWorldPos = vPos;
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if (m_aInstances[i]->pAB->isPlaying())
			Com3D(m_aInstances[i]->pAB, m_fDist, m_fVolume, m_vWorldPos, m_vListenerPos, m_vListenerDir, m_vListenerUp);
	}
}

//**************************************************************************

void CSoundEmitter::_setDistance(float fDist)
{
	m_fDist = fDist;
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if (m_aInstances[i]->pAB->isPlaying())
			Com3D(m_aInstances[i]->pAB, m_fDist, m_fVolume, m_vWorldPos, m_vListenerPos, m_vListenerDir, m_vListenerUp);
	}
}
