
#include "SoundLayer.h"

#include "SoundEmitter.h"
#include "SoundPlayer.h"
#include "SoundSystem.h"

//##########################################################################

CSoundLayer::~CSoundLayer()
{
	if (m_pParent)
		m_pParent->delLayer(this);

	for (maplayer::Iterator i = m_mapLayers.begin(); i != m_mapLayers.end(); i++)
	{
		m_mapLayers[i.first]->Release();
		m_mapLayers.erase(i.first);
	}

	for (mapsoundplayer::Iterator i = m_mapSndPlayers.begin(); i != m_mapSndPlayers.end(); i++)
	{
		m_mapSndPlayers[i.first]->Release();
		m_mapSndPlayers.erase(i.first);
	}

	mem_release(m_pPrimaryBuffer);
}

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

void CSoundLayer::addLayer(CSoundLayer *pLayer, const char *szName)
{
	m_mapLayers[szName] = pLayer;
}

void CSoundLayer::delLayer(CSoundLayer *pLayer)
{
	for (maplayer::Iterator i = m_mapLayers.begin(); i != m_mapLayers.end(); i++)
	{
		if (m_mapLayers[i.first] == pLayer)
		{
			m_mapLayers.erase(i.first);
			break;
		}
	}
}

void CSoundLayer::addSound(CSoundPlayer *pSound, const char *szName)
{
	m_mapSndPlayers[szName] = pSound;
}

void CSoundLayer::delSound(CSoundPlayer *pSound)
{
	for (mapsoundplayer::Iterator i = m_mapSndPlayers.begin(); i != m_mapSndPlayers.end(); i++)
	{
		if (m_mapSndPlayers[i.first] == pSound)
		{
			m_mapSndPlayers.erase(i.first);
			break;
		}
	}
}

void XMETHODCALLTYPE CSoundLayer::getDesc(AudioRawDesc *pDesc) const
{
	if (!m_pPrimaryBuffer)
		return;

	m_pPrimaryBuffer->getDesc(pDesc);
}

const char* XMETHODCALLTYPE CSoundLayer::getName() const
{
	return m_sName.c_str();
}

IXSoundLayer* XMETHODCALLTYPE CSoundLayer::findLayer(const char *szName)
{
	if (!szName)
		return NULL;

	if (strcasecmp(this->getName(), szName) == 0)
		return this;

	IXSoundLayer *pFound = NULL;

	for (maplayer::Iterator i = m_mapLayers.begin(); i != m_mapLayers.end(); i++)
	{
		if ((pFound = m_mapLayers[i.first]->findLayer(szName)))
			break;
	}

	return pFound;
}



uint32_t CSoundLayer::getStreamChunkSize(AudioRawDesc *pDesc) const
{
	return m_pPrimaryBuffer->getStreamChunkSize(pDesc);
}

IAudioBuffer* CSoundLayer::createAudioBuffer(AB_TYPE type, const AudioRawDesc *pDesc)
{
	return m_pPrimaryBuffer->createAudioBuffer(type, pDesc);
}

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
	CSoundEmitter *pEmitter = new CSoundEmitter();
	pEmitter->create(szName, this, m_pSoundSystem);
	return pEmitter;
}

IXSoundPlayer* XMETHODCALLTYPE CSoundLayer::newSoundPlayer(const char *szName, SOUND_DTYPE dtype)
{
	if (!szName)
		return NULL;

	CSoundPlayer *pSound = NULL;

	if (m_mapSndPlayers.KeyExists(szName))
	{
		pSound = m_mapSndPlayers[szName];
		return pSound->newInstance();
	}

	pSound = new CSoundPlayer();
	pSound->create(szName, this, m_pSoundSystem);

	addSound(pSound, szName);

	return pSound;
}

void XMETHODCALLTYPE CSoundLayer::play(bool canPlay)
{
	if (m_isPlaying == canPlay || (m_pParent && !m_pParent->isPlaying() && canPlay))
		return;

	m_isPlaying = canPlay;

	for (maplayer::Iterator i = m_mapLayers.begin(); i != m_mapLayers.end(); i++)
		m_mapLayers[i.first]->play(canPlay);

	for (mapsoundplayer::Iterator i = m_mapSndPlayers.begin(); i != m_mapSndPlayers.end(); i++)
	{
		if (canPlay)
			m_mapSndPlayers[i.first]->resume();
		else
			m_mapSndPlayers[i.first]->pause();
	}

	m_pPrimaryBuffer->play(canPlay);
}

bool XMETHODCALLTYPE CSoundLayer::isPlaying() const
{
	return m_isPlaying;
}

void CSoundLayer::update()
{
	for (maplayer::Iterator i = m_mapLayers.begin(); i != m_mapLayers.end(); i++)
		m_mapLayers[i.first]->update();

	for (mapsoundplayer::Iterator i = m_mapSndPlayers.begin(); i != m_mapLayers.end(); i++)
		m_mapSndPlayers[i.first]->update();
}

bool CSoundLayer::matchPrimaryLayer(const AudioRawDesc *pDesc)
{
	if (!pDesc || !m_pPrimaryBuffer)
		return false;

	AudioRawDesc oPrimaryDesc;
	m_pPrimaryBuffer->getDesc(&oPrimaryDesc);

	return (oPrimaryDesc.uSampleRate == pDesc->uSampleRate);
}
