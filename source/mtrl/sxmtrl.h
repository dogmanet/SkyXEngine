
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



#define SMtrl_MtlGetPhysicMaterial(id) MTLTYPE_PHYSIC_METAL
#define SMtrl_MtlGetDurability(id) 1.0f
#define SMtrl_MtlGetDensity(id) 1.0f
#define SMtrl_MtlGetHitChance(id) 1.0f


#endif

//!@} sxmtrl
