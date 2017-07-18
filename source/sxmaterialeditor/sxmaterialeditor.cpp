
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SX_MATERIAL_EDITOR
#define SX_EXE

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 

#include <SkyXEngine.cpp>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SkyXEngine_PreviewCreate();
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

	char shaderskitpath[1024];
	sprintf(shaderskitpath, "%s%s", GData::Pathes::ForExe, "\\shaders_kit.cfg");
	SXMaterialEditor::Shaders = new ShadersKit();
	SXMaterialEditor::Shaders->Load(shaderskitpath);

	for (int i = 0; i < SXMaterialEditor::Shaders->GetCount(); ++i)
	{
		SXMaterialEditor::ComboBoxShaders->AddItem(SXMaterialEditor::Shaders->GetName(i));
	}

	SXMaterialEditor::ComboBoxShaders->SetSel(0);


	char paramlpath[1024];
	sprintf(paramlpath, "%s%s", GData::Pathes::ForExe, "\\paraml_kit.cfg");
	SXMaterialEditor::ParamL = new ParamLKit();
	SXMaterialEditor::ParamL->Load(paramlpath);

	for (int i = 0; i < SXMaterialEditor::ParamL->GetCount(); ++i)
	{
		SXMaterialEditor::ComboBoxParamL->AddItem(SXMaterialEditor::ParamL->GetName(i));
	}

	SXMaterialEditor::ComboBoxParamL->SetSel(0);


	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SXMaterialEditor::EditSkyBox->SetText("sky_2_cube.dds");
		
	GData::ObjCamera->SetPosition(&float3(0, 0, -1.2*100));

	GData::Editors::SimModel = new ModelSim();

	GData::Editors::SimModel->Add("sphere.dse");
	GData::Editors::SimModel->Add("cube.dse");
	GData::Editors::SimModel->Add("plane.dse");

	SML_LigthsCreatePoint(
		&float3(110,110,-110),
		LIGHTS_GLOBAL_MAX_POWER,
		LIGHTS_GLOBAL_STD_RADIUS,
		&float3(1,1,1),
		true,
		true);
	SML_LigthsSetEnable(SML_LigthsGetCount() - 1, true);
	SML_LigthsSetName(SML_LigthsGetCount() - 1, "sun");

	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_LoadTexLoadTextures();

	SXMaterialEditor::InitMtl(GData::Editors::SimModel->GetIDMtl());
	SXMaterialEditor::IDMat = GData::Editors::SimModel->GetIDMtl();

	SkyXEngine_PreviewKill();
	SXMaterialEditor::JobWindow->Visible(true);
	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	SXMaterialEditor::DeleteAllElements();
	return result;
}