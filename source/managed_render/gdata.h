
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
license see in LICENSE
******************************************************/

/*!
\file
Заголовочный файл gdata - основные и необходимые данные для цикла рендера
*/

/*! \defgroup managed_render managed_render - управляемый рендер
@{*/

#ifndef __gdata_h
#define __gdata_h

#include <managed_render/model_sim.h>

#define G_DATA_LIGHT_FAR 100000 /*!< дальняя плоскость отсечения наблюдателя для света */

/*! \defgroup managed_render_gdata gdata - пространство имен для хранения основных и необходимых данных для цикла рендера
 \ingroup managed_render
@{
*/

//! пространство имен для хранения данных цикла рендера
namespace GData
{
	IDirect3DDevice9* DXDevice = 0;	//!< dx устройство

	//! функция обработчик оконных сообщений (окна рендера)
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//! инициализация окна рендера
	void InitWin(
		const char* name,	//!< имя окна (только латиница)
		const char* caption	//!< название окна
		);

	HWND Handle3D = 0;	//!< хэндл окна рендера

	float2_t WinSize = float2_t(800, 600);	//!< размер окна рендера (области рендера)
	bool IsWindowed = true;					//!<использовать ли оконный режим рендера?

	ModelSim* SimModel = 0;		//!< указатель симуляционной модели

	ISXCamera* ObjCamera = 0;	//!< камера для которой будет рендер
	
	int ReSize = 0;				//!< 0 - ничего не меняли, 1 - ресайз, 2 - переход между фуллскрин и окном

	float2_t NearFar = float2_t(0.25,400);	//!< значение дальней и ближней плоскостей отсечения
	float ProjFov = SM_PI * 0.25f;			//!< fov камеры

	ID DefaultGeomIDArr = -1;
	ID DefaultGreenIDArr = -1;
	ID DefaultAnimIDArr = -1;
	
	float3 ConstCurrCamPos;	//!< позиция камеры
	float3 ConstCurrCamDir;	//!< направление взгляда камеры

	//! Параметры перемещения камеры по умолчанию
	float4_t CamWalkParamEditor = float4_t(
		10.f,	//!< простое движенеи вперед
		5.f,	//!< коэфициент ускорения
		0.7f,	//!< коэфициент от основного движения в стороны 
		0.5f	//!< коэфициент от основного движения назад
		);

	//матрицы
	void InitAllMatrix();		//!< инициализация матриц
	float4x4 MCamView;			//!< матрица вида камеры
	float4x4 MCamProj;			//!< матрица проекции камеры
	float4x4 MLightProj;		//!< матрица проекции аналогична¤ камере, только дальна¤¤ плоскость дальше
	float4x4 MRefPlaneSkyProj;	//!< матрица проекции дл¤ рендера skybox и sky clouds, дл¤ плоских отражений, аналогична MCamProj
	float4x4 MRefCubeSkyProj;	//!< матрица проекции дл¤ рендера skybox и sky clouds, дл¤ куибческих отражений

	//////

	//! пространство имен с путями
	namespace Pathes
	{
		void InitAllPathes();	//!< инициализация всех путей

		char ForExe[1024];		//!< путь до запущенного исполняемого файла
		char WorkingTex[1024];	//!< путь до рабочих текстур (некоторые текстуры (render targets) предназначенные для рендера можно сохранить в сюда, для просмотра результатов)
		char Screenshots[1024];	//!< путь до папки куда будут сохраняться скриншоты
		char GameSource[1024];	//!< путь до папки с игровыми ресурсами
			char Configs[1024];		//!< путь до папки с конфигурациями
			char Levels[1024];		//!< путь до папки с уровнями
			char Meshes[1024];		//!< путь до папки со статическими моделями
			char Models[1024];		//!< путь до папки со скелетными моделями
			char Scripts[1024];		//!< путь до папки со скриптами
			char Materials[1024];	//!< путь до папки с материалами
			char GUIresources[1024];//!< путь до паки ресурсов gui
			char Sounds[1024];		//!< путь до папки со звуками
			char Shaders[1024];		//!< путь до папки с шейдерами
			char Textures[1024];	//!< путь до папки с текстурами
				char TexturesDetail[1024];	//!< путь до папки с детальными текстурами
				char TexturesSkyBoxes[1024];//!< путь до папки с текстурами предназначенными для скайбоксов либо для облаков
	};

	//! пространство имен с идентификаторами шейдеров
	namespace IDsShaders
	{
		//! загрузка всех необходимых шейдеров
		void InitAllShaders();

		//! вершинные шейдеры
		namespace VS
		{
			ID ScreenOut;
			ID ResPos;
		};

		//! пиксельные шейдеры
		namespace PS
		{
			ID ScreenOut;
			ID ComLightingNonShadow;
			ID ComLightingShadow;

			ID BlendAmbientSpecDiffColor;

			ID StencilStr;
			ID StencilColumn;
			ID StencilStrColumn;
			ID UnionAlpha;
		};
		};
	};

//!@} managed_render_gdata

#include <managed_render/model_sim.cpp>

#endif

//@} managed_render