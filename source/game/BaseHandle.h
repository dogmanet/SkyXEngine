#ifndef _BaseHandle_H_
#define _BaseHandle_H_

#include "BaseWeaponAddon.h"

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
