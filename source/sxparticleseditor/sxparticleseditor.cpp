
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SX_PARTICLES_EDITOR
#define SX_EXE

#define _CRT_SECURE_NO_WARNINGS

#include <SkyXEngine.cpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SkyXEngine_PreviewCreate();
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

	SkyXEngine_PreviewKill();
	SXParticlesEditor::JobWindow->Visible(true);
	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	SXParticlesEditor::DeleteAllElements();
	return result;
}