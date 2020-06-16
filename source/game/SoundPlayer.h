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
#include <xcommon/IXSoundSystem.h>


/*! Класс звукового плеера
	\ingroup cpointentity
*/
class CSoundPlayer : public CPointEntity
{
	DECLARE_CLASS(CSoundPlayer, CPointEntity);
	DECLARE_PROPTABLE();
public:
	//CSoundPlayer() = default;
	CSoundPlayer(CEntityManager * pMgr);

	virtual void setSound(const char *szSound);

	void onSync() override;

	void stop();
	
	void play();

protected:
	
	IXSoundPlayer *m_pPlayer = NULL;
	const char *m_szPathSound = "";

	float m_fDist = 10.f;
};

#endif
