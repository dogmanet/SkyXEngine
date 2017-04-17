
/*! 
\file 
 Зарезервированные (именованые) номера регистров 
*/


#ifndef G_RI
#define G_RI
/*! @name float4x4 register */
//! @{
#define G_RI_MATRIX_WORLD		0	/*!< мировая матрица */
#define G_RI_MATRIX_VIEW		1	/*!< видовая матрица */
#define G_RI_MATRIX_PROJECTION	2	/*!< матрица проекции */
#define G_RI_MATRIX_VIEWPROJ	3	/*!< комбинированна¤ матрица вида и проекции */
#define G_RI_MATRIX_TRANSP_VIEWPROJ 4	/*!< транспонированна¤ комбинированна¤ матрица вида и проекции */
//! @}

/*! @name bool register */
//! @{
#define G_RI_BOOL_CLIPPLANE0	0	/*!< использовать ли плоскость отсечения */
//! @}

/*! @name float3 register */
//! @{
#define G_RI_FLOAT3_CLIPPLANE0_NORMAL	0	/*!< нормаль плоскости отсечения */
#define G_RI_FLOAT3_CLIPPLANE0_POINT	1	/*!< точка на плоскости отсчения */
//! @}

/*! @name int register */
//! @{
#define G_RI_INT_COUNT_POLY		0	/*!< общее количество полигонов */
#define G_RI_INT_COUNT_DIP		1	/*!< количество вызовов draw indexed primitive */
#define G_RI_INT_CURRIDLIGHT	2	/*!< текущий идентификатора источнкиа света */
#define G_RI_INT_RENDERSTATE	3	/*!< текущее состо¤ние рендера */
//! @}

#endif