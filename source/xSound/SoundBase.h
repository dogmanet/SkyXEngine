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

	//########################################################################

	//! можно ли инстанцировать?
	virtual bool canInstance() const = 0;

	/*! создает и возвращает новую копию
		@note нельзя инстанцировать объекты где есть потоковая загрузка
	*/
	virtual CSoundBase* newInstance() = 0;

	//! обновление звука
	virtual void update(const float3 &vListenerPos, const float3 &vListenerDir, const float3 &vListenerUp) = 0;

	//########################################################################

	SOUND_TYPE XMETHODCALLTYPE getType() const;

	SOUND_SPACE XMETHODCALLTYPE getSpace() const;
	void XMETHODCALLTYPE setSpace(SOUND_SPACE space);

	const char* XMETHODCALLTYPE getName() const override;

	void XMETHODCALLTYPE setVolume(float fVolume) override;
	float XMETHODCALLTYPE getVolume() const override;

	void XMETHODCALLTYPE setPan(float fPan) override;
	float XMETHODCALLTYPE getPan() const override;

	void XMETHODCALLTYPE setWorldPos(const float3 &vPos) override;
	const float3& XMETHODCALLTYPE getWorldPos() const override;

	void XMETHODCALLTYPE setDistance(float fDist) override;
	float XMETHODCALLTYPE getDistance() const override;

protected:

	virtual void _setSpace(SOUND_SPACE space);
	virtual void _setVolume(float fVolume);
	virtual void _setPan(float fPan);
	virtual void _setWorldPos(const float3 &vPos);
	virtual void _setDistance(float fDist);

	//########################################################################

	SOUND_TYPE m_type;

	//! громкость [0.0, 1.0]
	float m_fVolume = 1.f;

	//! панорамирование (смещение между ушами слушателя) [-1.0, 1.0] <=> [L, R]
	float m_fPan = 0.f;

	//! дистанция слышимости
	float m_fDist = 10.f;

	//! мировая позиция звука
	float3 m_vWorldPos;

	//! тип звука
	SOUND_SPACE m_space = SOUND_SPACE_2D;

	//! состояние проигрывания
	SOUND_STATE m_state = SOUND_STATE_STOP;

	//! строковый идентификатор звука
	String m_sName = "";

	//! слой владелец
	CSoundLayer *m_pLayer = NULL;
};

#endif
