
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LightPoint.h"

#include <light/sxlight.h>

/*! \skydocent light_point
Точечный источник света
*/

BEGIN_PROPTABLE(CLightPoint)
	// empty
END_PROPTABLE()

REGISTER_ENTITY(CLightPoint, light_point);

CLightPoint::CLightPoint(CEntityManager * pMgr):BaseClass(pMgr)
{
	if(!m_pMgr->isServerMode())
	{
		m_idLight = SLight_CreatePoint(&float3(0, 0, 0), m_fDist, &(float3)m_vColor, false, true);
	}
}

CLightPoint::~CLightPoint()
{
	if(ID_VALID(m_idLight))
	{
		SLight_DeleteLight(m_idLight);
	}
}

