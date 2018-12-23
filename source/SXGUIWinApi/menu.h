
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __SXGUI_MENU_H
#define __SXGUI_MENU_H

#include "base.h"

class CGUIMenuBase : public virtual ISXGUIMenuBase
{
public:
	CGUIMenuBase();

	~CGUIMenuBase();

	void Release(){ mem_del(this); }


	bool insertItem(int index, const char* text, ID id, ID id_sub_menu);
	bool insertPopupItem(int index, const char* text, ID id, ID id_sub_menu);
	bool insertSeparator(int index, ID id, ID id_sub_menu);

	bool deleteItem(ID id);

	HMENU getSubMenu(ID id);

	bool setCheckItem(ID id, bool check);
	bool setEnableItem(ID id, int enable);

	bool getCheckedItem(ID id);
	int getEnabledItem(ID id);


	HMENU getHMENU();
	bool setToWindow(HWND hwnd);

protected:
	HMENU MenuHandle;
};

//##########################################################################

class CGUIMenuWindow : public CGUIMenuBase, public virtual ISXGUIMenuWindow
{
public:
	CGUIMenuWindow();
	CGUIMenuWindow(UINT menu);

	void Release(){ mem_del(this); }

	bool setToWindow(HWND hwnd);
};

//##########################################################################

class CGUIPopupMenu : public CGUIMenuBase, public virtual ISXGUIPopupMenu
{
public:
	CGUIPopupMenu();
	CGUIPopupMenu(UINT menu);

	void Release(){ mem_del(this); }

	bool track(HWND hwnd,int x,int y);
};

#endif