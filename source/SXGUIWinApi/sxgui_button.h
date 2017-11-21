
#ifndef SXGUI_BUTTON_H
#define SXGUI_BUTTON_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIButton : public SXGUITextual, public virtual ISXGUIButton
{
public:
	SXGUIButton();
	SXGUIButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, int image, HWND parent, WNDPROC handler, DWORD id);

	bool SetIconInFile(const char* path);
	bool SetBmpInFile(const char* path);

	bool SetIconInResourse(WORD id);
	bool SetBmpInResourse(WORD id);
};

#endif