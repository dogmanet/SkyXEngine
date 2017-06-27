
#include <SkyXEngine.h>

//!< инициализация движка
void SkyXEngine_Init()
{
	srand((UINT)time(0));
	InitOutLog();
	GData::Pathes::InitAllPathes();
	if (!Core_0IsProcessRun("sxconsole.exe"))
		ShellExecute(NULL, "open", "sxconsole.exe", NULL, GData::Pathes::ForExe, SW_SHOWNORMAL);

	SetConsoleTitle("sxconsole");

#if defined(SX_GAME)
	GData::InitWin("SkyXEngine", "SkyXEngine");
#endif
	Level::Name[0] = 0;
	SSInput_0Create("sxinput", GData::Handle3D, true);
	SSInput_Dbg_Set(printflog);

	Core_0Create("sxcore", true);
	Core_SetOutPtr();

	G_Timer_Render_Scene = Core_TimeAdd();
	G_Timer_Game = Core_TimeAdd();
	Core_RIntSet(G_RI_INT_TIMER_RENDER, G_Timer_Render_Scene);
	Core_RIntSet(G_RI_INT_TIMER_GAME, G_Timer_Game);

	tm ct = { 0, 0, 9, 27, 5, 2030 - 1900, 0, 0, 0 };
	Core_TimeUnixStartSet(G_Timer_Game, mktime(&ct));

	Core_TimeWorkingSet(G_Timer_Render_Scene, true);
	Core_TimeWorkingSet(G_Timer_Game, true);

	SSCore_0Create("sxsound", GData::Handle3D, GData::Pathes::Sounds, false);
	SSCore_Dbg_Set(printflog);

	SGCore_0Create("sxgcore", GData::Handle3D, GData::WinSize.x, GData::WinSize.y, GData::IsWindowed, 0, true);
	SGCore_Dbg_Set(printflog);
	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_ShaderSetStdPath(GData::Pathes::Shaders);

	SGCore_SetFunc_MtlSet(SXRenderFunc::RFuncMtlSet);
	SGCore_SetFunc_MtlLoad(SXRenderFunc::RFuncMtlLoad);
	SGCore_SetFunc_MtlGetSort((g_func_mtl_get_sort)SML_MtlGetTypeTransparency);
	SGCore_SetFunc_MtlGroupRenderIsSingly((g_func_mtl_group_render_is_singly)SML_MtlGetTypeReflection);
	SGCore_SetFunc_MtlGetPhysicType((g_func_mtl_get_physic_type)SML_MtlGetPhysicMaterial);

	SGCore_SkyBoxCr();
	SGCore_SkyBoxSetStdPathTex(GData::Pathes::TexturesSkyBoxes);

	SGCore_SkyCloudsCr();
	SGCore_SkyCloudsSetStdPathTex(GData::Pathes::TexturesSkyBoxes);

	GData::DXDevice = SGCore_GetDXDevice();
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

#ifndef SX_GAME
	GData::ObjCamera = SGCore_CrCamera();
	GData::ObjCamera->SetFOV(GData::ProjFov);
#endif

	SGeom_0Create("sxgeom", SGCore_GetDXDevice(), GData::Pathes::Meshes, true);
	SGeom_Dbg_Set(printflog);

	SML_0Create("sxml", SGCore_GetDXDevice(), GData::Pathes::Materials, GData::Pathes::Meshes, &GData::WinSize, GData::ProjFov, false);
	SML_Dbg_Set(printflog);

	SPE_0Create("sxparticles", SGCore_GetDXDevice(), false);
	SPE_Dbg_Set(printflog);
	SPE_RTDepthSet(SML_DSGetRT_ID(DS_RT::ds_rt_depth));
	Level::LoadParticles();

	SPP_0Create("sxpp", SGCore_GetDXDevice(), &GData::WinSize, false);
	SPP_Dbg_Set(printflog);

#if defined(SX_GAME)
	SPP_ChangeTexSun("fx_sun.dds");
#endif
	SPP_RTSetInput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com));
	SPP_RTSetOutput(SML_DSGetRT_ID(DS_RT::ds_rt_scene_light_com2));
	SPP_RTSetDepth0(SML_DSGetRT_ID(DS_RT::ds_rt_depth0));
	SPP_RTSetDepth1(SML_DSGetRT_ID(DS_RT::ds_rt_depth1));
	SPP_RTSetNormal(SML_DSGetRT_ID(DS_RT::ds_rt_normal));

	SXAnim_0Create();
	SXAnim_Dbg_Set(printflog);

	SXPhysics_0Create();
	SXPhysics_Dbg_Set(printflog);

	SXDecals_0Create();
	SXDecals_Dbg_Set(printflog);

#ifndef SX_PARTICLES_EDITOR
	SXGame_0Create();
	SXGame_Dbg_Set(printflog);
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

	Core_0RegisterCVarInt("pp_ssao", 1, 0);
	Core_0RegisterCVarBool("pp_bloom", true, 0);
	Core_0RegisterCVarBool("pp_lensflare", true, 0);
	Core_0RegisterCVarBool("pp_lensflare_usebloom", true, 0);
	Core_0RegisterCVarBool("pp_dlaa", true, 0);
	Core_0RegisterCVarBool("pp_nfaa", false, 0);

	Core_0RegisterCVarBool("pp_motionblur", true, 0);
	Core_0RegisterCVarFloat("pp_motionblur_coef", 0.1, 0);
	Core_0RegisterCVarBool("pssm_4or3", true, 0);

	Core_0RegisterCVarFloat("pssm_q", 1, 0);
	Core_0RegisterCVarFloat("lsm_q", 1, 0);

	Core_0RegisterCVarInt("grass_frec", 100, 0);
	Core_0RegisterCVarFloat("green_lod0", 50, 0);
	Core_0RegisterCVarFloat("green_lod1", 100, 0);
	Core_0RegisterCVarFloat("green_less", 20, 0);
	Core_0RegisterCVarFloat("p_far", 400, 0);

	Core_0RegisterCVarInt("r_s_filter", 1, 0);
	Core_0RegisterCVarInt("r_s_max_anisotropy", 0, 0);
	Core_0RegisterCVarInt("r_s_max_miplevel", 0, 0);

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
}

//!< рендер
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
#endif
	SXPhysics_Update();

	SXAnim_Sync();
	SXPhysics_Sync();
#ifndef SX_PARTICLES_EDITOR
	SXGame_Sync();
#endif

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SGeom_ModelsMSortGroups(&GData::ConstCurrCamPos, 2);
	SXRenderFunc::Delay::GeomSortGroup += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

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
	SML_Update(timeDelta, &GData::WinSize, &GData::NearFar, &GData::ConstCurrCamPos, &GData::MCamView, GData::ProjFov);

	GData::DXDevice->BeginScene();

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::UpdateReflection(timeDelta);
	SXRenderFunc::Delay::ComReflection += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	if (GData::FinalImage == DS_RT::ds_rt_ambient_diff || GData::FinalImage == DS_RT::ds_rt_specular || GData::FinalImage == DS_RT::ds_rt_scene_light_com)
	{
		//рендерим глубину от света
		ttime = TimeGetMcsU(G_Timer_Render_Scene);
		SXRenderFunc::UpdateShadow(timeDelta);
		SXRenderFunc::Delay::UpdateShadow += TimeGetMcsU(G_Timer_Render_Scene) - ttime;
	}

	//рисуем сцену и заполняем mrt данными
	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::RenderInMRT(timeDelta);
	SXRenderFunc::Delay::RenderMRT += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	if (GData::FinalImage == DS_RT::ds_rt_ambient_diff || GData::FinalImage == DS_RT::ds_rt_specular || GData::FinalImage == DS_RT::ds_rt_scene_light_com)
	{
		//освещаем сцену
		ttime = TimeGetMcsU(G_Timer_Render_Scene);
		SXRenderFunc::ComLighting(timeDelta, true);
		SXRenderFunc::Delay::ComLighting += TimeGetMcsU(G_Timer_Render_Scene) - ttime;
	}

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

#if defined(SX_GAME)
	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::RenderPostProcess(timeDelta);
	SXRenderFunc::Delay::PostProcess += TimeGetMcsU(G_Timer_Render_Scene) - ttime;
#endif

	SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::ScreenOut);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#if !defined(SX_GAME)
	GData::DXDevice->SetTexture(0, SML_DSGetRT(GData::FinalImage));
#else
	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(SPP_RTGetCurrSend()));
#endif

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	SXRenderFunc::RenderParticles(timeDelta);

	SXRenderFunc::RenderEditorMain();
	SXRenderFunc::RenderEditorLE(timeDelta);

	SXRenderFunc::OutputDebugInfo(timeDelta);

	SXPhysics_DebugRender();

	SXRenderFunc::ShaderRegisterData();

	GData::DXDevice->EndScene();

	//@@@
	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::ComVisibleForCamera();
	SXRenderFunc::Delay::UpdateVisibleForCamera += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::ComVisibleReflection();
	SXRenderFunc::Delay::UpdateVisibleForReflection += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SXRenderFunc::ComVisibleForLight();
	SXRenderFunc::Delay::UpdateVisibleForLight += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	SPE_EffectVisibleComAll(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);
	SPE_EffectComputeAll();
	SPE_EffectComputeLightingAll();
	SXRenderFunc::Delay::UpdateParticles += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	ttime = TimeGetMcsU(G_Timer_Render_Scene);
	GData::DXDevice->Present(0, 0, 0, 0);
	SXRenderFunc::Delay::Present += TimeGetMcsU(G_Timer_Render_Scene) - ttime;

	SXRenderFunc::UpdateDataCVar();

#if defined(SX_LEVEL_EDITOR)
	GData::Editors::LevelEditorUpdateStatusBar();
#endif

#if defined(SX_PARTICLES_EDITOR)
	GData::Editors::ParticlesEditorUpdateStatusBar();
#endif
}

//!< запуск основного цикла обработки
int SkyXEngine_CycleMain()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static DWORD lastTime = TimeGetMls(G_Timer_Render_Scene);

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

			if (SSInput_GetKeyEvents(SIK_T) == InputEvents::iv_k_first)
			{
				//Core_TimeWorkingSet(G_Timer_Render_Scene, !Core_TimeWorkingGet(G_Timer_Render_Scene));
				/*for (int i = 0; i < 1; ++i)
				{
				ID tmpid = SPE_EffectIdOfKey(i);
				//SPE_EffectPosSet(tmpid, &float3(i, 0, k));
				SPE_EffectAlifeSet(tmpid, !SPE_EffectAlifeGet(tmpid));
				}*/
			}

			DWORD currTime = TimeGetMls(G_Timer_Render_Scene);
			DWORD timeDelta = (currTime - lastTime);

#ifdef SX_GAME
			GData::ObjCamera = SXGame_GetActiveCamera();
#endif

			if (Core_TimeWorkingGet(G_Timer_Render_Scene) && (GetForegroundWindow() == GData::Handle3D || GetForegroundWindow() == GData::HandleParent3D || GetForegroundWindow() == FindWindow(NULL, "sxconsole")))
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

//!< уничтожение данных движка, освобождение памяти
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

	int width = 512;
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