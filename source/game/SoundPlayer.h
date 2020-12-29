/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

/*!
	\file
	Класс звукового плеера
*/
#ifndef __SOUND_PLAYER_H
#define __SOUND_PLAYER_H

#include "PointEntity.h"

//##########################################################################

//! Флаг автостарта проигрывания звука
#define SND_PLAYER_START_PLAYED ENT_FLAG_0

//! Флаг установки ambient типа звуку
#define SND_PLAYER_TYPE_AMBIENT ENT_FLAG_1

//##########################################################################

/*! Класс звукового плеера
	\ingroup cpointentity
*/
class CSoundPlayer: public CPointEntity
{
	DECLARE_CLASS(CSoundPlayer, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	~CSoundPlayer();

	void setSound(const char *szSound);

	void setPos(const float3 &pos) override;

	void setVolume(float fVolume);
	float getVolume();

	void setDistance(float fDistance);
	float getDistance();

	void setLoop(int iLoop);
	SOUND_LOOP getLoop();

protected:

	void updateFlags() override;

	void turnOn(inputdata_t *pInputdata);
	void turnOff(inputdata_t *pInputdata);

	//########################################################################

	output_t m_onTurnOn;
	output_t m_onTurnOff;
	
	IXSoundPlayer *m_pPlayer = NULL;
	const char *m_szPathSound = "";
	int m_iLoop = SOUND_LOOP_NONE;

	float m_fDist = 10.f;
	float m_fVolume = 1.f;
	
	//int m_iType = SOUND_DTYPE_3D;
};

#endif
