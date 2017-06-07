
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SX_LEVEL_EDITOR
#define SX_EXE

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 
#include <skyxengine.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SXGUIRegClass::RegGroupBox();
	InitOutLog();
	srand((unsigned int)time(0));

	SXLevelEditor::InitAllElements();

	GData::Handle3D = SXLevelEditor::RenderWindow->GetHWND();
	GData::HandleParent3D = SXLevelEditor::JobWindow->GetHWND();
	RECT winrndrect;
	SXLevelEditor::RenderWindow->GetClientRect(&winrndrect);

	GData::WinSize.x = winrndrect.right;
	GData::WinSize.y = winrndrect.bottom;

	SkyXEngine_Init();

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
					Core_0ConsoleUpdate();
					SXLevelEditor_Transform(10);
					SXRenderFunc::MainRender(timeDelta);

					if (SSInput_GetKeyEvents(SIM_LBUTTON) == InputEvents::iv_k_first)
					{
						/*float3 start = GData::ConstCurrCamPos;
						float3 end = start + (GData::ConstCurrCamDir * 1000);*/
						float3 _res;
						ID idmodel;
						ID idmtl;

						
						float3 camDir;
						float det;
						SMMATRIX mat = SMMatrixInverse(&det, GData::MCamView);
						POINT pt;
						GetCursorPos(&pt);
						ScreenToClient(GData::Handle3D, &pt);
						
						float3 pos = float3(
							(2.0f * (float)pt.x / GData::WinSize.x - 1.0f) / GData::MCamProj._11,
							-(2.0f * (float)pt.y / GData::WinSize.y - 1.0f) / GData::MCamProj._22,
							1.0f
							) * mat;
						camDir = pos - GData::ConstCurrCamPos;

						ID idgreen;
						ID idsplit;
						ID idobj;

						/*if (pt.x <= GData::WinSize.x && pt.y <= GData::WinSize.y && SGeom_GreenTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idgreen, &idsplit, &idobj, &idmtl))
						{
							GData::Editors::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;
							GData::Editors::ActiveElement = idgreen;
							GData::Editors::ActiveGreenSplit = idsplit;
							GData::Editors::ActiveGreenObject = idobj;
							SGeom_GreenDelObject(idgreen, idsplit, idobj);
							int qwert = 0;
						}*/

						if (pt.x <= GData::WinSize.x && pt.y <= GData::WinSize.y && SGeom_ModelsTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idmodel, &idmtl))
						{
							/*MCInitElemsSelModel(idmodel);
							GData::Editors::ActiveElement = idmodel;
							GData::Editors::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;*/
							SGeom_GreenAddObject(0, &_res);
						}
					}

					TimeCCadr = timeDelta;
					TimeStart = TimeThis;

					lastTime = currTime;
				}
		}

	mem_release(GData::ObjCamera);
	SGeom_AKill();
	SML_AKill();
	SGCore_AKill();
	SXLevelEditor::DeleteAllElements();

    return msg.wParam;
}