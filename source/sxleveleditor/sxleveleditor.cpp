
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

	int countgc = SXGame_EntGetClassListCount();
	const char** listgc = new const char*[countgc];

	SXGame_EntGetClassList(listgc, countgc);
	for (int i = 0; i < countgc; ++i)
	{
		SXLevelEditor::ComboBoxGameClass->AddItem(listgc[i]);
	}
	mem_delete_a(listgc);

	/*
	if (SSInput_GetKeyEvents(SIM_LBUTTON) == InputEvents::iv_k_first)
	{
	//float3 start = GData::ConstCurrCamPos;
	//float3 end = start + (GData::ConstCurrCamDir * 1000);
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

	//if (pt.x <= GData::WinSize.x && pt.y <= GData::WinSize.y && SGeom_GreenTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idgreen, &idsplit, &idobj, &idmtl))
	//{
	//GData::Editors::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GREEN;
	//GData::Editors::ActiveElement = idgreen;
	//GData::Editors::ActiveGreenSplit = idsplit;
	//GData::Editors::ActiveGreenObject = idobj;
	//SGeom_GreenDelObject(idgreen, idsplit, idobj);
	//int qwert = 0;
	//}

	if (pt.x <= GData::WinSize.x && pt.y <= GData::WinSize.y && SGeom_ModelsTraceBeam(&GData::ConstCurrCamPos, &camDir, &_res, &idmodel, &idmtl))
	{
		//MCInitElemsSelModel(idmodel);
		//GData::Editors::ActiveElement = idmodel;
		//GData::Editors::ActiveGroupType = EDITORS_LEVEL_GROUPTYPE_GEOM;
		SGeom_GreenAddObject(0, &_res);
	}
}*/


	

	//SXLevelEditor::GroupBoxData->Visible(false);

	/*HWND hWndLV = CreateWindow(WC_LISTVIEW, "",
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
		5, 430, 300, 200,
		SXLevelEditor::JobWindow->GetHWND(), 0, GetModuleHandle(NULL), NULL);

	// Чтобы определялись строка (item) и столбец (subitem) обязательно устанавливаем
	// расширенный стиль LVS_EX_FULLROWSELECT.
	ListView_SetExtendedListViewStyleEx(hWndLV, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);*/

	/*ISXGUIListView* lv = SXGUICrListView("", 50, 430, 300, 100, SXLevelEditor::JobWindow->GetHWND(), 0, 0);
	lv->InsertColumn("column", 0, 100);
	lv->AddColumn("column2", 100);
	lv->AddString();
	lv->AddString();
	lv->AddString();
	lv->AddString();
	lv->AddString();
	lv->AddString();
	lv->AddString();
	lv->AddString();
	lv->AddString();
	//lv->AddItem("0", 0);
	//lv->AddItem("1", 1);
	//lv->AddItem("2", 2);
	lv->SetTextItem("0", 0, 0);
	lv->SetTextItem("1", 0, 1);
	lv->SetTextItem("0", 1, 0);
	lv->SetTextItem("1", 1, 1);
	lv->SetTextItem("0", 2, 0);
	lv->SetTextItem("1", 2, 1);
	lv->SetTextItem("0", 3, 0);
	lv->SetTextItem("1", 3, 1);
	lv->SetTextItem("0", 4, 0);
	lv->SetTextItem("1", 4, 1);
	lv->SetTextItem("0", 5, 0);
	lv->SetTextItem("1", 5, 1);
	lv->SetTextItem("0", 6, 0);
	lv->SetTextItem("1", 6, 1);
	lv->SetTextItem("0", 7, 0);
	lv->SetTextItem("1", 7, 1);
	lv->SetSelString(1);
	int selectedstr = lv->GetSelString();
	//lv->SetTextItem(0, 2, "2");

	//lv->Clear();*/

	/*int str = SXLevelEditor::ListViewGameClass->AddString();
	SXLevelEditor::ListViewGameClass->SetTextItem("txtkey", 0, str);
	SXLevelEditor::ListViewGameClass->SetTextItem("txtval", 0, str);*/

	int result = SkyXEngine_CycleMain();
	SkyXEngine_Kill();
	return result;
}