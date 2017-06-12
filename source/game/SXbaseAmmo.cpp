#include "SXbaseAmmo.h"


BEGIN_PROPTABLE(SXbaseAmmo)
// empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(SXbaseAmmo, base_ammo);

SXbaseAmmo::SXbaseAmmo(EntityManager * pMgr):
	BaseClass(pMgr)
{
	m_bPickable = false;
	m_bInvStackable = false;
}
