
#include <SXGUIWinApi\SXGUI_group_box.h>

#pragma once

SXGUIGroupBox::SXGUIGroupBox()
{

}

SXGUIGroupBox::SXGUIGroupBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							"BUTTON",
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

SXGUIGroupBox::SXGUIGroupBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							WS_EX_TRANSPARENT,
							"BUTTON",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | BS_GROUPBOX,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

/*
LRESULT CALLBACK WndProcGroupBoxDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXGUIGroupBox *GroupBox = (SXGUIGroupBox *)GetWindowLong(hwnd,GWL_USERDATA);

	switch(msg)
	{
		case WM_CTLCOLORSTATIC:
			{
				SXGUIComponent *Component = (SXGUIComponent *)GetWindowLong((HWND)lParam,GWL_USERDATA);

					if(Component)
					{
							if(Component->TransparentBf)
								SetBkMode((HDC)wParam,TRANSPARENT);
							else
								SetBkColor((HDC)wParam, RGB(Component->ColorBk.r, Component->ColorBk.g, Component->ColorBk.b));
						SetTextColor((HDC)wParam, RGB(Component->ColorText.r, Component->ColorText.g, Component->ColorText.b));
		
						return (long)Component->HBrush;
					}
			}
		break;
	}
	
	return CallWindowProc(GroupBox->OldProc, hwnd, msg, wParam, lParam);
}*/