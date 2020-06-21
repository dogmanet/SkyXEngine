
#include "SoundLayer.h"

#include "SoundEmitter.h"
#include "SoundPlayer.h"
#include "SoundSystem.h"

//##########################################################################

CSoundLayer::~CSoundLayer()
{
	if (m_pParent)
		m_pParent->delLayer(this);

	for (maplayer::Iterator i = m_mapLayers.begin(); i; i++)
	{
		mem_release(m_mapLayers[i.first]);
		m_mapLayers.erase(i.first);
	}

	for (mapsoundplayer::Iterator i = m_mapSndPlayers.begin(); i; i++)
	{
		mem_release(m_mapSndPlayers[i.first]);
		m_mapSndPlayers.erase(i.first);
	}

	int iCount0 = m_mapSndEmitters.Size();
	for (mapsoundemitter::Iterator i = m_mapSndEmitters.begin(); i; i++)
	{
		mem_release(m_mapSndEmitters[i.first]);
		m_mapSndEmitters.erase(i.first);
	}

	mem_release_del(m_pPrimaryBuffer);
}

//**************************************************************************

bool CSoundLayer::init(CSoundSystem *pSoundSystem, CSoundLayer *pParent, const AudioRawDesc *pDesc, const char *szName)
{
	if (!pDesc || !pSoundSystem)
		return false;

	if (m_mapLayers.KeyExists(szName))
	{
		return false;
	}

	AudioRawDesc oDesc = *pDesc;
	oDesc.uSize = 0;

	if (pParent == NULL)
		m_pPrimaryBuffer = pSoundSystem->createMasterBuffer(&oDesc);
	else
		m_pPrimaryBuffer = dynamic_cast<IAudioBufferEx*>(createAudioBuffer(AB_TYPE_PRIMARY, &oDesc));

	m_sName = szName;
	m_pParent = pParent;
	m_pSoundSystem = pSoundSystem;

	return true;
}

//**************************************************************************

void CSoundLayer::addLayer(CSoundLayer *pLayer, const char *szName)
{
	m_mapLayers[szName] = pLayer;
}

void CSoundLayer::delLayer(const CSoundLayer *pLayer)
{
	for (maplayer::Iterator i = m_mapLayers.begin(); i; i++)
	{
		if (m_mapLayers[i.first] == pLayer)
		{
			m_mapLayers.erase(i.first);
			break;
		}
	}
}

//**************************************************************************

void CSoundLayer::addSndPlayer(CSoundPlayer *pSndPlayer, const char *szName)
{
	m_mapSndPlayers[szName] = pSndPlayer;
}

void CSoundLayer::delSndPlayer(const CSoundPlayer *pSndPlayer)
{
	for (mapsoundplayer::Iterator i = m_mapSndPlayers.begin(); i; i++)
	{
		if (m_mapSndPlayers[i.first] == pSndPlayer)
		{
			m_mapSndPlayers[i.first] = NULL;
			m_mapSndPlayers.erase(i.first);
			break;
		}
	}
}

//**************************************************************************

void CSoundLayer::addSndEmitter(CSoundEmitter *pSndEmitter, const char *szName)
{
	m_mapSndEmitters[szName] = pSndEmitter;
}

void CSoundLayer::delSndEmitter(const CSoundEmitter *pSndEmitter)
{
	int iCount0 = m_mapSndEmitters.Size();
	for (mapsoundemitter::Iterator i = m_mapSndEmitters.begin(); i; i++)
	{
		if (m_mapSndEmitters[i.first] == pSndEmitter)
		{
			m_mapSndEmitters[i.first] = NULL;
			m_mapSndEmitters.erase(i.first);
			break;
		}
	}
	int iCount1 = m_mapSndEmitters.Size();
	int qwerty = 0;
}

//**************************************************************************

void XMETHODCALLTYPE CSoundLayer::getDesc(AudioRawDesc *pDesc) const
{
	if (!m_pPrimaryBuffer)
		return;

	m_pPrimaryBuffer->getDesc(pDesc);
}

//**************************************************************************

const char* XMETHODCALLTYPE CSoundLayer::getName() const
{
	return m_sName.c_str();
}

//**************************************************************************

IXSoundLayer* XMETHODCALLTYPE CSoundLayer::findLayer(const char *szName)
{
	if (!szName)
		return NULL;

	if (strcasecmp(this->getName(), szName) == 0)
		return this;

	IXSoundLayer *pFound = NULL;

	for (maplayer::Iterator i = m_mapLayers.begin(); i; i++)
	{
		if ((pFound = m_mapLayers[i.first]->findLayer(szName)))
			break;
	}

	return pFound;
}

//**************************************************************************

uint32_t CSoundLayer::getStreamChunkSize(AudioRawDesc *pDesc) const
{
	return m_pPrimaryBuffer->getStreamChunkSize(pDesc);
}

IAudioBuffer* CSoundLayer::createAudioBuffer(AB_TYPE type, const AudioRawDesc *pDesc)
{
	return m_pPrimaryBuffer->createAudioBuffer(type, pDesc);
}

//**************************************************************************

IXSoundLayer* XMETHODCALLTYPE CSoundLayer::newSoundLayer(const AudioRawDesc *pDesc, const char *szName)
{
	if (!pDesc)
		return NULL;

	if (!matchPrimaryLayer(pDesc))
		return NULL;

	CSoundLayer *pLayer = new CSoundLayer();
	pLayer->init(m_pSoundSystem, this, pDesc, szName);

	addLayer(pLayer, szName);

	return pLayer;
}

IXSoundEmitter* XMETHODCALLTYPE CSoundLayer::newSoundEmitter(const char *szName, SOUND_DTYPE dtype)
{
	if (!szName)
		return NULL;

	CSoundEmitter *pEmitter = NULL;

	if (m_mapSndEmitters.KeyExists(szName))
	{
		CSoundEmitter *pEmitterOrigin = m_mapSndEmitters[szName];
		pEmitter = pEmitterOrigin->newInstance();
	}
	else
	{
		IXAudioCodecTarget *pCodecTarget = m_pSoundSystem->getCodecTarget(szName);
		if (!pCodecTarget)
			return NULL;

		pEmitter = new CSoundEmitter();
		pEmitter->create(this, pCodecTarget, dtype);
	}

	addSndEmitter(pEmitter, szName);

	return pEmitter;
}

IXSoundPlayer* XMETHODCALLTYPE CSoundLayer::newSoundPlayer(const char *szName, SOUND_DTYPE dtype)
{
	if (!szName)
		return NULL;

	CSoundPlayer *pPlayer = NULL;

	if (m_mapSndPlayers.KeyExists(szName))
	{
		if (m_mapSndPlayers[szName])
			pPlayer = m_mapSndPlayers[szName];
		//return pPlayer->newInstance();
	}
	else
	{
		IXAudioCodecTarget *pCodecTarget = m_pSoundSystem->getCodecTarget(szName);
		if (!pCodecTarget)
			return NULL;

		pPlayer = new CSoundPlayer();
		pPlayer->create(this, pCodecTarget, dtype);
	}

	addSndPlayer(pPlayer, szName);

	return pPlayer;
}

//##########################################################################

void XMETHODCALLTYPE CSoundLayer::play(bool canPlay)
{
	if (m_isPlaying == canPlay || (m_pParent && !m_pParent->isPlaying() && canPlay))
		return;

	m_isPlaying = canPlay;

	for (maplayer::Iterator i = m_mapLayers.begin(); i; i++)
		m_mapLayers[i.first]->play(canPlay);

	for (mapsoundplayer::Iterator i = m_mapSndPlayers.begin(); i; i++)
	{
		if (canPlay)
			m_mapSndPlayers[i.first]->resume();
		else
			m_mapSndPlayers[i.first]->pause();
	}

	for (mapsoundemitter::Iterator i = m_mapSndEmitters.begin(); i; i++)
	{
		if (canPlay)
			m_mapSndEmitters[i.first]->resume();
		else
			m_mapSndEmitters[i.first]->pause();
	}

	m_pPrimaryBuffer->play(canPlay);
}

bool XMETHODCALLTYPE CSoundLayer::isPlaying() const
{
	return m_isPlaying;
}

void CSoundLayer::update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp)
{
	for (maplayer::Iterator i = m_mapLayers.begin(); i; i++)
	{
		if (m_mapLayers[i.first])
			m_mapLayers[i.first]->update(vListenerPos, vListenerDir, vListenerUp);
	}

	for (mapsoundplayer::Iterator i = m_mapSndPlayers.begin(); i; i++)
	{
		if (m_mapSndPlayers[i.first])
			m_mapSndPlayers[i.first]->update(vListenerPos, vListenerDir, vListenerUp);
	}

	for (mapsoundemitter::Iterator i = m_mapSndEmitters.begin(); i; i++)
	{
		if (m_mapSndEmitters[i.first])
			m_mapSndEmitters[i.first]->update(vListenerPos, vListenerDir, vListenerUp);
	}
}

void CSoundLayer::getObserverParam(float3 *pPos, float3 *pLook, float3 *pUp)
{
	m_pSoundSystem->getObserverParam(pPos, pLook, pUp);
}

bool CSoundLayer::matchPrimaryLayer(const AudioRawDesc *pDesc)
{
	if (!pDesc || !m_pPrimaryBuffer)
		return false;

	AudioRawDesc oPrimaryDesc;
	m_pPrimaryBuffer->getDesc(&oPrimaryDesc);

	return (oPrimaryDesc.uSampleRate == pDesc->uSampleRate);
}
