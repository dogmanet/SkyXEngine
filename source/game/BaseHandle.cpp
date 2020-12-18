
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseHandle.h"

/*! \skydocent base_handle
Базовый класс ручки
*/

BEGIN_PROPTABLE(CBaseHandle)
//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseHandle, base_handle);

CBaseHandle::CBaseHandle()
{
	m_addonType = WPN_ADDON_HANDLE;
}