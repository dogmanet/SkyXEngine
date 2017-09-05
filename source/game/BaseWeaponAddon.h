#ifndef _BaseWeaponAddon_H_
#define _BaseWeaponAddon_H_

#include "SXbaseItem.h"

enum WPN_ADDON
{
	WPN_ADDON_NONE,
	WPN_ADDON_SCOPE,
	WPN_ADDON_SILENCER,
	WPN_ADDON_HANDLE,
	WPN_ADDON_MAG
};

class CBaseWeaponAddon: public SXbaseItem
{
	DECLARE_CLASS(CBaseWeaponAddon, SXbaseItem);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	WPN_ADDON getType() const;

protected:
	WPN_ADDON m_addonType;
};

#endif
