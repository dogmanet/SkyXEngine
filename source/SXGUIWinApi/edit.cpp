
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "edit.h"

CGUIEdit::CGUIEdit() 
{

}

CGUIEdit::~CGUIEdit()
{

}

CGUIEdit::CGUIEdit(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							"EDIT",
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	this->init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->initComponent();
}

CGUIEdit::CGUIEdit(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							0,
							"EDIT",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	this->init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->initComponent();
}

bool CGUIEdit::getReadOnly()
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if(style & ES_READONLY)
			return true;
		else
			return false;
}

bool CGUIEdit::setReadOnly(bool bf)
{
	long p = bf ? 1 : 0;
	SendMessage(this->getHWND(), EM_SETREADONLY, bf ? 1 : 0, 0);
	return getReadOnly() == bf;
}

int CGUIEdit::getAlign()
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if(style & ES_RIGHT)
			return SXGUI_TEXT_ALIGN_RIGHT;
		else if(style & ES_CENTER)
			return SXGUI_TEXT_ALIGN_CENTER;
		else
			return SXGUI_TEXT_ALIGN_LEFT;
}

bool CGUIEdit::setAlign(int align)
{
		if (align == SXGUI_TEXT_ALIGN_LEFT)
			return modifyStyle(ES_LEFT,0);
		else if (align == SXGUI_TEXT_ALIGN_RIGHT)
			return modifyStyle(ES_RIGHT,0);
		else if (align == SXGUI_TEXT_ALIGN_CENTER)
			return modifyStyle(ES_CENTER,0);
	return false;
}

bool CGUIEdit::getStatePassword()
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if(style & ES_PASSWORD)
			return true;
		else
			return false;
}

bool CGUIEdit::setStatePassword(bool bf)
{
	long p = bf ? 1 : 0;
	SendMessage(this->getHWND(), EM_SETPASSWORDCHAR, bf ? '*' : 0, 0);
	this->setFocus();
	UpdateWindow(this->getHWND());
	return getStatePassword() == bf;
}