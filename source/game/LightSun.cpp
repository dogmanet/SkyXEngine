
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LightSun.h"

/*! \skydocent light_sun
Солнце
*/

BEGIN_PROPTABLE(CLightSun)
	// empty
END_PROPTABLE()

REGISTER_ENTITY(CLightSun, light_sun, REC_ICON("dev_light_sun"));

CLightSun::CLightSun()
{
	if(m_pLightSystem)
	{
		m_pLight = m_pSun = m_pLightSystem->newSun();
		m_pLight->setColor(float4(float3(m_vColor) * m_vColor.w, m_fDist));
	}
}

void CLightSun::setOrient(const SMQuaternion &q)
{
	BaseClass::setOrient(q);

	SAFE_CALL(m_pSun, setDirection, q);
}
