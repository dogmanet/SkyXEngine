
#define SX_MATERIAL_EDITOR

#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <vld.h> 
#define SX_EXE

#include <windows.h>
#include <ctime>
#pragma comment(lib, "winmm.lib")
#include <gdefines.h>

#include <SkyXEngine.h>

#include <common\\array.h>
#include <common\\string.cpp>
#include <managed_render\\handler_out_log.cpp>

#if defined(_DEBUG)
#pragma comment(lib, "sxguiwinapi_d.lib")
#else
#pragma comment(lib, "sxguiwinapi.lib")
#endif
#include <SXGUIWinApi\\sxgui.h>

#include <managed_render\\gdata.h>

#include <common\\string_api.cpp>

#include <managed_render\\gdata.cpp>

#include <managed_render\\camera_update.cpp>
#include <managed_render\\render_func.cpp>

//#include <common\\dialogs.cpp>

#include <sxmaterialeditor/resource.h>
#include <sxmaterialeditor\\material_editor.cpp>




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SXGUIRegClass::RegGroupBox();
	InitOutLog();
	srand((unsigned int)time(0));

	SXMaterialEditor::InitAllElements();
	SXMaterialEditor::Nulling();
	SXMaterialEditor::JobWindow->AddHandler(ComMenuId, WM_COMMAND);
	SXMaterialEditor::JobWindow->AddHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXMaterialEditor::JobWindow->AddHandler(MsgEditSize, WM_SIZE);
	SXMaterialEditor::MainMenu = SXGUICrMenuEx(IDR_MENU1);
	SXMaterialEditor::MainMenu->SetToWindow(SXMaterialEditor::JobWindow->GetHWND());

	SXMaterialEditor::WindowRender->GAlign.left = true;
	SXMaterialEditor::WindowRender->GAlign.right = true;
	SXMaterialEditor::WindowRender->GAlign.top = true;
	SXMaterialEditor::WindowRender->GAlign.bottom = true;

	SXMaterialEditor::ToolBar->GAlign.left = true;
	SXMaterialEditor::ToolBar->GAlign.right = true;
	SXMaterialEditor::ToolBar->GAlign.top = true;
	SXMaterialEditor::ToolBar->GAlign.bottom = false;

	GData::Handle3D = SXMaterialEditor::WindowRender->GetHWND();

	RECT winrndrect;
	SXMaterialEditor::WindowRender->GetClientRect(&winrndrect);

	GData::WinSize.x = winrndrect.right;
	GData::WinSize.y = winrndrect.bottom;

	GData::Pathes::InitAllPathes();

	SSInput_0Create("SXMaterialEditor input", SXMaterialEditor::JobWindow->GetHWND(), true);
	SSInput_Dbg_Set(printflog);
	SGCore_0Create("SXMaterialEditor graphics", GData::Handle3D, GData::WinSize.x, GData::WinSize.y, GData::IsWindowed, 0, true);
	SGCore_Dbg_Set(printflog);
	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_ShaderSetStdPath(GData::Pathes::Shaders);

	SGCore_SetFunc_MtlSet(SXRenderFunc::RFuncMtlSet);
	SGCore_SetFunc_MtlLoad(SXRenderFunc::RFuncMtlLoad);
	SGCore_SetFunc_MtlGetSort((g_func_mtl_get_sort)SML_MtlGetTypeTransparency);
	SGCore_SetFunc_MtlGroupRenderIsSingly((g_func_mtl_group_render_is_singly)SML_MtlGetTypeReflection);

	SGCore_SkyBoxCr();
	SGCore_SkyBoxSetStdPathTex(GData::Pathes::TexturesSkyBoxes);

	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SXMaterialEditor::EditSkyBox->SetText("sky_2_cube.dds");
	
	SGeom_0Create("SXMaterialEditor geometry", SGCore_GetDXDevice(), GData::Pathes::Meshes, true);
	SGeom_Dbg_Set(printflog);

	SXAnim_0Create();

	GData::InitAllMatrix();
	
	
	GData::DXDevice = SGCore_GetDXDevice();
	
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	GData::ObjCamera = SGCore_CrCamera();
	GData::ObjCamera->SetPosition(&float3(0, 0, -1.2*100));

	GData::IDsShaders::InitAllShaders();

	
	SML_0Create("sxml", SGCore_GetDXDevice(), GData::Pathes::Materials, GData::Pathes::Meshes, &GData::WinSize, GData::ProjFov, false);
	SML_Dbg_Set(printflog);

	SPP_0Create("sxpp", SGCore_GetDXDevice(), &GData::WinSize, false);
	SPP_Dbg_Set(printflog);
	SPP_ChangeTexSun("fx_sun.dds");

	SPP_RTSetInput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com));
	SPP_RTSetOutput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com2));
	SPP_RTSetDepth0(SML_DSGetRT_ID(DS_RT::ds_rt_depth0));
	SPP_RTSetDepth1(SML_DSGetRT_ID(DS_RT::ds_rt_depth1));
	SPP_RTSetNormal(SML_DSGetRT_ID(DS_RT::ds_rt_normal));

	GData::SimModel = new ModelSim();

	GData::SimModel->Add("sphere.dse");
	GData::SimModel->Add("cube.dse");
	GData::SimModel->Add("plane.dse");

	SML_LigthsCreatePoint(
		&float3(10,300,0),
		LIGHTS_GLOBAL_MAX_POWER,
		LIGHTS_GLOBAL_STD_RADIUS,
		&float3(1,1,1),
		true,
		false);
	SML_LigthsSetEnable(SML_LigthsGetCount() - 1, true);
	SML_LigthsSetName(SML_LigthsGetCount() - 1, "sun");

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

	SXMaterialEditor::InitMtl(GData::SimModel->GetIDMtl());
	SXMaterialEditor::IDMat = GData::SimModel->GetIDMtl();

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

					SGCore_LoadTexLoadTextures();
					//SXLevelEditor_Transform(10);
					if (SXMaterialEditor::CheckBoxModelRot->GetCheck())
						GData::SimModel->Rotation.y -= float(timeDelta) * 0.001f * 0.25;
					SXRenderFunc::MainRender(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

	mem_release(GData::ObjCamera);
	SML_0Kill();
	SGCore_0Kill();
	SXMaterialEditor::DeleteAllElements();

    return msg.wParam;
}