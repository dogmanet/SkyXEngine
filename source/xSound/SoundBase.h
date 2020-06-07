/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __SOUNDBASE_H
#define __SOUNDBASE_H

#include <common/types.h>
#include <common/math.h>
#include <common/string.h>
#include <xcommon/IXSoundSystem.h>
#include <mital.h>

#include "SoundTypes.h"

//##########################################################################

class CSoundBase: public IXUnknownVirtualImplementation<IXSoundBase>
{
public:

	SX_ALIGNED_OP_MEM

	virtual SOUND_DTYPE XMETHODCALLTYPE getType() final;

	virtual void XMETHODCALLTYPE setVolume(float fVolume) final;
	virtual float XMETHODCALLTYPE getVolume() const final;

	virtual void XMETHODCALLTYPE setPan(float fPan) final;
	virtual float XMETHODCALLTYPE getPan() const final;

	virtual const float3& XMETHODCALLTYPE getWorldPos() const final;
	virtual void XMETHODCALLTYPE setWorldPos(const float3 &vPos) final;

	virtual float XMETHODCALLTYPE getDistance() const final;
	virtual void XMETHODCALLTYPE setDistance(float fDist) final;

protected:

	friend CSoundLayer;

	void update();

	bool create(SOUND_DTYPE type, const char *szName, CSoundLayer *pLayer, CSoundSystem *pSoundSystem);

	//************************************************************************

	float m_fVolume = 1.f;
	float m_fPan = 0.f;

	//! аудио буфер
	//IAudioBuffer *m_pAB = NULL;

	//! дистанция слышимости
	float m_fDist = 0.f;

	//! мировая позиция звука
	float3 m_vWorldPos;

	//! тип звука
	SOUND_DTYPE m_dtype = SOUND_DTYPE_2D;

	//! состояние проигрывания
	SOUND_STATE m_state = SOUND_STATE_STOP;

	//! строковый идентификатор звука
	String m_sName = "";

	//! слой владелец
	CSoundLayer *m_pLayer = NULL;

	//CSoundSystem *m_pSoundSystem = NULL;
};

#endif
