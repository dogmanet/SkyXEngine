
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxgcore.h"

#include <gcore/GeomOptimize.h>
#include <gcore/shader.h>
#include <gcore/creatortextures.h>
#include <gcore/loadertextures.h>
#include <gcore/bound.h>
#include <gcore/camera.h>

#include <gcore/loader_static.h>
#include <gcore/sky.h>

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

/*inline void LibReport(int iLevel, const char *szFormat, ...)
{
	static char szStr[REPORT_MSG_MAX_LEN];
	szStr[0] = 0;
	int iStrLen = sizeof(szStr);
	format_str(szStr, szFormat);

	g_fnReportf(iLevel, SX_LIB_NAME, szStr);
}*/

IDirect3DDevice9 *g_pDXDevice = 0;
D3DPRESENT_PARAMETERS g_oD3DAPP;
IDirect3D9 *g_pD3D9 = 0;
ID3DXFont *g_pFPStext = 0;
Array<DEVMODE> g_aArrModes;

//##########################################################################

void StdDrawIndexedPrimitive(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	Core_RIntSet(G_RI_INT_COUNT_DIP, Core_RIntGet(G_RI_INT_COUNT_DIP) + 1);
	g_pDXDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
}

void StdMtlSet(ID id, float4x4* world)
{

	g_pDXDevice->SetTexture(0, SGCore_LoadTexGetTex(id));
}

ID StdMtlLoad(const char* name, int mtl_type)
{
	return SGCore_LoadTexAddName(name, LOAD_TEXTURE_TYPE_LOAD);
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

IDirect3DVertexDeclaration9 *g_pStaticVertexDecl = 0;


#define SXGCORE_VERSION 1

CShaderManager *g_pManagerShaders = 0;
CreatorTextures *g_pManagerRenderTargets = 0;
LoaderTextures *g_pManagerTextures = 0;
ID3DXMesh *g_pScreenTexture = 0;
CSkyBox *g_pSkyBox = 0;
CSkyClouds *g_pSkyClouds = 0;


#define SG_PRECOND(retval) if(!g_pDXDevice){ LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxgcore is not init", GEN_MSG_LOCATION); return retval;}
#define SG_PRECOND_SKY_BOX(retval) SG_PRECOND(retval _VOID); if(!g_pSkyBox){ LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sky_box is not init", GEN_MSG_LOCATION); return retval;}
#define SG_PRECOND_SKY_CLOUDS(retval) SG_PRECOND(retval _VOID); if(!g_pSkyClouds){ LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sky_clouds is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

void GCoreInit(HWND hWnd, int iWidth, int iHeigth, bool isWindowed, DWORD dwFlags)
{
	g_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!g_pD3D9)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none detected d3d", GEN_MSG_LOCATION);
		return;
	}

	D3DCAPS9 caps;
	g_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	memset(&g_oD3DAPP, 0, sizeof(g_oD3DAPP));
	g_oD3DAPP.BackBufferWidth = iWidth;
	g_oD3DAPP.BackBufferHeight = iHeigth;
	g_oD3DAPP.BackBufferFormat = D3DFMT_A8R8G8B8;
	g_oD3DAPP.BackBufferCount = 1;
	g_oD3DAPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	g_oD3DAPP.MultiSampleQuality = 0;
	g_oD3DAPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_oD3DAPP.hDeviceWindow = hWnd;
	g_oD3DAPP.Windowed = isWindowed;
	g_oD3DAPP.EnableAutoDepthStencil = true;
	g_oD3DAPP.AutoDepthStencilFormat = D3DFMT_D24S8;
	g_oD3DAPP.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL | dwFlags | D3DCREATE_MULTITHREADED;
	g_oD3DAPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	g_oD3DAPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(g_pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | dwFlags | D3DCREATE_MULTITHREADED, &g_oD3DAPP, &g_pDXDevice)))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - failed initialized d3d", GEN_MSG_LOCATION);
		return;
	}

	D3DXFONT_DESC LF;
	ZeroMemory(&LF, sizeof(D3DXFONT_DESC));
	LF.Height = 14;
	LF.Width = 7;
	LF.Weight = 10;
	LF.Italic = 0;
	LF.CharSet = DEFAULT_CHARSET;
	sprintf(LF.FaceName, "Courier New");

	D3DXCreateFontIndirect(g_pDXDevice, &LF, &g_pFPStext);

	D3DVERTEXELEMENT9 layoutquad[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	//IDirect3DVertexDeclaration9* VertexDeclarationQuad;
	//GData::g_pDXDevice->CreateVertexDeclaration(layoutquad, &VertexDeclarationQuad);

	D3DXCreateMesh(2, 4, D3DXMESH_MANAGED, layoutquad, g_pDXDevice, &g_pScreenTexture);

	struct  VERTEX_SCREEN_TEXTURE { float x, y, z, tx, ty, tz; };

	const float offset_pixel_x = 1.0f / float(g_oD3DAPP.BackBufferWidth);
	const float offset_pixel_y = 1.0f / float(g_oD3DAPP.BackBufferHeight);

	VERTEX_SCREEN_TEXTURE AddVertices[] =
	{
		{ -1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 0.0f, 1.0f, 0 },
		{ -1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 0.0f, 0.0f, 1 },
		{ 1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 1.0f, 0.0f, 2 },
		{ 1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 1.0f, 1.0f, 3 },
	};

	void* Vertices;
	if (!FAILED(g_pScreenTexture->LockVertexBuffer(0, (void**)&Vertices)))
	{
		memcpy(Vertices, AddVertices, sizeof(AddVertices));
		g_pScreenTexture->UnlockVertexBuffer();
	}

	WORD* i = 0;
	g_pScreenTexture->LockIndexBuffer(0, (void**)&i);
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;
	g_pScreenTexture->UnlockIndexBuffer();

	
	//устанавливаем данные в регистры
	/*Core_RFloatSet(G_RI_FLOAT_WINSIZE_WIDTH, (float)width);
	Core_RFloatSet(G_RI_FLOAT_WINSIZE_HEIGHT, (float)heigth);
	Core_RBoolSet(G_RI_BOOL_RENDER_WINDOWED, windowed);*/

	g_pManagerShaders = new CShaderManager();
	g_pManagerRenderTargets = new CreatorTextures();
	g_pManagerTextures = new LoaderTextures();

	D3DVERTEXELEMENT9 layoutstatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	g_pDXDevice->CreateVertexDeclaration(layoutstatic, &g_pStaticVertexDecl);


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

SX_LIB_API void SGCore_0Create(const char *szName, HWND hWnd, int iWidth, int iHeigth, bool isWindowed, DWORD dwFlags, bool isUnic)
{
	if (szName && strlen(szName) > 1)
	{
		if (isUnic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, szName);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
				return;
			}
		}
		GCoreInit(hWnd, iWidth, iHeigth, isWindowed, dwFlags);
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name], sxgcore", GEN_MSG_LOCATION);
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
	mem_delete(g_pManagerShaders);
	mem_delete(g_pManagerRenderTargets);
	mem_delete(g_pManagerTextures);

	mem_release(g_pScreenTexture);
	mem_delete(g_pSkyBox);
	mem_delete(g_pSkyClouds);

	mem_release(g_pFPStext);
	mem_release(g_pDXDevice);
	mem_release(g_pD3D9);
}

SX_LIB_API IDirect3DDevice9* SGCore_GetDXDevice()
{
	SG_PRECOND(0);
	return g_pDXDevice;
}

SX_LIB_API void SGCore_DbgMsg(const char *szFormat, ...)
{
	SG_PRECOND(_VOID);
	
	va_list va;
	char buf[SXGC_STR_SIZE_DBG_MSG];
	va_start(va, szFormat);
	vsprintf_s(buf, SXGC_STR_SIZE_DBG_MSG, szFormat, va);
	va_end(va);

	RECT rect;
	rect.top = 10;
	rect.left = 10;
	rect.right = g_oD3DAPP.BackBufferWidth - 10;
	rect.bottom = g_oD3DAPP.BackBufferHeight - 10;
	g_pFPStext->DrawText(0, buf, -1, &rect, 0, 0xff000000);
	--rect.top;
	--rect.left;
	g_pFPStext->DrawText(0, buf, -1, &rect, 0, 0xffffffff);
}

SX_LIB_API void SGCore_OnLostDevice()
{
	SG_PRECOND(_VOID);

	g_pFPStext->OnLostDevice();
	g_pManagerRenderTargets->OnLostDevice();
}

SX_LIB_API bool SGCore_OnDeviceReset(int width, int heigth, bool windowed)
{
	SG_PRECOND(false);
	g_oD3DAPP.BackBufferWidth = width;
	g_oD3DAPP.BackBufferHeight = heigth;
	g_oD3DAPP.Windowed = windowed;

	/*Core_RFloatSet(G_RI_FLOAT_WINSIZE_WIDTH, (float)width);
	Core_RFloatSet(G_RI_FLOAT_WINSIZE_HEIGHT, (float)heigth);
	Core_RBoolSet(G_RI_BOOL_RENDER_WINDOWED, windowed);*/

	return (FAILED(g_pDXDevice->Reset(&g_oD3DAPP)));
}

SX_LIB_API void SGCore_OnResetDevice()
{
	SG_PRECOND(_VOID);

	g_pFPStext->OnResetDevice();
	g_pManagerRenderTargets->OnResetDevice();
	
	struct  VERTEX_SCREEN_TEXTURE { float x, y, z, tx, ty, tz; };

	const float offset_pixel_x = 1.0f / float(g_oD3DAPP.BackBufferWidth);
	const float offset_pixel_y = 1.0f / float(g_oD3DAPP.BackBufferHeight);

	VERTEX_SCREEN_TEXTURE AddVertices[] =
	{
		{ -1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 0.0f, 1.0f, 0 },
		{ -1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 0.0f, 0.0f, 1 },
		{ 1.0f - offset_pixel_x, 1.0f + offset_pixel_y, 1.0f, 1.0f, 0.0f, 2 },
		{ 1.0f - offset_pixel_x, -1.0f + offset_pixel_y, 1.0f, 1.0f, 1.0f, 3 },
	};

	void* Vertices;
	if (!FAILED(g_pScreenTexture->LockVertexBuffer(0, (void**)&Vertices)))
	{
		memcpy(Vertices, AddVertices, sizeof(AddVertices));
		g_pScreenTexture->UnlockVertexBuffer();
	}
}

SX_LIB_API void SGCore_ScreenQuadDraw()
{
	SG_PRECOND(_VOID);
	g_pScreenTexture->DrawSubset(0);
}

//##########################################################################

SX_LIB_API void SGCore_DIP(UINT uiTypePrimitive, long lBaseVertexIndex, UINT uiMinVertexIndex, UINT uiNumVertices, UINT uiStartIndex, UINT uiPrimitiveCount)
{
	SG_PRECOND(_VOID);
	FuncDIP(uiTypePrimitive, lBaseVertexIndex, uiMinVertexIndex, uiNumVertices, uiStartIndex, uiPrimitiveCount);
}

SX_LIB_API void SGCore_MtlSet(ID id, float4x4 *pWorld)
{
	SG_PRECOND(_VOID);
	FuncMtlSet(id, pWorld);
}

SX_LIB_API ID SGCore_MtlLoad(const char *szName, int iMtlType)
{
	SG_PRECOND(-1);
	return FuncMtlLoad(szName, iMtlType);
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


SX_LIB_API void SGCore_SetFunc_DIP(g_func_dip fnFunc)
{
	SG_PRECOND(_VOID);
	FuncDIP = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlSet(g_func_mtl_set fnFunc)
{
	SG_PRECOND(_VOID);
	FuncMtlSet = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlLoad(g_func_mtl_load fnFunc)
{
	SG_PRECOND(_VOID);
	FuncMtlLoad = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlGetSort(g_func_mtl_get_sort fnFunc)
{
	SG_PRECOND(_VOID);
	FuncMtlGetSort = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlGetPhysicType(g_func_mtl_get_physic_type fnFunc)
{
	SG_PRECOND(_VOID);
	FuncMtlGetPhysicType = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlGroupRenderIsSingly(g_func_mtl_group_render_is_singly fnFunc)
{
	SG_PRECOND(_VOID);
	FuncMtlGroupRenderIsSingly = fnFunc;
}

//##########################################################################

SX_LIB_API ID SGCore_ShaderLoad(SHADER_TYPE type_shader, const char *szPath, const char *szName, SHADER_CHECKDOUBLE check_double, D3DXMACRO *pMacro)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->preLoad(type_shader, szPath, szName, check_double, pMacro);
}

SX_LIB_API void SGCore_ShaderAllLoad()
{
	SG_PRECOND();

	return g_pManagerShaders->allLoad();
}

SX_LIB_API void SGCore_ShaderUpdateN(SHADER_TYPE type_shader, const char *szName)
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->update(type_shader, szName);
}

SX_LIB_API void SGCore_ShaderUpdate(SHADER_TYPE type_shader, ID id)
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->update(type_shader, id);
}

SX_LIB_API void SGCore_ShaderReloadAll()
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->reloadAll();
}

SX_LIB_API ID SGCore_ShaderGetID(SHADER_TYPE type_shader, const char *szNameShader)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->getID(type_shader, szNameShader);
}

SX_LIB_API void SGCore_ShaderBindN(SHADER_TYPE type_shader, const char *szNameShader)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->bind(type_shader, szNameShader);
}

SX_LIB_API void SGCore_ShaderBind(SHADER_TYPE type_shader, ID idShader)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->bind(type_shader, idShader);
}


SX_LIB_API void SGCore_ShaderUnBind()
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->unbind();
}

SX_LIB_API void SGCore_ShaderSetVRFN(SHADER_TYPE type_shader, const char* name_shader, const char* name_var, void* data, int count_float4)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->setValueRegisterF(type_shader, name_shader, name_var, data, count_float4);
}

SX_LIB_API void SGCore_ShaderSetVRF(SHADER_TYPE type_shader, ID num_shader, const char* name_var, void* data, int count_float4)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->setValueRegisterF(type_shader, num_shader, name_var, data, count_float4);
}

SX_LIB_API void SGCore_ShaderSetVRIN(SHADER_TYPE type_shader, const char* name_shader, const char* name_var, void* data, int count_int4)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->setValueRegisterI(type_shader, name_shader, name_var, data, count_int4);
}

SX_LIB_API void SGCore_ShaderSetVRI(SHADER_TYPE type_shader, ID num_shader, const char* name_var, void* data, int count_int4)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->setValueRegisterI(type_shader, num_shader, name_var, data, count_int4);
}


SX_LIB_API ID SGCore_ShaderExistsName(SHADER_TYPE type_shader, const char* name)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->existsName(type_shader, name);
}

SX_LIB_API ID SGCore_ShaderExistsPath(SHADER_TYPE type_shader, const char* name)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->existsName(type_shader, name);
}

SX_LIB_API bool SGCore_ShaderIsValidated(SHADER_TYPE type_shader, ID id)
{
	SG_PRECOND(0);

	return g_pManagerShaders->isValidated(type_shader, id);
}

SX_LIB_API void SGCore_ShaderGetPath(SHADER_TYPE type_shader, ID id, char* path)
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->getPath(type_shader, id, path);
}

SX_LIB_API void SGCore_ShaderGetName(SHADER_TYPE type_shader, ID id, char* name)
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->getName(type_shader, id, name);
}

SX_LIB_API bool SGCore_ShaderFileExists(const char* name)
{
	SG_PRECOND(false);

	return g_pManagerShaders->existsFile(name);
}

//##########################################################################

SX_LIB_API bool SGCore_LoadTexFileExists(const char* name)
{
	SG_PRECOND(false);

	return g_pManagerTextures->FileExists(name);
}

SX_LIB_API void SGCore_LoadTexClearLoaded()
{
	SG_PRECOND(_VOID);

	g_pManagerTextures->ClearLoaded();
}

SX_LIB_API void SGCore_LoadTexDelete(ID id)
{
	SG_PRECOND(_VOID);

	g_pManagerTextures->Delete(id);
}

SX_LIB_API ID SGCore_LoadTexAddName(const char* name, LOAD_TEXTURE_TYPE type)
{
	SG_PRECOND(-1);

	return g_pManagerTextures->AddName(name, type);
}

SX_LIB_API ID SGCore_LoadTexGetID(const char* name)
{
	SG_PRECOND(-1);

	return g_pManagerTextures->GetID(name);
}

SX_LIB_API void SGCore_LoadTexGetName(ID id, char* name)
{
	SG_PRECOND(_VOID);

	return g_pManagerTextures->GetName(id, name);
}

SX_LIB_API ID SGCore_LoadTexCreate(const char* name, IDirect3DTexture9* tex)
{
	SG_PRECOND(-1);

	return g_pManagerTextures->Create(name, tex);
}

SX_LIB_API ID SGCore_LoadTexUpdateN(const char* name, LOAD_TEXTURE_TYPE type)
{
	SG_PRECOND(-1);

	return g_pManagerTextures->Update(name, type);
}

SX_LIB_API void SGCore_LoadTexUpdate(ID id)
{
	SG_PRECOND(_VOID);

	return g_pManagerTextures->Update(id);
}

SX_LIB_API IDirect3DTexture9* SGCore_LoadTexGetTex(ID id)
{
	SG_PRECOND(0);

	return g_pManagerTextures->GetTexture(id);
}

SX_LIB_API void SGCore_LoadTexAllLoad()
{
	SG_PRECOND(_VOID);

	return g_pManagerTextures->LoadTextures();
}

//##########################################################################

SX_LIB_API ID SGCore_RTAdd(UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool, const char* name, float coeffullscreen)
{
	SG_PRECOND(-1);

	return g_pManagerRenderTargets->Add(width, height, levels, usage, format, pool, name, coeffullscreen);
}

SX_LIB_API void SGCore_RTDeleteN(const char* text)
{
	SG_PRECOND(_VOID);

	return g_pManagerRenderTargets->Delete(text);
}

SX_LIB_API void SGCore_RTDelete(ID num)
{
	SG_PRECOND(_VOID);

	return g_pManagerRenderTargets->Delete(num);
}

SX_LIB_API ID SGCore_RTGetNum(const char* text)
{
	SG_PRECOND(-1);

	return g_pManagerRenderTargets->GetNum(text);
}

SX_LIB_API IDirect3DTexture9* SGCore_RTGetTextureN(const char* text)
{
	SG_PRECOND(0);

	return g_pManagerRenderTargets->GetTexture(text);
}

SX_LIB_API IDirect3DTexture9* SGCore_RTGetTexture(ID num)
{
	SG_PRECOND(0);

	return g_pManagerRenderTargets->GetTexture(num);
}

//##########################################################################

SX_LIB_API void SGCore_FCreateCone(float fTopRadius, float fBottomRadius, float fHeight, ID3DXMesh ** ppMesh, UINT iSideCount)
{
	SG_PRECOND(_VOID);

	CreateCone(fTopRadius, fBottomRadius, fHeight, ppMesh, g_pDXDevice, iSideCount);
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

	CreateBoundingBoxMesh(min, max, bbmesh,g_pDXDevice);
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
	return new CFrustum();
}

SX_LIB_API ISXCamera* SGCore_CrCamera()
{
	return new CCamera();
}

SX_LIB_API ISXTransObject* SGCore_CrTransObject()
{
	return new CSXTransObject();
}

SX_LIB_API ISXBound* SGCore_CrBound()
{
	return new CSXBound();
}

//##########################################################################

SX_LIB_API void SGCore_SetSamplerFilter(DWORD id, DWORD value)
{
	SG_PRECOND(_VOID);

	g_pDXDevice->SetSamplerState(id, D3DSAMP_MAGFILTER, value);
	g_pDXDevice->SetSamplerState(id, D3DSAMP_MINFILTER, value);
	g_pDXDevice->SetSamplerState(id, D3DSAMP_MIPFILTER, value);
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

	g_pDXDevice->SetSamplerState(id, D3DSAMP_ADDRESSU, value);
	g_pDXDevice->SetSamplerState(id, D3DSAMP_ADDRESSV, value);
	g_pDXDevice->SetSamplerState(id, D3DSAMP_ADDRESSW, value);
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

	if (g_pSkyBox)
		LibReport(REPORT_MSG_LEVEL_WARNING, "sky_box is already init");
	else
		g_pSkyBox = new CSkyBox();
}

SX_LIB_API bool SGCore_SkyBoxIsCr()
{
	SG_PRECOND(false);
	return (g_pSkyBox != 0);
}

SX_LIB_API bool SGCore_SkyBoxIsLoadTex()
{
	SG_PRECOND_SKY_BOX(false);
	return g_pSkyBox->isLoadTex();
}

SX_LIB_API void SGCore_SkyBoxLoadTex(const char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	g_pSkyBox->loadTextures(texture);
}

SX_LIB_API void SGCore_SkyBoxChangeTex(const char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	g_pSkyBox->changeTexture(texture);
}

SX_LIB_API void SGCore_SkyBoxGetActiveTex(char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	g_pSkyBox->getActiveTexture(texture);
}

SX_LIB_API void SGCore_SkyBoxGetSecondTex(char *texture)
{
	SG_PRECOND_SKY_BOX(_VOID);
	g_pSkyBox->getSecondTexture(texture);
}

SX_LIB_API void SGCore_SkyBoxSetRot(float angle)
{
	SG_PRECOND_SKY_BOX(_VOID);
	g_pSkyBox->setRotation(angle);
}

SX_LIB_API float SGCore_SkyBoxGetRot()
{
	SG_PRECOND_SKY_BOX(0);
	return g_pSkyBox->getRotation();
}

SX_LIB_API void SGCore_SkyBoxSetColor(const float4_t* color)
{
	SG_PRECOND_SKY_BOX(_VOID);
	g_pSkyBox->setColor(color);
}

SX_LIB_API void SGCore_SkyBoxGetColor(float4_t* color)
{
	SG_PRECOND_SKY_BOX(_VOID);
	g_pSkyBox->getColor(color);
}

SX_LIB_API void SGCore_SkyBoxRender(float timeDelta, const float3* pos)
{
	SG_PRECOND_SKY_BOX(_VOID);
	g_pSkyBox->render(timeDelta, pos, false);
}

//**************************************************************************

SX_LIB_API void SGCore_SkyCloudsCr()
{
	SG_PRECOND(_VOID);

	if (g_pSkyClouds)
		LibReport(REPORT_MSG_LEVEL_WARNING, "sky_clouds is already init");
	else
		g_pSkyClouds = new CSkyClouds();
}

SX_LIB_API bool SGCore_SkyCloudsIsCr()
{
	SG_PRECOND(false);

	return (g_pSkyClouds != 0);
}

SX_LIB_API bool SGCore_SkyCloudsIsLoadTex()
{
	SG_PRECOND_SKY_CLOUDS(false);

	return g_pSkyClouds->isLoadTex();
}

SX_LIB_API void SGCore_SkyCloudsSetWidthHeightPos(float width, float height, const float3* pos)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	g_pSkyClouds->setWidthHeightPos(width, height, pos);
}

SX_LIB_API void SGCore_SkyCloudsLoadTex(const char *texture)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	g_pSkyClouds->loadTextures(texture);
}

SX_LIB_API void SGCore_SkyCloudsChangeTex(const char *texture)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	g_pSkyClouds->changeTexture(texture);
}

SX_LIB_API void SGCore_SkyCloudsSetRot(float angle)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	g_pSkyClouds->setRotation(angle);
}

SX_LIB_API float SGCore_SkyCloudsGetRot()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return g_pSkyClouds->getRotation();
}

SX_LIB_API void SGCore_SkyCloudsSetAlpha(float alpha)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	g_pSkyClouds->setAlpha(alpha);
}

SX_LIB_API float SGCore_SkyCloudsGetAlpha()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return g_pSkyClouds->getAlpha();
}

SX_LIB_API void SGCore_SkyCloudsSetSpeed(float speed)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	g_pSkyClouds->setSpeed(speed);
}

SX_LIB_API float SGCore_SkyCloudsGetSpeed()
{
	SG_PRECOND_SKY_CLOUDS(0);
	return g_pSkyClouds->getSpeed();
}

SX_LIB_API void SGCore_SkyCloudsSetColor(const float4_t* color)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	g_pSkyClouds->setColor(color);
}

SX_LIB_API void SGCore_SkyCloudsGetColor(float4_t* color)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	g_pSkyClouds->getColor(color);
}

SX_LIB_API void SGCore_SkyCloudsRender(DWORD timeDetlta, const float3* pos, bool is_shadow)
{
	SG_PRECOND_SKY_CLOUDS(_VOID);
	g_pSkyClouds->render(timeDetlta, pos, is_shadow);
}
