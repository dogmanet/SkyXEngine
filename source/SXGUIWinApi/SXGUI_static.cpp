
#include <SXGUIWinApi\SXGUI_static.h>

#pragma once

SXGUIStatic::SXGUIStatic()
{

}

SXGUIStatic::SXGUIStatic(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							"STATIC",
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

SXGUIStatic::SXGUIStatic(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"STATIC",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE  | SS_LEFT,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

SXGUIStatic::SXGUIStatic(WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,bool vertical)
{
	this->WindowHandle = CreateWindowEx(
							WS_EX_TOPMOST,
							"STATIC",
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE  | SS_LEFT | (vertical ? SS_ETCHEDVERT : SS_ETCHEDHORZ),
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,handler);
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

int SXGUIStatic::Align()
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if(style & SS_RIGHT)
			return SXGUI_TEXT_ALIGN_RIGHT;
		else if(style & SS_CENTER)
			return SXGUI_TEXT_ALIGN_CENTER;
		else
			return SXGUI_TEXT_ALIGN_LEFT;
}

bool SXGUIStatic::Align(int align)
{
	if (align == SXGUI_TEXT_ALIGN_LEFT)
		return this->ModifyStyle(SS_LEFT,0);
	else if (align == SXGUI_TEXT_ALIGN_RIGHT)
		return this->ModifyStyle(SS_RIGHT,0);
	else if (align == SXGUI_TEXT_ALIGN_CENTER)
		return this->ModifyStyle(SS_CENTER,0);
	return false;
}

bool SXGUIStatic::WordWrap()
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if(!(style & SS_LEFTNOWORDWRAP))
			return true;
	return false;
}

bool SXGUIStatic::WordWrap(bool bf)
{
	return this->ModifyStyle(!bf ? SS_LEFTNOWORDWRAP : 0,bf ? SS_LEFTNOWORDWRAP : 0);
}
