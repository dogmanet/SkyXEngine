
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_MEMO_H
#define __SXGUI_MEMO_H

#include "edit.h"


class CGUIMemo : public CGUIEdit, public virtual ISXGUIMemo
{
public:
	CGUIMemo();
	CGUIMemo(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIMemo(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj);

	bool getWordWrap();		//установлен ли автоперенос слов
	bool setWordWrap(bool bf);	//манипулирует свойством автоперенос слов
};

#endif