
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseSilencer.h"

/*! \skydocent base_silencer
Базовый класс глушителя
*/

BEGIN_PROPTABLE(CBaseSilencer)
//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseSilencer, base_silencer);

CBaseSilencer::CBaseSilencer()
{
	m_addonType = WPN_ADDON_SILENCER;
}
