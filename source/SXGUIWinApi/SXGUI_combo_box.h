
#ifndef SXGUI_COMBO_BOX_H
#define SXGUI_COMBO_BOX_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIComboBox : public SXGUIComponent, public virtual ISXGUIComboBox
{
public:
	SXGUIComboBox();
	SXGUIComboBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);

	SXGUIComboBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

	bool	InsertItem(int index,const char* text);		//вставка текста в строку
	bool	DeleteItem(int index);						//удаление строки

	int		GetCount();									//количество строк

	bool	SetSel(int index);							//выделяет определенную строку
	int		GetSel();									//номер выделенной строки

	bool	SetItemData(int index,LPARAM data);			//установка для строки userdata (32 бита)
	LPARAM	GetItemData(int index);						//получение userdata

	bool	Clear();									//удаляет все элементы

	bool	SetItemText(int index,const char* text);	//вставка текста в строку, подобие замены
	void	GetItemText(int index,char* buf);			//получить текст из строки 
	int		GetItemTextLength(int index);				//получить длину текста из строки
	bool	AddItem(const char* text);					//добавляет в конец новую строку
};

#endif