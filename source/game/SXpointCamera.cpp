#include "SXpointCamera.h"


BEGIN_PROPTABLE(SXpointCamera)
// empty
END_PROPTABLE()

REGISTER_ENTITY(SXpointCamera, point_camera);

SXpointCamera::SXpointCamera(EntityManager * pMgr):
	BaseClass(pMgr)
{
	m_pSXC = SGCore_CrCamera();
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
