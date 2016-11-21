
#include <SXGUIWinApi\SXGUI_list_box.h>

#pragma once

SXGUIListBox::SXGUIListBox()
{

}

SXGUIListBox::SXGUIListBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							"LISTBOX",
							0,
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

SXGUIListBox::SXGUIListBox(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id,bool miltiple_sel)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"LISTBOX",
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE  | LBS_HASSTRINGS | WS_VSCROLL | WS_BORDER | (miltiple_sel ? LBS_EXTENDEDSEL : 0),
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(), parent, (handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

bool SXGUIListBox::MultipleSel()
{
	long style = GetWindowLong(this->GetHWND(), GWL_STYLE);
		if(style & LBS_EXTENDEDSEL)
			return true;
		else
			return false;
}


bool SXGUIListBox::InsertItem(int index,const char* text)
{
		if(!SendMessage(this->GetHWND(),LB_INSERTSTRING,WPARAM(index),LPARAM(text))) 
			return false;
	return true;
}

bool SXGUIListBox::AddItem(const char* text)
{
		if(!SendMessage(this->GetHWND(),LB_ADDSTRING,0,LPARAM(text))) 
			return false;
	return true;
}

bool SXGUIListBox::DeleteItem(int index)
{
		if(!SendMessage(this->GetHWND(),LB_DELETESTRING,WPARAM(index),0))
			return false;
	return true;
}

int SXGUIListBox::GetCountItem()
{
	return SendMessage(this->GetHWND(),LB_GETCOUNT,0,0);
}

bool SXGUIListBox::SetSel(int index)
{
		if(!SendMessage(this->GetHWND(),LB_SETCURSEL,WPARAM(index),0))
			return false;
	return true;
}

int SXGUIListBox::GetSel()
{
	return SendMessage(this->GetHWND(),LB_GETCURSEL,0,0);
}


bool SXGUIListBox::SetItemData(int index,LPARAM data)
{
		if(!SendMessage(this->GetHWND(),LB_SETITEMDATA,WPARAM(index),data))
			return false;
	return true;
}

LPARAM SXGUIListBox::GetItemData(int index)
{
	return SendMessage(this->GetHWND(),LB_GETITEMDATA,WPARAM(index),0);
}

bool SXGUIListBox::Clear()
{
		if(!SendMessage(this->GetHWND(),LB_RESETCONTENT,0,0))
			return false;
	return true;
}

bool SXGUIListBox::SetTextItem(int index,const char* text)
{
	LPARAM data = this->GetItemData(index);
		/*if(!this->DeleteItem(index))
			return false;
		if(!this->InsertItem(index,text))
			return false;
	return this->SetItemData(index,data);*/

	this->DeleteItem(index);
	this->InsertItem(index,text);
	return this->SetItemData(index,data);
}

char* SXGUIListBox::GetItemText(int index)
{
	WORD CountLength = GetItemTextLength(index) + 1;
	char* text = new char[CountLength];
	SendMessage(this->GetHWND(),LB_GETTEXT,WPARAM(index),LPARAM(text));
	return text;
}

void SXGUIListBox::GetItemText(int index,char* buf)
{
	SendMessage(this->GetHWND(),LB_GETTEXT,WPARAM(index),LPARAM(buf));
}

int SXGUIListBox::GetItemTextLength(int index)
{
	return SendMessage(this->GetHWND(),LB_GETTEXTLEN,WPARAM(index),0);
}


int SXGUIListBox::GetMultipleSelCount()
{
	return SendMessage(this->GetHWND(),LB_GETSELCOUNT,0,0);
}

bool SXGUIListBox::SetMultipleSel(int index,bool sel)
{
		if(!SendMessage(this->GetHWND(),LB_SETSEL,WPARAM(sel ? 1 : 0),LPARAM(index)))
			return false;
	return true;
}

bool SXGUIListBox::GetMultipleSel(int index)
{
		if(!SendMessage(this->GetHWND(),LB_GETSEL,WPARAM(index),0))
			return false;	
	return true;
}

int* SXGUIListBox::GetMultipleSelArr()
{
	WORD count = this->GetMultipleSelCount();
		if(count <= 0)
			return 0;
	int* arr = new int[count];
	SendMessage(this->GetHWND(),LB_GETSELITEMS,WPARAM(count),LPARAM(arr));
	return arr;
}


bool SXGUIListBox::ScrollBars(bool h,bool v)
{
	return SXGUIFuctinon::ScroolBars(this,h,v);
}

bool SXGUIListBox::ScrollBarV()
{
	return SXGUIFuctinon::ScrollBarV(this);;
}

bool SXGUIListBox::ScrollBarH()
{
	return SXGUIFuctinon::ScrollBarH(this);
}

bool SXGUIListBox::ScrollLine(WORD scroll,WORD dir,int count)
{
	return SXGUIFuctinon::ScrollLine(this,scroll,dir,count);
}