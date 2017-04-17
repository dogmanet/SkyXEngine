//-----------------------------------------------------
// Файл: SXGUI_list_box.h
// Краткое описание: стандартный list box
// Project S.I.P for SkyXEngine (09,2012)
//-----------------------------------------------------

#ifndef SXGUI_LIST_BOX_H
#define SXGUI_LIST_BOX_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIListBox : public SXGUIComponent, public virtual ISXGUIListBox
{
public:
	SXGUIListBox();
	SXGUIListBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIListBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,bool miltiple_sel);

	bool	MultipleSel();	//установленно ли свойство множественного выделения строк

	bool	InsertItem(int index,const char* text);	//вставить текст в строку с номером index
	bool	AddItem(const char* text);				//добавить строку в конец списка
	int		GetCountItem();							//возвращает количество строк

	bool	DeleteItem(int index);					//удалить строку с номером index

	bool	SetSel(int index);						//выделить строку с номером index
	int		GetSel();

	bool	SetItemData(int index,LPARAM data);		//установить userdata для строки с номером index
	LPARAM	GetItemData(int index);					//возвращает userdata

	bool	Clear();								//очистить

	bool	SetTextItem(int index,const char* text);//установить текст в строке с номером index
	char*	GetItemText(int index);					//возвращает текст из строки !!!память выделяется в куче
	void	GetItemText(int index,char* buf);		//возвращает текст из строки

	int		GetItemTextLength(int index);			//возвращает длину текста в строке

	//мультивыделение строк, если this->MultipleSel() == true
	int		GetMultipleSelCount();					//возвращает количество выделенных строк

	bool	SetMultipleSel(int index,bool sel);		//устанавливает выделена/не выделена строка
	bool	GetMultipleSel(int index);				//возвращает выделена ли строка

	int*	GetMultipleSelArr();					//возвращает массив с номерами выделенных строк

	//SXGUIFuctinon
	bool ScrollBars(bool h,bool v);
	bool ScrollBarV();
	bool ScrollBarH();
	bool ScrollLine(WORD scroll,WORD dir,int count);
};

#endif