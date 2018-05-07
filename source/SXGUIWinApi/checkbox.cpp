
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "checkbox.h"


CGUICheckBox::CGUICheckBox()
{

}

CGUICheckBox::CGUICheckBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
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
	init(m_hWindow, parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

CGUICheckBox::CGUICheckBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj, bool cb_3_state)
{
	m_hWindow = CreateWindowEx(
							0,
							"BUTTON",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | (cb_3_state ? BS_AUTO3STATE : BS_AUTOCHECKBOX),
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(m_hWindow, parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

void CGUICheckBox::setCheckEx(SXGUI_CHECKBOX_STATE check)
{
	SendMessage(m_hWindow, BM_SETCHECK, check, 0);
}

void CGUICheckBox::setCheck(bool isCheck)
{
	SendMessage(m_hWindow, BM_SETCHECK, isCheck, 0);
}

SXGUI_CHECKBOX_STATE CGUICheckBox::getCheck()
{
	return (SXGUI_CHECKBOX_STATE)SendMessage(this->getHWND(), BM_GETCHECK, 0, 0);
}