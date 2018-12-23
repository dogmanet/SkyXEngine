
#include "dialog_select_dir.h"


namespace dialog_seldir
{
	ISXGUIBaseWnd *pSelectDirectory;
	ISXGUIBaseWnd *pPreView;
	ISXGUIListBox *pListBox;
	ISXGUIStatic *pStaticCreateNew;
	ISXGUIEdit *pEditCreateNew;
	ISXGUIButton *pButtonCreateNew;
	ISXGUIButton *pButtonCancel;
	ISXGUIButton *pButtonSelect;
	ISXGUIEdit *pEditCurrentPath;
	ISXGUIStatic *pStaticCurrentPath;

	void InitAllElements();

	void InitDataListBox(const char *szPath);

	void DeleteAllElements();

	String sCurrPath;
	String sSelName;
	String sDownDir;

	HandlerDialogOwndGetPreview lpfnHandlerPreview = 0;

	bool canExplore;
};

//##########################################################################

LRESULT ListBox_LUP(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (dialog_seldir::lpfnHandlerPreview)
		InvalidateRect(dialog_seldir::pPreView->getHWND(), 0, TRUE);

	return 0;
}

LRESULT ListBox_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!dialog_seldir::canExplore)
		return 0;

	int iSel = dialog_seldir::pListBox->getSel();

	if (iSel < 0)
		return 0;

	int iNesting = FileCountNesting(dialog_seldir::sCurrPath.c_str());

	char szSel[MAX_PATH];
	dialog_seldir::pListBox->getItemText(iSel, szSel);

	if (iNesting > 0 && iSel == 0 && strcmp(szSel,"..") == 0)
	{
		String sPrevPath = FileGetPrevDir(dialog_seldir::sCurrPath.c_str());
		dialog_seldir::InitDataListBox(sPrevPath.c_str());
	}
	else
	{
		//dialog_seldir::sCurrPath = dialog_seldir::sCurrPath + "/" + szSel;
		dialog_seldir::InitDataListBox((FileAppendSlash(dialog_seldir::sCurrPath.c_str()) + szSel).c_str());
	}

	return 0;
}

LRESULT EditCurrentPath_Enter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	dialog_seldir::pEditCurrentPath->getText(szPath, 1024);

	if (!FileExistsInPath(szPath, dialog_seldir::sDownDir.c_str()))
	{
		char szMessage[1024];
		sprintf(szMessage, "Unresolved explore down path [%s]", dialog_seldir::sDownDir.c_str());
		MessageBox(hWnd, szMessage, 0, 0);
		dialog_seldir::pEditCurrentPath->setText(dialog_seldir::sCurrPath.c_str());
		return 0;
	}

	if (FileExistsDir(szPath))
		dialog_seldir::InitDataListBox(szPath);
	else
	{
		char szMessage[1024];
		sprintf(szMessage, "Directory [%s] is not exists", szPath);
		MessageBox(hWnd, szMessage, 0, 0);
	}
	return 0;
}

LRESULT EditCurrentPath_Input(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	dialog_seldir::pEditCurrentPath->getText(szPath, 1024);

	const char *szBase = FileBaseName(szPath);
	dialog_seldir::pListBox->setSel(-1);
	if (strlen(szBase) == 0)
		return 0;

	char szDir[MAX_PATH];
	char *szFound = 0;
	for (int i = 0; i < dialog_seldir::pListBox->getItemCount(); ++i)
	{
		dialog_seldir::pListBox->getItemText(i, szDir);

		if ((szFound = strstr(szDir, szBase)) && (szFound - szDir) == 0)
		{
			//gui_func::scrollbar::ScrollLine(dialog_seldir::pListBox, SXGUI_SCROLL_TYPE_V, SXGUI_SCROLL_DIR_UP, dialog_seldir::pListBox->getItemCount());
			//gui_func::scrollbar::ScrollLine(dialog_seldir::pListBox, SXGUI_SCROLL_TYPE_V, SXGUI_SCROLL_DIR_DOWN, i);

			dialog_seldir::pListBox->setSel(i);

			break;
		}
	}

	return 0;
}

LRESULT SelectDirectory_Exit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	dialog_seldir::pSelectDirectory->setVisible(false);
	return 0;
}

LRESULT ButtonCreateNew_Click(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szNewDir[1024];
	dialog_seldir::pEditCreateNew->getText(szNewDir, 1024);

	if (strlen(szNewDir) == 0)
	{
		MessageBox(hWnd, "Input name for new directory", 0, 0);
	}
	else
	{
		String sNewPath = dialog_seldir::sCurrPath + szNewDir;
		if (FileExistsDir(sNewPath.c_str()))
			MessageBox(hWnd, "This directory is exists, input other name", 0, 0);
		else
		{
			CreateDirectory(sNewPath.c_str(), NULL);
			dialog_seldir::pEditCreateNew->setText("");
			dialog_seldir::InitDataListBox(dialog_seldir::sCurrPath.c_str());
		}
		dialog_seldir::pListBox->setFocus();
	}

	return 0;
}

LRESULT ButtonSelect_Click(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int iSel = dialog_seldir::pListBox->getSel();

	if (iSel < 0)
	{
		MessageBox(hWnd, "No select directory", 0, 0);
		return 0;
	}

	char szSelDir[1024];
	dialog_seldir::pListBox->getItemText(iSel, szSelDir);
	dialog_seldir::sSelName = szSelDir;
	dialog_seldir::pSelectDirectory->setVisible(false);

	return 0;
}

LRESULT CALLBACK PreviewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hmdc;
	HBITMAP hBitmap;
	BITMAP bm;

	switch (message)
	{
	case WM_PAINT:

		char szPreview[1024];
		int iSel = dialog_seldir::pListBox->getSel();
		if (iSel < 0)
			return DefWindowProc(hWnd, message, wParam, lParam);

		char szSel[1024];
		dialog_seldir::pListBox->getItemText(iSel, szSel);

		if (!dialog_seldir::lpfnHandlerPreview || !dialog_seldir::lpfnHandlerPreview((FileAppendSlash(dialog_seldir::sCurrPath.c_str()) + szSel).c_str(), szPreview))
			return DefWindowProc(hWnd, message, wParam, lParam);

		hdc = BeginPaint(hWnd, &ps);

		hmdc = CreateCompatibleDC(hdc);
		hBitmap = (HBITMAP)LoadImage(GetModuleHandle(0), szPreview, IMAGE_BITMAP, DIALOG_OWN_SELDIR_PREVIEW_WIDTH, DIALOG_OWN_SELDIR_PREVIEW_HEIGHT, LR_LOADFROMFILE);
		SelectObject(hmdc, hBitmap);
		GetObject(hBitmap, sizeof(bm), &bm);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hmdc, 0, 0, SRCCOPY);
		DeleteDC(hmdc);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
		break;
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));
}

//##########################################################################

void dialog_seldir::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int iCenterX = (wrect.right - DIALOG_OWN_SELDIR_WIDTH) / 2;
	int iCentery = (wrect.bottom - DIALOG_OWN_SELDIR_HEIGHT) / 2;

	dialog_seldir::pSelectDirectory = SXGUICrBaseWndEx("Select Direction", "Select Direction", iCenterX, iCentery, DIALOG_OWN_SELDIR_WIDTH, DIALOG_OWN_SELDIR_HEIGHT, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, SXGUI_WND_DIALOG & WS_VISIBLE, 0, WndProcAllDefault);
	gui_func::base_handlers::InitHandlerMsg(dialog_seldir::pSelectDirectory);
	dialog_seldir::pSelectDirectory->addHandler(SelectDirectory_Exit, WM_CLOSE, 0, 0, 0, 0, true);

	dialog_seldir::pPreView = SXGUICrBaseWndEx("SelDirPreView", "Preview", DIALOG_OWN_SELDIR_WIDTH, 25, DIALOG_OWN_SELDIR_PREVIEW_WIDTH, DIALOG_OWN_SELDIR_PREVIEW_HEIGHT, 0, LoadCursor(NULL, IDC_ARROW), CreateSolidBrush(RGB(200, 200, 200)), 0, CS_HREDRAW | CS_VREDRAW, WS_VISIBLE | WS_BORDER, dialog_seldir::pSelectDirectory->getHWND(), PreviewWndProc);
	dialog_seldir::pPreView->setFollowParentSide(SXGUI_SIDE_LEFT, true);

	dialog_seldir::pListBox = SXGUICrListBox(5, 50, 280, 255, dialog_seldir::pSelectDirectory->getHWND(), 0, 0, false);
	dialog_seldir::pListBox->setFont("Courier New", -11, 0, 400, 0, 0, 0);
	dialog_seldir::pListBox->setColorText(RGB(0, 0, 0));
	dialog_seldir::pListBox->setColorTextBk(RGB(255, 255, 255));
	dialog_seldir::pListBox->setTransparentTextBk(true);
	dialog_seldir::pListBox->setColorBrush(RGB(255, 255, 255));
	dialog_seldir::pListBox->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	dialog_seldir::pListBox->addHandler(ListBox_Click, WM_LBUTTONDBLCLK);
	dialog_seldir::pListBox->addHandler(ListBox_LUP, WM_LBUTTONUP);

	dialog_seldir::pStaticCreateNew = SXGUICrStatic("Create new:", 5, 5, 100, 15, dialog_seldir::pSelectDirectory->getHWND(), 0, 0);
	dialog_seldir::pStaticCreateNew->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_seldir::pStaticCreateNew->setColorText(RGB(0, 0, 0));
	dialog_seldir::pStaticCreateNew->setColorTextBk(RGB(255, 255, 255));
	dialog_seldir::pStaticCreateNew->setTransparentTextBk(true);
	dialog_seldir::pStaticCreateNew->setColorBrush(RGB(220, 220, 220));
	dialog_seldir::pStaticCreateNew->setFollowParentSide(SXGUI_SIDE_LEFT, true);

	dialog_seldir::pEditCreateNew = SXGUICrEdit("", 5, 25, 252, 15, dialog_seldir::pSelectDirectory->getHWND(), 0, 0);
	dialog_seldir::pEditCreateNew->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_seldir::pEditCreateNew->setColorText(RGB(0, 0, 0));
	dialog_seldir::pEditCreateNew->setColorTextBk(RGB(255, 255, 255));
	dialog_seldir::pEditCreateNew->setTransparentTextBk(true);
	dialog_seldir::pEditCreateNew->setColorBrush(RGB(255, 255, 255));
	dialog_seldir::pEditCreateNew->addHandler(ButtonCreateNew_Click, WM_KEYDOWN, VK_RETURN, 1, 0, 0, true);
	dialog_seldir::pEditCreateNew->setFollowParentSide(SXGUI_SIDE_LEFT, true);

	dialog_seldir::pButtonCreateNew = SXGUICrButton("Ok", 260, 25, 25, 15, SXGUI_BUTTON_IMAGE_NONE, dialog_seldir::pSelectDirectory->getHWND(), 0, 0);
	dialog_seldir::pButtonCreateNew->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_seldir::pButtonCreateNew->addHandler(ButtonCreateNew_Click, WM_LBUTTONUP, 0, 0, 0, 0, true);
	dialog_seldir::pButtonCreateNew->setFollowParentSide(SXGUI_SIDE_LEFT, true);

	dialog_seldir::pButtonCancel = SXGUICrButton("Cancel", 5, 345, 100, 20, SXGUI_BUTTON_IMAGE_NONE, dialog_seldir::pSelectDirectory->getHWND(), 0, 0);
	dialog_seldir::pButtonCancel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_seldir::pButtonCancel->addHandler(SelectDirectory_Exit, WM_LBUTTONUP, 0, 0, 0, 0, true);
	dialog_seldir::pButtonCancel->setFollowParentSide(SXGUI_SIDE_LEFT, true);

	dialog_seldir::pButtonSelect = SXGUICrButton("Select", 185, 345, 100, 20, SXGUI_BUTTON_IMAGE_NONE, dialog_seldir::pSelectDirectory->getHWND(), 0, 0);
	dialog_seldir::pButtonSelect->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_seldir::pButtonSelect->addHandler(ButtonSelect_Click, WM_LBUTTONUP, 0, 0, 0, 0, true);
	dialog_seldir::pButtonSelect->setFollowParentSide(SXGUI_SIDE_LEFT, true);

	dialog_seldir::pEditCurrentPath = SXGUICrEdit("0", 5, 310, 280, 15, dialog_seldir::pSelectDirectory->getHWND(), 0, 0);
	dialog_seldir::pEditCurrentPath->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_seldir::pEditCurrentPath->setColorText(RGB(0, 0, 0));
	dialog_seldir::pEditCurrentPath->setColorTextBk(RGB(255, 255, 255));
	dialog_seldir::pEditCurrentPath->setTransparentTextBk(true);
	dialog_seldir::pEditCurrentPath->setColorBrush(RGB(255, 255, 255));
	dialog_seldir::pEditCurrentPath->addHandler(EditCurrentPath_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	dialog_seldir::pEditCurrentPath->addHandler(EditCurrentPath_Input, WM_KEYUP);
	dialog_seldir::pEditCurrentPath->setFollowParentSide(SXGUI_SIDE_LEFT, true);

	dialog_seldir::pStaticCurrentPath = SXGUICrStatic("Current path:", 5, 290, 100, 15, dialog_seldir::pSelectDirectory->getHWND(), 0, 0);
	dialog_seldir::pStaticCurrentPath->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_seldir::pStaticCurrentPath->setColorText(RGB(0, 0, 0));
	dialog_seldir::pStaticCurrentPath->setColorTextBk(RGB(255, 255, 255));
	dialog_seldir::pStaticCurrentPath->setTransparentTextBk(true);
	dialog_seldir::pStaticCurrentPath->setColorBrush(RGB(220, 220, 220));
	dialog_seldir::pStaticCurrentPath->setFollowParentSide(SXGUI_SIDE_LEFT, true);
}

void dialog_seldir::InitDataListBox(const char *szPath)
{
	dialog_seldir::pListBox->clear();
	dialog_seldir::sCurrPath = FileAppendSlash(szPath);
	dialog_seldir::sSelName = "";
	int iNesting = FileCountNesting(dialog_seldir::sCurrPath.c_str());
	if (iNesting > 0 && dialog_seldir::canExplore && !(dialog_seldir::sDownDir.length() > 0 && dialog_seldir::sDownDir == dialog_seldir::sCurrPath))
		dialog_seldir::pListBox->addItem("..");
	Array<String> aStrings = FileGetListDirs(dialog_seldir::sCurrPath.c_str());
	for (int i = 0; i < aStrings.size(); ++i)
	{
		dialog_seldir::pListBox->addItem(aStrings[i].c_str());
	}

	dialog_seldir::pEditCurrentPath->setText(FileCanonizePathS(dialog_seldir::sCurrPath.c_str()).c_str());
}

void dialog_seldir::DeleteAllElements()
{
	mem_delete(dialog_seldir::pListBox);
	mem_delete(dialog_seldir::pStaticCreateNew);
	mem_delete(dialog_seldir::pEditCreateNew);
	mem_delete(dialog_seldir::pButtonCreateNew);
	mem_delete(dialog_seldir::pButtonCancel);
	mem_delete(dialog_seldir::pButtonSelect);
	mem_delete(dialog_seldir::pEditCurrentPath);
	mem_delete(dialog_seldir::pStaticCurrentPath);
	mem_delete(dialog_seldir::pSelectDirectory);
}

//##########################################################################

void DialogOwnSelectDirInit()
{
	dialog_seldir::InitAllElements();
	//dialog_seldir::pSelectDirectory->setVisible(false);
}

bool DialogOwnSelectDirRun(char *szOutName, char *szOutPath, const char *szStartPath, const char *szDialogName, bool canExplore, bool canCreateNew, const char *szDownPath, HandlerDialogOwndGetPreview lpfnHandlerPreview)
{
	if (!FileExistsDir(szStartPath))
	{
		char szMessage[1024];
		sprintf(szMessage, "Start path [%s] is not exists", szStartPath);
		MessageBox(0, szMessage, "Select Direction", 0);
	}

	dialog_seldir::lpfnHandlerPreview = lpfnHandlerPreview;

	if (szDownPath)
		dialog_seldir::sDownDir = FileAppendSlash(szDownPath);
	else
		dialog_seldir::sDownDir = "";

	dialog_seldir::pButtonCreateNew->setEnable(canCreateNew);
	dialog_seldir::pEditCreateNew->setEnable(canCreateNew);

	dialog_seldir::canExplore = canExplore;
	dialog_seldir::InitDataListBox(szStartPath);

	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int iCenterX = (wrect.right - (dialog_seldir::lpfnHandlerPreview ? DIALOG_OWN_SELDIR_WITH_PREVIEW_WIDTH : DIALOG_OWN_SELDIR_WIDTH)) / 2;
	int iCentery = (wrect.bottom - DIALOG_OWN_SELDIR_HEIGHT) / 2;
	MoveWindow(dialog_seldir::pSelectDirectory->getHWND(), iCenterX, iCentery, (dialog_seldir::lpfnHandlerPreview ? DIALOG_OWN_SELDIR_WITH_PREVIEW_WIDTH : DIALOG_OWN_SELDIR_WIDTH), DIALOG_OWN_SELDIR_HEIGHT, true);

	if (szDialogName)
		dialog_seldir::pSelectDirectory->setText(szDialogName);

	dialog_seldir::pSelectDirectory->setVisible(true);

	dialog_seldir::pEditCurrentPath->setEnable(dialog_seldir::canExplore);

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while (dialog_seldir::pSelectDirectory->getVisible())
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	if (dialog_seldir::sSelName.length() == 0)
		return false;

	if (szOutName)
		sprintf(szOutName, "%s", dialog_seldir::sSelName.c_str());

	if (szOutPath)
		sprintf(szOutPath, "%s%s", dialog_seldir::sCurrPath.c_str(), dialog_seldir::sSelName.c_str());

	return true;
}