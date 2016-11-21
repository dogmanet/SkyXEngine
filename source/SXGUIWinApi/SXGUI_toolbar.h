//-----------------------------------------------------
// Файл: SXGUI_toolbar.h
// Краткое описание: tool bar в "своем стиле"
// Project S.I.P for SkyXEngine (09,2012)
//-----------------------------------------------------

#ifndef SXGUI_TOOLBAR_H
#define SXGUI_TOOLBAR_H

#pragma once


class SXGUIToolBar : public SXGUIComponent, public virtual ISXGUIToolBar
{
public:
	SXGUIToolBar();
	SXGUIToolBar(const char* caption,WORD x,WORD y,WORD width,WORD heigth,WORD width_element,WORD heigth_element,HWND parent,WNDPROC handler,DWORD id);
	~SXGUIToolBar();

	void AddButton(WORD num,WORD id,const char* hint,WORD id_resource,DWORD mask);
	void AddSeparator(WORD num);

private:
	HIMAGELIST HimList;
};

#endif