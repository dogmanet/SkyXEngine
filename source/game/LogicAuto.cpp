
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LogicAuto.h"

/*! \skydocent logic_relay
Логическое реле
*/

BEGIN_PROPTABLE(CLogicAuto)
	//! При активации
	DEFINE_OUTPUT(m_onTrigger, "OnTrigger", "On Trigger")

	//! Задежка перед активацией
	DEFINE_FIELD_FLOAT(m_fDelay, PDFF_NONE, "delay", "Delay before trigger", EDITOR_TIMEFIELD)
END_PROPTABLE()

REGISTER_ENTITY(CLogicAuto, logic_auto);

CLogicAuto::CLogicAuto(CEntityManager * pMgr):
	BaseClass(pMgr)
{
}

void CLogicAuto::activate()
{
	SET_TIMEOUT(doTrigger, m_fDelay);
}

void CLogicAuto::doTrigger(float fDT)
{
	FIRE_OUTPUT(m_onTrigger, NULL);
}
