/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Навесы
*/

#ifndef _BaseWeaponAddon_H_
#define _BaseWeaponAddon_H_

#include "SXbaseItem.h"

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
class CBaseWeaponAddon: public SXbaseItem
{
	DECLARE_CLASS(CBaseWeaponAddon, SXbaseItem);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	//! Получить тип навеса
	WPN_ADDON getType() const;

protected:
	//! Тип навеса
	WPN_ADDON m_addonType;
};

#endif
