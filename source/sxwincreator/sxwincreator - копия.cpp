
#include <d3d9.h>
#include <d3dx9.h>
#include <ctime>
#include <core\sxconvertdata.cpp>
#include <SXGUIWinApi\SXGUI.h>
#include <редакторы\SXWinCreator\SXWinCreator\resource.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "Comctl32.lib")


namespace SXEngine
{
	HWND				Hwnd		= 0;
	HINSTANCE			Hinstance	= 0;
};

namespace SXMainWndElem
{
	SXGUIBaseWnd* MainWnd;
	RECT* MainWndOldRect;

	SXGUIStatusBar* StatusBar;
	SXGUIButton* Button;

	SXGUIMenu* MainMenu;

};

namespace SXBoolData
{
	bool ResizeScene	= false;
};

bool Render()
{
	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),&rect_main_wnd);

	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),SXMainWndElem::MainWndOldRect);
/*time_t t;
struct tm *t_m;
t=time(NULL);
t_m=localtime(&t);
//MessageBox(0,ToPointChar(DWORD(t_m->tm_hour)),0,0);
		//if(t_m->tm_hour == 1 && t_m->tm_min > 10)
		//{
			DWORD time = timeGetTime();
				while(1)
				{
						if(timeGetTime() - time >= 1000 * 60 * 20)
						{
							ShellExecute(0,0,"C:\\qwer.bat",0,0,SW_SHOWNORMAL);
							break;
						}
				}
		//}*/

	return true;
};

LRESULT MaximuzeWinChange(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//MessageBox(0,0,0,0);
	EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcUpdateSize,lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
		if(msg == WM_DESTROY)
		{
			::PostQuitMessage(0);
		}
		else if(msg == WM_CTLCOLORSTATIC || msg == WM_CTLCOLOREDIT || msg == WM_CTLCOLORBTN || msg == WM_CTLCOLORLISTBOX)
		{
			SXGUIComponent *Component = (SXGUIComponent *)GetWindowLong(hwnd,GWL_USERDATA);

				if(Component)
				{
					MessageBox(0,SXGUIFuctinon::GetText(Component),0,0);
						if(Component->TransparentBf)
							SetBkMode((HDC)wParam,TRANSPARENT);
						else
							SetBkColor((HDC)wParam, RGB(Component->ColorBk.r, Component->ColorBk.g, Component->ColorBk.b));
					SetTextColor((HDC)wParam, RGB(Component->ColorText.r, Component->ColorText.g, Component->ColorText.b));
	
					return (long)Component->HBrush;
				}
		}

		else if(msg == WM_SIZING)
        {
			RECT rc;
			GetWindowRect(hwnd, &rc);
			POINT p;
			GetCursorPos(&p);

			RECT lpRect = *((LPRECT)lParam);

			//MessageBox(0,ToPointChar(ToString(p.x) + "|" + ToString(p.y) + "||" + ToString(rc.right) + "|" + ToString(rc.bottom)),0,0);

				if(p.x <= rc.right - 17)
					lpRect.top = rc.top;

				if(p.y <= rc.bottom - 17)
					lpRect.left = rc.left;

				if(lpRect.right - lpRect.left < 640)
					lpRect.right += 640 - (lpRect.right - lpRect.left);

				if(lpRect.bottom - lpRect.top < 480)
					lpRect.bottom += 480 - (lpRect.bottom - lpRect.top);

			*((LPRECT)lParam) = lpRect;

			return TRUE;
        }

		else if(msg == WM_SIZE)
        {
			SXBoolData::ResizeScene = true;

				if(SXMainWndElem::StatusBar != 0)
					SXMainWndElem::StatusBar->Update();

			EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcUpdateSize,lParam);
			return TRUE;
        }

		else if(msg == WM_MOVE)
		{
			EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcUpdateRect,lParam);
			return TRUE;
		}

		else if(msg == WM_MOUSEMOVE)
		{
			EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcMouseMove,lParam);
		}

		else if(msg == WM_SETCURSOR)
		{
			SetCursor(LoadCursor(0, IDC_ARROW));
			EnumChildWindows(hwnd,(WNDENUMPROC)&SXGUIEnumChildWindow::EnumChildProcMouseMove,lParam);
		}

 
		else if(msg == WM_COMMAND)
        {
            WORD id = LOWORD(wParam);
        }

	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd)
{
	srand((unsigned int)time(0));
	SXEngine::Hinstance = hinstance;

	
	SXGUIRegClass::RegButtonImg();
	SXGUIRegClass::RegToolBar();

	SXMainWndElem::MainWnd = new SXGUIBaseWnd(
							"SkyX Windows Creator","SkyX Windows Creator",0,
							0,200,200,800,600,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							0,CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
							0,0);

	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::MainWnd);

	//MaximuzeWinChange

	SXMainWndElem::MainWnd->AddHandler(MaximuzeWinChange,WM_SYSCOMMAND,SC_MAXIMIZE,true,0,0);

	SXMainWndElem::MainWnd->BFMinSize = true;
	SXMainWndElem::MainWnd->MinSizeX = 640;
	SXMainWndElem::MainWnd->MinSizeY = 480;

	SXMainWndElem::MainWnd->BFSizingChangeLeft = true;
	SXMainWndElem::MainWnd->BFSizingChangeTop = true;

	SXMainWndElem::MainWnd->BFSizingChangeRight = true;
	SXMainWndElem::MainWnd->BFSizingChangeBottom = true;


	SXMainWndElem::MainMenu = new SXGUIMenu(IDR_MENU1);
	SXMainWndElem::MainMenu->SetToWindow(SXMainWndElem::MainWnd->GetHWND());

	SXMainWndElem::Button = new SXGUIButton("button",100,100,100,32,0,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::Button->GAlign.top = false;
	SXMainWndElem::Button->GAlign.left = true;
	SXMainWndElem::Button->GAlign.bottom = true;
	SXMainWndElem::Button->GAlign.right = false;


	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),&rect_main_wnd);

	SXMainWndElem::StatusBar = new SXGUIStatusBar("",0,0,0,0,SXMainWndElem::MainWnd->GetHWND(),0,0);

	int *arr = new int[5];
	arr[0] = 150; //всего моделей в сцене
	arr[1] = 300; //всего полигонов в сцене
	arr[2] = 450; //всего вершин в сцене
	arr[3] = 600; //всего подгрупп в сцене
	arr[4] = 750; //всего уникальных текстур

	SXMainWndElem::StatusBar->SetCountParts(5,arr);

	RECT rect_sb;
	GetClientRect(SXMainWndElem::StatusBar->GetHWND(),&rect_sb);

	SXMainWndElem::MainWndOldRect = new RECT;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),SXMainWndElem::MainWndOldRect);

	//SXInit::EnterMsgLoop(Render);

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

		while(msg.message != WM_QUIT)
		{
			if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{	
				Render();
			}
		}

	return msg.wParam;
}