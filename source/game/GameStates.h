#ifndef __GAMESTATES_H
#define __GAMESTATES_H

#include "IGameState.h"
#include <gui/guimain.h>

class CBaseline;

class CMainMenuGameState final: public IGameState
{
public:
	CMainMenuGameState();
	~CMainMenuGameState();
	void activate() override;
	void deactivate() override;

protected:
	gui::IDesktop *m_pDesktop;
};

class CIngameMenuGameState final: public IGameState
{
public:
	CIngameMenuGameState();
	~CIngameMenuGameState();
	void activate() override;
	void deactivate() override;

protected:
	gui::IDesktop *m_pDesktop;
};

class CIngameGameState final: public IGameState
{
public:
	void activate() override;
	void deactivate() override;

private:
	CBaseline *m_pBaseLine = NULL;
};

class CEditorState final: public IGameState
{
public:
	void activate() override;
	void deactivate() override;
};

#endif
