
#pragma once

#define SX_EXE

#include <windows.h>
#include <ctime>
#pragma comment(lib, "winmm.lib")
#include <gdefines.h>
//#include <string\string.cpp>
#if defined(_DEBUG)
#pragma comment(lib, "sxinput_d.lib")
#else
#pragma comment(lib, "sxinput.lib")
#endif
#include <input\\sxinput.h>
#include <core\\array.h>
#include <handler_out_log.cpp>
//#include <core\\sxcore.cpp>

#define SM_D3D_CONVERSIONS

#include <sxmath.h>


#if defined(_DEBUG)
#pragma comment(lib, "sxsound_d.lib")
#else
#pragma comment(lib, "sxsound.lib")
#endif
#include <sound\\sxsound.h>
/*
#if defined(_DEBUG)
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
#include <core\\sxcore.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxguiwinapi_d.lib")
#else
#pragma comment(lib, "sxguiwinapi.lib")
#endif
#include <sxguiwinapi\\sxguielements.h>

#include <geom\\static_geom.cpp>


StaticGeom* Geometry = 0;
ISXCamera* ObjCamera = 0;
D3DXMATRIX Projection;


//обработка вводы информации с клавиатуры
void UpdateInputKeyBoard(DWORD timeDelta)
{
	//обработка ходьбы
	if (::GetAsyncKeyState('W') & 0x8000f)
		ObjCamera->PosFrontBack(float(timeDelta) * 0.001f);

	if (::GetAsyncKeyState('S') & 0x8000f)
		ObjCamera->PosFrontBack(-float(timeDelta) * 0.001f);

	if (::GetAsyncKeyState('A') & 0x8000f)
		ObjCamera->PosFrontBack(-float(timeDelta) * 0.001f);

	if (::GetAsyncKeyState('D') & 0x8000f)
		ObjCamera->PosFrontBack(float(timeDelta) * 0.001f);

	//крен камеры
	if (::GetAsyncKeyState('Q') & 0x8000f)
		ObjCamera->Roll(0.2f);

	if (::GetAsyncKeyState('E') & 0x8000f)
		ObjCamera->Roll(-0.2f);
}

//обработка движения мыши вправо и влево
void UpdateInputMouseRotate(DWORD timeDelta)
{
	UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
	UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
	POINT p;
	GetCursorPos(&p);
	POINT centr;
	centr.x = cx; centr.y = cy;

	if (cx != UINT(p.x))
	{
		ObjCamera->RotRightLeft(float(timeDelta) * 0.001f * float(int(p.x - cx)));
		//SetCursorPos(centr.x,cy);
	}

	if (cy != UINT(p.y))
	{
		ObjCamera->RotUpDown(float(timeDelta) * 0.001f * float(int(p.y - cy)));
		//SetCursorPos(cx,centr.y);
	}
}

//обработка движения мыши вверх вниз
void UpdateInputMouseUpDown(DWORD timeDelta)
{
	UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
	UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
	POINT p;
	GetCursorPos(&p);
	POINT centr;
	centr.x = cx; centr.y = cy;

	if (cy != UINT(p.y))
	{
		ObjCamera->PosUpDown(float(timeDelta) * 0.001f * float(-int(p.y - cy)));
		//SetCursorPos(cx,centr.y);
	}
}

//центрирвоание курсора мыши
void CentererCursor()
{
	UINT cx = GetSystemMetrics(SM_CXSCREEN) / 2;
	UINT cy = GetSystemMetrics(SM_CYSCREEN) / 2;
	SetCursorPos(cx, cy);
}


int Render(DWORD timeDelta)
{
	DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x569874, 1.0f, 0);//очистка  буфера цвета и буфера глубины
	DXDevice->BeginScene();//подготовка к рисованию

	//обработка перемещения/вращения
	if (GetAsyncKeyState(VK_LCONTROL))
	{
		static bool IsFirstLBM = false;
		static bool IsFirstRBM = false;

		static bool IsSFirstLBM = false;
		static bool IsSFirstRBM = false;

		UpdateInputKeyBoard(timeDelta);

		//еси нажата левая кнопка мыши то можно вращать
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			//если не первый раз нажата ЛКМ то совершаем действие
			if (IsFirstLBM)
				UpdateInputMouseRotate(timeDelta);
			//иначе просто говорим что впервые и устанавливаем курсор в центр
			else
				IsFirstLBM = true;
			CentererCursor();
		}
		//если нажата правая кнопка мыши то можно поднимать и опускать камеру
		else if (GetAsyncKeyState(VK_RBUTTON))
		{
			if (IsFirstRBM)
				UpdateInputMouseUpDown(timeDelta);
			else
				IsFirstRBM = true;
			CentererCursor();
		}
		else
		{
			IsFirstLBM = false;
			IsFirstRBM = false;
		}
	}

	DXDevice->SetTransform(D3DTS_WORLD, &(SMMatrixIdentity().operator D3DXMATRIX()));

	//установка видовой матрицы из камеры для применения перемещения/вращения
	float4x4 view;
	ObjCamera->GetViewMatrix(&view);
	DXDevice->SetTransform(D3DTS_VIEW, &(view.operator D3DXMATRIX()));

	//установка матрицы проекции
	DXDevice->SetTransform(D3DTS_PROJECTION, &Projection);

	Geometry->Render();

	DXDevice->EndScene();	//заканчиваем отрисовку
	DXDevice->Present(0, 0, 0, 0);	//выводим картинку в окно
	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(0));
	ISXGUIBaseWnd* tmpwin = SXGUICrBaseWnd("SkyLandEditor", "SkyLandEditor", 0, 0, 273, 131, 657, 585, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0);
	SGCore_0Create("qq", tmpwin->GetHWND(), 800, 600, true, 0, false);
	D3DXMatrixPerspectiveFovLH(&Projection, D3DX_PI * 0.5f, 800.0 / 600.0, 0.05f, 1000.0f);
	Geometry = new StaticGeom();
	Geometry->AddModel("D:\\project\\engine\\build\\gamesource\\meshes\\rinok\\rinok\\land.dse");

	DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	ObjCamera = SGCore_CrCamera();

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

					Render(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

    return msg.wParam;
}