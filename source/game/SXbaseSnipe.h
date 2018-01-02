/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Снайперские
*/

#ifndef _SXbaseSnipe_H_
#define _SXbaseSnipe_H_

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
