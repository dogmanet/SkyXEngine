
#define _CRT_SECURE_NO_WARNINGS
#pragma once
//#include <vld.h> 
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


//#define SXGCORE_IN_SOURCE
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

	SGCore_SetFunc_SetMtl(SXRenderFunc::RFuncSetMtl);
	SGCore_SetFunc_LoadMtl(SXRenderFunc::RFuncLoadMtl);

	SGCore_SkyBoxCr();
	SGCore_SkyCloudsCr();
	SGCore_SkyBoxSetStdPathTex(GData::Pathes::TexturesSkyBoxes);
	SGCore_SkyCloudsSetStdPathTex(GData::Pathes::TexturesSkyBoxes);

	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	SGCore_SkyCloudsSetWidthHeightPos(2000, 2000, &float3(0, 0, 0));

	SGeom_0Create("SXLevelEditor geometry", SGCore_GetDXDevice(), GData::Pathes::Meshes, true);
	SGeom_Dbg_Set(printflog);
	

	GData::MCamProj = SMMatrixPerspectiveFovLH(D3DX_PI * 0.25f, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, GData::NearFar.y);
	GData::MLightProj = SMMatrixPerspectiveFovLH(D3DX_PI * 0.25f, GData::WinSize.x / GData::WinSize.y, GData::NearFar.x, 100000);
	
	
	
	GData::DXDevice = SGCore_GetDXDevice();
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	GData::ObjCamera = SGCore_CrCamera();

	
	GData::IDsShaders::VS::FreeGeometry = SGCore_ShaderLoad(0, "mtrl_base.vs", "mtrl_base", true);
	GData::IDsShaders::PS::FreeGeometry = SGCore_ShaderLoad(1, "mtrl_base.ps", "mtrl_base", true);

	GData::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(0, "pp_quad_render.vs", "pp_quad_render", true);
	GData::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(1, "pp_quad_render.ps", "pp_quad_render", true);

	GData::IDsShaders::VS::ResPos = SGCore_ShaderLoad(0, "pp_quad_render_res_pos.vs", "pp_quad_render_res_pos", true);


	GData::IDsShaders::PS::ComLightingNonShadow = SGCore_ShaderLoad(1, "lighting_com.ps", "lighting_com_nonshadow", false);
	D3DXMACRO Defines_IS_SHADOWED[] = { { "IS_SHADOWED", "" }, { 0, 0 } };
	GData::IDsShaders::PS::ComLightingShadow = SGCore_ShaderLoad(1, "lighting_com.ps", "lighting_com_shadow", false, Defines_IS_SHADOWED);
	GData::IDsShaders::PS::BlendAmbientSpecDiffcolor = SGCore_ShaderLoad(1, "lighting_blend.ps", "lighting_blend", true);


	



	SML_0Create("sxml", SGCore_GetDXDevice(), GData::Pathes::Materials, GData::Pathes::Meshes, &GData::WinSize, GData::ProjFov, false);
	SML_Dbg_Set(printflog);
	

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


	LoadLevel("test");

	//SGCore_LoadTexLoadTextures();
	//SXLevelEditor_Transform(10);
	//SXRenderFunc::LevelEditorRender(0);

	/*SGeom_ModelsClear();
	SGeom_GreenClear();
	SML_LigthsClear();*/

	/*MessageBox(0,0,0,0);
	for (int i = 0; i < 1000; ++i)
	{
		LoadLevel("test");

		//SGCore_LoadTexLoadTextures();
		//SXLevelEditor_Transform(10);
		//SXRenderFunc::LevelEditorRender(0);

		SGeom_ModelsClear();
		SGeom_GreenClear();
		SML_LigthsClear();
	}
	
	MessageBox(0, 0, 0, 0);*/

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