#include "BaseSilencer.h"

BEGIN_PROPTABLE(CBaseSilencer)
//empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseSilencer, base_silencer);

CBaseSilencer::CBaseSilencer(EntityManager * pMgr):
	BaseClass(pMgr)
{
	m_addonType = WPN_ADDON_SILENCER;
}