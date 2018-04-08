
#ifndef __WINDOW_OUTPUT_H
#define __WINDOW_OUTPUT_H

#include <windows.h>
#include <SXGUIWinApi/sxgui.h>
#include "sxwincreator.h"
#include "sx_param_wnd_data.h"

namespace SXNameSapce
{
	extern ISXGUIBaseWnd* OutputGUIInFile;
	extern ISXGUIStatic* NameSpace;
	extern ISXGUIEdit* SXNameSpace;
	extern ISXGUICheckBox* Constructors;
	extern ISXGUICheckBox* Destructors;
	extern ISXGUICheckBox* Comments;
	extern ISXGUICheckBox* Inlude;
	extern ISXGUICheckBox* DetectedNameSpace;
	extern ISXGUIStatic* SaveFile;
	extern ISXGUIEdit* PathForFile;
	extern ISXGUIButton* ButtonSelectFile;
	extern ISXGUIButton* StartOutput;

	void InitAllElements();

	void DeleteAllElements();

};

LRESULT StartButClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SelectButClick(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT HideSettingsWndClose(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif