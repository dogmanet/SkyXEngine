/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Винтовки
*/

#ifndef _SXbaseRiffle_H_
#define _SXbaseRiffle_H_

#include "SXbaseWeapon.h"

/*! Винтовки
\ingroup cbaseitem
*/
class SXbaseRiffle: public SXbaseWeapon
{
	DECLARE_CLASS(SXbaseRiffle, SXbaseWeapon);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

	//void PrimaryAction(BOOL st);
	//void SecondaryAction(BOOL st);
	//void Reload();

	//void Deploy();
};

#endif
