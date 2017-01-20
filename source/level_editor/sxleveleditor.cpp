
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

/*
#if defined(_DEBUG)
#pragma comment(lib, "sxguiwinapi_d.lib")
#else
#pragma comment(lib, "sxguiwinapi.lib")
#endif
#include <sxguiwinapi\\sxguielements.h>
*/
#include <SXGUIWinApi\\sxgui.h>

#include <common\\string_api.cpp>

#include <geom\\static_geom.h>
#include <geom\\green.h>

#include <geom\\static_geom.cpp>
#include <common\\gdata.h>
#include <geom\\green.cpp>

#include <common\\gdata.cpp>

#include <common\\camera_update.cpp>
#include <common\\render_func.cpp>

#include <common\\save_level.cpp>
#include <common\\load_level.cpp>
#include <common\\dialogs.cpp>

#include <../SkyXEngine/SXLevelEditor/SXLevelEditor/resource.h>
#include <level_editor\\level_editor.cpp>




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SXGUIRegClass::RegGroupBox();
	InitOutLog();
	srand((unsigned int)time(0));

	SXLevelEditor::InitAllElements();

	SXLevelEditor::JobWindow->AddHandler(ComMenuId, WM_COMMAND);
	SXLevelEditor::JobWindow->AddHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXLevelEditor::JobWindow->AddHandler(MsgEditSize, WM_SIZE);
	SXLevelEditor::MainMenu = SXGUICrMenuEx(IDR_MENU1);
	SXLevelEditor::MainMenu->SetToWindow(SXLevelEditor::JobWindow->GetHWND());

	SXLevelEditor::RenderWindow->GAlign.left = true;
	SXLevelEditor::RenderWindow->GAlign.right = true;
	SXLevelEditor::RenderWindow->GAlign.top = true;
	SXLevelEditor::RenderWindow->GAlign.bottom = true;

	SXLevelEditor::GroupBoxList->GAlign.left = false;
	SXLevelEditor::GroupBoxList->GAlign.right = true;
	SXLevelEditor::GroupBoxList->GAlign.top = true;
	SXLevelEditor::GroupBoxList->GAlign.bottom = true;

	SXLevelEditor::GroupBoxData->GAlign.left = true;
	SXLevelEditor::GroupBoxData->GAlign.right = true;
	SXLevelEditor::GroupBoxData->GAlign.top = false;
	SXLevelEditor::GroupBoxData->GAlign.bottom = true;


	SXLevelEditor::ToolBar1->GAlign.left = true;
	SXLevelEditor::ToolBar1->GAlign.right = true;
	SXLevelEditor::ToolBar1->GAlign.top = true;
	SXLevelEditor::ToolBar1->GAlign.bottom = false;

	GData::Handle3D = SXLevelEditor::RenderWindow->GetHWND();

	RECT winrndrect;
	SXLevelEditor::RenderWindow->GetClientRect(&winrndrect);

	GData::WinSize.x = winrndrect.right;
	GData::WinSize.y = winrndrect.bottom;

	GData::Pathes::InitAllPathes();

	SSInput_0Create("SXLevelEditor input", SXLevelEditor::JobWindow->GetHWND(), true);
	SGCore_0Create("SXLevelEditor graphics", GData::Handle3D, GData::WinSize.x, GData::WinSize.y, GData::IsWindowed, 0, true);
	SGCore_Dbg_Set(printflog);
	GData::MCamProj = SMMatrixPerspectiveFovLH(D3DX_PI * 0.25f, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, GData::NearFar.y);
	GData::MLightProj = SMMatrixPerspectiveFovLH(D3DX_PI * 0.25f, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, 10000);
	

	StaticGeom::DXDevice = SGCore_GetDXDevice();
	Green::DXDevice = SGCore_GetDXDevice();
	sprintf(Green::StdPath, "%s", GData::Pathes::Meshes);
	sprintf(StaticGeom::StdPath, "%s", GData::Pathes::Meshes);
	StaticGeom::DistForLod = 100;

	GData::Geometry = new StaticGeom();
	GData::StaticGreen = new Green();

	Green::BeginEndLessening = 30;
	Green::DistLods = float2_t(50, 100);
	Green::CurrentFreqGrass = 100;
	
	GData::DXDevice = SGCore_GetDXDevice();
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	GData::ObjCamera = SGCore_CrCamera();



	GData::Geometry->AddModel("rinok\\rinok\\land.dse", 0, "land.dse");
	GData::Geometry->AddModel("rinok\\rinok\\cv04.dse", 0, "cv04.dse");
	GData::Geometry->AddModel("rinok\\rinok\\rinok\\zab.dse", 0, "zab.dse");



	SGCore_ShaderSetStdPath(GData::Pathes::Shaders);
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

	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_LoadTexLoadTextures();


	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	
	static DWORD lastTime = timeGetTime();
	static DWORD TimeCCadr = 0;

		while (msg.message != WM_QUIT && IsWindow(GData::Handle3D))
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

					SXLevelEditor_Transform(10);
					SXRenderFunc::LevelEditorRender(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

    return msg.wParam;
}