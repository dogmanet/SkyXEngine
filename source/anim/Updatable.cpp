#include "Updatable.h"

#include "animated.h"
#include "AnimatedModelProvider.h"

extern CAnimatedModelProvider *g_pAnimatedModelProvider;
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
	g_pAnimatedModelProvider->update(fDelta);

	return(-1);
}

void CUpdatable::sync()
{
	g_mgr->sync();
	g_pAnimatedModelProvider->sync();
}
