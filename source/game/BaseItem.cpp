
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseItem.h"
#include "BaseCharacter.h"

/*! \skydocent base_item
Базовый объект элемента инвентаря игрока
*/

BEGIN_PROPTABLE(CBaseItem)
	//! Имя в инвентаре
	DEFINE_FIELD_STRING(m_szInvName, PDFF_NOEDIT | PDFF_NOEXPORT, "inv_name", "", EDITOR_NONE)
	//! Может ли стакаться
	DEFINE_FIELD_BOOL(m_bInvStackable, PDFF_NOEDIT | PDFF_NOEXPORT, "inv_stackable", "", EDITOR_NONE)
	//! Текущий размер стака
	DEFINE_FIELD_INT(m_iInvStackCurSize, PDFF_NOEDIT, "inv_stack_cur", "", EDITOR_NONE)
	//! Максимальное число объектов в стаке
	DEFINE_FIELD_INT(m_iInvStackMaxSize, PDFF_NOEDIT | PDFF_NOEXPORT, "inv_stack_max", "", EDITOR_NONE)
	//! Масса одного объекта, кг
	DEFINE_FIELD_FLOAT(m_iInvWeight, PDFF_NOEDIT | PDFF_NOEXPORT, "inv_weight", "", EDITOR_NONE)
	//! Можно ли поднимать объект
	DEFINE_FIELD_BOOL(m_bPickable, PDFF_NOEDIT | PDFF_NOEXPORT, "inv_pickable", "", EDITOR_NONE)

	DEFINE_OUTPUT(m_onPickUp, "OnPickUp", "On pickup")
	DEFINE_OUTPUT(m_onDrop, "OnDrop", "On drop")
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseItem, base_item);

CBaseItem::CBaseItem(CEntityManager * pMgr):
	BaseClass(pMgr),
	m_bInvStackable(true),
	m_iInvStackCurSize(0),
	m_iInvStackMaxSize(1),
	m_iInvWeight(0.0f),
	m_bPickable(true)
{
}

float CBaseItem::getWeight()
{
	return(m_iInvWeight);
}

void CBaseItem::onUse(CBaseEntity *pUser)
{
	BaseClass::onUse(pUser);
	if(!m_bPickable)
	{
		return;
	}

	CBaseCharacter *pCharacter = (CBaseCharacter*)pUser;
	pCharacter->getInventory()->putItem(this);
	FIRE_OUTPUT(m_onPickUp, pUser);

	if(m_bInvStackable)
	{
		REMOVE_ENTITY(this);
	}
	else
	{
		setModeInventory();
	}
}

void CBaseItem::setModeInventory()
{
	if(!m_bWorldMode)
	{
		return;
	}
	m_bWorldMode = false;

	releasePhysics();
	if(m_pAnimPlayer)
	{
		mem_release(m_pAnimPlayer);
	}
}

void CBaseItem::setModeWorld()
{
	if(m_bWorldMode)
	{
		return;
	}
	m_bWorldMode = true;

	setModel(m_szModelFile);
}
