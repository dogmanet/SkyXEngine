#ifndef __UPDATABLE_H
#define __UPDATABLE_H

#include <xcommon/IXUpdatable.h>

class CUpdatable: public IXUpdatable
{
public:
	UINT startup() override;
	void shutdown() override;

	ID run(float fDelta) override;
	void sync() override;
};

#endif
