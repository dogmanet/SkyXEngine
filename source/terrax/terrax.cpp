
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 
#include <skyxengine.h>
#include <io.h>
#include "terrax.h"
#include "Grid.h"

extern HWND g_hWndMain;
CGrid *g_pGrid = NULL;

//extern IGXSwapChain *g_pTopRightSwapChain;
//extern IGXSwapChain *g_pBottomLeftSwapChain;
//extern IGXSwapChain *g_pBottomRightSwapChain;

ATOM XRegisterClass(HINSTANCE hInstance);
BOOL XInitInstance(HINSTANCE, int);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
//	SkyXEngine_PreviewCreate();
//	SXGUIinit();


	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_BAR_CLASSES | ICC_TAB_CLASSES;
	InitCommonControlsEx(&icex);

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

	g_xConfig.m_pTopRightCamera = SGCore_CrCamera();
	g_xConfig.m_pTopRightCamera->setPosition(&X2D_TOP_POS);
	g_xConfig.m_pTopRightCamera->setOrientation(&X2D_TOP_ROT);
	g_xConfig.m_pBottomLeftCamera = SGCore_CrCamera();
	g_xConfig.m_pBottomLeftCamera->setPosition(&X2D_SIDE_POS);
	g_xConfig.m_pBottomLeftCamera->setOrientation(&X2D_SIDE_ROT);
	g_xConfig.m_pBottomRightCamera = SGCore_CrCamera();
	g_xConfig.m_pBottomRightCamera->setPosition(&X2D_FRONT_POS);
	g_xConfig.m_pBottomRightCamera->setOrientation(&X2D_FRONT_ROT);

//	SkyXEngine_RunGenPreview();
	Core_0SetCVarInt("r_final_image", DS_RT_COLOR);

	SRender_EditorSetRenderGrid(true);
	SRender_EditorSetRenderAxesStatic(true);

	g_pGrid = new CGrid();
	g_pGrid->setOpacity(0.7f);
	
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
	mem_delete(g_pGrid);
	SkyXEngine_Kill();
	return result;
}

void XRender2D(X_2D_VIEW view, float fScale)
{
	if(g_xConfig.m_bShowGrid)
	{
		GRID_STEP step = g_xConfig.m_gridStep;
		GRID_STEP minStep = CGrid::GetMinStepForScale(fScale);
		if(step < minStep)
		{
			step = minStep;
		}
		switch(view)
		{
		case X2D_FRONT:
			Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixRotationX(SM_PIDIV2));
			break;
		case X2D_SIDE:
			Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixRotationZ(SM_PIDIV2));
			break;
		}
		g_pGrid->setOpacity(g_xConfig.m_fGridOpacity);
		g_pGrid->setDottedMode(g_xConfig.m_bDottedGrid);
		g_pGrid->render(step);

		Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());
	}
}

