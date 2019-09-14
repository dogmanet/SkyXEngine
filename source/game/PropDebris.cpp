
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PropDebris.h"

/*! \skydocent prop_debris
Разбиваемый объект
*/

BEGIN_PROPTABLE(CPropDebris)
END_PROPTABLE()

REGISTER_ENTITY(CPropDebris, prop_debris);

CPropDebris::CPropDebris(CEntityManager *pMgr):BaseClass(pMgr)
{
	setCollisionGroup(CG_DEBRIS);
}

void CPropDebris::sheduleRemove()
{
	static const float *s_pMinTime = GET_PCVAR_FLOAT("lvl_debris_remove_time_min");
	static const float *s_pMaxTime = GET_PCVAR_FLOAT("lvl_debris_remove_time_max");

	SET_TIMEOUT(checkRemove, randf(*s_pMinTime, *s_pMaxTime));
}

void CPropDebris::checkRemove(float fDT)
{
	//@TODO: Reimplement me
#if 0
	if(!m_pAnimPlayer->isVisibleFor(SX_ANIM_DEFAULT_VISCALCOBJ))
	{
		REMOVE_ENTITY(this);
	}
	else
	{
		SET_TIMEOUT(checkRemove, randf(3.0f, 6.0f));
	}
#endif
}
