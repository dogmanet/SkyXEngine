#include "Updatable.h"

CUpdatable::CUpdatable(CAnimatedModelProvider *pAnimatedModelProvider, CDynamicModelProvider *pDynamicModelProvider):
	m_pAnimatedModelProvider(pAnimatedModelProvider),
	m_pDynamicModelProvider(pDynamicModelProvider)
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

	m_pDynamicModelProvider->update();

	return(-1);
}

void CUpdatable::sync()
{
	m_pAnimatedModelProvider->sync();
	m_pDynamicModelProvider->sync();
}
