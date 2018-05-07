
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_HINT_H
#define __SXGUI_HINT_H

#include <windows.h>
#include <commctrl.h>

#include "base.h"
#include "SXGUI.h"


class CGUIHint : public virtual ISXGUIHint
{
public:
	CGUIHint();
	CGUIHint(HWND parent);
	~CGUIHint();
	void Release(){ mem_del(this); }
	//autopop - время показа
	//init - сколько ждем появления подсказки после наведения курсора
	void setDelayTime(UINT init, UINT autopop);

	bool setParent(HWND parent);
	HWND getParent();

	HWND getHWND();

	void setVisible(bool bf);
	bool getVisible();

	void setText(const char* text);	//установка текста подсказки
	const char* getText();				//возвращает указатель на текст подсказки, очищается при вызове деструктора
	void getText(char* buf);

protected:
	HWND WindowHandle;
	HWND ParentHandle;
	TOOLINFO Hint;
	char hint[SX_HINT_COUNT_BUF_SIZE];
	bool IsVisible;
};

#endif