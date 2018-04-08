
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_TRACK_BAR_H
#define __SXGUI_TRACK_BAR_H

#include "base.h"


class CGUITrackBar : public CGUIComponent, public virtual ISXGUITrackBar
{
public:
	CGUITrackBar();
	CGUITrackBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUITrackBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj);

	void setPos(int pos);
	int getPos();

	void setMinMax(int min,int max);
	int getMin();
	int getMax();

	bool setTick(int pos);
	int getTick(int index);

	void setTickFrequency(int freq);

	bool setBuddies(HWND hwnd_left,HWND hwnd_right);
	HWND getLeftBuddy();
	HWND getRightBuddy();
};

#endif