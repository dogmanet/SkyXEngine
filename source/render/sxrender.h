
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


#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>
#include <geom/sxgeom.h>
#include <mtllight/sxmtllight.h>
#include <anim/sxanim.h>
#include <pp/sxpp.h>
#include <particles/sxparticles.h>

/*#ifdef SX_DLL
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif*/

#define SX_LIB_API
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

//! обработка потери и восстановление устройства
SX_LIB_API void ComDeviceLost();

//! обработка видимости для источников света
SX_LIB_API void ComVisibleForLight();				

//! обработка видимости для камеры
SX_LIB_API void ComVisibleForCamera();				

//! обработка видимости для отражений
SX_LIB_API void ComVisibleReflection();			


SX_LIB_API void UpdateDataCVar();

SX_LIB_API void UpdateView();						//!< обработка и установка основных матриц, обработка плоскостей отсечения
SX_LIB_API void OutputDebugInfo(DWORD timeDelta);	//!< вывод отладочной текстовой информации в окно рендера



SX_LIB_API void BuildMRT(DWORD timeDelta);		//!< построение G буфера, то есть рендер всей сцены
SX_LIB_API void UpdateShadow(DWORD timeDelta);		//!< обновление информации о тенях (рендер всего того что отбрасывает тени в буферы глубин источников света)
SX_LIB_API void UpdateReflection(DWORD timeDelta);	//!< обработка/обновление отражений

void RenderSky(DWORD timeDelta);					//!< отрисовка скайбокса и облаков
void ComLighting(DWORD timeDelta, bool render_sky);	//!< обработка освещения, render_sky - рисовать ли небо
void RenderParticles(DWORD timeDelta);				//!< отрисовка партиклов (эффектов)
void RenderPostProcess(DWORD timeDelta);			//!< отрисовка постпроцесса
void ShaderRegisterData();
/*
void SaveScreenShot();		//!< сохранить скриншот
void SaveWorkTex();			//!< сохранить рабочие текстуры (г-буфер и что к нему прилагается)
void ChangeModeWindow();	//!< изменить режим рендера (оконный/полноэкранный)
*/
#endif