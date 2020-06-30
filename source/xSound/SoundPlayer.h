/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDPLAYER_H
#define __SOUNDPLAYER_H

#ifdef _MSC_VER
#pragma warning(disable : 4250)
#endif

#include <common/queue.h>
#include <xcommon/IXSoundSystem.h>
#include "SoundBase.h"
#include <mital.h>

#include "SoundTypes.h"
#include "SoundSystem.h"

//##########################################################################

class CSoundPlayer: public CSoundBase, public virtual IXSoundPlayer
{
public:
	SX_ALIGNED_OP_MEM

	~CSoundPlayer();

	//########################################################################

	bool canInstance() const override;

	CSoundBase* newInstance() override;

	void update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp) override;

	//########################################################################

	void XMETHODCALLTYPE play() override;
	void XMETHODCALLTYPE resume() override;
	void XMETHODCALLTYPE pause() override;
	void XMETHODCALLTYPE stop() override;

	bool XMETHODCALLTYPE isPlaying() const override;

	SOUND_LOOP XMETHODCALLTYPE getLoop() const override;
	void XMETHODCALLTYPE setLoop(SOUND_LOOP loop) override;

	float XMETHODCALLTYPE getTime() const override;
	void XMETHODCALLTYPE setTime(float fTime) override;

	float XMETHODCALLTYPE getLength() const override;

private:
	virtual void XMETHODCALLTYPE FinalRelease() override;

protected:

	friend CSoundLayer;
	friend CSoundSystem;

	bool create(const char* szName, CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget, SOUND_SPACE space);

	//! возвращает текущую позицию проигрывания звука в байтах
	uint32_t getPosBytes() const;

	/*! установка позиции при потоковом воспроизведении
		@param uPos позиция в байтах
	*/
	void setPosStream(uint32_t uPos);

	//**********************************************************************
	// функции сообщений

	void _setSpace(SOUND_SPACE space);
	void _play();
	void _resume();
	void _pause();
	void _stop();
	void _setLoop(SOUND_LOOP loop);
	void _setTime(float fTime);

	//########################################################################

	//! аудио буфер
	IAudioBuffer *m_pAB = NULL;

	//! зацикливать ли воспроизведение
	SOUND_LOOP m_loop = SOUND_LOOP_NONE;

	//! длина звука в байтах
	uint32_t m_uLengthBytes = 0;

	//! длина звука в секундах
	uint32_t m_fLengthMls = 0;

	//! кодек
	IXAudioCodecTarget *m_pCodecTarget = NULL;

	//! данные потоковой загрузки
	CStreamData *m_pStream = NULL;

	//! параметры слушателя
	float3 m_vListenerPos, m_vListenerDir, m_vListenerUp;
};

#endif
