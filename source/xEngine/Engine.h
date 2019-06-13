#ifndef __ENGINE_H
#define __ENGINE_H

#include "IXEngine.h"

enum WANT_RESIZE
{
	WR_NONE = 0x0000,
	WR_WIDTH = 0x0001,
	WR_HEIGHT = 0x0002,
	WR_WINDOWED = 0x0004,
	WR_BORDERLESS = 0x0008
};

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

	void onRWinWidthChanged();
	void onRWinHeightChanged();
	void onRWinWindowedChanged();
	void onRWinBorderlessChanged();

	bool checkResize();

	IXEngineCallback *m_pCallback = NULL;

	IXCore *m_pCore = NULL;

	WANT_RESIZE m_wantResize = WR_NONE;
};

DEFINE_ENUM_FLAG_OPERATORS(WANT_RESIZE);

#endif
