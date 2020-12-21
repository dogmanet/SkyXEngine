
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PointCamera.h"

/*! \skydocent point_camera
Камера
*/

BEGIN_PROPTABLE(CPointCamera)
// empty
END_PROPTABLE()

REGISTER_ENTITY(CPointCamera, point_camera);

CPointCamera::CPointCamera()
{
	const float * r_default_fov = GET_PCVAR_FLOAT("r_default_fov");
	m_pSXC = SGCore_CrCamera();
	m_pSXC->setFOV(SMToRadian(*r_default_fov));
}

CPointCamera::~CPointCamera()
{
	mem_release(m_pSXC);
}

ICamera* CPointCamera::getCamera()
{
	return(m_pSXC);
}

void CPointCamera::setPos(const float3 &pos)
{
	BaseClass::setPos(pos);

	m_pSXC->setPosition(pos);
}

void CPointCamera::setOrient(const SMQuaternion &q)
{
	BaseClass::setOrient(q);

	m_pSXC->setOrientation(q);
}
