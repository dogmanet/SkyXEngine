//-----------------------------------------------------
// Файл: SXGUI_status_bar.h
// Краткое описание: стандартный status bar
// Project S.I.P for SkyXEngine (09,2012)
//-----------------------------------------------------

#ifndef SXGUI_STATUS_BAR_H
#define SXGUI_STATUS_BAR_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

#define SXGUI_SB_ALIGN_RS_NONE		0
#define SXGUI_SB_ALIGN_RS_PERCENT	1
#define SXGUI_SB_ALIGN_RS_PROP		2

class SXGUIStatusBar : public SXGUIComponent, public virtual ISXGUIStatusBar
{
public:
	SXGUIStatusBar();
	SXGUIStatusBar(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id);
	SXGUIStatusBar(const char* caption,HWND parent,WNDPROC handler,DWORD id);
	~SXGUIStatusBar();

	bool SetCountParts(WORD count,int *arr);
	WORD GetCountParts(int **arr);
	bool SetTextParts(WORD pos,const char* text);
	const char* GetTextParts(WORD pos);

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