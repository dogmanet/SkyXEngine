
#include "material_editor.h"

namespace material_editor
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
	ISXGUICheckBox *pCheckBoxDestColor;
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


void material_editor::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int cx = (wrect.right - MAINWIN_SIZE_X) / 2;
	int cy = (wrect.bottom - MAINWIN_SIZE_Y) / 2;

	material_editor::pJobWindow = SXGUICrBaseWndEx("material_editor", (String(SX_MATERIAL_EDITOR_NAME) + " | " + SKYXENGINE_VERSION4EDITORS).c_str(), cx, cy, MAINWIN_SIZE_X, MAINWIN_SIZE_Y, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_DLGFRAME | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	material_editor::pJobWindow->setVisible(false);
	gui_func::base_handlers::InitHandlerMsg(material_editor::pJobWindow);
	material_editor::pJobWindow->addHandler(SXMaterialEditor_JobWindow_CallWmCommand, WM_COMMAND);

	material_editor::pJobWindow->addHandler(ComMenuId, WM_COMMAND);
	material_editor::pJobWindow->addHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	material_editor::pJobWindow->addHandler(MsgEditSize, WM_SIZE);
	material_editor::pMainMenu = SXGUICrMenuWindowEx(IDR_MENU1);
	material_editor::pMainMenu->setToWindow(material_editor::pJobWindow->getHWND());

	material_editor::pWindowRender = SXGUICrBaseWndEx("WindowRender", "WindowRender",0,26,300,300,0,0,CreateSolidBrush(RGB(200,200,200)),0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,material_editor::pJobWindow->getHWND(),0);
	material_editor::pWindowRender->setStretchSides(false, false, false, false);
	
	/*material_editor::pWindowRender->GAlign.left = false;
	material_editor::pWindowRender->GAlign.right = false;
	material_editor::pWindowRender->GAlign.top = false;
	material_editor::pWindowRender->GAlign.bottom = false;*/
	
	material_editor::pToolBar = SXGUICrToolBar(0, 0, 645, 26, material_editor::pJobWindow->getHWND(), WndProcAllDefault, 0);
	material_editor::pToolBar->setStretchSides(false, false, false, false);
	
	/*material_editor::pToolBar->GAlign.left = false;
	material_editor::pToolBar->GAlign.right = false;
	material_editor::pToolBar->GAlign.top = false;
	material_editor::pToolBar->GAlign.bottom = false;*/
	material_editor::pToolBar->addHandler(SXMaterialEditor_ToolBar_CallWmCommand, WM_COMMAND);

	material_editor::pButtonTBOpen = SXGUICrButtonEx("", 2, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pButtonTBOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pButtonTBOpen->setStretchSides(true, false, false, true);
	/*material_editor::pButtonTBOpen->GAlign.left = true;
	material_editor::pButtonTBOpen->GAlign.top = true;*/
	material_editor::pButtonTBOpen->setBmpFromResourse(IDB_BITMAP1);

	material_editor::pButtonTBSave = SXGUICrButtonEx("", 26, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pButtonTBSave->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pButtonTBSave->setStretchSides(true, false, false, true);
	/*material_editor::pButtonTBSave->GAlign.left = true;
	material_editor::pButtonTBSave->GAlign.top = true;*/
	material_editor::pButtonTBSave->setBmpFromResourse(IDB_BITMAP2);

	material_editor::pCheckBoxTBCam = SXGUICrCheckBoxEx("", 54, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pCheckBoxTBCam->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxTBCam->setStretchSides(true, false, false, true);
	/*material_editor::pCheckBoxTBCam->GAlign.left = true;
	material_editor::pCheckBoxTBCam->GAlign.top = true;*/
	material_editor::pCheckBoxTBCam->setBmpFromResourse(IDB_BITMAP3);

	material_editor::pButtonTBView = SXGUICrButtonEx("", 78, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pButtonTBView->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pButtonTBView->setStretchSides(true, false, false, true);
	/*material_editor::pButtonTBView->GAlign.left = true;
	material_editor::pButtonTBView->GAlign.top = true;*/
	material_editor::pButtonTBView->setBmpFromResourse(IDB_BITMAP4);

	material_editor::pCheckBoxTBRColor = SXGUICrCheckBoxEx("", 106, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pCheckBoxTBRColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxTBRColor->setStretchSides(true, false, false, true);
	/*material_editor::pCheckBoxTBRColor->GAlign.left = true;
	material_editor::pCheckBoxTBRColor->GAlign.top = true;*/
	material_editor::pCheckBoxTBRColor->setBmpFromResourse(IDB_BITMAP5);

	material_editor::pCheckBoxTBRNormal = SXGUICrCheckBoxEx("", 130, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pCheckBoxTBRNormal->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxTBRNormal->setStretchSides(true, false, false, true);
	/*material_editor::pCheckBoxTBRNormal->GAlign.left = true;
	material_editor::pCheckBoxTBRNormal->GAlign.top = true;*/
	material_editor::pCheckBoxTBRNormal->setBmpFromResourse(IDB_BITMAP6);

	material_editor::pCheckBoxTBRParam = SXGUICrCheckBoxEx("", 154, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pCheckBoxTBRParam->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxTBRParam->setStretchSides(true, false, false, true);
	/*material_editor::pCheckBoxTBRParam->GAlign.left = true;
	material_editor::pCheckBoxTBRParam->GAlign.top = true;*/
	material_editor::pCheckBoxTBRParam->setBmpFromResourse(IDB_BITMAP7);

	material_editor::pCheckBoxTBRAmDiff = SXGUICrCheckBoxEx("", 178, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pCheckBoxTBRAmDiff->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxTBRAmDiff->setStretchSides(true, false, false, true);
	/*material_editor::pCheckBoxTBRAmDiff->GAlign.left = true;
	material_editor::pCheckBoxTBRAmDiff->GAlign.top = true;*/
	material_editor::pCheckBoxTBRAmDiff->setBmpFromResourse(IDB_BITMAP8);

	material_editor::pCheckBoxTBRSpecular = SXGUICrCheckBoxEx("", 202, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pCheckBoxTBRSpecular->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxTBRSpecular->setStretchSides(true, false, false, true);
	/*material_editor::pCheckBoxTBRSpecular->GAlign.left = true;
	material_editor::pCheckBoxTBRSpecular->GAlign.top = true;*/
	material_editor::pCheckBoxTBRSpecular->setBmpFromResourse(IDB_BITMAP9);

	material_editor::pCheckBoxTBRLighting = SXGUICrCheckBoxEx("", 226, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, material_editor::pToolBar->getHWND(), 0, 0);
	material_editor::pCheckBoxTBRLighting->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxTBRLighting->setStretchSides(true, false, false, true);
	/*material_editor::pCheckBoxTBRLighting->GAlign.left = true;
	material_editor::pCheckBoxTBRLighting->GAlign.top = true;*/
	material_editor::pCheckBoxTBRLighting->setBmpFromResourse(IDB_BITMAP10);


	

	
	material_editor::pButtonSkyBox = SXGUICrButton("...", 275, 330, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonSkyBox->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pButtonSkyBox->addHandler(SXMaterialEditor_ButtonSkyBox_Click, WM_LBUTTONUP);

	material_editor::pEditSkyBox = SXGUICrEdit("0", 100, 330, 170, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditSkyBox->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditSkyBox->setColorText(RGB(0, 0, 0));
	material_editor::pEditSkyBox->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditSkyBox->setTransparentTextBk(true);
	material_editor::pEditSkyBox->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditSkyBox->addHandler(SXMaterialEditor_EditSkyBox_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditSkyBox->addHandler(SXMaterialEditor_EditSkyBox_Enter, WM_KILLFOCUS);

	material_editor::pCheckBoxModelRot = SXGUICrCheckBox("rotate", 5, 330, 50, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxModelRot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxModelRot->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxModelRot->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxModelRot->setTransparentTextBk(true);
	material_editor::pCheckBoxModelRot->setColorBrush(RGB(220, 220, 220));

	material_editor::pButtonRotAngle0 = SXGUICrButton("0", 60, 330, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonRotAngle0->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pButtonRotAngle0->addHandler(SXMaterialEditor_ButtonRotAngle0_Click, WM_LBUTTONUP);

	material_editor::pStaticTestModel = SXGUICrStatic("Test model:", 5, 350, 70, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticTestModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticTestModel->setColorText(RGB(0, 0, 0));
	material_editor::pStaticTestModel->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticTestModel->setTransparentTextBk(true);
	material_editor::pStaticTestModel->setColorBrush(RGB(220, 220, 220));
	material_editor::pComboBoxTestModel = SXGUICrComboBox(100, 350, 200, 70, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pComboBoxTestModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pComboBoxTestModel->setColorText(RGB(0, 0, 0));
	material_editor::pComboBoxTestModel->setColorTextBk(RGB(255, 255, 255));
	material_editor::pComboBoxTestModel->setTransparentTextBk(true);
	material_editor::pComboBoxTestModel->setColorBrush(RGB(255, 255, 255));
	material_editor::pComboBoxTestModel->addItem("sphere");
	material_editor::pComboBoxTestModel->addItem("cube");
	material_editor::pComboBoxTestModel->addItem("plane");
	material_editor::pComboBoxTestModel->setSel(0);



	material_editor::pStaticLigthColorText = SXGUICrStatic("Color:", 5, 375, 35, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticLigthColorText->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticLigthColorText->setColorText(RGB(0, 0, 0));
	material_editor::pStaticLigthColorText->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticLigthColorText->setTransparentTextBk(true);
	material_editor::pStaticLigthColorText->setColorBrush(RGB(220, 220, 220));

	material_editor::pStaticLigthColorR = SXGUICrStatic("r:", 45, 375, 70, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticLigthColorR->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticLigthColorR->setColorText(RGB(0, 0, 0));
	material_editor::pStaticLigthColorR->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticLigthColorR->setTransparentTextBk(true);
	material_editor::pStaticLigthColorR->setColorBrush(RGB(220, 220, 220));
	material_editor::pEditLigthColorR = SXGUICrEdit("0", 55, 375, 55, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditLigthColorR->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditLigthColorR->setColorText(RGB(0, 0, 0));
	material_editor::pEditLigthColorR->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditLigthColorR->setTransparentTextBk(true);
	material_editor::pEditLigthColorR->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditLigthColorR->addHandler(SXMaterialEditor_EditLigthColorR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditLigthColorR->addHandler(SXMaterialEditor_EditLigthColorR_Enter, WM_KILLFOCUS);


	material_editor::pStaticLigthColorG = SXGUICrStatic("g:", 115, 375, 70, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticLigthColorG->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticLigthColorG->setColorText(RGB(0, 0, 0));
	material_editor::pStaticLigthColorG->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticLigthColorG->setTransparentTextBk(true);
	material_editor::pStaticLigthColorG->setColorBrush(RGB(220, 220, 220));
	material_editor::pEditLigthColorG = SXGUICrEdit("0", 125, 375, 55, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditLigthColorG->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditLigthColorG->setColorText(RGB(0, 0, 0));
	material_editor::pEditLigthColorG->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditLigthColorG->setTransparentTextBk(true);
	material_editor::pEditLigthColorG->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditLigthColorG->addHandler(SXMaterialEditor_EditLigthColorG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditLigthColorG->addHandler(SXMaterialEditor_EditLigthColorG_Enter, WM_KILLFOCUS);


	material_editor::pStaticLigthColorB = SXGUICrStatic("b:", 185, 375, 70, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticLigthColorB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticLigthColorB->setColorText(RGB(0, 0, 0));
	material_editor::pStaticLigthColorB->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticLigthColorB->setTransparentTextBk(true);
	material_editor::pStaticLigthColorB->setColorBrush(RGB(220, 220, 220));
	material_editor::pEditLigthColorB = SXGUICrEdit("0", 195, 375, 55, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditLigthColorB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditLigthColorB->setColorText(RGB(0, 0, 0));
	material_editor::pEditLigthColorB->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditLigthColorB->setTransparentTextBk(true);
	material_editor::pEditLigthColorB->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditLigthColorB->addHandler(SXMaterialEditor_EditLigthColorB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditLigthColorB->addHandler(SXMaterialEditor_EditLigthColorB_Enter, WM_KILLFOCUS);

	material_editor::pButtonLigthColor = SXGUICrButton("...", 275, 375, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonLigthColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pButtonLigthColor->addHandler(SXMaterialEditor_ButtonLigthColor_Click, WM_LBUTTONUP);

	material_editor::pStaticLigthColor = SXGUICrStatic("", 5, 395, 295, 10, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticLigthColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticLigthColor->setColorText(RGB(0, 0, 0));
	material_editor::pStaticLigthColor->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticLigthColor->setTransparentTextBk(true);
	material_editor::pStaticLigthColor->setColorBrush(RGB(220, 220, 220));


	
	material_editor::pStaticTypeModel = SXGUICrStatic("Type model:",5,415,70,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticTypeModel->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticTypeModel->setColorText(RGB(0, 0, 0));
	material_editor::pStaticTypeModel->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticTypeModel->setTransparentTextBk(true);
	material_editor::pStaticTypeModel->setColorBrush(RGB(220, 220, 220));

	material_editor::pComboBoxTypeModel = SXGUICrComboBox(100,415,200,100,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pComboBoxTypeModel->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pComboBoxTypeModel->setColorText(RGB(0, 0, 0));
	material_editor::pComboBoxTypeModel->setColorTextBk(RGB(255, 255, 255));
	material_editor::pComboBoxTypeModel->setTransparentTextBk(true);
	material_editor::pComboBoxTypeModel->setColorBrush(RGB(255, 255, 255));
	material_editor::pComboBoxTypeModel->addItem("geometry");
	material_editor::pComboBoxTypeModel->addItem("grass");
	material_editor::pComboBoxTypeModel->addItem("tree");
	material_editor::pComboBoxTypeModel->addItem("skin");
	material_editor::pComboBoxTypeModel->setSel(0);

	

	material_editor::pStaticTex = SXGUICrStatic("Tex:",5,440,30,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticTex->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticTex->setColorText(RGB(0, 0, 0));
	material_editor::pStaticTex->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticTex->setTransparentTextBk(true);
	material_editor::pStaticTex->setColorBrush(RGB(220, 220, 220));
	material_editor::pEditTex = SXGUICrEdit("0",40,440,230,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditTex->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditTex->setColorText(RGB(0, 0, 0));
	material_editor::pEditTex->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditTex->setTransparentTextBk(true);
	material_editor::pEditTex->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditTex->addHandler(SXMaterialEditor_EditTex_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditTex->addHandler(SXMaterialEditor_EditTex_Enter, WM_KILLFOCUS);

	material_editor::pButtonTex = SXGUICrButton("...", 275, 440, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonTex->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pButtonTex->addHandler(SXMaterialEditor_ButtonTex_Click, WM_LBUTTONUP);



	material_editor::pCheckBoxLighting = SXGUICrCheckBox("Lighting", 5, 460, 55, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxLighting->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxLighting->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxLighting->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxLighting->setTransparentTextBk(true);
	material_editor::pCheckBoxLighting->setColorBrush(RGB(220, 220, 220));

	material_editor::pCheckBoxAlphaTest = SXGUICrCheckBox("AlphaTest", 70, 460, 65, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxAlphaTest->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxAlphaTest->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxAlphaTest->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxAlphaTest->setTransparentTextBk(true);
	material_editor::pCheckBoxAlphaTest->setColorBrush(RGB(220, 220, 220));

	material_editor::pCheckBoxTransparent = SXGUICrCheckBox("Transparent", 145, 460, 75, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxTransparent->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxTransparent->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxTransparent->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxTransparent->setTransparentTextBk(true);
	material_editor::pCheckBoxTransparent->setColorBrush(RGB(220, 220, 220));

	material_editor::pCheckBoxDestColor = SXGUICrCheckBox("DestColor", 230, 460, 65, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDestColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxDestColor->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDestColor->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDestColor->setTransparentTextBk(true);
	material_editor::pCheckBoxDestColor->setColorBrush(RGB(220, 220, 220));


	material_editor::pStaticTypeReflect = SXGUICrStatic("Reflection:", 5, 480, 60, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticTypeReflect->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticTypeReflect->setColorText(RGB(0, 0, 0));
	material_editor::pStaticTypeReflect->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticTypeReflect->setTransparentTextBk(true);
	material_editor::pStaticTypeReflect->setColorBrush(RGB(220, 220, 220));

	material_editor::pComboBoxTypeReflect = SXGUICrComboBox(70, 478, 230, 115, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pComboBoxTypeReflect->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pComboBoxTypeReflect->setColorText(RGB(0, 0, 0));
	material_editor::pComboBoxTypeReflect->setColorTextBk(RGB(255, 255, 255));
	material_editor::pComboBoxTypeReflect->setTransparentTextBk(true);
	material_editor::pComboBoxTypeReflect->setColorBrush(RGB(255, 255, 255));
	material_editor::pComboBoxTypeReflect->addItem("none");
	material_editor::pComboBoxTypeReflect->addItem("reflect plane");
	material_editor::pComboBoxTypeReflect->addItem("reflect cube dynamic");
	material_editor::pComboBoxTypeReflect->addItem("reflect cube static");
	material_editor::pComboBoxTypeReflect->setSel(0);



	material_editor::pStaticShaders = SXGUICrStatic("Shaders:", 5, 515, 50, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticShaders->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticShaders->setColorText(RGB(0, 0, 0));
	material_editor::pStaticShaders->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticShaders->setTransparentTextBk(true);
	material_editor::pStaticShaders->setColorBrush(RGB(220, 220, 220));

	material_editor::pComboBoxShaders = SXGUICrComboBoxEx(60, 513, 240, 150, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pComboBoxShaders->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pComboBoxShaders->setColorText(RGB(0, 0, 0));
	material_editor::pComboBoxShaders->setColorTextBk(RGB(255, 255, 255));
	material_editor::pComboBoxShaders->setTransparentTextBk(true);
	material_editor::pComboBoxShaders->setColorBrush(RGB(255, 255, 255));
	material_editor::pComboBoxShaders->addItem("");


	material_editor::pStaticVS = SXGUICrStatic("VS:",5,535,30,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticVS->setColorText(RGB(0, 0, 0));
	material_editor::pStaticVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticVS->setTransparentTextBk(true);
	material_editor::pStaticVS->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditVS = SXGUICrEdit("0",40,535,230,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditVS->setColorText(RGB(0, 0, 0));
	material_editor::pEditVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditVS->setTransparentTextBk(true);
	material_editor::pEditVS->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditVS->addHandler(SXMaterialEditor_EditVS_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditVS->addHandler(SXMaterialEditor_EditVS_Enter, WM_KILLFOCUS);

	material_editor::pButtonVS = SXGUICrButton("...", 275, 535, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pButtonVS->addHandler(SXMaterialEditor_ButtonVS_Click, WM_LBUTTONUP);

	material_editor::pStaticPS = SXGUICrStatic("PS:",5,555,30,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticPS->setColorText(RGB(0, 0, 0));
	material_editor::pStaticPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticPS->setTransparentTextBk(true);
	material_editor::pStaticPS->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditPS = SXGUICrEdit("0",40,555,230,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditPS->setColorText(RGB(0, 0, 0));
	material_editor::pEditPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditPS->setTransparentTextBk(true);
	material_editor::pEditPS->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditPS->addHandler(SXMaterialEditor_EditPS_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditPS->addHandler(SXMaterialEditor_EditPS_Enter, WM_KILLFOCUS);

	material_editor::pButtonPS = SXGUICrButton("...", 275, 555, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonPS->addHandler(SXMaterialEditor_ButtonPS_Click, WM_LBUTTONUP);


	material_editor::pStaticPhysic = SXGUICrStatic("Physic material:", 5, 585, 80, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticPhysic->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticPhysic->setColorText(RGB(0, 0, 0));
	material_editor::pStaticPhysic->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticPhysic->setTransparentTextBk(true);
	material_editor::pStaticPhysic->setColorBrush(RGB(220, 220, 220));
	material_editor::pComboBoxPhysic = SXGUICrComboBox(100, 583, 200, 120, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pComboBoxPhysic->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pComboBoxPhysic->setColorText(RGB(0, 0, 0));
	material_editor::pComboBoxPhysic->setColorTextBk(RGB(255, 255, 255));
	material_editor::pComboBoxPhysic->setTransparentTextBk(true);
	material_editor::pComboBoxPhysic->setColorBrush(RGB(255, 255, 255));
	material_editor::pComboBoxPhysic->addItem("concrete (бетон)");
	material_editor::pComboBoxPhysic->addItem("metal (металл)");
	material_editor::pComboBoxPhysic->addItem("glass (стекло)");
	material_editor::pComboBoxPhysic->addItem("plastic (пластик)");
	material_editor::pComboBoxPhysic->addItem("tree (дерево)");
	material_editor::pComboBoxPhysic->addItem("flesh (плоть)");
	material_editor::pComboBoxPhysic->addItem("ground/sand (земля/песок)");
	material_editor::pComboBoxPhysic->addItem("water (вода)");
	material_editor::pComboBoxPhysic->addItem("leaf/grass (листва/трава)");
	material_editor::pComboBoxPhysic->setSel(0);

	material_editor::pStaticHitChance = SXGUICrStatic("Hit chance:", 5, 605, 60, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticHitChance->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticHitChance->setColorText(RGB(0, 0, 0));
	material_editor::pStaticHitChance->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticHitChance->setTransparentTextBk(true);
	material_editor::pStaticHitChance->setColorBrush(RGB(220, 220, 220));
	material_editor::pEditHitChance = SXGUICrEdit("0", 60, 605, 65, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditHitChance->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditHitChance->setColorText(RGB(0, 0, 0));
	material_editor::pEditHitChance->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditHitChance->setTransparentTextBk(true);
	material_editor::pEditHitChance->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditHitChance->addHandler(SXMaterialEditor_EditHitChance_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditHitChance->addHandler(SXMaterialEditor_EditHitChance_Enter, WM_KILLFOCUS);
	material_editor::pTrackBarHitChance = SXGUICrTrackBar("", 130, 605, 170, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarHitChance->setMinMax(0, 100);
	material_editor::pTrackBarHitChance->setTickFrequency(10);
	material_editor::pTrackBarHitChance->addHandler(SXMaterialEditor_TrackBarHitChance_MouseMove, WM_MOUSEMOVE);

	material_editor::pStaticDurability = SXGUICrStatic("Durability:", 5, 625, 60, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticDurability->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticDurability->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDurability->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDurability->setTransparentTextBk(true);
	material_editor::pStaticDurability->setColorBrush(RGB(220, 220, 220));
	material_editor::pEditDurability = SXGUICrEdit("0", 60, 625, 65, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditDurability->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditDurability->setColorText(RGB(0, 0, 0));
	material_editor::pEditDurability->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditDurability->setTransparentTextBk(true);
	material_editor::pEditDurability->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditDurability->addHandler(SXMaterialEditor_EditDurability_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditDurability->addHandler(SXMaterialEditor_EditDurability_Enter, WM_KILLFOCUS);

	material_editor::pStaticDensity = SXGUICrStatic("Density:", 135, 625, 60, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticDensity->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticDensity->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDensity->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDensity->setTransparentTextBk(true);
	material_editor::pStaticDensity->setColorBrush(RGB(220, 220, 220));
	material_editor::pEditDensity = SXGUICrEdit("0", 180, 625, 65, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditDensity->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditDensity->setColorText(RGB(0, 0, 0));
	material_editor::pEditDensity->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditDensity->setTransparentTextBk(true);
	material_editor::pEditDensity->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditDensity->addHandler(SXMaterialEditor_EditDensity_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditDensity->addHandler(SXMaterialEditor_EditDensity_Enter, WM_KILLFOCUS);

	


	material_editor::pStaticMR = SXGUICrStatic("M R:",305,175,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticMR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticMR->setColorText(RGB(0, 0, 0));
	material_editor::pStaticMR->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticMR->setTransparentTextBk(true);
	material_editor::pStaticMR->setColorBrush(RGB(220, 220, 220));
	
	material_editor::pStaticMG = SXGUICrStatic("M G:",305,195,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticMG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticMG->setColorText(RGB(0, 0, 0));
	material_editor::pStaticMG->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticMG->setTransparentTextBk(true);
	material_editor::pStaticMG->setColorBrush(RGB(220, 220, 220));


	material_editor::pStaticParamL = SXGUICrStatic("Parameters Lighting:", 305, 30, 100, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticParamL->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticParamL->setColorText(RGB(0, 0, 0));
	material_editor::pStaticParamL->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticParamL->setTransparentTextBk(true);
	material_editor::pStaticParamL->setColorBrush(RGB(220, 220, 220));

	material_editor::pComboBoxParamL = SXGUICrComboBoxEx(400, 28, 230, 150, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pComboBoxParamL->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pComboBoxParamL->setColorText(RGB(0, 0, 0));
	material_editor::pComboBoxParamL->setColorTextBk(RGB(255, 255, 255));
	material_editor::pComboBoxParamL->setTransparentTextBk(true);
	material_editor::pComboBoxParamL->setColorBrush(RGB(255, 255, 255));
	material_editor::pComboBoxParamL->addItem("");

	material_editor::pStaticThickness = SXGUICrStatic("Thickness:", 305, 50, 70, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticThickness->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticThickness->setColorText(RGB(0, 0, 0));
	material_editor::pStaticThickness->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticThickness->setTransparentTextBk(true);
	material_editor::pStaticThickness->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditThickness = SXGUICrEdit("0", 375, 50, 50, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditThickness->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditThickness->setColorText(RGB(0, 0, 0));
	material_editor::pEditThickness->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditThickness->setTransparentTextBk(true);
	material_editor::pEditThickness->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditThickness->addHandler(SXMaterialEditor_EditThickness_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditThickness->addHandler(SXMaterialEditor_EditThickness_Enter, WM_KILLFOCUS);

	material_editor::pTrackBarThickness = SXGUICrTrackBar("", 430, 50, 200, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarThickness->setMinMax(0, 100);
	material_editor::pTrackBarThickness->setTickFrequency(10);
	material_editor::pTrackBarThickness->addHandler(SXMaterialEditor_TrackBarThickness_MouseMove, WM_MOUSEMOVE);

	material_editor::pStaticRoughness = SXGUICrStatic("Roughness:", 305, 70, 70, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticRoughness->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticRoughness->setColorText(RGB(0, 0, 0));
	material_editor::pStaticRoughness->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticRoughness->setTransparentTextBk(true);
	material_editor::pStaticRoughness->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditRoughness = SXGUICrEdit("0",375,70,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditRoughness->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditRoughness->setColorText(RGB(0, 0, 0));
	material_editor::pEditRoughness->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditRoughness->setTransparentTextBk(true);
	material_editor::pEditRoughness->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditRoughness->addHandler(SXMaterialEditor_EditRoughness_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditRoughness->addHandler(SXMaterialEditor_EditRoughness_Enter, WM_KILLFOCUS);

	material_editor::pTrackBarRoughness = SXGUICrTrackBar("", 430, 70, 200, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarRoughness->setMinMax(0, 100);
	material_editor::pTrackBarRoughness->setTickFrequency(10);
	material_editor::pTrackBarRoughness->addHandler(SXMaterialEditor_TrackBarRoughness_MouseMove, WM_MOUSEMOVE);

	material_editor::pStaticF0 = SXGUICrStatic("f0:", 305, 90, 70, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticF0->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticF0->setColorText(RGB(0, 0, 0));
	material_editor::pStaticF0->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticF0->setTransparentTextBk(true);
	material_editor::pStaticF0->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditF0 = SXGUICrEdit("0",375,90,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditF0->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditF0->setColorText(RGB(0, 0, 0));
	material_editor::pEditF0->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditF0->setTransparentTextBk(true);
	material_editor::pEditF0->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditF0->addHandler(SXMaterialEditor_EditF0_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditF0->addHandler(SXMaterialEditor_EditF0_Enter, WM_KILLFOCUS);

	material_editor::pTrackBarF0 = SXGUICrTrackBar("",430,90,200,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pTrackBarF0->setMinMax(0, 100);
	material_editor::pTrackBarF0->setTickFrequency(10);
	material_editor::pTrackBarF0->addHandler(SXMaterialEditor_TrackBarF0_MouseMove, WM_MOUSEMOVE);

	material_editor::pEditTexLighting = SXGUICrEdit("0",370,110,230,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditTexLighting->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditTexLighting->setColorText(RGB(0, 0, 0));
	material_editor::pEditTexLighting->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditTexLighting->setTransparentTextBk(true);
	material_editor::pEditTexLighting->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditTexLighting->addHandler(SXMaterialEditor_EditTexLighting_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditTexLighting->addHandler(SXMaterialEditor_EditTexLighting_Enter, WM_KILLFOCUS);

	material_editor::pButtonTexLighting = SXGUICrButton("...", 605, 110, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonTexLighting->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonTexLighting->addHandler(SXMaterialEditor_ButtonTexLighting_Click, WM_LBUTTONUP);

	material_editor::pCheckBoxTexLighting = SXGUICrCheckBox("texture:", 305, 110, 60, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxTexLighting->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxTexLighting->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxTexLighting->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxTexLighting->setTransparentTextBk(true);
	material_editor::pCheckBoxTexLighting->setColorBrush(RGB(220, 220, 220));
	

	material_editor::pStaticMaD = SXGUICrStatic("Microrelief and detail", 305, 135, 100, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticMaD->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticMaD->setColorText(RGB(0, 0, 0));
	material_editor::pStaticMaD->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticMaD->setTransparentTextBk(true);
	material_editor::pStaticMaD->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditMR = SXGUICrEdit("0", 360, 175, 240, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditMR->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditMR->setColorText(RGB(0, 0, 0));
	material_editor::pEditMR->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditMR->setTransparentTextBk(true);
	material_editor::pEditMR->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditMR->addHandler(SXMaterialEditor_EditMR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditMR->addHandler(SXMaterialEditor_EditMR_Enter, WM_KILLFOCUS);

	material_editor::pCheckBoxDoSVVS = SXGUICrCheckBox("vs", 340, 390, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSVVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSVVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSVVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSVVS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSVVS->setColorBrush(RGB(220, 220, 220));


	material_editor::pStaticMask = SXGUICrStatic("Mask:", 305, 152, 50, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticMask->setColorText(RGB(0, 0, 0));
	material_editor::pStaticMask->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticMask->setTransparentTextBk(true);
	material_editor::pStaticMask->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditMask = SXGUICrEdit("0", 360, 152, 240, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pEditMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pEditMask->setColorText(RGB(0, 0, 0));
	material_editor::pEditMask->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditMask->setTransparentTextBk(true);
	material_editor::pEditMask->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditMask->addHandler(SXMaterialEditor_EditMask_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditMask->addHandler(SXMaterialEditor_EditMask_Enter, WM_KILLFOCUS);

	material_editor::pButtonMask = SXGUICrButton("...", 605, 152, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pButtonMask->addHandler(SXMaterialEditor_ButtonMask_Click, WM_LBUTTONUP);

	material_editor::pButtonMR = SXGUICrButton("...", 605, 175, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonMR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonMR->addHandler(SXMaterialEditor_ButtonMR_Click, WM_LBUTTONUP);

	material_editor::pStaticMB = SXGUICrStatic("M B:",305,215,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticMB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticMB->setColorText(RGB(0, 0, 0));
	material_editor::pStaticMB->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticMB->setTransparentTextBk(true);
	material_editor::pStaticMB->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticMA = SXGUICrStatic("M A:",305,235,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticMA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticMA->setColorText(RGB(0, 0, 0));
	material_editor::pStaticMA->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticMA->setTransparentTextBk(true);
	material_editor::pStaticMA->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticDR = SXGUICrStatic("D R:",305,260,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticDR->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDR->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDR->setTransparentTextBk(true);
	material_editor::pStaticDR->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditMG = SXGUICrEdit("0",360,195,240,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditMG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditMG->setColorText(RGB(0, 0, 0));
	material_editor::pEditMG->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditMG->setTransparentTextBk(true);
	material_editor::pEditMG->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditMG->addHandler(SXMaterialEditor_EditMG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditMG->addHandler(SXMaterialEditor_EditMG_Enter, WM_KILLFOCUS);

	material_editor::pEditMB = SXGUICrEdit("0",360,215,240,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditMB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditMB->setColorText(RGB(0, 0, 0));
	material_editor::pEditMB->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditMB->setTransparentTextBk(true);
	material_editor::pEditMB->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditMB->addHandler(SXMaterialEditor_EditMB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditMB->addHandler(SXMaterialEditor_EditMB_Enter, WM_KILLFOCUS);

	material_editor::pEditMA = SXGUICrEdit("0",360,235,240,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditMA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditMA->setColorText(RGB(0, 0, 0));
	material_editor::pEditMA->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditMA->setTransparentTextBk(true);
	material_editor::pEditMA->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditMA->addHandler(SXMaterialEditor_EditMA_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditMA->addHandler(SXMaterialEditor_EditMA_Enter, WM_KILLFOCUS);

	material_editor::pEditDR = SXGUICrEdit("0",360,260,240,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditDR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditDR->setColorText(RGB(0, 0, 0));
	material_editor::pEditDR->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditDR->setTransparentTextBk(true);
	material_editor::pEditDR->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditDR->addHandler(SXMaterialEditor_EditDR_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditDR->addHandler(SXMaterialEditor_EditDR_Enter, WM_KILLFOCUS);

	material_editor::pButtonMG = SXGUICrButton("...", 605, 195, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonMG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonMG->addHandler(SXMaterialEditor_ButtonMG_Click, WM_LBUTTONUP);

	material_editor::pButtonMB = SXGUICrButton("...", 605, 215, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonMB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonMB->addHandler(SXMaterialEditor_ButtonMB_Click, WM_LBUTTONUP);

	material_editor::pButtonMA = SXGUICrButton("...", 605, 235, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonMA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonMA->addHandler(SXMaterialEditor_ButtonMA_Click, WM_LBUTTONUP);

	material_editor::pButtonDR = SXGUICrButton("...", 605, 260, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonDR->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonDR->addHandler(SXMaterialEditor_ButtonDR_Click, WM_LBUTTONUP);

	material_editor::pStaticDG = SXGUICrStatic("D G:",305,280,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticDG->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDG->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDG->setTransparentTextBk(true);
	material_editor::pStaticDG->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticDB = SXGUICrStatic("D B:",305,300,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticDB->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDB->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDB->setTransparentTextBk(true);
	material_editor::pStaticDB->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticDA = SXGUICrStatic("D A:",305,320,50,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticDA->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDA->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDA->setTransparentTextBk(true);
	material_editor::pStaticDA->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditDG = SXGUICrEdit("0",360,280,240,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditDG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditDG->setColorText(RGB(0, 0, 0));
	material_editor::pEditDG->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditDG->setTransparentTextBk(true);
	material_editor::pEditDG->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditDG->addHandler(SXMaterialEditor_EditDG_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditDG->addHandler(SXMaterialEditor_EditDG_Enter, WM_KILLFOCUS);

	material_editor::pEditDB = SXGUICrEdit("0",360,300,240,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditDB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditDB->setColorText(RGB(0, 0, 0));
	material_editor::pEditDB->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditDB->setTransparentTextBk(true);
	material_editor::pEditDB->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditDB->addHandler(SXMaterialEditor_EditDB_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditDB->addHandler(SXMaterialEditor_EditDB_Enter, WM_KILLFOCUS);

	material_editor::pEditDA = SXGUICrEdit("0",360,320,240,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditDA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditDA->setColorText(RGB(0, 0, 0));
	material_editor::pEditDA->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditDA->setTransparentTextBk(true);
	material_editor::pEditDA->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditDA->addHandler(SXMaterialEditor_EditDA_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditDA->addHandler(SXMaterialEditor_EditDA_Enter, WM_KILLFOCUS);

	material_editor::pButtonDG = SXGUICrButton("...", 605, 280, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonDG->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonDG->addHandler(SXMaterialEditor_ButtonDG_Click, WM_LBUTTONUP);

	material_editor::pButtonDB = SXGUICrButton("...", 605, 300, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonDB->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonDB->addHandler(SXMaterialEditor_ButtonDB_Click, WM_LBUTTONUP);

	material_editor::pButtonDA = SXGUICrButton("...", 605, 320, 25, 15, SXGUI_BUTTON_IMAGE_NONE, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pButtonDA->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pButtonDA->addHandler(SXMaterialEditor_ButtonDA_Click, WM_LBUTTONUP);

	material_editor::pStaticDoS = SXGUICrStatic("Data for shaders:",305,350,100,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDoS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticDoS->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDoS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDoS->setTransparentTextBk(true);
	material_editor::pStaticDoS->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticDoSW = SXGUICrStatic("mW:",305,370,35,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDoSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticDoSW->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDoSW->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDoSW->setTransparentTextBk(true);
	material_editor::pStaticDoSW->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSWVS = SXGUICrCheckBox("vs", 340, 370, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSWVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSWVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSWVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSWVS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSWVS->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSWPS = SXGUICrCheckBox("ps", 373, 370, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSWPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSWPS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSWPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSWPS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSWPS->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticDoSV = SXGUICrStatic("mV:",305,390,35,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDoSV->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticDoSV->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDoSV->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDoSV->setTransparentTextBk(true);
	material_editor::pStaticDoSV->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSVPS = SXGUICrCheckBox("ps", 373, 390, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSVPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSVPS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSVPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSVPS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSVPS->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticDosP = SXGUICrStatic("mP:",305,410,35,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDosP->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticDosP->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDosP->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDosP->setTransparentTextBk(true);
	material_editor::pStaticDosP->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSPVS = SXGUICrCheckBox("vs", 340, 410, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSPVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSPVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSPVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSPVS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSPVS->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSPPS = SXGUICrCheckBox("ps", 373, 410, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSPPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSPPS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSPPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSPPS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSPPS->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticBoxDoSWV = SXGUICrStatic("mWV:",410,370,45,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticBoxDoSWV->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticBoxDoSWV->setColorText(RGB(0, 0, 0));
	material_editor::pStaticBoxDoSWV->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticBoxDoSWV->setTransparentTextBk(true);
	material_editor::pStaticBoxDoSWV->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticDoSWVP = SXGUICrStatic("mWVP:",410,390,45,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDoSWVP->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticDoSWVP->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDoSWVP->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDoSWVP->setTransparentTextBk(true);
	material_editor::pStaticDoSWVP->setColorBrush(RGB(220, 220, 220));
	material_editor::pStaticDoSCampos = SXGUICrStatic("CamPos:",410,410,45,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticDoSCampos->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticDoSCampos->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDoSCampos->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDoSCampos->setTransparentTextBk(true);
	material_editor::pStaticDoSCampos->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSWVVS = SXGUICrCheckBox("vs", 455, 370, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSWVVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSWVVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSWVVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSWVVS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSWVVS->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSWVPS = SXGUICrCheckBox("ps", 488, 370, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSWVPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSWVPS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSWVPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSWVPS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSWVPS->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSCamposVS = SXGUICrCheckBox("vs", 455, 410, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSCamposVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSCamposVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSCamposVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSCamposVS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSCamposVS->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSWVPVS = SXGUICrCheckBox("vs", 455, 390, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSWVPVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSWVPVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSWVPVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSWVPVS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSWVPVS->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSWVPPS = SXGUICrCheckBox("ps", 488, 390, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSWVPPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSWVPPS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSWVPPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSWVPPS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSWVPPS->setColorBrush(RGB(220, 220, 220));
	material_editor::pCheckBoxDoSCamposPS = SXGUICrCheckBox("ps", 488, 411, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSCamposPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxDoSCamposPS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSCamposPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSCamposPS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSCamposPS->setColorBrush(RGB(220, 220, 220));

	material_editor::pStaticDoSTimeDelta = SXGUICrStatic("Time:", 525, 370, 45, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticDoSTimeDelta->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticDoSTimeDelta->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDoSTimeDelta->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDoSTimeDelta->setTransparentTextBk(true);
	material_editor::pStaticDoSTimeDelta->setColorBrush(RGB(220, 220, 220));

	material_editor::pCheckBoxDoSTimeDeltaVS = SXGUICrCheckBox("vs", 570, 370, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSTimeDeltaVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxDoSTimeDeltaVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSTimeDeltaVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSTimeDeltaVS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSTimeDeltaVS->setColorBrush(RGB(220, 220, 220));

	material_editor::pCheckBoxDoSTimeDeltaPS = SXGUICrCheckBox("ps", 603, 370, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSTimeDeltaPS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxDoSTimeDeltaPS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSTimeDeltaPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSTimeDeltaPS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSTimeDeltaPS->setColorBrush(RGB(220, 220, 220));


	material_editor::pStaticDoSWinSize = SXGUICrStatic("WinSize:", 525, 390, 45, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pStaticDoSWinSize->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pStaticDoSWinSize->setColorText(RGB(0, 0, 0));
	material_editor::pStaticDoSWinSize->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticDoSWinSize->setTransparentTextBk(true);
	material_editor::pStaticDoSWinSize->setColorBrush(RGB(220, 220, 220));

	material_editor::pCheckBoxDoSWinSizeVS = SXGUICrCheckBox("vs", 570, 390, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSWinSizeVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxDoSWinSizeVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSWinSizeVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSWinSizeVS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSWinSizeVS->setColorBrush(RGB(220, 220, 220));

	material_editor::pCheckBoxDoSWinSizePS = SXGUICrCheckBox("ps", 603, 390, 30, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxDoSWinSizePS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxDoSWinSizePS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxDoSWinSizePS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxDoSWinSizePS->setTransparentTextBk(true);
	material_editor::pCheckBoxDoSWinSizePS->setColorBrush(RGB(220, 220, 220));


	material_editor::pCheckBoxUDVS = SXGUICrCheckBox("User data VS:", 305, 440, 100, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxUDVS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxUDVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxUDVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxUDVS->setTransparentTextBk(true);
	material_editor::pCheckBoxUDVS->setColorBrush(RGB(220, 220, 220));

	material_editor::pCheckBoxUDVSInPS = SXGUICrCheckBox("in PS", 445, 440, 100, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxUDVSInPS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxUDVSInPS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxUDVSInPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxUDVSInPS->setTransparentTextBk(true);
	material_editor::pCheckBoxUDVSInPS->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditUDVSX = SXGUICrEdit("0",325,460,80,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditUDVSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditUDVSX->setColorText(RGB(0, 0, 0));
	material_editor::pEditUDVSX->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditUDVSX->setTransparentTextBk(true);
	material_editor::pEditUDVSX->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditUDVSX->addHandler(SXMaterialEditor_EditUDVSX_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditUDVSX->addHandler(SXMaterialEditor_EditUDVSX_Enter, WM_KILLFOCUS);

	material_editor::pEditUDVSY = SXGUICrEdit("0",325,480,80,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditUDVSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditUDVSY->setColorText(RGB(0, 0, 0));
	material_editor::pEditUDVSY->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditUDVSY->setTransparentTextBk(true);
	material_editor::pEditUDVSY->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditUDVSY->addHandler(SXMaterialEditor_EditUDVSY_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditUDVSY->addHandler(SXMaterialEditor_EditUDVSY_Enter, WM_KILLFOCUS);

	
	material_editor::pStaticUDVSX = SXGUICrStatic("x:",305,460,15,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticUDVSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticUDVSX->setColorText(RGB(0, 0, 0));
	material_editor::pStaticUDVSX->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticUDVSX->setTransparentTextBk(true);
	material_editor::pStaticUDVSX->setColorBrush(RGB(220, 220, 220));

	material_editor::pStaticUDVSY = SXGUICrStatic("y:",305,480,15,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticUDVSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticUDVSY->setColorText(RGB(0, 0, 0));
	material_editor::pStaticUDVSY->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticUDVSY->setTransparentTextBk(true);
	material_editor::pStaticUDVSY->setColorBrush(RGB(220, 220, 220));

	material_editor::pStaticUDVSZ = SXGUICrStatic("z:",305,500,15,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticUDVSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticUDVSZ->setColorText(RGB(0, 0, 0));
	material_editor::pStaticUDVSZ->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticUDVSZ->setTransparentTextBk(true);
	material_editor::pStaticUDVSZ->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditUDVSZ = SXGUICrEdit("0",325,500,80,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditUDVSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditUDVSZ->setColorText(RGB(0, 0, 0));
	material_editor::pEditUDVSZ->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditUDVSZ->setTransparentTextBk(true);
	material_editor::pEditUDVSZ->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditUDVSZ->addHandler(SXMaterialEditor_EditUDVSZ_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditUDVSZ->addHandler(SXMaterialEditor_EditUDVSZ_Enter, WM_KILLFOCUS);


	material_editor::pStaticUDVSW = SXGUICrStatic("w:",305,520,15,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticUDVSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticUDVSW->setColorText(RGB(0, 0, 0));
	material_editor::pStaticUDVSW->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticUDVSW->setTransparentTextBk(true);
	material_editor::pStaticUDVSW->setColorBrush(RGB(220, 220, 220));

	material_editor::pEditUDVSW = SXGUICrEdit("0",325,520,80,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditUDVSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditUDVSW->setColorText(RGB(0, 0, 0));
	material_editor::pEditUDVSW->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditUDVSW->setTransparentTextBk(true);
	material_editor::pEditUDVSW->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditUDVSW->addHandler(SXMaterialEditor_EditUDVSW_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditUDVSW->addHandler(SXMaterialEditor_EditUDVSW_Enter, WM_KILLFOCUS);

	material_editor::pTrackBarUDVSX = SXGUICrTrackBar("", 410, 460, 220, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarUDVSX->setMinMax(-100, 100);
	material_editor::pTrackBarUDVSX->setTickFrequency(10);
	material_editor::pTrackBarUDVSX->addHandler(SXMaterialEditor_TrackBarUDVSX_MouseMove, WM_MOUSEMOVE);

	material_editor::pTrackBarUDVSY = SXGUICrTrackBar("", 410, 480, 220, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarUDVSY->setMinMax(-100, 100);
	material_editor::pTrackBarUDVSY->setTickFrequency(10);
	material_editor::pTrackBarUDVSY->addHandler(SXMaterialEditor_TrackBarUDVSY_MouseMove, WM_MOUSEMOVE);

	material_editor::pTrackBarUDVSZ = SXGUICrTrackBar("", 410, 500, 220, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarUDVSZ->setMinMax(-100, 100);
	material_editor::pTrackBarUDVSZ->setTickFrequency(10);
	material_editor::pTrackBarUDVSZ->addHandler(SXMaterialEditor_TrackBarUDVSZ_MouseMove, WM_MOUSEMOVE);

	material_editor::pTrackBarUDVSW = SXGUICrTrackBar("",410,520,220,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pTrackBarUDVSW->setMinMax(-100, 100);
	material_editor::pTrackBarUDVSW->setTickFrequency(10);
	material_editor::pTrackBarUDVSW->addHandler(SXMaterialEditor_TrackBarUDVSW_MouseMove, WM_MOUSEMOVE);
	


	material_editor::pCheckBoxUDPS = SXGUICrCheckBox("User data PS:", 305, 550, 100, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxUDPS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	material_editor::pCheckBoxUDPS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxUDPS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxUDPS->setTransparentTextBk(true);
	material_editor::pCheckBoxUDPS->setColorBrush(RGB(220, 220, 220));


	material_editor::pCheckBoxUDPSInVS = SXGUICrCheckBox("in VS", 445, 550, 100, 15, material_editor::pJobWindow->getHWND(), 0, 0, false);
	material_editor::pCheckBoxUDPSInVS->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pCheckBoxUDPSInVS->setColorText(RGB(0, 0, 0));
	material_editor::pCheckBoxUDPSInVS->setColorTextBk(RGB(255, 255, 255));
	material_editor::pCheckBoxUDPSInVS->setTransparentTextBk(true);
	material_editor::pCheckBoxUDPSInVS->setColorBrush(RGB(220, 220, 220));


	material_editor::pStaticUDPSX = SXGUICrStatic("x:",305,570,15,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticUDPSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticUDPSX->setColorText(RGB(0, 0, 0));
	material_editor::pStaticUDPSX->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticUDPSX->setTransparentTextBk(true);
	material_editor::pStaticUDPSX->setColorBrush(RGB(220, 220, 220));

	material_editor::pStaticUDPSY = SXGUICrStatic("y:",305,590,15,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticUDPSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticUDPSY->setColorText(RGB(0, 0, 0));
	material_editor::pStaticUDPSY->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticUDPSY->setTransparentTextBk(true);
	material_editor::pStaticUDPSY->setColorBrush(RGB(220, 220, 220));

	material_editor::pStaticUDPSZ = SXGUICrStatic("z:",305,610,15,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticUDPSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticUDPSZ->setColorText(RGB(0, 0, 0));
	material_editor::pStaticUDPSZ->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticUDPSZ->setTransparentTextBk(true);
	material_editor::pStaticUDPSZ->setColorBrush(RGB(220, 220, 220));

	material_editor::pStaticUDPSW = SXGUICrStatic("w:",305,630,15,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pStaticUDPSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pStaticUDPSW->setColorText(RGB(0, 0, 0));
	material_editor::pStaticUDPSW->setColorTextBk(RGB(255, 255, 255));
	material_editor::pStaticUDPSW->setTransparentTextBk(true);
	material_editor::pStaticUDPSW->setColorBrush(RGB(220, 220, 220));


	material_editor::pEditUDPSX = SXGUICrEdit("0",325,570,80,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditUDPSX->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditUDPSX->setColorText(RGB(0, 0, 0));
	material_editor::pEditUDPSX->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditUDPSX->setTransparentTextBk(true);
	material_editor::pEditUDPSX->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditUDPSX->addHandler(SXMaterialEditor_EditUDPSX_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditUDPSX->addHandler(SXMaterialEditor_EditUDPSX_Enter, WM_KILLFOCUS);

	material_editor::pEditUDPSY = SXGUICrEdit("0",325,590,80,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditUDPSY->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditUDPSY->setColorText(RGB(0, 0, 0));
	material_editor::pEditUDPSY->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditUDPSY->setTransparentTextBk(true);
	material_editor::pEditUDPSY->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditUDPSY->addHandler(SXMaterialEditor_EditUDPSY_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditUDPSY->addHandler(SXMaterialEditor_EditUDPSY_Enter, WM_KILLFOCUS);

	material_editor::pEditUDPSZ = SXGUICrEdit("0",325,610,80,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditUDPSZ->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditUDPSZ->setColorText(RGB(0, 0, 0));
	material_editor::pEditUDPSZ->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditUDPSZ->setTransparentTextBk(true);
	material_editor::pEditUDPSZ->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditUDPSZ->addHandler(SXMaterialEditor_EditUDPSZ_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditUDPSZ->addHandler(SXMaterialEditor_EditUDPSZ_Enter, WM_KILLFOCUS);
	
	material_editor::pEditUDPSW = SXGUICrEdit("0",325,630,80,15,material_editor::pJobWindow->getHWND(),0,0);
	material_editor::pEditUDPSW->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	material_editor::pEditUDPSW->setColorText(RGB(0, 0, 0));
	material_editor::pEditUDPSW->setColorTextBk(RGB(255, 255, 255));
	material_editor::pEditUDPSW->setTransparentTextBk(true);
	material_editor::pEditUDPSW->setColorBrush(RGB(255, 255, 255));
	material_editor::pEditUDPSW->addHandler(SXMaterialEditor_EditUDPSW_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	material_editor::pEditUDPSW->addHandler(SXMaterialEditor_EditUDPSW_Enter, WM_KILLFOCUS);


	material_editor::pTrackBarUDPSX = SXGUICrTrackBar("", 410, 570, 220, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarUDPSX->setMinMax(-100, 100);
	material_editor::pTrackBarUDPSX->setTickFrequency(10);
	material_editor::pTrackBarUDPSX->addHandler(SXMaterialEditor_TrackBarUDPSX_MouseMove, WM_MOUSEMOVE);

	material_editor::pTrackBarUDPSY = SXGUICrTrackBar("", 410, 590, 220, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarUDPSY->setMinMax(-100, 100);
	material_editor::pTrackBarUDPSY->setTickFrequency(10);
	material_editor::pTrackBarUDPSY->addHandler(SXMaterialEditor_TrackBarUDPSY_MouseMove, WM_MOUSEMOVE);

	material_editor::pTrackBarUDPSZ = SXGUICrTrackBar("", 410, 610, 220, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarUDPSZ->setMinMax(-100, 100);
	material_editor::pTrackBarUDPSZ->setTickFrequency(10);
	material_editor::pTrackBarUDPSZ->addHandler(SXMaterialEditor_TrackBarUDPSZ_MouseMove, WM_MOUSEMOVE);

	material_editor::pTrackBarUDPSW = SXGUICrTrackBar("", 410, 630, 220, 15, material_editor::pJobWindow->getHWND(), 0, 0);
	material_editor::pTrackBarUDPSW->setMinMax(-100, 100);
	material_editor::pTrackBarUDPSW->setTickFrequency(10);
	material_editor::pTrackBarUDPSW->addHandler(SXMaterialEditor_TrackBarUDPSW_MouseMove, WM_MOUSEMOVE);
	
}
void material_editor::DeleteAllElements()
{
	mem_delete(material_editor::pWindowRender);
	mem_delete(material_editor::pToolBar);
	mem_delete(material_editor::pStaticTypeModel);
	mem_delete(material_editor::pComboBoxTypeModel);
	mem_delete(material_editor::pStaticTestModel);
	mem_delete(material_editor::pComboBoxTestModel);
	mem_delete(material_editor::pStaticTex);
	mem_delete(material_editor::pEditTex);

	mem_delete(material_editor::pStaticLigthColorText);
	mem_delete(material_editor::pStaticLigthColorR);
	mem_delete(material_editor::pEditLigthColorR);
	mem_delete(material_editor::pStaticLigthColorG);
	mem_delete(material_editor::pEditLigthColorG);
	mem_delete(material_editor::pStaticLigthColorB);
	mem_delete(material_editor::pEditLigthColorB);
	mem_delete(material_editor::pButtonLigthColor);
	mem_delete(material_editor::pStaticLigthColor);

	mem_delete(material_editor::pStaticShaders);
	mem_delete(material_editor::pComboBoxShaders);

	mem_delete(material_editor::pStaticVS);
	mem_delete(material_editor::pEditVS);
	mem_delete(material_editor::pStaticPS);
	mem_delete(material_editor::pEditPS);
	mem_delete(material_editor::pButtonTex);
	mem_delete(material_editor::pButtonVS);
	mem_delete(material_editor::pButtonPS);
	mem_delete(material_editor::pStaticMR);

	mem_delete(material_editor::pStaticParamL);
	mem_delete(material_editor::pComboBoxParamL);

	mem_delete(material_editor::pStaticRoughness);
	mem_delete(material_editor::pStaticF0);
	mem_delete(material_editor::pStaticMG);
	mem_delete(material_editor::pEditRoughness);
	mem_delete(material_editor::pEditF0);
	mem_delete(material_editor::pEditMR);
	mem_delete(material_editor::pCheckBoxLighting);
	mem_delete(material_editor::pCheckBoxTransparent);
	mem_delete(material_editor::pTrackBarRoughness);
	mem_delete(material_editor::pTrackBarF0);
	mem_delete(material_editor::pTrackBarUDVSX);
	mem_delete(material_editor::pStaticMaD);
	mem_delete(material_editor::pEditTexLighting);
	mem_delete(material_editor::pButtonTexLighting);
	mem_delete(material_editor::pCheckBoxTexLighting);
	mem_delete(material_editor::pCheckBoxDoSVVS);
	mem_delete(material_editor::pComboBoxTypeReflect);
	mem_delete(material_editor::pStaticPhysic);
	mem_delete(material_editor::pComboBoxPhysic);
	mem_delete(material_editor::pButtonMR);
	mem_delete(material_editor::pStaticMB);
	mem_delete(material_editor::pStaticMA);
	mem_delete(material_editor::pStaticDR);
	mem_delete(material_editor::pEditMG);
	mem_delete(material_editor::pEditMB);
	mem_delete(material_editor::pEditMA);
	mem_delete(material_editor::pEditDR);
	mem_delete(material_editor::pButtonMG);
	mem_delete(material_editor::pButtonMB);
	mem_delete(material_editor::pButtonMA);
	mem_delete(material_editor::pButtonDR);
	mem_delete(material_editor::pStaticDG);
	mem_delete(material_editor::pStaticDB);
	mem_delete(material_editor::pStaticDA);
	mem_delete(material_editor::pEditDG);
	mem_delete(material_editor::pEditDB);
	mem_delete(material_editor::pEditDA);
	mem_delete(material_editor::pButtonDG);
	mem_delete(material_editor::pButtonDB);
	mem_delete(material_editor::pButtonDA);
	mem_delete(material_editor::pStaticDoS);
	mem_delete(material_editor::pStaticDoSW);
	mem_delete(material_editor::pCheckBoxDoSWVS);
	mem_delete(material_editor::pCheckBoxDoSWPS);
	mem_delete(material_editor::pStaticDoSV);
	mem_delete(material_editor::pCheckBoxDoSVPS);
	mem_delete(material_editor::pStaticDosP);
	mem_delete(material_editor::pCheckBoxDoSPVS);
	mem_delete(material_editor::pCheckBoxDoSPPS);
	mem_delete(material_editor::pStaticBoxDoSWV);
	mem_delete(material_editor::pStaticDoSWVP);
	mem_delete(material_editor::pStaticDoSCampos);
	mem_delete(material_editor::pCheckBoxDoSWVVS);
	mem_delete(material_editor::pCheckBoxDoSWVPS);
	mem_delete(material_editor::pCheckBoxDoSCamposVS);
	mem_delete(material_editor::pCheckBoxDoSWVPVS);
	mem_delete(material_editor::pCheckBoxDoSWVPPS);
	mem_delete(material_editor::pCheckBoxDoSCamposPS);
	mem_delete(material_editor::pCheckBoxUDVS);
	mem_delete(material_editor::pCheckBoxUDVSInPS);
	mem_delete(material_editor::pEditUDVSX);
	mem_delete(material_editor::pEditUDVSY);
	mem_delete(material_editor::pTrackBarUDVSY);
	mem_delete(material_editor::pStaticUDVSX);
	mem_delete(material_editor::pStaticUDVSY);
	mem_delete(material_editor::pStaticUDVSZ);
	mem_delete(material_editor::pEditUDVSZ);
	mem_delete(material_editor::pTrackBarUDVSZ);
	mem_delete(material_editor::pStaticUDVSW);
	mem_delete(material_editor::pEditUDVSW);
	mem_delete(material_editor::pTrackBarUDVSW);
	mem_delete(material_editor::pStaticDoSTimeDelta);
	mem_delete(material_editor::pCheckBoxDoSTimeDeltaVS);
	mem_delete(material_editor::pCheckBoxDoSTimeDeltaPS);
	mem_delete(material_editor::pCheckBoxUDPS);
	mem_delete(material_editor::pCheckBoxUDPSInVS);
	mem_delete(material_editor::pStaticUDPSX);
	mem_delete(material_editor::pStaticUDPSY);
	mem_delete(material_editor::pStaticUDPSZ);
	mem_delete(material_editor::pStaticUDPSW);
	mem_delete(material_editor::pEditUDPSX);
	mem_delete(material_editor::pEditUDPSY);
	mem_delete(material_editor::pTrackBarUDPSX);
	mem_delete(material_editor::pTrackBarUDPSY);
	mem_delete(material_editor::pEditUDPSZ);
	mem_delete(material_editor::pTrackBarUDPSZ);
	mem_delete(material_editor::pEditUDPSW);
	mem_delete(material_editor::pTrackBarUDPSW);
	mem_delete(material_editor::pStaticMask);
	mem_delete(material_editor::pEditMask);
	mem_delete(material_editor::pButtonMask);
	mem_delete(material_editor::pStaticDoSWinSize);
	mem_delete(material_editor::pCheckBoxDoSWinSizeVS);
	mem_delete(material_editor::pCheckBoxDoSWinSizePS);
	mem_delete(material_editor::pCheckBoxAlphaTest);
	mem_delete(material_editor::pCheckBoxDestColor);
	mem_delete(material_editor::pButtonSkyBox);
	mem_delete(material_editor::pEditSkyBox);
	mem_delete(material_editor::pCheckBoxModelRot);
	mem_delete(material_editor::pButtonRotAngle0);
	mem_delete(material_editor::pStaticThickness);
	mem_delete(material_editor::pEditThickness);
	mem_delete(material_editor::pTrackBarThickness);
	mem_delete(material_editor::pJobWindow);
}

//##########################################################################

void material_editor::MaterialEditorUpdate(DWORD timeDelta)
{
	ID skit = -1;

	if (!material_editor::pComboBoxShaders->getFocus())
	{
		char vs[1024];
		char ps[1024];

		SMtrl_MtlGetVS(material_editor::idMat, vs);
		SMtrl_MtlGetPS(material_editor::idMat, ps);

		if ((skit = material_editor::pShaders->find(vs, ps)) >= 0)
			material_editor::pComboBoxShaders->setSel(skit + 1);
		else
			material_editor::pComboBoxShaders->setSel(0);
	}

	if (!material_editor::pComboBoxParamL->getFocus())
	{
		float thikcness = SMtrl_MtlGetThickness(material_editor::idMat);
		float roughness = SMtrl_MtlGetRoughness(material_editor::idMat);
		float f0 = SMtrl_MtlGetF0(material_editor::idMat);

		skit = -1;
		if ((skit = material_editor::pParamL->find(thikcness, roughness, f0)) >= 0)
			material_editor::pComboBoxParamL->setSel(skit + 1);
		else
			material_editor::pComboBoxParamL->setSel(0);
	}

	if (material_editor::pCheckBoxModelRot->getCheck())
		SRender_SimModelSetRotationY(SRender_SimModelGetRotationY() - float(timeDelta) * 0.001f * 0.25);
}