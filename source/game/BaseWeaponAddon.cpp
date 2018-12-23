
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseWeaponAddon.h"

/*! \skydocent base_wpn_addon
Базовый класс навеса
*/

BEGIN_PROPTABLE(CBaseWeaponAddon)
	//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseWeaponAddon, base_wpn_addon);

CBaseWeaponAddon::CBaseWeaponAddon(CEntityManager * pMgr):
	BaseClass(pMgr),
	m_addonType(WPN_ADDON_NONE)
{
}

WPN_ADDON CBaseWeaponAddon::getType() const
{
	return(m_addonType);
}
