
#ifndef __LEVEL_EDITOR_H
#define __LEVEL_EDITOR_H

/*
Все данные сцены отображаются в pListBoxList, 
статическая геометрия, растительность  имеют такие же идентификаторы как они указаны в списке (нулевой элемент списка имеет нулевой идентификатор)
игровые объекты хранят свой id в юзердате элементов списка, то есть чтобы достать id объекта, надо достать юзердату строки pListBoxList
*/

#define SX_LE_MMENU_WEATHER_BEGIN_ID 50001

#define MAINWIN_SIZE_X	820
#define MAINWIN_SIZE_Y	690

#define SX_LEVEL_EDITOR_NAME "LevelEditor"

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

//##########################################################################

//! обработчик для получения превью уровня в диалоге выбора
bool HandlerGetPreviewLevel(const char *szPath, char *szBuff);

//##########################################################################

namespace level_editor
{
	extern ISXGUIBaseWnd *pJobWindow;
	extern ISXGUIMenuWindow *pMainMenu;
	extern ISXGUIBaseWnd *pRenderWindow;

	extern ISXGUIToolBar *pToolBar1;
	extern ISXGUIButton *pButtonTBNew;
	extern ISXGUIButton *pButtonTBOpen;
	extern ISXGUIButton *pButtonTBSave;
	extern ISXGUIButton *pButtonTBSaveAs;

	extern ISXGUICheckBox *pCheckBoxTBArrow;
	extern ISXGUICheckBox *pCheckBoxTBPos;
	extern ISXGUICheckBox *pCheckBoxTBRot;
	extern ISXGUICheckBox *pCheckBoxTBScale;

	extern ISXGUICheckBox *pCheckBoxTBGrid;
	extern ISXGUICheckBox *pCheckBoxTBAxes;

	extern ISXGUICheckBox *pCheckBoxTBRColor;
	extern ISXGUICheckBox *pCheckBoxTBRNormal;
	extern ISXGUICheckBox *pCheckBoxTBRParam;
	extern ISXGUICheckBox *pCheckBoxTBRAmDiff;
	extern ISXGUICheckBox *pCheckBoxTBRSpecular;
	extern ISXGUICheckBox *pCheckBoxTBRLighting;

	extern ISXGUICheckBox *pCheckBoxTBSelS;
	extern ISXGUICheckBox *pCheckBoxTBSelZTest;
	extern ISXGUICheckBox *pCheckBoxTBSelMesh;
	extern ISXGUICheckBox *pCheckBoxTBSelCullBack;

	extern ISXGUICheckBox *pCheckBoxTBAIGBound;
	extern ISXGUICheckBox *pCheckBoxTBAIGQuad;
	extern ISXGUICheckBox *pCheckBoxTBAIGGraphPoint;

	extern ISXGUICheckBox *pCheckBoxTBLevelType;
	extern ISXGUICheckBox *pCheckBoxTBGLightEnable;

	extern ISXGUIGroupBox *pGroupBoxList;
	extern ISXGUIGroupBox *pGroupBoxData;
	extern ISXGUIListBox *pListBoxList;
	extern ISXGUIStatic *pStaticListTextCount;
	extern ISXGUIStatic *pStaticListValCount;

	extern ISXGUIButton *pButtonDelete;

	extern ISXGUIButton *pButtonGeometryOpen;
	extern ISXGUIButton *pButtonGreenOpen;
	extern ISXGUIButton *pButtonGameObjectOpen;
	extern ISXGUIButton *pButtonAIGridOpen;


	//model
	//{{
	extern ISXGUIStatic *pStaticGeomName;
	extern ISXGUIEdit *pEditGeomName;

	extern ISXGUIStatic *pStaticGeomModel;
	extern ISXGUIEdit *pEditGeomModel;
	extern ISXGUIButton *pButtonGeomModel;

	extern ISXGUIStatic *pStaticGeomLod1;
	extern ISXGUIEdit *pEditGeomLod1;
	extern ISXGUIButton *pButtonGeomLod1;

	extern ISXGUIStatic *pStaticGeomPhysics;
	extern ISXGUIEdit *pEditGeomPhysics;
	extern ISXGUIButton *pButtonGeomPhysics;

	extern ISXGUICheckBox *pCheckBoxSegmentation;
	
	extern ISXGUIStatic *pStaticGeomPos;
	extern ISXGUIEdit *pEditGeomPosX;
	extern ISXGUIEdit *pEditGeomPosY;
	extern ISXGUIEdit *pEditGeomPosZ;
	extern ISXGUIRadioButton *pRadioButtonGeomPosX;
	extern ISXGUIRadioButton *pRadioButtonGeomPosY;
	extern ISXGUIRadioButton *pRadioButtonGeomPosZ;
	extern ISXGUIStatic *pStaticGeomRot;
	extern ISXGUIEdit *pEditGeomRotX;
	extern ISXGUIEdit *pEditGeomRotY;
	extern ISXGUIEdit *pEditGeomRotZ;
	extern ISXGUIRadioButton *pRadioButtonGeomRotX;
	extern ISXGUIRadioButton *pRadioButtonGeomRotY;
	extern ISXGUIRadioButton *pRadioButtonGeomRotZ;
	extern ISXGUIStatic *pStaticGeomScale;
	extern ISXGUIEdit *pEditGeomScaleX;
	extern ISXGUIEdit *pEditGeomScaleY;
	extern ISXGUIEdit *pEditGeomScaleZ;
	extern ISXGUIRadioButton *pRadioButtonGeomScaleX;
	extern ISXGUIRadioButton *pRadioButtonGeomScaleY;
	extern ISXGUIRadioButton *pRadioButtonGeomScaleZ;
	extern ISXGUIButton *pButtonGeomFinish;
	//}}

	//Green
	//{{
	extern ISXGUIStatic *pStaticGreenName;
	extern ISXGUIEdit *pEditGreenName;

	extern ISXGUIStatic *pStaticGreenModel;
	extern ISXGUIEdit *pEditGreenModel;
	extern ISXGUIButton *pButtonGreenModel;

	extern ISXGUIStatic *pStaticGreenLod1;
	extern ISXGUIEdit *pEditGreenLod1;
	extern ISXGUIButton *pButtonGreenLod1;

	extern ISXGUIStatic *pStaticGreenLod2;
	extern ISXGUIEdit *pEditGreenLod2;
	extern ISXGUIButton *pButtonGreenLod2;

	extern ISXGUIStatic *pStaticGreenMask;
	extern ISXGUIEdit *pEditGreenMask;
	extern ISXGUIButton *pButtonGreenMask;

	extern ISXGUICheckBox *pCheckBoxGreenAveragedRGB;
	
	extern ISXGUIStatic *pStaticGreenNav;
	extern ISXGUIEdit *pEditGreenNav;
	extern ISXGUIButton *pButtonGreenNav;

	extern ISXGUITrackBar *pTrackBarGreenDensity;
	extern ISXGUIButton *pButtonGreenGenerate;
	extern ISXGUIStatic *pStaticGreenDensityText;
	extern ISXGUIStatic *pStaticGreenDensityVal;

	extern ISXGUIEdit *pEditGreenSelX;
	extern ISXGUIEdit *pEditGreenSelY;
	extern ISXGUIEdit *pEditGreenSelZ;
	extern ISXGUIRadioButton *pRadioButtonGreenSelX;
	extern ISXGUIRadioButton *pRadioButtonGreenSelY;
	extern ISXGUIRadioButton *pRadioButtonGreenSelZ;
	extern ISXGUIComboBox *pComboBoxGreenSel;
	extern ISXGUIStatic *pStaticGreenSelX;
	extern ISXGUIStatic *pStaticGreenSelY;
	extern ISXGUIStatic *pStaticGreenSelZ;
	//}}

	//game object
	//{
	extern ISXGUIStatic *pStaticGameClass;
	extern ISXGUIComboBox *pComboBoxGameClass;
	extern ISXGUIButton *pButtonGameTab;

	//! текущий активный раздел игровых объектов, 0 - своейства, 1 - соединения
	extern int GameTabVal;

	extern ISXGUIListView *pListViewGameClass;
	extern ISXGUIComboBox *pComboBoxGameValue;
	extern ISXGUIEdit *pEditGameValue;
	extern ISXGUIButton *pButtonGameValue;
	extern ISXGUIStatic *pStaticGameHelp;
	extern ISXGUIMemo *pMemoGameHelp;
	extern ISXGUIButton *pButtonGameCreate;

	extern ISXGUICheckBox *pCheckBoxGameFlags[16];
	extern const char *aGameObjectFlags[16];

	extern ISXGUIListView *pListViewGameConnections;
	extern ISXGUIStatic *pStaticGameConnectionsEvent;
	extern ISXGUIComboBox *pComboBoxGameConnectionsEvent;
	extern ISXGUIStatic *pStaticGameConnectionsName;
	extern ISXGUIEdit *pEditGameConnectionsName;
	extern ISXGUIStatic *pStaticGameConnectionsAction;
	extern ISXGUIComboBox *pComboBoxGameConnectionsAction;
	extern ISXGUIStatic *pStaticGameConnectionsDelay;
	extern ISXGUIEdit *pEditGameConnectionsDelay;
	extern ISXGUIStatic *pStaticGameConnectionsParameter;
	extern ISXGUIEdit *pEditGameConnectionsParameter;
	extern ISXGUIButton *pButtonGameConnectionsCreate;
	extern ISXGUIButton *pButtonGameConnectionsDelete;
	//}

	//aigrid
	//{
	extern ISXGUIButton *pButtonAIQuadsDelSel;
	extern ISXGUIButton *pButtonAIGridGen;
	extern ISXGUIButton *pButtonAIGridClear;
	extern ISXGUIButton *pButtonAIClearAll;
	extern ISXGUIStatic *pStaticAIBBDimensions;
	extern ISXGUIStatic *pStaticAIBBDimensionsWidth;
	extern ISXGUIEdit *pEditAIBBDimensionsWidth;
	extern ISXGUIStatic *pStaticAIBBDimensionsHeight;
	extern ISXGUIEdit *pEditAIBBDimensionsHeight;
	extern ISXGUIStatic *pStaticAIBBDimensionsDepth;
	extern ISXGUIEdit *pEditAIBBDimensionsDepth;
	extern ISXGUIStatic *pStaticAIBBPos;
	extern ISXGUIStatic *pStaticAIBBPosX;
	extern ISXGUIEdit *pEditAIBBPosX;
	extern ISXGUIStatic *pStaticAIBBPosY;
	extern ISXGUIEdit *pEditAIBBPosY;
	extern ISXGUIStatic *pStaticAIBBPosZ;
	extern ISXGUIEdit *pEditAIBBPosZ;
	extern ISXGUIButton *pButtonAIBBFinish;
	extern ISXGUIButton *pButtonAIGPGen;
	extern ISXGUIButton *pButtonAIGPClear;
	extern ISXGUIRadioButton *pRadioButtonAIGPAdd;
	extern ISXGUIRadioButton *pRadioButtonAIGPDel;
	extern ISXGUIRadioButton *pRadioButtonAIQuadAdd;
	extern ISXGUIRadioButton *pRadioButtonAIQuadsMSel;
	extern ISXGUIRadioButton *pRadioButtonAIQuadsSelDel;
	extern ISXGUIButton *pButtonAIGridValidation;
	extern ISXGUICheckBox *pCheckBoxAIGridMarkedSplits;
	extern ISXGUIStatic *pStaticAIStatistics;
	extern ISXGUIStatic *pStaticAIStatsCountQuads;
	extern ISXGUIStatic *pStaticAIStatsCountGP;
	extern ISXGUIStatic *pStaticAIStatsCountSplits;
	extern ISXGUIEdit *pEditAIStatsCountQuads;
	extern ISXGUIEdit *pEditAIStatsCountGP;
	extern ISXGUIEdit *pEditAIStatsCountSplits;
	//}

	extern ISXGUIStatusBar *pStatusBar1;

	//######################################################################

	//! есть ли модель по относительнмоу пути
	bool existsFileStaticGeom(const char *szRelPath);

	//**********************************************************************

	//! инициализация всех элементов интерфейса
	void InitAllElements();

	//! удаление всех элементов интерфейса
	void DeleteAllElements();

	//**********************************************************************

	//! обновление данных в редакторе уровней
	void LevelEditorUpdate(DWORD timeDelta);

	//! создание рендерных данных для редактора
	void LEcreateRenderData();

	//! удаление рендерных данных редактора
	void LEdeleteRenderData();

	void NullingButtonSelTransform();

	//**********************************************************************

	//! создать новый уровень (удаляет все что было), useMessage - выдавать ли сообщение если есть загруженные ресурсы
	void LevelNew(bool useMessage = true);
	
	//! открыть диалог выбора уровеня, если будет выбран уровень то открыть уровень
	void LevelOpen();

	//! сохранить уровень
	void LevelSave();

	//! сохранить уровень как
	void LevelSaveAs();

	//**********************************************************************

	//! убирает отметки с элементво меню опции "финальное изображение"
	void FinalImageUncheckedMenu();

	//! заполнить листбокс информацией о статической геометрии, iSelect - выделить элемент
	void FillListBoxGeom(int iSelect=-1);

	void FillListBoxGreen(int iSelect = -1);

	void FillListBoxAIgrid();

	void FillListBoxGameObj(int iSelect = -1);
	
	//**********************************************************************

	void GeomActivateAll(bool bf);
	void GeomActivateCreate(bool bf);
	void GeomActivateTrans(bool bf);
	void GeomSel(int sel);

	//! обработка возможностей при трассировке луча для статической геометрии
	void GeomTraceSelect();

	void GeomTraceSetPos();

	void GeomTraceCreate();
	
	//! обновление временной позиции копирования
	void GeomUpdateCopyPos();

	//! копирование модели
	void GeomCopy();

	//! трансформация статики по данным хелпера
	void GeomTransformByHelper();

	void GeomDelete(int iSelected);

	//**********************************************************************

	void GreenActivateAll(bool bf);
	void GreenActivateMain(bool bf);
	void GreenActivateCreate(bool bf);
	void GreenActivateEdit(bool bf);
	void GreenSel(int iSelect);

	//! обработка возможностей при трассировке луча для растительности
	void GreenTraceSelect();

	void GreenTraceSetPos();

	//! трансформация растительности по данным хелпера
	void GreenTransformByHelper();

	void GreenSetPos4Box();

	void GreenDelete(int iSelected);

	//**********************************************************************

	/*! активировать все элементы интерфейса работы с игровыми объектами
	 \note Раздел видимости соединений скрывается при любом раскладе
	*/
	void GameVisible4Properties(bool bf);

	//! установка видимости раздела соединений
	void GameVisible4Connections(bool bf);

	//! выделение игрового объекта по номеру из списка listbox
	void GameSel(int iSelect);

	//! трассировка луча и установка позиции в месте пересечения
	void GameTraceSetPos();

	//! трассировка луча и создание в месте пересечения
	void GameTraceCreate();

	//! поиск объекта по имени для соединения
	CBaseEntity* GameGetObj4Connect(CBaseEntity* pEntity, const char *szName);

	//! выдача сообщения в случае если не найден игровой объект по имени (для #GameGetObj4Connect)
	void GameMessageNotFoundName(const char *szName);

	//! обновить позицию или повороты в таблице редактора
	void GameUpdatePosRot();

	//! установка видимости раздела свойств
	void GameVisibleProperties(bool bf);

	//! трансформация игровых объектов по данным хелпера
	void GameTransformByHelper();

	//! удалить игровой объект по номеру в списке
	void GameDelete(int iSelected);

	//**********************************************************************

	void AIGridActivateAll(bool bf);
	void AIGridEnableBB(bool bf);

	//! обработка возможностей при трассировке луча для аи сетки
	void AIGridTraceSelect();

	void AIGridMultiSelect();

	//######################################################################

	/*! \name Луч
	@{*/

	//! стартовая точка луча (позиция наблюдателя)
	extern float3 vRayOrigin;

	//! направление луча
	extern float3 vRayDir;

	//! прямое направление луча (направление наблюдателя)
	extern float3 vRayDirDirect;

	//! позиция мыши отнросительно окна рендера
	extern POINT oPointMouse;

	//!@}

	//**********************************************************************

	/*! \name Копирование сущностей
	@{*/

	//! используется ли копирование
	extern bool useCopyData;

	//! id элемента сущности, который надо скопировать
	extern ID idCopy;

	//! временная позиция копии (при визуальном перемещении)
	extern float3 vCopyPos;

	//!@}

	//**********************************************************************

	extern ID3DXMesh *pFigureBox;

	//! рендер хелпера трансформаций
	extern CAxesHelper *pAxesHelper;	


	/*extern float3 vHelperPos;
	extern float3 vHelperRot;
	extern float3 vHelperScale;*/

	//**********************************************************************

	extern int iActiveGroupType;		//!< текущая выделенная группа мировых сущностей EDITORS_LEVEL_GROUPTYPE_
	extern ID idActiveGreenSplit;		//!< текущий идентификатор сплита растительность (если выделена растительность)
	extern ID idActiveGreenObject;		//!< текущий идентификатор объекта растительности (если выделена растительность)

	extern ID idActiveElement;			//!< текущий идентификатор выделенного элемента из списка listbox

	//**********************************************************************

	extern bool canSelSelection;		//!< разрешено ли выделение?
	extern bool canSelZTest;			//!< использовать ли z-test при выделении?
	extern bool canSelMesh;				//!< рисовать сеткой (true) или целиком модель выделения (false)?
	extern bool canSelBackFacesCull;	//!< отсекать ли задние грани при выделении?

	//**********************************************************************

	extern bool canAIGBound;			//!< отрисовка боунда ai сетки
	extern bool canAIGQuad;				//!< отрисовка квадов ai сетки
	extern bool canAIGGraphPoint;		//!< отрисовка графпоинтов ai сетки

	//**********************************************************************

	//bound box для массового создания объектов растительности
	extern bool canGreenRenderBox;		//!< разрешено ли рисовать бокс?
	extern float3 vGreenBoxPos;			//!< позиция бокса
	extern float3_t vGreenBoxWHD;		//!< ширина, высота, длина бокса

	//**********************************************************************

	/*! добавление (true) либо создание нового (false) соединения для игрового объекта 
	 \note при добавлении все данные с редакторов записываются в новое соединеие, при создании редакторы отвязывается от таблицы и их можно спокойно изменять
	*/
	extern bool isAddGameConections;

	extern ID idMtl;
	extern ID idMenuWeatherCurr;
	extern int iMenuWeatherCount;
	extern Array<String> aMenuWeather;
};

#endif
