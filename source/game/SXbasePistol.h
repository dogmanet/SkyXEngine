/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Пистолеты
*/

#ifndef _SXbasePistol_H_
#define _SXbasePistol_H_

/*! Пистолеты
\ingroup cbaseitem
*/
#include "SXbaseWeapon.h"

class SXbasePistol: public SXbaseWeapon
{
	DECLARE_CLASS(SXbasePistol, SXbaseWeapon);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
