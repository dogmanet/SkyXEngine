#include "BaseHandle.h"

/*! \skydocent base_handle
Базовый класс ручки
*/

BEGIN_PROPTABLE(CBaseHandle)
//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseHandle, base_handle);

CBaseHandle::CBaseHandle(CEntityManager * pMgr):
	BaseClass(pMgr)
{
	m_addonType = WPN_ADDON_HANDLE;
}