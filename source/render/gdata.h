
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл gdata - основные и необходимые данные для цикла рендера
*/

/*! \defgroup render render - рендер
@{*/

#ifndef __GDATA_H
#define __GDATA_H

class CSimulationModel;
class CAxesStatic;
class CGrid;

#include <input/sxinput.h>
#include <render/simulation_model.h>
#include <render/editor/grid.h>
#include <render/editor/axes_static.h>
#include <render/sxrender.h>


/*! \defgroup render_gdata GData - пространство имен для хранения основных и необходимых данных для цикла рендера
 \ingroup render
@{
*/

//! пространство имен для хранения данных цикла рендера
namespace gdata
{
	//! dx устройство
	extern IGXContext* pDXDevice;
	//! Способности устройства
//	extern D3DCAPS9 dxDeviceCaps;

	//! хэндл окна родителя окна рендера, на случай редакторов
	extern HWND hHandleParent3D;	

	//! хэндл окна рендера
	extern HWND hHandle3D;
	
	//!< финальное изображение
	extern DS_RT typeFinalImage;

	//!< камера для которой будет рендер
	extern ICamera *pCamera;	

	//! id текстуры с цветмо выделения (для редакторов)
	extern ID idSelectTex;
	

	//! значение дальней и ближней плоскостей отсечения
	extern float2_t vNearFar;	

	//! fov камеры
	extern float fProjFov;				


	//! дефолтный id массива просчетов видимости для геометрии
	extern ID idDefaultGeomArr;

	//! дефолтный id массива просчетов видимости для растительности
	extern ID idDefaultGreenArr;

	//! дефолтный id массива просчетов видимости для анимационных моделей
	extern ID idDefaultAnimArr;
	

	//! позиция камеры
	extern float3 vConstCurrCamPos;	

	//! направление взгляда камеры
	extern float3 vConstCurrCamDir;	

	//! параметры перемещения камеры по умолчанию
	extern float4_t vCamWalkParamEditor;


	//! инициализация матриц
	void InitAllMatrix();	

	//! матрица вида камеры
	extern float4x4 mCamView;			

	//! матрица проекции камеры
	extern float4x4 mCamProj;			

	//! матрица проекции аналогичная камере, только дальнаяя плоскость дальше
	extern float4x4 mLightProj;		

	//! матрица проекции для рендера skybox и sky clouds, для плоских отражений, аналогична MCamProj
	extern float4x4 mRefPlaneSkyProj;	

	//! матрица проекции для рендера skybox и sky clouds, для кубических отражений
	extern float4x4 mRefCubeSkyProj;	

	//**********************************************************************

	//! пространство имен с идентификаторами шейдеров
	namespace shaders_id
	{
		//! загрузка всех необходимых шейдеров
		void InitAllShaders();

		//! вершинные шейдеры
		namespace vs
		{
			extern ID idScreenOut;
			extern ID idResPos;
		};

		//! пиксельные шейдеры
		namespace ps
		{
			extern ID idScreenOut;
			extern ID idComLightingNonShadow;
			extern ID idComLightingShadow;

			extern ID idBlendAmbientSpecDiffColor;

			
			extern ID idStencilStr;
			extern ID idStencilColumn;
			extern ID idStencilStrColumn;
			extern ID idUnionAlpha;
		};

		namespace kit
		{
			extern ID idScreenOut;
			extern ID idStencilStr;
			extern ID idStencilColumn;
			extern ID idStencilStrColumn;
			extern ID idBlendAmbientSpecDiffColor;
			extern ID idUnionAlpha;
			extern ID idComLightingNonShadow;
			extern ID idComLightingShadow;
		};
	};

	namespace rstates
	{
		extern IGXDepthStencilState *pDepthStencilStateNoZ;
		extern IGXDepthStencilState *pDepthStencilStateMrtStage0;
		extern IGXDepthStencilState *pDepthStencilStateMrtStage1;
		extern IGXDepthStencilState *pDepthStencilStateLightBound;
		extern IGXDepthStencilState *pDepthStencilStateLightClear;
		extern IGXDepthStencilState *pDepthStencilStateLightShadowNonGlobal;
		extern IGXDepthStencilState *pDepthStencilStateLightShadowGlobal;
		extern IGXDepthStencilState *pDepthStencilStateParticles;

		extern IGXSamplerState *pSamplerPointClamp;
		extern IGXSamplerState *pSamplerLinearWrap;
		extern IGXSamplerState *pSamplerLinearMirror;
		extern IGXSamplerState *pSamplerLinearClamp;
		extern IGXSamplerState *pSamplerAnisotopicClamp;
		extern IGXSamplerState *pSamplerAnisotopicWrap;

		extern IGXSamplerState *pSamplerScene;

		extern IGXBlendState *pBlendNoColor;
		extern IGXBlendState *pBlendRed;
		extern IGXBlendState *pBlendAlpha;
		extern IGXBlendState *pBlendAlphaOneOne;
		extern IGXBlendState *pBlendAlphaSky;

		extern IGXRasterizerState *pRasterizerCullFront;
		extern IGXRasterizerState *pRasterizerCullNone;
	};

	namespace Editors
	{
		//! возможно управлять камерой
		extern bool canMoveCamera;

		//! указатель симуляционной модели
		extern CSimulationModel *pSimModel;

		//! сетка дял редакторов
		extern CGrid *pGrid;				

		//! рендер статический осей в центре координат
		extern CAxesStatic *pAxesStatic;	


		//! рисовать ли #pGrid
		extern bool canRenderGrid;				

		//! рисовать ли #pAxesStatic
		extern bool canRenderAxesStatic;
	}
};

//!@} render_gdata

#endif

//@} render
