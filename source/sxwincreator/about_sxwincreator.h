
#ifndef __ABOUT_SXWINCREATOR_H
#define __ABOUT_SXWINCREATOR_H

#include <SXGUIWinApi/sxgui.h>
#include "sxwincreator.h"

namespace AboutSXWinCreator
{
	extern ISXGUIBaseWnd* JobWindow;
	extern ISXGUIStatic* StaticVerWinCreatorText;
	extern ISXGUIStatic* StaticThisIs;
	extern ISXGUIStatic* StaticVerGuiWinApiText;
	extern ISXGUIStatic* DevelodepBy;
	extern ISXGUIStatic* StaticVerWinCreatorVer;
	extern ISXGUIStatic* StaticVerGUIWinApiVer;

	void InitAllElements();

	void DeleteAllElements();
};

LRESULT HideAboutWndClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void AboutSXWinCreator::InitAllElements();

void AboutSXWinCreator::DeleteAllElements();

#endif