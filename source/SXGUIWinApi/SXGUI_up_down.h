
#ifndef SXGUI_UP_DOWN_H
#define SXGUI_UP_DOWN_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIUpDown : public SXGUIComponent, public virtual ISXGUIUpDown
{
public:
	SXGUIUpDown();
	~SXGUIUpDown();
	SXGUIUpDown(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id, HWND buddy);
	SXGUIUpDown(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, HWND buddy, bool align_left);

	void SetMinMax(long min,long max);
	long SetPos(long pos);

	long GetMin();
	long GetMax();
	long GetPos();

	HWND SetBuddy(HWND buddy);
	HWND GetBuddy();
};

#endif