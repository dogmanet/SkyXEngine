
#include <SXGUIWinApi\SXGUI_hint.h>

#pragma once

SXGUIHint::SXGUIHint()
{
	WindowHandle = 0;ParentHandle = 0;IsVisible = false;
}

SXGUIHint::SXGUIHint(HWND parent)
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

SXGUIHint::~SXGUIHint()
{
	::DestroyWindow(this->WindowHandle);
}

HWND SXGUIHint::GetHWND()
{
	return WindowHandle;
}

void SXGUIHint::SetDelayTime(DWORD init,DWORD autopop)
{
	SendMessage(WindowHandle,TTM_SETDELAYTIME,TTDT_AUTOPOP,autopop);
	SendMessage(WindowHandle,TTM_SETDELAYTIME,TTDT_INITIAL,init);
}

void SXGUIHint::SetText(const char* text)
{
	SendMessage(WindowHandle, TTM_DELTOOL, 0, (LPARAM) &Hint);
	sprintf(this->hint,"%s",text);
	Hint.lpszText = this->hint;
	SendMessage(WindowHandle, TTM_ADDTOOL, 0, (LPARAM) &Hint);
	//SendMessage(WindowHandle, TTM_SETTOOLINFO , 0, (LPARAM) &Hint);
}

char* SXGUIHint::GetText()
{
	return Hint.lpszText;
}

void SXGUIHint::GetText(char* buf)
{
		if(buf)
			sprintf(buf,"%s",Hint.lpszText);
}

bool SXGUIHint::Parent(HWND parent)
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

HWND SXGUIHint::Parent()
{
	return this->WindowHandle;
}

void SXGUIHint::Visible(bool bf)
{
	SendMessage(WindowHandle,TTM_ACTIVATE,bf ? 1 : 0,0);
	IsVisible = bf;
}

bool SXGUIHint::Visible()
{
	return IsVisible;
}