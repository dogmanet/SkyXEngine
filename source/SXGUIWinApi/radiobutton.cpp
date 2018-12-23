
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "radiobutton.h"


CGUIRadioButton::CGUIRadioButton()
{

}

CGUIRadioButton::CGUIRadioButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							"BUTTON",
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

CGUIRadioButton::CGUIRadioButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							0,
							"BUTTON",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | BS_AUTORADIOBUTTON,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

void CGUIRadioButton::setCheck(bool check)
{
	SendMessage(this->getHWND(),BM_SETCHECK,(check ? BST_CHECKED : BST_UNCHECKED),0);
}

bool CGUIRadioButton::getCheck()
{
	return SendMessage(this->getHWND(),BM_GETCHECK,0,0) ? true : false;
}