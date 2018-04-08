
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "menu.h"


CGUIMenuBase::CGUIMenuBase()
{
	MenuHandle = 0;
}

CGUIMenuBase::~CGUIMenuBase()
{
	DestroyMenu(this->MenuHandle);
}

bool CGUIMenuBase::insertItem(int index, const char* text, ID id, ID id_sub_menu)
{
	char str[256];
	sprintf(str,"%s",text);
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STRING|MIIM_ID;
	mi.dwTypeData = str;
	mi.wID=id;

	HMENU SubMenu = getSubMenu(id_sub_menu);
	BOOL bf = InsertMenuItem(id_sub_menu > 0 && SubMenu ? SubMenu : MenuHandle,index,TRUE,&mi);
	//DrawMenuBar(ParentHandle);
	return bf ? true : false;
}

bool CGUIMenuBase::insertPopupItem(int index, const char* text, ID id, ID id_sub_menu)
{
	char str[256];
	sprintf(str,"%s",text);
	MENUINFO mi;
	MENUITEMINFO mii;
	HMENU SubMenu = CreatePopupMenu();
	mi.cbSize = sizeof(MENUINFO);
	mi.fMask = MIM_STYLE;
	mi.dwStyle = MNS_NOTIFYBYPOS;
	SetMenuInfo(SubMenu, &mi);
 
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING | MIIM_SUBMENU | MIIM_FTYPE | MIIM_ID; 
	mii.fType = MFT_STRING;
	mii.dwTypeData = str;
	mii.wID = id;
	mii.hSubMenu = SubMenu;

	HMENU SubMenu2 = getSubMenu(id_sub_menu);
	BOOL bf = InsertMenuItem(id_sub_menu > 0 && SubMenu2 ? SubMenu2 : MenuHandle, index, TRUE, &mii);
	//DrawMenuBar(ParentHandle);
	return bf ? true : false;
}

bool CGUIMenuBase::insertSeparator(int index, ID id, ID id_sub_menu)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_ID | MIIM_FTYPE;
	mi.wID=id;
	mi.fType = MFT_SEPARATOR;

	HMENU SubMenu = getSubMenu(id_sub_menu);
	BOOL bf = InsertMenuItem(id_sub_menu > 0 && SubMenu ? SubMenu : MenuHandle,index,TRUE,&mi);
	//DrawMenuBar(ParentHandle);
	return bf ? true : false;
}

bool CGUIMenuBase::deleteItem(ID id)
{
	return DeleteMenu(this->MenuHandle,id,MF_BYCOMMAND) ? true : false;
}

HMENU CGUIMenuBase::getSubMenu(ID id)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_SUBMENU;
	mi.hSubMenu = 0;
	BOOL bf = GetMenuItemInfo(MenuHandle,id,FALSE,&mi);

	return bf ? mi.hSubMenu : 0;
}

bool CGUIMenuBase::setCheckItem(ID id, bool check)
{
	/*MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STATE;

		if(!GetMenuItemInfo(MenuHandle,id,FALSE,&mi))
			return false;

	mi.fState |= (check ? MFS_CHECKED : MFS_UNCHECKED);

		if(!SetMenuItemInfo(MenuHandle,id,FALSE,&mi))
			return false;
	return true;*/
	DWORD var = CheckMenuItem(MenuHandle,id,(check ? MF_CHECKED : MF_UNCHECKED));
	//DrawMenuBar(ParentHandle);
	return ((var != 0xFFFFFFFF) || (var == -1)) ? true : false;
}

bool CGUIMenuBase::getCheckedItem(ID id)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STATE;
	BOOL bf = GetMenuItemInfo(MenuHandle,id,FALSE,&mi);
		if(mi.fState & MFS_CHECKED)
			return true;
	return false;
}

bool CGUIMenuBase::setEnableItem(ID id, int enable)
{
	/*MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STATE;

		if(!GetMenuItemInfo(MenuHandle,id,FALSE,&mi))
			return false;

	DWORD Enable = SXGUI_MENU_ENABLED;

		if(enable == SXGUI_MENU_DISABLED)
			Enable = MFS_DISABLED;
		else if(enable == SXGUI_MENU_GRAYED)
			Enable = MFS_GRAYED;
		else
			Enable = MFS_ENABLED;

	mi.fState |= Enable;

		if(!SetMenuItemInfo(MenuHandle,id,FALSE,&mi))
			return false;
	return true;*/

	DWORD Enable = SXGUI_MENU_PART_STATE_ENABLED;

		if (enable == SXGUI_MENU_PART_STATE_DISABLED)
			Enable = MFS_DISABLED;
		else if (enable == SXGUI_MENU_PART_STATE_GRAYED)
			Enable = MFS_GRAYED;
		else
			Enable = MFS_ENABLED;

	DWORD var = EnableMenuItem(MenuHandle,id,Enable);;
	//DrawMenuBar(ParentHandle);
	return ((var != 0xFFFFFFFF) || (var == -1)) ? true : false;
}

int CGUIMenuBase::getEnabledItem(ID id)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STATE;
	BOOL bf = GetMenuItemInfo(MenuHandle,id,FALSE,&mi);

		if(mi.fState & MFS_DISABLED)
			return SXGUI_MENU_PART_STATE_DISABLED;
		else if(mi.fState & MFS_GRAYED)
			return SXGUI_MENU_PART_STATE_GRAYED;
		else
			return SXGUI_MENU_PART_STATE_ENABLED;

	return -1;
}

HMENU CGUIMenuBase::getHMENU()
{
	return this->MenuHandle;
}

//##########################################################################

CGUIMenuWindow::CGUIMenuWindow()
{
	MENUINFO mi;
	MenuHandle = CreateMenu();
	mi.cbSize = sizeof(MENUINFO);
	mi.fMask = MIM_STYLE;
	mi.dwStyle = MNS_NOTIFYBYPOS;
	SetMenuInfo(MenuHandle, &mi);
}

CGUIMenuWindow::CGUIMenuWindow(UINT menu)
{
	MenuHandle = LoadMenu(GetModuleHandle(0), MAKEINTRESOURCE(menu));
}

bool CGUIMenuWindow::setToWindow(HWND hwnd)
{
		if(!hwnd)
			return false;
		if(!SetMenu(hwnd,this->MenuHandle))
			return false;
	return true;
}

//##########################################################################

CGUIPopupMenu::CGUIPopupMenu(UINT menu)
{
	MenuHandle = LoadMenu(GetModuleHandle(0),MAKEINTRESOURCE(menu));
	MenuHandle = ::GetSubMenu(MenuHandle,0);
}

CGUIPopupMenu::CGUIPopupMenu()
{
	MENUINFO mi;
	MenuHandle = CreatePopupMenu();
	mi.cbSize = sizeof(MENUINFO);
	mi.fMask = MIM_STYLE;
	mi.dwStyle = MNS_NOTIFYBYPOS;
	SetMenuInfo(MenuHandle, &mi);
}

bool CGUIPopupMenu::track(HWND hwnd,int x,int y)
{
	int pos_x,pos_y;

		if((x == -1)&&(y==-1))
		{
			POINT point;
			GetCursorPos(&point);
			pos_x = point.x;
			pos_y = point.y;
		}
		else
		{
			pos_x=x; 
			pos_y=y;
		}
		if(!hwnd)
			return false;

		if(!TrackPopupMenu(MenuHandle,TPM_LEFTALIGN,pos_x,pos_y,0,hwnd,0))
			return false;
	return true;
}
