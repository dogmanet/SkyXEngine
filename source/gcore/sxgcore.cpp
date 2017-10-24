
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

#include "sxgcore.h"

#include <gcore\GeomOptimize.h>
#include <gcore\shader.h>
#include <gcore\creatortextures.h>
#include <gcore\loadertextures.h>
#include <gcore\bound.h>
#include <gcore\camera.h>

#include <gcore\\loader_static.h>
#include <gcore\\sky.h>

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

IDirect3DDevice9* DXDevice = 0;
D3DPRESENT_PARAMETERS D3DAPP;
IDirect3D9* d3d9 = 0;
ID3DXFont* FPSText = 0;
Array<DEVMODE> g_aArrModes;

//##########################################################################

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

//##########################################################################

IDirect3DVertexDeclaration9* StaticVertexDecl = 0;


#define SXGCORE_VERSION 1

ShaderManager* MShaders = 0;
CreatorTextures* MRenderTargets = 0;
LoaderTextures* MTextures = 0;
ID3DXMesh* ScreenTexture = 0;
SkyBox* ObjSkyBox = 0;
SkyClouds* ObjSkyClouds = 0;


#define SG_PRECOND(retval) if(!DXDevice){g_fnReportf(-1, "%s - sxgcore is not init", gen_msg_location); return retval;}
#define SG_PRECOND_SKY_BOX(retval) SG_PRECOND(retval _VOID); if(!ObjSkyBox){g_fnReportf(-1, "%s - sky_box is not init", gen_msg_location); return retval;}
#define SG_PRECOND_SKY_CLOUDS(retval) SG_PRECOND(retval _VOID); if(!ObjSkyClouds){g_fnReportf(-1, "%s - sky_clouds is not init", gen_msg_location); return retval;}


void GCoreInit(HWND hwnd, int width, int heigth, bool windowed, DWORD create_device_flags)
{
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		g_fnReportf(-1, "%s - none detected d3d, sxgcore", gen_msg_location);
		return;
	}

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	memset(&D3DAPP, 0, sizeof(D3DAPP));
	D3DAPP.BackBufferWidth = width;
	D3DAPP.BackBufferHeight = heigth;
	D3DAPP.BackBufferFormat = D3DFMT_A8R8G8B8;
	D3DAPP.BackBufferCount = 1;
	D3DAPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	D3DAPP.MultiSampleQuality = 0;
	D3DAPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DAPP.hDeviceWindow = hwnd;
	D3DAPP.Windowed = windowed;
	D3DAPP.EnableAutoDepthStencil = true;
	D3DAPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	D3DAPP.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL | create_device_flags | D3DCREATE_MULTITHREADED;
	D3DAPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	D3DAPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | create_device_flags | D3DCREATE_MULTITHREADED, &D3DAPP, &DXDevice)))
	{
		g_fnReportf(-1, "%s - failed initialized d3d, sxgcore", gen_msg_location);
		return;
	}

	D3DXFONT_DESC LF;
	ZeroMemory(&LF, sizeof(D3DXFONT_DESC));
	LF.Height = 10;    // в логических единицах
	LF.Width = 6;    // в логических единицах
	LF.Weight = 6;   // насыщенность, 
	// диапазон 0(тонкий) - 1000(жирный)
	LF.Italic = 0;
	LF.CharSet = DEFAULT_CHARSET;
	LF.FaceName[0] = 0;

	D3DXCreateFontIndirect(DXDevice, &LF, &FPSText);

	D3DVERTEXELEMENT9 layoutquad[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	//IDirect3DVertexDeclaration9* VertexDeclarationQuad;
	//GData::DXDevice->CreateVertexDeclaration(layoutquad, &VertexDeclarationQuad);

	D3DXCreateMesh(2, 4, D3DXMESH_MANAGED, layoutquad, DXDevice, &ScreenTexture);

	struct  VERTEX_SCREEN_TEXTURE { float x, y, z, tx, ty, tz; };

	const float offset_pixel_x = 1.0f / float(D3DAPP.BackBufferWidth);
	const float offset_pixel_y = 1.0f / float(D3DAPP.BackBufferHeight);

	VERTEX_SCREEN_TEXTURE AddVertices[] =
	{
		{ -1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 0.0f, 1.0f, 0 },
		{ -1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 0.0f, 0.0f, 1 },
		{ 1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 1.0f, 0.0f, 2 },
		{ 1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 1.0f, 1.0f, 3 },
	};

	void* Vertices;
	if (!FAILED(ScreenTexture->LockVertexBuffer(0, (void**)&Vertices)))
	{
		memcpy(Vertices, AddVertices, sizeof(AddVertices));
		ScreenTexture->UnlockVertexBuffer();
	}

	WORD* i = 0;
	ScreenTexture->LockIndexBuffer(0, (void**)&i);
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;
	ScreenTexture->UnlockIndexBuffer();

	
	//устанавливаем данные в регистры
	/*Core_RFloatSet(G_RI_FLOAT_WINSIZE_WIDTH, (float)width);
	Core_RFloatSet(G_RI_FLOAT_WINSIZE_HEIGHT, (float)heigth);
	Core_RBoolSet(G_RI_BOOL_RENDER_WINDOWED, windowed);*/

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


	//получение всех возможных разрешений монитора
	DEVMODE dm;
	int iNum = 0;
	while (EnumDisplaySettings(NULL, iNum, &dm))
	{
		iNum++;
		if (dm.dmBitsPerPel >= 32 && dm.dmPelsWidth >= 800 && dm.dmPelsHeight >= 600)
		{
			bool isUnic = true;
			for (int i = 0, il = g_aArrModes.size(); i < il; ++i)
			{
				if (g_aArrModes[i].dmPelsWidth == dm.dmPelsWidth && g_aArrModes[i].dmPelsHeight == dm.dmPelsHeight)
				{
					isUnic = false;
					break;
				}
			}

			if (isUnic)
				g_aArrModes.push_back(dm);
		}
	}

	int qwerty = 0;
}

//##########################################################################

SX_LIB_API long SGCore_0GetVersion()
{
	return SXGCORE_VERSION;
}

SX_LIB_API void SGCore_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SGCore_0Create(const char* name, HWND hwnd, int width, int heigth, bool windowed, DWORD create_device_flags, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				g_fnReportf(-1, "%s - none unic name, sxgcore", gen_msg_location);
				return;
			}
		}
		GCoreInit(hwnd, width, heigth, windowed, create_device_flags);
	}
	else
		g_fnReportf(-1, "%s - not init argument [name], sxgcore", gen_msg_location);
}

SX_LIB_API const DEVMODE* SGCore_GetModes(int *iCount)
{
	SG_PRECOND(0);

	if (iCount)
		*iCount = g_aArrModes.size();

	return &(g_aArrModes[0]);
}

SX_LIB_API void SGCore_AKill()
{
	SG_PRECOND(_VOID);
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

SX_LIB_API IDirect3DDevice9* SGCore_GetDXDevice()
{
	SG_PRECOND(0);
	return DXDevice;
}

SX_LIB_API void SGCore_DbgMsg(const char* format, ...)
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
	--rect.top;
	--rect.left;
	FPSText->DrawText(0, buf, -1, &rect, 0, 0xffffffff);
}

SX_LIB_API void SGCore_OnLostDevice()
{
	SG_PRECOND(_VOID);

	FPSText->OnLostDevice();
	MRenderTargets->OnLostDevice();
}

SX_LIB_API bool SGCore_OnDeviceReset(int width, int heigth, bool windowed)
{
	SG_PRECOND(false);
	D3DAPP.BackBufferWidth = width;
	D3DAPP.BackBufferHeight = heigth;
	D3DAPP.Windowed = windowed;

	/*Core_RFloatSet(G_RI_FLOAT_WINSIZE_WIDTH, (float)width);
	Core_RFloatSet(G_RI_FLOAT_WINSIZE_HEIGHT, (float)heigth);
	Core_RBoolSet(G_RI_BOOL_RENDER_WINDOWED, windowed);*/

	return (FAILED(DXDevice->Reset(&D3DAPP)));
}

SX_LIB_API void SGCore_OnResetDevice()
{
	SG_PRECOND(_VOID);

	FPSText->OnResetDevice();
	MRenderTargets->OnResetDevice();
	
	struct  VERTEX_SCREEN_TEXTURE { float x, y, z, tx, ty, tz; };

	const float offset_pixel_x = 1.0f / float(D3DAPP.BackBufferWidth);
	const float offset_pixel_y = 1.0f / float(D3DAPP.BackBufferHeight);

	VERTEX_SCREEN_TEXTURE AddVertices[] =
	{
		{ -1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 0.0f, 1.0f, 0 },
		{ -1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 0.0f, 0.0f, 1 },
		{ 1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 1.0f, 0.0f, 2 },
		{ 1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 1.0f, 1.0f, 3 },
	};

	void* Vertices;
	if (!FAILED(ScreenTexture->LockVertexBuffer(0, (void**)&Vertices)))
	{
		memcpy(Vertices, AddVertices, sizeof(AddVertices));
		ScreenTexture->UnlockVertexBuffer();
	}
}

SX_LIB_API void SGCore_ScreenQuadDraw()
{
	SG_PRECOND(_VOID);
	ScreenTexture->DrawSubset(0);
}

//////

SX_LIB_API void SGCore_DIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	SG_PRECOND(_VOID);
	FuncDIP(type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
}

SX_LIB_API void SGCore_MtlSet(ID id, float4x4* world)
{
	SG_PRECOND(_VOID);
	FuncMtlSet(id, world);
}

SX_LIB_API ID SGCore_MtlLoad(const char* name, int mtl_type)
{
	SG_PRECOND(-1);
	return FuncMtlLoad(name, mtl_type);
}

SX_LIB_API int SGCore_MtlGetSort(ID id)
{
	SG_PRECOND(-1);
	return FuncMtlGetSort(id);
}

SX_LIB_API int SGCore_MtlGetPhysicType(ID id)
{
	SG_PRECOND(-1);
	return FuncMtlGetPhysicType(id);
}

SX_LIB_API bool SGCore_MtlGroupRenderIsSingly(ID id)
{
	SG_PRECOND(false);
	return FuncMtlGroupRenderIsSingly(id);
}


SX_LIB_API void SGCore_SetFunc_DIP(g_func_dip func)
{
	SG_PRECOND(_VOID);
	FuncDIP = func;
}

SX_LIB_API void SGCore_SetFunc_MtlSet(g_func_mtl_set func)
{
	SG_PRECOND(_VOID);
	FuncMtlSet = func;
}

SX_LIB_API void SGCore_SetFunc_MtlLoad(g_func_mtl_load func)
{
	SG_PRECOND(_VOID);
	FuncMtlLoad = func;
}

SX_LIB_API void SGCore_SetFunc_MtlGetSort(g_func_mtl_get_sort func)
{
	SG_PRECOND(_VOID);
	FuncMtlGetSort = func;
}

SX_LIB_API void SGCore_SetFunc_MtlGetPhysicType(g_func_mtl_get_physic_type func)
{
	SG_PRECOND(_VOID);
	FuncMtlGetPhysicType = func;
}

SX_LIB_API void SGCore_SetFunc_MtlGroupRenderIsSingly(g_func_mtl_group_render_is_singly func)
{
	SG_PRECOND(_VOID);
	FuncMtlGroupRenderIsSingly = func;
}


//##########################################################################

SX_LIB_API ID SGCore_ShaderLoad(SHADER_TYPE type_shader, const char* path, const char* name, SHADER_CHECKDOUBLE is_check_double, D3DXMACRO* macro)
{
	SG_PRECOND(-1);

	return MShaders->Load(type_shader, path, name, is_check_double, macro);
}

SX_LIB_API void SGCore_ShaderUpdateN(SHADER_TYPE type_shader, const char* name, D3DXMACRO macro[])
{
	SG_PRECOND(_VOID);

	MShaders->Update(type_shader, name, macro);
}

SX_LIB_API void SGCore_ShaderUpdate(SHADER_TYPE type_shader, ID id, D3DXMACRO macro[])
{
	SG_PRECOND(_VOID);

	MShaders->Update(type_shader, id, macro);
}

SX_LIB_API void SGCore_ShaderReloadAll()
{
	SG_PRECOND(_VOID);

	MShaders->ReloadAll();
}

SX_LIB_API ID SGCore_ShaderGetID(SHADER_TYPE type_shader, const char* shader)
{
	SG_PRECOND(-1);

	return MShaders->GetID(type_shader, shader);
}

SX_LIB_API void SGCore_ShaderBindN(SHADER_TYPE type_shader, const char* shader)
{
	SG_PRECOND(_VOID);

	return MShaders->Bind(type_shader, shader);
}

SX_LIB_API void SGCore_ShaderBind(SHADER_TYPE type_shader, ID shader)
{
	SG_PRECOND(_VOID);

	return MShaders->Bind(type_shader, shader);
}


SX_LIB_API void SGCore_ShaderUnBind()
{
	SG_PRECOND(_VOID);

	return MShaders->UnBind();
}

SX_LIB_API void SGCore_ShaderSetVRFN(SHADER_TYPE type_shader, const char* name_shader, const char* name_var, void* data, int count_float4)
{
	SG_PRECOND(_VOID);

	return MShaders->SetValueRegisterF(type_shader, name_shader, name_var, data, count_float4);
}

SX_LIB_API void SGCore_ShaderSetVRF(SHADER_TYPE type_shader, ID num_shader, const char* name_var, void* data, int count_float4)
{
	SG_PRECOND(_VOID);

	return MShaders->SetValueRegisterF(type_shader, num_shader, name_var, data, count_float4);
}

SX_LIB_API void SGCore_ShaderSetVRIN(SHADER_TYPE type_shader, const char* name_shader, const char* name_var, void* data, int count_int4)
{
	SG_PRECOND(_VOID);

	return MShaders->SetValueRegisterI(type_shader, name_shader, name_var, data, count_int4);
}

SX_LIB_API void SGCore_ShaderSetVRI(SHADER_TYPE type_shader, ID num_shader, const char* name_var, void* data, int count_int4)
{
	SG_PRECOND(_VOID);

	return MShaders->SetValueRegisterI(type_shader, num_shader, name_var, data, count_int4);
}


SX_LIB_API ID SGCore_ShaderIsExistName(SHADER_TYPE type_shader, const char* name)
{
	SG_PRECOND(-1);

	return MShaders->IsExistName(type_shader, name);
}

SX_LIB_API ID SGCore_ShaderIsExistPath(SHADER_TYPE type_shader, const char* name)
{
	SG_PRECOND(-1);

	return MShaders->IsExistName(type_shader, name);
}

SX_LIB_API bool SGCore_ShaderIsValidate(SHADER_TYPE type_shader, ID id)
{
	SG_PRECOND(0);

	return MShaders->IsValidate(type_shader, id);
}

SX_LIB_API void SGCore_ShaderGetPath(SHADER_TYPE type_shader, ID id, char* path)
{
	SG_PRECOND(_VOID);

	MShaders->GetPath(type_shader, id, path);
}

SX_LIB_API void SGCore_ShaderGetName(SHADER_TYPE type_shader, ID id, char* name)
{
	SG_PRECOND(_VOID);

	MShaders->GetName(type_shader, id, name);
}

SX_LIB_API bool SGCore_ShaderFileExists(const char* name)
{
	SG_PRECOND(false);

	return MShaders->FileExists(name);
}

//##########################################################################

SX_LIB_API bool SGCore_LoadTexFileExists(const char* name)
{
	SG_PRECOND(false);

	return MTextures->FileExists(name);
}

SX_LIB_API void SGCore_LoadTexClearLoaded()
{
	SG_PRECOND(_VOID);

	MTextures->ClearLoaded();
}

SX_LIB_API void SGCore_LoadTexDelete(ID id)
{
	SG_PRECOND(_VOID);

	MTextures->Delete(id);
}

SX_LIB_API ID SGCore_LoadTexAddName(const char* name, LoadTexType type)
{
	SG_PRECOND(-1);

	return MTextures->AddName(name, type);
}

SX_LIB_API ID SGCore_LoadTexGetID(const char* name)
{
	SG_PRECOND(-1);

	return MTextures->GetID(name);
}

SX_LIB_API void SGCore_LoadTexGetName(ID id, char* name)
{
	SG_PRECOND(_VOID);

	return MTextures->GetName(id,name);
}

SX_LIB_API ID SGCore_LoadTexCreate(const char* name, IDirect3DTexture9* tex)
{
	SG_PRECOND(-1);

	return MTextures->Create(name, tex);
}

SX_LIB_API ID SGCore_LoadTexUpdateN(const char* name, LoadTexType type)
{
	SG_PRECOND(-1);

	return MTextures->Update(name, type);
}

SX_LIB_API void SGCore_LoadTexUpdate(ID id)
{
	SG_PRECOND(_VOID);

	return MTextures->Update(id);
}

SX_LIB_API IDirect3DTexture9* SGCore_LoadTexGetTex(ID id)
{
	SG_PRECOND(0);

	return MTextures->GetTexture(id);
}

SX_LIB_API void SGCore_LoadTexLoadTextures()
{
	SG_PRECOND(_VOID);

	return MTextures->LoadTextures();
}

//##########################################################################

SX_LIB_API ID SGCore_RTAdd(UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, const char* name, float coeffullscreen)
{
	SG_PRECOND(-1);

	return MRenderTargets->Add(width, height, levels, usage, format, pool, name, coeffullscreen);
}

SX_LIB_API void SGCore_RTDeleteN(const char* text)
{
	SG_PRECOND(_VOID);

	return MRenderTargets->Delete(text);
}

SX_LIB_API void SGCore_RTDelete(ID num)
{
	SG_PRECOND(_VOID);

	return MRenderTargets->Delete(num);
}

SX_LIB_API ID SGCore_RTGetNum(const char* text)
{
	SG_PRECOND(-1);

	return MRenderTargets->GetNum(text);
}

SX_LIB_API IDirect3DTexture9* SGCore_RTGetTextureN(const char* text)
{
	SG_PRECOND(0);

	return MRenderTargets->GetTexture(text);
}

SX_LIB_API IDirect3DTexture9* SGCore_RTGetTexture(ID num)
{
	SG_PRECOND(0);

	return MRenderTargets->GetTexture(num);
}

//##########################################################################

SX_LIB_API void SGCore_FCreateCone(float fTopRadius, float fBottomRadius, float fHeight, ID3DXMesh ** ppMesh, UINT iSideCount)
{
	SG_PRECOND(_VOID);

	CreateCone(fTopRadius, fBottomRadius, fHeight, ppMesh, DXDevice, iSideCount);
}

SX_LIB_API void SGCore_FCompBoundBox(IDirect3DVertexBuffer9* vertex_buffer, ISXBound** bound, DWORD count_vert, DWORD bytepervert)
{
	SG_PRECOND(_VOID);

	ComputeBoundingBox(vertex_buffer, bound, count_vert, bytepervert);
}

SX_LIB_API void SGCore_FCompBoundBox2(IDirect3DVertexBuffer9* vertex_buffer, ISXBound* bound, DWORD count_vert, DWORD bytepervert)
{
	SG_PRECOND(_VOID);

	ComputeBoundingBox2(vertex_buffer, bound, count_vert, bytepervert);
}

SX_LIB_API void SGCore_FCreateBoundingBoxMesh(float3* min, float3* max, ID3DXMesh** bbmesh)
{
	SG_PRECOND(_VOID);

	CreateBoundingBoxMesh(min, max, bbmesh,DXDevice);
}

SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint16(uint16_t* ib, uint16_t numFaces, uint16_t numVerts)
{
	SG_PRECOND(_VOID);

	OptimizeIndecesInSubsetUint16(ib, numFaces, numVerts);
}

SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint32(uint32_t* ib, uint32_t numFaces, uint32_t numVerts)
{
	SG_PRECOND(_VOID);

	OptimizeIndecesInSubsetUint32(ib, numFaces, numVerts);
}

//##########################################################################

SX_LIB_API bool SGCore_0InPos2D(float3* min, float3* max, float3* pos)
{
	return InPosition2D(min, max, pos);
}

SX_LIB_API bool SGCore_0InPosAbs2D(float3* min, float3* max, float3* pos)
{
	return InPositionAbs2D(min, max, pos);
}

SX_LIB_API int SGCore_0CountPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPoints2D(min, max, p1, p2, p3);
}

SX_LIB_API int SGCore_0CountPosPointsAbs2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPointsAbs2D(min, max, p1, p2, p3);
}

SX_LIB_API bool SGCore_0InPosPoints2D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return InPositionPoints2D(min, max, p1, p2, p3);
}

SX_LIB_API bool SGCore_0InPos3D(float3* min, float3* max, float3* pos)
{
	return InPosition3D(min, max, pos);
}

SX_LIB_API bool SGCore_0InPosAbs3D(float3* min, float3* max, float3* pos)
{
	return InPositionAbs3D(min, max, pos);
}

SX_LIB_API int SGCore_0CountPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPoints3D(min, max, p1, p2, p3);
}

SX_LIB_API int SGCore_0CountPosPointsAbs3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return CountPositionPointsAbs3D(min, max, p1, p2, p3);
}

SX_LIB_API bool SGCore_0InPosPoints3D(float3* min, float3* max, float3* p1, float3* p2, float3* p3)
{
	return InPositionPoints3D(min, max, p1, p2, p3);
}

SX_LIB_API void SGCore_0ComBoundBoxArr8(ISXBound* bound, ISXBound** bound_arr)
{
	ComputeBoundingBoxArr8(bound, bound_arr);
}

SX_LIB_API void SGCore_0ComBoundBoxArr4(ISXBound* bound, ISXBound** bound_arr)
{
	ComputeBoundingBoxArr4(bound, bound_arr);
}

SX_LIB_API float SGCore_0DistancePointBeam2(const float3 & p, const float3 & start, const float3 & dir)
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

SX_LIB_API bool SGCore_0InretsectBox(const float3 * min1, const float3 * max1, const float3 * min2, const float3 * max2)
{
	return (!((min1->x > max2->x || max1->x < min2->x)
		|| (min1->y > max2->y || max1->y < min2->y)
		|| (min1->z > max2->z || max1->z < min2->z)));
}

//##########################################################################

SX_LIB_API ISXFrustum* SGCore_CrFrustum()
{
	return new Frustum();
}

SX_LIB_API ISXCamera* SGCore_CrCamera()
{
	return new Camera();
}

SX_LIB_API ISXTransObject* SGCore_CrTransObject()
{
	return new SXTransObject();
}

SX_LIB_API ISXBound* SGCore_CrBound()
{
	return new SXBound();
}

//##########################################################################

SX_LIB_API void SGCore_SetSamplerFilter(DWORD id, DWORD value)
{
	SG_PRECOND(_VOID);

	DXDevice->SetSamplerState(id, D3DSAMP_MAGFILTER, value);
	DXDevice->SetSamplerState(id, D3DSAMP_MINFILTER, value);
	DXDevice->SetSamplerState(id, D3DSAMP_MIPFILTER, value);
}

SX_LIB_API void SGCore_SetSamplerFilter2(DWORD begin_id, DWORD end_id, DWORD value)
{
	SG_PRECOND(_VOID);

	if (begin_id >= 0 && end_id <= 16)
	{
		for (DWORD i = begin_id; i<end_id; i++)
			SGCore_SetSamplerFilter(i, value);
	}
}

SX_LIB_API void SGCore_SetSamplerAddress(DWORD id, DWORD value)
{
	SG_PRECOND(_VOID);

	DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSU, value);
	DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSV, value);
	DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSW, value);
}

SX_LIB_API void SGCore_SetSamplerAddress2(DWORD begin_id, DWORD end_id, DWORD value)
{
	SG_PRECOND(_VOID);

	if (begin_id >= 0 && end_id <= 16)
	{
		for (DWORD i = begin_id; i<end_id; i++)
			SGCore_SetSamplerAddress(i, value);
	}
}

//##########################################################################

SX_LIB_API void SGCore_SkyBoxCr()
{
	SG_PRECOND(_VOID);

	if (ObjSkyBox)
		g_fnReportf(1,"sxcore: sky_box is already init");
	else
		ObjSkyBox = new SkyBox();
}

SX_LIB_API bool SGCore_SkyBoxIsCr()
{
	SG_PRECOND(false);
	return (ObjSkyBox != 0);
}

SX_LIB_API bool SGCore_SkyBoxIsLoadTex()
{
	SG_PRECOND_SKY_BOX(false);
	return ObjSkyBox->IsLoadTex();
}

SX_LIB_API void SGCore_SkyBoxLoadTex(const char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->LoadTextures(texture);
}

SX_LIB_API void SGCore_SkyBoxChangeTex(const char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->ChangeTexture(texture);
}

SX_LIB_API void SGCore_SkyBoxGetActiveTex(char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->GetActiveTexture(texture);
}

SX_LIB_API void SGCore_SkyBoxGetSecondTex(char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->GetSecondTexture(texture);
}

SX_LIB_API void SGCore_SkyBoxSetRot(float angle)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->SetRotation(angle);
}

SX_LIB_API float SGCore_SkyBoxGetRot()
{
	SG_PRECOND_SKY_BOX(0);
	return ObjSkyBox->GetRotation();
}

SX_LIB_API void SGCore_SkyBoxSetColor(float4_t* color)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->SetColor(color);
}

SX_LIB_API void SGCore_SkyBoxGetColor(float4_t* color)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->GetColor(color);
}

SX_LIB_API void SGCore_SkyBoxRender(float timeDelta, float3* pos)
{
	SG_PRECOND_SKY_BOX(_VOID);
	ObjSkyBox->Render(timeDelta, pos, false);
}


SX_LIB_API void SGCore_SkyCloudsCr()
{
	SG_PRECOND(_VOID);

	if (ObjSkyClouds)
		g_fnReportf(1, "sxcore: sky_clouds is already init");
	else
		ObjSkyClouds = new SkyClouds();
}

SX_LIB_API bool SGCore_SkyCloudsIsCr()
{
	SG_PRECOND(false);

	return (ObjSkyClouds != 0);
}

SX_LIB_API bool SGCore_SkyCloudsIsLoadTex()
{
	SG_PRECOND_SKY_CLOUDS(false);

	return ObjSkyClouds->IsLoadTex();
}

SX_LIB_API void SGCore_SkyCloudsSetWidthHeightPos(float width, float height, float3* pos)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetWidthHeightPos(width, height, pos);
}

SX_LIB_API void SGCore_SkyCloudsLoadTex(const char *texture)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->LoadTextures(texture);
}

SX_LIB_API void SGCore_SkyCloudsChangeTex(const char *texture)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->ChangeTexture(texture);
}

SX_LIB_API void SGCore_SkyCloudsSetRot(float angle)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetRotation(angle);
}

SX_LIB_API float SGCore_SkyCloudsGetRot()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return ObjSkyClouds->GetRotation();
}

SX_LIB_API void SGCore_SkyCloudsSetAlpha(float alpha)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetAlpha(alpha);
}

SX_LIB_API float SGCore_SkyCloudsGetAlpha()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return ObjSkyClouds->GetAlpha();
}

SX_LIB_API void SGCore_SkyCloudsSetSpeed(float speed)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetSpeed(speed);
}

SX_LIB_API float SGCore_SkyCloudsGetSpeed()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return ObjSkyClouds->GetSpeed();
}

SX_LIB_API void SGCore_SkyCloudsSetColor(float4_t* color)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->SetColor(color);
}

SX_LIB_API void SGCore_SkyCloudsGetColor(float4_t* color)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->GetColor(color);
}


SX_LIB_API void SGCore_SkyCloudsRender(DWORD timeDetlta, float3* pos, bool is_shadow)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	ObjSkyClouds->Render(timeDetlta, pos, is_shadow);
}
