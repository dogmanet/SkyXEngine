
/*!
\file
Заголовочный файл render_func - пространства имен с орагнизацией рендера*/

/*! \defgroup managed_render_render_func render_func - пространство имен с орагнизацией рендера
 \ingroup managed_render
@{*/

#ifndef __render_func
#define __render_func

#include <GRegisterIndex.h>

/*! \name Состояния рендера
@{*/

#define RENDER_STATE_MATERIAL	0	/*!< отрисовка материалов */
#define RENDER_STATE_SHADOW		1	/*!< отрисовка теней */
#define RENDER_STATE_FREE		2	/*!< простая отрисовка (не материальная) */

//!@}

/*! \name Идентификаторы для определяния типа просчетов видимости
@{*/

#define RENDER_IDARRCOM_GEOM	0	/*!< геометрия */
#define RENDER_IDARRCOM_GREEN	1	/*!< растительность */

//!@}

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
	//}

	//! основная функция которая запускает все остальное, вызывается в цикле
	void MainRender(DWORD timeDelta);

	/*! обработка каких-либо не рисующих сообщений, 
	в данной версии сохранение рабочих текстур и скришотов*/
	void UpdateMsg(DWORD timeDelta);

	//! обработка потери и восстановление устройства
	void ComDeviceLost();

	void UpdateView();						//!< обработка и установка основных матриц, обработка плоскостей отсечения
	void OutputDebugInfo(DWORD timeDelta);	//!< вывод отладочной текстовой информации в окно рендера

	void ComVisibleForLight();			//!< обработка видимости для источников света
	void ComVisibleForCamera();			//!< обработка видимости для камеры
	void ComVisibleReflection();		//!< обработка видимости для отражений

	void RenderInMRT(DWORD timeDelta);		//!< построение G буфера, то есть рендер всей сцены
	void UpdateShadow(DWORD timeDelta);		//!< обновление информации о тенях (рендер всего того что отбрасывает тени в буферы глубин источников света)
	void UpdateReflection(DWORD timeDelta);	//!< обработка/обновление отражений

	void RenderSky(DWORD timeDelta);					//!< отрисовка скайбокса и облаков
	void ComLighting(DWORD timeDelta, bool render_sky);	//!< обработка освещения, render_sky - рисовать ли небо

	void PostProcess(DWORD timeDelta);	//!< отрисовка постпроцесса

	/*! \name Функции обертки, для передачи графическому ядру для замены стандартных
	@{*/

	//! функция отрисовки, в данной версии не назначается
	void RFuncDIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
	//! функция установки материала по id, world - мировая матрица
	void RFuncMtlSet(ID id, float4x4* world);
	//! функция загрузки материала
	ID RFuncMtlLoad(const char* name, int mtl_type);
	
	//!@}

	//! время задержек/ожидания выполнения некоторых функций рендера
	namespace Delay
	{
		DWORD UpdateVisibleForCamera = 0;
		DWORD UpdateVisibleForLight = 0;
		DWORD UpdateVisibleForReflection = 0;

		DWORD UpdateShadow = 0;
		DWORD RenderMRT = 0;
		DWORD ComLighting = 0;
		DWORD PostProcess = 0;
		DWORD ComReflection = 0;
		DWORD GeomSortGroup = 0;

		DWORD Present = 0;
	};
};

#endif

//!@} managed_render_render_func