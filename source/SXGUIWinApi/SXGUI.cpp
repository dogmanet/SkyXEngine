
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#include <SXGUIWinApi\SXGUI.h>
#include <SXGUIWinApi\SXGUI_base.cpp>
#include <SXGUIWinApi\SXGUI_base_wnd.cpp>
#include <SXGUIWinApi\SXGUI_static.cpp>
#include <SXGUIWinApi\SXGUI_button.cpp>
#include <SXGUIWinApi\SXGUI_img_button.cpp>
#include <SXGUIWinApi\SXGUI_edit.cpp>
#include <SXGUIWinApi\SXGUI_up_down.cpp>
#include <SXGUIWinApi\SXGUI_memo.cpp>
#include <SXGUIWinApi\SXGUI_combo_box.cpp>
#include <SXGUIWinApi\SXGUI_list_box.cpp>
#include <SXGUIWinApi\SXGUI_group_box.cpp>
#include <SXGUIWinApi\SXGUI_progress_bar.cpp>
#include <SXGUIWinApi\SXGUI_radio_button.cpp>
#include <SXGUIWinApi\SXGUI_check_box.cpp>
#include <SXGUIWinApi\SXGUI_track_bar.cpp>
#include <SXGUIWinApi\SXGUI_status_bar.cpp>
#include <SXGUIWinApi\SXGUI_hint.cpp>
#include <SXGUIWinApi\SXGUI_menu.cpp>
#include <SXGUIWinApi\SXGUI_toolbar.cpp>

ISXGUIBaseWnd* SXGUICrBaseWnd(
	const char* class_name, const char* caption, const char* menu,
	WORD id, WORD x, WORD y, WORD width, WORD heigth,
	HICON icon, HCURSOR cursor, HBRUSH brush,
	DWORD exstyle, DWORD wndstyle, DWORD style,
	HWND parent, WNDPROC handler
	)
{
	return new SXGUIBaseWnd(class_name, caption, menu,
		id, x, y, width, heigth,
		icon, cursor, brush,
		exstyle, wndstyle, style,
		parent, (handler == 0 ? WndProcAllDefault : handler));
}


ISXGUIStatic* SXGUICrStaticEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIStatic(caption, x, y, width, heigth, parent, handler, id);
}

ISXGUIStatic* SXGUICrStatic(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIStatic(caption, x, y, width, heigth, parent, handler, id);
}

ISXGUIStatic* SXGUICrStaticLine(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool vertical)
{
	return new SXGUIStatic(x, y, width, heigth, parent, handler, id, vertical);
}


ISXGUIButton* SXGUICrButtonEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIButton(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}
ISXGUIButton* SXGUICrButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, WORD image, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIButton(caption, x, y, width, heigth, image, parent, handler, id);
}


ISXGUIButtonImg* SXGUICrButtonImgEx(const char* path, WORD x, WORD y, WORD width, WORD heigth, DWORD alpha_color, DWORD bk_color, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIButtonImg(path, x, y, width, heigth, alpha_color, bk_color, parent, handler, id);
}

ISXGUIButtonImg* SXGUICrButtonImg(WORD button, WORD x, WORD y, WORD width, WORD heigth, DWORD alpha_color, DWORD bk_color, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIButtonImg(button, x, y, width, heigth, alpha_color, bk_color, parent, handler, id);
}

ISXGUIEdit* SXGUICrEditEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIEdit(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}

ISXGUIEdit* SXGUICrEdit(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIEdit(caption, x, y, width, heigth, parent, handler, id);
}


ISXGUIUpDown* SXGUICrUpDownEx(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id, HWND buddy)
{
	return new SXGUIUpDown(x, y, width, heigth, exstyle, style, parent, handler, id, buddy);
}

ISXGUIUpDown* SXGUICrUpDown(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, HWND buddy, WORD left_right)
{
	return new SXGUIUpDown(x, y, width, heigth, parent, handler, id, buddy, left_right);
}


ISXGUIMemo* SXGUICrMemoEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIMemo(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}

ISXGUIMemo* SXGUICrMemo(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIMemo(caption, x, y, width, heigth, parent, handler, id);
}


ISXGUIComboBox* SXGUICrComboBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIComboBox(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}

//Їыруш: (parent != 0 ? WS_CHILD : 0) | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS
ISXGUIComboBox* SXGUICrComboBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIComboBox(caption, x, y, width, heigth, parent, handler, id);
}


ISXGUIListBox* SXGUICrListBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIListBox(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}

ISXGUIListBox* SXGUICrListBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool miltiple_sel)
{
	return new SXGUIListBox(caption, x, y, width, heigth, parent, handler, id, miltiple_sel);
}


ISXGUIHint* SXGUICrHint(HWND parent)
{
	return new SXGUIHint(parent);
}


ISXGUIGroupBox* SXGUICrGroupBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIGroupBox(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}

ISXGUIGroupBox* SXGUICrGroupBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIGroupBox(caption, x, y, width, heigth, parent, handler, id);
}


ISXGUIProgressBar* SXGUICrProgressBarEx(WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIProgressBar(x, y, width, heigth, exstyle, style, parent, handler, id);
}

ISXGUIProgressBar* SXGUICrProgressBar(WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool vertical, bool smooth)
{
	return new SXGUIProgressBar(x, y, width, heigth, parent, handler, id, vertical, smooth);
}


ISXGUIRadioButton* SXGUICrRadioButtonEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIRadioButton(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}

ISXGUIRadioButton* SXGUICrRadioButton(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIRadioButton(caption, x, y, width, heigth, parent, handler, id);
}


ISXGUICheckBox* SXGUICrCheckBoxEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUICheckBox(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}

ISXGUICheckBox* SXGUICrCheckBox(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id, bool cb_3_state)
{
	return new SXGUICheckBox(caption, x, y, width, heigth, parent, handler, id, cb_3_state);
}


ISXGUITrackBar* SXGUICrTrackBarEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUITrackBar(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}

ISXGUITrackBar* SXGUICrTrackBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUITrackBar(caption, x, y, width, heigth, parent, handler, id);
}


ISXGUIStatusBar* SXGUICrStatusBarEx(const char* caption, WORD x, WORD y, WORD width, WORD heigth, DWORD exstyle, DWORD style, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIStatusBar(caption, x, y, width, heigth, exstyle, style, parent, handler, id);
}

ISXGUIStatusBar* SXGUICrStatusBar(const char* caption, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIStatusBar(caption, parent, handler, id);
}


ISXGUIMenu* SXGUICrMenu()
{
	return new SXGUIMenu();
}

ISXGUIMenu* SXGUICrMenuEx(WORD menu)
{
	return new SXGUIMenu(menu);
}


ISXGUIPopupMenu* SXGUICrPopupMenuEx(WORD menu)
{
	return new SXGUIPopupMenu(menu);
}

ISXGUIPopupMenu* SXGUICrPopupMenu()
{
	return new SXGUIPopupMenu();
}


ISXGUIToolBar* SXGUICrToolBar(const char* caption, WORD x, WORD y, WORD width, WORD heigth, WORD width_element, WORD heigth_element, HWND parent, WNDPROC handler, DWORD id)
{
	return new SXGUIToolBar(caption, x, y, width, heigth, width_element, heigth_element, parent, handler, id);
}
