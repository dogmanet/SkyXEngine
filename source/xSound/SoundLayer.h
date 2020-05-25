/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDLAYER_H
#define __SOUNDLAYER_H

#include <common/assotiativearray.h>
#include <xcommon/IXSoundSystem.h>
#include "SoundBase.h"
#include <mital.h>

#include "SoundTypes.h"

//##########################################################################

class CSoundLayer: public IXUnknownImplementation<IXSoundLayer>
{
public:

	~CSoundLayer();

	virtual void XMETHODCALLTYPE play(bool canPlay) override;
	virtual bool XMETHODCALLTYPE isPlaying() const override;
	virtual const char* XMETHODCALLTYPE getName() const override;

	virtual IXSoundLayer* XMETHODCALLTYPE findLayer(const char *szName) override;

	virtual IXSoundLayer* XMETHODCALLTYPE newSoundLayer(const AudioRawDesc *pDesc, const char *szName) override;
	virtual IXSoundEmitter* XMETHODCALLTYPE newSoundEmitter(const char *szName, SOUND_DTYPE dtype) override;
	virtual IXSoundPlayer* XMETHODCALLTYPE newSoundPlayer(const char *szName, SOUND_DTYPE dtype) override;

	virtual void XMETHODCALLTYPE getDesc(AudioRawDesc *pDesc) const override;

	uint32_t getStreamChunkSize(AudioRawDesc *pDesc) const;
	IAudioBuffer* createAudioBuffer(AB_TYPE type, const AudioRawDesc *pDesc);

	void update();

protected:

	friend CSoundSystem;
	friend CSoundPlayer;

	bool init(CSoundSystem *pSoundSystem, CSoundLayer *pParent, const AudioRawDesc *pDesc, const char *szName);

	void addLayer(CSoundLayer *pLayer, const char *szName);
	void delLayer(CSoundLayer *pLayer);

	void addSound(CSoundPlayer *pSound, const char *szName);
	void delSound(CSoundPlayer *pSound);

	//! соответствует ли описание (его критические элементы) аудио буфера первичному буферу
	bool matchPrimaryLayer(const AudioRawDesc *pDesc);

	void setStateLayers(SOUND_STATE state);
	void setStateSounds(SOUND_STATE state);

	SOUND_STATE m_state = SOUND_STATE_STOP;

	bool m_isPlaying = false;

	CSoundLayer *m_pParent = NULL;

	IAudioBufferEx *m_pPrimaryBuffer = NULL;
	CSoundSystem *m_pSoundSystem = NULL;

	String m_sName = "";

	typedef AssotiativeArray<String, CSoundLayer*> maplayer;
	maplayer m_mapLayers;

	typedef AssotiativeArray<String, CSoundPlayer*> mapsoundplayer;
	mapsoundplayer m_mapSndPlayers;
};

#endif
