
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
	Core_RIntSet(SGCORE_RI_INT_COUNT_DIP, Core_RIntGet(SGCORE_RI_INT_COUNT_DIP) + 1);
	DXDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
}

void StdSetMaterial(UINT id, float4x4* world)
{

	DXDevice->SetTexture(0, SGCore_LoadTexGetTex(id));
}

long StdLoadMaterial(const char* name, int mtl_type)
{
	return SGCore_LoadTexAddName(name);
}

g_func_dip FuncDIP = StdDrawIndexedPrimitive;
g_func_set_material FuncSetMaterial = StdSetMaterial;
g_func_load_material FuncLoadMaterial = StdLoadMaterial;
///

#include <string\string.cpp>
#include <gcore\shader.cpp>
#include <gcore\creatortextures.cpp>
#include <gcore\loadertextures.cpp>
#include <gcore\baseobject.cpp>
#include <gcore\workmodel.cpp>
#include <gcore\camera.cpp>
#include <gcore\\loader_static.cpp>
#include <gcore\\sky.cpp>

#define SXGCORE_VERSION 1

ShaderManager* MShaders = 0;
CreatorTextures* MRenderTargets = 0;
LoaderTextures* MTextures = 0;
ID3DXMesh* ScreenTexture = 0;
SkyBox* ObjSkyBox = 0;
SkyClouds* ObjSkyClouds = 0;

#define SG_PRECOND(retval) if(!DXDevice){reportf(-1, "%s - sxcore is not init", gen_msg_location); return retval;}
#define SG_PRECOND_SKY_BOX(retval) SG_PRECOND(retval); if(!ObjSkyBox){reportf(-1, "%s - sky_box is not init", gen_msg_location); return retval;}
#define SG_PRECOND_SKY_CLOUDS(retval) SG_PRECOND(retval); if(!ObjSkyClouds){reportf(-1, "%s - sky_clouds is not init", gen_msg_location); return retval;}

#include <gcore\dxdevice.cpp>

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
			}
			else
			{
				int cerr = InitD3D(hwnd, windowed, width, heigth, create_device_flags);
				if (cerr == SXGC_ERR_NON_DETECTED_D3D)
					reportf(-1, "%s - none detected d3d, sxgcore", gen_msg_location);
				else if (cerr == SXGC_ERR_FAILED_INIT_D3D)
					reportf(-1, "%s - failed initialized d3d, sxgcore", gen_msg_location);

				MShaders = new ShaderManager();
				MRenderTargets = new CreatorTextures();
				MTextures = new LoaderTextures();
			}
		}
		else
		{
			int cerr = InitD3D(hwnd, windowed, width, heigth, create_device_flags);
			if (cerr == SXGC_ERR_NON_DETECTED_D3D)
				reportf(-1, "%s - none detected d3d, sxgcore", gen_msg_location);
			else if (cerr == SXGC_ERR_FAILED_INIT_D3D)
				reportf(-1, "%s - failed initialized d3d, sxgcore", gen_msg_location);
			
			MShaders = new ShaderManager();
			MRenderTargets = new CreatorTextures();
			MTextures = new LoaderTextures();
		}
	}
	else
		reportf(-1, "%s - not init argument [name], sxgcore", gen_msg_location);
}

IDirect3DDevice9* SGCore_GetDXDevice()
{
	return DXDevice;
}

void SGCore_DbgMsg(const char* format, ...)
{
	SG_PRECOND();
	
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
	SG_PRECOND();

	FPSText->OnLostDevice();
	MRenderTargets->OnLostDevice();
}

int SGCore_OnDeviceReset(int width, int heigth, bool windewed)
{
	SG_PRECOND(-1);
	D3DAPP.BackBufferWidth = width;
	D3DAPP.BackBufferHeight = heigth;
	D3DAPP.Windowed = windewed;
	return (FAILED(DXDevice->Reset(&D3DAPP)));
}

void SGCore_OnResetDevice()
{
	SG_PRECOND();

	FPSText->OnResetDevice();
	MRenderTargets->OnResetDevice();
	ScreenQuadOnResetDevice();
}

void SGCore_ScreenQuadDraw()
{
	SG_PRECOND();
	ScreenTexture->DrawSubset(0);
}

//////

void SGCore_DIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	SG_PRECOND();
	FuncDIP(type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
}

void SGCore_SetMtl(UINT id, float4x4* world)
{
	SG_PRECOND();
	FuncSetMaterial(id, world);
}

long SGCore_LoadMtl(const char* name, int mtl_type)
{
	SG_PRECOND(-1);
	return FuncLoadMaterial(name, mtl_type);
}


void SGCore_SetFunc_DIP(g_func_dip func)
{
	SG_PRECOND();
	FuncDIP = func;
}

void SGCore_SetFunc_SetMtl(g_func_set_material func)
{
	SG_PRECOND();
	FuncSetMaterial = func;
}

void SGCore_SetFunc_LoadMtl(g_func_load_material func)
{
	SG_PRECOND();
	FuncLoadMaterial = func;
}


////////////

DWORD SGCore_ShaderLoad(int type_shader, const char* path, const char* name, int is_check_double, D3DXMACRO* macro)
{
	SG_PRECOND(-1);

	return MShaders->Load(type_shader, path, name, is_check_double, macro);
}

void SGCore_ShaderUpdateN(int type_shader, const char* name, D3DXMACRO macro[])
{
	SG_PRECOND();

	MShaders->Update(type_shader, name, macro);
}

void SGCore_ShaderUpdate(int type_shader, DWORD id, D3DXMACRO macro[])
{
	SG_PRECOND();

	MShaders->Update(type_shader, id, macro);
}

void SGCore_ShaderReloadAll()
{
	SG_PRECOND();

	MShaders->ReloadAll();
}

DWORD SGCore_ShaderGetID(int type_shader, const char* shader)
{
	SG_PRECOND(-1);

	return MShaders->GetID(type_shader, shader);
}

void SGCore_ShaderBindN(int type_shader, const char* shader)
{
	SG_PRECOND();

	return MShaders->Bind(type_shader, shader);
}

void SGCore_ShaderBind(int type_shader, DWORD shader)
{
	SG_PRECOND();

	return MShaders->Bind(type_shader, shader);
}


void SGCore_ShaderUnBind()
{
	SG_PRECOND();

	return MShaders->UnBind();
}

void SGCore_ShaderSetVRFN(int type_shader, const char* name_shader, const char* name_var, void* data)
{
	SG_PRECOND();

	return MShaders->SetValueRegisterF(type_shader, name_shader, name_var, data);
}

void SGCore_ShaderSetVRF(int type_shader, DWORD num_shader, const char* name_var, void* data)
{
	SG_PRECOND();

	return MShaders->SetValueRegisterF(type_shader, num_shader, name_var, data);
}

void SGCore_ShaderSetVRIN(int type_shader, const char* name_shader, const char* name_var, void* data)
{
	SG_PRECOND();

	return MShaders->SetValueRegisterI(type_shader, name_shader, name_var, data);
}

void SGCore_ShaderSetVRI(int type_shader, DWORD num_shader, const char* name_var, void* data)
{
	SG_PRECOND();

	return MShaders->SetValueRegisterI(type_shader, num_shader, name_var, data);
}


DWORD SGCore_ShaderIsExist(int type_shader, const char* name)
{
	SG_PRECOND(-1);

	return MShaders->IsExist(type_shader, name);
}

bool SGCore_ShaderIsValidate(int type_shader, DWORD id)
{
	SG_PRECOND(0);

	return MShaders->IsValidate(type_shader, id);
}

/*void SGCore_ShaderGetPath(int type_shader, DWORD id, char* path)
{
	SG_PRECOND();

	MShaders->GetPath(type_shader, id, path);
}*/

void SGCore_ShaderGetName(int type_shader, DWORD id, char* name)
{
	SG_PRECOND();

	MShaders->GetName(type_shader, id, name);
}

void SGCore_ShaderSetStdPath(const char* path)
{
	SG_PRECOND();

	MShaders->SetStdPath(path);
}

void SGCore_ShaderGetStdPath(char* path)
{
	SG_PRECOND();

	MShaders->GetStdPath(path);
}

////////////////////////

void SGCore_LoadTexStdPath(const char* path)
{
	SG_PRECOND();

	MTextures->SetStdPath(path);
	/*ObjSkyBox->SetStdPath(path);
	ObjSkyClouds->SetStdPath(path);*/
}

DWORD SGCore_LoadTexAddName(const char* name)
{
	SG_PRECOND(-1);

	return MTextures->AddName(name);
}

DWORD SGCore_LoadTexGetID(const char* name)
{
	SG_PRECOND(-1);

	return MTextures->GetID(name);
}

void SGCore_LoadTexGetName(DWORD id, char* name)
{
	SG_PRECOND();

	return MTextures->GetName(id,name);
}

DWORD SGCore_LoadTexCreate(const char* name, IDirect3DTexture9* tex)
{
	SG_PRECOND(-1);

	return MTextures->Create(name, tex);
}

DWORD SGCore_LoadTexUpdateN(const char* name)
{
	SG_PRECOND(-1);

	return MTextures->Update(name);
}

void SGCore_LoadTexUpdate(DWORD id)
{
	SG_PRECOND();

	return MTextures->Update(id);
}

IDirect3DTexture9* SGCore_LoadTexGetTex(DWORD id)
{
	SG_PRECOND(0);

	return MTextures->GetTexture(id);
}

void SGCore_LoadTexLoadTextures()
{
	SG_PRECOND();

	return MTextures->LoadTextures();
}

//////////////////////////

DWORD SGCore_RTAdd(UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, const char* name, float coeffullscreen)
{
	SG_PRECOND(-1);

	return MRenderTargets->Add(width, height, levels, usage, format, pool, name, coeffullscreen);
}

void SGCore_RTDeleteN(const char* text)
{
	SG_PRECOND();

	return MRenderTargets->Delete(text);
}

void SGCore_RTDelete(DWORD num)
{
	SG_PRECOND();

	return MRenderTargets->Delete(num);
}

DWORD SGCore_RTGetNum(const char* text)
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

IDirect3DTexture9* SGCore_RTGetTexture(DWORD num)
{
	SG_PRECOND(0);

	return MRenderTargets->GetTexture(num);
}

////////////

void SGCore_FCreateCone(float fTopRadius, float fBottomRadius, float fHeight, ID3DXMesh ** ppMesh, UINT iSideCount)
{
	SG_PRECOND();

	CreateCone(fTopRadius, fBottomRadius, fHeight, ppMesh, DXDevice, iSideCount);
}

void SGCore_FCompBoundBox(IDirect3DVertexBuffer9* vertex_buffer, ISXBound** bound, DWORD count_vert, DWORD bytepervert)
{
	SG_PRECOND();

	ComputeBoundingBox(vertex_buffer, bound, count_vert, bytepervert);
}

void SGCore_FCompBoundBox2(IDirect3DVertexBuffer9* vertex_buffer, ISXBound* bound, DWORD count_vert, DWORD bytepervert)
{
	SG_PRECOND();

	ComputeBoundingBox2(vertex_buffer, bound, count_vert, bytepervert);
}

void SGCore_FCreateBoundingBoxMesh(float3* min, float3* max, ID3DXMesh** bbmesh)
{
	SG_PRECOND();

	CreateBoundingBoxMesh(min, max, bbmesh,DXDevice);
}

void SGCore_OptimizeIndecesInSubsetUint16(uint16_t* ib, uint16_t numFaces, uint16_t numVerts)
{
	SG_PRECOND();

	OptimizeIndecesInSubsetUint16(ib, numFaces, numVerts);
}

void SGCore_OptimizeIndecesInSubsetUint32(uint32_t* ib, uint32_t numFaces, uint32_t numVerts)
{
	SG_PRECOND();

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
	SG_PRECOND();

	DXDevice->SetSamplerState(id, D3DSAMP_MAGFILTER, value);
	DXDevice->SetSamplerState(id, D3DSAMP_MINFILTER, value);
	DXDevice->SetSamplerState(id, D3DSAMP_MIPFILTER, value);
}

void SGCore_SetSamplerFilter2(DWORD begin_id, DWORD end_id, DWORD value)
{
	SG_PRECOND();

	if (begin_id >= 0 && end_id <= 16)
	{
		for (DWORD i = begin_id; i<end_id; i++)
			SGCore_SetSamplerFilter(i, value);
	}
}

void SGCore_SetSamplerAddress(DWORD id, DWORD value)
{
	SG_PRECOND();

	DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSU, value);
	DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSV, value);
	DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSW, value);
}

void SGCore_SetSamplerAddress2(DWORD begin_id, DWORD end_id, DWORD value)
{
	SG_PRECOND();

	if (begin_id >= 0 && end_id <= 16)
	{
		for (DWORD i = begin_id; i<end_id; i++)
			SGCore_SetSamplerAddress(i, value);
	}
}

/////////

void SGCore_SkyBoxCr()
{
	SG_PRECOND();

	if (ObjSkyBox)
		reportf(1,"sxcore: sky_box is already init");
	else
		ObjSkyBox = new SkyBox();
}

void SGCore_SkyBoxLoadTex(const char *texture)
{
	SG_PRECOND_SKY_BOX();
	ObjSkyBox->ChangeTexture(texture);
}

void SGCore_SkyBoxSetStdPathTex(const char* path)
{
	SG_PRECOND_SKY_BOX();
	ObjSkyBox->SetStdPath(path);
}

void SGCore_SkyBoxGetStdPathTex(char* path)
{
	SG_PRECOND_SKY_BOX();
	ObjSkyBox->GetStdPath(path);
}

void SGCore_SkyBoxChangeTex(const char *texture)
{
	SG_PRECOND_SKY_BOX();
	ObjSkyBox->ChangeTexture(texture);
}

void SGCore_SkyBoxSetRot(float angle)
{
	SG_PRECOND_SKY_BOX();
	ObjSkyBox->SetRotation(angle);
}

float SGCore_SkyBoxGetRot()
{
	SG_PRECOND_SKY_BOX(0);
	return ObjSkyBox->GetRotation();
}

void SGCore_SkyBoxSetColor(float4_t* color)
{
	SG_PRECOND_SKY_BOX();
	ObjSkyBox->SetColor(color);
}

void SGCore_SkyBoxGetColor(float4_t* color)
{
	SG_PRECOND_SKY_BOX();
	ObjSkyBox->GetColor(color);
}

void SGCore_SkyBoxRender(float timeDelta, float3* pos)
{
	SG_PRECOND_SKY_BOX();
	ObjSkyBox->Render(timeDelta, pos, false);
}


void SGCore_SkyCloudsCr()
{
	SG_PRECOND();

	if (ObjSkyClouds)
		reportf(1, "sxcore: sky_clouds is already init");
	else
		ObjSkyClouds = new SkyClouds();
}

void SGCore_SkyCloudsSetWidthHeightPos(float width, float height, float3* pos)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->SetWidthHeightPos(width, height, pos);
}

void SGCore_SkyCloudsLoadTex(const char *texture)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->LoadTextures(texture);
}

void SGCore_SkyCloudsChangeTex(const char *texture)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->ChangeTexture(texture);
}

void SGCore_SkyCloudsSetRot(float angle)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->SetRotation(angle);
}

float SGCore_SkyCloudsGetRot()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return ObjSkyClouds->GetRotation();
}

void SGCore_SkyCloudsSetAlpha(float alpha)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->SetAlpha(alpha);
}

float SGCore_SkyCloudsGetAlpha()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return ObjSkyClouds->GetAlpha();
}

void SGCore_SkyCloudsSetColor(float4_t* color)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->SetColor(color);
}

void SGCore_SkyCloudsGetColor(float4_t* color)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->GetColor(color);
}

void SGCore_SkyCloudsSetStdPathTex(const char* path)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->SetStdPath(path);
}

void SGCore_SkyCloudsGetStdPathTex(char* path)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->SetStdPath(path);
}

void SGCore_SkyCloudsRender(DWORD timeDetlta, float3* pos, bool is_shadow)
{
	SG_PRECOND_SKY_CLOUDS();
	ObjSkyClouds->Render(timeDetlta, pos, is_shadow);
}
