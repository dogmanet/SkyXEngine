
#include "SoundEmitter.h"

#include "SoundLoader.h"
#include "SoundLayer.h"

//##########################################################################

CSoundEmitter::~CSoundEmitter()
{
	m_pLayer->delSndEmitter(this);

	for (int i = 0; i < m_aInstances.size(); ++i)
	{
		mem_delete(m_aInstances[i]);
	}
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
	pEmitter->m_dtype = this->m_dtype;
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
	//если родительский слой не проигрывается, тогда не запускаем проигрывание
	if (!m_pLayer->isPlaying())
		return;

	float3 vPos, vLook, vUp;
	m_pLayer->getObserverParam(&vPos, &vLook, &vUp);

	IAudioBuffer *pAB = NULL;

	//проход по массиву инстансов звука, если есть первый попавшийся не проигрываемые тогда его проигрываем
	for (int i = 0, il = m_aInstances.size(); i<il; ++i)
	{
		if (!(m_aInstances[i]->pAB->isPlaying()))
		{
			pAB = m_aInstances[i]->pAB;
			continue;
		}
	}

	if (!pAB)
	{
		//если пришли сюда, значит нет свободных инстансов, создаем новый и проигрыаем
		IAudioBuffer *pInst = m_aInstances[0]->pAB->newInstance();
		pAB = pInst;
		m_aInstances.push_back(new Instance(pInst));
	}

	Com3D(pAB, m_fDist, m_fVolume, m_vWorldPos, vPos, vLook, vUp);
	pAB->play(true);
}

//**************************************************************************

void CSoundEmitter::resume()
{
	//если инстансы проигрывались тогда включаем проигрывание
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if (m_aInstances[i]->isPlaying)
			m_aInstances[i]->pAB->play(true);
	}
}

void CSoundEmitter::pause()
{
	//записываем состояния инстансов и останавливаем
	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		m_aInstances[i]->isPlaying = m_aInstances[i]->pAB->isPlaying();
		m_aInstances[i]->pAB->play(false);
	}
}

//**************************************************************************

bool CSoundEmitter::create(const char* szName, CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget, SOUND_DTYPE dtype)
{
	if (!pCodecTarget || !pLayer)
		return false;

	m_sName = szName;
	m_dtype = dtype;
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

	return true;
}

//##########################################################################

void CSoundEmitter::update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp)
{
	if (m_dtype == SOUND_DTYPE_2D)
		return;

	for (int i = 0, il = m_aInstances.size(); i < il; ++i)
	{
		if (m_aInstances[i]->pAB->isPlaying())
		{
			Com3D(m_aInstances[i]->pAB, m_fDist, m_fVolume, m_vWorldPos, vListenerPos, vListenerDir, vListenerUp);
		}
	}
}