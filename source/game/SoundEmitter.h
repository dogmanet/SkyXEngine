/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

/*!
	\file
	Класс звукового эмиттера
*/
#ifndef __SOUND_EMITTER_H
#define __SOUND_EMITTER_H

#include "PointEntity.h"
#include <xcommon/IXSoundSystem.h>

//##########################################################################

//! Флаг установки ambient типа звуку
#define SND_EMITTER_TYPE_AMBIENT ENT_FLAG_1

//##########################################################################

/*! Класс звукового плеера
	\ingroup cpointentity
*/
class CSoundEmitter: public CPointEntity
{
	DECLARE_CLASS(CSoundEmitter, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	~CSoundEmitter();

	virtual void setSound(const char *szSound);

	void setPos(const float3 &pos) override;

	void setVolume(float fVolume);
	float getVolume();

protected:

	void updateFlags() override;

	void play(inputdata_t *pInputdata);

	//########################################################################

	output_t m_onTurnOn;
//	output_t m_onTurnOff;
	
	IXSoundEmitter *m_pEmitter = NULL;
	const char *m_szPathSound = "";

	float m_fDist = 10.f;
	float m_fVolume = 1.f;
	
	//int m_iType = SOUND_DTYPE_3D;
};

#endif
