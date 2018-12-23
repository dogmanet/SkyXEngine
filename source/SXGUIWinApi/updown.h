
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_UP_DOWN_H
#define __SXGUI_UP_DOWN_H

#include "base.h"


class CGUIUpDown : public CGUIComponent, public virtual ISXGUIUpDown
{
public:
	CGUIUpDown();
	CGUIUpDown(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj, HWND buddy);
	CGUIUpDown(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj, HWND buddy, bool align_left);

	~CGUIUpDown();


	void setMinMax(int min, int max);
	long setPos(int pos);

	long getMin();
	long getMax();
	long getPos();

	HWND setBuddy(HWND buddy);
	HWND getBuddy();
};

#endif