#include "BaseScope.h"

/*! \skydocent base_scope
Базовый класс прицела
*/

BEGIN_PROPTABLE(CBaseScope)
	//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseScope, base_scope);

CBaseScope::CBaseScope(CEntityManager * pMgr):
	BaseClass(pMgr)
{
	m_addonType = WPN_ADDON_SCOPE;
}