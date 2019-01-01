
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseLight.h"

#include <light/sxlight.h>

/*! \skydocent base_light
Базовый источник света
*/

BEGIN_PROPTABLE(CBaseLight)
	//! Цвет
	DEFINE_FIELD_VECTOR(m_vColor, 0, "color", "Color", EDITOR_TEXTFIELD)
	//! Дальность
	DEFINE_FIELD_FLOAT(m_fDist, 0, "dist", "Distance", EDITOR_TEXTFIELD)
	//! Дальность дальняя
	DEFINE_FIELD_FLOAT(m_fShadowDist, 0, "light_far", "Shadow far plane", EDITOR_TEXTFIELD)
	//! Интенсивность теней
	DEFINE_FIELD_FLOAT(m_fShadowIntensity, 0, "shadow_intensity", "Shadow intensity", EDITOR_TEXTFIELD)
	//! Связанный свет (повторяет его состояние включения)
	DEFINE_FIELD_ENTITYFN(m_pLinkedTo, 0, "linked_to", "Linked light to", setLinkedTo, EDITOR_TEXTFIELD)

	//! Тип тени
	DEFINE_FIELD_INT(m_iShadowType, 0, "type_shadow", "Type shadow", EDITOR_COMBOBOX)
		COMBO_OPTION("None", "-1")   //!< Нет
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

CBaseLight::CBaseLight(CEntityManager * pMgr):
BaseClass(pMgr)
{
	m_vColor = float3(1, 1, 1);
	m_fDist = 10;
	m_fShadowDist = m_fDist;
	m_iShadowType = 1;
	m_fShadowIntensity = 1;
}

CBaseLight::~CBaseLight()
{
	if(m_pLinkedTo)
	{
		((CBaseLight*)m_pLinkedTo)->removeLinkedLight(this);
	}
}

void CBaseLight::toggleEnable()
{
	setEnable(!m_isEnable);
}

void CBaseLight::onSync()
{
	BaseClass::onSync();

	if(ID_VALID(m_idLight))
	{
		if(SLight_GetEnable(m_idLight) != m_isEnable)
			SLight_SetEnable(m_idLight, m_isEnable);

		float3 vec;
		SLight_GetPos(m_idLight, &vec, false);

		if(vec.x != m_vPosition.x || vec.y != m_vPosition.y || vec.z != m_vPosition.z)
			SLight_SetPos(m_idLight, &(float3)m_vPosition, false);

		SLight_SetColor(m_idLight, &(float3)m_vColor);

		if(SLight_GetDist(m_idLight) != m_fDist)
		{
			SLight_SetDist(m_idLight, m_fDist, true);
			m_fShadowDist = m_fDist;
		}

		SLight_SetShadowIntensity(m_idLight, m_fShadowIntensity);

		if(SLight_GetShadowLocalFar(m_idLight) != m_fShadowDist)
			SLight_SetShadowLocalFar(m_idLight, m_fShadowDist);

		if(SLight_GetTypeShadowed(m_idLight) != m_iShadowType)
			SLight_SetTypeShadowed(m_idLight, (LTYPE_SHADOW)m_iShadowType);
	}
}


void CBaseLight::turnOn(inputdata_t * pInputdata)
{
	if(!m_isEnable)
	{
		m_isEnable = true;
		FIRE_OUTPUT(m_onTurnOn, pInputdata->pInflictor);
	}
	for(int i = 0, l = m_vpLinkedLights.size(); i < l; ++i)
	{
		m_vpLinkedLights[i]->turnOn(pInputdata);
	}
}

void CBaseLight::turnOff(inputdata_t * pInputdata)
{
	if(m_isEnable)
	{
		m_isEnable = false;
		FIRE_OUTPUT(m_onTurnOff, pInputdata->pInflictor);
	}
	for(int i = 0, l = m_vpLinkedLights.size(); i < l; ++i)
	{
		m_vpLinkedLights[i]->turnOff(pInputdata);
	}
}

void CBaseLight::setEnable(bool isEnable)
{
	if(!m_isEnable && isEnable)
	{
		m_isEnable = isEnable;
		FIRE_OUTPUT(m_onTurnOn, this);
	}
	else if(m_isEnable && !isEnable)
	{
		m_isEnable = isEnable;
		FIRE_OUTPUT(m_onTurnOff, this);
	}

	for(int i = 0, l = m_vpLinkedLights.size(); i < l; ++i)
	{
		m_vpLinkedLights[i]->setEnable(isEnable);
	}
}

void CBaseLight::updateFlags()
{
	BaseClass::updateFlags();

	m_isEnable = !(getFlags() & LIGHT_INITIALLY_DARK);

	for(int i = 0, l = m_vpLinkedLights.size(); i < l; ++i)
	{
		if(m_isEnable)
		{
			m_vpLinkedLights[i]->setFlags(m_vpLinkedLights[i]->getFlags() & ~LIGHT_INITIALLY_DARK);
		}
		else
		{
			m_vpLinkedLights[i]->setFlags(m_vpLinkedLights[i]->getFlags() | LIGHT_INITIALLY_DARK);
		}
		m_vpLinkedLights[i]->updateFlags();
	}
}

bool CBaseLight::getMainColor(float3_t *pOut)
{
	if(pOut)
	{
		*pOut = m_vColor;
	}
	return(m_isEnable);
}

void CBaseLight::setLinkedTo(CBaseEntity *pEnt)
{
	if(m_pLinkedTo)
	{
		((CBaseLight*)m_pLinkedTo)->removeLinkedLight(this);
	}
	m_pLinkedTo = pEnt;
	if(m_pLinkedTo)
	{
		((CBaseLight*)m_pLinkedTo)->addLinkedLight(this);
		if((((CBaseLight*)m_pLinkedTo)->getFlags() & LIGHT_INITIALLY_DARK))
		{
			setFlags(getFlags() | LIGHT_INITIALLY_DARK);
		}
		else
		{
			setFlags(getFlags() & ~LIGHT_INITIALLY_DARK);
		}
		updateFlags();
	}
}

void CBaseLight::addLinkedLight(CBaseLight *pEnt)
{
	if(!pEnt)
	{
		return;
	}
	for(int i = 0, l = m_vpLinkedLights.size(); i < l; ++i)
	{
		if(m_vpLinkedLights[i] == pEnt)
		{
			return;
		}
	}
	m_vpLinkedLights.push_back(pEnt);
}

void CBaseLight::removeLinkedLight(CBaseLight *pEnt)
{
	if(!pEnt)
	{
		return;
	}
	for(int i = 0, l = m_vpLinkedLights.size(); i < l; ++i)
	{
		if(m_vpLinkedLights[i] == pEnt)
		{
			m_vpLinkedLights.erase(i);
			return;
		}
	}
}
