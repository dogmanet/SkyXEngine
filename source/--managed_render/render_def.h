
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл render_def - дефайны/контсанты рендера
*/

#ifndef __RENDER_DEF_H
#define __RENDER_DEF_H

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

//!@} managed_render_render_def

#endif