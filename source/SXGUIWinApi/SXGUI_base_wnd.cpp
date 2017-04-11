
#include <SXGUIWinApi\SXGUI_base_wnd.h>

LRESULT CALLBACK TmpWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

SXGUIBaseWnd::SXGUIBaseWnd(	
					const char* class_name,const char* caption,const char* menu,
					WORD id,WORD x,WORD y,WORD width,WORD heigth,
					HICON icon,HCURSOR cursor, HBRUSH brush,
					DWORD exstyle,DWORD wndstyle,DWORD style,
					HWND parent,WNDPROC handler
					)
{
	WNDCLASS wc;

	wc.style         = wndstyle;
	wc.lpfnWndProc   = DefWindowProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = GetModuleHandle(0);
	wc.hIcon         = icon != 0 ? icon : LoadCursor(GetModuleHandle(0), IDC_ARROW);
	wc.hCursor       = cursor;
	wc.hbrBackground = brush != 0 ? brush : (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = menu;
	wc.lpszClassName = class_name;

		if(!RegisterClass(&wc)) 
		{
			MessageBox(0, "Ошибка при регистрации класса окна!", class_name, 0);
		}
		
	this->WindowHandle = CreateWindowEx(
							exstyle,
							class_name,
							caption,
							style|WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
							x,y,width,heigth,
							parent,
							HMENU(id),
							GetModuleHandle(0),
							0); 

		if (!this->WindowHandle)
		{
			MessageBox(0, "Ошибка создания окна!!!", caption, 0);
		}
	this->Init(this->WindowHandle, parent, handler);
	SetWindowLong(this->WindowHandle, GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->InitComponent();
	ShowWindow(this->WindowHandle, SW_SHOW);
	UpdateWindow(this->WindowHandle);
}

SXGUIBaseWnd::~SXGUIBaseWnd()
{
	/*char ClassName[256];
	int error = GetClassName(this->WindowHandle,ClassName,256);
	DestroyWindow(this->WindowHandle);
	UnregisterClass(ClassName,GetModuleHandle(0));*/
}

bool SXGUIBaseWnd::AlphaBlend(bool bf)
{
	return this->ModifyExStyle(bf ? WS_EX_LAYERED : 0,!bf ? WS_EX_LAYERED : 0);
}

bool SXGUIBaseWnd::AlphaBlend()
{
	long style = GetWindowLong(this->WindowHandle, GWL_EXSTYLE);
		if(style & WS_EX_LAYERED)
			return true;
		else
			return false;
}

bool SXGUIBaseWnd::Alpha(WORD alpha)
{
	return SetLayeredWindowAttributes(this->WindowHandle, 0, alpha, LWA_ALPHA);
}

WORD SXGUIBaseWnd::Alpha()
{
	WORD alpha = 0;
	if (!GetLayeredWindowAttributes(this->WindowHandle, 0, (BYTE*)&alpha, 0))
			return -1;
	return alpha;
}

bool SXGUIBaseWnd::SetColorBrush(BYTE r,BYTE g,BYTE b)
{
	return SetColorBrush(RGB(r, g, b));
}

bool SXGUIBaseWnd::SetColorBrush(DWORD color)
{
	DWORD color2 = SetClassLong(this->WindowHandle, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(color));
	InvalidateRect(this->WindowHandle, 0, TRUE);
	return color2 > 0 ? true : false;
}

DWORD SXGUIBaseWnd::GetColorBrush()
{
	HBRUSH hbrush = (HBRUSH)GetClassLong(this->WindowHandle, GCL_HBRBACKGROUND);
	LOGBRUSH lbr;
	ZeroMemory(&lbr,sizeof(LOGBRUSH));
	GetObject(hbrush,sizeof(LOGBRUSH),&lbr);
	return (lbr.lbColor);
}

HBRUSH SXGUIBaseWnd::GetBrush()
{
	//return GetClassLongPtr(this->WindowHandle,GCLP_HBRBACKGROUND);
	return (HBRUSH)GetClassLong(this->WindowHandle, GCL_HBRBACKGROUND);
}