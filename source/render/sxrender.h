
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxrender - библиотека рендера
*/

#ifndef __SXRENDER_H
#define __SXRENDER_H

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxinput_d.lib")
#else
#pragma comment(lib, "sxinput.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxgeom_d.lib")
#else
#pragma comment(lib, "sxgeom.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxlight_d.lib")
#else
#pragma comment(lib, "sxlight.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxmtrl_d.lib")
#else
#pragma comment(lib, "sxmtrl.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxparticles_d.lib")
#else
#pragma comment(lib, "sxparticles.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxpp_d.lib")
#else
#pragma comment(lib, "sxpp.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxanim_d.lib")
#else
#pragma comment(lib, "sxanim.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxdecals_d.lib")
#else
#pragma comment(lib, "sxdecals.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxgame_d.lib")
#else
#pragma comment(lib, "sxgame.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>
#include <geom/sxgeom.h>
#include <light/sxlight.h>
#include <mtrl/sxmtrl.h>
#include <anim/sxanim.h>
#include <pp/sxpp.h>
#include <particles/sxparticles.h>


#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

/*! \defgroup sxrender sxrender - библиотека рендера
@{*/

//##########################################################################

/*! \defgroup sxrender_render_def render_def - дефайны/контсанты рендера
\ingroup sxrender
@{*/

/*! \name Состояния рендера
@{*/

/*! отрисовка материалов */
#define RENDER_STATE_MATERIAL	0	

/*! отрисовка теней */
#define RENDER_STATE_SHADOW		1	

/*! простая отрисовка (не материальная) */
#define RENDER_STATE_FREE		2	

//!@}

/*! \name Идентификаторы для определяния типа просчетов видимости
@{*/

/*! геометрия */
#define RENDER_IDARRCOM_GEOM	0	

/*! растительность */
#define RENDER_IDARRCOM_GREEN	1	

/*! анимации */
#define RENDER_IDARRCOM_ANIM	2	

//!@}


/*! минимальное значение для альфа теста */
#define RENDER_PARTICLES_ALPHATEST_VALUE 16	


/*! \name Идентификаторы для определяния типа просчетов видимости
@{*/

/*! нулевой (пустой слой, здесь будут к примеру скайбокс, облака и т.д.) */
#define RENDER_LAYER_NULL			0	

/*! непрозрачный слой */
#define RENDER_LAYER_UNTRANSPARENT	1	

/*! первый прозрачный слой, следующие слои могут быть прозрачными */
#define RENDER_LAYER_TRANSPARENT	2	

//!@}

//! тип изменения размеров окна рендера
enum RENDER_RESIZE
{
	/*! ничего не меняли */
	RENDER_RESIZE_NONE = 0,

	/*! ресайз */
	RENDER_RESIZE_RESIZE,

	/*! переход между фуллскрин и окном */
	RENDER_RESIZE_CHANGE,
};

//! цвет очистки цветового буфера сцены по умолчанию
#define RENDER_DEFAUL_BACKGROUND_COLOR D3DCOLOR_ARGB(0,128,128,128)

/*! дальняя плоскость отсечения наблюдателя для света */
#define G_DATA_LIGHT_FAR 100000 

//**************************************************************************

/*! \name Форматы файлов для диалога загрузки/сохранения
@{
*/

/*! текстуры*/
#define FILE_FILTER_TEXTURE	"All files\0*.*\0png file(.png)\0*.png\0dds file(.dds)\0*.dds\0\0"

/*! конфиг файл уровня*/
#define FILE_FILTER_LEVEL	"SkyX level file(.lvl)\0*.lvl\0All files\0*.*\0\0"

/*! dse модель*/
#define FILE_FILTER_MODEL	"dse file(.dse)\0*.dse\0All files\0*.*\0\0"

/*! вершинный шейдер*/
#define FILE_FILTER_VS		"vertex shader file(.vs)\0*.vs\0All files\0*.*\0\0"

/*! пиксельный шейдер*/
#define FILE_FILTER_PS		"pixel shader file(.ps)\0*.ps\0All files\0*.*\0\0"

//!@}

/*! \name Типы возможных выделенных объектов
@{
*/

/*! статическая геометрия */
#define EDITORS_LEVEL_GROUPTYPE_GEOM	1	

/*! растительность */
#define EDITORS_LEVEL_GROUPTYPE_GREEN	2	

/*! игровые объекты */
#define EDITORS_LEVEL_GROUPTYPE_GAME	3	

/*! ai сетка */
#define EDITORS_LEVEL_GROUPTYPE_AIGRID	4	

//!@}

//!@} sxrender_render_def

//##########################################################################

//! возвращает версию рендера
SX_LIB_API long SRender_0GetVersion();

//! установить функцию обработки сообщений
SX_LIB_API void SRender_Dbg_Set(report_func rf);

//! инициализция подсистемы
SX_LIB_API void SRender_0Create(
	const char *szName,			//!< присваиваемое имя
	HWND hWnd3D,				//!< хэндл окна рендера
	HWND hWndParent3D,			//!< хэндл родительского окна рендера (на случай если у нас редактор)
	bool isUnic = true			//!< должна ли подсистема быть уникальной по имени
	);

//! уничтожить подсистему
SX_LIB_API void SRender_AKill();

//##########################################################################

//! возвращает хэндл окна рендера
SX_LIB_API HWND SRender_GetHandleWin3D();

//! возвращает хэндл родительского окна рендера
SX_LIB_API HWND SRender_GetParentHandleWin3D();


//! установка хэндла окна рендера
SX_LIB_API void SRender_SetHandleWin3D(HWND hWnd);

//! установка хэндла родительского окна рендера
SX_LIB_API void SRender_SetParentHandleWin3D(HWND hWnd);


//! установка текущей камеры
SX_LIB_API void SRender_SetCamera(ICamera *pCamera);

//! возвращает текущую камеру
SX_LIB_API ICamera* SRender_GetCamera();

//**************************************************************************

/*! \name Симуляционная модель рендера
 \note Создана для редактора материалов, чтобы одной моделью предоставить рендер всех имеющихся типов
 \warning Добавить в симуляционную модель рендера возможно только статическую модель, у которой может быть только одна подгруппа с одной текстурой
@{*/

//! добавить статическую модель в симуляционноую модель рендера, szName - путь до статической модели относительно папки местонахождения
SX_LIB_API void SRender_SimModelAdd(const char *szName);

//! возвращает id материала
SX_LIB_API ID SRender_SimModelGetIDMtl();

//! возвращает тип модели
SX_LIB_API MTLTYPE_MODEL SRender_SimModelGetType();

//! установить тип модели
SX_LIB_API void SRender_SimModelSetType(MTLTYPE_MODEL type);


//! активирует загруженную статическую модель по номеру
SX_LIB_API void SRender_SimModelSetNumCurrModel(int iCurrNumModel);

//! возвращает текущий номер активной статической модели
SX_LIB_API int SRender_SimModelGetNumCurrModel();

//! установка текущего угла поворота по оси Y
SX_LIB_API void SRender_SimModelSetRotationY(float Rotation);

//! возвращает текущий угол поворота по оси Y
SX_LIB_API float SRender_SimModelGetRotationY();

//!@}

//**************************************************************************

/*! \name Статические объекты для рендера в редакторах
@{*/

//! установка рендера статической сетки
SX_LIB_API void SRender_EditorSetRenderGrid(bool canRender);

//! разрешен ли рендер статической сетки
SX_LIB_API bool SRender_EditorGetRenderGrid();

//! установка рендера статических осей
SX_LIB_API void SRender_EditorSetRenderAxesStatic(bool canRender);

//! разрешен ли рендер статических осей
SX_LIB_API bool SRender_EditorGetRenderAxesStatic();

//!@}

//**************************************************************************

//! возвращает id текстуры которая содержит цвет выделения
SX_LIB_API ID SRender_EditorGetSelectTex();

//! установка возможности управления камерой
SX_LIB_API void SRender_EditorCameraSetMove(bool canMove);

//! возвращает возможно ли управлять камерой
SX_LIB_API bool SRender_EditorCameraGetMove();

//! центрирвоание курсора мыши
SX_LIB_API void SRender_CentererCursor();

//! обновление навигации режим "редактор"
SX_LIB_API void SRender_UpdateEditorial(DWORD timeDelta);

//##########################################################################

//! обработка потери и восстановление устройства, isSetWindowSize - устанавливать ли размеры окна (сделано для редакторов)
SX_LIB_API void SRender_ComDeviceLost(bool isSetWindowSize);

//! обработка видимости для источников света
SX_LIB_API void SRender_ComVisibleForLight();

//! обработка видимости для камеры
SX_LIB_API void SRender_ComVisibleForCamera();

//! обработка видимости для отражений
SX_LIB_API void SRender_ComVisibleReflection();


//! обработка и установка основных матриц, обработка плоскостей отсечения
SX_LIB_API void SRender_UpdateView();

//! вывод отладочной текстовой информации в окно рендера
SX_LIB_API int SRender_OutputDebugInfo(DWORD timeDelta, bool needGameTime, const char *szStr = 0);


//! построение G буфера, то есть рендер всей сцены
SX_LIB_API void SRender_BuildMRT(DWORD timeDelta, bool isRenderSimulation);

//! обновление информации о тенях (рендер всего того что отбрасывает тени в буферы глубин источников света)
SX_LIB_API void SRender_UpdateShadow(DWORD timeDelta);


//! обработка/обновление отражений
SX_LIB_API void SRender_UpdateReflection(DWORD timeDelta, bool isRenderSimulation);

//! обновление отражений для сцены
SX_LIB_API void SRender_UpdateReflectionScene(DWORD timeDelta);

//! обновление отражений для симуляционной модели
SX_LIB_API void SRender_UpdateReflectionSimModel(DWORD timeDelta);


//! отрисовка скайбокса и облаков
SX_LIB_API void SRender_RenderSky(DWORD timeDelta);

//! обработка освещения
SX_LIB_API void SRender_ComLighting(DWORD timeDelta);

//! объединение слоев прозрачности
SX_LIB_API void SRender_UnionLayers();


//! отрисовка партиклов (эффектов)
SX_LIB_API void SRender_RenderParticles(DWORD timeDelta);

//! отрисовка основного постпроцесса
SX_LIB_API void SRender_RenderMainPostProcess(DWORD timeDelta);

//! отрисовка финального (после gui) постпроцесса
SX_LIB_API void SRender_RenderFinalPostProcess(DWORD timeDelta);

//! очистка регистров шейдеров (были случаи когда не инициаилизировав определенный регист в шейдере, шейдер брал то что было установлено до него)
SX_LIB_API void SRender_ShaderRegisterData();

//! рендер статичных объектов для редакторов (сетка, направление осей в центре системы)
SX_LIB_API void SRender_RenderEditorMain();

//##########################################################################

//! сохранить скриншот
SX_LIB_API void SRender_SaveScreenShot();

//! сохранить рабочие текстуры (г-буфер и что к нему прилагается)
SX_LIB_API void SRender_SaveWorkTex();

//! изменить режим рендера (оконный/полноэкранный)
SX_LIB_API void SRender_ChangeModeWindow();

//! переключение в режиме fullscreen в абсолютный fullscreen и обратно
SX_LIB_API void SRender_FullScreenChangeSizeAbs();

//!@} sxrender

#endif