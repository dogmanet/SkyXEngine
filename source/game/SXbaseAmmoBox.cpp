#include "SXbaseAmmoBox.h"


BEGIN_PROPTABLE(SXbaseAmmoBox)
	DEFINE_FIELD_INT(m_iMaxItems, PDFF_NOEDIT | PDFF_NOEXPORT, "max_items", "", EDITOR_NONE)
	DEFINE_FIELD_INT(m_iCurItems, PDFF_NOEDIT | PDFF_NOEXPORT, "cur_items", "", EDITOR_NONE)
	DEFINE_FIELD_INT(m_iItemsPerUse, PDFF_NOEDIT | PDFF_NOEXPORT, "items_per_use", "", EDITOR_NONE)
	DEFINE_FIELD_STRING(m_szAmmoType, PDFF_NOEDIT | PDFF_NOEXPORT, "item_class", "", EDITOR_NONE)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseAmmoBox, base_ammobox);

SXbaseAmmoBox::SXbaseAmmoBox(EntityManager * pMgr):
	BaseClass(pMgr),
	m_iMaxItems(0),
	m_iCurItems(0),
	m_iItemsPerUse(1)
{
	m_bPickable = false;
	m_bInvStackable = false;
}