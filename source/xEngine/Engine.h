#ifndef __ENGINE_H
#define __ENGINE_H

#include "IXEngine.h"

class CEngine: public IXEngine
{
	friend class CMainLoopTask;
public:
	CEngine(int argc, char **argv, const char *szName);
	~CEngine();

	int XMETHODCALLTYPE start() override;

	bool XMETHODCALLTYPE initGraphics(XWINDOW_OS_HANDLE hWindow, IXEngineCallback *pCallback) override;
	bool XMETHODCALLTYPE initServer() override;

	INT_PTR XMETHODCALLTYPE onMessage(UINT msg, WPARAM wParam, LPARAM lParam) override;
	IXCore* XMETHODCALLTYPE getCore() override;

protected:

	void initPaths();

	bool runFrame();

	IXEngineCallback *m_pCallback = NULL;

	IXCore *m_pCore = NULL;
};

#endif
