
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_RADIO_BUTTON_H
#define __SXGUI_RADIO_BUTTON_H

#include "base.h"
#include "button.h"


class CGUIRadioButton : public CGUIButton, public virtual ISXGUIRadioButton
{
public:
	CGUIRadioButton();
	CGUIRadioButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIRadioButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj);

	void setCheck(bool check);
	bool getCheck();
};

#endif