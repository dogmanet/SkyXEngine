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

#include <Shellapi.h>

#include <sxnet/INET.h>

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

#define C2M_RQ_PROFILE 6
#define C2M_RQ_GAMELIST 7
#define C2M_RQ_FRIENDLIST 8

#define M2C_PROFILE 5


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
NET::INET * pNET;
NET::IMasterClient * pMC;

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
	if(ev->key == KEY_ESCAPE || ev->key == KEY_LBUTTON)
	{
		pGui->MessageBox(L"Вы действительно хотите выйти?", L"", L"Да", L"on_exit", L"Нет", L"on_cancel", NULL);
	}
}

void cbPayPrompt(GUI::IEvent * ev)
{
	pGui->MessageBox(L"Пополнение баланса", L"Для пополнения баланса будет открыто окно браузера<br/>Продолжить?", L"Да", L"go_pay", L"Нет", L"on_cancel", NULL);
}

void cbPayGo(GUI::IEvent * ev)
{
	pGui->PopDesktop();
	ShellExecute(NULL, L"open", L"http://sip-game.su/balance", NULL, NULL, SW_SHOW);
}

void cbGameListRefresh(GUI::IEvent * ev)
{
	NET::INETbuff * buf = pNET->CreateBuffer();
	buf->writeInt8(C2M_RQ_GAMELIST);

	pMC->SendMessage(buf);

	pNET->ReleaseBuffer(buf);
}

void LoadClientInfo()
{
	wprintf(L"Load client profile\n");
	NET::INETbuff * buf = pNET->CreateBuffer();
	buf->writeInt8(C2M_RQ_PROFILE);
	buf->writeInt8(C2M_RQ_GAMELIST);
	buf->writeInt8(C2M_RQ_FRIENDLIST);

	pMC->SendMessage(buf);

	pNET->ReleaseBuffer(buf);
	//C2M_RQ_PROFILE 
}

Array<StringW> g_glMaps;
Array<StringW> g_glMapImgs;
Array<StringW> g_glMapDescs;

void evtGameList(const JSON * data)
{
	const JSON_ARRAY * list =& data->GetArray();
	const JSON_OBJECT * obj;

	GUI::DOM::IDOMnode * pRows = pDesk->GetDocument()->GetElementById(L"match_tbl_data");
	while(pRows->GetChilds()->size())
	{
		pRows->RemoveChild((*(pRows->GetChilds()))[0]);
	}


	
	StringW html;

	for(UINT i = 0, l = list->size(); i < l; ++i)
	{
		obj = &list[0][i]->GetObject();
		g_glMaps[i] = (*obj)[L"map"]->GetString();
		g_glMapDescs[i] = (*obj)[L"map_desc"]->GetString();
		g_glMapImgs[i] = (*obj)[L"map_img"]->GetString();
		html = StringW(L"<div onclick=\"game_select\" value=\"") + (int)i + L"\" class=\"tbl_row gl_item\">"
			L"<div class=\"col1\">" + (*obj)[L"status"]->GetString() + L"</div>"
			L"<div class=\"col2\">" + (*obj)[L"map"]->GetString() + L"</div>"
			L"<div class=\"col3\">" + (*obj)[L"mode"]->GetString() + L"</div>"
			L"<div class=\"col4\">" + (*obj)[L"players"]->GetString() + L"</div>"
			L"</div>";
		GUI::DOM::IDOMnode * pNewNode = pDesk->CreateFromText(html)[0];
		pRows->AppendChild(pNewNode);
	}
}

void cbGameListSelect(GUI::IEvent * ev)
{
	const GUI::DOM::IDOMnodeCollection * list = pDesk->GetDocument()->GetElementsByClass(L"gl_item");
	for(UINT i = 0, l = list->size(); i < l; ++i)
	{
		list[0][i]->SetAttribute(L"class", L"tbl_row gl_item");
		list[0][i]->UpdateStyles();
	}
	ev->currentTarget->SetAttribute(L"class", L"tbl_row gl_item active");
	int i = ev->currentTarget->GetAttribute(L"value").ToInt();
	pDesk->GetDocument()->GetElementById(L"map_name")->SetText(g_glMaps[i], TRUE);
	pDesk->GetDocument()->GetElementById(L"map_info")->SetText(g_glMapDescs[i], TRUE);
	GUI::DOM::IDOMnode * pNode = pDesk->GetDocument()->GetElementById(L"map_img");
	pNode->GetStyleSelf()->background_image->Set(StringW(L"/maps/") + g_glMapImgs[i]);
	pNode->UpdateStyles();
}

void cbOnLogin(UINT p, const StringW & token)
{
	GUI::DOM::IDOMnode * pNode = pGui->GetActiveDesktop()->GetDocument()->GetElementById(L"l_error");
	switch(p)
	{
	case NET::MSL_SUCCESS:
		pGui->SetActiveDesktopW(L"main_menu");
		{
			FILE * f = fopen("./token.bin", "wb");
			fwrite(token.c_str(), sizeof(WCHAR), token.length(), f);
			fclose(f);
		}
	case NET::MSL_ACCEPTOKEN:
		LoadClientInfo();
		break;
	case NET::MSL_BADPASS:
		if(pNode)
		{
			pNode->SetText(L"Неверный логин или пароль, проверьте данные и повторите ввод", TRUE);
			pNode->GetStyleSelf()->visibility->Set(GUI::CSS::ICSSproperty::VISIBILITY_VISIBLE);
			pNode->UpdateStyles();
		}
		break;
	case NET::MSL_ERROR:
		if(pNode)
		{
			pNode->SetText(L"Произошла ошибка, попробуйте позже", TRUE);
			pNode->GetStyleSelf()->visibility->Set(GUI::CSS::ICSSproperty::VISIBILITY_VISIBLE);
			pNode->UpdateStyles();
		}
		break;
	case NET::MSL_BADTOKEN:
		pGui->SetActiveDesktopW(L"main_login");
		break;
	}
}

void doLogin()
{
	StringW lgn = pGui->GetActiveDesktop()->GetDocument()->GetElementById(L"l_login")->GetText();
	StringW pass = pGui->GetActiveDesktop()->GetDocument()->GetElementById(L"l_passwd")->GetText();
	GUI::DOM::IDOMnode * pNode = pGui->GetActiveDesktop()->GetDocument()->GetElementById(L"l_error");
	pNode->GetStyleSelf()->visibility->Set(GUI::CSS::ICSSproperty::VISIBILITY_HIDDEN);
	pNode->UpdateStyles();
	//wprintf(L"Login: %s\nPass: %s\n", lgn.c_str(), pass.c_str());

	pMC->Login(lgn, pass);
}

//lgn_lgn
void cbLgnLgn(GUI::IEvent * ev)
{
	if(ev->key == KEY_TAB || ev->key == KEY_ENTER)
	{
		pGui->GetActiveDesktop()->RequestFocus(
			pGui->GetActiveDesktop()->GetDocument()->GetElementById(L"l_passwd")
			);
	}
}
//lgn_pass
void cbLgnPass(GUI::IEvent * ev)
{
	if(ev->key == KEY_TAB)
	{
		pGui->GetActiveDesktop()->RequestFocus(
			pGui->GetActiveDesktop()->GetDocument()->GetElementById(L"l_signin")
			);
	}

	if(ev->key == KEY_ENTER)
	{
		doLogin();
	}
}
//l_signin
void cbSignIn(GUI::IEvent * ev)
{
	doLogin();
}

void cbM2C_PROFILE(BYTE msg, NET::INETbuff * buf, NET::IMasterClient * mc)
{
	StringW username = buf->readPString();
	int balance = buf->readInt32();

	pDesk->GetDocument()->GetElementById(L"udata_name")->SetText(username, TRUE);
	pDesk->GetDocument()->GetElementById(L"udata_balance_real")->SetText(balance, TRUE);
	//udata_balance_real
}


void cbSetTab(GUI::IEvent * ev)
{
	static GUI::DOM::IDOMnode * pHolder = pGui->GetActiveDesktop()->GetDocument()->GetElementById(L"tabs_holder");
	if(!pHolder)
	{
		return;
	}

	
	static GUI::DOM::IDOMnode * pHdr = pGui->GetActiveDesktop()->GetDocument()->GetElementById(L"thdr_holder");
	if(!pHdr)
	{
		return;
	}

	for(UINT i = 0, l = pHdr->GetChilds()->size(); i < l; ++i)
	{
		GUI::DOM::IDOMnode * pNode = pHdr->GetChilds()[0][i];
		pNode->SetAttribute(L"class", L"");
		pNode->UpdateStyles();
	}

	StringW tab = ev->target->GetAttribute(L"tab");
	ev->target->SetAttribute(L"class", L"active");

	for(UINT i = 0, l = pHolder->GetChilds()->size(); i < l; ++i)
	{
		GUI::DOM::IDOMnode * pNode = pHolder->GetChilds()[0][i];
		if(tab == pNode->GetAttribute(L"name"))
		{
			pNode->GetStyleSelf()->visibility->Set(GUI::CSS::ICSSproperty::VISIBILITY_VISIBLE);
		}
		else
		{
			pNode->GetStyleSelf()->visibility->Set(GUI::CSS::ICSSproperty::VISIBILITY_HIDDEN);
		}
		pNode->UpdateStyles();
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
	pGui = pfnGUIInit(g_pd3dDevice, "./gamesource/gui", g_hWnd);
	pGui->RegisterCallback("on_exit", cbExit);
	pGui->RegisterCallback("on_cancel", cbCancel);
	pGui->RegisterCallback("exit_prompt", cbExitPrompt);
	pGui->RegisterCallback("main_tab", cbSetTab);
	pGui->RegisterCallback("prompt_pay", cbPayPrompt);
	pGui->RegisterCallback("go_pay", cbPayGo);
	pGui->RegisterCallback("lgn_lgn", cbLgnLgn);
	pGui->RegisterCallback("lgn_pass", cbLgnPass);
	pGui->RegisterCallback("lgn_signin", cbSignIn);
	pGui->RegisterCallback("gl_refresh", cbGameListRefresh);
	pGui->RegisterCallback("game_select", cbGameListSelect);

	

	pDesk = pGui->CreateDesktopA("main_menu", "main_menu.html");
	GUI::IDesktop * pLgnDesk = pGui->CreateDesktopA("main_login", "sys/login.html");

	StringW authToken;

	FILE * f = fopen("./token.bin", "rb");
	if(f)
	{
		//fwrite(token.c_str(), sizeof(WCHAR), token.length(), f);
		fseek(f, 0, SEEK_END);
		int len = ftell(f);
		fseek(f, 0, SEEK_SET);
		WCHAR * str = new WCHAR[len+1];
		str[fread(str, sizeof(WCHAR), len / sizeof(WCHAR), f)] = 0;
		authToken = str;
		delete[] str;
		fclose(f);
		pGui->SetActiveDesktop(pDesk);
	}
	else
	{
		pGui->SetActiveDesktop(pLgnDesk);
	}

	
	
	
	hDLL = LoadLibrary(L"sxnet.dll");
	if(!hDLL)
	{
		MessageBoxW(NULL, L"Ошибка загрузки sxnet.dll", L"Ошибка", MB_OK);
		CleanupDevice();
		return(0);
	}

	NET::PFNINITINSTANCE pfnInitNetwork;
	pfnInitNetwork = (NET::PFNINITINSTANCE)GetProcAddress(hDLL, "InitInstance");

	if(!pfnInitNetwork)
	{
		MessageBoxW(NULL, L"Не найдена точка входа в процедуру InitInstance в библиотеке sxnet.dll", L"Ошибка", MB_OK);
		CleanupDevice();
		return(0);
	}
	pNET = pfnInitNetwork();


	pMC = pNET->CreateMasterClient();
	pMC->SetAuthToken(authToken);
	pMC->SetLoginCallback(cbOnLogin);

	pMC->RegisterMessage(M2C_PROFILE, cbM2C_PROFILE);

	pMC->AddEvent(L"sx/slist", evtGameList);

	//pMC->Connect("ds-servers.com");
	pMC->Connect("wss://ds-servers.com/registry");
	//pMC->RegisterMessage(4, 

	while(WM_QUIT != msg.message)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//g_fpctext.Render();
		pGui->Update();
		pGui->Syncronize();
		pNET->Update();
		pNET->Syncronize();
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
			//pDesk->Release();
			//pDesk = pGui->CreateDesktopA("main_menu", "main_menu.html");
			pDesk = pGui->CreateDesktopA("main_login", "sys/login.html");
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

