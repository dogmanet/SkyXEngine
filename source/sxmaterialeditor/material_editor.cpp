
#include "material_editor.h"

namespace SXMaterialEditor
{
	ISXGUIBaseWnd *pJobWindow = 0;
	ISXGUIMenuWindow *pMainMenu = 0;
	ISXGUIBaseWnd *pWindowRender = 0;

	ISXGUIToolBar *pToolBar = 0;
	ISXGUIButton *pButtonTBOpen = 0;
	ISXGUIButton *pButtonTBSave = 0;

	ISXGUICheckBox *pCheckBoxTBCam = 0;
	ISXGUIButton *pButtonTBView = 0;

	ISXGUICheckBox *pCheckBoxTBRColor = 0;
	ISXGUICheckBox *pCheckBoxTBRNormal = 0;
	ISXGUICheckBox *pCheckBoxTBRParam = 0;
	ISXGUICheckBox *pCheckBoxTBRAmDiff = 0;
	ISXGUICheckBox *pCheckBoxTBRSpecular = 0;
	ISXGUICheckBox *pCheckBoxTBRLighting = 0;

	ISXGUIButton *pButtonSkyBox = 0;
	ISXGUIEdit *pEditSkyBox = 0;
	ISXGUICheckBox *pCheckBoxModelRot = 0;
	ISXGUIButton *pButtonRotAngle0 = 0;
	ISXGUIStatic *pStaticTypeModel = 0;
	ISXGUIComboBox *pComboBoxTypeModel = 0;

	ISXGUIStatic *pStaticLigthColorText = 0;
	ISXGUIStatic *pStaticLigthColorR = 0;
	ISXGUIEdit *pEditLigthColorR = 0;
	ISXGUIStatic *pStaticLigthColorG = 0;
	ISXGUIEdit *pEditLigthColorG = 0;
	ISXGUIStatic *pStaticLigthColorB = 0;
	ISXGUIEdit *pEditLigthColorB = 0;
	ISXGUIButton *pButtonLigthColor = 0;
	ISXGUIStatic *pStaticLigthColor = 0;

	ISXGUIStatic *pStaticPenetration = 0;
	ISXGUIStatic *pStaticHitChance = 0;
	ISXGUIEdit *pEditHitChance = 0;
	ISXGUITrackBar *pTrackBarHitChance = 0;
	ISXGUIStatic *pStaticDurability = 0;
	ISXGUIEdit *pEditDurability = 0;
	ISXGUIStatic *pStaticDensity = 0;
	ISXGUIEdit *pEditDensity = 0;

	ISXGUIStatic *pStaticTestModel = 0;
	ISXGUIComboBox *pComboBoxTestModel = 0;
	ISXGUIStatic *pStaticTex = 0;
	ISXGUIEdit *pEditTex = 0;
	ISXGUIStatic *pStaticVS = 0;
	ISXGUIEdit *pEditVS = 0;
	ISXGUIStatic *pStaticPS = 0;
	ISXGUIEdit *pEditPS = 0;
	ISXGUIButton *pButtonTex = 0;

	ISXGUIStatic *pStaticShaders = 0;
	ISXGUIComboBox *pComboBoxShaders = 0;

	ISXGUIButton *pButtonVS = 0;
	ISXGUIButton *pButtonPS = 0;
	ISXGUICheckBox *pCheckBoxAlphaTest = 0;
	ISXGUICheckBox *pCheckBoxTransparent = 0;
	ISXGUIStatic *pStaticMR = 0;
	ISXGUIStatic *pStaticRoughness = 0;
	ISXGUIStatic *pStaticF0 = 0;
	ISXGUIStatic *pStaticMG = 0;
	ISXGUIEdit *pEditRoughness = 0;
	ISXGUIEdit *pEditF0 = 0;
	ISXGUIEdit *pEditMR = 0;
	ISXGUICheckBox *pCheckBoxLighting = 0;
	ISXGUITrackBar *pTrackBarRoughness = 0;
	ISXGUITrackBar *pTrackBarF0 = 0;

	ISXGUIStatic *pStaticThickness = 0;
	ISXGUIEdit *pEditThickness = 0;
	ISXGUITrackBar *pTrackBarThickness = 0;

	ISXGUITrackBar *pTrackBarUDVSX = 0;
	ISXGUIStatic *pStaticMaD = 0;
	ISXGUIEdit *pEditTexLighting = 0;
	ISXGUIButton *pButtonTexLighting = 0;
	ISXGUICheckBox *pCheckBoxTexLighting = 0;

	ISXGUIStatic *pStaticParamL = 0;
	ISXGUIComboBox *pComboBoxParamL = 0;

	ISXGUICheckBox *pCheckBoxDoSVVS = 0;
	ISXGUIStatic *pStaticTypeReflect = 0;
	ISXGUIComboBox *pComboBoxTypeReflect = 0;
	ISXGUIStatic *pStaticPhysic = 0;
	ISXGUIComboBox *pComboBoxPhysic = 0;
	ISXGUIStatic *pStaticMask = 0;
	ISXGUIEdit *pEditMask = 0;
	ISXGUIButton *pButtonMask = 0;
	ISXGUIButton *pButtonMR = 0;
	ISXGUIStatic *pStaticMB = 0;
	ISXGUIStatic *pStaticMA = 0;
	ISXGUIStatic *pStaticDR = 0;
	ISXGUIEdit *pEditMG = 0;
	ISXGUIEdit *pEditMB = 0;
	ISXGUIEdit *pEditMA = 0;
	ISXGUIEdit *pEditDR = 0;
	ISXGUIButton *pButtonMG = 0;
	ISXGUIButton *pButtonMB = 0;
	ISXGUIButton *pButtonMA = 0;
	ISXGUIButton *pButtonDR = 0;
	ISXGUIStatic *pStaticDG = 0;
	ISXGUIStatic *pStaticDB = 0;
	ISXGUIStatic *pStaticDA = 0;
	ISXGUIEdit *pEditDG = 0;
	ISXGUIEdit *pEditDB = 0;
	ISXGUIEdit *pEditDA = 0;
	ISXGUIButton *pButtonDG = 0;
	ISXGUIButton *pButtonDB = 0;
	ISXGUIButton *pButtonDA = 0;
	ISXGUIStatic *pStaticDoS = 0;
	ISXGUIStatic *pStaticDoSW = 0;
	ISXGUICheckBox *pCheckBoxDoSWVS = 0;
	ISXGUICheckBox *pCheckBoxDoSWPS = 0;
	ISXGUIStatic *pStaticDoSV = 0;
	ISXGUICheckBox *pCheckBoxDoSVPS = 0;
	ISXGUIStatic *pStaticDosP = 0;
	ISXGUICheckBox *pCheckBoxDoSPVS = 0;
	ISXGUICheckBox *pCheckBoxDoSPPS = 0;
	ISXGUIStatic *pStaticBoxDoSWV = 0;
	ISXGUIStatic *pStaticDoSWVP = 0;
	ISXGUIStatic *pStaticDoSCampos = 0;
	ISXGUICheckBox *pCheckBoxDoSWVVS = 0;
	ISXGUICheckBox *pCheckBoxDoSWVPS = 0;
	ISXGUICheckBox *pCheckBoxDoSCamposVS = 0;
	ISXGUICheckBox *pCheckBoxDoSWVPVS = 0;
	ISXGUICheckBox *pCheckBoxDoSWVPPS = 0;
	ISXGUIStatic *pStaticDoSTimeDelta = 0;
	ISXGUICheckBox *pCheckBoxDoSCamposPS = 0;
	ISXGUIStatic *pStaticDoSWinSize = 0;
	ISXGUICheckBox *pCheckBoxDoSWinSizeVS = 0;
	ISXGUICheckBox *pCheckBoxDoSWinSizePS = 0;

	ISXGUICheckBox *pCheckBoxUDVS = 0;

	ISXGUICheckBox *pCheckBoxUDVSInPS = 0;
	ISXGUIEdit *pEditUDVSX = 0;
	ISXGUIEdit *pEditUDVSY = 0;
	ISXGUITrackBar *pTrackBarUDVSY = 0;
	ISXGUIStatic *pStaticUDVSX = 0;
	ISXGUIStatic *pStaticUDVSY = 0;
	ISXGUIStatic *pStaticUDVSZ = 0;
	ISXGUIEdit *pEditUDVSZ = 0;
	ISXGUITrackBar *pTrackBarUDVSZ = 0;
	ISXGUIStatic *pStaticUDVSW = 0;
	ISXGUIEdit *pEditUDVSW = 0;
	ISXGUITrackBar *pTrackBarUDVSW = 0;
	ISXGUIStatic * StaticDoSTimeDelta = 0;
	ISXGUICheckBox *pCheckBoxDoSTimeDeltaVS = 0;
	ISXGUICheckBox *pCheckBoxDoSTimeDeltaPS = 0;

	ISXGUICheckBox *pCheckBoxUDPS = 0;

	ISXGUICheckBox *pCheckBoxUDPSInVS = 0;
	ISXGUIStatic *pStaticUDPSX = 0;
	ISXGUIStatic *pStaticUDPSY = 0;
	ISXGUIStatic *pStaticUDPSZ = 0;
	ISXGUIStatic *pStaticUDPSW = 0;
	ISXGUIEdit *pEditUDPSX = 0;
	ISXGUIEdit *pEditUDPSY = 0;
	ISXGUITrackBar *pTrackBarUDPSX = 0;
	ISXGUITrackBar *pTrackBarUDPSY = 0;
	ISXGUIEdit *pEditUDPSZ = 0;
	ISXGUITrackBar *pTrackBarUDPSZ = 0;
	ISXGUIEdit *pEditUDPSW = 0;
	ISXGUITrackBar *pTrackBarUDPSW = 0;

	void InitAllElements();

	void DeleteAllElements();

	void Nulling();
	void InitMtl(ID id);

	void MtlOpen();
	void MtlSave();

	void FinalImageUncheckedMenu();

	ID idMat = -1;

	CShadersKit *pShaders = 0;
	CParamLigthKit *pParamL = 0;
};


void SXMaterialEditor::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int cx = (wrect.right - MAINWIN_SIZE_X) / 2;
	int cy = (wrect.bottom - MAINWIN_SIZE_Y) / 2;

	SXMaterialEditor::pJobWindow = SXGUICrBaseWndEx("SXMaterialEditor", "SXMaterialEditor", cx, cy, MAINWIN_SIZE_X, MAINWIN_SIZE_Y, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_DLGFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	SXMaterialEditor::pJobWindow->setVisible(false);
	gui_func::base_handlers::InitHandlerMsg(SXMaterialEditor::pJobWindow);
	SXMaterialEditor::pJobWindow->addHandler(SXMaterialEditor_JobWindow_CallWmCommand, WM_COMMAND);

	SXMaterialEditor::pJobWindow->addHandler(ComMenuId, WM_COMMAND);
	SXMaterialEditor::pJobWindow->addHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXMaterialEditor::pJobWindow->addHandler(MsgEditSize, WM_SIZE);
	SXMaterialEditor::pMainMenu = SXGUICrMenuWindowEx(IDR_MENU1);
	SXMaterialEditor::pMainMenu->setToWindow(SXMaterialEditor::pJobWindow->getHWND());

	SXMaterialEditor::pWindowRender = SXGUICrBaseWndEx("WindowRender", "WindowRender",0,26,300,300,0,0,CreateSolidBrush(RGB(200,200,200)),0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,SXMaterialEditor::pJobWindow->getHWND(),0);
	SXMaterialEditor::pWindowRender->setStretchSides(false, false, false, false);
	
	/*SXMaterialEditor::pWindowRender->GAlign.left = false;
	SXMaterialEditor::pWindowRender->GAlign.right = false;
	SXMaterialEditor::pWindowRender->GAlign.top = false;
	SXMaterialEditor::pWindowRender->GAlign.bottom = false;*/
	
	SXMaterialEditor::pToolBar = SXGUICrToolBar(0, 0, 645, 26, SXMaterialEditor::pJobWindow->getHWND(), WndProcAllDefault, 0);
	SXMaterialEditor::pToolBar->setStretchSides(false, false, false, false);
	
	/*SXMaterialEditor::pToolBar->GAlign.left = false;
	SXMaterialEditor::pToolBar->GAlign.right = false;
	SXMaterialEditor::pToolBar->GAlign.top = false;
	SXMaterialEditor::pToolBar->GAlign.bottom = false;*/
	SXMaterialEditor::pToolBar->addHandler(SXMaterialEditor_ToolBar_CallWmCommand, WM_COMMAND);

	SXMaterialEditor::pButtonTBOpen = SXGUICrButtonEx("", 2, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pButtonTBOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pButtonTBOpen->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pButtonTBOpen->GAlign.left = true;
	SXMaterialEditor::pButtonTBOpen->GAlign.top = true;*/
	SXMaterialEditor::pButtonTBOpen->setBmpFromResourse(IDB_BITMAP1);

	SXMaterialEditor::pButtonTBSave = SXGUICrButtonEx("", 26, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pButtonTBSave->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pButtonTBSave->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pButtonTBSave->GAlign.left = true;
	SXMaterialEditor::pButtonTBSave->GAlign.top = true;*/
	SXMaterialEditor::pButtonTBSave->setBmpFromResourse(IDB_BITMAP2);

	SXMaterialEditor::pCheckBoxTBCam = SXGUICrCheckBoxEx("", 54, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pCheckBoxTBCam->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxTBCam->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pCheckBoxTBCam->GAlign.left = true;
	SXMaterialEditor::pCheckBoxTBCam->GAlign.top = true;*/
	SXMaterialEditor::pCheckBoxTBCam->setBmpFromResourse(IDB_BITMAP3);

	SXMaterialEditor::pButtonTBView = SXGUICrButtonEx("", 78, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pButtonTBView->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pButtonTBView->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pButtonTBView->GAlign.left = true;
	SXMaterialEditor::pButtonTBView->GAlign.top = true;*/
	SXMaterialEditor::pButtonTBView->setBmpFromResourse(IDB_BITMAP4);

	SXMaterialEditor::pCheckBoxTBRColor = SXGUICrCheckBoxEx("", 106, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pCheckBoxTBRColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxTBRColor->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pCheckBoxTBRColor->GAlign.left = true;
	SXMaterialEditor::pCheckBoxTBRColor->GAlign.top = true;*/
	SXMaterialEditor::pCheckBoxTBRColor->setBmpFromResourse(IDB_BITMAP5);

	SXMaterialEditor::pCheckBoxTBRNormal = SXGUICrCheckBoxEx("", 130, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pCheckBoxTBRNormal->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxTBRNormal->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pCheckBoxTBRNormal->GAlign.left = true;
	SXMaterialEditor::pCheckBoxTBRNormal->GAlign.top = true;*/
	SXMaterialEditor::pCheckBoxTBRNormal->setBmpFromResourse(IDB_BITMAP6);

	SXMaterialEditor::pCheckBoxTBRParam = SXGUICrCheckBoxEx("", 154, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pCheckBoxTBRParam->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxTBRParam->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pCheckBoxTBRParam->GAlign.left = true;
	SXMaterialEditor::pCheckBoxTBRParam->GAlign.top = true;*/
	SXMaterialEditor::pCheckBoxTBRParam->setBmpFromResourse(IDB_BITMAP7);

	SXMaterialEditor::pCheckBoxTBRAmDiff = SXGUICrCheckBoxEx("", 178, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pCheckBoxTBRAmDiff->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxTBRAmDiff->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pCheckBoxTBRAmDiff->GAlign.left = true;
	SXMaterialEditor::pCheckBoxTBRAmDiff->GAlign.top = true;*/
	SXMaterialEditor::pCheckBoxTBRAmDiff->setBmpFromResourse(IDB_BITMAP8);

	SXMaterialEditor::pCheckBoxTBRSpecular = SXGUICrCheckBoxEx("", 202, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pCheckBoxTBRSpecular->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxTBRSpecular->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pCheckBoxTBRSpecular->GAlign.left = true;
	SXMaterialEditor::pCheckBoxTBRSpecular->GAlign.top = true;*/
	SXMaterialEditor::pCheckBoxTBRSpecular->setBmpFromResourse(IDB_BITMAP9);

	SXMaterialEditor::pCheckBoxTBRLighting = SXGUICrCheckBoxEx("", 226, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXMaterialEditor::pToolBar->getHWND(), 0, 0);
	SXMaterialEditor::pCheckBoxTBRLighting->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxTBRLighting->setStretchSides(true, false, false, true);
	/*SXMaterialEditor::pCheckBoxTBRLighting->GAlign.left = true;
	SXMaterialEditor::pCheckBoxTBRLighting->GAlign.top = true;*/
	SXMaterialEditor::pCheckBoxTBRLighting->setBmpFromResourse(IDB_BITMAP10);


	

	
	SXMaterialEditor::pButtonSkyBox = SXGUICrButton("...", 275, 330, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonSkyBox->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pButtonSkyBox->addHandler(SXMaterialEditor_ButtonSkyBox_Click, WM_LBUTTONUP);

	SXMaterialEditor::pEditSkyBox = SXGUICrEdit("0", 100, 330, 170, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditSkyBox->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditSkyBox->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditSkyBox->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditSkyBox->setTransparentTextBk(true);
	SXMaterialEditor::pEditSkyBox->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditSkyBox->addHandler(SXMaterialEditor_EditSkyBox_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditSkyBox->addHandler(SXMaterialEditor_EditSkyBox_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pCheckBoxModelRot = SXGUICrCheckBox("rotate", 5, 330, 50, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxModelRot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxModelRot->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxModelRot->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxModelRot->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxModelRot->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pButtonRotAngle0 = SXGUICrButton("0", 60, 330, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonRotAngle0->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pButtonRotAngle0->addHandler(SXMaterialEditor_ButtonRotAngle0_Click, WM_LBUTTONUP);

	SXMaterialEditor::pStaticTestModel = SXGUICrStatic("Test model:", 5, 350, 70, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticTestModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticTestModel->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticTestModel->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticTestModel->setTransparentTextBk(true);
	SXMaterialEditor::pStaticTestModel->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pComboBoxTestModel = SXGUICrComboBox(100, 350, 200, 70, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pComboBoxTestModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pComboBoxTestModel->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pComboBoxTestModel->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxTestModel->setTransparentTextBk(true);
	SXMaterialEditor::pComboBoxTestModel->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxTestModel->addItem("sphere");
	SXMaterialEditor::pComboBoxTestModel->addItem("cube");
	SXMaterialEditor::pComboBoxTestModel->addItem("plane");
	SXMaterialEditor::pComboBoxTestModel->setSel(0);



	SXMaterialEditor::pStaticLigthColorText = SXGUICrStatic("Color:", 5, 375, 35, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticLigthColorText->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticLigthColorText->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticLigthColorText->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticLigthColorText->setTransparentTextBk(true);
	SXMaterialEditor::pStaticLigthColorText->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pStaticLigthColorR = SXGUICrStatic("r:", 45, 375, 70, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticLigthColorR->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticLigthColorR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticLigthColorR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticLigthColorR->setTransparentTextBk(true);
	SXMaterialEditor::pStaticLigthColorR->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pEditLigthColorR = SXGUICrEdit("0", 55, 375, 55, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditLigthColorR->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditLigthColorR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditLigthColorR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditLigthColorR->setTransparentTextBk(true);
	SXMaterialEditor::pEditLigthColorR->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditLigthColorR->addHandler(SXMaterialEditor_EditLigthColorR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditLigthColorR->addHandler(SXMaterialEditor_EditLigthColorR_Enter, WM_KILLFOCUS);


	SXMaterialEditor::pStaticLigthColorG = SXGUICrStatic("g:", 115, 375, 70, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticLigthColorG->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticLigthColorG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticLigthColorG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticLigthColorG->setTransparentTextBk(true);
	SXMaterialEditor::pStaticLigthColorG->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pEditLigthColorG = SXGUICrEdit("0", 125, 375, 55, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditLigthColorG->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditLigthColorG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditLigthColorG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditLigthColorG->setTransparentTextBk(true);
	SXMaterialEditor::pEditLigthColorG->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditLigthColorG->addHandler(SXMaterialEditor_EditLigthColorG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditLigthColorG->addHandler(SXMaterialEditor_EditLigthColorG_Enter, WM_KILLFOCUS);


	SXMaterialEditor::pStaticLigthColorB = SXGUICrStatic("b:", 185, 375, 70, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticLigthColorB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticLigthColorB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticLigthColorB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticLigthColorB->setTransparentTextBk(true);
	SXMaterialEditor::pStaticLigthColorB->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pEditLigthColorB = SXGUICrEdit("0", 195, 375, 55, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditLigthColorB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditLigthColorB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditLigthColorB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditLigthColorB->setTransparentTextBk(true);
	SXMaterialEditor::pEditLigthColorB->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditLigthColorB->addHandler(SXMaterialEditor_EditLigthColorB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditLigthColorB->addHandler(SXMaterialEditor_EditLigthColorB_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pButtonLigthColor = SXGUICrButton("...", 275, 375, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonLigthColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pButtonLigthColor->addHandler(SXMaterialEditor_ButtonLigthColor_Click, WM_LBUTTONUP);

	SXMaterialEditor::pStaticLigthColor = SXGUICrStatic("", 5, 395, 295, 10, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticLigthColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticLigthColor->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticLigthColor->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticLigthColor->setTransparentTextBk(true);
	SXMaterialEditor::pStaticLigthColor->setColorBrush(RGB(220, 220, 220));


	
	SXMaterialEditor::pStaticTypeModel = SXGUICrStatic("Type model:",5,415,70,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticTypeModel->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticTypeModel->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticTypeModel->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticTypeModel->setTransparentTextBk(true);
	SXMaterialEditor::pStaticTypeModel->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pComboBoxTypeModel = SXGUICrComboBox(100,415,200,100,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pComboBoxTypeModel->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pComboBoxTypeModel->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pComboBoxTypeModel->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxTypeModel->setTransparentTextBk(true);
	SXMaterialEditor::pComboBoxTypeModel->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxTypeModel->addItem("geometry");
	SXMaterialEditor::pComboBoxTypeModel->addItem("grass");
	SXMaterialEditor::pComboBoxTypeModel->addItem("tree");
	SXMaterialEditor::pComboBoxTypeModel->addItem("skin");
	SXMaterialEditor::pComboBoxTypeModel->setSel(0);

	

	SXMaterialEditor::pStaticTex = SXGUICrStatic("Tex:",5,440,30,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticTex->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticTex->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticTex->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticTex->setTransparentTextBk(true);
	SXMaterialEditor::pStaticTex->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pEditTex = SXGUICrEdit("0",40,440,230,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditTex->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditTex->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditTex->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditTex->setTransparentTextBk(true);
	SXMaterialEditor::pEditTex->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditTex->addHandler(SXMaterialEditor_EditTex_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditTex->addHandler(SXMaterialEditor_EditTex_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pButtonTex = SXGUICrButton("...", 275, 440, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonTex->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pButtonTex->addHandler(SXMaterialEditor_ButtonTex_Click, WM_LBUTTONUP);



	SXMaterialEditor::pCheckBoxLighting = SXGUICrCheckBox("Lighting", 5, 460, 55, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxLighting->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxLighting->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxLighting->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxLighting->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxLighting->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pCheckBoxAlphaTest = SXGUICrCheckBox("Use AlphaTest", 65, 460, 90, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxAlphaTest->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxAlphaTest->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxAlphaTest->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxAlphaTest->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxAlphaTest->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pCheckBoxTransparent = SXGUICrCheckBox("Transparent", 160, 460, 90, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxTransparent->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxTransparent->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxTransparent->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxTransparent->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxTransparent->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::pStaticTypeReflect = SXGUICrStatic("Reflection:", 5, 480, 60, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticTypeReflect->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticTypeReflect->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticTypeReflect->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticTypeReflect->setTransparentTextBk(true);
	SXMaterialEditor::pStaticTypeReflect->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pComboBoxTypeReflect = SXGUICrComboBox(70, 478, 230, 115, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pComboBoxTypeReflect->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pComboBoxTypeReflect->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pComboBoxTypeReflect->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxTypeReflect->setTransparentTextBk(true);
	SXMaterialEditor::pComboBoxTypeReflect->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxTypeReflect->addItem("none");
	SXMaterialEditor::pComboBoxTypeReflect->addItem("reflect plane");
	SXMaterialEditor::pComboBoxTypeReflect->addItem("reflect cube dynamic");
	SXMaterialEditor::pComboBoxTypeReflect->addItem("reflect cube static");
	SXMaterialEditor::pComboBoxTypeReflect->setSel(0);



	SXMaterialEditor::pStaticShaders = SXGUICrStatic("Shaders:", 5, 515, 40, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticShaders->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticShaders->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticShaders->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticShaders->setTransparentTextBk(true);
	SXMaterialEditor::pStaticShaders->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pComboBoxShaders = SXGUICrComboBoxEx(60, 513, 240, 150, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pComboBoxShaders->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pComboBoxShaders->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pComboBoxShaders->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxShaders->setTransparentTextBk(true);
	SXMaterialEditor::pComboBoxShaders->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxShaders->addItem("");


	SXMaterialEditor::pStaticVS = SXGUICrStatic("VS:",5,535,30,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticVS->setTransparentTextBk(true);
	SXMaterialEditor::pStaticVS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditVS = SXGUICrEdit("0",40,535,230,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditVS->setTransparentTextBk(true);
	SXMaterialEditor::pEditVS->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditVS->addHandler(SXMaterialEditor_EditVS_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditVS->addHandler(SXMaterialEditor_EditVS_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pButtonVS = SXGUICrButton("...", 275, 535, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pButtonVS->addHandler(SXMaterialEditor_ButtonVS_Click, WM_LBUTTONUP);

	SXMaterialEditor::pStaticPS = SXGUICrStatic("PS:",5,555,30,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticPS->setTransparentTextBk(true);
	SXMaterialEditor::pStaticPS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditPS = SXGUICrEdit("0",40,555,230,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditPS->setTransparentTextBk(true);
	SXMaterialEditor::pEditPS->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditPS->addHandler(SXMaterialEditor_EditPS_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditPS->addHandler(SXMaterialEditor_EditPS_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pButtonPS = SXGUICrButton("...", 275, 555, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonPS->addHandler(SXMaterialEditor_ButtonPS_Click, WM_LBUTTONUP);


	SXMaterialEditor::pStaticPhysic = SXGUICrStatic("Physic material:", 5, 585, 80, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticPhysic->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticPhysic->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticPhysic->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticPhysic->setTransparentTextBk(true);
	SXMaterialEditor::pStaticPhysic->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pComboBoxPhysic = SXGUICrComboBox(100, 583, 200, 120, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pComboBoxPhysic->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pComboBoxPhysic->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pComboBoxPhysic->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxPhysic->setTransparentTextBk(true);
	SXMaterialEditor::pComboBoxPhysic->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxPhysic->addItem("concrete (бетон)");
	SXMaterialEditor::pComboBoxPhysic->addItem("metal (металл)");
	SXMaterialEditor::pComboBoxPhysic->addItem("glass (стекло)");
	SXMaterialEditor::pComboBoxPhysic->addItem("plastic (пластик)");
	SXMaterialEditor::pComboBoxPhysic->addItem("tree (дерево)");
	SXMaterialEditor::pComboBoxPhysic->addItem("flesh (плоть)");
	SXMaterialEditor::pComboBoxPhysic->addItem("ground/sand (земля/песок)");
	SXMaterialEditor::pComboBoxPhysic->addItem("water (вода)");
	SXMaterialEditor::pComboBoxPhysic->addItem("leaf/grass (листва/трава)");
	SXMaterialEditor::pComboBoxPhysic->setSel(0);

	SXMaterialEditor::pStaticHitChance = SXGUICrStatic("Hit chance:", 5, 605, 60, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticHitChance->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticHitChance->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticHitChance->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticHitChance->setTransparentTextBk(true);
	SXMaterialEditor::pStaticHitChance->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pEditHitChance = SXGUICrEdit("0", 60, 605, 65, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditHitChance->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditHitChance->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditHitChance->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditHitChance->setTransparentTextBk(true);
	SXMaterialEditor::pEditHitChance->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditHitChance->addHandler(SXMaterialEditor_EditHitChance_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditHitChance->addHandler(SXMaterialEditor_EditHitChance_Enter, WM_KILLFOCUS);
	SXMaterialEditor::pTrackBarHitChance = SXGUICrTrackBar("", 130, 605, 170, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarHitChance->setMinMax(0, 100);
	SXMaterialEditor::pTrackBarHitChance->setTickFrequency(10);
	SXMaterialEditor::pTrackBarHitChance->addHandler(SXMaterialEditor_TrackBarHitChance_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pStaticDurability = SXGUICrStatic("Durability:", 5, 625, 60, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticDurability->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticDurability->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDurability->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDurability->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDurability->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pEditDurability = SXGUICrEdit("0", 60, 625, 65, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditDurability->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditDurability->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditDurability->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditDurability->setTransparentTextBk(true);
	SXMaterialEditor::pEditDurability->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditDurability->addHandler(SXMaterialEditor_EditDurability_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditDurability->addHandler(SXMaterialEditor_EditDurability_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pStaticDensity = SXGUICrStatic("Density:", 135, 625, 60, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticDensity->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticDensity->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDensity->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDensity->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDensity->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pEditDensity = SXGUICrEdit("0", 180, 625, 65, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditDensity->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditDensity->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditDensity->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditDensity->setTransparentTextBk(true);
	SXMaterialEditor::pEditDensity->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditDensity->addHandler(SXMaterialEditor_EditDensity_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditDensity->addHandler(SXMaterialEditor_EditDensity_Enter, WM_KILLFOCUS);

	


	SXMaterialEditor::pStaticMR = SXGUICrStatic("M R:",305,175,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticMR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticMR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticMR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticMR->setTransparentTextBk(true);
	SXMaterialEditor::pStaticMR->setColorBrush(RGB(220, 220, 220));
	
	SXMaterialEditor::pStaticMG = SXGUICrStatic("M G:",305,195,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticMG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticMG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticMG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticMG->setTransparentTextBk(true);
	SXMaterialEditor::pStaticMG->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::pStaticParamL = SXGUICrStatic("Parameters Lighting:", 305, 30, 100, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticParamL->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticParamL->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticParamL->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticParamL->setTransparentTextBk(true);
	SXMaterialEditor::pStaticParamL->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pComboBoxParamL = SXGUICrComboBoxEx(400, 28, 230, 150, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pComboBoxParamL->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pComboBoxParamL->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pComboBoxParamL->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxParamL->setTransparentTextBk(true);
	SXMaterialEditor::pComboBoxParamL->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pComboBoxParamL->addItem("");

	SXMaterialEditor::pStaticThickness = SXGUICrStatic("Thickness:", 305, 50, 70, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticThickness->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticThickness->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticThickness->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticThickness->setTransparentTextBk(true);
	SXMaterialEditor::pStaticThickness->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditThickness = SXGUICrEdit("0", 375, 50, 50, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditThickness->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditThickness->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditThickness->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditThickness->setTransparentTextBk(true);
	SXMaterialEditor::pEditThickness->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditThickness->addHandler(SXMaterialEditor_EditThickness_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditThickness->addHandler(SXMaterialEditor_EditThickness_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pTrackBarThickness = SXGUICrTrackBar("", 430, 50, 200, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarThickness->setMinMax(0, 100);
	SXMaterialEditor::pTrackBarThickness->setTickFrequency(10);
	SXMaterialEditor::pTrackBarThickness->addHandler(SXMaterialEditor_TrackBarThickness_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pStaticRoughness = SXGUICrStatic("Roughness:", 305, 70, 70, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticRoughness->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticRoughness->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticRoughness->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticRoughness->setTransparentTextBk(true);
	SXMaterialEditor::pStaticRoughness->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditRoughness = SXGUICrEdit("0",375,70,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditRoughness->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditRoughness->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditRoughness->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditRoughness->setTransparentTextBk(true);
	SXMaterialEditor::pEditRoughness->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditRoughness->addHandler(SXMaterialEditor_EditRoughness_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditRoughness->addHandler(SXMaterialEditor_EditRoughness_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pTrackBarRoughness = SXGUICrTrackBar("", 430, 70, 200, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarRoughness->setMinMax(0, 100);
	SXMaterialEditor::pTrackBarRoughness->setTickFrequency(10);
	SXMaterialEditor::pTrackBarRoughness->addHandler(SXMaterialEditor_TrackBarRoughness_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pStaticF0 = SXGUICrStatic("f0:", 305, 90, 70, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticF0->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticF0->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticF0->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticF0->setTransparentTextBk(true);
	SXMaterialEditor::pStaticF0->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditF0 = SXGUICrEdit("0",375,90,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditF0->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditF0->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditF0->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditF0->setTransparentTextBk(true);
	SXMaterialEditor::pEditF0->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditF0->addHandler(SXMaterialEditor_EditF0_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditF0->addHandler(SXMaterialEditor_EditF0_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pTrackBarF0 = SXGUICrTrackBar("",430,90,200,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pTrackBarF0->setMinMax(0, 100);
	SXMaterialEditor::pTrackBarF0->setTickFrequency(10);
	SXMaterialEditor::pTrackBarF0->addHandler(SXMaterialEditor_TrackBarF0_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pEditTexLighting = SXGUICrEdit("0",370,110,230,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditTexLighting->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditTexLighting->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditTexLighting->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditTexLighting->setTransparentTextBk(true);
	SXMaterialEditor::pEditTexLighting->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditTexLighting->addHandler(SXMaterialEditor_EditTexLighting_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditTexLighting->addHandler(SXMaterialEditor_EditTexLighting_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pButtonTexLighting = SXGUICrButton("...", 605, 110, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonTexLighting->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonTexLighting->addHandler(SXMaterialEditor_ButtonTexLighting_Click, WM_LBUTTONUP);

	SXMaterialEditor::pCheckBoxTexLighting = SXGUICrCheckBox("texture:", 305, 110, 60, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxTexLighting->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxTexLighting->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxTexLighting->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxTexLighting->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxTexLighting->setColorBrush(RGB(220, 220, 220));
	

	SXMaterialEditor::pStaticMaD = SXGUICrStatic("Microrelief and detail", 305, 135, 100, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticMaD->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticMaD->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticMaD->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticMaD->setTransparentTextBk(true);
	SXMaterialEditor::pStaticMaD->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditMR = SXGUICrEdit("0", 360, 175, 240, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditMR->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditMR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditMR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditMR->setTransparentTextBk(true);
	SXMaterialEditor::pEditMR->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditMR->addHandler(SXMaterialEditor_EditMR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditMR->addHandler(SXMaterialEditor_EditMR_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pCheckBoxDoSVVS = SXGUICrCheckBox("vs", 340, 390, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSVVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSVVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSVVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSVVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSVVS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::pStaticMask = SXGUICrStatic("Mask:", 305, 152, 50, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticMask->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticMask->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticMask->setTransparentTextBk(true);
	SXMaterialEditor::pStaticMask->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditMask = SXGUICrEdit("0", 360, 152, 240, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pEditMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pEditMask->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditMask->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditMask->setTransparentTextBk(true);
	SXMaterialEditor::pEditMask->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditMask->addHandler(SXMaterialEditor_EditMask_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditMask->addHandler(SXMaterialEditor_EditMask_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pButtonMask = SXGUICrButton("...", 605, 152, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pButtonMask->addHandler(SXMaterialEditor_ButtonMask_Click, WM_LBUTTONUP);

	SXMaterialEditor::pButtonMR = SXGUICrButton("...", 605, 175, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonMR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonMR->addHandler(SXMaterialEditor_ButtonMR_Click, WM_LBUTTONUP);

	SXMaterialEditor::pStaticMB = SXGUICrStatic("M B:",305,215,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticMB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticMB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticMB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticMB->setTransparentTextBk(true);
	SXMaterialEditor::pStaticMB->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticMA = SXGUICrStatic("M A:",305,235,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticMA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticMA->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticMA->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticMA->setTransparentTextBk(true);
	SXMaterialEditor::pStaticMA->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticDR = SXGUICrStatic("D R:",305,260,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticDR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDR->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDR->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditMG = SXGUICrEdit("0",360,195,240,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditMG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditMG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditMG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditMG->setTransparentTextBk(true);
	SXMaterialEditor::pEditMG->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditMG->addHandler(SXMaterialEditor_EditMG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditMG->addHandler(SXMaterialEditor_EditMG_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pEditMB = SXGUICrEdit("0",360,215,240,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditMB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditMB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditMB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditMB->setTransparentTextBk(true);
	SXMaterialEditor::pEditMB->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditMB->addHandler(SXMaterialEditor_EditMB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditMB->addHandler(SXMaterialEditor_EditMB_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pEditMA = SXGUICrEdit("0",360,235,240,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditMA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditMA->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditMA->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditMA->setTransparentTextBk(true);
	SXMaterialEditor::pEditMA->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditMA->addHandler(SXMaterialEditor_EditMA_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditMA->addHandler(SXMaterialEditor_EditMA_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pEditDR = SXGUICrEdit("0",360,260,240,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditDR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditDR->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditDR->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditDR->setTransparentTextBk(true);
	SXMaterialEditor::pEditDR->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditDR->addHandler(SXMaterialEditor_EditDR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditDR->addHandler(SXMaterialEditor_EditDR_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pButtonMG = SXGUICrButton("...", 605, 195, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonMG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonMG->addHandler(SXMaterialEditor_ButtonMG_Click, WM_LBUTTONUP);

	SXMaterialEditor::pButtonMB = SXGUICrButton("...", 605, 215, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonMB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonMB->addHandler(SXMaterialEditor_ButtonMB_Click, WM_LBUTTONUP);

	SXMaterialEditor::pButtonMA = SXGUICrButton("...", 605, 235, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonMA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonMA->addHandler(SXMaterialEditor_ButtonMA_Click, WM_LBUTTONUP);

	SXMaterialEditor::pButtonDR = SXGUICrButton("...", 605, 260, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonDR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonDR->addHandler(SXMaterialEditor_ButtonDR_Click, WM_LBUTTONUP);

	SXMaterialEditor::pStaticDG = SXGUICrStatic("D G:",305,280,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticDG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDG->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDG->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticDB = SXGUICrStatic("D B:",305,300,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticDB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDB->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDB->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticDA = SXGUICrStatic("D A:",305,320,50,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticDA->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDA->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDA->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDA->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditDG = SXGUICrEdit("0",360,280,240,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditDG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditDG->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditDG->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditDG->setTransparentTextBk(true);
	SXMaterialEditor::pEditDG->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditDG->addHandler(SXMaterialEditor_EditDG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditDG->addHandler(SXMaterialEditor_EditDG_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pEditDB = SXGUICrEdit("0",360,300,240,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditDB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditDB->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditDB->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditDB->setTransparentTextBk(true);
	SXMaterialEditor::pEditDB->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditDB->addHandler(SXMaterialEditor_EditDB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditDB->addHandler(SXMaterialEditor_EditDB_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pEditDA = SXGUICrEdit("0",360,320,240,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditDA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditDA->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditDA->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditDA->setTransparentTextBk(true);
	SXMaterialEditor::pEditDA->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditDA->addHandler(SXMaterialEditor_EditDA_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditDA->addHandler(SXMaterialEditor_EditDA_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pButtonDG = SXGUICrButton("...", 605, 280, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonDG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonDG->addHandler(SXMaterialEditor_ButtonDG_Click, WM_LBUTTONUP);

	SXMaterialEditor::pButtonDB = SXGUICrButton("...", 605, 300, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonDB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonDB->addHandler(SXMaterialEditor_ButtonDB_Click, WM_LBUTTONUP);

	SXMaterialEditor::pButtonDA = SXGUICrButton("...", 605, 320, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pButtonDA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pButtonDA->addHandler(SXMaterialEditor_ButtonDA_Click, WM_LBUTTONUP);

	SXMaterialEditor::pStaticDoS = SXGUICrStatic("Data for shaders:",305,350,100,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDoS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticDoS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDoS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDoS->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDoS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticDoSW = SXGUICrStatic("W mat:",305,370,35,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDoSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticDoSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDoSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDoSW->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDoSW->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSWVS = SXGUICrCheckBox("vs", 340, 370, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSWVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSWVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSWVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSWVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSWVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSWPS = SXGUICrCheckBox("ps", 373, 370, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSWPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSWPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSWPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSWPS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSWPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticDoSV = SXGUICrStatic("V mat:",305,390,35,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDoSV->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticDoSV->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDoSV->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDoSV->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDoSV->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSVPS = SXGUICrCheckBox("ps", 373, 390, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSVPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSVPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSVPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSVPS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSVPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticDosP = SXGUICrStatic("P mat:",305,410,35,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDosP->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticDosP->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDosP->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDosP->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDosP->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSPVS = SXGUICrCheckBox("vs", 340, 410, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSPVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSPVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSPVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSPVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSPVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSPPS = SXGUICrCheckBox("ps", 373, 410, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSPPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSPPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSPPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSPPS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSPPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticBoxDoSWV = SXGUICrStatic("WV mat:",410,370,45,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticBoxDoSWV->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticBoxDoSWV->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticBoxDoSWV->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticBoxDoSWV->setTransparentTextBk(true);
	SXMaterialEditor::pStaticBoxDoSWV->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticDoSWVP = SXGUICrStatic("WVP:",410,390,45,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDoSWVP->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticDoSWVP->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDoSWVP->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDoSWVP->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDoSWVP->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pStaticDoSCampos = SXGUICrStatic("Campos:",410,410,45,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticDoSCampos->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticDoSCampos->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDoSCampos->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDoSCampos->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDoSCampos->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSWVVS = SXGUICrCheckBox("vs", 455, 370, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSWVVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSWVVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSWVVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSWVVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSWVVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSWVPS = SXGUICrCheckBox("ps", 488, 370, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSWVPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSWVPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSWVPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSWVPS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSWVPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSCamposVS = SXGUICrCheckBox("vs", 455, 410, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSCamposVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSCamposVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSCamposVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSCamposVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSCamposVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSWVPVS = SXGUICrCheckBox("vs", 455, 390, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSWVPVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSWVPVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSWVPVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSWVPVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSWVPVS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSWVPPS = SXGUICrCheckBox("ps", 488, 390, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSWVPPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSWVPPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSWVPPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSWVPPS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSWVPPS->setColorBrush(RGB(220, 220, 220));
	SXMaterialEditor::pCheckBoxDoSCamposPS = SXGUICrCheckBox("ps", 488, 411, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSCamposPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxDoSCamposPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSCamposPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSCamposPS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSCamposPS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pStaticDoSTimeDelta = SXGUICrStatic("count_td:", 525, 370, 45, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticDoSTimeDelta->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticDoSTimeDelta->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDoSTimeDelta->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDoSTimeDelta->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDoSTimeDelta->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pCheckBoxDoSTimeDeltaVS = SXGUICrCheckBox("vs", 570, 370, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSTimeDeltaVS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pCheckBoxDoSTimeDeltaPS = SXGUICrCheckBox("ps", 603, 370, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSTimeDeltaPS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::pStaticDoSWinSize = SXGUICrStatic("win size:", 525, 390, 40, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pStaticDoSWinSize->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pStaticDoSWinSize->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticDoSWinSize->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticDoSWinSize->setTransparentTextBk(true);
	SXMaterialEditor::pStaticDoSWinSize->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pCheckBoxDoSWinSizeVS = SXGUICrCheckBox("vs", 570, 390, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSWinSizeVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxDoSWinSizeVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSWinSizeVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSWinSizeVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSWinSizeVS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pCheckBoxDoSWinSizePS = SXGUICrCheckBox("ps", 603, 390, 30, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxDoSWinSizePS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxDoSWinSizePS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxDoSWinSizePS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxDoSWinSizePS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxDoSWinSizePS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::pCheckBoxUDVS = SXGUICrCheckBox("User data VS:", 305, 440, 100, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxUDVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxUDVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxUDVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxUDVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxUDVS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pCheckBoxUDVSInPS = SXGUICrCheckBox("in PS", 445, 440, 100, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxUDVSInPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxUDVSInPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxUDVSInPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxUDVSInPS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxUDVSInPS->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditUDVSX = SXGUICrEdit("0",325,460,80,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditUDVSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditUDVSX->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditUDVSX->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDVSX->setTransparentTextBk(true);
	SXMaterialEditor::pEditUDVSX->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDVSX->addHandler(SXMaterialEditor_EditUDVSX_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditUDVSX->addHandler(SXMaterialEditor_EditUDVSX_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pEditUDVSY = SXGUICrEdit("0",325,480,80,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditUDVSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditUDVSY->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditUDVSY->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDVSY->setTransparentTextBk(true);
	SXMaterialEditor::pEditUDVSY->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDVSY->addHandler(SXMaterialEditor_EditUDVSY_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditUDVSY->addHandler(SXMaterialEditor_EditUDVSY_Enter, WM_KILLFOCUS);

	
	SXMaterialEditor::pStaticUDVSX = SXGUICrStatic("x:",305,460,15,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticUDVSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticUDVSX->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticUDVSX->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticUDVSX->setTransparentTextBk(true);
	SXMaterialEditor::pStaticUDVSX->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pStaticUDVSY = SXGUICrStatic("y:",305,480,15,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticUDVSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticUDVSY->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticUDVSY->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticUDVSY->setTransparentTextBk(true);
	SXMaterialEditor::pStaticUDVSY->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pStaticUDVSZ = SXGUICrStatic("z:",305,500,15,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticUDVSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticUDVSZ->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticUDVSZ->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticUDVSZ->setTransparentTextBk(true);
	SXMaterialEditor::pStaticUDVSZ->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditUDVSZ = SXGUICrEdit("0",325,500,80,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditUDVSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditUDVSZ->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditUDVSZ->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDVSZ->setTransparentTextBk(true);
	SXMaterialEditor::pEditUDVSZ->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDVSZ->addHandler(SXMaterialEditor_EditUDVSZ_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditUDVSZ->addHandler(SXMaterialEditor_EditUDVSZ_Enter, WM_KILLFOCUS);


	SXMaterialEditor::pStaticUDVSW = SXGUICrStatic("w:",305,520,15,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticUDVSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticUDVSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticUDVSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticUDVSW->setTransparentTextBk(true);
	SXMaterialEditor::pStaticUDVSW->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pEditUDVSW = SXGUICrEdit("0",325,520,80,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditUDVSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditUDVSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditUDVSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDVSW->setTransparentTextBk(true);
	SXMaterialEditor::pEditUDVSW->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDVSW->addHandler(SXMaterialEditor_EditUDVSW_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditUDVSW->addHandler(SXMaterialEditor_EditUDVSW_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pTrackBarUDVSX = SXGUICrTrackBar("", 410, 460, 220, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarUDVSX->setMinMax(-100, 100);
	SXMaterialEditor::pTrackBarUDVSX->setTickFrequency(10);
	SXMaterialEditor::pTrackBarUDVSX->addHandler(SXMaterialEditor_TrackBarUDVSX_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pTrackBarUDVSY = SXGUICrTrackBar("", 410, 480, 220, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarUDVSY->setMinMax(-100, 100);
	SXMaterialEditor::pTrackBarUDVSY->setTickFrequency(10);
	SXMaterialEditor::pTrackBarUDVSY->addHandler(SXMaterialEditor_TrackBarUDVSY_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pTrackBarUDVSZ = SXGUICrTrackBar("", 410, 500, 220, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarUDVSZ->setMinMax(-100, 100);
	SXMaterialEditor::pTrackBarUDVSZ->setTickFrequency(10);
	SXMaterialEditor::pTrackBarUDVSZ->addHandler(SXMaterialEditor_TrackBarUDVSZ_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pTrackBarUDVSW = SXGUICrTrackBar("",410,520,220,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pTrackBarUDVSW->setMinMax(-100, 100);
	SXMaterialEditor::pTrackBarUDVSW->setTickFrequency(10);
	SXMaterialEditor::pTrackBarUDVSW->addHandler(SXMaterialEditor_TrackBarUDVSW_MouseMove, WM_MOUSEMOVE);
	


	SXMaterialEditor::pCheckBoxUDPS = SXGUICrCheckBox("User data PS:", 305, 550, 100, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxUDPS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXMaterialEditor::pCheckBoxUDPS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxUDPS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxUDPS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxUDPS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::pCheckBoxUDPSInVS = SXGUICrCheckBox("in VS", 445, 550, 100, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0, false);
	SXMaterialEditor::pCheckBoxUDPSInVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pCheckBoxUDPSInVS->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pCheckBoxUDPSInVS->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pCheckBoxUDPSInVS->setTransparentTextBk(true);
	SXMaterialEditor::pCheckBoxUDPSInVS->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::pStaticUDPSX = SXGUICrStatic("x:",305,570,15,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticUDPSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticUDPSX->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticUDPSX->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticUDPSX->setTransparentTextBk(true);
	SXMaterialEditor::pStaticUDPSX->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pStaticUDPSY = SXGUICrStatic("y:",305,590,15,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticUDPSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticUDPSY->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticUDPSY->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticUDPSY->setTransparentTextBk(true);
	SXMaterialEditor::pStaticUDPSY->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pStaticUDPSZ = SXGUICrStatic("z:",305,610,15,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticUDPSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticUDPSZ->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticUDPSZ->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticUDPSZ->setTransparentTextBk(true);
	SXMaterialEditor::pStaticUDPSZ->setColorBrush(RGB(220, 220, 220));

	SXMaterialEditor::pStaticUDPSW = SXGUICrStatic("w:",305,630,15,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pStaticUDPSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pStaticUDPSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pStaticUDPSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pStaticUDPSW->setTransparentTextBk(true);
	SXMaterialEditor::pStaticUDPSW->setColorBrush(RGB(220, 220, 220));


	SXMaterialEditor::pEditUDPSX = SXGUICrEdit("0",325,570,80,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditUDPSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditUDPSX->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditUDPSX->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDPSX->setTransparentTextBk(true);
	SXMaterialEditor::pEditUDPSX->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDPSX->addHandler(SXMaterialEditor_EditUDPSX_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditUDPSX->addHandler(SXMaterialEditor_EditUDPSX_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pEditUDPSY = SXGUICrEdit("0",325,590,80,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditUDPSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditUDPSY->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditUDPSY->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDPSY->setTransparentTextBk(true);
	SXMaterialEditor::pEditUDPSY->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDPSY->addHandler(SXMaterialEditor_EditUDPSY_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditUDPSY->addHandler(SXMaterialEditor_EditUDPSY_Enter, WM_KILLFOCUS);

	SXMaterialEditor::pEditUDPSZ = SXGUICrEdit("0",325,610,80,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditUDPSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditUDPSZ->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditUDPSZ->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDPSZ->setTransparentTextBk(true);
	SXMaterialEditor::pEditUDPSZ->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDPSZ->addHandler(SXMaterialEditor_EditUDPSZ_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditUDPSZ->addHandler(SXMaterialEditor_EditUDPSZ_Enter, WM_KILLFOCUS);
	
	SXMaterialEditor::pEditUDPSW = SXGUICrEdit("0",325,630,80,15,SXMaterialEditor::pJobWindow->getHWND(),0,0);
	SXMaterialEditor::pEditUDPSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXMaterialEditor::pEditUDPSW->setColorText(RGB(0, 0, 0));
	SXMaterialEditor::pEditUDPSW->setColorTextBk(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDPSW->setTransparentTextBk(true);
	SXMaterialEditor::pEditUDPSW->setColorBrush(RGB(255, 255, 255));
	SXMaterialEditor::pEditUDPSW->addHandler(SXMaterialEditor_EditUDPSW_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXMaterialEditor::pEditUDPSW->addHandler(SXMaterialEditor_EditUDPSW_Enter, WM_KILLFOCUS);


	SXMaterialEditor::pTrackBarUDPSX = SXGUICrTrackBar("", 410, 570, 220, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarUDPSX->setMinMax(-100, 100);
	SXMaterialEditor::pTrackBarUDPSX->setTickFrequency(10);
	SXMaterialEditor::pTrackBarUDPSX->addHandler(SXMaterialEditor_TrackBarUDPSX_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pTrackBarUDPSY = SXGUICrTrackBar("", 410, 590, 220, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarUDPSY->setMinMax(-100, 100);
	SXMaterialEditor::pTrackBarUDPSY->setTickFrequency(10);
	SXMaterialEditor::pTrackBarUDPSY->addHandler(SXMaterialEditor_TrackBarUDPSY_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pTrackBarUDPSZ = SXGUICrTrackBar("", 410, 610, 220, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarUDPSZ->setMinMax(-100, 100);
	SXMaterialEditor::pTrackBarUDPSZ->setTickFrequency(10);
	SXMaterialEditor::pTrackBarUDPSZ->addHandler(SXMaterialEditor_TrackBarUDPSZ_MouseMove, WM_MOUSEMOVE);

	SXMaterialEditor::pTrackBarUDPSW = SXGUICrTrackBar("", 410, 630, 220, 15, SXMaterialEditor::pJobWindow->getHWND(), 0, 0);
	SXMaterialEditor::pTrackBarUDPSW->setMinMax(-100, 100);
	SXMaterialEditor::pTrackBarUDPSW->setTickFrequency(10);
	SXMaterialEditor::pTrackBarUDPSW->addHandler(SXMaterialEditor_TrackBarUDPSW_MouseMove, WM_MOUSEMOVE);
	
}
void SXMaterialEditor::DeleteAllElements()
{
	mem_delete(SXMaterialEditor::pWindowRender);
	mem_delete(SXMaterialEditor::pToolBar);
	mem_delete(SXMaterialEditor::pStaticTypeModel);
	mem_delete(SXMaterialEditor::pComboBoxTypeModel);
	mem_delete(SXMaterialEditor::pStaticTestModel);
	mem_delete(SXMaterialEditor::pComboBoxTestModel);
	mem_delete(SXMaterialEditor::pStaticTex);
	mem_delete(SXMaterialEditor::pEditTex);

	mem_delete(SXMaterialEditor::pStaticLigthColorText);
	mem_delete(SXMaterialEditor::pStaticLigthColorR);
	mem_delete(SXMaterialEditor::pEditLigthColorR);
	mem_delete(SXMaterialEditor::pStaticLigthColorG);
	mem_delete(SXMaterialEditor::pEditLigthColorG);
	mem_delete(SXMaterialEditor::pStaticLigthColorB);
	mem_delete(SXMaterialEditor::pEditLigthColorB);
	mem_delete(SXMaterialEditor::pButtonLigthColor);
	mem_delete(SXMaterialEditor::pStaticLigthColor);

	mem_delete(SXMaterialEditor::pStaticShaders);
	mem_delete(SXMaterialEditor::pComboBoxShaders);

	mem_delete(SXMaterialEditor::pStaticVS);
	mem_delete(SXMaterialEditor::pEditVS);
	mem_delete(SXMaterialEditor::pStaticPS);
	mem_delete(SXMaterialEditor::pEditPS);
	mem_delete(SXMaterialEditor::pButtonTex);
	mem_delete(SXMaterialEditor::pButtonVS);
	mem_delete(SXMaterialEditor::pButtonPS);
	mem_delete(SXMaterialEditor::pStaticMR);

	mem_delete(SXMaterialEditor::pStaticParamL);
	mem_delete(SXMaterialEditor::pComboBoxParamL);

	mem_delete(SXMaterialEditor::pStaticRoughness);
	mem_delete(SXMaterialEditor::pStaticF0);
	mem_delete(SXMaterialEditor::pStaticMG);
	mem_delete(SXMaterialEditor::pEditRoughness);
	mem_delete(SXMaterialEditor::pEditF0);
	mem_delete(SXMaterialEditor::pEditMR);
	mem_delete(SXMaterialEditor::pCheckBoxLighting);
	mem_delete(SXMaterialEditor::pCheckBoxTransparent);
	mem_delete(SXMaterialEditor::pTrackBarRoughness);
	mem_delete(SXMaterialEditor::pTrackBarF0);
	mem_delete(SXMaterialEditor::pTrackBarUDVSX);
	mem_delete(SXMaterialEditor::pStaticMaD);
	mem_delete(SXMaterialEditor::pEditTexLighting);
	mem_delete(SXMaterialEditor::pButtonTexLighting);
	mem_delete(SXMaterialEditor::pCheckBoxTexLighting);
	mem_delete(SXMaterialEditor::pCheckBoxDoSVVS);
	mem_delete(SXMaterialEditor::pComboBoxTypeReflect);
	mem_delete(SXMaterialEditor::pStaticPhysic);
	mem_delete(SXMaterialEditor::pComboBoxPhysic);
	mem_delete(SXMaterialEditor::pButtonMR);
	mem_delete(SXMaterialEditor::pStaticMB);
	mem_delete(SXMaterialEditor::pStaticMA);
	mem_delete(SXMaterialEditor::pStaticDR);
	mem_delete(SXMaterialEditor::pEditMG);
	mem_delete(SXMaterialEditor::pEditMB);
	mem_delete(SXMaterialEditor::pEditMA);
	mem_delete(SXMaterialEditor::pEditDR);
	mem_delete(SXMaterialEditor::pButtonMG);
	mem_delete(SXMaterialEditor::pButtonMB);
	mem_delete(SXMaterialEditor::pButtonMA);
	mem_delete(SXMaterialEditor::pButtonDR);
	mem_delete(SXMaterialEditor::pStaticDG);
	mem_delete(SXMaterialEditor::pStaticDB);
	mem_delete(SXMaterialEditor::pStaticDA);
	mem_delete(SXMaterialEditor::pEditDG);
	mem_delete(SXMaterialEditor::pEditDB);
	mem_delete(SXMaterialEditor::pEditDA);
	mem_delete(SXMaterialEditor::pButtonDG);
	mem_delete(SXMaterialEditor::pButtonDB);
	mem_delete(SXMaterialEditor::pButtonDA);
	mem_delete(SXMaterialEditor::pStaticDoS);
	mem_delete(SXMaterialEditor::pStaticDoSW);
	mem_delete(SXMaterialEditor::pCheckBoxDoSWVS);
	mem_delete(SXMaterialEditor::pCheckBoxDoSWPS);
	mem_delete(SXMaterialEditor::pStaticDoSV);
	mem_delete(SXMaterialEditor::pCheckBoxDoSVPS);
	mem_delete(SXMaterialEditor::pStaticDosP);
	mem_delete(SXMaterialEditor::pCheckBoxDoSPVS);
	mem_delete(SXMaterialEditor::pCheckBoxDoSPPS);
	mem_delete(SXMaterialEditor::pStaticBoxDoSWV);
	mem_delete(SXMaterialEditor::pStaticDoSWVP);
	mem_delete(SXMaterialEditor::pStaticDoSCampos);
	mem_delete(SXMaterialEditor::pCheckBoxDoSWVVS);
	mem_delete(SXMaterialEditor::pCheckBoxDoSWVPS);
	mem_delete(SXMaterialEditor::pCheckBoxDoSCamposVS);
	mem_delete(SXMaterialEditor::pCheckBoxDoSWVPVS);
	mem_delete(SXMaterialEditor::pCheckBoxDoSWVPPS);
	mem_delete(SXMaterialEditor::pCheckBoxDoSCamposPS);
	mem_delete(SXMaterialEditor::pCheckBoxUDVS);
	mem_delete(SXMaterialEditor::pCheckBoxUDVSInPS);
	mem_delete(SXMaterialEditor::pEditUDVSX);
	mem_delete(SXMaterialEditor::pEditUDVSY);
	mem_delete(SXMaterialEditor::pTrackBarUDVSY);
	mem_delete(SXMaterialEditor::pStaticUDVSX);
	mem_delete(SXMaterialEditor::pStaticUDVSY);
	mem_delete(SXMaterialEditor::pStaticUDVSZ);
	mem_delete(SXMaterialEditor::pEditUDVSZ);
	mem_delete(SXMaterialEditor::pTrackBarUDVSZ);
	mem_delete(SXMaterialEditor::pStaticUDVSW);
	mem_delete(SXMaterialEditor::pEditUDVSW);
	mem_delete(SXMaterialEditor::pTrackBarUDVSW);
	mem_delete(SXMaterialEditor::pStaticDoSTimeDelta);
	mem_delete(SXMaterialEditor::pCheckBoxDoSTimeDeltaVS);
	mem_delete(SXMaterialEditor::pCheckBoxDoSTimeDeltaPS);
	mem_delete(SXMaterialEditor::pCheckBoxUDPS);
	mem_delete(SXMaterialEditor::pCheckBoxUDPSInVS);
	mem_delete(SXMaterialEditor::pStaticUDPSX);
	mem_delete(SXMaterialEditor::pStaticUDPSY);
	mem_delete(SXMaterialEditor::pStaticUDPSZ);
	mem_delete(SXMaterialEditor::pStaticUDPSW);
	mem_delete(SXMaterialEditor::pEditUDPSX);
	mem_delete(SXMaterialEditor::pEditUDPSY);
	mem_delete(SXMaterialEditor::pTrackBarUDPSX);
	mem_delete(SXMaterialEditor::pTrackBarUDPSY);
	mem_delete(SXMaterialEditor::pEditUDPSZ);
	mem_delete(SXMaterialEditor::pTrackBarUDPSZ);
	mem_delete(SXMaterialEditor::pEditUDPSW);
	mem_delete(SXMaterialEditor::pTrackBarUDPSW);
	mem_delete(SXMaterialEditor::pStaticMask);
	mem_delete(SXMaterialEditor::pEditMask);
	mem_delete(SXMaterialEditor::pButtonMask);
	mem_delete(SXMaterialEditor::pStaticDoSWinSize);
	mem_delete(SXMaterialEditor::pCheckBoxDoSWinSizeVS);
	mem_delete(SXMaterialEditor::pCheckBoxDoSWinSizePS);
	mem_delete(SXMaterialEditor::pCheckBoxAlphaTest);
	mem_delete(SXMaterialEditor::pButtonSkyBox);
	mem_delete(SXMaterialEditor::pEditSkyBox);
	mem_delete(SXMaterialEditor::pCheckBoxModelRot);
	mem_delete(SXMaterialEditor::pButtonRotAngle0);
	mem_delete(SXMaterialEditor::pStaticThickness);
	mem_delete(SXMaterialEditor::pEditThickness);
	mem_delete(SXMaterialEditor::pTrackBarThickness);
	mem_delete(SXMaterialEditor::pJobWindow);
}

//##########################################################################

void SXMaterialEditor::MaterialEditorUpdate(DWORD timeDelta)
{
	ID skit = -1;

	if (!SXMaterialEditor::pComboBoxShaders->getFocus())
	{
		char vs[1024];
		char ps[1024];

		SML_MtlGetVS(SXMaterialEditor::idMat, vs);
		SML_MtlGetPS(SXMaterialEditor::idMat, ps);

		if ((skit = SXMaterialEditor::pShaders->find(vs, ps)) >= 0)
			SXMaterialEditor::pComboBoxShaders->setSel(skit + 1);
		else
			SXMaterialEditor::pComboBoxShaders->setSel(0);
	}

	if (!SXMaterialEditor::pComboBoxParamL->getFocus())
	{
		float thikcness = SML_MtlGetThickness(SXMaterialEditor::idMat);
		float roughness = SML_MtlGetRoughness(SXMaterialEditor::idMat);
		float f0 = SML_MtlGetF0(SXMaterialEditor::idMat);

		skit = -1;
		if ((skit = SXMaterialEditor::pParamL->find(thikcness, roughness, f0)) >= 0)
			SXMaterialEditor::pComboBoxParamL->setSel(skit + 1);
		else
			SXMaterialEditor::pComboBoxParamL->setSel(0);
	}

	if (SXMaterialEditor::pCheckBoxModelRot->getCheck())
		SRender_SimModelSetRotationY(SRender_SimModelGetRotationY() - float(timeDelta) * 0.001f * 0.25);
}