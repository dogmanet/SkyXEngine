#include "EffectBrowserWindow.h"
#include "resource.h"

#include <windowsx.h>

#if 0
BOOL EnumLevels(CLevelInfo *pInfo)
{
	pInfo->m_szName[0] = 0;
	pInfo->m_szLocalName[0] = 0;
	pInfo->m_szLastEdit[0] = 0;

	bool bFound = false;
	const char *szDirName = NULL;
	if(!pInfo->m_pIterator)
	{
		pInfo->m_pIterator = Core_GetIXCore()->getFileSystem()->getFolderList("levels/");
	}
	if(pInfo->m_pIterator && (szDirName = pInfo->m_pIterator->next()))
	{
		bFound = true;
	}

	if(!bFound)
	{
		mem_release(pInfo->m_pIterator);
		return(FALSE);
	}

	const char *szLevelName = basename(szDirName);
	strncpy(pInfo->m_szName, szLevelName, MAX_LEVEL_STRING - 1);

	{
		char szFullPath[1024], szFSPath[1024];
		sprintf(szFullPath, "levels/%s/%s.lvl", pInfo->m_szName, pInfo->m_szName);

		if(Core_GetIXCore()->getFileSystem()->resolvePath(szFullPath, szFSPath, sizeof(szFSPath)))
		{
			GetLastWriteTime(szFSPath, pInfo->m_szLastEdit);


			ISXConfig *pConfig = Core_OpConfig(szFSPath);
			if(pConfig->keyExists("level", "local_name"))
			{
				strncpy(pInfo->m_szLocalName, pConfig->getKey("level", "local_name"), MAX_LEVEL_STRING - 1);
			}
			else
			{
				strncpy(pInfo->m_szLocalName, pInfo->m_szName, MAX_LEVEL_STRING - 1);
			}

			if(pConfig->keyExists("level", "type"))
			{
				pInfo->m_isIndoor = !!strcasecmp(pConfig->getKey("level", "type"), "outdoor");
			}
			else
			{
				pInfo->m_isIndoor = true;
			}
			mem_release(pConfig);
		}

		//sprintf(szFullPath, "%s%s/preview.bmp", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), pInfo->m_szName);
		//pInfo->m_bHasPreview = FileExistsFile(szFullPath);
	}

	return(TRUE);
}
#endif

CEffectBrowserWindow::CEffectBrowserWindow(HINSTANCE hInstance, HWND hMainWnd, IFileSystem *pFS):
	m_hInstance(hInstance),
	m_hMainWnd(hMainWnd),
	m_pFS(pFS)
{
	registerClass();

	CreateDialogParamA(hInstance, MAKEINTRESOURCE(IDD_EFFECT_BROWSER), hMainWnd, DlgProc, (LPARAM)this);
	ShowWindow(m_hDlgWnd, SW_SHOW);
}

CEffectBrowserWindow::~CEffectBrowserWindow()
{
	DestroyWindow(m_hDlgWnd);
}

INT_PTR CALLBACK CEffectBrowserWindow::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CEffectBrowserWindow *pThis = (CEffectBrowserWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(msg == WM_INITDIALOG)
	{
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)lParam);
		pThis = (CEffectBrowserWindow*)lParam;
		pThis->m_hDlgWnd = hWnd;
	}
	if(pThis)
	{
		return(pThis->dlgProc(hWnd, msg, wParam, lParam));
	}
	return(FALSE);
}

INT_PTR CALLBACK CEffectBrowserWindow::dlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			m_hOkButtonWnd = GetDlgItem(m_hDlgWnd, IDOK);
			Button_Enable(m_hOkButtonWnd, FALSE);

			m_hTreeWnd = GetDlgItem(m_hDlgWnd, IDC_TREE);

			createTree();
			


			//IFileIterator *pIter = m_pFS->getFileList("effects/", "eff");

#if 0
			m_hListWnd = GetDlgItem(m_hDlgWnd, IDC_LIST1);
			ListView_SetExtendedListViewStyle(m_hListWnd, ListView_GetExtendedListViewStyle(m_hListWnd) | LVS_EX_FULLROWSELECT);

			LV_COLUMNA lvColumn;
			memset(&lvColumn, 0, sizeof(lvColumn));

			RECT rc;
			GetClientRect(m_hListWnd, &rc);

			lvColumn.mask = LVCF_WIDTH | LVCF_TEXT;
			lvColumn.cx = (rc.right - rc.left) / 4;
			lvColumn.pszText = "Name";
			lvColumn.cchTextMax = strlen(lvColumn.pszText);
			ListView_InsertColumn(m_hListWnd, 0, &lvColumn);

			lvColumn.pszText = "Local name";
			lvColumn.cchTextMax = strlen(lvColumn.pszText);
			ListView_InsertColumn(m_hListWnd, 1, &lvColumn);

			lvColumn.pszText = "Last edit";
			lvColumn.cchTextMax = strlen(lvColumn.pszText);
			ListView_InsertColumn(m_hListWnd, 2, &lvColumn);

			lvColumn.pszText = "Type";
			lvColumn.cchTextMax = strlen(lvColumn.pszText);
			ListView_InsertColumn(m_hListWnd, 3, &lvColumn);

			loadLevels();
#endif
			break;
		}
	case WM_NOTIFY:
		switch(((LPNMHDR)lParam)->code)
		{
		case LVN_ITEMCHANGED:
#if 0
			if(((LPNMHDR)lParam)->idFrom == IDC_LIST1)
			{
				int iSel = ListView_GetNextItem(m_hListWnd, -1, LVNI_SELECTED);
				Button_Enable(m_hOkButtonWnd, iSel >= 0);
			}
#endif
			break;
		case NM_DBLCLK:
			{
				LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE)lParam;
				if(lpnmitem->iItem >= 0)
				{
					SendMessage(m_hDlgWnd, WM_COMMAND, MAKEWPARAM(IDOK, 0), NULL);
				}
			}
			break;
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
#if 0
				int iSel = ListView_GetNextItem(m_hListWnd, -1, LVNI_SELECTED);
				LVITEMW lvItem;
				memset(&lvItem, 0, sizeof(lvItem));
				lvItem.iItem = iSel;
				lvItem.mask = LVIF_TEXT;
				WCHAR tmp[MAX_LEVEL_STRING];
				tmp[0] = 0;
				lvItem.pszText = tmp;
				lvItem.cchTextMax = MAX_LEVEL_STRING;
				//ListView_GetItem(m_hListWnd, &lvItem);
				SNDMSG(m_hListWnd, LVM_GETITEMW, 0, (LPARAM)(LV_ITEMW*)(&lvItem));
				strcpy(m_szLevelName, CWC2MB(tmp));
#endif
				//EndDialog(m_hDlgWnd, 1);
			}
			break;
		case IDCANCEL:
			//EndDialog(m_hDlgWnd, 0);
			break;
		}
		
	default:
		return(FALSE);
	}
	return(TRUE);
}

#if 0
const char *CEffectBrowserWindow::getLevelName()
{
	INT_PTR res = DialogBoxParamA(m_hInstance, MAKEINTRESOURCE(IDD_LEVEL_OPEN), m_hMainWnd, DlgProc, (LPARAM)this);
	if(!res || res == -1)
	{
		return(NULL);
	}

	return(m_szLevelName);
}

void CEffectBrowserWindow::loadLevels()
{
	CLevelInfo levelInfo;

	LVITEMW lvItem;
	memset(&lvItem, 0, sizeof(lvItem));
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	
	LVFINDINFOA lvFindInfo;
	memset(&lvFindInfo, 0, sizeof(lvFindInfo));
	lvFindInfo.flags = LVFI_PARAM;

	UINT uIndex = 0;

	while(EnumLevels(&levelInfo))
	{
		StringW tmp2(CMB2WC(levelInfo.m_szName));

		lvItem.pszText = (LPWSTR)tmp2.c_str();
		lvItem.cchTextMax = tmp2.length();
		lvItem.lParam = (LPARAM)uIndex;
		SNDMSG(m_hListWnd, LVM_INSERTITEMW, 0, (LPARAM)(const LV_ITEMW *)(&lvItem));

		
		lvFindInfo.lParam = (LPARAM)uIndex;
		int iID = ListView_FindItem(m_hListWnd, -1, &lvFindInfo);

 		// ListView_SetItemText(m_hListWnd, iID, 1, (LPSTR)levelInfo.m_szLocalName);
		ListView_SetItemText(m_hListWnd, iID, 2, (LPSTR)levelInfo.m_szLastEdit);
		ListView_SetItemText(m_hListWnd, iID, 3, (LPSTR)(levelInfo.m_isIndoor ? "Indoor" : "Outdoor"));


		StringW tmp(CMB2WC(levelInfo.m_szLocalName));
		LV_ITEMW _macro_lvi;
		_macro_lvi.iSubItem = 1;
		_macro_lvi.pszText = (LPWSTR)tmp.c_str();
		SNDMSG(m_hListWnd, LVM_SETITEMTEXTW, (WPARAM)iID, (LPARAM)(LV_ITEM *)&_macro_lvi);
		
		++uIndex;
	}
}

#endif


void CEffectBrowserWindow::registerClass()
{
	WNDCLASSEX wcex;
	HBRUSH hBrush = NULL;

	// Resetting the structure members before use
	memset(&wcex, 0, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	//wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON_LOGO));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

	hBrush = CreateSolidBrush(RGB(0, 0, 0));


	wcex.hbrBackground = (HBRUSH)hBrush;
	wcex.lpszClassName = "XEffectBrowser";
	//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if(!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Unable to register window class!", "Error", MB_OK | MB_ICONSTOP);
	}
}

LRESULT CALLBACK CEffectBrowserWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CEffectBrowserWindow *pThis = (CEffectBrowserWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(pThis)
	{
		return(pThis->wndProc(hWnd, msg, wParam, lParam));
	}
	return(DefWindowProc(hWnd, msg, wParam, lParam));
}

LRESULT CALLBACK CEffectBrowserWindow::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int xPos;
	switch(msg)
	{
#if 0
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
		xPos = GET_X_LPARAM(lParam);

		SetFocus(hWnd);

		if(m_uPanelWidth - xPos <= m_pScrollBar->getWidth())
		{
			SetCapture(hWnd);
			m_isTrackingScroll = true;
			m_pScrollBar->onMouseDown(GET_Y_LPARAM(lParam));
		}
		else
		{
			if(msg == WM_LBUTTONDOWN)
			{
				int yPos = GET_Y_LPARAM(lParam);

				yPos += m_iScrollPos;

				for(UINT i = 0, l = m_aMaterials.size(); i < l; ++i)
				{
					MaterialItem &item = m_aMaterials[i];
					if(xPos > item.uXpos && yPos > item.uYpos && xPos <= item.uXpos + m_frameSize && yPos <= item.uYpos + m_frameSize + 20)
					{
						m_uSelectedItem = i;
						UINT uTexW = 0, uTexH = 0;

						if(item.pTexture)
						{
							uTexW = item.pTexture->getWidth();
							uTexH = item.pTexture->getHeight();
						}

						char tmp[64];
						sprintf(tmp, "%ux%u", uTexW, uTexH);

						SetDlgItemText(m_hDlgWnd, IDC_MAT_SIZE, tmp);
						SetDlgItemTextW(m_hDlgWnd, IDC_MAT_NAME, CMB2WC(item.sName.c_str()));

						break;
					}
				}
			}
			else if(m_uSelectedItem != ~0)
			{
				if(m_pCallback)
				{
					m_pCallback->onSelected(m_aMaterials[m_uSelectedItem].sName.c_str());
					m_pCallback = NULL;
				}
				ShowWindow(m_hDlgWnd, SW_HIDE);
			}
		}

		break;
	case WM_LBUTTONUP:
		if(m_isTrackingScroll)
		{
			m_isTrackingScroll = false;
			m_pScrollBar->onMouseUp();
			ReleaseCapture();
		}
		break;
	case WM_MOUSEMOVE:
	{
		xPos = GET_X_LPARAM(lParam);
		bool isHover = m_isTrackingScroll || (m_uPanelWidth - xPos <= m_pScrollBar->getWidth());
		if(isHover != m_isHoverScroll)
		{
			m_isHoverScroll = isHover;
			m_pScrollBar->onMouseHover(isHover);
		}
		if(isHover)
		{
			m_pScrollBar->onMouseMove(GET_Y_LPARAM(lParam));

			if(!m_isTrackingScroll && !m_isTrackingLeaveEvent)
			{
				TRACKMOUSEEVENT tme = {};
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = hWnd;
				tme.dwFlags = TME_LEAVE;
				m_isTrackingLeaveEvent = TrackMouseEvent(&tme);
			}
		}
	}
	break;

	case WM_MOUSELEAVE:
		m_isTrackingLeaveEvent = false;
		if(m_isHoverScroll)
		{
			m_isHoverScroll = false;
			m_pScrollBar->onMouseHover(false);
		}
		break;

	case WM_PAINT:
		m_isDirty = true;
		// NO BREAK!
#endif
	default:
		return(DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return(0);
}

void CEffectBrowserWindow::createTree()
{
	TreeView_DeleteAllItems(m_hTreeWnd);

	createTreeLevel("effects", TVI_ROOT);
}

static HTREEITEM InsertItem(HWND hWnd, const wchar_t *wszText, TVINSERTSTRUCTW &tvins)
{
	tvins.item.pszText = (LPWSTR)wszText;
	tvins.item.cchTextMax = wcslen(tvins.item.pszText);

	return((HTREEITEM)SNDMSG((hWnd), TVM_INSERTITEMW, 0, (LPARAM)(LPTV_INSERTSTRUCTW)(&tvins)));
}

void CEffectBrowserWindow::createTreeLevel(const char *szCurrentFolder, HTREEITEM hParent)
{
	TVINSERTSTRUCTW tvins;

	tvins.item.mask = TVIF_TEXT;

	tvins.hInsertAfter = TVI_LAST;

	tvins.hParent = hParent;

	HTREEITEM hCurrent = InsertItem(m_hTreeWnd, CMB2WC(basename(szCurrentFolder)), tvins);

	IFileIterator *pIter = m_pFS->getFolderList(szCurrentFolder);
	if(pIter)
	{
		const char *szDirName;
		while((szDirName = pIter->next()))
		{
			createTreeLevel(szDirName, hCurrent);
		}
		mem_release(pIter);
	}

	pIter = m_pFS->getFileList(szCurrentFolder, "eff");
	if(pIter)
	{
		tvins.hParent = hCurrent;
		const char *szFileName;
		while((szFileName = pIter->next()))
		{
			InsertItem(m_hTreeWnd, CMB2WC(basename(szFileName)), tvins);
		}
		mem_release(pIter);
	}
}
