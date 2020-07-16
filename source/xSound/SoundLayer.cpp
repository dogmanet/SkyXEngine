
#include "SoundLayer.h"

#include "SoundEmitter.h"
#include "SoundPlayer.h"
#include "SoundSystem.h"

//##########################################################################

CSoundLayer::~CSoundLayer()
{
	m_mapLayers.clear();

	for(MapPlayer::Iterator i = m_mapSndPlayers.begin(); i; ++i)
	{
		ArrayPlayer *oAP = i.second;
		oAP->clear();
	}
	m_mapSndPlayers.clear();

	for (MapEmitter::Iterator i = m_mapSndEmitters.begin(); i; ++i)
	{
		ArrayEmitter *oAE = i.second;
		oAE->clear();
	}
	m_mapSndEmitters.clear();

	mem_release(m_pPrimaryBuffer);
	mem_release(m_pParent);
}

//**************************************************************************

void XMETHODCALLTYPE CSoundLayer::FinalRelease()
{
	if(m_pParent)
	{
		SndQueueMsg oMsg;
		oMsg.type = SND_QUEUE_MSG_TYPE_LAYER_DELETE;
		oMsg.pLayer = this;
		oMsg.pOwner = m_pParent;
		m_pParent->addMessage(oMsg);
	}
	else
	{
		delete this;
	}
}

//**************************************************************************

void CSoundLayer::Release2()
{
	for (MapLayer::Iterator i = m_mapLayers.begin(); i; ++i)
		m_mapLayers[i.first]->Release2();
	
	Release();
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

	m_sName = szName;
	m_pParent = pParent;
	m_pSoundSystem = pSoundSystem;

	AudioRawDesc oDesc = *pDesc;
	oDesc.uSize = 0;

	if (pParent == NULL)
		m_pPrimaryBuffer = pSoundSystem->createMasterBuffer(&oDesc);
	else
		m_pPrimaryBuffer = dynamic_cast<IAudioBufferEx*>(pParent->createAudioBuffer(AB_TYPE_PRIMARY, &oDesc));

	if (m_pParent)
		m_pParent->AddRef();

	return true;
}

//**************************************************************************

void CSoundLayer::addLayer(CSoundLayer *pLayer)
{
	if (m_pSoundSystem->findLayer(pLayer->getName()))
		LibReport(REPORT_MSG_LEVEL_FATAL, "Layer name '%s' exists", pLayer->getName());

	m_mapLayers[pLayer->getName()] = pLayer;
}

void CSoundLayer::delLayer(const CSoundLayer *pLayer)
{
	for (MapLayer::Iterator i = m_mapLayers.begin(); i; ++i)
	{
		if(m_mapLayers[i.first] == pLayer)
		{
			m_mapLayers.erase(i.first);
			break;
		}
	}

	mem_delete(pLayer);
}

//**************************************************************************

void CSoundLayer::addSndPlayer(CSoundPlayer *pSndPlayer)
{
	m_mapSndPlayers[pSndPlayer->getName()].push_back(pSndPlayer);
}

void CSoundLayer::delSndPlayer(const CSoundPlayer *pSndPlayer)
{
	const char *szName = pSndPlayer->getName();
	if (!m_mapSndPlayers.KeyExists(szName))
		return;

	const ArrayPlayer *oAP = m_mapSndPlayers.at(szName);
	for (int i = 0, il = oAP->size(); i < il; ++i)
	{
		if (oAP->get(i) == pSndPlayer)
			m_mapSndPlayers[szName].erase(i);
	}

	mem_delete(pSndPlayer);
}

//**************************************************************************

void CSoundLayer::addSndEmitter(CSoundEmitter *pSndEmitter)
{
	m_mapSndEmitters[pSndEmitter->getName()].push_back(pSndEmitter);
}

void CSoundLayer::delSndEmitter(const CSoundEmitter *pSndEmitter)
{
	const char *szName = pSndEmitter->getName();
	if (!m_mapSndEmitters.KeyExists(szName))
		return;

	const ArrayEmitter *oAE = m_mapSndEmitters.at(szName);
	for (int i = 0, il = oAE->size(); i < il; ++i)
	{
		if (oAE->get(i) == pSndEmitter)
			m_mapSndEmitters[szName].erase(i);
	}

	mem_delete(pSndEmitter);
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

	for (MapLayer::Iterator i = m_mapLayers.begin(); i; ++i)
	{
		if((pFound = m_mapLayers[i.first]->findLayer(szName)))
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

//##########################################################################

IXSoundLayer* XMETHODCALLTYPE CSoundLayer::newSoundLayer(const AudioRawDesc *pDesc, const char *szName)
{
	if (!pDesc)
		return NULL;

	if (!matchPrimaryLayer(pDesc))
		return NULL;

	CSoundLayer *pLayer = new CSoundLayer();
	pLayer->init(m_pSoundSystem, this, pDesc, szName);

	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_LAYER_NEW;
	oMsg.pLayer = pLayer;
	oMsg.pOwner = this;
	m_pSoundSystem->addMessage(oMsg);

	return pLayer;
}

//**************************************************************************

IXSoundEmitter* XMETHODCALLTYPE CSoundLayer::newSoundEmitter(const char *szName, SOUND_SPACE space)
{
	if (!szName)
		return NULL;

	CSoundEmitter *pEmitter = NULL;

	if (m_mapSndEmitters.KeyExists(szName) && m_mapSndEmitters[szName].size() > 0)
	{
		CSoundEmitter *pEmitterOrigin = m_mapSndEmitters[szName][0];
		if (pEmitterOrigin->canInstance())
			pEmitter = static_cast<CSoundEmitter*>(pEmitterOrigin->newInstance());
	}
	
	if (!pEmitter)
	{
		IXAudioCodecTarget *pCodecTarget = m_pSoundSystem->getCodecTarget(szName);
		if (!pCodecTarget)
			return NULL;

		pEmitter = new CSoundEmitter();
		pEmitter->create(szName, this, pCodecTarget, space);
	}

	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_NEW;
	oMsg.pEmitter = pEmitter;
	oMsg.pOwner = this;
	m_pSoundSystem->addMessage(oMsg);

	return pEmitter;
}

//**************************************************************************

IXSoundPlayer* XMETHODCALLTYPE CSoundLayer::newSoundPlayer(const char *szName, SOUND_SPACE space)
{
	if (!szName)
		return NULL;

	CSoundPlayer *pPlayer = NULL;

	if (m_mapSndPlayers.KeyExists(szName) && m_mapSndPlayers[szName].size() > 0)
	{
		CSoundPlayer *pPlayerOrigin = m_mapSndPlayers[szName][0];
		if (pPlayerOrigin->canInstance())
			pPlayer = static_cast<CSoundPlayer*>(pPlayerOrigin->newInstance());
	}
	
	if (!pPlayer)
	{
		IXAudioCodecTarget *pCodecTarget = m_pSoundSystem->getCodecTarget(szName);
		if (!pCodecTarget)
			return NULL;

		pPlayer = new CSoundPlayer();
		pPlayer->create(szName, this, pCodecTarget, space);
	}

	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_SND_NEW;
	oMsg.pPlayer = pPlayer;
	oMsg.pOwner = this;
	m_pSoundSystem->addMessage(oMsg);

	return pPlayer;
}

//##########################################################################

void XMETHODCALLTYPE CSoundLayer::play(bool canPlay)
{
	SndQueueMsg oMsg;
	oMsg.type = SND_QUEUE_MSG_TYPE_LAYER_PLAY;
	oMsg.pLayer = this;
	oMsg.arg.b = canPlay;
	addMessage(oMsg);
}

//**************************************************************************

bool XMETHODCALLTYPE CSoundLayer::isPlaying() const
{
	return m_isPlaying;
}

//##########################################################################

void CSoundLayer::update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp)
{
	for (MapLayer::Iterator i = m_mapLayers.begin(); i; ++i)
	{
		if(m_mapLayers[i.first])
			m_mapLayers[i.first]->update(vListenerPos, vListenerDir, vListenerUp);
	}

	for(MapPlayer::Iterator i = m_mapSndPlayers.begin(); i; ++i)
	{
		ArrayPlayer *oAP = i.second;
		for(int k = 0, kl = oAP->size(); k < kl; ++k)
			oAP->get(k)->update(vListenerPos, vListenerDir, vListenerUp);
	}

	for(MapEmitter::Iterator i = m_mapSndEmitters.begin(); i; ++i)
	{
		ArrayEmitter *oAE = i.second;
		for(int k = 0, kl = oAE->size(); k < kl; ++k)
			oAE->get(k)->update(vListenerPos, vListenerDir, vListenerUp);
	}
}

//**************************************************************************

void CSoundLayer::addMessage(SndQueueMsg &oMsg) 
{
	m_pSoundSystem->addMessage(oMsg); 
}

//##########################################################################

bool CSoundLayer::matchPrimaryLayer(const AudioRawDesc *pDesc)
{
	if (!pDesc || !m_pPrimaryBuffer)
		return false;

	AudioRawDesc oPrimaryDesc;
	m_pPrimaryBuffer->getDesc(&oPrimaryDesc);

	return (oPrimaryDesc.uSampleRate == pDesc->uSampleRate);
}

//##########################################################################
//##########################################################################

void CSoundLayer::_play(bool canPlay)
{
	if (m_isPlaying == canPlay || (m_pParent && !m_pParent->isPlaying() && canPlay))
		return;

	m_isPlaying = canPlay;

	for (MapLayer::Iterator i = m_mapLayers.begin(); i; ++i)
		m_mapLayers[i.first]->play(canPlay);

	for (MapPlayer::Iterator i = m_mapSndPlayers.begin(); i; ++i)
	{
		ArrayPlayer *oAP = i.second;
		for (int k = 0, kl = oAP->size(); k < kl; ++k)
		{
			if (canPlay)
				oAP->get(k)->resume();
			else
				oAP->get(k)->pause();
		}
	}

	for (MapEmitter::Iterator i = m_mapSndEmitters.begin(); i; ++i)
	{
		ArrayEmitter *oAE = i.second;
		for (int k = 0, kl = oAE->size(); k < kl; ++k)
		{
			if (canPlay)
				oAE->get(k)->resume();
			else
				oAE->get(k)->pause();
		}
	}

	m_pPrimaryBuffer->play(canPlay);
}
