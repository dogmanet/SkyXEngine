
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "updown.h"


CGUIUpDown::CGUIUpDown()
{

}

CGUIUpDown::~CGUIUpDown()
{

}

CGUIUpDown::CGUIUpDown(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj, HWND buddy)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							UPDOWN_CLASS,
							0,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	SendMessage(this->getHWND(), UDM_SETRANGE32, -65535, 65535);
    SendMessage(this->getHWND(), UDM_SETPOS32, 0, 0);
		if(buddy)
			setBuddy(buddy);

		init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

CGUIUpDown::CGUIUpDown(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj, HWND buddy, bool align_left)
{
	/*this->getHWND() = CreateUpDownControl(
												WS_CHILD | WS_BORDER | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT | UDS_ARROWKEYS | (left_right == 1 ?UDS_ALIGNRIGHT : UDS_ALIGNLEFT), 
												x,y,width,heigth, 
												parent, 0, GetModuleHandle(0), buddy,
												0, 0, 0
											);*/
	m_hWindow = CreateWindowEx(
							0,
							UPDOWN_CLASS,
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | UDS_WRAP | UDS_SETBUDDYINT | UDS_ARROWKEYS | (align_left ? UDS_ALIGNRIGHT : UDS_ALIGNLEFT),
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	SendMessage(this->getHWND(), UDM_SETRANGE32, -65535, 65535);
    SendMessage(this->getHWND(), UDM_SETPOS32, 0, 0);
		if(buddy)
			setBuddy(buddy);

	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

void CGUIUpDown::setMinMax(int min, int max)
{
	SendMessage(this->getHWND(), UDM_SETRANGE32, min, max);
}

long CGUIUpDown::setPos(int pos)
{
	return SendMessage(this->getHWND(), UDM_SETPOS32, 0, pos);
}

long CGUIUpDown::getMin()
{
	long min = 0;
	SendMessage(this->getHWND(), UDM_GETRANGE32, (WPARAM)&min, 0);
	return min;
}

long CGUIUpDown::getMax()
{
	long max = 0;
	SendMessage(this->getHWND(), UDM_GETRANGE32, 0, (LPARAM)&max);
	return max;
}

long CGUIUpDown::getPos()
{
	return SendMessage(this->getHWND(), UDM_GETPOS32, 0, 0);
}

HWND CGUIUpDown::setBuddy(HWND buddy)
{
	return (HWND)SendMessage(this->getHWND(), UDM_SETBUDDY, (WPARAM)buddy, 0);
}

HWND CGUIUpDown::getBuddy()
{
	return (HWND)SendMessage(this->getHWND(), UDM_GETBUDDY, 0, 0);
}