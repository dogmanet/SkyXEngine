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

#include "SXbaseWeapon.h"

/*! Снайперские
\ingroup cbaseitem
*/
class SXbaseSnipe: public SXbaseWeapon
{
	DECLARE_CLASS(SXbaseSnipe, SXbaseWeapon);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
