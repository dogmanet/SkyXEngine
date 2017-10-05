#include "BaseHandle.h"

BEGIN_PROPTABLE(CBaseHandle)
//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseHandle, base_handle);

CBaseHandle::CBaseHandle(EntityManager * pMgr):
	BaseClass(pMgr)
{
	m_addonType = WPN_ADDON_HANDLE;
}