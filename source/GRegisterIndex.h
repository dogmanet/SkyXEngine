
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*! 
\file 
 Зарезервированные (именованые) номера регистров 
*/

#ifndef G_RI
#define G_RI

/*! @name float4x4 register */
//! @{
#define G_RI_MATRIX_WORLD			0	/*!< текущая мировая матрица */
#define G_RI_MATRIX_VIEW			1	/*!< текущая видовая матрица */
#define G_RI_MATRIX_PROJECTION		2	/*!< текущая матрица проекции */
#define G_RI_MATRIX_VIEWPROJ		3	/*!< текущая комбинированная матрица вида и проекции */
#define G_RI_MATRIX_TRANSP_VIEWPROJ 4	/*!< текущая транспонированная комбинированная матрица вида и проекции */
#define G_RI_MATRIX_OBSERVER_VIEW	5	/*!< матрица вида камеры наблюдателя */
#define G_RI_MATRIX_OBSERVER_PROJ	6	/*!< матрица проекции камеры наблюдателя */
#define G_RI_MATRIX_LIGHT_PROJ		7	/*!< матрица проекции для света */
//! @}

/*! @name bool register */
//! @{
#define G_RI_BOOL_CLIPPLANE0		0	/*!< использовать ли плоскость отсечения */
#define G_RI_BOOL_RENDER_WINDOWED	1	/*!< оконный рендер - true, полноэкранный - false */
//! @}

/*! @name float register */
//! @{
#define G_RI_FLOAT_OBSERVER_NEAR		0	/*!< ближняя плоскость отсечения наблюдателя */
#define G_RI_FLOAT_OBSERVER_FAR			1	/*!< дальняя плоскость отсечения наблюдателя или дальность видимости */
#define G_RI_FLOAT_OBSERVER_FOV			2	/*!< fov наблюдателя */
#define G_RI_FLOAT_WINSIZE_WIDTH		3	/*!< ширина окна рендера */
#define G_RI_FLOAT_WINSIZE_HEIGHT		4	/*!< высота окна рендера */
//! @}

/*! @name float3 register */
//! @{
#define G_RI_FLOAT3_CLIPPLANE0_NORMAL	0	/*!< нормаль плоскости отсечения */
#define G_RI_FLOAT3_CLIPPLANE0_POINT	1	/*!< точка на плоскости отсчения */
#define G_RI_FLOAT3_OBSERVER_POSITION	2	/*!< позиция наблюдателя */
#define G_RI_FLOAT3_OBSERVER_DIRECTION	3	/*!< направление взгляда наблюдателя */
//! @}

/*! @name int register */
//! @{
#define G_RI_INT_COUNT_POLY		0	/*!< общее количество полигонов */
#define G_RI_INT_COUNT_DIP		1	/*!< количество вызовов draw indexed primitive */
#define G_RI_INT_CURRIDLIGHT	2	/*!< текущий идентификатор источнкиа света */
#define G_RI_INT_RENDERSTATE	3	/*!< текущее состояние рендера */

#define G_RI_INT_TIMER_RENDER	4	/*!< время для рендера */
#define G_RI_INT_TIMER_GAME		5	/*!< время для игры */
//! @}

#endif
