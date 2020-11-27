
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

CPointCamera::CPointCamera(CEntityManager * pMgr):
	BaseClass(pMgr)
{
	const float * r_default_fov = GET_PCVAR_FLOAT("r_default_fov");
	m_pSXC = SGCore_CrCamera();
	m_pSXC->setFOV(SMToRadian(*r_default_fov));
}

CPointCamera::~CPointCamera()
{
	mem_release(m_pSXC);
}

ICamera * CPointCamera::getCamera()
{
	return(m_pSXC);
}

void CPointCamera::onSync()
{
	BaseClass::onSync();

	m_pSXC->setPosition(m_vPosition);
	m_pSXC->setOrientation(m_vOrientation);
}
