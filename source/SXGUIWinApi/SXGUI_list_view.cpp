
#include <SXGUIWinApi\SXGUI_list_view.h>

#pragma once

SXGUIListView::SXGUIListView()
{

}

SXGUIListView::SXGUIListView(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	this->WindowHandle = CreateWindowEx(
		exstyle,
		WC_LISTVIEW,
		0,
		style,
		x, y, width, heigth,
		parent,
		(HMENU)id,
		GetModuleHandle(0),
		0);

	ListView_SetExtendedListViewStyleEx(this->WindowHandle, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB);
	this->Init(this->GetHWND(), parent, handler);
	SetWindowLong(GetHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

SXGUIListView::SXGUIListView(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id)
{
	this->WindowHandle = CreateWindowEx(
		0,
		WC_LISTVIEW,
		0,
		(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | /*WS_VSCROLL | WS_HSCROLL | LVS_ALIGNLEFT |*/ /*LVS_SHOWSELALWAYS |*/ WS_BORDER | LVS_REPORT,
		x, y, width, heigth,
		parent,
		(HMENU)id,
		GetModuleHandle(0),
		0);

	ListView_SetExtendedListViewStyleEx(this->WindowHandle, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB);
	this->Init(this->GetHWND(), parent, handler);
	SetWindowLong(GetHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

int SXGUIListView::InsertColumn(char* text, int col, int width)
{
	RECT rcl;
	::GetClientRect(this->WindowHandle, &rcl);

	LVCOLUMN lvc;
	memset(&lvc, 0, sizeof(LVCOLUMN));
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;
	lvc.cx = width;
	lvc.pszText = text;

	return ListView_InsertColumn(this->WindowHandle, col, &lvc);
}

int SXGUIListView::AddColumn(char* text, int width)
{
	return InsertColumn(text, GetCountColumns(), width);
}

void SXGUIListView::DeleteColumn(int col)
{
	ListView_DeleteColumn(this->WindowHandle, col);
}

int SXGUIListView::GetCountColumns()
{
	HWND hwndHeader = ListView_GetHeader(this->WindowHandle);
	return ::SendMessage(hwndHeader, HDM_GETITEMCOUNT, 0, 0);
}

int SXGUIListView::AddString()
{
	LVITEM lvi;
	memset(&lvi, 0, sizeof(LVCOLUMN));
	lvi.mask = LVIF_TEXT;
	lvi.iItem = GetCountString();
	lvi.iSubItem = 0;

	return ListView_InsertItem(this->WindowHandle, &lvi);
}

void SXGUIListView::DeleteString(int str)
{
	ListView_DeleteItem(this->WindowHandle, str);
}

int SXGUIListView::GetCountString()
{
	return ListView_GetItemCount(this->WindowHandle);
}

void SXGUIListView::SetTextItem(char* text, int col, int str)
{
	ListView_SetItemText(this->WindowHandle, col, str, text);
}

void SXGUIListView::GetTextItem(char* text, int col, int str, int sizebuff)
{
	ListView_GetItemText(this->WindowHandle, col, str, text, sizebuff);
}

int SXGUIListView::GetSelString(int start)
{
	return ::SendMessage(this->WindowHandle, LVM_GETNEXTITEM, start, LVNI_SELECTED);
}

void SXGUIListView::SetSelString(int str)
{
	ListView_SetItemState(this->WindowHandle, str, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	this->SetFocus();
}

int SXGUIListView::GetSelColumn()
{
	return ListView_GetSelectedColumn(this->WindowHandle);
}

void SXGUIListView::SetSelColumn(int col)
{
	ListView_SetSelectedColumn(this->WindowHandle, col);
}

void SXGUIListView::Clear()
{
	ClearStrings();
	ListView_DeleteAllItems(this->WindowHandle);

	while (GetCountColumns() > 0)
	{
		DeleteColumn(0);
	}
}

void SXGUIListView::ClearStrings()
{
	TabCtrl_DeleteAllItems(this->WindowHandle);
}