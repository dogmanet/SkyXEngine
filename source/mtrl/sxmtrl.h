
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxmtrl - библиотеки материалов
*/

/*! \defgroup sxmtrl sxmtrl - библиотека материалов
@{
*/

#ifndef __SXMTRL_H
#define __SXMTRL_H

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

#include <gdefines.h>

#include <mtrl/IXMaterialSystem.h>

/*! \name Базовые функции библиотеки
@{*/
		
//! инициализция подсистемы
SX_LIB_API void SMtrl_0Create(
	const char *szName,				//!< присваиваемое имя
	bool isUnic = true,				//!< должна ли подсистема быть уникальной по имени
	bool isServerMode = false		//!< серверный режим (без графики)
	);

//! уничтожить подсистему
SX_LIB_API void SMtrl_AKill();

//!@}


//! обновление данных всей библиотеки
SX_LIB_API void SMtrl_Update(
	DWORD timeDelta	//!< время рендера кадра в млсек
	);

//#############################################################################

/*! \defgroup sxmtrl_mtl Система материалов
 \ingroup sxmtrl
@{*/

/*! максимальная длина имени директории материала */
#define MTL_MAX_SIZE_DIR 64		

/*! максимальная длина имени материала (без директории) */
#define MTL_MAX_SIZE_NAME 64	

/*! максимальное количество обновлений статических отражений, в первый момент рендера */
#define MTL_REF_UPDATE_MAX_COUNT_FOR_STATIC 3	

/*! \name Дистанции при которых будут сниженно или повышено количество отрисовок для отражений
меньше #MTL_REF_UPDATE_L0_DIST - обновление будет каждый кадр
больше #MTL_REF_UPDATE_L2_DIST - обновления не будет
@{*/

#define MTL_REF_UPDATE_L0_DIST 10	
#define MTL_REF_UPDATE_L1_DIST 30	
#define MTL_REF_UPDATE_L2_DIST 70	

//!@}

/*! \name Параметры альфа теста
 В данной реалзиации 2 вида альфа теста, принудительный (#SMtrl_MtlSetForceblyAlphaTest) и назначаемый непосредственно материалом
 \note функция альфа теста D3DCMP_GREATEREQUAL
@{*/

/*! значение свободного альфа теста (назначаемого материалом) */
#define MTL_ALPHATEST_FREE_VALUE 64		

/*! значение принудительного альфа теста */
#define MTL_ALPHATEST_FORCEBLY_VALUE 8	

//!@}

/*! \name Значения параметров освещения по умолчанию
 Данные значения по умолчанию будут применены к материалами которые не удалось загрузить, либо к стандартным материалам
@{*/

//! занчение шероховатости по умолчанию
#define MTL_LIGHTING_DEFAULT_ROUGHNESS 250.f/255.f

//! значение f0 (отражательной способности поверхности) по умолчанию
#define MTL_LIGHTING_DEFAULT_F0 2.f/255.f

//! значение thickness (толщина - на сколько будет использована диффузная составляющая, для твердых поверхностей - 1, для листьев деревьев и травы <1) по умолчанию
#define MTL_LIGHTING_DEFAULT_THICKNESS 255.f/255.f

//!@}

/*! \name Параметры отражений
@{*/

/*! размер текстур с отражениями */
#define MTL_REF_TEX_SIZE 512	

/*! ближняя плоскость отсечения */
#define MTl_REF_PROJ_NEAR 0.25	

/*! дальняя плоскость отсечения, или дистанция на которую будет рендер всего в текстуру отражений */
#define MTl_REF_PROJ_FAR 100	

//!@}

/*! \name Виртуальные директории
 \warning Имя виртуальной директории должно быть единичным символом (сделано для простоты сравнения), 
и как следствие нежелательно использоавть реальную директорию с таким именем для текстур либо материалов, 
и реальные директории не должны начинаться с данных символов
@{*/

//! имя виртуальной директории по умолчанию для "ручных" текстур с параметрами освещения
#define MTL_VIRTUAL_DIR_HAND_MTL '0'

//! имя виртуальной директории по умолчанию для стандартных (встроенных) материалов
#define MTL_VIRTUAL_DIR_STD_MTL "mtrldef"

//!@}

/*! \name Текстурные регистры
Распределение регистров между текстурами которые будут отправляться для формирования данных
@{*/

/*!< основная текстура */
#define MTL_TEX_R_MAIN		0	

/*!< маска */
#define MTL_TEX_R_MASK		1	

/*! стартовый регистр для детальных текстур
 \warning 4 следующих (включая указанный) будует заполнены детальными текстурами, поэтому никакие другие текстуры в [MTL_TEX_R_DETAIL, MTL_TEX_R_DETAIL+3] не должны отправляться
 \note Текстуры для каждого канала будут отправляться последовательно: r, g, b, a -  MTL_TEX_R_DETAIL, MTL_TEX_R_DETAIL + 1, MTL_TEX_R_DETAIL + 2, MTL_TEX_R_DETAIL + 3
*/
#define MTL_TEX_R_DETAIL	2

/*! стартовый регистр для микрорельефных текстур
\warning 4 следующих (включая указанный) будует заполнены микрорельефными текстурами, поэтому никакие другие текстуры в [MTL_TEX_R_MICRO, MTL_TEX_R_MICRO+3] не должны отправляться
\note Текстуры для каждого канала будут отправляться последовательно: r, g, b, a -  MTL_TEX_R_MICRO, MTL_TEX_R_MICRO + 1, MTL_TEX_R_MICRO + 2, MTL_TEX_R_MICRO + 3
*/
#define MTL_TEX_R_MICRO		6


/*! текстура с параметрами освещения */
#define MTL_TEX_R_PARAM_LIGHT	10	

/*! текстура с отражениями */
#define MTL_TEX_R_REFLECTION	12	

/*! текстура с преломлениями */
#define MTL_TEX_R_REFRACTION	13	

/*! текстура с текущей глубиной */
#define MTL_TEX_R_CURR_DEPTH	14	

//!@}

//! типы отражений
enum MTLTYPE_REFLECT
{
	//! нет отражения
	MTLTYPE_REFLECT_NONE,			

	//! плоское
	MTLTYPE_REFLECT_PLANE,			

	//! объемное динамическое
	MTLTYPE_REFLECT_CUBE_DYNAMIC,	

	//! объемное статическое, обработка идет первые несколько кадров, затем не обновляется
	MTLTYPE_REFLECT_CUBE_STATIC		
};

//! сорт материала
enum MTLSORT
{
	//! непрозрачный
	MTLSORT_OPAQUE = 0x00000001,

	//! прозрачный
	MTLTYPE_TRANSPARENCY = 0x00000010,

	//! непрозрачный
	MTLSORT_LIGHTED = 0x00000100,

	//! прозрачный
	MTLTYPE_UNLIT = 0x00001000,

	//! непрозрачный освещаемый
	/*MTLSORT_OPAQUE_LIGHTED,

	//! непрозрачный не освещаемый
	MTLSORT_OPAQUE_UNLIT,*/


	//! полупрозрачный освещаемый
	/*MTLTYPE_TRANSPARENCY_LIGHTED,	

	//! полупрозрачный не освещаемый
	MTLTYPE_TRANSPARENCY_UNLIT,*/
};

/*! \name Возможные слои
@{*/

//! прозрачные неосвещаемые
#define MTLTYPE_UNLIT	0

//! непрозрачные освещаемые
#define MTLTYPE_LIGHT	1

//!@}


//! данные отправляемые в шейдеры
enum MTL_SHADERSTD
{
	//! мировая матрица (трансформации модели), world
	MTL_SHADERSTD_MATRIX_WORLD = 0,		

	//! матрица вида, view
	MTL_SHADERSTD_MATRIX_VIEW,			

	//! матрица проекции, projection
	MTL_SHADERSTD_MATRIX_PROJECTION,	

	//! world * view
	MTL_SHADERSTD_MATRIX_WORLDVIEW,		

	//! world * view * projection
	MTL_SHADERSTD_MATRIX_WORLDVIEWPROJ,	

	//! позиция камеры/наблюдателя
	MTL_SHADERSTD_CAMPOS,				

	//! данные о времени float2(CountTimeRender,TimeDelta)
	MTL_SHADERSTD_TIMEDELTA,			

	//! размеры окна рендера
	MTL_SHADERSTD_WINSIZE,				

	//! пользовательские данные (float4)
	MTL_SHADERSTD_USERDATA				
};

#define SMtrl_MtlGetPhysicMaterial(id) MTLTYPE_PHYSIC_METAL



/*! \defgroup sxmtrl_mtl_propslayer Cвойства поверхности
 \ingroup sxmtrl_mtl
@{*/


#define SMtrl_MtlGetDurability(id) 1.0f
#define SMtrl_MtlGetDensity(id) 1.0f
#define SMtrl_MtlGetHitChance(id) 1.0f

//!@} sxmtrl_mtl

#endif

//!@} sxmtrl
