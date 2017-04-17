
#ifndef __render_func
#define __render_func

#include <GRegisterIndex.h>

//состо€ни€ рендера
#define RENDER_STATE_MATERIAL 0	//отрисовка материалов
#define RENDER_STATE_SHADOW 1	//отрисовка теней
#define RENDER_STATE_FREE 2		//проста€ отрисовка (не материальна€)

//идентификаторы дл€ определ€ни€ типа просчетов видимости
#define RENDER_IDARRCOM_GEOM 0	//геометри€
#define RENDER_IDARRCOM_GREEN 1	//растительность

namespace SXRenderFunc
{
	//быстра€ реализаци€ фильтрации и адресации
	//{
	//индифидуально дл€ регистра
	inline void SetSamplerFilter(DWORD id, DWORD value);
	inline void SetSamplerAddress(DWORD id, DWORD value);
	
	//дл€ указанного промежутка групп регистров
	inline void SetSamplerFilter(DWORD begin_id, DWORD end_id, DWORD value);
	inline void SetSamplerAddress(DWORD begin_id, DWORD end_id, DWORD value);
	//}

	//обработка каких-либо не рисущих сообщений
	//в данной версии сохранение рабочих текстур и скришотов
	void UpdateMsg(DWORD timeDelta);

	//обработка потери и восстановление устройства
	void ComDeviceLost();

	void UpdateView();	//обработка и установка основных матриц, обработка плоскостей отсечени€
	void OutputDebugInfo(DWORD timeDelta);	//вывод отладочной текстовой информации в окно рендера

	void ComVisibleForLight();	//обработка видимости дл€ источников света
	void ComVisibleForCamera();	//обработка видимости дл€ камеры
	void ComVisibleReflection();
	void ComReflection(DWORD timeDelta);	//обработка/обновление отражений

	void RenderInMRT(DWORD timeDelta);	//построение G буфера, то есть рендер всей сцены
	void UpdateShadow(DWORD timeDelta);	//обновление информации о тен€х (рендер всего того что отбрасывает тени в буферы глубин источников света)
	
	void RenderSky(DWORD timeDelta);	//отрисовка скайбокса и облаков
	void ComLighting(DWORD timeDelta, bool render_sky);	//обработка освещени€, render_sky - рисовать ли небо
	
	//виды рендеров
	void GameRender(DWORD timeDelta);
	void MainRender(DWORD timeDelta);
	void MaterialEditorRender(DWORD timeDelta);

	void PostProcess(DWORD timeDelta);	//отрисовка постпроцесса

	//функции обертки, дл€ передачи графическому €дру дл€ замены стандартных
	//{
	//функци€ отрисовки, в данной версии не назначаетс€
	void RFuncDIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
	//функци€ установки материала по id, world - мирова€ матрица
	void RFuncMtlSet(ID id, float4x4* world);
	//функци€ загрузки материала
	ID RFuncMtlLoad(const char* name, int mtl_type);
	//}

	//врем€ задержек/ожидани€ выполнени€ некоторых функций рендера
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