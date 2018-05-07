
#include "particles_editor.h"

namespace SXParticlesEditor
{
	ISXGUIBaseWnd* JobWindow;
	ISXGUIMenuWindow* MainMenu;
	ISXGUIStatusBar* StatusBar1;
	ISXGUIGroupBox* GroupBoxList;
	ISXGUIGroupBox* GroupBoxTabs;
	ISXGUIGroupBox* GroupBoxData;
	ISXGUIGroupBox* GroupBoxData2;
	ISXGUIBaseWnd* WindowRender;

	ISXGUIToolBar* ToolBar1;

	ISXGUIButton* ButtonTBReOpen;
	ISXGUIButton* ButtonTBSave;
	ISXGUICheckBox* CheckBoxTBGrid;
	ISXGUICheckBox* CheckBoxTBAxes;
	ISXGUICheckBox* CheckBoxTBBound;
	ISXGUICheckBox* CheckBoxTBPlay;
	ISXGUICheckBox* CheckBoxTBPause;
	ISXGUICheckBox* CheckBoxTBStop;

	ISXGUIListBox* ListBoxEffects;
	ISXGUIListBox* ListBoxEmitters;
	ISXGUIStatic* StaticEffects;
	ISXGUIButton* ButtonEffectsCreate;
	ISXGUIStatic* StaticEmitters;
	//ISXGUIButton* ButtonEffectsEdit;
	ISXGUIButton* ButtonEffectsDelete;
	ISXGUIButton* ButtonEmittersCreate;
	ISXGUIButton* ButtonEmittersBasis;
	ISXGUIButton* ButtonEmittersDelete;

	ISXGUIRadioButton* ButtonBase;
	ISXGUIRadioButton* ButtonTextureAnimTex;
	ISXGUIRadioButton* ButtonSpawn;
	ISXGUIRadioButton* ButtonBound;
	ISXGUIRadioButton* ButtonCharacters;
	ISXGUIRadioButton* ButtonVelocityAcceleration;
	ISXGUIStatic* StaticEffectsCount;
	ISXGUIStatic* StaticEmittersCount;

	//effect
	//{
	ISXGUIStatic* StaticEffName;
	ISXGUIEdit* EditEffName;
	ISXGUIButton* ButtonEffCreate;
	//}

	//base
	//{
	ISXGUIStatic* StaticName;
	ISXGUIEdit* EditName;
	ISXGUIButton* ButtonEmitterCreate;
	ISXGUIStatic* StaticReCreateCount;
	ISXGUIEdit* EditReCreateCount;
	ISXGUIStatic* StaticCount;
	ISXGUIEdit* EditCount;
	ISXGUIStatic* StaticColorCoef;
	ISXGUIEdit* EditColorCoef;
	ISXGUIStatic* StaticSoftCoef;
	ISXGUIEdit* EditSoftCoef;
	ISXGUIStatic* StaticRefractionCoef;
	ISXGUIEdit* EditRefractionCoef;
	ISXGUIStatic* StaticTransparencyCoef;
	ISXGUIEdit* EditTransparencyCoef;
	ISXGUICheckBox* CheckBoxLighting;
	ISXGUIStatic* StaticFigureType;
	ISXGUIComboBox* ComboBoxFigureType;
	ISXGUIStatic* StaticFigureCountQuads;
	ISXGUIEdit* EditFigureCountQuads;
	ISXGUICheckBox* CheckBoxFigureRotRand;
	ISXGUICheckBox* CheckBoxFigureTapX;
	ISXGUICheckBox* CheckBoxFigureTapY;
	ISXGUICheckBox* CheckBoxFigureTapZ;
	ISXGUIStatic* StaticAlphaBlendType;
	ISXGUIComboBox* ComboBoxAlphaBlendType;

	ISXGUIStatic* StaticColor;
	ISXGUIStatic* StaticColorR;
	ISXGUIEdit* EditColorR;
	ISXGUIStatic* StaticColorG;
	ISXGUIEdit* EditColorG;
	ISXGUIStatic* StaticColorB;
	ISXGUIEdit* EditColorB;
	ISXGUIStatic* StaticColorA;
	ISXGUIEdit* EditColorA;

	ISXGUIStatic* StaticTimeLife;
	ISXGUIEdit* EditTimeLife;
	ISXGUIStatic* StaticTimeLifeDisp;
	ISXGUIEdit* EditTimeLifeDisp;
	ISXGUIStatic* StaticAlphaAgeDepend;
	ISXGUIComboBox* ComboBoxAlphaDependAge;
	ISXGUIStatic* StaticSize;
	ISXGUIStatic* StaticSizeX;
	ISXGUIEdit* EditSizeX;
	ISXGUIStatic* StaticSizeY;
	ISXGUIEdit* EditSizeY;
	ISXGUIStatic* StaticSizeDisp;
	ISXGUIEdit* EditSizeDisp;
	ISXGUIStatic* StaticSizeDependAge;
	ISXGUIComboBox* ComboBoxSizeDependAge;
	ISXGUICheckBox* CheckBoxCollisionDelete;

	ISXGUICheckBox* CheckBoxTrack;
	ISXGUIStatic* StaticTrackSize;
	ISXGUIEdit* EditTrackSize;
	ISXGUIStatic* StaticTrackTime;
	ISXGUIEdit* EditTrackTime;

	//}

	//bound
	//{
	ISXGUIStatic* StaticBoundType;
	ISXGUIComboBox* ComboBoxBoundType;
	ISXGUIStatic* StaticBoundVec1;
	ISXGUIStatic* StaticBoundVec1X;
	ISXGUIEdit* EditBoundVec1X;
	ISXGUIStatic* StaticBoundVec1Y;
	ISXGUIEdit* EditBoundVec1Y;
	ISXGUIStatic* StaticBoundVec1Z;
	ISXGUIEdit* EditBoundVec1Z;
	ISXGUIStatic* StaticBoundVec1W;
	ISXGUIEdit* EditBoundVec1W;
	ISXGUIStatic* StaticBoundVec2;
	ISXGUIStatic* StaticBoundVec2X;
	ISXGUIEdit* EditBoundVec2X;
	ISXGUIStatic* StaticBoundVec2Y;
	ISXGUIEdit* EditBoundVec2Y;
	ISXGUIStatic* StaticBoundVec2Z;
	ISXGUIEdit* EditBoundVec2Z;
	ISXGUIStatic* StaticBoundVec2W;
	ISXGUIEdit* EditBoundVec2W;
	//}

	//character
	//{
	ISXGUICheckBox* CheckBoxCircle;
	ISXGUIStatic* StaticCircleAxis;
	ISXGUIComboBox* ComboBoxCircleAxis;
	ISXGUIStatic* StaticCircleAngle;
	ISXGUIEdit* EditCircleAngle;
	ISXGUIStatic* StaticCircleAngleDisp;
	ISXGUIEdit* EditCircleAngleDisp;
	ISXGUICheckBox* CheckBoxCircleAngleDispNeg;
	ISXGUICheckBox* CheckBoxRotate;
	ISXGUIStatic* StaticRotateAngle;
	ISXGUIEdit* EditRotateAngle;
	ISXGUIStatic* StaticRotateAngleDisp;
	ISXGUIEdit* EditRotateAngleDisp;
	ISXGUICheckBox* CheckBoxRotateAngleDispNeg;
	ISXGUICheckBox* CheckBoxDeviation;
	ISXGUIStatic* StaticDeviationType;
	ISXGUIComboBox* ComboBoxDeviationType;
	ISXGUIStatic* StaticDeviationAmplitude;
	ISXGUIStatic* StaticDeviationCoefAngle;
	ISXGUIEdit* EditDeviationAmplitude;
	ISXGUIEdit* EditDeviationCoefAngle;
	ISXGUIStatic* StaticDeviationAxis;
	ISXGUIComboBox* ComboBoxDeviationAxis;
	ISXGUIStatic* StaticDeviationCountDelayMls;
	ISXGUIEdit* EditDeviationCountDelayMls;
	ISXGUIStatic* StaticDeviationCoefAngleDisp;
	ISXGUIEdit* EditDeviationCoefAngleDisp;
	ISXGUICheckBox* CheckBoxDeviationCoefAngleDispNeg;
	ISXGUICheckBox* CheckBoxDeviationTapX;
	ISXGUICheckBox* CheckBoxDeviationTapY;
	ISXGUICheckBox* CheckBoxDeviationTapZ;
	//}

	//spawn
	//{
	ISXGUIStatic* StaticSpawnPosType;
	ISXGUIComboBox* ComboBoxSpawnPosType;
	ISXGUIStatic* StaticSpawnOrigin;
	ISXGUIStatic* StaticSpawnOriginX;
	ISXGUIEdit* EditSpawnOriginX;
	ISXGUIStatic* StaticSpawnOriginY;
	ISXGUIEdit* EditSpawnOriginY;
	ISXGUIStatic* StaticSpawnOriginZ;
	ISXGUIEdit* EditSpawnOriginZ;
	ISXGUIStatic* StaticSpawnOriginDisp;
	ISXGUIEdit* EditSpawnOriginDisp;
	ISXGUICheckBox* CheckBoxSpawnOriginDispXPos;
	ISXGUICheckBox* CheckBoxSpawnOriginDispXNeg;
	ISXGUICheckBox* CheckBoxSpawnOriginDispYPos;
	ISXGUICheckBox* CheckBoxSpawnOriginDispYNeg;
	ISXGUICheckBox* CheckBoxSpawnOriginDispZPos;
	ISXGUICheckBox* CheckBoxSpawnOriginDispZNeg;
	ISXGUIStatic* StaticSpawnNextTime;
	ISXGUIEdit* EditSpawnNextTime;
	ISXGUIStatic* StaticSpawnNextTimeDisp;
	ISXGUIEdit* EditSpawnNextTimeDisp;
	//}

	//tex
	//{
	ISXGUIStatic* StaticTexture;
	ISXGUIEdit* EditTexture;
	ISXGUIButton* ButtonTextureSel;
	ISXGUIStatic* StaticAnimTexCountCadrsX;
	ISXGUIStatic* StaticAnimTexCountCadrsY;
	ISXGUIEdit* EditAnimTexCountCadrsX;
	ISXGUIEdit* EditAnimTexCountCadrsY;
	ISXGUIStatic* StaticAnimTexRate;
	ISXGUIEdit* EditAnimTexRate;
	ISXGUIStatic* StaticAnimTexRateDisp;
	ISXGUIEdit* EditAnimTexRateDisp;
	ISXGUIStatic* StaticAnimTexStartCadr;
	ISXGUIEdit* EditAnimTexStartCadr;
	ISXGUIStatic* StaticAnimTexStartCadrDisp;
	ISXGUIEdit* EditAnimTexStartCadrDisp;
	//}

	//velocity/acceleration
	//{
	ISXGUIStatic* StaticVelocity;
	ISXGUIStatic* StaticVelocityX;
	ISXGUIEdit* EditVelocityX;
	ISXGUIStatic* StaticVelocityY;
	ISXGUIEdit* EditVelocityY;
	ISXGUIStatic* StaticVelocityZ;
	ISXGUIEdit* EditVelocityZ;
	ISXGUIStatic* StaticVelocityDisp;
	ISXGUIEdit* EditVelocityDisp;
	ISXGUICheckBox* CheckBoxVelocityDispXNeg;
	ISXGUICheckBox* CheckBoxVelocityDispYNeg;
	ISXGUICheckBox* CheckBoxVelocityDispZNeg;
	ISXGUIStatic* StaticAcceleration;
	ISXGUIStatic* StaticAccelerationX;
	ISXGUIEdit* EditAccelerationX;
	ISXGUIStatic* StaticAccelerationY;
	ISXGUIEdit* EditAccelerationY;
	ISXGUIStatic* StaticAccelerationZ;
	ISXGUIEdit* EditAccelerationZ;
	ISXGUIStatic* StaticAccelerationDisp;
	ISXGUIEdit* EditAccelerationDisp;
	ISXGUICheckBox* CheckBoxAccelerationDispXNeg;
	ISXGUICheckBox* CheckBoxAccelerationDispYNeg;
	ISXGUICheckBox* CheckBoxAccelerationDispZNeg;
	//}

	void InitAllElements();

	void UpdateCountInfo();

	void EffReOpen();
	void EffSave();

	void EffInitList();

	void EffVisible(bool visible, bool iscreate);
	void EffNulling();
	void EffDataInit();

	void TabsVisible(bool visible);

	void AllInTabsVisible(bool visible);
	void AllInTabsNulling();

	void EmitterCreateVisible(bool visible);

	void BaseVisible(bool visible);
	void BaseDataInit();
	void BaseNulling();

	void BoundVisible(bool visible);
	void BoundDataInit();
	void BoundNulling();

	void CharacterVisible(bool visible);
	void CharacterDataInit();
	void CharacterNulling();

	void SpawnVisible(bool visible);
	void SpawnDataInit();
	void SpawnNulling();

	void TexVisible(bool visible);
	void TexDataInit();
	void TexNulling();

	void VelocityAccVisible(bool visible);
	void VelocityDataInit();
	void VelocityAccNulling();

	void DeleteAllElements();


	void PEcreateData();
	void PEdeleteData();

	bool canRenderBound = false;

	ID3DXMesh* FigureBox = 0;
	ID3DXMesh* FigureSphere = 0;
	ID3DXMesh* FigureCone = 0;
	float3_t FigureConeParam;
	float3 FigureConePos;

	ID SelEffID = -1;
	ID SelEmitterID = -1;
};


void SXParticlesEditor::InitAllElements()
{
	RECT wrect;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
	int cx = (wrect.right - MAINWIN_SIZE_X) / 2;
	int cy = (wrect.bottom - MAINWIN_SIZE_Y) / 2;

	SXParticlesEditor::JobWindow = SXGUICrBaseWndEx("JobWindow", "SXParticlesEditor", cx, cy, MAINWIN_SIZE_X, MAINWIN_SIZE_Y, 0, 0, CreateSolidBrush(RGB(220, 220, 220)), 0, CS_HREDRAW | CS_VREDRAW, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION, 0, WndProcAllDefault);
	SXParticlesEditor::JobWindow->setVisible(false);
	gui_func::base_handlers::InitHandlerMsg(SXParticlesEditor::JobWindow);

	SXParticlesEditor::JobWindow->setMixSize(MAINWIN_SIZE_X, MAINWIN_SIZE_Y);

	SXParticlesEditor::JobWindow->addHandler(ComMenuId, WM_COMMAND);
	SXParticlesEditor::JobWindow->addHandler(TrueExit, WM_CLOSE, 0, 0, 0, 0, true);
	SXParticlesEditor::JobWindow->addHandler(MsgEditSize, WM_SIZE);
	SXParticlesEditor::MainMenu = SXGUICrMenuWindowEx(IDR_MENU1);
	SXParticlesEditor::MainMenu->setToWindow(SXParticlesEditor::JobWindow->getHWND());

	SXParticlesEditor::StatusBar1 = SXGUICrStatusBar("StatusBar1", SXParticlesEditor::JobWindow->getHWND(), 0, 0);
	SXParticlesEditor::StatusBar1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	int arrpart[5];
	arrpart[0] = 300;
	arrpart[1] = -1;
	SXParticlesEditor::StatusBar1->setPartsCount(2, arrpart);
	SXParticlesEditor::StatusBar1->setPartText(0, "Playing emitters: 0/0");
	SXParticlesEditor::StatusBar1->setPartText(1, "Living particles: 0/0");

	SXParticlesEditor::WindowRender = SXGUICrBaseWndEx("WindowRender","Window1",0,27,600,400,0,0,CreateSolidBrush(RGB(200,200,200)),0,CS_HREDRAW | CS_VREDRAW,WS_CHILD | WS_VISIBLE | WS_BORDER,SXParticlesEditor::JobWindow->getHWND(),0);
	SXParticlesEditor::WindowRender->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::WindowRender->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	SXParticlesEditor::WindowRender->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::WindowRender->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);
	
	SXParticlesEditor::ToolBar1 = SXGUICrToolBar(0, 0, 804, 27, SXParticlesEditor::JobWindow->getHWND(), WndProcAllDefault, 0);
	SXParticlesEditor::ToolBar1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ToolBar1->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	SXParticlesEditor::ToolBar1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ToolBar1->setFollowParentSide(SXGUI_SIDE_BOTTOM, false);
	SXParticlesEditor::ToolBar1->addHandler(SXParticlesEditor_ToolBar1_CallWmCommand, WM_COMMAND);

	SXParticlesEditor::ButtonTBReOpen = SXGUICrButtonEx("", 2, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXParticlesEditor::ToolBar1->getHWND(), 0, 0);
	SXParticlesEditor::ButtonTBReOpen->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonTBReOpen->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonTBReOpen->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonTBReOpen->setBmpFromResourse(IDB_BITMAP1);

	SXParticlesEditor::ButtonTBSave = SXGUICrButtonEx("", 26, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_PUSHLIKE | BS_BITMAP, SXParticlesEditor::ToolBar1->getHWND(), 0, 0);
	SXParticlesEditor::ButtonTBSave->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonTBSave->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonTBSave->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonTBSave->setBmpFromResourse(IDB_BITMAP2);

	SXParticlesEditor::CheckBoxTBGrid = SXGUICrCheckBoxEx("", 54, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXParticlesEditor::ToolBar1->getHWND(), 0, 0);
	SXParticlesEditor::CheckBoxTBGrid->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxTBGrid->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxTBGrid->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxTBGrid->setBmpFromResourse(IDB_BITMAP3);

	SXParticlesEditor::CheckBoxTBAxes = SXGUICrCheckBoxEx("", 78, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXParticlesEditor::ToolBar1->getHWND(), 0, 0);
	SXParticlesEditor::CheckBoxTBAxes->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxTBAxes->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxTBAxes->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxTBAxes->setBmpFromResourse(IDB_BITMAP4);

	SXParticlesEditor::CheckBoxTBBound = SXGUICrCheckBoxEx("", 102, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXParticlesEditor::ToolBar1->getHWND(), 0, 0);
	SXParticlesEditor::CheckBoxTBBound->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxTBBound->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxTBBound->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxTBBound->setBmpFromResourse(IDB_BITMAP5);

	SXParticlesEditor::CheckBoxTBPlay = SXGUICrCheckBoxEx("", 130, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXParticlesEditor::ToolBar1->getHWND(), 0, 0);
	SXParticlesEditor::CheckBoxTBPlay->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxTBPlay->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxTBPlay->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxTBPlay->setBmpFromResourse(IDB_BITMAP6);

	SXParticlesEditor::CheckBoxTBPause = SXGUICrCheckBoxEx("", 154, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXParticlesEditor::ToolBar1->getHWND(), 0, 0);
	SXParticlesEditor::CheckBoxTBPause->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxTBPause->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxTBPause->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxTBPause->setBmpFromResourse(IDB_BITMAP7);

	SXParticlesEditor::CheckBoxTBStop = SXGUICrCheckBoxEx("", 178, 1, 22, 22, 0, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | BS_BITMAP, SXParticlesEditor::ToolBar1->getHWND(), 0, 0);
	SXParticlesEditor::CheckBoxTBStop->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxTBStop->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxTBStop->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxTBStop->setBmpFromResourse(IDB_BITMAP8);

	
	
	SXParticlesEditor::GroupBoxList = SXGUICrGroupBox("", 601, 28, 204, 400, SXParticlesEditor::JobWindow->getHWND(), WndProcAllDefault, 0);
	gui_func::base_handlers::InitHandlerMsg(SXParticlesEditor::GroupBoxList);
	SXParticlesEditor::GroupBoxList->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::GroupBoxList->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::GroupBoxList->setColorTextBk(RGB(220, 220, 220));
	SXParticlesEditor::GroupBoxList->setTransparentTextBk(true);
	SXParticlesEditor::GroupBoxList->setColorBrush(RGB(220, 220, 220));

	SXParticlesEditor::GroupBoxList->setFollowParentSide(SXGUI_SIDE_LEFT, false);
	SXParticlesEditor::GroupBoxList->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	SXParticlesEditor::GroupBoxList->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::GroupBoxList->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);

	SXParticlesEditor::StaticEffects = SXGUICrStatic("Effects:", 10, 10, 50, 15, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::StaticEffects->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticEffects->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticEffects->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticEffects->setTransparentTextBk(true);
	SXParticlesEditor::StaticEffects->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticEffects->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticEffects->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::StaticEffectsCount = SXGUICrStatic("0", 70, 10, 100, 15, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::StaticEffectsCount->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticEffectsCount->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticEffectsCount->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticEffectsCount->setTransparentTextBk(true);
	SXParticlesEditor::StaticEffectsCount->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticEffectsCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticEffectsCount->setFollowParentSide(SXGUI_SIDE_TOP, true);
	
	SXParticlesEditor::ListBoxEffects = SXGUICrListBox(2, 30, 200, 200, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0, false);
	SXParticlesEditor::ListBoxEffects->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXParticlesEditor::ListBoxEffects->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ListBoxEffects->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ListBoxEffects->setTransparentTextBk(true);
	SXParticlesEditor::ListBoxEffects->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ListBoxEffects->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ListBoxEffects->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ListBoxEffects->addHandler(SXParticlesEditor_ListBoxEffects_Click,WM_LBUTTONUP);
	SXParticlesEditor::ListBoxEffects->addHandler(SXParticlesEditor_ListBoxEffects_Click, WM_KEYUP, VK_UP, true, 0, false, false);
	SXParticlesEditor::ListBoxEffects->addHandler(SXParticlesEditor_ListBoxEffects_Click, WM_KEYUP, VK_DOWN, true, 0, false, false);

	SXParticlesEditor::ButtonEffectsCreate = SXGUICrButton("Create", 5, 225, 60, 15, SXGUI_BUTTON_IMAGE_NONE, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::ButtonEffectsCreate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonEffectsCreate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonEffectsCreate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonEffectsCreate->addHandler(SXParticlesEditor_ButtonEffectsCreate_Click, WM_LBUTTONUP);
	/*SXParticlesEditor::ButtonEffectsEdit = SXGUICrButton("Edit", 70, 225, 60, 15, SXGUI_BUTTON_IMAGE_NONE, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::ButtonEffectsEdit->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonEffectsEdit->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonEffectsEdit->setFollowParentSide(SXGUI_SIDE_TOP, true);*/
	SXParticlesEditor::ButtonEffectsDelete = SXGUICrButton("Delete", 135, 225, 60, 15, SXGUI_BUTTON_IMAGE_NONE, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::ButtonEffectsDelete->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonEffectsDelete->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonEffectsDelete->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonEffectsDelete->addHandler(SXParticlesEditor_ButtonEffectsDelete_Click, WM_LBUTTONUP);


	SXParticlesEditor::StaticEmitters = SXGUICrStatic("Emitters:", 10, 250, 50, 15, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::StaticEmitters->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticEmitters->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticEmitters->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticEmitters->setTransparentTextBk(true);
	SXParticlesEditor::StaticEmitters->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticEmitters->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticEmitters->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::StaticEmittersCount = SXGUICrStatic("0", 65, 250, 100, 15, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::StaticEmittersCount->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticEmittersCount->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticEmittersCount->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticEmittersCount->setTransparentTextBk(true);
	SXParticlesEditor::StaticEmittersCount->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticEmittersCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticEmittersCount->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::ListBoxEmitters = SXGUICrListBox(2, 270, 200, 120, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0, false);
	SXParticlesEditor::ListBoxEmitters->setFont("MS Shell Dlg",-11,0,400,0,0,0);
	SXParticlesEditor::ListBoxEmitters->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ListBoxEmitters->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ListBoxEmitters->setTransparentTextBk(true);
	SXParticlesEditor::ListBoxEmitters->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ListBoxEmitters->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ListBoxEmitters->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ListBoxEmitters->addHandler(SXParticlesEditor_ListBoxEmitters_Click, WM_LBUTTONUP);
	SXParticlesEditor::ListBoxEmitters->addHandler(SXParticlesEditor_ListBoxEmitters_Click, WM_KEYUP, VK_UP, true, 0, false, false);
	SXParticlesEditor::ListBoxEmitters->addHandler(SXParticlesEditor_ListBoxEmitters_Click, WM_KEYUP, VK_DOWN, true, 0, false, false);
	
	SXParticlesEditor::ButtonEmittersCreate = SXGUICrButton("Create", 5, 380, 60, 15, SXGUI_BUTTON_IMAGE_NONE, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::ButtonEmittersCreate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonEmittersCreate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonEmittersCreate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonEmittersCreate->addHandler(SXParticlesEditor_ButtonEmittersCreate_Click, WM_LBUTTONUP);

	SXParticlesEditor::ButtonEmittersBasis = SXGUICrButton("basis", 70, 380, 60, 15, SXGUI_BUTTON_IMAGE_NONE, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::ButtonEmittersBasis->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonEmittersBasis->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonEmittersBasis->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonEmittersBasis->addHandler(SXParticlesEditor_ButtonEmittersBasis_Click, WM_LBUTTONUP);
	SXParticlesEditor::ButtonEmittersDelete = SXGUICrButton("Delete", 135, 380, 60, 15, SXGUI_BUTTON_IMAGE_NONE, SXParticlesEditor::GroupBoxList->getHWND(), 0, 0);
	SXParticlesEditor::ButtonEmittersDelete->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonEmittersDelete->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonEmittersDelete->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonEmittersDelete->addHandler(SXParticlesEditor_ButtonEmittersDelete_Click, WM_LBUTTONUP);


	SXParticlesEditor::GroupBoxData = SXGUICrGroupBox("", 0, 425, 803, 170, SXParticlesEditor::JobWindow->getHWND(), WndProcAllDefault, 0);
	gui_func::base_handlers::InitHandlerMsg(SXParticlesEditor::GroupBoxData);
	//SXParticlesEditor::GroupBoxData->addHandler(SXLevelEditor_GroupBoxLight_CallWmCommand, WM_COMMAND);
	SXParticlesEditor::GroupBoxData->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::GroupBoxData->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::GroupBoxData->setColorTextBk(RGB(220, 220, 220));
	SXParticlesEditor::GroupBoxData->setTransparentTextBk(true);
	SXParticlesEditor::GroupBoxData->setColorBrush(RGB(220, 220, 220));

	SXParticlesEditor::GroupBoxData->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::GroupBoxData->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	SXParticlesEditor::GroupBoxData->setFollowParentSide(SXGUI_SIDE_TOP, false);
	SXParticlesEditor::GroupBoxData->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);

	SXParticlesEditor::GroupBoxData2 = SXGUICrGroupBox("", 0, 25, 803, 145, SXParticlesEditor::GroupBoxData->getHWND(), WndProcAllDefault, 0);
	gui_func::base_handlers::InitHandlerMsg(SXParticlesEditor::GroupBoxData2);
	//SXParticlesEditor::GroupBoxData->addHandler(SXLevelEditor_GroupBoxLight_CallWmCommand, WM_COMMAND);
	SXParticlesEditor::GroupBoxData2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::GroupBoxData2->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::GroupBoxData2->setColorTextBk(RGB(220, 220, 220));
	SXParticlesEditor::GroupBoxData2->setTransparentTextBk(true);
	SXParticlesEditor::GroupBoxData2->setColorBrush(RGB(220, 220, 220));

	SXParticlesEditor::GroupBoxData2->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::GroupBoxData2->setFollowParentSide(SXGUI_SIDE_RIGHT, true);
	SXParticlesEditor::GroupBoxData2->setFollowParentSide(SXGUI_SIDE_TOP, false);
	SXParticlesEditor::GroupBoxData2->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);
	SXParticlesEditor::GroupBoxData2->addHandler(SXParticlesEditor_GroupBoxData2_CallWmCommand, WM_COMMAND);

	SXParticlesEditor::GroupBoxTabs = SXGUICrGroupBox("", 0, 0, 655, 30, SXParticlesEditor::GroupBoxData->getHWND(), WndProcAllDefault, 0);
	gui_func::base_handlers::InitHandlerMsg(SXParticlesEditor::GroupBoxTabs);
	SXParticlesEditor::GroupBoxTabs->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::GroupBoxTabs->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::GroupBoxTabs->setColorTextBk(RGB(220, 220, 220));
	SXParticlesEditor::GroupBoxTabs->setTransparentTextBk(true);
	SXParticlesEditor::GroupBoxTabs->setColorBrush(RGB(220, 220, 220));

	SXParticlesEditor::GroupBoxTabs->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::GroupBoxTabs->setFollowParentSide(SXGUI_SIDE_RIGHT, false);
	SXParticlesEditor::GroupBoxTabs->setFollowParentSide(SXGUI_SIDE_TOP, false);
	SXParticlesEditor::GroupBoxTabs->setFollowParentSide(SXGUI_SIDE_BOTTOM, true);


	SXParticlesEditor::ButtonBase = SXGUICrRadioButtonEx("Base", 10, 10, 100, 15, 0, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE, SXParticlesEditor::GroupBoxTabs->getHWND(), 0, 0);
	SXParticlesEditor::ButtonBase->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonBase->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonBase->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonBase->addHandler(SXParticlesEditor_ButtonBase_Click, WM_LBUTTONUP);

	SXParticlesEditor::ButtonTextureAnimTex = SXGUICrRadioButtonEx("TextureAnimTex", 115, 10, 100, 15, 0, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE, SXParticlesEditor::GroupBoxTabs->getHWND(), 0, 0);
	SXParticlesEditor::ButtonTextureAnimTex->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonTextureAnimTex->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonTextureAnimTex->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonTextureAnimTex->addHandler(SXParticlesEditor_ButtonTextureAnimTex_Click, WM_LBUTTONUP);

	SXParticlesEditor::ButtonSpawn = SXGUICrRadioButtonEx("Spawn", 220, 10, 100, 15, 0, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE, SXParticlesEditor::GroupBoxTabs->getHWND(), 0, 0);
	SXParticlesEditor::ButtonSpawn->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonSpawn->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonSpawn->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonSpawn->addHandler(SXParticlesEditor_ButtonSpawn_Click, WM_LBUTTONUP);

	SXParticlesEditor::ButtonBound = SXGUICrRadioButtonEx("Bound", 325, 10, 100, 15, 0, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE, SXParticlesEditor::GroupBoxTabs->getHWND(), 0, 0);
	SXParticlesEditor::ButtonBound->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonBound->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonBound->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonBound->addHandler(SXParticlesEditor_ButtonBound_Click, WM_LBUTTONUP);

	SXParticlesEditor::ButtonCharacters = SXGUICrRadioButtonEx("Charaters", 430, 10, 100, 15, 0, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE, SXParticlesEditor::GroupBoxTabs->getHWND(), 0, 0);
	SXParticlesEditor::ButtonCharacters->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonCharacters->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonCharacters->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonCharacters->addHandler(SXParticlesEditor_ButtonCharacters_Click, WM_LBUTTONUP);

	SXParticlesEditor::ButtonVelocityAcceleration = SXGUICrRadioButtonEx("VelocityAcceleration", 535, 10, 110, 15, 0, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | BS_PUSHLIKE, SXParticlesEditor::GroupBoxTabs->getHWND(), 0, 0);
	SXParticlesEditor::ButtonVelocityAcceleration->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonVelocityAcceleration->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonVelocityAcceleration->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonVelocityAcceleration->addHandler(SXParticlesEditor_ButtonVelocityAcceleration_Click, WM_LBUTTONUP);

	SXParticlesEditor::StaticEffName = SXGUICrStatic("Name:", 5, 10, 40, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticEffName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticEffName->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticEffName->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticEffName->setTransparentTextBk(true);
	SXParticlesEditor::StaticEffName->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticEffName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticEffName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditEffName = SXGUICrEdit("0", 50, 10, 200, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditEffName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditEffName->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditEffName->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditEffName->setTransparentTextBk(true);
	SXParticlesEditor::EditEffName->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditEffName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditEffName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditEffName->addHandler(SXParticlesEditor_EditEffName_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);

	SXParticlesEditor::ButtonEffCreate = SXGUICrButton("Create", 260, 10, 50, 15, SXGUI_BUTTON_IMAGE_NONE, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ButtonEffCreate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonEffCreate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonEffCreate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonEffCreate->addHandler(SXParticlesEditor_ButtonEffCreate_Click, WM_LBUTTONUP);


	//base
	//{
	SXParticlesEditor::StaticName = SXGUICrStatic("Name:", 5, 10, 40, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticName->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticName->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticName->setTransparentTextBk(true);
	SXParticlesEditor::StaticName->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditName = SXGUICrEdit("0", 50, 10, 200, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditName->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditName->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditName->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditName->setTransparentTextBk(true);
	SXParticlesEditor::EditName->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditName->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditName->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditName->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditName->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticCount = SXGUICrStatic("Count:", 5, 30, 75, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticCount->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticCount->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticCount->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticCount->setTransparentTextBk(true);
	SXParticlesEditor::StaticCount->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticCount->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditCount = SXGUICrEdit("0", 50, 30, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditCount->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditCount->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditCount->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditCount->setTransparentTextBk(true);
	SXParticlesEditor::EditCount->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditCount->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditCount->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditCount->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticColorCoef = SXGUICrStatic("ColorCoef:", 130, 30, 75, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticColorCoef->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticColorCoef->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticColorCoef->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticColorCoef->setTransparentTextBk(true);
	SXParticlesEditor::StaticColorCoef->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticColorCoef->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticColorCoef->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditColorCoef = SXGUICrEdit("0", 180, 30, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditColorCoef->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditColorCoef->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditColorCoef->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditColorCoef->setTransparentTextBk(true);
	SXParticlesEditor::EditColorCoef->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditColorCoef->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditColorCoef->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditColorCoef->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditColorCoef->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::ButtonEmitterCreate = SXGUICrButton("CREATE", 725, 120, 70, 20, SXGUI_BUTTON_IMAGE_NONE, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ButtonEmitterCreate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonEmitterCreate->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ButtonEmitterCreate->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ButtonEmitterCreate->setTransparentTextBk(true);
	SXParticlesEditor::ButtonEmitterCreate->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::ButtonEmitterCreate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonEmitterCreate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonEmitterCreate->addHandler(SXParticlesEditor_ButtonEmitterCreate_Click, WM_LBUTTONUP);

	SXParticlesEditor::StaticReCreateCount = SXGUICrStatic("ReCreateCount:", 5, 50, 75, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticReCreateCount->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticReCreateCount->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticReCreateCount->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticReCreateCount->setTransparentTextBk(true);
	SXParticlesEditor::StaticReCreateCount->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticReCreateCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticReCreateCount->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditReCreateCount = SXGUICrEdit("0", 100, 50, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditReCreateCount->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditReCreateCount->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditReCreateCount->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditReCreateCount->setTransparentTextBk(true);
	SXParticlesEditor::EditReCreateCount->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditReCreateCount->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditReCreateCount->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditReCreateCount->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditReCreateCount->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticSoftCoef = SXGUICrStatic("SoftCoef:", 5, 70, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSoftCoef->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSoftCoef->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSoftCoef->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSoftCoef->setTransparentTextBk(true);
	SXParticlesEditor::StaticSoftCoef->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSoftCoef->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSoftCoef->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSoftCoef = SXGUICrEdit("0", 100, 70, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSoftCoef->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSoftCoef->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSoftCoef->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSoftCoef->setTransparentTextBk(true);
	SXParticlesEditor::EditSoftCoef->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSoftCoef->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSoftCoef->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSoftCoef->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSoftCoef->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticRefractionCoef = SXGUICrStatic("RefractionCoef:", 5, 90, 75, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticRefractionCoef->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticRefractionCoef->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticRefractionCoef->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticRefractionCoef->setTransparentTextBk(true);
	SXParticlesEditor::StaticRefractionCoef->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticRefractionCoef->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticRefractionCoef->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditRefractionCoef = SXGUICrEdit("0", 100, 90, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditRefractionCoef->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditRefractionCoef->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditRefractionCoef->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditRefractionCoef->setTransparentTextBk(true);
	SXParticlesEditor::EditRefractionCoef->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditRefractionCoef->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditRefractionCoef->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditRefractionCoef->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditRefractionCoef->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticTransparencyCoef = SXGUICrStatic("TransparencyCoef:", 5, 110, 90, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticTransparencyCoef->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticTransparencyCoef->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticTransparencyCoef->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticTransparencyCoef->setTransparentTextBk(true);
	SXParticlesEditor::StaticTransparencyCoef->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticTransparencyCoef->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticTransparencyCoef->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTransparencyCoef = SXGUICrEdit("0", 100, 110, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditTransparencyCoef->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditTransparencyCoef->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditTransparencyCoef->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditTransparencyCoef->setTransparentTextBk(true);
	SXParticlesEditor::EditTransparencyCoef->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditTransparencyCoef->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditTransparencyCoef->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTransparencyCoef->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditTransparencyCoef->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::CheckBoxLighting = SXGUICrCheckBox("Lighting", 180, 50, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxLighting->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxLighting->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxLighting->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxLighting->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxLighting->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxLighting->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxLighting->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::CheckBoxCollisionDelete = SXGUICrCheckBox("CollisionDelete", 180, 70, 90, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxCollisionDelete->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxCollisionDelete->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxCollisionDelete->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxCollisionDelete->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxCollisionDelete->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxCollisionDelete->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxCollisionDelete->setFollowParentSide(SXGUI_SIDE_TOP, true);


	SXParticlesEditor::StaticFigureType = SXGUICrStatic("FigureType:", 270, 12, 60, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticFigureType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticFigureType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticFigureType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticFigureType->setTransparentTextBk(true);
	SXParticlesEditor::StaticFigureType->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticFigureType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticFigureType->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ComboBoxFigureType = SXGUICrComboBox(335, 10, 170, 75, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ComboBoxFigureType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ComboBoxFigureType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ComboBoxFigureType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxFigureType->setTransparentTextBk(true);
	SXParticlesEditor::ComboBoxFigureType->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxFigureType->addItem("quad");
	SXParticlesEditor::ComboBoxFigureType->addItem("billboard");
	SXParticlesEditor::ComboBoxFigureType->addItem("quad_composite");
	SXParticlesEditor::ComboBoxFigureType->setSel(0);
	SXParticlesEditor::ComboBoxFigureType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ComboBoxFigureType->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::StaticFigureCountQuads = SXGUICrStatic("FigureCountQuads:", 270, 35, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticFigureCountQuads->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticFigureCountQuads->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticFigureCountQuads->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticFigureCountQuads->setTransparentTextBk(true);
	SXParticlesEditor::StaticFigureCountQuads->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticFigureCountQuads->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticFigureCountQuads->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditFigureCountQuads = SXGUICrEdit("0", 375, 35, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditFigureCountQuads->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditFigureCountQuads->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditFigureCountQuads->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditFigureCountQuads->setTransparentTextBk(true);
	SXParticlesEditor::EditFigureCountQuads->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditFigureCountQuads->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditFigureCountQuads->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditFigureCountQuads->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditFigureCountQuads->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::CheckBoxFigureRotRand = SXGUICrCheckBox("FigureRotRand", 270, 55, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxFigureRotRand->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxFigureRotRand->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxFigureRotRand->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxFigureRotRand->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxFigureRotRand->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxFigureRotRand->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxFigureRotRand->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxFigureTapX = SXGUICrCheckBox("FigureTapX", 270, 75, 75, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxFigureTapX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxFigureTapX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxFigureTapX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxFigureTapX->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxFigureTapX->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxFigureTapX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxFigureTapX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxFigureTapY = SXGUICrCheckBox("FigureTapY", 350, 75, 75, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxFigureTapY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxFigureTapY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxFigureTapY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxFigureTapY->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxFigureTapY->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxFigureTapY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxFigureTapY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxFigureTapZ = SXGUICrCheckBox("FigureTapZ", 430, 75, 75, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxFigureTapZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxFigureTapZ->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxFigureTapZ->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxFigureTapZ->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxFigureTapZ->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxFigureTapZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxFigureTapZ->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::StaticAlphaBlendType = SXGUICrStatic("AlphaBlendType:", 270, 97, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAlphaBlendType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAlphaBlendType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAlphaBlendType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAlphaBlendType->setTransparentTextBk(true);
	SXParticlesEditor::StaticAlphaBlendType->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAlphaBlendType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAlphaBlendType->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ComboBoxAlphaBlendType = SXGUICrComboBox(355, 95, 150, 75, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ComboBoxAlphaBlendType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ComboBoxAlphaBlendType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ComboBoxAlphaBlendType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxAlphaBlendType->setTransparentTextBk(true);
	SXParticlesEditor::ComboBoxAlphaBlendType->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxAlphaBlendType->addItem("pabt_alpha");
	SXParticlesEditor::ComboBoxAlphaBlendType->addItem("pabt_add");
	SXParticlesEditor::ComboBoxAlphaBlendType->setSel(0);
	SXParticlesEditor::ComboBoxAlphaBlendType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ComboBoxAlphaBlendType->setFollowParentSide(SXGUI_SIDE_TOP, true);


	SXParticlesEditor::StaticColor = SXGUICrStatic("Color:", 270, 120, 30, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticColor->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticColor->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticColor->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticColor->setTransparentTextBk(true);
	SXParticlesEditor::StaticColor->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticColor->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticColor->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::StaticColorR = SXGUICrStatic("r:", 302, 120, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticColorR->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticColorR->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticColorR->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticColorR->setTransparentTextBk(true);
	SXParticlesEditor::StaticColorR->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticColorR->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticColorR->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::EditColorR = SXGUICrEdit("0", 310, 120, 45, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditColorR->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditColorR->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditColorR->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditColorR->setTransparentTextBk(true);
	SXParticlesEditor::EditColorR->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditColorR->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditColorR->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditColorR->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditColorR->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticColorG = SXGUICrStatic("g:", 356, 120, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticColorG->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticColorG->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticColorG->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticColorG->setTransparentTextBk(true);
	SXParticlesEditor::StaticColorG->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticColorG->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticColorG->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::EditColorG = SXGUICrEdit("0", 365, 120, 45, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditColorG->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditColorG->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditColorG->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditColorG->setTransparentTextBk(true);
	SXParticlesEditor::EditColorG->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditColorG->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditColorG->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditColorG->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditColorG->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticColorB = SXGUICrStatic("b:", 411, 120, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticColorB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticColorB->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticColorB->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticColorB->setTransparentTextBk(true);
	SXParticlesEditor::StaticColorB->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticColorB->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticColorB->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::EditColorB = SXGUICrEdit("0", 420, 120, 45, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditColorB->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditColorB->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditColorB->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditColorB->setTransparentTextBk(true);
	SXParticlesEditor::EditColorB->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditColorB->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditColorB->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditColorB->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditColorB->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticColorA = SXGUICrStatic("a:", 466, 120, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticColorA->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticColorA->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticColorA->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticColorA->setTransparentTextBk(true);
	SXParticlesEditor::StaticColorA->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticColorA->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticColorA->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::EditColorA = SXGUICrEdit("0", 475, 120, 45, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditColorA->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditColorA->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditColorA->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditColorA->setTransparentTextBk(true);
	SXParticlesEditor::EditColorA->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditColorA->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditColorA->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditColorA->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditColorA->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	/*ISXGUIStatic* StaticColorG;
	ISXGUIEdit* EditColorG;
	ISXGUIStatic* StaticColorB;
	ISXGUIEdit* EditColorB;
	ISXGUIStatic* StaticColorA;
	ISXGUIEdit* EditColorA;*/


	SXParticlesEditor::StaticTimeLife = SXGUICrStatic("TimeLife:", 530, 10, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticTimeLife->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticTimeLife->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticTimeLife->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticTimeLife->setTransparentTextBk(true);
	SXParticlesEditor::StaticTimeLife->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticTimeLife->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticTimeLife->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTimeLife = SXGUICrEdit("0", 585, 10, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditTimeLife->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditTimeLife->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditTimeLife->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditTimeLife->setTransparentTextBk(true);
	SXParticlesEditor::EditTimeLife->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditTimeLife->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditTimeLife->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTimeLife->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditTimeLife->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticTimeLifeDisp = SXGUICrStatic("TimeLifeDisp:", 660, 10, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticTimeLifeDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticTimeLifeDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticTimeLifeDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticTimeLifeDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticTimeLifeDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticTimeLifeDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticTimeLifeDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTimeLifeDisp = SXGUICrEdit("0", 730, 10, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditTimeLifeDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditTimeLifeDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditTimeLifeDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditTimeLifeDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditTimeLifeDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditTimeLifeDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditTimeLifeDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTimeLifeDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditTimeLifeDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticAlphaAgeDepend = SXGUICrStatic("AlphaAgeDepend:", 530, 32, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAlphaAgeDepend->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAlphaAgeDepend->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAlphaAgeDepend->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAlphaAgeDepend->setTransparentTextBk(true);
	SXParticlesEditor::StaticAlphaAgeDepend->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAlphaAgeDepend->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAlphaAgeDepend->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ComboBoxAlphaDependAge = SXGUICrComboBox(630, 30, 170, 75, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ComboBoxAlphaDependAge->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ComboBoxAlphaDependAge->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ComboBoxAlphaDependAge->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxAlphaDependAge->setTransparentTextBk(true);
	SXParticlesEditor::ComboBoxAlphaDependAge->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxAlphaDependAge->addItem("none");
	SXParticlesEditor::ComboBoxAlphaDependAge->addItem("direct");
	SXParticlesEditor::ComboBoxAlphaDependAge->addItem("inverse");
	SXParticlesEditor::ComboBoxAlphaDependAge->setSel(0);
	SXParticlesEditor::ComboBoxAlphaDependAge->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ComboBoxAlphaDependAge->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::StaticSize = SXGUICrStatic("Size:", 530, 55, 25, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSize->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSize->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSize->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSize->setTransparentTextBk(true);
	SXParticlesEditor::StaticSize->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSize->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSize->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticSizeX = SXGUICrStatic("x:", 555, 55, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSizeX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSizeX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSizeX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSizeX->setTransparentTextBk(true);
	SXParticlesEditor::StaticSizeX->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSizeX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSizeX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSizeX = SXGUICrEdit("0", 565, 55, 65, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSizeX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSizeX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSizeX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSizeX->setTransparentTextBk(true);
	SXParticlesEditor::EditSizeX->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSizeX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSizeX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSizeX->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSizeX->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticSizeY = SXGUICrStatic("y:", 635, 55, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSizeY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSizeY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSizeY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSizeY->setTransparentTextBk(true);
	SXParticlesEditor::StaticSizeY->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSizeY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSizeY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSizeY = SXGUICrEdit("0", 645, 55, 65, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSizeY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSizeY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSizeY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSizeY->setTransparentTextBk(true);
	SXParticlesEditor::EditSizeY->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSizeY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSizeY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSizeY->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSizeY->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticSizeDisp = SXGUICrStatic("Disp:", 710, 55, 25, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSizeDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSizeDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSizeDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSizeDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticSizeDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSizeDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSizeDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSizeDisp = SXGUICrEdit("0", 735, 55, 65, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSizeDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSizeDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSizeDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSizeDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditSizeDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSizeDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSizeDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSizeDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSizeDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticSizeDependAge = SXGUICrStatic("SizeDependAge:", 530, 77, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSizeDependAge->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSizeDependAge->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSizeDependAge->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSizeDependAge->setTransparentTextBk(true);
	SXParticlesEditor::StaticSizeDependAge->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSizeDependAge->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSizeDependAge->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ComboBoxSizeDependAge = SXGUICrComboBox(630, 75, 170, 75, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ComboBoxSizeDependAge->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ComboBoxSizeDependAge->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ComboBoxSizeDependAge->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxSizeDependAge->setTransparentTextBk(true);
	SXParticlesEditor::ComboBoxSizeDependAge->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxSizeDependAge->addItem("none");
	SXParticlesEditor::ComboBoxSizeDependAge->addItem("direct");
	SXParticlesEditor::ComboBoxSizeDependAge->addItem("inverse");
	SXParticlesEditor::ComboBoxSizeDependAge->setSel(0);
	SXParticlesEditor::ComboBoxSizeDependAge->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ComboBoxSizeDependAge->setFollowParentSide(SXGUI_SIDE_TOP, true);


	SXParticlesEditor::CheckBoxTrack = SXGUICrCheckBox("Track", 530, 100, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxTrack->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxTrack->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxTrack->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxTrack->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxTrack->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxTrack->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxTrack->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::StaticTrackSize = SXGUICrStatic("Size:", 580, 100, 25, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticTrackSize->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticTrackSize->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticTrackSize->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticTrackSize->setTransparentTextBk(true);
	SXParticlesEditor::StaticTrackSize->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticTrackSize->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticTrackSize->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::EditTrackSize = SXGUICrEdit("0", 605, 100, 60, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditTrackSize->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditTrackSize->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditTrackSize->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditTrackSize->setTransparentTextBk(true);
	SXParticlesEditor::EditTrackSize->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditTrackSize->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditTrackSize->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTrackSize->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditTrackSize->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticTrackTime = SXGUICrStatic("Time:", 670, 100, 25, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticTrackTime->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticTrackTime->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticTrackTime->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticTrackTime->setTransparentTextBk(true);
	SXParticlesEditor::StaticTrackTime->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticTrackTime->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticTrackTime->setFollowParentSide(SXGUI_SIDE_TOP, true);

	SXParticlesEditor::EditTrackTime = SXGUICrEdit("0", 700, 100, 60, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditTrackTime->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditTrackTime->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditTrackTime->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditTrackTime->setTransparentTextBk(true);
	SXParticlesEditor::EditTrackTime->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditTrackTime->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditTrackTime->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTrackTime->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditTrackTime->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);
	//}

	//bound
	//{
	SXParticlesEditor::StaticBoundType = SXGUICrStatic("Bound type:", 5, 12, 60, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundType->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundType->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundType->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ComboBoxBoundType = SXGUICrComboBox(80, 10, 140, 100, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ComboBoxBoundType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ComboBoxBoundType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ComboBoxBoundType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxBoundType->setTransparentTextBk(true);
	SXParticlesEditor::ComboBoxBoundType->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxBoundType->addItem("none");
	SXParticlesEditor::ComboBoxBoundType->addItem("sphere");
	SXParticlesEditor::ComboBoxBoundType->addItem("box");
	SXParticlesEditor::ComboBoxBoundType->addItem("cone");
	SXParticlesEditor::ComboBoxBoundType->setSel(0);
	SXParticlesEditor::ComboBoxBoundType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ComboBoxBoundType->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticBoundVec1 = SXGUICrStatic("Vector1:", 5, 40, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec1->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec1->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec1->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec1->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec1->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec1->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec1->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticBoundVec1X = SXGUICrStatic("x:", 60, 40, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec1X->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec1X->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec1X->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec1X->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec1X->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec1X->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec1X->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec1X = SXGUICrEdit("0", 70, 40, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditBoundVec1X->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditBoundVec1X->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditBoundVec1X->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec1X->setTransparentTextBk(true);
	SXParticlesEditor::EditBoundVec1X->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec1X->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditBoundVec1X->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec1X->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditBoundVec1X->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticBoundVec1Y = SXGUICrStatic("y:", 150, 40, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec1Y->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec1Y->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec1Y->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec1Y->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec1Y->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec1Y->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec1Y->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec1Y = SXGUICrEdit("0", 160, 40, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditBoundVec1Y->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditBoundVec1Y->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditBoundVec1Y->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec1Y->setTransparentTextBk(true);
	SXParticlesEditor::EditBoundVec1Y->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec1Y->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditBoundVec1Y->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec1Y->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditBoundVec1Y->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticBoundVec1Z = SXGUICrStatic("z:", 235, 40, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec1Z->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec1Z->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec1Z->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec1Z->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec1Z->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec1Z->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec1Z->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec1Z = SXGUICrEdit("0", 245, 40, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditBoundVec1Z->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditBoundVec1Z->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditBoundVec1Z->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec1Z->setTransparentTextBk(true);
	SXParticlesEditor::EditBoundVec1Z->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec1Z->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditBoundVec1Z->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec1Z->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditBoundVec1Z->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticBoundVec1W = SXGUICrStatic("w:", 320, 40, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec1W->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec1W->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec1W->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec1W->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec1W->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec1W->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec1W->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec1W = SXGUICrEdit("0", 330, 40, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditBoundVec1W->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditBoundVec1W->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditBoundVec1W->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec1W->setTransparentTextBk(true);
	SXParticlesEditor::EditBoundVec1W->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec1W->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditBoundVec1W->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec1W->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditBoundVec1W->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticBoundVec2 = SXGUICrStatic("Vector2:", 5, 60, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec2->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec2->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec2->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec2->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec2->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec2->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec2->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticBoundVec2X = SXGUICrStatic("x:", 60, 60, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec2X->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec2X->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec2X->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec2X->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec2X->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec2X->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec2X->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec2X = SXGUICrEdit("0", 70, 60, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditBoundVec2X->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditBoundVec2X->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditBoundVec2X->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec2X->setTransparentTextBk(true);
	SXParticlesEditor::EditBoundVec2X->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec2X->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditBoundVec2X->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec2X->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditBoundVec2X->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticBoundVec2Y = SXGUICrStatic("y:", 150, 60, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec2Y->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec2Y->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec2Y->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec2Y->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec2Y->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec2Y->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec2Y->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec2Y = SXGUICrEdit("0", 160, 60, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditBoundVec2Y->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditBoundVec2Y->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditBoundVec2Y->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec2Y->setTransparentTextBk(true);
	SXParticlesEditor::EditBoundVec2Y->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec2Y->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditBoundVec2Y->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec2Y->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditBoundVec2Y->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticBoundVec2Z = SXGUICrStatic("z:", 235, 60, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec2Z->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec2Z->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec2Z->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec2Z->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec2Z->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec2Z->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec2Z->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec2Z = SXGUICrEdit("0", 245, 60, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditBoundVec2Z->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditBoundVec2Z->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditBoundVec2Z->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec2Z->setTransparentTextBk(true);
	SXParticlesEditor::EditBoundVec2Z->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec2Z->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditBoundVec2Z->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec2Z->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditBoundVec2Z->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticBoundVec2W = SXGUICrStatic("w:", 320, 60, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticBoundVec2W->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticBoundVec2W->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticBoundVec2W->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticBoundVec2W->setTransparentTextBk(true);
	SXParticlesEditor::StaticBoundVec2W->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticBoundVec2W->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticBoundVec2W->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec2W = SXGUICrEdit("0", 330, 60, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditBoundVec2W->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditBoundVec2W->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditBoundVec2W->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec2W->setTransparentTextBk(true);
	SXParticlesEditor::EditBoundVec2W->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditBoundVec2W->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditBoundVec2W->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditBoundVec2W->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditBoundVec2W->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	//}

	//character
	//{
	SXParticlesEditor::CheckBoxCircle = SXGUICrCheckBox("Circle", 5, 10, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxCircle->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxCircle->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxCircle->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxCircle->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxCircle->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxCircle->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxCircle->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticCircleAxis = SXGUICrStatic("CircleAxis:", 5, 32, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticCircleAxis->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticCircleAxis->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticCircleAxis->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticCircleAxis->setTransparentTextBk(true);
	SXParticlesEditor::StaticCircleAxis->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticCircleAxis->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticCircleAxis->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ComboBoxCircleAxis = SXGUICrComboBox(60, 30, 100, 75, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ComboBoxCircleAxis->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ComboBoxCircleAxis->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ComboBoxCircleAxis->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxCircleAxis->setTransparentTextBk(true);
	SXParticlesEditor::ComboBoxCircleAxis->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxCircleAxis->addItem("x");
	SXParticlesEditor::ComboBoxCircleAxis->addItem("y");
	SXParticlesEditor::ComboBoxCircleAxis->addItem("z");
	SXParticlesEditor::ComboBoxCircleAxis->setSel(0);
	SXParticlesEditor::ComboBoxCircleAxis->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ComboBoxCircleAxis->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticCircleAngle = SXGUICrStatic("CircleAngle:", 5, 55, 60, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticCircleAngle->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticCircleAngle->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticCircleAngle->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticCircleAngle->setTransparentTextBk(true);
	SXParticlesEditor::StaticCircleAngle->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticCircleAngle->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticCircleAngle->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditCircleAngle = SXGUICrEdit("0", 90, 55, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditCircleAngle->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditCircleAngle->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditCircleAngle->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditCircleAngle->setTransparentTextBk(true);
	SXParticlesEditor::EditCircleAngle->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditCircleAngle->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditCircleAngle->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditCircleAngle->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditCircleAngle->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticCircleAngleDisp = SXGUICrStatic("CircleAngleDisp:", 6, 75, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticCircleAngleDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticCircleAngleDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticCircleAngleDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticCircleAngleDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticCircleAngleDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticCircleAngleDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticCircleAngleDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditCircleAngleDisp = SXGUICrEdit("0", 90, 75, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditCircleAngleDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditCircleAngleDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditCircleAngleDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditCircleAngleDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditCircleAngleDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditCircleAngleDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditCircleAngleDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditCircleAngleDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditCircleAngleDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::CheckBoxRotate = SXGUICrCheckBox("Rotate", 175, 10, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxRotate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxRotate->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxRotate->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxRotate->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxRotate->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxRotate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxRotate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticRotateAngle = SXGUICrStatic("RotateAngle:", 175, 30, 85, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticRotateAngle->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticRotateAngle->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticRotateAngle->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticRotateAngle->setTransparentTextBk(true);
	SXParticlesEditor::StaticRotateAngle->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticRotateAngle->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticRotateAngle->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditRotateAngle = SXGUICrEdit("0", 265, 30, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditRotateAngle->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditRotateAngle->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditRotateAngle->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditRotateAngle->setTransparentTextBk(true);
	SXParticlesEditor::EditRotateAngle->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditRotateAngle->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditRotateAngle->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditRotateAngle->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditRotateAngle->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticRotateAngleDisp = SXGUICrStatic("RotateAngleDisp:", 175, 50, 85, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticRotateAngleDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticRotateAngleDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticRotateAngleDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticRotateAngleDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticRotateAngleDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticRotateAngleDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticRotateAngleDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxCircleAngleDispNeg = SXGUICrCheckBox("CircleAngleDispNeg", 5, 95, 120, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxCircleAngleDispNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditRotateAngleDisp = SXGUICrEdit("0", 265, 50, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditRotateAngleDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditRotateAngleDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditRotateAngleDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditRotateAngleDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditRotateAngleDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditRotateAngleDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditRotateAngleDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditRotateAngleDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditRotateAngleDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::CheckBoxRotateAngleDispNeg = SXGUICrCheckBox("RotateAngleDispNeg", 175, 70, 120, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxRotateAngleDispNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxDeviation = SXGUICrCheckBox("Deviation", 350, 10, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxDeviation->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxDeviation->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxDeviation->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxDeviation->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxDeviation->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxDeviation->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxDeviation->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticDeviationType = SXGUICrStatic("DeviationType:", 350, 30, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticDeviationType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticDeviationType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticDeviationType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticDeviationType->setTransparentTextBk(true);
	SXParticlesEditor::StaticDeviationType->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticDeviationType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticDeviationType->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ComboBoxDeviationType = SXGUICrComboBox(455, 25, 130, 75, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ComboBoxDeviationType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ComboBoxDeviationType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ComboBoxDeviationType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxDeviationType->setTransparentTextBk(true);
	SXParticlesEditor::ComboBoxDeviationType->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxDeviationType->addItem("rnd");
	SXParticlesEditor::ComboBoxDeviationType->addItem("along");
	SXParticlesEditor::ComboBoxDeviationType->addItem("wave");
	SXParticlesEditor::ComboBoxDeviationType->setSel(0);
	SXParticlesEditor::ComboBoxDeviationType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ComboBoxDeviationType->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticDeviationAmplitude = SXGUICrStatic("DeviationAmplitude:", 350, 50, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticDeviationAmplitude->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticDeviationAmplitude->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticDeviationAmplitude->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticDeviationAmplitude->setTransparentTextBk(true);
	SXParticlesEditor::StaticDeviationAmplitude->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticDeviationAmplitude->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticDeviationAmplitude->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticDeviationCoefAngle = SXGUICrStatic("DeviationCoefAngle:", 350, 70, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticDeviationCoefAngle->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticDeviationCoefAngle->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticDeviationCoefAngle->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticDeviationCoefAngle->setTransparentTextBk(true);
	SXParticlesEditor::StaticDeviationCoefAngle->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticDeviationCoefAngle->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticDeviationCoefAngle->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditDeviationAmplitude = SXGUICrEdit("0", 490, 50, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditDeviationAmplitude->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditDeviationAmplitude->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditDeviationAmplitude->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditDeviationAmplitude->setTransparentTextBk(true);
	SXParticlesEditor::EditDeviationAmplitude->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditDeviationAmplitude->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditDeviationAmplitude->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditDeviationAmplitude->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditDeviationAmplitude->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::EditDeviationCoefAngle = SXGUICrEdit("0", 490, 70, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditDeviationCoefAngle->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditDeviationCoefAngle->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditDeviationCoefAngle->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditDeviationCoefAngle->setTransparentTextBk(true);
	SXParticlesEditor::EditDeviationCoefAngle->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditDeviationCoefAngle->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditDeviationCoefAngle->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditDeviationCoefAngle->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditDeviationCoefAngle->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticDeviationAxis = SXGUICrStatic("DeviationAxis:", 590, 25, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticDeviationAxis->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticDeviationAxis->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticDeviationAxis->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticDeviationAxis->setTransparentTextBk(true);
	SXParticlesEditor::StaticDeviationAxis->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticDeviationAxis->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticDeviationAxis->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ComboBoxDeviationAxis = SXGUICrComboBox(665, 20, 130, 75, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ComboBoxDeviationAxis->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ComboBoxDeviationAxis->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ComboBoxDeviationAxis->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxDeviationAxis->setTransparentTextBk(true);
	SXParticlesEditor::ComboBoxDeviationAxis->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxDeviationAxis->addItem("x");
	SXParticlesEditor::ComboBoxDeviationAxis->addItem("y");
	SXParticlesEditor::ComboBoxDeviationAxis->addItem("z");
	SXParticlesEditor::ComboBoxDeviationAxis->setSel(0);
	SXParticlesEditor::ComboBoxDeviationAxis->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ComboBoxDeviationAxis->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticDeviationCountDelayMls = SXGUICrStatic("DeviationCountDelayMls:", 590, 45, 120, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticDeviationCountDelayMls->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticDeviationCountDelayMls->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticDeviationCountDelayMls->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticDeviationCountDelayMls->setTransparentTextBk(true);
	SXParticlesEditor::StaticDeviationCountDelayMls->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticDeviationCountDelayMls->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticDeviationCountDelayMls->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditDeviationCountDelayMls = SXGUICrEdit("0", 715, 45, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditDeviationCountDelayMls->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditDeviationCountDelayMls->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditDeviationCountDelayMls->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditDeviationCountDelayMls->setTransparentTextBk(true);
	SXParticlesEditor::EditDeviationCountDelayMls->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditDeviationCountDelayMls->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditDeviationCountDelayMls->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditDeviationCountDelayMls->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditDeviationCountDelayMls->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticDeviationCoefAngleDisp = SXGUICrStatic("DeviationCoefAngleDisp:", 350, 90, 120, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticDeviationCoefAngleDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticDeviationCoefAngleDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticDeviationCoefAngleDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticDeviationCoefAngleDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticDeviationCoefAngleDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticDeviationCoefAngleDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticDeviationCoefAngleDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditDeviationCoefAngleDisp = SXGUICrEdit("0", 490, 90, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditDeviationCoefAngleDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditDeviationCoefAngleDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditDeviationCoefAngleDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditDeviationCoefAngleDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditDeviationCoefAngleDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditDeviationCoefAngleDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditDeviationCoefAngleDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditDeviationCoefAngleDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditDeviationCoefAngleDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg = SXGUICrCheckBox("DeviationCoefAngleDispNeg", 350, 110, 160, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxDeviationTapX = SXGUICrCheckBox("DeviationTapX", 590, 65, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxDeviationTapX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxDeviationTapX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxDeviationTapX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxDeviationTapX->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxDeviationTapX->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxDeviationTapX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxDeviationTapX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxDeviationTapY = SXGUICrCheckBox("DeviationTapY", 590, 85, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxDeviationTapY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxDeviationTapY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxDeviationTapY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxDeviationTapY->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxDeviationTapY->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxDeviationTapY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxDeviationTapY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxDeviationTapZ = SXGUICrCheckBox("DeviationTapZ", 590, 105, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxDeviationTapZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxDeviationTapZ->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxDeviationTapZ->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxDeviationTapZ->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxDeviationTapZ->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxDeviationTapZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxDeviationTapZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	//}

	//spawn
	//{
	SXParticlesEditor::StaticSpawnPosType = SXGUICrStatic("SpawnPosType:", 5, 12, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSpawnPosType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSpawnPosType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSpawnPosType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSpawnPosType->setTransparentTextBk(true);
	SXParticlesEditor::StaticSpawnPosType->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSpawnPosType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSpawnPosType->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ComboBoxSpawnPosType = SXGUICrComboBox(90, 10, 140, 75, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ComboBoxSpawnPosType->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ComboBoxSpawnPosType->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::ComboBoxSpawnPosType->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxSpawnPosType->setTransparentTextBk(true);
	SXParticlesEditor::ComboBoxSpawnPosType->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::ComboBoxSpawnPosType->addItem("strictly");
	SXParticlesEditor::ComboBoxSpawnPosType->addItem("bound");
	SXParticlesEditor::ComboBoxSpawnPosType->setSel(0);
	SXParticlesEditor::ComboBoxSpawnPosType->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ComboBoxSpawnPosType->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticSpawnOrigin = SXGUICrStatic("SpawnOrigin:", 5, 40, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSpawnOrigin->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSpawnOrigin->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSpawnOrigin->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSpawnOrigin->setTransparentTextBk(true);
	SXParticlesEditor::StaticSpawnOrigin->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSpawnOrigin->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSpawnOrigin->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticSpawnOriginX = SXGUICrStatic("x:", 90, 40, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSpawnOriginX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSpawnOriginX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSpawnOriginX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSpawnOriginX->setTransparentTextBk(true);
	SXParticlesEditor::StaticSpawnOriginX->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSpawnOriginX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSpawnOriginX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnOriginX = SXGUICrEdit("0", 100, 40, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSpawnOriginX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSpawnOriginX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSpawnOriginX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnOriginX->setTransparentTextBk(true);
	SXParticlesEditor::EditSpawnOriginX->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnOriginX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSpawnOriginX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnOriginX->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSpawnOriginX->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticSpawnOriginY = SXGUICrStatic("y:", 180, 40, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSpawnOriginY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSpawnOriginY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSpawnOriginY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSpawnOriginY->setTransparentTextBk(true);
	SXParticlesEditor::StaticSpawnOriginY->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSpawnOriginY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSpawnOriginY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnOriginY = SXGUICrEdit("0", 190, 40, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSpawnOriginY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSpawnOriginY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSpawnOriginY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnOriginY->setTransparentTextBk(true);
	SXParticlesEditor::EditSpawnOriginY->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnOriginY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSpawnOriginY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnOriginY->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSpawnOriginY->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticSpawnOriginZ = SXGUICrStatic("z:", 270, 40, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSpawnOriginZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSpawnOriginZ->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSpawnOriginZ->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSpawnOriginZ->setTransparentTextBk(true);
	SXParticlesEditor::StaticSpawnOriginZ->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSpawnOriginZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSpawnOriginZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnOriginZ = SXGUICrEdit("0", 280, 40, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSpawnOriginZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSpawnOriginZ->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSpawnOriginZ->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnOriginZ->setTransparentTextBk(true);
	SXParticlesEditor::EditSpawnOriginZ->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnOriginZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSpawnOriginZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnOriginZ->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSpawnOriginZ->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticSpawnOriginDisp = SXGUICrStatic("SpawnOriginDisp:", 370, 40, 85, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSpawnOriginDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSpawnOriginDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSpawnOriginDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSpawnOriginDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticSpawnOriginDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSpawnOriginDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSpawnOriginDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnOriginDisp = SXGUICrEdit("0", 455, 40, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSpawnOriginDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSpawnOriginDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSpawnOriginDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnOriginDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditSpawnOriginDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnOriginDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSpawnOriginDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnOriginDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSpawnOriginDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::CheckBoxSpawnOriginDispXPos = SXGUICrCheckBox("x+", 90, 60, 30, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispXPos->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg = SXGUICrCheckBox("x-", 140, 60, 30, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispXNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos = SXGUICrCheckBox("y+", 180, 60, 30, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispYPos->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg = SXGUICrCheckBox("y-", 230, 60, 30, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispYNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos = SXGUICrCheckBox("z+", 270, 60, 30, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispZPos->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg = SXGUICrCheckBox("z-", 320, 59, 30, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxSpawnOriginDispZNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticSpawnNextTime = SXGUICrStatic("SpawnNextTime:", 5, 90, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSpawnNextTime->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSpawnNextTime->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSpawnNextTime->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSpawnNextTime->setTransparentTextBk(true);
	SXParticlesEditor::StaticSpawnNextTime->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSpawnNextTime->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSpawnNextTime->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnNextTime = SXGUICrEdit("0", 90, 90, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSpawnNextTime->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSpawnNextTime->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSpawnNextTime->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnNextTime->setTransparentTextBk(true);
	SXParticlesEditor::EditSpawnNextTime->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnNextTime->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSpawnNextTime->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnNextTime->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSpawnNextTime->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticSpawnNextTimeDisp = SXGUICrStatic("SpawnNextTimeDisp:", 175, 90, 105, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticSpawnNextTimeDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticSpawnNextTimeDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticSpawnNextTimeDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticSpawnNextTimeDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticSpawnNextTimeDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticSpawnNextTimeDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticSpawnNextTimeDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnNextTimeDisp = SXGUICrEdit("0", 280, 90, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditSpawnNextTimeDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditSpawnNextTimeDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditSpawnNextTimeDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnNextTimeDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditSpawnNextTimeDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditSpawnNextTimeDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditSpawnNextTimeDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditSpawnNextTimeDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditSpawnNextTimeDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	//}

	//tex
	//{
	SXParticlesEditor::StaticTexture = SXGUICrStatic("Texture:", 5, 10, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticTexture->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticTexture->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticTexture->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticTexture->setTransparentTextBk(true);
	SXParticlesEditor::StaticTexture->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticTexture->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticTexture->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTexture = SXGUICrEdit("0", 60, 10, 290, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditTexture->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditTexture->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditTexture->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditTexture->setTransparentTextBk(true);
	SXParticlesEditor::EditTexture->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditTexture->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditTexture->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditTexture->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditTexture->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);


	SXParticlesEditor::ButtonTextureSel = SXGUICrButton("...", 355, 10, 25, 15, SXGUI_BUTTON_IMAGE_NONE, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::ButtonTextureSel->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::ButtonTextureSel->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::ButtonTextureSel->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::ButtonTextureSel->addHandler(SXParticlesEditor_ButtonTextureSel_Click, WM_LBUTTONUP);

	SXParticlesEditor::StaticAnimTexCountCadrsX = SXGUICrStatic("AnimTexCountCadrsX:", 5, 35, 110, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAnimTexCountCadrsX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAnimTexCountCadrsX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAnimTexCountCadrsX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAnimTexCountCadrsX->setTransparentTextBk(true);
	SXParticlesEditor::StaticAnimTexCountCadrsX->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAnimTexCountCadrsX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAnimTexCountCadrsX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticAnimTexCountCadrsY = SXGUICrStatic("AnimTexCountCadrsY:", 200, 35, 110, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAnimTexCountCadrsY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAnimTexCountCadrsY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAnimTexCountCadrsY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAnimTexCountCadrsY->setTransparentTextBk(true);
	SXParticlesEditor::StaticAnimTexCountCadrsY->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAnimTexCountCadrsY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAnimTexCountCadrsY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexCountCadrsX = SXGUICrEdit("0", 115, 35, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAnimTexCountCadrsX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAnimTexCountCadrsX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAnimTexCountCadrsX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexCountCadrsX->setTransparentTextBk(true);
	SXParticlesEditor::EditAnimTexCountCadrsX->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexCountCadrsX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAnimTexCountCadrsX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexCountCadrsX->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAnimTexCountCadrsX->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::EditAnimTexCountCadrsY = SXGUICrEdit("0", 310, 35, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAnimTexCountCadrsY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAnimTexCountCadrsY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAnimTexCountCadrsY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexCountCadrsY->setTransparentTextBk(true);
	SXParticlesEditor::EditAnimTexCountCadrsY->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexCountCadrsY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAnimTexCountCadrsY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexCountCadrsY->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAnimTexCountCadrsY->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticAnimTexRate = SXGUICrStatic("AnimTexRate:", 5, 55, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAnimTexRate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAnimTexRate->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAnimTexRate->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAnimTexRate->setTransparentTextBk(true);
	SXParticlesEditor::StaticAnimTexRate->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAnimTexRate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAnimTexRate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexRate = SXGUICrEdit("0", 115, 55, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAnimTexRate->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAnimTexRate->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAnimTexRate->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexRate->setTransparentTextBk(true);
	SXParticlesEditor::EditAnimTexRate->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexRate->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAnimTexRate->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexRate->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAnimTexRate->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticAnimTexRateDisp = SXGUICrStatic("AnimTexRateDisp:", 200, 55, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAnimTexRateDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAnimTexRateDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAnimTexRateDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAnimTexRateDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticAnimTexRateDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAnimTexRateDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAnimTexRateDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexRateDisp = SXGUICrEdit("0", 310, 55, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAnimTexRateDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAnimTexRateDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAnimTexRateDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexRateDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditAnimTexRateDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexRateDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAnimTexRateDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexRateDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAnimTexRateDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticAnimTexStartCadr = SXGUICrStatic("AnimTexStartCadr:", 5, 75, 100, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAnimTexStartCadr->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAnimTexStartCadr->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAnimTexStartCadr->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAnimTexStartCadr->setTransparentTextBk(true);
	SXParticlesEditor::StaticAnimTexStartCadr->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAnimTexStartCadr->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAnimTexStartCadr->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexStartCadr = SXGUICrEdit("0", 115, 75, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAnimTexStartCadr->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAnimTexStartCadr->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAnimTexStartCadr->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexStartCadr->setTransparentTextBk(true);
	SXParticlesEditor::EditAnimTexStartCadr->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexStartCadr->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAnimTexStartCadr->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexStartCadr->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAnimTexStartCadr->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticAnimTexStartCadrDisp = SXGUICrStatic("AnimTexStartCadrDisp:", 200, 75, 110, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAnimTexStartCadrDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAnimTexStartCadrDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAnimTexStartCadrDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAnimTexStartCadrDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticAnimTexStartCadrDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAnimTexStartCadrDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAnimTexStartCadrDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexStartCadrDisp = SXGUICrEdit("0", 310, 75, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAnimTexStartCadrDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAnimTexStartCadrDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAnimTexStartCadrDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexStartCadrDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditAnimTexStartCadrDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAnimTexStartCadrDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAnimTexStartCadrDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAnimTexStartCadrDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAnimTexStartCadrDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	//}

	//velocity/acceleration
	//{
	SXParticlesEditor::StaticVelocity = SXGUICrStatic("Velocity:", 5, 15, 50, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticVelocity->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticVelocity->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticVelocity->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticVelocity->setTransparentTextBk(true);
	SXParticlesEditor::StaticVelocity->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticVelocity->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticVelocity->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticVelocityX = SXGUICrStatic("x:", 80, 15, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticVelocityX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticVelocityX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticVelocityX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticVelocityX->setTransparentTextBk(true);
	SXParticlesEditor::StaticVelocityX->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticVelocityX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticVelocityX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditVelocityX = SXGUICrEdit("0", 90, 15, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditVelocityX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditVelocityX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditVelocityX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditVelocityX->setTransparentTextBk(true);
	SXParticlesEditor::EditVelocityX->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditVelocityX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditVelocityX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditVelocityX->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditVelocityX->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticVelocityY = SXGUICrStatic("y:", 170, 15, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticVelocityY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticVelocityY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticVelocityY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticVelocityY->setTransparentTextBk(true);
	SXParticlesEditor::StaticVelocityY->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticVelocityY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticVelocityY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditVelocityY = SXGUICrEdit("0", 180, 15, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditVelocityY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditVelocityY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditVelocityY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditVelocityY->setTransparentTextBk(true);
	SXParticlesEditor::EditVelocityY->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditVelocityY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditVelocityY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditVelocityY->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditVelocityY->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::CheckBoxVelocityDispXNeg = SXGUICrCheckBox("XDispNeg", 80, 35, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxVelocityDispXNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticVelocityZ = SXGUICrStatic("z:", 260, 15, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticVelocityZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticVelocityZ->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticVelocityZ->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticVelocityZ->setTransparentTextBk(true);
	SXParticlesEditor::StaticVelocityZ->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticVelocityZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticVelocityZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditVelocityZ = SXGUICrEdit("0", 270, 15, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditVelocityZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditVelocityZ->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditVelocityZ->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditVelocityZ->setTransparentTextBk(true);
	SXParticlesEditor::EditVelocityZ->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditVelocityZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditVelocityZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditVelocityZ->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditVelocityZ->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticVelocityDisp = SXGUICrStatic("Dispersion:", 355, 15, 60, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticVelocityDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticVelocityDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticVelocityDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticVelocityDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticVelocityDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticVelocityDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticVelocityDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditVelocityDisp = SXGUICrEdit("0", 420, 15, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditVelocityDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditVelocityDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditVelocityDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditVelocityDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditVelocityDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditVelocityDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditVelocityDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditVelocityDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditVelocityDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::CheckBoxVelocityDispYNeg = SXGUICrCheckBox("YDispNeg", 170, 35, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxVelocityDispYNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxVelocityDispZNeg = SXGUICrCheckBox("ZDispNeg", 260, 35, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxVelocityDispZNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticAcceleration = SXGUICrStatic("Acceleration:", 5, 65, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAcceleration->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAcceleration->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAcceleration->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAcceleration->setTransparentTextBk(true);
	SXParticlesEditor::StaticAcceleration->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAcceleration->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAcceleration->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::StaticAccelerationX = SXGUICrStatic("x:", 80, 65, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAccelerationX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAccelerationX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAccelerationX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAccelerationX->setTransparentTextBk(true);
	SXParticlesEditor::StaticAccelerationX->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAccelerationX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAccelerationX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAccelerationX = SXGUICrEdit("0", 90, 65, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAccelerationX->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAccelerationX->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAccelerationX->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAccelerationX->setTransparentTextBk(true);
	SXParticlesEditor::EditAccelerationX->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAccelerationX->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAccelerationX->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAccelerationX->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAccelerationX->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticAccelerationY = SXGUICrStatic("y:", 170, 65, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAccelerationY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAccelerationY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAccelerationY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAccelerationY->setTransparentTextBk(true);
	SXParticlesEditor::StaticAccelerationY->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAccelerationY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAccelerationY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAccelerationY = SXGUICrEdit("0", 180, 65, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAccelerationY->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAccelerationY->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAccelerationY->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAccelerationY->setTransparentTextBk(true);
	SXParticlesEditor::EditAccelerationY->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAccelerationY->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAccelerationY->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAccelerationY->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAccelerationY->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticAccelerationZ = SXGUICrStatic("z:", 260, 65, 10, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAccelerationZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAccelerationZ->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAccelerationZ->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAccelerationZ->setTransparentTextBk(true);
	SXParticlesEditor::StaticAccelerationZ->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAccelerationZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAccelerationZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAccelerationZ = SXGUICrEdit("0", 270, 65, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAccelerationZ->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAccelerationZ->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAccelerationZ->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAccelerationZ->setTransparentTextBk(true);
	SXParticlesEditor::EditAccelerationZ->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAccelerationZ->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAccelerationZ->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAccelerationZ->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAccelerationZ->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::StaticAccelerationDisp = SXGUICrStatic("Dispersion:", 355, 65, 60, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::StaticAccelerationDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::StaticAccelerationDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::StaticAccelerationDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::StaticAccelerationDisp->setTransparentTextBk(true);
	SXParticlesEditor::StaticAccelerationDisp->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::StaticAccelerationDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::StaticAccelerationDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAccelerationDisp = SXGUICrEdit("0", 420, 65, 70, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0);
	SXParticlesEditor::EditAccelerationDisp->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::EditAccelerationDisp->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::EditAccelerationDisp->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::EditAccelerationDisp->setTransparentTextBk(true);
	SXParticlesEditor::EditAccelerationDisp->setColorBrush(RGB(255, 255, 255));
	SXParticlesEditor::EditAccelerationDisp->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::EditAccelerationDisp->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::EditAccelerationDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KEYDOWN, VK_RETURN, 1, 0, 0, 0);
	SXParticlesEditor::EditAccelerationDisp->addHandler(SXParticlesEditor_Edits_Enter, WM_KILLFOCUS);

	SXParticlesEditor::CheckBoxAccelerationDispXNeg = SXGUICrCheckBox("XDispNeg", 80, 85, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxAccelerationDispXNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxAccelerationDispYNeg = SXGUICrCheckBox("YDispNeg", 170, 85, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxAccelerationDispYNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	SXParticlesEditor::CheckBoxAccelerationDispZNeg = SXGUICrCheckBox("ZDispNeg", 260, 85, 80, 15, SXParticlesEditor::GroupBoxData2->getHWND(), 0, 0, false);
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setFont("MS Shell Dlg", -11, 0, 400, 0, 0, 0);
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setColorText(RGB(0, 0, 0));
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setColorTextBk(RGB(255, 255, 255));
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setTransparentTextBk(true);
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setColorBrush(RGB(220, 220, 220));
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setFollowParentSide(SXGUI_SIDE_LEFT, true);
	SXParticlesEditor::CheckBoxAccelerationDispZNeg->setFollowParentSide(SXGUI_SIDE_TOP, true);
	//}
}

void SXParticlesEditor::DeleteAllElements()
{
	mem_release(SXParticlesEditor::WindowRender);
	mem_release(SXParticlesEditor::ToolBar1);
	mem_release(SXParticlesEditor::ListBoxEffects);
	mem_release(SXParticlesEditor::ListBoxEmitters);
	mem_release(SXParticlesEditor::StaticEffects);
	mem_release(SXParticlesEditor::ButtonEffectsCreate);
	mem_release(SXParticlesEditor::StaticEmitters);
	//mem_release(SXParticlesEditor::ButtonEffectsEdit);
	mem_release(SXParticlesEditor::ButtonEffectsDelete);
	mem_release(SXParticlesEditor::ButtonEmittersCreate);
	mem_release(SXParticlesEditor::ButtonEmittersBasis);
	mem_release(SXParticlesEditor::ButtonEmittersDelete);
	mem_release(SXParticlesEditor::ButtonBase);
	mem_release(SXParticlesEditor::ButtonTextureAnimTex);
	mem_release(SXParticlesEditor::ButtonSpawn);
	mem_release(SXParticlesEditor::ButtonBound);
	mem_release(SXParticlesEditor::ButtonCharacters);
	mem_release(SXParticlesEditor::ButtonVelocityAcceleration);
	mem_release(SXParticlesEditor::StaticEffectsCount);
	mem_release(SXParticlesEditor::StaticEmittersCount);

	mem_release(SXParticlesEditor::StaticName);
	mem_release(SXParticlesEditor::EditName);
	mem_release(SXParticlesEditor::ButtonEmitterCreate);
	mem_release(SXParticlesEditor::StaticCount);
	mem_release(SXParticlesEditor::EditCount);
	mem_release(StaticColorCoef);
	mem_release(EditColorCoef);
	mem_release(SXParticlesEditor::StaticReCreateCount);
	mem_release(SXParticlesEditor::EditReCreateCount);
	mem_release(SXParticlesEditor::StaticSoftCoef);
	mem_release(SXParticlesEditor::EditSoftCoef);
	mem_release(SXParticlesEditor::StaticRefractionCoef);
	mem_release(SXParticlesEditor::EditRefractionCoef);
	mem_release(SXParticlesEditor::StaticTransparencyCoef);
	mem_release(SXParticlesEditor::EditTransparencyCoef);
	mem_release(SXParticlesEditor::CheckBoxLighting);
	mem_release(SXParticlesEditor::StaticFigureType);
	mem_release(SXParticlesEditor::ComboBoxFigureType);
	mem_release(SXParticlesEditor::StaticFigureCountQuads);
	mem_release(SXParticlesEditor::EditFigureCountQuads);
	mem_release(SXParticlesEditor::CheckBoxFigureRotRand);
	mem_release(SXParticlesEditor::CheckBoxFigureTapX);
	mem_release(SXParticlesEditor::CheckBoxFigureTapY);
	mem_release(SXParticlesEditor::CheckBoxFigureTapZ);
	mem_release(StaticAlphaBlendType);
	mem_release(ComboBoxAlphaBlendType);
	mem_release(SXParticlesEditor::StaticTimeLife);
	mem_release(SXParticlesEditor::EditTimeLife);
	mem_release(SXParticlesEditor::StaticTimeLifeDisp);
	mem_release(SXParticlesEditor::EditTimeLifeDisp);
	mem_release(SXParticlesEditor::StaticAlphaAgeDepend);
	mem_release(SXParticlesEditor::ComboBoxAlphaDependAge);
	mem_release(SXParticlesEditor::StaticSize);
	mem_release(SXParticlesEditor::StaticSizeX);
	mem_release(SXParticlesEditor::EditSizeX);
	mem_release(SXParticlesEditor::StaticSizeY);
	mem_release(SXParticlesEditor::EditSizeY);
	mem_release(SXParticlesEditor::StaticSizeDisp);
	mem_release(SXParticlesEditor::EditSizeDisp);
	mem_release(SXParticlesEditor::StaticSizeDependAge);
	mem_release(SXParticlesEditor::ComboBoxSizeDependAge);
	mem_release(SXParticlesEditor::CheckBoxCollisionDelete);

	mem_release(SXParticlesEditor::StaticBoundType);
	mem_release(SXParticlesEditor::ComboBoxBoundType);
	mem_release(SXParticlesEditor::StaticBoundVec1);
	mem_release(SXParticlesEditor::StaticBoundVec1X);
	mem_release(SXParticlesEditor::EditBoundVec1X);
	mem_release(SXParticlesEditor::StaticBoundVec1Y);
	mem_release(SXParticlesEditor::EditBoundVec1Y);
	mem_release(SXParticlesEditor::StaticBoundVec1Z);
	mem_release(SXParticlesEditor::EditBoundVec1Z);
	mem_release(SXParticlesEditor::StaticBoundVec1W);
	mem_release(SXParticlesEditor::EditBoundVec1W);
	mem_release(SXParticlesEditor::StaticBoundVec2);
	mem_release(SXParticlesEditor::StaticBoundVec2X);
	mem_release(SXParticlesEditor::EditBoundVec2X);
	mem_release(SXParticlesEditor::StaticBoundVec2Y);
	mem_release(SXParticlesEditor::EditBoundVec2Y);
	mem_release(SXParticlesEditor::StaticBoundVec2Z);
	mem_release(SXParticlesEditor::EditBoundVec2Z);
	mem_release(SXParticlesEditor::StaticBoundVec2W);
	mem_release(SXParticlesEditor::EditBoundVec2W);
	
	mem_release(SXParticlesEditor::CheckBoxCircle);
	mem_release(SXParticlesEditor::StaticCircleAxis);
	mem_release(SXParticlesEditor::ComboBoxCircleAxis);
	mem_release(SXParticlesEditor::StaticCircleAngle);
	mem_release(SXParticlesEditor::EditCircleAngle);
	mem_release(SXParticlesEditor::StaticCircleAngleDisp);
	mem_release(SXParticlesEditor::EditCircleAngleDisp);
	mem_release(SXParticlesEditor::CheckBoxRotate);
	mem_release(SXParticlesEditor::StaticRotateAngle);
	mem_release(SXParticlesEditor::EditRotateAngle);
	mem_release(SXParticlesEditor::StaticRotateAngleDisp);
	mem_release(SXParticlesEditor::CheckBoxCircleAngleDispNeg);
	mem_release(SXParticlesEditor::EditRotateAngleDisp);
	mem_release(SXParticlesEditor::CheckBoxRotateAngleDispNeg);
	mem_release(SXParticlesEditor::CheckBoxDeviation);
	mem_release(SXParticlesEditor::StaticDeviationType);
	mem_release(SXParticlesEditor::ComboBoxDeviationType);
	mem_release(SXParticlesEditor::StaticDeviationAmplitude);
	mem_release(SXParticlesEditor::StaticDeviationCoefAngle);
	mem_release(SXParticlesEditor::EditDeviationAmplitude);
	mem_release(SXParticlesEditor::EditDeviationCoefAngle);
	mem_release(SXParticlesEditor::StaticDeviationAxis);
	mem_release(SXParticlesEditor::ComboBoxDeviationAxis);
	mem_release(SXParticlesEditor::StaticDeviationCountDelayMls);
	mem_release(SXParticlesEditor::EditDeviationCountDelayMls);
	mem_release(SXParticlesEditor::StaticDeviationCoefAngleDisp);
	mem_release(SXParticlesEditor::EditDeviationCoefAngleDisp);
	mem_release(SXParticlesEditor::CheckBoxDeviationCoefAngleDispNeg);
	mem_release(SXParticlesEditor::CheckBoxDeviationTapX);
	mem_release(SXParticlesEditor::CheckBoxDeviationTapY);
	mem_release(SXParticlesEditor::CheckBoxDeviationTapZ);

	mem_release(SXParticlesEditor::StaticSpawnPosType);
	mem_release(SXParticlesEditor::ComboBoxSpawnPosType);
	mem_release(SXParticlesEditor::StaticSpawnOrigin);
	mem_release(SXParticlesEditor::StaticSpawnOriginX);
	mem_release(SXParticlesEditor::EditSpawnOriginX);
	mem_release(SXParticlesEditor::StaticSpawnOriginY);
	mem_release(SXParticlesEditor::EditSpawnOriginY);
	mem_release(SXParticlesEditor::StaticSpawnOriginZ);
	mem_release(SXParticlesEditor::EditSpawnOriginZ);
	mem_release(SXParticlesEditor::StaticSpawnOriginDisp);
	mem_release(SXParticlesEditor::EditSpawnOriginDisp);
	mem_release(SXParticlesEditor::CheckBoxSpawnOriginDispXPos);
	mem_release(SXParticlesEditor::CheckBoxSpawnOriginDispXNeg);
	mem_release(SXParticlesEditor::CheckBoxSpawnOriginDispYPos);
	mem_release(SXParticlesEditor::CheckBoxSpawnOriginDispYNeg);
	mem_release(SXParticlesEditor::CheckBoxSpawnOriginDispZPos);
	mem_release(SXParticlesEditor::CheckBoxSpawnOriginDispZNeg);
	mem_release(SXParticlesEditor::StaticSpawnNextTime);
	mem_release(SXParticlesEditor::EditSpawnNextTime);
	mem_release(SXParticlesEditor::StaticSpawnNextTimeDisp);
	mem_release(SXParticlesEditor::EditSpawnNextTimeDisp);

	mem_release(SXParticlesEditor::StaticTexture);
	mem_release(SXParticlesEditor::EditTexture);
	mem_release(SXParticlesEditor::ButtonTextureSel);
	mem_release(SXParticlesEditor::StaticAnimTexCountCadrsX);
	mem_release(SXParticlesEditor::StaticAnimTexCountCadrsY);
	mem_release(SXParticlesEditor::EditAnimTexCountCadrsX);
	mem_release(SXParticlesEditor::EditAnimTexCountCadrsY);
	mem_release(SXParticlesEditor::StaticAnimTexRate);
	mem_release(SXParticlesEditor::EditAnimTexRate);
	mem_release(SXParticlesEditor::StaticAnimTexRateDisp);
	mem_release(SXParticlesEditor::EditAnimTexRateDisp);
	mem_release(SXParticlesEditor::StaticAnimTexStartCadr);
	mem_release(SXParticlesEditor::EditAnimTexStartCadr);
	mem_release(SXParticlesEditor::StaticAnimTexStartCadrDisp);
	mem_release(SXParticlesEditor::EditAnimTexStartCadrDisp);

	mem_release(SXParticlesEditor::StaticVelocity);
	mem_release(SXParticlesEditor::StaticVelocityX);
	mem_release(SXParticlesEditor::EditVelocityX);
	mem_release(SXParticlesEditor::StaticVelocityY);
	mem_release(SXParticlesEditor::EditVelocityY);
	mem_release(SXParticlesEditor::CheckBoxVelocityDispXNeg);
	mem_release(SXParticlesEditor::StaticVelocityZ);
	mem_release(SXParticlesEditor::EditVelocityZ);
	mem_release(SXParticlesEditor::StaticVelocityDisp);
	mem_release(SXParticlesEditor::EditVelocityDisp);
	mem_release(SXParticlesEditor::CheckBoxVelocityDispYNeg);
	mem_release(SXParticlesEditor::CheckBoxVelocityDispZNeg);
	mem_release(SXParticlesEditor::StaticAcceleration);
	mem_release(SXParticlesEditor::StaticAccelerationX);
	mem_release(SXParticlesEditor::EditAccelerationX);
	mem_release(SXParticlesEditor::StaticAccelerationY);
	mem_release(SXParticlesEditor::EditAccelerationY);
	mem_release(SXParticlesEditor::StaticAccelerationZ);
	mem_release(SXParticlesEditor::EditAccelerationZ);
	mem_release(SXParticlesEditor::StaticAccelerationDisp);
	mem_release(SXParticlesEditor::EditAccelerationDisp);
	mem_release(SXParticlesEditor::CheckBoxAccelerationDispXNeg);
	mem_release(SXParticlesEditor::CheckBoxAccelerationDispYNeg);
	mem_release(SXParticlesEditor::CheckBoxAccelerationDispZNeg);

	mem_release(SXParticlesEditor::JobWindow);
}

//##########################################################################

void SXParticlesEditor::ParticlesEditorUpdate(DWORD timeDelta)
{
	bool whyplay = false;
	for (int i = 0; i < SPE_EffectCountGet(); ++i)
	{
		if (SPE_EffectEnableGet(SPE_EffectIdOfKey(i)))
		{
			SXParticlesEditor::CheckBoxTBPlay->setCheck(true);
			SXParticlesEditor::CheckBoxTBPause->setCheck(false);
			SXParticlesEditor::CheckBoxTBStop->setCheck(false);
			whyplay = true;
			break;
		}
	}

	if (!whyplay)
	{
		SXParticlesEditor::CheckBoxTBPlay->setCheck(false);
		SXParticlesEditor::CheckBoxTBPause->setCheck(false);
		SXParticlesEditor::CheckBoxTBStop->setCheck(true);
	}

	int emitters_count = 0;
	int emitters_all_count = 0;
	int particles_life_count = 0;
	int particles_all_count = 0;

	if (SXParticlesEditor::SelEffID >= 0)
	{
		emitters_all_count = SPE_EmitterSCountGet(SXParticlesEditor::SelEffID);
		if (SXParticlesEditor::SelEmitterID < 0)
		{
			for (int k = 0; k < SPE_EmitterSCountGet(SXParticlesEditor::SelEffID); ++k)
			{
				if (SPE_EmitterEnableGet(SXParticlesEditor::SelEffID, k))
					++emitters_count;

				particles_all_count += SPE_EmitterCountGet(SXParticlesEditor::SelEffID, k);
				particles_life_count += SPE_EmitterCountLifeGet(SXParticlesEditor::SelEffID, k);
			}
		}

		if (SXParticlesEditor::SelEmitterID >= 0)
		{
			emitters_count = SPE_EmitterEnableGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID);
			particles_all_count += SPE_EmitterCountGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID);
			particles_life_count += SPE_EmitterCountLifeGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID);
		}
	}

	char ttext[256];

	sprintf(ttext, "Playing emitters: %d/%d", emitters_all_count, emitters_count);
	SXParticlesEditor::StatusBar1->setPartText(0, ttext);

	sprintf(ttext, "Living particles: %d/%d", particles_all_count, particles_life_count);
	SXParticlesEditor::StatusBar1->setPartText(1, ttext);


	IDirect3DDevice9 *pDXdevice = SGCore_GetDXDevice();

	pDXdevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (SXParticlesEditor::canRenderBound && SXParticlesEditor::SelEffID != -1 && SXParticlesEditor::SelEmitterID != -1 && SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundType) != PARTICLESTYPE_BOUND_NONE)
	{
		pDXdevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		pDXdevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

		ParticlesData* pdata = SPE_EmitterGetData(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID);
		pDXdevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		if (pdata->BoundType == PARTICLESTYPE_BOUND_BOX)
		{
			pDXdevice->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)(SMMatrixScaling(float3(pdata->BoundVec2 - pdata->BoundVec1)) * SMMatrixTranslation(float3(pdata->BoundVec2 + pdata->BoundVec1)*0.5f)));
			SXParticlesEditor::FigureBox->DrawSubset(0);
		}
		else if (pdata->BoundType == PARTICLESTYPE_BOUND_SPHERE)
		{
			pDXdevice->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)(SMMatrixScaling(pdata->BoundVec1.w, pdata->BoundVec1.w, pdata->BoundVec1.w) * SMMatrixTranslation(float3(pdata->BoundVec1))));
			SXParticlesEditor::FigureSphere->DrawSubset(0);
		}
		else if (pdata->BoundType == PARTICLESTYPE_BOUND_CONE)
		{
			if (SXParticlesEditor::FigureConeParam.x != pdata->BoundVec2.w || SXParticlesEditor::FigureConeParam.y != pdata->BoundVec1.w || SXParticlesEditor::FigureConeParam.z != pdata->BoundVec2.y - pdata->BoundVec1.y)
			{
				SXParticlesEditor::FigureConeParam.x = pdata->BoundVec2.w;
				SXParticlesEditor::FigureConeParam.y = pdata->BoundVec1.w;
				SXParticlesEditor::FigureConeParam.z = pdata->BoundVec2.y - pdata->BoundVec1.y;

				SGCore_FCreateCone(SXParticlesEditor::FigureConeParam.x, SXParticlesEditor::FigureConeParam.y, SXParticlesEditor::FigureConeParam.z, &SXParticlesEditor::FigureCone, 20);
			}

			pDXdevice->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)SMMatrixTranslation(pdata->BoundVec1.x, pdata->BoundVec2.y, pdata->BoundVec1.z));
			SXParticlesEditor::FigureCone->DrawSubset(0);
		}

		pDXdevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		pDXdevice->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)SMMatrixIdentity());
	}
}

//##########################################################################

void SXParticlesEditor::PEcreateData()
{
	D3DXCreateBox(SGCore_GetDXDevice(), 1, 1, 1, &SXParticlesEditor::FigureBox, 0);
	D3DXCreateSphere(SGCore_GetDXDevice(), 1, 20, 20, &SXParticlesEditor::FigureSphere, 0);

	SXParticlesEditor::FigureConeParam.x = 1;
	SXParticlesEditor::FigureConeParam.y = 0.1;
	SXParticlesEditor::FigureConeParam.z = 1;

	SGCore_FCreateCone(SXParticlesEditor::FigureConeParam.x, SXParticlesEditor::FigureConeParam.y, SXParticlesEditor::FigureConeParam.z, &SXParticlesEditor::FigureCone, 20);
}

void SXParticlesEditor::PEdeleteData()
{
	mem_release(SXParticlesEditor::FigureBox);
	mem_release(SXParticlesEditor::FigureSphere);
}