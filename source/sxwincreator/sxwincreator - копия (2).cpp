
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
	//главное окно
	SXGUIBaseWnd* MainWnd;

	//окно с параметрами элемента
	//SXGUIBaseWnd* ParamWnd;
	//SXGUIBaseWnd* TableParamWnd;
	//SXGUIStatic* StaticTableParamWnd;
	RECT* MainWndOldRect;

	SXGUIToolBar* SettingsPanel;
	SXGUIToolBar* ToolsPanel;

	//типа панель элементов
	SXGUIButtonImg* TPButton;
	SXGUIButtonImg* TPEdit;
	SXGUIButtonImg* TPCheckBox;
	SXGUIButtonImg* TPComboBox;
	SXGUIButtonImg* TPGroupBox;
	SXGUIButtonImg* TPListBox;
	SXGUIButtonImg* TPMemo;
	SXGUIButtonImg* TPProgressBar;
	SXGUIButtonImg* TPRadioButton;
	SXGUIButtonImg* TPStatic;
	SXGUIButtonImg* TPTrackBar;

	SXGUIButtonImg* TPStatusBar;
	SXGUIButtonImg* TPToolBar;
	SXGUIButtonImg* TPUpDown;
	SXGUIButtonImg* TPWindow;
	SXGUIButtonImg* TPMainMenu;
	SXGUIButtonImg* TPPopupMenu;


	SXGUIStatic* StaticCurrentWinObj;
	SXGUIComboBox* CurrentWinObj;

	SXGUIStatusBar* StatusBar;
	SXGUIButton* Button;
	SXGUIStatic* Static;

	SXGUIMenu* MainMenu;

	//
	SXGUIBaseWnd* JobMainWnd;

};

namespace SXBoolData
{
	bool ResizeScene	= false;
};

bool Render()
{
	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),&rect_main_wnd);

		if(rect_main_wnd.right != SXMainWndElem::MainWndOldRect->right && rect_main_wnd.bottom != SXMainWndElem::MainWndOldRect->bottom)
			SendMessage(SXMainWndElem::StatusBar->GetHWND(),WM_SIZE,0,0);
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

LRESULT SizeChangeForStatusBar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
		if(SXMainWndElem::StatusBar != 0)
		{
			//MessageBox(0,0,0,0);
			SendMessage(SXMainWndElem::StatusBar->GetHWND(),WM_SIZE,0,0);
			//SXMainWndElem::StatusBar->Update();
		}

	return TRUE;
}

LRESULT MouseWheel(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//MessageBox(0,0,0,0);
	int tmp, off;
	tmp = off = GetScrollPos(hwnd, SB_VERT);
	off = (HIWORD(wParam) == 0x78) ? off-10 : off+10;
	SetScrollPos(hwnd, SB_VERT, off, TRUE);
	off = GetScrollPos(hwnd, SB_VERT);
		if(tmp != off) 
		{
			ScrollWindow(hwnd, 0, tmp - off, NULL, NULL);
			UpdateWindow(hwnd);
			//ScrollWindow(SXMainWndElem::TableParamWnd->GetHWND(), 0, tmp - off, NULL, NULL);
			//UpdateWindow(SXMainWndElem::TableParamWnd->GetHWND());
		}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT ScrollBarCom(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int qwer=0;
		if(LOWORD(wParam) == SB_LINEUP || LOWORD(wParam) == SB_LINEDOWN)
		{
			int tmp, off;
			tmp = GetScrollPos(hwnd, SB_VERT);

			off = (LOWORD(wParam) == SB_LINEUP) ? -10 : +10;
			SetScrollPos(hwnd, SB_VERT, tmp + off, TRUE);
			off = GetScrollPos(hwnd, SB_VERT);

				if(tmp != off) 
				{
					ScrollWindow(hwnd, 0, tmp - off, NULL, NULL);
					UpdateWindow(hwnd);
				}
			qwer = off;
		}
		else if(LOWORD(wParam) == SB_THUMBPOSITION || LOWORD(wParam) == SB_THUMBTRACK)
		{
			int qwerqwerqwer = 0;
			
				if(qwer != HIWORD(wParam))
				{
					qwerqwerqwer = qwer - HIWORD(wParam);
					SetScrollPos(hwnd, SB_VERT, HIWORD(wParam), TRUE);
					ScrollWindow(hwnd, 0, qwer - HIWORD(wParam), NULL, NULL);
					UpdateWindow(hwnd);
				}
			qwer = HIWORD(wParam);
		}


	return DefWindowProc(hwnd, msg, wParam, lParam);
}




int WINAPI WinMain(HINSTANCE hinstance,HINSTANCE prevInstance,PSTR cmdLine,int showCmd)
{
	srand((unsigned int)time(0));
	SXEngine::Hinstance = hinstance;

	
	SXGUIRegClass::RegButtonImg();
	SXGUIRegClass::RegToolBar();

	INITCOMMONCONTROLSEX icex;
    
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);

	RECT* wrect = new RECT;
	SystemParametersInfo(SPI_GETWORKAREA, 0, wrect, 0);
	int WidthDeskTop = wrect->right;
	int JobHeightDeskTop = wrect->bottom;

	//int WidthDeskTop = 1600;
	//int JobHeightDeskTop = 800;

	SXMainWndElem::MainWnd = new SXGUIBaseWnd(
							"SkyX Windows Creator","SkyX Windows Creator",0,
							0,0,0,WidthDeskTop,150,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							0,CS_HREDRAW | CS_VREDRAW,WS_DLGFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
							0,0);

	SXMainWndElem::ParamWnd = new SXGUIBaseWnd(
							"ParamWnd","ParamWnd",0,
							0,WidthDeskTop-256,160,256,JobHeightDeskTop-160,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							WS_EX_TOOLWINDOW,CS_HREDRAW | CS_VREDRAW,WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU | WS_OVERLAPPED,
							0,0);

	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::ParamWnd);
	//SXMainWndElem::ParamWnd->AddHandler(MouseWheel,WM_MOUSEWHEEL,0,0,0,0,true);

	SXMainWndElem::TableParamWnd = new SXGUIBaseWnd(
							"TableParamWnd","TableParamWnd",0,
							//0,3,60,250,JobHeightDeskTop-220,
							0,10,60,200,JobHeightDeskTop-300,
							0,0,CreateSolidBrush(RGB(200,200,200)),
							0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_VSCROLL,
							SXMainWndElem::ParamWnd->GetHWND(),0);

	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::TableParamWnd);
	SXMainWndElem::TableParamWnd->AddHandler(MouseWheel,WM_MOUSEWHEEL,0,0,0,0,true);
	SXMainWndElem::TableParamWnd->AddHandler(ScrollBarCom,WM_VSCROLL,0,0,0,0,true);

	//SXGUIFuctinon::ScroolBars(SXMainWndElem::TableParamWnd,false,true);

	//SXGUIFuctinon::ScrollLine(SXMainWndElem::TableParamWnd,SXGUI_VERT_SCROLL,SXGUI_RIGTH_SCROLL,10);

	SXMainWndElem::StaticTableParamWnd = new SXGUIStatic("Текущий выбранный объект:",5,600,240,24,SXMainWndElem::TableParamWnd->GetHWND(),0,0);
	SXGUIStatic* StaticTableParamWndqq = new SXGUIStatic("____________",5,JobHeightDeskTop-320,240,20,SXMainWndElem::TableParamWnd->GetHWND(),0,0);
	

	

	/*SXMainWndElem::PanelManag = new SXGUIGroupBox(0,SXMainWndElem::MainWndOldRect->right - 257,51,256,SXMainWndElem::MainWndOldRect->bottom - 75,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::PanelManag->GAlign.top = true;
	SXMainWndElem::PanelManag->GAlign.left = false;
	SXMainWndElem::PanelManag->GAlign.bottom = true;
	SXMainWndElem::PanelManag->GAlign.right = true;*/

	SXMainWndElem::StaticCurrentWinObj = new SXGUIStatic("Текущий выбранный объект:",5,5,240,24,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::StaticCurrentWinObj->Align(SXGUI_STATIC_ALIGN_CENTER);
	SXMainWndElem::StaticCurrentWinObj->ColorBk = SXColor(220,220,220);
	SXMainWndElem::StaticCurrentWinObj->SetColorBrush(220,220,220);
	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::StaticCurrentWinObj);

	SXMainWndElem::CurrentWinObj = new SXGUIComboBox("",5,25,240,150,SXMainWndElem::ParamWnd->GetHWND(),0,0);
	SXMainWndElem::CurrentWinObj->GAlign.top = true;
	SXMainWndElem::CurrentWinObj->GAlign.left = true;
	SXMainWndElem::CurrentWinObj->GAlign.bottom = false;
	SXMainWndElem::CurrentWinObj->GAlign.right = true;

	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::MainWnd);
	//SXMainWndElem::MainWnd->AddHandler(SizeChangeForStatusBar,WM_SIZE);
	//SXMainWndElem::MainWnd->AddHandler(SizeChangeForStatusBar,WM_SIZING);
	//SXMainWndElem::MainWnd->AddHandler(SizeChangeForStatusBar,WM_SYSCOMMAND,SC_MAXIMIZE,true,0,0,false);

	SXMainWndElem::MainWnd->BFMinSize = true;
	SXMainWndElem::MainWnd->MinSizeX = 1600;
	SXMainWndElem::MainWnd->MinSizeY = 150;

	SXMainWndElem::MainWnd->BFSizingChangeLeft = true;
	SXMainWndElem::MainWnd->BFSizingChangeTop = true;

	SXMainWndElem::MainWnd->BFSizingChangeRight = true;
	SXMainWndElem::MainWnd->BFSizingChangeBottom = true;


	SXMainWndElem::MainMenu = new SXGUIMenu(IDR_MENU1);
	SXMainWndElem::MainMenu->SetToWindow(SXMainWndElem::MainWnd->GetHWND());

	SXMainWndElem::Button = new SXGUIButton("button",1000,1000,100,32,0,SXMainWndElem::MainWnd->GetHWND(),0,0);
	SXMainWndElem::Button->GAlign.top = false;
	SXMainWndElem::Button->GAlign.left = true;
	SXMainWndElem::Button->GAlign.bottom = true;
	SXMainWndElem::Button->GAlign.right = false;


	RECT rect_main_wnd;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),&rect_main_wnd);

	//SXMainWndElem::StatusBar = new SXGUIStatusBar("",0,0,0,0,SXMainWndElem::MainWnd->GetHWND(),0,0);
	/*SXMainWndElem::StatusBar->GAlign.top = false;
	SXMainWndElem::StatusBar->GAlign.left = true;
	SXMainWndElem::StatusBar->GAlign.bottom = true;
	SXMainWndElem::StatusBar->GAlign.right = true;*/

	/*int *arr = new int[5];
	arr[0] = 150; //всего моделей в сцене
	arr[1] = 300; //всего полигонов в сцене
	arr[2] = 450; //всего вершин в сцене
	arr[3] = 600; //всего подгрупп в сцене
	arr[4] = 750; //всего уникальных текстур

	SXMainWndElem::StatusBar->SetCountParts(5,arr);*/

	/*RECT rect_sb;
	GetClientRect(SXMainWndElem::StatusBar->GetHWND(),&rect_sb);*/

	SXMainWndElem::MainWndOldRect = new RECT;
	GetClientRect(SXMainWndElem::MainWnd->GetHWND(),SXMainWndElem::MainWndOldRect);

	SXMainWndElem::SettingsPanel = new SXGUIToolBar("",0,0,SXMainWndElem::MainWndOldRect->right,24,24,24,SXMainWndElem::MainWnd->GetHWND(),0,123);
	SXMainWndElem::SettingsPanel->GAlign.top = true;
	SXMainWndElem::SettingsPanel->GAlign.left = true;
	SXMainWndElem::SettingsPanel->GAlign.bottom = false;
	SXMainWndElem::SettingsPanel->GAlign.right = true;

	SXMainWndElem::ToolsPanel = new SXGUIToolBar("",0,25,SXMainWndElem::MainWndOldRect->right,31,24,24,SXMainWndElem::MainWnd->GetHWND(),0,123);
	SXMainWndElem::ToolsPanel->GAlign.top = true;
	SXMainWndElem::ToolsPanel->GAlign.left = true;
	SXMainWndElem::ToolsPanel->GAlign.bottom = false;
	SXMainWndElem::ToolsPanel->GAlign.right = true;

	SXGUIBaseHandlers::InitHandlerMsg(SXMainWndElem::MainWnd);

	SXMainWndElem::TPButton = new SXGUIButtonImg(IDB_BITMAP2,3,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPButton->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPButton->GAlign.top = true;
	SXMainWndElem::TPButton->GAlign.left = true;
	SXMainWndElem::TPButton->GAlign.bottom = false;
	SXMainWndElem::TPButton->GAlign.right = false;

	SXMainWndElem::TPEdit = new SXGUIButtonImg(IDB_BITMAP3,30,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPEdit->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPEdit->GAlign.top = true;
	SXMainWndElem::TPEdit->GAlign.left = true;
	SXMainWndElem::TPEdit->GAlign.bottom = false;
	SXMainWndElem::TPEdit->GAlign.right = false;

	SXMainWndElem::TPCheckBox = new SXGUIButtonImg(IDB_BITMAP4,57,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPCheckBox->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPCheckBox->GAlign.top = true;
	SXMainWndElem::TPCheckBox->GAlign.left = true;
	SXMainWndElem::TPCheckBox->GAlign.bottom = false;
	SXMainWndElem::TPCheckBox->GAlign.right = false;

	SXMainWndElem::TPComboBox = new SXGUIButtonImg(IDB_BITMAP5,84,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPComboBox->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPComboBox->GAlign.top = true;
	SXMainWndElem::TPComboBox->GAlign.left = true;
	SXMainWndElem::TPComboBox->GAlign.bottom = false;
	SXMainWndElem::TPComboBox->GAlign.right = false;


	SXMainWndElem::TPGroupBox = new SXGUIButtonImg(IDB_BITMAP6,111,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPGroupBox->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPGroupBox->GAlign.top = true;
	SXMainWndElem::TPGroupBox->GAlign.left = true;
	SXMainWndElem::TPGroupBox->GAlign.bottom = false;
	SXMainWndElem::TPGroupBox->GAlign.right = false;

	SXMainWndElem::TPListBox = new SXGUIButtonImg(IDB_BITMAP7,138,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPListBox->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPListBox->GAlign.top = true;
	SXMainWndElem::TPListBox->GAlign.left = true;
	SXMainWndElem::TPListBox->GAlign.bottom = false;
	SXMainWndElem::TPListBox->GAlign.right = false;


	SXMainWndElem::TPMemo = new SXGUIButtonImg(IDB_BITMAP8,165,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPMemo->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPMemo->GAlign.top = true;
	SXMainWndElem::TPMemo->GAlign.left = true;
	SXMainWndElem::TPMemo->GAlign.bottom = false;
	SXMainWndElem::TPMemo->GAlign.right = false;

	SXMainWndElem::TPProgressBar = new SXGUIButtonImg(IDB_BITMAP9,192,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPProgressBar->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPProgressBar->GAlign.top = true;
	SXMainWndElem::TPProgressBar->GAlign.left = true;
	SXMainWndElem::TPProgressBar->GAlign.bottom = false;
	SXMainWndElem::TPProgressBar->GAlign.right = false;

	SXMainWndElem::TPRadioButton = new SXGUIButtonImg(IDB_BITMAP10,219,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPRadioButton->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPRadioButton->GAlign.top = true;
	SXMainWndElem::TPRadioButton->GAlign.left = true;
	SXMainWndElem::TPRadioButton->GAlign.bottom = false;
	SXMainWndElem::TPRadioButton->GAlign.right = false;

	SXMainWndElem::TPStatic = new SXGUIButtonImg(IDB_BITMAP11,246,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPStatic->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPStatic->GAlign.top = true;
	SXMainWndElem::TPStatic->GAlign.left = true;
	SXMainWndElem::TPStatic->GAlign.bottom = false;
	SXMainWndElem::TPStatic->GAlign.right = false;

	SXMainWndElem::TPTrackBar = new SXGUIButtonImg(IDB_BITMAP12,273,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPTrackBar->FrameColor = SXColor(100,100,100);
	//SXMainWndElem::TPTrackBar = new SXGUIButtonImg(IDB_BITMAP12,273,256,24,24,RGB(0,0,0),RGB(200,200,200),SXMainWndElem::MainWnd->GetHWND(),0,1245);
	//SXMainWndElem::TPTrackBar->EnableBf = true;
	SXMainWndElem::TPTrackBar->GAlign.top = true;
	SXMainWndElem::TPTrackBar->GAlign.left = true;
	SXMainWndElem::TPTrackBar->GAlign.bottom = false;
	SXMainWndElem::TPTrackBar->GAlign.right = false;


	SXMainWndElem::TPStatusBar = new SXGUIButtonImg(IDB_BITMAP13,300,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPStatusBar->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPStatusBar->GAlign.top = true;
	SXMainWndElem::TPStatusBar->GAlign.left = true;
	SXMainWndElem::TPStatusBar->GAlign.bottom = false;
	SXMainWndElem::TPStatusBar->GAlign.right = false;

	SXMainWndElem::TPToolBar = new SXGUIButtonImg(IDB_BITMAP14,327,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPToolBar->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPToolBar->GAlign.top = true;
	SXMainWndElem::TPToolBar->GAlign.left = true;
	SXMainWndElem::TPToolBar->GAlign.bottom = false;
	SXMainWndElem::TPToolBar->GAlign.right = false;

	SXMainWndElem::TPUpDown = new SXGUIButtonImg(IDB_BITMAP15,354,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPUpDown->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPUpDown->GAlign.top = true;
	SXMainWndElem::TPUpDown->GAlign.left = true;
	SXMainWndElem::TPUpDown->GAlign.bottom = false;
	SXMainWndElem::TPUpDown->GAlign.right = false;

	SXMainWndElem::TPWindow = new SXGUIButtonImg(IDB_BITMAP16,381,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPWindow->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPWindow->GAlign.top = true;
	SXMainWndElem::TPWindow->GAlign.left = true;
	SXMainWndElem::TPWindow->GAlign.bottom = false;
	SXMainWndElem::TPWindow->GAlign.right = false;

	SXMainWndElem::TPMainMenu = new SXGUIButtonImg(IDB_BITMAP17,408,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPMainMenu->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPMainMenu->GAlign.top = true;
	SXMainWndElem::TPMainMenu->GAlign.left = true;
	SXMainWndElem::TPMainMenu->GAlign.bottom = false;
	SXMainWndElem::TPMainMenu->GAlign.right = false;

	SXMainWndElem::TPPopupMenu = new SXGUIButtonImg(IDB_BITMAP18,435,1,24,24,RGB(0,0,0),RGB(0,0,0),SXMainWndElem::ToolsPanel->GetHWND(),0,0);
	SXMainWndElem::TPPopupMenu->FrameColor = SXColor(100,100,100);
	SXMainWndElem::TPPopupMenu->GAlign.top = true;
	SXMainWndElem::TPPopupMenu->GAlign.left = true;
	SXMainWndElem::TPPopupMenu->GAlign.bottom = false;
	SXMainWndElem::TPPopupMenu->GAlign.right = false;



	

	//int WidthDeskTop = wrect->right;
	//int JobHeightDeskTop = wrect->bottom;

	SXMainWndElem::JobMainWnd = new SXGUIBaseWnd(
							"Window 1","Window 1",0,
							0,100,250,WidthDeskTop - 456,JobHeightDeskTop-350,
							0,0,CreateSolidBrush(RGB(220,220,220)),
							0,CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
							0,0);

	int minP,maxP;
	GetScrollRange(SXMainWndElem::TableParamWnd->GetHWND(),SB_VERT,&minP,&maxP);
	//MessageBox(0,ToPointChar(minP),ToPointChar(maxP),0);
//MessageBox(0,ToPointChar(JobHeightDeskTop-300),ToPointChar(maxP),0);
	//SetScrollRange(SXMainWndElem::TableParamWnd->GetHWND(),SB_VERT,0,30,1);*/

	SCROLLINFO lpsi;
	BOOL bf = GetScrollInfo(SXMainWndElem::TableParamWnd->GetHWND(),SB_VERT,&lpsi);
	lpsi.cbSize = sizeof(lpsi);
	lpsi.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	//lpsi.nMax = 100 + ((630-560) - 1); //100 + ((вся возможная область - область без прокрутки) - 1)
	//lpsi.nMin = 0;
	//lpsi.nPage = (100.0/630)*560.0; //(100.0/вся возможная область)*область без прокрутки

	//lpsi.nMax = 100 + (((630)-(560)) - 1); //100 + ((вся возможная область - область без прокрутки) - 1)
	//lpsi.nMin = 0;
	//lpsi.nPage = lpsi.nMax / 2.0;//(100.0/(630/1.5))*560.0; //(100.0/вся возможная область)*область без прокрутки 169

	lpsi.nMax = 630; //100 + ((вся возможная область - область без прокрутки) - 1)
	lpsi.nMin = 0;
	lpsi.nPage = 560;//(100.0/(630/1.5))*560.0; //(100.0/вся возможная область)*область без прокрутки 169
	lpsi.nPos = 10;

	SetScrollInfo(SXMainWndElem::TableParamWnd->GetHWND(),SB_VERT,&lpsi,1);


	/* siVertical.nMax = 100; 
            siVertical.nPage = iHeightArea / iVertStep;
            if ( siVertical.nPage > siVertical.nMax )
                siVertical.nPos = siVertical.nMin;
 
            iVertRange = siVertical.nMax - siVertical.nMin + 1;*/

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