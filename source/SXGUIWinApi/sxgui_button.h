//----------------------------------------
// Файл: SXGUI_button.h
// Краткое описание: стандартная кнопка
// Project S.I.P for SkyXEngine (09,2012)
//----------------------------------------

#ifndef SXGUI_BUTTON_H
#define SXGUI_BUTTON_H

#if defined(_USRDLL)
#include <SXGUIWinApi\SXGUI_base.cpp>
#else
#include <SXGUIWinApi\SXGUI_base.h>
#endif

#pragma once

#define SXGUI_BUTTON_IMAGE_NONE		0
#define SXGUI_BUTTON_IMAGE_ICON		1
#define SXGUI_BUTTON_IMAGE_BITMAT	2


class SXGUIButton : public SXGUITextual, public virtual ISXGUIButton
{
public:
	SXGUIButton();
	SXGUIButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,WORD image,HWND parent,WNDPROC handler,DWORD id);

	bool SetIconInFile(const char* path);
	bool SetBmpInFile(const char* path);

	bool SetIconInResourse(WORD id);
	bool SetBmpInResourse(WORD id);
};

#endif