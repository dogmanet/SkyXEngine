
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_STATIC_H
#define __SXGUI_STATIC_H

#include "base.h"

class CGUIStatic : public CGUITextual, public virtual ISXGUIStatic
{
public:
	CGUIStatic();
	CGUIStatic(const char* caption, int x, int y, int width, int heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIStatic(const char* caption, int x, int y, int width, int heigth, HWND parent, WNDPROC handler, ID idObj);

	//создает линию
	CGUIStatic(int x, int y, int width, int heigth, HWND parent, WNDPROC handler, ID idObj, bool vertical);

	SXGUI_TEXT_ALIGN getAlign();			//возвращает флаг выравнивания текста
	bool setAlign(SXGUI_TEXT_ALIGN align);	//устанавливает выравнивание текста

	bool getWordWrap();		//установлен ли автоперенос строк
	bool setWordWrap(bool bf); //манипуляция со свойством автоперенос строк
};

#endif