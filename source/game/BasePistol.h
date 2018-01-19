
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Пистолеты
*/

#ifndef __BASE_PISTOL_H
#define __BASE_PISTOL_H

/*! Пистолеты
\ingroup cbaseitem
*/
#include "BaseWeapon.h"

class CBasePistol: public CBaseWeapon
{
	DECLARE_CLASS(CBasePistol, CBaseWeapon);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
