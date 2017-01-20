#include "Editor.h"

Editor::Editor()
{
	InitUI();
}

Editor::~Editor()
{
	DestroyUI();
}

LRESULT Editor::MenuCmd(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ISXGUIComponent * cmp = (ISXGUIComponent*)GetWindowLong(hwnd, GWL_USERDATA);
	Editor * edt = (Editor*)cmp->GetUserPtr();
	if(msg == WM_COMMAND)
	{
		switch(LOWORD(wParam))
		{
		case ID_FILE_OPEN:
			edt->MenuBrowse(hwnd);
			break;
		case ID_FILE_SAVE:
			edt->MenuSave();
			break;
		case ID_FILE_SAVEAS:
			edt->MenuSaveAs(hwnd);
			break;

		case ID_FILE_EXIT:
			PostQuitMessage(0);
			break;
		}
	}
	return(0);
}

void Editor::MenuBrowse(HWND hwnd)
{
	OPENFILENAME ofn;
	wchar_t szFile[260];

	if(m_bDirty && MessageBoxA(hwnd, "All unsaved changes will be lost", "Open?", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) != IDOK)
	{
		return;
	}

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ZeroMemory(szFile, sizeof(wchar_t) * 260);
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(wchar_t) * 260;
	ofn.lpstrFilter = L"Model file (*.dse)\0*.dse\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	wchar_t bf[256];
	GetCurrentDirectoryW(256, bf);

	if(GetOpenFileName(&ofn) == TRUE)
	{
		SetCurrentDirectoryW(bf);
		wprintf(L"File: %s\n", ofn.lpstrFile);
	}
}

void Editor::MenuSave()
{
	m_bDirty = false;
}
void Editor::MenuSaveAs(HWND hwnd)
{
	m_bDirty = false;
}

void Editor::InitUI()
{
	MainWindow = SXGUICrBaseWnd("MainWindow", "MainWindow", 0, 0, 256, 199, 1320, 730, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(MainWindow);
	MainWindow->AddHandler(MenuCmd, WM_COMMAND);

	MainWindow->SetUserPtr(this);

	Menu = SXGUICrMenuEx(IDR_MENU1);
	Menu->SetToWindow(MainWindow->GetHWND());

	Window1 = SXGUICrBaseWnd("Window1", "Window1", 0, 0, 279, 6, 1023, 473, 0, 0, CreateSolidBrush(RGB(200, 200, 200)), 0, CS_HREDRAW | CS_VREDRAW, WS_CHILD | WS_VISIBLE | WS_BORDER, MainWindow->GetHWND(), 0);
	Window1->GAlign = {true, true, true, true};

	m_pTM = new TabManager(MainWindow);

	AnimationsGB = SXGUICrGroupBox("Animations", 3, 0, 275, 669, MainWindow->GetHWND(), 0, 0);
	AnimationsGB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimationsGB->SetColorText(0, 0, 0);
	AnimationsGB->SetColorTextBk(255, 255, 255);
	AnimationsGB->SetTransparentTextBk(true);
	AnimationsGB->SetColorBrush(255, 255, 255);
	AnimationsGB->GAlign = {true, true, true, false};
	AnimList = SXGUICrListBox("", 2, 43, 270, 623, AnimationsGB->GetHWND(), 0, 0, false);
	AnimList->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimList->SetColorText(0, 0, 0);
	AnimList->SetColorTextBk(255, 255, 255);
	AnimList->SetTransparentTextBk(true);
	AnimList->SetColorBrush(255, 255, 255);
	AnimList->GAlign = {true, true, true, true};
	AnimFilter = SXGUICrEdit("AnimFilter", 44, 16, 228, 23, AnimationsGB->GetHWND(), 0, 0);
	AnimFilter->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	AnimFilter->SetColorText(0, 0, 0);
	AnimFilter->SetColorTextBk(255, 255, 255);
	AnimFilter->SetTransparentTextBk(true);
	AnimFilter->SetColorBrush(255, 255, 255);
	Static1 = SXGUICrStatic("Filter:", 8, 18, 32, 20, AnimationsGB->GetHWND(), 0, 0);
	Static1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	Static1->SetColorText(0, 0, 0);
	Static1->SetColorTextBk(255, 255, 255);
	Static1->SetTransparentTextBk(true);
	Static1->SetColorBrush(255, 255, 255);

	AnimList->AddItem("text hahaha");
	AnimList->AddItem("text hahahb");
	AnimList->AddItem("text hahahc");

	CurAnimName = SXGUICrStatic("Animation: idle", 1149, 649, 145, 19, MainWindow->GetHWND(), 0, 0);
	CurAnimName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	CurAnimName->SetColorText(0, 0, 0);
	CurAnimName->SetColorTextBk(255, 255, 255);
	CurAnimName->SetTransparentTextBk(true);
	CurAnimName->SetColorBrush(255, 255, 255);
	CurAnimName->GAlign = {false, false, true, true};
}
void Editor::DestroyUI()
{
	mem_delete(m_pTM);

	mem_delete(Window1);
	mem_delete(AnimList);
	mem_delete(AnimationsGB);
	mem_delete(AnimFilter);
	mem_delete(Static1);
	mem_delete(CurAnimName);
	mem_delete(MainWindow);
}
