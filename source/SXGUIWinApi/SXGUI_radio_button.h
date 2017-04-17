//-----------------------------------------------------
// Файл: SXGUI_radio_button.h
// Краткое описание: стандартная радио кнопка
// Project S.I.P for SkyXEngine (09,2012)
//-----------------------------------------------------

#ifndef SXGUI_RADIO_BUTTON_H
#define SXGUI_RADIO_BUTTON_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIRadioButton : public SXGUITextual, public virtual ISXGUIRadioButton
{
public:
	SXGUIRadioButton();
	SXGUIRadioButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIRadioButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

	void SetCheck(bool check);
	bool GetCheck();
};

#endif