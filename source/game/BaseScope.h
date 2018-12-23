
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Прицелы
*/

#ifndef __BASE_SCOPE_H
#define __BASE_SCOPE_H

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
