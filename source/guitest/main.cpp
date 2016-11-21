#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crtdbg.h>
#include <io.h>
#include <fcntl.h>

#ifndef _SX_D3D9_H_
#	define _SX_D3D9_H_
#	ifdef _DEBUG
#		define D3D_DEBUG_INFO
#	endif
#	include <d3d9.h>
#	include <d3dx9.h>
#	include <d3dcompiler.h>
#	include <d3dx9math.h>
#	pragma comment(lib, "d3dcompiler.lib")
#	pragma comment(lib, "d3d9.lib")
#	ifdef _DEBUG
#		pragma comment(lib, "d3dx9d.lib")
#	else
#		pragma comment(lib, "d3dx9.lib")
#	endif
#endif

#include <gui/guimain.h>

HRESULT InitWindow(HINSTANCE hInstance, int nCmSXhow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitDevice();
void CleanupDevice();

HWND g_hWnd;
HINSTANCE g_hInst;
IDirect3D9 * g_pD3D;
D3DPRESENT_PARAMETERS pp;
IDirect3DSwapChain9 * g_pSwapChain = NULL;
IDirect3DDevice9 * g_pd3dDevice = NULL;

GUI::IGUI * pGui;

GUI::IDesktop * pDesk;

void Render()
{
	HRESULT hr = g_pd3dDevice->TestCooperativeLevel();
	if(hr == D3DERR_DEVICENOTRESET)
	{
		g_pd3dDevice->Reset(&pp);
#ifdef _COMPULE_GUI
		//SXGUI::HandleDeviceLost();
#endif
	}


	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(16, 32, 48), 1.0f, 0);

	pGui->Render();
	//g_pd3dDevice->BeginScene();


	//g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);



	//g_pd3dDevice->EndScene();

	g_pSwapChain->Present(NULL, NULL, NULL, NULL, D3DSWAPEFFECT_DISCARD);
}


void cbExit(GUI::IEvent * ev)
{
	PostQuitMessage(0);
}

void cbCancel(GUI::IEvent * ev)
{
	pGui->PopDesktop();
}

void cbExitPrompt(GUI::IEvent * ev)
{
	if(ev->key == KEY_ESCAPE)
	{
		pGui->MessageBox(L"Вы действительно хотите выйти?", L"", L"Да", L"on_exit", L"Нет", L"on_cancel", NULL);
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmSXhow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 249;
	MSG msg = {0};
	if(AllocConsole())
	{
		int hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 4);
		*stdout = *(::_fdopen(hCrt, "w"));
		::setvbuf(stdout, NULL, _IONBF, 0);
		*stderr = *(::_fdopen(hCrt, "w"));
		::setvbuf(stderr, NULL, _IONBF, 0);
		std::ios::sync_with_stdio();
		_setmode(_fileno(stdout), _O_U16TEXT);
	}
	//GUI::
	//GUI::IDesktop * dp;
	//dp->GetDocument()->GetElementById(L"asd")->GetStyle()->background_attachment->GetInt();
	//dp->

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	if(FAILED(InitWindow(hInstance, nCmSXhow)))
	{
		MessageBoxW(NULL, L"Ошибка при создании окна", L"Ошибка", MB_OK);
		return(0);
	}

	if(FAILED(InitDevice()))
	{
		MessageBoxW(NULL, L"Ошибка при инициализации DirectX", L"Ошибка", MB_OK);
		CleanupDevice();
		return(0);
	}
	HMODULE hDLL = LoadLibrary(L"gui.dll");
	if(!hDLL)
	{
		MessageBoxW(NULL, L"Ошибка загрузки gui.dll", L"Ошибка", MB_OK);
		CleanupDevice();
		return(0);
	}

	GUI::PFNINITINSTANCE pfnGUIInit;
	pfnGUIInit = (GUI::PFNINITINSTANCE)GetProcAddress(hDLL, "InitInstance");

	if(!pfnGUIInit)
	{
		MessageBoxW(NULL, L"Не найдена точка входа в процедуру InitInstance в библиотеке gui.dll", L"Ошибка", MB_OK);
		CleanupDevice();
		return(0);
	}
	pGui = pfnGUIInit(g_pd3dDevice, "../data/gui", g_hWnd);
	pGui->RegisterCallback("on_exit", cbExit);
	pGui->RegisterCallback("on_cancel", cbCancel);
	pGui->RegisterCallback("exit_prompt", cbExitPrompt);
	

	pDesk = pGui->CreateDesktopA("main_menu", "main_menu.html");
	pGui->SetActiveDesktop(pDesk);

	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//g_fpctext.Render();
		pGui->Update();
		pGui->Syncronize();
		Render();
	}

	CleanupDevice();

	//printf("_CrtDumpMemoryLeaks() = %d\n", _CrtDumpMemoryLeaks());
	return((int)msg.wParam);
}



HRESULT InitWindow(HINSTANCE hInstance, int nCmSXhow)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW/* | CS_DBLCLKS*/;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"SXengineWindowClass";
	//wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON1);
	//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.hIconSm = NULL;
	//SHOW_ERROR;
	if(!RegisterClassEx(&wcex))
	{
		MessageBoxW(NULL, L"Ошибка при регистрации класса окна", L"Ошибка", MB_OK);
		return(E_FAIL);
	}



	g_hInst = hInstance;
	RECT rc = {0, 0, 1366, 768};
	//RECT rc = {0, 0, 1800, 1000};
	//RECT rc = {0, 0, 800, 600};
	//AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);
	//g_hWnd = CreateWindow(L"SXengineWindowClass", L"SXengine", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	g_hWnd = CreateWindow(L"SXengineWindowClass", L"FPS Test", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	//g_hWnd = CreateWindow(L"SXengineWidowClass", L"SXengine", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if(!g_hWnd)
	{
		MessageBoxW(NULL, L"Ошибка при создании окна", L"Ошибка", MB_OK);
		return(E_FAIL);
	}
	ShowWindow(g_hWnd, nCmSXhow);
	return(S_OK);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//SXGUIEvent event;
	static int wheelDelta = 0;
	/*if(!SXGUI::PutMessage(message, wParam, lParam))
	{
	return(NULL);
	}*/
	switch(message)
	{

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case KEY_F5:
			pDesk->Release();
			pDesk = pGui->CreateDesktopA("main_menu", "main_menu.html");
			pGui->SetActiveDesktop(pDesk);
			break;
		case KEY_ESCAPE:
			//pGui->MessageBox(L"Вы действительно хотите выйти?", L"", L"Да", L"on_exit", L"Нет", L"on_cancel", NULL);
			break;
		}
		break;
	}

	if(pGui)
	{
		pGui->PutMessage(message, wParam, lParam);
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));
}

HRESULT InitDevice()
{

	HRESULT hr = S_OK;
	RECT rc;
	GetClientRect(g_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(!g_pD3D)
	{
		return(S_FALSE);
	}

	ZeroMemory(&pp, sizeof(pp));
	pp.Windowed = true;
	pp.BackBufferFormat = D3DFMT_UNKNOWN;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.BackBufferWidth = 1366;
	pp.BackBufferHeight = 768;
	//pp.BackBufferWidth = 800;
	//pp.BackBufferHeight = 600;
	pp.BackBufferCount = 1;
	pp.hDeviceWindow = g_hWnd;
	pp.MultiSampleQuality = 0;
	//pp.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
	pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	pp.EnableAutoDepthStencil = true;
	pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &pp, &g_pd3dDevice);
	if(FAILED(hr))
	{
		return(hr);
	}

	g_pd3dDevice->GetSwapChain(0, &g_pSwapChain);

	return(S_OK);
}

void CleanupDevice()
{
	if(pGui)
	{
		pGui->Release();
		pGui = NULL;
	}

	g_pd3dDevice->Release();
	g_pD3D->Release();

	DestroyWindow(g_hWnd);
	g_hWnd = NULL;
	UnregisterClassW(L"SXengineWindowClass", g_hInst);
}

