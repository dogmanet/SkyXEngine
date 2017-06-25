
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SX_MATERIAL_EDITOR
#define SX_EXE

#define _CRT_SECURE_NO_WARNINGS

#include <vld.h> 

#include <SkyXEngine.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SXGUIRegClass::RegGroupBox();
	srand((unsigned int)time(0));

	SXMaterialEditor::InitAllElements();
	SXMaterialEditor::Nulling();
	
	

	GData::Handle3D = SXMaterialEditor::WindowRender->GetHWND();
	GData::HandleParent3D = SXMaterialEditor::JobWindow->GetHWND();

	RECT winrndrect;
	SXMaterialEditor::WindowRender->GetClientRect(&winrndrect);

	GData::WinSize.x = winrndrect.right;
	GData::WinSize.y = winrndrect.bottom;

	SkyXEngine_Init();

	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SXMaterialEditor::EditSkyBox->SetText("sky_2_cube.dds");
		
	GData::ObjCamera->SetPosition(&float3(0, 0, -1.2*100));

	GData::Editors::SimModel = new ModelSim();

	GData::Editors::SimModel->Add("sphere.dse");
	GData::Editors::SimModel->Add("cube.dse");
	GData::Editors::SimModel->Add("plane.dse");

	SML_LigthsCreatePoint(
		&float3(10,300,0),
		LIGHTS_GLOBAL_MAX_POWER,
		LIGHTS_GLOBAL_STD_RADIUS,
		&float3(1,1,1),
		true,
		false);
	SML_LigthsSetEnable(SML_LigthsGetCount() - 1, true);
	SML_LigthsSetName(SML_LigthsGetCount() - 1, "sun");

	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_LoadTexLoadTextures();

	SXMaterialEditor::InitMtl(GData::Editors::SimModel->GetIDMtl());
	SXMaterialEditor::IDMat = GData::Editors::SimModel->GetIDMtl();

	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	return result;
}