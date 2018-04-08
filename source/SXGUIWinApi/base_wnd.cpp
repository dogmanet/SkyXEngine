
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "base_wnd.h"

CGUIBaseWnd::CGUIBaseWnd(	
					const char* class_name,const char* caption,
					int x,int y,int width,int heigth,
					HICON icon,HCURSOR cursor, HBRUSH brush,
					DWORD exstyle,DWORD wndstyle,DWORD style,
					HWND parent, WNDPROC handler, ID idObj
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
	wc.lpszMenuName  = 0;
	wc.lpszClassName = class_name;

		if(!RegisterClass(&wc)) 
		{
			MessageBox(0, "Ошибка при регистрации класса окна!", class_name, 0);
		}
		
	m_hWindow = CreateWindowEx(
							exstyle,
							class_name,
							caption,
							(parent != 0 ? WS_CHILD : 0) | style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
							x,y,width,heigth,
							parent,
							(idObj > 0 ? (HMENU)idObj : 0),
							GetModuleHandle(0),
							0); 

	if (!m_hWindow)
	{
			MessageBox(0, "Ошибка создания окна!!!", caption, 0);
	}
	this->init(m_hWindow, parent, handler, idObj);
	SetWindowLong(m_hWindow, GWL_USERDATA, (LONG)dynamic_cast<ISXGUIComponent*>(this));
	this->initComponent();
	ShowWindow(m_hWindow, SW_SHOW);
	UpdateWindow(m_hWindow);
}

CGUIBaseWnd::~CGUIBaseWnd()
{

}

bool CGUIBaseWnd::setAlphaBlend(bool bf)
{
	return this->modifyExStyle(bf ? WS_EX_LAYERED : 0,!bf ? WS_EX_LAYERED : 0);
}

bool CGUIBaseWnd::getAlphaBlend()
{
	long style = GetWindowLong(m_hWindow, GWL_EXSTYLE);
	return (style & WS_EX_LAYERED);
}

bool CGUIBaseWnd::setAlpha(float fAlpha)
{
	return SetLayeredWindowAttributes(m_hWindow, 0, (BYTE)(fAlpha * 255.f), LWA_ALPHA);
}

float CGUIBaseWnd::getAlpha()
{
	WORD alpha = 0;
	if (!GetLayeredWindowAttributes(m_hWindow, 0, (BYTE*)&alpha, 0))
			return -1;
	return float(alpha) / 255.f;
}

bool CGUIBaseWnd::setColorBrush(DWORD color)
{
	DWORD color2 = SetClassLong(m_hWindow, GCL_HBRBACKGROUND, (LONG)CreateSolidBrush(color));
	InvalidateRect(m_hWindow, 0, TRUE);
	return color2 > 0 ? true : false;
}

DWORD CGUIBaseWnd::getColorBrush()
{
	HBRUSH hbrush = (HBRUSH)GetClassLong(m_hWindow, GCL_HBRBACKGROUND);
	LOGBRUSH lbr;
	ZeroMemory(&lbr,sizeof(LOGBRUSH));
	GetObject(hbrush,sizeof(LOGBRUSH),&lbr);
	return (lbr.lbColor);
}

HBRUSH CGUIBaseWnd::getBrush()
{
	//return GetClassLongPtr(m_hWindow,GCLP_HBRBACKGROUND);
	return (HBRUSH)GetClassLong(m_hWindow, GCL_HBRBACKGROUND);
}