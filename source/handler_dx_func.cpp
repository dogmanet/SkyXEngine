
#ifndef __handler_dx_func
#define __handler_dx_func

//состояния рендера
#define RENDER_STATE_MATERIAL 0	//отрисовка материалов
#define RENDER_STATE_SHADOW 1	//отрисовка теней
#define RENDER_STATE_FREE 2		//простая отрисовка (не материальная)

/*int GRenderState = RENDER_STATE_FREE;	//текущее состояние рендера (для правильного назначения текстур/материалов)
long GAllCountPoly = 0;	//количество полигонов прошедшее рендер
long GAllCountDIPs = 0;	//количество dip вызовов прошедшее рендер

void GNullingRTRenderData()
{
	GAllCountPoly = GAllCountDIPs = 0;
}*/

/*typedef void(*g_func_dip) (IDirect3DDevice9* device, UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
typedef void(*g_func_set_material) (UINT id, float4x4* world);
typedef long(*g_func_load_material) (const char* name, int mtl_type);

void StdDrawIndexedPrimitive(IDirect3DDevice9* device, UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	if (device)
	{
		device->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
		GAllCountPoly += prim_count;
		++GAllCountDIPs;
	}
}

void StdSetMaterial(UINT id, float4x4* world)
{
	if (GRenderState == RENDER_STATE_SHADOW)
	{
		SML_MtlSetMainTexture(0, id);
		SML_LigthsShadowSetShaderOfTypeMat(Core_RLongGet(0), SML_MtlGetType(id), world);
	}
	else if (GRenderState == RENDER_STATE_FREE)
	{
		SML_MtlSetMainTexture(0, id);
	}
	else if (GRenderState == RENDER_STATE_MATERIAL)
		SML_MtlRender(id, world);
}

long StdLoadMaterial(const char* name, int mtl_type = MTL_GEOM)
{
	return SML_MtlLoad(name, mtl_type);
}

g_func_dip FuncDIP = StdDrawIndexedPrimitive;
g_func_set_material FuncSetMaterial = StdSetMaterial;
g_func_load_material FuncLoadMaterial = StdLoadMaterial;*/

#endif