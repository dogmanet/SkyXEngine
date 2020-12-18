
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LightDirectional.h"

/*! \skydocent light_directional
Направленный источник света
*/

BEGIN_PROPTABLE(CLightDirectional)
	//! Внешний угол
	DEFINE_FIELD_FLOAT(m_fOuterAngle, 0, "angle", "Outer angle", EDITOR_TEXTFIELD)
	//! Внутренний угол
	DEFINE_FIELD_FLOAT(m_fInnerAngle, 0, "inner_angle", "Inner angle", EDITOR_TEXTFIELD)
	//! Верхний радиус
	DEFINE_FIELD_FLOAT(m_fRadiusTop, 0, "radius_top", "Radius top", EDITOR_TEXTFIELD)
END_PROPTABLE()

REGISTER_ENTITY(CLightDirectional, light_directional);

CLightDirectional::CLightDirectional()
{
	if(m_pLightSystem)
	{
		if(m_fInnerAngle > m_fOuterAngle)
		{
			m_fInnerAngle = m_fOuterAngle;
		}
		m_pLight = m_pLightSpot = m_pLightSystem->newSpot();
		//m_pLight->setDistance(m_fDist);
		m_pLight->setColor(float3(m_vColor) * m_vColor.w);
		m_pLightSpot->setDirection(SMQuaternion(-SM_PI, 'z'));
		m_pLightSpot->setOuterAngle(m_fOuterAngle);
		m_pLightSpot->setInnerAngle(m_fInnerAngle);
	}
}

CLightDirectional::~CLightDirectional()
{
	mem_release(m_pLight);
}

void CLightDirectional::onSync()
{
	BaseClass::onSync();

	if(m_pLightSpot)
	{
		m_pLightSpot->setDirection(m_vOrientation);
		m_pLightSpot->setOuterAngle(m_fOuterAngle);
		m_pLightSpot->setInnerAngle(m_fInnerAngle);
	}
#if 0
	if (SLight_GetTopRadius(m_idLight) != m_fRadiusTop)
		SLight_SetTopRadius(m_idLight, m_fRadiusTop);
#endif
}

