
#define _CRT_SECURE_NO_WARNINGS
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

#define SXGCORE_IN_SOURCE
#if !defined(SXGCORE_IN_SOURCE)
#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif
#include <gcore\\sxgcore.h>
#else
#include <gcore\\sxgcore.cpp>
#endif

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

IDirect3DVertexBuffer9* vb;
IDirect3DIndexBuffer9* ib;
long count_vertex;
long count_index;

#include <common\\gdata.h>

#include <common\\string_api.cpp>
#include <material_ligth\\material_ligth.cpp>
#include <geom\\sxgeom.cpp>




IDirect3DTexture9* g_pTexAdaptedLuminanceLast, *g_pTexAdaptedLuminanceCur;

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
	SSInput_Dbg_Set(printflog);
	SGCore_0Create("SXLevelEditor graphics", GData::Handle3D, GData::WinSize.x, GData::WinSize.y, GData::IsWindowed, 0, true);
	SGCore_Dbg_Set(printflog);
	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_ShaderSetStdPath(GData::Pathes::Shaders);
	SGeom_0Create("SXLevelEditor geometry", SGCore_GetDXDevice(), GData::Pathes::Meshes, true);
	SGeom_Dbg_Set(printflog);
	

	GData::MCamProj = SMMatrixPerspectiveFovLH(D3DX_PI * 0.25f, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, GData::NearFar.y);
	GData::MLightProj = SMMatrixPerspectiveFovLH(D3DX_PI * 0.25f, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, 100000);
	
	
	
	GData::DXDevice = SGCore_GetDXDevice();
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	GData::ObjCamera = SGCore_CrCamera();

	
	//цвет (текстуры)
	/*GData::IDSRenderTargets::ColorScene = SGCore_RTAdd(GData::WinSize.x, GData::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_color", 1);
	//номрали + микрорельеф
	GData::IDSRenderTargets::NormalScene = SGCore_RTAdd(GData::WinSize.x, GData::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A2B10G10R10, D3DPOOL_DEFAULT, "ds_normal", 1);
	//параметры освещения
	GData::IDSRenderTargets::ParamsScene = SGCore_RTAdd(GData::WinSize.x, GData::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, "ds_param", 1);

	GData::IDSRenderTargets::DepthScene = SGCore_RTAdd(GData::WinSize.x, GData::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_depth", 1);


	GData::IDSRenderTargets::LightAmbient = SGCore_RTAdd(GData::WinSize.x, GData::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_ambient", 1);
	GData::IDSRenderTargets::LightSpecDiff = SGCore_RTAdd(GData::WinSize.x, GData::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_specdiff", 1);

	for (int i = 0; i < 4; i++)
	{
		int iSampleLen = 1 << (2 * i);

		GData::IDSRenderTargets::ToneMaps[i] = SGCore_RTAdd(iSampleLen, iSampleLen, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "", 0);

	}

	GData::IDSRenderTargets::AdaptLumCurr = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "", 0);
	GData::IDSRenderTargets::AdaptLumLast = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "", 0);

	g_pTexAdaptedLuminanceLast = SGCore_RTGetTexture(GData::IDSRenderTargets::AdaptLumLast);
	g_pTexAdaptedLuminanceCur = SGCore_RTGetTexture(GData::IDSRenderTargets::AdaptLumCurr);
	*/
	//GData::IDSRenderTargets::LightSpecDiffD2 = SGCore_RTAdd(GData::WinSize.x*0.5f, GData::WinSize.y*0.5f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A32B32G32R32F, D3DPOOL_DEFAULT, "ds_specdiffD2", 0.5);
	//GData::DXDevice->CreateOffscreenPlainSurface(1, 1, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &GData::ComLightSurf1x1, 0);

	//DXDevice->CreateRenderTarget(1, 1, D3DFMT_A8R8G8B8, 0, 0, 0, &GData::ComLightSurf1x1, 0);
	//GData::IDSRenderTargets::LigthCom_1x1 = SGCore_RTAdd(GData::WinSize.x*0.5f, GData::WinSize.y*0.5f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_lightcom_1x1", 1);
	/*GData::IDSRenderTargets::LigthCom = SGCore_RTAdd(GData::WinSize.x, GData::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A2B10G10R10, D3DPOOL_DEFAULT, "ds_lightcom", 1);
	GData::IDSRenderTargets::LigthCom2 = SGCore_RTAdd(GData::WinSize.x, GData::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A2B10G10R10, D3DPOOL_DEFAULT, "ds_lightcom", 1);

	GData::IDSRenderTargets::LigthComScaled = SGCore_RTAdd(GData::WinSize.x*0.25f, GData::WinSize.y*0.25f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A2B10G10R10, D3DPOOL_DEFAULT, "ds_lightcomscaled", 0.25);
	*///GData::IDSRenderTargets::LigthComD2 = SGCore_RTAdd(GData::WinSize.x*0.5f, GData::WinSize.y*0.5f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_lightcomD2", 0.5);
	//GData::IDSRenderTargets::LigthComD4 = SGCore_RTAdd(GData::WinSize.x*0.25f, GData::WinSize.y*0.25f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_lightcomD4", 0.25);
	//GData::IDSRenderTargets::LigthComD8 = SGCore_RTAdd(GData::WinSize.x*0.125f, GData::WinSize.y*0.125f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_lightcomD8", 0.125);

	GData::IDsShaders::VS::FreeGeometry = SGCore_ShaderLoad(0, "mtrl_base.vs", "mtrl_base", true);
	GData::IDsShaders::PS::FreeGeometry = SGCore_ShaderLoad(1, "mtrl_base.ps", "mtrl_base", true);

	GData::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(0, "pp_quad_render.vs", "pp_quad_render", true);
	GData::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(1, "pp_quad_render.ps", "pp_quad_render", true);

	GData::IDsShaders::VS::ResPos = SGCore_ShaderLoad(0, "pp_quad_render_res_pos.vs", "pp_quad_render_res_pos", true);
	GData::IDsShaders::PS::ComLightingNonShadow = SGCore_ShaderLoad(1, "comlighting.ps", "comlighting_nonshadow", false);
	D3DXMACRO Defines_IS_SHADOWED[] = { { "IS_SHADOWED", "" }, { 0, 0 } };
	GData::IDsShaders::PS::ComLightingShadow = SGCore_ShaderLoad(1, "comlighting.ps", "comlighting_shadow", false, Defines_IS_SHADOWED);
	GData::IDsShaders::PS::BlendAmbientSpecDiffcolor = SGCore_ShaderLoad(1, "blendambientspecdiffcolor.ps", "blendambientspecdiffcolor", true);

	/*GData::IDsShaders::PS::CalcAdaptedLum = SGCore_ShaderLoad(1, "pp_calc_adapted_lum.ps", "pp_calc_adapted_lum", true);
	GData::IDsShaders::PS::SampleLumInit = SGCore_ShaderLoad(1, "pp_sample_lum_init.ps", "pp_sample_lum_init", true);
	GData::IDsShaders::PS::SampleLumIterative = SGCore_ShaderLoad(1, "pp_sample_lum_iterative.ps", "pp_sample_lum_iterative", true);
	GData::IDsShaders::PS::SampleLumFinal = SGCore_ShaderLoad(1, "pp_sample_lum_final.ps", "pp_sample_lum_final", true);
	GData::IDsShaders::PS::FinalHRDL = SGCore_ShaderLoad(1, "pp_final_hrdl.ps", "pp_final_hrdl", true);*/





	SML_0Create("sxml", SGCore_GetDXDevice(), GData::Pathes::Materials, GData::Pathes::Meshes, &GData::WinSize, GData::ProjFov, false);
	SML_Dbg_Set(printflog);
	//SML_LigthsCreatePoint(&float4(10, 40, 10, 100), &float3(0, 1, 0), false, true, 0);
	//SML_LigthsCreatePoint(&float4(45, 45, 0, 1000000), &float3(1, 0, 0), true, true);

	//SML_LigthsCreatePoint(&float4(40, 50, 40, 300), &float3(0, 0, 1), false, true, 0);

	//SML_LigthsCreateDirection(&float4(10, 30, 10, 200), &float3(0, 0, 1), &float3(-0.84, -0.55, -0.01), 0.1, 1.0, true, 0);

	/*GData::Geometry->AddModel("rinok\\rinok\\land.dse", 0, "land.dse");
	GData::Geometry->AddModel("rinok\\rinok\\cv04.dse", 0, "cv04.dse");
	GData::Geometry->AddModel("rinok\\rinok\\rinok\\zab.dse", 0, "zab.dse");*/

	/*GData::StaticGreen->Init(GData::Geometry, "tree.dse",
		"trava_mask.dds",
		1.f,
		"green\\tree.dse",
		"green\\tree.dse",
		"green\\tree.dse",
		"green\\tree.dse");*/

	/*float3_t** arr_vertex;
	long* arr_count_vertex;
	DWORD** arr_index;
	long* arr_count_index;
	long count_model;
	GData::StaticGreen->GetNavMeshAndTransform(&arr_vertex, &arr_count_vertex, &arr_index, &arr_count_index, &count_model);

	int curr_model = 2;
	count_vertex = arr_count_vertex[curr_model];
	count_index = arr_count_index[curr_model];

	float3_t* pData;
	DXDevice->CreateVertexBuffer(sizeof(float3_t)* count_vertex, NULL, NULL, D3DPOOL_MANAGED, &vb, 0);
	vb->Lock(0, 0, (void**)&pData, 0);
	memcpy(pData, arr_vertex[curr_model], sizeof(float3_t)* count_vertex);
	vb->Unlock();

	UINT* pDataI;
	DXDevice->CreateIndexBuffer(sizeof(UINT)* count_index, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &ib, 0);

	ib->Lock(0, 0, (void**)&pDataI, 0);
	memcpy(pDataI, arr_index[curr_model], sizeof(UINT)* count_index);

	
	ib->Unlock();*/

	/*float3_t** arr_vertex;
	long* arr_count_vertex;
	DWORD** arr_index;
	long* arr_count_index;
	float4x4** arr_transform;
	long* arr_count_transform;
	long count_model;
	GData::StaticGreen->GetNavMeshAndTransform(&arr_vertex, &arr_count_vertex, &arr_index, &arr_count_index, &arr_transform, &arr_count_transform, &count_model);

	int curr_model = 0;
	count_vertex = arr_count_vertex[curr_model];
	count_index = arr_count_index[curr_model];

	float3_t* pData;
	DXDevice->CreateVertexBuffer(sizeof(float3_t)* count_vertex, NULL, NULL, D3DPOOL_MANAGED, &vb, 0);
	vb->Lock(0, 0, (void**)&pData, 0);
	memcpy(pData, arr_vertex[curr_model], sizeof(float3_t)* count_vertex);
	vb->Unlock();

	UINT* pDataI;
	DXDevice->CreateIndexBuffer(sizeof(UINT)* count_index, NULL, D3DFMT_INDEX32, D3DPOOL_MANAGED, &ib, 0);

	ib->Lock(0, 0, (void**)&pDataI, 0);
	memcpy(pDataI, arr_index[curr_model], sizeof(UINT)* count_index);


	ib->Unlock();*/

	//GData::Geometry->DelModel(1);
	//GData::Geometry->DelModel(1);

	//GData::Geometry->AddModel("rinok\\rinok\\land.dse", 0, "land.dse");
	//GData::Geometry->AddModel("rinok\\rinok\\cv04.dse", 0, "cv04.dse");
	//GData::Geometry->AddModel("rinok\\rinok\\rinok\\zab.dse", 0, "zab.dse");

	

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

					SGCore_LoadTexLoadTextures();
					SXLevelEditor_Transform(10);
					SXRenderFunc::LevelEditorRender(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

    return msg.wParam;
}