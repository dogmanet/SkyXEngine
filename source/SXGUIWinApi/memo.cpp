
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "memo.h"


CGUIMemo::CGUIMemo()
{

}

CGUIMemo::CGUIMemo(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
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
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

CGUIMemo::CGUIMemo(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							0,
							"EDIT",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL | ES_LEFT | ES_WANTRETURN | ES_MULTILINE,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);

	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}


bool CGUIMemo::getWordWrap()
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if((style & ES_AUTOVSCROLL) && !(style & ES_AUTOHSCROLL))
			return true;
	return false;
}

bool CGUIMemo::setWordWrap(bool bf)
{
	return modifyStyle(bf ? ES_AUTOVSCROLL : 0,!bf ? ES_AUTOVSCROLL : 0);
}