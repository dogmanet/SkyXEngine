
#include <SXGUIWinApi\SXGUI_edit.h>

#pragma once

SXGUIEdit::SXGUIEdit() {}
SXGUIEdit::~SXGUIEdit() {}

SXGUIEdit::SXGUIEdit(const char* caption,WORD x,WORD y,WORD width,WORD heigth,DWORD exstyle,DWORD style,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							exstyle,
							"EDIT",
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

SXGUIEdit::SXGUIEdit(const char* caption,WORD x,WORD y,WORD width,WORD heigth,HWND parent,WNDPROC handler,DWORD id)
{
	this->WindowHandle = CreateWindowEx(
							0,
							"EDIT",
							caption,
							(parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
							x,y,width,heigth,
							parent,
							(HMENU)id,
							GetModuleHandle(0),
							0);
	this->Init(this->GetHWND(),parent,(handler == 0 ? WndProcAllDefault : handler));
	SetWindowLong(GetHWND(),GWL_USERDATA,(LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
}

bool SXGUIEdit::ReadOnly()
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if(style & ES_READONLY)
			return true;
		else
			return false;
}

bool SXGUIEdit::ReadOnly(bool bf)
{
	long p = bf ? 1 : 0;
	SendMessage(this->GetHWND(), EM_SETREADONLY, bf ? 1 : 0, 0);
	return this->ReadOnly() == bf;
}

int SXGUIEdit::Align()
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if(style & ES_RIGHT)
			return SXGUI_TEXT_ALIGN_RIGHT;
		else if(style & ES_CENTER)
			return SXGUI_TEXT_ALIGN_CENTER;
		else
			return SXGUI_TEXT_ALIGN_LEFT;
}

bool SXGUIEdit::Align(int align)
{
		if (align == SXGUI_TEXT_ALIGN_LEFT)
			return this->ModifyStyle(ES_LEFT,0);
		else if (align == SXGUI_TEXT_ALIGN_RIGHT)
			return this->ModifyStyle(ES_RIGHT,0);
		else if (align == SXGUI_TEXT_ALIGN_CENTER)
			return this->ModifyStyle(ES_CENTER,0);
	return false;
}

bool SXGUIEdit::Password()
{
	long style = GetWindowLong(this->GetHWND(),GWL_STYLE);
		if(style & ES_PASSWORD)
			return true;
		else
			return false;
}

bool SXGUIEdit::Password(bool bf)
{
	long p = bf ? 1 : 0;
	SendMessage(this->GetHWND(), EM_SETPASSWORDCHAR, bf ? '*' : 0, 0);
	this->SetFocus();
	UpdateWindow(this->GetHWND());
	return this->Password() == bf;
}