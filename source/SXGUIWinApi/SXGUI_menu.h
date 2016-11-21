//-----------------------------------------------------
// Файл: SXGUI_menu.h
// Краткое описание: меню окна и контекстное меню
// Project S.I.P for SkyXEngine (09,2012)
//-----------------------------------------------------

#ifndef SXGUI_MENU_H
#define SXGUI_MENU_H

#include <SXGUIWinApi\SXGUI_base.h>

#pragma once

#define SXGUI_MENU_ENABLED	0
#define SXGUI_MENU_DISABLED	1
#define SXGUI_MENU_GRAYED	2


class SXGUIMenu : public virtual ISXGUIMenu
{
public:
	SXGUIMenu();
	SXGUIMenu(WORD menu);
	~SXGUIMenu();

	bool InsertItem(int index,const char* text,WORD id,WORD id_sub_menu);
	bool InsertPopupItem(int index,const char* text,WORD id,WORD id_sub_menu);
	bool InsertSeparator(int index,WORD id,WORD id_sub_menu);

	bool DeleteItem(WORD id);

	HMENU GetSubMenu(WORD id);

	bool CheckItem(WORD id,bool check);
	bool EnableItem(WORD id,WORD enable);

	bool GetCheckedItem(WORD id);
	WORD GetEnabledItem(WORD id);


	HMENU GetHMENU();
	bool SetToWindow(HWND hwnd);

protected:
	HMENU MenuHandle;
};

/////////////////////////

class SXGUIPopupMenu : public virtual ISXGUIPopupMenu
{
public:
	SXGUIPopupMenu(WORD menu);
	SXGUIPopupMenu();
	~SXGUIPopupMenu();

	bool InsertItem(int index,const char* text,WORD id,WORD id_sub_menu);
	bool InsertPopupItem(int index,const char* text,WORD id,WORD id_sub_menu);
	bool InsertSeparator(int index,WORD id,WORD id_sub_menu);

	bool DeleteItem(WORD id);

	HMENU GetSubMenu(WORD id);

	bool CheckItem(WORD id,bool check);
	bool EnableItem(WORD id,WORD enable);

	bool GetCheckedItem(WORD id);
	WORD GetEnabledItem(WORD id);

	bool Track(HWND hwnd,int x,int y);


	HMENU GetHMENU();

protected:
	HMENU MenuHandle;
};

#endif