
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
#include <common/sxtypes.h>
#include <common/file_utils.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>

#include <render/gdata.h>

#include <geom/sxgeom.h>
#include <light/sxlight.h>
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
	//индифидуально для регистра
	inline void SetSamplerFilter(DWORD dwId, DWORD dwValue);
	inline void SetSamplerAddress(DWORD dwId, DWORD dwValue);
	
	//для указанного промежутка групп регистров
	inline void SetSamplerFilter(DWORD dwIdStart, DWORD dwIdFisnish, DWORD dwValue);
	inline void SetSamplerAddress(DWORD dwIdStart, DWORD dwIdFisnish, DWORD dwValue);

	void SetRenderSceneFilter();
	void SetRenderSceneFilterUn();
	//}

	//**********************************************************************

	//! обработка потери и восстановление устройства
	void ComDeviceLost(bool isSetWindowSize);

	//! обработка и установка основных матриц, обработка плоскостей отсечения
	void UpdateView();						

	//! вывод отладочной текстовой информации в окно рендера
	int OutputDebugInfo(DWORD timeDelta, bool needGameTime, const char *szStr = 0);

	//! обработка видимости для источников света
	void ComVisibleForLight();				

	//! обработка видимости для камеры
	void ComVisibleForCamera();				

	//! обработка видимости для отражений
	void ComVisibleReflection();			


	//! построение G буфера, то есть рендер всей сцены
	void BuildMRT(DWORD timeDelta, bool isRenderSimulation);		


	//! обновление информации о тенях (рендер всего того что отбрасывает тени в буферы глубин источников света)
	void UpdateShadow(DWORD timeDelta);		
	

	//! обработка/обновление отражений
	void UpdateReflection(DWORD timeDelta, bool isRenderSimulation);	
	void UpdateReflectionScene(DWORD timeDelta);
	void UpdateReflectionSimModel(DWORD timeDelta);
	

	//! отрисовка скайбокса и облаков
	void RenderSky(DWORD timeDelta);					

	//! обработка освещения, render_sky - рисовать ли небо
	void ComLighting(DWORD timeDelta);	
	
	//! объединение слоев прозрачности
	void UnionLayers();
	

	//! отрисовка партиклов (эффектов)
	void RenderParticles(DWORD timeDelta);				

	//! отрисовка основного постпроцесса
	void RenderMainPostProcess(DWORD timeDelta);

	void RenderFinalPostProcess(DWORD timeDelta);

	void ShaderRegisterData();

	//**********************************************************************

	//! рендер основных элементов для редакторов
	void RenderEditorMain();				

	//**********************************************************************

	//! сохранить скриншот
	void SaveScreenShot();		

	//! сохранить рабочие текстуры (г-буфер и что к нему прилагается)
	void SaveWorkTex();	

	//! установка режима окна по текущим значениям
	void InitModeWindow();

	//! изменить режим рендера (оконный/полноэкранный)
	void ChangeModeWindow();	

	void FullScreenChangeSizeAbs();
};

#endif

//!@} render_render_func
