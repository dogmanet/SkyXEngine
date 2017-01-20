
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

	ISXGUITrackBar* TrackBarDensity;
	ISXGUIButton* ButtonGenerate;
	ISXGUIStatic* StaticDensityText;
	ISXGUIStatic* StaticDensityVal;

	void InitAllElements();

	void DeleteAllElements();

	int HowActivateType = 0;	
};

#include <level_editor\\green_callback.cpp>
#include <level_editor\\model_callback.cpp>
#include <level_editor\\common_callback.cpp>

void SXLevelEditor::InitAllElements()
{
	SXLevelEditor::JobWindow = SXGUICrBaseWnd("JobWindow","SX Level editor",0,0,256,71,820,620,0,0,CreateSolidBrush(RGB(220,220,220)),0,CS_HREDRAW | CS_VREDRAW,WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,0,WndProcAllDefault);
	SXGUIBaseHandlers::InitHandlerMsg(SXLevelEditor::JobWindow);

	SXLevelEditor::RenderWindow = SXGUICrBaseWnd("RenderWindow","RenderWindow",0,0,0,25,600,400,0,0,CreateSolidBrush(RGB(200,200,200)),0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,SXLevelEditor::JobWindow->GetHWND(),0);
	SXLevelEditor::ToolBar1 = SXGUICrToolBar("",0,1,810,24,810,24,SXLevelEditor::JobWindow->GetHWND(),0,0);
	
	SXLevelEditor::GroupBoxList = SXGUICrGroupBox("GroupBoxList", 601, 26, 200, 400, SXLevelEditor::JobWindow->GetHWND(), WndProcAllDefault, 0);
	SXGUIBaseHandlers::InitHandlerMsg(SXLevelEditor::GroupBoxList);
	SXLevelEditor::GroupBoxList->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::GroupBoxList->SetColorText(0,0,0);
	SXLevelEditor::GroupBoxList->SetColorTextBk(220,220,220);
	SXLevelEditor::GroupBoxList->SetTransparentTextBk(true);
	SXLevelEditor::GroupBoxList->SetColorBrush(220,220,220);

	SXLevelEditor::GroupBoxData = SXGUICrGroupBox("GroupBoxData", 0, 425, 800, 120, SXLevelEditor::JobWindow->GetHWND(), WndProcAllDefault, 0);
	SXGUIBaseHandlers::InitHandlerMsg(SXLevelEditor::GroupBoxData);
	SXLevelEditor::GroupBoxData->SetFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::GroupBoxData->SetColorText(0,0,0);
	SXLevelEditor::GroupBoxData->SetColorTextBk(220,220,220);
	SXLevelEditor::GroupBoxData->SetTransparentTextBk(true);
	SXLevelEditor::GroupBoxData->SetColorBrush(220,220,220);

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

	SXLevelEditor::StatusBar1 = SXGUICrStatusBar("StatusBar1", SXLevelEditor::JobWindow->GetHWND(), 0, 0);
	SXLevelEditor::StatusBar1->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);


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




	SXLevelEditor::TrackBarDensity = SXGUICrTrackBar("", 330, 40, 200, 20, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::TrackBarDensity->SetMinMax(1, 10);
	SXLevelEditor::TrackBarDensity->AddHandler(SXLevelEditor_TrackBarDensity_MouseMove, WM_MOUSEMOVE);
	SXLevelEditor::TrackBarDensity->GAlign.left = true;
	SXLevelEditor::TrackBarDensity->GAlign.top = true;
	SXLevelEditor::TrackBarDensity->Visible(false);

	SXLevelEditor::ButtonGenerate = SXGUICrButton("Generate", 380, 75, 100, 40, 0, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::ButtonGenerate->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGenerate->GAlign.left = true;
	SXLevelEditor::ButtonGenerate->GAlign.top = true;
	SXLevelEditor::ButtonGenerate->Visible(false);
	SXLevelEditor::ButtonGenerate->AddHandler(SXLevelEditor_ButtonGenerate_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticDensityText = SXGUICrStatic("Density:", 380, 20, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
	SXLevelEditor::StaticDensityText->SetFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticDensityText->SetColorText(0, 0, 0);
	SXLevelEditor::StaticDensityText->SetColorTextBk(255, 255, 255);
	SXLevelEditor::StaticDensityText->SetTransparentTextBk(true);
	SXLevelEditor::StaticDensityText->SetColorBrush(220, 220, 220);
	SXLevelEditor::StaticDensityText->GAlign.left = true;
	SXLevelEditor::StaticDensityText->GAlign.top = true;
	SXLevelEditor::StaticDensityText->Visible(false);

	SXLevelEditor::StaticDensityVal = SXGUICrStatic("0", 430, 20, 50, 15, SXLevelEditor::GroupBoxData->GetHWND(), 0, 0);
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
}
void SXLevelEditor::DeleteAllElements()
{
	mem_delete(SXLevelEditor::RenderWindow);
	mem_delete(SXLevelEditor::ToolBar1);
	mem_delete(SXLevelEditor::GroupBoxList);
	mem_delete(SXLevelEditor::GroupBoxData);
	mem_delete(SXLevelEditor::ListBoxList);
	mem_delete(SXLevelEditor::StaticListTextCount);
	mem_delete(SXLevelEditor::StaticListValCount);
	mem_delete(SXLevelEditor::ButtonGeometryOpen);
	mem_delete(SXLevelEditor::ButtonGreenOpen);
	mem_delete(SXLevelEditor::ButtonLightOpen);
	mem_delete(SXLevelEditor::JobWindow);
}
