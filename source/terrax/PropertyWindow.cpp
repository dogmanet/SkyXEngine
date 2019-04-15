#include "PropertyWindow.h"
#include "resource.h"

#include <windowsx.h>
#include <commctrl.h>

static UINT g_uEditorDlgIds[] = {
	IDD_PROPEDIT_TEXT,
	IDD_PROPEDIT_FILE,
	IDD_PROPEDIT_COMBO
};

static_assert(ARRAYSIZE(g_uEditorDlgIds) == XPET__LAST, "g_uEditorDlgIds must match X_PROP_EDITOR_TYPE");

CPropertyWindow::CPropertyWindow(HINSTANCE hInstance, HWND hMainWnd):
	m_hInstance(hInstance)
{
	CreateDialogParamA(m_hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hMainWnd, DlgProc, (LPARAM)this);
	show();

	// SetWindowPos(m_hDlgWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

CPropertyWindow::~CPropertyWindow()
{
	DestroyWindow(m_hDlgWnd);
}

void CPropertyWindow::show()
{
	ShowWindow(m_hDlgWnd, SW_SHOWNA);
}
void CPropertyWindow::hide()
{
	ShowWindow(m_hDlgWnd, SW_HIDE);
}
bool CPropertyWindow::isVisible()
{
	return(IsWindowVisible(m_hDlgWnd));
}

INT_PTR CALLBACK CPropertyWindow::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CPropertyWindow *pThis = (CPropertyWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(msg == WM_INITDIALOG)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)lParam);
		pThis = (CPropertyWindow*)lParam;
		pThis->m_hDlgWnd = hWnd;
	}
	if(pThis)
	{
		return(pThis->dlgProc(hWnd, msg, wParam, lParam));
	}
	return(FALSE);
}

INT_PTR CALLBACK CPropertyWindow::PropDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	/*case WM_CTLCOLOREDIT:
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORLISTBOX:
		return((INT_PTR)GetSysColorBrush(COLOR_WINDOW));
		*/
	case WM_COMMAND:
	case WM_NOTIFY:
		return(SendMessage(GetParent(hWnd), msg, wParam, lParam));

	case WM_LBUTTONDOWN:
		SetFocus(GetParent(hWnd));
		return(TRUE);

	}
	return(FALSE);
}

INT_PTR CALLBACK CPropertyWindow::EditorDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
	case WM_NOTIFY:
		return(SendMessage(GetParent(hWnd), msg, wParam, lParam));

	case WM_LBUTTONDOWN:
		SetFocus(GetParent(GetParent(hWnd)));
		return(TRUE);

	}
	return(FALSE);
}

INT_PTR CALLBACK CPropertyWindow::dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			m_hTabControl = GetDlgItem(hWnd, IDC_TAB1);

			TCITEM ti;
			ti.mask = TCIF_TEXT;
			ti.pszText = "Properties";
			TabCtrl_InsertItem(m_hTabControl, 0, &ti);
			ti.pszText = "Flags";
			TabCtrl_InsertItem(m_hTabControl, 1, &ti);
			ti.pszText = "Outputs";
			TabCtrl_InsertItem(m_hTabControl, 2, &ti);
			TabCtrl_SetCurSel(m_hTabControl, 0);


			RECT rcDisplay = {0};
			GetWindowRect(m_hTabControl, &rcDisplay);
			MapWindowPoints(HWND_DESKTOP, m_hDlgWnd, (LPPOINT)&rcDisplay, 2);
			TabCtrl_AdjustRect(m_hTabControl, FALSE, &rcDisplay);

			m_hPropTabs[0] = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_OP_PROPS), m_hDlgWnd, PropDlgProc);
			m_hPropTabs[1] = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_OP_FLAGS), m_hDlgWnd, NULL);
			m_hPropTabs[2] = CreateDialog(m_hInstance, MAKEINTRESOURCE(IDD_OP_OUTPUTS), m_hDlgWnd, NULL);

			const int iXDelta = -3;

			for(int i = 0; i < 3; ++i)
			{
				SetWindowPos(m_hPropTabs[i], HWND_TOP, rcDisplay.left + iXDelta, rcDisplay.top, rcDisplay.right - rcDisplay.left - iXDelta, rcDisplay.bottom - rcDisplay.top, 0);
			}

			onSelChanged();

			m_hNoPropsText = GetDlgItem(m_hDlgWnd, IDC_NO_PROPS);

			m_hClassListWnd = GetDlgItem(m_hPropTabs[0], IDC_CLASS_NAME);
			COMBOBOXINFO info = {sizeof(COMBOBOXINFO)};
			GetComboBoxInfo(m_hClassListWnd, &info);
			SetWindowLongPtr(info.hwndItem, GWLP_USERDATA, (LONG_PTR)this);
			m_pfnClassListOldWndproc = (WNDPROC)GetWindowLongPtr(info.hwndItem, GWLP_WNDPROC);
			SetWindowLongPtr(info.hwndItem, GWLP_WNDPROC, (LONG_PTR)ClassListWndProc);

			m_hPropListWnd = GetDlgItem(m_hPropTabs[0], IDC_LIST1);
			ListView_SetExtendedListViewStyle(m_hPropListWnd, ListView_GetExtendedListViewStyle(m_hPropListWnd) | LVS_EX_FULLROWSELECT);

			LV_COLUMNA lvColumn;
			memset(&lvColumn, 0, sizeof(lvColumn));

			RECT rc;
			GetClientRect(m_hPropListWnd, &rc);

			lvColumn.mask = LVCF_WIDTH | LVCF_TEXT;
			lvColumn.cx = (rc.right - rc.left) / 2;
			lvColumn.pszText = "Property";
			lvColumn.cchTextMax = strlen(lvColumn.pszText);
			ListView_InsertColumn(m_hPropListWnd, 0, &lvColumn);

			lvColumn.pszText = "Value";
			lvColumn.cchTextMax = strlen(lvColumn.pszText);
			ListView_InsertColumn(m_hPropListWnd, 1, &lvColumn);


			HWND hEditorArea = GetDlgItem(m_hPropTabs[0], IDC_EDITAREA);

			GetWindowRect(hEditorArea, &rcDisplay);
			MapWindowPoints(HWND_DESKTOP, m_hPropTabs[0], (LPPOINT)&rcDisplay, 2);
			ShowWindow(hEditorArea, SW_HIDE);
			for(int i = 0; i < XPET__LAST; ++i)
			{
				m_phEditors[i] = CreateDialog(m_hInstance, MAKEINTRESOURCE(g_uEditorDlgIds[i]), m_hPropTabs[0], EditorDlgProc);
				SetWindowPos(m_phEditors[i], HWND_TOP, rcDisplay.left, rcDisplay.top, 0, 0, SWP_NOSIZE);
			}
			//ShowWindow(m_phEditors[XPET__LAST - 1], SW_SHOW);
			break;
		}
	case WM_CLOSE:
		ShowWindow(hWnd, SW_HIDE);
		break;
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case TCN_SELCHANGE:
			onSelChanged();
			break;

		case LVN_ITEMCHANGED:
			if(((LPNMHDR)lParam)->idFrom == IDC_LIST1)
			{
				onPropListChanged();
			}
			break;
		}
		break;

	case WM_COMMAND:
		switch(HIWORD(wParam))
		{
		case CBN_EDITCHANGE:
			if(1)
			{
				int iLen = ComboBox_GetTextLength(m_hClassListWnd) + 1;
				char *szTemp = (char*)alloca(sizeof(char)* iLen);
				ComboBox_GetText(m_hClassListWnd, szTemp, iLen);
				filterClassList(szTemp);
			}
			break;

		case EN_KILLFOCUS:
			if(LOWORD(wParam) == IDC_OPE_TEXT || LOWORD(wParam) == IDC_OPE_FILE)
			{
				HWND hEdit = (HWND)lParam;
				int iValLen = Edit_GetTextLength(hEdit) + 1;
				char *szValue = (char*)alloca(sizeof(char) * iValLen);
				Edit_GetText(hEdit, szValue, iValLen);

				int iSel = ListView_GetNextItem(m_hPropListWnd, -1, LVNI_SELECTED);
				LVITEM lvItem;
				memset(&lvItem, 0, sizeof(lvItem));
				lvItem.iItem = iSel;
				lvItem.mask = LVIF_PARAM;
				ListView_GetItem(m_hPropListWnd, &lvItem);
				prop_s *pField = &m_aPropFields[AAString((char*)lvItem.lParam)];

				setPropFieldValue(pField->field.szKey, szValue);
				if(m_pCallback)
				{
					m_pCallback->onPropertyChanged(pField->field.szKey, szValue);
				}
			}
			break;
		}
		break;

	default:
		return(FALSE);
	}
	return(TRUE);
}

INT_PTR CALLBACK CPropertyWindow::ClassListWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CPropertyWindow *pThis = (CPropertyWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	switch(message)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
		case VK_RETURN:
			ComboBox_ShowDropdown(GetParent(hWnd), FALSE);
			SetFocus(pThis->m_hDlgWnd);
			break;
		}
		break;
	}

	return(CallWindowProc(pThis->m_pfnClassListOldWndproc, hWnd, message, wParam, lParam));
}

void CPropertyWindow::onPropListChanged()
{
	if(ListView_GetSelectedCount(m_hPropListWnd))
	{
		int iSel = ListView_GetNextItem(m_hPropListWnd, -1, LVNI_SELECTED);
		LVITEM lvItem;
		memset(&lvItem, 0, sizeof(lvItem));
		lvItem.iItem = iSel;
		lvItem.mask = LVIF_PARAM;
		ListView_GetItem(m_hPropListWnd, &lvItem);
		prop_s *pField = &m_aPropFields[AAString((char*)lvItem.lParam)];
		
		SetDlgItemTextW(m_hPropTabs[0], IDC_OP_HELP, pField->field.szHelp ? StringW(String(pField->field.szHelp)).c_str() : L"");

		initEditor(pField->field.editorType, pField->field.pEditorData, pField->sValue.c_str());
	}
}

void CPropertyWindow::onSelChanged()
{
	int iSel = TabCtrl_GetCurSel(m_hTabControl);

	if(m_hCurrentTab != NULL)
	{
		ShowWindow(m_hCurrentTab, FALSE);
	}

	m_hCurrentTab = m_hPropTabs[iSel];
	if(m_hCurrentTab)
	{
		ShowWindow(m_hCurrentTab, TRUE);
	}
}

void CPropertyWindow::clearClassList()
{
	m_asClassList.clearFast();
	ComboBox_Enable(m_hClassListWnd, FALSE);
	ComboBox_ResetContent(m_hClassListWnd);

	ShowWindow(m_hTabControl, SW_HIDE);
	ShowWindow(m_hNoPropsText, SW_SHOW);
	if(m_hCurrentTab)
	{
		ShowWindow(m_hCurrentTab, SW_HIDE);
	}
}
void CPropertyWindow::addClass(const char *szClassName)
{
	m_asClassList.push_back(szClassName);
	ComboBox_AddString(m_hClassListWnd, szClassName);
	int iCount = ComboBox_GetCount(m_hClassListWnd);
	if(iCount == 1)
	{
		ComboBox_SetCurSel(m_hClassListWnd, 0);
	}
	ComboBox_Enable(m_hClassListWnd, iCount > 1);
	ShowWindow(m_hTabControl, SW_SHOW);
	ShowWindow(m_hNoPropsText, SW_HIDE);
	if(m_hCurrentTab)
	{
		ShowWindow(m_hCurrentTab, SW_SHOW);
	}
}

void CPropertyWindow::filterClassList(const char *szFilter)
{
	int iLen = ComboBox_GetTextLength(m_hClassListWnd) + 1;
	char *szTemp = (char*)alloca(sizeof(char)* iLen);
	ComboBox_GetText(m_hClassListWnd, szTemp, iLen);

	DWORD dwRange = SendMessage(m_hClassListWnd, CB_GETEDITSEL, NULL, NULL);

	ComboBox_ShowDropdown(m_hClassListWnd, TRUE);
	SendMessage(m_hClassListWnd, WM_SETCURSOR, 0, 0);
	ComboBox_ResetContent(m_hClassListWnd);
	for(UINT i = 0, l = m_asClassList.size(); i < l; ++i)
	{
		if(!szFilter[0] || strstr(m_asClassList[i].c_str(), szFilter))
		{
			ComboBox_AddString(m_hClassListWnd, m_asClassList[i].c_str());
		}
	}
	ComboBox_SetCurSel(m_hClassListWnd, 0);
	ComboBox_SetText(m_hClassListWnd, szTemp);
	SendMessage(m_hClassListWnd, CB_SETEDITSEL, NULL, dwRange);
}

void CPropertyWindow::clearProps()
{
	ListView_DeleteAllItems(m_hPropListWnd);
	m_aPropFields.clear();
}
void CPropertyWindow::addPropField(const X_PROP_FIELD *pField, const char *szValue)
{
	m_aPropFields[AAString(pField->szKey)] = {*pField, szValue ? szValue : ""};
	LVITEMA lvItem;
	memset(&lvItem, 0, sizeof(lvItem));
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.pszText = (LPSTR)pField->szName;
	lvItem.cchTextMax = strlen(lvItem.pszText);
	lvItem.lParam = (LPARAM)pField->szKey;
	ListView_InsertItem(m_hPropListWnd, &lvItem);

	if(szValue)
	{
		setPropFieldValue(pField->szKey, szValue);
	}

}
void CPropertyWindow::setPropFieldValue(const char *szKey, const char *szValue)
{
	prop_s *pField = &m_aPropFields[AAString(szKey)];
	pField->sValue = szValue;

	LVFINDINFOA lvFindInfo;
	memset(&lvFindInfo, 0, sizeof(lvFindInfo));
	lvFindInfo.flags = LVFI_PARAM;
	lvFindInfo.lParam = (LPARAM)szKey;
	int iID = ListView_FindItem(m_hPropListWnd, -1, &lvFindInfo);

	ListView_SetItemText(m_hPropListWnd, iID, 1, (LPSTR)pField->sValue.c_str());

	onPropListChanged();
}

void CPropertyWindow::initEditor(X_PROP_EDITOR_TYPE type, const void *pData, const char *szValue)
{
	if(m_editorActive != XPET__LAST)
	{
		ShowWindow(m_phEditors[m_editorActive], SW_HIDE);
	}
	m_editorActive = type;
	HWND hEditorDlg = m_phEditors[m_editorActive];
	ShowWindow(hEditorDlg, SW_SHOW);

	switch(type)
	{
	case XPET_TEXT:
		SetDlgItemText(hEditorDlg, IDC_OPE_TEXT, szValue);
		break;
	case XPET_FILE:
		SetDlgItemText(hEditorDlg, IDC_OPE_FILE, szValue);
		break;
	case XPET_COMBO:
		if(1)
		{
			HWND hCombo = GetDlgItem(hEditorDlg, IDC_OPE_COMBO);
			ComboBox_ResetContent(hCombo);
			edt_kv *pKV = (edt_kv*)pData;
			while(pKV->szKey)
			{
				ComboBox_AddString(hCombo, pKV->szKey);
				ComboBox_SetItemData(hCombo, ComboBox_GetCount(hCombo) - 1, pKV->szValue);
				if(!fstrcmp(pKV->szValue, szValue))
				{
					ComboBox_SetCurSel(hCombo, ComboBox_GetCount(hCombo) - 1);
				}
				++pKV;
			}
		}
		break;
	}
}
