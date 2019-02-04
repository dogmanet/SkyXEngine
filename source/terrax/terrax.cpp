
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 
#include <skyxengine.h>
#include <io.h>
#include "terrax.h"

extern HWND g_hWndMain;

//extern IGXSwapChain *g_pTopRightSwapChain;
//extern IGXSwapChain *g_pBottomLeftSwapChain;
//extern IGXSwapChain *g_pBottomRightSwapChain;

ATOM XRegisterClass(HINSTANCE hInstance);
BOOL XInitInstance(HINSTANCE, int);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
//	SkyXEngine_PreviewCreate();
//	SXGUIinit();


	InitCommonControls();
	XRegisterClass(hInstance);

	// Perform application initialization:
	if(!XInitInstance(hInstance, nCmdShow))
	{
		return(1);
	}

	SkyXEngine_Init(g_hTopLeftWnd, g_hWndMain, lpCmdLine);
	
	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	SGCore_SkyBoxSetUse(false);
	SGCore_SkyCloudsSetUse(false);

	g_pTopRightCamera = SGCore_CrCamera();
	g_pTopRightCamera->setPosition(&X2D_TOP_POS);
	g_pTopRightCamera->setOrientation(&X2D_TOP_ROT);
	g_pBottomLeftCamera = SGCore_CrCamera();
	g_pBottomLeftCamera->setPosition(&X2D_SIDE_POS);
	g_pBottomLeftCamera->setOrientation(&X2D_SIDE_ROT);
	g_pBottomRightCamera = SGCore_CrCamera();
	g_pBottomRightCamera->setPosition(&X2D_FRONT_POS);
	g_pBottomRightCamera->setOrientation(&X2D_FRONT_ROT);

//	SkyXEngine_RunGenPreview();
	Core_0SetCVarInt("r_final_image", DS_RT_COLOR);

	SRender_EditorSetRenderGrid(true);
	SRender_EditorSetRenderAxesStatic(true);
	
	int countgc = SGame_EntGetClassListCount();
	const char** listgc = new const char*[countgc];

	SGame_EntGetClassList(listgc, countgc);
	for(int i = 0; i < countgc; ++i)
	{
	//	level_editor::pComboBoxGameClass->addItem(listgc[i]);
	}
	mem_delete_a(listgc);


//	SkyXEngine_PreviewKill();



	int result = SkyXEngine_CycleMain();
	
	SkyXEngine_Kill();
	return result;
}

