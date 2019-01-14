#ifndef __GAMESTATES_H
#define __GAMESTATES_H

#include "IGameState.h"
#include <gui/guimain.h>

class CMainMenuGameState: public IGameState
{
public:
	CMainMenuGameState();
	~CMainMenuGameState();
	void activate();

protected:
	gui::IDesktop * m_pDesktop;
};

class CIngameMenuGameState: public IGameState
{
public:
	CIngameMenuGameState();
	~CIngameMenuGameState();
	void activate();

protected:
	gui::IDesktop * m_pDesktop;
};

class CIngameGameState: public IGameState
{
public:
	void activate();
	void deactivate();
};

class CEditorState: public IGameState
{
public:
	void activate();
	void deactivate();
};

class CServerIntermissionState: public IGameState
{
public:
	void activate();
	void deactivate();
};

class CServerState: public IGameState
{
public:
	void activate();
	void deactivate();
};

#endif
