#include "Updatable.h"

CUpdatable::CUpdatable(CAmbient *pAmbient):
	m_pAmbient(pAmbient)
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
	m_pAmbient->update(fDelta);

	return(-1);
}

void CUpdatable::sync()
{
	// do nothing
}
