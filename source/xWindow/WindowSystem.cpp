#include "WindowSystem.h"
#include "Window.h"

#if defined(_GTK_WINDOWS)
#	include <gtk/gtk.h>
#endif

IXWindow* XMETHODCALLTYPE CWindowSystem::createWindow(const XWINDOW_DESC *pWindowDesc, IXWindowCallback *pCallback, IXWindow *pParent)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	return(new CWindow(hInst, m_uNextWindowId++, pWindowDesc, pCallback, pParent));
}

bool XMETHODCALLTYPE CWindowSystem::processMessages(UINT uMaxMessages)
{
	UINT uProcessed = 0;

#if defined(_WINDOWS)
	MSG msg = {0};

	while((!uMaxMessages || (uProcessed++ < uMaxMessages)) && PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if(msg.message == WM_QUIT)
		{
			return(false);
		}

		/*
		if(g_hAccelTable && TranslateAccelerator(GetParent((HWND)SGCore_GetHWND()), g_hAccelTable, &msg))
		{
			continue;
		}
		*/

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
#else
	while((!uMaxMessages || (uProcessed++ < uMaxMessages)) && gtk_events_pending())
	{
		if(gtk_main_iteration())
		{
			return(false);
		}
	}
#endif

	return(true);
}

C XWAPI IXWindowSystem* XWindowInit()
{
	return(new CWindowSystem());
}
