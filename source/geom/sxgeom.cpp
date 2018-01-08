
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SXGEOM_VERSION 1

#include "sxgeom.h"

#include "static_geom.h"
#include "green.h"

bool StaticGeom::UseSortFrontToBackSplits = true;
bool StaticGeom::UseSortFrontToBackModels = true;
IDirect3DDevice9* StaticGeom::DXDevice = 0;
float StaticGeom::DistForLod = 200.f;

int Green::CurrentFreqGrass = 100;
float2_t Green::DistLods = float2_t(50, 100);
float Green::BeginEndLessening = 30;
bool Green::UseSortFrontToBackSplits = false;
IDirect3DDevice9* Green::DXDevice = 0;

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

StaticGeom* GeometryObj = 0;
Green* GreenObj = 0;

#define GEOM_PRECOND(retval) if(!GeometryObj || !GreenObj){g_fnReportf(-1, "%s - sxgeom is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SGeom_0GetVersion()
{
	return SXGEOM_VERSION;
}

SX_LIB_API void SGeom_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SGeom_0Create(const char *szName, bool isUnic)
{
	if (szName && strlen(szName) > 1)
	{
		if (isUnic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, szName);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				g_fnReportf(-1, "%s - none unic name, sxgeom", GEN_MSG_LOCATION);
			}
			else
			{
				StaticGeom::DXDevice = SGCore_GetDXDevice();
				Green::DXDevice = SGCore_GetDXDevice();
				GeometryObj = new StaticGeom();
				GreenObj = new Green();
			}
		}
		else
		{
			StaticGeom::DXDevice = SGCore_GetDXDevice();
			Green::DXDevice = SGCore_GetDXDevice();
			GeometryObj = new StaticGeom();
			GreenObj = new Green();
		}
	}
	else
		g_fnReportf(-1, "%s - not init argument [name], sxgeom", GEN_MSG_LOCATION);
}

SX_LIB_API void SGeom_AKill()
{
	mem_delete(GeometryObj);
	mem_delete(GreenObj);
}

SX_LIB_API void SGeom_OnLostDevice()
{
	GEOM_PRECOND(_VOID);

	GreenObj->OnLostDevice();
	GeometryObj->OnLostDevice();
}

SX_LIB_API void SGeom_OnResetDevice()
{
	GEOM_PRECOND(_VOID);

	GreenObj->OnResetDevice();
	GeometryObj->OnResetDevice();
}

//##########################################################################

SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackModels(bool val)
{

}

SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackModels()
{
	return false;
}


SX_LIB_API void SGeom_0SettModelsSetSortFrontToBackSplits(bool val)
{
	StaticGeom::UseSortFrontToBackSplits = val;
}

SX_LIB_API bool SGeom_0SettModelsGetSortFrontToBackSplits()
{
	return StaticGeom::UseSortFrontToBackSplits;
}


SX_LIB_API float SGeom_0SettGreenGetDistLods1()
{
	return Green::DistLods.x;
}

SX_LIB_API float SGeom_0SettGreenGetDistLods2()
{
	return Green::DistLods.y;
}

SX_LIB_API int SGeom_0SettGreenGetFreqGrass()
{
	return Green::CurrentFreqGrass;
}

SX_LIB_API float SGeom_0SettGreenGetBeginEndLessening()
{
	return Green::BeginEndLessening;
}

SX_LIB_API bool SGeom_0SettGreenGetSortFrontToBackSplits()
{
	return Green::UseSortFrontToBackSplits;
}


SX_LIB_API void SGeom_0SettGreenSetDistLods1(float val)
{
	Green::DistLods.x = val;
}

SX_LIB_API void SGeom_0SettGreenSetDistLods2(float val)
{
	Green::DistLods.y = val;
}

SX_LIB_API void SGeom_0SettGreenSetFreqGrass(int val)
{
	Green::CurrentFreqGrass = val;
}

SX_LIB_API void SGeom_0SettGreenSetBeginEndLessening(float val)
{
	Green::BeginEndLessening = val;
}

SX_LIB_API void SGeom_0SettGreenSetSortFrontToBackSplits(bool val)
{
	Green::UseSortFrontToBackSplits = val;
}

//##########################################################################

SX_LIB_API void SGeom_ModelsClear()
{
	GEOM_PRECOND(_VOID);
	GeometryObj->Clear();
}

SX_LIB_API void SGeom_ModelsSave(const char* path)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->Save(path);
}

SX_LIB_API void SGeom_ModelsLoad(const char* path)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->Load(path);
}

SX_LIB_API long SGeom_ModelsGetCount()
{
	GEOM_PRECOND(-1);
	return GeometryObj->GetCountModel();
}

SX_LIB_API void SGeom_ModelsComVisible(ISXFrustum* frustum, float3* viewpos, ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->CPUFillingArrIndeces(frustum, viewpos, id_arr);
}

SX_LIB_API bool SGeom_ModelsSortExistsForRender(int sort, ID id_arr)
{
	GEOM_PRECOND(false);
	return GeometryObj->SortExistsForRender(sort, id_arr);
}

SX_LIB_API void SGeom_ModelsRender(DWORD timeDelta, int sort_mtl, ID id_arr, bool is_sorted, ID exclude_model_id, ID exclude_group_id)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->GPURender(timeDelta, sort_mtl, id_arr, exclude_model_id, exclude_group_id, is_sorted);
}

SX_LIB_API void SGeom_ModelsRenderSingly(DWORD timeDelta, ID id, ID id_tex)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->GPURenderSingly(timeDelta, id, id_tex);
}

SX_LIB_API ID SGeom_ModelsAddModel(const char* path, const char* lod1, const char* name)
{
	GEOM_PRECOND(-1);
	return GeometryObj->AddModel(path, lod1, name);
}

SX_LIB_API void SGeom_ModelsDelModel(ID id)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->DelModel(id);
}

SX_LIB_API void SGeom_ModelsGetMinMax(float3* min, float3* max)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->GetMinMax(min, max);
}

SX_LIB_API ID SGeom_ModelsAddArrForCom()
{
	GEOM_PRECOND(-1);
	return GeometryObj->AddArrForCom();
}

SX_LIB_API bool SGeom_ModelsExistsArrForCom(ID id)
{
	GEOM_PRECOND(false);
	return GeometryObj->existsArrForCom(id);
}

SX_LIB_API void SGeom_ModelsDelArrForCom(ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->DelArrForCom(id_arr);
}


SX_LIB_API void SGeom_ModelsMGetMinMax(ID id, float3* min, float3* max)
{
	GEOM_PRECOND(_VOID);
	return GeometryObj->GetModelMinMax(id, min, max);
}

SX_LIB_API char* SGeom_ModelsMGetName(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelName(id);
}

SX_LIB_API const char* SGeom_ModelsMGetPathName(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelPathName(id);
}

SX_LIB_API long SGeom_ModelsMGetCountPoly(ID id)
{
	GEOM_PRECOND(-1);
	return GeometryObj->GetModelCountPoly(id);
}


SX_LIB_API float3* SGeom_ModelsMGetPosition(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelPosition(id);
}

SX_LIB_API float3* SGeom_ModelsMGetRotation(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelRotation(id);
}

SX_LIB_API float3* SGeom_ModelsMGetScale(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelScale(id);
}


SX_LIB_API const char* SGeom_ModelsMGetLodPath(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelLodPath(id);
}

SX_LIB_API void SGeom_ModelsMSetLodPath(ID id, const char* path)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->SetModelLodPath(id, path);
}


SX_LIB_API void SGeom_ModelsMApplyTransform(ID id)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->ApplyTransform(id);
}


SX_LIB_API void SGeom_ModelsMSortGroups(float3* viewpos, int sort_mtl)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->SortGroup(viewpos, sort_mtl);
}


SX_LIB_API ID SGeom_ModelsMGetCountGroups(ID id)
{
	GEOM_PRECOND(-1);
	return GeometryObj->GetModelCountGroups(id);
}

SX_LIB_API ID SGeom_ModelsMGetGroupIDMat(ID id, ID group)
{
	GEOM_PRECOND(-1);
	return GeometryObj->GetModelGroupIDMat(id, group);
}

SX_LIB_API void SGeom_ModelsMGetGroupCenter(ID id, ID group, float3_t* center)
{
	GEOM_PRECOND(_VOID);
	return GeometryObj->GetModelGroupCenter(id, group, center);
}

SX_LIB_API void SGeom_ModelsMGetGroupMin(ID id, ID group, float3_t* min)
{
	GEOM_PRECOND(_VOID);
	return GeometryObj->GetModelGroupMin(id, group, min);
}

SX_LIB_API void SGeom_ModelsMGetGroupMax(ID id, ID group, float3_t* max)
{
	GEOM_PRECOND(_VOID);
	return GeometryObj->GetModelGroupMax(id, group, max);
}

SX_LIB_API void SGeom_ModelsMGetGroupPlane(ID id, ID group, D3DXPLANE* plane)
{
	GEOM_PRECOND(_VOID);
	return GeometryObj->GetModelGroupPlane(id, group, plane);
}



SX_LIB_API void SGeom_ModelsGetArrBuffsGeom(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, ID*** arr_mtl, int32_t** arr_count_index, int32_t* count_models)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->GetArrBuffsGeom(arr_vertex, arr_count_vertex, arr_index, arr_mtl, arr_count_index, count_models);
}

SX_LIB_API void SGeom_ModelsClearArrBuffsGeom(float3_t** arr_vertex, int32_t* arr_count_vertex, uint32_t** arr_index, ID** arr_mtl, int32_t* arr_count_index, int32_t count_models)
{
	for(int32_t i = 0; i < count_models; ++i)
	{
		mem_delete_a(arr_vertex[i]);
		mem_delete_a(arr_index[i]);
		mem_delete_a(arr_mtl[i]);
	}
	mem_delete_a(arr_vertex);
	mem_delete_a(arr_count_vertex);
	mem_delete_a(arr_index);
	mem_delete_a(arr_mtl);
	mem_delete_a(arr_count_index);
}

SX_LIB_API bool SGeom_ModelsTraceBeam(float3* start, float3* dir, float3* _res, ID* idmodel, ID* idmtl)
{
	GEOM_PRECOND(false);
	return GeometryObj->TraceBeam(start, dir, _res, idmodel, idmtl);
}

//##########################################################################

SX_LIB_API ID SGeom_GreenAddGreen(const char* name,
	const char* path_mask,
	float count_max,
	const char* path, const char* lod1, const char* lod2,
	const char* navmesh)
{
	GEOM_PRECOND(-1);
	return GreenObj->Init(GeometryObj, name, path_mask, count_max, path, lod1, lod2, navmesh);
}

SX_LIB_API ID SGeom_GreenAddObject(ID id, float3* pos, ID* idsplit)
{
	GEOM_PRECOND(-1);
	return GreenObj->AddObject(id, pos, 0, idsplit);
}

SX_LIB_API void SGeom_GreenDelObject(ID id, ID idsplit, ID idobj)
{
	GEOM_PRECOND(_VOID);
	return GreenObj->DelObject(id, idsplit, idobj);
}

SX_LIB_API void SGeom_GreenGetPosObject(ID id, ID idsplit, ID idobj, float3_t* pos)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GetPosObject(id, idsplit, idobj, pos);
}

SX_LIB_API void SGeom_GreenSetPosObject(ID id, ID* idsplit, ID* idobj, float3_t* pos)
{
	GEOM_PRECOND(_VOID);
	GreenObj->SetPosObject(id, idsplit, idobj, pos);
}

SX_LIB_API void SGeom_GreenDelGreen(ID id)
{
	GEOM_PRECOND(_VOID);
	GreenObj->DelGreen(id);
}

SX_LIB_API ID SGeom_GreenAddArrForCom()
{
	GEOM_PRECOND(-1);
	return GreenObj->AddArrForCom();
}

SX_LIB_API bool SGeom_GreenExistsArrForCom(ID id)
{
	GEOM_PRECOND(false);
	return GreenObj->existsArrForCom(id);
}

SX_LIB_API void SGeom_GreenDelArrForCom(ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GreenObj->DelArrForCom(id_arr);
}

SX_LIB_API void SGeom_GreenSave(const char* path)
{
	GEOM_PRECOND(_VOID);
	GreenObj->Save(path);
}

SX_LIB_API void SGeom_GreenLoad(const char* path)
{
	GEOM_PRECOND(_VOID);
	GreenObj->Load(path);
}

SX_LIB_API void SGeom_GreenClear()
{
	GEOM_PRECOND(_VOID);
	GreenObj->Clear();
}

SX_LIB_API void SGeom_GreenComVisible(ISXFrustum* frustum, float3* viewpos, ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GreenObj->CPUFillingArrIndeces(frustum, viewpos, id_arr);
}

SX_LIB_API void SGeom_GreenRender(DWORD timeDelta, float3* viewpos, GREEN_TYPE type, ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GPURender(timeDelta, viewpos, type, id_arr);
}

SX_LIB_API void SGeom_GreenRenderSingly(DWORD timeDelta, float3* viewpos, ID id, ID id_tex)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GPURenderSingly(timeDelta, viewpos, id, id_tex);
}

SX_LIB_API void SGeom_GreenRenderObject(DWORD timeDelta, float3* viewpos, ID id, ID split, ID idobj, ID id_tex)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GPURenderObject(timeDelta, viewpos, id, split, idobj, id_tex);
}


SX_LIB_API long SGeom_GreenGetCount()
{
	GEOM_PRECOND(-1);
	return GreenObj->GetCountGreen();
}

SX_LIB_API char* SGeom_GreenMGetName(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenName(id);
}

SX_LIB_API long SGeom_GreenMGetCountGen(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenCountGen(id);
}

SX_LIB_API long SGeom_GreenMGetCountPoly(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenCountPoly(id);
}

SX_LIB_API int SGeom_GreenMGetTypeCountGen(ID id)
{
	GEOM_PRECOND(-1);
	return GreenObj->GetGreenTypeCountGen(id);
}

SX_LIB_API const char* SGeom_GreenMGetModel(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenModel(id);
}

SX_LIB_API const char* SGeom_GreenMGetLod1(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenLod1(id);
}

SX_LIB_API const char* SGeom_GreenMGetLod2(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenLod2(id);
}

SX_LIB_API const char* SGeom_GreenMGetMask(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenMask(id);
}

SX_LIB_API const char* SGeom_GreenMGetNav(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenNav(id);
}

SX_LIB_API void SGeom_GreenMSetLod(ID id, int lod, const char* pathname)
{
	GEOM_PRECOND(_VOID);
	GreenObj->SetGreenLod(id, lod, pathname);
}

SX_LIB_API void SGeom_GreenMSetNav(ID id, const char* pathname)
{
	GEOM_PRECOND(_VOID);
	GreenObj->SetGreenNav(id, pathname);
}


SX_LIB_API void SGeom_GreenGetNavMeshAndTransform(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, ID*** arr_mtl, int32_t** arr_count_index, GreenDataVertex*** arr_transform, int32_t** arr_count_transform, int32_t* arr_count_green)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GetNavMeshAndTransform(arr_vertex, arr_count_vertex, arr_index, arr_mtl, arr_count_index, arr_transform, arr_count_transform, arr_count_green);
}

SX_LIB_API void SGeom_GreenClearNavMeshAndTransform(float3_t** arr_vertex, int32_t* arr_count_vertex, uint32_t** arr_index, ID** arr_mtl, int32_t* arr_count_index, GreenDataVertex** arr_transform, int32_t* arr_count_transform, int32_t arr_count_green)
{
	GEOM_PRECOND(_VOID);
	
	for (int i = 0; i < arr_count_green; ++i)
	{
		mem_delete_a(arr_vertex[i]);
		mem_delete_a(arr_index[i]);
		mem_delete_a(arr_mtl[i]);
		mem_delete_a(arr_transform[i]);
	}
	mem_delete_a(arr_vertex);
	mem_delete_a(arr_count_vertex);
	mem_delete_a(arr_index);
	mem_delete_a(arr_mtl);
	mem_delete_a(arr_transform);
	mem_delete_a(arr_count_transform);
	mem_delete_a(arr_count_index);
}

SX_LIB_API bool SGeom_GreenTraceBeam(float3* start, float3* dir, float3* _res, ID* idgreen, ID* idsplits, ID* idobj, ID* idmtl)
{
	GEOM_PRECOND(false);
	return GreenObj->TraceBeam(start, dir, _res, idgreen, idsplits, idobj, idmtl);
}

SX_LIB_API bool SGeom_GreenGetOccurencessLeafGrass(float3* bbmin, float3* bbmax, int physic_mtl)
{
	GEOM_PRECOND(false);
	return GreenObj->GetOccurencessLeafGrass(bbmin, bbmax, physic_mtl);
}
