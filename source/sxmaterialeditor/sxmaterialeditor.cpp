
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
	SXMaterialEditor::JobWindow->AddHandler(ComMenuId, WM_COMMAND);
	SXMaterialEditor::JobWindow->AddHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXMaterialEditor::JobWindow->AddHandler(MsgEditSize, WM_SIZE);
	SXMaterialEditor::MainMenu = SXGUICrMenuEx(IDR_MENU1);
	SXMaterialEditor::MainMenu->SetToWindow(SXMaterialEditor::JobWindow->GetHWND());
	SXMaterialEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
	GData::FinalImage = DS_RT::ds_rt_scene_light_com;

	SXMaterialEditor::WindowRender->GAlign.left = false;
	SXMaterialEditor::WindowRender->GAlign.right = false;
	SXMaterialEditor::WindowRender->GAlign.top = false;
	SXMaterialEditor::WindowRender->GAlign.bottom = false;

	SXMaterialEditor::ToolBar->GAlign.left = false;
	SXMaterialEditor::ToolBar->GAlign.right = false;
	SXMaterialEditor::ToolBar->GAlign.top = false;
	SXMaterialEditor::ToolBar->GAlign.bottom = false;

	GData::Handle3D = SXMaterialEditor::WindowRender->GetHWND();
	GData::HandleParent3D = SXMaterialEditor::JobWindow->GetHWND();

	RECT winrndrect;
	SXMaterialEditor::WindowRender->GetClientRect(&winrndrect);

	GData::WinSize.x = winrndrect.right;
	GData::WinSize.y = winrndrect.bottom;

	
	SkyXEngine_Init();

	SSInput_0Create("sxinput", SXMaterialEditor::JobWindow->GetHWND(), true);
	SSInput_Dbg_Set(printflog);

	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SXMaterialEditor::EditSkyBox->SetText("sky_2_cube.dds");
		
	GData::DXDevice = SGCore_GetDXDevice();
	
	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	GData::ObjCamera = SGCore_CrCamera();
	GData::ObjCamera->SetPosition(&float3(0, 0, -1.2*100));

	GData::SimModel = new ModelSim();

	GData::SimModel->Add("sphere.dse");
	GData::SimModel->Add("cube.dse");
	GData::SimModel->Add("plane.dse");

	SML_LigthsCreatePoint(
		&float3(10,300,0),
		LIGHTS_GLOBAL_MAX_POWER,
		LIGHTS_GLOBAL_STD_RADIUS,
		&float3(1,1,1),
		true,
		false);
	SML_LigthsSetEnable(SML_LigthsGetCount() - 1, true);
	SML_LigthsSetName(SML_LigthsGetCount() - 1, "sun");

	char tmppathexe[1024];
	char tmppath[1024];
	GetModuleFileName(NULL, tmppath, 1024);
	int len = strlen(tmppath);
	while (tmppath[len--] != '\\')
	{
		if (tmppath[len - 1] == '\\')
		{
			len--;
			memcpy(tmppathexe, tmppath, len);
			tmppathexe[len] = 0;
		}
	}

	SGCore_LoadTexStdPath(GData::Pathes::Textures);
	SGCore_LoadTexLoadTextures();



	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));
	
	static DWORD lastTime = GetTickCount();
	static DWORD TimeCCadr = 0;

	SXMaterialEditor::InitMtl(GData::SimModel->GetIDMtl());
	SXMaterialEditor::IDMat = GData::SimModel->GetIDMtl();

		while (msg.message != WM_QUIT && IsWindow(GData::Handle3D))
		{
				if(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
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
					//SXLevelEditor_Transform(10);
					if (SXMaterialEditor::CheckBoxModelRot->GetCheck())
						GData::SimModel->Rotation.y -= float(timeDelta) * 0.001f * 0.25;
					SXRenderFunc::MainRender(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

	mem_delete(GData::SimModel);
	SXMaterialEditor::DeleteAllElements();

	SkyXEngine_Kill();

    return msg.wParam;
}