
#include "SXGUI_track_bar.h"

SXGUITrackBar::SXGUITrackBar()
{

}

SXGUITrackBar::SXGUITrackBar(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							TRACKBAR_CLASS,
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

SXGUITrackBar::SXGUITrackBar(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							TRACKBAR_CLASS,
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | TBS_AUTOTICKS,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

void SXGUITrackBar::SetPos(int pos)
{
	SendMessage(this->GetHWND(),TBM_SETPOS,WPARAM(TRUE),LPARAM(pos));
}

int SXGUITrackBar::GetPos()
{
	return SendMessage(this->GetHWND(),TBM_GETPOS,0,0);
}

void SXGUITrackBar::SetMinMax(int min,int max)
{
	SendMessage(this->GetHWND(),TBM_SETRANGE,WPARAM(TRUE),MAKELPARAM(min,max));
}

int SXGUITrackBar::GetMin()
{
	return SendMessage(this->GetHWND(),TBM_GETRANGEMIN,0,0);
}

int SXGUITrackBar::GetMax()
{
	return SendMessage(this->GetHWND(),TBM_GETRANGEMAX,0,0);
}

bool SXGUITrackBar::SetTick(int pos)
{
		if(!SendMessage(this->GetHWND(),TBM_SETTIC,0,LPARAM(pos)))
			return false;
	return true;
}

int SXGUITrackBar::GetTick(int index)
{
	return SendMessage(this->GetHWND(),TBM_GETTIC,WPARAM(index),0);
}

void SXGUITrackBar::SetTickFrequency(int freq)
{
	SendMessage(this->GetHWND(),TBM_SETTICFREQ,WPARAM(freq),0);
}

bool SXGUITrackBar::SetBuddies(HWND hwnd_left,HWND hwnd_right)
{
		if(!hwnd_left && !hwnd_left)
			return false;
	
	SendMessage(this->GetHWND(),TBM_SETBUDDY,WPARAM(TRUE),LPARAM(hwnd_left));
	SendMessage(this->GetHWND(),TBM_SETBUDDY,WPARAM(FALSE),LPARAM(hwnd_right));

	return true;
}

HWND SXGUITrackBar::GetLeftBuddy()
{
	return HWND(SendMessage(this->GetHWND(),TBM_GETBUDDY,WPARAM(TRUE),0));
}

HWND SXGUITrackBar::GetRightBuddy()
{
	return HWND(SendMessage(this->GetHWND(),TBM_GETBUDDY,WPARAM(FALSE),0));
}