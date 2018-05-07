
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "button.h"

CGUIButton::CGUIButton()
{

}

CGUIButton::CGUIButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							"Button",
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

CGUIButton::CGUIButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, SXGUI_BUTTON_IMAGE type_image, HWND parent, WNDPROC handler, ID idObj)
{
	DWORD image_style = BS_TEXT;

	if (type_image == SXGUI_BUTTON_IMAGE_ICON)
		image_style = BS_ICON;
	else if (type_image == SXGUI_BUTTON_IMAGE_BITMAT)
		image_style = BS_BITMAP;

	m_hWindow = CreateWindowEx(
							0,
							"BUTTON",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | image_style,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(m_hWindow, parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	ISXGUIComponent* component = dynamic_cast<ISXGUIComponent*>(this);
	SetWindowLong(m_hWindow, GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

bool CGUIButton::setIconFromFile(const char* path)
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if(!(style & BS_ICON))
			return false;

	HICON icon = (HICON)LoadImage(NULL,path,IMAGE_ICON,0,0,LR_LOADFROMFILE);

		if(icon == 0)
			return false;

	SendMessage(this->getHWND(),BM_SETIMAGE,IMAGE_ICON,(LPARAM)icon);
	return true;
}

bool CGUIButton::setBmpFromFile(const char* path)
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if(!(style & BS_BITMAP))
			return false;

	HBITMAP bmp = (HBITMAP)LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

		if(bmp == 0)
			return false;

	SendMessage(this->getHWND(),BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)bmp);
	return true;
}


bool CGUIButton::setIconFromResourse(UINT idRes)
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if(!(style & BS_ICON))
			return false;

		HICON icon = LoadCursor(GetModuleHandle(0), MAKEINTRESOURCE(idRes));

		if(icon == 0)
			return false;

	SendMessage(this->getHWND(),BM_SETIMAGE,IMAGE_ICON,(LPARAM)icon);
	return true;
}

bool CGUIButton::setBmpFromResourse(UINT idRes)
{
	long style = GetWindowLong(this->getHWND(),GWL_STYLE);
		if(!(style & BS_BITMAP))
			return false;

		HBITMAP bmp = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(idRes));

		if(bmp == 0)
			return false;

	SendMessage(this->getHWND(),BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)bmp);
	return true;
}