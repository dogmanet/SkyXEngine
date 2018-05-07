
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "base.h"


CGUIControl::CGUIControl()
{
	m_hWindow = 0;
	m_hParent = 0;
	m_id = -1;
}

CGUIControl::~CGUIControl()
{
	::DestroyWindow(m_hWindow);
}

CGUIControl::CGUIControl(HWND window_handle, HWND parent_handle, WNDPROC handler)
{
	m_hWindow = window_handle;
	m_hParent = parent_handle;
	if (handler != 0)
		m_lpfnPrevProc = (WNDPROC)SetWindowLong(m_hWindow, GWL_WNDPROC, (LONG)handler);
}

void CGUIControl::init(HWND window_handle, HWND parent_handle, WNDPROC handler, ID idObj)
{
	m_hWindow = window_handle;
	m_hParent = parent_handle;

	if (handler != 0)
	{
		if ((WNDPROC)GetWindowLong(m_hWindow, GWL_WNDPROC) != handler)
		{
			DWORD ptrf = SetWindowLong(m_hWindow, GWL_WNDPROC, (LONG)handler);
			m_lpfnPrevProc = (WNDPROC)ptrf;
		}
	}

	m_id = idObj;
}

HWND CGUIControl::getParent()
{
	return m_hParent;
}

bool CGUIControl::setParent(HWND parent_handle)
{
	HWND OldParent = GetParent(m_hWindow);
	HWND tmp_parent = SetParent(m_hWindow, parent_handle);
	if (tmp_parent = OldParent)
	{
		m_hParent = parent_handle;
		return true;
	}
	return false;
}

HWND CGUIControl::getHWND()
{
	return m_hWindow;
}

void CGUIControl::setFocus()
{
	::SetFocus(m_hWindow);
}

bool CGUIControl::getFocus()
{
	return GetFocus() == m_hWindow ? true : false;
}

void CGUIControl::setVisible(bool bf)
{
	ShowWindow(m_hWindow, bf ? SW_SHOW : SW_HIDE);
}

bool CGUIControl::getVisible()
{
	return IsWindowVisible(m_hWindow);
}

WNDPROC CGUIControl::getPrevWndProc()
{
	return m_lpfnPrevProc;
}

void* CGUIControl::getUserPtr()
{
	return(m_pUserPtr);
}
void* CGUIControl::setUserPtr(void *ptr)
{
	void * oldPtr = m_pUserPtr;
	m_pUserPtr = ptr;
	return(oldPtr);
}

//##########################################################################

CGUIComponent::CGUIComponent()
{
	m_pHint = 0;

	m_aFollowParentSide[SXGUI_SIDE_TOP] = false;
	m_aFollowParentSide[SXGUI_SIDE_BOTTOM] = false;
	m_aFollowParentSide[SXGUI_SIDE_RIGHT] = false;
	m_aFollowParentSide[SXGUI_SIDE_LEFT] = false;

	m_rcParentScreen.top = m_rcParentScreen.bottom = m_rcParentScreen.left = m_rcParentScreen.right = 0;
	m_isTransparentTextBk = true;

	m_aStrethSide[SXGUI_SIDE_TOP] = true;
	m_aStrethSide[SXGUI_SIDE_BOTTOM] = true;
	m_aStrethSide[SXGUI_SIDE_RIGHT] = true;
	m_aStrethSide[SXGUI_SIDE_LEFT] = true;

	m_iMinSizeWidth = m_iMinSizeHeight = 0;
	//Font = 0;
}

CGUIComponent::~CGUIComponent()
{
	mem_delete(m_pHint);
	//delete Font;
	//Font = 0;
}

void CGUIComponent::initComponent()
{
	COLORREF color_bk = GetBkColor(GetDC(m_hWindow));
	COLORREF color_text = GetTextColor(GetDC(m_hWindow));
	COLORREF color_brush = GetDCBrushColor(GetDC(m_hWindow));

	m_dwColorText = color_text;
	m_dwColorTextBk = color_bk;
	m_dwColorBrush = color_brush;

	m_hBrush = CreateSolidBrush(m_dwColorBrush);

	GetWindowRect(getParent(), &m_rcParentScreen);

	GetWindowRect(m_hWindow, &m_rcWinScreen);
	MapWindowPoints(getParent(), HWND_DESKTOP, (LPPOINT)&m_rcWinScreen, 2);

	RECT rc;
	GetWindowRect(m_hWindow, &rc);

	m_rcOffsetParent.top = m_rcOffsetParent.bottom = m_rcOffsetParent.left = m_rcOffsetParent.right = 0;

	//MinSizeX = rc.right - rc.left;
	//MinSizeY = rc.bottom - rc.top;

	//HFONT HandleFont;
	//SystemParametersInfo(SPI_GETICONTITLELOGFONT, 0, &HandleFont, 0);

	HFONT HandleFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(
		m_hWindow,
		WM_SETFONT,
		WPARAM(HandleFont),
		0);
	DeleteObject(HandleFont);
	//Font = new SXFont();
	//SetFont(Font);
}

bool CGUIComponent::getEnable()
{
	return ((IsWindowEnabled(getHWND()) == 0) ? false : true);
}

void CGUIComponent::setEnable(bool bf)
{
	EnableWindow(getHWND(), (bf ? 1 : 0));
}

bool CGUIComponent::setWinRect(const RECT* rect, bool alignment_screen_space)
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
	if (alignment_screen_space)
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
	BOOL bf = MoveWindow(getHWND(), x, y, width, heigth, true);
	return bf == TRUE ? true : false;
}

void CGUIComponent::getWinRect(RECT* rect)
{
	GetWindowRect(getHWND(), rect);
}

bool CGUIComponent::setClientRect(const RECT* rect, bool alignment_screen_space)
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
	if (alignment_screen_space)
	{
		x = x < width_screen ? x : width_screen;
		y = y < heigth_screen ? y : heigth_screen;
		//MessageBox(0,ToPointChar(ToString(rect->right) + "|" + ToString(rect->bottom)),"wh",0);
		width = rect->right <= width_screen ? width : width_screen - x;
		heigth = rect->bottom <= heigth_screen ? heigth : heigth_screen - y;
	}
	//MessageBox(0,ToPointChar(ToString(width) + "|" + ToString(heigth)),"wh",0);
	BOOL bf = MoveWindow(getHWND(), x, y, width, heigth, TRUE);
	/*RECT* qwerr = GetClientRect();
	if(x != qwerr->left || y != qwerr->top || width != qwerr->right - qwerr->left || heigth != qwerr->bottom - qwerr->top)
	MessageBox(0,0,0,0);*/
	return bf == TRUE ? true : false;
}

void CGUIComponent::getClientRect(RECT* rect)
{
	GetWindowRect(getHWND(), rect);
	MapWindowPoints(NULL, getParent(), (LPPOINT)rect, 2);
}

void CGUIComponent::setFont(const char* name, int height, int width, int weight, int italic, int underline, int strike_out)
{
	HFONT hfont = getHFont();
	LOGFONT LogFont;
	ZeroMemory(&LogFont, sizeof(&LogFont));
	GetObject(hfont, sizeof(LOGFONT), &LogFont);
	if (name != 0)
		strcpy(LogFont.lfFaceName, name);

	if (height != -1)
		LogFont.lfHeight = height;

	if (width != -1)
		LogFont.lfWidth = width;

	if (weight != -1)
		LogFont.lfWeight = weight;

	if (italic != -1)
		LogFont.lfItalic = italic;

	if (underline != -1)
		LogFont.lfUnderline = underline;

	if (strike_out != -1)
		LogFont.lfStrikeOut = strike_out;
	DeleteObject(hfont);
	hfont = ::CreateFontIndirect(&LogFont);
	SendMessage(getHWND(), WM_SETFONT, WPARAM(hfont), 1);
}

void CGUIComponent::setHFont(HFONT hfont)
{
	SendMessage(getHWND(), WM_SETFONT, WPARAM(hfont), 1);
}
/*
void CGUIComponent::GetFont(SXFont* font)
{
if(font)
{
Font->SetHandleFont((HFONT)SendMessage(m_hWindow, WM_GETFONT, 0, 0));
memcpy(font,Font,sizeof(SXFont));
}
//return (HFONT)SendMessage(m_hWindow, WM_GETFONT, 0, 0);
}*/

HFONT CGUIComponent::getHFont()
{
	return (HFONT)SendMessage(m_hWindow, WM_GETFONT, 0, 0);
}

//какая то херня происходила при вызове диалога смены шрифта
//шрифт менялся только на дефлотный и все ... и все
/*void CGUIComponent::SetFont(SXFont* font)
{
if(font)
{
memcpy(Font,font,sizeof(SXFont));
//Font->SetHandleFont(font->GetHandleFont());
SendMessage(m_hWindow,WM_SETFONT,WPARAM(font->GetHandleFont()),0);
}
}*/

HFONT CGUIComponent::getParentHFont()
{
	return (HFONT)SendMessage(getParent(), WM_GETFONT, 0, 0);
}

bool CGUIComponent::hasParentFont()
{
	HFONT ChildHFont = getHFont();
	HFONT ParentHFont = getParentHFont();
	LOGFONT ChildLF, ParentLF;
	ZeroMemory(&ChildLF, sizeof(&ChildLF));
	GetObject(ChildHFont, sizeof(LOGFONT), &ChildLF);
	ZeroMemory(&ParentLF, sizeof(&ParentLF));
	GetObject(ParentHFont, sizeof(LOGFONT), &ParentLF);
	if (ChildLF.lfCharSet != ParentLF.lfCharSet)
		return false;
	if (ChildLF.lfClipPrecision != ParentLF.lfClipPrecision)
		return false;
	if (ChildLF.lfEscapement != ParentLF.lfEscapement)
		return false;
	if (ChildLF.lfHeight != ParentLF.lfHeight)
		return false;
	if (ChildLF.lfItalic != ParentLF.lfItalic)
		return false;
	if (ChildLF.lfOrientation != ParentLF.lfOrientation)
		return false;
	if (ChildLF.lfOutPrecision != ParentLF.lfOutPrecision)
		return false;
	if (ChildLF.lfPitchAndFamily != ParentLF.lfPitchAndFamily)
		return false;
	if (ChildLF.lfQuality != ParentLF.lfQuality)
		return false;
	if (ChildLF.lfStrikeOut != ParentLF.lfStrikeOut)
		return false;
	if (ChildLF.lfUnderline != ParentLF.lfUnderline)
		return false;
	if (ChildLF.lfWeight != ParentLF.lfWeight)
		return false;
	if (ChildLF.lfWidth != ParentLF.lfWidth)
		return false;
	if (strcmp(ChildLF.lfFaceName, ParentLF.lfFaceName) != 0)
		return false;
	return true;
}


bool CGUIComponent::modifyStyle(long style_add, long style_del)
{

	if (style_add)
	{
		if (!SetWindowLong(getHWND(), GWL_STYLE, GetWindowLong(getHWND(), GWL_STYLE) | style_add))
			return false;
	}

	if (style_del)
	{
		if (!SetWindowLong(getHWND(), GWL_STYLE, GetWindowLong(getHWND(), GWL_STYLE)&~style_del))
			return false;
	}
	return true;
}

bool CGUIComponent::modifyExStyle(long style_add, long style_del)
{

	if (style_add)
	{
		if (!SetWindowLong(getHWND(), GWL_EXSTYLE, GetWindowLong(getHWND(), GWL_EXSTYLE) | style_add))
			return false;
	}

	if (style_del)
	{
		if (!SetWindowLong(getHWND(), GWL_EXSTYLE, GetWindowLong(getHWND(), GWL_EXSTYLE)&~style_del))
			return false;
	}
	return true;
}

void CGUIComponent::setShowHint(bool bf)
{
	if (m_pHint != 0)
		m_pHint->setVisible(bf);
	else
	{
		m_pHint = new CGUIHint(getHWND());
		m_pHint->setVisible(bf);
	}
}

bool CGUIComponent::getShowHint()
{
	if (m_pHint != 0)
		return m_pHint->getVisible();
	return false;
}

void CGUIComponent::setHintText(const char* text)
{
	if (m_pHint == 0)
		m_pHint = new CGUIHint(getHWND());
	m_pHint->setText(text);
}

const char* CGUIComponent::getHintText()
{
	if (m_pHint != 0)
		return m_pHint->getText();
	return 0;
}

void CGUIComponent::updateSize()
{
	char classname[256];
	GetClassName(getHWND(), classname, 256);
	if (strcmp(classname, WC_LISTVIEW) == 0)
		int qwerty = 0;
	RECT rect;
	RECT win_screen_rect;
	BOOL bf = GetWindowRect(getHWND(), &rect);

	GetWindowRect(getHWND(), &win_screen_rect);
	//MapWindowPoints(m_hParent, HWND_DESKTOP, (LPPOINT)&win_screen_rect, 2);

	RECT NewParentRect;
	GetWindowRect(getParent(), &NewParentRect);

	//if(NewParentRect.top != m_rcParentScreen.top)
	//MessageBox(0,ToPointChar(NewParentRect.top),ToPointChar(m_rcParentScreen.top),0);

	//трансляция координат в пространство родителя
	//MapWindowPoints(NULL, m_hParent, (LPPOINT)&rect, 2);

	if (abs(m_rcOffsetParent.left) > 30000 || abs(m_rcOffsetParent.right) > 30000 || abs(m_rcOffsetParent.top) > 30000 || abs(m_rcOffsetParent.bottom) > 30000)
		return;

	//смещение по всем направлениям
	RECT offset;
	offset.left = m_rcOffsetParent.left ? m_rcOffsetParent.left : NewParentRect.left - m_rcParentScreen.left;
	offset.right = m_rcOffsetParent.right ? m_rcOffsetParent.right : NewParentRect.right - m_rcParentScreen.right;
	offset.top = m_rcOffsetParent.top ? m_rcOffsetParent.top : NewParentRect.top - m_rcParentScreen.top;
	offset.bottom = m_rcOffsetParent.bottom ? m_rcOffsetParent.bottom : NewParentRect.bottom - m_rcParentScreen.bottom;

	//MessageBox(0,ToPointChar(offset.top),ToPointChar(m_rcParentScreen.top),0);


	if (m_aFollowParentSide[SXGUI_SIDE_LEFT]) //rect.left
	{
		if (m_aFollowParentSide[SXGUI_SIDE_RIGHT])
			rect.right -= offset.left;
		/*else
		rect.right += offset.left;*/
	}
	else
	{
		rect.left = rect.left - offset.left;
		rect.right -= offset.left;
	}

	if (m_aFollowParentSide[SXGUI_SIDE_TOP]) //rect.top
	{
		//MessageBox(0,ToPointChar(offset.top),ToPointChar(rect.top),0);
		//rect.top = rect.top + offset.top;
		if (m_aFollowParentSide[SXGUI_SIDE_BOTTOM])
			rect.bottom -= offset.top;
	}
	else
	{
		rect.top = rect.top - offset.top;
		rect.bottom -= offset.top;
	}

	if (m_aFollowParentSide[SXGUI_SIDE_RIGHT]) //rect.right
	{
		rect.right = rect.right + offset.right;
		if (!m_aFollowParentSide[SXGUI_SIDE_LEFT]) //rect.left
			rect.left += offset.right;
	}

	if (m_aFollowParentSide[SXGUI_SIDE_BOTTOM]) //rect.bottom
	{

		rect.bottom = rect.bottom + offset.bottom;
		if (!m_aFollowParentSide[SXGUI_SIDE_TOP]) //rect.top
			rect.top += offset.bottom;
	}

	MapWindowPoints(NULL, getParent(), (LPPOINT)&rect, 2);


	//MapWindowPoints(m_hParent, HWND_DESKTOP, (LPPOINT)&m_rcWinScreen, 2);
	//MessageBox(0,ToPointChar(rect.bottom - rect.top),ToPointChar(rect.bottom - rect.top),0);

	bf = MoveWindow(getHWND(), rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, true);
	//SetWinRect(&rect,true);
	GetWindowRect(getParent(), &m_rcParentScreen);
	GetWindowRect(getHWND(), &m_rcWinScreen);

	m_rcOffsetParent.top = m_rcOffsetParent.bottom = m_rcOffsetParent.left = m_rcOffsetParent.right = 0;

	DWORD err = GetLastError();
	int qweerty = 0;
}

void CGUIComponent::updateRect()
{
	//MessageBox(0,"UpdateRect","UpdateRect",0);
	RECT tmprect;
	GetWindowRect(getParent(), &tmprect);
	m_rcOffsetParent.top = tmprect.top - m_rcParentScreen.top;
	m_rcOffsetParent.left = tmprect.left - m_rcParentScreen.left;
	m_rcOffsetParent.bottom = tmprect.bottom - m_rcParentScreen.bottom;
	m_rcOffsetParent.right = tmprect.right - m_rcParentScreen.right;
	//MessageBox(0,ToPointChar(tmprect.top - m_rcParentScreen.top),ToPointChar(0),0);
	GetWindowRect(getParent(), &m_rcParentScreen);
	//GetWindowRect(m_hWindow,&m_rcWinScreen);
}

void CGUIComponent::setColorText(DWORD color)
{
	m_dwColorText = color;
	InvalidateRect(getHWND(), 0, 1);
}

DWORD CGUIComponent::getColorText()
{
	return m_dwColorText;
}

void CGUIComponent::setTransparentTextBk(bool bf)
{
	m_isTransparentTextBk = bf;
	InvalidateRect(getHWND(), 0, 1);
}

bool CGUIComponent::getTransparentTextBk()
{
	return m_isTransparentTextBk;
}

void CGUIComponent::setColorTextBk(DWORD color)
{
	m_dwColorTextBk = color;
	InvalidateRect(getHWND(), 0, 1);
}

DWORD CGUIComponent::getColorTextBk()
{
	return m_dwColorTextBk;
}


bool CGUIComponent::setColorBrush(DWORD color)
{
	m_dwColorBrush = color;

	DeleteObject(m_hBrush);
	m_hBrush = CreateSolidBrush(m_dwColorBrush);
	InvalidateRect(getHWND(), 0, 1);
	return true;
}

DWORD CGUIComponent::getColorBrush()
{
	return m_dwColorBrush;
}

HBRUSH CGUIComponent::getBrush()
{
	return m_hBrush;
}

void CGUIComponent::addHandler(HandlerFunc Handler, UINT Msg, WPARAM wParam, bool considerWparam, LPARAM lParam, bool considerLparam, bool isMain)
{
	CHandlerData oHandler;
	oHandler.m_fnHandler = Handler;
	oHandler.m_uMsg = Msg;
	oHandler.m_wParam = wParam;
	oHandler.m_needAllowW = considerWparam;
	oHandler.m_lParam = lParam;
	oHandler.m_needAllowL = considerLparam;
	oHandler.m_isMainFunction = isMain;

	m_aHandlers.push_back(oHandler);
}

void CGUIComponent::addHandler(HandlerFunc Handler, UINT Msg)
{
	CHandlerData oHandler;
	oHandler.m_fnHandler = Handler;
	oHandler.m_uMsg = Msg;
	oHandler.m_wParam = 0;
	oHandler.m_needAllowW = false;
	oHandler.m_lParam = 0;
	oHandler.m_needAllowL = false;
	oHandler.m_isMainFunction = false;

	m_aHandlers.push_back(oHandler);
}

CHandlerData* CGUIComponent::getHandler(ID idHandler)
{
	if (idHandler >= 0 && m_aHandlers.size() > idHandler)
		return &(m_aHandlers[idHandler]);

	return 0;
}

int CGUIComponent::getCountHandlers()
{
	return m_aHandlers.size();
}

LRESULT CGUIComponent::executeHandler(ID idHandler, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (idHandler >= 0 && m_aHandlers.size() > idHandler)
		return (m_aHandlers[idHandler].m_fnHandler)(hWnd, uMsg, wParam, lParam);

	return 0;
}

void CGUIComponent::setMixSize(UINT uMinWidth, UINT uMinHeght)
{
	m_iMinSizeWidth = uMinWidth;
	m_iMinSizeHeight = uMinHeght;
}

UINT CGUIComponent::getMinWidth()
{
	return m_iMinSizeWidth;
}

UINT CGUIComponent::getMinHeight()
{
	return m_iMinSizeHeight;
}

void CGUIComponent::setStretchSides(bool canTop, bool canBottom, bool canRight, bool canLeft)
{
	m_aStrethSide[SXGUI_SIDE_TOP] = canTop;
	m_aStrethSide[SXGUI_SIDE_BOTTOM] = canBottom;
	m_aStrethSide[SXGUI_SIDE_RIGHT] = canRight;
	m_aStrethSide[SXGUI_SIDE_LEFT] = canLeft;
}

void CGUIComponent::setStretchSide(SXGUI_SIDE side, bool can)
{
	m_aStrethSide[side] = can;
}

bool CGUIComponent::getStretchSide(SXGUI_SIDE side)
{
	return m_aStrethSide[side];
}


void CGUIComponent::setFollowParentSides(bool canTop, bool canBottom, bool canRight, bool canLeft)
{
	m_aFollowParentSide[SXGUI_SIDE_TOP] = canTop;
	m_aFollowParentSide[SXGUI_SIDE_BOTTOM] = canBottom;
	m_aFollowParentSide[SXGUI_SIDE_RIGHT] = canRight;
	m_aFollowParentSide[SXGUI_SIDE_LEFT] = canLeft;
}

void CGUIComponent::setFollowParentSide(SXGUI_SIDE side, bool can)
{
	m_aFollowParentSide[side] = can;
}

bool CGUIComponent::getFollowParentSide(SXGUI_SIDE side)
{
	return m_aFollowParentSide[side];
}

//##########################################################################

CGUITextual::CGUITextual()
{

}

CGUITextual::~CGUITextual()
{

}

bool CGUITextual::setText(const char* text)
{
	return gui_func::text::SetText(this, text);
}

/*const char* CGUITextual::GetText()
{
return gui_func::text::GetTextOut(this);
}*/

void CGUITextual::getText(char* buf, int size)
{
	gui_func::text::GetText(this, buf, size);
}

int CGUITextual::getTextLen()
{
	return gui_func::text::GetTextLen(this);
}

//##########################################################################

BOOL IsEdit(HWND hWnd)
{
	if (hWnd == NULL)
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
		if (msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN)
		{
			Component->setFocus();
		}

		if (msg == WM_KEYDOWN)
		{
			if (wParam == 'A' && (GetKeyState(VK_CONTROL) & 0x80))
			{
				// User pressed Ctrl-A.  Let's select-all
				if (IsEdit(hwnd))
				{
					SendMessage(hwnd, EM_SETSEL, 0, -1);
					return(1);
				}
			}
		}
		HandlerFunc MainFunction = 0;								//главная функция-обработчик, ее значение и будет возвращаться
		Array<HandlerFunc> SecondFunction;	//массив дополнительных обработчиков на сообщения, их значения не должны смысла
		//int CountSecond = 0;	//количество дополнительных функций-обработчиков
		//инициализируем вышеописанные данные
		for (int i = 0; i<Component->getCountHandlers(); i++)
		{
			CHandlerData *pHandler = Component->getHandler(i);
			if (pHandler->m_uMsg == msg)
			{
				if (
					(!(pHandler->m_needAllowW) || (pHandler->m_needAllowW && pHandler->m_wParam == wParam)) &&
					(!(pHandler->m_needAllowL) || (pHandler->m_needAllowL && pHandler->m_lParam == lParam))
					)
				{
					if (pHandler->m_isMainFunction)
						MainFunction = pHandler->m_fnHandler;
					else
						SecondFunction.push_back(pHandler->m_fnHandler);
				}
			}
		}

		//вызываем все дополнительные функции обработчики
		for (int i = 0, il = SecondFunction.size(); i < il; ++i)
		{
			SecondFunction[i](hwnd, msg, wParam, lParam);
		}

		//если была найдена главная функция то вызываем ее и возвраащем ее значение
		if (MainFunction)
			return MainFunction(hwnd, msg, wParam, lParam);

		return CallWindowProc(Component->getPrevWndProc(), hwnd, msg, wParam, lParam);
	}

	return CallWindowProc(Component->getPrevWndProc(), hwnd, msg, wParam, lParam);
}

//##########################################################################

/*bool gui_func::text::ScroolBars(ISXGUIControl *Control, bool h, bool v)
{
	return ShowScrollBar(Control->getHWND(), SB_HORZ, h ? 1 : 0) && ShowScrollBar(Control->getHWND(), SB_VERT, v ? 1 : 0);
}

bool gui_func::text::ScrollBarV(ISXGUIControl *Control)
{
	long style = GetWindowLong(Control->getHWND(), GWL_STYLE);
	if (style & WS_VSCROLL)
		return true;
	return false;
}

bool gui_func::text::ScrollBarH(ISXGUIControl *Control)
{
	long style = GetWindowLong(Control->getHWND(), GWL_STYLE);
	if (style & WS_HSCROLL)
		return true;
	return false;
}

bool gui_func::text::ScrollLine(ISXGUIControl *Control, int scroll, int dir, int count)
{
	long _scroll = scroll == SXGUI_SCROLL_TYPE_VERT ? WM_VSCROLL : WM_HSCROLL;
	long _dir = 0;
	if (_scroll == WM_VSCROLL)
		_dir = dir == SXGUI_SCROLL_DIR_DOWN ? SB_LINEDOWN : SB_LINEUP ;
	else
		_dir = dir == SXGUI_SCROLL_DIR_RIGTH ? SB_LINERIGHT : SB_LINELEFT ;

	bool bf = true;
	for (int i = 0; i<count; i++)
	{
		bf = SendMessage(Control->getHWND(), _scroll, _dir, 0);
	}

	return bf;//SendMessage(Control->getHWND(),_scroll,_dir,LPARAM(count)) == 0 ? true : false;
}
*/
bool gui_func::text::SetText(ISXGUIControl*const Control, const char* text)
{
	if (!SetWindowText(Control->getHWND(), text))
		return false;
	else
		return true;
}

void gui_func::text::GetText(ISXGUIControl *Control, char* buf, int count)
{
	GetWindowText(Control->getHWND(), buf, count);
}

int gui_func::text::GetTextLen(ISXGUIControl *Control)
{
	return GetWindowTextLength(Control->getHWND());
}

//##########################################################################

BOOL CALLBACK gui_func::enum_child::EnumChildProcUpdateImgButton(HWND hwnd, LPARAM lParam)
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

BOOL CALLBACK gui_func::enum_child::EnumChildProcUpdateSize(HWND hwnd, LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd, GWL_USERDATA);
	if (Component)
		Component->updateSize();

	return TRUE;
}

BOOL CALLBACK gui_func::enum_child::EnumChildProcUpdateRect(HWND hwnd, LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd, GWL_USERDATA);
	if (Component)
		Component->updateRect();

	return TRUE;
}

BOOL CALLBACK gui_func::enum_child::EnumChildProcMouseMove(HWND hwnd, LPARAM lParam)
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
			SendMessage(Component->getHWND(), WM_NCMOUSEMOVE, lParam, 0);
		}
	}

	return TRUE;
}

//##########################################################################

void gui_func::base_handlers::InitHandlerMsg(ISXGUIComponent* Component)
{
	Component->addHandler(gui_func::base_handlers::CtlColorChange, WM_CTLCOLORSTATIC, 0, 0, 0, 0, true);
	Component->addHandler(gui_func::base_handlers::CtlColorChange, WM_CTLCOLOREDIT, 0, 0, 0, 0, true);
	Component->addHandler(gui_func::base_handlers::CtlColorChange, WM_CTLCOLORBTN, 0, 0, 0, 0, true);
	Component->addHandler(gui_func::base_handlers::CtlColorChange, WM_CTLCOLORLISTBOX, 0, 0, 0, 0, true);

	Component->addHandler(gui_func::base_handlers::SizeChange, WM_SIZE, 0, 0, 0, 0, true);
	Component->addHandler(gui_func::base_handlers::SizingChange, WM_SIZING, 0, 0, 0, 0, true);
	Component->addHandler(gui_func::base_handlers::WinDestroy, WM_DESTROY, 0, 0, 0, 0, true);

	Component->addHandler(gui_func::base_handlers::MoveChange, WM_MOVE, 0, 0, 0, 0, true);
	Component->addHandler(gui_func::base_handlers::MouseMoveChange, WM_MOUSEMOVE, 0, 0, 0, 0, true);
	Component->addHandler(gui_func::base_handlers::MaximuzeWinChange, WM_SYSCOMMAND, SC_MAXIMIZE, true, 0, 0, true);
	Component->addHandler(gui_func::base_handlers::SetCursorChange, WM_SETCURSOR, 0, 0, 0, 0, true);
}

LRESULT gui_func::base_handlers::CtlColorChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong((HWND)lParam, GWL_USERDATA);

	if (Component)
	{
		if (Component->getTransparentTextBk())
			SetBkMode((HDC)wParam, TRANSPARENT);
		else
			SetBkColor((HDC)wParam, Component->getColorTextBk());
		SetTextColor((HDC)wParam, Component->getColorText());
		return (long)Component->getBrush();
	}
	return((LRESULT)INVALID_HANDLE_VALUE);
}

LRESULT gui_func::base_handlers::SizeChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd, (WNDENUMPROC)&gui_func::enum_child::EnumChildProcUpdateSize, lParam);
	return TRUE;
}

LRESULT gui_func::base_handlers::SizingChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent *Component = (ISXGUIComponent *)GetWindowLong(hwnd, GWL_USERDATA);

	if (!Component)
		return TRUE;

	RECT rc;
	GetWindowRect(hwnd, &rc);
	POINT p;
	GetCursorPos(&p);

	RECT lpRect = *((LPRECT)lParam);

	if (Component)
	{

		if (!Component->getStretchSide(SXGUI_SIDE_TOP))
			lpRect.top = rc.top;

		if (!Component->getStretchSide(SXGUI_SIDE_LEFT))
			lpRect.left = rc.left;

		if (!Component->getStretchSide(SXGUI_SIDE_RIGHT))
			lpRect.right = rc.right;

		if (!Component->getStretchSide(SXGUI_SIDE_BOTTOM))
			lpRect.bottom = rc.bottom;

		if (Component->getMinWidth() != 0 && Component->getMinHeight() != 0)
		{
			if (lpRect.right - lpRect.left < Component->getMinWidth() && Component->getStretchSide(SXGUI_SIDE_RIGHT))
			{
				lpRect.right += Component->getMinWidth() - (lpRect.right - lpRect.left);
			}

			if (lpRect.bottom - lpRect.top < Component->getMinHeight() && Component->getStretchSide(SXGUI_SIDE_BOTTOM))
			{
				lpRect.bottom += Component->getMinHeight() - (lpRect.bottom - lpRect.top);
			}
		}
	}

	*((LPRECT)lParam) = lpRect;

	return TRUE;
}

LRESULT gui_func::base_handlers::MoveChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd, (WNDENUMPROC)&gui_func::enum_child::EnumChildProcUpdateRect, lParam);
	return TRUE;
}

LRESULT gui_func::base_handlers::MouseMoveChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd, (WNDENUMPROC)&gui_func::enum_child::EnumChildProcMouseMove, lParam);
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT gui_func::base_handlers::SetCursorChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd, (WNDENUMPROC)&gui_func::enum_child::EnumChildProcMouseMove, lParam);
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT gui_func::base_handlers::MaximuzeWinChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EnumChildWindows(hwnd, (WNDENUMPROC)&gui_func::enum_child::EnumChildProcUpdateSize, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT gui_func::base_handlers::WinDestroy(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//##########################################################################

void gui_func::dialogs::SelectFileStd(int type, char* path, char* name, const char* stdpath, const char* filter)
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

	char bf[256];
	GetCurrentDirectory(256, bf);
	BOOL Result = FALSE;
	if (type == SXGUI_DIALOG_FILE_OPEN)
		Result = GetOpenFileName(&ofn);
	else if (type == SXGUI_DIALOG_FILE_SAVE)
		Result = GetSaveFileName(&ofn);

	SetCurrentDirectory(bf);

	if (Result)
	{
		if (path)
			strcpy(path, tpath);

		if (name)
			strcpy(name, tname);
	}
}

SX_LIB_API bool gui_func::dialogs::SelectDirOwn(char *szOutName, char *szOutPath, const char *szStartPath, const char *szDialogName, bool canExplore, bool canCreateNew, const char *szDownPath, HandlerDialogOwndGetPreview lpfnHandlerPreview)
{
	return DialogOwnSelectDirRun(szOutName, szOutPath, szStartPath, szDialogName, canExplore, canCreateNew, szDownPath, lpfnHandlerPreview);
}

SX_LIB_API bool gui_func::dialogs::SelectFileOwn(char *szOutName, char *szOutPath, const char *szStartPath, const char *szFilterExt, const char *szDialogName, bool canExplore, const char *szDownPath, HWND hWndLock, HandlerDialogOwndGetPreview2 lpfnHandlerPreview, HandlerDialogOwndGetInfo lpfnHandlerInfo)
{
	return DialogOwnSelectFileRun(szOutName, szOutPath, szStartPath, szFilterExt, szDialogName, canExplore, szDownPath, hWndLock, lpfnHandlerPreview, lpfnHandlerInfo);
}