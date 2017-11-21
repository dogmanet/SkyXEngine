
#ifndef SXGUI_RADIO_BUTTON_H
#define SXGUI_RADIO_BUTTON_H

#include "SXGUI_base.h"
#include "SXGUI_button.h"

#pragma once

class SXGUIRadioButton : public SXGUIButton, public virtual ISXGUIRadioButton
{
public:
	SXGUIRadioButton();
	SXGUIRadioButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIRadioButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

	void SetCheck(bool check);
	bool GetCheck();
};

#endif