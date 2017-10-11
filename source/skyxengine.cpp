
#include "SkyXEngine.h"

void SkyXEngine_Init()
{
	srand((UINT)time(0));
	InitOutLog();
	SkyXEngine_InitPaths();

	if (!Core_0IsProcessRun("sxconsole.exe"))
		ShellExecute(0, "open", "sxconsole.exe", 0, Core_RStringGet(G_RI_STRING_PATH_EXE), SW_SHOWNORMAL);

#if defined(SX_GAME)
	GData::InitWin("SkyXEngine", "SkyXEngine");
#endif
	Level::Name[0] = 0;
	SSInput_0Create("sxinput", GData::Handle3D, false);
	SSInput_Dbg_Set(PrintfLog);

	Core_0Create("sxcore", false);
	Core_Dbg_Set(PrintfLog);
	Core_SetOutPtr();

	ID idTimerRender = Core_TimeAdd();
	ID idTimerGame = Core_TimeAdd();
	Core_RIntSet(G_RI_INT_TIMER_RENDER, idTimerRender);
	Core_RIntSet(G_RI_INT_TIMER_GAME, idTimerGame);

	tm ct = { 0, 0, 0, 27, 5, 2030 - 1900, 0, 0, 0 };
	Core_TimeUnixStartSet(idTimerGame, mktime(&ct));

	Core_TimeWorkingSet(idTimerRender, true);
	Core_TimeWorkingSet(idTimerGame, true);

	Core_TimeSpeedSet(idTimerGame, 100);

	SSCore_0Create("sxsound", GData::Handle3D, false);
	SSCore_Dbg_Set(PrintfLog);

	SGCore_0Create("sxgcore", GData::Handle3D, GData::WinSize.x, GData::WinSize.y, GData::IsWindowed, 0, false);
	SGCore_Dbg_Set(PrintfLog);

	SGCore_SetFunc_MtlSet(RFuncMtlSet);
	SGCore_SetFunc_MtlLoad(RFuncMtlLoad);
	SGCore_SetFunc_MtlGetSort((g_func_mtl_get_sort)SML_MtlGetTypeTransparency);
	SGCore_SetFunc_MtlGroupRenderIsSingly((g_func_mtl_group_render_is_singly)SML_MtlGetTypeReflection);
	SGCore_SetFunc_MtlGetPhysicType((g_func_mtl_get_physic_type)SML_MtlGetPhysicMaterial);

	SGCore_SkyBoxCr();
	SGCore_SkyCloudsCr();

	GData::DXDevice = SGCore_GetDXDevice();
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

#ifndef SX_GAME
	GData::ObjCamera = SGCore_CrCamera();
	GData::ObjCamera->SetFOV(GData::ProjFov);
#endif

	SGeom_0Create("sxgeom", false);
	SGeom_Dbg_Set(PrintfLog);

	SML_0Create("sxml", false);
	SML_Dbg_Set(PrintfLog);

	SPE_0Create("sxparticles", false);
	SPE_Dbg_Set(PrintfLog);
	SPE_SetFunc_ParticlesPhyCollision(RFuncParticlesPhyCollision);
	SPE_RTDepthSet(SML_DSGetRT_ID(DS_RT::ds_rt_depth));
	Level::LoadParticles();

	SPP_0Create("sxpp", false);
	SPP_Dbg_Set(PrintfLog);

#if defined(SX_GAME)
	SPP_ChangeTexSun("fx_sun.dds");
#endif
	SPP_RTSetInput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com));
	SPP_RTSetOutput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com2));
	SPP_RTSetDepth0(SML_DSGetRT_ID(DS_RT::ds_rt_depth0));
	SPP_RTSetDepth1(SML_DSGetRT_ID(DS_RT::ds_rt_depth1));
	SPP_RTSetNormal(SML_DSGetRT_ID(DS_RT::ds_rt_normal));

	SXAnim_0Create();
	SXAnim_Dbg_Set(PrintfLog);

	SXPhysics_0Create();
	SXPhysics_Dbg_Set(PrintfLog);

	SXDecals_0Create();
	SXDecals_Dbg_Set(PrintfLog);

#if defined(SX_LEVEL_EDITOR)
	SAIG_0Create("sxaigrid", true, false);
	SAIG_BBCreate(&float3(0, 0, 0), &float3(10, 10, 10));
#else
	SAIG_0Create("sxaigrid", true, false);
#endif
	SAIG_Dbg_Set(PrintfLog);
	SAIG_SetFunc_QuadPhyNavigate(RFuncAIQuadPhyNavigate);

#ifndef SX_PARTICLES_EDITOR
	SXGame_0Create();
	SXGame_Dbg_Set(PrintfLog);
#endif

#ifdef SX_GAME
	GData::ObjCamera = SXGame_GetActiveCamera();
#endif

#if defined(SX_LEVEL_EDITOR) || defined(SX_PARTICLES_EDITOR)
	GData::Editors::ObjGrid = new Grid();
	GData::Editors::ObjGrid->Create(100, 100, D3DCOLOR_ARGB(255, 200, 200, 200));

	GData::Editors::ObjAxesStatic = new AxesStatic();
	GData::Editors::ObjAxesStatic->Create(1);
	GData::Editors::ObjAxesHelper = new AxesHelper();

	GData::ObjCamera->SetPosition(&float3(0, 0.5, -2));

	GData::Editors::RenderGrid = GData::Editors::RenderAxesStatic = true;
#endif

#if defined(SX_MATERIAL_EDITOR)
	GData::ObjCamera->SetPosition(&float3(0, 0, -1.2 * 100));
#endif

#if defined(SX_PARTICLES_EDITOR)
	GData::Editors::RenderBound = true;

	D3DXCreateBox(GData::DXDevice, 1, 1, 1, &GData::Editors::FigureBox, 0);
	D3DXCreateSphere(GData::DXDevice, 1, 20, 20, &GData::Editors::FigureSphere, 0);

	GData::Editors::FigureConeParam.x = 1;
	GData::Editors::FigureConeParam.y = 0.1;
	GData::Editors::FigureConeParam.z = 1;

	SGCore_FCreateCone(GData::Editors::FigureConeParam.x, GData::Editors::FigureConeParam.y, GData::Editors::FigureConeParam.z, &GData::Editors::FigureCone, 20);

#endif

#if defined(SX_LEVEL_EDITOR)
	D3DXCreateBox(GData::DXDevice, 1, 1, 1, &GData::Editors::FigureBox, 0);
#endif

	GData::InitAllMatrix();

	GData::IDsShaders::InitAllShaders();

	Core_0RegisterCVarFloat("cl_mousesense", 0.001f, "Mouse sense value");

	Core_0RegisterCVarInt("pp_ssao", 1, 0);
	Core_0RegisterCVarBool("pp_bloom", true, 0);
	Core_0RegisterCVarBool("pp_lensflare", true, 0);
	Core_0RegisterCVarBool("pp_lensflare_usebloom", true, 0);
	Core_0RegisterCVarBool("pp_dlaa", true, 0);
	Core_0RegisterCVarBool("pp_nfaa", false, 0);

	Core_0RegisterCVarFloat("pp_fog_density", 0.5, 0);
	Core_0RegisterCVarFloat("pp_fog_sky", 0.0, 0);
	Core_0RegisterCVarFloat("pp_fog_min", 0.0, 0);
	Core_0RegisterCVarFloat("pp_fog_max", 0.9, 0);

	Core_0RegisterCVarBool("pp_motionblur", true, 0);
	Core_0RegisterCVarFloat("pp_motionblur_coef", 0.1, 0);
	Core_0RegisterCVarBool("pssm_4or3", true, 0);

	Core_0RegisterCVarBool("pssm_shadowed", true, 0);
	Core_0RegisterCVarFloat("pssm_q", 1, 0);
	Core_0RegisterCVarFloat("lsm_q", 1, 0);
	Core_0RegisterCVarInt("shadow_soft", 1, 0);

	Core_0RegisterCVarInt("grass_frec", 100, 0);
	Core_0RegisterCVarFloat("green_lod0", 50, 0);
	Core_0RegisterCVarFloat("green_lod1", 100, 0);
	Core_0RegisterCVarFloat("green_less", 20, 0);
	Core_0RegisterCVarFloat("p_far", 400, 0);
	Core_0RegisterCVarFloat("hdr_adapted_coef", 0.03f, 0);

	Core_0RegisterCVarInt("r_s_filter", 2, 0);
	Core_0RegisterCVarInt("r_s_max_anisotropy", 16, 0);
	Core_0RegisterCVarInt("r_s_max_miplevel", 0, 0);
	Core_0RegisterCVarInt("rs_stats", 1, 0);

	Core_0RegisterCVarBool("g_time_run", true, 0);
	Core_0RegisterCVarFloat("g_time_speed", 1.f, 0);
	
	Core_0RegisterCVarFloat("main_rain_density", 1.f, 0);
	Core_0RegisterCVarBool("main_thunderbolt", true, 0);

	Core_0RegisterCVarFloat("weather_snd_volume", 1.f, 0);
	Core_0RegisterCVarFloat("ambient_snd_volume", 1.f, 0);

	static float3_t fog_color(0.5, 0.5, 0.5);
	Core_0RegisterCVarInt("e_pp_fog_color", ((int)&fog_color));

	Core_0RegisterConcmd("screenshot", SXRenderFunc::SaveScreenShot);
	Core_0RegisterConcmd("save_worktex", SXRenderFunc::SaveWorkTex);
	Core_0RegisterConcmd("shader_reload", SGCore_ShaderReloadAll);

#if defined(SX_GAME)
	Core_0RegisterConcmd("change_mode_window", SXRenderFunc::ChangeModeWindow);
#endif

	Core_0ConsoleExecCmd("exec ../sysconfig.cfg");
	Core_0ConsoleExecCmd("exec ../userconfig.cfg");

#if !defined(SX_GAME)
	Core_0ConsoleExecCmd("exec ../editor.cfg");
#endif

#if defined(SX_GAME)
	if (GData::IsWindowed)
		ShowWindow(GData::Handle3D, SW_SHOW);
	else
		ShowWindow(GData::Handle3D, SW_MAXIMIZE);
#endif

	/*IAnimPlayer * pl;
	pl = SXAnim_CreatePlayer("models/stalker_zombi/stalker_zombi_a.dse");
	pl->SetPos(float3(-17.18, -1.38f, -32.3));
	pl->Play("reload");*/
}

//**************************************************************************

void SkyXEngine_InitPaths()
{
	char tmppath[MAX_PATH];
	char tmppathexe[MAX_PATH];
	GetModuleFileName(NULL, tmppath, MAX_PATH);
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

	Core_RStringSet(G_RI_STRING_PATH_EXE, tmppathexe);

	sprintf(tmppath, "%s%s", tmppathexe, "\\worktex\\");
	Core_RStringSet(G_RI_STRING_PATH_WORKTEX, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\");
	Core_RStringSet(G_RI_STRING_PATH_GAMESOURCE, tmppath);
	SetCurrentDirectoryA(tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\screenshots\\");
	Core_RStringSet(G_RI_STRING_PATH_SCREENSHOTS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\config\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_CONFIGS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\levels\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_LEVELS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\meshes\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_MESHES, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\models\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_MODELS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\shaders\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_SHADERS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\sounds\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_SOUNDS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\scripts\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_SCRIPTS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\textures\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_TEXTURES, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\materials\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_MTRLS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "\\gamesource\\resource\\");
	Core_RStringSet(G_RI_STRING_PATH_GS_GUI, tmppath);
}

//#############################################################################

void SkyXEngine_Render(DWORD timeDelta)
{
	int64_t ttime;
	//потеряно ли устройство или произошло изменение размеров?
	if (GData::DXDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET || GData::ReSize)
	{
		//если не свернуто окно
		if (!IsIconic(GData::Handle3D) && ((GData::HandleParent3D != 0 && !IsIconic(GData::HandleParent3D)) || GData::HandleParent3D == 0))
			SXRenderFunc::ComDeviceLost();	//пытаемся восстановить
		return;
	}

#if !defined(SX_GAME) //&& !defined(SX_MATERIAL_EDITOR)
#if defined(SX_MATERIAL_EDITOR)
	if (GData::Editors::MoveCamera)
#endif
		CameraUpdate::UpdateEditorial(timeDelta);
#endif

	SXAnim_Update();
#ifndef SX_PARTICLES_EDITOR
	SXGame_Update();
	SGame_WeatherUpdate();
	SGame_AmbientSndUpdate();
#endif
	SXPhysics_Update();

	SXAnim_Sync();
	SXPhysics_Sync();
#ifndef SX_PARTICLES_EDITOR
	SXGame_Sync();
#endif

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SGeom_ModelsMSortGroups(&GData::ConstCurrCamPos, 2);
	SXRenderFunc::Delay::GeomSortGroup += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	if (GData::DefaultGeomIDArr < 0)
		GData::DefaultGeomIDArr = SGeom_ModelsAddArrForCom();

	if (GData::DefaultGreenIDArr < 0)
		GData::DefaultGreenIDArr = SGeom_GreenAddArrForCom();

	if (GData::DefaultAnimIDArr < 0)
		GData::DefaultAnimIDArr = SXAnim_ModelsAddArrForCom();

	/**/
	if (SGeom_GreenGetOccurencessLeafGrass(&float3(GData::ConstCurrCamPos - float3(0.25, 1.8, 0.25)), &float3(GData::ConstCurrCamPos + float3(0.25, 0, 0.25)), MtlPhysicType::mpt_leaf_grass))
		SXRenderFunc::Delay::FreeVal = 1;
	else
		SXRenderFunc::Delay::FreeVal = 0;
	/**/

	SXRenderFunc::UpdateView();
	SML_Update(timeDelta);

	GData::DXDevice->BeginScene();
	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SXRenderFunc::UpdateReflection(timeDelta);
	SXRenderFunc::Delay::ComReflection += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	if (GData::FinalImage == DS_RT::ds_rt_ambient_diff || GData::FinalImage == DS_RT::ds_rt_specular || GData::FinalImage == DS_RT::ds_rt_scene_light_com)
	{
		//рендерим глубину от света
		ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
		SXRenderFunc::UpdateShadow(timeDelta);
		SXRenderFunc::Delay::UpdateShadow += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;
	}

	//рисуем сцену и заполняем mrt данными
	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SXRenderFunc::BuildMRT(timeDelta);
	SXRenderFunc::Delay::RenderMRT += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	if (GData::FinalImage == DS_RT::ds_rt_ambient_diff || GData::FinalImage == DS_RT::ds_rt_specular || GData::FinalImage == DS_RT::ds_rt_scene_light_com)
	{
		//освещаем сцену
		ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
		SXRenderFunc::ComLighting(timeDelta, true);
		SXRenderFunc::UnionLayers();
		if (SGCore_SkyBoxIsCr())
			SXRenderFunc::RenderSky(timeDelta);
		SXRenderFunc::ApplyToneMapping();
		SXRenderFunc::ComToneMapping(timeDelta);

		SXRenderFunc::Delay::ComLighting += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;
	}

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

#if defined(SX_GAME)
	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SXRenderFunc::RenderPostProcess(timeDelta);
	SXRenderFunc::Delay::PostProcess += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;
#endif

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::ScreenOut);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#if !defined(SX_GAME)
	GData::DXDevice->SetTexture(0, SML_DSGetRT(GData::FinalImage));
#else
	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(SPP_RTGetCurrSend()));
#endif

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	SXRenderFunc::RenderParticles(timeDelta);

	GData::DXDevice->SetTransform(D3DTS_WORLD, &((D3DXMATRIX)SMMatrixIdentity()));
	GData::DXDevice->SetTransform(D3DTS_VIEW, &((D3DXMATRIX)GData::MCamView));
	GData::DXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)GData::MLightProj));
	SXRenderFunc::RenderEditorMain();
	SXRenderFunc::RenderEditorLE(timeDelta);
	SXRenderFunc::RenderEditorPE(timeDelta);

#ifdef _DEBUG
	SAIG_RenderQuads(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos, GData::NearFar.y);
#endif

#if defined(SX_GAME)
	SXGame_RenderHUD();
#endif

	SXRenderFunc::OutputDebugInfo(timeDelta);

	SXPhysics_DebugRender();

	SXRenderFunc::ShaderRegisterData();

	GData::DXDevice->EndScene();

	//@@@
	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SXRenderFunc::ComVisibleForCamera();
	SXRenderFunc::Delay::UpdateVisibleForCamera += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SXRenderFunc::ComVisibleReflection();
	SXRenderFunc::Delay::UpdateVisibleForReflection += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SXRenderFunc::ComVisibleForLight();
	SXRenderFunc::Delay::UpdateVisibleForLight += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SPE_EffectVisibleComAll(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);
	SPE_EffectComputeAll();
	SPE_EffectComputeLightingAll();
	SXRenderFunc::Delay::UpdateParticles += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	GData::DXDevice->Present(0, 0, 0, 0);
	SXRenderFunc::Delay::Present += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	SXRenderFunc::UpdateDataCVar();

#if defined(SX_LEVEL_EDITOR)
	GData::Editors::LevelEditorUpdate();
#endif

#if defined(SX_MATERIAL_EDITOR)
	GData::Editors::MaterialEditorUpdate();
#endif

#if defined(SX_PARTICLES_EDITOR)
	GData::Editors::ParticlesEditorUpdate();
#endif
}

//#############################################################################

int SkyXEngine_CycleMain()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT && IsWindow(GData::Handle3D))
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);

#if !defined(SX_GAME)
			IMSG imsg;
			imsg.lParam = msg.lParam;
			imsg.wParam = msg.wParam;
			imsg.message = msg.message;

			SSInput_AddMsg(imsg);
#endif

			::DispatchMessage(&msg);
		}
		else
		{
			Core_TimesUpdate();
			Core_0ConsoleUpdate();
			SSInput_Update();
			SSCore_Update(&GData::ConstCurrCamPos, &GData::ConstCurrCamDir);
			SGCore_LoadTexLoadTextures();

			static DWORD lastTime = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
			DWORD currTime = TimeGetMls(Core_RIntGet(G_RI_INT_TIMER_RENDER));
			DWORD timeDelta = (currTime - lastTime);
			Core_RIntSet(G_RI_INT_TIME_DELTA, timeDelta);
#ifdef SX_GAME
			GData::ObjCamera = SXGame_GetActiveCamera();
#endif


			static const bool * g_time_run = GET_PCVAR_BOOL("g_time_run");
			static bool g_time_run_old = true;

			if (g_time_run && g_time_run_old != (*g_time_run))
			{
				g_time_run_old = (*g_time_run);
				Core_TimeWorkingSet(Core_RIntGet(G_RI_INT_TIMER_GAME), g_time_run_old);
			}

			static const float * g_time_speed = GET_PCVAR_FLOAT("g_time_speed");
			static float g_time_speed_old = true;

			if (g_time_speed && g_time_speed_old != (*g_time_speed))
			{
				g_time_speed_old = (*g_time_speed);
				Core_TimeSpeedSet(Core_RIntGet(G_RI_INT_TIMER_GAME), g_time_speed_old);
			}


			if (Core_TimeWorkingGet(Core_RIntGet(G_RI_INT_TIMER_RENDER)) && (GetForegroundWindow() == GData::Handle3D || GetForegroundWindow() == GData::HandleParent3D || GetForegroundWindow() == FindWindow(NULL, "sxconsole")))
			{

#if defined(SX_LEVEL_EDITOR)
				SXLevelEditor_Transform(10);
#endif

#if defined(SX_MATERIAL_EDITOR)
				if (SXMaterialEditor::CheckBoxModelRot->GetCheck())
					GData::Editors::SimModel->Rotation.y -= float(timeDelta) * 0.001f * 0.25;
#endif

				SkyXEngine_Render(timeDelta);
			}

			lastTime = currTime;
		}
	}

	return msg.wParam;
}

//#############################################################################

void SkyXEngine_Kill()
{
#if defined(SX_MATERIAL_EDITOR)
	mem_delete(GData::Editors::SimModel);
#endif

#if !defined(SX_GAME)
	mem_delete(GData::Editors::ObjGrid);
	mem_delete(GData::Editors::ObjAxesStatic);
	mem_release(GData::Editors::FigureBox);
	mem_release(GData::Editors::FigureSphere);
	mem_release(GData::Editors::FigureCone);
	mem_delete(GData::Editors::ObjAxesHelper);
#endif

#if !defined(SX_PARTICLES_EDITOR)
	SXGame_AKill();
#endif
	SPE_AKill();
	SXDecals_AKill();
	SXPhysics_AKill();
	SXAnim_AKill();
#ifndef SX_GAME
	mem_release(GData::ObjCamera);
#endif
	SGeom_AKill();
	SML_AKill();
	SSCore_AKill();
	SGCore_AKill();
	Core_AKill();
}

//#############################################################################

LRESULT CALLBACK SkyXEngine_PreviewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HDC hmdc;
	HBITMAP hBitmap;
	BITMAP bm;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hmdc = CreateCompatibleDC(hdc);
		hBitmap = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP_PREVIEW));
		SelectObject(hmdc, hBitmap);
		GetObject(hBitmap, sizeof(bm), &bm);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hmdc, 0, 0, SRCCOPY);
		DeleteDC(hmdc);
		DeleteObject(hBitmap);
		EndPaint(hWnd, &ps);
		break;
	}

	return(DefWindowProc(hWnd, message, wParam, lParam));
}

void SkyXEngine_PreviewCreate()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = SkyXEngine_PreviewWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(0);
	wcex.hIcon = NULL;
	wcex.hCursor = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "SkyXEngine_Preview";
	wcex.hIconSm = 0;

	RegisterClassEx(&wcex);

	int width = 1024;
	int height = 256;

	int posx = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int posy = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	GData::HandlePreview = CreateWindowEx(
		WS_EX_TOPMOST,
		"SkyXEngine_Preview",
		"SkyXEngine_Preview",
		WS_POPUP,
		(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - height) / 2,
		width, height,
		0, 0,
		GetModuleHandle(0),
		0);

	ShowWindow(GData::HandlePreview, SW_SHOW);

	DWORD ttime = GetTickCount();
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (GetTickCount() - ttime > 500)
			break;

		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
}

void SkyXEngine_PreviewKill()
{
	DestroyWindow(GData::HandlePreview);
	GData::HandlePreview = 0;
}