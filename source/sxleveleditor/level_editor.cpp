

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
	ISXGUIButton* Button5;

	ISXGUIButton* ButtonGeometryOpen;
	ISXGUIButton* ButtonGreenOpen;
	ISXGUIButton* ButtonLightOpen;


	//model
	//{{
	ISXGUIStatic* StaticName;
	ISXGUIStatic* StaticModel;
	ISXGUIStatic* StaticLod1;
	ISXGUIStatic* StaticLod2;
	ISXGUIEdit* EditName;
	ISXGUIEdit* EditModel;
	ISXGUIEdit* EditLod1;
	ISXGUIEdit* EditLod2;
	ISXGUIButton* ButtonLod1;
	ISXGUIButton* ButtonModel;
	ISXGUIButton* ButtonLod2;
	ISXGUIButton* ButtonMask;
	ISXGUIStatic* StaticMask;
	ISXGUIEdit* EditMask;
	ISXGUIStatusBar* StatusBar1;
	ISXGUIStatic* StaticPos;
	ISXGUIEdit* EditPosX;
	ISXGUIEdit* EditPosY;
	ISXGUIEdit* EditPosZ;
	ISXGUIRadioButton* RadioButtonPosX;
	ISXGUIRadioButton* RadioButtonPosY;
	ISXGUIRadioButton* RadioButtonPosZ;
	ISXGUIStatic* StaticRot;
	ISXGUIEdit* EditRotX;
	ISXGUIEdit* EditRotY;
	ISXGUIEdit* EditRotZ;
	ISXGUIRadioButton* RadioButtonRotX;
	ISXGUIRadioButton* RadioButtonRotY;
	ISXGUIRadioButton* RadioButtonRotZ;
	ISXGUIStatic* StaticScale;
	ISXGUIEdit* EditScaleX;
	ISXGUIEdit* EditScaleY;
	ISXGUIEdit* EditScaleZ;
	ISXGUIRadioButton* RadioButtonScaleX;
	ISXGUIRadioButton* RadioButtonScaleY;
	ISXGUIRadioButton* RadioButtonScaleZ;
	//ISXGUICheckBox* CheckBoxCG;
	ISXGUIButton* ButtonFinish;
	//}}

	ISXGUIStatic* StaticGreenNav;
	ISXGUIEdit* EditGreenNav;
	ISXGUIButton* ButtonGreenNav;

	ISXGUITrackBar* TrackBarDensity;
	ISXGUIButton* ButtonGenerate;
	ISXGUIStatic* StaticDensityText;
	ISXGUIStatic* StaticDensityVal;


	///
	ISXGUIStatic* LightStaticBias;
	ISXGUICheckBox* LightCheckBoxShadow;
	ISXGUICheckBox* LightCheckBoxGlobal;
	ISXGUICheckBox* LightCheckBoxEnable;
	ISXGUIComboBox* LightComboBoxTypeLight;
	ISXGUIComboBox* LightComboBoxTypeLightShadow;
	ISXGUIStatic* LightStaticFarShadow;
	ISXGUIEdit* LightEditFarShadow;
	ISXGUIStatic* LightStaticPos;
	//ISXGUICheckBox* LightCheckBoxPosLightOrMesh;
	ISXGUIEdit* LightEditPosY;
	ISXGUIEdit* LightEditPosZ;
	ISXGUIRadioButton* LightRadioButtonPosX;
	ISXGUIRadioButton* LightRadioButtonPosY;
	ISXGUIRadioButton* LightRadioButtonPosZ;
	ISXGUIEdit* LightEditPosX;
	ISXGUIStatic* LightStaticRot;
	ISXGUICheckBox* LightCheckBoxRotLightOrMesh;
	ISXGUIEdit* LightEditRotX;
	ISXGUIEdit* LightEditRotY;
	ISXGUIEdit* LightEditRotZ;
	ISXGUIRadioButton* LightRadioButtonRotX;
	ISXGUIRadioButton* LightRadioButtonRotY;
	ISXGUIRadioButton* LightRadioButtonRotZ;
	ISXGUICheckBox* LightCheckBoxDirOrRot;
	ISXGUIStatic* LightStaticCubeEdge;
	ISXGUICheckBox* LightCheckBoxCubeEdgeXPos;
	ISXGUICheckBox* LightCheckBoxCubeEdgeYPos;
	ISXGUICheckBox* LightCheckBoxCubeEdgeZPos;
	ISXGUICheckBox* LightCheckBoxCubeEdgeXNeg;
	ISXGUICheckBox* LightCheckBoxCubeEdgeYNeg;
	ISXGUICheckBox* LightCheckBoxCubeEdgeZNeg;
	ISXGUIStatic* LightStaticColor;
	ISXGUIEdit* LightEditColorR;
	ISXGUIEdit* LightEditColorG;
	ISXGUIEdit* LightEditColorB;
	ISXGUIStatic* LightStaticPower;
	ISXGUIEdit* LightEditPower;
	ISXGUIStatic* LightStaticDist;
	ISXGUIEdit* LightEditDist;
	ISXGUIButton* LightButtonColorSel;
	ISXGUIButton* LightButtonFinish;
	ISXGUIStatic* LightStaticColorR;
	ISXGUIStatic* LightStaticColorG;
	ISXGUIStatic* LightStaticColorB;
	ISXGUIStatic* LightStaticTopRadius;
	ISXGUIEdit* LightEditTopRadius;
	ISXGUIEdit* LightEditBias;
	ISXGUIStatic* LightStaticAngle;
	ISXGUIEdit* LightEditAngle;

	ISXGUIStatic* LightStaticSourceMesh;
	ISXGUIEdit* LightEditSourceMesh;
	ISXGUIButton* LightButtonSourceMesh;
	ISXGUIComboBox* LightComboBoxSourceMesh;

	void InitAllElements();

	void DeleteAllElements();
	void FinalImageUncheckedMenu();
	//int HowActivateType = 0;	
};

#include <SXLevelEditor/light_callback.cpp>
#include <SXLevelEditor/green_callback.cpp>
#include <SXLevelEditor/model_callback.cpp>
#include <SXLevelEditor/common_callback.cpp>

void SXLevelEditor::InitAllElements()
{
	SXLevelEditor::JobWindow = SXGUICrBaseWnd("JobWindow","SX Level editor",0,0,256,71,820,620,0,0,CreateSolidBrush(RGB(220,220,220)),0,CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,0,WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(SXLevelEditor::JobWindow);

	SXLevelEditor::JobWindow->AddHandler(ComMenuId, WM_COMMAND);
	SXLevelEditor::JobWindow->AddHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXLevelEditor::JobWindow->AddHandler(MsgEditSize, WM_SIZE);
	SXLevelEditor::JobWindow->MinSizeX = 820;
	SXLevelEditor::JobWindow->MinSizeY = 620;
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

	SXLevelEditor::RenderWindow = SXGUICrBaseWnd("RenderWindow","RenderWindow",0,0,0,25,600,400,0,0,CreateSolidBrush(RGB(200,200,200)),0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,SXLevelEditor::JobWindow->GetHWND(),0);
	SXLevelEditor::RenderWindow->GAlign.left = true;
	SXLevelEditor::RenderWindow->GAlign.right = true;
	SXLevelEditor::RenderWindow->GAlign.top = true;
	SXLevelEditor::RenderWindow->GAlign.bottom = true;

	SXLevelEditor::ToolBar1 = SXGUICrToolBar(0,1,810,24,SXLevelEditor::JobWindow->GetHWND(),0,0);
	SXLevelEditor::ToolBar1->GAlign.left = true;
	SXLevelEditor::ToolBar1->GAlign.right = true;
	SXLevelEditor::ToolBar1->GAlign.top = true;
	SXLevelEditor::ToolBar1->GAlign.bottom = false;
	
	SXLevelEditor::GroupBoxList = SXGUICrGroupBox("GroupBoxList", 601, 26, 200, 400, SXLevelEditor::JobWindow->GetHWND(), WndProcAllDefault, 0);
	SXGUIBaseHandlers::InitHandlerMsg(SXLevelEditor::GroupBoxList);
	
	SXLevelEditor::GroupBoxList->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::GroupBoxList->SetColorText(0,0,0);
	SXLevelEditor::GroupBoxList->SetColorTextBk(220,220,220);
	SXLevelEditor::GroupBoxList->SetTransparentTextBk(true);
	SXLevelEditor::GroupBoxList->SetColorBrush(220,220,220);

	SXLevelEditor::GroupBoxList->GAlign.left = false;
	SXLevelEditor::GroupBoxList->GAlign.right = true;
	SXLevelEditor::GroupBoxList->GAlign.top = true;
	SXLevelEditor::GroupBoxList->GAlign.bottom = true;

	SXLevelEditor::GroupBoxData = SXGUICrGroupBox("GroupBoxData", 0, 425, 800, 120, SXLevelEditor::JobWindow->GetHWND(), WndProcAllDefault, 0);
	SXGUIBaseHandlers::InitHandlerMsg(SXLevelEditor::GroupBoxData);
	SXLevelEditor::GroupBoxData->AddHandler(SXLevelEditor_GroupBoxLight_CallWmCommand, WM_COMMAND);
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
	SXLevelEditor::ListBoxList->AddHandler(SXLevelEditor_ListBoxList_Click, WM_LBUTTONUP);

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

	SXLevelEditor::ButtonLightOpen = SXGUICrButton("Light", 135, 350, 60, 20, 0, SXLevelEditor::GroupBoxList->GetHWND(), 0, 0);
	SXLevelEditor::ButtonLightOpen->GAlign.left = true;
	SXLevelEditor::ButtonLightOpen->GAlign.top = true;
	SXLevelEditor::ButtonLightOpen->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::ButtonLightOpen->AddHandler(SXLevelEditor_ButtonLightOpen_Click, WM_LBUTTONUP);

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

	////
	SXLevelEditor::StaticName = SXGUICrStatic("Name", 5, 20, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticName->SetColorText(0, 0, 0);
	SXLevelEditor::StaticName->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticName->SetTransparentTextBk(true);
	SXLevelEditor::StaticName->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticName->GAlign.left = true;
	SXLevelEditor::StaticName->GAlign.top = true;
	SXLevelEditor::StaticName->Visible(false);

	SXLevelEditor::StaticModel = SXGUICrStatic("Model", 5, 40, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticModel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticModel->SetColorText(0, 0, 0);
	SXLevelEditor::StaticModel->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticModel->SetTransparentTextBk(true);
	SXLevelEditor::StaticModel->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticModel->GAlign.left = true;
	SXLevelEditor::StaticModel->GAlign.top = true;
	SXLevelEditor::StaticModel->Visible(false);

	SXLevelEditor::StaticLod1 = SXGUICrStatic("Lod1", 5, 60, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticLod1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticLod1->SetColorText(0, 0, 0);
	SXLevelEditor::StaticLod1->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticLod1->SetTransparentTextBk(true);
	SXLevelEditor::StaticLod1->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticLod1->GAlign.left = true;
	SXLevelEditor::StaticLod1->GAlign.top = true;
	SXLevelEditor::StaticLod1->Visible(false);

	SXLevelEditor::EditName = SXGUICrEdit("0", 80, 20, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditName->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditName->SetColorText(0, 0, 0);
	SXLevelEditor::EditName->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditName->SetTransparentTextBk(true);
	SXLevelEditor::EditName->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditName->GAlign.left = true;
	SXLevelEditor::EditName->GAlign.top = true;
	SXLevelEditor::EditName->Visible(false);
	SXLevelEditor::EditName->AddHandler(SXLevelEditor_EditName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::EditModel = SXGUICrEdit("0", 80, 40, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditModel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditModel->SetColorText(0, 0, 0);
	SXLevelEditor::EditModel->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditModel->SetTransparentTextBk(true);
	SXLevelEditor::EditModel->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditModel->GAlign.left = true;
	SXLevelEditor::EditModel->GAlign.top = true;
	SXLevelEditor::EditModel->Visible(false);

	SXLevelEditor::EditLod1 = SXGUICrEdit("0", 80, 60, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditLod1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditLod1->SetColorText(0, 0, 0);
	SXLevelEditor::EditLod1->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditLod1->SetTransparentTextBk(true);
	SXLevelEditor::EditLod1->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditLod1->GAlign.left = true;
	SXLevelEditor::EditLod1->GAlign.top = true;
	SXLevelEditor::EditLod1->Visible(false);

	SXLevelEditor::ButtonLod1 = SXGUICrButton("...", 285, 60, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonLod1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonLod1->GAlign.left = true;
	SXLevelEditor::ButtonLod1->GAlign.top = true;
	SXLevelEditor::ButtonLod1->Visible(false);
	SXLevelEditor::ButtonLod1->AddHandler(SXLevelEditor_ButtonLod1_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonModel = SXGUICrButton("...", 285, 40, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonModel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonModel->GAlign.left = true;
	SXLevelEditor::ButtonModel->GAlign.top = true;
	SXLevelEditor::ButtonModel->Visible(false);
	SXLevelEditor::ButtonModel->AddHandler(SXLevelEditor_ButtonModel_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticPos = SXGUICrStatic("Position:", 320, 35, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticPos->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticPos->SetColorText(0, 0, 0);
	SXLevelEditor::StaticPos->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticPos->SetTransparentTextBk(true);
	SXLevelEditor::StaticPos->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticPos->GAlign.left = true;
	SXLevelEditor::StaticPos->GAlign.top = true;
	SXLevelEditor::StaticPos->Visible(false);

	SXLevelEditor::EditPosX = SXGUICrEdit("0", 370, 15, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditPosX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditPosX->SetColorText(0, 0, 0);
	SXLevelEditor::EditPosX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditPosX->SetTransparentTextBk(true);
	SXLevelEditor::EditPosX->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditPosX->GAlign.left = true;
	SXLevelEditor::EditPosX->GAlign.top = true;
	SXLevelEditor::EditPosX->Visible(false);
	SXLevelEditor::EditPosX->AddHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::EditPosY = SXGUICrEdit("0", 370, 35, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditPosY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditPosY->SetColorText(0, 0, 0);
	SXLevelEditor::EditPosY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditPosY->SetTransparentTextBk(true);
	SXLevelEditor::EditPosY->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditPosY->GAlign.left = true;
	SXLevelEditor::EditPosY->GAlign.top = true;
	SXLevelEditor::EditPosY->Visible(false);
	SXLevelEditor::EditPosY->AddHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::EditPosZ = SXGUICrEdit("0", 370, 55, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditPosZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditPosZ->SetColorText(0, 0, 0);
	SXLevelEditor::EditPosZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditPosZ->SetTransparentTextBk(true);
	SXLevelEditor::EditPosZ->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditPosZ->GAlign.left = true;
	SXLevelEditor::EditPosZ->GAlign.top = true;
	SXLevelEditor::EditPosZ->Visible(false);
	SXLevelEditor::EditPosZ->AddHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::RadioButtonPosX = SXGUICrRadioButton("x", 440, 15, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonPosX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonPosX->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonPosX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonPosX->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonPosX->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonPosX->GAlign.left = true;
	SXLevelEditor::RadioButtonPosX->GAlign.top = true;
	SXLevelEditor::RadioButtonPosX->Visible(false);

	SXLevelEditor::RadioButtonPosY = SXGUICrRadioButton("y", 440, 35, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonPosY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonPosY->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonPosY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonPosY->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonPosY->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonPosY->GAlign.left = true;
	SXLevelEditor::RadioButtonPosY->GAlign.top = true;
	SXLevelEditor::RadioButtonPosY->Visible(false);

	SXLevelEditor::RadioButtonPosZ = SXGUICrRadioButton("z", 440, 55, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonPosZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonPosZ->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonPosZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonPosZ->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonPosZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonPosZ->GAlign.left = true;
	SXLevelEditor::RadioButtonPosZ->GAlign.top = true;
	SXLevelEditor::RadioButtonPosZ->Visible(false);

	SXLevelEditor::StaticRot = SXGUICrStatic("Rotation:", 470, 35, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticRot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticRot->SetColorText(0, 0, 0);
	SXLevelEditor::StaticRot->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticRot->SetTransparentTextBk(true);
	SXLevelEditor::StaticRot->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticRot->GAlign.left = true;
	SXLevelEditor::StaticRot->GAlign.top = true;
	SXLevelEditor::StaticRot->Visible(false);

	SXLevelEditor::EditRotX = SXGUICrEdit("0", 525, 15, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditRotX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditRotX->SetColorText(0, 0, 0);
	SXLevelEditor::EditRotX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditRotX->SetTransparentTextBk(true);
	SXLevelEditor::EditRotX->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditRotX->GAlign.left = true;
	SXLevelEditor::EditRotX->GAlign.top = true;
	SXLevelEditor::EditRotX->Visible(false);
	SXLevelEditor::EditRotX->AddHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::EditRotY = SXGUICrEdit("0", 525, 35, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditRotY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditRotY->SetColorText(0, 0, 0);
	SXLevelEditor::EditRotY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditRotY->SetTransparentTextBk(true);
	SXLevelEditor::EditRotY->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditRotY->GAlign.left = true;
	SXLevelEditor::EditRotY->GAlign.top = true;
	SXLevelEditor::EditRotY->Visible(false);
	SXLevelEditor::EditRotY->AddHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::EditRotZ = SXGUICrEdit("0", 525, 55, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditRotZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditRotZ->SetColorText(0, 0, 0);
	SXLevelEditor::EditRotZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditRotZ->SetTransparentTextBk(true);
	SXLevelEditor::EditRotZ->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditRotZ->GAlign.left = true;
	SXLevelEditor::EditRotZ->GAlign.top = true;
	SXLevelEditor::EditRotZ->Visible(false);
	SXLevelEditor::EditRotZ->AddHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::RadioButtonRotX = SXGUICrRadioButton("x", 595, 15, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonRotX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonRotX->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonRotX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonRotX->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonRotX->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonRotX->GAlign.left = true;
	SXLevelEditor::RadioButtonRotX->GAlign.top = true;
	SXLevelEditor::RadioButtonRotX->Visible(false);

	SXLevelEditor::RadioButtonRotY = SXGUICrRadioButton("y", 595, 35, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonRotY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonRotY->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonRotY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonRotY->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonRotY->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonRotY->GAlign.left = true;
	SXLevelEditor::RadioButtonRotY->GAlign.top = true;
	SXLevelEditor::RadioButtonRotY->Visible(false);

	SXLevelEditor::RadioButtonRotZ = SXGUICrRadioButton("z", 595, 55, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonRotZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonRotZ->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonRotZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonRotZ->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonRotZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonRotZ->GAlign.left = true;
	SXLevelEditor::RadioButtonRotZ->GAlign.top = true;
	SXLevelEditor::RadioButtonRotZ->Visible(false);

	SXLevelEditor::StaticScale = SXGUICrStatic("Scale:", 625, 35, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticScale->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticScale->SetColorText(0, 0, 0);
	SXLevelEditor::StaticScale->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticScale->SetTransparentTextBk(true);
	SXLevelEditor::StaticScale->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticScale->GAlign.left = true;
	SXLevelEditor::StaticScale->GAlign.top = true;
	SXLevelEditor::StaticScale->Visible(false);

	SXLevelEditor::EditScaleX = SXGUICrEdit("0", 675, 15, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditScaleX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditScaleX->SetColorText(0, 0, 0);
	SXLevelEditor::EditScaleX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditScaleX->SetTransparentTextBk(true);
	SXLevelEditor::EditScaleX->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditScaleX->GAlign.left = true;
	SXLevelEditor::EditScaleX->GAlign.top = true;
	SXLevelEditor::EditScaleX->Visible(false);
	SXLevelEditor::EditScaleX->AddHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::EditScaleY = SXGUICrEdit("0", 675, 35, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditScaleY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditScaleY->SetColorText(0, 0, 0);
	SXLevelEditor::EditScaleY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditScaleY->SetTransparentTextBk(true);
	SXLevelEditor::EditScaleY->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditScaleY->GAlign.left = true;
	SXLevelEditor::EditScaleY->GAlign.top = true;
	SXLevelEditor::EditScaleY->Visible(false);
	SXLevelEditor::EditScaleY->AddHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::EditScaleZ = SXGUICrEdit("0", 675, 55, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditScaleZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditScaleZ->SetColorText(0, 0, 0);
	SXLevelEditor::EditScaleZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditScaleZ->SetTransparentTextBk(true);
	SXLevelEditor::EditScaleZ->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditScaleZ->GAlign.left = true;
	SXLevelEditor::EditScaleZ->GAlign.top = true;
	SXLevelEditor::EditScaleZ->Visible(false);
	SXLevelEditor::EditScaleZ->AddHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::RadioButtonScaleX = SXGUICrRadioButton("x", 745, 15, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonScaleX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonScaleX->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonScaleX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonScaleX->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonScaleX->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonScaleX->GAlign.left = true;
	SXLevelEditor::RadioButtonScaleX->GAlign.top = true;
	SXLevelEditor::RadioButtonScaleX->Visible(false);

	SXLevelEditor::RadioButtonScaleY = SXGUICrRadioButton("y", 745, 35, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonScaleY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonScaleY->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonScaleY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonScaleY->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonScaleY->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonScaleY->GAlign.left = true;
	SXLevelEditor::RadioButtonScaleY->GAlign.top = true;
	SXLevelEditor::RadioButtonScaleY->Visible(false);

	SXLevelEditor::RadioButtonScaleZ = SXGUICrRadioButton("z", 745, 55, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::RadioButtonScaleZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonScaleZ->SetColorText(0, 0, 0);
	SXLevelEditor::RadioButtonScaleZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::RadioButtonScaleZ->SetTransparentTextBk(true);
	SXLevelEditor::RadioButtonScaleZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::RadioButtonScaleZ->GAlign.left = true;
	SXLevelEditor::RadioButtonScaleZ->GAlign.top = true;
	SXLevelEditor::RadioButtonScaleZ->Visible(false);

	/*SXLevelEditor::CheckBoxCG = SXGUICrCheckBox("Closed geometry", 100, 75, 100, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::CheckBoxCG->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxCG->SetColorText(0, 0, 0);
	SXLevelEditor::CheckBoxCG->SetColorTextBk(255, 255, 255);
	SXLevelEditor::CheckBoxCG->SetTransparentTextBk(true);
	SXLevelEditor::CheckBoxCG->SetColorBrush(220, 220, 220);
	SXLevelEditor::CheckBoxCG->GAlign.left = true;
	SXLevelEditor::CheckBoxCG->GAlign.top = true;
	SXLevelEditor::CheckBoxCG->Visible(false);*/

	SXLevelEditor::ButtonFinish = SXGUICrButton("Finish", 100, 95, 100, 20, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonFinish->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonFinish->GAlign.left = true;
	SXLevelEditor::ButtonFinish->GAlign.top = true;
	SXLevelEditor::ButtonFinish->Visible(false);
	SXLevelEditor::ButtonFinish->AddHandler(SXLevelEditor_ButtonFinish_Click, WM_LBUTTONUP);




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

	SXLevelEditor::TrackBarDensity = SXGUICrTrackBar("", 330, 65, 300, 20, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::TrackBarDensity->SetMinMax(1, 100);
	SXLevelEditor::TrackBarDensity->AddHandler(SXLevelEditor_TrackBarDensity_MouseMove, WM_MOUSEMOVE);
	SXLevelEditor::TrackBarDensity->GAlign.left = true;
	SXLevelEditor::TrackBarDensity->GAlign.top = true;
	SXLevelEditor::TrackBarDensity->Visible(false);

	SXLevelEditor::ButtonGenerate = SXGUICrButton("Generate", 430, 87, 100, 30, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGenerate->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGenerate->GAlign.left = true;
	SXLevelEditor::ButtonGenerate->GAlign.top = true;
	SXLevelEditor::ButtonGenerate->Visible(false);
	SXLevelEditor::ButtonGenerate->AddHandler(SXLevelEditor_ButtonGenerate_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticDensityText = SXGUICrStatic("Density:", 380, 45, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticDensityText->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticDensityText->SetColorText(0, 0, 0);
	SXLevelEditor::StaticDensityText->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticDensityText->SetTransparentTextBk(true);
	SXLevelEditor::StaticDensityText->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticDensityText->GAlign.left = true;
	SXLevelEditor::StaticDensityText->GAlign.top = true;
	SXLevelEditor::StaticDensityText->Visible(false);

	SXLevelEditor::StaticDensityVal = SXGUICrStatic("0", 430, 45, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticDensityVal->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticDensityVal->SetColorText(0, 0, 0);
	SXLevelEditor::StaticDensityVal->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticDensityVal->SetTransparentTextBk(true);
	SXLevelEditor::StaticDensityVal->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticDensityVal->GAlign.left = true;
	SXLevelEditor::StaticDensityVal->GAlign.top = true;
	SXLevelEditor::StaticDensityVal->Visible(false);

	SXLevelEditor::StaticLod2 = SXGUICrStatic("Lod2", 5, 80, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticLod2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticLod2->SetColorText(0, 0, 0);
	SXLevelEditor::StaticLod2->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticLod2->SetTransparentTextBk(true);
	SXLevelEditor::StaticLod2->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticLod2->GAlign.left = true;
	SXLevelEditor::StaticLod2->GAlign.top = true;
	SXLevelEditor::StaticLod2->Visible(false);

	SXLevelEditor::EditLod2 = SXGUICrEdit("0", 80, 80, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditLod2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditLod2->SetColorText(0, 0, 0);
	SXLevelEditor::EditLod2->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditLod2->SetTransparentTextBk(true);
	SXLevelEditor::EditLod2->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditLod2->GAlign.left = true;
	SXLevelEditor::EditLod2->GAlign.top = true;
	SXLevelEditor::EditLod2->Visible(false);

	SXLevelEditor::ButtonLod2 = SXGUICrButton("...", 285, 80, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonLod2->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonLod2->GAlign.left = true;
	SXLevelEditor::ButtonLod2->GAlign.top = true;
	SXLevelEditor::ButtonLod2->Visible(false);
	SXLevelEditor::ButtonLod2->AddHandler(SXLevelEditor_ButtonLod2_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonMask = SXGUICrButton("...", 285, 100, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonMask->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonMask->GAlign.left = true;
	SXLevelEditor::ButtonMask->GAlign.top = true;
	SXLevelEditor::ButtonMask->Visible(false);
	SXLevelEditor::ButtonMask->AddHandler(SXLevelEditor_ButtonMask_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticMask = SXGUICrStatic("Mask texture", 5, 100, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticMask->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticMask->SetColorText(0, 0, 0);
	SXLevelEditor::StaticMask->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticMask->SetTransparentTextBk(true);
	SXLevelEditor::StaticMask->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticMask->GAlign.left = true;
	SXLevelEditor::StaticMask->GAlign.top = true;
	SXLevelEditor::StaticMask->Visible(false);

	SXLevelEditor::EditMask = SXGUICrEdit("0", 80, 100, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::EditMask->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditMask->SetColorText(0, 0, 0);
	SXLevelEditor::EditMask->SetColorTextBk(255, 255, 255);
	SXLevelEditor::EditMask->SetTransparentTextBk(true);
	SXLevelEditor::EditMask->SetColorBrush(255, 255, 255);
	SXLevelEditor::EditMask->GAlign.left = true;
	SXLevelEditor::EditMask->GAlign.top = true;
	SXLevelEditor::EditMask->Visible(false);







	SXLevelEditor::LightStaticBias = SXGUICrStatic("Bias:", 80, 80, 30, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticBias->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticBias->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticBias->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticBias->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticBias->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticBias->GAlign.left = true;
	SXLevelEditor::LightStaticBias->GAlign.top = true;
	SXLevelEditor::LightStaticBias->Visible(false);

	SXLevelEditor::LightEditBias = SXGUICrEdit("0", 150, 80, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditBias->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditBias->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditBias->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditBias->SetTransparentTextBk(true);
	SXLevelEditor::LightEditBias->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditBias->GAlign.left = true;
	SXLevelEditor::LightEditBias->GAlign.top = true;
	SXLevelEditor::LightEditBias->Visible(false);
	SXLevelEditor::LightEditBias->AddHandler(SXLevelEditor_LightEditObject_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightCheckBoxShadow = SXGUICrCheckBox("Shadow", 5, 40, 60, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxShadow->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxShadow->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxShadow->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxShadow->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxShadow->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxShadow->GAlign.left = true;
	SXLevelEditor::LightCheckBoxShadow->GAlign.top = true;
	SXLevelEditor::LightCheckBoxShadow->Visible(false);

	SXLevelEditor::LightCheckBoxGlobal = SXGUICrCheckBox("Global", 5, 60, 60, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxGlobal->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxGlobal->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxGlobal->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxGlobal->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxGlobal->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxGlobal->GAlign.left = true;
	SXLevelEditor::LightCheckBoxGlobal->GAlign.top = true;
	SXLevelEditor::LightCheckBoxGlobal->Visible(false);

	SXLevelEditor::LightCheckBoxEnable = SXGUICrCheckBox("Enable", 5, 80, 60, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxEnable->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxEnable->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxEnable->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxEnable->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxEnable->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxEnable->GAlign.left = true;
	SXLevelEditor::LightCheckBoxEnable->GAlign.top = true;
	SXLevelEditor::LightCheckBoxEnable->Visible(false);

	SXLevelEditor::LightComboBoxTypeLight = SXGUICrComboBox("", 80, 38, 98, 70, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightComboBoxTypeLight->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightComboBoxTypeLight->SetColorText(0, 0, 0);
	SXLevelEditor::LightComboBoxTypeLight->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightComboBoxTypeLight->SetTransparentTextBk(true);
	SXLevelEditor::LightComboBoxTypeLight->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightComboBoxTypeLight->AddItem("point");
	SXLevelEditor::LightComboBoxTypeLight->AddItem("direction");
	SXLevelEditor::LightComboBoxTypeLight->SetSel(0);
	SXLevelEditor::LightComboBoxTypeLight->GAlign.left = true;
	SXLevelEditor::LightComboBoxTypeLight->GAlign.top = true;
	SXLevelEditor::LightComboBoxTypeLight->Visible(false);

	SXLevelEditor::LightComboBoxTypeLightShadow = SXGUICrComboBox("", 180, 38, 98, 70, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightComboBoxTypeLightShadow->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightComboBoxTypeLightShadow->SetColorText(0, 0, 0);
	SXLevelEditor::LightComboBoxTypeLightShadow->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightComboBoxTypeLightShadow->SetTransparentTextBk(true);
	SXLevelEditor::LightComboBoxTypeLightShadow->SetColorBrush(255, 255, 255);

	SXLevelEditor::LightComboBoxTypeLightShadow->AddItem("static");
	SXLevelEditor::LightComboBoxTypeLightShadow->AddItem("dynamic");
	SXLevelEditor::LightComboBoxTypeLightShadow->SetSel(0);

	SXLevelEditor::LightComboBoxTypeLightShadow->GAlign.left = true;
	SXLevelEditor::LightComboBoxTypeLightShadow->GAlign.top = true;
	SXLevelEditor::LightComboBoxTypeLightShadow->Visible(false);

	SXLevelEditor::LightStaticFarShadow = SXGUICrStatic("Far shadow:", 80, 62, 60, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticFarShadow->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticFarShadow->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticFarShadow->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticFarShadow->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticFarShadow->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticFarShadow->GAlign.left = true;
	SXLevelEditor::LightStaticFarShadow->GAlign.top = true;
	SXLevelEditor::LightStaticFarShadow->Visible(false);

	SXLevelEditor::LightEditFarShadow = SXGUICrEdit("0", 150, 62, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditFarShadow->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditFarShadow->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditFarShadow->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditFarShadow->SetTransparentTextBk(true);
	SXLevelEditor::LightEditFarShadow->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditFarShadow->GAlign.left = true;
	SXLevelEditor::LightEditFarShadow->GAlign.top = true;
	SXLevelEditor::LightEditFarShadow->Visible(false);
	SXLevelEditor::LightEditFarShadow->AddHandler(SXLevelEditor_LightEditObject_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightStaticPos = SXGUICrStatic("Position:", 290, 40, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticPos->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticPos->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticPos->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticPos->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticPos->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticPos->GAlign.left = true;
	SXLevelEditor::LightStaticPos->GAlign.top = true;
	SXLevelEditor::LightStaticPos->Visible(false);

	/*SXLevelEditor::LightCheckBoxPosLightOrMesh = SXGUICrCheckBox("mesh", 290, 60, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxPosLightOrMesh->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxPosLightOrMesh->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxPosLightOrMesh->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxPosLightOrMesh->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxPosLightOrMesh->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxPosLightOrMesh->GAlign.left = true;
	SXLevelEditor::LightCheckBoxPosLightOrMesh->GAlign.top = true;
	SXLevelEditor::LightCheckBoxPosLightOrMesh->Visible(false);*/

	SXLevelEditor::LightEditPosY = SXGUICrEdit("0", 340, 60, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditPosY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditPosY->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditPosY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditPosY->SetTransparentTextBk(true);
	SXLevelEditor::LightEditPosY->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditPosY->GAlign.left = true;
	SXLevelEditor::LightEditPosY->GAlign.top = true;
	SXLevelEditor::LightEditPosY->Visible(false);
	SXLevelEditor::LightEditPosY->AddHandler(SXLevelEditor_LightEditTransform_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightEditPosZ = SXGUICrEdit("0", 340, 80, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditPosZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditPosZ->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditPosZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditPosZ->SetTransparentTextBk(true);
	SXLevelEditor::LightEditPosZ->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditPosZ->GAlign.left = true;
	SXLevelEditor::LightEditPosZ->GAlign.top = true;
	SXLevelEditor::LightEditPosZ->Visible(false);
	SXLevelEditor::LightEditPosZ->AddHandler(SXLevelEditor_LightEditTransform_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightRadioButtonPosX = SXGUICrRadioButton("x", 410, 40, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightRadioButtonPosX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightRadioButtonPosX->SetColorText(0, 0, 0);
	SXLevelEditor::LightRadioButtonPosX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightRadioButtonPosX->SetTransparentTextBk(true);
	SXLevelEditor::LightRadioButtonPosX->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightRadioButtonPosX->GAlign.left = true;
	SXLevelEditor::LightRadioButtonPosX->GAlign.top = true;
	SXLevelEditor::LightRadioButtonPosX->Visible(false);

	SXLevelEditor::LightRadioButtonPosY = SXGUICrRadioButton("y", 410, 60, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightRadioButtonPosY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightRadioButtonPosY->SetColorText(0, 0, 0);
	SXLevelEditor::LightRadioButtonPosY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightRadioButtonPosY->SetTransparentTextBk(true);
	SXLevelEditor::LightRadioButtonPosY->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightRadioButtonPosY->GAlign.left = true;
	SXLevelEditor::LightRadioButtonPosY->GAlign.top = true;
	SXLevelEditor::LightRadioButtonPosY->Visible(false);

	SXLevelEditor::LightRadioButtonPosZ = SXGUICrRadioButton("z", 410, 80, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightRadioButtonPosZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightRadioButtonPosZ->SetColorText(0, 0, 0);
	SXLevelEditor::LightRadioButtonPosZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightRadioButtonPosZ->SetTransparentTextBk(true);
	SXLevelEditor::LightRadioButtonPosZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightRadioButtonPosZ->GAlign.left = true;
	SXLevelEditor::LightRadioButtonPosZ->GAlign.top = true;
	SXLevelEditor::LightRadioButtonPosZ->Visible(false);

	SXLevelEditor::LightEditPosX = SXGUICrEdit("0", 340, 40, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditPosX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditPosX->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditPosX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditPosX->SetTransparentTextBk(true);
	SXLevelEditor::LightEditPosX->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditPosX->GAlign.left = true;
	SXLevelEditor::LightEditPosX->GAlign.top = true;
	SXLevelEditor::LightEditPosX->Visible(false);
	SXLevelEditor::LightEditPosX->AddHandler(SXLevelEditor_LightEditTransform_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightStaticRot = SXGUICrStatic("Rotation:", 440, 40, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticRot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticRot->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticRot->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticRot->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticRot->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticRot->GAlign.left = true;
	SXLevelEditor::LightStaticRot->GAlign.top = true;
	SXLevelEditor::LightStaticRot->Visible(false);

	SXLevelEditor::LightCheckBoxRotLightOrMesh = SXGUICrCheckBox("mesh", 440, 60, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxRotLightOrMesh->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxRotLightOrMesh->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxRotLightOrMesh->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxRotLightOrMesh->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxRotLightOrMesh->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxRotLightOrMesh->GAlign.left = true;
	SXLevelEditor::LightCheckBoxRotLightOrMesh->GAlign.top = true;
	SXLevelEditor::LightCheckBoxRotLightOrMesh->Visible(false);

	SXLevelEditor::LightCheckBoxDirOrRot = SXGUICrCheckBox("direction", 440, 80, 60, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxDirOrRot->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxDirOrRot->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxDirOrRot->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxDirOrRot->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxDirOrRot->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxDirOrRot->GAlign.left = true;
	SXLevelEditor::LightCheckBoxDirOrRot->GAlign.top = true;
	SXLevelEditor::LightCheckBoxDirOrRot->Visible(false);

	SXLevelEditor::LightEditRotX = SXGUICrEdit("0", 500, 40, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditRotX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditRotX->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditRotX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditRotX->SetTransparentTextBk(true);
	SXLevelEditor::LightEditRotX->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditRotX->GAlign.left = true;
	SXLevelEditor::LightEditRotX->GAlign.top = true;
	SXLevelEditor::LightEditRotX->Visible(false);
	SXLevelEditor::LightEditRotX->AddHandler(SXLevelEditor_LightEditTransform_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightEditRotY = SXGUICrEdit("0", 500, 60, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditRotY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditRotY->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditRotY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditRotY->SetTransparentTextBk(true);
	SXLevelEditor::LightEditRotY->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditRotY->GAlign.left = true;
	SXLevelEditor::LightEditRotY->GAlign.top = true;
	SXLevelEditor::LightEditRotY->Visible(false);
	SXLevelEditor::LightEditRotY->AddHandler(SXLevelEditor_LightEditTransform_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightEditRotZ = SXGUICrEdit("0", 500, 80, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditRotZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditRotZ->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditRotZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditRotZ->SetTransparentTextBk(true);
	SXLevelEditor::LightEditRotZ->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditRotZ->GAlign.left = true;
	SXLevelEditor::LightEditRotZ->GAlign.top = true;
	SXLevelEditor::LightEditRotZ->Visible(false);
	SXLevelEditor::LightEditRotZ->AddHandler(SXLevelEditor_LightEditTransform_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightRadioButtonRotX = SXGUICrRadioButton("x", 570, 40, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightRadioButtonRotX->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightRadioButtonRotX->SetColorText(0, 0, 0);
	SXLevelEditor::LightRadioButtonRotX->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightRadioButtonRotX->SetTransparentTextBk(true);
	SXLevelEditor::LightRadioButtonRotX->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightRadioButtonRotX->GAlign.left = true;
	SXLevelEditor::LightRadioButtonRotX->GAlign.top = true;
	SXLevelEditor::LightRadioButtonRotX->Visible(false);

	SXLevelEditor::LightRadioButtonRotY = SXGUICrRadioButton("y", 570, 60, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightRadioButtonRotY->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightRadioButtonRotY->SetColorText(0, 0, 0);
	SXLevelEditor::LightRadioButtonRotY->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightRadioButtonRotY->SetTransparentTextBk(true);
	SXLevelEditor::LightRadioButtonRotY->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightRadioButtonRotY->GAlign.left = true;
	SXLevelEditor::LightRadioButtonRotY->GAlign.top = true;
	SXLevelEditor::LightRadioButtonRotY->Visible(false);

	SXLevelEditor::LightRadioButtonRotZ = SXGUICrRadioButton("z", 570, 80, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightRadioButtonRotZ->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightRadioButtonRotZ->SetColorText(0, 0, 0);
	SXLevelEditor::LightRadioButtonRotZ->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightRadioButtonRotZ->SetTransparentTextBk(true);
	SXLevelEditor::LightRadioButtonRotZ->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightRadioButtonRotZ->GAlign.left = true;
	SXLevelEditor::LightRadioButtonRotZ->GAlign.top = true;
	SXLevelEditor::LightRadioButtonRotZ->Visible(false);

	SXLevelEditor::LightStaticCubeEdge = SXGUICrStatic("Cube edge:", 230, 65, 60, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticCubeEdge->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticCubeEdge->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticCubeEdge->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticCubeEdge->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticCubeEdge->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticCubeEdge->GAlign.left = true;
	SXLevelEditor::LightStaticCubeEdge->GAlign.top = true;
	SXLevelEditor::LightStaticCubeEdge->Visible(false);

	SXLevelEditor::LightCheckBoxCubeEdgeXPos = SXGUICrCheckBox("x+", 230, 80, 30, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxCubeEdgeXPos->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeXPos->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeXPos->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxCubeEdgeXPos->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxCubeEdgeXPos->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxCubeEdgeXPos->GAlign.left = true;
	SXLevelEditor::LightCheckBoxCubeEdgeXPos->GAlign.top = true;
	SXLevelEditor::LightCheckBoxCubeEdgeXPos->Visible(false);

	SXLevelEditor::LightCheckBoxCubeEdgeYPos = SXGUICrCheckBox("y+", 265, 80, 30, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->GAlign.left = true;
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->GAlign.top = true;
	SXLevelEditor::LightCheckBoxCubeEdgeYPos->Visible(false);

	SXLevelEditor::LightCheckBoxCubeEdgeZPos = SXGUICrCheckBox("z+", 300, 80, 30, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->GAlign.left = true;
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->GAlign.top = true;
	SXLevelEditor::LightCheckBoxCubeEdgeZPos->Visible(false);

	SXLevelEditor::LightCheckBoxCubeEdgeXNeg = SXGUICrCheckBox("x-", 230, 100, 30, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->GAlign.left = true;
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->GAlign.top = true;
	SXLevelEditor::LightCheckBoxCubeEdgeXNeg->Visible(false);

	SXLevelEditor::LightCheckBoxCubeEdgeYNeg = SXGUICrCheckBox("y-", 265, 100, 30, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->GAlign.left = true;
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->GAlign.top = true;
	SXLevelEditor::LightCheckBoxCubeEdgeYNeg->Visible(false);

	SXLevelEditor::LightCheckBoxCubeEdgeZNeg = SXGUICrCheckBox("z-", 300, 100, 30, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0, false);
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->SetColorText(0, 0, 0);
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->SetTransparentTextBk(true);
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->GAlign.left = true;
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->GAlign.top = true;
	SXLevelEditor::LightCheckBoxCubeEdgeZNeg->Visible(false);

	SXLevelEditor::LightStaticColor = SXGUICrStatic("Color:", 600, 60, 30, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticColor->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticColor->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticColor->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticColor->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticColor->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticColor->GAlign.left = true;
	SXLevelEditor::LightStaticColor->GAlign.top = true;
	SXLevelEditor::LightStaticColor->Visible(false);

	SXLevelEditor::LightEditColorR = SXGUICrEdit("0", 650, 40, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditColorR->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditColorR->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditColorR->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditColorR->SetTransparentTextBk(true);
	SXLevelEditor::LightEditColorR->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditColorR->GAlign.left = true;
	SXLevelEditor::LightEditColorR->GAlign.top = true;
	SXLevelEditor::LightEditColorR->Visible(false);
	SXLevelEditor::LightEditColorR->AddHandler(SXLevelEditor_LightEditColor_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightEditColorG = SXGUICrEdit("0", 650, 60, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditColorG->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditColorG->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditColorG->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditColorG->SetTransparentTextBk(true);
	SXLevelEditor::LightEditColorG->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditColorG->GAlign.left = true;
	SXLevelEditor::LightEditColorG->GAlign.top = true;
	SXLevelEditor::LightEditColorG->Visible(false);
	SXLevelEditor::LightEditColorG->AddHandler(SXLevelEditor_LightEditColor_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightEditColorB = SXGUICrEdit("0", 650, 80, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditColorB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditColorB->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditColorB->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditColorB->SetTransparentTextBk(true);
	SXLevelEditor::LightEditColorB->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditColorB->GAlign.left = true;
	SXLevelEditor::LightEditColorB->GAlign.top = true;
	SXLevelEditor::LightEditColorB->Visible(false);
	SXLevelEditor::LightEditColorB->AddHandler(SXLevelEditor_LightEditColor_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightStaticPower = SXGUICrStatic("Power:", 5, 100, 40, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticPower->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticPower->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticPower->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticPower->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticPower->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticPower->GAlign.left = true;
	SXLevelEditor::LightStaticPower->GAlign.top = true;
	SXLevelEditor::LightStaticPower->Visible(false);

	SXLevelEditor::LightEditPower = SXGUICrEdit("0", 40, 100, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditPower->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditPower->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditPower->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditPower->SetTransparentTextBk(true);
	SXLevelEditor::LightEditPower->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditPower->GAlign.left = true;
	SXLevelEditor::LightEditPower->GAlign.top = true;
	SXLevelEditor::LightEditPower->Visible(false);
	SXLevelEditor::LightEditPower->AddHandler(SXLevelEditor_LightEditObject_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);


	SXLevelEditor::LightStaticDist = SXGUICrStatic("Dist:", 120, 100, 25, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticDist->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticDist->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticDist->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticDist->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticDist->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticDist->GAlign.left = true;
	SXLevelEditor::LightStaticDist->GAlign.top = true;
	SXLevelEditor::LightStaticDist->Visible(false);

	SXLevelEditor::LightEditDist = SXGUICrEdit("0", 150, 100, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditDist->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditDist->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditDist->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditDist->SetTransparentTextBk(true);
	SXLevelEditor::LightEditDist->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditDist->GAlign.left = true;
	SXLevelEditor::LightEditDist->GAlign.top = true;
	SXLevelEditor::LightEditDist->Visible(false);
	SXLevelEditor::LightEditDist->AddHandler(SXLevelEditor_LightEditObject_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);


	SXLevelEditor::LightButtonColorSel = SXGUICrButton("...", 725, 48, 20, 40, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightButtonColorSel->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightButtonColorSel->GAlign.left = true;
	SXLevelEditor::LightButtonColorSel->GAlign.top = true;
	SXLevelEditor::LightButtonColorSel->Visible(false);
	SXLevelEditor::LightButtonColorSel->AddHandler(SXLevelEditor_LightButtonColorSel_Click, WM_LBUTTONUP);

	SXLevelEditor::LightStaticColorR = SXGUICrStatic("r:", 635, 40, 10, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticColorR->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticColorR->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticColorR->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticColorR->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticColorR->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticColorR->GAlign.left = true;
	SXLevelEditor::LightStaticColorR->GAlign.top = true;
	SXLevelEditor::LightStaticColorR->Visible(false);

	SXLevelEditor::LightStaticColorG = SXGUICrStatic("g:", 635, 60, 10, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticColorG->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticColorG->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticColorG->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticColorG->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticColorG->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticColorG->GAlign.left = true;
	SXLevelEditor::LightStaticColorG->GAlign.top = true;
	SXLevelEditor::LightStaticColorG->Visible(false);

	SXLevelEditor::LightStaticColorB = SXGUICrStatic("b:", 635, 80, 10, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticColorB->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticColorB->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticColorB->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticColorB->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticColorB->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticColorB->GAlign.left = true;
	SXLevelEditor::LightStaticColorB->GAlign.top = true;
	SXLevelEditor::LightStaticColorB->Visible(false);

	SXLevelEditor::LightStaticTopRadius = SXGUICrStatic("Top radius:", 440, 100, 60, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticTopRadius->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticTopRadius->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticTopRadius->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticTopRadius->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticTopRadius->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticTopRadius->GAlign.left = true;
	SXLevelEditor::LightStaticTopRadius->GAlign.top = true;
	SXLevelEditor::LightStaticTopRadius->Visible(false);

	SXLevelEditor::LightEditTopRadius = SXGUICrEdit("0", 500, 100, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditTopRadius->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditTopRadius->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditTopRadius->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditTopRadius->SetTransparentTextBk(true);
	SXLevelEditor::LightEditTopRadius->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditTopRadius->GAlign.left = true;
	SXLevelEditor::LightEditTopRadius->GAlign.top = true;
	SXLevelEditor::LightEditTopRadius->Visible(false);
	SXLevelEditor::LightEditTopRadius->AddHandler(SXLevelEditor_LightEditObject_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightStaticAngle = SXGUICrStatic("Angle:", 590, 100, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticAngle->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticAngle->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticAngle->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticAngle->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticAngle->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticAngle->GAlign.left = true;
	SXLevelEditor::LightStaticAngle->GAlign.top = true;
	SXLevelEditor::LightStaticAngle->Visible(false);

	SXLevelEditor::LightEditAngle = SXGUICrEdit("0", 650, 100, 70, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditAngle->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditAngle->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditAngle->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditAngle->SetTransparentTextBk(true);
	SXLevelEditor::LightEditAngle->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditAngle->GAlign.left = true;
	SXLevelEditor::LightEditAngle->GAlign.top = true;
	SXLevelEditor::LightEditAngle->Visible(false);
	SXLevelEditor::LightEditAngle->AddHandler(SXLevelEditor_LightEditObject_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightButtonFinish = SXGUICrButton("Finish", 729, 95, 66, 20, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightButtonFinish->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightButtonFinish->GAlign.left = true;
	SXLevelEditor::LightButtonFinish->GAlign.top = true;
	SXLevelEditor::LightButtonFinish->Visible(false);
	SXLevelEditor::LightButtonFinish->AddHandler(SXLevelEditor_LightButtonFinish_Click, WM_LBUTTONUP);


	SXLevelEditor::LightStaticSourceMesh = SXGUICrStatic("Source:", 290, 20, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightStaticSourceMesh->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightStaticSourceMesh->SetColorText(0, 0, 0);
	SXLevelEditor::LightStaticSourceMesh->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightStaticSourceMesh->SetTransparentTextBk(true);
	SXLevelEditor::LightStaticSourceMesh->SetColorBrush(220, 220, 220);
	SXLevelEditor::LightStaticSourceMesh->GAlign.left = true;
	SXLevelEditor::LightStaticSourceMesh->GAlign.top = true;
	SXLevelEditor::LightStaticSourceMesh->Visible(false);

	SXLevelEditor::LightEditSourceMesh = SXGUICrEdit("0", 340, 20, 200, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightEditSourceMesh->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightEditSourceMesh->SetColorText(0, 0, 0);
	SXLevelEditor::LightEditSourceMesh->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightEditSourceMesh->SetTransparentTextBk(true);
	SXLevelEditor::LightEditSourceMesh->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightEditSourceMesh->GAlign.left = true;
	SXLevelEditor::LightEditSourceMesh->GAlign.top = true;
	SXLevelEditor::LightEditSourceMesh->Visible(false);
	//SXLevelEditor::LightEditSourceMesh->AddHandler(SXLevelEditor_LightEditObject_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXLevelEditor::LightButtonSourceMesh = SXGUICrButton("...", 545, 20, 25, 15, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightButtonSourceMesh->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightButtonSourceMesh->GAlign.left = true;
	SXLevelEditor::LightButtonSourceMesh->GAlign.top = true;
	SXLevelEditor::LightButtonSourceMesh->Visible(false);
	SXLevelEditor::LightButtonSourceMesh->AddHandler(SXLevelEditor_LightButtonSourceMesh_Click, WM_LBUTTONUP);

	SXLevelEditor::LightComboBoxSourceMesh = SXGUICrComboBox("", 580, 17, 200, 70, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::LightComboBoxSourceMesh->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::LightComboBoxSourceMesh->SetColorText(0, 0, 0);
	SXLevelEditor::LightComboBoxSourceMesh->SetColorTextBk(255, 255, 255);
	SXLevelEditor::LightComboBoxSourceMesh->SetTransparentTextBk(true);
	SXLevelEditor::LightComboBoxSourceMesh->SetColorBrush(255, 255, 255);
	SXLevelEditor::LightComboBoxSourceMesh->GAlign.left = true;
	SXLevelEditor::LightComboBoxSourceMesh->GAlign.top = true;
	SXLevelEditor::LightComboBoxSourceMesh->Visible(false);

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
	mem_release(Button5);

	mem_release(ButtonGeometryOpen);
	mem_release(ButtonGreenOpen);
	mem_release(ButtonLightOpen);


	//model
	//{{
	mem_release(StaticName);
	mem_release(StaticModel);
	mem_release(StaticLod1);
	mem_release(StaticLod2);
	mem_release(EditName);
	mem_release(EditModel);
	mem_release(EditLod1);
	mem_release(EditLod2);
	mem_release(ButtonLod1);
	mem_release(ButtonModel);
	mem_release(ButtonLod2);
	mem_release(ButtonMask);
	mem_release(StaticMask);
	mem_release(EditMask);
	mem_release(StatusBar1);
	mem_release(StaticPos);
	mem_release(EditPosX);
	mem_release(EditPosY);
	mem_release(EditPosZ);
	mem_release(RadioButtonPosX);
	mem_release(RadioButtonPosY);
	mem_release(RadioButtonPosZ);
	mem_release(StaticRot);
	mem_release(EditRotX);
	mem_release(EditRotY);
	mem_release(EditRotZ);
	mem_release(RadioButtonRotX);
	mem_release(RadioButtonRotY);
	mem_release(RadioButtonRotZ);
	mem_release(StaticScale);
	mem_release(EditScaleX);
	mem_release(EditScaleY);
	mem_release(EditScaleZ);
	mem_release(RadioButtonScaleX);
	mem_release(RadioButtonScaleY);
	mem_release(RadioButtonScaleZ);
	//ISXGUICheckBox* CheckBoxCG;
	mem_release(ButtonFinish);
	//}}

	mem_release(StaticGreenNav);
	mem_release(EditGreenNav);
	mem_release(ButtonGreenNav);

	mem_release(TrackBarDensity);
	mem_release(ButtonGenerate);
	mem_release(StaticDensityText);
	mem_release(StaticDensityVal);


	///
	mem_release(LightStaticBias);
	mem_release(LightCheckBoxShadow);
	mem_release(LightCheckBoxGlobal);
	mem_release(LightCheckBoxEnable);
	mem_release(LightComboBoxTypeLight);
	mem_release(LightComboBoxTypeLightShadow);
	mem_release(LightStaticFarShadow);
	mem_release(LightEditFarShadow);
	mem_release(LightStaticPos);
	//ISXGUICheckBox* LightCheckBoxPosLightOrMesh;
	mem_release(LightEditPosY);
	mem_release(LightEditPosZ);
	mem_release(LightRadioButtonPosX);
	mem_release(LightRadioButtonPosY);
	mem_release(LightRadioButtonPosZ);
	mem_release(LightEditPosX);
	mem_release(LightStaticRot);
	mem_release(LightCheckBoxRotLightOrMesh);
	mem_release(LightEditRotX);
	mem_release(LightEditRotY);
	mem_release(LightEditRotZ);
	mem_release(LightRadioButtonRotX);
	mem_release(LightRadioButtonRotY);
	mem_release(LightRadioButtonRotZ);
	mem_release(LightCheckBoxDirOrRot);
	mem_release(LightStaticCubeEdge);
	mem_release(LightCheckBoxCubeEdgeXPos);
	mem_release(LightCheckBoxCubeEdgeYPos);
	mem_release(LightCheckBoxCubeEdgeZPos);
	mem_release(LightCheckBoxCubeEdgeXNeg);
	mem_release(LightCheckBoxCubeEdgeYNeg);
	mem_release(LightCheckBoxCubeEdgeZNeg);
	mem_release(LightStaticColor);
	mem_release(LightEditColorR);
	mem_release(LightEditColorG);
	mem_release(LightEditColorB);
	mem_release(LightStaticPower);
	mem_release(LightEditPower);
	mem_release(LightStaticDist);
	mem_release(LightEditDist);
	mem_release(LightButtonColorSel);
	mem_release(LightButtonFinish);
	mem_release(LightStaticColorR);
	mem_release(LightStaticColorG);
	mem_release(LightStaticColorB);
	mem_release(LightStaticTopRadius);
	mem_release(LightEditTopRadius);
	mem_release(LightEditBias);
	mem_release(LightStaticAngle);
	mem_release(LightEditAngle);

	mem_release(LightStaticSourceMesh);
	mem_release(LightEditSourceMesh);
	mem_release(LightButtonSourceMesh);
	mem_release(LightComboBoxSourceMesh);
}
