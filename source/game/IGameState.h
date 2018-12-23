#ifndef __IGAMESTATE_H
#define __IGAMESTATE_H

#include <gdefines.h>

class IGameState
{
public:
	virtual ~IGameState()
	{
	}

	virtual void activate() = 0;
	virtual void deactivate()
	{
	}
};

#endif
