
#pragma once

#define SX_EXE

#include <windows.h>
#include <ctime>
#pragma comment(lib, "winmm.lib")
#include <gdefines.h>
#if defined(_DEBUG)
#pragma comment(lib, "sxinput_d.lib")
#else
#pragma comment(lib, "sxinput.lib")
#endif
#include <input\\sxinput.h>
#include <core\\array.h>
#include <handler_out_log.cpp>
//#include <core\\sxcore.cpp>
#include <sxmath.h>


#if defined(_DEBUG)
#pragma comment(lib, "sxsound_d.lib")
#else
#pragma comment(lib, "sxsound.lib")
#endif
#include <sound\\sxsound.h>

/*#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif*/
#include <gcore\\sxgcore.cpp>

#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif
#include <core\\sxcore.cpp>

#if defined(_DEBUG)
#pragma comment(lib, "sxguiwinapi_d.lib")
#else
#pragma comment(lib, "sxguiwinapi.lib")
#endif
#include <sxguiwinapi\\sxguielements.h>


/*
class WndMsgTask : public SXTask
{
public:
	WndMsgTask() : SXTask(BACKGROUND_SYNC | REPEATING)
	{
	}
	~WndMsgTask()
	{
	}

	void run()
	{
		Sleep(1000);
	}

};*/

void runqq()
{
	Sleep(1000);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	ISXFile* tmpfile = Core_OpFile("D:\\project\\engine\\build\\log.txt", SXFILE_TEXT);

	int arg1 = 147;
	float arg2;
	int arg3;

	tmpfile->ReadT("%d%f%d", &arg1, &arg2, &arg3);
	/*SXTaskManager * pTaskManager = new SXTaskManager();

	///Добавляем задачи 
	pTaskManager->add(runqq);
	pTaskManager->add(runqq);

	//pTaskManager->add(SXTaskManager::TaskPtr(new WndMsgTask()));




	/// Добавляем задачу обработки сообщений windows
	//pTaskManager->add(SXTaskManager::TaskPtr(new WndMsgTask(this)));
	/// Запускаем выполнение
	pTaskManager->start();*/
	ISXLConfig* tmpconfig = Core_OpLConfig("D:\\project\\builds\\build_22_04_2016\\gamesource\\configs\\camera_fly\\test.cf");
	const char* tmpstr = tmpconfig->GetSectionName(0);
	Core_0Create("qqq", true);
	Core_MTaskAdd(runqq);
	Core_MTaskStart();
	//CreateElements[1]->Object;
	srand((unsigned int)time(0));
	SXGUIRegClass::RegButtonImg();
	//memcpy(test2, test3, sizeof(test2));
	//test2::release = test3::release;
	//test2::method1 = test3::method1;
	ISXGUIBaseWnd* tmpwin = SXGUICrBaseWnd("SkyLandEditor", "SkyLandEditor", 0, 0, 273, 131, 657, 585, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0);
	//tmpwin->SetColorBrush(255, 0, 0);
	tmpwin->SetText("qqqqqqqqqqqqqqqqqqqqqqqqq");
	SXGUIBaseHandlers::InitHandlerMsg(tmpwin);
	ISXGUIButtonImg* ImgNew = SXGUICrButtonImgEx("D:\\project\\engine\\old source\\SkyX Engine source\\sxwincreator\\ресурсы\\меню\\new.bmp", 3, 1, 30, 30, RGB(255, 0, 110), RGB(220, 220, 220), tmpwin->GetHWND(), 0, 0);
	ImgNew->InitCallBack();
	ImgNew->Visible(true);
	ImgNew->AlphaColor = RGB(200, 100, 100);
	ImgNew->BkColor = RGB(250, 0, 0);
	/*ImgNew->EnableBf = true;
	ImgNew->EnableState = true;*/
	//SXGUIEnumChildWindow::EnumChildProcUpdateImgButton(0, 0);
	//ISXGUIStatic* tst = SXCreateStatic("3d модель:", 3, 5, 65, 15, tmpwin->GetHWND(), 0, 0);
	ISXGUIButton* tbut = SXGUICrButton("...", 300, 5, 25, 15, 0, tmpwin->GetHWND(), 0, 0);
	//SSound_0Create("test", 0, false);
	//SSound_Play(0);
	InitOutLog();
	//SSInput_Dbg_Set(printflog);
	//SSInput_0Create("test", 0, false);
	//SSInput_Update();
	

	float2 WidthHeight(800, 600);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = DefWindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(0);
	wcex.hIcon = NULL;
	wcex.hCursor = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "test";
	wcex.hIconSm = 0;

	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, WidthHeight.x, WidthHeight.y };
	AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);

	HWND hwnd3d3 = CreateWindowEx(
		0,
		"test",
		"test",
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0,
		rc.right - rc.left, rc.bottom - rc.top,
		0, 0,
		GetModuleHandle(0),
		0);

	ShowWindow(hwnd3d3, SW_SHOW);

		SGCore_0Create("qwert", hwnd3d3, WidthHeight.x, WidthHeight.y, true, 0, true);
		IDirect3DDevice9* DXDevice = SGCore_GetDXDevice();

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	
	static DWORD lastTime = timeGetTime();
	static DWORD TimeCCadr = 0;

		while(msg.message != WM_QUIT /*&& IsWindow(SkyXEngine::Core::Data::HandleWinD3D)*/)
		{
				if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				else
				{	
					static DWORD TimeStart = 0;
					DWORD TimeThis = timeGetTime();

					DWORD currTime  = timeGetTime();
					DWORD timeDelta = (currTime - lastTime);
					timeDelta = (timeDelta + TimeCCadr) / 2;
					//SkyXEngine::Core::Render::MainRender(timeDelta);

					DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						0x569874, 1.0f, 0);
					DXDevice->Present(0, 0, 0, 0); // показ вторичного буфера


					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

    return msg.wParam;
}