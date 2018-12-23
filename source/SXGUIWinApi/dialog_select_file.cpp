
#include "dialog_select_file.h"

namespace dialog_selfile
{
	ISXGUIBaseWnd *SelectFile;
	ISXGUIStatic *StaticDirectories;
	ISXGUIListBox *ListBoxDirectories;
	ISXGUIStatic *StaticFiles;
	ISXGUIListBox *ListBoxFiles;
	ISXGUIStatic *StaticPreview;
	ISXGUIBaseWnd *WindowPreview;
	ISXGUIStatic *StaticDirFullPath;
	ISXGUIStatic *StaticFileFullPath;
	ISXGUIEdit *EditDirFullPath;
	ISXGUIButton *ButtonCancel;
	ISXGUIButton *ButtonSelect;
	ISXGUIStatic *StaticInformation;
	ISXGUIStatic *StaticInformationText;
	ISXGUIEdit *EditFileFullPath;

	void InitAllElements();

	void DeleteAllElements();

	void InitDataListBoxDirs(const char *szPath);
	void InitDataListBoxFiles(const char *szPath);


	String sCurrPath;
	String sSelName;
	String sDownDir;
	String sFilterExt;

	HandlerDialogOwndGetPreview2 lpfnHandlerPreview = 0;
	HandlerDialogOwndGetInfo lpfnHandlerInfo = 0;

	bool canExplore;
};

//##########################################################################

LRESULT SelFile_ListBoxDirs_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!dialog_selfile::canExplore)
		return 0;

	int iSel = dialog_selfile::ListBoxDirectories->getSel();

	if (iSel < 0)
		return 0;

	int iNesting = FileCountNesting(dialog_selfile::sCurrPath.c_str());

	char szSel[MAX_PATH];
	dialog_selfile::ListBoxDirectories->getItemText(iSel, szSel);

	if (iNesting > 0 && iSel == 0 && strcmp(szSel, "..") == 0)
	{
		String sPrevPath = FileGetPrevDir(dialog_selfile::sCurrPath.c_str());
		dialog_selfile::InitDataListBoxDirs(sPrevPath.c_str());
		dialog_selfile::InitDataListBoxFiles(sPrevPath.c_str());
	}
	else
	{
		//dialog_selfile::sCurrPath = dialog_selfile::sCurrPath + "/" + szSel;
		dialog_selfile::InitDataListBoxDirs((FileAppendSlash(dialog_selfile::sCurrPath.c_str()) + szSel).c_str());
		dialog_selfile::InitDataListBoxFiles((FileAppendSlash(dialog_selfile::sCurrPath.c_str()) + szSel).c_str());
	}

	return 0;
}

LRESULT SelFile_ListBoxFiles_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!dialog_selfile::canExplore)
		return 0;

	int iSel = dialog_selfile::ListBoxFiles->getSel();

	if (iSel < 0)
		return 0;

	char szSel[MAX_PATH];
	dialog_selfile::ListBoxFiles->getItemText(iSel, szSel);

	dialog_selfile::EditFileFullPath->setText((FileAppendSlash(dialog_selfile::sCurrPath.c_str()) + szSel).c_str());
	InvalidateRect(dialog_selfile::WindowPreview->getHWND(), 0, TRUE);

	if (dialog_selfile::lpfnHandlerInfo)
	{
		char szBuf[1024];
		if (dialog_selfile::lpfnHandlerInfo((FileAppendSlash(dialog_selfile::sCurrPath.c_str()) + szSel).c_str(), szBuf))
			dialog_selfile::StaticInformationText->setText(szBuf);
		else
			dialog_selfile::StaticInformationText->setText("");
	}

	return 0;
}

LRESULT SelFile_EditDirFullPath_Enter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	dialog_selfile::EditDirFullPath->getText(szPath, 1024);

	if (!FileExistsInPath(szPath, dialog_selfile::sDownDir.c_str()))
	{
		char szMessage[1024];
		sprintf(szMessage, "Unresolved explore down path [%s]", dialog_selfile::sDownDir.c_str());
		MessageBox(hWnd, szMessage, 0, 0);
		dialog_selfile::EditDirFullPath->setText(dialog_selfile::sCurrPath.c_str());
		return 0;
	}

	if (FileExistsDir(szPath))
		dialog_selfile::InitDataListBoxDirs(szPath);
	else
	{
		char szMessage[1024];
		sprintf(szMessage, "Directory [%s] is not exists", szPath);
		MessageBox(hWnd, szMessage, 0, 0);
	}
	return 0;
}

/*LRESULT SelFile_EditDirFullPath_Input(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	dialog_selfile::EditDirFullPath->getText(szPath, 1024);

	const char *szBase = FileBaseName(szPath);
	dialog_selfile::ListBoxDirectories->setSel(-1);
	if (strlen(szBase) == 0)
		return 0;

	char szDir[MAX_PATH];
	char *szFound = 0;
	for (int i = 0; i < dialog_selfile::ListBoxDirectories->getItemCount(); ++i)
	{
		dialog_selfile::ListBoxDirectories->getItemText(i, szDir);

		if ((szFound = strstr(szDir, szBase)) && (szFound - szDir) == 0)
		{
			//gui_func::scrollbar::ScrollLine(dialog_selfile::pListBox, SXGUI_SCROLL_TYPE_V, SXGUI_SCROLL_DIR_UP, dialog_selfile::pListBox->getItemCount());
			//gui_func::scrollbar::ScrollLine(dialog_selfile::pListBox, SXGUI_SCROLL_TYPE_V, SXGUI_SCROLL_DIR_DOWN, i);

			dialog_selfile::ListBoxDirectories->setSel(i);
			SelFile_ListBoxDirs_Click(hWnd, uMsg, wParam, lParam);

			break;
		}
	}

	return 0;
}*/

//LRESULT SelFile_EditFileFullPath_Input(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT SelFile_EditFileFullPath_Enter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	dialog_selfile::EditFileFullPath->getText(szPath, 1024);

	if (!FileExistsInPath(szPath, dialog_selfile::sDownDir.c_str()))
	{
		char szMessage[1024];
		sprintf(szMessage, "Unresolved explore down path [%s]", dialog_selfile::sDownDir.c_str());
		MessageBox(hWnd, szMessage, 0, 0);
		dialog_selfile::EditDirFullPath->setText(dialog_selfile::sCurrPath.c_str());
		return 0;
	}

	if (FileExistsFile(szPath))
	{
		dialog_selfile::InitDataListBoxDirs(FileGetPrevDir(szPath).c_str());
		dialog_selfile::InitDataListBoxFiles(FileGetPrevDir(szPath).c_str());
		dialog_selfile::EditFileFullPath->setText(szPath);
		//SelFile_EditFileFullPath_Input(hWnd, uMsg, wParam, lParam);

		dialog_selfile::EditFileFullPath->getText(szPath, 1024);
		const char *szBase = FileBaseName(szPath);

		char szDir[MAX_PATH];
		char *szFound = 0;
		for (int i = 0; i < dialog_selfile::ListBoxFiles->getItemCount(); ++i)
		{
			dialog_selfile::ListBoxFiles->getItemText(i, szDir);

			if ((szFound = strstr(szDir, szBase)) && (szFound - szDir) == 0)
			{
				//gui_func::scrollbar::ScrollLine(dialog_selfile::pListBox, SXGUI_SCROLL_TYPE_V, SXGUI_SCROLL_DIR_UP, dialog_selfile::pListBox->getItemCount());
				//gui_func::scrollbar::ScrollLine(dialog_selfile::pListBox, SXGUI_SCROLL_TYPE_V, SXGUI_SCROLL_DIR_DOWN, i);

				dialog_selfile::ListBoxFiles->setSel(i);
				SelFile_ListBoxFiles_Click(hWnd, uMsg, wParam, lParam);

				break;
			}
		}
	}
	else
	{
		char szMessage[1024];
		sprintf(szMessage, "File [%s] is not exists", szPath);
		MessageBox(hWnd, szMessage, 0, 0);
	}
	return 0;
}

/*LRESULT SelFile_EditFileFullPath_Input(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char szPath[1024];
	dialog_selfile::EditFileFullPath->getText(szPath, 1024);

	const char *szBase = FileBaseName(szPath);
	dialog_selfile::ListBoxFiles->setSel(-1);
	if (strlen(szBase) == 0)
		return 0;

	char szDir[MAX_PATH];
	char *szFound = 0;
	for (int i = 0; i < dialog_selfile::ListBoxFiles->getItemCount(); ++i)
	{
		dialog_selfile::ListBoxFiles->getItemText(i, szDir);

		if ((szFound = strstr(szDir, szBase)) && (szFound - szDir) == 0)
		{
			//gui_func::scrollbar::ScrollLine(dialog_selfile::pListBox, SXGUI_SCROLL_TYPE_V, SXGUI_SCROLL_DIR_UP, dialog_selfile::pListBox->getItemCount());
			//gui_func::scrollbar::ScrollLine(dialog_selfile::pListBox, SXGUI_SCROLL_TYPE_V, SXGUI_SCROLL_DIR_DOWN, i);

			dialog_selfile::ListBoxFiles->setSel(i);
			SelFile_ListBoxFiles_Click(hWnd, uMsg, wParam, lParam);

			break;
		}
	}

	return 0;
}*/

LRESULT SelectFile_Exit(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	dialog_selfile::SelectFile->setVisible(false);
	return 0;
}

LRESULT SelFile_ButtonSelect_Click(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int iSel = dialog_selfile::ListBoxFiles->getSel();

	if (iSel < 0)
	{
		if (hWnd == dialog_selfile::ButtonSelect->getHWND())
			MessageBox(hWnd, "No select file", 0, 0);
		return 0;
	}

	char szSelDir[1024];
	dialog_selfile::ListBoxFiles->getItemText(iSel, szSelDir);
	dialog_selfile::sSelName = szSelDir;
	dialog_selfile::SelectFile->setVisible(false);

	return 0;
}

LRESULT CALLBACK SelFile_PreviewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hmdc;
	HBITMAP hBitmap;
	BITMAP bm;

	switch (message)
	{
	case WM_PAINT:

		int iSel = dialog_selfile::ListBoxFiles->getSel();
		if (iSel < 0)
			return DefWindowProc(hWnd, message, wParam, lParam);

		char szSel[1024];
		dialog_selfile::ListBoxFiles->getItemText(iSel, szSel);

		void *pBuf = 0;
		int iSizeBuf = 0;
		int iWidth = 0;
		int iHeight = 0;

		if (!dialog_selfile::lpfnHandlerPreview || !dialog_selfile::lpfnHandlerPreview((FileAppendSlash(dialog_selfile::sCurrPath.c_str()) + szSel).c_str(), &pBuf, &iSizeBuf, &iWidth, &iHeight))
			return DefWindowProc(hWnd, message, wParam, lParam);

		hdc = BeginPaint(hWnd, &ps);

		/*hmdc = CreateCompatibleDC(hdc);
		hBitmap = (HBITMAP)LoadImage(GetModuleHandle(0), szPreview, IMAGE_BITMAP, DIALOG_OWN_SELFILE_PREVIEW_WIDTH, DIALOG_OWN_SELFILE_PREVIEW_HEIGHT, LR_LOADFROMFILE);
		SelectObject(hmdc, hBitmap);
		GetObject(hBitmap, sizeof(bm), &bm);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hmdc, 0, 0, SRCCOPY);
		DeleteDC(hmdc);
		DeleteObject(hBitmap);*/

		BITMAPINFO bmi;
		memset(&bmi, 0, sizeof(bmi));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
		bmi.bmiHeader.biWidth = iWidth;
		bmi.bmiHeader.biHeight = -iHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = iSizeBuf;

		//
		// Создадим SetDIBitsToDevice.
		//

		int iRet = SetDIBitsToDevice(hdc,
			0, 0,
			iWidth, iHeight,
			0, 0,
			0, iHeight,
			pBuf,
			&bmi,
			DIB_RGB_COLORS);

		DWORD error = GetLastError();

		EndPaint(hWnd, &ps);
		break;
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));
}

//##########################################################################

void dialog_selfile::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int iCenterX = (wrect.right - DIALOG_OWN_SELFILE_WIDTH) / 2;
	int iCentery = (wrect.bottom - DIALOG_OWN_SELFILE_HEIGHT) / 2;

	dialog_selfile::SelectFile = SXGUICrBaseWndEx("Select_file", "Select_file", iCenterX, iCentery, DIALOG_OWN_SELFILE_WIDTH, DIALOG_OWN_SELFILE_HEIGHT, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, SXGUI_WND_DIALOG, 0, WndProcAllDefault);
	gui_func::base_handlers::InitHandlerMsg(dialog_selfile::SelectFile);
	dialog_selfile::SelectFile->addHandler(SelectFile_Exit, WM_CLOSE, 0, 0, 0, 0, true);

	dialog_selfile::StaticDirectories = SXGUICrStatic("Directories:", 5, 5, 60, 15, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::StaticDirectories->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::StaticDirectories->setColorText(RGB(0, 0, 0));
	dialog_selfile::StaticDirectories->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::StaticDirectories->setTransparentTextBk(true);
	dialog_selfile::StaticDirectories->setColorBrush(RGB(220, 220, 220));
	dialog_selfile::ListBoxDirectories = SXGUICrListBox(5, 25, 200, 320, dialog_selfile::SelectFile->getHWND(), 0, 0, false);
	dialog_selfile::ListBoxDirectories->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::ListBoxDirectories->setColorText(RGB(0, 0, 0));
	dialog_selfile::ListBoxDirectories->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::ListBoxDirectories->setTransparentTextBk(true);
	dialog_selfile::ListBoxDirectories->setColorBrush(RGB(255, 255, 255));
	dialog_selfile::ListBoxDirectories->addHandler(SelFile_ListBoxDirs_Click, WM_LBUTTONUP);

	dialog_selfile::StaticFiles = SXGUICrStatic("Files:", 215, 5, 50, 15, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::StaticFiles->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::StaticFiles->setColorText(RGB(0, 0, 0));
	dialog_selfile::StaticFiles->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::StaticFiles->setTransparentTextBk(true);
	dialog_selfile::StaticFiles->setColorBrush(RGB(220, 220, 220));
	dialog_selfile::ListBoxFiles = SXGUICrListBox(215, 25, 250, 320, dialog_selfile::SelectFile->getHWND(), 0, 0, false);
	dialog_selfile::ListBoxFiles->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::ListBoxFiles->setColorText(RGB(0, 0, 0));
	dialog_selfile::ListBoxFiles->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::ListBoxFiles->setTransparentTextBk(true);
	dialog_selfile::ListBoxFiles->setColorBrush(RGB(255, 255, 255));
	dialog_selfile::ListBoxFiles->addHandler(SelFile_ListBoxFiles_Click, WM_LBUTTONUP);
	dialog_selfile::ListBoxFiles->addHandler(SelFile_ButtonSelect_Click, WM_LBUTTONDBLCLK);

	dialog_selfile::StaticPreview = SXGUICrStatic("Preview:", 470, 5, 50, 15, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::StaticPreview->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::StaticPreview->setColorText(RGB(0, 0, 0));
	dialog_selfile::StaticPreview->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::StaticPreview->setTransparentTextBk(true);
	dialog_selfile::StaticPreview->setColorBrush(RGB(220, 220, 220));
	
	dialog_selfile::WindowPreview = SXGUICrBaseWndEx("WindowPreview", "WindowPreview", 470, 25, DIALOG_OWN_SELFILE_PREVIEW_SIZE, DIALOG_OWN_SELFILE_PREVIEW_SIZE, 0, 0, CreateSolidBrush(RGB(200, 200, 200)), 0, CS_HREDRAW | CS_VREDRAW, WS_CHILD | WS_VISIBLE | WS_BORDER, dialog_selfile::SelectFile->getHWND(), SelFile_PreviewWndProc);
	
	dialog_selfile::StaticDirFullPath = SXGUICrStatic("Dir full path:", 5, 345, 60, 15, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::StaticDirFullPath->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::StaticDirFullPath->setColorText(RGB(0, 0, 0));
	dialog_selfile::StaticDirFullPath->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::StaticDirFullPath->setTransparentTextBk(true);
	dialog_selfile::StaticDirFullPath->setColorBrush(RGB(220, 220, 220));
	dialog_selfile::StaticFileFullPath = SXGUICrStatic("File full path:", 5, 365, 60, 15, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::StaticFileFullPath->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::StaticFileFullPath->setColorText(RGB(0, 0, 0));
	dialog_selfile::StaticFileFullPath->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::StaticFileFullPath->setTransparentTextBk(true);
	dialog_selfile::StaticFileFullPath->setColorBrush(RGB(220, 220, 220));
	dialog_selfile::EditDirFullPath = SXGUICrEdit("0", 65, 345, 400, 15, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::EditDirFullPath->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::EditDirFullPath->setColorText(RGB(0, 0, 0));
	dialog_selfile::EditDirFullPath->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::EditDirFullPath->setTransparentTextBk(true);
	dialog_selfile::EditDirFullPath->setColorBrush(RGB(255, 255, 255));
	dialog_selfile::EditDirFullPath->addHandler(SelFile_EditDirFullPath_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	//dialog_selfile::EditDirFullPath->addHandler(SelFile_EditDirFullPath_Input, WM_KEYUP);

	dialog_selfile::ButtonCancel = SXGUICrButton("Cancel", 245, 395, 100, 20, SXGUI_BUTTON_IMAGE_NONE, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::ButtonCancel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::ButtonCancel->addHandler(SelectFile_Exit, WM_LBUTTONUP, 0, 0, 0, 0, true);

	dialog_selfile::ButtonSelect = SXGUICrButton("Select", 365, 395, 100, 20, SXGUI_BUTTON_IMAGE_NONE, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::ButtonSelect->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::ButtonSelect->addHandler(SelFile_ButtonSelect_Click, WM_LBUTTONUP, 0, 0, 0, 0, true);

	dialog_selfile::StaticInformation = SXGUICrStatic("Information:", 470, 285, 60, 15, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::StaticInformation->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::StaticInformation->setColorText(RGB(0, 0, 0));
	dialog_selfile::StaticInformation->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::StaticInformation->setTransparentTextBk(true);
	dialog_selfile::StaticInformation->setColorBrush(RGB(220, 220, 220));
	dialog_selfile::StaticInformationText = SXGUICrStatic("0", 470, 305, 256, 110, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::StaticInformationText->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::StaticInformationText->setColorText(RGB(0, 0, 0));
	dialog_selfile::StaticInformationText->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::StaticInformationText->setTransparentTextBk(true);
	dialog_selfile::StaticInformationText->setColorBrush(RGB(255, 255, 255));
	dialog_selfile::EditFileFullPath = SXGUICrEdit("0", 65, 365, 400, 15, dialog_selfile::SelectFile->getHWND(), 0, 0);
	dialog_selfile::EditFileFullPath->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	dialog_selfile::EditFileFullPath->setColorText(RGB(0, 0, 0));
	dialog_selfile::EditFileFullPath->setColorTextBk(RGB(255, 255, 255));
	dialog_selfile::EditFileFullPath->setTransparentTextBk(true);
	dialog_selfile::EditFileFullPath->setColorBrush(RGB(255, 255, 255));
	dialog_selfile::EditFileFullPath->addHandler(SelFile_EditFileFullPath_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	//dialog_selfile::EditFileFullPath->addHandler(SelFile_EditFileFullPath_Input, WM_KEYUP);
}

void dialog_selfile::DeleteAllElements()
{
	mem_delete(dialog_selfile::StaticDirectories);
	mem_delete(dialog_selfile::ListBoxDirectories);
	mem_delete(dialog_selfile::StaticFiles);
	mem_delete(dialog_selfile::ListBoxFiles);
	mem_delete(dialog_selfile::StaticPreview);
	mem_delete(dialog_selfile::WindowPreview);
	mem_delete(dialog_selfile::StaticDirFullPath);
	mem_delete(dialog_selfile::StaticFileFullPath);
	mem_delete(dialog_selfile::EditDirFullPath);
	mem_delete(dialog_selfile::ButtonCancel);
	mem_delete(dialog_selfile::ButtonSelect);
	mem_delete(dialog_selfile::StaticInformation);
	mem_delete(dialog_selfile::StaticInformationText);
	mem_delete(dialog_selfile::EditFileFullPath);
	mem_delete(dialog_selfile::SelectFile);
}

//##########################################################################

void dialog_selfile::InitDataListBoxDirs(const char *szPath)
{
	dialog_selfile::ListBoxDirectories->clear();
	dialog_selfile::ListBoxFiles->clear();
	dialog_selfile::sCurrPath = FileCanonizePathS(FileAppendSlash(szPath).c_str());
	dialog_selfile::sSelName = "";
	int iNesting = FileCountNesting(dialog_selfile::sCurrPath.c_str());
	if (iNesting > 0 && dialog_selfile::canExplore && !(dialog_selfile::sDownDir.length() > 0 && dialog_selfile::sDownDir == dialog_selfile::sCurrPath))
		dialog_selfile::ListBoxDirectories->addItem("..");
	Array<String> aStrings = FileGetListDirs(dialog_selfile::sCurrPath.c_str());
	for (int i = 0; i < aStrings.size(); ++i)
	{
		dialog_selfile::ListBoxDirectories->addItem(aStrings[i].c_str());
	}

	dialog_selfile::EditDirFullPath->setText(FileCanonizePathS(dialog_selfile::sCurrPath.c_str()).c_str());
}

void dialog_selfile::InitDataListBoxFiles(const char *szPath)
{
	dialog_selfile::ListBoxFiles->clear();
	dialog_selfile::sSelName = "";
	String sPathFilter = FileAppendSlash(dialog_selfile::sCurrPath.c_str()) + (dialog_selfile::sFilterExt.length() > 0 ? (String("*.") + dialog_selfile::sFilterExt) : "");
	Array<String> aStrings = FileGetListFiles((FileAppendSlash(dialog_selfile::sCurrPath.c_str()) + "*" + dialog_selfile::sFilterExt).c_str());
	for (int i = 0; i < aStrings.size(); ++i)
	{
		dialog_selfile::ListBoxFiles->addItem(aStrings[i].c_str());
	}

	dialog_selfile::EditFileFullPath->setText("");
}

//##########################################################################

void DialogOwnSelectFileInit()
{
	dialog_selfile::InitAllElements();
}

bool DialogOwnSelectFileRun(char *szOutName, char *szOutPath, const char *szStartPath, const char *szFilterExt, const char *szDialogName, bool canExplore, const char *szDownPath, HWND hWndLock, HandlerDialogOwndGetPreview2 lpfnHandlerPreview, HandlerDialogOwndGetInfo lpfnHandlerInfo)
{
	if (!FileExistsDir(szStartPath))
	{
		char szMessage[1024];
		sprintf(szMessage, "Start path [%s] is not exists", szStartPath);
		MessageBox(0, szMessage, "Select Direction", 0);
	}

	dialog_selfile::lpfnHandlerPreview = lpfnHandlerPreview;
	dialog_selfile::lpfnHandlerInfo = lpfnHandlerInfo;

	if (szDownPath)
		dialog_selfile::sDownDir = FileCanonizePathS(FileAppendSlash(szDownPath).c_str());
	else
		dialog_selfile::sDownDir = "";

	if (szFilterExt)
		dialog_selfile::sFilterExt = szFilterExt;
	else
		dialog_selfile::sFilterExt = "";

	dialog_selfile::canExplore = canExplore;
	dialog_selfile::InitDataListBoxDirs(szStartPath);
	dialog_selfile::InitDataListBoxFiles(szStartPath);

	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int iCenterX = (wrect.right - DIALOG_OWN_SELFILE_WIDTH) / 2;
	int iCentery = (wrect.bottom - DIALOG_OWN_SELFILE_HEIGHT) / 2;
	MoveWindow(dialog_selfile::SelectFile->getHWND(), iCenterX, iCentery, DIALOG_OWN_SELFILE_WIDTH, DIALOG_OWN_SELFILE_HEIGHT, true);

	if (szDialogName)
		dialog_selfile::SelectFile->setText(szDialogName);

	dialog_selfile::SelectFile->setVisible(true);

	dialog_selfile::EditDirFullPath->setEnable(dialog_selfile::canExplore);
	dialog_selfile::EditFileFullPath->setEnable(dialog_selfile::canExplore);

	if (hWndLock)
		EnableWindow(hWndLock, FALSE);

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while (dialog_selfile::SelectFile->getVisible())
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	SetForegroundWindow(hWndLock);

	dialog_selfile::StaticInformationText->setText("");

	if (hWndLock)
		EnableWindow(hWndLock, TRUE);

	if (dialog_selfile::sSelName.length() == 0)
		return false;

	if (szOutName)
		sprintf(szOutName, "%s", dialog_selfile::sSelName.c_str());

	if (szOutPath)
		sprintf(szOutPath, "%s%s", dialog_selfile::sCurrPath.c_str(), dialog_selfile::sSelName.c_str());

	return true;
}