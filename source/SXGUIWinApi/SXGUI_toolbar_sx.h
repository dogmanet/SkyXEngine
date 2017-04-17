//-----------------------------------------------------
// Файл: SXGUI_toolbar.h
// Краткое описание: tool bar в "своем стиле"
// Project S.I.P for SkyXEngine (09,2012)
//-----------------------------------------------------

#ifndef SXGUI_TOOLBAR_SX_H
#define SXGUI_TOOLBAR_SX_H

#if defined(_USRDLL)
#include <SXGUIWinApi\SXGUI_img_button.cpp>
#else
#include <SXGUIWinApi\SXGUI_img_button.h>
#endif

#pragma once

SX_LIB_API LRESULT CALLBACK WndProcToolBarDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class SXGUIToolBarSX : public SXGUIComponent, public virtual ISXGUIToolBarSX
{
public:
	SXGUIToolBarSX();
	SXGUIToolBarSX(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,const char* path_button_ctrl,DWORD id_ctrl_button);
	SXGUIToolBarSX(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,WORD button_res,DWORD id_ctrl_button);
	~SXGUIToolBarSX();

	SXGUIButtonImg *ImgButton;

	RECT MaxRect;
	RECT MinRect;

	const char* Caption;

	DWORD ColorTop;
	DWORD ColorClient;

	SXGUIToolBarSX* ArrChildToolBar[1024];
	WORD CountChildToolBar;
};

#endif