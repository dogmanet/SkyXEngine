
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LightPoint.h"

/*! \skydocent light_point
Точечный источник света
*/

BEGIN_PROPTABLE(CLightPoint)
	// empty
END_PROPTABLE()

REGISTER_ENTITY(CLightPoint, light_point);

CLightPoint::CLightPoint()
{
	if(m_pLightSystem)
	{
		m_pLight = m_pLightSystem->newPoint();
		//m_pLight->setDistance(m_fDist);
		m_pLight->setColor(float4(float3(m_vColor) * m_vColor.w, m_fDist));
	}
}

CLightPoint::~CLightPoint()
{
	mem_release(m_pLight);
}

