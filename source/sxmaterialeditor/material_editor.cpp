
#include "material_editor.h"

namespace SXMaterialEditor
{
	ISXGUIBaseWnd* JobWindow;
	ISXGUIMenuWindow* MainMenu;
	ISXGUIBaseWnd* WindowRender;

	ISXGUIToolBar* ToolBar;
	ISXGUIButton* ButtonTBOpen;
	ISXGUIButton* ButtonTBSave;

	ISXGUICheckBox* CheckBoxTBCam;
	ISXGUIButton* ButtonTBView;

	ISXGUICheckBox* CheckBoxTBRColor;
	ISXGUICheckBox* CheckBoxTBRNormal;
	ISXGUICheckBox* CheckBoxTBRParam;
	ISXGUICheckBox* CheckBoxTBRAmDiff;
	ISXGUICheckBox* CheckBoxTBRSpecular;
	ISXGUICheckBox* CheckBoxTBRLighting;

	ISXGUIButton* ButtonSkyBox;
	ISXGUIEdit* EditSkyBox;
	ISXGUICheckBox* CheckBoxModelRot;
	ISXGUIButton* ButtonRotAngle0;
	ISXGUIStatic* StaticTypeModel;
	ISXGUIComboBox* ComboBoxTypeModel;

	ISXGUIStatic* StaticPenetration;
	ISXGUIEdit* EditPenetration;
	ISXGUITrackBar* TrackBarPenetration;

	ISXGUIStatic* StaticTestModel;
	ISXGUIComboBox* ComboBoxTestModel;
	ISXGUIStatic* StaticTex;
	ISXGUIEdit* EditTex;
	ISXGUIStatic* StaticVS;
	ISXGUIEdit* EditVS;
	ISXGUIStatic* StaticPS;
	ISXGUIEdit* EditPS;
	ISXGUIButton* ButtonTex;

	ISXGUIStatic* StaticShaders;
	ISXGUIComboBox* ComboBoxShaders;
	
	ISXGUIButton* ButtonVS;
	ISXGUIButton* ButtonPS;
	ISXGUICheckBox* CheckBoxAlphaTest;
	ISXGUIStatic* StaticMR;
	ISXGUIStatic* StaticRoughness;
	ISXGUIStatic* StaticF0;
	ISXGUIStatic* StaticMG;
	ISXGUIEdit* EditRoughness;
	ISXGUIEdit* EditF0;
	ISXGUIEdit* EditMR;
	ISXGUICheckBox* CheckBoxLighting;
	ISXGUITrackBar* TrackBarRoughness;
	ISXGUITrackBar* TrackBarF0;

	ISXGUIStatic* StaticThickness;
	ISXGUIEdit* EditThickness;
	ISXGUITrackBar* TrackBarThickness;

	ISXGUITrackBar* TrackBarUDVSX;
	ISXGUIStatic* StaticMaD;
	ISXGUIEdit* EditTexLighting;
	ISXGUIButton* ButtonTexLighting;
	ISXGUICheckBox* CheckBoxTexLighting;

	ISXGUIStatic* StaticParamL;
	ISXGUIComboBox* ComboBoxParamL;

	ISXGUIStatic* StaticTypeRefract;
	ISXGUIComboBox* ComboBoxTypeRefract;
	ISXGUICheckBox* CheckBoxDoSVVS;
	ISXGUIStatic* StaticTypeReflect;
	ISXGUIComboBox* ComboBoxTypeReflect;
	ISXGUIStatic* StaticPhysic;
	ISXGUIComboBox* ComboBoxPhysic;
	ISXGUIStatic* StaticMask;
	ISXGUIEdit* EditMask;
	ISXGUIButton* ButtonMask;
	ISXGUIButton* ButtonMR;
	ISXGUIStatic* StaticMB;
	ISXGUIStatic* StaticMA;
	ISXGUIStatic* StaticDR;
	ISXGUIEdit* EditMG;
	ISXGUIEdit* EditMB;
	ISXGUIEdit* EditMA;
	ISXGUIEdit* EditDR;
	ISXGUIButton* ButtonMG;
	ISXGUIButton* ButtonMB;
	ISXGUIButton* ButtonMA;
	ISXGUIButton* ButtonDR;
	ISXGUIStatic* StaticDG;
	ISXGUIStatic* StaticDB;
	ISXGUIStatic* StaticDA;
	ISXGUIEdit* EditDG;
	ISXGUIEdit* EditDB;
	ISXGUIEdit* EditDA;
	ISXGUIButton* ButtonDG;
	ISXGUIButton* ButtonDB;
	ISXGUIButton* ButtonDA;
	ISXGUIStatic* StaticDoS;
	ISXGUIStatic* StaticDoSW;
	ISXGUICheckBox* CheckBoxDoSWVS;
	ISXGUICheckBox* CheckBoxDoSWPS;
	ISXGUIStatic* StaticDoSV;
	ISXGUICheckBox* CheckBoxDoSVPS;
	ISXGUIStatic* StaticDosP;
	ISXGUICheckBox* CheckBoxDoSPVS;
	ISXGUICheckBox* CheckBoxDoSPPS;
	ISXGUIStatic* StaticBoxDoSWV;
	ISXGUIStatic* StaticDoSWVP;
	ISXGUIStatic* StaticDoSCampos;
	ISXGUICheckBox* CheckBoxDoSWVVS;
	ISXGUICheckBox* CheckBoxDoSWVPS;
	ISXGUICheckBox* CheckBoxDoSCamposVS;
	ISXGUICheckBox* CheckBoxDoSWVPVS;
	ISXGUICheckBox* CheckBoxDoSWVPPS;
	ISXGUICheckBox* CheckBoxDoSCamposPS;
	ISXGUIStatic* StaticDoSWinSize;
	ISXGUICheckBox* CheckBoxDoSWinSizeVS;
	ISXGUICheckBox* CheckBoxDoSWinSizePS;

	ISXGUICheckBox* CheckBoxUDVS;

	ISXGUICheckBox* CheckBoxUDVSInPS;
	ISXGUIEdit* EditUDVSX;
	ISXGUIEdit* EditUDVSY;
	ISXGUITrackBar* TrackBarUDVSY;
	ISXGUIStatic* StaticUDVSX;
	ISXGUIStatic* StaticUDVSY;
	ISXGUIStatic* StaticUDVSZ;
	ISXGUIEdit* EditUDVSZ;
	ISXGUITrackBar* TrackBarUDVSZ;
	ISXGUIStatic* StaticUDVSW;
	ISXGUIEdit* EditUDVSW;
	ISXGUITrackBar* TrackBarUDVSW;
	ISXGUIStatic* StaticDoSTimeDelta;
	ISXGUICheckBox* CheckBoxDoSTimeDeltaVS;
	ISXGUICheckBox* CheckBoxDoSTimeDeltaPS;

	ISXGUICheckBox* CheckBoxUDPS;

	ISXGUICheckBox* CheckBoxUDPSInVS;
	ISXGUIStatic* StaticUDPSX;
	ISXGUIStatic* StaticUDPSY;
	ISXGUIStatic* StaticUDPSZ;
	ISXGUIStatic* StaticUDPSW;
	ISXGUIEdit* EditUDPSX;
	ISXGUIEdit* EditUDPSY;
	ISXGUITrackBar* TrackBarUDPSX;
	ISXGUITrackBar* TrackBarUDPSY;
	ISXGUIEdit* EditUDPSZ;
	ISXGUITrackBar* TrackBarUDPSZ;
	ISXGUIEdit* EditUDPSW;
	ISXGUITrackBar* TrackBarUDPSW;

	void InitAllElements();

	void DeleteAllElements();

	void Nulling();
	void InitMtl(ID id);

	void MtlOpen();
	void MtlSave();

	void FinalImageUncheckedMenu();

	ID IDMat = -1;

	ShadersKit* Shaders = 0;
	ParamLKit* ParamL = 0;
};


void SXMaterialEditor::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int cx = (wrect.right - MAINWIN_SIZE_X) / 2;
	int cy = (wrect.bottom - MAINWIN_SIZE_Y) / 2;

	SXMaterialEditor::JobWindow = SXGUICrBaseWndEx("SXMaterialEditor", "SXMaterialEditor", cx, cy, MAINWIN_SIZE_X, MAINWIN_SIZE_Y, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_DLGFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	SXMaterialEditor::JobWindow->setVisible(false);
	gui_func::base_handlers::InitHandlerMsg(SXMaterialEditor::JobWindow);
	SXMaterialEditor::JobWindow->addHandler(SXMaterialEditor_JobWindow_CallWmCommand, WM_COMMAND);

	SXMaterialEditor::JobWindow->addHandler(ComMenuId, WM_COMMAND);
	SXMaterialEditor::JobWindow->addHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXMaterialEditor::JobWindow->addHandler(MsgEditSize, WM_SIZE);
	SXMaterialEditor::MainMenu = SXGUICrMenuWindowEx(IDR_MENU1);
	SXMaterialEditor::MainMenu->setToWindow(SXMaterialEditor::JobWindow->getHWND());

	SXMaterialEditor::WindowRender = SXGUICrBaseWndEx("WindowRender", "WindowRender",0,26,300,300,0,0,CreateSolidBrush(RGB(200,200,200)),0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,SXMaterialEditor::JobWindow->getHWND(),0);
	SXMaterialEditor::WindowRender->setStretchSides(false, false, false, false);
	
	/*SXMaterialEditor::WindowRender->GAlign.left = false;
	SXMaterialEditor::WindowRender->GAlign.right = false;
	SXMaterialEditor::WindowRender->GAlign.top = false;
	SXMaterialEditor::WindowRender->GAlign.bottom = false;*/
	
	SXMaterialEditor::ToolBar = SXGUICrToolBar(0, 0, 645, 26, SXMaterialEditor::JobWindow->getHWND(), WndProcAllDefault, 0);
	SXMaterialEditor::ToolBar->setStretchSides(false, false, false, false);
	
	/*SXMaterialEditor::ToolBar->GAlign.left = false;
	SXMaterialEditor::ToolBar->GAlign.right = false;
	SXMaterialEditor::ToolBar->GAlign.top = false;
	SXMaterialEditor::ToolBar->GAlign.bottom = false;*/
	SXMaterialEditor::ToolBar->addHandler(SXMaterialEditor_ToolBar_CallWmCommand, WM_COMMAND);

	SXMaterialEditor::ButtonTBOpen = SXGUICrButtonEx("", 2, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::ButtonTBOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonTBOpen->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::ButtonTBOpen->GAlign.left = true;
	SXMaterialEditor::ButtonTBOpen->GAlign.top = true;*/
	SXMaterialEditor::ButtonTBOpen->setBmpFromResourse(IDB_BITMAP1);

	SXMaterialEditor::ButtonTBSave = SXGUICrButtonEx("", 26, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::ButtonTBSave->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonTBSave->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::ButtonTBSave->GAlign.left = true;
	SXMaterialEditor::ButtonTBSave->GAlign.top = true;*/
	SXMaterialEditor::ButtonTBSave->setBmpFromResourse(IDB_BITMAP2);

	SXMaterialEditor::CheckBoxTBCam = SXGUICrCheckBoxEx("", 54, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::CheckBoxTBCam->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxTBCam->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::CheckBoxTBCam->GAlign.left = true;
	SXMaterialEditor::CheckBoxTBCam->GAlign.top = true;*/
	SXMaterialEditor::CheckBoxTBCam->setBmpFromResourse(IDB_BITMAP3);

	SXMaterialEditor::ButtonTBView = SXGUICrButtonEx("", 78, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::ButtonTBView->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonTBView->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::ButtonTBView->GAlign.left = true;
	SXMaterialEditor::ButtonTBView->GAlign.top = true;*/
	SXMaterialEditor::ButtonTBView->setBmpFromResourse(IDB_BITMAP4);

	SXMaterialEditor::CheckBoxTBRColor = SXGUICrCheckBoxEx("", 106, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::CheckBoxTBRColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxTBRColor->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::CheckBoxTBRColor->GAlign.left = true;
	SXMaterialEditor::CheckBoxTBRColor->GAlign.top = true;*/
	SXMaterialEditor::CheckBoxTBRColor->setBmpFromResourse(IDB_BITMAP5);

	SXMaterialEditor::CheckBoxTBRNormal = SXGUICrCheckBoxEx("", 130, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::CheckBoxTBRNormal->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxTBRNormal->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::CheckBoxTBRNormal->GAlign.left = true;
	SXMaterialEditor::CheckBoxTBRNormal->GAlign.top = true;*/
	SXMaterialEditor::CheckBoxTBRNormal->setBmpFromResourse(IDB_BITMAP6);

	SXMaterialEditor::CheckBoxTBRParam = SXGUICrCheckBoxEx("", 154, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::CheckBoxTBRParam->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxTBRParam->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::CheckBoxTBRParam->GAlign.left = true;
	SXMaterialEditor::CheckBoxTBRParam->GAlign.top = true;*/
	SXMaterialEditor::CheckBoxTBRParam->setBmpFromResourse(IDB_BITMAP7);

	SXMaterialEditor::CheckBoxTBRAmDiff = SXGUICrCheckBoxEx("", 178, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::CheckBoxTBRAmDiff->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxTBRAmDiff->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::CheckBoxTBRAmDiff->GAlign.left = true;
	SXMaterialEditor::CheckBoxTBRAmDiff->GAlign.top = true;*/
	SXMaterialEditor::CheckBoxTBRAmDiff->setBmpFromResourse(IDB_BITMAP8);

	SXMaterialEditor::CheckBoxTBRSpecular = SXGUICrCheckBoxEx("", 202, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::CheckBoxTBRSpecular->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxTBRSpecular->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::CheckBoxTBRSpecular->GAlign.left = true;
	SXMaterialEditor::CheckBoxTBRSpecular->GAlign.top = true;*/
	SXMaterialEditor::CheckBoxTBRSpecular->setBmpFromResourse(IDB_BITMAP9);

	SXMaterialEditor::CheckBoxTBRLighting = SXGUICrCheckBoxEx("", 226, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::ToolBar->getHWND(), 0, 0);
	SXMaterialEditor::CheckBoxTBRLighting->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxTBRLighting->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::CheckBoxTBRLighting->GAlign.left = true;
	SXMaterialEditor::CheckBoxTBRLighting->GAlign.top = true;*/
	SXMaterialEditor::CheckBoxTBRLighting->setBmpFromResourse(IDB_BITMAP10);


	

	
	SXMaterialEditor::ButtonSkyBox = SXGUICrButton("...", 275, 330, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonSkyBox->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonSkyBox->addHandler(SXMaterialEditor_ButtonSkyBox_Click, WM_LBUTTONUP);

	SXMaterialEditor::EditSkyBox = SXGUICrEdit("0", 100, 330, 170, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::EditSkyBox->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::EditSkyBox->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditSkyBox->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditSkyBox->setTransparentTextBk(true);
	SXMaterialEditor::EditSkyBox->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditSkyBox->addHandler(SXMaterialEditor_EditSkyBox_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditSkyBox->addHandler(SXMaterialEditor_EditSkyBox_Enter, WM_KILLFOCUS);

	SXMaterialEditor::CheckBoxModelRot = SXGUICrCheckBox("rotate", 5, 330, 50, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxModelRot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxModelRot->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxModelRot->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxModelRot->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxModelRot->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::ButtonRotAngle0 = SXGUICrButton("0", 60, 330, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonRotAngle0->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonRotAngle0->addHandler(SXMaterialEditor_ButtonRotAngle0_Click, WM_LBUTTONUP);
	
	SXMaterialEditor::StaticTypeModel = SXGUICrStatic("Type model:",5,350,70,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticTypeModel->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticTypeModel->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticTypeModel->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticTypeModel->setTransparentTextBk(true);
	SXMaterialEditor::StaticTypeModel->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::ComboBoxTypeModel = SXGUICrComboBox(100,347,200,100,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::ComboBoxTypeModel->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ComboBoxTypeModel->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::ComboBoxTypeModel->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxTypeModel->setTransparentTextBk(true);
	SXMaterialEditor::ComboBoxTypeModel->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxTypeModel->addItem("geometry");
	SXMaterialEditor::ComboBoxTypeModel->addItem("grass");
	SXMaterialEditor::ComboBoxTypeModel->addItem("tree");
	SXMaterialEditor::ComboBoxTypeModel->addItem("skin");
	SXMaterialEditor::ComboBoxTypeModel->setSel(0);

	SXMaterialEditor::StaticTestModel = SXGUICrStatic("Test model:",5,372,70,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticTestModel->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticTestModel->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticTestModel->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticTestModel->setTransparentTextBk(true);
	SXMaterialEditor::StaticTestModel->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::ComboBoxTestModel = SXGUICrComboBox(100,370,200,70,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::ComboBoxTestModel->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ComboBoxTestModel->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::ComboBoxTestModel->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxTestModel->setTransparentTextBk(true);
	SXMaterialEditor::ComboBoxTestModel->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxTestModel->addItem("sphere");
	SXMaterialEditor::ComboBoxTestModel->addItem("cube");
	SXMaterialEditor::ComboBoxTestModel->addItem("plane");
	SXMaterialEditor::ComboBoxTestModel->setSel(0);

	SXMaterialEditor::StaticTex = SXGUICrStatic("Tex:",5,400,30,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticTex->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticTex->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticTex->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticTex->setTransparentTextBk(true);
	SXMaterialEditor::StaticTex->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::EditTex = SXGUICrEdit("0",40,400,230,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditTex->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditTex->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditTex->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditTex->setTransparentTextBk(true);
	SXMaterialEditor::EditTex->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditTex->addHandler(SXMaterialEditor_EditTex_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditTex->addHandler(SXMaterialEditor_EditTex_Enter, WM_KILLFOCUS);

	SXMaterialEditor::ButtonTex = SXGUICrButton("...", 275, 400, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonTex->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonTex->addHandler(SXMaterialEditor_ButtonTex_Click, WM_LBUTTONUP);


	SXMaterialEditor::StaticShaders = SXGUICrStatic("Shaders^", 5, 420, 40, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticShaders->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticShaders->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticShaders->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticShaders->setTransparentTextBk(true);
	SXMaterialEditor::StaticShaders->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::ComboBoxShaders = SXGUICrComboBoxEx(60, 418, 240, 150, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ComboBoxShaders->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ComboBoxShaders->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::ComboBoxShaders->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxShaders->setTransparentTextBk(true);
	SXMaterialEditor::ComboBoxShaders->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxShaders->addItem("");


	SXMaterialEditor::StaticVS = SXGUICrStatic("VS:",5,440,30,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticVS->setTransparentTextBk(true);
	SXMaterialEditor::StaticVS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditVS = SXGUICrEdit("0",40,440,230,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditVS->setTransparentTextBk(true);
	SXMaterialEditor::EditVS->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditVS->addHandler(SXMaterialEditor_EditVS_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditVS->addHandler(SXMaterialEditor_EditVS_Enter, WM_KILLFOCUS);

	SXMaterialEditor::ButtonVS = SXGUICrButton("...", 275, 440, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonVS->addHandler(SXMaterialEditor_ButtonVS_Click, WM_LBUTTONUP);

	SXMaterialEditor::StaticPS = SXGUICrStatic("PS:",5,460,30,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticPS->setTransparentTextBk(true);
	SXMaterialEditor::StaticPS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditPS = SXGUICrEdit("0",40,460,230,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditPS->setTransparentTextBk(true);
	SXMaterialEditor::EditPS->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditPS->addHandler(SXMaterialEditor_EditPS_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditPS->addHandler(SXMaterialEditor_EditPS_Enter, WM_KILLFOCUS);

	SXMaterialEditor::ButtonPS = SXGUICrButton("...", 275, 460, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonPS->addHandler(SXMaterialEditor_ButtonPS_Click, WM_LBUTTONUP);

	SXMaterialEditor::StaticPhysic = SXGUICrStatic("Physic material:", 5, 485, 80, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticPhysic->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticPhysic->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticPhysic->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticPhysic->setTransparentTextBk(true);
	SXMaterialEditor::StaticPhysic->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::ComboBoxPhysic = SXGUICrComboBox(100, 482, 200, 120, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ComboBoxPhysic->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ComboBoxPhysic->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::ComboBoxPhysic->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxPhysic->setTransparentTextBk(true);
	SXMaterialEditor::ComboBoxPhysic->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxPhysic->addItem("concrete (бетон)");
	SXMaterialEditor::ComboBoxPhysic->addItem("metal (металл)");
	SXMaterialEditor::ComboBoxPhysic->addItem("glass (стекло)");
	SXMaterialEditor::ComboBoxPhysic->addItem("plastic (пластик)");
	SXMaterialEditor::ComboBoxPhysic->addItem("tree (дерево)");
	SXMaterialEditor::ComboBoxPhysic->addItem("flesh (плоть)");
	SXMaterialEditor::ComboBoxPhysic->addItem("ground/sand (земля/песок)");
	SXMaterialEditor::ComboBoxPhysic->addItem("water (вода)");
	SXMaterialEditor::ComboBoxPhysic->addItem("leaf/grass (листва/трава)");
	SXMaterialEditor::ComboBoxPhysic->setSel(0);

	SXMaterialEditor::StaticPenetration = SXGUICrStatic("Penetraion:", 5, 510, 60, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticPenetration->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticPenetration->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticPenetration->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticPenetration->setTransparentTextBk(true);
	SXMaterialEditor::StaticPenetration->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::EditPenetration = SXGUICrEdit("0", 60, 510, 65, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::EditPenetration->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::EditPenetration->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditPenetration->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditPenetration->setTransparentTextBk(true);
	SXMaterialEditor::EditPenetration->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditPenetration->addHandler(SXMaterialEditor_EditPenetration_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditPenetration->addHandler(SXMaterialEditor_EditPenetration_Enter, WM_KILLFOCUS);
	SXMaterialEditor::TrackBarPenetration = SXGUICrTrackBar("", 130, 510, 170, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarPenetration->setMinMax(0, 100);
	SXMaterialEditor::TrackBarPenetration->setTickFrequency(10);
	SXMaterialEditor::TrackBarPenetration->addHandler(SXMaterialEditor_TrackBarPenetration_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::CheckBoxLighting = SXGUICrCheckBox("Lighting", 5, 535, 55, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxLighting->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxLighting->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxLighting->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxLighting->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxLighting->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::CheckBoxAlphaTest = SXGUICrCheckBox("alphatest", 65, 535, 60, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxAlphaTest->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxAlphaTest->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxAlphaTest->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxAlphaTest->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxAlphaTest->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::StaticTypeRefract = SXGUICrStatic("Refraction:", 5, 560, 60, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticTypeRefract->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticTypeRefract->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticTypeRefract->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticTypeRefract->setTransparentTextBk(true);
	SXMaterialEditor::StaticTypeRefract->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::ComboBoxTypeRefract = SXGUICrComboBox(70, 558, 230, 115, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ComboBoxTypeRefract->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ComboBoxTypeRefract->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::ComboBoxTypeRefract->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxTypeRefract->setTransparentTextBk(true);
	SXMaterialEditor::ComboBoxTypeRefract->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxTypeRefract->addItem("none");
	SXMaterialEditor::ComboBoxTypeRefract->addItem("transparency lighting");
	SXMaterialEditor::ComboBoxTypeRefract->setSel(0);

	SXMaterialEditor::StaticTypeReflect = SXGUICrStatic("Reflection:", 5, 580, 60, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticTypeReflect->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticTypeReflect->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticTypeReflect->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticTypeReflect->setTransparentTextBk(true);
	SXMaterialEditor::StaticTypeReflect->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::ComboBoxTypeReflect = SXGUICrComboBox(70, 578, 230, 115, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ComboBoxTypeReflect->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ComboBoxTypeReflect->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::ComboBoxTypeReflect->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxTypeReflect->setTransparentTextBk(true);
	SXMaterialEditor::ComboBoxTypeReflect->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxTypeReflect->addItem("none");
	SXMaterialEditor::ComboBoxTypeReflect->addItem("reflect plane");
	SXMaterialEditor::ComboBoxTypeReflect->addItem("reflect cube dynamic");
	SXMaterialEditor::ComboBoxTypeReflect->addItem("reflect cube static");
	SXMaterialEditor::ComboBoxTypeReflect->setSel(0);


	SXMaterialEditor::StaticMR = SXGUICrStatic("M R:",305,175,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticMR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticMR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticMR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticMR->setTransparentTextBk(true);
	SXMaterialEditor::StaticMR->setColorBrush(RGB(220, 220, 220));
	
	SXMaterialEditor::StaticMG = SXGUICrStatic("M G:",305,195,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticMG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticMG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticMG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticMG->setTransparentTextBk(true);
	SXMaterialEditor::StaticMG->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::StaticParamL = SXGUICrStatic("Parameters Lighting:", 305, 30, 100, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticParamL->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticParamL->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticParamL->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticParamL->setTransparentTextBk(true);
	SXMaterialEditor::StaticParamL->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::ComboBoxParamL = SXGUICrComboBoxEx(400, 28, 230, 150, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ComboBoxParamL->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ComboBoxParamL->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::ComboBoxParamL->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxParamL->setTransparentTextBk(true);
	SXMaterialEditor::ComboBoxParamL->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::ComboBoxParamL->addItem("");

	SXMaterialEditor::StaticThickness = SXGUICrStatic("Thickness:", 305, 50, 70, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticThickness->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticThickness->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticThickness->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticThickness->setTransparentTextBk(true);
	SXMaterialEditor::StaticThickness->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditThickness = SXGUICrEdit("0", 375, 50, 50, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::EditThickness->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::EditThickness->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditThickness->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditThickness->setTransparentTextBk(true);
	SXMaterialEditor::EditThickness->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditThickness->addHandler(SXMaterialEditor_EditThickness_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditThickness->addHandler(SXMaterialEditor_EditThickness_Enter, WM_KILLFOCUS);

	SXMaterialEditor::TrackBarThickness = SXGUICrTrackBar("", 430, 50, 200, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarThickness->setMinMax(0, 100);
	SXMaterialEditor::TrackBarThickness->setTickFrequency(10);
	SXMaterialEditor::TrackBarThickness->addHandler(SXMaterialEditor_TrackBarThickness_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::StaticRoughness = SXGUICrStatic("Roughness:", 305, 70, 70, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticRoughness->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticRoughness->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticRoughness->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticRoughness->setTransparentTextBk(true);
	SXMaterialEditor::StaticRoughness->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditRoughness = SXGUICrEdit("0",375,70,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditRoughness->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditRoughness->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditRoughness->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditRoughness->setTransparentTextBk(true);
	SXMaterialEditor::EditRoughness->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditRoughness->addHandler(SXMaterialEditor_EditRoughness_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditRoughness->addHandler(SXMaterialEditor_EditRoughness_Enter, WM_KILLFOCUS);

	SXMaterialEditor::TrackBarRoughness = SXGUICrTrackBar("", 430, 70, 200, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarRoughness->setMinMax(0, 100);
	SXMaterialEditor::TrackBarRoughness->setTickFrequency(10);
	SXMaterialEditor::TrackBarRoughness->addHandler(SXMaterialEditor_TrackBarRoughness_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::StaticF0 = SXGUICrStatic("f0:", 305, 90, 70, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticF0->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticF0->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticF0->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticF0->setTransparentTextBk(true);
	SXMaterialEditor::StaticF0->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditF0 = SXGUICrEdit("0",375,90,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditF0->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditF0->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditF0->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditF0->setTransparentTextBk(true);
	SXMaterialEditor::EditF0->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditF0->addHandler(SXMaterialEditor_EditF0_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditF0->addHandler(SXMaterialEditor_EditF0_Enter, WM_KILLFOCUS);

	SXMaterialEditor::TrackBarF0 = SXGUICrTrackBar("",430,90,200,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::TrackBarF0->setMinMax(0, 100);
	SXMaterialEditor::TrackBarF0->setTickFrequency(10);
	SXMaterialEditor::TrackBarF0->addHandler(SXMaterialEditor_TrackBarF0_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::EditTexLighting = SXGUICrEdit("0",370,110,230,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditTexLighting->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditTexLighting->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditTexLighting->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditTexLighting->setTransparentTextBk(true);
	SXMaterialEditor::EditTexLighting->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditTexLighting->addHandler(SXMaterialEditor_EditTexLighting_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditTexLighting->addHandler(SXMaterialEditor_EditTexLighting_Enter, WM_KILLFOCUS);

	SXMaterialEditor::ButtonTexLighting = SXGUICrButton("...", 605, 110, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonTexLighting->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonTexLighting->addHandler(SXMaterialEditor_ButtonTexLighting_Click, WM_LBUTTONUP);

	SXMaterialEditor::CheckBoxTexLighting = SXGUICrCheckBox("texture:", 305, 110, 60, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxTexLighting->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxTexLighting->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxTexLighting->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxTexLighting->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxTexLighting->setColorBrush(RGB(220, 220, 220));
	

	SXMaterialEditor::StaticMaD = SXGUICrStatic("Microrelief and detail", 305, 135, 100, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticMaD->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticMaD->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticMaD->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticMaD->setTransparentTextBk(true);
	SXMaterialEditor::StaticMaD->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditMR = SXGUICrEdit("0", 360, 175, 240, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::EditMR->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::EditMR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditMR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditMR->setTransparentTextBk(true);
	SXMaterialEditor::EditMR->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditMR->addHandler(SXMaterialEditor_EditMR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditMR->addHandler(SXMaterialEditor_EditMR_Enter, WM_KILLFOCUS);

	SXMaterialEditor::CheckBoxDoSVVS = SXGUICrCheckBox("vs", 340, 390, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSVVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSVVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSVVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSVVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSVVS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::StaticMask = SXGUICrStatic("Mask:", 305, 152, 50, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticMask->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticMask->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticMask->setTransparentTextBk(true);
	SXMaterialEditor::StaticMask->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditMask = SXGUICrEdit("0", 360, 152, 240, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::EditMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::EditMask->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditMask->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditMask->setTransparentTextBk(true);
	SXMaterialEditor::EditMask->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditMask->addHandler(SXMaterialEditor_EditMask_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditMask->addHandler(SXMaterialEditor_EditMask_Enter, WM_KILLFOCUS);

	SXMaterialEditor::ButtonMask = SXGUICrButton("...", 605, 152, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::ButtonMask->addHandler(SXMaterialEditor_ButtonMask_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonMR = SXGUICrButton("...", 605, 175, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonMR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonMR->addHandler(SXMaterialEditor_ButtonMR_Click, WM_LBUTTONUP);

	SXMaterialEditor::StaticMB = SXGUICrStatic("M B:",305,215,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticMB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticMB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticMB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticMB->setTransparentTextBk(true);
	SXMaterialEditor::StaticMB->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticMA = SXGUICrStatic("M A:",305,235,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticMA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticMA->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticMA->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticMA->setTransparentTextBk(true);
	SXMaterialEditor::StaticMA->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticDR = SXGUICrStatic("D R:",305,260,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDR->setTransparentTextBk(true);
	SXMaterialEditor::StaticDR->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditMG = SXGUICrEdit("0",360,195,240,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditMG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditMG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditMG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditMG->setTransparentTextBk(true);
	SXMaterialEditor::EditMG->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditMG->addHandler(SXMaterialEditor_EditMG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditMG->addHandler(SXMaterialEditor_EditMG_Enter, WM_KILLFOCUS);

	SXMaterialEditor::EditMB = SXGUICrEdit("0",360,215,240,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditMB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditMB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditMB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditMB->setTransparentTextBk(true);
	SXMaterialEditor::EditMB->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditMB->addHandler(SXMaterialEditor_EditMB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditMB->addHandler(SXMaterialEditor_EditMB_Enter, WM_KILLFOCUS);

	SXMaterialEditor::EditMA = SXGUICrEdit("0",360,235,240,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditMA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditMA->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditMA->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditMA->setTransparentTextBk(true);
	SXMaterialEditor::EditMA->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditMA->addHandler(SXMaterialEditor_EditMA_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditMA->addHandler(SXMaterialEditor_EditMA_Enter, WM_KILLFOCUS);

	SXMaterialEditor::EditDR = SXGUICrEdit("0",360,260,240,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditDR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditDR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditDR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditDR->setTransparentTextBk(true);
	SXMaterialEditor::EditDR->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditDR->addHandler(SXMaterialEditor_EditDR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditDR->addHandler(SXMaterialEditor_EditDR_Enter, WM_KILLFOCUS);

	SXMaterialEditor::ButtonMG = SXGUICrButton("...", 605, 195, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonMG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonMG->addHandler(SXMaterialEditor_ButtonMG_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonMB = SXGUICrButton("...", 605, 215, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonMB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonMB->addHandler(SXMaterialEditor_ButtonMB_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonMA = SXGUICrButton("...", 605, 235, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonMA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonMA->addHandler(SXMaterialEditor_ButtonMA_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonDR = SXGUICrButton("...", 605, 260, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonDR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonDR->addHandler(SXMaterialEditor_ButtonDR_Click, WM_LBUTTONUP);

	SXMaterialEditor::StaticDG = SXGUICrStatic("D G:",305,280,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDG->setTransparentTextBk(true);
	SXMaterialEditor::StaticDG->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticDB = SXGUICrStatic("D B:",305,300,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDB->setTransparentTextBk(true);
	SXMaterialEditor::StaticDB->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticDA = SXGUICrStatic("D A:",305,320,50,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDA->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDA->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDA->setTransparentTextBk(true);
	SXMaterialEditor::StaticDA->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditDG = SXGUICrEdit("0",360,280,240,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditDG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditDG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditDG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditDG->setTransparentTextBk(true);
	SXMaterialEditor::EditDG->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditDG->addHandler(SXMaterialEditor_EditDG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditDG->addHandler(SXMaterialEditor_EditDG_Enter, WM_KILLFOCUS);

	SXMaterialEditor::EditDB = SXGUICrEdit("0",360,300,240,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditDB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditDB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditDB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditDB->setTransparentTextBk(true);
	SXMaterialEditor::EditDB->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditDB->addHandler(SXMaterialEditor_EditDB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditDB->addHandler(SXMaterialEditor_EditDB_Enter, WM_KILLFOCUS);

	SXMaterialEditor::EditDA = SXGUICrEdit("0",360,320,240,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditDA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditDA->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditDA->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditDA->setTransparentTextBk(true);
	SXMaterialEditor::EditDA->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditDA->addHandler(SXMaterialEditor_EditDA_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditDA->addHandler(SXMaterialEditor_EditDA_Enter, WM_KILLFOCUS);

	SXMaterialEditor::ButtonDG = SXGUICrButton("...", 605, 280, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonDG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonDG->addHandler(SXMaterialEditor_ButtonDG_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonDB = SXGUICrButton("...", 605, 300, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonDB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonDB->addHandler(SXMaterialEditor_ButtonDB_Click, WM_LBUTTONUP);

	SXMaterialEditor::ButtonDA = SXGUICrButton("...", 605, 320, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::ButtonDA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::ButtonDA->addHandler(SXMaterialEditor_ButtonDA_Click, WM_LBUTTONUP);

	SXMaterialEditor::StaticDoS = SXGUICrStatic("Data for shaders:",305,350,100,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDoS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDoS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDoS->setTransparentTextBk(true);
	SXMaterialEditor::StaticDoS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticDoSW = SXGUICrStatic("W mat:",305,370,35,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDoSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDoSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDoSW->setTransparentTextBk(true);
	SXMaterialEditor::StaticDoSW->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSWVS = SXGUICrCheckBox("vs", 340, 370, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSWVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSWVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSWPS = SXGUICrCheckBox("ps", 373, 370, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSWPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSWPS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticDoSV = SXGUICrStatic("V mat:",305,390,35,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDoSV->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoSV->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDoSV->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDoSV->setTransparentTextBk(true);
	SXMaterialEditor::StaticDoSV->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSVPS = SXGUICrCheckBox("ps", 373, 390, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSVPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSVPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSVPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSVPS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSVPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticDosP = SXGUICrStatic("P mat:",305,410,35,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDosP->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticDosP->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDosP->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDosP->setTransparentTextBk(true);
	SXMaterialEditor::StaticDosP->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSPVS = SXGUICrCheckBox("vs", 340, 410, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSPVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSPVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSPVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSPVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSPVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSPPS = SXGUICrCheckBox("ps", 373, 410, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSPPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSPPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSPPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSPPS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSPPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticBoxDoSWV = SXGUICrStatic("WV mat:",410,370,45,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticBoxDoSWV->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticBoxDoSWV->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticBoxDoSWV->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticBoxDoSWV->setTransparentTextBk(true);
	SXMaterialEditor::StaticBoxDoSWV->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticDoSWVP = SXGUICrStatic("WVP:",410,390,45,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDoSWVP->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoSWVP->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDoSWVP->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDoSWVP->setTransparentTextBk(true);
	SXMaterialEditor::StaticDoSWVP->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::StaticDoSCampos = SXGUICrStatic("Campos:",410,410,45,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticDoSCampos->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticDoSCampos->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDoSCampos->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDoSCampos->setTransparentTextBk(true);
	SXMaterialEditor::StaticDoSCampos->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSWVVS = SXGUICrCheckBox("vs", 455, 370, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSWVVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSWVVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSWVPS = SXGUICrCheckBox("ps", 488, 370, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSWVPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSWVPS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSCamposVS = SXGUICrCheckBox("vs", 455, 410, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSCamposVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSCamposVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSCamposVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSCamposVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSCamposVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSWVPVS = SXGUICrCheckBox("vs", 455, 390, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVPVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVPVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSWVPVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSWVPVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVPVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSWVPPS = SXGUICrCheckBox("ps", 488, 390, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWVPPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSWVPPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSWVPPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSWVPPS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWVPPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::CheckBoxDoSCamposPS = SXGUICrCheckBox("ps", 488, 411, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSCamposPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxDoSCamposPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSCamposPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSCamposPS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSCamposPS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::StaticDoSTimeDelta = SXGUICrStatic("count_td:", 525, 370, 45, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticDoSTimeDelta->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticDoSTimeDelta->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDoSTimeDelta->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDoSTimeDelta->setTransparentTextBk(true);
	SXMaterialEditor::StaticDoSTimeDelta->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::CheckBoxDoSTimeDeltaVS = SXGUICrCheckBox("vs", 570, 370, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSTimeDeltaVS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::CheckBoxDoSTimeDeltaPS = SXGUICrCheckBox("ps", 603, 370, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSTimeDeltaPS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::StaticDoSWinSize = SXGUICrStatic("win size:", 525, 390, 40, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::StaticDoSWinSize->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::StaticDoSWinSize->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticDoSWinSize->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticDoSWinSize->setTransparentTextBk(true);
	SXMaterialEditor::StaticDoSWinSize->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::CheckBoxDoSWinSizeVS = SXGUICrCheckBox("vs", 570, 390, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWinSizeVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxDoSWinSizeVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSWinSizeVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSWinSizeVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWinSizeVS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::CheckBoxDoSWinSizePS = SXGUICrCheckBox("ps", 603, 390, 30, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxDoSWinSizePS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxDoSWinSizePS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxDoSWinSizePS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxDoSWinSizePS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxDoSWinSizePS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::CheckBoxUDVS = SXGUICrCheckBox("User data VS:", 305, 440, 100, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxUDVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxUDVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxUDVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxUDVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxUDVS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::CheckBoxUDVSInPS = SXGUICrCheckBox("in PS", 445, 440, 100, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxUDVSInPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxUDVSInPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxUDVSInPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxUDVSInPS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxUDVSInPS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditUDVSX = SXGUICrEdit("0",325,460,80,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditUDVSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDVSX->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditUDVSX->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditUDVSX->setTransparentTextBk(true);
	SXMaterialEditor::EditUDVSX->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditUDVSX->addHandler(SXMaterialEditor_EditUDVSX_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditUDVSX->addHandler(SXMaterialEditor_EditUDVSX_Enter, WM_KILLFOCUS);

	SXMaterialEditor::EditUDVSY = SXGUICrEdit("0",325,480,80,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditUDVSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDVSY->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditUDVSY->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditUDVSY->setTransparentTextBk(true);
	SXMaterialEditor::EditUDVSY->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditUDVSY->addHandler(SXMaterialEditor_EditUDVSY_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditUDVSY->addHandler(SXMaterialEditor_EditUDVSY_Enter, WM_KILLFOCUS);

	
	SXMaterialEditor::StaticUDVSX = SXGUICrStatic("x:",305,460,15,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticUDVSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDVSX->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticUDVSX->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticUDVSX->setTransparentTextBk(true);
	SXMaterialEditor::StaticUDVSX->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::StaticUDVSY = SXGUICrStatic("y:",305,480,15,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticUDVSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDVSY->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticUDVSY->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticUDVSY->setTransparentTextBk(true);
	SXMaterialEditor::StaticUDVSY->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::StaticUDVSZ = SXGUICrStatic("z:",305,500,15,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticUDVSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDVSZ->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticUDVSZ->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticUDVSZ->setTransparentTextBk(true);
	SXMaterialEditor::StaticUDVSZ->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditUDVSZ = SXGUICrEdit("0",325,500,80,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditUDVSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDVSZ->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditUDVSZ->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditUDVSZ->setTransparentTextBk(true);
	SXMaterialEditor::EditUDVSZ->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditUDVSZ->addHandler(SXMaterialEditor_EditUDVSZ_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditUDVSZ->addHandler(SXMaterialEditor_EditUDVSZ_Enter, WM_KILLFOCUS);


	SXMaterialEditor::StaticUDVSW = SXGUICrStatic("w:",305,520,15,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticUDVSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDVSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticUDVSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticUDVSW->setTransparentTextBk(true);
	SXMaterialEditor::StaticUDVSW->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::EditUDVSW = SXGUICrEdit("0",325,520,80,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditUDVSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDVSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditUDVSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditUDVSW->setTransparentTextBk(true);
	SXMaterialEditor::EditUDVSW->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditUDVSW->addHandler(SXMaterialEditor_EditUDVSW_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditUDVSW->addHandler(SXMaterialEditor_EditUDVSW_Enter, WM_KILLFOCUS);

	SXMaterialEditor::TrackBarUDVSX = SXGUICrTrackBar("", 410, 460, 220, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDVSX->setMinMax(-100, 100);
	SXMaterialEditor::TrackBarUDVSX->setTickFrequency(10);
	SXMaterialEditor::TrackBarUDVSX->addHandler(SXMaterialEditor_TrackBarUDVSX_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDVSY = SXGUICrTrackBar("", 410, 480, 220, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDVSY->setMinMax(-100, 100);
	SXMaterialEditor::TrackBarUDVSY->setTickFrequency(10);
	SXMaterialEditor::TrackBarUDVSY->addHandler(SXMaterialEditor_TrackBarUDVSY_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDVSZ = SXGUICrTrackBar("", 410, 500, 220, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDVSZ->setMinMax(-100, 100);
	SXMaterialEditor::TrackBarUDVSZ->setTickFrequency(10);
	SXMaterialEditor::TrackBarUDVSZ->addHandler(SXMaterialEditor_TrackBarUDVSZ_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDVSW = SXGUICrTrackBar("",410,520,220,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::TrackBarUDVSW->setMinMax(-100, 100);
	SXMaterialEditor::TrackBarUDVSW->setTickFrequency(10);
	SXMaterialEditor::TrackBarUDVSW->addHandler(SXMaterialEditor_TrackBarUDVSW_MouseMove, WM_MOUSEMOVE);
	


	SXMaterialEditor::CheckBoxUDPS = SXGUICrCheckBox("User data PS:", 305, 550, 100, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxUDPS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::CheckBoxUDPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxUDPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxUDPS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxUDPS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::CheckBoxUDPSInVS = SXGUICrCheckBox("in VS", 445, 550, 100, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::CheckBoxUDPSInVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::CheckBoxUDPSInVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::CheckBoxUDPSInVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::CheckBoxUDPSInVS->setTransparentTextBk(true);
	SXMaterialEditor::CheckBoxUDPSInVS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::StaticUDPSX = SXGUICrStatic("x:",305,570,15,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticUDPSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDPSX->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticUDPSX->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticUDPSX->setTransparentTextBk(true);
	SXMaterialEditor::StaticUDPSX->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::StaticUDPSY = SXGUICrStatic("y:",305,590,15,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticUDPSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDPSY->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticUDPSY->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticUDPSY->setTransparentTextBk(true);
	SXMaterialEditor::StaticUDPSY->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::StaticUDPSZ = SXGUICrStatic("z:",305,610,15,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticUDPSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDPSZ->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticUDPSZ->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticUDPSZ->setTransparentTextBk(true);
	SXMaterialEditor::StaticUDPSZ->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::StaticUDPSW = SXGUICrStatic("w:",305,630,15,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::StaticUDPSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::StaticUDPSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::StaticUDPSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::StaticUDPSW->setTransparentTextBk(true);
	SXMaterialEditor::StaticUDPSW->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::EditUDPSX = SXGUICrEdit("0",325,570,80,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditUDPSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDPSX->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditUDPSX->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditUDPSX->setTransparentTextBk(true);
	SXMaterialEditor::EditUDPSX->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditUDPSX->addHandler(SXMaterialEditor_EditUDPSX_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditUDPSX->addHandler(SXMaterialEditor_EditUDPSX_Enter, WM_KILLFOCUS);

	SXMaterialEditor::EditUDPSY = SXGUICrEdit("0",325,590,80,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditUDPSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDPSY->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditUDPSY->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditUDPSY->setTransparentTextBk(true);
	SXMaterialEditor::EditUDPSY->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditUDPSY->addHandler(SXMaterialEditor_EditUDPSY_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditUDPSY->addHandler(SXMaterialEditor_EditUDPSY_Enter, WM_KILLFOCUS);

	SXMaterialEditor::EditUDPSZ = SXGUICrEdit("0",325,610,80,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditUDPSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDPSZ->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditUDPSZ->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditUDPSZ->setTransparentTextBk(true);
	SXMaterialEditor::EditUDPSZ->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditUDPSZ->addHandler(SXMaterialEditor_EditUDPSZ_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditUDPSZ->addHandler(SXMaterialEditor_EditUDPSZ_Enter, WM_KILLFOCUS);
	
	SXMaterialEditor::EditUDPSW = SXGUICrEdit("0",325,630,80,15,SXMaterialEditor::JobWindow->getHWND(),0,0);
	SXMaterialEditor::EditUDPSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::EditUDPSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::EditUDPSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::EditUDPSW->setTransparentTextBk(true);
	SXMaterialEditor::EditUDPSW->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::EditUDPSW->addHandler(SXMaterialEditor_EditUDPSW_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::EditUDPSW->addHandler(SXMaterialEditor_EditUDPSW_Enter, WM_KILLFOCUS);


	SXMaterialEditor::TrackBarUDPSX = SXGUICrTrackBar("", 410, 570, 220, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDPSX->setMinMax(-100, 100);
	SXMaterialEditor::TrackBarUDPSX->setTickFrequency(10);
	SXMaterialEditor::TrackBarUDPSX->addHandler(SXMaterialEditor_TrackBarUDPSX_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDPSY = SXGUICrTrackBar("", 410, 590, 220, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDPSY->setMinMax(-100, 100);
	SXMaterialEditor::TrackBarUDPSY->setTickFrequency(10);
	SXMaterialEditor::TrackBarUDPSY->addHandler(SXMaterialEditor_TrackBarUDPSY_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDPSZ = SXGUICrTrackBar("", 410, 610, 220, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDPSZ->setMinMax(-100, 100);
	SXMaterialEditor::TrackBarUDPSZ->setTickFrequency(10);
	SXMaterialEditor::TrackBarUDPSZ->addHandler(SXMaterialEditor_TrackBarUDPSZ_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::TrackBarUDPSW = SXGUICrTrackBar("", 410, 630, 220, 15, SXMaterialEditor::JobWindow->getHWND(), 0, 0);
	SXMaterialEditor::TrackBarUDPSW->setMinMax(-100, 100);
	SXMaterialEditor::TrackBarUDPSW->setTickFrequency(10);
	SXMaterialEditor::TrackBarUDPSW->addHandler(SXMaterialEditor_TrackBarUDPSW_MouseMove, WM_MOUSEMOVE);
	
}
void SXMaterialEditor::DeleteAllElements()
{
	mem_delete(SXMaterialEditor::WindowRender);
	mem_delete(SXMaterialEditor::ToolBar);
	mem_delete(SXMaterialEditor::StaticTypeModel);
	mem_delete(SXMaterialEditor::ComboBoxTypeModel);
	mem_delete(SXMaterialEditor::StaticTestModel);
	mem_delete(SXMaterialEditor::ComboBoxTestModel);
	mem_delete(SXMaterialEditor::StaticTex);
	mem_delete(SXMaterialEditor::EditTex);

	mem_delete(SXMaterialEditor::StaticShaders);
	mem_delete(SXMaterialEditor::ComboBoxShaders);

	mem_delete(SXMaterialEditor::StaticVS);
	mem_delete(SXMaterialEditor::EditVS);
	mem_delete(SXMaterialEditor::StaticPS);
	mem_delete(SXMaterialEditor::EditPS);
	mem_delete(SXMaterialEditor::ButtonTex);
	mem_delete(SXMaterialEditor::ButtonVS);
	mem_delete(SXMaterialEditor::ButtonPS);
	mem_delete(SXMaterialEditor::StaticMR);

	mem_delete(SXMaterialEditor::StaticParamL);
	mem_delete(SXMaterialEditor::ComboBoxParamL);

	mem_delete(SXMaterialEditor::StaticRoughness);
	mem_delete(SXMaterialEditor::StaticF0);
	mem_delete(SXMaterialEditor::StaticMG);
	mem_delete(SXMaterialEditor::EditRoughness);
	mem_delete(SXMaterialEditor::EditF0);
	mem_delete(SXMaterialEditor::EditMR);
	mem_delete(SXMaterialEditor::CheckBoxLighting);
	mem_delete(SXMaterialEditor::TrackBarRoughness);
	mem_delete(SXMaterialEditor::TrackBarF0);
	mem_delete(SXMaterialEditor::TrackBarUDVSX);
	mem_delete(SXMaterialEditor::StaticMaD);
	mem_delete(SXMaterialEditor::EditTexLighting);
	mem_delete(SXMaterialEditor::ButtonTexLighting);
	mem_delete(SXMaterialEditor::CheckBoxTexLighting);
	mem_delete(SXMaterialEditor::ComboBoxTypeRefract);
	mem_delete(SXMaterialEditor::CheckBoxDoSVVS);
	mem_delete(SXMaterialEditor::ComboBoxTypeReflect);
	mem_delete(SXMaterialEditor::StaticPhysic);
	mem_delete(SXMaterialEditor::ComboBoxPhysic);
	mem_delete(SXMaterialEditor::ButtonMR);
	mem_delete(SXMaterialEditor::StaticMB);
	mem_delete(SXMaterialEditor::StaticMA);
	mem_delete(SXMaterialEditor::StaticDR);
	mem_delete(SXMaterialEditor::EditMG);
	mem_delete(SXMaterialEditor::EditMB);
	mem_delete(SXMaterialEditor::EditMA);
	mem_delete(SXMaterialEditor::EditDR);
	mem_delete(SXMaterialEditor::ButtonMG);
	mem_delete(SXMaterialEditor::ButtonMB);
	mem_delete(SXMaterialEditor::ButtonMA);
	mem_delete(SXMaterialEditor::ButtonDR);
	mem_delete(SXMaterialEditor::StaticDG);
	mem_delete(SXMaterialEditor::StaticDB);
	mem_delete(SXMaterialEditor::StaticDA);
	mem_delete(SXMaterialEditor::EditDG);
	mem_delete(SXMaterialEditor::EditDB);
	mem_delete(SXMaterialEditor::EditDA);
	mem_delete(SXMaterialEditor::ButtonDG);
	mem_delete(SXMaterialEditor::ButtonDB);
	mem_delete(SXMaterialEditor::ButtonDA);
	mem_delete(SXMaterialEditor::StaticDoS);
	mem_delete(SXMaterialEditor::StaticDoSW);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWPS);
	mem_delete(SXMaterialEditor::StaticDoSV);
	mem_delete(SXMaterialEditor::CheckBoxDoSVPS);
	mem_delete(SXMaterialEditor::StaticDosP);
	mem_delete(SXMaterialEditor::CheckBoxDoSPVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSPPS);
	mem_delete(SXMaterialEditor::StaticBoxDoSWV);
	mem_delete(SXMaterialEditor::StaticDoSWVP);
	mem_delete(SXMaterialEditor::StaticDoSCampos);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVPS);
	mem_delete(SXMaterialEditor::CheckBoxDoSCamposVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVPVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWVPPS);
	mem_delete(SXMaterialEditor::CheckBoxDoSCamposPS);
	mem_delete(SXMaterialEditor::CheckBoxUDVS);
	mem_delete(SXMaterialEditor::CheckBoxUDVSInPS);
	mem_delete(SXMaterialEditor::EditUDVSX);
	mem_delete(SXMaterialEditor::EditUDVSY);
	mem_delete(SXMaterialEditor::TrackBarUDVSY);
	mem_delete(SXMaterialEditor::StaticUDVSX);
	mem_delete(SXMaterialEditor::StaticUDVSY);
	mem_delete(SXMaterialEditor::StaticUDVSZ);
	mem_delete(SXMaterialEditor::EditUDVSZ);
	mem_delete(SXMaterialEditor::TrackBarUDVSZ);
	mem_delete(SXMaterialEditor::StaticUDVSW);
	mem_delete(SXMaterialEditor::EditUDVSW);
	mem_delete(SXMaterialEditor::TrackBarUDVSW);
	mem_delete(SXMaterialEditor::StaticDoSTimeDelta);
	mem_delete(SXMaterialEditor::CheckBoxDoSTimeDeltaVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSTimeDeltaPS);
	mem_delete(SXMaterialEditor::CheckBoxUDPS);
	mem_delete(SXMaterialEditor::CheckBoxUDPSInVS);
	mem_delete(SXMaterialEditor::StaticUDPSX);
	mem_delete(SXMaterialEditor::StaticUDPSY);
	mem_delete(SXMaterialEditor::StaticUDPSZ);
	mem_delete(SXMaterialEditor::StaticUDPSW);
	mem_delete(SXMaterialEditor::EditUDPSX);
	mem_delete(SXMaterialEditor::EditUDPSY);
	mem_delete(SXMaterialEditor::TrackBarUDPSX);
	mem_delete(SXMaterialEditor::TrackBarUDPSY);
	mem_delete(SXMaterialEditor::EditUDPSZ);
	mem_delete(SXMaterialEditor::TrackBarUDPSZ);
	mem_delete(SXMaterialEditor::EditUDPSW);
	mem_delete(SXMaterialEditor::TrackBarUDPSW);
	mem_delete(SXMaterialEditor::StaticMask);
	mem_delete(SXMaterialEditor::EditMask);
	mem_delete(SXMaterialEditor::ButtonMask);
	mem_delete(SXMaterialEditor::StaticDoSWinSize);
	mem_delete(SXMaterialEditor::CheckBoxDoSWinSizeVS);
	mem_delete(SXMaterialEditor::CheckBoxDoSWinSizePS);
	mem_delete(SXMaterialEditor::CheckBoxAlphaTest);
	mem_delete(SXMaterialEditor::ButtonSkyBox);
	mem_delete(SXMaterialEditor::EditSkyBox);
	mem_delete(SXMaterialEditor::CheckBoxModelRot);
	mem_delete(SXMaterialEditor::ButtonRotAngle0);
	mem_delete(SXMaterialEditor::StaticThickness);
	mem_delete(SXMaterialEditor::EditThickness);
	mem_delete(SXMaterialEditor::TrackBarThickness);
	mem_delete(SXMaterialEditor::JobWindow);
}

//##########################################################################

void SXMaterialEditor::MaterialEditorUpdate(DWORD timeDelta)
{
	ID skit = -1;

	if (!SXMaterialEditor::ComboBoxShaders->getFocus())
	{
		char vs[1024];
		char ps[1024];

		SML_MtlGetVS(SXMaterialEditor::IDMat, vs);
		SML_MtlGetPS(SXMaterialEditor::IDMat, ps);

		if ((skit = SXMaterialEditor::Shaders->Find(vs, ps)) >= 0)
			SXMaterialEditor::ComboBoxShaders->setSel(skit + 1);
		else
			SXMaterialEditor::ComboBoxShaders->setSel(0);
	}

	if (!SXMaterialEditor::ComboBoxParamL->getFocus())
	{
		float thikcness = SML_MtlGetThickness(SXMaterialEditor::IDMat);
		float roughness = SML_MtlGetRoughness(SXMaterialEditor::IDMat);
		float f0 = SML_MtlGetF0(SXMaterialEditor::IDMat);

		skit = -1;
		if ((skit = SXMaterialEditor::ParamL->Find(thikcness, roughness, f0)) >= 0)
			SXMaterialEditor::ComboBoxParamL->setSel(skit + 1);
		else
			SXMaterialEditor::ComboBoxParamL->setSel(0);
	}

	if (SXMaterialEditor::CheckBoxModelRot->getCheck())
		SRender_SimModelSetRotationY(SRender_SimModelGetRotationY() - float(timeDelta) * 0.001f * 0.25);
}