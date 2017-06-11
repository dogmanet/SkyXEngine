
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
	
	GData::FinalImage = DS_RT::ds_rt_scene_light_com;

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



	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	
	static DWORD lastTime = GetTickCount();
	static DWORD TimeCCadr = 0;

	SXMaterialEditor::InitMtl(GData::Editors::SimModel->GetIDMtl());
	SXMaterialEditor::IDMat = GData::Editors::SimModel->GetIDMtl();

	while (msg.message != WM_QUIT && IsWindow(GData::Handle3D))
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);

			IMSG imsg;
			imsg.lParam = msg.lParam;
			imsg.wParam = msg.wParam;
			imsg.message = msg.message;

			SSInput_AddMsg(imsg);

			::DispatchMessage(&msg);
		}
		else
		{
			static DWORD TimeStart = 0;
			DWORD TimeThis = GetTickCount();

			DWORD currTime = GetTickCount();
			DWORD timeDelta = (currTime - lastTime);

			SGCore_LoadTexLoadTextures();
			if (SXMaterialEditor::CheckBoxModelRot->GetCheck())
				GData::Editors::SimModel->Rotation.y -= float(timeDelta) * 0.001f * 0.25;
			SXRenderFunc::MainRender(timeDelta);

			TimeCCadr = timeDelta;
			TimeStart = TimeThis;

			lastTime = currTime;
		}
	}

	mem_delete(GData::Editors::SimModel);
	SXMaterialEditor::DeleteAllElements();

	SkyXEngine_Kill();

    return msg.wParam;
}