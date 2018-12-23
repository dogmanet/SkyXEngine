
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_STATUS_BAR_H
#define __SXGUI_STATUS_BAR_H

#include "base.h"

class CGUIStatusBar : public CGUIComponent, public virtual ISXGUIStatusBar
{
public:
	CGUIStatusBar();
	CGUIStatusBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj);
	CGUIStatusBar(const char* caption, HWND parent, WNDPROC handler, ID idObj);
	
	~CGUIStatusBar();

	void Release(){ mem_del(this); }


	bool setPartsCount(int count, int *arr);
	int getPartsCount(int **arr);
	bool setPartText(int pos, const char* text);
	bool getPartText(int pos, char* buf, int len);

	void update();
	void updateSize();

	void setAlignReSize(SXGUI_STATUSBAR_ALIGN_RESIZE alignrs);
	SXGUI_STATUSBAR_ALIGN_RESIZE getAlignReSize();

protected:

	SXGUI_STATUSBAR_ALIGN_RESIZE AlignReSizing;

	RECT OldRect;

	void comCoef();

	float*	ArrCoef;
	int*	ArrWidth;
	WORD	CountArr;
};

#endif