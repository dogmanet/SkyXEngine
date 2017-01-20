
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

#include <geom\\static_geom.h>
#include <geom\\green.h>

#include <geom\\static_geom.cpp>
#include <common\\gdata.h>
#include <geom\\green.cpp>

#include <common\\gdata.h>
#include <common\\camera_update.cpp>
#include <common\\render_func.cpp>




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	InitOutLog();
	srand((unsigned int)time(0));
	ISXGUIBaseWnd* tmpwin = SXGUICrBaseWnd("SkyXEngine", "SkyXEngine", 0, 0, 0, 0, GData::WinSize.x, GData::WinSize.y, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0);
	GData::Handle3D = tmpwin->GetHWND();
	SSInput_0Create("SkyXEngine input", GData::Handle3D, true);
	SGCore_0Create("SkyXEngine graphics", tmpwin->GetHWND(), GData::WinSize.x, GData::WinSize.y, GData::IsWindowed, 0, true);
	SGCore_Dbg_Set(printflog);
	GData::MCamProj = SMMatrixPerspectiveFovLH(D3DX_PI * 0.25f, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, GData::NearFar.y);
	GData::MLightProj = SMMatrixPerspectiveFovLH(D3DX_PI * 0.25f, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, 10000);
	

	StaticGeom::DXDevice = SGCore_GetDXDevice();
	Green::DXDevice = SGCore_GetDXDevice();
	GData::Geometry = new StaticGeom();
	
	
	//GData::Geometry->AddModel("D:\\project\\engine\\build\\gamesource\\meshes\\rinok\\rinok\\land.dse");
	/*GData::Geometry->AddModel("D:\\project\\engine\\build\\gamesource\\meshes\\rinok\\rinok\\cv04.dse");
	GData::Geometry->AddModel("D:\\project\\engine\\build\\gamesource\\meshes\\rinok\\rinok\\rinok\\zab.dse");
	GData::Geometry->AddModel("D:\\project\\engine\\build\\gamesource\\meshes\\rinok\\rinok\\land.dse");
	
	GData::Geometry->Save("D:\\project\\engine\\build\\gamesource\\test.lvl");*/


	GData::Geometry->Load("D:\\project\\engine\\build\\gamesource\\test.lvl");

	GData::StaticGreen = new Green();
	/*GData::StaticGreen->Load(
		"D:\\project\\engine\\build\\gamesource\\meshes\\green\\trava1.dse",
		"D:\\project\\engine\\build\\gamesource\\meshes\\green\\trava1.dse",
		"D:\\project\\engine\\build\\gamesource\\meshes\\green\\trava1.dse",
		"D:\\project\\engine\\build\\gamesource\\levels\\abazar\\2.gt",
		50
		);*/
	
	
	GData::DXDevice = SGCore_GetDXDevice();
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	GData::ObjCamera = SGCore_CrCamera();

	SGCore_ShaderSetStdPath("D:\\project\\engine\\build\\gamesource\\shaders\\");
	GData::IDShaderVSRenderGreenTree = SGCore_ShaderLoad(0, "mtrl_base_green_tree.vs", "mtrl_base_green_tree.vs", false);
	GData::IDShaderVSRenderGreenGrass = SGCore_ShaderLoad(0, "mtrl_base_green_grass.vs", "mtrl_base_green_grass.vs", false);
	GData::IDShaderPSRenderGreenTree = SGCore_ShaderLoad(1, "mtrl_base_green.ps", "mtrl_base_green.ps", false);

	char tmppathexe[1024];
	char tmppath[1024];
	GetModuleFileName(NULL, tmppath, 1024);
	int len = strlen(tmppath);
	while (tmppath[len--] != '\\')
	{
		if (tmppath[len - 1] == '\\')
		{
			len--;
			memcpy(tmppathexe, tmppath, len);
			tmppathexe[len] = 0;
		}
	}

	
	char tmppathtex[1024];
	sprintf(tmppathtex, "%s%s", tmppathexe, "\\gamesource\\textures\\");
	SGCore_LoadTexStdPath(tmppathtex);
	SGCore_LoadTexLoadTextures();

	DWORD ttime = timeGetTime();
	GData::StaticGreen->Init(GData::Geometry,
		"trava_mask.dds",
		2,
		"D:\\project\\engine\\build\\gamesource\\meshes\\green\\trava1.dse",
		0,
		0,
		50);
	ttime = timeGetTime() - ttime;


	//GData::StaticGreen->Load("D:\\project\\engine\\build\\gamesource\\test.green");

	SGCore_LoadTexLoadTextures();

	Green::BeginEndLessening = 30;
	Green::DistLods = float2_t(50,100);
	Green::CurrentFreqGrass = 100;

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

					SXRenderFunc::GameRender(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

    return msg.wParam;
}