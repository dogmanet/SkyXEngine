
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

	bool SetCountParts(int count, int *arr);
	int GetCountParts(int **arr);
	bool SetTextParts(int pos, const char* text);
	bool GetTextParts(int pos, char* buf, int len);

	void Update();
	void UpdateSize();

	void SetAlignRS(int alignrs);
	int GetAlignRS();

protected:

	int AlignReSizing;

	RECT OldRect;

	void ComCoef();

	float*	ArrCoef;
	int*	ArrWidth;
	WORD	CountArr;
};

#endif