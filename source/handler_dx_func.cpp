
#ifndef __handler_dx_func
#define __handler_dx_func

typedef void(*g_func_dip) (UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count);
typedef void(*g_func_set_material) (UINT stage, UINT id);

void SXDrawIndexedPrimitive(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{

}

void SXSetMaterial(UINT stage,UINT id)
{

}

#endif