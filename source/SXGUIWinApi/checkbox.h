
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_CHECK_BOX_H
#define __SXGUI_CHECK_BOX_H

#include "base.h"
#include "button.h"


class CGUICheckBox : public CGUIButton, public virtual ISXGUICheckBox
{
public:
	CGUICheckBox();
	CGUICheckBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUICheckBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj, bool cb_3_state);

	void setCheckEx(SXGUI_CHECKBOX_STATE check);
	void setCheck(bool isCheck);
	SXGUI_CHECKBOX_STATE getCheck();
};

#endif