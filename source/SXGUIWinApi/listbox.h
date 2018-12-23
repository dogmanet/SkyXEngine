
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_LIST_BOX_H
#define __SXGUI_LIST_BOX_H

#include "base.h"


class CGUIListBox : public CGUIComponent, public virtual ISXGUIListBox
{
public:
	CGUIListBox();
	CGUIListBox(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIListBox(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj, bool miltiple_sel);

	bool getStateMultiSel();	//установленно ли свойство множественного выделения строк
	void setStateMultiSel(bool isMultiSel);

	bool insertItem(int index,const char* text);	//вставить текст в строку с номером index
	bool addItem(const char* text);				//добавить строку в конец списка
	int getItemCount();							//возвращает количество строк

	bool deleteItem(int index);					//удалить строку с номером index

	bool setSel(int index);						//выделить строку с номером index
	int	getSel();

	bool setItemData(int index,LPARAM data);		//установить userdata для строки с номером index
	LPARAM getItemData(int index);					//возвращает userdata

	bool clear();								//очистить

	bool setItemText(int index, const char* text);//установить текст в строке с номером index
	void getItemText(int index,char* buf);		//возвращает текст из строки

	int	getItemTextLength(int index);			//возвращает длину текста в строке

	//мультивыделение строк, если this->MultipleSel() == true
	int getMultiSelCount();					//возвращает количество выделенных строк

	bool setMultiSel(int index,bool sel);		//устанавливает выделена/не выделена строка
	bool getMultiSel(int index);				//возвращает выделена ли строка

	
	int getMultiSelArr(int **ppArr);					//возвращает массив с номерами выделенных строк
};

#endif