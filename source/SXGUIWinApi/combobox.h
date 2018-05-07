
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_COMBO_BOX_H
#define __SXGUI_COMBO_BOX_H

#include "base.h"


class CGUIComboBox : public CGUIComponent, public virtual ISXGUIComboBox
{
public:
	CGUIComboBox();
	CGUIComboBox(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);

	CGUIComboBox(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj);

	bool insertItem(int index,const char* text);		//вставка текста в строку
	bool deleteItem(int index);						//удаление строки

	int	getCount();									//количество строк

	bool setSel(int index);							//выделяет определенную строку
	int	getSel();									//номер выделенной строки

	bool setItemData(int index,LPARAM data);			//установка для строки userdata (32 бита)
	LPARAM getItemData(int index);						//получение userdata

	bool clear();									//удаляет все элементы

	bool setItemText(int index,const char* text);	//вставка текста в строку, подобие замены
	void getItemText(int index,char* buf);			//получить текст из строки 
	int	getItemTextLength(int index);				//получить длину текста из строки
	bool addItem(const char* text);					//добавляет в конец новую строку
};

#endif