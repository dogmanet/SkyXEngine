
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#include <gcore\sxgcore.h>

#pragma once

//флаги компил€ции шейдеров
#define SHADER_DEBUG D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3DXSHADER_AVOID_FLOW_CONTROL | D3DXSHADER_SKIPOPTIMIZATION
#define SHADER_RELEASE D3DXSHADER_OPTIMIZATION_LEVEL3 | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY | D3DXSHADER_PARTIALPRECISION | D3DXSHADER_PREFER_FLOW_CONTROL

//определ€ем флаг компилции шейдеров
#if defined(DEBUG) || defined(_DEBUG) 
#define SHADER_FLAGS SHADER_DEBUG 
#else 
#define SHADER_FLAGS SHADER_RELEASE 
#endif

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func reportf = def_report;
#endif

IDirect3DDevice9* DXDevice = 0;
D3DPRESENT_PARAMETERS D3DAPP;
IDirect3D9* d3d9 = 0;
ID3DXFont* FPSText = 0;


///

void StdDrawIndexedPrimitive(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	Core_RIntSet(G_RI_INT_COUNT_DIP, Core_RIntGet(G_RI_INT_COUNT_DIP) + 1);
	DXDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
}

void StdMtlSet(ID id, float4x4* world)
{

	DXDevice->SetTexture(0, SGCore_LoadTexGetTex(id));
}

ID StdMtlLoad(const char* name, int mtl_type)
{
	return SGCore_LoadTexAddName(name, LoadTexType::ltt_load);
}

int StdMtlGetSort(ID id)
{
	return 0;
}

int StdMtlGetPhysicType(ID id)
{
	return 0;
}

bool StdMtlGroupIsSyngly(ID id)
{
	return false;
}

g_func_dip FuncDIP = StdDrawIndexedPrimitive;
g_func_mtl_set FuncMtlSet = StdMtlSet;
g_func_mtl_load FuncMtlLoad = StdMtlLoad;
g_func_mtl_get_sort FuncMtlGetSort = StdMtlGetSort;
g_func_mtl_get_physic_type FuncMtlGetPhysicType = StdMtlGetPhysicType;
g_func_mtl_group_render_is_singly FuncMtlGroupRenderIsSingly = StdMtlGroupIsSyngly;
///

#include <common\string.cpp>
#include <gcore\shader.cpp>
#include <gcore\creatortextures.cpp>
#include <gcore\loadertextures.cpp>
#include <gcore\baseobject.cpp>
#include <gcore\workmodel.cpp>
#include <gcore\camera.cpp>

IDirect3DVertexDeclaration9* StaticVertexDecl = 0;
#include <gcore\\loader_static.cpp>
#include <gcore\\sky.cpp>

#define SXGCORE_VERSION 1

ShaderManager* MShaders = 0;
CreatorTextures* MRenderTargets = 0;
LoaderTextures* MTextures = 0;
ID3DXMesh* ScreenTexture = 0;
SkyBox* ObjSkyBox = 0;
SkyClouds* ObjSkyClouds = 0;


#define SG_PRECOND(retval) if(!DXDevice){reportf(-1, "%s - sxgcore is not init", gen_msg_location); return retval;}
#define SG_PRECOND_SKY_BOX(retval) SG_PRECOND(retval _VOID); if(!ObjSkyBox){reportf(-1, "%s - sky_box is not init", gen_msg_location); return retval;}
#define SG_PRECOND_SKY_CLOUDS(retval) SG_PRECOND(retval _VOID); if(!ObjSkyClouds){reportf(-1, "%s - sky_clouds is not init", gen_msg_location); return retval;}

#include <gcore\dxdevice.cpp>

void GCoreInit(HWND hwnd, int width, int heigth, bool windowed, DWORD create_device_flags)
{
	int cerr = InitD3D(hwnd, windowed, width, heigth, create_device_flags);
	if (cerr == SXGC_ERR_NON_DETECTED_D3D)
		reportf(-1, "%s - none detected d3d, sxgcore", gen_msg_location);
	else if (cerr == SXGC_ERR_FAILED_INIT_D3D)
		reportf(-1, "%s - failed initialized d3d, sxgcore", gen_msg_location);

	//устанавливаем данные в регистры
	Core_RFloatSet(G_RI_FLOAT_WINSIZE_WIDTH, (float)width);
	Core_RFloatSet(G_RI_FLOAT_WINSIZE_HEIGHT, (float)heigth);
	Core_RBoolSet(G_RI_BOOL_RENDER_WINDOWED, windowed);

	MShaders = new ShaderManager();
	MRenderTargets = new CreatorTextures();
	MTextures = new LoaderTextures();

	D3DVERTEXELEMENT9 layoutstatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	DXDevice->CreateVertexDeclaration(layoutstatic, &StaticVertexDecl);
}

long SGCore_0GetVersion()
{
	return SXGCORE_VERSION;
}

void SGCore_Dbg_Set(report_func rf)
{
	reportf = rf;
}

void SGCore_0Create(const char* name, HWND hwnd, int width, int heigth, bool windowed, DWORD create_device_flags, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				reportf(-1, "%s - none unic name, sxgcore", gen_msg_location);
				return;
			}
		}
		GCoreInit(hwnd, width, heigth, windowed, create_device_flags);
	}
	else
		reportf(-1, "%s - not init argument [name], sxgcore", gen_msg_location);
}

void SGCore_AKill()
{
	mem_delete(MShaders);
	mem_delete(MRenderTargets);
	mem_delete(MTextures);

	mem_release(ScreenTexture);
	mem_delete(ObjSkyBox);
	mem_delete(ObjSkyClouds);

	mem_release(FPSText);
	mem_release(DXDevice);
	mem_release(d3d9);
}

IDirect3DDevice9* SGCore_GetDXDevice()
{
	return DXDevice;
}

void SGCore_DbgMsg(const char* format, ...)
{
	SG_PRECOND(_VOID);
	
	va_list va;
	char buf[SXGC_STR_SIZE_DBG_MSG];
	va_start(va, format);
	vsprintf_s(buf, SXGC_STR_SIZE_DBG_MSG, format, va);
	va_end(va);

	RECT rect;
	rect.top = 10;
	rect.left = 10;
	rect.right = D3DAPP.BackBufferWidth - 10;
	rect.bottom = D3DAPP.BackBufferHeight - 10;
	FPSText->DrawText(0, buf, -1, &rect, 0, 0xff000000);
}

void SGCore_OnLostDevice()
{
	SG_PRECOND(_VOID);

	FPSText->OnLostDevice();
	MRenderTargets->OnLostDevice();
}

bool SGCore_OnDeviceReset(int width, int heigth, bool windowed)
{
	SG_PRECOND(false);
	D3DAPP.BackBufferWidth = width;
	D3DAPP.BackBufferHeight = heigth;
	D3DAPP.Windowed = windowed;

	Core_RFloatSet(G_RI_FLOAT_WINSIZE_WIDTH, (float)width);
	Core_RFloatSet(G_RI_FLOAT_WINSIZE_HEIGHT, (float)heigth);
	Core_RBoolSet(G_RI_BOOL_RENDER_WINDOWED, windowed);

	return (FAILED(DXDevice->Reset(&D3DAPP)));
}

void SGCore_OnResetDevice()
{
	SG_PRECOND(_VOID);

	FPSText->OnResetDevice();
	MRenderTargets->OnResetDevice();
	ScreenQuadOnResetDevice();
}

void SGCore_ScreenQuadDraw()
{
	SG_PRECOND(_VOID);
	ScreenTexture->DrawSubset(0);
}

//////

void SGCore_DIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	SG_PRECOND(_VOID);
	FuncDIP(type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
}

void SGCore_MtlSet(ID id, float4x4* world)
{
	SG_PRECOND(_VOID);
	FuncMtlSet(id, world);
}

ID SGCore_MtlLoad(const char* name, int mtl_type)
{
	SG_PRECOND(-1);
	return FuncMtlLoad(name, mtl_type);
}

int SGCore_MtlGetSort(ID id)
{
	SG_PRECOND(-1);
	return FuncMtlGetSort(id);
}

int SGCore_MtlGetPhysicType(ID id)
{
	SG_PRECOND(-1);
	return FuncMtlGetPhysicType(id);
}

bool SGCore_MtlGroupRenderIsSingly(ID id)
{
	SG_PRECOND(false);
	return FuncMtlGroupRenderIsSingly(id);
}


void SGCore_SetFunc_DIP(g_func_dip func)
{
	SG_PRECOND(_VOID);
	FuncDIP = func;
}

void SGCore_SetFunc_MtlSet(g_func_mtl_set func)
{
	SG_PRECOND(_VOID);
	FuncMtlSet = func;
}

void SGCore_SetFunc_MtlLoad(g_func_mtl_load func)
{
	SG_PRECOND(_VOID);
	FuncMtlLoad = func;
}

void SGCore_SetFunc_MtlGetSort(g_func_mtl_get_sort func)
{
	SG_PRECOND(_VOID);
	FuncMtlGetSort = func;
}

void SGCore_SetFunc_MtlGetPhysicType(g_func_mtl_get_physic_type func)
{
	SG_PRECOND(_VOID);
	FuncMtlGetPhysicType = func;
}

void SGCore_SetFunc_MtlGroupRenderIsSingly(g_func_mtl_group_render_is_singly func)
{
	SG_PRECOND(_VOID);
	FuncMtlGroupRenderIsSingly = func;
}


////////////

ID SGCore_ShaderLoad(ShaderType type_shader, const char* path, const char* name, ShaderCheckDouble is_check_double, D3DXMACRO* macro)
{
	SG_PRECOND(-1);

	return MShaders->Load(type_shader, path, name, is_check_double, macro);
}

void SGCore_ShaderUpdateN(ShaderType type_shader, const char* name, D3DXMACRO macro[])
{
	SG_PRECOND(_VOID);

	MShaders->Update(type_shader, name, macro);
}

void SGCore_ShaderUpdate(ShaderType type_shader, ID id, D3DXMACRO macro[])
{
	SG_PRECOND(_VOID);

	MShaders->Update(type_shader, id, macro);
}

void SGCore_ShaderReloadAll()
{
	SG_PRECOND(_VOID);

	MShaders->ReloadAll();
}

ID SGCore_ShaderGetID(ShaderType type_shader, const char* shader)
{
	SG_PRECOND(-1);

	return MShaders->GetID(type_shader, shader);
}

void SGCore_ShaderBindN(ShaderType type_shader, const char* shader)
{
	SG_PRECOND(_VOID);

	return MShaders->Bind(type_shader, shader);
}

void SGCore_ShaderBind(ShaderType type_shader, ID shader)
{
	SG_PRECOND(_VOID);

	return MShaders->Bind(type_shader, shader);
}


void SGCore_ShaderUnBind()
{
	SG_PRECOND(_VOID);

	return MShaders->UnBind();
}

void SGCore_ShaderSetVRFN(ShaderType type_shader, const char* name_shader, const char* name_var, void* data, int count_float4)
{
	SG_PRECOND(_VOID);

	return MShaders->SetValueRegisterF(type_shader, name_shader, name_var, data, count_float4);
}

void SGCore_ShaderSetVRF(ShaderType type_shader, ID num_shader, const char* name_var, void* data, int count_float4)
{
	SG_PRECOND(_VOID);

	return MShaders->SetValueRegisterF(type_shader, num_shader, name_var, data, count_float4);
}

void SGCore_ShaderSetVRIN(ShaderType type_shader, const char* name_shader, const char* name_var, void* data, int count_int4)
{
	SG_PRECOND(_VOID);

	return MShaders->SetValueRegisterI(type_shader, name_shader, name_var, data, count_int4);
}

void SGCore_ShaderSetVRI(ShaderType type_shader, ID num_shader, const char* name_var, void* data, int count_int4)
{
	SG_PRECOND(_VOID);

	return MShaders->SetValueRegisterI(type_shader, num_shader, name_var, data, count_int4);
}


ID SGCore_ShaderIsExistName(ShaderType type_shader, const char* name)
{
	SG_PRECOND(-1);

	return MShaders->IsExistName(type_shader, name);
}

ID SGCore_ShaderIsExistPath(ShaderType type_shader, const char* name)
{
	SG_PRECOND(-1);

	return MShaders->IsExistName(type_shader, name);
}

bool SGCore_ShaderIsValidate(ShaderType type_shader, ID id)
{
	SG_PRECOND(0);

	return MShaders->IsValidate(type_shader, id);
}

void SGCore_ShaderGetPath(ShaderType type_shader, ID id, char* path)
{
	SG_PRECOND(_VOID);

	MShaders->GetPath(type_shader, id, path);
}

void SGCore_ShaderGetName(ShaderType type_shader, ID id, char* name)
{
	SG_PRECOND(_VOID);

	MShaders->GetName(type_shader, id, name);
}

void SGCore_ShaderSetStdPath(const char* path)
{
	SG_PRECOND(_VOID);

	MShaders->SetStdPath(path);
}

void SGCore_ShaderGetStdPath(char* path)
{
	SG_PRECOND(_VOID);

	MShaders->GetStdPath(path);
}

////////////////////////

void SGCore_LoadTexStdPath(const char* path)
{
	SG_PRECOND(_VOID);

	MTextures->SetStdPath(path);
	/*ObjSkyBox->SetStdPath(path);
	ObjSkyClouds->SetStdPath(path);*/
}

void SGCore_LoadTexClearLoaded()
{
	SG_PRECOND(_VOID);

	MTextures->ClearLoaded();
}

void SGCore_LoadTexDelete(ID id)
{
	SG_PRECOND(_VOID);

	MTextures->Delete(id);
}

ID SGCore_LoadTexAddName(const char* name, LoadTexType type)
{
	SG_PRECOND(-1);

	return MTextures->AddName(name, type);
}

ID SGCore_LoadTexGetID(const char* name)
{
	SG_PRECOND(-1);

	return MTextures->GetID(name);
}

void SGCore_LoadTexGetName(ID id, char* name)
{
	SG_PRECOND(_VOID);

	return MTextures->GetName(id,name);
}

ID SGCore_LoadTexCreate(const char* name, IDirect3DTexture9* tex)
{
	SG_PRECOND(-1);

	return MTextures->Create(name, tex);
}

ID SGCore_LoadTexUpdateN(const char* name, LoadTexType type)
{
	SG_PRECOND(-1);

	return MTextures->Update(name, type);
}

void SGCore_LoadTexUpdate(ID id)
{
	SG_PRECOND(_VOID);

	return MTextures->Update(id);
}

IDirect3DTexture9* SGCore_LoadTexGetTex(ID id)
{
	SG_PRECOND(0);

	return MTextures->GetTexture(id);
}

void SGCore_LoadTexLoadTextures()
{
	SG_PRECOND(_VOID);

	return MTextures->LoadTextures();
}

//////////////////////////

ID SGCore_RTAdd(UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, const char* name, float coeffullscreen)
{
	SG_PRECOND(-1);

	return MRenderTargets->Add(width, height, levels, usage, format, pool, name, coeffullscreen);
}

void SGCore_RTDeleteN(const char* text)
{
	SG_PRECOND(_VOID);

	return MRenderTargets->Delete(text);
}

void SGCore_RTDelete(ID num)
{
	SG_PRECOND(_VOID);

	return MRenderTargets->Delete(num);
}

ID SGCore_RTGetNum(const char* text)
{
	SG_PRECOND(-1);

	return MRenderTargets->GetNum(text);
}

/*void SGCore_RTOnLostDevice()
{
	SG_PRECOND();

	return MRenderTargets->OnLostDevice();
}

void SGCore_RTOnResetDevice()
{
	SG_PRECOND();

	return MRenderTargets->OnResetDevice();
}*/

IDirect3DTexture9* SGCore_RTGetTextureN(const char* text)
{
	SG_PRECOND(0);

	return MRenderTargets->GetTexture(text);
}

IDirect3DTexture9* SGCore_RTGetTexture(ID num)
{
	SG_PRECOND(0);

	return MRenderTargets->GetTexture(num);
}

////////////

void SGCore_FCreateCone(float fTopRadius, float fBottomRadius, float fHeight, ID3DXMesh ** ppMesh, UINT iSideCount)
{
	SG_PRECOND(_VOID);

	CreateCone(fTopRadius, fBottomRadius, fHeight, ppMesh, DXDevice, iSideCount);
}

void SGCore_FCompBoundBox(IDirect3DVertexBuffer9* vertex_buffer, ISXBound** bound, DWORD count_vert, DWORD bytepervert)
{
	SG_PRECOND(_VOID);

	ComputeBoundingBox(vertex_buffer, bound, count_vert, bytepervert);
}

void SGCore_FCompBoundBox2(IDirect3DVertexBuffer9* vertex_buffer, ISXBound* bound, DWORD count_vert, DWORD bytepervert)
{
	SG_PRECOND(_VOID);

	ComputeBoundingBox2(vertex_buffer, bound, count_vert, bytepervert);
}

void SGCore_FCreateBoundingBoxMesh(float3* min, float3* max, ID3DXMesh** bbmesh)
{
	SG_PRECOND(_VOID);

	CreateBoundingBoxMesh(min, max, bbmesh,DXDevice);
}

void SGCore_OptimizeIndecesInSubsetUint16(uint16_t* ib, uint16_t numFaces, uint16_t numVerts)
{
	SG_PRECOND(_VOID);

	OptimizeIndecesInSubsetUint16(ib, numFaces, numVerts);
}

void SGCore_OptimizeIndecesInSubsetUint32(uint32_t* ib, uint32_t numFaces, uint32_t numVerts)
{
	SG_PRECOND(_VOID);

	OptimizeIndecesInSubsetUint32(ib, numFaces, numVerts);
}


bool SGCore_0InPos2D(float3* min, float3* max, float3* pos)
{
	return InPosition2D(min, max, pos);
}

bool SGCore_0InPosAbs2D(float3* min, float3* max, float3* pos)
{
	return InPositionAbs2D(min, max, pos);
}

int SGCore_0CountPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPoints2D(min, max, p1, p2, p3);
}

int SGCore_0CountPosPointsAbs2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPointsAbs2D(min, max, p1, p2, p3);
}

bool SGCore_0InPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return InPositionPoints2D(min, max, p1, p2, p3);
}

bool SGCore_0InPos3D(float3* min, float3* max, float3* pos)
{
	return InPosition3D(min, max, pos);
}

bool SGCore_0InPosAbs3D(float3* min, float3* max, float3* pos)
{
	return InPositionAbs3D(min, max, pos);
}

int SGCore_0CountPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPoints3D(min, max, p1, p2, p3);
}

int SGCore_0CountPosPointsAbs3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPointsAbs3D(min, max, p1, p2, p3);
}

bool SGCore_0InPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return InPositionPoints3D(min, max, p1, p2, p3);
}

void SGCore_0ComBoundBoxArr8(ISXBound* bound, ISXBound** bound_arr)
{
	ComputeBoundingBoxArr8(bound, bound_arr);
}

void SGCore_0ComBoundBoxArr4(ISXBound* bound, ISXBound** bound_arr)
{
	ComputeBoundingBoxArr4(bound, bound_arr);
}

float SGCore_0DistancePointBeam2(const float3 & p, const float3 & start, const float3 & dir)
{
	float3 v = dir;
	float3 w = p - start;
	float c1;
	if ((c1 = SMVector3Dot(w, v)) <= 0.0f)
	{
		return(SMVector3Length2(p - start));
	}
	float c2 = SMVector3Dot(v, v);

	float b = c1 / c2;
	float3 Pb = start + b * v;
	return(SMVector3Length2(p - Pb));
}

float SGCore_0InretsectBox(const float3 * min1, const float3 * max1, const float3 * min2, const float3 * max2)
{
	return (!((min1->x > max2->x || max1->x < min2->x)
		|| (min1->y > max2->y || max1->y < min2->y)
		|| (min1->z > max2->z || max1->z < min2->z)));
}

////////////

ISXFrustum* SGCore_CrFrustum()
{
	return new Frustum();
}

ISXCamera* SGCore_CrCamera()
{
	return new Camera();
}

ISXTransObject* SGCore_CrTransObject()
{
	return new SXTransObject();
}

ISXBound* SGCore_CrBound()
{
	return new SXBound();
}

//////////////////////

void SGCore_SetSamplerFilter(DWORD id, DWORD value)
{
	SG_PRECOND(_VOID);

	DXDevice->SetSamplerState(id, D3DSAMP_MAGFILTER, value);
	DXDevice->SetSamplerState(id, D3DSAMP_MINFILTER, value);
	DXDevice->SetSamplerState(id, D3DSAMP_MIPFILTER, value);
}

void SGCore_SetSamplerFilter2(DWORD begin_id, DWORD end_id, DWORD value)
{
	SG_PRECOND(_VOID);

	if (begin_id >= 0 && end_id <= 16)
	{
		for (DWORD i = begin_id; i<end_id; i++)
			SGCore_SetSamplerFilter(i, value);
	}
}

void SGCore_SetSamplerAddress(DWORD id, DWORD value)
{
	SG_PRECOND(_VOID);

	DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSU, value);
	DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSV, value);
	DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSW, value);
}

void SGCore_SetSamplerAddress2(DWORD begin_id, DWORD end_id, DWORD value)
{
	SG_PRECOND(_VOID);

	if (begin_id >= 0 && end_id <= 16)
	{
		for (DWORD i = begin_id; i<end_id; i++)
			SGCore_SetSamplerAddress(i, value);
	}
}

/////////

void SGCore_SkyBoxCr()
{
	SG_PRECOND(_VOID);

	if (ObjSkyBox)
		reportf(1,"sxcore: sky_box is already init");
	else
		ObjSkyBox = new SkyBox();
}

bool SGCore_SkyBoxIsCr()
{
	SG_PRECOND(false);
	return (ObjSkyBox != 0);
}

bool SGCore_SkyBoxIsLoadTex()
{
	SG_PRECOND_SKY_BOX(false);
	return ObjSkyBox->IsLoadTex();
}

void SGCore_SkyBoxLoadTex(const char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->ChangeTexture(texture);
}

void SGCore_SkyBoxSetStdPathTex(const char* path)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->SetStdPath(path);
}

void SGCore_SkyBoxGetStdPathTex(char* path)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->GetStdPath(path);
}

void SGCore_SkyBoxChangeTex(const char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->ChangeTexture(texture);
}

void SGCore_SkyBoxGetActiveTex(char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->GetActiveTexture(texture);
}

void SGCore_SkyBoxGetSecondTex(char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->GetSecondTexture(texture);
}

void SGCore_SkyBoxSetRot(float angle)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->SetRotation(angle);
}

float SGCore_SkyBoxGetRot()
{
	SG_PRECOND_SKY_BOX(0);
	return ObjSkyBox->GetRotation();
}

void SGCore_SkyBoxSetColor(float4_t* color)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->SetColor(color);
}

void SGCore_SkyBoxGetColor(float4_t* color)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->GetColor(color);
}

void SGCore_SkyBoxRender(float timeDelta, float3* pos)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->Render(timeDelta, pos, false);
}


void SGCore_SkyCloudsCr()
{
	SG_PRECOND(_VOID);

	if (ObjSkyClouds)
		reportf(1, "sxcore: sky_clouds is already init");
	else
		ObjSkyClouds = new SkyClouds();
}

bool SGCore_SkyCloudsIsCr()
{
	SG_PRECOND(false);

	return (ObjSkyClouds != 0);
}

bool SGCore_SkyCloudsIsLoadTex()
{
	SG_PRECOND_SKY_CLOUDS(false);

	return ObjSkyClouds->IsLoadTex();
}

void SGCore_SkyCloudsSetWidthHeightPos(float width, float height, float3* pos)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetWidthHeightPos(width, height, pos);
}

void SGCore_SkyCloudsLoadTex(const char *texture)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->LoadTextures(texture);
}

void SGCore_SkyCloudsChangeTex(const char *texture)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->ChangeTexture(texture);
}

void SGCore_SkyCloudsSetRot(float angle)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetRotation(angle);
}

float SGCore_SkyCloudsGetRot()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return ObjSkyClouds->GetRotation();
}

void SGCore_SkyCloudsSetAlpha(float alpha)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetAlpha(alpha);
}

float SGCore_SkyCloudsGetAlpha()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return ObjSkyClouds->GetAlpha();
}

void SGCore_SkyCloudsSetColor(float4_t* color)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetColor(color);
}

void SGCore_SkyCloudsGetColor(float4_t* color)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->GetColor(color);
}

void SGCore_SkyCloudsSetStdPathTex(const char* path)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetStdPath(path);
}

void SGCore_SkyCloudsGetStdPathTex(char* path)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetStdPath(path);
}

void SGCore_SkyCloudsRender(DWORD timeDetlta, float3* pos, bool is_shadow)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->Render(timeDetlta, pos, is_shadow);
}
