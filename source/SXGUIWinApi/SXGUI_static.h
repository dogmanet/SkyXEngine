
#ifndef SXGUI_STATIC_H
#define SXGUI_STATIC_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIStatic : public SXGUITextual, public virtual ISXGUIStatic
{
public:
	SXGUIStatic();
	SXGUIStatic(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIStatic(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

	//создает линию
	SXGUIStatic(WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,bool vertical);

	int Align();			//возвращает флаг выравнивания текста
	bool Align(int align);	//устанавливает выравнивание текста

	bool WordWrap();		//установлен ли автоперенос строк
	bool WordWrap(bool bf); //манипуляция со свойством автоперенос строк
};

#endif