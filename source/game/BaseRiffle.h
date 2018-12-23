
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Винтовки
*/

#ifndef __BASE_RIFFLE_H
#define __BASE_RIFFLE_H

#include "BaseWeapon.h"

/*! Винтовки
\ingroup cbaseitem
*/
class CBaseRiffle: public CBaseWeapon
{
	DECLARE_CLASS(CBaseRiffle, CBaseWeapon);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

	//void primaryAction(BOOL st);
	//void secondaryAction(BOOL st);
	//void reload();

	//void Deploy();
};

#endif
