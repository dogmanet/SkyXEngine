
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SX_LEVEL_EDITOR
#define SX_EXE

#define _CRT_SECURE_NO_WARNINGS

#include <vld.h> 
#include <skyxengine.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SXGUIRegClass::RegGroupBox();
	InitOutLog();
	srand((unsigned int)time(0));

	SXLevelEditor::InitAllElements();

	SXLevelEditor::JobWindow->AddHandler(ComMenuId, WM_COMMAND);
	SXLevelEditor::JobWindow->AddHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXLevelEditor::JobWindow->AddHandler(MsgEditSize, WM_SIZE);
	SXLevelEditor::JobWindow->MinSizeX = 820;
	SXLevelEditor::JobWindow->MinSizeY = 620;
	SXLevelEditor::MainMenu = SXGUICrMenuEx(IDR_MENU1);
	SXLevelEditor::MainMenu->SetToWindow(SXLevelEditor::JobWindow->GetHWND());
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
	GData::FinalImage = DS_RT::ds_rt_scene_light_com;

	GData::Editors::SelSelection = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_SELECTION, true);
	GData::Editors::SelBackFacesCull = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_BACKFACESCULL, true);
	GData::Editors::SelZTest = false;
	GData::Editors::SelMesh = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_MESH, true);
	

	SXLevelEditor::RenderWindow->GAlign.left = true;
	SXLevelEditor::RenderWindow->GAlign.right = true;
	SXLevelEditor::RenderWindow->GAlign.top = true;
	SXLevelEditor::RenderWindow->GAlign.bottom = true;

	SXLevelEditor::GroupBoxList->GAlign.left = false;
	SXLevelEditor::GroupBoxList->GAlign.right = true;
	SXLevelEditor::GroupBoxList->GAlign.top = true;
	SXLevelEditor::GroupBoxList->GAlign.bottom = true;

	SXLevelEditor::GroupBoxData->GAlign.left = true;
	SXLevelEditor::GroupBoxData->GAlign.right = true;
	SXLevelEditor::GroupBoxData->GAlign.top = false;
	SXLevelEditor::GroupBoxData->GAlign.bottom = true;


	SXLevelEditor::ToolBar1->GAlign.left = true;
	SXLevelEditor::ToolBar1->GAlign.right = true;
	SXLevelEditor::ToolBar1->GAlign.top = true;
	SXLevelEditor::ToolBar1->GAlign.bottom = false;

	GData::Handle3D = SXLevelEditor::RenderWindow->GetHWND();
	GData::HandleParent3D = SXLevelEditor::JobWindow->GetHWND();
	RECT winrndrect;
	SXLevelEditor::RenderWindow->GetClientRect(&winrndrect);

	GData::WinSize.x = winrndrect.right;
	GData::WinSize.y = winrndrect.bottom;

	SkyXEngine_Init();

	SSInput_0Create("sxinput", SXLevelEditor::JobWindow->GetHWND(), true);
	SSInput_Dbg_Set(printflog);

	SGCore_SkyBoxLoadTex("sky_2_cube.dds");
	SGCore_SkyCloudsLoadTex("sky_oblaka.dds");
	SGCore_SkyCloudsSetWidthHeightPos(2000, 2000, &float3(0, 0, 0));

	IDirect3DTexture9* SelectMaterial;
	GData::DXDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &SelectMaterial, NULL);
	D3DLOCKED_RECT LockedRect;
	uint32_t tmpColor = D3DCOLOR_ARGB(255, 255, 0, 255);

	SelectMaterial->LockRect(0, &LockedRect, 0, 0);

	uint32_t* tmpOldColor = (uint32_t*)LockedRect.pBits + 0 * LockedRect.Pitch + 0 * sizeof(uint32_t);
	memcpy(tmpOldColor, &tmpColor, sizeof(uint32_t));

	SelectMaterial->UnlockRect(0);

	SGCore_LoadTexLoadTextures();
	GData::IDSelectTex = SGCore_LoadTexCreate("select_material__", SelectMaterial);

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
					SXLevelEditor_Transform(10);
					SXRenderFunc::MainRender(timeDelta);

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

	mem_release(GData::ObjCamera);
	SGeom_0CreateKill();
	SML_0Kill();
	SGCore_0Kill();
	SXLevelEditor::DeleteAllElements();

    return msg.wParam;
}