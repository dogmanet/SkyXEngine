
#ifndef SXGUI_TRACK_BAR_H
#define SXGUI_TRACK_BAR_H

#include "SXGUI_base.h"

class SXGUITrackBar : public SXGUIComponent, public virtual ISXGUITrackBar
{
public:
	SXGUITrackBar();
	SXGUITrackBar(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUITrackBar(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id);

	void SetPos(int pos);
	int GetPos();

	void SetMinMax(int min,int max);
	int GetMin();
	int GetMax();

	bool SetTick(int pos);
	int GetTick(int index);

	void SetTickFrequency(int freq);

	bool SetBuddies(HWND hwnd_left,HWND hwnd_right);
	HWND GetLeftBuddy();
	HWND GetRightBuddy();
};

#endif