
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseMag.h"

/*! \skydocent base_mag
Базовый класс магазина
*/

BEGIN_PROPTABLE(CBaseMag)
	//! Вместимость
	DEFINE_FIELD_INT(m_iCapacity, PDFF_NOEDIT | PDFF_NOEXPORT, "capacity", "", EDITOR_NONE)
	//! Текущая загрузка
	DEFINE_FIELD_INT(m_iCurrentLoad, PDFF_NOEDIT, "current_load", "", EDITOR_NONE)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseMag, base_mag);

CBaseMag::CBaseMag()
{
	m_addonType = WPN_ADDON_MAG;
}

int CBaseMag::getLoad()
{
	return(m_iCurrentLoad);
}

int CBaseMag::getCapacity()
{
	return(m_iCapacity);
}

void CBaseMag::load(int count)
{
	m_iCurrentLoad += count;
	if(m_iCurrentLoad > m_iCapacity)
	{
		m_iCurrentLoad = m_iCapacity;
	}
}
