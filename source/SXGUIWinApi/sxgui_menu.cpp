
#include <SXGUIWinApi\SXGUI_menu.h>

#pragma once

SXGUIMenu::SXGUIMenu()
{
	MENUINFO mi;
	MenuHandle = CreateMenu();
	mi.cbSize = sizeof(MENUINFO);
	mi.fMask = MIM_STYLE;
	mi.dwStyle = MNS_NOTIFYBYPOS;
	SetMenuInfo(MenuHandle, &mi);
}

SXGUIMenu::SXGUIMenu(WORD menu)
{
	MenuHandle = LoadMenu(GetModuleHandle(0),MAKEINTRESOURCE(menu));
}

SXGUIMenu::~SXGUIMenu()
{
	DestroyMenu(this->MenuHandle);
}

bool SXGUIMenu::InsertItem(int index,const char* text,WORD id,WORD id_sub_menu)
{
	char str[256];
	sprintf(str,"%s",text);
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STRING|MIIM_ID;
	mi.dwTypeData = str;
	mi.wID=id;

	HMENU SubMenu = this->GetSubMenu(id_sub_menu);
	BOOL bf = InsertMenuItem(id_sub_menu > 0 && SubMenu ? SubMenu : MenuHandle,index,TRUE,&mi);
	//DrawMenuBar(ParentHandle);
	return bf ? true : false;
}

bool SXGUIMenu::InsertPopupItem(int index,const char* text,WORD id,WORD id_sub_menu)
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

	HMENU SubMenu2 = this->GetSubMenu(id_sub_menu);
	BOOL bf = InsertMenuItem(id_sub_menu > 0 && SubMenu2 ? SubMenu2 : MenuHandle, index, TRUE, &mii);
	//DrawMenuBar(ParentHandle);
	return bf ? true : false;
}

bool SXGUIMenu::InsertSeparator(int index,WORD id,WORD id_sub_menu)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_ID | MIIM_FTYPE;
	mi.wID=id;
	mi.fType = MFT_SEPARATOR;

	HMENU SubMenu = this->GetSubMenu(id_sub_menu);
	BOOL bf = InsertMenuItem(id_sub_menu > 0 && SubMenu ? SubMenu : MenuHandle,index,TRUE,&mi);
	//DrawMenuBar(ParentHandle);
	return bf ? true : false;
}

bool SXGUIMenu::DeleteItem(WORD id)
{
	return DeleteMenu(this->MenuHandle,id,MF_BYCOMMAND) ? true : false;
}

HMENU SXGUIMenu::GetSubMenu(WORD id)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_SUBMENU;
	mi.hSubMenu = 0;
	BOOL bf = GetMenuItemInfo(MenuHandle,id,FALSE,&mi);

	return bf ? mi.hSubMenu : 0;
}

bool SXGUIMenu::CheckItem(WORD id,bool check)
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

bool SXGUIMenu::GetCheckedItem(WORD id)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STATE;
	BOOL bf = GetMenuItemInfo(MenuHandle,id,FALSE,&mi);
		if(mi.fState & MFS_CHECKED)
			return true;
	return false;
}

bool SXGUIMenu::EnableItem(WORD id,WORD enable)
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

	DWORD Enable = SXGUI_MENU_ENABLED;

		if(enable == SXGUI_MENU_DISABLED)
			Enable = MFS_DISABLED;
		else if(enable == SXGUI_MENU_GRAYED)
			Enable = MFS_GRAYED;
		else
			Enable = MFS_ENABLED;

	DWORD var = EnableMenuItem(MenuHandle,id,Enable);;
	//DrawMenuBar(ParentHandle);
	return ((var != 0xFFFFFFFF) || (var == -1)) ? true : false;
}

WORD SXGUIMenu::GetEnabledItem(WORD id)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STATE;
	BOOL bf = GetMenuItemInfo(MenuHandle,id,FALSE,&mi);

		if(mi.fState & MFS_DISABLED)
			return SXGUI_MENU_DISABLED;
		else if(mi.fState & MFS_GRAYED)
			return SXGUI_MENU_GRAYED;
		else
			return SXGUI_MENU_ENABLED;

	return -1;
}

HMENU SXGUIMenu::GetHMENU()
{
	return this->MenuHandle;
}

bool SXGUIMenu::SetToWindow(HWND hwnd)
{
		if(!hwnd)
			return false;
		if(!SetMenu(hwnd,this->MenuHandle))
			return false;
	return true;
}

//////////////////////

SXGUIPopupMenu::SXGUIPopupMenu(WORD menu)
{
	MenuHandle = LoadMenu(GetModuleHandle(0),MAKEINTRESOURCE(menu));
	MenuHandle = ::GetSubMenu(MenuHandle,0);
}

SXGUIPopupMenu::SXGUIPopupMenu()
{
	MENUINFO mi;
	MenuHandle = CreatePopupMenu();
	mi.cbSize = sizeof(MENUINFO);
	mi.fMask = MIM_STYLE;
	mi.dwStyle = MNS_NOTIFYBYPOS;
	SetMenuInfo(MenuHandle, &mi);
}

SXGUIPopupMenu::~SXGUIPopupMenu()
{
	DestroyMenu(this->MenuHandle);
}

bool SXGUIPopupMenu::InsertItem(int index,const char* text,WORD id,WORD id_sub_menu)
{
	char str[256];
	sprintf(str,"%s",text);
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STRING|MIIM_ID;
	mi.dwTypeData=str;
	mi.wID=id;

	HMENU SubMenu = this->GetSubMenu(id_sub_menu);
	BOOL bf = InsertMenuItem(id_sub_menu > 0 && SubMenu ? SubMenu : MenuHandle,index,TRUE,&mi);
	//DrawMenuBar(ParentHandle);
	return bf ? true : false;
}

bool SXGUIPopupMenu::InsertPopupItem(int index,const char* text,WORD id,WORD id_sub_menu)
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

	HMENU SubMenu2 = this->GetSubMenu(id_sub_menu);
	BOOL bf = InsertMenuItem(id_sub_menu > 0 && SubMenu2 ? SubMenu2 : MenuHandle, index, TRUE, &mii);
	//DrawMenuBar(ParentHandle);
	return bf ? true : false;
}

bool SXGUIPopupMenu::InsertSeparator(int index,WORD id,WORD id_sub_menu)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_ID | MIIM_FTYPE;
	mi.wID=id;
	mi.fType = MFT_SEPARATOR;

	HMENU SubMenu = this->GetSubMenu(id_sub_menu);
	BOOL bf = InsertMenuItem(id_sub_menu > 0 && SubMenu ? SubMenu : MenuHandle,index,TRUE,&mi);
	//DrawMenuBar(ParentHandle);
	return bf ? true : false;
}

bool SXGUIPopupMenu::DeleteItem(WORD id)
{
	return DeleteMenu(this->MenuHandle,id,MF_BYCOMMAND) ? true : false;
}

HMENU SXGUIPopupMenu::GetSubMenu(WORD id)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_SUBMENU;
	mi.hSubMenu = 0;
	BOOL bf = GetMenuItemInfo(MenuHandle,id,FALSE,&mi);

	return bf ? mi.hSubMenu : 0;
}

bool SXGUIPopupMenu::CheckItem(WORD id,bool check)
{
	DWORD var = CheckMenuItem(MenuHandle,id,(check ? MF_CHECKED : MF_UNCHECKED));
	//DrawMenuBar(ParentHandle);
	return ((var != 0xFFFFFFFF) || (var == -1)) ? true : false;
}

bool SXGUIPopupMenu::GetCheckedItem(WORD id)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STATE;
	BOOL bf = GetMenuItemInfo(MenuHandle,id,FALSE,&mi);
		if(mi.fState & MFS_CHECKED)
			return true;
	return false;
}

bool SXGUIPopupMenu::EnableItem(WORD id,WORD enable)
{
	DWORD Enable = SXGUI_MENU_ENABLED;

		if(enable == SXGUI_MENU_DISABLED)
			Enable = MFS_DISABLED;
		else if(enable == SXGUI_MENU_GRAYED)
			Enable = MFS_GRAYED;
		else
			Enable = MFS_ENABLED;

	DWORD var = EnableMenuItem(MenuHandle,id,Enable);;
	//DrawMenuBar(ParentHandle);
	return ((var != 0xFFFFFFFF) || (var == -1)) ? true : false;
}

WORD SXGUIPopupMenu::GetEnabledItem(WORD id)
{
	MENUITEMINFO mi;
	mi.cbSize=sizeof(MENUITEMINFO);
	mi.fMask=MIIM_STATE;
	BOOL bf = GetMenuItemInfo(MenuHandle,id,FALSE,&mi);

		if(mi.fState & MFS_DISABLED)
			return SXGUI_MENU_DISABLED;
		else if(mi.fState & MFS_GRAYED)
			return SXGUI_MENU_GRAYED;
		else
			return SXGUI_MENU_ENABLED;

	return -1;
}

bool SXGUIPopupMenu::Track(HWND hwnd,int x,int y)
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
			pos_y=x; 
			pos_y=y;
		}
		if(!hwnd)
			return false;

		if(!TrackPopupMenu(MenuHandle,TPM_LEFTALIGN,pos_x,pos_y,0,hwnd,0))
			return false;
	return true;
}


HMENU SXGUIPopupMenu::GetHMENU()
{
	return this->MenuHandle;
}