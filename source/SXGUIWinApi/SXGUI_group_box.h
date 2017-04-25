//----------------------------------------
// Файл: SXGUI_group_box.h
// Краткое описание: стандартный group box
// Project S.I.P for SkyXEngine (09,2012)
//----------------------------------------

#ifndef SXGUI_GROUP_BOX_H
#define SXGUI_GROUP_BOX_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIGroupBox : public SXGUITextual, public virtual ISXGUIGroupBox
{
public:
	SXGUIGroupBox();
	SXGUIGroupBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIGroupBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

//private:
	char* StrText;
	int LenStrText;
};

#endif