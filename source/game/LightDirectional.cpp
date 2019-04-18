
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LightDirectional.h"

/*! \skydocent light_directional
Направленный источник света
*/

BEGIN_PROPTABLE(CLightDirectional)
	//! Угол
	DEFINE_FIELD_FLOAT(m_fAngle, 0, "angle", "Angle", EDITOR_TEXTFIELD)
	//! Верхний радиус
	DEFINE_FIELD_FLOAT(m_fRadiusTop, 0, "radius_top", "Radius top", EDITOR_TEXTFIELD)
END_PROPTABLE()

REGISTER_ENTITY(CLightDirectional, light_directional);

CLightDirectional::CLightDirectional(CEntityManager *pMgr):BaseClass(pMgr)
{
	m_fAngle = SM_PI * 0.4f;
	m_fRadiusTop = 0.01f;
	if(m_pLightSystem)
	{
		m_pLight = m_pLightSpot = m_pLightSystem->createSpot();
		//m_pLight->setDistance(m_fDist);
		m_pLight->setColor(m_vColor);
		m_pLightSpot->setDirection(SMQuaternion(-SM_PI, 'z'));
		m_pLightSpot->setOuterAngle(m_fAngle);
		m_pLightSpot->setInnerAngle(m_fAngle * 0.7f);
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
		m_pLightSpot->setOuterAngle(m_fAngle);
		m_pLightSpot->setInnerAngle(m_fAngle * 0.7f);
	}
#if 0
	if (SLight_GetTopRadius(m_idLight) != m_fRadiusTop)
		SLight_SetTopRadius(m_idLight, m_fRadiusTop);
#endif
}

