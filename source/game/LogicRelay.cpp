
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LogicRelay.h"

/*! \skydocent logic_relay
Логическое реле
*/

BEGIN_PROPTABLE(CLogicRelay)
	//! Включить
	DEFINE_INPUT(turnOn, "turnOn", "Turn On", PDF_NONE)
	//! Выключить
	DEFINE_INPUT(turnOff, "turnOff", "Turn Off", PDF_NONE)
	//! Переключить
	DEFINE_INPUT(toggle, "toggle", "Toggle", PDF_NONE)
	//! Активировать
	DEFINE_INPUT(trigger, "trigger", "Trigger", PDF_NONE)

	//! При активации
	DEFINE_OUTPUT(m_onTrigger, "OnTrigger", "On Trigger")

	//! Изначально выключен
	DEFINE_FLAG(LOGIC_START_DISABLED, "Start Disabled")
END_PROPTABLE()

REGISTER_ENTITY(CLogicRelay, logic_relay);

void CLogicRelay::turnOn(inputdata_t * pInputdata)
{
	m_isEnabled = true;
}

void CLogicRelay::turnOff(inputdata_t * pInputdata)
{
	m_isEnabled = false;
}

void CLogicRelay::toggle(inputdata_t * pInputdata)
{
	m_isEnabled = !m_isEnabled;
}

void CLogicRelay::trigger(inputdata_t * pInputdata)
{
	if(m_isEnabled)
	{
		FIRE_OUTPUT(m_onTrigger, pInputdata->pInflictor);
	}
}

void CLogicRelay::onPostLoad()
{
	m_isEnabled = !(getFlags() & LOGIC_START_DISABLED);

	BaseClass::onPostLoad();
}
