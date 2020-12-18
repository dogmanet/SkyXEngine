
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

REGISTER_ENTITY(CLightSun, light_sun);

CLightSun::CLightSun()
{
	if(m_pLightSystem)
	{
		m_pLight = m_pSun = m_pLightSystem->newSun();
		m_pLight->setColor(float4(float3(m_vColor) * m_vColor.w, m_fDist));
	}
}

CLightSun::~CLightSun()
{
	mem_release(m_pLight);
}

void CLightSun::onSync()
{
	BaseClass::onSync();

	if(m_pSun)
	{
		m_pSun->setDirection(m_vOrientation);
	}
}
