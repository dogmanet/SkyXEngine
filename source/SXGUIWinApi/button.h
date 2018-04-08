
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_BUTTON_H
#define __SXGUI_BUTTON_H

#include "base.h"


class CGUIButton : public CGUITextual, public virtual ISXGUIButton
{
public:
	CGUIButton();
	CGUIButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, SXGUI_BUTTON_IMAGE type_image, HWND parent, WNDPROC handler, ID idObj);

	bool setIconFromFile(const char* path);
	bool setBmpFromFile(const char* path);

	bool setIconFromResourse(UINT idRes);
	bool setBmpFromResourse(UINT idRes);
};

#endif