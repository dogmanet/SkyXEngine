/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Патроны
*/

#ifndef _SXbaseAmmo_H_
#define _SXbaseAmmo_H_

#include "SXbaseSupply.h"

/*! Базовый класс патронов
\ingroup cbaseitem
*/
class SXbaseAmmo: public SXbaseSupply
{
	DECLARE_CLASS(SXbaseAmmo, SXbaseSupply);
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
