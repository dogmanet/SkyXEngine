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

	void XMETHODCALLTYPE play(bool canPlay) override;
	bool XMETHODCALLTYPE isPlaying() const override;
	const char* XMETHODCALLTYPE getName() const override;

	IXSoundLayer* XMETHODCALLTYPE findLayer(const char *szName) override;

	IXSoundLayer* XMETHODCALLTYPE newSoundLayer(const char *szName, const AudioRawDesc *pDesc = NULL) override;
	IXSoundEmitter* XMETHODCALLTYPE newSoundEmitter(const char *szName, SOUND_SPACE space) override;
	IXSoundPlayer* XMETHODCALLTYPE newSoundPlayer(const char *szName, SOUND_SPACE space) override;

	void XMETHODCALLTYPE getDesc(AudioRawDesc *pDesc) const override;

	//########################################################################

	size_t getStreamChunkSize(AudioRawDesc *pDesc) const;
	IAudioBuffer* createAudioBuffer(AB_TYPE type, const AudioRawDesc *pDesc);

	void update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp);

	void addMessage(SndQueueMsg &oMsg);

private:
	virtual void XMETHODCALLTYPE FinalRelease() override;

protected:

	friend CSoundSystem;
	friend CSoundPlayer;
	friend CSoundEmitter;

	bool init(CSoundSystem *pSoundSystem, CSoundLayer *pParent, const AudioRawDesc *pDesc, const char *szName);

	//************************************************************************
	// функции добавления/удаления обьектов в массивы
	// при удалении из массива произойдет mem_delete удаляемого обьекта

	void addLayer(CSoundLayer *pLayer);
	void delLayer(CSoundLayer *pLayer);

	void addSndPlayer(CSoundPlayer *pSndPlayer);
	void delSndPlayer(const CSoundPlayer *pSndPlayer);

	void addSndEmitter(CSoundEmitter *pSndEmitter);
	void delSndEmitter(const CSoundEmitter *pSndEmitter);

	void _play(bool canPlay, bool isFromParent);

	//************************************************************************

	//! соответствует ли описание (его критические элементы) аудио буфера первичному буферу
	bool matchPrimaryLayer(const AudioRawDesc *pDesc);

	//########################################################################

	SOUND_STATE m_state = SOUND_STATE_STOP;

	bool m_isPlaying = false;
	bool m_isPlayingTotal = false;

	CSoundLayer *m_pParent = NULL;

	IAudioBufferEx *m_pPrimaryBuffer = NULL;
	CSoundSystem *m_pSoundSystem = NULL;

	String m_sName = "";

	typedef AssotiativeArray<String, CSoundLayer*> MapLayer;
	MapLayer m_mapLayers;
	Array<CSoundLayer*> m_aLayers;

	typedef Array<CSoundPlayer*> ArrayPlayer;
	typedef AssotiativeArray<String, ArrayPlayer> MapPlayer;
	MapPlayer m_mapSndPlayers;
	Array<CSoundPlayer*> m_aSndPlayers;

	typedef Array<CSoundEmitter*> ArrayEmitter;
	typedef AssotiativeArray<String, ArrayEmitter> MapEmitter;
	MapEmitter m_mapSndEmitters;
	Array<CSoundEmitter*> m_aSndEmitters;
};

#endif
