
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_BASE_WND_H
#define __SXGUI_BASE_WND_H

#include "base.h"
#include "menu.h"


class CGUIBaseWnd : public CGUITextual, public virtual ISXGUIBaseWnd
{
public:
	CGUIBaseWnd(	
					const char* class_name, const char* caption,
					int x, int y, int width, int heigth,
					HICON icon,HCURSOR cursor, HBRUSH brush,
					DWORD exstyle,DWORD wndstyle,DWORD style,
					HWND parent,WNDPROC handler, ID idObj
				);
	~CGUIBaseWnd();

	bool setColorBrush(DWORD color);
	DWORD getColorBrush();
	HBRUSH getBrush();

	bool setAlpha(float fAlpha);
	float getAlpha();
	bool setAlphaBlend(bool isBlend);
	bool getAlphaBlend();
};

#endif