
#include "GameStateManager.h"

CGameStateManager::CGameStateManager():
	m_pActiveState(NULL)
{

}

CGameStateManager::~CGameStateManager()
{
	for(auto i = m_mStates.begin(); i; i++)
	{
		mem_delete(*(i.second));
	}
}

void CGameStateManager::addState(IGameState * pState, const char * szName)
{
	m_mStates[AAString(szName)] = pState;
}

void CGameStateManager::activate(const char * szName)
{
	if(m_pActiveState)
	{
		m_pActiveState->deactivate();
	}

	const AssotiativeArray<AAString, IGameState*>::Node *pNode;
	if(m_mStates.KeyExists(AAString(szName), &pNode))
	{
		m_pActiveState = *(pNode->Val);
		m_pActiveState->activate();
	}
	else
	{
		m_pActiveState = NULL;
	}
}
