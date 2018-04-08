
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "hint.h"


CGUIHint::CGUIHint()
{
	WindowHandle = 0;ParentHandle = 0;IsVisible = false;
}

CGUIHint::CGUIHint(HWND parent)
{
	ParentHandle = parent;

	static unsigned int uid = 0;
	uid++;
	RECT rect;

	WindowHandle = CreateWindowEx(	WS_EX_TOPMOST,
									TOOLTIPS_CLASS,
									NULL,
									WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,        
									CW_USEDEFAULT,
 									CW_USEDEFAULT,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									ParentHandle,
									NULL,
									GetModuleHandle(NULL),
									NULL
								);
 
	SetWindowPos(	WindowHandle,
					HWND_TOPMOST,
					0,
					0,
					0,
					0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
				);
 
	GetClientRect (ParentHandle, &rect);

	hint[0] = 0;

	Hint.cbSize = TTTOOLINFO_V1_SIZE;
	Hint.uFlags = TTF_SUBCLASS;
	Hint.hwnd = ParentHandle;
	//Hint.hinst = GetModuleHandle(NULL);
	Hint.uId = uid;
	Hint.lpszText = hint;

    Hint.rect.left = rect.left;    
    Hint.rect.top = rect.top;
    Hint.rect.right = rect.right;
    Hint.rect.bottom = rect.bottom;

	SendMessage(WindowHandle, TTM_ADDTOOL, 0, (LPARAM) &Hint);

	IsVisible = true;
}

CGUIHint::~CGUIHint()
{
	::DestroyWindow(this->WindowHandle);
}

HWND CGUIHint::getHWND()
{
	return WindowHandle;
}

void CGUIHint::setDelayTime(UINT init, UINT autopop)
{
	SendMessage(WindowHandle,TTM_SETDELAYTIME,TTDT_AUTOPOP,autopop);
	SendMessage(WindowHandle,TTM_SETDELAYTIME,TTDT_INITIAL,init);
}

void CGUIHint::setText(const char* text)
{
	SendMessage(WindowHandle, TTM_DELTOOL, 0, (LPARAM) &Hint);
	sprintf(this->hint,"%s",text);
	Hint.lpszText = this->hint;
	SendMessage(WindowHandle, TTM_ADDTOOL, 0, (LPARAM) &Hint);
	//SendMessage(WindowHandle, TTM_SETTOOLINFO , 0, (LPARAM) &Hint);
}

const char* CGUIHint::getText()
{
	return Hint.lpszText;
}

void CGUIHint::getText(char* buf)
{
		if(buf)
			sprintf(buf,"%s",Hint.lpszText);
}

bool CGUIHint::setParent(HWND parent)
{
	HWND OldParent = GetParent(this->WindowHandle);
	HWND tmp_parent = SetParent(this->WindowHandle,parent);
		if(tmp_parent = OldParent)
		{
			ParentHandle = parent;
			return true;
		}
	return false;
}

HWND CGUIHint::getParent()
{
	return this->WindowHandle;
}

void CGUIHint::setVisible(bool bf)
{
	SendMessage(WindowHandle,TTM_ACTIVATE,bf ? 1 : 0,0);
	IsVisible = bf;
}

bool CGUIHint::getVisible()
{
	return IsVisible;
}