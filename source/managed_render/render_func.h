
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл render_func - пространства имен с орагнизацией рендера
*/

/*! \defgroup managed_render_render_func render_func - пространство имен с орагнизацией рендера
 \ingroup managed_render
@{*/

#ifndef __RENDER_FUNC_H
#define __RENDER_FUNC_H

#include <GRegisterIndex.h>
#include <windows.h>
#include <common/sxtypes.h>

#define SM_D3D_CONVERSIONS
#include <common/SXMath.h>

#include <managed_render/gdata.h>
#include <managed_render/handler_log.h>
#include <managed_render/render_def.h>

#include <geom/sxgeom.h>
#include <mtllight/sxmtllight.h>
#include <aigrid/sxaigrid.h>
#include <physics/sxphysics.h>
#include <game/sxgame.h>
#include <particles/sxparticles.h>
#include <pp/sxpp.h>
#include <decals/sxdecals.h>


//! пространство имен с орагнизацией рендера
namespace SXRenderFunc
{
	//быстрая реализация фильтрации и адресации
	//{
	//индифидуально для регистра
	inline void SetSamplerFilter(DWORD id, DWORD value);
	inline void SetSamplerAddress(DWORD id, DWORD value);
	
	//для указанного промежутка групп регистров
	inline void SetSamplerFilter(DWORD begin_id, DWORD end_id, DWORD value);
	inline void SetSamplerAddress(DWORD begin_id, DWORD end_id, DWORD value);

	void SetRenderSceneFilter();
	void SetRenderSceneFilterUn();
	//}

	//**********************************************************************

	//! обработка потери и восстановление устройства
	void ComDeviceLost();

	void UpdateDataCVar();

	void UpdateView();						//!< обработка и установка основных матриц, обработка плоскостей отсечения
	void OutputDebugInfo(DWORD timeDelta);	//!< вывод отладочной текстовой информации в окно рендера

	void ComVisibleForLight();				//!< обработка видимости для источников света
	void ComVisibleForCamera();				//!< обработка видимости для камеры
	void ComVisibleReflection();			//!< обработка видимости для отражений

	void BuildMRT(DWORD timeDelta);		//!< построение G буфера, то есть рендер всей сцены

	void UpdateShadow(DWORD timeDelta);		//!< обновление информации о тенях (рендер всего того что отбрасывает тени в буферы глубин источников света)
	
	void UpdateReflection(DWORD timeDelta);	//!< обработка/обновление отражений
	void UpdateReflectionScene(DWORD timeDelta);
	void UpdateRflectionSimModel(DWORD timeDelta);
	
	void RenderSky(DWORD timeDelta);					//!< отрисовка скайбокса и облаков
	void ComLighting(DWORD timeDelta, bool render_sky);	//!< обработка освещения, render_sky - рисовать ли небо
	
	//! объединение слоев прозрачности
	void UnionLayers();

	//! применение тонмаппинга к рт
	void ApplyToneMapping();

	//! просчет тонмаппинга
	void ComToneMapping(DWORD timeDelta);
	
	void RenderParticles(DWORD timeDelta);				//!< отрисовка партиклов (эффектов)
	void RenderPostProcess(DWORD timeDelta);			//!< отрисовка постпроцесса

	void ShaderRegisterData();

	//**********************************************************************

	void RenderEditorMain();				//!< рендер основных элементов для редакторов
	void RenderEditorLE(DWORD timeDelta);	//!< рендер элементов для редактора уровней
	void RenderEditorPE(DWORD timeDelta);

	//**********************************************************************

	void SaveScreenShot();		//!< сохранить скриншот
	void SaveWorkTex();			//!< сохранить рабочие текстуры (г-буфер и что к нему прилагается)
	void ChangeModeWindow();	//!< изменить режим рендера (оконный/полноэкранный)

	//! время задержек/ожидания выполнения некоторых функций рендера
	namespace Delay
	{
		extern int64_t UpdateVisibleForCamera;
		extern int64_t UpdateVisibleForLight;
		extern int64_t UpdateVisibleForReflection;

		extern int64_t UpdateShadow;
		extern int64_t UpdateParticles;
		extern int64_t RenderMRT;
		extern int64_t ComLighting;
		extern int64_t PostProcess;
		extern int64_t ComReflection;
		extern int64_t GeomSortGroup;

		extern int64_t Present;

		extern int64_t FreeVal;
		extern float FreeValF1;
		extern float FreeValF2;
		extern float FreeValF3;
	};
};

#endif

//!@} managed_render_render_func
