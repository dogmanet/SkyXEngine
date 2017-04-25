
#include <SXGUIWinApi\SXGUI_group_box.h>

#pragma once

LRESULT WndProcGroupBoxPaint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	SXGUIGroupBox *GroupBox = dynamic_cast<SXGUIGroupBox*>((ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA));
	/*CallWindowProc(Component->OldProc, hwnd, msg, wParam, lParam);*/
	//DefWindowProc(hwnd, msg, wParam, lParam);
	PAINTSTRUCT PaintStruct;
	HDC hdcp = BeginPaint(hwnd, &PaintStruct);
	
	RECT clrectorigin,clrect, clrect2;
	GetClientRect(hwnd, &clrect);
	clrectorigin = clrect;
	clrect.top += 3;
	clrect.bottom -= 2;
	clrect.left += 2;
	clrect.right -= 2;

	POINT arrpoint[6];
	arrpoint[0].x = clrect.left+5;
	arrpoint[0].y = clrect.top;

	arrpoint[1].x = clrect.left;
	arrpoint[1].y = clrect.top;

	arrpoint[2].x = clrect.left;
	arrpoint[2].y = clrect.bottom;

	arrpoint[3].x = clrect.right;
	arrpoint[3].y = clrect.bottom;

	arrpoint[4].x = clrect.right;
	arrpoint[4].y = clrect.top;

	arrpoint[5].x = clrect.left;
	arrpoint[5].y = clrect.top;
	
	clrect2 = clrect;
	clrect2.top += 1;
	clrect2.bottom -= 1;
	clrect2.left += 1;
	clrect2.right -= 1;

	HBRUSH color_rect = CreateSolidBrush(GroupBox->GetColorBrush());
	FillRect(hdcp, &clrectorigin, color_rect);

	HFONT hFont = GroupBox->GetFont();
	if (!hFont)
		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	SelectObject(hdcp, hFont);
	long tmplen = GetWindowTextLength(GroupBox->GetHWND());
	if (GetWindowTextLength(GroupBox->GetHWND())+1 > GroupBox->LenStrText)
	{
		GroupBox->LenStrText = GetWindowTextLength(GroupBox->GetHWND())+1;
		mem_delete(GroupBox->StrText);
		GroupBox->StrText = new char[GroupBox->LenStrText];
	}

	GetWindowText(GroupBox->GetHWND(), GroupBox->StrText, GroupBox->LenStrText);

	SetTextColor(hdcp, GroupBox->GetColorText());
	SetBkColor(hdcp, GroupBox->GetColorTextBk());

	clrect.top -= 3;
	clrect.left += 10;
	clrect.right -= 3;
	DrawText(hdcp, GroupBox->StrText, strlen(GroupBox->StrText), &clrect, DT_LEFT);

	SIZE sz;
	GetTextExtentPoint32(hdcp, GroupBox->StrText, strlen(GroupBox->StrText), &sz);

	if (sz.cx > clrect.right - clrect.left)
		arrpoint[5].x = arrpoint[4].x - 5;
	else
		arrpoint[5].x = clrect.left + sz.cx;
	Polyline(hdcp, arrpoint, 6);

	DeleteObject(color_rect);
	DeleteObject(hFont);
	EndPaint(hwnd, &PaintStruct);
	return 1;
}

SXGUIGroupBox::SXGUIGroupBox()
{
	LenStrText = 0; StrText = 0;
}

SXGUIGroupBox::SXGUIGroupBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	LenStrText = strlen(caption)+1;
	StrText = new char[LenStrText];
	sprintf(StrText, "%s", caption);
	this->WindowHandle = CreateWindowEx(
							exstyle,
							"SXGUIGROUPBOX",
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
	LenStrText = strlen(caption)+1;
	StrText = new char[LenStrText];
	sprintf(StrText, "%s", caption);
	this->WindowHandle = CreateWindowEx(
							0,
							"SXGUIGROUPBOX",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE /*| BS_GROUPBOX*/,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(), parent, (handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
	this->AddHandler(WndProcGroupBoxPaint, WM_PAINT,0,0,0,0,true);
}


