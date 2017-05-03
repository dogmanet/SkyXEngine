
#ifndef SXGUI_EDIT_H
#define SXGUI_EDIT_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIEdit : public SXGUITextual, public virtual ISXGUIEdit
{
public:
	SXGUIEdit();
	~SXGUIEdit();
	SXGUIEdit(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIEdit(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

	bool ReadOnly();		//установлено ли свойство "только для чтения"
	bool ReadOnly(bool bf);	//манипулирует свойством "только для чтения", возвращает результат действия

	//дефолтово скрытый текст заменяется на *
	bool Password();		//установлено ли свойств скрытия текста
	bool Password(bool bf);	//манипулирует свойством скрытия текста

	int Align();			//возвращает флаг выравнивания текста
	bool Align(int align);	//устанавливает выравнивание
};

#endif