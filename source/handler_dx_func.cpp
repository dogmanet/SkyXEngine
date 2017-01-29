
#ifndef __handler_dx_func
#define __handler_dx_func

typedef void(*g_func_dip) (IDirect3DDevice9* device, UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
typedef void(*g_func_set_material) (IDirect3DDevice9* device, UINT stage, UINT id);

void StdDrawIndexedPrimitive(IDirect3DDevice9* device, UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	if (device)
	{
		device->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
	}
}

void StdSetMaterial(IDirect3DDevice9* device, UINT stage, UINT id)
{
	if (device)
	{
		device->SetTexture(stage, SGCore_LoadTexGetTex(id));
	}
}

g_func_dip FuncDIP = StdDrawIndexedPrimitive;
g_func_set_material FuncSetMaterial = StdSetMaterial;

#endif