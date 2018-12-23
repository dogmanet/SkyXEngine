
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Руки зомби
*/

#ifndef __ZOMBIE_HANDS_H
#define __ZOMBIE_HANDS_H

#include "BaseWeapon.h"

/*! Руки зомби
\ingroup cbaseitem
*/
class CZombieHands: public CBaseWeapon
{
	DECLARE_CLASS(CZombieHands, CBaseWeapon);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

protected:

	//! Задача стрельбы
	virtual void taskShoot(float dt);
	float m_fDamage = 30.0f;

	ID m_idActualShootTimeout = -1;
	void actualShoot(float dt);
	void stopAction();
};

#endif
