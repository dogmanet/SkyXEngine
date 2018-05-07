
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "combobox.h"

CGUIComboBox::CGUIComboBox()
{

}

CGUIComboBox::CGUIComboBox(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							"COMBOBOX",
							0,
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

CGUIComboBox::CGUIComboBox(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							0,
							"COMBOBOX",
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

bool CGUIComboBox::insertItem(int index,const char* text)
{
	if (SendMessage(this->getHWND(), CB_INSERTSTRING, (WPARAM)index, (LPARAM)text) < 0)
			return false;
	return true;
}

bool CGUIComboBox::deleteItem(int index)
{
	if (SendMessage(this->getHWND(), CB_DELETESTRING, WPARAM(index), 0) < 0)
			return false;
	return true;
}

int CGUIComboBox::getCount()
{
	return SendMessage(this->getHWND(), CB_GETCOUNT, 0, 0);
}

bool CGUIComboBox::setSel(int index)
{
	if (SendMessage(this->getHWND(), CB_SETCURSEL, WPARAM(index), 0) < 0)
			return false;
	return true;
}

int CGUIComboBox::getSel()
{
	return SendMessage(this->getHWND(), CB_GETCURSEL, 0, 0);
}


bool CGUIComboBox::clear()
{
	if (!SendMessage(this->getHWND(), CB_RESETCONTENT, 0, 0))
			return false;
	return true;
}

bool CGUIComboBox::setItemData(int index,LPARAM data)
{
	if (!SendMessage(this->getHWND(), CB_SETITEMDATA, WPARAM(index), data))
			return false;
	return true;
}

LPARAM CGUIComboBox::getItemData(int index)
{
	return SendMessage(this->getHWND(), CB_GETITEMDATA, WPARAM(index), 0);
}

bool CGUIComboBox::setItemText(int index,const char* text)
{
	LPARAM data = getItemData(index);
		if(!deleteItem(index))
			return false;
		if(!insertItem(index,text))
			return false;
		if(!setItemData(index,data))
			return false;
	return true;
}

void CGUIComboBox::getItemText(int index,char* buf)
{
	SendMessage(this->getHWND(), CB_GETLBTEXT, WPARAM(index), LPARAM(buf));
}

int CGUIComboBox::getItemTextLength(int index)
{
	return SendMessage(this->getHWND(), CB_GETLBTEXTLEN, WPARAM(index), 0);
}

bool CGUIComboBox::addItem(const char* text)
{
	return this->insertItem(-1,text);
}