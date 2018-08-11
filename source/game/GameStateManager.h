#ifndef __GAMESTATEMANAGER_H
#define __GAMESTATEMANAGER_H

#include <common/assotiativearray.h>
#include <common/aastring.h>

#include "IGameState.h"

class CGameStateManager
{
public:
	CGameStateManager();
	~CGameStateManager();

	void addState(IGameState * pState, const char * szName);
	void activate(const char * szName);

protected:
	AssotiativeArray<AAString, IGameState*> m_mStates;
	IGameState *m_pActiveState;
};

#endif
