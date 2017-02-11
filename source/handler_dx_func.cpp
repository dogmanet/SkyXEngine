
#ifndef __handler_dx_func
#define __handler_dx_func

typedef void(*g_func_dip) (IDirect3DDevice9* device, UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
typedef void(*g_func_set_material) (/*IDirect3DDevice9* device, */UINT id, float4x4* world);
typedef long(*g_func_load_material) (const char* name);

void StdDrawIndexedPrimitive(IDirect3DDevice9* device, UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	if (device)
	{
		device->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
	}
}

void StdSetMaterial(/*IDirect3DDevice9* device, */UINT id, float4x4* world)
{
	/*if (device)
	{
		//device->SetTexture(stage, SGCore_LoadTexGetTex(id));
	}*/

	if (GData::CurrStateRender == 2)
		SML_MatrialSetMainTexture(0, id);
	else
		SML_MatrialRender(id, world);
}

long StdLoadMaterial(const char* name)
{
	return SML_MatrialLoad(name);
}

g_func_dip FuncDIP = StdDrawIndexedPrimitive;
g_func_set_material FuncSetMaterial = StdSetMaterial;
g_func_load_material FuncLoadMaterial = StdLoadMaterial;

#endif