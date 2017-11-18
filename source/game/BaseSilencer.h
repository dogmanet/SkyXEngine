/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Глушители
*/

#ifndef _BaseSilencer_H_
#define _BaseSilencer_H_

#include "BaseWeaponAddon.h"

/*! Глушители
\ingroup cbaseitem
*/
class CBaseSilencer: public CBaseWeaponAddon
{
	DECLARE_CLASS(CBaseSilencer, CBaseWeaponAddon);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	// уменьшение разброса
	// замена звука выстрела
	// уменьшение урона
	// уменьшение бронебойности
	// замена эффекта выстрела
};

#endif
