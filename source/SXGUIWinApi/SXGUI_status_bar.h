
#ifndef SXGUI_STATUS_BAR_H
#define SXGUI_STATUS_BAR_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIStatusBar : public SXGUIComponent, public virtual ISXGUIStatusBar
{
public:
	SXGUIStatusBar();
	SXGUIStatusBar(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIStatusBar(const char* caption,HWND parent,WNDPROC handler,DWORD id);
	void Release(){ mem_del(this); }
	~SXGUIStatusBar();

	bool SetCountParts(WORD count,int *arr);
	WORD GetCountParts(int **arr);
	bool SetTextParts(WORD pos,const char* text);
	bool GetTextParts(WORD pos, char* buf, int len);

	void Update();
	void UpdateSize();

	WORD AlignReSizing;
protected:

	RECT OldRect;

	void ComCoef();

	float*	ArrCoef;
	int*	ArrWidth;
	WORD	CountArr;
};

#endif