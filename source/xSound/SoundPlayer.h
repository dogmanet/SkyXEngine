/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDPLAYER_H
#define __SOUNDPLAYER_H

#include <xcommon/IXSoundSystem.h>
#include "SoundBase.h"
#include <mital.h>

#include "SoundTypes.h"

//##########################################################################

class CSoundPlayer : public CSoundBase, public virtual IXSoundPlayer
{
public:
	SX_ALIGNED_OP_MEM

		~CSoundPlayer();

	CSoundPlayer* newInstance();

	virtual void XMETHODCALLTYPE play() override;
	virtual void XMETHODCALLTYPE resume() override;
	virtual void XMETHODCALLTYPE pause() override;
	virtual void XMETHODCALLTYPE stop() override;

	virtual bool XMETHODCALLTYPE isPlaying() const override;

	virtual SOUND_LOOP XMETHODCALLTYPE getLoop() const override;
	virtual void XMETHODCALLTYPE setLoop(SOUND_LOOP loop) override;

	virtual float XMETHODCALLTYPE getTime() const override;
	virtual void XMETHODCALLTYPE setTime(float fTime) override;

	virtual float XMETHODCALLTYPE getLength() const override;

protected:

	friend CSoundLayer;

	void update();

	bool create(CSoundLayer *pLayer, IXAudioCodecTarget *pCodecTarget);

	//! возвращает текущую позицию проигрывания звука в байтах
	uint32_t getPosBytes() const;

	/*! установка позиции при потоковом воспроизведении
	@param uPos позиция в байтах
	*/
	void setPosStream(uint32_t uPos);

	//************************************************************************

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
};

#endif
