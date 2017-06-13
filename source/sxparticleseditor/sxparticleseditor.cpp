
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SX_PARTICLES_EDITOR
#define SX_EXE

#define _CRT_SECURE_NO_WARNINGS

#include <SkyXEngine.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SXGUIRegClass::RegGroupBox();
	SXGUIRegClass::RegButtonImg();
	srand((unsigned int)time(0));

	SXParticlesEditor::InitAllElements();

	SXParticlesEditor::TabsVisible(false);
	SXParticlesEditor::AllInTabsVisible(false);
	
	GData::FinalImage = DS_RT::ds_rt_scene_light_com;

	GData::Handle3D = SXParticlesEditor::WindowRender->GetHWND();
	GData::HandleParent3D = SXParticlesEditor::JobWindow->GetHWND();

	RECT winrndrect;
	SXParticlesEditor::WindowRender->GetClientRect(&winrndrect);

	GData::WinSize.x = winrndrect.right;
	GData::WinSize.y = winrndrect.bottom;

	SkyXEngine_Init();

	SGCore_LoadTexLoadTextures();

	SXParticlesEditor::EffInitList();

	int result = SkyXEngine_CycleMain();

	SXParticlesEditor::DeleteAllElements();
	SkyXEngine_Kill();

	return result;
}