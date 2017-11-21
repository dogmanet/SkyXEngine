/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Ручки
*/

#ifndef _BaseHandle_H_
#define _BaseHandle_H_

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
