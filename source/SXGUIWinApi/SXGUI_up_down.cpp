
#include <SXGUIWinApi\SXGUI_up_down.h>

#pragma once

SXGUIUpDown::SXGUIUpDown()
{

}

SXGUIUpDown::~SXGUIUpDown()
{

}

SXGUIUpDown::SXGUIUpDown(WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id,HWND buddy)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							UPDOWN_CLASS,
							0,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	SendMessage(this->GetHWND(), UDM_SETRANGE32, -65535, 65535);
    SendMessage(this->GetHWND(), UDM_SETPOS32, 0, 0);
		if(buddy)
			this->SetBuddy(buddy);

	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

SXGUIUpDown::SXGUIUpDown(WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,HWND buddy,WORD left_right)
{
	/*this->GetHWND() = CreateUpDownControl(
												WS_CHILD | WS_BORDER | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT | UDS_ARROWKEYS | (left_right == 1 ?UDS_ALIGNRIGHT : UDS_ALIGNLEFT), 
												x,y,width,heigth, 
												parent, 0, GetModuleHandle(0), buddy,
												0, 0, 0
											);*/
	this->WindowHandle = CreateWindowEx(
							0,
							UPDOWN_CLASS,
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT | UDS_ARROWKEYS | (left_right == 1 ?UDS_ALIGNRIGHT : UDS_ALIGNLEFT),
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	SendMessage(this->GetHWND(), UDM_SETRANGE32, -65535, 65535);
    SendMessage(this->GetHWND(), UDM_SETPOS32, 0, 0);
		if(buddy)
			this->SetBuddy(buddy);

	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

void SXGUIUpDown::SetMinMax(long min,long max)
{
	SendMessage(this->GetHWND(), UDM_SETRANGE32, min, max);
}

long SXGUIUpDown::SetPos(long pos)
{
	return SendMessage(this->GetHWND(), UDM_SETPOS32, 0, pos);
}

long SXGUIUpDown::GetMin()
{
	long min = 0;
	SendMessage(this->GetHWND(), UDM_GETRANGE32, (WPARAM)&min, 0);
	return min;
}

long SXGUIUpDown::GetMax()
{
	long max = 0;
	SendMessage(this->GetHWND(), UDM_GETRANGE32, 0, (LPARAM)&max);
	return max;
}

long SXGUIUpDown::GetPos()
{
	return SendMessage(this->GetHWND(), UDM_GETPOS32, 0, 0);
}

HWND SXGUIUpDown::SetBuddy(HWND buddy)
{
	return (HWND)SendMessage(this->GetHWND(), UDM_SETBUDDY, (WPARAM)buddy, 0);
}

HWND SXGUIUpDown::GetBuddy()
{
	return (HWND)SendMessage(this->GetHWND(), UDM_GETBUDDY, 0, 0);
}