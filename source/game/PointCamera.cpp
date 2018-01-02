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
	m_pSXC->SetFOV(SMToRadian(*r_default_fov));
}

CPointCamera::~CPointCamera()
{
	mem_release(m_pSXC);
}

ISXCamera * CPointCamera::getCamera()
{
	return(m_pSXC);
}

void CPointCamera::onSync()
{
	BaseClass::onSync();

	m_pSXC->SetPosition(&(float3)m_vPosition);
	m_pSXC->SetOrientation(m_vOrientation);
}
