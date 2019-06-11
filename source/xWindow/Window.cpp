#include "Window.h"

#if defined(_WINDOWS)
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#endif

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CWindow *pWindow = (CWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	
	if(pWindow)
	{
		switch(msg)
		{
		case WM_DESTROY:
		case WM_CLOSE:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_ACTIVATE:
		case WM_SIZE:
		case WM_COMMAND:
		case WM_KILLFOCUS:
		case WM_XBUTTONDBLCLK:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_INPUT:
		case WM_SETCURSOR:
		case WM_MOUSEMOVE:
			return(pWindow->runCallback(msg, wParam, lParam));
		}
	}
	return(DefWindowProc(hWnd, msg, wParam, lParam));
}

CWindow::CWindow(HINSTANCE hInst, UINT uId, const XWINDOW_DESC *pWindowDesc, IXWindowCallback *pCallback, IXWindow *pParent):
	m_hInst(hInst),
	m_uId(uId),
	m_pCallback(pCallback)
{
	assert(pCallback);

	m_windowDesc = *pWindowDesc;

#if defined(_WINDOWS)
	UINT style = CS_HREDRAW | CS_VREDRAW;

	if(!(pWindowDesc->flags & XWF_BUTTON_CLOSE))
	{
		style |= CS_NOCLOSE;
	}

	char szClassName[64];
	sprintf_s(szClassName, "XWindowClass_%u", uId);

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = style;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassName;
	wcex.hIconSm = NULL;
	//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	if(!RegisterClassEx(&wcex))
	{
		LibReport(REPORT_MSG_LEVEL_FATAL, "Unable to register window class!\n");
	}

	//#############################################################################

	RECT rc = {0, 0, pWindowDesc->iSizeX, pWindowDesc->iSizeY};
	DWORD wndStyle = 0;

	if(pWindowDesc->flags & XWF_BUTTON_MINIMIZE)
	{
		wndStyle |= WS_MINIMIZEBOX | WS_SYSMENU;
	}
	if(pWindowDesc->flags & XWF_BUTTON_MAXIMIZE)
	{
		wndStyle |= WS_MAXIMIZEBOX | WS_SYSMENU;
	}
	if(pWindowDesc->flags & XWF_BUTTON_CLOSE)
	{
		wndStyle |= WS_SYSMENU;
	}
	if(pWindowDesc->flags & XWF_NOBORDER)
	{
		wndStyle &= ~(WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MAXIMIZE | WS_MINIMIZE | WS_THICKFRAME);
	}
	AdjustWindowRect(&rc, wndStyle, false);

	int iPosX = pWindowDesc->iPosX;
	int iPosY = pWindowDesc->iPosY;

	if(iPosX == XCW_USEDEFAULT)
	{
		iPosX = CW_USEDEFAULT;
	}
	if(iPosY == XCW_USEDEFAULT)
	{
		iPosY = CW_USEDEFAULT;
	}

	m_hWnd = CreateWindowA(szClassName, pWindowDesc->szTitle, wndStyle, iPosX, iPosY, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, wcex.hInstance, NULL);
	if(!m_hWnd)
	{
		LibReport(REPORT_MSG_LEVEL_FATAL, "Unable to create window!\n");
	}

	if(pWindowDesc->flags & XWF_TRANSPARENT)
	{
		DWM_BLURBEHIND dwmBlur = {0};
		dwmBlur.dwFlags = DWM_BB_ENABLE;
		dwmBlur.fEnable = TRUE;

		DwmEnableBlurBehindWindow(m_hWnd, &dwmBlur);
	}

	SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG)this);

	ShowWindow(m_hWnd, SW_NORMAL);


	if(pWindowDesc->flags & XWF_NOBORDER)
	{
		LONG lStyle = GetWindowLong(m_hWnd, GWL_STYLE);
		lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
		SetWindowLong(m_hWnd, GWL_STYLE, lStyle);
	}
#endif
}
CWindow::~CWindow()
{
#if defined(_WINDOWS)
	char szClassName[64];
	sprintf_s(szClassName, "XWindowClass_%u", m_uId);

	UnregisterClassA(szClassName, m_hInst);
	DestroyWindow(m_hWnd);
#endif
}

XWINDOW_OS_HANDLE XMETHODCALLTYPE CWindow::getOSHandle()
{
	return(m_hWnd);
}

void XMETHODCALLTYPE CWindow::hide()
{
	ShowWindow(m_hWnd, SW_MINIMIZE);
}
void XMETHODCALLTYPE CWindow::close()
{
	ShowWindow(m_hWnd, SW_HIDE);
}
void XMETHODCALLTYPE CWindow::show()
{
	ShowWindow(m_hWnd, SW_SHOW);
}
bool XMETHODCALLTYPE CWindow::isVisible()
{
	return(!!IsWindowVisible(m_hWnd));
}
void XMETHODCALLTYPE CWindow::setTitle(const char *szTitle)
{
	SetWindowTextA(m_hWnd, szTitle);
}
void XMETHODCALLTYPE CWindow::update(const XWINDOW_DESC *pWindowDesc)
{
	SetClassLong(m_hWnd, GCL_STYLE, GetClassLong(m_hWnd, GCL_STYLE) | CS_NOCLOSE);
}

INT_PTR XMETHODCALLTYPE CWindow::runDefaultCallback(UINT msg, WPARAM wParam, LPARAM lParam)
{
	return(DefWindowProcA(m_hWnd, msg, wParam, lParam));
}

INT_PTR CWindow::runCallback(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(m_pCallback)
	{
		return(m_pCallback->onMessage(msg, wParam, lParam, this));
	}
	return(runDefaultCallback(msg, wParam, lParam));
}