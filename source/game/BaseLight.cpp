
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseLight.h"

/*! \skydocent base_light
Базовый источник света
*/

BEGIN_PROPTABLE(CBaseLight)
	//! Цвет
	DEFINE_FIELD_VECTOR4FN(m_vColor, 0, "color", "Color", setColor, EDITOR_TEXTFIELD)
	//! Дальность
	DEFINE_FIELD_FLOAT(m_fDist, 0, "dist", "Distance", EDITOR_TEXTFIELD)
	//! Дальность дальняя
	//DEFINE_FIELD_FLOAT(m_fShadowDist, 0, "light_far", "Shadow far plane", EDITOR_TEXTFIELD)
	//! Интенсивность теней
	DEFINE_FIELD_FLOATFN(m_fShadowIntensity, 0, "shadow_intensity", "Shadow intensity", setShadowIntensity, EDITOR_TEXTFIELD)

	//! Тип тени
	DEFINE_FIELD_INTFN(m_iShadowType, 0, "type_shadow", "Type shadow", setShadowType, EDITOR_COMBOBOX)
		//COMBO_OPTION("None", "-1")   //!< Нет
		COMBO_OPTION("Static", "0")  //!< Статическая тень
		COMBO_OPTION("Dynamic", "1") //!< Динамическая тень
	EDITOR_COMBO_END()

	//! Включить
	DEFINE_INPUT(turnOn, "turnOn", "Turn On", PDF_NONE)
	//! Выключить
	DEFINE_INPUT(turnOff, "turnOff", "Turn Off", PDF_NONE)

	//! При включении
	DEFINE_OUTPUT(m_onTurnOn, "OnTurnOn", "On Turn On")
	//! При выключении
	DEFINE_OUTPUT(m_onTurnOff, "OnTurnOff", "On Turn Off")

	//! Изначально выключена
	DEFINE_FLAG(LIGHT_INITIALLY_DARK, "Initially dark")
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseLight, base_light);

CBaseLight::~CBaseLight()
{
	mem_release(m_pLight);
}

void CBaseLight::toggleEnable()
{
	setEnable(!m_isEnable);
}

void CBaseLight::turnOn(inputdata_t * pInputdata)
{
	if(!m_isEnable)
	{
		m_isEnable = true;
		SAFE_CALL(m_pLight, setEnabled, m_isEnable);
		FIRE_OUTPUT(m_onTurnOn, pInputdata->pInflictor);
	}
}

void CBaseLight::turnOff(inputdata_t * pInputdata)
{
	if(m_isEnable)
	{
		m_isEnable = false;
		SAFE_CALL(m_pLight, setEnabled, m_isEnable);
		FIRE_OUTPUT(m_onTurnOff, pInputdata->pInflictor);
	}
}

void CBaseLight::setEnable(bool isEnable)
{
	if(!m_isEnable && isEnable)
	{
		m_isEnable = isEnable;
		SAFE_CALL(m_pLight, setEnabled, m_isEnable);
		FIRE_OUTPUT(m_onTurnOn, this);
	}
	else if(m_isEnable && !isEnable)
	{
		m_isEnable = isEnable;
		SAFE_CALL(m_pLight, setEnabled, m_isEnable);
		FIRE_OUTPUT(m_onTurnOff, this);
	}
}

void CBaseLight::updateFlags()
{
	BaseClass::updateFlags();

	m_isEnable = !(getFlags() & LIGHT_INITIALLY_DARK);
	SAFE_CALL(m_pLight, setEnabled, m_isEnable);
}

bool CBaseLight::getMainColor(float3_t *pOut)
{
	if(pOut)
	{
		*pOut = (float3)m_vColor;
	}
	return(m_isEnable);
}

void CBaseLight::setPos(const float3 &pos)
{
	BaseClass::setPos(pos);

	SAFE_CALL(m_pLight, setPosition, pos);
}
