
#include "level_editor.h"

namespace level_editor
{
	ISXGUIBaseWnd *pJobWindow;
	ISXGUIMenuWindow *pMainMenu;
	ISXGUIBaseWnd *pRenderWindow;

	ISXGUIToolBar *pToolBar1;
	ISXGUIButton *pButtonTBNew;
	ISXGUIButton *pButtonTBOpen;
	ISXGUIButton *pButtonTBSave;
	ISXGUIButton *pButtonTBSaveAs;

	ISXGUICheckBox *pCheckBoxTBArrow;
	ISXGUICheckBox *pCheckBoxTBPos;
	ISXGUICheckBox *pCheckBoxTBRot;
	ISXGUICheckBox *pCheckBoxTBScale;

	ISXGUICheckBox *pCheckBoxTBGrid;
	ISXGUICheckBox *pCheckBoxTBAxes;

	ISXGUICheckBox *pCheckBoxTBRColor;
	ISXGUICheckBox *pCheckBoxTBRNormal;
	ISXGUICheckBox *pCheckBoxTBRParam;
	ISXGUICheckBox *pCheckBoxTBRAmDiff;
	ISXGUICheckBox *pCheckBoxTBRSpecular;
	ISXGUICheckBox *pCheckBoxTBRLighting;

	ISXGUICheckBox *pCheckBoxTBSelS;
	ISXGUICheckBox *pCheckBoxTBSelZTest;
	ISXGUICheckBox *pCheckBoxTBSelMesh;
	ISXGUICheckBox *pCheckBoxTBSelCullBack;

	ISXGUICheckBox *pCheckBoxTBAIGBound;
	ISXGUICheckBox *pCheckBoxTBAIGQuad;
	ISXGUICheckBox *pCheckBoxTBAIGGraphPoint;

	ISXGUICheckBox *pCheckBoxTBLevelType;
	ISXGUICheckBox *pCheckBoxTBGLightEnable;

	ISXGUIGroupBox *pGroupBoxList;
	ISXGUIGroupBox *pGroupBoxData;
	ISXGUIListBox *pListBoxList;
	ISXGUIStatic *pStaticListTextCount;
	ISXGUIStatic *pStaticListValCount;

	ISXGUIButton *pButtonDelete;

	ISXGUIButton *pButtonGeometryOpen;
	ISXGUIButton *pButtonGreenOpen;
	ISXGUIButton *pButtonGameObjectOpen;
	ISXGUIButton *pButtonAIGridOpen;


	//model
	//{{
	ISXGUIStatic *pStaticGeomName;
	ISXGUIEdit *pEditGeomName;

	ISXGUIStatic *pStaticGeomModel;
	ISXGUIEdit *pEditGeomModel;
	ISXGUIButton *pButtonGeomModel;

	ISXGUIStatic *pStaticGeomLod1;
	ISXGUIEdit *pEditGeomLod1;
	ISXGUIButton *pButtonGeomLod1;

	ISXGUIStatic *pStaticGeomPhysics;
	ISXGUIEdit *pEditGeomPhysics;
	ISXGUIButton *pButtonGeomPhysics;

	ISXGUICheckBox *pCheckBoxSegmentation;
	
	ISXGUIStatic *pStaticGeomPos;
	ISXGUIEdit *pEditGeomPosX;
	ISXGUIEdit *pEditGeomPosY;
	ISXGUIEdit *pEditGeomPosZ;
	ISXGUIRadioButton *pRadioButtonGeomPosX;
	ISXGUIRadioButton *pRadioButtonGeomPosY;
	ISXGUIRadioButton *pRadioButtonGeomPosZ;
	ISXGUIStatic *pStaticGeomRot;
	ISXGUIEdit *pEditGeomRotX;
	ISXGUIEdit *pEditGeomRotY;
	ISXGUIEdit *pEditGeomRotZ;
	ISXGUIRadioButton *pRadioButtonGeomRotX;
	ISXGUIRadioButton *pRadioButtonGeomRotY;
	ISXGUIRadioButton *pRadioButtonGeomRotZ;
	ISXGUIStatic *pStaticGeomScale;
	ISXGUIEdit *pEditGeomScaleX;
	ISXGUIEdit *pEditGeomScaleY;
	ISXGUIEdit *pEditGeomScaleZ;
	ISXGUIRadioButton *pRadioButtonGeomScaleX;
	ISXGUIRadioButton *pRadioButtonGeomScaleY;
	ISXGUIRadioButton *pRadioButtonGeomScaleZ;
	ISXGUIButton *pButtonGeomFinish;
	//}}

	//Green
	//{{
	ISXGUIStatic *pStaticGreenName;
	ISXGUIEdit *pEditGreenName;

	ISXGUIStatic *pStaticGreenModel;
	ISXGUIEdit *pEditGreenModel;
	ISXGUIButton *pButtonGreenModel;

	ISXGUIStatic *pStaticGreenLod1;
	ISXGUIEdit *pEditGreenLod1;
	ISXGUIButton *pButtonGreenLod1;

	ISXGUIStatic *pStaticGreenLod2;
	ISXGUIEdit *pEditGreenLod2;
	ISXGUIButton *pButtonGreenLod2;

	ISXGUIStatic *pStaticGreenMask;
	ISXGUIEdit *pEditGreenMask;
	ISXGUIButton *pButtonGreenMask;

	ISXGUICheckBox *pCheckBoxGreenAveragedRGB;
	
	ISXGUIStatic *pStaticGreenNav;
	ISXGUIEdit *pEditGreenNav;
	ISXGUIButton *pButtonGreenNav;

	ISXGUITrackBar *pTrackBarGreenDensity;
	ISXGUIButton *pButtonGreenGenerate;
	ISXGUIStatic *pStaticGreenDensityText;
	ISXGUIStatic *pStaticGreenDensityVal;

	ISXGUIEdit *pEditGreenSelX;
	ISXGUIEdit *pEditGreenSelY;
	ISXGUIEdit *pEditGreenSelZ;
	ISXGUIRadioButton *pRadioButtonGreenSelX;
	ISXGUIRadioButton *pRadioButtonGreenSelY;
	ISXGUIRadioButton *pRadioButtonGreenSelZ;
	ISXGUIComboBox *pComboBoxGreenSel;
	ISXGUIStatic *pStaticGreenSelX;
	ISXGUIStatic *pStaticGreenSelY;
	ISXGUIStatic *pStaticGreenSelZ;
	//}}

	//game object
	//{
	ISXGUIStatic *pStaticGameClass;
	ISXGUIComboBox *pComboBoxGameClass;
	ISXGUIButton *pButtonGameTab;
	int GameTabVal = 0;
	ISXGUIListView *pListViewGameClass;
	ISXGUIComboBox *pComboBoxGameValue;
	ISXGUIEdit *pEditGameValue;
	ISXGUIButton *pButtonGameValue;
	ISXGUIStatic *pStaticGameHelp;
	ISXGUIMemo *pMemoGameHelp;
	ISXGUIButton *pButtonGameCreate;

	ISXGUICheckBox *pCheckBoxGameFlags[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	const char *aGameObjectFlags[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	ISXGUIListView *pListViewGameConnections;
	ISXGUIStatic *pStaticGameConnectionsEvent;
	ISXGUIComboBox *pComboBoxGameConnectionsEvent;
	ISXGUIStatic *pStaticGameConnectionsName;
	ISXGUIEdit *pEditGameConnectionsName;
	ISXGUIStatic *pStaticGameConnectionsAction;
	ISXGUIComboBox *pComboBoxGameConnectionsAction;
	ISXGUIStatic *pStaticGameConnectionsDelay;
	ISXGUIEdit *pEditGameConnectionsDelay;
	ISXGUIStatic *pStaticGameConnectionsParameter;
	ISXGUIEdit *pEditGameConnectionsParameter;
	ISXGUIButton *pButtonGameConnectionsCreate;
	ISXGUIButton *pButtonGameConnectionsDelete;
	//}

	//aigrid
	//{
	ISXGUIButton *pButtonAIQuadsDelSel;
	ISXGUIButton *pButtonAIGridGen;
	ISXGUIButton *pButtonAIGridClear;
	ISXGUIButton *pButtonAIClearAll;
	ISXGUIStatic *pStaticAIBBDimensions;
	ISXGUIStatic *pStaticAIBBDimensionsWidth;
	ISXGUIEdit *pEditAIBBDimensionsWidth;
	ISXGUIStatic *pStaticAIBBDimensionsHeight;
	ISXGUIEdit *pEditAIBBDimensionsHeight;
	ISXGUIStatic *pStaticAIBBDimensionsDepth;
	ISXGUIEdit *pEditAIBBDimensionsDepth;
	ISXGUIStatic *pStaticAIBBPos;
	ISXGUIStatic *pStaticAIBBPosX;
	ISXGUIEdit *pEditAIBBPosX;
	ISXGUIStatic *pStaticAIBBPosY;
	ISXGUIEdit *pEditAIBBPosY;
	ISXGUIStatic *pStaticAIBBPosZ;
	ISXGUIEdit *pEditAIBBPosZ;
	ISXGUIButton *pButtonAIBBFinish;
	ISXGUIButton *pButtonAIGPGen;
	ISXGUIButton *pButtonAIGPClear;
	ISXGUIRadioButton *pRadioButtonAIGPAdd;
	ISXGUIRadioButton *pRadioButtonAIGPDel;
	ISXGUIRadioButton *pRadioButtonAIQuadAdd;
	ISXGUIRadioButton *pRadioButtonAIQuadsMSel;
	ISXGUIRadioButton *pRadioButtonAIQuadsSelDel;
	ISXGUIButton *pButtonAIGridValidation;
	ISXGUICheckBox *pCheckBoxAIGridMarkedSplits;
	ISXGUIStatic *pStaticAIStatistics;
	ISXGUIStatic *pStaticAIStatsCountQuads;
	ISXGUIStatic *pStaticAIStatsCountGP;
	ISXGUIStatic *pStaticAIStatsCountSplits;
	ISXGUIEdit *pEditAIStatsCountQuads;
	ISXGUIEdit *pEditAIStatsCountGP;
	ISXGUIEdit *pEditAIStatsCountSplits;
	//}

	ISXGUIStatusBar *pStatusBar1;

	


	float3 vRayOrigin;
	float3 vRayDir;
	float3 vRayDirDirect;
	POINT oPointMouse;

	bool useCopyData = false;
	ID idCopy = -1;
	float3 vCopyPos;

	ID3DXMesh *pFigureBox = 0;
	CAxesHelper *pAxesHelper = 0;

	int iActiveGroupType = 0;		//!< текущая выделенная группа мировых сущностей EDITORS_LEVEL_GROUPTYPE_
	ID idActiveGreenSplit = -1;		//!< текущий идентификатор сплита растительность (если выделена растительность)
	ID idActiveGreenObject = -1;		//!< текущий идентификатор объекта растительности (если выделена растительность)

	ID idActiveElement = -1;			//!< текущий идентификатор выделенного элемента из списка
	bool canSelSelection = false;		//!< разрешено ли выделение?
	bool canSelZTest = false;			//!< использовать ли z-test при выделении?
	bool canSelMesh = false;			//!< рисовать сеткой (true) или целиком модель выделения (false)?
	bool canSelBackFacesCull = false;	//!< отсекать ли задние грани при выделении?

	bool canAIGBound = true;			//!< отрисовка боунда ai сетки
	bool canAIGQuad = true;			//!< отрисовка квадов ai сетки
	bool canAIGGraphPoint = true;		//!< отрисовка графпоинтов ai сетки

	//bound box для массового создания объектов растительности
	bool canGreenRenderBox = false;	//!< разрешено ли рисовать бокс?
	float3 vGreenBoxPos;				//!< позиция бокса
	float3_t vGreenBoxWHD(1, 1, 1);	//!< ширина, высота, длина бокса

	float3 vHelperPos;
	float3 vHelperRot;
	float3 vHelperScale;

	bool isAddGameConections = false;

	ID idMtl = -1;
	ID idMenuWeatherCurr = -1;
	int iMenuWeatherCount = 0;
	Array<String> aMenuWeather;
};

//##########################################################################

bool level_editor::existsFileStaticGeom(const char *szRelPath)
{
	char szPath[1024];
	sprintf(szPath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES), szRelPath);
	return FileExistsFile(szPath);
}

void level_editor::InitAllElements()
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int cx = (wrect.right - MAINWIN_SIZE_X) / 2;
	int cy = (wrect.bottom - MAINWIN_SIZE_Y) / 2;

	level_editor::pJobWindow = SXGUICrBaseWndEx("LevelEditor", (String(SX_LEVEL_EDITOR_NAME) + " | " + SKYXENGINE_VERSION4EDITORS).c_str(), cx, cy, MAINWIN_SIZE_X, MAINWIN_SIZE_Y, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	//level_editor::pJobWindow->setVisible(false);
	gui_func::base_handlers::InitHandlerMsg(level_editor::pJobWindow);

	level_editor::pJobWindow->addHandler(ComMenuId, WM_COMMAND);
	level_editor::pJobWindow->addHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	level_editor::pJobWindow->addHandler(MsgEditSize, WM_SIZE);
	level_editor::pJobWindow->setMixSize(MAINWIN_SIZE_X, MAINWIN_SIZE_Y);
	level_editor::pMainMenu = SXGUICrMenuWindowEx(IDR_MENU1);
	level_editor::pMainMenu->setToWindow(level_editor::pJobWindow->getHWND());	

	level_editor::pRenderWindow = SXGUICrBaseWndEx("RenderWindow", "RenderWindow", 0, 27, 600, 400, 0, LoadCursor(NULL, IDC_ARROW), CreateSolidBrush(RGB(200, 200, 200)), 0, CS_HREDRAW | CS_VREDRAW, WS_VISIBLE | WS_BORDER, level_editor::pJobWindow->getHWND(), WndProcAllDefault);
	level_editor::pRenderWindow->setFollowParentSides(true, true, true, true);
	level_editor::pRenderWindow->addHandler(SXLevelEditor_RenderWindow_MouseMove, WM_MOUSEMOVE);
	level_editor::pRenderWindow->addHandler(SXLevelEditor_RenderWindow_LDown, WM_LBUTTONDOWN);
	level_editor::pRenderWindow->addHandler(SXLevelEditor_RenderWindow_LClick, WM_LBUTTONUP);
	level_editor::pRenderWindow->addHandler(SXLevelEditor_RenderWindow_RClick, WM_RBUTTONUP);
	level_editor::pRenderWindow->addHandler(SXLevelEditor_RenderWindow_MBUp, WM_MBUTTONUP);
	level_editor::pRenderWindow->addHandler(SXLevelEditor_RenderWindow_Delete, WM_KEYUP, VK_DELETE, 1, 0, 0, 0);

	level_editor::pToolBar1 = SXGUICrToolBar(0, 1, 810, 26, level_editor::pJobWindow->getHWND(), WndProcAllDefault, 0);
	level_editor::pToolBar1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pToolBar1->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	level_editor::pToolBar1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pToolBar1->setFollowParentSide(SXGUI_SIDE_BOTTOM, false);
	level_editor::pToolBar1->addHandler(SXLevelEditor_ToolBar1_CallWmCommand, WM_COMMAND);

	level_editor::pButtonTBNew = SXGUICrButtonEx("", 2, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pButtonTBNew->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonTBNew->setFollowParentSides(true, false, false, true);
	level_editor::pButtonTBNew->setBmpFromResourse(IDB_BITMAP1);
	level_editor::pButtonTBNew->setHintText("Create a new level");

	level_editor::pButtonTBOpen = SXGUICrButtonEx("", 26, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pButtonTBOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonTBOpen->setFollowParentSides(true, false, false, true);
	level_editor::pButtonTBOpen->setBmpFromResourse(IDB_BITMAP2);
	level_editor::pButtonTBOpen->setHintText("Open level");

	level_editor::pButtonTBSave = SXGUICrButtonEx("", 50, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pButtonTBSave->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonTBSave->setFollowParentSides(true, false, false, true);
	level_editor::pButtonTBSave->setBmpFromResourse(IDB_BITMAP4);
	level_editor::pButtonTBSave->setHintText("Save level");

	level_editor::pButtonTBSaveAs = SXGUICrButtonEx("", 74, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pButtonTBSaveAs->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonTBSaveAs->setFollowParentSides(true, false, false, true);
	level_editor::pButtonTBSaveAs->setBmpFromResourse(IDB_BITMAP3);
	level_editor::pButtonTBSaveAs->setHintText("Save level as ...");

	level_editor::pCheckBoxTBArrow = SXGUICrCheckBoxEx("", 104, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBArrow->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBArrow->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBArrow->setBmpFromResourse(IDB_BITMAP5);
	level_editor::pCheckBoxTBArrow->setHintText("Select object");

	level_editor::pCheckBoxTBPos = SXGUICrCheckBoxEx("", 128, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBPos->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBPos->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBPos->setBmpFromResourse(IDB_BITMAP6);
	level_editor::pCheckBoxTBPos->setHintText("Change position (helper)");

	level_editor::pCheckBoxTBRot = SXGUICrCheckBoxEx("", 152, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBRot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBRot->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBRot->setBmpFromResourse(IDB_BITMAP7);
	level_editor::pCheckBoxTBRot->setHintText("Change rotation (helper)");

	level_editor::pCheckBoxTBScale = SXGUICrCheckBoxEx("", 176, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBScale->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBScale->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBScale->setBmpFromResourse(IDB_BITMAP8);
	level_editor::pCheckBoxTBScale->setHintText("Change scale (helper)");


	level_editor::pCheckBoxTBGrid = SXGUICrCheckBoxEx("", 206, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBGrid->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBGrid->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBGrid->setBmpFromResourse(IDB_BITMAP9);
	level_editor::pCheckBoxTBGrid->setHintText("Show grid");

	level_editor::pCheckBoxTBAxes = SXGUICrCheckBoxEx("", 230, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBAxes->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBAxes->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBAxes->setBmpFromResourse(IDB_BITMAP10);
	level_editor::pCheckBoxTBAxes->setHintText("Show axes");


	level_editor::pCheckBoxTBRColor = SXGUICrCheckBoxEx("", 260, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBRColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBRColor->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBRColor->setBmpFromResourse(IDB_BITMAP11);
	level_editor::pCheckBoxTBRColor->setHintText("Show color of scene");

	level_editor::pCheckBoxTBRNormal = SXGUICrCheckBoxEx("", 284, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBRNormal->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBRNormal->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBRNormal->setBmpFromResourse(IDB_BITMAP12);
	level_editor::pCheckBoxTBRNormal->setHintText("Show normals of scene");

	level_editor::pCheckBoxTBRParam = SXGUICrCheckBoxEx("", 308, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBRParam->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBRParam->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBRParam->setBmpFromResourse(IDB_BITMAP13);
	level_editor::pCheckBoxTBRParam->setHintText("Show parameters lighting of scene");

	level_editor::pCheckBoxTBRAmDiff = SXGUICrCheckBoxEx("", 332, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBRAmDiff->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBRAmDiff->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBRAmDiff->setBmpFromResourse(IDB_BITMAP14);
	level_editor::pCheckBoxTBRAmDiff->setHintText("Show ambient/diffuse components of scene");

	level_editor::pCheckBoxTBRSpecular = SXGUICrCheckBoxEx("", 356, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBRSpecular->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBRSpecular->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBRSpecular->setBmpFromResourse(IDB_BITMAP15);
	level_editor::pCheckBoxTBRSpecular->setHintText("Show specular of scene");

	level_editor::pCheckBoxTBRLighting = SXGUICrCheckBoxEx("", 380, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBRLighting->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBRLighting->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBRLighting->setBmpFromResourse(IDB_BITMAP16);
	level_editor::pCheckBoxTBRLighting->setHintText("Show lighting of scene");

	
	level_editor::pCheckBoxTBSelS = SXGUICrCheckBoxEx("", 410, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBSelS->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBSelS->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBSelS->setBmpFromResourse(IDB_BITMAP17);
	level_editor::pCheckBoxTBSelS->setHintText("Show selected object");

	level_editor::pCheckBoxTBSelZTest = SXGUICrCheckBoxEx("", 434, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBSelZTest->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBSelZTest->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBSelZTest->setBmpFromResourse(IDB_BITMAP18);
	level_editor::pCheckBoxTBSelZTest->setHintText("Use Z-test for show selected object");

	level_editor::pCheckBoxTBSelMesh = SXGUICrCheckBoxEx("", 458, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBSelMesh->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBSelMesh->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBSelMesh->setBmpFromResourse(IDB_BITMAP19);
	level_editor::pCheckBoxTBSelMesh->setHintText("Draw mesh for show selected object");

	level_editor::pCheckBoxTBSelCullBack = SXGUICrCheckBoxEx("", 482, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBSelCullBack->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBSelCullBack->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBSelCullBack->setBmpFromResourse(IDB_BITMAP20);
	level_editor::pCheckBoxTBSelCullBack->setHintText("Use cullback edges for show selected object");


	level_editor::pCheckBoxTBAIGBound = SXGUICrCheckBoxEx("", 512, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBAIGBound->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBAIGBound->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBAIGBound->setBmpFromResourse(IDB_BITMAP22);
	level_editor::pCheckBoxTBAIGBound->setHintText("Show boxes for AI grid");

	level_editor::pCheckBoxTBAIGQuad = SXGUICrCheckBoxEx("", 536, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBAIGQuad->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBAIGQuad->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBAIGQuad->setBmpFromResourse(IDB_BITMAP23);
	level_editor::pCheckBoxTBAIGQuad->setHintText("Show quads for AI grid");

	level_editor::pCheckBoxTBAIGGraphPoint = SXGUICrCheckBoxEx("", 560, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBAIGGraphPoint->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBAIGGraphPoint->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBAIGGraphPoint->setBmpFromResourse(IDB_BITMAP24);
	level_editor::pCheckBoxTBAIGGraphPoint->setHintText("Show graphpoints for AI grid");

	level_editor::pCheckBoxTBLevelType = SXGUICrCheckBoxEx("", 588, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBLevelType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBLevelType->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
	level_editor::pCheckBoxTBLevelType->setHintText("Indoor/outdoor type of level");

	level_editor::pCheckBoxTBGLightEnable = SXGUICrCheckBoxEx("", 612, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, level_editor::pToolBar1->getHWND(), 0, 0);
	level_editor::pCheckBoxTBGLightEnable->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxTBGLightEnable->setFollowParentSides(true, false, false, true);
	level_editor::pCheckBoxTBGLightEnable->setBmpFromResourse(IDB_BITMAP27);
	level_editor::pCheckBoxTBGLightEnable->setHintText("Enable/disable global light (sun)");


	
	level_editor::pGroupBoxList = SXGUICrGroupBox("List of objects", 601, 28, 200, 400, level_editor::pJobWindow->getHWND(), WndProcAllDefault, 0);
	gui_func::base_handlers::InitHandlerMsg(level_editor::pGroupBoxList);
	level_editor::pGroupBoxList->addHandler(SXLevelEditor_GroupBoxList_CallWmCommand, WM_COMMAND);
	level_editor::pGroupBoxList->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	level_editor::pGroupBoxList->setColorText(RGB(0,0,0));
	level_editor::pGroupBoxList->setColorTextBk(RGB(220,220,220));
	level_editor::pGroupBoxList->setTransparentTextBk(true);
	level_editor::pGroupBoxList->setColorBrush(RGB(220, 220, 220));

	level_editor::pGroupBoxList->setFollowParentSide(SXGUI_SIDE_LEFT, false);
	level_editor::pGroupBoxList->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	level_editor::pGroupBoxList->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pGroupBoxList->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);

	level_editor::pGroupBoxData = SXGUICrGroupBox("", 0, 425, 800, 190, level_editor::pJobWindow->getHWND(), WndProcAllDefault, 0);
	gui_func::base_handlers::InitHandlerMsg(level_editor::pGroupBoxData);
	level_editor::pGroupBoxData->addHandler(SXLevelEditor_GroupBox_CallWmCommand, WM_COMMAND);
	level_editor::pGroupBoxData->addHandler(SXLevelEditor_GroupBox_Notify, WM_NOTIFY);
	level_editor::pGroupBoxData->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	level_editor::pGroupBoxData->setColorText(RGB(0,0,0));
	level_editor::pGroupBoxData->setColorTextBk(RGB(220, 220, 220));
	level_editor::pGroupBoxData->setTransparentTextBk(true);
	level_editor::pGroupBoxData->setColorBrush(RGB(220, 220, 220));

	level_editor::pGroupBoxData->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pGroupBoxData->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	level_editor::pGroupBoxData->setFollowParentSide(SXGUI_SIDE_TOP, false);
	level_editor::pGroupBoxData->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);

	level_editor::pListBoxList = SXGUICrListBox(4, 30, 193, 290, level_editor::pGroupBoxList->getHWND(), 0, 0, false);
	level_editor::pListBoxList->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pListBoxList->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pListBoxList->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	level_editor::pListBoxList->setColorText(RGB(0,0,0));
	level_editor::pListBoxList->setColorTextBk(RGB(255, 255, 255));
	level_editor::pListBoxList->setTransparentTextBk(true);
	level_editor::pListBoxList->setColorBrush(RGB(255, 255, 255));
	//level_editor::pListBoxList->addHandler(SXLevelEditor_ListBoxList_Click, WM_LBUTTONUP);

	level_editor::pStaticListTextCount = SXGUICrStatic("Count:", 65, 15, 30, 15, level_editor::pGroupBoxList->getHWND(), 0);
	level_editor::pStaticListTextCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticListTextCount->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticListTextCount->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	level_editor::pStaticListTextCount->setColorText(RGB(0,0,0));
	level_editor::pStaticListTextCount->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticListTextCount->setTransparentTextBk(true);
	level_editor::pStaticListTextCount->setColorBrush(RGB(220, 220, 220));

	level_editor::pStaticListValCount = SXGUICrStatic("0", 100, 15, 50, 15, level_editor::pGroupBoxList->getHWND(), 0);
	level_editor::pStaticListValCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticListValCount->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticListValCount->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	level_editor::pStaticListValCount->setColorText(RGB(0,0,0));
	level_editor::pStaticListValCount->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticListValCount->setTransparentTextBk(true);
	level_editor::pStaticListValCount->setColorBrush(RGB(220, 220, 220));

	level_editor::pButtonDelete = SXGUICrButton("Delete", 135, 325, 60, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxList->getHWND(), 0, 0);
	level_editor::pButtonDelete->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonDelete->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonDelete->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonDelete->addHandler(SXLevelEditor_ButtonDelete_Click, WM_LBUTTONUP);

	level_editor::pButtonGeometryOpen = SXGUICrButton("Geometry", 5, 350, 60, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxList->getHWND(), 0, 0);
	level_editor::pButtonGeometryOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGeometryOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGeometryOpen->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	level_editor::pButtonGeometryOpen->addHandler(SXLevelEditor_ButtonGeometryOpen_Click, WM_LBUTTONUP);

	level_editor::pButtonGreenOpen = SXGUICrButton("Green", 70, 350, 60, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxList->getHWND(), 0, 0);
	level_editor::pButtonGreenOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGreenOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGreenOpen->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	level_editor::pButtonGreenOpen->addHandler(SXLevelEditor_ButtonGreenOpen_Click, WM_LBUTTONUP);

	level_editor::pButtonGameObjectOpen = SXGUICrButton("Game", 135, 350, 60, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxList->getHWND(), 0, 0);
	level_editor::pButtonGameObjectOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGameObjectOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGameObjectOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGameObjectOpen->addHandler(SXLevelEditor_ButtonGameObjectOpen_Click, WM_LBUTTONUP);

	level_editor::pButtonAIGridOpen = SXGUICrButton("AI Grid", 5, 375, 60, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxList->getHWND(), 0, 0);
	level_editor::pButtonAIGridOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonAIGridOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonAIGridOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonAIGridOpen->addHandler(SXLevelEditor_ButtonAIGridOpen_Click, WM_LBUTTONUP);

	level_editor::pStatusBar1 = SXGUICrStatusBar("StatusBar1", level_editor::pJobWindow->getHWND(), 0, 0);
	level_editor::pStatusBar1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	int arrpart[5];
	arrpart[0] = 150;
	arrpart[1] = 300;
	arrpart[2] = 450;
	arrpart[3] = 600;
	arrpart[4] = -1;
	level_editor::pStatusBar1->setPartsCount(5, arrpart);
	level_editor::pStatusBar1->setPartText(0, "Level poly: ");
	level_editor::pStatusBar1->setPartText(1, "Geom poly: ");
	level_editor::pStatusBar1->setPartText(2, "Green poly: ");
	level_editor::pStatusBar1->setPartText(3, "");
	level_editor::pStatusBar1->setPartText(4, "");

	//Geom
	//{
	level_editor::pStaticGeomName = SXGUICrStatic("Name", 5, 20, 70, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGeomName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGeomName->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGeomName->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGeomName->setTransparentTextBk(true);
	level_editor::pStaticGeomName->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGeomName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGeomName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGeomName->setVisible(false);

	level_editor::pEditGeomName = SXGUICrEdit("0", 80, 20, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomName->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomName->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomName->setTransparentTextBk(true);
	level_editor::pEditGeomName->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomName->setVisible(false);
	level_editor::pEditGeomName->addHandler(SXLevelEditor_EditGeomName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomName->addHandler(SXLevelEditor_EditGeomName_Enter, WM_KILLFOCUS);

	level_editor::pStaticGeomModel = SXGUICrStatic("Model", 5, 40, 70, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGeomModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGeomModel->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGeomModel->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGeomModel->setTransparentTextBk(true);
	level_editor::pStaticGeomModel->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGeomModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGeomModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGeomModel->setVisible(false);

	level_editor::pEditGeomModel = SXGUICrEdit("0", 80, 40, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomModel->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomModel->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomModel->setTransparentTextBk(true);
	level_editor::pEditGeomModel->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomModel->setVisible(false);

	level_editor::pButtonGeomModel = SXGUICrButton("...", 285, 40, 25, 15, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGeomModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGeomModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGeomModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGeomModel->setVisible(false);
	level_editor::pButtonGeomModel->addHandler(SXLevelEditor_ButtonGeomModel_Click, WM_LBUTTONUP);

	level_editor::pStaticGeomLod1 = SXGUICrStatic("Lod1", 5, 60, 70, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGeomLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGeomLod1->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGeomLod1->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGeomLod1->setTransparentTextBk(true);
	level_editor::pStaticGeomLod1->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGeomLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGeomLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGeomLod1->setVisible(false);

	level_editor::pEditGeomLod1 = SXGUICrEdit("0", 80, 60, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomLod1->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomLod1->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomLod1->setTransparentTextBk(true);
	level_editor::pEditGeomLod1->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomLod1->setVisible(false);

	level_editor::pButtonGeomLod1 = SXGUICrButton("...", 285, 60, 25, 15, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGeomLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGeomLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGeomLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGeomLod1->setVisible(false);
	level_editor::pButtonGeomLod1->addHandler(SXLevelEditor_ButtonGeomLod1OrPhysics_Click, WM_LBUTTONUP);

	level_editor::pStaticGeomPhysics = SXGUICrStatic("Physics", 5, 80, 70, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGeomPhysics->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGeomPhysics->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGeomPhysics->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGeomPhysics->setTransparentTextBk(true);
	level_editor::pStaticGeomPhysics->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGeomPhysics->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGeomPhysics->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGeomPhysics->setVisible(false);

	level_editor::pEditGeomPhysics = SXGUICrEdit("0", 80, 80, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomPhysics->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomPhysics->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomPhysics->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomPhysics->setTransparentTextBk(true);
	level_editor::pEditGeomPhysics->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomPhysics->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomPhysics->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomPhysics->setVisible(false);

	level_editor::pButtonGeomPhysics = SXGUICrButton("...", 285, 80, 25, 15, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGeomPhysics->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGeomPhysics->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGeomPhysics->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGeomPhysics->setVisible(false);
	level_editor::pButtonGeomPhysics->addHandler(SXLevelEditor_ButtonGeomLod1OrPhysics_Click, WM_LBUTTONUP);


	level_editor::pCheckBoxSegmentation = SXGUICrCheckBox("Segmentation", 80, 100, 100, 20, level_editor::pGroupBoxData->getHWND(), 0, 0, false);
	level_editor::pCheckBoxSegmentation->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxSegmentation->setColorText(RGB(0, 0, 0));
	level_editor::pCheckBoxSegmentation->setColorTextBk(RGB(255, 255, 255));
	level_editor::pCheckBoxSegmentation->setTransparentTextBk(true);
	level_editor::pCheckBoxSegmentation->setColorBrush(RGB(220, 220, 220));
	level_editor::pCheckBoxSegmentation->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pCheckBoxSegmentation->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pCheckBoxSegmentation->setVisible(false);
	
	level_editor::pStaticGeomPos = SXGUICrStatic("Position:", 320, 35, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGeomPos->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGeomPos->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGeomPos->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGeomPos->setTransparentTextBk(true);
	level_editor::pStaticGeomPos->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGeomPos->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGeomPos->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGeomPos->setVisible(false);

	level_editor::pEditGeomPosX = SXGUICrEdit("0", 370, 15, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomPosX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomPosX->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomPosX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomPosX->setTransparentTextBk(true);
	level_editor::pEditGeomPosX->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomPosX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomPosX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomPosX->setVisible(false);
	level_editor::pEditGeomPosX->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomPosX->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KILLFOCUS);

	level_editor::pEditGeomPosY = SXGUICrEdit("0", 370, 35, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomPosY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomPosY->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomPosY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomPosY->setTransparentTextBk(true);
	level_editor::pEditGeomPosY->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomPosY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomPosY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomPosY->setVisible(false);
	level_editor::pEditGeomPosY->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomPosY->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KILLFOCUS);

	level_editor::pEditGeomPosZ = SXGUICrEdit("0", 370, 55, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomPosZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomPosZ->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomPosZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomPosZ->setTransparentTextBk(true);
	level_editor::pEditGeomPosZ->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomPosZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomPosZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomPosZ->setVisible(false);
	level_editor::pEditGeomPosZ->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomPosZ->addHandler(SXLevelEditor_EditTransformPos_Enter, WM_KILLFOCUS);

	level_editor::pRadioButtonGeomPosX = SXGUICrRadioButton("x", 440, 15, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGeomPosX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGeomPosX->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGeomPosX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGeomPosX->setTransparentTextBk(true);
	level_editor::pRadioButtonGeomPosX->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGeomPosX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGeomPosX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGeomPosX->setVisible(false);

	level_editor::pRadioButtonGeomPosY = SXGUICrRadioButton("y", 440, 35, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGeomPosY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGeomPosY->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGeomPosY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGeomPosY->setTransparentTextBk(true);
	level_editor::pRadioButtonGeomPosY->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGeomPosY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGeomPosY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGeomPosY->setVisible(false);

	level_editor::pRadioButtonGeomPosZ = SXGUICrRadioButton("z", 440, 55, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGeomPosZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGeomPosZ->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGeomPosZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGeomPosZ->setTransparentTextBk(true);
	level_editor::pRadioButtonGeomPosZ->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGeomPosZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGeomPosZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGeomPosZ->setVisible(false);

	level_editor::pStaticGeomRot = SXGUICrStatic("Rotation:", 470, 35, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGeomRot->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGeomRot->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGeomRot->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGeomRot->setTransparentTextBk(true);
	level_editor::pStaticGeomRot->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGeomRot->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGeomRot->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGeomRot->setVisible(false);

	level_editor::pEditGeomRotX = SXGUICrEdit("0", 525, 15, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomRotX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomRotX->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomRotX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomRotX->setTransparentTextBk(true);
	level_editor::pEditGeomRotX->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomRotX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomRotX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomRotX->setVisible(false);
	level_editor::pEditGeomRotX->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomRotX->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KILLFOCUS);

	level_editor::pEditGeomRotY = SXGUICrEdit("0", 525, 35, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomRotY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomRotY->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomRotY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomRotY->setTransparentTextBk(true);
	level_editor::pEditGeomRotY->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomRotY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomRotY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomRotY->setVisible(false);
	level_editor::pEditGeomRotY->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomRotY->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KILLFOCUS);

	level_editor::pEditGeomRotZ = SXGUICrEdit("0", 525, 55, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomRotZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomRotZ->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomRotZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomRotZ->setTransparentTextBk(true);
	level_editor::pEditGeomRotZ->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomRotZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomRotZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomRotZ->setVisible(false);
	level_editor::pEditGeomRotZ->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomRotZ->addHandler(SXLevelEditor_EditTransformRot_Enter, WM_KILLFOCUS);

	level_editor::pRadioButtonGeomRotX = SXGUICrRadioButton("x", 595, 15, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGeomRotX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGeomRotX->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGeomRotX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGeomRotX->setTransparentTextBk(true);
	level_editor::pRadioButtonGeomRotX->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGeomRotX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGeomRotX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGeomRotX->setVisible(false);

	level_editor::pRadioButtonGeomRotY = SXGUICrRadioButton("y", 595, 35, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGeomRotY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGeomRotY->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGeomRotY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGeomRotY->setTransparentTextBk(true);
	level_editor::pRadioButtonGeomRotY->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGeomRotY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGeomRotY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGeomRotY->setVisible(false);

	level_editor::pRadioButtonGeomRotZ = SXGUICrRadioButton("z", 595, 55, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGeomRotZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGeomRotZ->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGeomRotZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGeomRotZ->setTransparentTextBk(true);
	level_editor::pRadioButtonGeomRotZ->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGeomRotZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGeomRotZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGeomRotZ->setVisible(false);

	level_editor::pStaticGeomScale = SXGUICrStatic("Scale:", 625, 35, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGeomScale->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGeomScale->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGeomScale->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGeomScale->setTransparentTextBk(true);
	level_editor::pStaticGeomScale->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGeomScale->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGeomScale->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGeomScale->setVisible(false);

	level_editor::pEditGeomScaleX = SXGUICrEdit("0", 675, 15, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomScaleX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomScaleX->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomScaleX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomScaleX->setTransparentTextBk(true);
	level_editor::pEditGeomScaleX->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomScaleX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomScaleX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomScaleX->setVisible(false);
	level_editor::pEditGeomScaleX->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomScaleX->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KILLFOCUS);

	level_editor::pEditGeomScaleY = SXGUICrEdit("0", 675, 35, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomScaleY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomScaleY->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomScaleY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomScaleY->setTransparentTextBk(true);
	level_editor::pEditGeomScaleY->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomScaleY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomScaleY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomScaleY->setVisible(false);
	level_editor::pEditGeomScaleY->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomScaleY->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KILLFOCUS);

	level_editor::pEditGeomScaleZ = SXGUICrEdit("0", 675, 55, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGeomScaleZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGeomScaleZ->setColorText(RGB(0, 0, 0));
	level_editor::pEditGeomScaleZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGeomScaleZ->setTransparentTextBk(true);
	level_editor::pEditGeomScaleZ->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGeomScaleZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGeomScaleZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGeomScaleZ->setVisible(false);
	level_editor::pEditGeomScaleZ->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGeomScaleZ->addHandler(SXLevelEditor_EditTransformScale_Enter, WM_KILLFOCUS);

	level_editor::pRadioButtonGeomScaleX = SXGUICrRadioButton("x", 745, 15, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGeomScaleX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGeomScaleX->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGeomScaleX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGeomScaleX->setTransparentTextBk(true);
	level_editor::pRadioButtonGeomScaleX->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGeomScaleX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGeomScaleX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGeomScaleX->setVisible(false);

	level_editor::pRadioButtonGeomScaleY = SXGUICrRadioButton("y", 745, 35, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGeomScaleY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGeomScaleY->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGeomScaleY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGeomScaleY->setTransparentTextBk(true);
	level_editor::pRadioButtonGeomScaleY->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGeomScaleY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGeomScaleY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGeomScaleY->setVisible(false);

	level_editor::pRadioButtonGeomScaleZ = SXGUICrRadioButton("z", 745, 55, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGeomScaleZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGeomScaleZ->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGeomScaleZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGeomScaleZ->setTransparentTextBk(true);
	level_editor::pRadioButtonGeomScaleZ->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGeomScaleZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGeomScaleZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGeomScaleZ->setVisible(false);

	level_editor::pButtonGeomFinish = SXGUICrButton("Finish", 100, 135, 100, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGeomFinish->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGeomFinish->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGeomFinish->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGeomFinish->setVisible(false);
	level_editor::pButtonGeomFinish->addHandler(SXLevelEditor_ButtonGeomFinish_Click, WM_LBUTTONUP);
	//}


	//Green
	//{

	level_editor::pStaticGreenName = SXGUICrStatic("Name", 5, 20, 70, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenName->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenName->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenName->setTransparentTextBk(true);
	level_editor::pStaticGreenName->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenName->setVisible(false);

	level_editor::pEditGreenName = SXGUICrEdit("0", 80, 20, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGreenName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGreenName->setColorText(RGB(0, 0, 0));
	level_editor::pEditGreenName->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGreenName->setTransparentTextBk(true);
	level_editor::pEditGreenName->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGreenName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGreenName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGreenName->setVisible(false);
	level_editor::pEditGreenName->addHandler(SXLevelEditor_EditGreenName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGreenName->addHandler(SXLevelEditor_EditGreenName_Enter, WM_KILLFOCUS);

	level_editor::pStaticGreenModel = SXGUICrStatic("Model", 5, 40, 70, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenModel->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenModel->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenModel->setTransparentTextBk(true);
	level_editor::pStaticGreenModel->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenModel->setVisible(false);

	level_editor::pEditGreenModel = SXGUICrEdit("0", 80, 40, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGreenModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGreenModel->setColorText(RGB(0, 0, 0));
	level_editor::pEditGreenModel->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGreenModel->setTransparentTextBk(true);
	level_editor::pEditGreenModel->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGreenModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGreenModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGreenModel->setVisible(false);

	level_editor::pButtonGreenModel = SXGUICrButton("...", 285, 40, 25, 15, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGreenModel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGreenModel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGreenModel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGreenModel->setVisible(false);
	level_editor::pButtonGreenModel->addHandler(SXLevelEditor_ButtonGreenModel_Click, WM_LBUTTONUP);

	level_editor::pStaticGreenLod1 = SXGUICrStatic("Lod1", 5, 60, 70, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenLod1->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenLod1->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenLod1->setTransparentTextBk(true);
	level_editor::pStaticGreenLod1->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenLod1->setVisible(false);

	level_editor::pEditGreenLod1 = SXGUICrEdit("0", 80, 60, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGreenLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGreenLod1->setColorText(RGB(0, 0, 0));
	level_editor::pEditGreenLod1->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGreenLod1->setTransparentTextBk(true);
	level_editor::pEditGreenLod1->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGreenLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGreenLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGreenLod1->setVisible(false);

	level_editor::pButtonGreenLod1 = SXGUICrButton("...", 285, 60, 25, 15, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGreenLod1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGreenLod1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGreenLod1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGreenLod1->setVisible(false);
	level_editor::pButtonGreenLod1->addHandler(SXLevelEditor_ButtonGreenLod1_Click, WM_LBUTTONUP);

	level_editor::pStaticGreenLod2 = SXGUICrStatic("Lod2", 5, 80, 70, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenLod2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenLod2->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenLod2->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenLod2->setTransparentTextBk(true);
	level_editor::pStaticGreenLod2->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenLod2->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenLod2->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenLod2->setVisible(false);

	level_editor::pEditGreenLod2 = SXGUICrEdit("0", 80, 80, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGreenLod2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGreenLod2->setColorText(RGB(0, 0, 0));
	level_editor::pEditGreenLod2->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGreenLod2->setTransparentTextBk(true);
	level_editor::pEditGreenLod2->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGreenLod2->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGreenLod2->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGreenLod2->setVisible(false);

	level_editor::pButtonGreenLod2 = SXGUICrButton("...", 285, 80, 25, 15, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGreenLod2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGreenLod2->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGreenLod2->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGreenLod2->setVisible(false);
	level_editor::pButtonGreenLod2->addHandler(SXLevelEditor_ButtonGreenLod2_Click, WM_LBUTTONUP);

	level_editor::pButtonGreenMask = SXGUICrButton("...", 285, 100, 25, 15, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGreenMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGreenMask->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGreenMask->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGreenMask->setVisible(false);
	level_editor::pButtonGreenMask->addHandler(SXLevelEditor_ButtonGreenMask_Click, WM_LBUTTONUP);

	level_editor::pCheckBoxGreenAveragedRGB = SXGUICrCheckBox("Use averaged RGB (default use alpha-channel)", 5, 120, 250, 20, level_editor::pGroupBoxData->getHWND(), 0, 0, false);
	level_editor::pCheckBoxGreenAveragedRGB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxGreenAveragedRGB->setColorText(RGB(0, 0, 0));
	level_editor::pCheckBoxGreenAveragedRGB->setColorTextBk(RGB(255, 255, 255));
	level_editor::pCheckBoxGreenAveragedRGB->setTransparentTextBk(true);
	level_editor::pCheckBoxGreenAveragedRGB->setColorBrush(RGB(220, 220, 220));
	level_editor::pCheckBoxGreenAveragedRGB->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pCheckBoxGreenAveragedRGB->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pCheckBoxGreenAveragedRGB->setVisible(false);

	level_editor::pStaticGreenMask = SXGUICrStatic("Mask texture", 5, 100, 70, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenMask->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenMask->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenMask->setTransparentTextBk(true);
	level_editor::pStaticGreenMask->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenMask->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenMask->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenMask->setVisible(false);

	level_editor::pEditGreenMask = SXGUICrEdit("0", 80, 100, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGreenMask->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGreenMask->setColorText(RGB(0, 0, 0));
	level_editor::pEditGreenMask->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGreenMask->setTransparentTextBk(true);
	level_editor::pEditGreenMask->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGreenMask->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGreenMask->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGreenMask->setVisible(false);

	level_editor::pStaticGreenNav = SXGUICrStatic("NavMesh:", 330, 20, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenNav->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenNav->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenNav->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenNav->setTransparentTextBk(true);
	level_editor::pStaticGreenNav->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenNav->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenNav->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenNav->setVisible(false);

	level_editor::pEditGreenNav = SXGUICrEdit("", 380, 20, 200, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGreenNav->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGreenNav->setColorText(RGB(0, 0, 0));
	level_editor::pEditGreenNav->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGreenNav->setTransparentTextBk(true);
	level_editor::pEditGreenNav->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGreenNav->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGreenNav->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGreenNav->setVisible(false);

	level_editor::pButtonGreenNav = SXGUICrButton("...", 585, 20, 25, 15, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGreenNav->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGreenNav->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGreenNav->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGreenNav->setVisible(false);
	level_editor::pButtonGreenNav->addHandler(SXLevelEditor_ButtonGreenNav_Click, WM_LBUTTONUP);

	level_editor::pTrackBarGreenDensity = SXGUICrTrackBar("", 330, 65, 280, 20, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pTrackBarGreenDensity->setMinMax(1, 100);
	level_editor::pTrackBarGreenDensity->setTickFrequency(10);
	level_editor::pTrackBarGreenDensity->addHandler(SXLevelEditor_TrackBarGreenDensity_MouseMove, WM_MOUSEMOVE);
	level_editor::pTrackBarGreenDensity->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pTrackBarGreenDensity->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pTrackBarGreenDensity->setVisible(false);

	level_editor::pButtonGreenGenerate = SXGUICrButton("Generate", 420, 87, 100, 30, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGreenGenerate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGreenGenerate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGreenGenerate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGreenGenerate->setVisible(false);
	level_editor::pButtonGreenGenerate->addHandler(SXLevelEditor_ButtonGreenGenerate_Click, WM_LBUTTONUP);

	level_editor::pStaticGreenDensityText = SXGUICrStatic("Density:", 380, 45, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenDensityText->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenDensityText->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenDensityText->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenDensityText->setTransparentTextBk(true);
	level_editor::pStaticGreenDensityText->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenDensityText->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenDensityText->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenDensityText->setVisible(false);

	level_editor::pStaticGreenDensityVal = SXGUICrStatic("0", 430, 45, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenDensityVal->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenDensityVal->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenDensityVal->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenDensityVal->setTransparentTextBk(true);
	level_editor::pStaticGreenDensityVal->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenDensityVal->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenDensityVal->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenDensityVal->setVisible(false);

	level_editor::pRadioButtonGreenSelX = SXGUICrRadioButton("x", 765, 40, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGreenSelX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGreenSelX->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGreenSelX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGreenSelX->setTransparentTextBk(true);
	level_editor::pRadioButtonGreenSelX->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGreenSelX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGreenSelX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGreenSelX->setVisible(false);

	level_editor::pRadioButtonGreenSelY = SXGUICrRadioButton("y", 765, 60, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGreenSelY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGreenSelY->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGreenSelY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGreenSelY->setTransparentTextBk(true);
	level_editor::pRadioButtonGreenSelY->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGreenSelY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGreenSelY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGreenSelY->setVisible(false);

	level_editor::pRadioButtonGreenSelZ = SXGUICrRadioButton("z", 765, 80, 25, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonGreenSelZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonGreenSelZ->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonGreenSelZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonGreenSelZ->setTransparentTextBk(true);
	level_editor::pRadioButtonGreenSelZ->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonGreenSelZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonGreenSelZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonGreenSelZ->setVisible(false);
	
	level_editor::pComboBoxGreenSel = SXGUICrComboBox(620, 20, 150, 100, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pComboBoxGreenSel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pComboBoxGreenSel->setColorText(RGB(0, 0, 0));
	level_editor::pComboBoxGreenSel->setColorTextBk(RGB(255, 255, 255));
	level_editor::pComboBoxGreenSel->setTransparentTextBk(true);
	level_editor::pComboBoxGreenSel->setColorBrush(RGB(220, 220, 220));
	level_editor::pComboBoxGreenSel->addItem("single select");
	level_editor::pComboBoxGreenSel->addItem("single create");
	level_editor::pComboBoxGreenSel->addItem("multiple create");
	level_editor::pComboBoxGreenSel->addItem("single delete");
	//level_editor::pComboBoxGreenSel->AddItem("multiple delete");
	level_editor::pComboBoxGreenSel->setSel(0);
	level_editor::pComboBoxGreenSel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pComboBoxGreenSel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pComboBoxGreenSel->setVisible(false);
	
	level_editor::pStaticGreenSelX = SXGUICrStatic("Width volume:", 620, 40, 75, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenSelX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenSelX->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenSelX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenSelX->setTransparentTextBk(true);
	level_editor::pStaticGreenSelX->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenSelX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenSelX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenSelX->setVisible(false);

	level_editor::pStaticGreenSelY = SXGUICrStatic("Height volume:", 620, 60, 75, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenSelY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenSelY->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenSelY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenSelY->setTransparentTextBk(true);
	level_editor::pStaticGreenSelY->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenSelY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenSelY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenSelY->setVisible(false);

	level_editor::pStaticGreenSelZ = SXGUICrStatic("Depth volume:", 620, 80, 75, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGreenSelZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGreenSelZ->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGreenSelZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGreenSelZ->setTransparentTextBk(true);
	level_editor::pStaticGreenSelZ->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGreenSelZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGreenSelZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGreenSelZ->setVisible(false);

	level_editor::pEditGreenSelX = SXGUICrEdit("0", 695, 40, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGreenSelX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGreenSelX->setColorText(RGB(0, 0, 0));
	level_editor::pEditGreenSelX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGreenSelX->setTransparentTextBk(true);
	level_editor::pEditGreenSelX->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGreenSelX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGreenSelX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGreenSelX->setVisible(false);

	level_editor::pEditGreenSelY = SXGUICrEdit("0", 695, 60, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGreenSelY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGreenSelY->setColorText(RGB(0, 0, 0));
	level_editor::pEditGreenSelY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGreenSelY->setTransparentTextBk(true);
	level_editor::pEditGreenSelY->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGreenSelY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGreenSelY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGreenSelY->setVisible(false);

	level_editor::pEditGreenSelZ = SXGUICrEdit("0", 695, 80, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGreenSelZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGreenSelZ->setColorText(RGB(0, 0, 0));
	level_editor::pEditGreenSelZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGreenSelZ->setTransparentTextBk(true);
	level_editor::pEditGreenSelZ->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGreenSelZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGreenSelZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGreenSelZ->setVisible(false);
	//}

	//Game
	//{
	level_editor::pStaticGameClass = SXGUICrStatic("Class:", 415, 15, 35, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGameClass->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGameClass->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGameClass->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGameClass->setTransparentTextBk(true);
	level_editor::pStaticGameClass->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGameClass->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGameClass->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGameClass->setVisible(false);

	level_editor::pComboBoxGameClass = SXGUICrComboBoxEx(455, 15, 230, 180, 0, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pComboBoxGameClass->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pComboBoxGameClass->setColorText(RGB(0, 0, 0));
	level_editor::pComboBoxGameClass->setColorTextBk(RGB(255, 255, 255));
	level_editor::pComboBoxGameClass->setTransparentTextBk(true);
	level_editor::pComboBoxGameClass->setColorBrush(RGB(255, 255, 255));
	level_editor::pComboBoxGameClass->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pComboBoxGameClass->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pComboBoxGameClass->setVisible(false);
	level_editor::pComboBoxGameClass->addItem("");

	level_editor::pButtonGameTab = SXGUICrButton("Connections", 695, 15, 100, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGameTab->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGameTab->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGameTab->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGameTab->setVisible(false);
	level_editor::pButtonGameTab->addHandler(SXLevelEditor_ButtonGameTab_Click, WM_LBUTTONUP);
	level_editor::GameTabVal = 0;

	level_editor::pListViewGameClass = SXGUICrListView(5, 5, 400, 180, level_editor::pGroupBoxData->getHWND(), WndProcAllDefault, 0);
	level_editor::pListViewGameClass->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pListViewGameClass->setColorText(RGB(0, 0, 0));
	level_editor::pListViewGameClass->setColorTextBk(RGB(255, 255, 255));
	level_editor::pListViewGameClass->setTransparentTextBk(true);
	level_editor::pListViewGameClass->setColorBrush(RGB(255, 255, 255));
	level_editor::pListViewGameClass->addColumn("Property Name",199);
	level_editor::pListViewGameClass->addColumn("Value", 199);
	level_editor::pListViewGameClass->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pListViewGameClass->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pListViewGameClass->setVisible(false);

	level_editor::pComboBoxGameValue = SXGUICrComboBox(415, 45, 270, 180, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pComboBoxGameValue->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pComboBoxGameValue->setColorText(RGB(0, 0, 0));
	level_editor::pComboBoxGameValue->setColorTextBk(RGB(255, 255, 255));
	level_editor::pComboBoxGameValue->setTransparentTextBk(true);
	level_editor::pComboBoxGameValue->setColorBrush(RGB(255, 255, 255));
	level_editor::pComboBoxGameValue->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pComboBoxGameValue->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pComboBoxGameValue->setVisible(false);

	level_editor::pEditGameValue = SXGUICrEdit("", 415, 45, 240, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGameValue->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGameValue->setColorText(RGB(0, 0, 0));
	level_editor::pEditGameValue->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGameValue->setTransparentTextBk(true);
	level_editor::pEditGameValue->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGameValue->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGameValue->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGameValue->setVisible(false);
	level_editor::pEditGameValue->addHandler(SXLevelEditor_EditGameValue_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGameValue->addHandler(SXLevelEditor_EditGameValue_Enter, WM_KILLFOCUS);

	level_editor::pButtonGameValue = SXGUICrButton("...", 660, 45, 25, 15, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGameValue->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGameValue->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGameValue->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGameValue->setVisible(false);
	level_editor::pButtonGameValue->addHandler(SXLevelEditor_ButtonGameValue_Click, WM_LBUTTONUP);

	level_editor::pStaticGameHelp = SXGUICrStatic("Help:", 415, 65, 35, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGameHelp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGameHelp->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGameHelp->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGameHelp->setTransparentTextBk(true);
	level_editor::pStaticGameHelp->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGameHelp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGameHelp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGameHelp->setVisible(false);

	level_editor::pMemoGameHelp = SXGUICrMemo("Help", 415, 85, 270, 95, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pMemoGameHelp->setReadOnly(true);
	level_editor::pMemoGameHelp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pMemoGameHelp->setColorText(RGB(0, 0, 0));
	level_editor::pMemoGameHelp->setColorTextBk(RGB(255, 255, 255));
	level_editor::pMemoGameHelp->setTransparentTextBk(true);
	level_editor::pMemoGameHelp->setColorBrush(RGB(220, 220, 220));
	level_editor::pMemoGameHelp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pMemoGameHelp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pMemoGameHelp->setVisible(false);

	level_editor::pButtonGameCreate = SXGUICrButton("Create", 695, 165, 100, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGameCreate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGameCreate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGameCreate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGameCreate->setVisible(false);
	level_editor::pButtonGameCreate->addHandler(SXLevelEditor_ButtonGameCreate_Click, WM_LBUTTONUP);

	for (int i = 0; i < 8; ++i)
	{
		level_editor::pCheckBoxGameFlags[i] = SXGUICrCheckBox("", 415, 45 + (15*i), 180, 15, level_editor::pGroupBoxData->getHWND(), 0, 0, false);
		level_editor::pCheckBoxGameFlags[i]->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
		level_editor::pCheckBoxGameFlags[i]->setColorText(RGB(0, 0, 0));
		level_editor::pCheckBoxGameFlags[i]->setColorTextBk(RGB(255, 255, 255));
		level_editor::pCheckBoxGameFlags[i]->setTransparentTextBk(true);
		level_editor::pCheckBoxGameFlags[i]->setColorBrush(RGB(220, 220, 220));
		level_editor::pCheckBoxGameFlags[i]->setFollowParentSide(SXGUI_SIDE_LEFT, true);
		level_editor::pCheckBoxGameFlags[i]->setFollowParentSide(SXGUI_SIDE_TOP, true);
		level_editor::pCheckBoxGameFlags[i]->setVisible(false);
	}

	for (int i = 0; i < 8; ++i)
	{
		level_editor::pCheckBoxGameFlags[i + 8] = SXGUICrCheckBox("", 600, 45 + (15 * i), 180, 15, level_editor::pGroupBoxData->getHWND(), 0, 0, false);
		level_editor::pCheckBoxGameFlags[i + 8]->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
		level_editor::pCheckBoxGameFlags[i + 8]->setColorText(RGB(0, 0, 0));
		level_editor::pCheckBoxGameFlags[i + 8]->setColorTextBk(RGB(255, 255, 255));
		level_editor::pCheckBoxGameFlags[i + 8]->setTransparentTextBk(true);
		level_editor::pCheckBoxGameFlags[i + 8]->setColorBrush(RGB(220, 220, 220));
		level_editor::pCheckBoxGameFlags[i + 8]->setFollowParentSide(SXGUI_SIDE_LEFT, true);
		level_editor::pCheckBoxGameFlags[i + 8]->setFollowParentSide(SXGUI_SIDE_TOP, true);
		level_editor::pCheckBoxGameFlags[i + 8]->setVisible(false);
	}


	level_editor::pListViewGameConnections = SXGUICrListView(5, 5, 550, 180, level_editor::pGroupBoxData->getHWND(), WndProcAllDefault, 0);
	level_editor::pListViewGameConnections->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pListViewGameConnections->setColorText(RGB(0, 0, 0));
	level_editor::pListViewGameConnections->setColorTextBk(RGB(255, 255, 255));
	level_editor::pListViewGameConnections->setTransparentTextBk(true);
	level_editor::pListViewGameConnections->setColorBrush(RGB(255, 255, 255));
	level_editor::pListViewGameConnections->addColumn("Event", 110);
	level_editor::pListViewGameConnections->addColumn("Name", 110);
	level_editor::pListViewGameConnections->addColumn("Action", 110);
	level_editor::pListViewGameConnections->addColumn("Delay", 110);
	level_editor::pListViewGameConnections->addColumn("Parameter", 110);
	level_editor::pListViewGameConnections->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pListViewGameConnections->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pListViewGameConnections->setVisible(false);

	level_editor::pStaticGameConnectionsEvent = SXGUICrStatic("Event:", 560, 50, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGameConnectionsEvent->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGameConnectionsEvent->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGameConnectionsEvent->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGameConnectionsEvent->setTransparentTextBk(true);
	level_editor::pStaticGameConnectionsEvent->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGameConnectionsEvent->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGameConnectionsEvent->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGameConnectionsEvent->setVisible(false);

	level_editor::pComboBoxGameConnectionsEvent = SXGUICrComboBox(610, 50, 185, 180, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pComboBoxGameConnectionsEvent->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pComboBoxGameConnectionsEvent->setColorText(RGB(0, 0, 0));
	level_editor::pComboBoxGameConnectionsEvent->setColorTextBk(RGB(255, 255, 255));
	level_editor::pComboBoxGameConnectionsEvent->setTransparentTextBk(true);
	level_editor::pComboBoxGameConnectionsEvent->setColorBrush(RGB(255, 255, 255));
	level_editor::pComboBoxGameConnectionsEvent->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pComboBoxGameConnectionsEvent->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pComboBoxGameConnectionsEvent->setVisible(false);

	level_editor::pStaticGameConnectionsName = SXGUICrStatic("Name:", 560, 75, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGameConnectionsName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGameConnectionsName->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGameConnectionsName->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGameConnectionsName->setTransparentTextBk(true);
	level_editor::pStaticGameConnectionsName->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGameConnectionsName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGameConnectionsName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGameConnectionsName->setVisible(false);

	level_editor::pEditGameConnectionsName = SXGUICrEdit("", 610, 75, 185, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGameConnectionsName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGameConnectionsName->setColorText(RGB(0, 0, 0));
	level_editor::pEditGameConnectionsName->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGameConnectionsName->setTransparentTextBk(true);
	level_editor::pEditGameConnectionsName->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGameConnectionsName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGameConnectionsName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGameConnectionsName->setVisible(false);
	level_editor::pEditGameConnectionsName->addHandler(SXLevelEditor_EditGameConnectionsName_IN, WM_KEYUP);
	level_editor::pEditGameConnectionsName->addHandler(SXLevelEditor_EditGameConnectionsName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGameConnectionsName->addHandler(SXLevelEditor_EditGameConnectionsName_Enter, WM_KILLFOCUS);

	level_editor::pStaticGameConnectionsAction = SXGUICrStatic("Action:", 560, 95, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGameConnectionsAction->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGameConnectionsAction->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGameConnectionsAction->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGameConnectionsAction->setTransparentTextBk(true);
	level_editor::pStaticGameConnectionsAction->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGameConnectionsAction->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGameConnectionsAction->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGameConnectionsAction->setVisible(false);

	level_editor::pComboBoxGameConnectionsAction = SXGUICrComboBox(610, 95, 185, 135, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pComboBoxGameConnectionsAction->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pComboBoxGameConnectionsAction->setColorText(RGB(0, 0, 0));
	level_editor::pComboBoxGameConnectionsAction->setColorTextBk(RGB(255, 255, 255));
	level_editor::pComboBoxGameConnectionsAction->setTransparentTextBk(true);
	level_editor::pComboBoxGameConnectionsAction->setColorBrush(RGB(255, 255, 255));
	level_editor::pComboBoxGameConnectionsAction->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pComboBoxGameConnectionsAction->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pComboBoxGameConnectionsAction->setVisible(false);

	level_editor::pStaticGameConnectionsDelay = SXGUICrStatic("Delay:", 560, 120, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGameConnectionsDelay->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGameConnectionsDelay->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGameConnectionsDelay->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGameConnectionsDelay->setTransparentTextBk(true);
	level_editor::pStaticGameConnectionsDelay->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGameConnectionsDelay->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGameConnectionsDelay->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGameConnectionsDelay->setVisible(false);

	level_editor::pEditGameConnectionsDelay = SXGUICrEdit("", 610, 120, 185, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGameConnectionsDelay->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGameConnectionsDelay->setColorText(RGB(0, 0, 0));
	level_editor::pEditGameConnectionsDelay->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGameConnectionsDelay->setTransparentTextBk(true);
	level_editor::pEditGameConnectionsDelay->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGameConnectionsDelay->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGameConnectionsDelay->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGameConnectionsDelay->setVisible(false);
	level_editor::pEditGameConnectionsDelay->addHandler(SXLevelEditor_EditGameConnections_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGameConnectionsDelay->addHandler(SXLevelEditor_EditGameConnections_Enter, WM_KILLFOCUS);

	level_editor::pStaticGameConnectionsParameter = SXGUICrStatic("Parameter:", 560, 140, 50, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticGameConnectionsParameter->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticGameConnectionsParameter->setColorText(RGB(0, 0, 0));
	level_editor::pStaticGameConnectionsParameter->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticGameConnectionsParameter->setTransparentTextBk(true);
	level_editor::pStaticGameConnectionsParameter->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticGameConnectionsParameter->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticGameConnectionsParameter->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticGameConnectionsParameter->setVisible(false);

	level_editor::pEditGameConnectionsParameter = SXGUICrEdit("", 610, 140, 185, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditGameConnectionsParameter->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditGameConnectionsParameter->setColorText(RGB(0, 0, 0));
	level_editor::pEditGameConnectionsParameter->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditGameConnectionsParameter->setTransparentTextBk(true);
	level_editor::pEditGameConnectionsParameter->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditGameConnectionsParameter->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditGameConnectionsParameter->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditGameConnectionsParameter->setVisible(false);
	level_editor::pEditGameConnectionsParameter->addHandler(SXLevelEditor_EditGameConnections_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditGameConnectionsParameter->addHandler(SXLevelEditor_EditGameConnections_Enter, WM_KILLFOCUS);

	
	level_editor::pButtonGameConnectionsCreate = SXGUICrButton("Create", 565, 160, 100, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGameConnectionsCreate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGameConnectionsCreate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGameConnectionsCreate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGameConnectionsCreate->setVisible(false);
	level_editor::pButtonGameConnectionsCreate->addHandler(SXLevelEditor_ButtonGameConnectionsCreate_Click, WM_LBUTTONUP);
	level_editor::isAddGameConections = false;

	level_editor::pButtonGameConnectionsDelete = SXGUICrButton("Delete", 690, 160, 100, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonGameConnectionsDelete->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonGameConnectionsDelete->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonGameConnectionsDelete->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonGameConnectionsDelete->setVisible(false);
	level_editor::pButtonGameConnectionsDelete->addHandler(SXLevelEditor_ButtonGameConnectionsDelete_Click, WM_LBUTTONUP);

	//}

	//aigrid
	//{
	level_editor::pStaticAIBBDimensions = SXGUICrStatic("Bound box dimensions:", 5, 10, 120, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIBBDimensions->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIBBDimensions->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIBBDimensions->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIBBDimensions->setTransparentTextBk(true);
	level_editor::pStaticAIBBDimensions->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIBBDimensions->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIBBDimensions->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIBBDimensions->setVisible(false);

	level_editor::pStaticAIBBDimensionsWidth = SXGUICrStatic("Width:", 5, 30, 40, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIBBDimensionsWidth->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIBBDimensionsWidth->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIBBDimensionsWidth->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIBBDimensionsWidth->setTransparentTextBk(true);
	level_editor::pStaticAIBBDimensionsWidth->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIBBDimensionsWidth->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIBBDimensionsWidth->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIBBDimensionsWidth->setVisible(false);

	level_editor::pEditAIBBDimensionsWidth = SXGUICrEdit("0", 50, 30, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditAIBBDimensionsWidth->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditAIBBDimensionsWidth->setColorText(RGB(0, 0, 0));
	level_editor::pEditAIBBDimensionsWidth->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditAIBBDimensionsWidth->setTransparentTextBk(true);
	level_editor::pEditAIBBDimensionsWidth->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditAIBBDimensionsWidth->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditAIBBDimensionsWidth->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditAIBBDimensionsWidth->setVisible(false);
	level_editor::pEditAIBBDimensionsWidth->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditAIBBDimensionsWidth->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KILLFOCUS);

	level_editor::pStaticAIBBDimensionsHeight = SXGUICrStatic("Height:", 5, 50, 40, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIBBDimensionsHeight->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIBBDimensionsHeight->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIBBDimensionsHeight->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIBBDimensionsHeight->setTransparentTextBk(true);
	level_editor::pStaticAIBBDimensionsHeight->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIBBDimensionsHeight->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIBBDimensionsHeight->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIBBDimensionsHeight->setVisible(false);

	level_editor::pEditAIBBDimensionsHeight = SXGUICrEdit("0", 50, 50, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditAIBBDimensionsHeight->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditAIBBDimensionsHeight->setColorText(RGB(0, 0, 0));
	level_editor::pEditAIBBDimensionsHeight->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditAIBBDimensionsHeight->setTransparentTextBk(true);
	level_editor::pEditAIBBDimensionsHeight->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditAIBBDimensionsHeight->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditAIBBDimensionsHeight->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditAIBBDimensionsHeight->setVisible(false);
	level_editor::pEditAIBBDimensionsHeight->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditAIBBDimensionsHeight->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KILLFOCUS);

	level_editor::pStaticAIBBDimensionsDepth = SXGUICrStatic("Depth:", 5, 70, 40, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIBBDimensionsDepth->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIBBDimensionsDepth->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIBBDimensionsDepth->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIBBDimensionsDepth->setTransparentTextBk(true);
	level_editor::pStaticAIBBDimensionsDepth->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIBBDimensionsDepth->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIBBDimensionsDepth->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIBBDimensionsDepth->setVisible(false);

	level_editor::pEditAIBBDimensionsDepth = SXGUICrEdit("0", 50, 70, 70, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditAIBBDimensionsDepth->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditAIBBDimensionsDepth->setColorText(RGB(0, 0, 0));
	level_editor::pEditAIBBDimensionsDepth->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditAIBBDimensionsDepth->setTransparentTextBk(true);
	level_editor::pEditAIBBDimensionsDepth->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditAIBBDimensionsDepth->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditAIBBDimensionsDepth->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditAIBBDimensionsDepth->setVisible(false);
	level_editor::pEditAIBBDimensionsDepth->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditAIBBDimensionsDepth->addHandler(SXLevelEditor_EditAIBBDimensions_Enter, WM_KILLFOCUS);


	level_editor::pStaticAIBBPos = SXGUICrStatic("Bound box position:", 5, 95, 100, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIBBPos->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIBBPos->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIBBPos->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIBBPos->setTransparentTextBk(true);
	level_editor::pStaticAIBBPos->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIBBPos->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIBBPos->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIBBPos->setVisible(false);

	level_editor::pStaticAIBBPosX = SXGUICrStatic("x:", 5, 115, 10, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIBBPosX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIBBPosX->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIBBPosX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIBBPosX->setTransparentTextBk(true);
	level_editor::pStaticAIBBPosX->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIBBPosX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIBBPosX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIBBPosX->setVisible(false);

	level_editor::pEditAIBBPosX = SXGUICrEdit("0", 15, 115, 65, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditAIBBPosX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditAIBBPosX->setColorText(RGB(0, 0, 0));
	level_editor::pEditAIBBPosX->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditAIBBPosX->setTransparentTextBk(true);
	level_editor::pEditAIBBPosX->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditAIBBPosX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditAIBBPosX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditAIBBPosX->setVisible(false);
	level_editor::pEditAIBBPosX->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditAIBBPosX->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KILLFOCUS);

	level_editor::pStaticAIBBPosY = SXGUICrStatic("y:", 85, 115, 10, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIBBPosY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIBBPosY->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIBBPosY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIBBPosY->setTransparentTextBk(true);
	level_editor::pStaticAIBBPosY->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIBBPosY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIBBPosY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIBBPosY->setVisible(false);

	level_editor::pEditAIBBPosY = SXGUICrEdit("0", 95, 115, 65, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditAIBBPosY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditAIBBPosY->setColorText(RGB(0, 0, 0));
	level_editor::pEditAIBBPosY->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditAIBBPosY->setTransparentTextBk(true);
	level_editor::pEditAIBBPosY->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditAIBBPosY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditAIBBPosY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditAIBBPosY->setVisible(false);
	level_editor::pEditAIBBPosY->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditAIBBPosY->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KILLFOCUS);

	level_editor::pStaticAIBBPosZ = SXGUICrStatic("z:", 165, 115, 10, 15, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIBBPosZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIBBPosZ->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIBBPosZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIBBPosZ->setTransparentTextBk(true);
	level_editor::pStaticAIBBPosZ->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIBBPosZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIBBPosZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIBBPosZ->setVisible(false);

	level_editor::pEditAIBBPosZ = SXGUICrEdit("0", 175, 115, 65, 15, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditAIBBPosZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditAIBBPosZ->setColorText(RGB(0, 0, 0));
	level_editor::pEditAIBBPosZ->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditAIBBPosZ->setTransparentTextBk(true);
	level_editor::pEditAIBBPosZ->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditAIBBPosZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditAIBBPosZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditAIBBPosZ->setVisible(false);
	level_editor::pEditAIBBPosZ->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	level_editor::pEditAIBBPosZ->addHandler(SXLevelEditor_EditAIBBPos_Enter, WM_KILLFOCUS);

	level_editor::pButtonAIBBFinish = SXGUICrButton("Create bound box", 15, 140, 100, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonAIBBFinish->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonAIBBFinish->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonAIBBFinish->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonAIBBFinish->setVisible(false);
	level_editor::pButtonAIBBFinish->addHandler(SXLevelEditor_ButtonAIBBFinish_Click, WM_LBUTTONUP);
	

	level_editor::pRadioButtonAIQuadAdd = SXGUICrRadioButton("AI quad add", 250, 10, 130, 20, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonAIQuadAdd->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonAIQuadAdd->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonAIQuadAdd->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonAIQuadAdd->setTransparentTextBk(true);
	level_editor::pRadioButtonAIQuadAdd->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonAIQuadAdd->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonAIQuadAdd->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonAIQuadAdd->setVisible(false);

	level_editor::pRadioButtonAIQuadsMSel = SXGUICrRadioButton("AI quads multi select", 250, 40, 130, 20, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonAIQuadsMSel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonAIQuadsMSel->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonAIQuadsMSel->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonAIQuadsMSel->setTransparentTextBk(true);
	level_editor::pRadioButtonAIQuadsMSel->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonAIQuadsMSel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonAIQuadsMSel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonAIQuadsMSel->setVisible(false);

	level_editor::pRadioButtonAIQuadsSelDel = SXGUICrRadioButton("AI quads select->delete", 250, 70, 130, 20, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonAIQuadsSelDel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonAIQuadsSelDel->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonAIQuadsSelDel->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonAIQuadsSelDel->setTransparentTextBk(true);
	level_editor::pRadioButtonAIQuadsSelDel->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonAIQuadsSelDel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonAIQuadsSelDel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonAIQuadsSelDel->setVisible(false);

	level_editor::pButtonAIQuadsDelSel = SXGUICrButton("AI quads delete selected", 250, 100, 130, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonAIQuadsDelSel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonAIQuadsDelSel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonAIQuadsDelSel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonAIQuadsDelSel->setVisible(false);
	level_editor::pButtonAIQuadsDelSel->addHandler(SXLevelEditor_ButtonAIQuadsDelSel_Click, WM_LBUTTONUP);

	level_editor::pButtonAIGridGen = SXGUICrButton("AI grid generation", 250, 130, 130, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonAIGridGen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonAIGridGen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonAIGridGen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonAIGridGen->setVisible(false);
	level_editor::pButtonAIGridGen->addHandler(SXLevelEditor_ButtonAIGridGen_Click, WM_LBUTTONUP);

	level_editor::pButtonAIGridClear = SXGUICrButton("AI grid clear", 250, 160, 130, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonAIGridClear->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonAIGridClear->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonAIGridClear->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonAIGridClear->setVisible(false);
	level_editor::pButtonAIGridClear->addHandler(SXLevelEditor_ButtonAIGridClear_Click, WM_LBUTTONUP);
	
	
	level_editor::pRadioButtonAIGPAdd = SXGUICrRadioButton("Graph point add", 390, 10, 110, 20, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonAIGPAdd->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonAIGPAdd->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonAIGPAdd->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonAIGPAdd->setTransparentTextBk(true);
	level_editor::pRadioButtonAIGPAdd->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonAIGPAdd->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonAIGPAdd->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonAIGPAdd->setVisible(false);

	level_editor::pRadioButtonAIGPDel = SXGUICrRadioButton("Graph point delete", 390, 40, 110, 20, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pRadioButtonAIGPDel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pRadioButtonAIGPDel->setColorText(RGB(0, 0, 0));
	level_editor::pRadioButtonAIGPDel->setColorTextBk(RGB(255, 255, 255));
	level_editor::pRadioButtonAIGPDel->setTransparentTextBk(true);
	level_editor::pRadioButtonAIGPDel->setColorBrush(RGB(220, 220, 220));
	level_editor::pRadioButtonAIGPDel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pRadioButtonAIGPDel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pRadioButtonAIGPDel->setVisible(false);

	level_editor::pButtonAIGPGen = SXGUICrButton("Graph points generate", 390, 70, 110, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonAIGPGen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonAIGPGen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonAIGPGen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonAIGPGen->setVisible(false);
	level_editor::pButtonAIGPGen->addHandler(SXLevelEditor_ButtonAIGPGen_Click, WM_LBUTTONUP);

	level_editor::pButtonAIGPClear = SXGUICrButton("Graph points clear", 390, 100, 110, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonAIGPClear->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonAIGPClear->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonAIGPClear->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonAIGPClear->setVisible(false);
	level_editor::pButtonAIGPClear->addHandler(SXLevelEditor_ButtonAIGPClear_Click, WM_LBUTTONUP);


	level_editor::pButtonAIGridValidation = SXGUICrButton("AI grid validation", 510, 10, 100, 20, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonAIGridValidation->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonAIGridValidation->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonAIGridValidation->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonAIGridValidation->setVisible(false);
	level_editor::pButtonAIGridValidation->addHandler(SXLevelEditor_ButtonAIGridValidation_Click, WM_LBUTTONUP);

	level_editor::pCheckBoxAIGridMarkedSplits = SXGUICrCheckBox("Marked splits", 510, 40, 100, 20, level_editor::pGroupBoxData->getHWND(), 0, 0, false);
	level_editor::pCheckBoxAIGridMarkedSplits->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pCheckBoxAIGridMarkedSplits->setColorText(RGB(0, 0, 0));
	level_editor::pCheckBoxAIGridMarkedSplits->setColorTextBk(RGB(255, 255, 255));
	level_editor::pCheckBoxAIGridMarkedSplits->setTransparentTextBk(true);
	level_editor::pCheckBoxAIGridMarkedSplits->setColorBrush(RGB(220, 220, 220));
	level_editor::pCheckBoxAIGridMarkedSplits->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pCheckBoxAIGridMarkedSplits->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pCheckBoxAIGridMarkedSplits->setVisible(false);

	level_editor::pStaticAIStatistics = SXGUICrStatic("Statistics:", 620, 10, 100, 20, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIStatistics->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIStatistics->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIStatistics->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIStatistics->setTransparentTextBk(true);
	level_editor::pStaticAIStatistics->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIStatistics->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIStatistics->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIStatistics->setVisible(false);

	level_editor::pStaticAIStatsCountQuads = SXGUICrStatic("Count quads:", 620, 40, 100, 20, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIStatsCountQuads->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIStatsCountQuads->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIStatsCountQuads->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIStatsCountQuads->setTransparentTextBk(true);
	level_editor::pStaticAIStatsCountQuads->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIStatsCountQuads->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIStatsCountQuads->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIStatsCountQuads->setVisible(false);

	level_editor::pStaticAIStatsCountGP = SXGUICrStatic("Count graph points:", 620, 70, 100, 20, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIStatsCountGP->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIStatsCountGP->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIStatsCountGP->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIStatsCountGP->setTransparentTextBk(true);
	level_editor::pStaticAIStatsCountGP->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIStatsCountGP->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIStatsCountGP->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIStatsCountGP->setVisible(false);

	level_editor::pStaticAIStatsCountSplits = SXGUICrStatic("Count splits:", 620, 100, 100, 20, level_editor::pGroupBoxData->getHWND(), 0);
	level_editor::pStaticAIStatsCountSplits->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pStaticAIStatsCountSplits->setColorText(RGB(0, 0, 0));
	level_editor::pStaticAIStatsCountSplits->setColorTextBk(RGB(255, 255, 255));
	level_editor::pStaticAIStatsCountSplits->setTransparentTextBk(true);
	level_editor::pStaticAIStatsCountSplits->setColorBrush(RGB(220, 220, 220));
	level_editor::pStaticAIStatsCountSplits->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pStaticAIStatsCountSplits->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pStaticAIStatsCountSplits->setVisible(false);

	level_editor::pEditAIStatsCountQuads = SXGUICrEdit("0", 720, 40, 70, 20, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditAIStatsCountQuads->setReadOnly(true);
	level_editor::pEditAIStatsCountQuads->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditAIStatsCountQuads->setColorText(RGB(0, 0, 0));
	level_editor::pEditAIStatsCountQuads->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditAIStatsCountQuads->setTransparentTextBk(true);
	level_editor::pEditAIStatsCountQuads->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditAIStatsCountQuads->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditAIStatsCountQuads->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditAIStatsCountQuads->setVisible(false);

	level_editor::pEditAIStatsCountGP = SXGUICrEdit("0", 720, 70, 70, 20, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditAIStatsCountGP->setReadOnly(true);
	level_editor::pEditAIStatsCountGP->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditAIStatsCountGP->setColorText(RGB(0, 0, 0));
	level_editor::pEditAIStatsCountGP->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditAIStatsCountGP->setTransparentTextBk(true);
	level_editor::pEditAIStatsCountGP->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditAIStatsCountGP->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditAIStatsCountGP->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditAIStatsCountGP->setVisible(false);

	level_editor::pEditAIStatsCountSplits = SXGUICrEdit("0", 720, 100, 70, 20, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pEditAIStatsCountSplits->setReadOnly(true);
	level_editor::pEditAIStatsCountSplits->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pEditAIStatsCountSplits->setColorText(RGB(0, 0, 0));
	level_editor::pEditAIStatsCountSplits->setColorTextBk(RGB(255, 255, 255));
	level_editor::pEditAIStatsCountSplits->setTransparentTextBk(true);
	level_editor::pEditAIStatsCountSplits->setColorBrush(RGB(255, 255, 255));
	level_editor::pEditAIStatsCountSplits->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pEditAIStatsCountSplits->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pEditAIStatsCountSplits->setVisible(false);

	level_editor::pButtonAIClearAll = SXGUICrButton("Clear all", 660, 130, 130, 30, SXGUI_BUTTON_IMAGE_NONE, level_editor::pGroupBoxData->getHWND(), 0, 0);
	level_editor::pButtonAIClearAll->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	level_editor::pButtonAIClearAll->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	level_editor::pButtonAIClearAll->setFollowParentSide(SXGUI_SIDE_TOP, true);
	level_editor::pButtonAIClearAll->setVisible(false);
	level_editor::pButtonAIClearAll->addHandler(SXLevelEditor_ButtonAIClearAll_Click, WM_LBUTTONUP);
	//}
}

void level_editor::DeleteAllElements()
{
	mem_release(level_editor::pJobWindow);
	mem_release(level_editor::pMainMenu);
	mem_release(level_editor::pRenderWindow);

	mem_release(level_editor::pToolBar1);
	mem_release(level_editor::pButtonTBNew);
	mem_release(level_editor::pButtonTBOpen);
	mem_release(level_editor::pButtonTBSave);
	mem_release(level_editor::pButtonTBSaveAs);

	mem_release(level_editor::pCheckBoxTBArrow);
	mem_release(level_editor::pCheckBoxTBPos);
	mem_release(level_editor::pCheckBoxTBRot);
	mem_release(level_editor::pCheckBoxTBScale);

	mem_release(level_editor::pCheckBoxTBGrid);
	mem_release(level_editor::pCheckBoxTBAxes);

	mem_release(level_editor::pCheckBoxTBRColor);
	mem_release(level_editor::pCheckBoxTBRNormal);
	mem_release(level_editor::pCheckBoxTBRParam);
	mem_release(level_editor::pCheckBoxTBRAmDiff);
	mem_release(level_editor::pCheckBoxTBRSpecular);
	mem_release(level_editor::pCheckBoxTBRLighting);

	mem_release(level_editor::pCheckBoxTBSelS);
	mem_release(level_editor::pCheckBoxTBSelZTest);
	mem_release(level_editor::pCheckBoxTBSelMesh);
	mem_release(level_editor::pCheckBoxTBSelCullBack);

	mem_release(level_editor::pCheckBoxTBAIGBound);
	mem_release(level_editor::pCheckBoxTBAIGQuad);
	mem_release(level_editor::pCheckBoxTBAIGGraphPoint);

	mem_release(level_editor::pGroupBoxList);
	mem_release(level_editor::pGroupBoxData);
	mem_release(level_editor::pListBoxList);
	mem_release(level_editor::pStaticListTextCount);
	mem_release(level_editor::pStaticListValCount);

	mem_release(level_editor::pButtonDelete);

	mem_release(level_editor::pButtonGeometryOpen);
	mem_release(level_editor::pButtonGreenOpen);
	mem_release(level_editor::pButtonGameObjectOpen);
	mem_release(level_editor::pButtonAIGridOpen);
	mem_release(level_editor::pStatusBar1);

	//model
	//{{
	mem_release(level_editor::pStaticGeomName);
	mem_release(level_editor::pStaticGeomModel);
	mem_release(level_editor::pStaticGeomLod1);
	mem_release(level_editor::pEditGeomName);
	mem_release(level_editor::pEditGeomModel);
	mem_release(level_editor::pEditGeomLod1);
	mem_release(level_editor::pButtonGeomLod1);
	mem_release(level_editor::pStaticGeomPhysics);
	mem_release(level_editor::pEditGeomPhysics);
	mem_release(level_editor::pButtonGeomPhysics);
	mem_release(level_editor::pCheckBoxSegmentation);
	mem_release(level_editor::pButtonGeomModel);
	
	mem_release(level_editor::pStaticGeomPos);
	mem_release(level_editor::pEditGeomPosX);
	mem_release(level_editor::pEditGeomPosY);
	mem_release(level_editor::pEditGeomPosZ);
	mem_release(level_editor::pRadioButtonGeomPosX);
	mem_release(level_editor::pRadioButtonGeomPosY);
	mem_release(level_editor::pRadioButtonGeomPosZ);
	mem_release(level_editor::pStaticGeomRot);
	mem_release(level_editor::pEditGeomRotX);
	mem_release(level_editor::pEditGeomRotY);
	mem_release(level_editor::pEditGeomRotZ);
	mem_release(level_editor::pRadioButtonGeomRotX);
	mem_release(level_editor::pRadioButtonGeomRotY);
	mem_release(level_editor::pRadioButtonGeomRotZ);
	mem_release(level_editor::pStaticGeomScale);
	mem_release(level_editor::pEditGeomScaleX);
	mem_release(level_editor::pEditGeomScaleY);
	mem_release(level_editor::pEditGeomScaleZ);
	mem_release(level_editor::pRadioButtonGeomScaleX);
	mem_release(level_editor::pRadioButtonGeomScaleY);
	mem_release(level_editor::pRadioButtonGeomScaleZ);
	mem_release(level_editor::pButtonGeomFinish);
	//}}

	mem_release(level_editor::pStaticGreenName);
	mem_release(level_editor::pStaticGreenModel);
	mem_release(level_editor::pStaticGreenLod1);
	mem_release(level_editor::pStaticGreenLod2);
	mem_release(level_editor::pEditGreenName);
	mem_release(level_editor::pEditGreenModel);
	mem_release(level_editor::pEditGreenLod1);
	mem_release(level_editor::pEditGreenLod2);
	mem_release(level_editor::pButtonGreenLod1);
	mem_release(level_editor::pButtonGreenModel);
	mem_release(level_editor::pButtonGreenLod2);
	mem_release(level_editor::pButtonGreenMask);
	mem_release(level_editor::pCheckBoxGreenAveragedRGB);
	mem_release(level_editor::pStaticGreenMask);
	mem_release(level_editor::pEditGreenMask);
	mem_release(level_editor::pStaticGreenNav);
	mem_release(level_editor::pEditGreenNav);
	mem_release(level_editor::pButtonGreenNav);

	mem_release(level_editor::pTrackBarGreenDensity);
	mem_release(level_editor::pButtonGreenGenerate);
	mem_release(level_editor::pStaticGreenDensityText);
	mem_release(level_editor::pStaticGreenDensityVal);

	mem_delete(level_editor::pEditGreenSelX);
	mem_delete(level_editor::pEditGreenSelY);
	mem_delete(level_editor::pEditGreenSelZ);
	mem_delete(level_editor::pRadioButtonGreenSelX);
	mem_delete(level_editor::pRadioButtonGreenSelY);
	mem_delete(level_editor::pRadioButtonGreenSelZ);
	//mem_delete(level_editor::pButtonSingleSelDel);
	mem_delete(level_editor::pComboBoxGreenSel);
	mem_delete(level_editor::pStaticGreenSelX);
	mem_delete(level_editor::pStaticGreenSelY);
	mem_delete(level_editor::pStaticGreenSelZ);
	/*mem_delete(level_editor::pEditMultipleCreateVWidth);
	mem_delete(level_editor::pEditMultipleCreateVHeight);
	mem_delete(level_editor::pEditMultipleCreateVDepth);*/


	mem_delete(level_editor::pStaticGameClass);
	mem_delete(level_editor::pComboBoxGameClass);
	mem_delete(level_editor::pListViewGameClass);
	mem_delete(level_editor::pComboBoxGameValue);
	mem_delete(level_editor::pEditGameValue);
	mem_delete(level_editor::pButtonGameValue);
	mem_delete(level_editor::pStaticGameHelp);
	mem_delete(level_editor::pMemoGameHelp);
	mem_delete(level_editor::pButtonGameCreate);

	mem_delete(level_editor::pButtonAIQuadsDelSel);
	mem_delete(level_editor::pButtonAIGridGen);
	mem_delete(level_editor::pButtonAIClearAll);
	mem_delete(level_editor::pStaticAIBBDimensions);
	mem_delete(level_editor::pStaticAIBBDimensionsWidth);
	mem_delete(level_editor::pEditAIBBDimensionsWidth);
	mem_delete(level_editor::pStaticAIBBDimensionsHeight);
	mem_delete(level_editor::pEditAIBBDimensionsHeight);
	mem_delete(level_editor::pStaticAIBBDimensionsDepth);
	mem_delete(level_editor::pEditAIBBDimensionsDepth);
	mem_delete(level_editor::pStaticAIBBPos);
	mem_delete(level_editor::pStaticAIBBPosX);
	mem_delete(level_editor::pEditAIBBPosX);
	mem_delete(level_editor::pStaticAIBBPosY);
	mem_delete(level_editor::pEditAIBBPosY);
	mem_delete(level_editor::pStaticAIBBPosZ);
	mem_delete(level_editor::pEditAIBBPosZ);
	mem_delete(level_editor::pButtonAIBBFinish);
	mem_delete(level_editor::pButtonAIGPGen);
	mem_delete(level_editor::pButtonAIGPClear);
	mem_delete(level_editor::pRadioButtonAIGPAdd);
	mem_delete(level_editor::pRadioButtonAIGPDel);
	mem_delete(level_editor::pRadioButtonAIQuadAdd);
	mem_delete(level_editor::pRadioButtonAIQuadsMSel);
	mem_delete(level_editor::pRadioButtonAIQuadsSelDel);
	mem_delete(level_editor::pButtonAIGridValidation);
	mem_delete(level_editor::pCheckBoxAIGridMarkedSplits);
	mem_delete(level_editor::pStaticAIStatistics);
	mem_delete(level_editor::pStaticAIStatsCountQuads);
	mem_delete(level_editor::pStaticAIStatsCountGP);
	mem_delete(level_editor::pStaticAIStatsCountSplits);
	mem_delete(level_editor::pEditAIStatsCountQuads);
	mem_delete(level_editor::pEditAIStatsCountGP);
	mem_delete(level_editor::pEditAIStatsCountSplits);
}

//##########################################################################

void level_editor::LEcreateRenderData()
{
	level_editor::pAxesHelper = new CAxesHelper();
	D3DXCreateBox(SGCore_GetDXDevice(), 1, 1, 1, &level_editor::pFigureBox, 0);
}

void level_editor::LEdeleteRenderData()
{
	mem_delete(level_editor::pAxesHelper);
	mem_release(level_editor::pFigureBox);
}

void level_editor::NullingButtonSelTransform()
{
	level_editor::pCheckBoxTBPos->setCheck(false);
	level_editor::pCheckBoxTBRot->setCheck(false);
	level_editor::pCheckBoxTBScale->setCheck(false);
	level_editor::pAxesHelper->setType(CAxesHelper::HANDLER_TYPE_NONE);
}

void level_editor::LevelEditorUpdate(DWORD timeDelta)
{
	static float3 vObserverPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

	level_editor::vRayOrigin = vObserverPos;
	
	static float4x4 mObserverView, mObserverProj;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mObserverView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mObserverProj);

	float fDeterminant=0;
	SMMATRIX mInvObserverView = SMMatrixInverse(&fDeterminant, mObserverView);
	GetCursorPos(&level_editor::oPointMouse);
	ScreenToClient(SRender_GetHandleWin3D(), &level_editor::oPointMouse);

	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	level_editor::vRayDirDirect = float3(
		(2.0f * (float)level_editor::oPointMouse.x / float(*r_win_width) - 1.0f) / mObserverProj._11,
		-(2.0f * (float)level_editor::oPointMouse.y / float(*r_win_height) - 1.0f) / mObserverProj._22,
		1.0f
		) * mInvObserverView;
	level_editor::vRayDir = level_editor::vRayDirDirect - vObserverPos;
	
	//**********************************************************************

	static const float * r_far = GET_PCVAR_FLOAT("r_far");

	long count_poly_green = 0;
	for (int i = 0; i < SGreen_GetCount(); ++i)
	{
		count_poly_green += SGreen_MGetCountGen(i) * SGreen_MGetCountPoly(i);
	}

	long count_poly_geom = 0;
	for (int i = 0; i < SGeom_GetCountModels(); ++i)
	{
		count_poly_geom += SGeom_ModelGetCountPoly(i);
	}

	char text[256];
	sprintf(text, "%s%d", "Level poly: ", count_poly_geom + count_poly_green);
	level_editor::pStatusBar1->setPartText(0, text);

	sprintf(text, "%s%d", "Geom poly: ", count_poly_geom);
	level_editor::pStatusBar1->setPartText(1, text);

	sprintf(text, "%s%d", "Green poly: ", count_poly_green);
	level_editor::pStatusBar1->setPartText(2, text);

	int iCountGameObjects = 0;

	for (int i = 0, il = SXGame_EntGetCount(); i < il; ++i)
	{
		if (SXGame_EntGet(i))
			++iCountGameObjects;
	}

	sprintf(text, "%s%d", "Count game object: ", iCountGameObjects);
	level_editor::pStatusBar1->setPartText(3, text);

	if (level_editor::idMtl >= 0)
	{
		//sprintf(text, "%s", EDITORS_LEVEL_STATUSBAR_GAME_COUNT, SXGame_EntGetCount());
		SMtrl_MtlGetTexture(level_editor::idMtl, text);
		level_editor::pStatusBar1->setPartText(4, text);
	}



	if (level_editor::canGreenRenderBox)
	{
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		float3 tmppos = level_editor::vGreenBoxPos;
		tmppos.y += level_editor::vGreenBoxWHD.y * 0.5f;
		SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)(SMMatrixScaling(level_editor::vGreenBoxWHD) * SMMatrixTranslation(tmppos)));
		SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(SRender_EditorGetSelectTex()));
		level_editor::pFigureBox->DrawSubset(0);

		SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	if (level_editor::canSelSelection)
	{
		if (level_editor::canSelZTest)
		{
			SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		}
		else
		{
			SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		}

		if (level_editor::canSelMesh)
			SGCore_GetDXDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		if (level_editor::canSelBackFacesCull)
			SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);



		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
		{
			if (level_editor::idActiveElement > -1)
			{
				SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(SRender_EditorGetSelectTex()));
				SGeom_RenderSingly(timeDelta, level_editor::idActiveElement, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_STATIC));
			}
		}

		if (level_editor::useCopyData && level_editor::idCopy >= 0)
		{
			if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
			{
				SGeom_RenderSingly(timeDelta, level_editor::idCopy, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_STATIC), &(level_editor::vCopyPos));
			}
		}

		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (level_editor::idActiveElement > -1)
			{
				if (level_editor::idActiveGreenSplit >= 0 && level_editor::idActiveGreenObject >= 0)
				{
					SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(SRender_EditorGetSelectTex()));
					SGreen_RenderObject(timeDelta, &vObserverPos, level_editor::idActiveElement, level_editor::idActiveGreenSplit, level_editor::idActiveGreenObject, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_TREE));
				}
				else
				{
					SGCore_GetDXDevice()->SetTexture(0, SGCore_LoadTexGetTex(SRender_EditorGetSelectTex()));
					SGreen_RenderSingly(timeDelta, &vObserverPos, level_editor::idActiveElement, SMtrl_MtlGetStdMtl(MTLTYPE_MODEL_TREE));
				}
			}
		}

		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GAME)
		{
			ID idGameObj = level_editor::pListBoxList->getItemData(level_editor::idActiveElement);
			SXGame_EditorRender(idGameObj, SRender_EditorGetSelectTex());
		}

		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

		if (level_editor::canSelMesh)
			SGCore_GetDXDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		if (level_editor::canSelBackFacesCull)
			SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}


	SGCore_GetDXDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (level_editor::canAIGQuad)
		SAIG_RenderQuads(SRender_GetCamera()->getFrustum(), &vObserverPos, *r_far);

	if (level_editor::canAIGGraphPoint)
		SAIG_RenderGraphPoints(&vObserverPos, *r_far);

	if (level_editor::canAIGBound)
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

	if (level_editor::pAxesHelper)
	{
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		level_editor::pAxesHelper->render();
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		SGCore_GetDXDevice()->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	}

	if (!level_editor::useCopyData && SSInput_GetKeyState(SIK_LCONTROL) && SSInput_GetKeyState(SIK_C))
	{
		if (level_editor::iActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
		{
			int iSelected = level_editor::pListBoxList->getSel();
			if (iSelected < 0)
				return;

			level_editor::useCopyData = true;
			level_editor::idCopy = iSelected;
			SGeom_ModelGetPosition(level_editor::idCopy, &(level_editor::vCopyPos));
		}
	}

	//! если ПКМ либо данные копирования не валидны, то обнуляем данные копирования
	if (SSInput_GetKeyState(SIM_RBUTTON) || (level_editor::useCopyData && level_editor::idCopy < 0))
	{
		level_editor::useCopyData = false;
		level_editor::idCopy = -1;
	}
}

//##########################################################################

void level_editor::FinalImageUncheckedMenu()
{
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_COLOR, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_NORMALS, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_PARAMETERS, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_AMBIENTDIFFUSE, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_SPECULAR, false);
	level_editor::pMainMenu->setCheckItem(ID_FINALIMAGE_LIGHTINGSCENE, false);

	level_editor::pCheckBoxTBRColor->setCheck(false);
	level_editor::pCheckBoxTBRNormal->setCheck(false);
	level_editor::pCheckBoxTBRParam->setCheck(false);
	level_editor::pCheckBoxTBRAmDiff->setCheck(false);
	level_editor::pCheckBoxTBRSpecular->setCheck(false);
	level_editor::pCheckBoxTBRLighting->setCheck(false);
}

//##########################################################################

void level_editor::LevelNew(bool mess)
{
	if (mess && (SGeom_GetCountModels() <= 0 || (SGeom_GetCountModels() > 0 && MessageBox(0, "Are you sure you need to create a new level?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDNO)))
		return;

	SLevel_Clear();
	char tmpcaption[256];
	sprintf(tmpcaption, "%s: new level ** | %s", SX_LEVEL_EDITOR_NAME, SKYXENGINE_VERSION4EDITORS);
	level_editor::pJobWindow->setText(tmpcaption);
	level_editor::iActiveGroupType = 0;
	level_editor::idActiveElement = -1;
	level_editor::idActiveGreenSplit = -1;
	level_editor::idActiveGreenObject = -1;

	ID gid = SLight_GetGlobal();
	if (gid >= 0)
		SLight_DeleteLight(gid);

	level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
	level_editor::pCheckBoxTBLevelType->setCheck(false);
	level_editor::pCheckBoxTBGLightEnable->setCheck(false);

	SXLevelEditor_ButtonGeometryOpen_Click(0,0,0,0);
}

void level_editor::LevelOpen()
{
	if (SGeom_GetCountModels() > 0 && MessageBox(0, "Are you sure that you need to open the level?", 0, MB_YESNO | MB_ICONWARNING | MB_TASKMODAL) == IDNO)
		return;

	level_editor::LevelNew(false);

	char szSelName[MAX_PATH];
	char szSelPath[1024];
	szSelName[0] = szSelPath[0] = 0;
	gui_func::dialogs::SelectDirOwn(szSelName, szSelPath, Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), "Open level", false, false, 0, HandlerGetPreviewLevel);

	if (STR_VALIDATE(szSelPath))
	{
		SLevel_Load(szSelName, false);
		char szCaption[256];
		sprintf(szCaption, "%s: %s | %s", SX_LEVEL_EDITOR_NAME, szSelName, SKYXENGINE_VERSION4EDITORS);
		level_editor::pJobWindow->setText(szCaption);

		ID idGlobalLight = SLight_GetGlobal();
		if (idGlobalLight >= 0)
		{
			level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP26);
			level_editor::pCheckBoxTBLevelType->setCheck(true);
			level_editor::pCheckBoxTBGLightEnable->setCheck(true);
		}
		else
		{
			level_editor::pCheckBoxTBLevelType->setBmpFromResourse(IDB_BITMAP25);
			level_editor::pCheckBoxTBLevelType->setCheck(false);
			level_editor::pCheckBoxTBGLightEnable->setCheck(false);
		}
	}
}

void level_editor::LevelSave()
{
	if (SLevel_GetName()[0])
		SLevel_Save(SLevel_GetName());
	else
	{
		if (SGeom_GetCountModels() <= 0)
			MessageBox(0, "You need to create a level!", 0, 0);
		else
			level_editor::LevelSaveAs();
	}
}

void level_editor::LevelSaveAs()
{
	if (SGeom_GetCountModels() <= 0)
	{
		MessageBox(0, "You need to create a level!", 0, 0);
		return;
	}

	char szPath[1024];
	szPath[0] = 0;
	char szName[1024];

	gui_func::dialogs::SelectDirOwn(szName, szPath, Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), "Save level", false, true, 0/*, HandlerPreviewLevel*/);
	if (STR_VALIDATE(szPath))
	{
		SLevel_Save(szName);
		char szCaption[256];
		sprintf(szCaption, "%s: %s | %s", SX_LEVEL_EDITOR_NAME, szName, SKYXENGINE_VERSION4EDITORS);
		level_editor::pJobWindow->setText(szCaption);
	}
}

//##########################################################################

bool HandlerGetPreviewLevel(const char *szPath, char *szBuff)
{
	String sPathImg = FileAppendSlash(szPath) + "preview.bmp";
	if (FileExistsFile(sPathImg.c_str()))
	{
		sprintf(szBuff, "%s", sPathImg.c_str());
		return true;
	}
	return false;
}