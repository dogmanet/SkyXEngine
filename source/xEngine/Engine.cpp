#include "Engine.h"
#include <core/task.h>
#include <GRegisterIndex.h>

#include <core/taskmanager.h>

#include "SkyXEngine.h"

#ifdef _DEBUG
#	pragma comment(lib, "sxcore_d.lib")
#else
#	pragma comment(lib, "sxcore.lib")
#endif

class CMainLoopTask: public ITaskImpl<ITask>
{
public:
	CMainLoopTask(CEngine *pEngine):
		m_pEngine(pEngine),
		ITaskImpl(CORE_TASK_FLAG_MAINTHREAD_REPEATING)
	{
	}

	void run() override
	{
		if(!m_pEngine->runFrame())
		{
			XCoreStop();
		}
	}

protected:
	CEngine *m_pEngine;
};

//##########################################################################

CEngine::CEngine(int argc, char **argv, const char *szName)
{
	srand((UINT)time(0));

	//SkyXEngine_InitOutLog();
	initPaths();

	{
		char szPath[MAX_PATH];
		GetModuleFileNameA(NULL, szPath, MAX_PATH);
		canonize_path(szPath);
		dirname(szPath);
		dirname(szPath);
		strcat(szPath, "gamesource/");
		SetCurrentDirectoryA(szPath);
	}


	m_pCore = XCoreInit(szName);
	INIT_OUTPUT_STREAM(m_pCore);
	LibReport(REPORT_MSG_LEVEL_NOTICE, "LIB core initialized\n");
	
	Core_0RegisterCVarString("engine_version", SKYXENGINE_VERSION, "Текущая версия движка", FCVAR_READONLY);


	// init mtrl
	SMtrl_0Create("sxml", false, true);
	LibReport(REPORT_MSG_LEVEL_NOTICE, "LIB mtrl initialized\n");

	// init physics
	SPhysics_0Create();
	LibReport(REPORT_MSG_LEVEL_NOTICE, "LIB physics initialized\n");
}
CEngine::~CEngine()
{
	SRender_AKill();
	SGame_AKill();
	SPhysics_AKill();
	SMtrl_AKill();
	SGCore_AKill();
	//SSInput_AKill();
	mem_delete(m_pCore);
}

bool XMETHODCALLTYPE CEngine::initGraphics(XWINDOW_OS_HANDLE hWindow, IXEngineCallback *pCallback)
{
	m_pCallback = pCallback;

	// init input
	SSInput_0Create("sxinput", (HWND)hWindow, false);
	LibReport(REPORT_MSG_LEVEL_NOTICE, "LIB input initialized\n");

	// init graphics
	Core_0RegisterCVarInt("r_win_width", 800, "Размер окна по горизонтали (в пикселях)");
	Core_0RegisterCVarInt("r_win_height", 600, "Размер окна по вертикали (в пикселях)");
	Core_0RegisterCVarBool("r_win_windowed", true, "Режим рендера true - оконный, false - полноэкранный");
	Core_0RegisterCVarFloat("r_default_fov", SM_PI * 0.25f, "Дефолтный fov в радианах");
	Core_0RegisterCVarFloat("r_near", 0.025f, "Ближняя плоскость отсчечения");
	Core_0RegisterCVarFloat("r_far", 800, "Дальняя плоскость отсечения (дальность видимости)");

	Core_0RegisterCVarInt("r_final_image", DS_RT_SCENELIGHT, "Тип финального (выводимого в окно рендера) изображения из перечисления DS_RT");

	Core_0RegisterCVarInt("r_resize", RENDER_RESIZE_NONE, "Тип изменения размеров окан рендера из перечисления RENDER_RESIZE");


	static int *r_win_width = (int*)GET_PCVAR_INT("r_win_width");
	static int *r_win_height = (int*)GET_PCVAR_INT("r_win_height");
	static const bool *r_win_windowed = GET_PCVAR_BOOL("r_win_windowed");

	SGCore_0Create("sxgcore", (HWND)hWindow, *r_win_width, *r_win_height, *r_win_windowed, false);
	
	SGCore_SetFunc_MtlIsTransparency((g_func_mtl_is_transparency)SMtrl_MtlIsTransparency);
	SGCore_SetFunc_MtlGroupRenderIsSingly((g_func_mtl_group_render_is_singly)SMtrl_MtlGetTypeReflection);
	SGCore_SetFunc_MtlGetPhysicType((g_func_mtl_get_physic_type)SMtrl_MtlGetPhysicMaterial);

	LibReport(REPORT_MSG_LEVEL_NOTICE, "LIB gcore initialized\n");

	SMtrl_DevSet(SGCore_GetDXDevice());

	// init render
	SRender_0Create("sxrender", (HWND)hWindow, NULL, false);
	LibReport(REPORT_MSG_LEVEL_NOTICE, "LIB render initialized\n");


	// init game
	SGame_0Create((HWND)hWindow, true);
	LibReport(REPORT_MSG_LEVEL_NOTICE, "LIB game initialized\n");

	// init updatable
	Core_GetIXCore()->initUpdatable();

	return(true);
}
bool XMETHODCALLTYPE CEngine::initServer()
{

	// init game
	SGame_0Create(NULL, true);
	LibReport(REPORT_MSG_LEVEL_NOTICE, "LIB game initialized\n");

	// init updatable
	Core_GetIXCore()->initUpdatable();

	return(true);
}

INT_PTR XMETHODCALLTYPE CEngine::onMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	IMSG imsg;
	imsg.lParam = lParam;
	imsg.wParam = wParam;
	imsg.message = msg;

	SSInput_AddMsg(imsg);

	if(!SGame_AddWMsg(msg, wParam, lParam))
	{
		return(TRUE);
	}

	return(FALSE);
}
IXCore* XMETHODCALLTYPE CEngine::getCore()
{
	return(m_pCore);
}

int XMETHODCALLTYPE CEngine::start()
{
	CTaskManager::TaskPtr pTask = new CMainLoopTask(this);
	XCoreAddTask(pTask);
	XCoreStart();
	mem_release(pTask);
	return(0);
}

bool CEngine::runFrame()
{
	if(m_pCallback && !m_pCallback->processWindowMessages())
	{
		return(false);
	}

	Core_0ConsoleUpdate();


	// draw frame

	Sleep(10);

	return(true);
}

void CEngine::initPaths()
{
	char szPath[MAX_PATH];
	/*GetModuleFileName(NULL, szPath, MAX_PATH);

	String sPathExe = FileCanonizePathS(FileGetPrevDir(szPath).c_str());
	Core_RStringSet(G_RI_STRING_PATH_EXE, sPathExe.c_str());

	String sPrevPathExe = FileGetPrevDir(sPathExe.c_str());
	*/
	String sPrevPathExe = "../";

	sprintf(szPath, "%s%s", sPrevPathExe.c_str(), "/worktex/");
	Core_RStringSet(G_RI_STRING_PATH_WORKTEX, szPath);
	//FileCreateDir(szPath);

	sprintf(szPath, "%s/%s/", sPrevPathExe.c_str(), "gamesource");
	Core_RStringSet(G_RI_STRING_PATH_GAMESOURCE, szPath);
	SetCurrentDirectoryA(szPath);

	sprintf(szPath, "%s/%s/", sPrevPathExe.c_str(), "editors_cache");
	Core_RStringSet(G_RI_STRING_PATH_EDITOR_CACHE, szPath);

	sprintf(szPath, "%s%s", sPrevPathExe.c_str(), "/screenshots/");
	Core_RStringSet(G_RI_STRING_PATH_SCREENSHOTS, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "config");
	Core_RStringSet(G_RI_STRING_PATH_GS_CONFIGS, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "levels");
	Core_RStringSet(G_RI_STRING_PATH_GS_LEVELS, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "meshes");
	Core_RStringSet(G_RI_STRING_PATH_GS_MESHES, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "models");
	Core_RStringSet(G_RI_STRING_PATH_GS_MODELS, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "shaders");
	Core_RStringSet(G_RI_STRING_PATH_GS_SHADERS, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "sounds");
	Core_RStringSet(G_RI_STRING_PATH_GS_SOUNDS, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "scripts");
	Core_RStringSet(G_RI_STRING_PATH_GS_SCRIPTS, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "textures");
	Core_RStringSet(G_RI_STRING_PATH_GS_TEXTURES, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "materials");
	Core_RStringSet(G_RI_STRING_PATH_GS_MTRLS, szPath);
	// FileCreateDir(szPath);

	sprintf(szPath, "%s%s/", Core_RStringGet(G_RI_STRING_PATH_GAMESOURCE), "gui");
	Core_RStringSet(G_RI_STRING_PATH_GS_GUI, szPath);
	// FileCreateDir(szPath);
}

//##########################################################################

C XAPI IXEngine* XEngineInit(int argc, char **argv, const char *szName)
{
	return(new CEngine(argc, argv, szName));
}
