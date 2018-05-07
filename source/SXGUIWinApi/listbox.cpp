
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "listbox.h"


CGUIListBox::CGUIListBox()
{

}

CGUIListBox::CGUIListBox(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, ID idObj)
{
	m_hWindow = CreateWindowEx(
							exstyle,
							"LISTBOX",
							0,
							style,
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

CGUIListBox::CGUIListBox(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, ID idObj, bool miltiple_sel)
{
	m_hWindow = CreateWindowEx(
							0,
							"LISTBOX",
							0,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | LBS_HASSTRINGS | LBS_NOTIFY | WS_VSCROLL | WS_BORDER | (miltiple_sel ? LBS_EXTENDEDSEL : 0),
							x,y,width,heigth,
							parent,
							(HMENU)idObj,
							GetModuleHandle(0),
							0);
	init(this->getHWND(), parent, (handler == 0 ? WndProcAllDefault : handler), idObj);
	SetWindowLong(getHWND(), GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	initComponent();
}

bool CGUIListBox::getStateMultiSel()
{
	long style = GetWindowLong(this->getHWND(), GWL_STYLE);
		if(style & LBS_EXTENDEDSEL)
			return true;
		else
			return false;
}

void CGUIListBox::setStateMultiSel(bool isMultiSel)
{
	modifyStyle((isMultiSel ? LBS_EXTENDEDSEL : 0), (isMultiSel ? LBS_EXTENDEDSEL : 0));
}


bool CGUIListBox::insertItem(int index,const char* text)
{
		if(!SendMessage(this->getHWND(),LB_INSERTSTRING,WPARAM(index),LPARAM(text))) 
			return false;
	return true;
}

bool CGUIListBox::addItem(const char* text)
{
		if(!SendMessage(this->getHWND(),LB_ADDSTRING,0,LPARAM(text))) 
			return false;
	return true;
}

bool CGUIListBox::deleteItem(int index)
{
		if(!SendMessage(this->getHWND(),LB_DELETESTRING,WPARAM(index),0))
			return false;
	return true;
}

int CGUIListBox::getItemCount()
{
	return SendMessage(this->getHWND(),LB_GETCOUNT,0,0);
}

bool CGUIListBox::setSel(int index)
{
	return SendMessage(this->getHWND(), LB_SETCURSEL, (WPARAM)index, 0);
}

int CGUIListBox::getSel()
{
	return SendMessage(this->getHWND(),LB_GETCURSEL,0,0);
}


bool CGUIListBox::setItemData(int index,LPARAM data)
{
		if(!SendMessage(this->getHWND(),LB_SETITEMDATA,WPARAM(index),data))
			return false;
	return true;
}

LPARAM CGUIListBox::getItemData(int index)
{
	return SendMessage(this->getHWND(),LB_GETITEMDATA,WPARAM(index),0);
}

bool CGUIListBox::clear()
{
		if(!SendMessage(this->getHWND(),LB_RESETCONTENT,0,0))
			return false;
	return true;
}

bool CGUIListBox::setItemText(int index, const char* text)
{
	LPARAM data = getItemData(index);
	int sel = getSel();
		/*if(!this->DeleteItem(index))
			return false;
		if(!this->InsertItem(index,text))
			return false;
	return this->SetItemData(index,data);*/

	deleteItem(index);
	insertItem(index,text);
	if(sel == index)
	{
		setSel(index);
	}
	return setItemData(index,data);
}

void CGUIListBox::getItemText(int index,char* buf)
{
	SendMessage(this->getHWND(),LB_GETTEXT,WPARAM(index),LPARAM(buf));
}

int CGUIListBox::getItemTextLength(int index)
{
	return SendMessage(this->getHWND(),LB_GETTEXTLEN,WPARAM(index),0);
}


int CGUIListBox::getMultiSelCount()
{
	return SendMessage(this->getHWND(),LB_GETSELCOUNT,0,0);
}

bool CGUIListBox::setMultiSel(int index,bool sel)
{
		if(!SendMessage(this->getHWND(),LB_SETSEL,WPARAM(sel ? 1 : 0),LPARAM(index)))
			return false;
	return true;
}

bool CGUIListBox::getMultiSel(int index)
{
		if(!SendMessage(this->getHWND(),LB_GETSEL,WPARAM(index),0))
			return false;	
	return true;
}

int CGUIListBox::getMultiSelArr(int **ppArr)
{
	if (!ppArr)
		return 0;

	int iCount = getMultiSelCount();

	if (iCount <= 0)
		return iCount;

	*ppArr = new int[iCount];
	SendMessage(this->getHWND(), LB_GETSELITEMS, WPARAM(iCount), LPARAM(*ppArr));
	return iCount;
}
