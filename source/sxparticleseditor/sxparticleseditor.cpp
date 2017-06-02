
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SX_PARTICLES_EDITOR
#define SX_EXE

#define _CRT_SECURE_NO_WARNINGS

#include <vld.h> 

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

	SSInput_0Create("sxinput", GData::Handle3D, true);
	SSInput_Dbg_Set(printflog);

	
	GData::DXDevice = SGCore_GetDXDevice();

	GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	GData::ObjCamera = SGCore_CrCamera();
	GData::ObjCamera->SetPosition(&float3(0, 0.5, -2));

	GData::ObjGrid = new Grid();
	GData::ObjGrid->Create(100, 100, D3DCOLOR_ARGB(255,200,200,200));

	GData::ObjAxesStatic = new AxesStatic();
	GData::ObjAxesStatic->Create(1);

	D3DXCreateBox(GData::DXDevice, 1, 1, 1, &GData::FigureBox, 0);
	D3DXCreateSphere(GData::DXDevice, 1, 20, 20, &GData::FigureSphere, 0);
	
	GData::FigureConeParam.x = 1;
	GData::FigureConeParam.y = 0.1;
	GData::FigureConeParam.z = 1;

	SGCore_FCreateCone(GData::FigureConeParam.x, GData::FigureConeParam.y, GData::FigureConeParam.z, &GData::FigureCone, 20);

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

	Level::LoadParticles();

	SXParticlesEditor::EffInitList();

	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static DWORD lastTime = GetTickCount();
	static DWORD TimeCCadr = 0;

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
			SXRenderFunc::MainRender(timeDelta);

			TimeCCadr = timeDelta;
			TimeStart = TimeThis;

			lastTime = currTime;
		}
	}

	mem_delete(GData::SimModel);
	SXParticlesEditor::DeleteAllElements();

	SkyXEngine_Kill();

	return msg.wParam;
}