
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "trackbar.h"


CGUITrackBar::CGUITrackBar()
{

}

CGUITrackBar::CGUITrackBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							TRACKBAR_CLASS,
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

CGUITrackBar::CGUITrackBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							0,
							TRACKBAR_CLASS,
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | TBS_AUTOTICKS,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

void CGUITrackBar::setPos(int pos)
{
	SendMessage(this->getHWND(),TBM_SETPOS,WPARAM(TRUE),LPARAM(pos));
}

int CGUITrackBar::getPos()
{
	return SendMessage(this->getHWND(),TBM_GETPOS,0,0);
}

void CGUITrackBar::setMinMax(int min,int max)
{
	SendMessage(this->getHWND(),TBM_SETRANGE,WPARAM(TRUE),MAKELPARAM(min,max));
}

int CGUITrackBar::getMin()
{
	return SendMessage(this->getHWND(),TBM_GETRANGEMIN,0,0);
}

int CGUITrackBar::getMax()
{
	return SendMessage(this->getHWND(),TBM_GETRANGEMAX,0,0);
}

bool CGUITrackBar::setTick(int pos)
{
		if(!SendMessage(this->getHWND(),TBM_SETTIC,0,LPARAM(pos)))
			return false;
	return true;
}

int CGUITrackBar::getTick(int index)
{
	return SendMessage(this->getHWND(),TBM_GETTIC,WPARAM(index),0);
}

void CGUITrackBar::setTickFrequency(int freq)
{
	SendMessage(this->getHWND(),TBM_SETTICFREQ,WPARAM(freq),0);
}

bool CGUITrackBar::setBuddies(HWND hwnd_left,HWND hwnd_right)
{
		if(!hwnd_left && !hwnd_left)
			return false;
	
	SendMessage(this->getHWND(),TBM_SETBUDDY,WPARAM(TRUE),LPARAM(hwnd_left));
	SendMessage(this->getHWND(),TBM_SETBUDDY,WPARAM(FALSE),LPARAM(hwnd_right));

	return true;
}

HWND CGUITrackBar::getLeftBuddy()
{
	return HWND(SendMessage(this->getHWND(),TBM_GETBUDDY,WPARAM(TRUE),0));
}

HWND CGUITrackBar::getRightBuddy()
{
	return HWND(SendMessage(this->getHWND(),TBM_GETBUDDY,WPARAM(FALSE),0));
}