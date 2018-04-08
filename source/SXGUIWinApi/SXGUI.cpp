
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "SXGUI.h"
#include "base.h"
#include "base_wnd.h"
#include "static.h"
#include "button.h"
#include "img_button.h"
#include "edit.h"
#include "updown.h"
#include "memo.h"
#include "combobox.h"
#include "listbox.h"
#include "listview.h"
#include "groupbox.h"
#include "progressbar.h"
#include "radiobutton.h"
#include "checkbox.h"
#include "trackbar.h"
#include "statusbar.h"
#include "hint.h"
#include "menu.h"
#include "toolbar.h"

//##########################################################################

SX_LIB_API void SXGUIinit()
{
	WNDCLASS wcButtonImg;

	wcButtonImg.style = CS_HREDRAW | CS_VREDRAW;
	wcButtonImg.lpfnWndProc = DefWindowProc;
	wcButtonImg.cbClsExtra = 0;
	wcButtonImg.cbWndExtra = 0;
	wcButtonImg.hInstance = GetModuleHandle(0);
	wcButtonImg.hIcon = 0;
	wcButtonImg.hCursor = 0;
	wcButtonImg.hbrBackground = 0;
	wcButtonImg.lpszMenuName = 0;
	wcButtonImg.lpszClassName = SXGUI_DEF_BUTTONIMG;

	RegisterClass(&wcButtonImg);


	WNDCLASS wcGroupBox;

	wcGroupBox.style = CS_HREDRAW | CS_VREDRAW;
	wcGroupBox.lpfnWndProc = DefWindowProc;
	wcGroupBox.cbClsExtra = 0;
	wcGroupBox.cbWndExtra = 0;
	wcGroupBox.hInstance = GetModuleHandle(0);
	wcGroupBox.hIcon = 0;
	wcGroupBox.hCursor = 0;
	wcGroupBox.hbrBackground = 0;
	wcGroupBox.lpszMenuName = 0;
	wcGroupBox.lpszClassName = SXGUI_DEF_GROUPBOX;

	RegisterClass(&wcGroupBox);


	INITCOMMONCONTROLSEX icex;

	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);
}

//##########################################################################

int g_iCountCreateWnd = 0;

SX_LIB_API ISXGUIBaseWnd* SXGUICrBaseWnd(
	const char *szCaption,
	int iPosX, int iPosY, int iWidth, int iHeight,
	DWORD dwExstyle, DWORD dwStyle,
	HWND hParent, WNDPROC lpfnHandler,
	ID idObj
	)
{
	char szClassName[64];
	sprintf(szClassName, "SXGUICrBaseWnd_$d", g_iCountCreateWnd);
	++g_iCountCreateWnd;

	return new CGUIBaseWnd(szClassName, szCaption,
		iPosX, iPosY, iWidth, iHeight,
		0, 0, 0,
		dwExstyle, CS_HREDRAW | CS_VREDRAW, dwStyle,
		hParent, (lpfnHandler == 0 ? WndProcAllDefault : lpfnHandler), idObj);
}

SX_LIB_API ISXGUIBaseWnd* SXGUICrBaseWndEx(
	const char* class_name, const char *szCaption, 
	int iPosX, int iPosY, int iWidth, int iHeight,
	HICON icon, HCURSOR cursor, HBRUSH brush,
	DWORD dwExstyle, DWORD wndstyle, DWORD dwStyle,
	HWND hParent, WNDPROC lpfnHandler,
	ID idObj
	)
{
	return new CGUIBaseWnd(class_name, szCaption, 
		iPosX, iPosY, iWidth, iHeight,
		icon, cursor, brush,
		dwExstyle, wndstyle, dwStyle,
		hParent, (lpfnHandler == 0 ? WndProcAllDefault : lpfnHandler), idObj);
}

//##########################################################################

SX_LIB_API ISXGUIStatic* SXGUICrStaticEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIStatic(szCaption, iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIStatic* SXGUICrStatic(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIStatic(szCaption, iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIStatic* SXGUICrStaticLine(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj, bool vertical)
{
	return new CGUIStatic(iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj, vertical);
}

//##########################################################################

SX_LIB_API ISXGUIButton* SXGUICrButtonEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIButton(szCaption, iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIButton* SXGUICrButton(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, SXGUI_BUTTON_IMAGE type_image, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIButton(szCaption, iPosX, iPosY, iWidth, iHeight, type_image, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUIButtonImg* SXGUICrButtonImgLoad(const char* path, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwAlphaColor, DWORD dwBkColor, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIButtonImg(path, iPosX, iPosY, iWidth, iHeight, dwAlphaColor, dwBkColor, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIButtonImg* SXGUICrButtonImgRes(UINT uButton, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwAlphaColor, DWORD dwBkColor, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIButtonImg(uButton, iPosX, iPosY, iWidth, iHeight, dwAlphaColor, dwBkColor, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUIEdit* SXGUICrEditEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIEdit(szCaption, iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIEdit* SXGUICrEdit(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIEdit(szCaption, iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUIUpDown* SXGUICrUpDownEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj, HWND buddy)
{
	return new CGUIUpDown(iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj, buddy);
}

SX_LIB_API ISXGUIUpDown* SXGUICrUpDown(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj, HWND buddy, bool align_left)
{
	return new CGUIUpDown(iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj, buddy, align_left);
}

//##########################################################################

SX_LIB_API ISXGUIMemo* SXGUICrMemoEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIMemo(szCaption, iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIMemo* SXGUICrMemo(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIMemo(szCaption, iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUIComboBox* SXGUICrComboBoxEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIComboBox(iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

//Їыруш: (hParent != 0 ? WS_CHILD : 0) | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS
SX_LIB_API ISXGUIComboBox* SXGUICrComboBox(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIComboBox(iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUIListBox* SXGUICrListBoxEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIListBox(iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIListBox* SXGUICrListBox(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj, bool miltiple_sel)
{
	return new CGUIListBox(iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj, miltiple_sel);
}

//##########################################################################

SX_LIB_API ISXGUIListView* SXGUICrListViewEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIListView(iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIListView* SXGUICrListView(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIListView(iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUIHint* SXGUICrHint(HWND hParent)
{
	return new CGUIHint(hParent);
}

SX_LIB_API ISXGUIHint* SXGUICrHintEx(HWND hParent, const char *szText, UINT uInit, UINT uAutopop)
{
	CGUIHint *pHint = new CGUIHint(hParent);
	pHint->setText(szText);
	pHint->setDelayTime(uInit, uAutopop);

	return pHint;
}

//##########################################################################

SX_LIB_API ISXGUIGroupBox* SXGUICrGroupBoxEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIGroupBox(szCaption, iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIGroupBox* SXGUICrGroupBox(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIGroupBox(szCaption, iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUIProgressBar* SXGUICrProgressBarEx(int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIProgressBar(iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIProgressBar* SXGUICrProgressBar(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj, bool vertical, bool smooth)
{
	return new CGUIProgressBar(iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj, vertical, smooth);
}

//##########################################################################

SX_LIB_API ISXGUIRadioButton* SXGUICrRadioButtonEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIRadioButton(szCaption, iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIRadioButton* SXGUICrRadioButton(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIRadioButton(szCaption, iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUICheckBox* SXGUICrCheckBoxEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUICheckBox(szCaption, iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUICheckBox* SXGUICrCheckBox(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj, bool cb_3_state)
{
	return new CGUICheckBox(szCaption, iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj, cb_3_state);
}

//##########################################################################

SX_LIB_API ISXGUITrackBar* SXGUICrTrackBarEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUITrackBar(szCaption, iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUITrackBar* SXGUICrTrackBar(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUITrackBar(szCaption, iPosX, iPosY, iWidth, iHeight, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUIStatusBar* SXGUICrStatusBarEx(const char *szCaption, int iPosX, int iPosY, int iWidth, int iHeight, DWORD dwExstyle, DWORD dwStyle, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIStatusBar(szCaption, iPosX, iPosY, iWidth, iHeight, dwExstyle, dwStyle, hParent, lpfnHandler, idObj);
}

SX_LIB_API ISXGUIStatusBar* SXGUICrStatusBar(const char *szCaption, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIStatusBar(szCaption, hParent, lpfnHandler, idObj);
}

//##########################################################################

SX_LIB_API ISXGUIMenuWindow* SXGUICrMenuWindow()
{
	return new CGUIMenuWindow();
}

SX_LIB_API ISXGUIMenuWindow* SXGUICrMenuWindowEx(UINT uResMenu)
{
	return new CGUIMenuWindow(uResMenu);
}

//##########################################################################

SX_LIB_API ISXGUIPopupMenu* SXGUICrPopupMenuEx(UINT uResMenu)
{
	return new CGUIPopupMenu(uResMenu);
}

SX_LIB_API ISXGUIPopupMenu* SXGUICrPopupMenu()
{
	return new CGUIPopupMenu();
}

//##########################################################################

SX_LIB_API ISXGUIToolBar* SXGUICrToolBar(int iPosX, int iPosY, int iWidth, int iHeight, HWND hParent, WNDPROC lpfnHandler, ID idObj)
{
	return new CGUIToolBar(0, iPosX, iPosY, iWidth, iHeight, 0, 0, hParent, lpfnHandler, idObj);
}

//##########################################################################

bool gui_func::scrollbar::InitScroolBars(ISXGUIControl *pControl, bool h, bool v)
{
	return ShowScrollBar(pControl->getHWND(), SB_HORZ, h ? 1 : 0) && ShowScrollBar(pControl->getHWND(), SB_VERT, v ? 1 : 0);
}

bool gui_func::scrollbar::existsScrollBar(ISXGUIControl *pControl, SXGUI_SCROLL_TYPE scroll_type)
{
	long style = GetWindowLong(pControl->getHWND(), GWL_STYLE);
	if (scroll_type == SXGUI_SCROLL_TYPE_V && style & WS_VSCROLL)
		return true;
	else if (scroll_type == SXGUI_SCROLL_TYPE_H && style & WS_HSCROLL)
		return true;
	return false;
}

bool gui_func::scrollbar::ScrollLine(ISXGUIControl *pControl, SXGUI_SCROLL_TYPE scroll_type, SXGUI_SCROLL_DIR dir, int iCountString)
{
	long _scroll = scroll_type == SXGUI_SCROLL_TYPE_V ? WM_VSCROLL : WM_HSCROLL;
	long _dir = 0;
	if (_scroll == WM_VSCROLL)
		_dir = dir == SXGUI_SCROLL_DIR_DOWN ? SB_LINEDOWN : SB_LINEUP /*SB_BOTTOM :SB_TOP*/;
	else
		_dir = dir == SXGUI_SCROLL_DIR_RIGTH ? SB_LINERIGHT : SB_LINELEFT /*SB_RIGHT :SB_LEFT*/;

	bool bf = true;
	for (int i = 0; i<iCountString; i++)
	{
		bf = SendMessage(pControl->getHWND(), _scroll, _dir, 0);
	}

	return bf;//SendMessage(Control->GetHWND(),_scroll,_dir,LPARAM(count)) == 0 ? true : false;
}