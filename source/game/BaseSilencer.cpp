#include "BaseSilencer.h"

/*! \skydocent base_silencer
Базовый класс глушителя
*/

BEGIN_PROPTABLE(CBaseSilencer)
//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseSilencer, base_silencer);

CBaseSilencer::CBaseSilencer(CEntityManager * pMgr):
	BaseClass(pMgr)
{
	m_addonType = WPN_ADDON_SILENCER;
}