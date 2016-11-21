//----------------------------------------
// Файл: SXGUI_check_box.h
// Краткое описание: стандартный check box
// Project S.I.P for SkyXEngine (09,2012)
//----------------------------------------

#ifndef SXGUI_CHECK_BOX_H
#define SXGUI_CHECK_BOX_H

#include <SXGUIWinApi\SXGUI_base.h>

#define SXGUI_CB_UNCHECKED		BST_UNCHECKED
#define SXGUI_CB_CHECKED		BST_CHECKED
#define SXGUI_CB_INDETERMINATE	BST_INDETERMINATE

#pragma once

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