
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <SkyXEngine.h>
//#include <common/string_func.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
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

	SetWindowPos(SGCore_GetHWND(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	SLevel_AmbientSndPlay();
	SLevel_WeatherSndPlay();

	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	return result;
}