
#include <SXGUIWinApi\SXGUI_button.h>

SXGUIButton::SXGUIButton()
{

}

SXGUIButton::SXGUIButton(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							"Button",
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

SXGUIButton::SXGUIButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, int image, HWND parent, WNDPROC handler, DWORD id)
{
	DWORD image_style = BS_TEXT;

	if (image == SXGUI_BUTTON_IMAGE_ICON)
		image_style = BS_ICON;
	else if (image == SXGUI_BUTTON_IMAGE_BITMAT)
		image_style = BS_BITMAP;

	this->WindowHandle = CreateWindowEx(
							0,
							"BUTTON",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | image_style,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->WindowHandle, parent, (handler == 0 ? WndProcAllDefault : handler));
	ISXGUIComponent* component = dynamic_cast<ISXGUIComponent*>(this);
	SetWindowLong(this->WindowHandle, GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

bool SXGUIButton::SetIconInFile(const char* path)
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if(!(style & BS_ICON))
			return false;

	HICON icon = (HICON)LoadImage(NULL,path,IMAGE_ICON,0,0,LR_LOADFROMFILE);

		if(icon == 0)
			return false;

	SendMessage(this->GetHWND(),BM_SETIMAGE,IMAGE_ICON,(LPARAM)icon);
	return true;
}

bool SXGUIButton::SetBmpInFile(const char* path)
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if(!(style & BS_BITMAP))
			return false;

	HBITMAP bmp = (HBITMAP)LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

		if(bmp == 0)
			return false;

	SendMessage(this->GetHWND(),BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)bmp);
	return true;
}


bool SXGUIButton::SetIconInResourse(WORD id)
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if(!(style & BS_ICON))
			return false;

	HICON icon = LoadCursor(GetModuleHandle(0),MAKEINTRESOURCE(id));

		if(icon == 0)
			return false;

	SendMessage(this->GetHWND(),BM_SETIMAGE,IMAGE_ICON,(LPARAM)icon);
	return true;
}

bool SXGUIButton::SetBmpInResourse(WORD id)
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if(!(style & BS_BITMAP))
			return false;

	HBITMAP bmp = LoadBitmap(GetModuleHandle(0),MAKEINTRESOURCE(id));

		if(bmp == 0)
			return false;

	SendMessage(this->GetHWND(),BM_SETIMAGE,IMAGE_BITMAP,(LPARAM)bmp);
	return true;
}