
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <SkyXEngine.h>
//#include <common/string_func.h>

#if 0
#include <xWindow/IXWindowSystem.h>

#ifdef _DEBUG
#	pragma comment(lib, "xWindow_d.lib")
#else
#	pragma comment(lib, "xWindow.lib")
#endif

class CWindowCallback: public IXWindowCallback
{
public:
	INT_PTR XMETHODCALLTYPE onMessage(UINT msg, WPARAM wParam, LPARAM lParam, IXWindow *pWindow)
	{
		return(pWindow->runDefaultCallback(msg, wParam, lParam));
	}
};
#endif

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
#if 0
	IXWindowSystem *pWindowSystem = XWindowInit();

	XWINDOW_DESC wdesc;
	wdesc.iPosX = XCW_USEDEFAULT;
	wdesc.iPosY = XCW_USEDEFAULT;
	wdesc.iSizeX = 800;
	wdesc.iSizeY = 600;
	wdesc.szTitle = "xWindow";
	wdesc.flags = XWF_BUTTON_CLOSE | XWF_BUTTON_MINIMIZE | XWF_NORESIZE;

	CWindowCallback cb;
	IXWindow *pWindow = pWindowSystem->createWindow(&wdesc, &cb);

	while(pWindowSystem->processMessages())
	{
		Sleep(10);
	}
	mem_release(pWindow);
	mem_release(pWindowSystem);
	return(0);
#endif

	//MessageBox(0, 0, 0, 0);
	SkyXEngine_PreviewCreate();
	SkyXEngine_Init(0, 0, lpCmdLine);
	SkyXEngine_PreviewKill();

	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	SGCore_SkyBoxSetUse(false);
	SGCore_SkyCloudsSetUse(false);
	
	//SGCore_OC_SetEnable(false);

	SGreen_0SettSetFreqGrass(100);

	SGCore_ShaderAllLoad();
	SGCore_LoadTexAllLoad();

	SetWindowPos((HWND)SGCore_GetHWND(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	SLevel_AmbientSndPlay();
	SLevel_WeatherSndPlay();

	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	return result;
}