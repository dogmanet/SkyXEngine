
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Аммобокс
*/

#ifndef __BASE_AMMO_BOX_H
#define __BASE_AMMO_BOX_H

#include "BaseItem.h"

/*! Аммобокс
\ingroup cbaseitem
*/
class CBaseAmmoBox: public CBaseItem
{
	DECLARE_CLASS(CBaseAmmoBox, CBaseItem);
	DECLARE_PROPTABLE();
public:
	ThisClass(CEntityManager * pMgr);

	void onUse(CBaseEntity *pUser);

	int m_iMaxItems = 0; //!< Максимальное количество предметов, которые бокс может выдать
	int m_iCurItems = -1; //!< Сколько осталось
	int m_iItemsPerUse = 1; //!< Сколько выдавать за раз

	const char * m_szAmmoType; //!< Тип предметов для выдачи
};

#endif
