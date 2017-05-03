
#ifndef SXGUI_IMG_BUTTON_H
#define SXGUI_IMG_BUTTON_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

class SXGUIButtonImg : public SXGUIComponent, public virtual ISXGUIButtonImg
{
public:
	SXGUIButtonImg();
	SXGUIButtonImg(const char* path,WORD x,WORD y,WORD width,WORD heigth,DWORD alpha_color,DWORD bk_color,HWND parent,WNDPROC handler,DWORD id);
	SXGUIButtonImg(WORD button,WORD x,WORD y,WORD width,WORD heigth,DWORD alpha_color,DWORD bk_color,HWND parent,WNDPROC handler,DWORD id);
	void Release(){ mem_del(this); }
	~SXGUIButtonImg();

	void ReInitImage(const char* path);

	void InitCallBack();
	
	void GetPathForImg(char* buf);

private:
	char PathForImg[1024];
};

#endif