
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл gdata - основные и необходимые данные для цикла рендера
*/

/*! \defgroup render render - рендер
@{*/

#ifndef __GDATA_H
#define __GDATA_H

class ModelSim;
class CAxesStatic;
class CGrid;

#include <input/sxinput.h>
#include <render/model_sim.h>
#include <render/editor/grid.h>
#include <render/editor/axes_static.h>
#include <render/sxrender.h>


/*! \defgroup render_gdata GData - пространство имен для хранения основных и необходимых данных для цикла рендера
 \ingroup render
@{
*/

//! пространство имен для хранения данных цикла рендера
namespace GData
{
	//! dx устройство
	extern IDirect3DDevice9* DXDevice;	

	//! хэндл окна родителя окна рендера, на случай редакторов
	extern HWND HandleParent3D;	

	//! хэндл окна рендера
	extern HWND Handle3D;		

	//! использовать ли оконный режим рендера?
	//extern bool IsWindowed;		

	
	//!< финальное изображение
	extern DS_RT FinalImage;

	//!< камера для которой будет рендер
	extern ISXCamera* ObjCamera;	

	//! id текстуры с цветмо выделения (для редакторов)
	extern ID IDSelectTex;
	

	//! значение дальней и ближней плоскостей отсечения
	extern float2_t NearFar;	

	//! fov камеры
	extern float ProjFov;				


	//! дефолтный id массива просчетов видимости для геометрии
	extern ID DefaultGeomIDArr;

	//! дефолтный id массива просчетов видимости для растительности
	extern ID DefaultGreenIDArr;

	//! дефолтный id массива просчетов видимости для анимационных моделей
	extern ID DefaultAnimIDArr;
	

	//! позиция камеры
	extern float3 ConstCurrCamPos;	

	//! направление взгляда камеры
	extern float3 ConstCurrCamDir;	

	//! параметры перемещения камеры по умолчанию
	extern float4_t CamWalkParamEditor;


	//! инициализация матриц
	void InitAllMatrix();	

	//! матрица вида камеры
	extern float4x4 MCamView;			

	//! матрица проекции камеры
	extern float4x4 MCamProj;			

	//! матрица проекции аналогичная камере, только дальнаяя плоскость дальше
	extern float4x4 MLightProj;		

	//! матрица проекции для рендера skybox и sky clouds, для плоских отражений, аналогична MCamProj
	extern float4x4 MRefPlaneSkyProj;	

	//! матрица проекции для рендера skybox и sky clouds, для кубических отражений
	extern float4x4 MRefCubeSkyProj;	

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
		//! возможно управлять камерой
		extern bool MoveCamera;

		//! указатель симуляционной модели
		extern ModelSim* SimModel;			

		//! сетка дял редакторов
		extern CGrid* ObjGrid;				

		//! рендер статический осей в центре координат
		extern CAxesStatic* ObjAxesStatic;	


		//! рисовать ли #ObjGrid
		extern bool RenderGrid;				

		//! рисовать ли #ObjAxesStatic
		extern bool RenderAxesStatic;		
	}
};

//!@} render_gdata

#endif

//@} render
