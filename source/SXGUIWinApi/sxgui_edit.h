//------------------------------------------------
// ‘айл: SXGUI_edit.h
//  раткое описание: стандартное поле ввода текста
// Project S.I.P for SkyXEngine (09,2012)
//------------------------------------------------

#ifndef SXGUI_EDIT_H
#define SXGUI_EDIT_H

#include <SXGUIWinApi\SXGUI_base.h>
#if defined(_USRDLL)
#include <SXGUIWinApi\SXGUI_up_down.cpp>
#else
#include <SXGUIWinApi\SXGUI_up_down.h>
#endif

#define SXGUI_EDIT_ALIGN_LEFT	0
#define SXGUI_EDIT_ALIGN_RIGHT	1
#define SXGUI_EDIT_ALIGN_CENTER	2

#pragma once

class SXGUIEdit : public SXGUITextual, public virtual ISXGUIEdit
{
public:
	SXGUIEdit();// {}
	~SXGUIEdit();// {}
	SXGUIEdit(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIEdit(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

	bool ReadOnly();		//установлено ли свойство "только дл€ чтени€"
	bool ReadOnly(bool bf);	//манипулирует свойством "только дл€ чтени€", возвращает результат действи€

	//дефолтово скрытый текст замен€етс€ на *
	bool Password();		//установлено ли свойств скрыти€ текста
	bool Password(bool bf);	//манипулирует свойством скрыти€ текста

	WORD Align();			//возвращает флаг выравнивани€ текста
	bool Align(WORD align);	//устанавливает выравнивание
};

#endif