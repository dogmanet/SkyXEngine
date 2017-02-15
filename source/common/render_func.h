
#ifndef __render_func
#define __render_func

//ЗАНИМАЕМЫЕ РЕГИСТРЫ
//{
#define RENDER_RI_INT_CURRIDLIGHT	10
#define RENDER_RI_INT_RENDERSTATE	11
//}

//состояния рендера
#define RENDER_STATE_MATERIAL 0	//отрисовка материалов
#define RENDER_STATE_SHADOW 1	//отрисовка теней
#define RENDER_STATE_FREE 2		//простая отрисовка (не материальная)

//идентификаторы для определяния типа просчетов видимости
//хотя тип на самом деле это порядковое число
#define RENDER_IDARRCOM_GEOM 0	//геометрия
#define RENDER_IDARRCOM_GREEN 1	//растительность

namespace SXRenderFunc
{
	inline void SetSamplerFilter(DWORD id, DWORD value);
	inline void SetSamplerAddress(DWORD id, DWORD value);
	
	inline void SetSamplerFilter(DWORD begin_id, DWORD end_id, DWORD value);
	inline void SetSamplerAddress(DWORD begin_id, DWORD end_id, DWORD value);

	void UpdateLight();

	void ComDeviceLost();

	void UpdateView(DWORD timeDelta);
	void OutputDebugInfo(DWORD timeDelta);

	void RenderInMRT(DWORD timeDelta);
	void UpdateShadow(DWORD timeDelta);
	
	void RenderSky(DWORD timeDelta);
	void ComLighting(DWORD timeDelta, bool render_sky, bool blend_in_old);
	void GameRender(DWORD timeDelta);
	void LevelEditorRender(DWORD timeDelta);

	void RFuncDIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
	void RFuncSetMtl(UINT id, float4x4* world);
	long RFuncLoadMtl(const char* name, int mtl_type);
};

#endif