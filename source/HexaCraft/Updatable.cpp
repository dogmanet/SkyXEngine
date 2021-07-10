#include "Updatable.h"

CUpdatable::CUpdatable(CChunkManager *pChunkManager):
	m_pChunkManager(pChunkManager)
{
	// do nothing
}

UINT CUpdatable::startup()
{
	//m_pChunkManager->init();
	//m_pChunkManager->generate();
	return(20);
}

void CUpdatable::shutdown()
{
	// do nothing
}


ID CUpdatable::run(float fDelta)
{
	return(-1);
}

void CUpdatable::sync()
{
	// do nothing
}
