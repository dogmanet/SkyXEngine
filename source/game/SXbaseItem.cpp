#include "SXbaseItem.h"

/*! \skydocent base_item
Базовый объект элемента инвентаря игрока
*/

BEGIN_PROPTABLE(SXbaseItem)
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
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseItem, base_item);

SXbaseItem::SXbaseItem(EntityManager * pMgr):
	BaseClass(pMgr),
	m_bInvStackable(true),
	m_iInvStackCurSize(0),
	m_iInvStackMaxSize(0),
	m_iInvWeight(0.0f),
	m_bPickable(true)
{
}
