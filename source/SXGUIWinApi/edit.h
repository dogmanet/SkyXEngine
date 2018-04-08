
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_EDIT_H
#define __SXGUI_EDIT_H

#include "base.h"


class CGUIEdit : public CGUITextual, public virtual ISXGUIEdit
{
public:
	CGUIEdit();
	~CGUIEdit();
	CGUIEdit(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIEdit(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj);

	bool getReadOnly();		//установлено ли свойство "только для чтения"
	bool setReadOnly(bool bf);	//манипулирует свойством "только для чтения", возвращает результат действия

	//дефолтово скрытый текст заменяется на *
	bool getStatePassword();		//установлено ли свойств скрытия текста
	bool setStatePassword(bool bf);	//манипулирует свойством скрытия текста

	int getAlign();				//возвращает флаг выравнивания текста
	bool setAlign(int align);	//устанавливает выравнивание
};

#endif