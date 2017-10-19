
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxrender - библиотека функций рендера
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
#pragma comment(lib, "sxmtllight_d.lib")
#else
#pragma comment(lib, "sxmtllight.lib")
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

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>
#include <geom/sxgeom.h>
#include <mtllight/sxmtllight.h>
#include <anim/sxanim.h>
#include <pp/sxpp.h>
#include <particles/sxparticles.h>


#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

/*
//быстрая реализация фильтрации и адресации
//{
//индивидуально для регистра
inline void SetSamplerFilter(DWORD id, DWORD value);
inline void SetSamplerAddress(DWORD id, DWORD value);

//для указанного промежутка групп регистров
inline void SetSamplerFilter(DWORD begin_id, DWORD end_id, DWORD value);
inline void SetSamplerAddress(DWORD begin_id, DWORD end_id, DWORD value);

void SetRenderSceneFilter();
void SetRenderSceneFilterUn();
//}
*/

//##########################################################################

/*! \defgroup managed_render_render_def render_def - дефайны/контсанты рендера
\ingroup managed_render
@{*/

/*! \name Состояния рендера
@{*/

#define RENDER_STATE_MATERIAL	0	/*!< отрисовка материалов */
#define RENDER_STATE_SHADOW		1	/*!< отрисовка теней */
#define RENDER_STATE_FREE		2	/*!< простая отрисовка (не материальная) */

//!@}

/*! \name Идентификаторы для определяния типа просчетов видимости
@{*/

#define RENDER_IDARRCOM_GEOM	0	/*!< геометрия */
#define RENDER_IDARRCOM_GREEN	1	/*!< растительность */
#define RENDER_IDARRCOM_ANIM	2	/*!< анимации */

//!@}

#define RENDER_PARTICLES_ALPHATEST_VALUE 16	/*!< минимальное значение для альфа теста */

/*! \name Идентификаторы для определяния типа просчетов видимости
@{*/

#define RENDER_LAYER_NULL			0	/*!< нулевой (пустой слой, здесь будут к примеру скайбокс, облака и т.д.) */
#define RENDER_LAYER_UNTRANSPARENT	1	/*!< непрозрачный слой */
#define RENDER_LAYER_TRANSPARENT	2	/*!< первый прозрачный слой, следующие слои могут быть прозрачными */

//!@}

/*! ничего не меняли */
#define RENDER_RESIZE_NONE		0

/*! ресайз */
#define RENDER_RESIZE_RESIZE	1

/*! переход между фуллскрин и окном */
#define RENDER_RESIZE_CHANGE	2

//! цвет очистки цветового буфера сцены по умолчанию
#define RENDER_DEFAUL_BACKGROUND_COLOR D3DCOLOR_ARGB(0,128,128,128)

#define G_DATA_LIGHT_FAR 100000 /*!< дальняя плоскость отсечения наблюдателя для света */

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

#define EDITORS_LEVEL_GROUPTYPE_GEOM	1	/*!< статическая геометрия */
#define EDITORS_LEVEL_GROUPTYPE_GREEN	2	/*!< растительность */
#define EDITORS_LEVEL_GROUPTYPE_GAME	3	/*!< игровые объекты */
#define EDITORS_LEVEL_GROUPTYPE_AIGRID	4	/*!< ai сетка */

//!@}

#define EDITORS_LEVEL_CAPTION "SXLevelEditor"
#define EDITORS_LEVEL_STATUSBAR_LEVEL_POLY "Level poly: "
#define EDITORS_LEVEL_STATUSBAR_GEOM_POLY "Geom poly: "
#define EDITORS_LEVEL_STATUSBAR_GREEN_POLY "Green poly: "
#define EDITORS_LEVEL_STATUSBAR_GAME_COUNT "Count game object: "

//##########################################################################

SX_LIB_API long SRender_0GetVersion();

//! установить функцию обработки сообщений
SX_LIB_API void SRender_Dbg_Set(report_func rf);

//! инициализция подсистемы
SX_LIB_API void SRender_0Create(
	const char *szName,			//!< присваиваемое имя
	HWND hWnd3D,
	HWND hWndParent3D,
	bool isUnic = true			//!< должна ли подсистема быть уникальной по имени
	);

//! уничтожить подсистему
SX_LIB_API void SRender_AKill();

//##########################################################################

SX_LIB_API HWND SRender_GetHandleWin3D();
SX_LIB_API HWND SRender_GetParentHandleWin3D();

SX_LIB_API void SRender_SetHandleWin3D(HWND hWnd);
SX_LIB_API void SRender_SetParentHandleWin3D(HWND hWnd);

SX_LIB_API void SRender_SetCamera(ISXCamera *pCamera);
SX_LIB_API ISXCamera* SRender_GetCamera();

SX_LIB_API void SRender_EditorCameraSetMove(bool canMove);
SX_LIB_API bool SRender_EditorCameraGetMove();

//**************************************************************************

//SX_LIB_API void SRender_SimModelSetPosition(const float3 *pPos);
SX_LIB_API void SRender_SimModelAdd(const char *szName);
SX_LIB_API ID SRender_SimModelGetIDMtl();

SX_LIB_API MTLTYPE_MODEL SRender_SimModelGetType();
SX_LIB_API void SRender_SimModelSetType(MTLTYPE_MODEL type);

SX_LIB_API void SRender_SimModelSetNumCurrModel(int iCurrNumModel);
SX_LIB_API int SRender_SimModelGetNumCurrModel();

SX_LIB_API void SRender_SimModelSetRotationY(float Rotation);
SX_LIB_API float SRender_SimModelGetRotationY();

//**************************************************************************

SX_LIB_API void SRender_EditorSetRenderGrid(bool canRender);
SX_LIB_API bool SRender_EditorGetRenderGrid();

SX_LIB_API void SRender_EditorSetRenderAxesStatic(bool canRender);
SX_LIB_API bool SRender_EditorGetRenderAxesStatic();

//**************************************************************************

SX_LIB_API ID SRender_EditorGetSelectTex();

//**************************************************************************

//##########################################################################

//! обработка вводы информации с клавиатуры
SX_LIB_API void SRender_UpdateInputKeyBoard(DWORD timeDelta);

//! обработка движения мыши вправо и влево
SX_LIB_API void SRender_UpdateInputMouseRotate(DWORD timeDelta);

//! обработка движения мыши вверх вниз
SX_LIB_API void SRender_UpdateInputMouseUpDown(DWORD timeDelta);

//! центрирвоание курсора мыши
SX_LIB_API void SRender_CentererCursor();

//! обновление навигации режим "редактор"
SX_LIB_API void SRender_UpdateEditorial(DWORD timeDelta);

//##########################################################################

//! обработка потери и восстановление устройства
SX_LIB_API void SRender_ComDeviceLost();

//! обработка видимости для источников света
SX_LIB_API void SRender_ComVisibleForLight();

//! обработка видимости для камеры
SX_LIB_API void SRender_ComVisibleForCamera();

//! обработка видимости для отражений
SX_LIB_API void SRender_ComVisibleReflection();


//! обработка и установка основных матриц, обработка плоскостей отсечения
SX_LIB_API void SRender_UpdateView();

//! вывод отладочной текстовой информации в окно рендера
SX_LIB_API void SRender_OutputDebugInfo(DWORD timeDelta, bool needGameTime);


//! построение G буфера, то есть рендер всей сцены
SX_LIB_API void SRender_BuildMRT(DWORD timeDelta, bool isRenderSimulation);

//! обновление информации о тенях (рендер всего того что отбрасывает тени в буферы глубин источников света)
SX_LIB_API void SRender_UpdateShadow(DWORD timeDelta);

//! обработка/обновление отражений
SX_LIB_API void SRender_UpdateReflection(DWORD timeDelta, bool isRenderSimulation);

SX_LIB_API void SRender_UpdateReflectionScene(DWORD timeDelta);

SX_LIB_API void SRender_UpdateReflectionSimModel(DWORD timeDelta);

//! отрисовка скайбокса и облаков
SX_LIB_API void SRender_RenderSky(DWORD timeDelta);

//! обработка освещения
SX_LIB_API void SRender_ComLighting(DWORD timeDelta);

//! объединение слоев прозрачности
SX_LIB_API void SRender_UnionLayers();

//! применение тонмаппинга к рт
SX_LIB_API void SRender_ApplyToneMapping();

//! просчет тонмаппинга
SX_LIB_API void SRender_ComToneMapping(DWORD timeDelta);


//! отрисовка партиклов (эффектов)
SX_LIB_API void SRender_RenderParticles(DWORD timeDelta);

//! отрисовка постпроцесса
SX_LIB_API void SRender_RenderPostProcess(DWORD timeDelta);

SX_LIB_API void SRender_ShaderRegisterData();

SX_LIB_API void SRender_RenderEditorMain();

//##########################################################################

//! сохранить скриншот
SX_LIB_API void SRender_SaveScreenShot();

//! сохранить рабочие текстуры (г-буфер и что к нему прилагается)
SX_LIB_API void SRender_SaveWorkTex();

//! изменить режим рендера (оконный/полноэкранный)
SX_LIB_API void SRender_ChangeModeWindow();

#endif