
#include <SXGUIWinApi\SXGUI_progress_bar.h>

#pragma once

SXGUIProgressBar::SXGUIProgressBar()
{

}

SXGUIProgressBar::SXGUIProgressBar(WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							PROGRESS_CLASS,
							0,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	OldProc = DefWindowProc;
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

SXGUIProgressBar::SXGUIProgressBar(WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,bool vertical,bool smooth)
{
	this->WindowHandle = CreateWindowEx(
							0,
							PROGRESS_CLASS,
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | (smooth ? PBS_SMOOTH : 0) | (vertical ? PBS_VERTICAL : 0),
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	//SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	ISXGUIComponent* component = dynamic_cast<ISXGUIComponent*>(this);
	SetWindowLong(GetHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

int SXGUIProgressBar::GetPos()
{
	return SendMessage(this->GetHWND(),PBM_GETPOS,0,0);
}

int SXGUIProgressBar::SetPos(int pos)
{
	return SendMessage(this->GetHWND(),PBM_SETPOS,WPARAM(pos),0);
}

int SXGUIProgressBar::GetStep()
{
	int step = SendMessage(this->GetHWND(),PBM_SETSTEP,WPARAM(0),0);

	SendMessage(this->GetHWND(),PBM_SETSTEP,WPARAM(step),0);
	return step;
}

int SXGUIProgressBar::SetStep(int step)
{
	return SendMessage(this->GetHWND(),PBM_SETSTEP,WPARAM(step),0);
}

int SXGUIProgressBar::SetPosSteps(int steps)
{
	return SendMessage(this->GetHWND(),PBM_DELTAPOS,WPARAM(steps),0);
}

int SXGUIProgressBar::SetOneStep()
{
	return SendMessage(this->GetHWND(),PBM_STEPIT,0,0);
}

bool SXGUIProgressBar::SetMinMax(int min,int max)
{
	return SendMessage(this->GetHWND(),PBM_SETRANGE32,WPARAM(min),LPARAM(max));
}


int SXGUIProgressBar::GetMin()
{
	return SendMessage(this->GetHWND(),PBM_GETRANGE,WPARAM(1),0);
}

int SXGUIProgressBar::GetMax()
{
	return SendMessage(this->GetHWND(),PBM_GETRANGE,WPARAM(0),0);
}

bool SXGUIProgressBar::SetBarColor(BYTE r, BYTE g, BYTE b)
{
	return SendMessage(this->GetHWND(),PBM_SETBARCOLOR,0,LPARAM(RGB(r,g,b)));
}

bool SXGUIProgressBar::SetBkColor(BYTE r, BYTE g, BYTE b)
{
	return SendMessage(this->GetHWND(),PBM_SETBKCOLOR,1,LPARAM(RGB(r,g,b)));
}

DWORD SXGUIProgressBar::GetBarColor()
{
	COLORREF color = SendMessage(this->GetHWND(),PBM_SETBARCOLOR,0,0);
	return color;
}

DWORD SXGUIProgressBar::GetBkColor()
{
	COLORREF color = SendMessage(this->GetHWND(),PBM_SETBKCOLOR,0,0);
	return color;
}