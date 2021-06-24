
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseScope.h"

/*! \skydocent base_scope
Базовый класс прицела
*/

BEGIN_PROPTABLE(CBaseScope)
	//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseScope, base_scope);

CBaseScope::CBaseScope()
{
	m_addonType = WPN_ADDON_SCOPE;
}
