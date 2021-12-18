
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PropButton.h"

/*! \skydocent prop_button
Кнопка
*/

BEGIN_PROPTABLE(CPropButton)
	//! Включить
	DEFINE_INPUT(turnOn, "turnOn", "Turn On", PDF_NONE)
	//! Выключить
	DEFINE_INPUT(turnOff, "turnOff", "Turn Off", PDF_NONE)
	//! Нажать
	DEFINE_INPUT(press, "press", "Press", PDF_NONE)
	//! Отжать
	DEFINE_INPUT(pushUp, "pushUp", "Push up", PDF_NONE)
	//! Вжать
	DEFINE_INPUT(pushDown, "pushDown", "Push down", PDF_NONE)
	//! Переключить
	DEFINE_INPUT(toggle, "toggle", "Toggle", PDF_NONE)

	//! При нажатии
	DEFINE_OUTPUT(m_onPressed, "OnPressed", "On pressed")
	//! При отжатии
	DEFINE_OUTPUT(m_onPushUp, "OnPushUp", "On push up")
	//! При вжатии
	DEFINE_OUTPUT(m_onPushDown, "OnPushDown", "On push down")
	//! При переключении
	DEFINE_OUTPUT(m_onToggle, "OnToggle", "On toggle")
	//! При попытке нажатия в выключенном состоянии
	DEFINE_OUTPUT(m_onUseDisabled, "OnUseDisabled", "On use disabled")

	//! Изначально выключен
	DEFINE_FLAG(BUTTON_START_DISABLED, "Start Disabled")
	//! Имеет состояния нажата/отжата
	DEFINE_FLAG(BUTTON_TOGGLEABLE, "Toggleable")
	//! Изначально нажата
	DEFINE_FLAG(BUTTON_START_PRESSED, "Start pressed")
END_PROPTABLE()

REGISTER_ENTITY(CPropButton, prop_button, REC_MODEL_FIELD("model"));

void CPropButton::turnOn(inputdata_t * pInputdata)
{
	m_isEnabled = true;
}

void CPropButton::turnOff(inputdata_t * pInputdata)
{
	m_isEnabled = false;
}

void CPropButton::press(inputdata_t * pInputdata)
{
	if(m_isEnabled)
	{
		FIRE_OUTPUT(m_onPressed, pInputdata->pInflictor);
		if(m_isToggleable)
		{
			m_bState = !m_bState;
			FIRE_OUTPUT(m_onToggle, pInputdata->pInflictor);
			FIRE_OUTPUT(m_bState ? m_onPushDown : m_onPushUp, pInputdata->pInflictor);
		}
	}
	else
	{
		FIRE_OUTPUT(m_onUseDisabled, pInputdata->pInflictor);
	}
}

void CPropButton::pushUp(inputdata_t * pInputdata)
{
	if(!m_isToggleable)
	{
		return;
	}
	if(m_isEnabled)
	{
		if(m_bState)
		{
			m_bState = false;
			FIRE_OUTPUT(m_onToggle, pInputdata->pInflictor);
			FIRE_OUTPUT(m_onPushUp, pInputdata->pInflictor);
		}
	}
}

void CPropButton::pushDown(inputdata_t * pInputdata)
{
	if(!m_isToggleable)
	{
		return;
	}
	if(m_isEnabled)
	{
		if(!m_bState)
		{
			m_bState = true;
			FIRE_OUTPUT(m_onToggle, pInputdata->pInflictor);
			FIRE_OUTPUT(m_onPushUp, pInputdata->pInflictor);
		}
	}
}

void CPropButton::toggle(inputdata_t * pInputdata)
{
	if(!m_isToggleable)
	{
		return;
	}
	if(m_isEnabled)
	{
		m_bState = !m_bState;
		FIRE_OUTPUT(m_onToggle, pInputdata->pInflictor);
		FIRE_OUTPUT(m_bState ? m_onPushDown : m_onPushUp, pInputdata->pInflictor);
	}
}

void CPropButton::onPostLoad()
{
	m_isEnabled = (getFlags() & BUTTON_START_DISABLED) == 0;
	m_isToggleable = (getFlags() & BUTTON_TOGGLEABLE) != 0;
	m_bState = (getFlags() & BUTTON_START_PRESSED) != 0;

	BaseClass::onPostLoad();
}

void CPropButton::onUse(CBaseEntity *pUser)
{
	inputdata_t inputdata = {0};
	inputdata.pActivator = pUser;
	inputdata.pInflictor = pUser;
	inputdata.type = PDF_NONE;

	press(&inputdata);

	BaseClass::onUse(pUser);
}
