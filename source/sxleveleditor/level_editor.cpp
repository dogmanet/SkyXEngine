
namespace SXLevelEditor
{
	ISXGUIBaseWnd* JobWindow;
	ISXGUIMenu* MainMenu;
	ISXGUIBaseWnd* RenderWindow;
	ISXGUIToolBar* ToolBar1;
	ISXGUIGroupBox* GroupBoxList;
	ISXGUIGroupBox* GroupBoxData;
	ISXGUIListBox* ListBoxList;
	ISXGUIStatic* StaticListTextCount;
	ISXGUIStatic* StaticListValCount;

	ISXGUIButton* ButtonDelete;

	ISXGUIButton* ButtonGeometryOpen;
	ISXGUIButton* ButtonGreenOpen;
	ISXGUIButton* ButtonGameObject;


	//model
	//{{
	ISXGUIStatic* StaticGeomName;
	ISXGUIEdit* EditGeomName;

	ISXGUIStatic* StaticGeomModel;
	ISXGUIEdit* EditGeomModel;
	ISXGUIButton* ButtonGeomModel;

	ISXGUIStatic* StaticGeomLod1;
	ISXGUIEdit* EditGeomLod1;
	ISXGUIButton* ButtonGeomLod1;
	
	ISXGUIStatic* StaticGeomPos;
	ISXGUIEdit* EditGeomPosX;
	ISXGUIEdit* EditGeomPosY;
	ISXGUIEdit* EditGeomPosZ;
	ISXGUIRadioButton* RadioButtonGeomPosX;
	ISXGUIRadioButton* RadioButtonGeomPosY;
	ISXGUIRadioButton* RadioButtonGeomPosZ;
	ISXGUIStatic* StaticGeomRot;
	ISXGUIEdit* EditGeomRotX;
	ISXGUIEdit* EditGeomRotY;
	ISXGUIEdit* EditGeomRotZ;
	ISXGUIRadioButton* RadioButtonGeomRotX;
	ISXGUIRadioButton* RadioButtonGeomRotY;
	ISXGUIRadioButton* RadioButtonGeomRotZ;
	ISXGUIStatic* StaticGeomScale;
	ISXGUIEdit* EditGeomScaleX;
	ISXGUIEdit* EditGeomScaleY;
	ISXGUIEdit* EditGeomScaleZ;
	ISXGUIRadioButton* RadioButtonGeomScaleX;
	ISXGUIRadioButton* RadioButtonGeomScaleY;
	ISXGUIRadioButton* RadioButtonGeomScaleZ;
	ISXGUIButton* ButtonGeomFinish;
	//}}

	//Green
	//{{
	ISXGUIStatic* StaticGreenName;
	ISXGUIEdit* EditGreenName;

	ISXGUIStatic* StaticGreenModel;
	ISXGUIEdit* EditGreenModel;
	ISXGUIButton* ButtonGreenModel;

	ISXGUIStatic* StaticGreenLod1;
	ISXGUIEdit* EditGreenLod1;
	ISXGUIButton* ButtonGreenLod1;

	ISXGUIStatic* StaticGreenLod2;
	ISXGUIEdit* EditGreenLod2;
	ISXGUIButton* ButtonGreenLod2;

	ISXGUIStatic* StaticGreenMask;
	ISXGUIEdit* EditGreenMask;
	ISXGUIButton* ButtonGreenMask;
	
	ISXGUIStatic* StaticGreenNav;
	ISXGUIEdit* EditGreenNav;
	ISXGUIButton* ButtonGreenNav;

	ISXGUITrackBar* TrackBarGreenDensity;
	ISXGUIButton* ButtonGreenGenerate;
	ISXGUIStatic* StaticGreenDensityText;
	ISXGUIStatic* StaticGreenDensityVal;

	ISXGUIEdit* EditGreenSelX;
	ISXGUIEdit* EditGreenSelY;
	ISXGUIEdit* EditGreenSelZ;
	ISXGUIRadioButton* RadioButtonGreenSelX;
	ISXGUIRadioButton* RadioButtonGreenSelY;
	ISXGUIRadioButton* RadioButtonGreenSelZ;
	ISXGUIComboBox* ComboBoxGreenSel;
	ISXGUIStatic* StaticGreenSelX;
	ISXGUIStatic* StaticGreenSelY;
	ISXGUIStatic* StaticGreenSelZ;
	//}}

	//game object
	//{
	ISXGUIStatic* StaticGameClass;
	ISXGUIComboBox* ComboBoxGameClass;
	ISXGUIListView* ListViewGameClass;
	ISXGUIComboBox* ComboBoxGameValue;
	ISXGUIEdit* EditGameValue;
	ISXGUIButton* ButtonGameValue;
	ISXGUIStatic* StaticGameHelp;
	ISXGUIMemo* MemoGameHelp;
	ISXGUIButton* ButtonGameCreate;
	//}

	ISXGUIStatusBar* StatusBar1;

	void InitAllElements();

	void DeleteAllElements();
	void FinalImageUncheckedMenu();
	

	void GeomActivateAll(bool bf);
	void GeomActivateCreate(bool bf);
	void GeomActivateTrans(bool bf);
	void GeomSel(int sel);

	void GreenActivateAll(bool bf);
	void GreenActivateMain(bool bf);
	void GreenActivateCreate(bool bf);
	void GreenActivateEdit(bool bf);
	void GreenSel(int sel);

	void GameActivateAll(bool bf);
	void GameSel(int sel);
};

LRESULT SXLevelEditor_ButtonGameObjectOpen_Click(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include <SXLevelEditor/game_callback.cpp>
#include <SXLevelEditor/green_callback.cpp>
#include <SXLevelEditor/model_callback.cpp>
#include <SXLevelEditor/common_callback.cpp>

void SXLevelEditor::InitAllElements()
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	SXLevelEditor::JobWindow = SXGUICrBaseWnd("JobWindow","SX Level editor",0,0,0,20,820,690,0,0,CreateSolidBrush(RGB(220,220,220)),0,CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,0,WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(SXLevelEditor::JobWindow);

	SXLevelEditor::JobWindow->AddHandler(ComMenuId, WM_COMMAND);
	SXLevelEditor::JobWindow->AddHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXLevelEditor::JobWindow->AddHandler(MsgEditSize, WM_SIZE);
	SXLevelEditor::JobWindow->MinSizeX = 820;
	SXLevelEditor::JobWindow->MinSizeY = 690;
	SXLevelEditor::MainMenu = SXGUICrMenuEx(IDR_MENU1);
	SXLevelEditor::MainMenu->SetToWindow(SXLevelEditor::JobWindow->GetHWND());

	SXLevelEditor::MainMenu->CheckItem(ID_VIEW_GRID, true);
	SXLevelEditor::MainMenu->CheckItem(ID_VIEW_AXES, true);

	SXLevelEditor::MainMenu->CheckItem(ID_FINALIMAGE_LIGHTINGSCENE, true);
	GData::FinalImage = DS_RT::ds_rt_scene_light_com;

	GData::Editors::SelSelection = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_SELECTION, true);
	GData::Editors::SelBackFacesCull = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_BACKFACESCULL, true);
	GData::Editors::SelZTest = false;
	GData::Editors::SelMesh = true;
	SXLevelEditor::MainMenu->CheckItem(ID_SELECTIONSETTINGS_MESH, true);

	SXLevelEditor::RenderWindow = SXGUICrBaseWnd("RenderWindow", "RenderWindow", 0, 0, 0, 25, 600, 400, 0, LoadCursor(NULL, IDC_ARROW), CreateSolidBrush(RGB(200, 200, 200)), 0, CS_HREDRAW | CS_VREDRAW, WS_CHILD | WS_VISIBLE | WS_BORDER, SXLevelEditor::JobWindow->GetHWND(), 0);
	SXLevelEditor::RenderWindow->GAlign.left = true;
	SXLevelEditor::RenderWindow->GAlign.right = true;
	SXLevelEditor::RenderWindow->GAlign.top = true;
	SXLevelEditor::RenderWindow->GAlign.bottom = true;
	SXLevelEditor::RenderWindow->AddHandler(SXLevelEditor_RenderWindow_MouseMove, WM_MOUSEMOVE);
	SXLevelEditor::RenderWindow->AddHandler(SXLevelEditor_RenderWindow_LDown, WM_LBUTTONDOWN);
	SXLevelEditor::RenderWindow->AddHandler(SXLevelEditor_RenderWindow_LClick, WM_LBUTTONUP);
	SXLevelEditor::RenderWindow->AddHandler(SXLevelEditor_RenderWindow_RClick, WM_RBUTTONUP);
	SXLevelEditor::RenderWindow->AddHandler(SXLevelEditor_RenderWindow_Delete, WM_KEYDOWN, VK_DELETE, 1, 0, 0, 0);

	SXLevelEditor::ToolBar1 = SXGUICrToolBar(0,1,810,24,SXLevelEditor::JobWindow->GetHWND(),0,0);
	SXLevelEditor::ToolBar1->GAlign.left = true;
	SXLevelEditor::ToolBar1->GAlign.right = true;
	SXLevelEditor::ToolBar1->GAlign.top = true;
	SXLevelEditor::ToolBar1->GAlign.bottom = false;
	
	SXLevelEditor::GroupBoxList = SXGUICrGroupBox("GroupBoxList", 601, 26, 200, 400, SXLevelEditor::JobWindow->GetHWND(), WndProcAllDefault, 0);
	SXGUIBaseHandlers::InitHandlerMsg(SXLevelEditor::GroupBoxList);
	SXLevelEditor::GroupBoxList->AddHandler(SXLevelEditor_GroupBoxList_CallWmCommand, WM_COMMAND);
	SXLevelEditor::GroupBoxList->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::GroupBoxList->SetColorText(0,0,0);
	SXLevelEditor::GroupBoxList->SetColorTextBk(220,220,220);
	SXLevelEditor::GroupBoxList->SetTransparentTextBk(true);
	SXLevelEditor::GroupBoxList->SetColorBrush(220,220,220);

	SXLevelEditor::GroupBoxList->GAlign.left = false;
	SXLevelEditor::GroupBoxList->GAlign.right = true;
	SXLevelEditor::GroupBoxList->GAlign.top = true;
	SXLevelEditor::GroupBoxList->GAlign.bottom = true;

	SXLevelEditor::GroupBoxData = SXGUICrGroupBox("", 0, 425, 800, 190, SXLevelEditor::JobWindow->GetHWND(), WndProcAllDefault, 0);
	SXGUIBaseHandlers::InitHandlerMsg(SXLevelEditor::GroupBoxData);
	SXLevelEditor::GroupBoxData->AddHandler(SXLevelEditor_GroupBox_CallWmCommand, WM_COMMAND);
	SXLevelEditor::GroupBoxData->AddHandler(SXLevelEditor_GroupBox_Notify, WM_NOTIFY);
	SXLevelEditor::GroupBoxData->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::GroupBoxData->SetColorText(0,0,0);
	SXLevelEditor::GroupBoxData->SetColorTextBk(220,220,220);
	SXLevelEditor::GroupBoxData->SetTransparentTextBk(true);
	SXLevelEditor::GroupBoxData->SetColorBrush(220,220,220);

	SXLevelEditor::GroupBoxData->GAlign.left = true;
	SXLevelEditor::GroupBoxData->GAlign.right = true;
	SXLevelEditor::GroupBoxData->GAlign.top = false;
	SXLevelEditor::GroupBoxData->GAlign.bottom = true;

	SXLevelEditor::ListBoxList = SXGUICrListBox("", 4, 30, 193, 290, SXLevelEditor::GroupBoxList->GetHWND(), 0, 0, false);
	SXLevelEditor::ListBoxList->GAlign.left = true;
	SXLevelEditor::ListBoxList->GAlign.top = true;
	SXLevelEditor::ListBoxList->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::ListBoxList->SetColorText(0,0,0);
	SXLevelEditor::ListBoxList->SetColorTextBk(255,255,255);
	SXLevelEditor::ListBoxList->SetTransparentTextBk(true);
	SXLevelEditor::ListBoxList->SetColorBrush(255,255,255);
	//SXLevelEditor::ListBoxList->AddHandler(SXLevelEditor_ListBoxList_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticListTextCount = SXGUICrStatic("Count:", 65, 15, 30, 15, SXLevelEditor::GroupBoxList->GetHWND(), 0, 0);
	SXLevelEditor::StaticListTextCount->GAlign.left = true;
	SXLevelEditor::StaticListTextCount->GAlign.top = true;
	SXLevelEditor::StaticListTextCount->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::StaticListTextCount->SetColorText(0,0,0);
	SXLevelEditor::StaticListTextCount->SetColorTextBk(255,255,255);
	SXLevelEditor::StaticListTextCount->SetTransparentTextBk(true);
	SXLevelEditor::StaticListTextCount->SetColorBrush(220,220,220);

	SXLevelEditor::StaticListValCount = SXGUICrStatic("0", 100, 15, 50, 15, SXLevelEditor::GroupBoxList->GetHWND(), 0, 0);
	SXLevelEditor::StaticListValCount->GAlign.left = true;
	SXLevelEditor::StaticListValCount->GAlign.top = true;
	SXLevelEditor::StaticListValCount->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::StaticListValCount->SetColorText(0,0,0);
	SXLevelEditor::StaticListValCount->SetColorTextBk(255,255,255);
	SXLevelEditor::StaticListValCount->SetTransparentTextBk(true);
	SXLevelEditor::StaticListValCount->SetColorBrush(220,220,220);

	SXLevelEditor::ButtonDelete = SXGUICrButton("Delete", 135, 325, 60, 20, 0, SXLevelEditor::GroupBoxList->GetHWND(), 0, 0);
	SXLevelEditor::ButtonDelete->GAlign.left = true;
	SXLevelEditor::ButtonDelete->GAlign.top = true;
	SXLevelEditor::ButtonDelete->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonDelete->AddHandler(SXLevelEditor_ButtonDelete_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonGeometryOpen = SXGUICrButton("Geometry", 5, 350, 60, 20, 0, SXLevelEditor::GroupBoxList->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGeometryOpen->GAlign.left = true;
	SXLevelEditor::ButtonGeometryOpen->GAlign.top = true;
	SXLevelEditor::ButtonGeometryOpen->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::ButtonGeometryOpen->AddHandler(SXLevelEditor_ButtonGeometryOpen_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonGreenOpen = SXGUICrButton("Green", 70, 350, 60, 20, 0, SXLevelEditor::GroupBoxList->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGreenOpen->GAlign.left = true;
	SXLevelEditor::ButtonGreenOpen->GAlign.top = true;
	SXLevelEditor::ButtonGreenOpen->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::ButtonGreenOpen->AddHandler(SXLevelEditor_ButtonGreenOpen_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonGameObject = SXGUICrButton("Game", 135, 350, 60, 20, 0, SXLevelEditor::GroupBoxList->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGameObject->GAlign.left = true;
	SXLevelEditor::ButtonGameObject->GAlign.top = true;
	SXLevelEditor::ButtonGameObject->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGameObject->AddHandler(SXLevelEditor_ButtonGameObjectOpen_Click, WM_LBUTTONUP);

	SXLevelEditor::StatusBar1 = SXGUICrStatusBar("StatusBar1", SXLevelEditor::JobWindow->GetHWND(), 0, 0);
	SXLevelEditor::StatusBar1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	int arrpart[5];
	arrpart[0] = 150;
	arrpart[1] = 300;
	arrpart[2] = 450;
	arrpart[3] = 600;
	arrpart[4] = -1;
	SXLevelEditor::StatusBar1->SetCountParts(5, arrpart);
	SXLevelEditor::StatusBar1->SetTextParts(0, "Level poly: ");
	SXLevelEditor::StatusBar1->SetTextParts(1, "Geom poly: ");
	SXLevelEditor::StatusBar1->SetTextParts(2, "Green poly: ");
	SXLevelEditor::StatusBar1->SetTextParts(3, "");
	SXLevelEditor::StatusBar1->SetTextParts(4, "");

	//Geom
	//{
	SXLevelEditor::StaticGeomName = SXGUICrStatic("Name", 5, 20, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGeomName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomName->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGeomName->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGeomName->SetTransparentTextBk(true);
	SXLevelEditor::StaticGeomName->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGeomName->GAlign.left = true;
	SXLevelEditor::StaticGeomName->GAlign.top = true;
	SXLevelEditor::StaticGeomName->Visible(false);

	SXLevelEditor::EditGeomName = SXGUICrEdit("0", 80, 20, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomName->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomName->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomName->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomName->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomName->GAlign.left = true;
	SXLevelEditor::EditGeomName->GAlign.top = true;
	SXLevelEditor::EditGeomName->Visible(false);
	SXLevelEditor::EditGeomName->AddHandler(SXLevelEditor_EditGeomName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomName->AddHandler(SXLevelEditor_EditGeomName_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticGeomModel = SXGUICrStatic("Model", 5, 40, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGeomModel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomModel->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGeomModel->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGeomModel->SetTransparentTextBk(true);
	SXLevelEditor::StaticGeomModel->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGeomModel->GAlign.left = true;
	SXLevelEditor::StaticGeomModel->GAlign.top = true;
	SXLevelEditor::StaticGeomModel->Visible(false);

	SXLevelEditor::EditGeomModel = SXGUICrEdit("0", 80, 40, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomModel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomModel->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomModel->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomModel->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomModel->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomModel->GAlign.left = true;
	SXLevelEditor::EditGeomModel->GAlign.top = true;
	SXLevelEditor::EditGeomModel->Visible(false);

	SXLevelEditor::ButtonGeomModel = SXGUICrButton("...", 285, 40, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGeomModel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGeomModel->GAlign.left = true;
	SXLevelEditor::ButtonGeomModel->GAlign.top = true;
	SXLevelEditor::ButtonGeomModel->Visible(false);
	SXLevelEditor::ButtonGeomModel->AddHandler(SXLevelEditor_ButtonGeomModel_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGeomLod1 = SXGUICrStatic("Lod1", 5, 60, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGeomLod1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomLod1->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGeomLod1->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGeomLod1->SetTransparentTextBk(true);
	SXLevelEditor::StaticGeomLod1->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGeomLod1->GAlign.left = true;
	SXLevelEditor::StaticGeomLod1->GAlign.top = true;
	SXLevelEditor::StaticGeomLod1->Visible(false);

	SXLevelEditor::EditGeomLod1 = SXGUICrEdit("0", 80, 60, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomLod1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomLod1->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomLod1->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomLod1->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomLod1->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomLod1->GAlign.left = true;
	SXLevelEditor::EditGeomLod1->GAlign.top = true;
	SXLevelEditor::EditGeomLod1->Visible(false);

	SXLevelEditor::ButtonGeomLod1 = SXGUICrButton("...", 285, 60, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGeomLod1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGeomLod1->GAlign.left = true;
	SXLevelEditor::ButtonGeomLod1->GAlign.top = true;
	SXLevelEditor::ButtonGeomLod1->Visible(false);
	SXLevelEditor::ButtonGeomLod1->AddHandler(SXLevelEditor_ButtonGeomLod1_Click, WM_LBUTTONUP);
	
	SXLevelEditor::StaticGeomPos = SXGUICrStatic("Position:", 320, 35, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGeomPos->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomPos->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGeomPos->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGeomPos->SetTransparentTextBk(true);
	SXLevelEditor::StaticGeomPos->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGeomPos->GAlign.left = true;
	SXLevelEditor::StaticGeomPos->GAlign.top = true;
	SXLevelEditor::StaticGeomPos->Visible(false);

	SXLevelEditor::EditGeomPosX = SXGUICrEdit("0", 370, 15, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomPosX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomPosX->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomPosX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomPosX->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomPosX->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomPosX->GAlign.left = true;
	SXLevelEditor::EditGeomPosX->GAlign.top = true;
	SXLevelEditor::EditGeomPosX->Visible(false);
	SXLevelEditor::EditGeomPosX->AddHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomPosX->AddHandler(SXLevelEditor_EditTransformPos_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomPosY = SXGUICrEdit("0", 370, 35, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomPosY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomPosY->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomPosY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomPosY->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomPosY->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomPosY->GAlign.left = true;
	SXLevelEditor::EditGeomPosY->GAlign.top = true;
	SXLevelEditor::EditGeomPosY->Visible(false);
	SXLevelEditor::EditGeomPosY->AddHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomPosY->AddHandler(SXLevelEditor_EditTransformPos_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomPosZ = SXGUICrEdit("0", 370, 55, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomPosZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomPosZ->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomPosZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomPosZ->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomPosZ->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomPosZ->GAlign.left = true;
	SXLevelEditor::EditGeomPosZ->GAlign.top = true;
	SXLevelEditor::EditGeomPosZ->Visible(false);
	SXLevelEditor::EditGeomPosZ->AddHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomPosZ->AddHandler(SXLevelEditor_EditTransformPos_Enter, WM_KILLFOCUS);

	SXLevelEditor::RadioButtonGeomPosX = SXGUICrRadioButton("x", 440, 15, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomPosX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomPosX->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGeomPosX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGeomPosX->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomPosX->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGeomPosX->GAlign.left = true;
	SXLevelEditor::RadioButtonGeomPosX->GAlign.top = true;
	SXLevelEditor::RadioButtonGeomPosX->Visible(false);

	SXLevelEditor::RadioButtonGeomPosY = SXGUICrRadioButton("y", 440, 35, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomPosY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomPosY->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGeomPosY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGeomPosY->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomPosY->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGeomPosY->GAlign.left = true;
	SXLevelEditor::RadioButtonGeomPosY->GAlign.top = true;
	SXLevelEditor::RadioButtonGeomPosY->Visible(false);

	SXLevelEditor::RadioButtonGeomPosZ = SXGUICrRadioButton("z", 440, 55, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomPosZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomPosZ->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGeomPosZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGeomPosZ->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomPosZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGeomPosZ->GAlign.left = true;
	SXLevelEditor::RadioButtonGeomPosZ->GAlign.top = true;
	SXLevelEditor::RadioButtonGeomPosZ->Visible(false);

	SXLevelEditor::StaticGeomRot = SXGUICrStatic("Rotation:", 470, 35, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGeomRot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomRot->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGeomRot->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGeomRot->SetTransparentTextBk(true);
	SXLevelEditor::StaticGeomRot->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGeomRot->GAlign.left = true;
	SXLevelEditor::StaticGeomRot->GAlign.top = true;
	SXLevelEditor::StaticGeomRot->Visible(false);

	SXLevelEditor::EditGeomRotX = SXGUICrEdit("0", 525, 15, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomRotX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomRotX->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomRotX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomRotX->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomRotX->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomRotX->GAlign.left = true;
	SXLevelEditor::EditGeomRotX->GAlign.top = true;
	SXLevelEditor::EditGeomRotX->Visible(false);
	SXLevelEditor::EditGeomRotX->AddHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomRotX->AddHandler(SXLevelEditor_EditTransformRot_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomRotY = SXGUICrEdit("0", 525, 35, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomRotY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomRotY->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomRotY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomRotY->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomRotY->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomRotY->GAlign.left = true;
	SXLevelEditor::EditGeomRotY->GAlign.top = true;
	SXLevelEditor::EditGeomRotY->Visible(false);
	SXLevelEditor::EditGeomRotY->AddHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomRotY->AddHandler(SXLevelEditor_EditTransformRot_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomRotZ = SXGUICrEdit("0", 525, 55, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomRotZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomRotZ->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomRotZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomRotZ->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomRotZ->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomRotZ->GAlign.left = true;
	SXLevelEditor::EditGeomRotZ->GAlign.top = true;
	SXLevelEditor::EditGeomRotZ->Visible(false);
	SXLevelEditor::EditGeomRotZ->AddHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomRotZ->AddHandler(SXLevelEditor_EditTransformRot_Enter, WM_KILLFOCUS);

	SXLevelEditor::RadioButtonGeomRotX = SXGUICrRadioButton("x", 595, 15, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomRotX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomRotX->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGeomRotX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGeomRotX->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomRotX->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGeomRotX->GAlign.left = true;
	SXLevelEditor::RadioButtonGeomRotX->GAlign.top = true;
	SXLevelEditor::RadioButtonGeomRotX->Visible(false);

	SXLevelEditor::RadioButtonGeomRotY = SXGUICrRadioButton("y", 595, 35, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomRotY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomRotY->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGeomRotY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGeomRotY->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomRotY->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGeomRotY->GAlign.left = true;
	SXLevelEditor::RadioButtonGeomRotY->GAlign.top = true;
	SXLevelEditor::RadioButtonGeomRotY->Visible(false);

	SXLevelEditor::RadioButtonGeomRotZ = SXGUICrRadioButton("z", 595, 55, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomRotZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomRotZ->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGeomRotZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGeomRotZ->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomRotZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGeomRotZ->GAlign.left = true;
	SXLevelEditor::RadioButtonGeomRotZ->GAlign.top = true;
	SXLevelEditor::RadioButtonGeomRotZ->Visible(false);

	SXLevelEditor::StaticGeomScale = SXGUICrStatic("Scale:", 625, 35, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGeomScale->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomScale->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGeomScale->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGeomScale->SetTransparentTextBk(true);
	SXLevelEditor::StaticGeomScale->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGeomScale->GAlign.left = true;
	SXLevelEditor::StaticGeomScale->GAlign.top = true;
	SXLevelEditor::StaticGeomScale->Visible(false);

	SXLevelEditor::EditGeomScaleX = SXGUICrEdit("0", 675, 15, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomScaleX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomScaleX->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomScaleX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomScaleX->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomScaleX->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomScaleX->GAlign.left = true;
	SXLevelEditor::EditGeomScaleX->GAlign.top = true;
	SXLevelEditor::EditGeomScaleX->Visible(false);
	SXLevelEditor::EditGeomScaleX->AddHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomScaleX->AddHandler(SXLevelEditor_EditTransformScale_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomScaleY = SXGUICrEdit("0", 675, 35, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomScaleY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomScaleY->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomScaleY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomScaleY->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomScaleY->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomScaleY->GAlign.left = true;
	SXLevelEditor::EditGeomScaleY->GAlign.top = true;
	SXLevelEditor::EditGeomScaleY->Visible(false);
	SXLevelEditor::EditGeomScaleY->AddHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomScaleY->AddHandler(SXLevelEditor_EditTransformScale_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomScaleZ = SXGUICrEdit("0", 675, 55, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGeomScaleZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomScaleZ->SetColorText(0, 0, 0);
	SXLevelEditor::EditGeomScaleZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGeomScaleZ->SetTransparentTextBk(true);
	SXLevelEditor::EditGeomScaleZ->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGeomScaleZ->GAlign.left = true;
	SXLevelEditor::EditGeomScaleZ->GAlign.top = true;
	SXLevelEditor::EditGeomScaleZ->Visible(false);
	SXLevelEditor::EditGeomScaleZ->AddHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomScaleZ->AddHandler(SXLevelEditor_EditTransformScale_Enter, WM_KILLFOCUS);

	SXLevelEditor::RadioButtonGeomScaleX = SXGUICrRadioButton("x", 745, 15, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomScaleX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomScaleX->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGeomScaleX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGeomScaleX->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomScaleX->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGeomScaleX->GAlign.left = true;
	SXLevelEditor::RadioButtonGeomScaleX->GAlign.top = true;
	SXLevelEditor::RadioButtonGeomScaleX->Visible(false);

	SXLevelEditor::RadioButtonGeomScaleY = SXGUICrRadioButton("y", 745, 35, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomScaleY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomScaleY->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGeomScaleY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGeomScaleY->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomScaleY->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGeomScaleY->GAlign.left = true;
	SXLevelEditor::RadioButtonGeomScaleY->GAlign.top = true;
	SXLevelEditor::RadioButtonGeomScaleY->Visible(false);

	SXLevelEditor::RadioButtonGeomScaleZ = SXGUICrRadioButton("z", 745, 55, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomScaleZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomScaleZ->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGeomScaleZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGeomScaleZ->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomScaleZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGeomScaleZ->GAlign.left = true;
	SXLevelEditor::RadioButtonGeomScaleZ->GAlign.top = true;
	SXLevelEditor::RadioButtonGeomScaleZ->Visible(false);

	SXLevelEditor::ButtonGeomFinish = SXGUICrButton("Finish", 100, 95, 100, 20, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGeomFinish->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGeomFinish->GAlign.left = true;
	SXLevelEditor::ButtonGeomFinish->GAlign.top = true;
	SXLevelEditor::ButtonGeomFinish->Visible(false);
	SXLevelEditor::ButtonGeomFinish->AddHandler(SXLevelEditor_ButtonGeomFinish_Click, WM_LBUTTONUP);
	//}


	//Green
	//{

	SXLevelEditor::StaticGreenName = SXGUICrStatic("Name", 5, 20, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenName->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenName->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenName->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenName->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenName->GAlign.left = true;
	SXLevelEditor::StaticGreenName->GAlign.top = true;
	SXLevelEditor::StaticGreenName->Visible(false);

	SXLevelEditor::EditGreenName = SXGUICrEdit("0", 80, 20, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGreenName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenName->SetColorText(0, 0, 0);
	SXLevelEditor::EditGreenName->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGreenName->SetTransparentTextBk(true);
	SXLevelEditor::EditGreenName->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGreenName->GAlign.left = true;
	SXLevelEditor::EditGreenName->GAlign.top = true;
	SXLevelEditor::EditGreenName->Visible(false);
	SXLevelEditor::EditGreenName->AddHandler(SXLevelEditor_EditGreenName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGreenName->AddHandler(SXLevelEditor_EditGreenName_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticGreenModel = SXGUICrStatic("Model", 5, 40, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenModel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenModel->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenModel->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenModel->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenModel->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenModel->GAlign.left = true;
	SXLevelEditor::StaticGreenModel->GAlign.top = true;
	SXLevelEditor::StaticGreenModel->Visible(false);

	SXLevelEditor::EditGreenModel = SXGUICrEdit("0", 80, 40, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGreenModel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenModel->SetColorText(0, 0, 0);
	SXLevelEditor::EditGreenModel->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGreenModel->SetTransparentTextBk(true);
	SXLevelEditor::EditGreenModel->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGreenModel->GAlign.left = true;
	SXLevelEditor::EditGreenModel->GAlign.top = true;
	SXLevelEditor::EditGreenModel->Visible(false);

	SXLevelEditor::ButtonGreenModel = SXGUICrButton("...", 285, 40, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGreenModel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenModel->GAlign.left = true;
	SXLevelEditor::ButtonGreenModel->GAlign.top = true;
	SXLevelEditor::ButtonGreenModel->Visible(false);
	SXLevelEditor::ButtonGreenModel->AddHandler(SXLevelEditor_ButtonGreenModel_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGreenLod1 = SXGUICrStatic("Lod1", 5, 60, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenLod1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenLod1->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenLod1->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenLod1->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenLod1->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenLod1->GAlign.left = true;
	SXLevelEditor::StaticGreenLod1->GAlign.top = true;
	SXLevelEditor::StaticGreenLod1->Visible(false);

	SXLevelEditor::EditGreenLod1 = SXGUICrEdit("0", 80, 60, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGreenLod1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenLod1->SetColorText(0, 0, 0);
	SXLevelEditor::EditGreenLod1->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGreenLod1->SetTransparentTextBk(true);
	SXLevelEditor::EditGreenLod1->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGreenLod1->GAlign.left = true;
	SXLevelEditor::EditGreenLod1->GAlign.top = true;
	SXLevelEditor::EditGreenLod1->Visible(false);

	SXLevelEditor::ButtonGreenLod1 = SXGUICrButton("...", 285, 60, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGreenLod1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenLod1->GAlign.left = true;
	SXLevelEditor::ButtonGreenLod1->GAlign.top = true;
	SXLevelEditor::ButtonGreenLod1->Visible(false);
	SXLevelEditor::ButtonGreenLod1->AddHandler(SXLevelEditor_ButtonGreenLod1_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGreenLod2 = SXGUICrStatic("Lod2", 5, 80, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenLod2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenLod2->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenLod2->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenLod2->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenLod2->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenLod2->GAlign.left = true;
	SXLevelEditor::StaticGreenLod2->GAlign.top = true;
	SXLevelEditor::StaticGreenLod2->Visible(false);

	SXLevelEditor::EditGreenLod2 = SXGUICrEdit("0", 80, 80, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGreenLod2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenLod2->SetColorText(0, 0, 0);
	SXLevelEditor::EditGreenLod2->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGreenLod2->SetTransparentTextBk(true);
	SXLevelEditor::EditGreenLod2->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGreenLod2->GAlign.left = true;
	SXLevelEditor::EditGreenLod2->GAlign.top = true;
	SXLevelEditor::EditGreenLod2->Visible(false);

	SXLevelEditor::ButtonGreenLod2 = SXGUICrButton("...", 285, 80, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGreenLod2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenLod2->GAlign.left = true;
	SXLevelEditor::ButtonGreenLod2->GAlign.top = true;
	SXLevelEditor::ButtonGreenLod2->Visible(false);
	SXLevelEditor::ButtonGreenLod2->AddHandler(SXLevelEditor_ButtonGreenLod2_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonGreenMask = SXGUICrButton("...", 285, 100, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGreenMask->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenMask->GAlign.left = true;
	SXLevelEditor::ButtonGreenMask->GAlign.top = true;
	SXLevelEditor::ButtonGreenMask->Visible(false);
	SXLevelEditor::ButtonGreenMask->AddHandler(SXLevelEditor_ButtonGreenMask_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGreenMask = SXGUICrStatic("Mask texture", 5, 100, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenMask->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenMask->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenMask->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenMask->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenMask->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenMask->GAlign.left = true;
	SXLevelEditor::StaticGreenMask->GAlign.top = true;
	SXLevelEditor::StaticGreenMask->Visible(false);

	SXLevelEditor::EditGreenMask = SXGUICrEdit("0", 80, 100, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGreenMask->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenMask->SetColorText(0, 0, 0);
	SXLevelEditor::EditGreenMask->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGreenMask->SetTransparentTextBk(true);
	SXLevelEditor::EditGreenMask->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGreenMask->GAlign.left = true;
	SXLevelEditor::EditGreenMask->GAlign.top = true;
	SXLevelEditor::EditGreenMask->Visible(false);

	SXLevelEditor::StaticGreenNav = SXGUICrStatic("NavMesh:", 330, 20, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenNav->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenNav->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenNav->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenNav->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenNav->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenNav->GAlign.left = true;
	SXLevelEditor::StaticGreenNav->GAlign.top = true;
	SXLevelEditor::StaticGreenNav->Visible(false);

	SXLevelEditor::EditGreenNav = SXGUICrEdit("", 380, 20, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGreenNav->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenNav->SetColorText(0, 0, 0);
	SXLevelEditor::EditGreenNav->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGreenNav->SetTransparentTextBk(true);
	SXLevelEditor::EditGreenNav->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGreenNav->GAlign.left = true;
	SXLevelEditor::EditGreenNav->GAlign.top = true;
	SXLevelEditor::EditGreenNav->Visible(false);

	SXLevelEditor::ButtonGreenNav = SXGUICrButton("...", 585, 20, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGreenNav->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenNav->GAlign.left = true;
	SXLevelEditor::ButtonGreenNav->GAlign.top = true;
	SXLevelEditor::ButtonGreenNav->Visible(false);
	SXLevelEditor::ButtonGreenNav->AddHandler(SXLevelEditor_ButtonGreenNav_Click, WM_LBUTTONUP);

	SXLevelEditor::TrackBarGreenDensity = SXGUICrTrackBar("", 330, 65, 280, 20, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::TrackBarGreenDensity->SetMinMax(1, 100);
	SXLevelEditor::TrackBarGreenDensity->AddHandler(SXLevelEditor_TrackBarGreenDensity_MouseMove, WM_MOUSEMOVE);
	SXLevelEditor::TrackBarGreenDensity->GAlign.left = true;
	SXLevelEditor::TrackBarGreenDensity->GAlign.top = true;
	SXLevelEditor::TrackBarGreenDensity->Visible(false);

	SXLevelEditor::ButtonGreenGenerate = SXGUICrButton("Generate", 420, 87, 100, 30, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGreenGenerate->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenGenerate->GAlign.left = true;
	SXLevelEditor::ButtonGreenGenerate->GAlign.top = true;
	SXLevelEditor::ButtonGreenGenerate->Visible(false);
	SXLevelEditor::ButtonGreenGenerate->AddHandler(SXLevelEditor_ButtonGreenGenerate_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGreenDensityText = SXGUICrStatic("Density:", 380, 45, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenDensityText->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenDensityText->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenDensityText->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenDensityText->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenDensityText->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenDensityText->GAlign.left = true;
	SXLevelEditor::StaticGreenDensityText->GAlign.top = true;
	SXLevelEditor::StaticGreenDensityText->Visible(false);

	SXLevelEditor::StaticGreenDensityVal = SXGUICrStatic("0", 430, 45, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenDensityVal->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenDensityVal->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenDensityVal->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenDensityVal->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenDensityVal->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenDensityVal->GAlign.left = true;
	SXLevelEditor::StaticGreenDensityVal->GAlign.top = true;
	SXLevelEditor::StaticGreenDensityVal->Visible(false);

	SXLevelEditor::RadioButtonGreenSelX = SXGUICrRadioButton("x", 765, 40, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGreenSelX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGreenSelX->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGreenSelX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGreenSelX->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGreenSelX->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGreenSelX->GAlign.left = true;
	SXLevelEditor::RadioButtonGreenSelX->GAlign.top = true;
	SXLevelEditor::RadioButtonGreenSelX->Visible(false);

	SXLevelEditor::RadioButtonGreenSelY = SXGUICrRadioButton("y", 765, 60, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGreenSelY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGreenSelY->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGreenSelY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGreenSelY->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGreenSelY->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGreenSelY->GAlign.left = true;
	SXLevelEditor::RadioButtonGreenSelY->GAlign.top = true;
	SXLevelEditor::RadioButtonGreenSelY->Visible(false);

	SXLevelEditor::RadioButtonGreenSelZ = SXGUICrRadioButton("z", 765, 80, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonGreenSelZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGreenSelZ->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonGreenSelZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonGreenSelZ->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonGreenSelZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonGreenSelZ->GAlign.left = true;
	SXLevelEditor::RadioButtonGreenSelZ->GAlign.top = true;
	SXLevelEditor::RadioButtonGreenSelZ->Visible(false);
	
	SXLevelEditor::ComboBoxGreenSel = SXGUICrComboBox("", 620, 20, 150, 100, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ComboBoxGreenSel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ComboBoxGreenSel->SetColorText(0, 0, 0);
	SXLevelEditor::ComboBoxGreenSel->SetColorTextBk(255, 255, 255);
	SXLevelEditor::ComboBoxGreenSel->SetTransparentTextBk(true);
	SXLevelEditor::ComboBoxGreenSel->SetColorBrush(220, 220, 220);
	SXLevelEditor::ComboBoxGreenSel->AddItem("single select");
	SXLevelEditor::ComboBoxGreenSel->AddItem("single create");
	SXLevelEditor::ComboBoxGreenSel->AddItem("multiple create");
	SXLevelEditor::ComboBoxGreenSel->AddItem("single delete");
	//SXLevelEditor::ComboBoxGreenSel->AddItem("multiple delete");
	SXLevelEditor::ComboBoxGreenSel->SetSel(0);
	SXLevelEditor::ComboBoxGreenSel->GAlign.left = true;
	SXLevelEditor::ComboBoxGreenSel->GAlign.top = true;
	SXLevelEditor::ComboBoxGreenSel->Visible(false);
	
	SXLevelEditor::StaticGreenSelX = SXGUICrStatic("Width volume:", 620, 40, 75, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenSelX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenSelX->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenSelX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenSelX->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenSelX->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenSelX->GAlign.left = true;
	SXLevelEditor::StaticGreenSelX->GAlign.top = true;
	SXLevelEditor::StaticGreenSelX->Visible(false);

	SXLevelEditor::StaticGreenSelY = SXGUICrStatic("Height volume:", 620, 60, 75, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenSelY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenSelY->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenSelY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenSelY->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenSelY->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenSelY->GAlign.left = true;
	SXLevelEditor::StaticGreenSelY->GAlign.top = true;
	SXLevelEditor::StaticGreenSelY->Visible(false);

	SXLevelEditor::StaticGreenSelZ = SXGUICrStatic("Depth volume:", 620, 80, 75, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGreenSelZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenSelZ->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGreenSelZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGreenSelZ->SetTransparentTextBk(true);
	SXLevelEditor::StaticGreenSelZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGreenSelZ->GAlign.left = true;
	SXLevelEditor::StaticGreenSelZ->GAlign.top = true;
	SXLevelEditor::StaticGreenSelZ->Visible(false);

	SXLevelEditor::EditGreenSelX = SXGUICrEdit("0", 695, 40, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGreenSelX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenSelX->SetColorText(0, 0, 0);
	SXLevelEditor::EditGreenSelX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGreenSelX->SetTransparentTextBk(true);
	SXLevelEditor::EditGreenSelX->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGreenSelX->GAlign.left = true;
	SXLevelEditor::EditGreenSelX->GAlign.top = true;
	SXLevelEditor::EditGreenSelX->Visible(false);

	SXLevelEditor::EditGreenSelY = SXGUICrEdit("0", 695, 60, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGreenSelY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenSelY->SetColorText(0, 0, 0);
	SXLevelEditor::EditGreenSelY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGreenSelY->SetTransparentTextBk(true);
	SXLevelEditor::EditGreenSelY->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGreenSelY->GAlign.left = true;
	SXLevelEditor::EditGreenSelY->GAlign.top = true;
	SXLevelEditor::EditGreenSelY->Visible(false);

	SXLevelEditor::EditGreenSelZ = SXGUICrEdit("0", 695, 80, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGreenSelZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenSelZ->SetColorText(0, 0, 0);
	SXLevelEditor::EditGreenSelZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGreenSelZ->SetTransparentTextBk(true);
	SXLevelEditor::EditGreenSelZ->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGreenSelZ->GAlign.left = true;
	SXLevelEditor::EditGreenSelZ->GAlign.top = true;
	SXLevelEditor::EditGreenSelZ->Visible(false);
	//}

	//Game
	//{
	SXLevelEditor::StaticGameClass = SXGUICrStatic("Class:", 415, 15, 35, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGameClass->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGameClass->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGameClass->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGameClass->SetTransparentTextBk(true);
	SXLevelEditor::StaticGameClass->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGameClass->GAlign.left = true;
	SXLevelEditor::StaticGameClass->GAlign.top = true;
	SXLevelEditor::StaticGameClass->Visible(false);

	SXLevelEditor::ComboBoxGameClass = SXGUICrComboBoxEx("", 455, 15, 230, 100, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ComboBoxGameClass->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ComboBoxGameClass->SetColorText(0, 0, 0);
	SXLevelEditor::ComboBoxGameClass->SetColorTextBk(255, 255, 255);
	SXLevelEditor::ComboBoxGameClass->SetTransparentTextBk(true);
	SXLevelEditor::ComboBoxGameClass->SetColorBrush(255, 255, 255);
	SXLevelEditor::ComboBoxGameClass->GAlign.left = true;
	SXLevelEditor::ComboBoxGameClass->GAlign.top = true;
	SXLevelEditor::ComboBoxGameClass->Visible(false);
	SXLevelEditor::ComboBoxGameClass->AddItem("");

	SXLevelEditor::ListViewGameClass = SXGUICrListView("", 5, 5, 400, 180, SXLevelEditor::GroupBoxData->GetHWND(), WndProcAllDefault, 0);
	SXLevelEditor::ListViewGameClass->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ListViewGameClass->SetColorText(0, 0, 0);
	SXLevelEditor::ListViewGameClass->SetColorTextBk(255, 255, 255);
	SXLevelEditor::ListViewGameClass->SetTransparentTextBk(true);
	SXLevelEditor::ListViewGameClass->SetColorBrush(255, 255, 255);
	SXLevelEditor::ListViewGameClass->AddColumn("Property Name",199);
	SXLevelEditor::ListViewGameClass->AddColumn("Value", 199);
	SXLevelEditor::ListViewGameClass->GAlign.left = true;
	SXLevelEditor::ListViewGameClass->GAlign.top = true;
	SXLevelEditor::ListViewGameClass->Visible(false);

	SXLevelEditor::ComboBoxGameValue = SXGUICrComboBox("", 415, 45, 270, 100, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ComboBoxGameValue->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ComboBoxGameValue->SetColorText(0, 0, 0);
	SXLevelEditor::ComboBoxGameValue->SetColorTextBk(255, 255, 255);
	SXLevelEditor::ComboBoxGameValue->SetTransparentTextBk(true);
	SXLevelEditor::ComboBoxGameValue->SetColorBrush(255, 255, 255);
	SXLevelEditor::ComboBoxGameValue->GAlign.left = true;
	SXLevelEditor::ComboBoxGameValue->GAlign.top = true;
	SXLevelEditor::ComboBoxGameValue->Visible(false);

	SXLevelEditor::EditGameValue = SXGUICrEdit("", 415, 45, 240, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditGameValue->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGameValue->SetColorText(0, 0, 0);
	SXLevelEditor::EditGameValue->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditGameValue->SetTransparentTextBk(true);
	SXLevelEditor::EditGameValue->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditGameValue->GAlign.left = true;
	SXLevelEditor::EditGameValue->GAlign.top = true;
	SXLevelEditor::EditGameValue->Visible(false);
	SXLevelEditor::EditGameValue->AddHandler(SXLevelEditor_EditGameValue_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGameValue->AddHandler(SXLevelEditor_EditGameValue_Enter, WM_KILLFOCUS);

	SXLevelEditor::ButtonGameValue = SXGUICrButton("...", 660, 45, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGameValue->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGameValue->GAlign.left = true;
	SXLevelEditor::ButtonGameValue->GAlign.top = true;
	SXLevelEditor::ButtonGameValue->Visible(false);
	SXLevelEditor::ButtonGameValue->AddHandler(SXLevelEditor_ButtonGameValue_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGameHelp = SXGUICrStatic("Help:", 415, 65, 35, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticGameHelp->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGameHelp->SetColorText(0, 0, 0);
	SXLevelEditor::StaticGameHelp->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticGameHelp->SetTransparentTextBk(true);
	SXLevelEditor::StaticGameHelp->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticGameHelp->GAlign.left = true;
	SXLevelEditor::StaticGameHelp->GAlign.top = true;
	SXLevelEditor::StaticGameHelp->Visible(false);

	SXLevelEditor::MemoGameHelp = SXGUICrMemo("Help", 415, 85, 270, 95, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::MemoGameHelp->ReadOnly(true);
	SXLevelEditor::MemoGameHelp->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::MemoGameHelp->SetColorText(0, 0, 0);
	SXLevelEditor::MemoGameHelp->SetColorTextBk(255, 255, 255);
	SXLevelEditor::MemoGameHelp->SetTransparentTextBk(true);
	SXLevelEditor::MemoGameHelp->SetColorBrush(220, 220, 220);
	SXLevelEditor::MemoGameHelp->GAlign.left = true;
	SXLevelEditor::MemoGameHelp->GAlign.top = true;
	SXLevelEditor::MemoGameHelp->Visible(false);

	SXLevelEditor::ButtonGameCreate = SXGUICrButton("Create", 695, 160, 100, 20, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGameCreate->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGameCreate->GAlign.left = true;
	SXLevelEditor::ButtonGameCreate->GAlign.top = true;
	SXLevelEditor::ButtonGameCreate->Visible(false);
	SXLevelEditor::ButtonGameCreate->AddHandler(SXLevelEditor_ButtonGameCreate_Click, WM_LBUTTONUP);
	//}
}

void SXLevelEditor::DeleteAllElements()
{
	mem_release(JobWindow);
	mem_release(MainMenu);
	mem_release(RenderWindow);
	mem_release(ToolBar1);
	mem_release(GroupBoxList);
	mem_release(GroupBoxData);
	mem_release(ListBoxList);
	mem_release(StaticListTextCount);
	mem_release(StaticListValCount);

	mem_release(ButtonDelete);

	mem_release(ButtonGeometryOpen);
	mem_release(ButtonGreenOpen);
	mem_release(ButtonGameObject);
	mem_release(StatusBar1);

	//model
	//{{
	mem_release(StaticGeomName);
	mem_release(StaticGeomModel);
	mem_release(StaticGeomLod1);
	mem_release(EditGeomName);
	mem_release(EditGeomModel);
	mem_release(EditGeomLod1);
	mem_release(ButtonGeomLod1);
	mem_release(ButtonGeomModel);
	
	mem_release(StaticGeomPos);
	mem_release(EditGeomPosX);
	mem_release(EditGeomPosY);
	mem_release(EditGeomPosZ);
	mem_release(RadioButtonGeomPosX);
	mem_release(RadioButtonGeomPosY);
	mem_release(RadioButtonGeomPosZ);
	mem_release(StaticGeomRot);
	mem_release(EditGeomRotX);
	mem_release(EditGeomRotY);
	mem_release(EditGeomRotZ);
	mem_release(RadioButtonGeomRotX);
	mem_release(RadioButtonGeomRotY);
	mem_release(RadioButtonGeomRotZ);
	mem_release(StaticGeomScale);
	mem_release(EditGeomScaleX);
	mem_release(EditGeomScaleY);
	mem_release(EditGeomScaleZ);
	mem_release(RadioButtonGeomScaleX);
	mem_release(RadioButtonGeomScaleY);
	mem_release(RadioButtonGeomScaleZ);
	mem_release(ButtonGeomFinish);
	//}}

	mem_release(StaticGreenName);
	mem_release(StaticGreenModel);
	mem_release(StaticGreenLod1);
	mem_release(StaticGreenLod2);
	mem_release(EditGreenName);
	mem_release(EditGreenModel);
	mem_release(EditGreenLod1);
	mem_release(EditGreenLod2);
	mem_release(ButtonGreenLod1);
	mem_release(ButtonGreenModel);
	mem_release(ButtonGreenLod2);
	mem_release(ButtonGreenMask);
	mem_release(StaticGreenMask);
	mem_release(EditGreenMask);
	mem_release(StaticGreenNav);
	mem_release(EditGreenNav);
	mem_release(ButtonGreenNav);

	mem_release(TrackBarGreenDensity);
	mem_release(ButtonGreenGenerate);
	mem_release(StaticGreenDensityText);
	mem_release(StaticGreenDensityVal);

	mem_delete(SXLevelEditor::EditGreenSelX);
	mem_delete(SXLevelEditor::EditGreenSelY);
	mem_delete(SXLevelEditor::EditGreenSelZ);
	mem_delete(SXLevelEditor::RadioButtonGreenSelX);
	mem_delete(SXLevelEditor::RadioButtonGreenSelY);
	mem_delete(SXLevelEditor::RadioButtonGreenSelZ);
	//mem_delete(SXLevelEditor::ButtonSingleSelDel);
	mem_delete(SXLevelEditor::ComboBoxGreenSel);
	mem_delete(SXLevelEditor::StaticGreenSelX);
	mem_delete(SXLevelEditor::StaticGreenSelY);
	mem_delete(SXLevelEditor::StaticGreenSelZ);
	/*mem_delete(SXLevelEditor::EditMultipleCreateVWidth);
	mem_delete(SXLevelEditor::EditMultipleCreateVHeight);
	mem_delete(SXLevelEditor::EditMultipleCreateVDepth);*/


	mem_delete(SXLevelEditor::StaticGameClass);
	mem_delete(SXLevelEditor::ComboBoxGameClass);
	mem_delete(SXLevelEditor::ListViewGameClass);
	mem_delete(SXLevelEditor::ComboBoxGameValue);
	mem_delete(SXLevelEditor::EditGameValue);
	mem_delete(SXLevelEditor::ButtonGameValue);
	mem_delete(SXLevelEditor::StaticGameHelp);
	mem_delete(SXLevelEditor::MemoGameHelp);
	mem_delete(SXLevelEditor::ButtonGameCreate);
}
