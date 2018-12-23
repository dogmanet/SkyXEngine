
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Ручки
*/

#ifndef __BASE_HANDLE_H
#define __BASE_HANDLE_H

#include "BaseWeaponAddon.h"

/*! Ручки
\ingroup cbaseitem
*/
class CBaseHandle: public CBaseWeaponAddon
{
	DECLARE_CLASS(CBaseHandle, CBaseWeaponAddon);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	//уменьшение раскачки
	//уменьшение разброса
};

#endif
