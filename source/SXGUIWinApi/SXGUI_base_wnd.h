
#ifndef SXGUI_BASE_WND_H
#define SXGUI_BASE_WND_H

#include <SXGUIWinApi\SXGUI_base.h>
#include <SXGUIWinApi\SXGUI_menu.h>

#pragma once

class SXGUIBaseWnd : public SXGUITextual, public virtual ISXGUIBaseWnd
{
public:
	SXGUIBaseWnd(	
					const char* class_name,const char* caption,const char* menu,
					WORD id,int x,int y,WORD width,WORD heigth,
					HICON icon,HCURSOR cursor, HBRUSH brush,
					DWORD exstyle,DWORD wndstyle,DWORD style,
					HWND parent,WNDPROC handler
				);
	~SXGUIBaseWnd();

	bool SetColorBrush(BYTE r,BYTE g,BYTE b);
	bool SetColorBrush(DWORD color);
	DWORD GetColorBrush();
	HBRUSH GetBrush();

	bool Alpha(WORD alpha);
	WORD Alpha();
	bool AlphaBlend(bool bf);
	bool AlphaBlend();
};

#endif