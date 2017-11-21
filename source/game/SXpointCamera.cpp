#include "SXpointCamera.h"

/*! \skydocent point_camera
Камера
*/

BEGIN_PROPTABLE(SXpointCamera)
// empty
END_PROPTABLE()

REGISTER_ENTITY(SXpointCamera, point_camera);

SXpointCamera::SXpointCamera(EntityManager * pMgr):
	BaseClass(pMgr)
{
	const float * r_default_fov = GET_PCVAR_FLOAT("r_default_fov");
	m_pSXC = SGCore_CrCamera();
	m_pSXC->SetFOV(SMToRadian(*r_default_fov));
}

SXpointCamera::~SXpointCamera()
{
	mem_release(m_pSXC);
}

ISXCamera * SXpointCamera::GetCamera()
{
	return(m_pSXC);
}

void SXpointCamera::OnSync()
{
	BaseClass::OnSync();

	m_pSXC->SetPosition(&(float3)m_vPosition);
	m_pSXC->SetOrientation(m_vOrientation);
}
