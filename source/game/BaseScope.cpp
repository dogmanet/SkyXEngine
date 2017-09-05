#include "BaseScope.h"

BEGIN_PROPTABLE(CBaseScope)
	//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseScope, base_scope);

CBaseScope::CBaseScope(EntityManager * pMgr):
	BaseClass(pMgr)
{
	m_addonType = WPN_ADDON_SCOPE;
}