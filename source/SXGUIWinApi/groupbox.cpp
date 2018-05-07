
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "groupbox.h"


LRESULT WndProcGroupBoxPaint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CGUIGroupBox *GroupBox = dynamic_cast<CGUIGroupBox*>((ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA));
	
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
	if (!GroupBox->StrText || (GroupBox->StrText && GroupBox->StrText[0] == 0))
		arrpoint[0].x += 5;
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

	HBRUSH color_rect = CreateSolidBrush(GroupBox->getColorBrush());
	FillRect(hdcp, &clrectorigin, color_rect);

	HFONT hFont = GroupBox->getHFont();
	if (!hFont)
		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	SelectObject(hdcp, hFont);
	long tmplen = GetWindowTextLength(GroupBox->getHWND());
	if (GetWindowTextLength(GroupBox->getHWND())+1 > GroupBox->LenStrText)
	{
		GroupBox->LenStrText = GetWindowTextLength(GroupBox->getHWND())+1;
		mem_delete(GroupBox->StrText);
		GroupBox->StrText = new char[GroupBox->LenStrText];
	}

	GetWindowText(GroupBox->getHWND(), GroupBox->StrText, GroupBox->LenStrText);

	SetTextColor(hdcp, GroupBox->getColorText());
	SetBkColor(hdcp, GroupBox->getColorTextBk());

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

CGUIGroupBox::CGUIGroupBox()
{
	LenStrText = 0; StrText = 0;
}

CGUIGroupBox::~CGUIGroupBox()
{
	mem_delete(StrText);
}

CGUIGroupBox::CGUIGroupBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	LenStrText = strlen(caption)+1;
	StrText = new char[LenStrText];
	sprintf(StrText, "%s", caption);
	m_hWindow = CreateWindowEx(
							exstyle,
							SXGUI_DEF_GROUPBOX,
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	this->init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->initComponent();
}

CGUIGroupBox::CGUIGroupBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj)
{
	LenStrText = strlen(caption)+1;
	StrText = new char[LenStrText];
	sprintf(StrText, "%s", caption);
	m_hWindow = CreateWindowEx(
							0,
							SXGUI_DEF_GROUPBOX,
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE /*| BS_GROUPBOX*/,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	
	this->init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->initComponent();
	this->addHandler(WndProcGroupBoxPaint, WM_PAINT,0,0,0,0,true);
}


