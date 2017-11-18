#include "SXbaseAmmo.h"

/*! \skydocent base_ammo
Базовый класс для патронов
*/

BEGIN_PROPTABLE(SXbaseAmmo)
	//! Начальная скорость пули
	DEFINE_FIELD_FLOAT(m_fStartSpeed, PDFF_NOEDIT | PDFF_NOEXPORT, "start_speed", "", EDITOR_NONE)
	//! Масса пули, кг
	DEFINE_FIELD_FLOAT(m_fBulletMass, PDFF_NOEDIT | PDFF_NOEXPORT, "bullet_mass", "", EDITOR_NONE)
	//! Бронебойность
	DEFINE_FIELD_FLOAT(m_fArmorPiercing, PDFF_NOEDIT | PDFF_NOEXPORT, "armor_piercing", "", EDITOR_NONE)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseAmmo, base_ammo);

SXbaseAmmo::SXbaseAmmo(EntityManager * pMgr):
	BaseClass(pMgr),
	m_fStartSpeed(0.0f),
	m_fBulletMass(0.0f),
	m_fArmorPiercing(0.0f)
{
	m_bPickable = false;
	m_bInvStackable = false;
}
