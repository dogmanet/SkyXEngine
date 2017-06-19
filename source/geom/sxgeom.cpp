
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#define SXGEOM_VERSION 1

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

#include <geom\\sxgeom.h>

#include <geom\\static_geom.cpp>
#include <geom\\green.cpp>
#include <geom\\geom_data.cpp>

StaticGeom* GeometryObj = 0;
Green* GreenObj = 0;

#define GEOM_PRECOND(retval) if(!GeometryObj || !GreenObj){reportf(-1, "%s - sxgeom is not init", gen_msg_location); return retval;}

long SGeom_0GetVersion()
{
	return SXGEOM_VERSION;
}

void SGeom_Dbg_Set(report_func rf)
{
	reportf = rf;
}

void SGeom_0Create(const char* name, IDirect3DDevice9* device, const char* std_path_mesh, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				reportf(-1, "%s - none unic name, sxgeom", gen_msg_location);
			}
			else
			{
				GeomDataInit(device, std_path_mesh);
				GeometryObj = new StaticGeom();
				GreenObj = new Green();
			}
		}
		else
		{
			GeomDataInit(device, std_path_mesh);
			GeometryObj = new StaticGeom();
			GreenObj = new Green();
		}
	}
	else
		reportf(-1, "%s - not init argument [name], sxgeom", gen_msg_location);
}

void SGeom_AKill()
{
	mem_delete(GeometryObj);
	mem_delete(GreenObj);
}

void SGeom_OnLostDevice()
{
	GEOM_PRECOND(_VOID);

	GreenObj->OnLostDevice();
	GeometryObj->OnLostDevice();
}

void SGeom_OnResetDevice()
{
	GEOM_PRECOND(_VOID);

	GreenObj->OnResetDevice();
	GeometryObj->OnResetDevice();
}

/////////

void SGeom_0SettModelsSetSortFrontToBackModels(bool val)
{

}

bool SGeom_0SettModelsGetSortFrontToBackModels()
{
	return false;
}


void SGeom_0SettModelsSetSortFrontToBackSplits(bool val)
{
	StaticGeom::UseSortFrontToBackSplits = val;
}

bool SGeom_0SettModelsGetSortFrontToBackSplits()
{
	return StaticGeom::UseSortFrontToBackSplits;
}


float SGeom_0SettGreenGetDistLods1()
{
	return Green::DistLods.x;
}

float SGeom_0SettGreenGetDistLods2()
{
	return Green::DistLods.y;
}

int SGeom_0SettGreenGetFreqGrass()
{
	return Green::CurrentFreqGrass;
}

float SGeom_0SettGreenGetBeginEndLessening()
{
	return Green::BeginEndLessening;
}

bool SGeom_0SettGreenGetSortFrontToBackSplits()
{
	return Green::UseSortFrontToBackSplits;
}


void SGeom_0SettGreenSetDistLods1(float val)
{
	Green::DistLods.x = val;
}

void SGeom_0SettGreenSetDistLods2(float val)
{
	Green::DistLods.y = val;
}

void SGeom_0SettGreenSetFreqGrass(int val)
{
	Green::CurrentFreqGrass = val;
}

void SGeom_0SettGreenSetBeginEndLessening(float val)
{
	Green::BeginEndLessening = val;
}

void SGeom_0SettGreenSetSortFrontToBackSplits(bool val)
{
	Green::UseSortFrontToBackSplits = val;
}


/////////

void SGeom_ModelsClear()
{
	GEOM_PRECOND(_VOID);
	GeometryObj->Clear();
}

void SGeom_ModelsSave(const char* path)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->Save(path);
}

void SGeom_ModelsLoad(const char* path)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->Load(path);
}


long SGeom_ModelsGetCount()
{
	GEOM_PRECOND(-1);
	return GeometryObj->GetCountModel();
}


void SGeom_ModelsComVisible(ISXFrustum* frustum, float3* viewpos, ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->CPUFillingArrIndeces(frustum, viewpos, id_arr);
}

bool SGeom_ModelsSortExistsForRender(int sort, ID id_arr)
{
	GEOM_PRECOND(false);
	return GeometryObj->SortExistsForRender(sort, id_arr);
}


void SGeom_ModelsRender(DWORD timeDelta, int sort_mtl, ID id_arr, bool is_sorted, ID exclude_model_id, ID exclude_group_id)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->GPURender(timeDelta, sort_mtl, id_arr, exclude_model_id, exclude_group_id, is_sorted);
}

void SGeom_ModelsRenderSingly(DWORD timeDelta, ID id, ID id_tex)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->GPURenderSingly(timeDelta, id, id_tex);
}

ID SGeom_ModelsAddModel(const char* path, const char* lod1, const char* name)
{
	GEOM_PRECOND(-1);
	return GeometryObj->AddModel(path, lod1, name);
}

void SGeom_ModelsDelModel(ID id)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->DelModel(id);
}


void SGeom_ModelsGetMinMax(float3* min, float3* max)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->GetMinMax(min, max);
}


ID SGeom_ModelsAddArrForCom()
{
	GEOM_PRECOND(-1);
	return GeometryObj->AddArrForCom();
}

void SGeom_ModelsDelArrForCom(ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->DelArrForCom(id_arr);
}


char* SGeom_ModelsMGetName(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelName(id);
}

const char* SGeom_ModelsMGetPathName(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelPathName(id);
}

long SGeom_ModelsMGetCountPoly(ID id)
{
	GEOM_PRECOND(-1);
	return GeometryObj->GetModelCountPoly(id);
}


float3* SGeom_ModelsMGetPosition(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelPosition(id);
}

float3* SGeom_ModelsMGetRotation(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelRotation(id);
}

float3* SGeom_ModelsMGetScale(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelScale(id);
}


const char* SGeom_ModelsMGetLodPath(ID id)
{
	GEOM_PRECOND(0);
	return GeometryObj->GetModelLodPath(id);
}

void SGeom_ModelsMSetLodPath(ID id, const char* path)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->SetModelLodPath(id, path);
}


void SGeom_ModelsMApplyTransform(ID id)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->ApplyTransform(id);
}


void SGeom_ModelsMSortGroups(float3* viewpos, int sort_mtl)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->SortGroup(viewpos, sort_mtl);
}


ID SGeom_ModelsMGetCountGroups(ID id)
{
	GEOM_PRECOND(-1);
	return GeometryObj->GetModelCountGroups(id);
}

ID SGeom_ModelsMGetGroupIDMat(ID id, ID group)
{
	GEOM_PRECOND(-1);
	return GeometryObj->GetModelGroupIDMat(id, group);
}

void SGeom_ModelsMGetGroupCenter(ID id, ID group, float3_t* center)
{
	GEOM_PRECOND(_VOID);
	return GeometryObj->GetModelGroupCenter(id, group, center);
}

void SGeom_ModelsMGetGroupMin(ID id, ID group, float3_t* min)
{
	GEOM_PRECOND(_VOID);
	return GeometryObj->GetModelGroupMin(id, group, min);
}

void SGeom_ModelsMGetGroupMax(ID id, ID group, float3_t* max)
{
	GEOM_PRECOND(_VOID);
	return GeometryObj->GetModelGroupMax(id, group, max);
}

void SGeom_ModelsMGetGroupPlane(ID id, ID group, D3DXPLANE* plane)
{
	GEOM_PRECOND(_VOID);
	return GeometryObj->GetModelGroupPlane(id, group, plane);
}



void SGeom_ModelsGetArrBuffsGeom(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, ID*** arr_mtl, int32_t** arr_count_index, int32_t* count_models)
{
	GEOM_PRECOND(_VOID);
	GeometryObj->GetArrBuffsGeom(arr_vertex, arr_count_vertex, arr_index, arr_mtl, arr_count_index, count_models);
}

void SGeom_ModelsClearArrBuffsGeom(float3_t** arr_vertex, int32_t* arr_count_vertex, uint32_t** arr_index, ID** arr_mtl, int32_t* arr_count_index, int32_t count_models)
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

bool SGeom_ModelsTraceBeam(float3* start, float3* dir, float3* _res, ID* idmodel, ID* idmtl)
{
	GEOM_PRECOND(false);
	return GeometryObj->TraceBeam(start, dir, _res, idmodel, idmtl);
}

///////////

ID SGeom_GreenAddGreen(const char* name,
	const char* path_mask,
	float count_max,
	const char* path, const char* lod1, const char* lod2,
	const char* navmesh)
{
	GEOM_PRECOND(-1);
	return GreenObj->Init(GeometryObj, name, path_mask, count_max, path, lod1, lod2, navmesh);
}

ID SGeom_GreenAddObject(ID id, float3* pos, ID* idsplit)
{
	GEOM_PRECOND(-1);
	return GreenObj->AddObject(id, pos, 0, idsplit);
}

void SGeom_GreenDelObject(ID id, ID idsplit, ID idobj)
{
	GEOM_PRECOND(_VOID);
	return GreenObj->DelObject(id, idsplit, idobj);
}

void SGeom_GreenGetPosObject(ID id, ID idsplit, ID idobj, float3_t* pos)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GetPosObject(id, idsplit, idobj, pos);
}

void SGeom_GreenSetPosObject(ID id, ID* idsplit, ID* idobj, float3_t* pos)
{
	GEOM_PRECOND(_VOID);
	GreenObj->SetPosObject(id, idsplit, idobj, pos);
}

void SGeom_GreenDelGreen(ID id)
{
	GEOM_PRECOND(_VOID);
	GreenObj->DelGreen(id);
}


ID SGeom_GreenAddArrForCom()
{
	GEOM_PRECOND(-1);
	return GreenObj->AddArrForCom();
}

void SGeom_GreenDelArrForCom(ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GreenObj->DelArrForCom(id_arr);
}


void SGeom_GreenSave(const char* path)
{
	GEOM_PRECOND(_VOID);
	GreenObj->Save(path);
}

void SGeom_GreenLoad(const char* path)
{
	GEOM_PRECOND(_VOID);
	GreenObj->Load(path);
}

void SGeom_GreenClear()
{
	GEOM_PRECOND(_VOID);
	GreenObj->Clear();
}

void SGeom_GreenComVisible(ISXFrustum* frustum, float3* viewpos, ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GreenObj->CPUFillingArrIndeces(frustum, viewpos, id_arr);
}

void SGeom_GreenRender(DWORD timeDelta, float3* viewpos, GeomGreenType type, ID id_arr)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GPURender(timeDelta, viewpos, type, id_arr);
}

void SGeom_GreenRenderSingly(DWORD timeDelta, float3* viewpos, ID id, ID id_tex)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GPURenderSingly(timeDelta, viewpos, id, id_tex);
}

void SGeom_GreenRenderObject(DWORD timeDelta,float3* viewpos,ID id,ID split,ID idobj,ID id_tex)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GPURenderObject(timeDelta, viewpos, id, split, idobj, id_tex);
}


long SGeom_GreenGetCount()
{
	GEOM_PRECOND(-1);
	return GreenObj->GetCountGreen();
}

char* SGeom_GreenMGetName(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenName(id);
}

long SGeom_GreenMGetCountGen(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenCountGen(id);
}

long SGeom_GreenMGetCountPoly(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenCountPoly(id);
}

int SGeom_GreenMGetTypeCountGen(ID id)
{
	GEOM_PRECOND(-1);
	return GreenObj->GetGreenTypeCountGen(id);
}

const char* SGeom_GreenMGetModel(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenModel(id);
}

const char* SGeom_GreenMGetLod1(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenLod1(id);
}

const char* SGeom_GreenMGetLod2(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenLod2(id);
}

const char* SGeom_GreenMGetMask(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenMask(id);
}

const char* SGeom_GreenMGetNav(ID id)
{
	GEOM_PRECOND(0);
	return GreenObj->GetGreenNav(id);
}

void SGeom_GreenMSetLod(ID id, int lod, const char* pathname)
{
	GEOM_PRECOND(_VOID);
	GreenObj->SetGreenLod(id, lod, pathname);
}

void SGeom_GreenMSetNav(ID id, const char* pathname)
{
	GEOM_PRECOND(_VOID);
	GreenObj->SetGreenNav(id, pathname);
}


void SGeom_GreenGetNavMeshAndTransform(float3_t*** arr_vertex, int32_t** arr_count_vertex, uint32_t*** arr_index, ID*** arr_mtl, int32_t** arr_count_index, GreenDataVertex*** arr_transform, int32_t** arr_count_transform, int32_t* arr_count_green)
{
	GEOM_PRECOND(_VOID);
	GreenObj->GetNavMeshAndTransform(arr_vertex, arr_count_vertex, arr_index, arr_mtl, arr_count_index, arr_transform, arr_count_transform, arr_count_green);
}

void SGeom_GreenClearNavMeshAndTransform(float3_t** arr_vertex, int32_t* arr_count_vertex, uint32_t** arr_index, ID** arr_mtl, int32_t* arr_count_index, GreenDataVertex** arr_transform, int32_t* arr_count_transform, int32_t arr_count_green)
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

bool SGeom_GreenTraceBeam(float3* start, float3* dir, float3* _res, ID* idgreen, ID* idsplits, ID* idobj, ID* idmtl)
{
	GEOM_PRECOND(false);
	return GreenObj->TraceBeam(start, dir, _res, idgreen, idsplits, idobj, idmtl);
}

bool SGeom_GreenGetOccurencessLeafGrass(float3* bbmin, float3* bbmax, int physic_mtl)
{
	GEOM_PRECOND(false);
	return GreenObj->GetOccurencessLeafGrass(bbmin, bbmax, physic_mtl);
}
