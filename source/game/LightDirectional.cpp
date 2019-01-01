
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "LightDirectional.h"

#include <light/sxlight.h>

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
	if(!m_pMgr->isServerMode())
	{
		m_idLight = SLight_CreateDirection(&float3(0, 0, 0), m_fDist, &(float3)m_vColor, &SMQuaternion(-SM_PI, 'z'), m_fRadiusTop, m_fAngle, true);
	}

	//@FIXME: Что это??
	//float3 f = LIGHTS_DIR_BASE;
	//float3 a = SMVector3Cross(f, f);
	//float ang = acosf(SMVector3Dot(f, f));
	//m_vOrientation = SMQuaternion(a, ang);
}

CLightDirectional::~CLightDirectional()
{
	if(ID_VALID(m_idLight))
	{
		SLight_DeleteLight(m_idLight);
	}
}

void CLightDirectional::onSync()
{
	BaseClass::onSync();

	SMQuaternion curr_rot;
	if(ID_VALID(m_idLight))
	{
		SLight_GetOrient(m_idLight, &curr_rot);

		if(curr_rot.x != m_vOrientation.x || curr_rot.y != m_vOrientation.y || curr_rot.z != m_vOrientation.z || curr_rot.w != m_vOrientation.w)
			SLight_SetOrient(m_idLight, &m_vOrientation);

		if(SLight_GetAngle(m_idLight) != m_fAngle)
			SLight_SetAngle(m_idLight, m_fAngle);

		if(SLight_GetTopRadius(m_idLight) != m_fRadiusTop)
			SLight_SetTopRadius(m_idLight, m_fRadiusTop);
	}
}

