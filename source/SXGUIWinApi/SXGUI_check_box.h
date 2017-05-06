
#ifndef SXGUI_CHECK_BOX_H
#define SXGUI_CHECK_BOX_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

/*
BST_UNCHECKED
BST_CHECKED
BST_INDETERMINATE
*/

class SXGUICheckBox : public SXGUITextual, public virtual ISXGUICheckBox
{
public:
	SXGUICheckBox();
	SXGUICheckBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUICheckBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,bool cb_3_state);

	void SetCheck(int check);
	int GetCheck();
};

#endif