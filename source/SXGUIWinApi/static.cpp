
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "static.h"


CGUIStatic::CGUIStatic()
{

}

CGUIStatic::CGUIStatic(const char* caption, int x, int y, int width, int heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							SXGUI_DEF_STATIC,
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

CGUIStatic::CGUIStatic(const char* caption, int x, int y, int width, int heigth, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							0,
							SXGUI_DEF_STATIC,
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE  | SS_LEFT,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

CGUIStatic::CGUIStatic(int x, int y, int width, int heigth, HWND parent, WNDPROC handler, ID idObj, bool vertical)
{
	m_hWindow = CreateWindowEx(
							WS_EX_TOPMOST,
							SXGUI_DEF_STATIC,
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE  | SS_LEFT | (vertical ? SS_ETCHEDVERT : SS_ETCHEDHORZ),
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, handler, idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

SXGUI_TEXT_ALIGN CGUIStatic::getAlign()
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if(style & SS_RIGHT)
			return SXGUI_TEXT_ALIGN_RIGHT;
		else if(style & SS_CENTER)
			return SXGUI_TEXT_ALIGN_CENTER;
		else
			return SXGUI_TEXT_ALIGN_LEFT;
}

bool CGUIStatic::setAlign(SXGUI_TEXT_ALIGN align)
{
	if (align == SXGUI_TEXT_ALIGN_LEFT)
		return this->modifyStyle(SS_LEFT,0);
	else if (align == SXGUI_TEXT_ALIGN_RIGHT)
		return this->modifyStyle(SS_RIGHT,0);
	else if (align == SXGUI_TEXT_ALIGN_CENTER)
		return this->modifyStyle(SS_CENTER,0);
	return false;
}

bool CGUIStatic::getWordWrap()
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if(!(style & SS_LEFTNOWORDWRAP))
			return true;
	return false;
}

bool CGUIStatic::setWordWrap(bool bf)
{
	return this->modifyStyle(!bf ? SS_LEFTNOWORDWRAP : 0,bf ? SS_LEFTNOWORDWRAP : 0);
}
