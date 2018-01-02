/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Пистолеты
*/

#ifndef _CBasePistol_H_
#define _CBasePistol_H_

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
