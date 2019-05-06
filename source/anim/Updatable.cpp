#include "Updatable.h"

#include "animated.h"

extern AnimationManager *g_mgr;

UINT CUpdatable::startup()
{
	return(20);
}

void CUpdatable::shutdown()
{}


ID CUpdatable::run(float fDelta)
{
	g_mgr->update();

	return(-1);
}

void CUpdatable::sync()
{
	g_mgr->sync();
}
