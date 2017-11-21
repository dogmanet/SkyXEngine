
#ifndef SXGUI_MEMO_H
#define SXGUI_MEMO_H

#include <SXGUIWinApi\SXGUI_edit.h>

#pragma once

class SXGUIMemo : public SXGUIEdit, public virtual ISXGUIMemo
{
public:
	SXGUIMemo();
	SXGUIMemo(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIMemo(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

	bool WordWrap();		//установлен ли автоперенос слов
	bool WordWrap(bool bf);	//манипулирует свойством автоперенос слов

	//SXGUIFuctinon
	bool ScrollBars(bool h,bool v);
	bool ScrollBarV();
	bool ScrollBarH();
	bool ScrollLine(WORD scroll,WORD dir,int count);
};

#endif