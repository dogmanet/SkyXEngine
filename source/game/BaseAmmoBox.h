/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Аммобокс
*/

#ifndef _CBaseAmmoBox_H_
#define _CBaseAmmoBox_H_

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

	int m_iMaxItems; //!< Максимальное количество предметов, которые бокс может выдать
	int m_iCurItems; //!< Сколько осталось
	int m_iItemsPerUse; //!< Сколько выдавать за раз

	const char * m_szAmmoType; //!< Тип предметов для выдачи
};

#endif
