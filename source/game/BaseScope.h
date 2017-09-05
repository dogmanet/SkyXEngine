#ifndef _BaseScope_H_
#define _BaseScope_H_

#include "BaseWeaponAddon.h"

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
