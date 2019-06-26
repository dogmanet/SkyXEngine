

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <windowsx.h>

#include "resource.h"
#include <commctrl.h>
#include <combaseapi.h>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include "LevelOpenDialog.h"

#include <uxtheme.h>
#pragma comment(lib, "UxTheme.lib")

#include <skyxengine.h>
#include <core/sxcore.h>
#include <gcore/sxgcore.h>
#include <render/sxrender.h>
#include <input/sxinput.h>
//#include <sxguiwinapi/sxgui.h>
//#include <level/sxlevel.h>

#include "terrax.h"
#include <xcommon/editor/IXEditorObject.h>
#include <xcommon/editor/IXEditable.h>
#include "UndoManager.h"

#include "CommandSelect.h"
#include "CommandMove.h"
#include "CommandScale.h"
#include "CommandRotate.h"
#include "CommandDelete.h"
#include "CommandCreate.h"
#include "CommandProperties.h"

#include "PropertyWindow.h"

#include <gui/guimain.h>

extern Array<IXEditorObject*> g_pLevelObjects;
extern AssotiativeArray<AAString, IXEditable*> g_mEditableSystems;

// Global Variables:
HINSTANCE hInst;								// current instance
HWND g_hWndMain = NULL;
HWND g_hTopRightWnd = NULL;
HWND g_hTopLeftWnd = NULL;
HWND g_hBottomRightWnd = NULL;
HWND g_hBottomLeftWnd = NULL;
HWND g_hStatusWnd = NULL;
HWND g_hObjectTreeWnd = NULL;
HWND g_hComboTypesWnd = NULL;
HWND g_hStaticTypesWnd = NULL;
HWND g_hComboClassesWnd = NULL;
HWND g_hStaticClassesWnd = NULL;

HWND g_hABArrowButton = NULL;
HWND g_hABCameraButton = NULL;
HWND g_hABCreateButton = NULL;

BOOL g_isXResizeable = TRUE;
BOOL g_isYResizeable = TRUE;

BOOL g_is3DRotating = FALSE;
BOOL g_is3DPanning = FALSE;

BOOL g_isPropWindowVisible = FALSE;
CPropertyWindow *g_pPropWindow = NULL;

extern HACCEL g_hAccelTable;

HMENU g_hMenu = NULL;

HWND g_pGuiWnd = NULL;
gui::IGUI *g_pGUI = NULL;

CTerraXConfig g_xConfig;
CTerraXState g_xState;

extern CUndoManager *g_pUndoManager;

extern Array<IXEditable*> g_pEditableSystems;

extern String g_sLevelName;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK GuiWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK RenderWndProc(HWND, UINT, WPARAM, LPARAM);
void DisplayContextMenu(HWND hwnd, POINT pt, int iMenu, int iSubmenu, int iCheckItem = -1);
void XInitViewportLayout(X_VIEWPORT_LAYOUT layout);
BOOL XCheckMenuItem(HMENU hMenu, UINT uIDCheckItem, bool bCheck);
void XUpdateStatusGrid();
void XUpdateStatusMPos();
void XUpdateUndoRedo();

class CPropertyCallback: public CPropertyWindow::ICallback
{
public:
	void onClassChanged(const char *szNewClassName)
	{

	}
	bool onPropertyChanged(const char *szKey, const char *szValue)
	{
		if(!m_pPropsCmd)
		{
			m_pPropsCmd = new CCommandProperties(); 
			for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
			{
				IXEditorObject *pObj = g_pLevelObjects[i];
				if(pObj->isSelected())
				{
					m_pPropsCmd->addObject(i);
				}
			}
		}

		m_pPropsCmd->setKV(szKey, szValue);
		return(true);
	}
	void onCancel()
	{
		if(m_pPropsCmd)
		{
			m_pPropsCmd->undo();
			mem_delete(m_pPropsCmd);
		}
	}
	void onApply()
	{
		if(m_pPropsCmd)
		{
			CCommandProperties *pPropsCmd = m_pPropsCmd;
			m_pPropsCmd = NULL;
			g_pUndoManager->execCommand(pPropsCmd);
		}
	}
protected:
	CCommandProperties *m_pPropsCmd = NULL;
};

CPropertyCallback g_propertyCallback;

ATOM XRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	HBRUSH	hBrush = NULL;

	// Resetting the structure members before use
	memset(&wcex, 0, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLITTER_WND));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

	hBrush = CreateSolidBrush(RGB(240, 240, 240));


	wcex.hbrBackground = (HBRUSH)hBrush;
	//	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_SPLITTER_WND);
	wcex.lpszClassName = MAIN_WINDOW_CLASS;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if(!RegisterClassEx(&wcex))
	{
		return(FALSE);
	}

	// Intilaizing the class for the viewport window
	wcex.lpfnWndProc = RenderWndProc;
	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	wcex.hbrBackground = (HBRUSH)hBrush;
	wcex.lpszClassName = RENDER_WINDOW_CLASS;

	if(!RegisterClassEx(&wcex))
	{
		return(FALSE);
	}


	
	//DeleteObject(hBrush);


	// Intilaizing the class for the gui window
	wcex.lpfnWndProc = GuiWndProc;
	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	wcex.hbrBackground = (HBRUSH)hBrush;
	wcex.lpszClassName = GUI_WINDOW_CLASS;

	if(!RegisterClassEx(&wcex))
	{
		return(FALSE);
	}

	return(TRUE);
}

void XInitGuiWindow(bool pre)
{
	if(pre)
	{
		g_pGuiWnd = CreateWindowA(GUI_WINDOW_CLASS, "Material editor", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInst, NULL);
		ShowWindow(g_pGuiWnd, SW_SHOW);
		UpdateWindow(g_pGuiWnd);

		DWM_BLURBEHIND dwmBlur = {0};
		dwmBlur.dwFlags = DWM_BB_ENABLE;
		dwmBlur.fEnable = TRUE;
		
		DwmEnableBlurBehindWindow(g_pGuiWnd, &dwmBlur);
	}
	else
	{
		HMODULE hDLL = LoadLibrary("sxgui"
#ifdef _DEBUG
			"_d"
#endif
			".dll");
		if(!hDLL)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "Unable to load sxgui"
#ifdef _DEBUG
				"_d"
#endif
				".dll");
		}

		gui::PFNINITINSTANCE pfnGUIInit;
		pfnGUIInit = (gui::PFNINITINSTANCE)GetProcAddress(hDLL, "InitInstance");

		if(!pfnGUIInit)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "The procedure entry point InitInstance could not be located in the dynamic link library sxgui.dll");
		}

		g_pGUI = pfnGUIInit(SGCore_GetDXDevice(), "./editor_gui/", g_pGuiWnd);

		g_pGUI->pushDesktop(g_pGUI->createDesktopA("main", "main.html"));
	}
}

void XGuiRender()
{
	g_pGUI->render();
}

BOOL XInitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	// Varible used to postion the window, center to the desktop
	UINT nx_size = GetSystemMetrics(SM_CXSCREEN);
	UINT ny_size = GetSystemMetrics(SM_CYSCREEN);

	UINT nx_pos = (nx_size - WINDOW_WIDTH) / 2;
	UINT ny_pos = (ny_size - WINDOW_HEIGHT) / 2;

	g_hWndMain = CreateWindowA(MAIN_WINDOW_CLASS, MAIN_WINDOW_TITLE " | " SKYXENGINE_VERSION4EDITORS, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPEDWINDOW, nx_pos, ny_pos, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, g_hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1)), hInstance, NULL);

	if(!g_hWndMain)
	{
		UINT ret_val;

		ret_val = GetLastError();
		return FALSE;
	}

	ShowWindow(g_hWndMain, nCmdShow);
//	ShowWindow(g_hWndMain, SW_MAXIMIZE);
	UpdateWindow(g_hWndMain);

	g_hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	XCheckMenuItem(g_hMenu, ID_VIEW_GRID, g_xConfig.m_bShowGrid);

	return TRUE;
}

bool IsEditMessage()
{
	HWND hFocused = GetFocus();
	char className[6];
	GetClassName(hFocused, className, 6);
	return(hFocused && !strcasecmp(className, "edit"));
}

WNDPROC g_pfnTreeOldWndproc;
LRESULT CALLBACK TreeViewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_MOUSEWHEEL)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
		if(pt.x < rc.left || pt.x > rc.right || pt.y < rc.top || pt.y > rc.bottom)
		{
			SendMessage(g_hWndMain, message, wParam, lParam);
			return(0);
		}
	}
	return(CallWindowProc(g_pfnTreeOldWndproc, hWnd, message, wParam, lParam));
}

WNDPROC g_pfnClassesComboOldWndproc;
LRESULT CALLBACK ClassesComboWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_MOUSEWHEEL)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);
		POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
		if(pt.x < rc.left || pt.x > rc.right || pt.y < rc.top || pt.y > rc.bottom)
		{
			SendMessage(g_hWndMain, message, wParam, lParam);
			return(0);
		}
	}
	return(CallWindowProc(g_pfnClassesComboOldWndproc, hWnd, message, wParam, lParam));
}

WNDPROC g_pfnStatusBarOldWndproc;
LRESULT CALLBACK StatusBarWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_SIZE)
	{
		// 200 100 100 150
		// selection, world position, lwh, zoom, grid

		RECT rcClient;
		int iParts = 5;

		// Get the coordinates of the parent window's client area.
		GetClientRect(GetParent(hWnd), &rcClient);

		// Allocate an array for holding the right edge coordinates.
		HLOCAL hloc = LocalAlloc(LHND, sizeof(int) * iParts);
		int *paParts = (int*)LocalLock(hloc);

		// Calculate the right edge coordinate for each part, and
		// copy the coordinates to the array.
		paParts[0] = 0;
		paParts[1] = 200;
		paParts[2] = 100;
		paParts[3] = 100;
		paParts[4] = 150;
		int iTotal = 0;
		for(int i = 0; i < iParts; ++i)
		{
			iTotal += paParts[i];
		}
		paParts[0] = rcClient.right - iTotal;
		for(int i = 1; i < iParts; ++i)
		{
			paParts[i] += paParts[i - 1];
		}

		// Tell the status bar to create the window parts.
		SendMessage(hWnd, SB_SETPARTS, (WPARAM)iParts, (LPARAM)paParts);
		// Free the array, and return.
		LocalUnlock(hloc);
		LocalFree(hloc);
	}
	return(CallWindowProc(g_pfnStatusBarOldWndproc, hWnd, message, wParam, lParam));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;
	static HCURSOR	hcSizeEW = NULL;
	static HCURSOR	hcSizeNS = NULL;
	static HCURSOR	hcSizeNESW = NULL;

	

	static BOOL xSizing;
	static BOOL ySizing;


	static float fCoeffWidth = 0.0f;
	static float fCoeffHeight = 0.0f;
	static int iLeftWidth = 0;
	static int iTopHeight = 0;
	
	IMSG msg;
	msg.lParam = lParam;
	msg.wParam = wParam;
	msg.message = message;

	SSInput_AddMsg(msg);

	switch(message)
	{
	case WM_CREATE:
	{
		hcSizeEW = LoadCursor(NULL, IDC_SIZEWE);
		hcSizeNS = LoadCursor(NULL, IDC_SIZENS);
		hcSizeNESW = LoadCursor(NULL, IDC_SIZEALL);

		//	g_hMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MENU1));
		//	SetMenu(g_hWndMain, g_hMenu); 
		//	DrawMenuBar(g_hWndMain);


		GetClientRect(hWnd, &rect);

		rect.top += MARGIN_TOP;
		rect.bottom -= MARGIN_BOTTOM;
		rect.left += MARGIN_LEFT;
		rect.right -= MARGIN_RIGHT;

		iLeftWidth = (rect.right - rect.left - SPLITTER_BAR_WIDTH) / 2;
		iTopHeight = (rect.bottom - rect.top - SPLITTER_BAR_WIDTH) / 2;

		fCoeffWidth = (float)iLeftWidth / (float)(rect.right - rect.left);
		fCoeffHeight = (float)iTopHeight / (float)(rect.bottom - rect.top);

		// Creates the left window using the width and height read from the XML 
		// files
		g_hTopLeftWnd = CreateWindowExA(WS_EX_CLIENTEDGE, RENDER_WINDOW_CLASS, "", WS_CHILD | WS_VISIBLE | SS_SUNKEN, rect.left, rect.top, iLeftWidth, iTopHeight, hWnd, NULL, hInst, NULL);
		if(g_hTopLeftWnd)
		{
			ShowWindow(g_hTopLeftWnd, SW_SHOW);
			//	UpdateWindow(g_hTopLeftWnd);
		}

		g_hBottomLeftWnd = CreateWindowExA(WS_EX_CLIENTEDGE, RENDER_WINDOW_CLASS, "", WS_CHILD | WS_VISIBLE | SS_SUNKEN, rect.left, rect.top + iTopHeight + SPLITTER_BAR_WIDTH, iLeftWidth, iTopHeight, hWnd, NULL, hInst, NULL);
		if(g_hTopLeftWnd)
		{
			ShowWindow(g_hTopLeftWnd, SW_SHOW);
			//	UpdateWindow(g_hTopLeftWnd);
		}

		g_hTopRightWnd = CreateWindowExA(WS_EX_CLIENTEDGE, RENDER_WINDOW_CLASS, "", WS_CHILD | WS_VISIBLE | SS_SUNKEN, rect.left + iLeftWidth + SPLITTER_BAR_WIDTH, rect.top, iLeftWidth, iTopHeight, hWnd, NULL, hInst, NULL);
		if(g_hTopRightWnd)
		{
			ShowWindow(g_hTopRightWnd, SW_SHOW);
			//	UpdateWindow(g_hTopLeftWnd);
		}

		g_hBottomRightWnd = CreateWindowExA(WS_EX_CLIENTEDGE, RENDER_WINDOW_CLASS, "", WS_CHILD | WS_VISIBLE | SS_SUNKEN, rect.left + iLeftWidth + SPLITTER_BAR_WIDTH, rect.top + iTopHeight + SPLITTER_BAR_WIDTH, iLeftWidth, iTopHeight, hWnd, NULL, hInst, NULL);
		if(g_hBottomRightWnd)
		{
			ShowWindow(g_hBottomRightWnd, SW_SHOW);
			//	UpdateWindow(g_hTopLeftWnd);
		}

		g_hStatusWnd = CreateWindowEx(0, STATUSCLASSNAME, "For help, press F1", SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)IDC_STATUSBAR, hInst, NULL);                   // no window creation data
		{
			g_pfnStatusBarOldWndproc = (WNDPROC)GetWindowLongPtr(g_hStatusWnd, GWLP_WNDPROC);
			SetWindowLongPtr(g_hStatusWnd, GWLP_WNDPROC, (LONG_PTR)StatusBarWndProc);
			SendMessage(g_hStatusWnd, WM_SIZE, 0, 0);
			XUpdateStatusGrid();
		}

		g_hABArrowButton = CreateWindow("Button", "", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_BITMAP | BS_PUSHLIKE | BS_CHECKBOX, rect.left - MARGIN_LEFT, rect.top, MARGIN_LEFT, AB_BUTTON_HEIGHT, hWnd, (HMENU)IDC_AB_ARROW, hInst, NULL);
		{
			SetWindowTheme(g_hABArrowButton, L" ", L" ");
			HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP30));
			SendMessage(g_hABArrowButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
			Button_SetCheck(g_hABArrowButton, TRUE);
		}

		//WS_EX_DLGMODALFRAME

		g_hABCameraButton = CreateWindow("Button", "", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_BITMAP | BS_PUSHLIKE | BS_CHECKBOX, rect.left - MARGIN_LEFT, rect.top + AB_BUTTON_HEIGHT, MARGIN_LEFT, AB_BUTTON_HEIGHT, hWnd, (HMENU)IDC_AB_CAMERA, hInst, NULL);
		{
			SetWindowTheme(g_hABCameraButton, L" ", L" ");
			HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP31));
			SendMessage(g_hABCameraButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		}

		g_hABCreateButton = CreateWindow("Button", "", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_BITMAP | BS_PUSHLIKE | BS_CHECKBOX, rect.left - MARGIN_LEFT, rect.top + AB_BUTTON_HEIGHT * 2, MARGIN_LEFT, AB_BUTTON_HEIGHT, hWnd, (HMENU)IDC_AB_CREATE, hInst, NULL);
		{
			SetWindowTheme(g_hABCreateButton, L" ", L" ");
			HBITMAP hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP34));
			SendMessage(g_hABCreateButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
		}

		g_hObjectTreeWnd = CreateWindowExA(0, WC_TREEVIEW, "", WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS | TVS_DISABLEDRAGDROP | TVS_CHECKBOXES | TVS_NOHSCROLL, rect.right, rect.top, MARGIN_RIGHT, OBJECT_TREE_HEIGHT, hWnd, 0, hInst, NULL);
		{
			g_pfnTreeOldWndproc = (WNDPROC)GetWindowLongPtr(g_hObjectTreeWnd, GWLP_WNDPROC);
			SetWindowLongPtr(g_hObjectTreeWnd, GWLP_WNDPROC, (LONG_PTR)TreeViewWndProc);
		}
		
		g_hStaticTypesWnd = CreateWindowExA(0, WC_STATIC, "Object type", WS_VISIBLE | WS_CHILD, rect.right, rect.top + OBJECT_TREE_HEIGHT, MARGIN_RIGHT, 15, hWnd, 0, hInst, NULL);
		{
			SetWindowFont(g_hStaticTypesWnd, GetStockObject(DEFAULT_GUI_FONT), FALSE);
		}

		g_hComboTypesWnd = CreateWindowExA(0, WC_COMBOBOX, "", WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_SORT | CBS_DROPDOWNLIST | CBS_HASSTRINGS, rect.right, rect.top + OBJECT_TREE_HEIGHT + 15, MARGIN_RIGHT, OBJECT_TREE_HEIGHT, hWnd, (HMENU)IDC_CMB_TYPE, hInst, NULL);
		{
			SetWindowFont(g_hComboTypesWnd, GetStockObject(DEFAULT_GUI_FONT), FALSE);

			//g_pfnClassesComboOldWndproc = (WNDPROC)GetWindowLongPtr(g_hComboTypesWnd, GWLP_WNDPROC);
			SetWindowLongPtr(g_hComboTypesWnd, GWLP_WNDPROC, (LONG_PTR)ClassesComboWndProc);
		}

		g_hStaticClassesWnd = CreateWindowExA(0, WC_STATIC, "Object class", WS_VISIBLE | WS_CHILD, rect.right, rect.top + OBJECT_TREE_HEIGHT + 15 + 25, MARGIN_RIGHT, 15, hWnd, 0, hInst, NULL);
		{
			SetWindowFont(g_hStaticClassesWnd, GetStockObject(DEFAULT_GUI_FONT), FALSE);
		}

		g_hComboClassesWnd = CreateWindowExA(0, WC_COMBOBOX, "", WS_VISIBLE | WS_CHILD | WS_BORDER | CBS_SORT | CBS_DROPDOWNLIST | CBS_HASSTRINGS, rect.right, rect.top + OBJECT_TREE_HEIGHT + 15 + 15 + 25, MARGIN_RIGHT, OBJECT_TREE_HEIGHT, hWnd, (HMENU)IDC_CMB_CLASS, hInst, NULL);
		{
			SetWindowFont(g_hComboClassesWnd, GetStockObject(DEFAULT_GUI_FONT), FALSE);

			g_pfnClassesComboOldWndproc = (WNDPROC)GetWindowLongPtr(g_hComboClassesWnd, GWLP_WNDPROC);
			SetWindowLongPtr(g_hComboClassesWnd, GWLP_WNDPROC, (LONG_PTR)ClassesComboWndProc);
		}

		{
			TV_INSERTSTRUCT tvis;
			memset(&tvis, 0, sizeof(tvis));
			tvis.hParent = TVI_ROOT;
			tvis.hInsertAfter = TVI_LAST;
			tvis.item.mask = TVIF_TEXT;
			tvis.item.pszText = "Test item";
			tvis.item.cchTextMax = lstrlen(tvis.item.pszText);
			HTREEITEM hItem = TreeView_InsertItem(g_hObjectTreeWnd, &tvis);

			tvis.hParent = hItem;
			tvis.item.pszText = "Object 1";
			tvis.item.cchTextMax = lstrlen(tvis.item.pszText);
			TreeView_InsertItem(g_hObjectTreeWnd, &tvis);
			tvis.item.pszText = "Object 2";
			tvis.item.cchTextMax = lstrlen(tvis.item.pszText);
			TreeView_InsertItem(g_hObjectTreeWnd, &tvis);
		}

		return FALSE;
	}
		break;

	case WM_ENTERSIZEMOVE:
	{
		GetClientRect(hWnd, &rect);

		rect.top += MARGIN_TOP;
		rect.bottom -= MARGIN_BOTTOM;
		rect.left += MARGIN_LEFT;
		rect.right -= MARGIN_RIGHT;

		fCoeffWidth = (float)iLeftWidth / (float)(rect.right - rect.left);
		fCoeffHeight = (float)iTopHeight / (float)(rect.bottom - rect.top);
	}
		break;

	case WM_SIZE:
	{
		if(g_isPropWindowVisible)
		{
			if(wParam == SIZE_MINIMIZED)
			{
				g_isPropWindowVisible = g_pPropWindow->isVisible();
				g_pPropWindow->hide();
			}
			else if(wParam == SIZE_RESTORED)
			{
				g_pPropWindow->show();
			}
		}
		GetClientRect(hWnd, &rect);

		rect.top += MARGIN_TOP;
		rect.bottom -= MARGIN_BOTTOM;
		rect.left += MARGIN_LEFT;
		rect.right -= MARGIN_RIGHT;

		if(!(xSizing || ySizing))
		{
			RECT rcTopLeft, rcBottomRight;
			GetClientRect(g_hTopLeftWnd, &rcTopLeft);
			GetClientRect(g_hBottomRightWnd, &rcBottomRight);

			UINT uLeftWidth = rcTopLeft.right - rcTopLeft.left;
			UINT uRightWidth = rcBottomRight.right - rcBottomRight.left;
			UINT uTopHeight = rcTopLeft.bottom - rcTopLeft.top;
			UINT uBottomHeight = rcBottomRight.bottom - rcBottomRight.top;

			if(fCoeffHeight && uTopHeight + uBottomHeight + SPLITTER_BAR_WIDTH + WIDTH_ADJUST * 4 != rect.bottom - rect.top)
			{
				iTopHeight = (UINT)(fCoeffHeight * (float)(rect.bottom - rect.top));
			}
			if(fCoeffWidth && uLeftWidth + uRightWidth + SPLITTER_BAR_WIDTH + WIDTH_ADJUST * 4 != rect.right - rect.left)
			{
				iLeftWidth = (UINT)(fCoeffWidth * (float)(rect.right - rect.left));
			}
		}
		MoveWindow(g_hTopLeftWnd, rect.left, rect.top, iLeftWidth, iTopHeight, FALSE);
		MoveWindow(g_hBottomLeftWnd, rect.left, rect.top + iTopHeight + SPLITTER_BAR_WIDTH, iLeftWidth, rect.bottom - rect.top - SPLITTER_BAR_WIDTH - iTopHeight, FALSE);
		MoveWindow(g_hTopRightWnd, rect.left + iLeftWidth + SPLITTER_BAR_WIDTH, rect.top, rect.right - rect.left - iLeftWidth - SPLITTER_BAR_WIDTH, iTopHeight, FALSE);
		MoveWindow(g_hBottomRightWnd, rect.left + iLeftWidth + SPLITTER_BAR_WIDTH, rect.top + iTopHeight + SPLITTER_BAR_WIDTH, rect.right - rect.left - iLeftWidth - SPLITTER_BAR_WIDTH, rect.bottom - rect.top - SPLITTER_BAR_WIDTH - iTopHeight, FALSE);
		MoveWindow(g_hObjectTreeWnd, rect.right, rect.top, MARGIN_RIGHT, OBJECT_TREE_HEIGHT, FALSE);

		MoveWindow(g_hStaticTypesWnd, rect.right, rect.top + OBJECT_TREE_HEIGHT, MARGIN_RIGHT, 15, FALSE);
		MoveWindow(g_hComboTypesWnd, rect.right, rect.top + OBJECT_TREE_HEIGHT + 15, MARGIN_RIGHT, OBJECT_TREE_HEIGHT, FALSE);
		MoveWindow(g_hStaticClassesWnd, rect.right, rect.top + OBJECT_TREE_HEIGHT + 15 + 25, MARGIN_RIGHT, 15, FALSE);
		MoveWindow(g_hComboClassesWnd, rect.right, rect.top + OBJECT_TREE_HEIGHT + 15 + 15 + 25, MARGIN_RIGHT, OBJECT_TREE_HEIGHT, FALSE);

		InvalidateRect(hWnd, &rect, TRUE);

		if(g_pEngine)
		{
			RECT rcTopLeft;
			GetClientRect(g_hTopLeftWnd, &rcTopLeft);
			g_pEngine->getCore()->execCmd2("r_win_width %d\nr_win_height %d", rcTopLeft.right - rcTopLeft.left, rcTopLeft.bottom - rcTopLeft.top);
		}

		SendMessage(g_hStatusWnd, WM_SIZE, wParam, lParam);
	}
		break;

	case WM_INITMENU:
		if(IsEditMessage())	
		{
			HMENU hMenu = (HMENU)wParam;
			EnableMenuItem(hMenu, ID_EDIT_CUT, MF_ENABLED);
			EnableMenuItem(hMenu, ID_EDIT_COPY, MF_ENABLED);
			EnableMenuItem(hMenu, ID_EDIT_PASTE, MF_ENABLED);
			EnableMenuItem(hMenu, ID_EDIT_DELETE, MF_ENABLED);
			EnableMenuItem(hMenu, ID_EDIT_UNDO, MF_ENABLED);
			EnableMenuItem(hMenu, ID_EDIT_REDO, MF_ENABLED);
		}
		XUpdateUndoRedo();

		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_FILE_OPEN:
		{
			if(g_pUndoManager->isDirty())
			{
				const char *szLevelName = g_sLevelName.c_str();
				char szPrompt[128];
				if(szLevelName[0])
				{
					sprintf_s(szPrompt, "Save changes to %s?", szLevelName);
				}
				UINT mb = MessageBoxA(hWnd, szLevelName[0] ? szPrompt : "Save changes?", MAIN_WINDOW_TITLE, MB_YESNOCANCEL);
				if(mb == IDYES)
				{
					if(!XSaveLevel())
					{
						break;
					}
				}
				else if(mb == IDCANCEL)
				{
					break;
				}
			}
			char szSelName[MAX_PATH];
			char szSelPath[1024];
			szSelName[0] = szSelPath[0] = 0;
			{
				CLevelOpenDialog dlg(hInst, g_hWndMain);
				const char *szLevelName = dlg.getLevelName();
				if(szLevelName)
				{
					XLoadLevel(szLevelName);
				}
			}
			break;
		}
		case ID_FILE_NEW:
		{
			if(g_pUndoManager->isDirty())
			{
				const char *szLevelName = g_sLevelName.c_str();
				char szPrompt[128];
				if(szLevelName[0])
				{
					sprintf_s(szPrompt, "Save changes to %s?", szLevelName);
				}
				UINT mb = MessageBoxA(hWnd, szLevelName[0] ? szPrompt : "Save changes?", MAIN_WINDOW_TITLE, MB_YESNOCANCEL);
				if(mb == IDYES)
				{
					if(!XSaveLevel())
					{
						break;
					}
				}
				else if(mb == IDCANCEL)
				{
					break;
				}
			}

			XResetLevel();
			break;
		}
		
		case ID_FILE_SAVE:
			if(g_pUndoManager->isDirty())
			{
				XSaveLevel();
			}
			break;

		case ID_FILE_SAVEAS:
			//if(g_pUndoManager->isDirty())
			{
				XSaveLevel(NULL, true);
			}
			break;

		case IDC_CTRL_BACK:
			if(HIWORD(wParam) == 1)
			{
				if(IsEditMessage())
				{
					HWND hEdit = GetFocus();
					SendMessage(hEdit, WM_KEYDOWN, VK_CONTROL, 0);
					SendMessage(hEdit, WM_KEYDOWN, VK_SHIFT, 0);
					SendMessage(hEdit, WM_KEYDOWN, VK_LEFT, 0);
					SendMessage(hEdit, WM_KEYUP, VK_LEFT, 0);
					SendMessage(hEdit, WM_KEYUP, VK_SHIFT, 0);
					SendMessage(hEdit, WM_KEYUP, VK_CONTROL, 0);
					SendMessage(hEdit, WM_KEYDOWN, VK_DELETE, 0);
					SendMessage(hEdit, WM_KEYUP, VK_DELETE, 0);
					break;
				}
			}
			break;

		case ID_VIEW_AUTOSIZEVIEWS:

			if(HIWORD(wParam) == 1)
			{
				if(IsEditMessage())
				{
					SendMessage(GetFocus(), EM_SETSEL, 0, -1);
					break;
				}
			}

			GetClientRect(hWnd, &rect);

			rect.top += MARGIN_TOP;
			rect.bottom -= MARGIN_BOTTOM;
			rect.left += MARGIN_LEFT;
			rect.right -= MARGIN_RIGHT;

			if(g_isXResizeable)
			{
				iLeftWidth = (rect.right - rect.left - SPLITTER_BAR_WIDTH) / 2;
			}
			else
			{
				iLeftWidth = rect.right - rect.left;
			}
			if(g_isYResizeable)
			{
				iTopHeight = (rect.bottom - rect.top - SPLITTER_BAR_WIDTH) / 2;
			}
			else
			{
				iTopHeight = rect.bottom - rect.top;
			}

			fCoeffWidth = (float)iLeftWidth / (float)(rect.right - rect.left);
			fCoeffHeight = (float)iTopHeight / (float)(rect.bottom - rect.top);
			SendMessage(hWnd, WM_SIZE, 0, 0);
			break;

		case ID_VEWPORTCONFIGURATION_1X2:
			XInitViewportLayout(XVIEW_1X2);
			break;
		case ID_VEWPORTCONFIGURATION_2X2:
			XInitViewportLayout(XVIEW_2X2);
			break;
		case ID_VEWPORTCONFIGURATION_2X1:
			XInitViewportLayout(XVIEW_2X1);
			break;
		case ID_VEWPORTCONFIGURATION_3DONLY:
			XInitViewportLayout(XVIEW_3DONLY);
			break;

		case IDC_ESCAPE:
			if(IsEditMessage())
			{
				HWND hEdit = GetFocus();
				SendMessage(hEdit, WM_KEYDOWN, VK_ESCAPE, 0);
				SendMessage(hEdit, WM_KEYUP, VK_ESCAPE, 0);
				break;
			}
			//! no break here!
		case IDC_AB_ARROW:
			CheckDlgButton(hWnd, IDC_AB_ARROW, TRUE);
			CheckDlgButton(hWnd, IDC_AB_CAMERA, FALSE);
			CheckDlgButton(hWnd, IDC_AB_CREATE, FALSE);
			g_xState.bCreateMode = false;
			break;
		case IDC_AB_CAMERA:
			CheckDlgButton(hWnd, IDC_AB_ARROW, FALSE);
			CheckDlgButton(hWnd, IDC_AB_CAMERA, TRUE);
			CheckDlgButton(hWnd, IDC_AB_CREATE, FALSE);
			g_xState.bCreateMode = false;
			break;
		case IDC_AB_CREATE:
			CheckDlgButton(hWnd, IDC_AB_ARROW, FALSE);
			CheckDlgButton(hWnd, IDC_AB_CAMERA, FALSE);
			CheckDlgButton(hWnd, IDC_AB_CREATE, TRUE);
			break;

		case ID_GRIDSIZE_SMALLER:
			if(g_xConfig.m_gridStep > GRID_STEP_MINIMAL)
			{
				g_xConfig.m_gridStep = (GRID_STEP)(g_xConfig.m_gridStep - 1);
				XUpdateStatusGrid();
			}
			break;
		case ID_GRIDSIZE_BIGGER:
			if(g_xConfig.m_gridStep < GRID_STEP_MAXIMAL)
			{
				g_xConfig.m_gridStep = (GRID_STEP)(g_xConfig.m_gridStep + 1);
				XUpdateStatusGrid();
			}
			break;
		case ID_VIEW_GRID:
			g_xConfig.m_bShowGrid = !g_xConfig.m_bShowGrid;
			XCheckMenuItem(g_hMenu, ID_VIEW_GRID, g_xConfig.m_bShowGrid);
			break;

		case ID_EDIT_UNDO:
			if(IsEditMessage())
			{
				SendMessage(GetFocus(), EM_UNDO, 0, 0);
				break;
			}
			g_pUndoManager->undo();
			break;

		case ID_EDIT_REDO:
			if(IsEditMessage())
			{
				SendMessage(GetFocus(), EM_UNDO, 0, 0);
				break;
			}
			g_pUndoManager->redo();
			break;

		case ID_EDIT_COPY:
			if(IsEditMessage())
			{
				SendMessage(GetFocus(), WM_COPY, 0, 0);
				break;
			}
			break;

		case ID_EDIT_CUT:
			if(IsEditMessage())
			{
				SendMessage(GetFocus(), WM_CUT, 0, 0);
				break;
			}
			break;

		case ID_EDIT_PASTE:
			if(IsEditMessage())
			{
				SendMessage(GetFocus(), WM_PASTE, 0, 0);
				break;
			}
			break;

		case ID_EDIT_DELETE:
		{
			if(IsEditMessage())
			{
				//SendMessage(GetFocus(), WM_CLEAR, 0, 0);
				SendMessage(GetFocus(), WM_KEYDOWN, VK_DELETE, 0);
				SendMessage(GetFocus(), WM_KEYUP, VK_DELETE, 0);
				break;
			}

			CCommandDelete *pDelCmd = new CCommandDelete();
			for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
			{
				if(g_pLevelObjects[i]->isSelected())
				{
					pDelCmd->addObject(i);
				}
			}
			XExecCommand(pDelCmd);
			break;
		}

		case ID_EDIT_PROPERTIES:
			if(g_pPropWindow)
			{
				g_pPropWindow->show();
			}
			else
			{
				g_pPropWindow = new CPropertyWindow(hInst, g_hWndMain);
				g_pPropWindow->clearClassList();
				g_pPropWindow->setCallback(&g_propertyCallback);
			}
			XUpdatePropWindow();
			g_isPropWindowVisible = TRUE;
			break;

		case IDC_CMB_TYPE:
			{
				int iSel = ComboBox_GetCurSel(g_hComboTypesWnd);
				int iLen = ComboBox_GetLBTextLen(g_hComboTypesWnd, iSel);
				char *szTypeName = (char*)alloca(sizeof(char)* (iLen + 1));
				ComboBox_GetLBText(g_hComboTypesWnd, iSel, szTypeName);

				ComboBox_ResetContent(g_hComboClassesWnd);

				const AssotiativeArray<AAString, IXEditable*>::Node *pNode;
				if(g_mEditableSystems.KeyExists(AAString(szTypeName), &pNode))
				{
					IXEditable *pEditable = *pNode->Val;
					UINT uClassCount = pEditable->getClassCount();
					for(UINT i = 0; i < uClassCount; ++i)
					{
						ComboBox_AddString(g_hComboClassesWnd, pEditable->getClass(i));
					}
					ComboBox_Enable(g_hComboClassesWnd, uClassCount > 1);
					ComboBox_SetCurSel(g_hComboClassesWnd, 0);
				}
			}
			break;

		case IDC_RETURN:
			if(IsEditMessage())
			{
				SendMessage(GetFocus(), WM_KEYDOWN, VK_RETURN, 0);
				SendMessage(GetFocus(), WM_KEYUP, VK_RETURN, 0);
				SendMessage(GetFocus(), WM_CHAR, VK_RETURN, 0);
				break;
			}
			//! No break here!
		case IDC_CTRL_RETURN:
			if(g_xState.bCreateMode)
			{
				int iSel1 = ComboBox_GetCurSel(g_hComboTypesWnd);
				int iLen1 = ComboBox_GetLBTextLen(g_hComboTypesWnd, iSel1);
				char *szTypeName = (char*)alloca(sizeof(char)* (iLen1 + 1));
				ComboBox_GetLBText(g_hComboTypesWnd, iSel1, szTypeName);

				int iSel2 = ComboBox_GetCurSel(g_hComboClassesWnd);
				int iLen2 = ComboBox_GetLBTextLen(g_hComboClassesWnd, iSel2);
				char *szClassName = (char*)alloca(sizeof(char)* (iLen2 + 1));
				ComboBox_GetLBText(g_hComboClassesWnd, iSel2, szClassName);

				if(GetKeyState(VK_CONTROL) >= 0)
				{
					CCommandSelect *pCmdUnselect = new CCommandSelect();
					for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
					{
						IXEditorObject *pObj = g_pLevelObjects[i];
						if(pObj->isSelected())
						{
							pCmdUnselect->addDeselected(i);
						}
					}
					g_pUndoManager->execCommand(pCmdUnselect);
				}

				CCommandCreate *pCmd = new CCommandCreate(g_xState.vCreateOrigin, szTypeName, szClassName);
				g_pUndoManager->execCommand(pCmd);

				g_xState.bCreateMode = false;
			}
			break;

		case ID_FILE_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
	{
		int                 xPos;
		int                 yPos;

		// Varible used to get the mouse cursor x and y co-ordinates
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);

		// Checks whether the mouse is over the splitter window
		xSizing = g_isXResizeable && (xPos > iLeftWidth + MARGIN_LEFT - SPLITTER_BAR_WIDTH && xPos < iLeftWidth + MARGIN_LEFT + SPLITTER_BAR_WIDTH);
		ySizing = g_isYResizeable && (yPos > iTopHeight + MARGIN_TOP - SPLITTER_BAR_WIDTH && yPos < iTopHeight + MARGIN_TOP + SPLITTER_BAR_WIDTH);

		if(xSizing || ySizing)
		{
			// Api to capture mouse input
			SetCapture(hWnd);
			if(xSizing && ySizing)
			{
				SetCursor(hcSizeNESW);
			}
			else if(xSizing)
			{
				SetCursor(hcSizeEW);
			}
			else if(ySizing)
			{
				SetCursor(hcSizeNS);
			}
		}
	}
	break;

	case WM_LBUTTONUP:
		if(xSizing || ySizing)
		{
			RECT    focusrect;
			HDC     hdc;

			// Releases the captured mouse input
			ReleaseCapture();
			// Get the main window dc to draw a focus rectangle
			hdc = GetDC(hWnd);
			GetClientRect(hWnd, &rect);

			rect.top += MARGIN_TOP;
			rect.bottom -= MARGIN_BOTTOM;
			rect.left += MARGIN_LEFT;
			rect.right -= MARGIN_RIGHT;

			fCoeffWidth = (float)iLeftWidth / (float)(rect.right - rect.left);
			fCoeffHeight = (float)iTopHeight / (float)(rect.bottom - rect.top);

			if(xSizing)
			{
				SetRect(&focusrect, iLeftWidth - (WIDTH_ADJUST * 2), rect.top, iLeftWidth + WIDTH_ADJUST, rect.bottom);

				// Call api to vanish the dragging rectangle 
				DrawFocusRect(hdc, &focusrect);

				xSizing = FALSE;
			}
			if(ySizing)
			{
				SetRect(&focusrect, rect.left, iTopHeight - (WIDTH_ADJUST * 2), rect.right, iTopHeight + WIDTH_ADJUST);

				// Call api to vanish the dragging rectangle 
				DrawFocusRect(hdc, &focusrect);

				ySizing = FALSE;
			}
			// Release the dc once done 
			ReleaseDC(hWnd, hdc);
			// Post a WM_SIZE message to redraw the windows
			PostMessage(hWnd, WM_SIZE, 0, 0);
		}

		break;

	case WM_MOUSEMOVE:
	{
		int   xPos;
		int   yPos;

		// Get the x and y co-ordinates of the mouse

		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);

		GetClientRect(hWnd, &rect);

		rect.top += MARGIN_TOP;
		rect.bottom -= MARGIN_BOTTOM;
		rect.left += MARGIN_LEFT;
		rect.right -= MARGIN_RIGHT;

		if(xPos < rect.left + SPLITTER_BAR_WIDTH * 3)
		{
			xPos = rect.left + SPLITTER_BAR_WIDTH * 3;
		}
		if(xPos > rect.right - SPLITTER_BAR_WIDTH * 3)
		{
			xPos = rect.right - SPLITTER_BAR_WIDTH * 3;
		}
		if(yPos < rect.top + SPLITTER_BAR_WIDTH * 3)
		{
			yPos = rect.top + SPLITTER_BAR_WIDTH * 3;
		}
		if(yPos > rect.bottom - SPLITTER_BAR_WIDTH * 3)
		{
			yPos = rect.bottom - SPLITTER_BAR_WIDTH * 3;
		}

		// Checks if the left button is pressed during dragging the splitter
		if(wParam == MK_LBUTTON)
		{
			// If the window is d`agged using the splitter, get the
			// cursors current postion and draws a focus rectangle 
			if(xSizing)
			{
				RECT    focusrect;
				HDC     hdc;

				hdc = GetDC(hWnd);


				SetRect(&focusrect, iLeftWidth + MARGIN_LEFT - (WIDTH_ADJUST * 2), rect.top, iLeftWidth + MARGIN_LEFT + WIDTH_ADJUST, rect.bottom);

				DrawFocusRect(hdc, &focusrect);

				// Get the size of the left window to increase
				iLeftWidth = xPos - MARGIN_LEFT;

				// Draws a focus rectangle
				SetRect(&focusrect, iLeftWidth + MARGIN_LEFT - (SPLITTER_BAR_WIDTH * 2), rect.top, iLeftWidth + MARGIN_LEFT + SPLITTER_BAR_WIDTH, rect.bottom);

				DrawFocusRect(hdc, &focusrect);

				ReleaseDC(hWnd, hdc);
			}

			if(ySizing)
			{
				RECT focusrect;
				HDC hdc;
				hdc = GetDC(hWnd);
				SetRect(&focusrect, rect.left, iTopHeight + MARGIN_TOP - (WIDTH_ADJUST * 2), rect.right, iTopHeight + MARGIN_TOP + WIDTH_ADJUST);
				DrawFocusRect(hdc, &focusrect);
				iTopHeight = yPos - MARGIN_TOP;
				SetRect(&focusrect, rect.left, iTopHeight + MARGIN_TOP - (SPLITTER_BAR_WIDTH * 2), rect.right, iTopHeight + MARGIN_TOP + SPLITTER_BAR_WIDTH);
				DrawFocusRect(hdc, &focusrect);
				ReleaseDC(hWnd, hdc);
			}
		}
		// Set the cursor image to east west direction when the mouse is over 
		// the splitter window
		BOOL x = g_isXResizeable && (xPos > iLeftWidth + MARGIN_LEFT - SPLITTER_BAR_WIDTH && xPos < iLeftWidth + MARGIN_LEFT + SPLITTER_BAR_WIDTH);
		BOOL y = g_isYResizeable && (yPos > iTopHeight + MARGIN_TOP - SPLITTER_BAR_WIDTH && yPos < iTopHeight + MARGIN_TOP + SPLITTER_BAR_WIDTH);
		if(x && y)
		{
			SetCursor(hcSizeNESW);
		}
		else if(x)
		{
			SetCursor(hcSizeEW);
		}
		else if(y)
		{
			SetCursor(hcSizeNS);
		}
	}
	break;

	case WM_MOUSEWHEEL:
	{
		POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
		ScreenToClient(hWnd, &pt);

		GetClientRect(hWnd, &rect);

		rect.top += MARGIN_TOP;
		rect.bottom -= MARGIN_BOTTOM;
		rect.left += MARGIN_LEFT;
		rect.right -= MARGIN_RIGHT;
		if(pt.x < rect.left || pt.x > rect.right || pt.y < rect.top || pt.y > rect.bottom)
		{
			return(TRUE);
		}
		BOOL isLeft = (pt.x < rect.left + iLeftWidth),
			isTop = (pt.y < rect.top + iTopHeight);
		ICamera *pCamera = NULL;
		X_2D_VIEW x2dView;
		HWND hTargetWnd;
		float *pfOldScale = NULL;
		if(isLeft)
		{
			if(isTop)
			{
				// pCamera = g_pTopLeftCamera;
				pCamera = g_xConfig.m_pViewportCamera[XWP_TOP_LEFT];
				float3 vLook, vPos;
				pCamera->getLook(&vLook);
				pCamera->getPosition(&vPos);
				pCamera->setPosition(&(float3)(vPos + vLook * (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA * 0.5f));
				pCamera = NULL;
				hTargetWnd = g_hTopLeftWnd;
			}
			else
			{
				pCamera = g_xConfig.m_pViewportCamera[XWP_BOTTOM_LEFT];
				x2dView = g_xConfig.m_x2DView[XWP_BOTTOM_LEFT];
				hTargetWnd = g_hBottomLeftWnd;
				pfOldScale = &g_xConfig.m_fViewportScale[XWP_BOTTOM_LEFT];
			}
		}
		else
		{
			if(isTop)
			{
				pCamera = g_xConfig.m_pViewportCamera[XWP_TOP_RIGHT];
				x2dView = g_xConfig.m_x2DView[XWP_TOP_RIGHT];
				hTargetWnd = g_hTopRightWnd;
				pfOldScale = &g_xConfig.m_fViewportScale[XWP_TOP_RIGHT];
			}
			else
			{
				pCamera = g_xConfig.m_pViewportCamera[XWP_BOTTOM_RIGHT];
				x2dView = g_xConfig.m_x2DView[XWP_BOTTOM_RIGHT];
				hTargetWnd = g_hBottomRightWnd;
				pfOldScale = &g_xConfig.m_fViewportScale[XWP_BOTTOM_RIGHT];
			}
		}
		if(pCamera)
		{
			float fNewScale = *pfOldScale - *pfOldScale * (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA * 0.1f;
			RECT rc;
			GetWindowRect(hTargetWnd, &rc);
			MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rc, 2);
			float2 vCenter((float)(rc.left + rc.right) * 0.5f, (float)(rc.top + rc.bottom) * 0.5f);
			float2 vDelta = (float2((float)pt.x, (float)pt.y) - vCenter) * float2(1.0f, -1.0f);
			float2 vWorldPt;
			float3 vCamPos;
			pCamera->getPosition(&vCamPos);
			switch(x2dView)
			{
			case X2D_TOP:
				vWorldPt = float2(vCamPos.x, vCamPos.z);
				break;
			case X2D_FRONT:
				vWorldPt = float2(vCamPos.x, vCamPos.y);
				break;
			case X2D_SIDE:
				vWorldPt = float2(vCamPos.z, vCamPos.y);
			//	vDelta.x *= -1;
				break;
			}
			vWorldPt += vDelta * (*pfOldScale - fNewScale);
			switch(x2dView)
			{
			case X2D_TOP:
				pCamera->setPosition(&float3(vWorldPt.x, vCamPos.y, vWorldPt.y));
				break;
			case X2D_FRONT:
				pCamera->setPosition(&float3(vWorldPt.x, vWorldPt.y, vCamPos.z));
				break;
			case X2D_SIDE:
				pCamera->setPosition(&float3(vCamPos.x, vWorldPt.y, vWorldPt.x));
				break;
			}
			*pfOldScale = fNewScale;
		}
		else
		{
			// 3d
		}
		break;
	}

	case WM_DESTROY:
		mem_delete(g_pPropWindow);
		DestroyMenu(g_hMenu);
		DeleteObject(hcSizeEW);
		DeleteObject(hcSizeNS);
		DeleteObject(hcSizeNESW);
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		if(g_pUndoManager->isDirty())
		{
			const char *szLevelName = g_sLevelName.c_str();
			char szPrompt[128];
			if(szLevelName[0])
			{
				sprintf_s(szPrompt, "Save changes to %s?", szLevelName);
			}
			UINT mb = MessageBoxA(hWnd, szLevelName[0] ? szPrompt : "Save changes?", MAIN_WINDOW_TITLE, MB_YESNOCANCEL);
			if(mb == IDYES)
			{
				if(!XSaveLevel())
				{
					break;
				}
			}
			else if(mb == IDCANCEL)
			{
				break;
			}
		}
		DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return(0);
}

LRESULT CALLBACK GuiWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	PAINTSTRUCT ps;
	HDC hdc;
	
	static float fCoeffWidth = 0.0f;
	static float fCoeffHeight = 0.0f;
	static int iLeftWidth = 0;
	static int iTopHeight = 0;

	IMSG msg;
	msg.lParam = lParam;
	msg.wParam = wParam;
	msg.message = message;

	// SSInput_AddMsg(msg);


	if(g_pGUI && !g_pGUI->putMessage(message, wParam, lParam))
	{
		return(TRUE);
	}

	switch(message)
	{
	case WM_SIZE:
		{
			static int *r_resize = (int*)GET_PCVAR_INT("r_resize");

			if(!r_resize)
			{
				r_resize = (int*)GET_PCVAR_INT("r_resize");
			}

			if(r_resize)
			{
				*r_resize = 1;
			}
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK RenderWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(g_pEngine && g_pEngine->onMessage(message, wParam, lParam))
	{
		return(TRUE);
	}

	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect; 
	static HCURSOR hcPtr = NULL;
	static HCURSOR hcSizeNS = NULL;
	static HCURSOR hcSizeWE = NULL;
	static HCURSOR hcSizeNESW = NULL;
	static HCURSOR hcSizeNWSE = NULL;
	static HCURSOR hcRotate = NULL;

	static const int *r_final_image = GET_PCVAR_INT("r_final_image");
	if(!r_final_image)
	{
		r_final_image = GET_PCVAR_INT("r_final_image");
	}

	static CCommandMove *s_pMoveCmd = NULL;
	static CCommandScale *s_pScaleCmd = NULL;
	static CCommandRotate *s_pRotateCmd = NULL;

	switch(message)
	{
	case WM_CREATE:
		hcPtr = LoadCursor(NULL, IDC_CROSS);
		hcSizeNS = LoadCursor(NULL, IDC_SIZENS);
		hcSizeWE = LoadCursor(NULL, IDC_SIZEWE);
		hcSizeNWSE = LoadCursor(NULL, IDC_SIZENWSE);
		hcSizeNESW = LoadCursor(NULL, IDC_SIZENESW); 
		hcRotate = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1));
		break;

	case WM_RBUTTONUP:
		if(g_is3DPanning)
		{
			SSInput_SetEnable(false);
			ReleaseCapture();
			g_is3DPanning = FALSE;
		}
		break;

	case WM_LBUTTONUP:
	{
		if(g_is3DRotating)
		{
			g_is3DRotating = FALSE;
			SSInput_SetEnable(false);
			ReleaseCapture();
			break;
		}
		if(g_xState.isFrameSelect)
		{
			g_xState.isFrameSelect = false;
			ReleaseCapture();

			if(memcmp(&g_xState.vWorldMousePos, &g_xState.vFrameSelectStart, sizeof(float2_t)))
			{
				X_2D_VIEW xCurView = g_xConfig.m_x2DView[g_xState.activeWindow];

				CCommandSelect *pCmd = new CCommandSelect();
				bool bUse = false;
				for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
				{
					IXEditorObject *pObj = g_pLevelObjects[i];
					float3_t vPos = pObj->getPos();
					bool sel = false;
					switch(xCurView)
					{
					case X2D_TOP:
						sel = ((vPos.x > g_xState.vWorldMousePos.x && vPos.x <= g_xState.vFrameSelectStart.x) || (vPos.x < g_xState.vWorldMousePos.x && vPos.x >= g_xState.vFrameSelectStart.x))
							&& ((vPos.z > g_xState.vWorldMousePos.y && vPos.z <= g_xState.vFrameSelectStart.y) || (vPos.z < g_xState.vWorldMousePos.y && vPos.z >= g_xState.vFrameSelectStart.y));
						break;
					case X2D_FRONT:
						sel = ((vPos.x > g_xState.vWorldMousePos.x && vPos.x <= g_xState.vFrameSelectStart.x) || (vPos.x < g_xState.vWorldMousePos.x && vPos.x >= g_xState.vFrameSelectStart.x))
							&& ((vPos.y > g_xState.vWorldMousePos.y && vPos.y <= g_xState.vFrameSelectStart.y) || (vPos.y < g_xState.vWorldMousePos.y && vPos.y >= g_xState.vFrameSelectStart.y));
						break;
					case X2D_SIDE:
						sel = ((vPos.z > g_xState.vWorldMousePos.x && vPos.z <= g_xState.vFrameSelectStart.x) || (vPos.z < g_xState.vWorldMousePos.x && vPos.z >= g_xState.vFrameSelectStart.x))
							&& ((vPos.y > g_xState.vWorldMousePos.y && vPos.y <= g_xState.vFrameSelectStart.y) || (vPos.y < g_xState.vWorldMousePos.y && vPos.y >= g_xState.vFrameSelectStart.y));
						break;
					}
					if(wParam & MK_CONTROL)
					{
						if(sel && !pObj->isSelected())
						{
							//pObj->setSelected(true);
							bUse = true;
							pCmd->addSelected(i);
						}
					}
					else
					{
						if(!pObj->isSelected() && sel)
						{
							bUse = true;
							pCmd->addSelected(i);
							//pObj->setSelected(sel);
						}
						else if(pObj->isSelected() && !sel)
						{
							bUse = true;
							pCmd->addDeselected(i);
							//pObj->setSelected(sel);
						}
					}
				}

				if(bUse)
				{
					if(!g_xState.bHasSelection)
					{
						g_xState.xformType = X2DXF_SCALE;
					}
					XExecCommand(pCmd);
				}
			}
		}
		else if(s_pMoveCmd)
		{
			ReleaseCapture();
			if(!XExecCommand(s_pMoveCmd) && g_xState.bHasSelection)
			{
				g_xState.xformType = (X_2DXFORM_TYPE)((g_xState.xformType + 1) % X2DXF__LAST);
			}
			s_pMoveCmd = NULL;
		}
		else if(s_pScaleCmd)
		{
			ReleaseCapture();
			XExecCommand(s_pScaleCmd);
			s_pScaleCmd = NULL;
		}
		else if(s_pRotateCmd)
		{
			ReleaseCapture();
			XExecCommand(s_pRotateCmd);
			s_pRotateCmd = NULL;
		}

		POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
		GetClientRect(hWnd, &rect);
		if(pt.x > rect.left && pt.x < rect.left + 20
			&& pt.y > rect.top && pt.y < rect.top + 20)
		{
			int iActiveMenu = -1;
			X_2D_VIEW x2dView = g_xConfig.m_x2DView[g_xState.activeWindow];
			if(hWnd == g_hTopLeftWnd)
			{
				switch(*r_final_image)
				{
				case DS_RT_COLOR:
					iActiveMenu = ID_3D_TEXTURED;
					break;
				case DS_RT_NORMAL:
					iActiveMenu = ID_3D_NORMALS;
					break;
				case DS_RT_PARAM:
					iActiveMenu = ID_3D_PARAMETERS;
					break;
				case DS_RT_AMBIENTDIFF:
					iActiveMenu = ID_3D_AMBIENTDIFFUSE;
					break;
				case DS_RT_SPECULAR:
					iActiveMenu = ID_3D_SPECULAR;
					break;
				case DS_RT_SCENELIGHT:
					iActiveMenu = ID_3D_LIGHTINGSCENE;
					break;
				case DS_RT_DEPTH:
					iActiveMenu = ID_3D_3DDEPTH;
					break;
				}
			}
			else
			{
				switch(x2dView)
				{
				case X2D_TOP:
					iActiveMenu = ID_2D_TOP;
					break;
				case X2D_FRONT:
					iActiveMenu = ID_2D_FRONT;
					break;
				case X2D_SIDE:
					iActiveMenu = ID_2D_SIDE;
					break;
				}
			}
			DisplayContextMenu(hWnd, pt, IDR_MENU2, hWnd == g_hTopLeftWnd ? 0 : 1, iActiveMenu);
		}
	}
		break;

	case WM_LBUTTONDOWN:
	{
		POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
		GetClientRect(hWnd, &rect);
		if(pt.x > rect.left && pt.x < rect.left + 20
			&& pt.y > rect.top && pt.y < rect.top + 20)
		{
			break;
		}

		if(hWnd != g_hTopLeftWnd)
		{
			if(Button_GetCheck(g_hABArrowButton))
			{
				X_2D_VIEW xCurView = g_xConfig.m_x2DView[g_xState.activeWindow];
				float fViewScale = g_xConfig.m_fViewportScale[g_xState.activeWindow];
				
				const float fWorldSize = 3.5f * fViewScale;

				if(g_xState.bHasSelection)
				{
					float fScale = g_xConfig.m_fViewportScale[g_xState.activeWindow];
					float fPtSize = 3.0f * fScale;
					float fPtMargin = 7.0f * fScale;
					float2 vSelectionCenter;
					float4 vBorder;

					const float2_t &fMPos = g_xState.vWorldMousePos;
					float3 vStartPos;
					float3 vMask;

					switch(g_xConfig.m_x2DView[g_xState.activeWindow])
					{
					case X2D_TOP:
						vBorder = float4(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.z,
							g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.z);
						vSelectionCenter = float2(g_xState.vSelectionBoundMin.x + g_xState.vSelectionBoundMax.x,
							g_xState.vSelectionBoundMin.z + g_xState.vSelectionBoundMax.z) * 0.5f;
						vStartPos = float3(fMPos.x, 0.0f, fMPos.y);
						vMask = float3(1.0f, 0.0f, 1.0f);
						break;
					case X2D_FRONT:
						vBorder = float4(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.y,
							g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.y);
						vSelectionCenter = float2(g_xState.vSelectionBoundMin.x + g_xState.vSelectionBoundMax.x,
							g_xState.vSelectionBoundMin.y + g_xState.vSelectionBoundMax.y) * 0.5f;
						vStartPos = float3(fMPos.x, fMPos.y, 0.0f);
						vMask = float3(1.0f, 1.0f, 0.0f);
						break;
					case X2D_SIDE:
						vBorder = float4(g_xState.vSelectionBoundMin.z, g_xState.vSelectionBoundMin.y,
							g_xState.vSelectionBoundMax.z, g_xState.vSelectionBoundMax.y);
						vSelectionCenter = float2(g_xState.vSelectionBoundMin.z + g_xState.vSelectionBoundMax.z,
							g_xState.vSelectionBoundMin.y + g_xState.vSelectionBoundMax.y) * 0.5f;
						vStartPos = float3(0.0f, fMPos.y, fMPos.x);
						vMask = float3(0.0f, 1.0f, 1.0f);
						break;
					}

					// bottom center
					float2_t vCenters[] = {
						{vSelectionCenter.x, vBorder.y - fPtMargin}, // bottom center
						{vBorder.z + fPtMargin, vSelectionCenter.y}, // center right
						{vSelectionCenter.x, vBorder.w + fPtMargin}, // top center
						{vBorder.x - fPtMargin, vSelectionCenter.y}, // center left
						{vBorder.x - fPtMargin, vBorder.y - fPtMargin}, // bottom left
						{vBorder.z + fPtMargin, vBorder.y - fPtMargin}, // bottom right
						{vBorder.z + fPtMargin, vBorder.w + fPtMargin}, // top right
						{vBorder.x - fPtMargin, vBorder.w + fPtMargin} // top left
					};
					bool bHandled = false;
					HCURSOR hcs[] = {hcSizeNS, hcSizeWE, hcSizeNS, hcSizeWE, hcSizeNESW, hcSizeNWSE, hcSizeNESW, hcSizeNWSE};
					X_DIR dirs[][8] = {
						// X2D_TOP (x/z)
						{XDIR_Z_NEG, XDIR_X_POS, XDIR_Z_POS, XDIR_X_NEG, XDIR_Z_NEG | XDIR_X_NEG, XDIR_Z_NEG | XDIR_X_POS, XDIR_Z_POS | XDIR_X_POS, XDIR_Z_POS | XDIR_X_NEG},
						// X2D_FRONT (x/y)
						{XDIR_Y_NEG, XDIR_X_POS, XDIR_Y_POS, XDIR_X_NEG, XDIR_Y_NEG | XDIR_X_NEG, XDIR_Y_NEG | XDIR_X_POS, XDIR_Y_POS | XDIR_X_POS, XDIR_Y_POS | XDIR_X_NEG},
						// X2D_SIDE (z/y)
						{XDIR_Y_NEG, XDIR_Z_POS, XDIR_Y_POS, XDIR_Z_NEG, XDIR_Y_NEG | XDIR_Z_NEG, XDIR_Y_NEG | XDIR_Z_POS, XDIR_Y_POS | XDIR_Z_POS, XDIR_Y_POS | XDIR_Z_NEG}
					};
					for(int i = g_xState.xformType == X2DXF_SCALE ? 0 : 4; i < 8; ++i)
					{
						if(fabsf(fMPos.x - vCenters[i].x) <= fPtSize && fabsf(fMPos.y - vCenters[i].y) <= fPtSize)
						{
							SetCapture(hWnd);

							if(g_xState.xformType == X2DXF_SCALE)
							{
								// create scale command
								s_pScaleCmd = new CCommandScale();
								s_pScaleCmd->setStartAABB(g_xState.vSelectionBoundMin, g_xState.vSelectionBoundMax);
								s_pScaleCmd->setTransformDir(dirs[g_xConfig.m_x2DView[g_xState.activeWindow]][i]);
								s_pScaleCmd->setStartPos(vStartPos);
								for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
								{
									if(g_pLevelObjects[i]->isSelected())
									{
										s_pScaleCmd->addObject(i);
									}
								}
							}
							else if(g_xState.xformType == X2DXF_ROTATE)
							{
								// create rotate command
								s_pRotateCmd = new CCommandRotate();
								s_pRotateCmd->setStartOrigin((g_xState.vSelectionBoundMax + g_xState.vSelectionBoundMin) * 0.5f * vMask);
								s_pRotateCmd->setStartPos(vStartPos);
								for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
								{
									if(g_pLevelObjects[i]->isSelected())
									{
										s_pRotateCmd->addObject(i);
									}
								}
							}
							bHandled = true;
							break;
						}
					}
					if(bHandled)
					{
						break;
					}
				}

				if(!XIsMouseInSelection(g_xState.activeWindow) || (wParam & MK_CONTROL))
				{
					CCommandSelect *pCmd = new CCommandSelect();
					bool bUse = false;
					for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
					{
						IXEditorObject *pObj = g_pLevelObjects[i];
						float3_t vPos = pObj->getPos();
						bool sel = false;
						switch(xCurView)
						{
						case X2D_TOP:
							sel = fabsf(vPos.x - g_xState.vWorldMousePos.x) < fWorldSize && fabsf(vPos.z - g_xState.vWorldMousePos.y) < fWorldSize;
							break;
						case X2D_FRONT:
							sel = fabsf(vPos.x - g_xState.vWorldMousePos.x) < fWorldSize && fabsf(vPos.y - g_xState.vWorldMousePos.y) < fWorldSize;
							break;
						case X2D_SIDE:
							sel = fabsf(vPos.z - g_xState.vWorldMousePos.x) < fWorldSize && fabsf(vPos.y - g_xState.vWorldMousePos.y) < fWorldSize;
							break;
						}
						if(wParam & MK_CONTROL)
						{
							if(sel)
							{
								if(pObj->isSelected())
								{
									pCmd->addDeselected(i);
								}
								else
								{
									pCmd->addSelected(i);
								}
								bUse = true;
							}
						}
						else
						{
							if(pObj->isSelected() && !sel)
							{
								pCmd->addDeselected(i);
							}
							else if(!pObj->isSelected() && sel)
							{
								pCmd->addSelected(i);
							}
							bUse = true;
						}
					}

					if(bUse)
					{
						XExecCommand(pCmd);
						XUpdateSelectionBound();
					}
				}

				if(XIsMouseInSelection(g_xState.activeWindow))
				{
					SetCapture(hWnd);
					SetCursor(hcPtr);
					s_pMoveCmd = new CCommandMove();
					float3 vStartPos;
					switch(xCurView)
					{
					case X2D_TOP:
						vStartPos = float3(g_xState.vWorldMousePos.x, 0.0f, g_xState.vWorldMousePos.y);
						break;
					case X2D_FRONT:
						vStartPos = float3(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, 0.0f);
						break;
					case X2D_SIDE:
						vStartPos = float3(0.0f, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
						break;
					}
					s_pMoveCmd->setStartPos(vStartPos);
					for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
					{
						if(g_pLevelObjects[i]->isSelected())
						{
							s_pMoveCmd->addObject(i);
						}
					}
					// if mouse in selected object
					// start move
				}
				else
				{ // start frame select
					g_xState.isFrameSelect = true;
					SetCapture(hWnd);
					g_xState.vFrameSelectStart = g_xState.vWorldMousePos;
				}
			}
			else if(Button_GetCheck(g_hABCreateButton))
			{
				X_2D_VIEW xCurView = g_xConfig.m_x2DView[g_xState.activeWindow];

				g_xState.bCreateMode = true;

				switch(xCurView)
				{
				case X2D_TOP:
					g_xState.vCreateOrigin = float3(g_xState.vWorldMousePos.x, g_xState.vCreateOrigin.y, g_xState.vWorldMousePos.y);
					break;
				case X2D_FRONT:
					g_xState.vCreateOrigin = float3(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, g_xState.vCreateOrigin.z);
					break;
				case X2D_SIDE:
					g_xState.vCreateOrigin = float3(g_xState.vCreateOrigin.x, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
					break;
				}
			}
			break;
		}

		if(g_is3DPanning)
		{
			break;
		}
		if(Button_GetCheck(g_hABCameraButton))
		{
			g_is3DRotating = TRUE;
			SetCapture(hWnd);
			SSInput_SetEnable(true);
		}
		else
		break;
	}

	case WM_RBUTTONDOWN:
	{
		if(hWnd != g_hTopLeftWnd || g_is3DRotating)
		{
			break;
		}
		if(Button_GetCheck(g_hABCameraButton))
		{
			g_is3DPanning = TRUE;
			SetCapture(hWnd);
			SSInput_SetEnable(true);
		}
		break;
	}

	case WM_MOUSEMOVE:
		if(!g_is3DRotating && !g_is3DPanning)
		{
			if(hWnd == g_hTopRightWnd)
			{
				g_xState.activeWindow = XWP_TOP_RIGHT;
			}
			else if(hWnd == g_hBottomLeftWnd)
			{
				g_xState.activeWindow = XWP_BOTTOM_LEFT;
			}
			else if(hWnd == g_hBottomRightWnd)
			{
				g_xState.activeWindow = XWP_BOTTOM_RIGHT;
			}
			else
			{
				g_xState.activeWindow = XWP_TOP_LEFT;
			}

			g_xState.vMousePos = {(float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam)};

			if(g_xState.activeWindow != XWP_TOP_LEFT)
			{
				ICamera *pCamera = g_xConfig.m_pViewportCamera[g_xState.activeWindow];
				if(!pCamera)
				{
					break;
				}
				X_2D_VIEW xCurView = g_xConfig.m_x2DView[g_xState.activeWindow];
				float fViewScale = g_xConfig.m_fViewportScale[g_xState.activeWindow];

				float3 fCamWorld;
				pCamera->getPosition(&fCamWorld);
				switch(xCurView)
				{
				case X2D_TOP:
					fCamWorld = float3(fCamWorld.x, fCamWorld.z, 0.0f);
					break;
				case X2D_FRONT:
					fCamWorld = float3(fCamWorld.x, fCamWorld.y, 0.0f);
					break;
				case X2D_SIDE:
					fCamWorld = float3(fCamWorld.z, fCamWorld.y, 0.0f);
					break;
				}

				RECT rc;
				GetClientRect(hWnd, &rc);

				float2 vCenter((float)(rc.left + rc.right) * 0.5f, (float)(rc.top + rc.bottom) * 0.5f);
				float2 vDelta = (g_xState.vMousePos - vCenter) * float2(1.0f, -1.0f);

				float2 vSize((float)(rc.right - rc.left), (float)(rc.bottom - rc.top));
				vSize *= 0.5f * fViewScale;
				float2 vTopLeft = fCamWorld - vSize;
				float2 vBottomRight = fCamWorld + vSize;
				g_xState.m_vViewportBorders[g_xState.activeWindow] = float4(vTopLeft.x, vTopLeft.y, vBottomRight.x, vBottomRight.y);

				g_xState.vWorldMousePos = (float2)(fCamWorld + vDelta * fViewScale);

				if(s_pMoveCmd || s_pScaleCmd || s_pRotateCmd)
				{
					float3 vCurPos;
					switch(xCurView)
					{
					case X2D_TOP:
						vCurPos = float3(g_xState.vWorldMousePos.x, 0.0f, g_xState.vWorldMousePos.y);
						break;
					case X2D_FRONT:
						vCurPos = float3(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, 0.0f);
						break;
					case X2D_SIDE:
						vCurPos = float3(0.0f, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
						break;
					}

					if(s_pMoveCmd)
					{
						s_pMoveCmd->setCurrentPos(vCurPos);
					}
					if(s_pScaleCmd)
					{
						s_pScaleCmd->setCurrentPos(vCurPos);
					}
					if(s_pRotateCmd)
					{
						s_pRotateCmd->setCurrentPos(vCurPos);
					}
				}
			}
			XUpdateStatusMPos();

			if(Button_GetCheck(g_hABArrowButton) && !s_pMoveCmd && !s_pScaleCmd && !s_pRotateCmd)
			{
				if(XIsMouseInSelection(g_xState.activeWindow))
				{
					SetCursor(hcPtr);
				}
				else if(g_xState.bHasSelection)
				{
					float fScale = g_xConfig.m_fViewportScale[g_xState.activeWindow];
					float fPtSize = 3.0f * fScale;
					float fPtMargin = 7.0f * fScale;
					float2 vSelectionCenter;
					float4 vBorder;

					const float2_t &fMPos = g_xState.vWorldMousePos;

					switch(g_xConfig.m_x2DView[g_xState.activeWindow])
					{
					case X2D_TOP:
						vBorder = float4(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.z,
							g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.z);
						vSelectionCenter = float2(g_xState.vSelectionBoundMin.x + g_xState.vSelectionBoundMax.x,
							g_xState.vSelectionBoundMin.z + g_xState.vSelectionBoundMax.z) * 0.5f;
						break;
					case X2D_FRONT:
						vBorder = float4(g_xState.vSelectionBoundMin.x, g_xState.vSelectionBoundMin.y,
							g_xState.vSelectionBoundMax.x, g_xState.vSelectionBoundMax.y);
						vSelectionCenter = float2(g_xState.vSelectionBoundMin.x + g_xState.vSelectionBoundMax.x,
							g_xState.vSelectionBoundMin.y + g_xState.vSelectionBoundMax.y) * 0.5f;
						break;
					case X2D_SIDE:
						vBorder = float4(g_xState.vSelectionBoundMin.z, g_xState.vSelectionBoundMin.y,
							g_xState.vSelectionBoundMax.z, g_xState.vSelectionBoundMax.y);
						vSelectionCenter = float2(g_xState.vSelectionBoundMin.z + g_xState.vSelectionBoundMax.z,
							g_xState.vSelectionBoundMin.y + g_xState.vSelectionBoundMax.y) * 0.5f;
						break;
					}

					// bottom center
					float2_t vCenters[] = {
						{vSelectionCenter.x, vBorder.y - fPtMargin}, // bottom center
						{vBorder.z + fPtMargin, vSelectionCenter.y}, // center right
						{vSelectionCenter.x, vBorder.w + fPtMargin}, // top center
						{vBorder.x - fPtMargin, vSelectionCenter.y}, // center left

						{vBorder.x - fPtMargin, vBorder.y - fPtMargin}, // bottom left
						{vBorder.z + fPtMargin, vBorder.y - fPtMargin}, // bottom right
						{vBorder.z + fPtMargin, vBorder.w + fPtMargin}, // top right
						{vBorder.x - fPtMargin, vBorder.w + fPtMargin} // top left
					};
					HCURSOR hcs[] = {hcSizeNS, hcSizeWE, hcSizeNS, hcSizeWE, hcSizeNESW, hcSizeNWSE, hcSizeNESW, hcSizeNWSE};
					for(int i = (g_xState.xformType == X2DXF_SCALE) ? 0 : 4; i < 8; ++i)
					{
						if(fabsf(fMPos.x - vCenters[i].x) <= fPtSize && fabsf(fMPos.y - vCenters[i].y) <= fPtSize)
						{
							if(g_xState.xformType == X2DXF_SCALE)
							{
								SetCursor(hcs[i]);
							}
							else if(g_xState.xformType == X2DXF_ROTATE)
							{
								SetCursor(hcRotate);
							}
							break;
						}
					}
				}
			}
			if(s_pMoveCmd)
			{
				SetCursor(hcPtr);
			}
			if(s_pScaleCmd)
			{
				//@TODO:SetCursor(???);
			}
			if(s_pRotateCmd)
			{
				SetCursor(hcRotate);
			}

			if(Button_GetCheck(g_hABCreateButton) && g_xState.bCreateMode && GetAsyncKeyState(VK_LBUTTON) < 0)
			{
				switch(g_xConfig.m_x2DView[g_xState.activeWindow])
				{
				case X2D_TOP:
					g_xState.vCreateOrigin = float3(g_xState.vWorldMousePos.x, g_xState.vCreateOrigin.y, g_xState.vWorldMousePos.y);
					break;
				case X2D_FRONT:
					g_xState.vCreateOrigin = float3(g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y, g_xState.vCreateOrigin.z);
					break;
				case X2D_SIDE:
					g_xState.vCreateOrigin = float3(g_xState.vCreateOrigin.x, g_xState.vWorldMousePos.y, g_xState.vWorldMousePos.x);
					break;
				}
			}
			return(TRUE);
		}
		break;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case ID_3D_TEXTURED:
			Core_0SetCVarInt("r_final_image", DS_RT_COLOR);
			break;
		case ID_3D_NORMALS:
			Core_0SetCVarInt("r_final_image", DS_RT_NORMAL);
			break;
		case ID_3D_PARAMETERS:
			Core_0SetCVarInt("r_final_image", DS_RT_PARAM);
			break;
		case ID_3D_AMBIENTDIFFUSE:
			Core_0SetCVarInt("r_final_image", DS_RT_AMBIENTDIFF);
			break;
		case ID_3D_SPECULAR:
			Core_0SetCVarInt("r_final_image", DS_RT_SPECULAR);
			break;
		case ID_3D_LIGHTINGSCENE:
			Core_0SetCVarInt("r_final_image", DS_RT_SCENELIGHT);
			break;
		case ID_3D_3DDEPTH:
			Core_0SetCVarInt("r_final_image", DS_RT_DEPTH);
			break;
			

		case ID_2D_TOP:
		case ID_2D_FRONT:
		case ID_2D_SIDE:
		{
			ICamera *pTargetCam = g_xConfig.m_pViewportCamera[g_xState.activeWindow];
			X_2D_VIEW *pX2DView = &g_xConfig.m_x2DView[g_xState.activeWindow];
			
			switch(LOWORD(wParam))
			{
			case ID_2D_TOP:
				pTargetCam->setPosition(&X2D_TOP_POS);
				pTargetCam->setOrientation(&X2D_TOP_ROT);
				*pX2DView = X2D_TOP;
				break;
			case ID_2D_FRONT:
				pTargetCam->setPosition(&X2D_FRONT_POS);
				pTargetCam->setOrientation(&X2D_FRONT_ROT);
				*pX2DView = X2D_FRONT;
				break;
			case ID_2D_SIDE:
				pTargetCam->setPosition(&X2D_SIDE_POS);
				pTargetCam->setOrientation(&X2D_SIDE_ROT);
				*pX2DView = X2D_SIDE;
				break;
			}
		}
		}
		break;

	case WM_DESTROY:
		DeleteObject(hcPtr);
		DeleteObject(hcSizeNS);
		DeleteObject(hcSizeWE);
		DeleteObject(hcSizeNWSE);
		DeleteObject(hcSizeNESW);
		DeleteObject(hcRotate);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void DisplayContextMenu(HWND hwnd, POINT pt, int iMenu, int iSubmenu, int iCheckItem)
{
	HMENU hmenu;            // top-level menu 
	HMENU hmenuTrackPopup;  // shortcut menu 

	// Load the menu resource. 

	if((hmenu = LoadMenu(hInst, MAKEINTRESOURCE(iMenu))) == NULL)
		return;
	hmenuTrackPopup = GetSubMenu(hmenu, iSubmenu);

	MENUITEMINFOA mii;
	memset(&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE;
	mii.fState = MFS_UNCHECKED;
	for(UINT i = 0, l = GetMenuItemCount(hmenuTrackPopup); i < l; ++i)
	{
		SetMenuItemInfoA(hmenuTrackPopup, i, TRUE, &mii);
	}
	if(iCheckItem)
	{
		mii.fState = MFS_CHECKED;
		SetMenuItemInfoA(hmenuTrackPopup, iCheckItem, FALSE, &mii);
	}

	ClientToScreen(hwnd, (LPPOINT)&pt);
	// Display the shortcut menu. Track the right mouse 
	// button. 

	TrackPopupMenu(hmenuTrackPopup,
		TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		pt.x, pt.y, 0, hwnd, NULL);

	// Destroy the menu. 

	DestroyMenu(hmenu);
}

void XInitViewportLayout(X_VIEWPORT_LAYOUT layout)
{
	MENUITEMINFOA mii;
	memset(&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE;
	mii.fState = MFS_UNCHECKED;
	SetMenuItemInfoA(g_hMenu, ID_VEWPORTCONFIGURATION_1X2, FALSE, &mii);
	SetMenuItemInfoA(g_hMenu, ID_VEWPORTCONFIGURATION_2X2, FALSE, &mii);
	SetMenuItemInfoA(g_hMenu, ID_VEWPORTCONFIGURATION_2X1, FALSE, &mii);
	SetMenuItemInfoA(g_hMenu, ID_VEWPORTCONFIGURATION_3DONLY, FALSE, &mii);
	UINT uMenuId = 0;
	mii.fState = MFS_CHECKED;
	switch(layout)
	{
	case XVIEW_2X2:
		uMenuId = ID_VEWPORTCONFIGURATION_2X2;
		ShowWindow(g_hTopRightWnd, TRUE);
		ShowWindow(g_hBottomLeftWnd, TRUE);
		ShowWindow(g_hBottomRightWnd, TRUE);
		g_isXResizeable = TRUE;
		g_isYResizeable = TRUE;
		break;
	case XVIEW_1X2:
		uMenuId = ID_VEWPORTCONFIGURATION_1X2;
		ShowWindow(g_hTopRightWnd, TRUE);
		ShowWindow(g_hBottomLeftWnd, FALSE);
		ShowWindow(g_hBottomRightWnd, FALSE);
		g_isXResizeable = TRUE;
		g_isYResizeable = FALSE;
		break;
	case XVIEW_2X1:
		uMenuId = ID_VEWPORTCONFIGURATION_2X1;
		ShowWindow(g_hTopRightWnd, FALSE);
		ShowWindow(g_hBottomLeftWnd, TRUE);
		ShowWindow(g_hBottomRightWnd, FALSE);
		g_isXResizeable = FALSE;
		g_isYResizeable = TRUE;
		break;
	case XVIEW_3DONLY:
		uMenuId = ID_VEWPORTCONFIGURATION_3DONLY;
		ShowWindow(g_hTopRightWnd, FALSE);
		ShowWindow(g_hBottomLeftWnd, FALSE);
		ShowWindow(g_hBottomRightWnd, FALSE);
		g_isXResizeable = FALSE;
		g_isYResizeable = FALSE;
		break;
	}
	
	SendMessage(g_hWndMain, WM_COMMAND, MAKEWPARAM(ID_VIEW_AUTOSIZEVIEWS, 0), 0);

	g_xConfig.m_xViewportLayout = layout;
	SetMenuItemInfoA(g_hMenu, uMenuId, FALSE, &mii);
}

BOOL XCheckMenuItem(HMENU hMenu, UINT uIDCheckItem, bool bCheck)
{
	MENUITEMINFOA mii;
	memset(&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE;
	mii.fState = bCheck ? MFS_CHECKED : MFS_UNCHECKED;
	return(SetMenuItemInfoA(hMenu, uIDCheckItem, FALSE, &mii));
}

void XUpdateStatusGrid()
{
	char szMsg[64];
	const char *szGrid = "";
	switch(g_xConfig.m_gridStep)
	{
	case GRID_STEP_1CM:
		szGrid = "1cm";
		break;
	case GRID_STEP_2CM:
		szGrid = "2cm";
		break;
	case GRID_STEP_5CM:
		szGrid = "5cm";
		break;
	case GRID_STEP_10CM:
		szGrid = "10cm";
		break;
	case GRID_STEP_20CM:
		szGrid = "20cm";
		break;
	case GRID_STEP_50CM:
		szGrid = "50cm";
		break;
	case GRID_STEP_1M:
		szGrid = "1m";
		break;
	case GRID_STEP_2M:
		szGrid = "2m";
		break;
	case GRID_STEP_5M:
		szGrid = "5m";
		break;
	case GRID_STEP_10M:
		szGrid = "10m";
		break;
	case GRID_STEP_20M:
		szGrid = "20m";
		break;
	case GRID_STEP_50M:
		szGrid = "50m";
		break;
	case GRID_STEP_100M:
		szGrid = "100m";
		break;
	case GRID_STEP_AXES:
		szGrid = "axes";
		break;
	}

	sprintf_s(szMsg, "Snap: %s; Grid: %s", g_xConfig.m_bSnapGrid ? "on" : "off", szGrid);
	SendMessage(g_hStatusWnd, SB_SETTEXT, MAKEWPARAM(4, 0), (LPARAM)szMsg);
}

void XUpdateStatusMPos()
{
	if(g_xState.activeWindow == XWP_TOP_LEFT)
	{
		SendMessage(g_hStatusWnd, SB_SETTEXT, MAKEWPARAM(2, 0), (LPARAM)"");
	}
	else
	{
		char szMsg[32];
		sprintf_s(szMsg, "@%.2f, %.2f", g_xState.vWorldMousePos.x, g_xState.vWorldMousePos.y);
		SendMessage(g_hStatusWnd, SB_SETTEXT, MAKEWPARAM(2, 0), (LPARAM)szMsg);
	}
}

void XUpdateUndoRedo()
{
	MENUITEMINFOA mii;
	memset(&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_STATE | MIIM_STRING;
	char str[64]; 
	mii.dwTypeData = str;

	mii.fState = g_pUndoManager->canUndo() ? MFS_ENABLED : MFS_DISABLED;
	if(g_pUndoManager->canUndo())
	{
		mii.cch = sprintf(str, "Undo %s\tCtrl+Z", g_pUndoManager->getUndoText());
	}
	else
	{
		mii.cch = sprintf(str, "Can't undo\tCtrl+Z");
	}
	SetMenuItemInfoA(g_hMenu, ID_EDIT_UNDO, FALSE, &mii);

	mii.fState = g_pUndoManager->canRedo() ? MFS_ENABLED : MFS_DISABLED;
	if(g_pUndoManager->canRedo())
	{
		mii.cch = sprintf(str, "Redo %s\tCtrl+Y", g_pUndoManager->getRedoText());
	}
	else
	{
		mii.cch = sprintf(str, "Can't redo\tCtrl+Y");
	}
	SetMenuItemInfoA(g_hMenu, ID_EDIT_REDO, FALSE, &mii);
}

void XUpdatePropWindow()
{
	if(!g_pPropWindow)
	{
		return;
	}
	g_propertyCallback.onApply();

	const char *szFirstType = NULL;
	const char *szFirstClass = NULL;
	bool bDifferentClasses = false;
	bool bDifferentTypes = false;
	struct prop_item_s
	{
		X_PROP_FIELD xPropField;
		bool isEnabled;
		const char *szValue;
	};
	AssotiativeArray<AAString, prop_item_s> mProps;
	g_pPropWindow->clearClassList();
	g_pPropWindow->clearProps();
	for(UINT i = 0, l = g_pLevelObjects.size(); i < l; ++i)
	{
		IXEditorObject *pObj = g_pLevelObjects[i];
		if(pObj->isSelected())
		{
			if(!szFirstType)
			{
				szFirstType = pObj->getTypeName();
				szFirstClass = pObj->getClassName();
			}
			else
			{
				if(!bDifferentTypes)
				{
					bDifferentTypes = fstrcmp(szFirstType, pObj->getTypeName()) != 0;
				}
				if(!bDifferentTypes && !bDifferentClasses)
				{
					bDifferentClasses = fstrcmp(szFirstClass, pObj->getClassName()) != 0;
				}
			}

			for(UINT i = 0, l = pObj->getProperyCount(); i < l; ++i)
			{
				const X_PROP_FIELD *pField = pObj->getPropertyMeta(i);

				const AssotiativeArray<AAString, prop_item_s>::Node *pNode;
				if(mProps.KeyExists(AAString(pField->szKey), &pNode))
				{
					if(pNode->Val->isEnabled)
					{
						if(pField->editorType != pNode->Val->xPropField.editorType
							|| fstrcmp(pField->szName, pNode->Val->xPropField.szName)
							|| pField->pEditorData != pNode->Val->xPropField.pEditorData
							)
						{
							pNode->Val->isEnabled = false;
						}
						else if(fstrcmp(pField->szHelp, pNode->Val->xPropField.szHelp))
						{
							pNode->Val->xPropField.szHelp = "";
						}

						if(pNode->Val->isEnabled)
						{
							if(fstrcmp(pObj->getKV(pField->szKey), pNode->Val->szValue))
							{
								pNode->Val->szValue = "";
							}
						}
					}
				}
				else
				{
					mProps[AAString(pField->szKey)] = {*pField, true, pObj->getKV(pField->szKey)};
				}
			}
		}
	}

	// Nothing selected
	if(!szFirstType)
	{
		return;
	}

	if(!bDifferentTypes)
	{
		// add classes for type
		const AssotiativeArray<AAString, IXEditable*>::Node *pNode;
		if(g_mEditableSystems.KeyExists(AAString(szFirstType), &pNode))
		{
			for(UINT i = 0, l = (*pNode->Val)->getClassCount(); i < l; ++i)
			{
				g_pPropWindow->addClass((*pNode->Val)->getClass(i));
			}
		}

		if(!bDifferentClasses)
		{
			//@TODO: uncomment this
			//g_pPropWindow->allowClassChange(true);
			g_pPropWindow->setClassName(szFirstClass);
		}
		else
		{
			g_pPropWindow->allowClassChange(false);
		}
	}
	else
	{
		g_pPropWindow->addClass("");
		g_pPropWindow->allowClassChange(false);
		g_pPropWindow->setClassName("");
	}

	for(AssotiativeArray<AAString, prop_item_s>::Iterator i = mProps.begin(); i; i++)
	{
		g_pPropWindow->addPropField(&i.second->xPropField, i.second->szValue);
	}

}
