
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Патроны
*/

#ifndef __BASE_AMMO_H
#define __BASE_AMMO_H

#include "BaseSupply.h"

/*! Базовый класс патронов
\ingroup cbaseitem
*/
class CBaseAmmo: public CBaseSupply
{
	DECLARE_CLASS(CBaseAmmo, CBaseSupply);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	//! Стреляет пулю
	void fire(const float3 &vStart, const float3 &vEnd);

protected:

	//! Начальная скорость
	float m_fStartSpeed;
	//! Масса пули
	float m_fBulletMass;
	//! Бронебойность
	float m_fArmorPiercing;
};

#endif
