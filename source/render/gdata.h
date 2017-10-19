
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл gdata - основные и необходимые данные для цикла рендера
*/

/*! \defgroup managed_render managed_render - управляемый рендер
@{*/

#ifndef __GDATA_H
#define __GDATA_H

class ModelSim;
class AxesHelper;
class AxesStatic;
class Grid;

#include <input/sxinput.h>
#include <render/render_def.h>
#include <render/model_sim.h>
#include <render/editor/grid.h>
#include <render/editor/axes_static.h>
#include <render/sxrender.h>

/*#if defined(SX_GAME)
#include <SkyXEngine_Build/resource.h>
#endif*/


/*! \defgroup managed_render_gdata gdata - пространство имен для хранения основных и необходимых данных для цикла рендера
 \ingroup managed_render
@{
*/



//##########################################################################

//! пространство имен для хранения данных цикла рендера
namespace GData
{
	extern IDirect3DDevice9* DXDevice;	//!< dx устройство

	extern HWND HandleParent3D;	//!< хэндл окна родителя окна рендера, на случай редакторов
	extern HWND Handle3D;		//!< хэндл окна рендера

	extern bool IsWindowed;					//!<использовать ли оконный режим рендера?

	
	extern DS_RT FinalImage;//!< финальное изображение
	extern ISXCamera* ObjCamera;	//!< камера для которой будет рендер
	extern ID IDSelectTex;
	
	extern float2_t NearFar;	//!< значение дальней и ближней плоскостей отсечения
	extern float ProjFov;				//!< fov камеры

	extern ID DefaultGeomIDArr;
	extern ID DefaultGreenIDArr;
	extern ID DefaultAnimIDArr;
	
	extern float3 ConstCurrCamPos;	//!< позиция камеры
	extern float3 ConstCurrCamDir;	//!< направление взгляда камеры

	//! Параметры перемещения камеры по умолчанию
	extern float4_t CamWalkParamEditor;

	//матрицы
	void InitAllMatrix();		//!< инициализация матриц
	extern float4x4 MCamView;			//!< матрица вида камеры
	extern float4x4 MCamProj;			//!< матрица проекции камеры
	extern float4x4 MLightProj;		//!< матрица проекции аналогична¤ камере, только дальна¤¤ плоскость дальше
	extern float4x4 MRefPlaneSkyProj;	//!< матрица проекции дл¤ рендера skybox и sky clouds, дл¤ плоских отражений, аналогична MCamProj
	extern float4x4 MRefCubeSkyProj;	//!< матрица проекции дл¤ рендера skybox и sky clouds, дл¤ куибческих отражений

	//**********************************************************************

	//! пространство имен с идентификаторами шейдеров
	namespace IDsShaders
	{
		//! загрузка всех необходимых шейдеров
		void InitAllShaders();

		//! вершинные шейдеры
		namespace VS
		{
			extern ID ScreenOut;
			extern ID ResPos;
		};

		//! пиксельные шейдеры
		namespace PS
		{
			extern ID ScreenOut;
			extern ID ComLightingNonShadow;
			extern ID ComLightingShadow;

			extern ID BlendAmbientSpecDiffColor;

			extern ID ToneMapping;

			extern ID StencilStr;
			extern ID StencilColumn;
			extern ID StencilStrColumn;
			extern ID UnionAlpha;
		};
	};

	namespace Editors
	{
		extern bool MoveCamera;
		extern ModelSim* SimModel;			//!< указатель симуляционной модели
		extern Grid* ObjGrid;				//!< сетка дял редакторов
		extern AxesStatic* ObjAxesStatic;	//!< рендер статический осей в центре координат
		/*extern ID3DXMesh* FigureBox;		//!< бокс для определения ограничивающего объема для эммитера партиклов
		extern float3_t vFigureBoxParam;	
		extern ID3DXMesh* FigureSphere;		//!< сфера для определения ограничивающего объема для эммитера партиклов
		extern float3_t vFigureSphereParam;	
		extern ID3DXMesh* FigureCone;		//!< конус для определения ограничивающего объема для эммитера партиклов
		extern float3_t vFigureConeParam;	//!< параметры для FigureCone (x - верхний радиус, y - нижний радиус, z - высота)
		extern float3 vFigurePos;			//!< позиция для рисуемой фигуры*/

		extern bool RenderGrid;				//!< рисовать ли #ObjGrid
		extern bool RenderAxesStatic;		//!< рисовать ли #ObjAxesStatic
		/*extern bool RenderBoundBox;			//!< рисовать ли ограничивающий объем box
		extern bool RenderBoundSphere;		//!< рисовать ли ограничивающий объем sphere
		extern bool RenderBoundCone;		//!< рисовать ли ограничивающий объем cone*/
	}

/*#if !defined(SX_GAME)
	namespace Editors
	{
		ModelSim* SimModel = 0;			//!< указатель симуляционной модели
		Grid* ObjGrid = 0;				//!< сетка дял редакторов
		AxesStatic* ObjAxesStatic = 0;	//!< рендер статический осей в центре координат
		AxesHelper* ObjAxesHelper = 0;	//!< рендер хелпера трансформаций
		ID3DXMesh* FigureBox;			//!< бокс для определения ограничивающего объема для эммитера партиклов
		ID3DXMesh* FigureSphere;		//!< сфера для определения ограничивающего объема для эммитера партиклов
		ID3DXMesh* FigureCone;			//!< конус для определения ограничивающего объема для эммитера партиклов
		float3_t FigureConeParam;		//!< параметры для FigureCone (x - верхний радиус, y - нижний радиус, z - высота)
		float3 FigureConePos;			//!< позиция для FigureCone

		bool RenderGrid = false;		//!< рисовать ли #ObjGrid
		bool RenderAxesStatic = false;	//!< рисовать ли #ObjAxesStatic

#if defined(SX_MATERIAL_EDITOR)
		bool MoveCamera = false;		//!< разрешено ли двигать камеру
		void MaterialEditorUpdate();	//!< обновление данных в редакторе материалов
#endif

#if defined(SX_LEVEL_EDITOR)
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
		float3_t GreenBoxWHD(1,1,1);	//!< ширина, высота, длина бокса

		void LevelEditorUpdate();		//!< обновление данных в редакторе уровней
#endif

#if defined(SX_PARTICLES_EDITOR)
		bool RenderBound = false;		//!< рисовать ли ограничивающий объем для эффекта
		void ParticlesEditorUpdate();	//!< обновление данных в редакторе партиклов
#endif
	};
#endif
	*/
};

//!@} managed_render_gdata

#endif

//@} managed_render
