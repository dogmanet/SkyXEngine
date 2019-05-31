#ifndef __UPDATABLE_H
#define __UPDATABLE_H

#include <xcommon/IXUpdatable.h>
#include "AnimatedModelProvider.h"

class CUpdatable: public IXUpdatable
{
public:
	CUpdatable(CAnimatedModelProvider *pAnimatedModelProvider);

	UINT startup() override;
	void shutdown() override;

	ID run(float fDelta) override;
	void sync() override;

protected:
	CAnimatedModelProvider *m_pAnimatedModelProvider;
};

#endif
