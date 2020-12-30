
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Навесы
*/

#ifndef __BASE_WEAPON_ADDON_H
#define __BASE_WEAPON_ADDON_H

#include "BaseItem.h"

/*! Тип навеса
\ingroup cbaseitem
*/
enum WPN_ADDON
{
	WPN_ADDON_NONE, //!< Нет
	WPN_ADDON_SCOPE, //!< Прицел
	WPN_ADDON_SILENCER, //!< Глушитель
	WPN_ADDON_HANDLE, //!< Ручка
	WPN_ADDON_MAG //!< Магазин
};

/*! Навесы на оружие
\ingroup cbaseitem
*/
class CBaseWeaponAddon: public CBaseItem
{
	DECLARE_CLASS(CBaseWeaponAddon, CBaseItem);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

	//! Получить тип навеса
	WPN_ADDON getType() const;

protected:
	//! Тип навеса
	WPN_ADDON m_addonType = WPN_ADDON_NONE;
};

#endif
