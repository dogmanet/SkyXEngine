
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Снайперские
*/

#ifndef __BASE_SNIPE_H
#define __BASE_SNIPE_H

#include "BaseWeapon.h"

/*! Снайперские
\ingroup cbaseitem
*/
class SXbaseSnipe: public CBaseWeapon
{
	DECLARE_CLASS(SXbaseSnipe, CBaseWeapon);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
