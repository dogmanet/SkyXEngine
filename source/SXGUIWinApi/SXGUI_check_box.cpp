
#include <SXGUIWinApi\SXGUI_check_box.h>

#pragma once

SXGUICheckBox::SXGUICheckBox()
{

}

SXGUICheckBox::SXGUICheckBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							"BUTTON",
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

SXGUICheckBox::SXGUICheckBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,bool cb_3_state)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"BUTTON",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | (cb_3_state ? BS_AUTO3STATE : BS_AUTOCHECKBOX),
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

void SXGUICheckBox::SetCheck(int check)
{
	SendMessage(this->GetHWND(),BM_SETCHECK,check,0);
}

int SXGUICheckBox::GetCheck()
{
	return SendMessage(this->GetHWND(),BM_GETCHECK,0,0);
}