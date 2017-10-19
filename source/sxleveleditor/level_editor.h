
#ifndef __LEVEL_EDITOR_H
#define __LEVEL_EDITOR_H

#define SX_LE_MMENU_WEATHER_BEGIN_ID 50001

#define MAINWIN_SIZE_X	820
#define MAINWIN_SIZE_Y	690

#include <common/string.h>
#include <common/array.h>

#include <sxguiwinapi/sxgui.h>
#include <geom/sxgeom.h>
#include <game/sxgame.h>
#include <render/sxrender.h>
#include <aigrid/sxaigrid.h>
#include <level/sxlevel.h>
#include <input/sxinput.h>
#include <sxleveleditor/resource.h>

#include <editors_utils/axes_helper.h>

#include "aigrid_callback.h"
#include "game_callback.h"
#include "green_callback.h"
#include "model_callback.h"
#include "common_callback.h"

namespace SXLevelEditor
{
	extern ISXGUIBaseWnd* JobWindow;
	extern ISXGUIMenu* MainMenu;
	extern ISXGUIBaseWnd* RenderWindow;

	extern ISXGUIToolBar* ToolBar1;
	extern ISXGUIButton* ButtonTBNew;
	extern ISXGUIButton* ButtonTBOpen;
	extern ISXGUIButton* ButtonTBSave;
	extern ISXGUIButton* ButtonTBSaveAs;

	extern ISXGUICheckBox* CheckBoxTBArrow;
	extern ISXGUICheckBox* CheckBoxTBPos;
	extern ISXGUICheckBox* CheckBoxTBRot;
	extern ISXGUICheckBox* CheckBoxTBScale;

	extern ISXGUICheckBox* CheckBoxTBGrid;
	extern ISXGUICheckBox* CheckBoxTBAxes;

	extern ISXGUICheckBox* CheckBoxTBRColor;
	extern ISXGUICheckBox* CheckBoxTBRNormal;
	extern ISXGUICheckBox* CheckBoxTBRParam;
	extern ISXGUICheckBox* CheckBoxTBRAmDiff;
	extern ISXGUICheckBox* CheckBoxTBRSpecular;
	extern ISXGUICheckBox* CheckBoxTBRLighting;

	extern ISXGUICheckBox* CheckBoxTBSelS;
	extern ISXGUICheckBox* CheckBoxTBSelZTest;
	extern ISXGUICheckBox* CheckBoxTBSelMesh;
	extern ISXGUICheckBox* CheckBoxTBSelCullBack;

	extern ISXGUICheckBox* CheckBoxTBAIGBound;
	extern ISXGUICheckBox* CheckBoxTBAIGQuad;
	extern ISXGUICheckBox* CheckBoxTBAIGGraphPoint;

	extern ISXGUICheckBox* CheckBoxTBLevelType;
	extern ISXGUICheckBox* CheckBoxTBGLightEnable;

	extern ISXGUIGroupBox* GroupBoxList;
	extern ISXGUIGroupBox* GroupBoxData;
	extern ISXGUIListBox* ListBoxList;
	extern ISXGUIStatic* StaticListTextCount;
	extern ISXGUIStatic* StaticListValCount;

	extern ISXGUIButton* ButtonDelete;

	extern ISXGUIButton* ButtonGeometryOpen;
	extern ISXGUIButton* ButtonGreenOpen;
	extern ISXGUIButton* ButtonGameObjectOpen;
	extern ISXGUIButton* ButtonAIGridOpen;


	//model
	//{{
	extern ISXGUIStatic* StaticGeomName;
	extern ISXGUIEdit* EditGeomName;

	extern ISXGUIStatic* StaticGeomModel;
	extern ISXGUIEdit* EditGeomModel;
	extern ISXGUIButton* ButtonGeomModel;

	extern ISXGUIStatic* StaticGeomLod1;
	extern ISXGUIEdit* EditGeomLod1;
	extern ISXGUIButton* ButtonGeomLod1;
	
	extern ISXGUIStatic* StaticGeomPos;
	extern ISXGUIEdit* EditGeomPosX;
	extern ISXGUIEdit* EditGeomPosY;
	extern ISXGUIEdit* EditGeomPosZ;
	extern ISXGUIRadioButton* RadioButtonGeomPosX;
	extern ISXGUIRadioButton* RadioButtonGeomPosY;
	extern ISXGUIRadioButton* RadioButtonGeomPosZ;
	extern ISXGUIStatic* StaticGeomRot;
	extern ISXGUIEdit* EditGeomRotX;
	extern ISXGUIEdit* EditGeomRotY;
	extern ISXGUIEdit* EditGeomRotZ;
	extern ISXGUIRadioButton* RadioButtonGeomRotX;
	extern ISXGUIRadioButton* RadioButtonGeomRotY;
	extern ISXGUIRadioButton* RadioButtonGeomRotZ;
	extern ISXGUIStatic* StaticGeomScale;
	extern ISXGUIEdit* EditGeomScaleX;
	extern ISXGUIEdit* EditGeomScaleY;
	extern ISXGUIEdit* EditGeomScaleZ;
	extern ISXGUIRadioButton* RadioButtonGeomScaleX;
	extern ISXGUIRadioButton* RadioButtonGeomScaleY;
	extern ISXGUIRadioButton* RadioButtonGeomScaleZ;
	extern ISXGUIButton* ButtonGeomFinish;
	//}}

	//Green
	//{{
	extern ISXGUIStatic* StaticGreenName;
	extern ISXGUIEdit* EditGreenName;

	extern ISXGUIStatic* StaticGreenModel;
	extern ISXGUIEdit* EditGreenModel;
	extern ISXGUIButton* ButtonGreenModel;

	extern ISXGUIStatic* StaticGreenLod1;
	extern ISXGUIEdit* EditGreenLod1;
	extern ISXGUIButton* ButtonGreenLod1;

	extern ISXGUIStatic* StaticGreenLod2;
	extern ISXGUIEdit* EditGreenLod2;
	extern ISXGUIButton* ButtonGreenLod2;

	extern ISXGUIStatic* StaticGreenMask;
	extern ISXGUIEdit* EditGreenMask;
	extern ISXGUIButton* ButtonGreenMask;
	
	extern ISXGUIStatic* StaticGreenNav;
	extern ISXGUIEdit* EditGreenNav;
	extern ISXGUIButton* ButtonGreenNav;

	extern ISXGUITrackBar* TrackBarGreenDensity;
	extern ISXGUIButton* ButtonGreenGenerate;
	extern ISXGUIStatic* StaticGreenDensityText;
	extern ISXGUIStatic* StaticGreenDensityVal;

	extern ISXGUIEdit* EditGreenSelX;
	extern ISXGUIEdit* EditGreenSelY;
	extern ISXGUIEdit* EditGreenSelZ;
	extern ISXGUIRadioButton* RadioButtonGreenSelX;
	extern ISXGUIRadioButton* RadioButtonGreenSelY;
	extern ISXGUIRadioButton* RadioButtonGreenSelZ;
	extern ISXGUIComboBox* ComboBoxGreenSel;
	extern ISXGUIStatic* StaticGreenSelX;
	extern ISXGUIStatic* StaticGreenSelY;
	extern ISXGUIStatic* StaticGreenSelZ;
	//}}

	//game object
	//{
	extern ISXGUIStatic* StaticGameClass;
	extern ISXGUIComboBox* ComboBoxGameClass;
	extern ISXGUIButton* ButtonGameTab;

	extern int GameTabVal;

	extern ISXGUIListView* ListViewGameClass;
	extern ISXGUIComboBox* ComboBoxGameValue;
	extern ISXGUIEdit* EditGameValue;
	extern ISXGUIButton* ButtonGameValue;
	extern ISXGUIStatic* StaticGameHelp;
	extern ISXGUIMemo* MemoGameHelp;
	extern ISXGUIButton* ButtonGameCreate;

	extern ISXGUIListView* ListViewGameConnections;
	extern ISXGUIStatic* StaticGameConnectionsEvent;
	extern ISXGUIComboBox* ComboBoxGameConnectionsEvent;
	extern ISXGUIStatic* StaticGameConnectionsName;
	extern ISXGUIEdit* EditGameConnectionsName;
	extern ISXGUIStatic* StaticGameConnectionsAction;
	extern ISXGUIComboBox* ComboBoxGameConnectionsAction;
	extern ISXGUIStatic* StaticGameConnectionsDelay;
	extern ISXGUIEdit* EditGameConnectionsDelay;
	extern ISXGUIStatic* StaticGameConnectionsParameter;
	extern ISXGUIEdit* EditGameConnectionsParameter;
	//}

	//aigrid
	//{
	extern ISXGUIButton* ButtonAIQuadsDelSel;
	extern ISXGUIButton* ButtonAIGridGen;
	extern ISXGUIButton* ButtonAIGridClear;
	extern ISXGUIButton* ButtonAIClearAll;
	extern ISXGUIStatic* StatiAIBBDimensions;
	extern ISXGUIStatic* StaticAIBBDimensionsWidth;
	extern ISXGUIEdit* EditAIBBDimensionsWidth;
	extern ISXGUIStatic* StaticAIBBDimensionsHeight;
	extern ISXGUIEdit* EditAIBBDimensionsHeight;
	extern ISXGUIStatic* StaticAIBBDimensionsDepth;
	extern ISXGUIEdit* EditAIBBDimensionsDepth;
	extern ISXGUIStatic* StaticAIBBPos;
	extern ISXGUIStatic* StaticAIBBPosX;
	extern ISXGUIEdit* EditAIBBPosX;
	extern ISXGUIStatic* StaticAIBBPosY;
	extern ISXGUIEdit* EditAIBBPosY;
	extern ISXGUIStatic* StaticAIBBPosZ;
	extern ISXGUIEdit* EditAIBBPosZ;
	extern ISXGUIButton* ButtonAIBBFinish;
	extern ISXGUIButton* ButtonAIGPGen;
	extern ISXGUIButton* ButtonAIGPClear;
	extern ISXGUIRadioButton* RadioButtonAIGPAdd;
	extern ISXGUIRadioButton* RadioButtonAIGPDel;
	extern ISXGUIRadioButton* RadioButtonAIQuadAdd;
	extern ISXGUIRadioButton* RadioButtonAIQuadsMSel;
	extern ISXGUIRadioButton* RadioButtonAIQuadsSelDel;
	extern ISXGUIButton* ButtonAIGridValidation;
	extern ISXGUICheckBox* CheckBoxAIGridMarkedSplits;
	extern ISXGUIStatic* StaticAIStatistics;
	extern ISXGUIStatic* StaticAIStatsCountQuads;
	extern ISXGUIStatic* StaticAIStatsCountGP;
	extern ISXGUIStatic* StaticAIStatsCountSplits;
	extern ISXGUIEdit* EditAIStatsCountQuads;
	extern ISXGUIEdit* EditAIStatsCountGP;
	extern ISXGUIEdit* EditAIStatsCountSplits;
	//}

	extern ISXGUIStatusBar* StatusBar1;

	void InitAllElements();

	void DeleteAllElements();

	void LevelNew(bool mess = true);
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


	void LevelEditorUpdate(DWORD timeDelta);		//!< обновление данных в редакторе уровней

	void LEcreateData();
	void LEdeleteData();

	extern ID3DXMesh* FigureBox;
	extern AxesHelper* ObjAxesHelper;	//!< рендер хелпера трансформаций

	extern int ActiveGroupType;		//!< текущая выделенная группа мировых сущностей EDITORS_LEVEL_GROUPTYPE_
	extern ID ActiveGreenSplit;		//!< текущий идентификатор сплита растительность (если выделена растительность)
	extern ID ActiveGreenObject;		//!< текущий идентификатор объекта растительности (если выделена растительность)

	extern ID ActiveElement;			//!< текущий идентификатор выделенного элемента из списка
	extern bool SelSelection;		//!< разрешено ли выделение?
	extern bool SelZTest;			//!< использовать ли z-test при выделении?
	extern bool SelMesh;			//!< рисовать сеткой (true) или целиком модель выделения (false)?
	extern bool SelBackFacesCull;	//!< отсекать ли задние грани при выделении?

	extern bool AIGBound;			//!< отрисовка боунда ai сетки
	extern bool AIGQuad;			//!< отрисовка квадов ai сетки
	extern bool AIGGraphPoint;		//!< отрисовка графпоинтов ai сетки

	//bound box для массового создания объектов растительности
	extern bool GreenRenderBox;		//!< разрешено ли рисовать бокс?
	extern float3 GreenBoxPos;		//!< позиция бокса
	extern float3_t GreenBoxWHD;	//!< ширина, высота, длина бокса




	extern float3 HelperPos;
	extern float3 HelperRot;
	extern float3 HelperScale;

	extern ID IdMtl;
	extern ID MenuWeatherCurrID;
	extern int MenuWeatherCount;
	extern Array<String> MenuWeatherArr;
};

#endif
