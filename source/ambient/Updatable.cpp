#include "Updatable.h"

CUpdatable::CUpdatable(CSkyBox *pSkyBox):
	m_pSkyBox(pSkyBox)
{
	// do nothing
}

UINT CUpdatable::startup()
{
	return(20);
}

void CUpdatable::shutdown()
{
	// do nothing
}


ID CUpdatable::run(float fDelta)
{
	m_pSkyBox->update(fDelta);

	return(-1);
}

void CUpdatable::sync()
{
	// do nothing
}
