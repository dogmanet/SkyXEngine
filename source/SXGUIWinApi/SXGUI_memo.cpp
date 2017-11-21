
#include <SXGUIWinApi\SXGUI_memo.h>

#pragma once

SXGUIMemo::SXGUIMemo()
{

}

SXGUIMemo::SXGUIMemo(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							"EDIT",
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

SXGUIMemo::SXGUIMemo(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"EDIT",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | ES_LEFT | ES_WANTRETURN | ES_MULTILINE,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);

	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

bool SXGUIMemo::ScrollBars(bool h,bool v)
{
	return SXGUIFuctinon::ScroolBars(this,h,v);
}

bool SXGUIMemo::ScrollBarV()
{
	return SXGUIFuctinon::ScrollBarV(this);
}

bool SXGUIMemo::ScrollBarH()
{
	return SXGUIFuctinon::ScrollBarH(this);
}


bool SXGUIMemo::WordWrap()
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if((style & ES_AUTOVSCROLL) && !(style & ES_AUTOHSCROLL))
			return true;
	return false;
}

bool SXGUIMemo::WordWrap(bool bf)
{
	return this->ModifyStyle(bf ? ES_AUTOVSCROLL : 0,!bf ? ES_AUTOVSCROLL : 0);
}

bool SXGUIMemo::ScrollLine(WORD scroll,WORD dir,int count)
{
	return SXGUIFuctinon::ScrollLine(this,scroll,dir,count);
}