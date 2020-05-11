
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseAmmoBox.h"
#include "BaseCharacter.h"

/*! \skydocent base_ammobox
Базовый класс коробки с припасами. При использовании, выдает игроку items_per_use объектов item_class, если внутри содержится достаточно
*/

BEGIN_PROPTABLE(CBaseAmmoBox)
	//! Объем коробки в итемах
	DEFINE_FIELD_INT(m_iMaxItems, PDFF_NOEDIT | PDFF_NOEXPORT, "max_items", "", EDITOR_NONE)
	//! Текущее количество итемов внутри
	DEFINE_FIELD_INT(m_iCurItems, PDFF_NOEDIT | PDFF_NOEXPORT, "cur_items", "", EDITOR_NONE)
	//! Сколько выдать за одно использование
	DEFINE_FIELD_INT(m_iItemsPerUse, PDFF_NOEDIT | PDFF_NOEXPORT, "items_per_use", "", EDITOR_NONE)
	//! Класс выдаваемого объекта
	DEFINE_FIELD_STRING(m_szAmmoType, PDFF_NOEDIT | PDFF_NOEXPORT, "item_class", "", EDITOR_NONE)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseAmmoBox, base_ammobox);

CBaseAmmoBox::CBaseAmmoBox(CEntityManager * pMgr):
	BaseClass(pMgr)
{
	m_bPickable = false;
	m_bInvStackable = false;
}

void CBaseAmmoBox::onUse(CBaseEntity *pUser)
{
	if(m_iCurItems < 0)
	{
		m_iCurItems = m_iMaxItems;
	}
	if(m_iMaxItems <= 0)
	{
		m_iCurItems = m_iItemsPerUse;
	}

	CBaseCharacter *pCharacter = (CBaseCharacter*)pUser;

	int iItemsToUse = min(m_iCurItems, m_iItemsPerUse);
	if(iItemsToUse)
	{
		pCharacter->getInventory()->putItems(m_szAmmoType, iItemsToUse);
		m_iCurItems -= iItemsToUse;
	}
	// FIRE_OUTPUT(m_onPickUp, pUser);

	BaseClass::onUse(pUser);
}
