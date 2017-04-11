
#include <SXGUIWinApi\SXGUI_radio_button.h>

#pragma once

SXGUIRadioButton::SXGUIRadioButton()
{

}

SXGUIRadioButton::SXGUIRadioButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
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

SXGUIRadioButton::SXGUIRadioButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"BUTTON",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | BS_AUTORADIOBUTTON,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

void SXGUIRadioButton::SetCheck(bool check)
{
	SendMessage(this->GetHWND(),BM_SETCHECK,(check ? BST_CHECKED : BST_UNCHECKED),0);
}

bool SXGUIRadioButton::GetCheck()
{
	return SendMessage(this->GetHWND(),BM_GETCHECK,0,0) ? true : false;
}