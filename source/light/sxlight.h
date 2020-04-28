
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxlight - библиотеки освещения
*/

/*! \defgroup sxlight sxlight - библиотека освещения
@{
*/

#ifndef __SXLIGHT_H
#define __SXLIGHT_H

#if defined(_DEBUG)
#pragma comment(lib, "sxgcore_d.lib")
#else
#pragma comment(lib, "sxgcore.lib")
#endif

#if defined(_DEBUG)
#pragma comment(lib, "sxphysics_d.lib")
#else
#pragma comment(lib, "sxphysics.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#include "IXLight.h"

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

#include <gdefines.h>

/*! \name Базовые функции библиотеки
@{*/

//! версия подсистемы
SX_LIB_API long SLight_0GetVersion();			

//! инициализция подсистемы
SX_LIB_API void SLight_0Create(
	const char *szName,				//!< присваиваемое имя
	bool isUnic = true				//!< должна ли подсистема быть уникальной по имени
	);

//! уничтожить подсистему
SX_LIB_API void SLight_AKill();

//!@}

#endif

//!@} sxlight
