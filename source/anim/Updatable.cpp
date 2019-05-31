#include "Updatable.h"

CUpdatable::CUpdatable(CAnimatedModelProvider *pAnimatedModelProvider):
	m_pAnimatedModelProvider(pAnimatedModelProvider)
{

}

UINT CUpdatable::startup()
{
	return(20);
}

void CUpdatable::shutdown()
{
}


ID CUpdatable::run(float fDelta)
{
	m_pAnimatedModelProvider->update(fDelta);

	return(-1);
}

void CUpdatable::sync()
{
	m_pAnimatedModelProvider->sync();
}
