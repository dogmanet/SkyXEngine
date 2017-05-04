
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define _CRT_SECURE_NO_WARNINGS
#pragma once

#define SX_EXE

#include <windows.h>
#include <ctime>
#pragma comment(lib, "winmm.lib")
#include <gdefines.h>


#include <managed_render\\handler_out_log.cpp>
#include <SkyXEngine.h>
#include <managed_render\\gdata.cpp>
#include <managed_render\\camera_update.cpp>
#include <managed_render\\render_func.cpp>
#include <managed_render\\level.cpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	InitOutLog();
	srand((unsigned int)time(0));

	GData::InitWin("SkyXEngine", "SkyXEngine");
	GData::Pathes::InitAllPathes();

	SSInput_0Create("SXLevelEditor input", GData::Handle3D, true);
	SSInput_Dbg_Set(printflog);
	Core_0Create("SkyXEngine Core", true);
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


	GData::DXDevice = SGCore_GetDXDevice();
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	GData::ObjCamera = SGCore_CrCamera();


	SGeom_0Create("SXLevelEditor geometry", SGCore_GetDXDevice(), GData::Pathes::Meshes, true);
	SGeom_Dbg_Set(printflog);


	SML_0Create("sxml", SGCore_GetDXDevice(), GData::Pathes::Materials, GData::Pathes::Meshes, &GData::WinSize, GData::ProjFov, false);
	SML_Dbg_Set(printflog);


	SPP_0Create("sxpp", SGCore_GetDXDevice(), &GData::WinSize, false);
	SPP_Dbg_Set(printflog);
	SPP_ChangeTexSun("fx_sun.dds");

	SXAnim_0Create();

	SPP_RTSetInput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com));
	SPP_RTSetOutput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com2));
	SPP_RTSetDepth0(SML_DSGetRT_ID(DS_RT::ds_rt_depth0));
	SPP_RTSetDepth1(SML_DSGetRT_ID(DS_RT::ds_rt_depth1));
	SPP_RTSetNormal(SML_DSGetRT_ID(DS_RT::ds_rt_normal));

	GData::InitAllMatrix();

	GData::IDsShaders::InitAllShaders();

	Level::Load("stalker_atp");

	IAnimPlayer * pl;
	pl = SXAnim_CreatePlayer("models/stalker_zombi/stalker_zombi_a.dse");
	pl->SetPos(float3(-17.18, -1.38f, -32.3));
	pl->Play("reload");
	
	/*SGeom_ModelsAddModel("stalker_atp.dse", 0, "stalker_atp.dse");

	SGeom_GreenAddGreen("tree_topol", "terrain_mp_atp_mask_tree.dds", 0.3, "green\\tree_topol_lod0.dse", "green\\tree_topol_lod1.dse", "green\\tree_topol_lod2.dse", "green\\tree_topol_lod2.dse");
	SGeom_GreenAddGreen("trava_green_det2", "terrain_mp_atp_mask_grass.dds", 0.2, "green\\trava_green_det2.dse", 0, 0, 0);


	SML_LigthsCreatePoint(
		&float3(60,60,0),
		LIGHTS_GLOBAL_MAX_POWER,
		LIGHTS_GLOBAL_STD_RADIUS,
		&float3(1,1,1),
		true,
		true);
		SML_LigthsSetEnable(SML_LigthsGetCount() - 1, true);
		SML_LigthsSetName(SML_LigthsGetCount() - 1, "sun");
		
	Level::Save("stalker_atp"); */

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
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			static DWORD TimeStart = 0;
			DWORD TimeThis = timeGetTime();

			DWORD currTime = timeGetTime();
			DWORD timeDelta = (currTime - lastTime);

			if (GetActiveWindow() == GData::Handle3D)
			{
				SGCore_LoadTexLoadTextures();
				SXRenderFunc::MainRender(timeDelta);
			}

			TimeCCadr = timeDelta;
			TimeStart = TimeThis;

			lastTime = currTime;
		}
	}

	SXAnim_0Kill();
	mem_release(GData::ObjCamera);
	SGeom_0CreateKill();
	SML_0Kill();
	SGCore_0Kill();

	return msg.wParam;
}