
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл render_func - пространства имен с орагнизацией рендера
*/

/*! \defgroup render_render_func render_func - пространство имен с орагнизацией рендера
 \ingroup render
@{*/

#ifndef __RENDER_FUNC_H
#define __RENDER_FUNC_H

#include <GRegisterIndex.h>
#include <windows.h>
#include <common/types.h>
#include <common/file_utils.h>

//#define SM_D3D_CONVERSIONS
#include <common/Math.h>

#include <render/gdata.h>

#include <green/sxgreen.h>
#include <aigrid/sxaigrid.h>
#include <physics/sxphysics.h>
#include <game/sxgame.h>
#include <particles/sxparticles.h>
#include <pp/sxpp.h>
#include <decals/sxdecals.h>

enum REFLECTION_RENDER
{
	REFLECTION_RENDER_ONLY_SKY	= 0,
	REFLECTION_RENDER_GEOM		= 1,
	REFLECTION_RENDER_GREEN		= 2,
	REFLECTION_RENDER_ANIM		= 3,
};

//! пространство имен с орагнизацией рендера
namespace rfunc
{
	//быстрая реализация фильтрации и адресации
	//{
	void SetRenderSceneFilter();
	void SetRenderSceneFilterUn();
	//}

	//**********************************************************************

	//! обработка потери и восстановление устройства
	bool ComDeviceLost(bool isSetWindowSize);

	//! обработка и установка основных матриц, обработка плоскостей отсечения
	void UpdateView();						

	//! обработка видимости для отражений
	void ComVisibleReflection();			

	

	//! отрисовка партиклов (эффектов)
	void RenderParticles(DWORD timeDelta);				

	//! отрисовка основного постпроцесса
	void RenderMainPostProcess(DWORD timeDelta);

	void RenderFinalPostProcess(DWORD timeDelta);

	//**********************************************************************

	//! сохранить скриншот
	void SaveScreenShot();		

	//! сохранить рабочие текстуры (г-буфер и что к нему прилагается)
	void SaveWorkTex();	

	//! установка режима окна по текущим значениям
	void InitModeWindow();

	//! изменить режим рендера (оконный/полноэкранный)
	void ChangeModeWindow();	
};

#endif

//!@} render_render_func
