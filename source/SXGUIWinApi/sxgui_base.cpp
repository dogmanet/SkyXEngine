
#pragma once
#include <SXGUIWinApi\SXGUI_base.h>


SXGUIControl::SXGUIControl()
{
	WindowHandle = 0;
	ParentHandle = 0;
}

SXGUIControl::~SXGUIControl()
{
	::DestroyWindow(this->WindowHandle);
}

SXGUIControl::SXGUIControl(HWND window_handle,HWND parent_handle,WNDPROC handler)
{
	WindowHandle = window_handle;
	ParentHandle = parent_handle;
		if(handler != 0)
			OldProc = (WNDPROC)SetWindowLong(WindowHandle, GWL_WNDPROC, (LONG)handler);
}

void SXGUIControl::Init(HWND window_handle, HWND parent_handle, WNDPROC handler)
{
	WindowHandle = window_handle;
	ParentHandle = parent_handle;

		if(handler != 0)
		{	
			if ((WNDPROC)GetWindowLong(WindowHandle, GWL_WNDPROC) != handler)
			{
				DWORD ptrf = SetWindowLong(WindowHandle, GWL_WNDPROC, (LONG)handler);
				OldProc = (WNDPROC)ptrf;
			}
		}
	
}

HWND SXGUIControl::Parent()
{
	return ParentHandle;
}

bool SXGUIControl::Parent(HWND parent_handle)
{
	HWND OldParent = GetParent(this->WindowHandle);
	HWND tmp_parent = SetParent(this->WindowHandle,parent_handle);
		if(tmp_parent = OldParent)
		{
			ParentHandle = parent_handle;
			return true;
		}
	return false;
}

HWND SXGUIControl::GetHWND()
{
	return WindowHandle;
}

void SXGUIControl::SetFocus()
{
	::SetFocus(WindowHandle);
}

bool SXGUIControl::Focus()
{
	return GetFocus() == WindowHandle ? true : false;
}

void SXGUIControl::Visible(bool bf)
{
	ShowWindow(WindowHandle,bf ? SW_SHOW : SW_HIDE);
}

bool SXGUIControl::Visible()
{
	return IsWindowVisible(WindowHandle);
}

void * SXGUIControl::GetUserPtr()
{
	return(UserPtr);
}
void * SXGUIControl::SetUserPtr(void * ptr)
{
	void * oldPtr = UserPtr;
	UserPtr = ptr;
	return(oldPtr);
}

///////////////////

SXGUIComponent::SXGUIComponent()
{
	Hint = 0;
	CountKeyArrHandler = 0;
	GAlign = SXRectBool();
	ParentRect.top = ParentRect.bottom = ParentRect.left = ParentRect.right = 0;
	TransparenTextBf = true;

	BFSizingChangeTop = true;
	BFSizingChangeBottom = true;
	BFSizingChangeRight = true;
	BFSizingChangeLeft = true;

	MinSizeX = MinSizeY = 0;
	//this->Font = 0;
}

SXGUIComponent::~SXGUIComponent()
{
	mem_delete(this->Hint);
	//delete this->Font;
	//this->Font = 0;
}

void SXGUIComponent::InitComponent()
{
	COLORREF color_bk = GetBkColor(GetDC(this->WindowHandle));
	COLORREF color_text = GetTextColor(GetDC(this->WindowHandle));
	COLORREF color_brush = GetDCBrushColor(GetDC(this->WindowHandle));

	ColorText = color_text;
	ColorTextBk = color_bk;
	ColorBrush = color_brush;

	HBrush = CreateSolidBrush(ColorBrush);

	GetWindowRect(this->Parent(),&ParentRect);

	GetWindowRect(this->WindowHandle, &WinScreenRect);
	MapWindowPoints(this->Parent(), HWND_DESKTOP, (LPPOINT)&WinScreenRect, 2);

	RECT rc;
	GetWindowRect(this->WindowHandle, &rc);

	OffsetParentRect.top = OffsetParentRect.bottom = OffsetParentRect.left = OffsetParentRect.right = 0;

	//MinSizeX = rc.right - rc.left;
	//MinSizeY = rc.bottom - rc.top;

	//HFONT HandleFont;
	//SystemParametersInfo(SPI_GETICONTITLELOGFONT, 0, &HandleFont, 0);

	HFONT HandleFont = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(
		this->WindowHandle, 
		WM_SETFONT, 
		WPARAM(HandleFont),
		0);
	DeleteObject(HandleFont);
	//this->Font = new SXFont();
	//this->SetFont(this->Font);
}

bool SXGUIComponent::Enable()
{
	return ((IsWindowEnabled(this->GetHWND()) == 0) ? false : true);
}

void SXGUIComponent::Enable(bool bf)
{
	EnableWindow(this->GetHWND(), (bf ? 1 : 0));
}

bool SXGUIComponent::SetWinRect(RECT* rect,bool alignment_screen_space)
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	UINT width_screen = wrect.right;
	UINT heigth_screen = wrect.bottom;

	//MessageBox(0,ToPointChar(ToString(width_screen) + "|" + ToString(heigth_screen)),0,0);

	WORD x = rect->left;
	WORD y = rect->top;
	WORD width = rect->right - rect->left;
	WORD heigth = rect->bottom - rect->top;
	//MessageBox(0,ToPointChar(ToString(width) + "|" + ToString(heigth)),"wh",0);
		if(alignment_screen_space)
		{
			x = x < width_screen ? x : width_screen;
			y = y < heigth_screen ? y : heigth_screen;
			//MessageBox(0,ToPointChar(ToString(rect->right) + "|" + ToString(rect->bottom)),"wh",0);
			width = rect->right <= width_screen ? width : width_screen - x;
			heigth = rect->bottom <= heigth_screen ? heigth : heigth_screen - y;

				/*if(x < 0)
					x = 0;
				if(y < 0)
					y = 0;*/
		}
	//MessageBox(0,ToPointChar(ToString(width) + "|" + ToString(heigth)),"wh",0);
		BOOL bf = MoveWindow(this->GetHWND(), x, y, width, heigth, true);
	return bf == TRUE ? true : false;
}

void SXGUIComponent::GetWinRect(RECT* rect)
{
	GetWindowRect(this->GetHWND(), rect);
}

bool SXGUIComponent::SetClientRect(RECT* rect,bool alignment_screen_space)
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	long width_screen = wrect.right;
	long heigth_screen = wrect.bottom;

	//MessageBox(0,ToPointChar(ToString(width_screen) + "|" + ToString(heigth_screen)),0,0);

	long x = rect->left;
	long y = rect->top;
	long width = rect->right - rect->left;
	long heigth = rect->bottom - rect->top;
	//MessageBox(0,ToPointChar(ToString(width) + "|" + ToString(heigth)),"wh",0);
		if(alignment_screen_space)
		{
			x = x < width_screen ? x : width_screen;
			y = y < heigth_screen ? y : heigth_screen;
			//MessageBox(0,ToPointChar(ToString(rect->right) + "|" + ToString(rect->bottom)),"wh",0);
			width = rect->right <= width_screen ? width : width_screen - x;
			heigth = rect->bottom <= heigth_screen ? heigth : heigth_screen - y;
		}
	//MessageBox(0,ToPointChar(ToString(width) + "|" + ToString(heigth)),"wh",0);
		BOOL bf = MoveWindow(this->GetHWND(), x, y, width, heigth, TRUE);
	/*RECT* qwerr = GetClientRect();
		if(x != qwerr->left || y != qwerr->top || width != qwerr->right - qwerr->left || heigth != qwerr->bottom - qwerr->top)
			MessageBox(0,0,0,0);*/
	return bf == TRUE ? true : false;
}

void SXGUIComponent::GetClientRect(RECT* rect)
{
	GetWindowRect(this->GetHWND(), rect);
	MapWindowPoints(NULL, this->Parent(), (LPPOINT)rect, 2);
}

void SXGUIComponent::SetFont(const char* name,int height,int width,int weight,int italic,int underline,int strike_out)
{
	HFONT hfont = this->GetFont();
	LOGFONT LogFont;
	ZeroMemory(&LogFont,sizeof(&LogFont));
	GetObject(hfont, sizeof(LOGFONT), &LogFont);
		if(name != 0)
			strcpy(LogFont.lfFaceName, name);

		if(height != -1)
			LogFont.lfHeight = height;

		if(width != -1)
			LogFont.lfWidth = width;

		if(weight != -1)
			LogFont.lfWeight = weight;

		if(italic != -1)
			LogFont.lfItalic = italic;

		if(underline != -1)
			LogFont.lfUnderline = underline;

		if(strike_out != -1)
			LogFont.lfStrikeOut = strike_out;
	DeleteObject(hfont);
	hfont = ::CreateFontIndirect(&LogFont);
	SendMessage(this->GetHWND(), WM_SETFONT, WPARAM(hfont), 1);
}

void SXGUIComponent::SetFont(HFONT hfont)
{
	SendMessage(this->GetHWND(), WM_SETFONT, WPARAM(hfont), 1);
}
/*
void SXGUIComponent::GetFont(SXFont* font)
{
		if(font)
		{
			this->Font->SetHandleFont((HFONT)SendMessage(this->WindowHandle, WM_GETFONT, 0, 0));
			memcpy(font,this->Font,sizeof(SXFont));
		}
	//return (HFONT)SendMessage(this->WindowHandle, WM_GETFONT, 0, 0);
}*/

HFONT SXGUIComponent::GetFont()
{
	return (HFONT)SendMessage(this->WindowHandle, WM_GETFONT, 0, 0);
}

//какая то херня происходила при вызове диалога смены шрифта
//шрифт менялся только на дефлотный и все ... и все
/*void SXGUIComponent::SetFont(SXFont* font)
{
		if(font)
		{
			memcpy(this->Font,font,sizeof(SXFont));
			//this->Font->SetHandleFont(font->GetHandleFont());
			SendMessage(this->WindowHandle,WM_SETFONT,WPARAM(font->GetHandleFont()),0);
		}
}*/

HFONT SXGUIComponent::ParentFont()
{
	return (HFONT)SendMessage(this->Parent(), WM_GETFONT, 0, 0);
}

bool SXGUIComponent::IsParentFont()
{
	HFONT ChildHFont = this->GetFont();
	HFONT ParentHFont = this->ParentFont();
	LOGFONT ChildLF,ParentLF;
	ZeroMemory(&ChildLF,sizeof(&ChildLF));
	GetObject(ChildHFont, sizeof(LOGFONT), &ChildLF);
	ZeroMemory(&ParentLF,sizeof(&ParentLF));
	GetObject(ParentHFont, sizeof(LOGFONT), &ParentLF);
		if(ChildLF.lfCharSet != ParentLF.lfCharSet)
			return false;
		if(ChildLF.lfClipPrecision != ParentLF.lfClipPrecision)
			return false;
		if(ChildLF.lfEscapement != ParentLF.lfEscapement)
			return false;
		if(ChildLF.lfHeight != ParentLF.lfHeight)
			return false;
		if(ChildLF.lfItalic != ParentLF.lfItalic)
			return false;
		if(ChildLF.lfOrientation != ParentLF.lfOrientation)
			return false;
		if(ChildLF.lfOutPrecision != ParentLF.lfOutPrecision)
			return false;
		if(ChildLF.lfPitchAndFamily != ParentLF.lfPitchAndFamily)
			return false;
		if(ChildLF.lfQuality != ParentLF.lfQuality)
			return false;
		if(ChildLF.lfStrikeOut != ParentLF.lfStrikeOut)
			return false;
		if(ChildLF.lfUnderline != ParentLF.lfUnderline)
			return false;
		if(ChildLF.lfWeight != ParentLF.lfWeight)
			return false;
		if(ChildLF.lfWidth != ParentLF.lfWidth)
			return false;
		if(strcmp(ChildLF.lfFaceName,ParentLF.lfFaceName) != 0)
			return false;
	return true;
}


bool SXGUIComponent::ModifyStyle(long style_add,long style_del)
{

		if(style_add)
		{
			if (!SetWindowLong(this->GetHWND(), GWL_STYLE, GetWindowLong(this->GetHWND(), GWL_STYLE) | style_add))
					return false;
		}

		if(style_del)
		{
			if (!SetWindowLong(this->GetHWND(), GWL_STYLE, GetWindowLong(this->GetHWND(), GWL_STYLE)&~style_del))
					return false;
		}
	return true;
}

bool SXGUIComponent::ModifyExStyle(long style_add,long style_del)
{

		if(style_add)
		{
			if (!SetWindowLong(this->GetHWND(), GWL_EXSTYLE, GetWindowLong(this->GetHWND(), GWL_EXSTYLE) | style_add))
					return false;
		}

		if(style_del)
		{
			if (!SetWindowLong(this->GetHWND(), GWL_EXSTYLE, GetWindowLong(this->GetHWND(), GWL_EXSTYLE)&~style_del))
					return false;
		}
	return true;
}

void SXGUIComponent::ShowHint(bool bf)
{
		if(Hint != 0)
			Hint->Visible(bf);
		else
		{
			Hint = new SXGUIHint(this->GetHWND());
			Hint->Visible(bf);
		}
}

bool SXGUIComponent::ShowHint()
{
		if(Hint != 0)
			return Hint->Visible();
	return false;
}

void SXGUIComponent::SetHintText(const char* text)
{
		if(Hint == 0)
			Hint = new SXGUIHint(this->GetHWND());
	Hint->SetText(text);
}

const char* SXGUIComponent::GetHintText()
{
		if(Hint != 0)
			return Hint->GetText();
	return 0;
}

void SXGUIComponent::GetHintText(char* buf)
{
		if(Hint != 0)
			Hint->GetText(buf);
		else
			buf[0] = 0;
}

void SXGUIComponent::UpdateSize()
{
	char classname[256];
	GetClassName(this->GetHWND(), classname, 256);
	if (strcmp(classname, WC_LISTVIEW) == 0)
		int qwerty = 0;
	RECT rect;
	RECT win_screen_rect;
	BOOL bf = GetWindowRect(this->GetHWND(), &rect);

	GetWindowRect(this->GetHWND(), &win_screen_rect);
	//MapWindowPoints(this->ParentHandle, HWND_DESKTOP, (LPPOINT)&win_screen_rect, 2);

	RECT NewParentRect;
	GetWindowRect(this->Parent(),&NewParentRect);

		//if(NewParentRect.top != ParentRect.top)
	//MessageBox(0,ToPointChar(NewParentRect.top),ToPointChar(ParentRect.top),0);

	//трансляция координат в пространство родителя
	//MapWindowPoints(NULL, this->ParentHandle, (LPPOINT)&rect, 2);

	if (abs(OffsetParentRect.left) > 30000 || abs(OffsetParentRect.right) > 30000 || abs(OffsetParentRect.top) > 30000 || abs(OffsetParentRect.bottom) > 30000)
		return;

	//смещение по всем направлениям
	RECT offset;
	offset.left = OffsetParentRect.left ? OffsetParentRect.left : NewParentRect.left - ParentRect.left;
	offset.right = OffsetParentRect.right ? OffsetParentRect.right : NewParentRect.right - ParentRect.right;
	offset.top = OffsetParentRect.top ? OffsetParentRect.top : NewParentRect.top - ParentRect.top;
	offset.bottom = OffsetParentRect.bottom ? OffsetParentRect.bottom : NewParentRect.bottom - ParentRect.bottom;

	//MessageBox(0,ToPointChar(offset.top),ToPointChar(ParentRect.top),0);


		if(GAlign.left) //rect.left
		{
				if(GAlign.right)
					rect.right -= offset.left;
				/*else
					rect.right += offset.left;*/
		}
		else
		{
			rect.left = rect.left - offset.left;
			rect.right -= offset.left;
		}

		if(GAlign.top) //rect.top
		{
			//MessageBox(0,ToPointChar(offset.top),ToPointChar(rect.top),0);
			//rect.top = rect.top + offset.top;
				if(GAlign.bottom)
					rect.bottom -= offset.top;
		}
		else
		{
			rect.top = rect.top - offset.top;
			rect.bottom -= offset.top;
		}

		if(GAlign.right) //rect.right
		{
			rect.right = rect.right + offset.right;
				if(!GAlign.left) //rect.left
					rect.left += offset.right;
		}

		if(GAlign.bottom) //rect.bottom
		{
			
			rect.bottom = rect.bottom + offset.bottom;
				if(!GAlign.top) //rect.top
					rect.top += offset.bottom;
		}

	MapWindowPoints(NULL, this->Parent(), (LPPOINT)&rect, 2);

	
	//MapWindowPoints(this->ParentHandle, HWND_DESKTOP, (LPPOINT)&WinScreenRect, 2);
	//MessageBox(0,ToPointChar(rect.bottom - rect.top),ToPointChar(rect.bottom - rect.top),0);
	
	bf = MoveWindow(this->GetHWND(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
	//SetWinRect(&rect,true);
	GetWindowRect(this->Parent(),&this->ParentRect);
	GetWindowRect(this->GetHWND(), &WinScreenRect);

	OffsetParentRect.top = OffsetParentRect.bottom = OffsetParentRect.left = OffsetParentRect.right = 0;

	DWORD err = GetLastError();
	int qweerty = 0;
}

void SXGUIComponent::UpdateRect()
{
	//MessageBox(0,"UpdateRect","UpdateRect",0);
	RECT tmprect;
	GetWindowRect(this->Parent(),&tmprect);
	OffsetParentRect.top = tmprect.top - ParentRect.top;
	OffsetParentRect.left = tmprect.left - ParentRect.left;
	OffsetParentRect.bottom = tmprect.bottom - ParentRect.bottom;
	OffsetParentRect.right = tmprect.right - ParentRect.right;
	//MessageBox(0,ToPointChar(tmprect.top - ParentRect.top),ToPointChar(0),0);
	GetWindowRect(this->Parent(),&ParentRect);
	//GetWindowRect(this->WindowHandle,&WinScreenRect);
}

void SXGUIComponent::SetColorText(BYTE r,BYTE g, BYTE b)
{
	this->ColorText = RGB(r,g,b);
	InvalidateRect(this->GetHWND(), 0, 1);
}

void SXGUIComponent::SetColorText(DWORD color)
{
	this->ColorText = color;
	InvalidateRect(this->GetHWND(), 0, 1);
}

DWORD SXGUIComponent::GetColorText()
{
	return this->ColorText;
}

void SXGUIComponent::SetTransparentTextBk(bool bf)
{
	this->TransparenTextBf = bf;
	InvalidateRect(this->GetHWND(), 0, 1);
}

bool SXGUIComponent::GetTransparentTextBk()
{
	return this->TransparenTextBf;
}

void SXGUIComponent::SetColorTextBk(BYTE r,BYTE g, BYTE b)
{
	this->ColorTextBk = RGB(r,g,b);
	InvalidateRect(this->GetHWND(), 0, 1);
}

void SXGUIComponent::SetColorTextBk(DWORD color)
{
	this->ColorTextBk = color;
	InvalidateRect(this->GetHWND(), 0, 1);
}

DWORD SXGUIComponent::GetColorTextBk()
{
	return this->ColorTextBk;
}


bool SXGUIComponent::SetColorBrush(BYTE r,BYTE g,BYTE b)
{
	ColorBrush = RGB(r,g,b);

	DeleteObject(HBrush);
	HBrush = CreateSolidBrush(ColorBrush);
	InvalidateRect(this->GetHWND(), 0, 1);
	return true;
}

bool SXGUIComponent::SetColorBrush(DWORD color)
{
	ColorBrush = color;

	DeleteObject(HBrush);
	HBrush = CreateSolidBrush(ColorBrush);
	InvalidateRect(this->GetHWND(), 0, 1);
	return true;
}

DWORD SXGUIComponent::GetColorBrush()
{
	return ColorBrush;	
}

HBRUSH SXGUIComponent::GetBrush()
{
	return this->HBrush;
}

bool SXGUIComponent::AddHandler(HandlerMsg Handler,UINT Msg,WPARAM wParam,bool considerWparam,LPARAM lParam,bool considerLparam,bool isMain)
{
		if(CountKeyArrHandler < SXGUI_COUNT_HANDLERS_MSG_IN_ARR)
		{
			ArrHandler[CountKeyArrHandler].Handler = Handler;
			ArrHandler[CountKeyArrHandler].Msg = Msg;
			ArrHandler[CountKeyArrHandler].WParam = wParam;
			ArrHandler[CountKeyArrHandler].BFCondiderWParam = considerWparam;
			ArrHandler[CountKeyArrHandler].LParam = lParam;
			ArrHandler[CountKeyArrHandler].BFCondiderLParam = considerLparam;
			ArrHandler[CountKeyArrHandler].IsMainFunction = isMain;
			CountKeyArrHandler += 1;
			return true;
		}
		else
			return false;
}

bool SXGUIComponent::AddHandler(HandlerMsg Handler,UINT Msg)
{
		if(CountKeyArrHandler < SXGUI_COUNT_HANDLERS_MSG_IN_ARR)
		{
			ArrHandler[CountKeyArrHandler].Handler = Handler;
			ArrHandler[CountKeyArrHandler].Msg = Msg;
			ArrHandler[CountKeyArrHandler].WParam = 0;
			ArrHandler[CountKeyArrHandler].BFCondiderWParam = false;
			ArrHandler[CountKeyArrHandler].LParam = 0;
			ArrHandler[CountKeyArrHandler].BFCondiderLParam = false;
			ArrHandler[CountKeyArrHandler].IsMainFunction = false;
			CountKeyArrHandler += 1;
			return true;
		}
		else
			return false;
}

int SXGUIComponent::GetCountKeyArrHandler()
{
	return CountKeyArrHandler;
}

LRESULT SXGUIComponent::ExecuteHandler(int key,HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
		if(key < CountKeyArrHandler)
			return (ArrHandler[key].Handler)(hwnd, msg, wParam, lParam);
		else
			return 0;
}

UINT SXGUIComponent::GetMsgHandler(int key)
{
		if(key < CountKeyArrHandler)
			return ArrHandler[key].Msg;
		else
			return 0;
}

WPARAM SXGUIComponent::GetWParamHandler(int key)
{
		if(key < CountKeyArrHandler)
			return ArrHandler[key].WParam;
		else
			return 0;
}

LPARAM SXGUIComponent::GetLParamHandler(int key)
{
		if(key < CountKeyArrHandler)
			return ArrHandler[key].LParam;
		else
			return 0;
}

bool SXGUIComponent::GetConsiderWParamHandler(int key)
{
		if(key < CountKeyArrHandler)
			return ArrHandler[key].BFCondiderWParam;
		else
			return false;
}

bool SXGUIComponent::GetConsiderLParamHandler(int key)
{
		if(key < CountKeyArrHandler)
			return ArrHandler[key].BFCondiderLParam;
		else
			return false;
}

bool SXGUIComponent::IsMainFunction(int key)
{
		if(key < CountKeyArrHandler)
			return ArrHandler[key].IsMainFunction;
		else
			return false;
}

HandlerMsg SXGUIComponent::GetHandlerFunction(int key)
{
		if(key < CountKeyArrHandler)
			return ArrHandler[key].Handler;
		else
			return false;
}

///////////////////////////////////

SXGUITextual::SXGUITextual()
{

}

SXGUITextual::~SXGUITextual()
{

}

bool SXGUITextual::SetText(const char* text)
{
	return SXGUIFuctinon::SetText(this,text);
}

/*const char* SXGUITextual::GetText()
{
	return SXGUIFuctinon::GetTextOut(this);
}*/

void SXGUITextual::GetText(char* buf,int size)
{
	SXGUIFuctinon::GetText(this,buf,size);
}

int SXGUITextual::GetTextLen()
{
	return SXGUIFuctinon::GetTextLen(this);
}

///////////////////////////////////

BOOL IsEdit(HWND hWnd)
{
	if(hWnd == NULL)
		return FALSE;

	TCHAR szClassName[6];
	return ::GetClassNameA(hWnd, szClassName, 6) &&
		stricmp(szClassName, "Edit") == 0;
}

LRESULT CALLBACK WndProcAllDefault(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	
	if (Component)
	{
			if(msg == WM_KEYDOWN)
			{
				if(wParam == 'A' && (GetKeyState(VK_CONTROL) & 0x80))
				{
					// User pressed Ctrl-A.  Let's select-all
					if(IsEdit(hwnd))
					{
						SendMessage(hwnd, EM_SETSEL, 0, -1);
						return(1);
					}
				}
			}
			HandlerMsg MainFunction = 0;								//главная функция-обработчик, ее значение и будет возвращаться
			HandlerMsg SecondFunction[SXGUI_COUNT_HANDLERS_MSG_IN_ARR];	//массив дополнительных обработчиков на сообщения, их значения не должны смысла
			int CountSecond = 0;	//количество дополнительных функций-обработчиков
				//инициализируем вышеописанные данные
				for(int i=0;i<Component->GetCountKeyArrHandler();i++)
				{
						if(Component->GetMsgHandler(i) == msg)
						{
								if(
									Component->GetConsiderWParamHandler(i) && Component->GetWParamHandler(i) == wParam && 
									Component->GetConsiderLParamHandler(i) && Component->GetLParamHandler(i) == lParam
									)
								{
										if(Component->IsMainFunction(i))
										{
											MainFunction = Component->GetHandlerFunction(i);
										}
										else
										{
											SecondFunction[CountSecond] = Component->GetHandlerFunction(i);
											CountSecond++;
										}

								}
								else if(
									!Component->GetConsiderWParamHandler(i) && 
									Component->GetConsiderLParamHandler(i) && Component->GetLParamHandler(i) == lParam
									)
								{
										if(Component->IsMainFunction(i))
										{
											MainFunction = Component->GetHandlerFunction(i);
										}
										else
										{
											SecondFunction[CountSecond] = Component->GetHandlerFunction(i);
											CountSecond++;
										}

								}
								else if(
									Component->GetConsiderWParamHandler(i) && Component->GetWParamHandler(i) == wParam && 
									!Component->GetConsiderLParamHandler(i)
									)
								{
										if(Component->IsMainFunction(i))
										{
											MainFunction = Component->GetHandlerFunction(i);
										}
										else
										{
											SecondFunction[CountSecond] = Component->GetHandlerFunction(i);
											CountSecond++;
										}

								}
								else if(
									!Component->GetConsiderWParamHandler(i) && 
									!Component->GetConsiderLParamHandler(i)
									)
								{
										if(Component->IsMainFunction(i))
										{
											MainFunction = Component->GetHandlerFunction(i);
										}
										else
										{
											SecondFunction[CountSecond] = Component->GetHandlerFunction(i);
											CountSecond++;
										}

								}
						}
				}

				//вызываем все дополнительные функции обработчики
				for(int i=0;i<CountSecond;i++)
				{
					SecondFunction[i](hwnd, msg, wParam, lParam);
				}

					//если была найдена главная функция то вызываем ее и возвраащем ее значение
					if(MainFunction)
						return MainFunction(hwnd, msg, wParam, lParam);
					//else
						//return Component->OldProc(hwnd, msg, wParam, lParam);
					//!!!а если нет то что???
					//else mf
				//return CallWindowProc(Component->OldProc, hwnd, msg, wParam, lParam);
				return CallWindowProc(Component->OldProc, hwnd, msg, wParam, lParam);;
		}
	
	return CallWindowProc(Component->OldProc, hwnd, msg, wParam, lParam);
}

/////////////////////////

bool SXGUIFuctinon::ScroolBars(ISXGUIControl *Control,bool h,bool v)
{
	return ShowScrollBar(Control->GetHWND(),SB_HORZ,h ? 1 : 0) && ShowScrollBar(Control->GetHWND(),SB_VERT,v ? 1 : 0);
}

bool SXGUIFuctinon::ScrollBarV(ISXGUIControl *Control)
{
	long style = GetWindowLong(Control->GetHWND(),GWL_STYLE);
		if(style & WS_VSCROLL)
			return true;
	return false;
}

bool SXGUIFuctinon::ScrollBarH(ISXGUIControl *Control)
{
	long style = GetWindowLong(Control->GetHWND(),GWL_STYLE);
		if(style & WS_HSCROLL)
			return true;
	return false;
}

bool SXGUIFuctinon::ScrollLine(ISXGUIControl *Control,int scroll,int dir,int count)
{
	long _scroll = scroll == SXGUI_SCROLL_TYPE_VERT ? WM_VSCROLL : WM_HSCROLL;
	long _dir = 0;
		if(_scroll == WM_VSCROLL)
			_dir = dir == SXGUI_SCROLL_DIR_DOWN ? SB_LINEDOWN : SB_LINEUP /*SB_BOTTOM :SB_TOP*/;
		else
			_dir = dir == SXGUI_SCROLL_DIR_RIGTH ? SB_LINERIGHT : SB_LINELEFT /*SB_RIGHT :SB_LEFT*/;

	bool bf = true;
		for(int i=0;i<count;i++)
		{
			bf = SendMessage(Control->GetHWND(),_scroll,_dir,0);
		}

	return bf;//SendMessage(Control->GetHWND(),_scroll,_dir,LPARAM(count)) == 0 ? true : false;
}

bool SXGUIFuctinon::SetText(ISXGUIControl*const Control, const char* text)
{
	if(!SetWindowText(Control->GetHWND(),text))
			return false;
		else
			return true;
}	

void SXGUIFuctinon::GetText(ISXGUIControl *Control,char* buf,int count)
{
	GetWindowText(Control->GetHWND(),buf,count);
}

int SXGUIFuctinon::GetTextLen(ISXGUIControl *Control)
{
	return GetWindowTextLength(Control->GetHWND());
}

/////////////////////////

BOOL CALLBACK SXGUIEnumChildWindow::EnumChildProcUpdateImgButton(HWND hwnd,LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	
	if (Component)
	{
		char ClassName[256];
		int error = GetClassName(hwnd, ClassName, 256);
		if (error && strcmp(ClassName, "SXGUIBUTTONIMG") == 0 && Component)
			SendMessage(hwnd, WM_KILLFOCUS, 0, lParam);
	}
		
	return TRUE;
}

BOOL CALLBACK SXGUIEnumChildWindow::EnumChildProcUpdateSize(HWND hwnd,LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd, GWL_USERDATA);
	if (Component)
		Component->UpdateSize();

	return TRUE;
}

BOOL CALLBACK SXGUIEnumChildWindow::EnumChildProcUpdateRect(HWND hwnd,LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd, GWL_USERDATA);
	if (Component)
		Component->UpdateRect();
		
	return TRUE;
}

BOOL CALLBACK SXGUIEnumChildWindow::EnumChildProcMouseMove(HWND hwnd,LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd, GWL_USERDATA);

	if (Component)
	{
		POINT p;
		GetCursorPos(&p);
		RECT rect;
		GetWindowRect(hwnd, &rect);

		if (Component && !((p.x >= rect.left && p.x <= rect.right) && (p.y >= rect.top && p.y <= rect.bottom)))
		{
			SendMessage(Component->GetHWND(), WM_NCMOUSEMOVE, lParam, 0);
		}
	}

	return TRUE;
}

////////////////////////////////

int SXGUIBaseHandlers::InitHandlerMsg(ISXGUIComponent* Component)
{
		if(!Component->AddHandler(SXGUIBaseHandlers::CtlColorChange,WM_CTLCOLORSTATIC,0,0,0,0,true))
			return 1;
		if(!Component->AddHandler(SXGUIBaseHandlers::CtlColorChange,WM_CTLCOLOREDIT,0,0,0,0,true))
			return 2;
		if(!Component->AddHandler(SXGUIBaseHandlers::CtlColorChange,WM_CTLCOLORBTN,0,0,0,0,true))
			return 3;
		if(!Component->AddHandler(SXGUIBaseHandlers::CtlColorChange,WM_CTLCOLORLISTBOX,0,0,0,0,true))
			return 4;
	
		if(!Component->AddHandler(SXGUIBaseHandlers::SizeChange,WM_SIZE,0,0,0,0,true))
			return 5;
		if(!Component->AddHandler(SXGUIBaseHandlers::SizingChange,WM_SIZING,0,0,0,0,true))
			return 6;
		if(!Component->AddHandler(SXGUIBaseHandlers::WinDestroy,WM_DESTROY,0,0,0,0,true))
			return 7;

		if(!Component->AddHandler(SXGUIBaseHandlers::MoveChange,WM_MOVE,0,0,0,0,true))
			return 8;
		if(!Component->AddHandler(SXGUIBaseHandlers::MouseMoveChange,WM_MOUSEMOVE,0,0,0,0,true))
			return 9;
		if(!Component->AddHandler(SXGUIBaseHandlers::MaximuzeWinChange,WM_SYSCOMMAND,SC_MAXIMIZE,true,0,0,true))
			return 10;
		if(!Component->AddHandler(SXGUIBaseHandlers::SetCursorChange,WM_SETCURSOR,0,0,0,0,true))
			return 11;

	return 0;
}

LRESULT SXGUIBaseHandlers::CtlColorChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong((HWND)lParam, GWL_USERDATA);

		if(Component)
		{
				if(Component->GetTransparentTextBk())
					SetBkMode((HDC)wParam,TRANSPARENT);
				else
					SetBkColor((HDC)wParam, Component->GetColorTextBk());
			SetTextColor((HDC)wParam, Component->GetColorText());
			return (long)Component->GetBrush();
		}
}

LRESULT SXGUIBaseHandlers::SizeChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcUpdateSize,lParam);
	return TRUE;
}

LRESULT SXGUIBaseHandlers::SizingChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd, GWL_USERDATA);
	
	if (!Component)
		return TRUE;

	RECT rc;
	GetWindowRect(hwnd, &rc);
	POINT p;
	GetCursorPos(&p);

	RECT lpRect = *((LPRECT)lParam);

		if(Component)
		{

				if(!Component->BFSizingChangeTop)
					lpRect.top = rc.top;

				if(!Component->BFSizingChangeLeft)
					lpRect.left = rc.left;

				if(!Component->BFSizingChangeRight)
					lpRect.right = rc.right;

				if(!Component->BFSizingChangeBottom)
					lpRect.bottom = rc.bottom;

				if (Component->MinSizeX != 0 && Component->MinSizeY != 0)
				{
						if(lpRect.right - lpRect.left < Component->MinSizeX && Component->BFSizingChangeRight)
						{
							lpRect.right += Component->MinSizeX - (lpRect.right - lpRect.left);
						}

						if(lpRect.bottom - lpRect.top < Component->MinSizeY && Component->BFSizingChangeBottom)
						{
							lpRect.bottom += Component->MinSizeY - (lpRect.bottom - lpRect.top);
						}
				}
		}

	*((LPRECT)lParam) = lpRect;

	return TRUE;
}

LRESULT SXGUIBaseHandlers::MoveChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcUpdateRect,lParam);
	return TRUE;
}

LRESULT SXGUIBaseHandlers::MouseMoveChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcMouseMove,lParam);
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT SXGUIBaseHandlers::SetCursorChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcMouseMove,lParam);
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT SXGUIBaseHandlers::MaximuzeWinChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcUpdateSize,lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT SXGUIBaseHandlers::WinDestroy(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

////////////////////////////////

bool SXGUIRegClass::RegButtonImg()
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = DefWindowProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = GetModuleHandle(0);
	wc.hIcon         = 0;
	wc.hCursor       = 0;
	wc.hbrBackground = 0;
	wc.lpszMenuName  = 0;
	wc.lpszClassName = "SXGUIBUTTONIMG";

		if(!RegisterClass(&wc)) 
			return false;
	return true;
}

bool SXGUIRegClass::RegGroupBox()
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = 0;
	wc.hCursor = 0;
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = "SXGUIGROUPBOX";

	if (!RegisterClass(&wc))
		return false;
	return true;
}

void SXGUIDialogs::SelectFile(int type, char* path, char* name, const char* stdpath, const char* filter)
{
	OPENFILENAME ofn;

	char tpath[1024];
	char tname[256];
	
	if (!path && !name)
		return;

	if (path)
		tpath[0] = tpath[1] = 0;

	if (name)
		tname[0] = tname[1] = 0;

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hInstance = GetModuleHandle(0);
	ofn.hwndOwner = 0;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = tpath;
	ofn.nMaxFile = sizeof(tpath);
	ofn.lpstrInitialDir = stdpath;

	if (name)
	{
		ofn.lpstrFileTitle = tname;
		ofn.nMaxFileTitle = sizeof(tname);
	}

	ofn.Flags = 0;

	//if (path)
	ofn.Flags |= OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	/*if (name)
		ofn.Flags |= OFN_FILEMUSTEXIST;*/
	BOOL Result = FALSE;
	if (type == SXGUI_DIALOG_FILE_OPEN)
		Result = GetOpenFileName(&ofn);
	else if (type == SXGUI_DIALOG_FILE_SAVE)
		Result = GetSaveFileName(&ofn);

	if (Result)
	{
		if (path)
			strcpy(path, tpath);

		if (name)
			strcpy(name, tname);
	}
}