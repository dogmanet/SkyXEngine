
#include "SkyXEngine.h"

//##########################################################################

//! поток ведения лога
FILE *g_pFileOutLog = 0;	

BOOL CALLBACK SkyXEngine_EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	if (pid == lParam && IsWindow(hwnd))
		SendMessage(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
	return TRUE;
}

void SkyXEngine_HandlerError(const char *szFormat, ...)
{
	va_list va;
	char buf[4096];
	va_start(va, szFormat);
	vsprintf_s(buf, 4096, szFormat, va);
	va_end(va);

	DWORD PID = GetCurrentProcessId();
	EnumWindows(SkyXEngine_EnumWindowsProc, PID);

	MessageBox(0, buf, "SkyXEngine error", MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL);
	exit(0);
}

void SkyXEngine_InitOutLog()
{
	AllocConsole();
	freopen("CONOUT$", "wt", stdout);
	freopen("CONOUT$", "wt", stderr);
	freopen("CONIN$", "rt", stdin);

	char path[256];
	char PathForExe[1024];
	GetModuleFileName(NULL, path, 256);
	int len = strlen(path);
	while (path[len--] != '\\')
	{
		if (path[len - 1] == '\\')
		{
			len--;
			memcpy(PathForExe, path, len);
			PathForExe[len] = 0;
		}
	}
	char FileLogPath[1024];
	sprintf(FileLogPath, "%s%s", PathForExe, "/log.txt");
	g_pFileOutLog = fopen(FileLogPath, "w");

	if (!g_pFileOutLog)
	{
		SkyXEngine_HandlerError("Debug system is not init, this is big problem!");
		exit(0);
	}
}

void SkyXEngine_PrintfLog(int level, const char *szFormat, ...)
{
	va_list va;
	char buf[REPORT_MSG_MAX_LEN];
	va_start(va, szFormat);
	vsprintf_s(buf, REPORT_MSG_MAX_LEN, szFormat, va);
	va_end(va);

	if (g_pFileOutLog)
	{
		if (level == REPORT_MSG_LEVEL_ERROR)
		{
			printf(COLOR_LRED "! ");
			fwrite("! ", 1, 2, g_pFileOutLog);
		}
		else if (level == REPORT_MSG_LEVEL_WARNING)
		{
			printf(COLOR_YELLOW "* ");
			fwrite("* ", 1, 2, g_pFileOutLog);
		}

		printf(buf);
		if (level == REPORT_MSG_LEVEL_ERROR || level == REPORT_MSG_LEVEL_WARNING)
		{
			printf(COLOR_RESET);
		}
		fwrite(buf, 1, strlen(buf), g_pFileOutLog);
		//fprintf(FileOutLog, "\n");
		fflush(g_pFileOutLog);

		if (level == REPORT_MSG_LEVEL_ERROR)
		{
			SkyXEngine_HandlerError(buf);
		}
	}
}

//##########################################################################

void SkyXEngine_Init()
{
	srand((UINT)time(0));
	SkyXEngine_InitOutLog();
	SkyXEngine_InitPaths();

	if (!Core_0IsProcessRun("sxconsole.exe"))
		ShellExecute(0, "open", "sxconsole.exe", 0, Core_RStringGet(G_RI_STRING_PATH_EXE), SW_SHOWNORMAL);

	HWND hWnd3D = 0;
#if defined(SX_GAME)
	hWnd3D = SkyXEngine_CreateWindow("SkyXEngine", "SkyXEngine", 800, 600);
#endif
	
	SSInput_0Create("sxinput", hWnd3D, false);
	SSInput_Dbg_Set(SkyXEngine_PrintfLog);

	Core_0Create("sxcore", false);
	Core_Dbg_Set(SkyXEngine_PrintfLog);
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

	SSCore_0Create("sxsound", hWnd3D, false);
	SSCore_Dbg_Set(SkyXEngine_PrintfLog);

	SGCore_0Create("sxgcore", hWnd3D, GData::WinSize.x, GData::WinSize.y, GData::IsWindowed, 0, false);
	SGCore_Dbg_Set(SkyXEngine_PrintfLog);

	SGCore_SetFunc_MtlSet(RFuncMtlSet);
	SGCore_SetFunc_MtlLoad(RFuncMtlLoad);
	SGCore_SetFunc_MtlGetSort((g_func_mtl_get_sort)SML_MtlGetTypeTransparency);
	SGCore_SetFunc_MtlGroupRenderIsSingly((g_func_mtl_group_render_is_singly)SML_MtlGetTypeReflection);
	SGCore_SetFunc_MtlGetPhysicType((g_func_mtl_get_physic_type)SML_MtlGetPhysicMaterial);

	SGCore_SkyBoxCr();
	SGCore_SkyCloudsCr();

	GData::DXDevice = SGCore_GetDXDevice();
	SGCore_GetDXDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

#ifndef SX_GAME
	ISXCamera *pCamera = SGCore_CrCamera();
	pCamera->SetFOV(GData::ProjFov);

	SRender_SetCamera(pCamera);
#endif

	SGeom_0Create("sxgeom", false);
	SGeom_Dbg_Set(SkyXEngine_PrintfLog);

	SML_0Create("sxml", false);
	SML_Dbg_Set(SkyXEngine_PrintfLog);

	SPE_0Create("sxparticles", false);
	SPE_Dbg_Set(SkyXEngine_PrintfLog);
	SPE_SetFunc_ParticlesPhyCollision(RFuncParticlesPhyCollision);
	SPE_RTDepthSet(SML_DSGetRT_ID(DS_RT_DEPTH));
	

	SPP_0Create("sxpp", false);
	SPP_Dbg_Set(SkyXEngine_PrintfLog);

#if defined(SX_GAME)
	SPP_ChangeTexSun("fx_sun.dds");
#endif
	SPP_RTSetInput(SML_DSGetRT_ID(DS_RT_SCENELIGHT));
	SPP_RTSetOutput(SML_DSGetRT_ID(DS_RT_SCENELIGHT2));
	SPP_RTSetDepth0(SML_DSGetRT_ID(DS_RT_DEPTH0));
	SPP_RTSetDepth1(SML_DSGetRT_ID(DS_RT_DEPTH1));
	SPP_RTSetNormal(SML_DSGetRT_ID(DS_RT_NORMAL));

	SXAnim_0Create();
	SXAnim_Dbg_Set(SkyXEngine_PrintfLog);

	SXPhysics_0Create();
	SXPhysics_Dbg_Set(SkyXEngine_PrintfLog);

	SXDecals_0Create();
	SXDecals_Dbg_Set(SkyXEngine_PrintfLog);

#if defined(SX_LEVEL_EDITOR)
	SAIG_0Create("sxaigrid", true, false);
	SAIG_BBCreate(&float3(0, 0, 0), &float3(10, 10, 10));
#else
	SAIG_0Create("sxaigrid", true, false);
#endif
	SAIG_Dbg_Set(SkyXEngine_PrintfLog);
	SAIG_SetFunc_QuadPhyNavigate(RFuncAIQuadPhyNavigate);


	SLevel_0Create("sxlevel", false);
	SLevel_Dbg_Set(SkyXEngine_PrintfLog);


#ifndef SX_PARTICLES_EDITOR
	SXGame_0Create();
	SXGame_Dbg_Set(SkyXEngine_PrintfLog);
#endif

#ifdef SX_GAME
	//GData::ObjCamera = SXGame_GetActiveCamera();
	SRender_SetCamera(SXGame_GetActiveCamera());
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

	/*GData::InitAllMatrix();

	GData::IDsShaders::InitAllShaders();*/

	SRender_0Create("sxrender", hWnd3D, 0, false);
	SRender_Dbg_Set(SkyXEngine_PrintfLog);


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
	Core_0RegisterCVarPointer("pp_fog_color", ((UINT_PTR)&fog_color));

	Core_0RegisterConcmd("screenshot", SRender_SaveScreenShot);
	Core_0RegisterConcmd("save_worktex", SRender_SaveWorkTex);
	Core_0RegisterConcmd("shader_reload", SGCore_ShaderReloadAll);

#if defined(SX_GAME)
	Core_0RegisterConcmd("change_mode_window", SRender_ChangeModeWindow);
#endif

	Core_0ConsoleExecCmd("exec ../sysconfig.cfg");
	Core_0ConsoleExecCmd("exec ../userconfig.cfg");

#if !defined(SX_GAME)
	Core_0ConsoleExecCmd("exec ../editor.cfg");
#endif

#if defined(SX_GAME)
	if (GData::IsWindowed)
		ShowWindow(hWnd3D, SW_SHOW);
	else
		ShowWindow(hWnd3D, SW_MAXIMIZE);
#endif
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

	sprintf(tmppath, "%s%s", tmppathexe, "/worktex/");
	Core_RStringSet(G_RI_STRING_PATH_WORKTEX, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/");
	Core_RStringSet(G_RI_STRING_PATH_GAMESOURCE, tmppath);
	SetCurrentDirectoryA(tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/screenshots/");
	Core_RStringSet(G_RI_STRING_PATH_SCREENSHOTS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/config/");
	Core_RStringSet(G_RI_STRING_PATH_GS_CONFIGS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/levels/");
	Core_RStringSet(G_RI_STRING_PATH_GS_LEVELS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/meshes/");
	Core_RStringSet(G_RI_STRING_PATH_GS_MESHES, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/models/");
	Core_RStringSet(G_RI_STRING_PATH_GS_MODELS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/shaders/");
	Core_RStringSet(G_RI_STRING_PATH_GS_SHADERS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/sounds/");
	Core_RStringSet(G_RI_STRING_PATH_GS_SOUNDS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/scripts/");
	Core_RStringSet(G_RI_STRING_PATH_GS_SCRIPTS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/textures/");
	Core_RStringSet(G_RI_STRING_PATH_GS_TEXTURES, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/materials/");
	Core_RStringSet(G_RI_STRING_PATH_GS_MTRLS, tmppath);

	sprintf(tmppath, "%s%s", tmppathexe, "/gamesource/resource/");
	Core_RStringSet(G_RI_STRING_PATH_GS_GUI, tmppath);
}

LRESULT CALLBACK SkyXEngine_WndProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	IMSG msg;
	msg.lParam = lParam;
	msg.wParam = wParam;
	msg.message = uiMessage;

	SSInput_AddMsg(msg);

	switch (uiMessage)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hWnd, uiMessage, wParam, lParam));
}

HWND SkyXEngine_CreateWindow(const char *szName, const char *szCaption, int iWidth, int iHeight)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = SkyXEngine_WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(0);
	wcex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_LOGO));
	wcex.hCursor = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szName;
	wcex.hIconSm = wcex.hIcon;

	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, iWidth, iHeight };
	AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);

	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	int posx = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	int posy = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	HWND hWnd = CreateWindowEx(
		0,
		szName,
		szCaption,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		posx, posy,
		width, height,
		0, 0,
		GetModuleHandle(0),
		0);

	ShowWindow(hWnd, SW_HIDE);
	return hWnd;
}

//#############################################################################

void SkyXEngine_Frame(DWORD timeDelta)
{
	static IDirect3DDevice9 *pDXDevice = SGCore_GetDXDevice();
	static float3 vCamPos, vCamDir;
	static float4x4 mView, mProjLight;

	if (!pDXDevice)
	{
		SkyXEngine_PrintfLog(REPORT_MSG_LEVEL_ERROR, "dxdevice not found ...");
		return;
	}

	int64_t ttime;
	//потеряно ли устройство или произошло изменение размеров?
	if (pDXDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET || GData::ReSize)
	{
		//если не свернуто окно
		if (!IsIconic((HWND)SRender_GetHandleWin3D()) && ((SRender_GetParentHandleWin3D() != 0 && !IsIconic((HWND)SRender_GetParentHandleWin3D())) || SRender_GetParentHandleWin3D() == 0))
			SRender_ComDeviceLost();	//пытаемся восстановить
		return;
	}

#if !defined(SX_GAME) //&& !defined(SX_MATERIAL_EDITOR)
#if defined(SX_MATERIAL_EDITOR)
	if (GData::Editors::MoveCamera)
#endif
		SRender_UpdateEditorial(timeDelta);
#endif

	SXAnim_Update();
#ifndef SX_PARTICLES_EDITOR
	SXGame_Update();
	SLevel_WeatherUpdate();
	SLevel_AmbientSndUpdate();
#endif
	SXPhysics_Update();

	SXAnim_Sync();
	SXPhysics_Sync();
#ifndef SX_PARTICLES_EDITOR
	SXGame_Sync();
#endif

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SGeom_ModelsMSortGroups(&vCamPos, 2);
	//SXRenderFunc::Delay::GeomSortGroup += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	if (GData::DefaultGeomIDArr < 0)
		GData::DefaultGeomIDArr = SGeom_ModelsAddArrForCom();

	if (GData::DefaultGreenIDArr < 0)
		GData::DefaultGreenIDArr = SGeom_GreenAddArrForCom();

	if (GData::DefaultAnimIDArr < 0)
		GData::DefaultAnimIDArr = SXAnim_ModelsAddArrForCom();

	/**/
	/*if (SGeom_GreenGetOccurencessLeafGrass(&float3(vCamPos - float3(0.25, 1.8, 0.25)), &float3(vCamPos + float3(0.25, 0, 0.25)), MTLTYPE_PHYSIC_LEAF_GRASS))
		SXRenderFunc::Delay::FreeVal = 1;
	else
		SXRenderFunc::Delay::FreeVal = 0;*/
	/**/

	SRender_UpdateView();

	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_DIRECTION, &vCamDir);

	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_LIGHT_PROJ, &mProjLight);

	SML_Update(timeDelta);

	pDXDevice->BeginScene();
	pDXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SRender_UpdateReflection(timeDelta);
	//SXRenderFunc::Delay::ComReflection += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	if (GData::FinalImage == DS_RT_AMBIENTDIFF || GData::FinalImage == DS_RT_SPECULAR || GData::FinalImage == DS_RT_SCENELIGHT)
	{
		//рендерим глубину от света
		ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
		SRender_UpdateShadow(timeDelta);
		//SXRenderFunc::Delay::UpdateShadow += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;
	}

	//рисуем сцену и заполняем mrt данными
	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SRender_BuildMRT(timeDelta);
	//SXRenderFunc::Delay::RenderMRT += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	if (GData::FinalImage == DS_RT_AMBIENTDIFF || GData::FinalImage == DS_RT_SPECULAR || GData::FinalImage == DS_RT_SCENELIGHT)
	{
		//освещаем сцену
		ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
		SRender_ComLighting(timeDelta);
		SRender_UnionLayers();
		if (SGCore_SkyBoxIsCr())
			SRender_RenderSky(timeDelta);
		SRender_ApplyToneMapping();
		SRender_ComToneMapping(timeDelta);

		//SXRenderFunc::Delay::ComLighting += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;
	}

	pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

#if defined(SX_GAME)
	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SRender_RenderPostProcess(timeDelta);
	//SXRenderFunc::Delay::PostProcess += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;
#endif

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::ScreenOut);

	pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

#if !defined(SX_GAME)
	pDXDevice->SetTexture(0, SML_DSGetRT(GData::FinalImage));
#else
	pDXDevice->SetTexture(0, SGCore_RTGetTexture(SPP_RTGetCurrSend()));
#endif

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	SRender_RenderParticles(timeDelta);

	pDXDevice->SetTransform(D3DTS_WORLD, &((D3DXMATRIX)SMMatrixIdentity()));
	pDXDevice->SetTransform(D3DTS_VIEW, &((D3DXMATRIX)mView));
	pDXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)mProjLight));
	/*SXRenderFunc::RenderEditorMain();
	SXRenderFunc::RenderEditorLE(timeDelta);
	SXRenderFunc::RenderEditorPE(timeDelta);*/

#ifdef _DEBUG
	SAIG_RenderQuads(SRender_GetCamera()->ObjFrustum, &vCamPos, GData::NearFar.y);
#endif

#if defined(SX_GAME)
	SXGame_RenderHUD();
#endif

	SRender_OutputDebugInfo(timeDelta);

	SXPhysics_DebugRender();

	SRender_ShaderRegisterData();

	pDXDevice->EndScene();

	//@@@
	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SRender_ComVisibleForCamera();
	//SXRenderFunc::Delay::UpdateVisibleForCamera += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SRender_ComVisibleReflection();
	//SXRenderFunc::Delay::UpdateVisibleForReflection += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SRender_ComVisibleForLight();
	//SXRenderFunc::Delay::UpdateVisibleForLight += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	SPE_EffectVisibleComAll(SRender_GetCamera()->ObjFrustum, &vCamPos);
	SPE_EffectComputeAll();
	SPE_EffectComputeLightingAll();
	//SXRenderFunc::Delay::UpdateParticles += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	ttime = TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER));
	pDXDevice->Present(0, 0, 0, 0);
	//SXRenderFunc::Delay::Present += TimeGetMcsU(Core_RIntGet(G_RI_INT_TIMER_RENDER)) - ttime;

	SkyXEngind_UpdateDataCVar();

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

void SkyXEngind_UpdateDataCVar()
{
	ID GlobalLight = SML_LigthsGetGlobal();
	static const bool * pssm_4or3 = GET_PCVAR_BOOL("pssm_4or3");
	static bool pssm_4or3_old = true;

	//проверяем не изменилось ли значение квара, если изменилось то меняем и количество сплитов
	if (pssm_4or3 && pssm_4or3_old != (*pssm_4or3) && GlobalLight >= 0)
	{
		pssm_4or3_old = (*pssm_4or3);
		SML_LigthsSet4Or3SplitsG(GlobalLight, pssm_4or3_old);
	}

	static const bool * pssm_shadowed = GET_PCVAR_BOOL("pssm_shadowed");
	static bool pssm_shadowed_old = true;

	if (pssm_shadowed && pssm_shadowed_old != (*pssm_shadowed) && GlobalLight >= 0)
	{
		pssm_shadowed_old = (*pssm_shadowed);
		SML_LigthsSetTypeShadowed(GlobalLight, (pssm_shadowed_old ? LTYPE_SHADOW_DYNAMIC : LTYPE_SHADOW_NONE));
	}

	static const float * pssm_q = GET_PCVAR_FLOAT("pssm_q");
	static float pssm_q_old = 1;

	if (pssm_q && pssm_q_old != (*pssm_q))
	{
		pssm_q_old = (*pssm_q);
		if (pssm_q_old < 0.5f)
		{
			pssm_q_old = 0.5f;
			Core_0SetCVarFloat("pssm_q", pssm_q_old);
		}
		else if (pssm_q_old > 4.f)
		{
			pssm_q_old = 4.f;
			Core_0SetCVarFloat("pssm_q", pssm_q_old);
		}
		SML_LigthsSettGCoefSizeDepth(pssm_q_old);
	}

	static const float * lsm_q = GET_PCVAR_FLOAT("lsm_q");
	static float lsm_q_old = 1;

	if (lsm_q && lsm_q_old != (*lsm_q))
	{
		lsm_q_old = (*lsm_q);
		if (lsm_q_old < 0.5f)
		{
			lsm_q_old = 0.5f;
			Core_0SetCVarFloat("lsm_q", lsm_q_old);
		}
		else if (lsm_q_old > 4.f)
		{
			lsm_q_old = 4.f;
			Core_0SetCVarFloat("lsm_q", lsm_q_old);
		}
		SML_LigthsSettLCoefSizeDepth(lsm_q_old);
	}

	static const int * grass_frec = GET_PCVAR_INT("grass_frec");
	static int grass_frec_old = 1;

	if (grass_frec && grass_frec_old != (*grass_frec))
	{
		grass_frec_old = (*grass_frec);
		if (grass_frec_old <= 0)
		{
			grass_frec_old = 1;
			Core_0SetCVarInt("grass_frec", grass_frec_old);
		}
		else if (grass_frec_old > 100)
		{
			grass_frec_old = 100;
			Core_0SetCVarInt("grass_frec", grass_frec_old);
		}
		SGeom_0SettGreenSetFreqGrass(grass_frec_old);
	}

	static const float * green_lod0 = GET_PCVAR_FLOAT("green_lod0");
	static float green_lod0_old = 50;

	if (green_lod0 && green_lod0_old != (*green_lod0))
	{
		green_lod0_old = (*green_lod0);
		if (green_lod0_old <= 20)
		{
			green_lod0_old = 20;
			Core_0SetCVarFloat("green_lod0", green_lod0_old);
		}
		else if (green_lod0_old > 100)
		{
			green_lod0_old = 100;
			Core_0SetCVarFloat("green_lod0", green_lod0_old);
		}
		SGeom_0SettGreenSetDistLods1(green_lod0_old);
	}

	static const float * green_lod1 = GET_PCVAR_FLOAT("green_lod1");
	static float green_lod1_old = 50;

	if (green_lod1 && green_lod1_old != (*green_lod1))
	{
		green_lod1_old = (*green_lod1);
		if (green_lod1_old <= 50)
		{
			green_lod1_old = 50;
			Core_0SetCVarFloat("green_lod1", green_lod1_old);
		}
		else if (green_lod1_old > 150)
		{
			green_lod1_old = 150;
			Core_0SetCVarFloat("green_lod1", green_lod1_old);
		}
		SGeom_0SettGreenSetDistLods2(green_lod1_old);
	}

	static const float * green_less = GET_PCVAR_FLOAT("green_less");
	static float green_less_old = 10;

	if (green_less && green_less_old != (*green_less))
	{
		green_less_old = (*green_less);
		if (green_less_old <= 10)
		{
			green_less_old = 10;
			Core_0SetCVarFloat("green_less", green_less_old);
		}
		else if (green_less_old > 90)
		{
			green_less_old = 90;
			Core_0SetCVarFloat("green_less", green_less_old);
		}
		SGeom_0SettGreenSetBeginEndLessening(green_less_old);
	}

	static const float * p_far = GET_PCVAR_FLOAT("p_far");
	static float p_far_old = 400;

	if (p_far && p_far_old != (*p_far))
	{
		p_far_old = (*p_far);

		if (p_far_old < 100.f)
		{
			p_far_old = 100.f;
			Core_0SetCVarFloat("p_far", p_far_old);
		}
		else if (p_far_old > 1000)
		{
			p_far_old = 1000.f;
			Core_0SetCVarFloat("p_far", p_far_old);
		}

		GData::NearFar.y = p_far_old;
		GData::InitAllMatrix();
	}
}

//#############################################################################

int SkyXEngine_CycleMain()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT && IsWindow((HWND)SRender_GetHandleWin3D()))
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
			//GData::ObjCamera = SXGame_GetActiveCamera();
			SRender_SetCamera(SXGame_GetActiveCamera());
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


			if (Core_TimeWorkingGet(Core_RIntGet(G_RI_INT_TIMER_RENDER)) && (GetForegroundWindow() == (HWND)SRender_GetHandleWin3D() || GetForegroundWindow() == (HWND)SRender_GetParentHandleWin3D() || GetForegroundWindow() == FindWindow(NULL, "sxconsole")))
			{

#if defined(SX_LEVEL_EDITOR)
				SXLevelEditor_Transform(10);
#endif

#if defined(SX_MATERIAL_EDITOR)
				if (SXMaterialEditor::CheckBoxModelRot->GetCheck())
					GData::Editors::SimModel->Rotation.y -= float(timeDelta) * 0.001f * 0.25;
#endif

				SkyXEngine_Frame(timeDelta);
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