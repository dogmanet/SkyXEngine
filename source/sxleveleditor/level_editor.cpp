
#include "level_editor.h"

namespace SXLevelEditor
{
	ISXGUIBaseWnd* JobWindow;
	ISXGUIMenuWindow* MainMenu;
	ISXGUIBaseWnd* RenderWindow;

	ISXGUIToolBar* ToolBar1;
	ISXGUIButton* ButtonTBNew;
	ISXGUIButton* ButtonTBOpen;
	ISXGUIButton* ButtonTBSave;
	ISXGUIButton* ButtonTBSaveAs;

	ISXGUICheckBox* CheckBoxTBArrow;
	ISXGUICheckBox* CheckBoxTBPos;
	ISXGUICheckBox* CheckBoxTBRot;
	ISXGUICheckBox* CheckBoxTBScale;

	ISXGUICheckBox* CheckBoxTBGrid;
	ISXGUICheckBox* CheckBoxTBAxes;

	ISXGUICheckBox* CheckBoxTBRColor;
	ISXGUICheckBox* CheckBoxTBRNormal;
	ISXGUICheckBox* CheckBoxTBRParam;
	ISXGUICheckBox* CheckBoxTBRAmDiff;
	ISXGUICheckBox* CheckBoxTBRSpecular;
	ISXGUICheckBox* CheckBoxTBRLighting;

	ISXGUICheckBox* CheckBoxTBSelS;
	ISXGUICheckBox* CheckBoxTBSelZTest;
	ISXGUICheckBox* CheckBoxTBSelMesh;
	ISXGUICheckBox* CheckBoxTBSelCullBack;

	ISXGUICheckBox* CheckBoxTBAIGBound;
	ISXGUICheckBox* CheckBoxTBAIGQuad;
	ISXGUICheckBox* CheckBoxTBAIGGraphPoint;

	ISXGUICheckBox* CheckBoxTBLevelType;
	ISXGUICheckBox* CheckBoxTBGLightEnable;

	ISXGUIGroupBox* GroupBoxList;
	ISXGUIGroupBox* GroupBoxData;
	ISXGUIListBox* ListBoxList;
	ISXGUIStatic* StaticListTextCount;
	ISXGUIStatic* StaticListValCount;

	ISXGUIButton* ButtonDelete;

	ISXGUIButton* ButtonGeometryOpen;
	ISXGUIButton* ButtonGreenOpen;
	ISXGUIButton* ButtonGameObjectOpen;
	ISXGUIButton* ButtonAIGridOpen;


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
	ISXGUIButton* ButtonGameTab;
	int GameTabVal = 0;
	ISXGUIListView* ListViewGameClass;
	ISXGUIComboBox* ComboBoxGameValue;
	ISXGUIEdit* EditGameValue;
	ISXGUIButton* ButtonGameValue;
	ISXGUIStatic* StaticGameHelp;
	ISXGUIMemo* MemoGameHelp;
	ISXGUIButton* ButtonGameCreate;

	ISXGUICheckBox* CheckBoxGameFlags[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	const char* aGameObjectFlags[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	ISXGUIListView* ListViewGameConnections;
	ISXGUIStatic* StaticGameConnectionsEvent;
	ISXGUIComboBox* ComboBoxGameConnectionsEvent;
	ISXGUIStatic* StaticGameConnectionsName;
	ISXGUIEdit* EditGameConnectionsName;
	ISXGUIStatic* StaticGameConnectionsAction;
	ISXGUIComboBox* ComboBoxGameConnectionsAction;
	ISXGUIStatic* StaticGameConnectionsDelay;
	ISXGUIEdit* EditGameConnectionsDelay;
	ISXGUIStatic* StaticGameConnectionsParameter;
	ISXGUIEdit* EditGameConnectionsParameter;
	ISXGUIButton* ButtonGameConnectionsCreate;
	ISXGUIButton* ButtonGameConnectionsDelete;
	//}

	//aigrid
	//{
	ISXGUIButton* ButtonAIQuadsDelSel;
	ISXGUIButton* ButtonAIGridGen;
	ISXGUIButton* ButtonAIGridClear;
	ISXGUIButton* ButtonAIClearAll;
	ISXGUIStatic* StatiAIBBDimensions;
	ISXGUIStatic* StaticAIBBDimensionsWidth;
	ISXGUIEdit* EditAIBBDimensionsWidth;
	ISXGUIStatic* StaticAIBBDimensionsHeight;
	ISXGUIEdit* EditAIBBDimensionsHeight;
	ISXGUIStatic* StaticAIBBDimensionsDepth;
	ISXGUIEdit* EditAIBBDimensionsDepth;
	ISXGUIStatic* StaticAIBBPos;
	ISXGUIStatic* StaticAIBBPosX;
	ISXGUIEdit* EditAIBBPosX;
	ISXGUIStatic* StaticAIBBPosY;
	ISXGUIEdit* EditAIBBPosY;
	ISXGUIStatic* StaticAIBBPosZ;
	ISXGUIEdit* EditAIBBPosZ;
	ISXGUIButton* ButtonAIBBFinish;
	ISXGUIButton* ButtonAIGPGen;
	ISXGUIButton* ButtonAIGPClear;
	ISXGUIRadioButton* RadioButtonAIGPAdd;
	ISXGUIRadioButton* RadioButtonAIGPDel;
	ISXGUIRadioButton* RadioButtonAIQuadAdd;
	ISXGUIRadioButton* RadioButtonAIQuadsMSel;
	ISXGUIRadioButton* RadioButtonAIQuadsSelDel;
	ISXGUIButton* ButtonAIGridValidation;
	ISXGUICheckBox* CheckBoxAIGridMarkedSplits;
	ISXGUIStatic* StaticAIStatistics;
	ISXGUIStatic* StaticAIStatsCountQuads;
	ISXGUIStatic* StaticAIStatsCountGP;
	ISXGUIStatic* StaticAIStatsCountSplits;
	ISXGUIEdit* EditAIStatsCountQuads;
	ISXGUIEdit* EditAIStatsCountGP;
	ISXGUIEdit* EditAIStatsCountSplits;
	//}

	ISXGUIStatusBar* StatusBar1;

	void InitAllElements();

	void DeleteAllElements();

	void LevelNew(bool mess);
	void LevelOpen();
	void LevelSave();
	void LevelSaveAs();

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
	void GameUpdatePosRot();
	void GameVisibleProperties(bool bf);
	void GameVisibleConnections(bool bf);

	void AIGridActivateAll(bool bf);
	void AIGridEnableBB(bool bf);


	ID3DXMesh* FigureBox = 0;
	AxesHelper* ObjAxesHelper = 0;

	int ActiveGroupType = 0;		//!< текущая выделенная группа мировых сущностей EDITORS_LEVEL_GROUPTYPE_
	ID ActiveGreenSplit = -1;		//!< текущий идентификатор сплита растительность (если выделена растительность)
	ID ActiveGreenObject = -1;		//!< текущий идентификатор объекта растительности (если выделена растительность)

	ID ActiveElement = -1;			//!< текущий идентификатор выделенного элемента из списка
	bool SelSelection = false;		//!< разрешено ли выделение?
	bool SelZTest = false;			//!< использовать ли z-test при выделении?
	bool SelMesh = false;			//!< рисовать сеткой (true) или целиком модель выделения (false)?
	bool SelBackFacesCull = false;	//!< отсекать ли задние грани при выделении?

	bool AIGBound = true;			//!< отрисовка боунда ai сетки
	bool AIGQuad = true;			//!< отрисовка квадов ai сетки
	bool AIGGraphPoint = true;		//!< отрисовка графпоинтов ai сетки

	//bound box для массового создания объектов растительности
	bool GreenRenderBox = false;	//!< разрешено ли рисовать бокс?
	float3 GreenBoxPos;				//!< позиция бокса
	float3_t GreenBoxWHD(1, 1, 1);	//!< ширина, высота, длина бокса




	float3 HelperPos;
	float3 HelperRot;
	float3 HelperScale;

	bool isAddGameConections = false;

	ID IdMtl = -1;
	ID MenuWeatherCurrID = -1;
	int MenuWeatherCount = 0;
	Array<String> MenuWeatherArr;
};

//

void SXLevelEditor::InitAllElements()
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int cx = (wrect.right - MAINWIN_SIZE_X) / 2;
	int cy = (wrect.bottom - MAINWIN_SIZE_Y) / 2;

	SXLevelEditor::JobWindow = SXGUICrBaseWndEx("JobWindow", "SX Level editor", cx, cy, MAINWIN_SIZE_X, MAINWIN_SIZE_Y, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	//SXLevelEditor::JobWindow->setVisible(false);
	gui_func::base_handlers::InitHandlerMsg(SXLevelEditor::JobWindow);

	SXLevelEditor::JobWindow->addHandler(ComMenuId, WM_COMMAND);
	SXLevelEditor::JobWindow->addHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXLevelEditor::JobWindow->addHandler(MsgEditSize, WM_SIZE);
	SXLevelEditor::JobWindow->setMixSize(MAINWIN_SIZE_X, MAINWIN_SIZE_Y);
	SXLevelEditor::MainMenu = SXGUICrMenuWindowEx(IDR_MENU1);
	SXLevelEditor::MainMenu->setToWindow(SXLevelEditor::JobWindow->getHWND());


	ISXGUIPopupMenu *pPMenu = SXGUICrPopupMenu();
	pPMenu->insertItem(0, "1", 0, 0);
	pPMenu->insertItem(0, "1", 0, 0);
	pPMenu->insertItem(0, "1", 0, 0);
	pPMenu->insertItem(0, "1", 0, 0);
	pPMenu->insertItem(0, "1", 0, 0);
	pPMenu->insertItem(0, "1", 0, 0);
	pPMenu->track(SXLevelEditor::JobWindow->getHWND(), 150, 150);
	

	SXLevelEditor::RenderWindow = SXGUICrBaseWndEx("RenderWindow", "RenderWindow", 0, 27, 600, 400, 0, LoadCursor(NULL, IDC_ARROW), CreateSolidBrush(RGB(200, 200, 200)), 0, CS_HREDRAW | CS_VREDRAW, WS_VISIBLE | WS_BORDER, SXLevelEditor::JobWindow->getHWND(), WndProcAllDefault);
	SXLevelEditor::RenderWindow->setFollowParentSides(true, true, true, true);
	/*SXLevelEditor::RenderWindow->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RenderWindow->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	SXLevelEditor::RenderWindow->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RenderWindow->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);*/
	SXLevelEditor::RenderWindow->addHandler(SXLevelEditor_RenderWindow_MouseMove, WM_MOUSEMOVE);
	SXLevelEditor::RenderWindow->addHandler(SXLevelEditor_RenderWindow_LDown, WM_LBUTTONDOWN);
	SXLevelEditor::RenderWindow->addHandler(SXLevelEditor_RenderWindow_LClick, WM_LBUTTONUP);
	SXLevelEditor::RenderWindow->addHandler(SXLevelEditor_RenderWindow_RClick, WM_RBUTTONUP);
	SXLevelEditor::RenderWindow->addHandler(SXLevelEditor_RenderWindow_MBUp, WM_MBUTTONUP);
	SXLevelEditor::RenderWindow->addHandler(SXLevelEditor_RenderWindow_Delete, WM_KEYDOWN, VK_DELETE, 1, 0, 0, 0);

	SXLevelEditor::ToolBar1 = SXGUICrToolBar(0, 1, 810, 26, SXLevelEditor::JobWindow->getHWND(), WndProcAllDefault, 0);
	//SXLevelEditor::ToolBar1->setFollowParentSides(true, true, true, false);
	SXLevelEditor::ToolBar1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ToolBar1->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	SXLevelEditor::ToolBar1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ToolBar1->setFollowParentSide(SXGUI_SIDE_BOTTOM, false);
	SXLevelEditor::ToolBar1->addHandler(SXLevelEditor_ToolBar1_CallWmCommand, WM_COMMAND);

	SXLevelEditor::ButtonTBNew = SXGUICrButtonEx("", 2, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::ButtonTBNew->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonTBNew->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::ButtonTBNew->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonTBNew->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::ButtonTBNew->setBmpFromResourse(IDB_BITMAP1);

	SXLevelEditor::ButtonTBOpen = SXGUICrButtonEx("", 26, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::ButtonTBOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonTBOpen->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::ButtonTBOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonTBOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::ButtonTBOpen->setBmpFromResourse(IDB_BITMAP2);

	SXLevelEditor::ButtonTBSave = SXGUICrButtonEx("", 50, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::ButtonTBSave->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonTBSave->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::ButtonTBSave->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonTBSave->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::ButtonTBSave->setBmpFromResourse(IDB_BITMAP4);

	SXLevelEditor::ButtonTBSaveAs = SXGUICrButtonEx("", 74, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::ButtonTBSaveAs->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonTBSaveAs->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::ButtonTBSaveAs->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonTBSaveAs->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::ButtonTBSaveAs->setBmpFromResourse(IDB_BITMAP3);

	SXLevelEditor::CheckBoxTBArrow = SXGUICrCheckBoxEx("", 104, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBArrow->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBArrow->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBArrow->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBArrow->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBArrow->setBmpFromResourse(IDB_BITMAP5);

	SXLevelEditor::CheckBoxTBPos = SXGUICrCheckBoxEx("", 128, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBPos->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBPos->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBPos->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBPos->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBPos->setBmpFromResourse(IDB_BITMAP6);

	SXLevelEditor::CheckBoxTBRot = SXGUICrCheckBoxEx("", 152, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBRot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBRot->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBRot->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBRot->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBRot->setBmpFromResourse(IDB_BITMAP7);

	SXLevelEditor::CheckBoxTBScale = SXGUICrCheckBoxEx("", 176, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBScale->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBScale->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBScale->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBScale->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBScale->setBmpFromResourse(IDB_BITMAP8);


	SXLevelEditor::CheckBoxTBGrid = SXGUICrCheckBoxEx("", 206, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBGrid->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBGrid->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBGrid->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBGrid->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBGrid->setBmpFromResourse(IDB_BITMAP9);

	SXLevelEditor::CheckBoxTBAxes = SXGUICrCheckBoxEx("", 230, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBAxes->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBAxes->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBAxes->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBAxes->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBAxes->setBmpFromResourse(IDB_BITMAP10);


	SXLevelEditor::CheckBoxTBRColor = SXGUICrCheckBoxEx("", 260, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBRColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBRColor->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBRColor->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBRColor->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBRColor->setBmpFromResourse(IDB_BITMAP11);

	SXLevelEditor::CheckBoxTBRNormal = SXGUICrCheckBoxEx("", 284, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBRNormal->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBRNormal->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBRNormal->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBRNormal->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBRNormal->setBmpFromResourse(IDB_BITMAP12);

	SXLevelEditor::CheckBoxTBRParam = SXGUICrCheckBoxEx("", 308, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBRParam->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBRParam->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBRParam->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBRParam->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBRParam->setBmpFromResourse(IDB_BITMAP13);

	SXLevelEditor::CheckBoxTBRAmDiff = SXGUICrCheckBoxEx("", 332, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBRAmDiff->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBRAmDiff->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBRAmDiff->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBRAmDiff->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBRAmDiff->setBmpFromResourse(IDB_BITMAP14);

	SXLevelEditor::CheckBoxTBRSpecular = SXGUICrCheckBoxEx("", 356, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBRSpecular->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBRSpecular->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBRSpecular->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBRSpecular->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBRSpecular->setBmpFromResourse(IDB_BITMAP15);

	SXLevelEditor::CheckBoxTBRLighting = SXGUICrCheckBoxEx("", 380, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBRLighting->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBRLighting->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBRLighting->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBRLighting->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBRLighting->setBmpFromResourse(IDB_BITMAP16);

	
	SXLevelEditor::CheckBoxTBSelS = SXGUICrCheckBoxEx("", 410, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBSelS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBSelS->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBSelS->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBSelS->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBSelS->setBmpFromResourse(IDB_BITMAP17);

	SXLevelEditor::CheckBoxTBSelZTest = SXGUICrCheckBoxEx("", 434, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBSelZTest->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBSelZTest->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBSelZTest->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBSelZTest->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBSelZTest->setBmpFromResourse(IDB_BITMAP18);

	SXLevelEditor::CheckBoxTBSelMesh = SXGUICrCheckBoxEx("", 458, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBSelMesh->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBSelMesh->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBSelMesh->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBSelMesh->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBSelMesh->setBmpFromResourse(IDB_BITMAP19);

	SXLevelEditor::CheckBoxTBSelCullBack = SXGUICrCheckBoxEx("", 482, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBSelCullBack->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBSelCullBack->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBSelCullBack->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBSelCullBack->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBSelCullBack->setBmpFromResourse(IDB_BITMAP20);


	SXLevelEditor::CheckBoxTBAIGBound = SXGUICrCheckBoxEx("", 512, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBAIGBound->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBAIGBound->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBAIGBound->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBAIGBound->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBAIGBound->setBmpFromResourse(IDB_BITMAP22);

	SXLevelEditor::CheckBoxTBAIGQuad = SXGUICrCheckBoxEx("", 536, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBAIGQuad->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBAIGQuad->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBAIGQuad->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBAIGQuad->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBAIGQuad->setBmpFromResourse(IDB_BITMAP23);

	SXLevelEditor::CheckBoxTBAIGGraphPoint = SXGUICrCheckBoxEx("", 560, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBAIGGraphPoint->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBAIGGraphPoint->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBAIGGraphPoint->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBAIGGraphPoint->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBAIGGraphPoint->setBmpFromResourse(IDB_BITMAP24);

	SXLevelEditor::CheckBoxTBLevelType = SXGUICrCheckBoxEx("", 588, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBLevelType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBLevelType->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBLevelType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBLevelType->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);

	SXLevelEditor::CheckBoxTBGLightEnable = SXGUICrCheckBoxEx("", 612, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXLevelEditor::ToolBar1->getHWND(), 0, 0);
	SXLevelEditor::CheckBoxTBGLightEnable->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxTBGLightEnable->setFollowParentSides(true, false, false, true);
	/*SXLevelEditor::CheckBoxTBGLightEnable->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxTBGLightEnable->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXLevelEditor::CheckBoxTBGLightEnable->setBmpFromResourse(IDB_BITMAP27);


	
	SXLevelEditor::GroupBoxList = SXGUICrGroupBox("GroupBoxList", 601, 28, 200, 400, SXLevelEditor::JobWindow->getHWND(), WndProcAllDefault, 0);
	gui_func::base_handlers::InitHandlerMsg(SXLevelEditor::GroupBoxList);
	SXLevelEditor::GroupBoxList->addHandler(SXLevelEditor_GroupBoxList_CallWmCommand, WM_COMMAND);
	SXLevelEditor::GroupBoxList->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::GroupBoxList->setColorText(RGB(0,0,0));
	SXLevelEditor::GroupBoxList->setColorTextBk(RGB(220,220,220));
	SXLevelEditor::GroupBoxList->setTransparentTextBk(true);
	SXLevelEditor::GroupBoxList->setColorBrush(RGB(220, 220, 220));

	SXLevelEditor::GroupBoxList->setFollowParentSide(SXGUI_SIDE_LEFT, false);
	SXLevelEditor::GroupBoxList->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	SXLevelEditor::GroupBoxList->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::GroupBoxList->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);

	SXLevelEditor::GroupBoxData = SXGUICrGroupBox("", 0, 425, 800, 190, SXLevelEditor::JobWindow->getHWND(), WndProcAllDefault, 0);
	gui_func::base_handlers::InitHandlerMsg(SXLevelEditor::GroupBoxData);
	SXLevelEditor::GroupBoxData->addHandler(SXLevelEditor_GroupBox_CallWmCommand, WM_COMMAND);
	SXLevelEditor::GroupBoxData->addHandler(SXLevelEditor_GroupBox_Notify, WM_NOTIFY);
	SXLevelEditor::GroupBoxData->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::GroupBoxData->setColorText(RGB(0,0,0));
	SXLevelEditor::GroupBoxData->setColorTextBk(RGB(220, 220, 220));
	SXLevelEditor::GroupBoxData->setTransparentTextBk(true);
	SXLevelEditor::GroupBoxData->setColorBrush(RGB(220, 220, 220));

	SXLevelEditor::GroupBoxData->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::GroupBoxData->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	SXLevelEditor::GroupBoxData->setFollowParentSide(SXGUI_SIDE_TOP, false);
	SXLevelEditor::GroupBoxData->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);

	SXLevelEditor::ListBoxList = SXGUICrListBox(4, 30, 193, 290, SXLevelEditor::GroupBoxList->getHWND(), 0, 0, false);
	SXLevelEditor::ListBoxList->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ListBoxList->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ListBoxList->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::ListBoxList->setColorText(RGB(0,0,0));
	SXLevelEditor::ListBoxList->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::ListBoxList->setTransparentTextBk(true);
	SXLevelEditor::ListBoxList->setColorBrush(RGB(255, 255, 255));
	//SXLevelEditor::ListBoxList->addHandler(SXLevelEditor_ListBoxList_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticListTextCount = SXGUICrStatic("Count:", 65, 15, 30, 15, SXLevelEditor::GroupBoxList->getHWND(), 0);
	SXLevelEditor::StaticListTextCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticListTextCount->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticListTextCount->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::StaticListTextCount->setColorText(RGB(0,0,0));
	SXLevelEditor::StaticListTextCount->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticListTextCount->setTransparentTextBk(true);
	SXLevelEditor::StaticListTextCount->setColorBrush(RGB(220, 220, 220));

	SXLevelEditor::StaticListValCount = SXGUICrStatic("0", 100, 15, 50, 15, SXLevelEditor::GroupBoxList->getHWND(), 0);
	SXLevelEditor::StaticListValCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticListValCount->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticListValCount->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::StaticListValCount->setColorText(RGB(0,0,0));
	SXLevelEditor::StaticListValCount->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticListValCount->setTransparentTextBk(true);
	SXLevelEditor::StaticListValCount->setColorBrush(RGB(220, 220, 220));

	SXLevelEditor::ButtonDelete = SXGUICrButton("Delete", 135, 325, 60, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxList->getHWND(), 0, 0);
	SXLevelEditor::ButtonDelete->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonDelete->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonDelete->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonDelete->addHandler(SXLevelEditor_ButtonDelete_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonGeometryOpen = SXGUICrButton("Geometry", 5, 350, 60, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxList->getHWND(), 0, 0);
	SXLevelEditor::ButtonGeometryOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGeometryOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGeometryOpen->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::ButtonGeometryOpen->addHandler(SXLevelEditor_ButtonGeometryOpen_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonGreenOpen = SXGUICrButton("Green", 70, 350, 60, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxList->getHWND(), 0, 0);
	SXLevelEditor::ButtonGreenOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGreenOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGreenOpen->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXLevelEditor::ButtonGreenOpen->addHandler(SXLevelEditor_ButtonGreenOpen_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonGameObjectOpen = SXGUICrButton("Game", 135, 350, 60, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxList->getHWND(), 0, 0);
	SXLevelEditor::ButtonGameObjectOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGameObjectOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGameObjectOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGameObjectOpen->addHandler(SXLevelEditor_ButtonGameObjectOpen_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonAIGridOpen = SXGUICrButton("AI Grid", 5, 375, 60, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxList->getHWND(), 0, 0);
	SXLevelEditor::ButtonAIGridOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonAIGridOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonAIGridOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonAIGridOpen->addHandler(SXLevelEditor_ButtonAIGridOpen_Click, WM_LBUTTONUP);

	SXLevelEditor::StatusBar1 = SXGUICrStatusBar("StatusBar1", SXLevelEditor::JobWindow->getHWND(), 0, 0);
	SXLevelEditor::StatusBar1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	int arrpart[5];
	arrpart[0] = 150;
	arrpart[1] = 300;
	arrpart[2] = 450;
	arrpart[3] = 600;
	arrpart[4] = -1;
	SXLevelEditor::StatusBar1->setPartsCount(5, arrpart);
	SXLevelEditor::StatusBar1->setPartText(0, "Level poly: ");
	SXLevelEditor::StatusBar1->setPartText(1, "Geom poly: ");
	SXLevelEditor::StatusBar1->setPartText(2, "Green poly: ");
	SXLevelEditor::StatusBar1->setPartText(3, "");
	SXLevelEditor::StatusBar1->setPartText(4, "");

	//Geom
	//{
	SXLevelEditor::StaticGeomName = SXGUICrStatic("Name", 5, 20, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGeomName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomName->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGeomName->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGeomName->setTransparentTextBk(true);
	SXLevelEditor::StaticGeomName->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGeomName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGeomName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGeomName->setVisible(false);

	SXLevelEditor::EditGeomName = SXGUICrEdit("0", 80, 20, 200, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomName->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomName->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomName->setTransparentTextBk(true);
	SXLevelEditor::EditGeomName->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomName->setVisible(false);
	SXLevelEditor::EditGeomName->addHandler(SXLevelEditor_EditGeomName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomName->addHandler(SXLevelEditor_EditGeomName_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticGeomModel = SXGUICrStatic("Model", 5, 40, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGeomModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomModel->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGeomModel->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGeomModel->setTransparentTextBk(true);
	SXLevelEditor::StaticGeomModel->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGeomModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGeomModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGeomModel->setVisible(false);

	SXLevelEditor::EditGeomModel = SXGUICrEdit("0", 80, 40, 200, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomModel->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomModel->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomModel->setTransparentTextBk(true);
	SXLevelEditor::EditGeomModel->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomModel->setVisible(false);

	SXLevelEditor::ButtonGeomModel = SXGUICrButton("...", 285, 40, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGeomModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGeomModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGeomModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGeomModel->setVisible(false);
	SXLevelEditor::ButtonGeomModel->addHandler(SXLevelEditor_ButtonGeomModel_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGeomLod1 = SXGUICrStatic("Lod1", 5, 60, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGeomLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomLod1->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGeomLod1->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGeomLod1->setTransparentTextBk(true);
	SXLevelEditor::StaticGeomLod1->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGeomLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGeomLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGeomLod1->setVisible(false);

	SXLevelEditor::EditGeomLod1 = SXGUICrEdit("0", 80, 60, 200, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomLod1->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomLod1->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomLod1->setTransparentTextBk(true);
	SXLevelEditor::EditGeomLod1->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomLod1->setVisible(false);

	SXLevelEditor::ButtonGeomLod1 = SXGUICrButton("...", 285, 60, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGeomLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGeomLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGeomLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGeomLod1->setVisible(false);
	SXLevelEditor::ButtonGeomLod1->addHandler(SXLevelEditor_ButtonGeomLod1_Click, WM_LBUTTONUP);
	
	SXLevelEditor::StaticGeomPos = SXGUICrStatic("Position:", 320, 35, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGeomPos->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomPos->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGeomPos->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGeomPos->setTransparentTextBk(true);
	SXLevelEditor::StaticGeomPos->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGeomPos->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGeomPos->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGeomPos->setVisible(false);

	SXLevelEditor::EditGeomPosX = SXGUICrEdit("0", 370, 15, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomPosX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomPosX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomPosX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomPosX->setTransparentTextBk(true);
	SXLevelEditor::EditGeomPosX->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomPosX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomPosX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomPosX->setVisible(false);
	SXLevelEditor::EditGeomPosX->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomPosX->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomPosY = SXGUICrEdit("0", 370, 35, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomPosY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomPosY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomPosY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomPosY->setTransparentTextBk(true);
	SXLevelEditor::EditGeomPosY->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomPosY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomPosY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomPosY->setVisible(false);
	SXLevelEditor::EditGeomPosY->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomPosY->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomPosZ = SXGUICrEdit("0", 370, 55, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomPosZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomPosZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomPosZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomPosZ->setTransparentTextBk(true);
	SXLevelEditor::EditGeomPosZ->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomPosZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomPosZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomPosZ->setVisible(false);
	SXLevelEditor::EditGeomPosZ->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomPosZ->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KILLFOCUS);

	SXLevelEditor::RadioButtonGeomPosX = SXGUICrRadioButton("x", 440, 15, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomPosX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomPosX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGeomPosX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGeomPosX->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomPosX->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGeomPosX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGeomPosX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGeomPosX->setVisible(false);

	SXLevelEditor::RadioButtonGeomPosY = SXGUICrRadioButton("y", 440, 35, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomPosY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomPosY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGeomPosY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGeomPosY->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomPosY->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGeomPosY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGeomPosY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGeomPosY->setVisible(false);

	SXLevelEditor::RadioButtonGeomPosZ = SXGUICrRadioButton("z", 440, 55, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomPosZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomPosZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGeomPosZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGeomPosZ->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomPosZ->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGeomPosZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGeomPosZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGeomPosZ->setVisible(false);

	SXLevelEditor::StaticGeomRot = SXGUICrStatic("Rotation:", 470, 35, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGeomRot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomRot->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGeomRot->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGeomRot->setTransparentTextBk(true);
	SXLevelEditor::StaticGeomRot->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGeomRot->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGeomRot->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGeomRot->setVisible(false);

	SXLevelEditor::EditGeomRotX = SXGUICrEdit("0", 525, 15, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomRotX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomRotX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomRotX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomRotX->setTransparentTextBk(true);
	SXLevelEditor::EditGeomRotX->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomRotX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomRotX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomRotX->setVisible(false);
	SXLevelEditor::EditGeomRotX->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomRotX->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomRotY = SXGUICrEdit("0", 525, 35, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomRotY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomRotY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomRotY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomRotY->setTransparentTextBk(true);
	SXLevelEditor::EditGeomRotY->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomRotY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomRotY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomRotY->setVisible(false);
	SXLevelEditor::EditGeomRotY->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomRotY->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomRotZ = SXGUICrEdit("0", 525, 55, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomRotZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomRotZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomRotZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomRotZ->setTransparentTextBk(true);
	SXLevelEditor::EditGeomRotZ->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomRotZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomRotZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomRotZ->setVisible(false);
	SXLevelEditor::EditGeomRotZ->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomRotZ->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KILLFOCUS);

	SXLevelEditor::RadioButtonGeomRotX = SXGUICrRadioButton("x", 595, 15, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomRotX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomRotX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGeomRotX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGeomRotX->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomRotX->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGeomRotX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGeomRotX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGeomRotX->setVisible(false);

	SXLevelEditor::RadioButtonGeomRotY = SXGUICrRadioButton("y", 595, 35, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomRotY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomRotY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGeomRotY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGeomRotY->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomRotY->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGeomRotY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGeomRotY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGeomRotY->setVisible(false);

	SXLevelEditor::RadioButtonGeomRotZ = SXGUICrRadioButton("z", 595, 55, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomRotZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomRotZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGeomRotZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGeomRotZ->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomRotZ->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGeomRotZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGeomRotZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGeomRotZ->setVisible(false);

	SXLevelEditor::StaticGeomScale = SXGUICrStatic("Scale:", 625, 35, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGeomScale->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGeomScale->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGeomScale->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGeomScale->setTransparentTextBk(true);
	SXLevelEditor::StaticGeomScale->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGeomScale->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGeomScale->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGeomScale->setVisible(false);

	SXLevelEditor::EditGeomScaleX = SXGUICrEdit("0", 675, 15, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomScaleX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomScaleX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomScaleX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomScaleX->setTransparentTextBk(true);
	SXLevelEditor::EditGeomScaleX->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomScaleX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomScaleX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomScaleX->setVisible(false);
	SXLevelEditor::EditGeomScaleX->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomScaleX->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomScaleY = SXGUICrEdit("0", 675, 35, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomScaleY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomScaleY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomScaleY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomScaleY->setTransparentTextBk(true);
	SXLevelEditor::EditGeomScaleY->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomScaleY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomScaleY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomScaleY->setVisible(false);
	SXLevelEditor::EditGeomScaleY->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomScaleY->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KILLFOCUS);

	SXLevelEditor::EditGeomScaleZ = SXGUICrEdit("0", 675, 55, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGeomScaleZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGeomScaleZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGeomScaleZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGeomScaleZ->setTransparentTextBk(true);
	SXLevelEditor::EditGeomScaleZ->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGeomScaleZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGeomScaleZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGeomScaleZ->setVisible(false);
	SXLevelEditor::EditGeomScaleZ->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGeomScaleZ->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KILLFOCUS);

	SXLevelEditor::RadioButtonGeomScaleX = SXGUICrRadioButton("x", 745, 15, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomScaleX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomScaleX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGeomScaleX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGeomScaleX->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomScaleX->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGeomScaleX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGeomScaleX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGeomScaleX->setVisible(false);

	SXLevelEditor::RadioButtonGeomScaleY = SXGUICrRadioButton("y", 745, 35, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomScaleY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomScaleY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGeomScaleY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGeomScaleY->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomScaleY->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGeomScaleY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGeomScaleY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGeomScaleY->setVisible(false);

	SXLevelEditor::RadioButtonGeomScaleZ = SXGUICrRadioButton("z", 745, 55, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGeomScaleZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGeomScaleZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGeomScaleZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGeomScaleZ->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGeomScaleZ->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGeomScaleZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGeomScaleZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGeomScaleZ->setVisible(false);

	SXLevelEditor::ButtonGeomFinish = SXGUICrButton("Finish", 100, 95, 100, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGeomFinish->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGeomFinish->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGeomFinish->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGeomFinish->setVisible(false);
	SXLevelEditor::ButtonGeomFinish->addHandler(SXLevelEditor_ButtonGeomFinish_Click, WM_LBUTTONUP);
	//}


	//Green
	//{

	SXLevelEditor::StaticGreenName = SXGUICrStatic("Name", 5, 20, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenName->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenName->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenName->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenName->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenName->setVisible(false);

	SXLevelEditor::EditGreenName = SXGUICrEdit("0", 80, 20, 200, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGreenName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenName->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGreenName->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGreenName->setTransparentTextBk(true);
	SXLevelEditor::EditGreenName->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGreenName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGreenName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGreenName->setVisible(false);
	SXLevelEditor::EditGreenName->addHandler(SXLevelEditor_EditGreenName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGreenName->addHandler(SXLevelEditor_EditGreenName_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticGreenModel = SXGUICrStatic("Model", 5, 40, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenModel->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenModel->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenModel->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenModel->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenModel->setVisible(false);

	SXLevelEditor::EditGreenModel = SXGUICrEdit("0", 80, 40, 200, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGreenModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenModel->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGreenModel->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGreenModel->setTransparentTextBk(true);
	SXLevelEditor::EditGreenModel->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGreenModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGreenModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGreenModel->setVisible(false);

	SXLevelEditor::ButtonGreenModel = SXGUICrButton("...", 285, 40, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGreenModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGreenModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGreenModel->setVisible(false);
	SXLevelEditor::ButtonGreenModel->addHandler(SXLevelEditor_ButtonGreenModel_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGreenLod1 = SXGUICrStatic("Lod1", 5, 60, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenLod1->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenLod1->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenLod1->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenLod1->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenLod1->setVisible(false);

	SXLevelEditor::EditGreenLod1 = SXGUICrEdit("0", 80, 60, 200, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGreenLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenLod1->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGreenLod1->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGreenLod1->setTransparentTextBk(true);
	SXLevelEditor::EditGreenLod1->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGreenLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGreenLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGreenLod1->setVisible(false);

	SXLevelEditor::ButtonGreenLod1 = SXGUICrButton("...", 285, 60, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGreenLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGreenLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGreenLod1->setVisible(false);
	SXLevelEditor::ButtonGreenLod1->addHandler(SXLevelEditor_ButtonGreenLod1_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGreenLod2 = SXGUICrStatic("Lod2", 5, 80, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenLod2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenLod2->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenLod2->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenLod2->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenLod2->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenLod2->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenLod2->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenLod2->setVisible(false);

	SXLevelEditor::EditGreenLod2 = SXGUICrEdit("0", 80, 80, 200, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGreenLod2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenLod2->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGreenLod2->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGreenLod2->setTransparentTextBk(true);
	SXLevelEditor::EditGreenLod2->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGreenLod2->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGreenLod2->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGreenLod2->setVisible(false);

	SXLevelEditor::ButtonGreenLod2 = SXGUICrButton("...", 285, 80, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGreenLod2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenLod2->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGreenLod2->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGreenLod2->setVisible(false);
	SXLevelEditor::ButtonGreenLod2->addHandler(SXLevelEditor_ButtonGreenLod2_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonGreenMask = SXGUICrButton("...", 285, 100, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGreenMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenMask->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGreenMask->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGreenMask->setVisible(false);
	SXLevelEditor::ButtonGreenMask->addHandler(SXLevelEditor_ButtonGreenMask_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGreenMask = SXGUICrStatic("Mask texture", 5, 100, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenMask->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenMask->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenMask->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenMask->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenMask->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenMask->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenMask->setVisible(false);

	SXLevelEditor::EditGreenMask = SXGUICrEdit("0", 80, 100, 200, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGreenMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenMask->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGreenMask->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGreenMask->setTransparentTextBk(true);
	SXLevelEditor::EditGreenMask->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGreenMask->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGreenMask->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGreenMask->setVisible(false);

	SXLevelEditor::StaticGreenNav = SXGUICrStatic("NavMesh:", 330, 20, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenNav->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenNav->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenNav->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenNav->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenNav->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenNav->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenNav->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenNav->setVisible(false);

	SXLevelEditor::EditGreenNav = SXGUICrEdit("", 380, 20, 200, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGreenNav->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenNav->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGreenNav->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGreenNav->setTransparentTextBk(true);
	SXLevelEditor::EditGreenNav->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGreenNav->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGreenNav->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGreenNav->setVisible(false);

	SXLevelEditor::ButtonGreenNav = SXGUICrButton("...", 585, 20, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGreenNav->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenNav->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGreenNav->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGreenNav->setVisible(false);
	SXLevelEditor::ButtonGreenNav->addHandler(SXLevelEditor_ButtonGreenNav_Click, WM_LBUTTONUP);

	SXLevelEditor::TrackBarGreenDensity = SXGUICrTrackBar("", 330, 65, 280, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::TrackBarGreenDensity->setMinMax(1, 100);
	SXLevelEditor::TrackBarGreenDensity->setTickFrequency(10);
	SXLevelEditor::TrackBarGreenDensity->addHandler(SXLevelEditor_TrackBarGreenDensity_MouseMove, WM_MOUSEMOVE);
	SXLevelEditor::TrackBarGreenDensity->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::TrackBarGreenDensity->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::TrackBarGreenDensity->setVisible(false);

	SXLevelEditor::ButtonGreenGenerate = SXGUICrButton("Generate", 420, 87, 100, 30, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGreenGenerate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGreenGenerate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGreenGenerate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGreenGenerate->setVisible(false);
	SXLevelEditor::ButtonGreenGenerate->addHandler(SXLevelEditor_ButtonGreenGenerate_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGreenDensityText = SXGUICrStatic("Density:", 380, 45, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenDensityText->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenDensityText->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenDensityText->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenDensityText->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenDensityText->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenDensityText->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenDensityText->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenDensityText->setVisible(false);

	SXLevelEditor::StaticGreenDensityVal = SXGUICrStatic("0", 430, 45, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenDensityVal->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenDensityVal->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenDensityVal->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenDensityVal->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenDensityVal->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenDensityVal->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenDensityVal->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenDensityVal->setVisible(false);

	SXLevelEditor::RadioButtonGreenSelX = SXGUICrRadioButton("x", 765, 40, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGreenSelX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGreenSelX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGreenSelX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGreenSelX->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGreenSelX->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGreenSelX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGreenSelX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGreenSelX->setVisible(false);

	SXLevelEditor::RadioButtonGreenSelY = SXGUICrRadioButton("y", 765, 60, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGreenSelY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGreenSelY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGreenSelY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGreenSelY->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGreenSelY->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGreenSelY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGreenSelY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGreenSelY->setVisible(false);

	SXLevelEditor::RadioButtonGreenSelZ = SXGUICrRadioButton("z", 765, 80, 25, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonGreenSelZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonGreenSelZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonGreenSelZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonGreenSelZ->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonGreenSelZ->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonGreenSelZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonGreenSelZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonGreenSelZ->setVisible(false);
	
	SXLevelEditor::ComboBoxGreenSel = SXGUICrComboBox(620, 20, 150, 100, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ComboBoxGreenSel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ComboBoxGreenSel->setColorText(RGB(0, 0, 0));
	SXLevelEditor::ComboBoxGreenSel->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::ComboBoxGreenSel->setTransparentTextBk(true);
	SXLevelEditor::ComboBoxGreenSel->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::ComboBoxGreenSel->addItem("single select");
	SXLevelEditor::ComboBoxGreenSel->addItem("single create");
	SXLevelEditor::ComboBoxGreenSel->addItem("multiple create");
	SXLevelEditor::ComboBoxGreenSel->addItem("single delete");
	//SXLevelEditor::ComboBoxGreenSel->AddItem("multiple delete");
	SXLevelEditor::ComboBoxGreenSel->setSel(0);
	SXLevelEditor::ComboBoxGreenSel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ComboBoxGreenSel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ComboBoxGreenSel->setVisible(false);
	
	SXLevelEditor::StaticGreenSelX = SXGUICrStatic("Width volume:", 620, 40, 75, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenSelX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenSelX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenSelX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenSelX->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenSelX->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenSelX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenSelX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenSelX->setVisible(false);

	SXLevelEditor::StaticGreenSelY = SXGUICrStatic("Height volume:", 620, 60, 75, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenSelY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenSelY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenSelY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenSelY->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenSelY->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenSelY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenSelY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenSelY->setVisible(false);

	SXLevelEditor::StaticGreenSelZ = SXGUICrStatic("Depth volume:", 620, 80, 75, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGreenSelZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGreenSelZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGreenSelZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGreenSelZ->setTransparentTextBk(true);
	SXLevelEditor::StaticGreenSelZ->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGreenSelZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGreenSelZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGreenSelZ->setVisible(false);

	SXLevelEditor::EditGreenSelX = SXGUICrEdit("0", 695, 40, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGreenSelX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenSelX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGreenSelX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGreenSelX->setTransparentTextBk(true);
	SXLevelEditor::EditGreenSelX->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGreenSelX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGreenSelX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGreenSelX->setVisible(false);

	SXLevelEditor::EditGreenSelY = SXGUICrEdit("0", 695, 60, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGreenSelY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenSelY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGreenSelY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGreenSelY->setTransparentTextBk(true);
	SXLevelEditor::EditGreenSelY->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGreenSelY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGreenSelY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGreenSelY->setVisible(false);

	SXLevelEditor::EditGreenSelZ = SXGUICrEdit("0", 695, 80, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGreenSelZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGreenSelZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGreenSelZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGreenSelZ->setTransparentTextBk(true);
	SXLevelEditor::EditGreenSelZ->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGreenSelZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGreenSelZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGreenSelZ->setVisible(false);
	//}

	//Game
	//{
	SXLevelEditor::StaticGameClass = SXGUICrStatic("Class:", 415, 15, 35, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGameClass->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGameClass->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGameClass->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGameClass->setTransparentTextBk(true);
	SXLevelEditor::StaticGameClass->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGameClass->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGameClass->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGameClass->setVisible(false);

	SXLevelEditor::ComboBoxGameClass = SXGUICrComboBoxEx(455, 15, 230, 180, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ComboBoxGameClass->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ComboBoxGameClass->setColorText(RGB(0, 0, 0));
	SXLevelEditor::ComboBoxGameClass->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::ComboBoxGameClass->setTransparentTextBk(true);
	SXLevelEditor::ComboBoxGameClass->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::ComboBoxGameClass->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ComboBoxGameClass->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ComboBoxGameClass->setVisible(false);
	SXLevelEditor::ComboBoxGameClass->addItem("");

	SXLevelEditor::ButtonGameTab = SXGUICrButton("Connections", 695, 15, 100, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGameTab->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGameTab->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGameTab->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGameTab->setVisible(false);
	SXLevelEditor::ButtonGameTab->addHandler(SXLevelEditor_ButtonGameTab_Click, WM_LBUTTONUP);
	SXLevelEditor::GameTabVal = 0;

	SXLevelEditor::ListViewGameClass = SXGUICrListView(5, 5, 400, 180, SXLevelEditor::GroupBoxData->getHWND(), WndProcAllDefault, 0);
	SXLevelEditor::ListViewGameClass->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ListViewGameClass->setColorText(RGB(0, 0, 0));
	SXLevelEditor::ListViewGameClass->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::ListViewGameClass->setTransparentTextBk(true);
	SXLevelEditor::ListViewGameClass->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::ListViewGameClass->addColumn("Property Name",199);
	SXLevelEditor::ListViewGameClass->addColumn("Value", 199);
	SXLevelEditor::ListViewGameClass->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ListViewGameClass->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ListViewGameClass->setVisible(false);

	SXLevelEditor::ComboBoxGameValue = SXGUICrComboBox(415, 45, 270, 180, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ComboBoxGameValue->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ComboBoxGameValue->setColorText(RGB(0, 0, 0));
	SXLevelEditor::ComboBoxGameValue->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::ComboBoxGameValue->setTransparentTextBk(true);
	SXLevelEditor::ComboBoxGameValue->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::ComboBoxGameValue->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ComboBoxGameValue->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ComboBoxGameValue->setVisible(false);

	SXLevelEditor::EditGameValue = SXGUICrEdit("", 415, 45, 240, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGameValue->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGameValue->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGameValue->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGameValue->setTransparentTextBk(true);
	SXLevelEditor::EditGameValue->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGameValue->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGameValue->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGameValue->setVisible(false);
	SXLevelEditor::EditGameValue->addHandler(SXLevelEditor_EditGameValue_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGameValue->addHandler(SXLevelEditor_EditGameValue_Enter, WM_KILLFOCUS);

	SXLevelEditor::ButtonGameValue = SXGUICrButton("...", 660, 45, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGameValue->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGameValue->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGameValue->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGameValue->setVisible(false);
	SXLevelEditor::ButtonGameValue->addHandler(SXLevelEditor_ButtonGameValue_Click, WM_LBUTTONUP);

	SXLevelEditor::StaticGameHelp = SXGUICrStatic("Help:", 415, 65, 35, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGameHelp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGameHelp->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGameHelp->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGameHelp->setTransparentTextBk(true);
	SXLevelEditor::StaticGameHelp->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGameHelp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGameHelp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGameHelp->setVisible(false);

	SXLevelEditor::MemoGameHelp = SXGUICrMemo("Help", 415, 85, 270, 95, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::MemoGameHelp->setReadOnly(true);
	SXLevelEditor::MemoGameHelp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::MemoGameHelp->setColorText(RGB(0, 0, 0));
	SXLevelEditor::MemoGameHelp->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::MemoGameHelp->setTransparentTextBk(true);
	SXLevelEditor::MemoGameHelp->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::MemoGameHelp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::MemoGameHelp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::MemoGameHelp->setVisible(false);

	SXLevelEditor::ButtonGameCreate = SXGUICrButton("Create", 695, 165, 100, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGameCreate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGameCreate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGameCreate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGameCreate->setVisible(false);
	SXLevelEditor::ButtonGameCreate->addHandler(SXLevelEditor_ButtonGameCreate_Click, WM_LBUTTONUP);

	for (int i = 0; i < 8; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i] = SXGUICrCheckBox("", 415, 45 + (15*i), 180, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0, false);
		SXLevelEditor::CheckBoxGameFlags[i]->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
		SXLevelEditor::CheckBoxGameFlags[i]->setColorText(RGB(0, 0, 0));
		SXLevelEditor::CheckBoxGameFlags[i]->setColorTextBk(RGB(255, 255, 255));
		SXLevelEditor::CheckBoxGameFlags[i]->setTransparentTextBk(true);
		SXLevelEditor::CheckBoxGameFlags[i]->setColorBrush(RGB(220, 220, 220));
		SXLevelEditor::CheckBoxGameFlags[i]->setFollowParentSide(SXGUI_SIDE_LEFT, true);
		SXLevelEditor::CheckBoxGameFlags[i]->setFollowParentSide(SXGUI_SIDE_TOP, true);
		SXLevelEditor::CheckBoxGameFlags[i]->setVisible(false);
	}

	for (int i = 0; i < 8; ++i)
	{
		SXLevelEditor::CheckBoxGameFlags[i + 8] = SXGUICrCheckBox("", 600, 45 + (15 * i), 180, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0, false);
		SXLevelEditor::CheckBoxGameFlags[i + 8]->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
		SXLevelEditor::CheckBoxGameFlags[i + 8]->setColorText(RGB(0, 0, 0));
		SXLevelEditor::CheckBoxGameFlags[i + 8]->setColorTextBk(RGB(255, 255, 255));
		SXLevelEditor::CheckBoxGameFlags[i + 8]->setTransparentTextBk(true);
		SXLevelEditor::CheckBoxGameFlags[i + 8]->setColorBrush(RGB(220, 220, 220));
		SXLevelEditor::CheckBoxGameFlags[i + 8]->setFollowParentSide(SXGUI_SIDE_LEFT, true);
		SXLevelEditor::CheckBoxGameFlags[i + 8]->setFollowParentSide(SXGUI_SIDE_TOP, true);
		SXLevelEditor::CheckBoxGameFlags[i + 8]->setVisible(false);
	}


	SXLevelEditor::ListViewGameConnections = SXGUICrListView(5, 5, 550, 180, SXLevelEditor::GroupBoxData->getHWND(), WndProcAllDefault, 0);
	SXLevelEditor::ListViewGameConnections->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ListViewGameConnections->setColorText(RGB(0, 0, 0));
	SXLevelEditor::ListViewGameConnections->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::ListViewGameConnections->setTransparentTextBk(true);
	SXLevelEditor::ListViewGameConnections->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::ListViewGameConnections->addColumn("Event", 110);
	SXLevelEditor::ListViewGameConnections->addColumn("Name", 110);
	SXLevelEditor::ListViewGameConnections->addColumn("Action", 110);
	SXLevelEditor::ListViewGameConnections->addColumn("Delay", 110);
	SXLevelEditor::ListViewGameConnections->addColumn("Parameter", 110);
	SXLevelEditor::ListViewGameConnections->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ListViewGameConnections->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ListViewGameConnections->setVisible(false);

	SXLevelEditor::StaticGameConnectionsEvent = SXGUICrStatic("Event:", 560, 50, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGameConnectionsEvent->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGameConnectionsEvent->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGameConnectionsEvent->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGameConnectionsEvent->setTransparentTextBk(true);
	SXLevelEditor::StaticGameConnectionsEvent->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGameConnectionsEvent->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGameConnectionsEvent->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGameConnectionsEvent->setVisible(false);

	SXLevelEditor::ComboBoxGameConnectionsEvent = SXGUICrComboBox(610, 50, 185, 180, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ComboBoxGameConnectionsEvent->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ComboBoxGameConnectionsEvent->setColorText(RGB(0, 0, 0));
	SXLevelEditor::ComboBoxGameConnectionsEvent->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::ComboBoxGameConnectionsEvent->setTransparentTextBk(true);
	SXLevelEditor::ComboBoxGameConnectionsEvent->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::ComboBoxGameConnectionsEvent->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ComboBoxGameConnectionsEvent->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ComboBoxGameConnectionsEvent->setVisible(false);

	SXLevelEditor::StaticGameConnectionsName = SXGUICrStatic("Name:", 560, 75, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGameConnectionsName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGameConnectionsName->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGameConnectionsName->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGameConnectionsName->setTransparentTextBk(true);
	SXLevelEditor::StaticGameConnectionsName->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGameConnectionsName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGameConnectionsName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGameConnectionsName->setVisible(false);

	SXLevelEditor::EditGameConnectionsName = SXGUICrEdit("", 610, 75, 185, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGameConnectionsName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGameConnectionsName->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGameConnectionsName->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGameConnectionsName->setTransparentTextBk(true);
	SXLevelEditor::EditGameConnectionsName->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGameConnectionsName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGameConnectionsName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGameConnectionsName->setVisible(false);
	SXLevelEditor::EditGameConnectionsName->addHandler(SXLevelEditor_EditGameConnectionsName_IN, WM_KEYUP);
	SXLevelEditor::EditGameConnectionsName->addHandler(SXLevelEditor_EditGameConnectionsName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGameConnectionsName->addHandler(SXLevelEditor_EditGameConnectionsName_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticGameConnectionsAction = SXGUICrStatic("Action:", 560, 95, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGameConnectionsAction->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGameConnectionsAction->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGameConnectionsAction->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGameConnectionsAction->setTransparentTextBk(true);
	SXLevelEditor::StaticGameConnectionsAction->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGameConnectionsAction->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGameConnectionsAction->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGameConnectionsAction->setVisible(false);

	SXLevelEditor::ComboBoxGameConnectionsAction = SXGUICrComboBox(610, 95, 185, 135, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ComboBoxGameConnectionsAction->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ComboBoxGameConnectionsAction->setColorText(RGB(0, 0, 0));
	SXLevelEditor::ComboBoxGameConnectionsAction->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::ComboBoxGameConnectionsAction->setTransparentTextBk(true);
	SXLevelEditor::ComboBoxGameConnectionsAction->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::ComboBoxGameConnectionsAction->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ComboBoxGameConnectionsAction->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ComboBoxGameConnectionsAction->setVisible(false);

	SXLevelEditor::StaticGameConnectionsDelay = SXGUICrStatic("Delay:", 560, 120, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGameConnectionsDelay->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGameConnectionsDelay->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGameConnectionsDelay->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGameConnectionsDelay->setTransparentTextBk(true);
	SXLevelEditor::StaticGameConnectionsDelay->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGameConnectionsDelay->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGameConnectionsDelay->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGameConnectionsDelay->setVisible(false);

	SXLevelEditor::EditGameConnectionsDelay = SXGUICrEdit("", 610, 120, 185, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGameConnectionsDelay->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGameConnectionsDelay->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGameConnectionsDelay->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGameConnectionsDelay->setTransparentTextBk(true);
	SXLevelEditor::EditGameConnectionsDelay->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGameConnectionsDelay->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGameConnectionsDelay->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGameConnectionsDelay->setVisible(false);
	SXLevelEditor::EditGameConnectionsDelay->addHandler(SXLevelEditor_EditGameConnections_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGameConnectionsDelay->addHandler(SXLevelEditor_EditGameConnections_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticGameConnectionsParameter = SXGUICrStatic("Parameter:", 560, 140, 50, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticGameConnectionsParameter->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticGameConnectionsParameter->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticGameConnectionsParameter->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticGameConnectionsParameter->setTransparentTextBk(true);
	SXLevelEditor::StaticGameConnectionsParameter->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticGameConnectionsParameter->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticGameConnectionsParameter->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticGameConnectionsParameter->setVisible(false);

	SXLevelEditor::EditGameConnectionsParameter = SXGUICrEdit("", 610, 140, 185, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditGameConnectionsParameter->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditGameConnectionsParameter->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditGameConnectionsParameter->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditGameConnectionsParameter->setTransparentTextBk(true);
	SXLevelEditor::EditGameConnectionsParameter->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditGameConnectionsParameter->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditGameConnectionsParameter->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditGameConnectionsParameter->setVisible(false);
	SXLevelEditor::EditGameConnectionsParameter->addHandler(SXLevelEditor_EditGameConnections_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditGameConnectionsParameter->addHandler(SXLevelEditor_EditGameConnections_Enter, WM_KILLFOCUS);

	
	SXLevelEditor::ButtonGameConnectionsCreate = SXGUICrButton("Create", 565, 160, 100, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGameConnectionsCreate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGameConnectionsCreate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGameConnectionsCreate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGameConnectionsCreate->setVisible(false);
	SXLevelEditor::ButtonGameConnectionsCreate->addHandler(SXLevelEditor_ButtonGameConnectionsCreate_Click, WM_LBUTTONUP);
	SXLevelEditor::isAddGameConections = false;

	SXLevelEditor::ButtonGameConnectionsDelete = SXGUICrButton("Delete", 690, 160, 100, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonGameConnectionsDelete->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonGameConnectionsDelete->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonGameConnectionsDelete->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonGameConnectionsDelete->setVisible(false);
	SXLevelEditor::ButtonGameConnectionsDelete->addHandler(SXLevelEditor_ButtonGameConnectionsDelete_Click, WM_LBUTTONUP);

	//}

	//aigrid
	//{
	SXLevelEditor::StatiAIBBDimensions = SXGUICrStatic("Bound box dimensions:", 5, 10, 120, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StatiAIBBDimensions->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StatiAIBBDimensions->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StatiAIBBDimensions->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StatiAIBBDimensions->setTransparentTextBk(true);
	SXLevelEditor::StatiAIBBDimensions->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StatiAIBBDimensions->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StatiAIBBDimensions->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StatiAIBBDimensions->setVisible(false);

	SXLevelEditor::StaticAIBBDimensionsWidth = SXGUICrStatic("Width:", 5, 30, 40, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIBBDimensionsWidth->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIBBDimensionsWidth->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIBBDimensionsWidth->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIBBDimensionsWidth->setTransparentTextBk(true);
	SXLevelEditor::StaticAIBBDimensionsWidth->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIBBDimensionsWidth->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIBBDimensionsWidth->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIBBDimensionsWidth->setVisible(false);

	SXLevelEditor::EditAIBBDimensionsWidth = SXGUICrEdit("0", 50, 30, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditAIBBDimensionsWidth->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditAIBBDimensionsWidth->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditAIBBDimensionsWidth->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBDimensionsWidth->setTransparentTextBk(true);
	SXLevelEditor::EditAIBBDimensionsWidth->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBDimensionsWidth->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditAIBBDimensionsWidth->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditAIBBDimensionsWidth->setVisible(false);
	SXLevelEditor::EditAIBBDimensionsWidth->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditAIBBDimensionsWidth->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticAIBBDimensionsHeight = SXGUICrStatic("Height:", 5, 50, 40, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIBBDimensionsHeight->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIBBDimensionsHeight->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIBBDimensionsHeight->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIBBDimensionsHeight->setTransparentTextBk(true);
	SXLevelEditor::StaticAIBBDimensionsHeight->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIBBDimensionsHeight->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIBBDimensionsHeight->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIBBDimensionsHeight->setVisible(false);

	SXLevelEditor::EditAIBBDimensionsHeight = SXGUICrEdit("0", 50, 50, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditAIBBDimensionsHeight->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditAIBBDimensionsHeight->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditAIBBDimensionsHeight->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBDimensionsHeight->setTransparentTextBk(true);
	SXLevelEditor::EditAIBBDimensionsHeight->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBDimensionsHeight->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditAIBBDimensionsHeight->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditAIBBDimensionsHeight->setVisible(false);
	SXLevelEditor::EditAIBBDimensionsHeight->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditAIBBDimensionsHeight->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticAIBBDimensionsDepth = SXGUICrStatic("Depth:", 5, 70, 40, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIBBDimensionsDepth->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIBBDimensionsDepth->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIBBDimensionsDepth->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIBBDimensionsDepth->setTransparentTextBk(true);
	SXLevelEditor::StaticAIBBDimensionsDepth->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIBBDimensionsDepth->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIBBDimensionsDepth->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIBBDimensionsDepth->setVisible(false);

	SXLevelEditor::EditAIBBDimensionsDepth = SXGUICrEdit("0", 50, 70, 70, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditAIBBDimensionsDepth->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditAIBBDimensionsDepth->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditAIBBDimensionsDepth->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBDimensionsDepth->setTransparentTextBk(true);
	SXLevelEditor::EditAIBBDimensionsDepth->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBDimensionsDepth->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditAIBBDimensionsDepth->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditAIBBDimensionsDepth->setVisible(false);
	SXLevelEditor::EditAIBBDimensionsDepth->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditAIBBDimensionsDepth->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KILLFOCUS);


	SXLevelEditor::StaticAIBBPos = SXGUICrStatic("Bound box position:", 5, 95, 100, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIBBPos->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIBBPos->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIBBPos->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIBBPos->setTransparentTextBk(true);
	SXLevelEditor::StaticAIBBPos->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIBBPos->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIBBPos->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIBBPos->setVisible(false);

	SXLevelEditor::StaticAIBBPosX = SXGUICrStatic("x:", 5, 115, 10, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIBBPosX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIBBPosX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIBBPosX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIBBPosX->setTransparentTextBk(true);
	SXLevelEditor::StaticAIBBPosX->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIBBPosX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIBBPosX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIBBPosX->setVisible(false);

	SXLevelEditor::EditAIBBPosX = SXGUICrEdit("0", 15, 115, 65, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditAIBBPosX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditAIBBPosX->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditAIBBPosX->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBPosX->setTransparentTextBk(true);
	SXLevelEditor::EditAIBBPosX->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBPosX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditAIBBPosX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditAIBBPosX->setVisible(false);
	SXLevelEditor::EditAIBBPosX->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditAIBBPosX->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticAIBBPosY = SXGUICrStatic("y:", 85, 115, 10, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIBBPosY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIBBPosY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIBBPosY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIBBPosY->setTransparentTextBk(true);
	SXLevelEditor::StaticAIBBPosY->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIBBPosY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIBBPosY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIBBPosY->setVisible(false);

	SXLevelEditor::EditAIBBPosY = SXGUICrEdit("0", 95, 115, 65, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditAIBBPosY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditAIBBPosY->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditAIBBPosY->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBPosY->setTransparentTextBk(true);
	SXLevelEditor::EditAIBBPosY->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBPosY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditAIBBPosY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditAIBBPosY->setVisible(false);
	SXLevelEditor::EditAIBBPosY->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditAIBBPosY->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KILLFOCUS);

	SXLevelEditor::StaticAIBBPosZ = SXGUICrStatic("z:", 165, 115, 10, 15, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIBBPosZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIBBPosZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIBBPosZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIBBPosZ->setTransparentTextBk(true);
	SXLevelEditor::StaticAIBBPosZ->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIBBPosZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIBBPosZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIBBPosZ->setVisible(false);

	SXLevelEditor::EditAIBBPosZ = SXGUICrEdit("0", 175, 115, 65, 15, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditAIBBPosZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditAIBBPosZ->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditAIBBPosZ->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBPosZ->setTransparentTextBk(true);
	SXLevelEditor::EditAIBBPosZ->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditAIBBPosZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditAIBBPosZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditAIBBPosZ->setVisible(false);
	SXLevelEditor::EditAIBBPosZ->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXLevelEditor::EditAIBBPosZ->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KILLFOCUS);

	SXLevelEditor::ButtonAIBBFinish = SXGUICrButton("Create bound box", 15, 140, 100, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonAIBBFinish->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonAIBBFinish->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonAIBBFinish->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonAIBBFinish->setVisible(false);
	SXLevelEditor::ButtonAIBBFinish->addHandler(SXLevelEditor_ButtonAIBBFinish_Click, WM_LBUTTONUP);
	

	SXLevelEditor::RadioButtonAIQuadAdd = SXGUICrRadioButton("AI quad add", 250, 10, 130, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonAIQuadAdd->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonAIQuadAdd->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonAIQuadAdd->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonAIQuadAdd->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonAIQuadAdd->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonAIQuadAdd->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonAIQuadAdd->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonAIQuadAdd->setVisible(false);

	SXLevelEditor::RadioButtonAIQuadsMSel = SXGUICrRadioButton("AI quads multi select", 250, 40, 130, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonAIQuadsMSel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonAIQuadsMSel->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonAIQuadsMSel->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonAIQuadsMSel->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonAIQuadsMSel->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonAIQuadsMSel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonAIQuadsMSel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonAIQuadsMSel->setVisible(false);

	SXLevelEditor::RadioButtonAIQuadsSelDel = SXGUICrRadioButton("AI quads select->delete", 250, 70, 130, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonAIQuadsSelDel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonAIQuadsSelDel->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonAIQuadsSelDel->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonAIQuadsSelDel->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonAIQuadsSelDel->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonAIQuadsSelDel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonAIQuadsSelDel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonAIQuadsSelDel->setVisible(false);

	SXLevelEditor::ButtonAIQuadsDelSel = SXGUICrButton("AI quads delete selected", 250, 100, 130, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonAIQuadsDelSel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonAIQuadsDelSel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonAIQuadsDelSel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonAIQuadsDelSel->setVisible(false);
	SXLevelEditor::ButtonAIQuadsDelSel->addHandler(SXLevelEditor_ButtonAIQuadsDelSel_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonAIGridGen = SXGUICrButton("AI grid generation", 250, 130, 130, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonAIGridGen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonAIGridGen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonAIGridGen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonAIGridGen->setVisible(false);
	SXLevelEditor::ButtonAIGridGen->addHandler(SXLevelEditor_ButtonAIGridGen_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonAIGridClear = SXGUICrButton("AI grid clear", 250, 160, 130, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonAIGridClear->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonAIGridClear->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonAIGridClear->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonAIGridClear->setVisible(false);
	SXLevelEditor::ButtonAIGridClear->addHandler(SXLevelEditor_ButtonAIGridClear_Click, WM_LBUTTONUP);
	
	
	SXLevelEditor::RadioButtonAIGPAdd = SXGUICrRadioButton("Graph point add", 390, 10, 110, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonAIGPAdd->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonAIGPAdd->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonAIGPAdd->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonAIGPAdd->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonAIGPAdd->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonAIGPAdd->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonAIGPAdd->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonAIGPAdd->setVisible(false);

	SXLevelEditor::RadioButtonAIGPDel = SXGUICrRadioButton("Graph point delete", 390, 40, 110, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::RadioButtonAIGPDel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::RadioButtonAIGPDel->setColorText(RGB(0, 0, 0));
	SXLevelEditor::RadioButtonAIGPDel->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::RadioButtonAIGPDel->setTransparentTextBk(true);
	SXLevelEditor::RadioButtonAIGPDel->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::RadioButtonAIGPDel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::RadioButtonAIGPDel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::RadioButtonAIGPDel->setVisible(false);

	SXLevelEditor::ButtonAIGPGen = SXGUICrButton("Graph points generate", 390, 70, 110, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonAIGPGen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonAIGPGen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonAIGPGen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonAIGPGen->setVisible(false);
	SXLevelEditor::ButtonAIGPGen->addHandler(SXLevelEditor_ButtonAIGPGen_Click, WM_LBUTTONUP);

	SXLevelEditor::ButtonAIGPClear = SXGUICrButton("Graph points clear", 390, 100, 110, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonAIGPClear->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonAIGPClear->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonAIGPClear->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonAIGPClear->setVisible(false);
	SXLevelEditor::ButtonAIGPClear->addHandler(SXLevelEditor_ButtonAIGPClear_Click, WM_LBUTTONUP);


	SXLevelEditor::ButtonAIGridValidation = SXGUICrButton("AI grid validation", 510, 10, 100, 20, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonAIGridValidation->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonAIGridValidation->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonAIGridValidation->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonAIGridValidation->setVisible(false);
	SXLevelEditor::ButtonAIGridValidation->addHandler(SXLevelEditor_ButtonAIGridValidation_Click, WM_LBUTTONUP);

	SXLevelEditor::CheckBoxAIGridMarkedSplits = SXGUICrCheckBox("Marked splits", 510, 40, 100, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0, false);
	SXLevelEditor::CheckBoxAIGridMarkedSplits->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::CheckBoxAIGridMarkedSplits->setColorText(RGB(0, 0, 0));
	SXLevelEditor::CheckBoxAIGridMarkedSplits->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::CheckBoxAIGridMarkedSplits->setTransparentTextBk(true);
	SXLevelEditor::CheckBoxAIGridMarkedSplits->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::CheckBoxAIGridMarkedSplits->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::CheckBoxAIGridMarkedSplits->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::CheckBoxAIGridMarkedSplits->setVisible(false);

	SXLevelEditor::StaticAIStatistics = SXGUICrStatic("Statistics:", 620, 10, 100, 20, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIStatistics->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIStatistics->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIStatistics->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIStatistics->setTransparentTextBk(true);
	SXLevelEditor::StaticAIStatistics->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIStatistics->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIStatistics->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIStatistics->setVisible(false);

	SXLevelEditor::StaticAIStatsCountQuads = SXGUICrStatic("Count quads:", 620, 40, 100, 20, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIStatsCountQuads->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIStatsCountQuads->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIStatsCountQuads->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIStatsCountQuads->setTransparentTextBk(true);
	SXLevelEditor::StaticAIStatsCountQuads->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIStatsCountQuads->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIStatsCountQuads->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIStatsCountQuads->setVisible(false);

	SXLevelEditor::StaticAIStatsCountGP = SXGUICrStatic("Count graph points:", 620, 70, 100, 20, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIStatsCountGP->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIStatsCountGP->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIStatsCountGP->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIStatsCountGP->setTransparentTextBk(true);
	SXLevelEditor::StaticAIStatsCountGP->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIStatsCountGP->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIStatsCountGP->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIStatsCountGP->setVisible(false);

	SXLevelEditor::StaticAIStatsCountSplits = SXGUICrStatic("Count splits:", 620, 100, 100, 20, SXLevelEditor::GroupBoxData->getHWND(), 0);
	SXLevelEditor::StaticAIStatsCountSplits->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::StaticAIStatsCountSplits->setColorText(RGB(0, 0, 0));
	SXLevelEditor::StaticAIStatsCountSplits->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::StaticAIStatsCountSplits->setTransparentTextBk(true);
	SXLevelEditor::StaticAIStatsCountSplits->setColorBrush(RGB(220, 220, 220));
	SXLevelEditor::StaticAIStatsCountSplits->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::StaticAIStatsCountSplits->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::StaticAIStatsCountSplits->setVisible(false);

	SXLevelEditor::EditAIStatsCountQuads = SXGUICrEdit("0", 720, 40, 70, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditAIStatsCountQuads->setReadOnly(true);
	SXLevelEditor::EditAIStatsCountQuads->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditAIStatsCountQuads->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditAIStatsCountQuads->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditAIStatsCountQuads->setTransparentTextBk(true);
	SXLevelEditor::EditAIStatsCountQuads->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditAIStatsCountQuads->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditAIStatsCountQuads->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditAIStatsCountQuads->setVisible(false);

	SXLevelEditor::EditAIStatsCountGP = SXGUICrEdit("0", 720, 70, 70, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditAIStatsCountGP->setReadOnly(true);
	SXLevelEditor::EditAIStatsCountGP->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditAIStatsCountGP->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditAIStatsCountGP->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditAIStatsCountGP->setTransparentTextBk(true);
	SXLevelEditor::EditAIStatsCountGP->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditAIStatsCountGP->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditAIStatsCountGP->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditAIStatsCountGP->setVisible(false);

	SXLevelEditor::EditAIStatsCountSplits = SXGUICrEdit("0", 720, 100, 70, 20, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::EditAIStatsCountSplits->setReadOnly(true);
	SXLevelEditor::EditAIStatsCountSplits->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::EditAIStatsCountSplits->setColorText(RGB(0, 0, 0));
	SXLevelEditor::EditAIStatsCountSplits->setColorTextBk(RGB(255, 255, 255));
	SXLevelEditor::EditAIStatsCountSplits->setTransparentTextBk(true);
	SXLevelEditor::EditAIStatsCountSplits->setColorBrush(RGB(255, 255, 255));
	SXLevelEditor::EditAIStatsCountSplits->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::EditAIStatsCountSplits->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::EditAIStatsCountSplits->setVisible(false);

	SXLevelEditor::ButtonAIClearAll = SXGUICrButton("Clear all", 660, 130, 130, 30, SXGUI_BUTTON_IMAGE_NONE, SXLevelEditor::GroupBoxData->getHWND(), 0, 0);
	SXLevelEditor::ButtonAIClearAll->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXLevelEditor::ButtonAIClearAll->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXLevelEditor::ButtonAIClearAll->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXLevelEditor::ButtonAIClearAll->setVisible(false);
	SXLevelEditor::ButtonAIClearAll->addHandler(SXLevelEditor_ButtonAIClearAll_Click, WM_LBUTTONUP);
	//}
}

void SXLevelEditor::DeleteAllElements()
{
	mem_release(JobWindow);
	mem_release(MainMenu);
	mem_release(RenderWindow);

	mem_release(ToolBar1);
	mem_release(ButtonTBNew);
	mem_release(ButtonTBOpen);
	mem_release(ButtonTBSave);
	mem_release(ButtonTBSaveAs);

	mem_release(CheckBoxTBArrow);
	mem_release(CheckBoxTBPos);
	mem_release(CheckBoxTBRot);
	mem_release(CheckBoxTBScale);

	mem_release(CheckBoxTBGrid);
	mem_release(CheckBoxTBAxes);

	mem_release(CheckBoxTBRColor);
	mem_release(CheckBoxTBRNormal);
	mem_release(CheckBoxTBRParam);
	mem_release(CheckBoxTBRAmDiff);
	mem_release(CheckBoxTBRSpecular);
	mem_release(CheckBoxTBRLighting);

	mem_release(CheckBoxTBSelS);
	mem_release(CheckBoxTBSelZTest);
	mem_release(CheckBoxTBSelMesh);
	mem_release(CheckBoxTBSelCullBack);

	mem_release(CheckBoxTBAIGBound);
	mem_release(CheckBoxTBAIGQuad);
	mem_release(CheckBoxTBAIGGraphPoint);

	mem_release(GroupBoxList);
	mem_release(GroupBoxData);
	mem_release(ListBoxList);
	mem_release(StaticListTextCount);
	mem_release(StaticListValCount);

	mem_release(ButtonDelete);

	mem_release(ButtonGeometryOpen);
	mem_release(ButtonGreenOpen);
	mem_release(ButtonGameObjectOpen);
	mem_release(ButtonAIGridOpen);
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

	mem_delete(SXLevelEditor::ButtonAIQuadsDelSel);
	mem_delete(SXLevelEditor::ButtonAIGridGen);
	mem_delete(SXLevelEditor::ButtonAIClearAll);
	mem_delete(SXLevelEditor::StatiAIBBDimensions);
	mem_delete(SXLevelEditor::StaticAIBBDimensionsWidth);
	mem_delete(SXLevelEditor::EditAIBBDimensionsWidth);
	mem_delete(SXLevelEditor::StaticAIBBDimensionsHeight);
	mem_delete(SXLevelEditor::EditAIBBDimensionsHeight);
	mem_delete(SXLevelEditor::StaticAIBBDimensionsDepth);
	mem_delete(SXLevelEditor::EditAIBBDimensionsDepth);
	mem_delete(SXLevelEditor::StaticAIBBPos);
	mem_delete(SXLevelEditor::StaticAIBBPosX);
	mem_delete(SXLevelEditor::EditAIBBPosX);
	mem_delete(SXLevelEditor::StaticAIBBPosY);
	mem_delete(SXLevelEditor::EditAIBBPosY);
	mem_delete(SXLevelEditor::StaticAIBBPosZ);
	mem_delete(SXLevelEditor::EditAIBBPosZ);
	mem_delete(SXLevelEditor::ButtonAIBBFinish);
	mem_delete(SXLevelEditor::ButtonAIGPGen);
	mem_delete(SXLevelEditor::ButtonAIGPClear);
	mem_delete(SXLevelEditor::RadioButtonAIGPAdd);
	mem_delete(SXLevelEditor::RadioButtonAIGPDel);
	mem_delete(SXLevelEditor::RadioButtonAIQuadAdd);
	mem_delete(SXLevelEditor::RadioButtonAIQuadsMSel);
	mem_delete(SXLevelEditor::RadioButtonAIQuadsSelDel);
	mem_delete(SXLevelEditor::ButtonAIGridValidation);
	mem_delete(SXLevelEditor::CheckBoxAIGridMarkedSplits);
	mem_delete(SXLevelEditor::StaticAIStatistics);
	mem_delete(SXLevelEditor::StaticAIStatsCountQuads);
	mem_delete(SXLevelEditor::StaticAIStatsCountGP);
	mem_delete(SXLevelEditor::StaticAIStatsCountSplits);
	mem_delete(SXLevelEditor::EditAIStatsCountQuads);
	mem_delete(SXLevelEditor::EditAIStatsCountGP);
	mem_delete(SXLevelEditor::EditAIStatsCountSplits);
}

//##########################################################################

void SXLevelEditor::LevelEditorUpdate(DWORD timeDelta)
{
	static float3 vCamPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vCamPos);

	static const float * r_far = GET_PCVAR_FLOAT("r_far");

	long count_poly_green = 0;
	for (int i = 0; i < SGeom_GreenGetCount(); ++i)
	{
		count_poly_green += SGeom_GreenMGetCountGen(i) * SGeom_GreenMGetCountPoly(i);
	}

	long count_poly_geom = 0;
	for (int i = 0; i < SGeom_ModelsGetCount(); ++i)
	{
		count_poly_geom += SGeom_ModelsMGetCountPoly(i);
	}

	char text[256];
	sprintf(text, "%s%d", "Level poly: ", count_poly_geom + count_poly_green);
	SXLevelEditor::StatusBar1->setPartText(0, text);

	sprintf(text, "%s%d", "Geom poly: ", count_poly_geom);
	SXLevelEditor::StatusBar1->setPartText(1, text);

	sprintf(text, "%s%d", "Green poly: ", count_poly_green);
	SXLevelEditor::StatusBar1->setPartText(2, text);

	sprintf(text, "%s%d", "Count game object: ", SXGame_EntGetCount());
	SXLevelEditor::StatusBar1->setPartText(3, text);

	if (SXLevelEditor::IdMtl >= 0)
	{
		//sprintf(text, "%s", EDITORS_LEVEL_STATUSBAR_GAME_COUNT, SXGame_EntGetCount());
		SML_MtlGetTexture(SXLevelEditor::IdMtl, text);
		SXLevelEditor::StatusBar1->setPartText(4, text);
	}



	if (SXLevelEditor::GreenRenderBox)
	{
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		float3 tmppos = SXLevelEditor::GreenBoxPos;
		tmppos.y += SXLevelEditor::GreenBoxWHD.y * 0.5f;
		SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)(SMMatrixScaling(SXLevelEditor::GreenBoxWHD) * SMMatrixTranslation(tmppos)));
		SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(SRender_EditorGetSelectTex()));
		SXLevelEditor::FigureBox->DrawSubset(0);

		SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	if (SXLevelEditor::SelSelection)
	{
		if (SXLevelEditor::SelZTest)
		{
			SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		}
		else
		{
			SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		}

		if (SXLevelEditor::SelMesh)
			SGCore_GetDXDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		if (SXLevelEditor::SelBackFacesCull)
			SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);



		if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
		{
			if (SXLevelEditor::ActiveElement > -1)
			{
				SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(SRender_EditorGetSelectTex()));
				SGeom_ModelsRenderSingly(timeDelta, SXLevelEditor::ActiveElement, SML_MtlGetStdMtl(MTLTYPE_MODEL_STATIC));
			}
		}

		if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (SXLevelEditor::ActiveElement > -1)
			{
				if (SXLevelEditor::ActiveGreenSplit >= 0 && SXLevelEditor::ActiveGreenObject >= 0)
				{
					SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(SRender_EditorGetSelectTex()));
					SGeom_GreenRenderObject(timeDelta, &vCamPos, SXLevelEditor::ActiveElement, SXLevelEditor::ActiveGreenSplit, SXLevelEditor::ActiveGreenObject, SML_MtlGetStdMtl(MTLTYPE_MODEL_TREE));
				}
				else
				{
					SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(SRender_EditorGetSelectTex()));
					SGeom_GreenRenderSingly(timeDelta, &vCamPos, SXLevelEditor::ActiveElement, SML_MtlGetStdMtl(MTLTYPE_MODEL_TREE));
				}
			}
		}

		if (SXLevelEditor::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME)
		{
			SXGame_EditorRender(SXLevelEditor::ActiveElement, SRender_EditorGetSelectTex());
		}

		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

		if (SXLevelEditor::SelMesh)
			SGCore_GetDXDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		if (SXLevelEditor::SelBackFacesCull)
			SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}


	SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (SXLevelEditor::AIGQuad)
		SAIG_RenderQuads(SRender_GetCamera()->getFrustum(), &vCamPos, *r_far);

	if (SXLevelEditor::AIGGraphPoint)
		SAIG_RenderGraphPoints(&vCamPos, *r_far);

	if (SXLevelEditor::AIGBound)
	{
		SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		SGCore_GetDXDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		SGCore_GetDXDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		SGCore_GetDXDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
		SAIG_RenderBB();
		SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	if (SXLevelEditor::ObjAxesHelper)
	{
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		SXLevelEditor::ObjAxesHelper->Render();
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	}
}

//##########################################################################

void SXLevelEditor::LEcreateData()
{
	SXLevelEditor::ObjAxesHelper = new AxesHelper();
	D3DXCreateBox(SGCore_GetDXDevice(), 1, 1, 1, &SXLevelEditor::FigureBox, 0);
}

void SXLevelEditor::LEdeleteData()
{
	mem_delete(SXLevelEditor::ObjAxesHelper);
	mem_release(SXLevelEditor::FigureBox);
}