
#include <SXGUIWinApi\SXGUI_combo_box.h>

#pragma once

SXGUIComboBox::SXGUIComboBox()
{

}

SXGUIComboBox::SXGUIComboBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							"COMBOBOX",
							caption,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(), parent, (handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

SXGUIComboBox::SXGUIComboBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"COMBOBOX",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(), parent, (handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

bool SXGUIComboBox::InsertItem(int index,const char* text)
{
	if (SendMessage(this->GetHWND(), CB_INSERTSTRING, (WPARAM)index, (LPARAM)text) < 0)
			return false;
	return true;
}

bool SXGUIComboBox::DeleteItem(int index)
{
	if (SendMessage(this->GetHWND(), CB_DELETESTRING, WPARAM(index), 0) < 0)
			return false;
	return true;
}

int SXGUIComboBox::GetCount()
{
	return SendMessage(this->GetHWND(), CB_GETCOUNT, 0, 0);
}

bool SXGUIComboBox::SetSel(int index)
{
	if (SendMessage(this->GetHWND(), CB_SETCURSEL, WPARAM(index), 0) < 0)
			return false;
	return true;
}

int SXGUIComboBox::GetSel()
{
	return SendMessage(this->GetHWND(), CB_GETCURSEL, 0, 0);
}


bool SXGUIComboBox::Clear()
{
	if (!SendMessage(this->GetHWND(), CB_RESETCONTENT, 0, 0))
			return false;
	return true;
}

bool SXGUIComboBox::SetItemData(int index,LPARAM data)
{
	if (!SendMessage(this->GetHWND(), CB_SETITEMDATA, WPARAM(index), data))
			return false;
	return true;
}

LPARAM SXGUIComboBox::GetItemData(int index)
{
	return SendMessage(this->GetHWND(), CB_GETITEMDATA, WPARAM(index), 0);
}

bool SXGUIComboBox::SetItemText(int index,const char* text)
{
	LPARAM data = this->GetItemData(index);
		if(!this->DeleteItem(index))
			return false;
		if(!this->InsertItem(index,text))
			return false;
		if(!this->SetItemData(index,data))
			return false;
	return true;
}

void SXGUIComboBox::GetItemText(int index,char* buf)
{
	SendMessage(this->GetHWND(), CB_GETLBTEXT, WPARAM(index), LPARAM(buf));
}

int SXGUIComboBox::GetItemTextLength(int index)
{
	return SendMessage(this->GetHWND(), CB_GETLBTEXTLEN, WPARAM(index), 0);
}

bool SXGUIComboBox::AddItem(const char* text)
{
	return this->InsertItem(-1,text);
}