
#define SX_LEVEL_EDITOR
#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <vld.h> 
#define SX_EXE

#include <windows.h>
#include <ctime>
#pragma comment(lib, "winmm.lib")
#include <gdefines.h>
#include <skyxengine.h>

#include <common\\array.h>
#include <common\\string.cpp>
#include <managed_render/handler_out_log.cpp>


#if defined(_DEBUG)
#pragma comment(lib, "sxguiwinapi_d.lib")
#else
#pragma comment(lib, "sxguiwinapi.lib")
#endif
#include <sxguiwinapi\\sxgui.h>


#include <common\\string_api.cpp>

#include <managed_render\\gdata.h>
#include <managed_render\\camera_update.h>
#include <managed_render\\render_func.h>
#include <managed_render\\level.h>

#include <SXLevelEditor/resource.h>
#include <SXLevelEditor\\level_editor.cpp>

#include <managed_render\\gdata.cpp>
#include <managed_render\\camera_update.cpp>
#include <managed_render\\render_func.cpp>
#include <managed_render\\level.cpp>






int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//MessageBox(0,0,0,0);
	SXGUIRegClass::RegGroupBox();
	InitOutLog();
	srand((unsigned int)time(0));

	SXLevelEditor::InitAllElements();

	SXLevelEditor::JobWindow->AddHandler(ComMenuId, WM_COMMAND);
	SXLevelEditor::JobWindow->AddHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXLevelEditor::JobWindow->AddHandler(MsgEditSize, WM_SIZE);
	SXLevelEditor::MainMenu = SXGUICrMenuEx(IDR_MENU1);
	SXLevelEditor::MainMenu->SetToWindow(SXLevelEditor::JobWindow->GetHWND());
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
	GData::FinalImage = DS_RT::ds_rt_scene_light_com;

	GData::Editors::SelSelection = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_SELECTION, true);
	GData::Editors::SelBackFacesCull = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_BACKFACESCULL, true);
	GData::Editors::SelZTest = false;
	GData::Editors::SelMesh = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_MESH, true);
	

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
	SSInput_Dbg_Set(printflog);
	SGCore_0Create("SXLevelEditor graphics", GData::Handle3D, GData::WinSize.x, GData::WinSize.y, GData::IsWindowed, 0, true);
	SGCore_Dbg_Set(printflog);
	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_ShaderSetStdPath(GData::Pathes::Shaders);

	SGCore_SetFunc_MtlSet(SXRenderFunc::RFuncMtlSet);
	SGCore_SetFunc_MtlLoad(SXRenderFunc::RFuncMtlLoad);
	SGCore_SetFunc_MtlGetSort((g_func_mtl_get_sort)SML_MtlGetTypeTransparency);
	SGCore_SetFunc_MtlGroupRenderIsSingly((g_func_mtl_group_render_is_singly)SML_MtlGetTypeReflection);

	SGCore_SkyBoxCr();
	SGCore_SkyCloudsCr();
	SGCore_SkyBoxSetStdPathTex(GData::Pathes::TexturesSkyBoxes);
	SGCore_SkyCloudsSetStdPathTex(GData::Pathes::TexturesSkyBoxes);

	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	SGCore_SkyCloudsSetWidthHeightPos(2000, 2000, &float3(0, 0, 0));


	SGeom_0Create("SXLevelEditor geometry", SGCore_GetDXDevice(), GData::Pathes::Meshes, true);
	SGeom_Dbg_Set(printflog);
	
	SXAnim_0Create();

	GData::InitAllMatrix();
	
	
	GData::DXDevice = SGCore_GetDXDevice();
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	GData::ObjCamera = SGCore_CrCamera();


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


	IDirect3DTexture9* SelectMaterial;
	GData::DXDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &SelectMaterial, NULL);
	D3DLOCKED_RECT LockedRect;
	uint32_t tmpColor = D3DCOLOR_ARGB(255, 255, 0, 255);

	SelectMaterial->LockRect(0, &LockedRect, 0, 0);

	uint32_t* tmpOldColor = (uint32_t*)LockedRect.pBits + 0 * LockedRect.Pitch + 0 * sizeof(uint32_t);
	memcpy(tmpOldColor, &tmpColor, sizeof(uint32_t));

	SelectMaterial->UnlockRect(0);

	SGCore_LoadTexLoadTextures();
	GData::IDSelectTex = SGCore_LoadTexCreate("select_material__", SelectMaterial);

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


	//LoadLevel("darkvalley2");

	//SGCore_LoadTexLoadTextures();
	//SXLevelEditor_Transform(10);
	//SXRenderFunc::LevelEditorRender(0);

	/*SGeom_ModelsClear();
	SGeom_GreenClear();
	SML_LigthsClear();*/

	/*MessageBox(0,0,0,0);
	for (int i = 0; i < 10; ++i)
	{
		LoadLevel("darkvalley");

		//SGCore_LoadTexLoadTextures();
		//SXLevelEditor_Transform(10);
		//SXRenderFunc::LevelEditorRender(0);

		SGeom_ModelsClear();
		SGeom_GreenClear();
		SML_LigthsClear();
	}
	
	MessageBox(0, 0, 0, 0);*/

	/*mem_release(GData::ObjCamera);
	SGeom_0CreateKill();
	SML_0Kill();
	SGCore_0Kill();
	SXLevelEditor::DeleteAllElements();

	return 0;*/

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

					SGCore_LoadTexLoadTextures();
					SXLevelEditor_Transform(10);
					SXRenderFunc::MainRender(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

	mem_release(GData::ObjCamera);
	SGeom_0CreateKill();
	SML_0Kill();
	SGCore_0Kill();
	SXLevelEditor::DeleteAllElements();

    return msg.wParam;
}