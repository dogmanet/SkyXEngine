
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define _CRT_SECURE_NO_WARNINGS

//#include <vld.h> 
#include <skyxengine.h>
#include "level_editor.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	SkyXEngine_PreviewCreate();
	SXGUIRegClass::RegGroupBox();
	SXLevelEditor::InitAllElements();

	SkyXEngine_Init(SXLevelEditor::RenderWindow->GetHWND(), SXLevelEditor::JobWindow->GetHWND());
	SRender_GetCamera()->SetPosition(&float3(0, 0.5, -2));
	SXLevelEditor::MainMenu->InsertPopupItem(4, "Weather", 4, 0);
	SXLevelEditor::MainMenu->InsertItem(4, "none", SX_LE_MMENU_WEATHER_BEGIN_ID, 4);
	SXLevelEditor::MainMenu->CheckItem(SX_LE_MMENU_WEATHER_BEGIN_ID, true);

	SXLevelEditor::CheckBoxTBAIGBound->SetCheck(false);
	SXLevelEditor::AIGBound = false;
	SXLevelEditor::CheckBoxTBAIGQuad->SetCheck(true);
	SXLevelEditor::AIGQuad = true;
	SXLevelEditor::CheckBoxTBAIGGraphPoint->SetCheck(true);
	SXLevelEditor::AIGGraphPoint = true;

	SXLevelEditor::CheckBoxTBGrid->SetCheck(true);
	SXLevelEditor::CheckBoxTBAxes->SetCheck(true);
	SXLevelEditor::MainMenu->CheckItem(ID_VIEW_GRID, true);
	SXLevelEditor::MainMenu->CheckItem(ID_VIEW_AXES, true);

	SXLevelEditor::CheckBoxTBRColor->SetCheck(true);
	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_COLOR, true);
	Core_0SetCVarInt("final_image", DS_RT_COLOR);

	SXLevelEditor::CheckBoxTBSelS->SetCheck(true);
	SXLevelEditor::SelSelection = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_SELECTION, true);

	SXLevelEditor::CheckBoxTBSelCullBack->SetCheck(true);
	SXLevelEditor::SelBackFacesCull = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_BACKFACESCULL, true);
	SXLevelEditor::SelZTest = false;

	SXLevelEditor::CheckBoxTBSelMesh->SetCheck(true);
	SXLevelEditor::SelMesh = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_MESH, true);

	SRender_EditorSetRenderGrid(true);
	SRender_EditorSetRenderAxesStatic(true);


	SXLevelEditor::LEcreateData();


	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	SXLevelEditor::MenuWeatherCount = 1;
	char tpath[1024];
	sprintf(tpath, "%sweather\\*.cfg", Core_RStringGet(G_RI_STRING_PATH_GS_CONFIGS));
	hf = FindFirstFile(tpath, &FindFileData);
	if (hf != INVALID_HANDLE_VALUE){
		do{
			SXLevelEditor::MainMenu->InsertItem(4, FindFileData.cFileName, SX_LE_MMENU_WEATHER_BEGIN_ID + SXLevelEditor::MenuWeatherCount, 4);
			++SXLevelEditor::MenuWeatherCount;
			SXLevelEditor::MenuWeatherArr.push_back(FindFileData.cFileName);
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}

	

	int countgc = SXGame_EntGetClassListCount();
	const char** listgc = new const char*[countgc];

	SXGame_EntGetClassList(listgc, countgc);
	for (int i = 0; i < countgc; ++i)
	{
		SXLevelEditor::ComboBoxGameClass->AddItem(listgc[i]);
	}
	mem_delete_a(listgc);


	SkyXEngine_PreviewKill();
	SXLevelEditor::JobWindow->Visible(true);
	int result = SkyXEngine_CycleMain();

	SXLevelEditor::LEdeleteData();

	SkyXEngine_Kill();
	return result;
}