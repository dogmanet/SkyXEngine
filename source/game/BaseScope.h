/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Прицелы
*/

#ifndef _BaseScope_H_
#define _BaseScope_H_

#include "BaseWeaponAddon.h"

/*! Прицелы
\ingroup cbaseitem
*/
class CBaseScope: public CBaseWeaponAddon
{
	DECLARE_CLASS(CBaseScope, CBaseWeaponAddon);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	// уменьшение разброса
	// время сведения
};

#endif
