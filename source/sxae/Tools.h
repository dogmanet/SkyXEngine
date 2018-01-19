
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __TOOLS_H
#define __TOOLS_H

#include <Windows.h>

class Tools
{
public:
	static int DlgPrompt(char * pOut, int * pMaxlen, const char * szText = "Enter text", const char * szTitle = "Prompt", const char * szDefault = "");
	static int DlgConfirm(const char * szText = "Are you shure?", const char * szTitle = "Confirm", int def=1);

	static LRESULT ProxyCommandProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif
