/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Аммобокс
*/

#ifndef _SXbaseAmmoBox_H_
#define _SXbaseAmmoBox_H_

#include "SXbaseItem.h"

/*! Аммобокс
\ingroup cbaseitem
*/
class SXbaseAmmoBox: public SXbaseItem
{
	DECLARE_CLASS(SXbaseAmmoBox, SXbaseItem);
	DECLARE_PROPTABLE();
public:
	ThisClass(EntityManager * pMgr);

	int m_iMaxItems; //!< Максимальное количество предметов, которые бокс может выдать
	int m_iCurItems; //!< Сколько осталось
	int m_iItemsPerUse; //!< Сколько выдавать за раз

	const char * m_szAmmoType; //!< Тип предметов для выдачи
};

#endif
