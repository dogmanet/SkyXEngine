
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxdecals - библиотека декалей
*/

/*! \defgroup sxdecals sxdecals - библиотека декалей
@{
*/


#ifndef _SXDECALS_H_
#define _SXDECALS_H_

#include <gdefines.h>
#include <common/SXMath.h>

enum DECAL_TYPE
{
	DECAL_TYPE_CUSTOM = -1,
	DECAL_TYPE_CONCRETE = 0,
	DECAL_TYPE_METAL,
	DECAL_TYPE_GLASS,
	DECAL_TYPE_PLASTIC,
	DECAL_TYPE_WOOD,
	DECAL_TYPE_FLESH,
	DECAL_TYPE_EARTH,
	DECAL_TYPE_BLOOD_BIG,

	DECAL_TYPE_LAST
};


/*! Инициализирует библиотеку
*/
SX_LIB_API void SXDecals_0Create();

/*! Деинициализирует библиотеку
*/
SX_LIB_API void SXDecals_AKill();

/*! Устанавливает функцию сообщений
*/
SX_LIB_API void SXDecals_Dbg_Set(report_func rf);


/*! Выполняет обновление физики
*/
SX_LIB_API void SXDecals_Update();

/*! Выполняет синхронизацию. Для многопоточного обновления
*/
SX_LIB_API void SXDecals_Sync();


/*! Выполняет отрисовку физических объектов
*/
SX_LIB_API void SXDecals_Render();

/*! Добавляет декаль
*/
SX_LIB_API void SXDecals_ShootDecal(DECAL_TYPE type, const float3 & fWorldPos, const float3 & normal);
SX_LIB_API void SXDecals_ShootDecalEx(DECAL_TYPE type, const float3 & fWorldPos, const float3 & normal, int flags, ID material = -1, float fScale = 1.0f, const float3 * saxis = NULL);


#endif

/*! @} */

