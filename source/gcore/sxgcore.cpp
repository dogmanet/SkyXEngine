
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

IDirect3DSurface9 *g_pOCsurfDepthBuffer[3];
float *g_pOCarrDepthBuffer = 0;
float4 *g_pOCarrWorldPos = 0;
float *g_pOCarrDepthBufferReProjection = 0;
float *g_pOCarrDepthBufferRasterize = 0;
bool g_isOCenable = false;
float4x4 g_mOColdView;
float4x4 g_mOColdProj;
int g_iOCcountPixels = 0;
int g_iOCWidth = 0;
int g_iOCHeight = 0;
ID RT_DepthOC[2];
int g_iOCcurrDepth = 0;
ID VS_ScreenOut = -1;
ID PS_ScreenOut = -1;
ID PS_FindMax9 = -1;

//! погрешность разниц глубин для теста occlusion culling
float g_fOCbiasDEpth = 0.0001f;

//! количество пикселей расширения треугольников для 
float g_fOCextTriangle = 2.f;

//! коэфициент размера буфера глубины occlusion culling
#define OC_SIZE_COEF 0.25f

//! дистанция (в метрах) при которой тест occlusion culling всегда будет давать true
#define OC_DIST_NEAR_NOT_CULL 4.f

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

void GCoreInit(HWND hWnd, int iWidth, int iHeight, bool isWindowed, DWORD dwFlags)
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
	g_oD3DAPP.BackBufferHeight = iHeight;
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

	
	g_iOCWidth = float(iWidth * OC_SIZE_COEF);
	g_iOCHeight = float(iHeight * OC_SIZE_COEF);
	g_iOCcountPixels = g_iOCWidth * g_iOCHeight;

	g_pDXDevice->CreateOffscreenPlainSurface(g_iOCWidth, g_iOCHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &g_pOCsurfDepthBuffer[0], 0);
	g_pDXDevice->CreateOffscreenPlainSurface(g_iOCWidth, g_iOCHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &g_pOCsurfDepthBuffer[1], 0);
	g_pDXDevice->CreateOffscreenPlainSurface(g_iOCWidth, g_iOCHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &g_pOCsurfDepthBuffer[2], 0);
	g_pOCarrDepthBuffer = new float[g_iOCcountPixels+1];
	g_pOCarrWorldPos = new float4[g_iOCcountPixels+1];
	g_pOCarrDepthBufferReProjection = new float[g_iOCcountPixels+1];
	g_pOCarrDepthBufferRasterize = new float[g_iOCcountPixels];

	RT_DepthOC[0] = SGCore_RTAdd(g_iOCWidth, g_iOCHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "depth_oc", 0.25f);
	RT_DepthOC[1] = SGCore_RTAdd(g_iOCWidth, g_iOCHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "depth_oc2", 0.25f);
	
	VS_ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	PS_ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);
	PS_FindMax9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_depth_find_max9.ps", "pp_depth_find_max9.ps", SHADER_CHECKDOUBLE_NAME);
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

	mem_release(g_pOCsurfDepthBuffer[0]);
	mem_release(g_pOCsurfDepthBuffer[1]);
	mem_release(g_pOCsurfDepthBuffer[2]);
	mem_delete_a(g_pOCarrDepthBuffer);
	mem_delete_a(g_pOCarrDepthBufferReProjection);
	mem_delete_a(g_pOCarrWorldPos);
	mem_delete_a(g_pOCarrDepthBufferRasterize);

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

	mem_release(g_pOCsurfDepthBuffer[0]);
	mem_release(g_pOCsurfDepthBuffer[1]);
	mem_release(g_pOCsurfDepthBuffer[2]);
	mem_delete_a(g_pOCarrDepthBuffer);
	mem_delete_a(g_pOCarrDepthBufferReProjection);
	mem_delete_a(g_pOCarrWorldPos);
	mem_delete_a(g_pOCarrDepthBufferRasterize);

	g_pFPStext->OnLostDevice();
	g_pManagerRenderTargets->OnLostDevice();
}

SX_LIB_API bool SGCore_OnDeviceReset(int iWidth, int iHeight, bool windowed)
{
	SG_PRECOND(false);

	g_iOCWidth = float(iWidth * OC_SIZE_COEF);
	g_iOCHeight = float(iHeight * OC_SIZE_COEF);
	g_iOCcountPixels = g_iOCWidth * g_iOCHeight;

	g_pDXDevice->CreateOffscreenPlainSurface(g_iOCWidth, g_iOCHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &g_pOCsurfDepthBuffer[0], 0);
	g_pDXDevice->CreateOffscreenPlainSurface(g_iOCWidth, g_iOCHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &g_pOCsurfDepthBuffer[1], 0);
	g_pDXDevice->CreateOffscreenPlainSurface(g_iOCWidth, g_iOCHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &g_pOCsurfDepthBuffer[2], 0);
	g_pOCarrDepthBuffer = new float[g_iOCcountPixels + 1];
	g_pOCarrWorldPos = new float4[g_iOCcountPixels + 1];
	g_pOCarrDepthBufferReProjection = new float[g_iOCcountPixels + 1];
	g_pOCarrDepthBufferRasterize = new float[g_iOCcountPixels];

	g_oD3DAPP.BackBufferWidth = iWidth;
	g_oD3DAPP.BackBufferHeight = iHeight;
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

//! небольшео расширение бокса для теста occlusion culling
const float3 g_cvOCext(0.05f, 0.05f, 0.05f);

SX_LIB_API void SGCore_OC_SetEnable(bool isEnable)
{
	SG_PRECOND(_VOID);
	g_isOCenable = isEnable;
}

const ISXFrustum *g_pFrustum = 0;
int g_iOCcountFC = 0;
int g_iOCcountFCfail = 0;

SX_LIB_API void SGCore_OC_Update(ID idDepthMap, const ISXFrustum *pFrustum)
{
	SG_PRECOND(_VOID);

	if (!g_isOCenable)
		return;

	g_pFrustum = pFrustum;
	g_iOCcountFC = 0;
	g_iOCcountFCfail = 0;

	

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	// если нет кваров плоскостей отсечения значит что-то не так
	if (!r_near || !r_far)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - cvar r_near or r_far is not found!", GEN_MSG_LOCATION);
		return;
	}

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	//если нет квара fov значит что-то не так
	if (!r_default_fov)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - cvar r_default_fov is not found!", GEN_MSG_LOCATION);
		return;
	}

	g_fOCbiasDEpth = 1.f / (*r_far);

	/*float fWidth = g_oD3DAPP.BackBufferWidth * OC_SIZE_COEF;
	float fHeight = g_oD3DAPP.BackBufferHeight * OC_SIZE_COEF;*/

	IDirect3DTexture9 *pTexDepth = SGCore_RTGetTexture(idDepthMap);
	IDirect3DTexture9 *pTexDepthOC = SGCore_RTGetTexture(RT_DepthOC[g_iOCcurrDepth]);

	//рисуем глубину с максимальной выборкой в уменьшенный буфер
	g_pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	g_pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	g_pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf2, BackBuf2;

	pTexDepthOC->GetSurfaceLevel(0, &RenderSurf2);
	g_pDXDevice->GetRenderTarget(0, &BackBuf2);
	g_pDXDevice->SetRenderTarget(0, RenderSurf2);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	g_pDXDevice->SetTexture(0, pTexDepth);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, VS_ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PS_FindMax9);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PS_FindMax9, "PixelSize", (&float2(1.f / float(g_oD3DAPP.BackBufferWidth), 1.f / float(g_oD3DAPP.BackBufferHeight))));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	g_pDXDevice->SetRenderTarget(0, BackBuf2);

	mem_release(RenderSurf2);
	mem_release(BackBuf2);

	//**********************************************************************

	//копируем отрисованную глубину в массив
	LPDIRECT3DSURFACE9 pDepthSurf;

	int iCurrOld = 1 - g_iOCcurrDepth;

	pTexDepthOC = SGCore_RTGetTexture(RT_DepthOC[g_iOCcurrDepth]);
	
	pTexDepthOC->GetSurfaceLevel(0, &pDepthSurf);
	g_pDXDevice->GetRenderTargetData(pDepthSurf, g_pOCsurfDepthBuffer[g_iOCcurrDepth]);

	D3DLOCKED_RECT  srect;
	g_pOCsurfDepthBuffer[iCurrOld]->LockRect(&srect, 0, D3DLOCK_READONLY | D3DLOCK_DONOTWAIT);
	
	memcpy(g_pOCarrDepthBuffer, srect.pBits, sizeof(float)* g_iOCcountPixels);
	g_pOCsurfDepthBuffer[iCurrOld]->UnlockRect();

	mem_release(pDepthSurf);

	//**********************************************************************
	
	float fPosX = 0, fPosY = 0;
	
	float4 vWorldPos;

	float4x4 mView, mProjection;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mProjection);

	float4x4 mViewProj = g_mOColdView * g_mOColdProj;// = mView * mProjection;
	float fD = 0.f;
	float4x4 mInvVP = SMMatrixInverse(&fD, mViewProj);

	//цикл восстановления мировой позиции каждого пикселя по глубине
	/*for (int i = 0; i < iCountPixels; ++i)
	{
		fPosY = int(float(i) / fWidth);
		fPosX = int(i - (fPosY * fWidth));

		fPosX = fPosX / fWidth;
		fPosY = fPosY / fHeight;

		fPosX = 2.f * fPosX - 1.f;
		fPosY = 2.f * (1.f - fPosY) - 1.f;
		
		vWorldPos = SMVector4Transform(float4(fPosX, fPosY, g_pOCarrDepthBuffer[i], 1.f), mInvVP);
		vWorldPos /= vWorldPos.w;
		g_pOCarrWorldPos[i] = vWorldPos;

		//g_pOCarrDepthBuffer[i] = (*r_near) / ((*r_far) + (*r_near) - g_pOCarrDepthBuffer[i] * ((*r_far) - (*r_near))); //vWorldPos.z / (*r_far);
	}*/

	float3 vObserverPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

	float fTanHalfFOV = tan((*r_default_fov) * 0.5f);
	float fAspectRatio = float(g_iOCWidth) / float(g_iOCHeight);
	float fFarY = fTanHalfFOV * (*r_far);
	float fFarX = fFarY * fAspectRatio;

	float4 vEyeRay, vWorldRay;
	float4x4 mInvView = SMMatrixInverse(&fD, g_mOColdView);

	/*for (int i = 0; i < g_iOCcountPixels; ++i)
	{
		//float linearDepth = (2.0 * (*r_near)) / ((*r_far) + (*r_near) - g_pArrDepthBufferOC[i] * ((*r_far) - (*r_near)));
		//g_pArrDepthBufferOC[i] = linearDepth;

		fPosY = int(float(i) / g_iOCWidth);
		fPosX = int(i - (fPosY * g_iOCWidth));

		fPosX = fPosX / g_iOCWidth;
		fPosY = fPosY / g_iOCHeight;

		fPosX = 2.f * fPosX - 1.f;
		fPosY = 2.f * (1.f - fPosY) - 1.f;
		vEyeRay = float4((fPosX)* fFarX, (fPosY)* fFarY, (*r_far), 0.f);
		vWorldRay = SMVector4Transform(vEyeRay, mInvView);

		vWorldPos = float4(vObserverPos + vWorldRay * g_pOCarrDepthBuffer[i], 1.f);
		g_pOCarrWorldPos[i] = vWorldPos;
		int qwerty = 0;
	}*/

	float4 aWorldRays[4];
	aWorldRays[0] = SMVector4Transform(float4(-1.f * fFarX, -1.f * fFarY, (*r_far), 0.f), mInvView);
	aWorldRays[1] = SMVector4Transform(float4(1.f * fFarX, -1.f * fFarY, (*r_far), 0.f), mInvView);
	aWorldRays[2] = SMVector4Transform(float4(1.f * fFarX, 1.f * fFarY, (*r_far), 0.f), mInvView);
	aWorldRays[3] = SMVector4Transform(float4(-1.f * fFarX, 1.f * fFarY, (*r_far), 0.f), mInvView);

	float4 vWorldRay0, vWorldRay1;

	for (int x = 0; x < g_iOCWidth; ++x)
	{
		vWorldRay0 = SMVectorLerp(aWorldRays[0], aWorldRays[1], float(x) / g_iOCWidth);
		vWorldRay1 = SMVectorLerp(aWorldRays[3], aWorldRays[2], float(x) / g_iOCWidth);
		for (int y = 0; y < g_iOCHeight; ++y)
		{
			int iPosPixel = (y * g_iOCWidth) + x;
			vWorldRay = SMVectorLerp(vWorldRay1, vWorldRay0, float(y) / g_iOCHeight);
			vWorldPos = vObserverPos + vWorldRay * g_pOCarrDepthBuffer[iPosPixel];
			vWorldPos.w = 1.f;
			g_pOCarrWorldPos[iPosPixel] = vWorldPos;
		}
	}

	g_mOColdView = mView;
	g_mOColdProj = mProjection;

	++g_iOCcurrDepth;

	if (g_iOCcurrDepth >= 2)
		g_iOCcurrDepth = 0;

	
	if (GetKeyState('M'))
	{
		g_pOCsurfDepthBuffer[2]->LockRect(&srect, 0, D3DLOCK_READONLY);

		memcpy(srect.pBits, g_pOCarrDepthBuffer, sizeof(float) * g_iOCcountPixels);
		g_pOCsurfDepthBuffer[2]->UnlockRect();

		D3DXSaveSurfaceToFile("C:/1/g_pOCsurfDepthBuffer.jpg", D3DXIFF_JPG, g_pOCsurfDepthBuffer[2], NULL, NULL);
	}
}


/*struct CTriV
{
	float3_t m_vA;
	float3_t m_vB;
	float3_t m_vC;
};

Array<CTriV> g_aVetrs;*/

SX_LIB_API void SGCore_OC_Reprojection()
{
	SG_PRECOND(_VOID);

	if (!g_isOCenable)
		return;

	/*if (GetAsyncKeyState('C'))
		g_aVetrs.clear();*/

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	if (!r_near || !r_far)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - cvar r_near or r_far is not found!", GEN_MSG_LOCATION);
		return;
	}

	/*float fWidth = g_oD3DAPP.BackBufferWidth * OC_SIZE_COEF;
	float fHeight = g_oD3DAPP.BackBufferHeight * OC_SIZE_COEF;*/

	float4x4 mView, mProjection;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mProjection);

	float4x4 mViewProj = mView * mProjection;

	//int iCountPixels = fWidth * fHeight;

	float4 vNewPos;

	for (int i = 0; i < g_iOCcountPixels; ++i)
	{
		g_pOCarrDepthBufferReProjection[i] = 1.f;
		g_pOCarrDepthBufferRasterize[i] = 1.f;
	}

	float3 vObserverPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

	float2 vNewPos2;

	//цикл репроекции каждого пикселя, расчет новой позиции в screen-space и новой глубины
	for (int i = 0; i < g_iOCcountPixels; ++i)
	{
		vNewPos = SMVector4Transform(g_pOCarrWorldPos[i], mViewProj);

		vNewPos.x /= abs(vNewPos.w);
		vNewPos.y /= abs(vNewPos.w);
		vNewPos.z = (vNewPos.z + (*r_near)) / (*r_far);
		/*vNewPos.x /= abs(vNewPos.w);
		vNewPos.y /= abs(vNewPos.w);
		vNewPos.z /= abs(vNewPos.w);*/

		vNewPos.x = vNewPos.x * 0.5f + 0.5f;
		vNewPos.y = (vNewPos.y * (-0.5f) + 0.5f);
		
		//костыль решения проблем округления, без этого будут белые линии
		
		vNewPos2.x = float(int(vNewPos.x * 10000.f) / 10000.f);
		vNewPos2.y = float(int(vNewPos.y * 10000.f) / 10000.f);

		if (vNewPos2.x == 0.f || vNewPos2.x == 1.f)
			vNewPos.x = vNewPos2.x;

		if (vNewPos2.y == 0.f || vNewPos2.y == 1.f)
			vNewPos.y = vNewPos2.y;

		//******************************************************************

		if ((vNewPos.x <= 1.f && vNewPos.x >= 0.f) && (vNewPos.y <= 1.f && vNewPos.y >= 0.f))
		{
			int x = floor(vNewPos.x * float(g_iOCWidth) + 0.5f);
			int y = floor(vNewPos.y * g_iOCHeight + 0.5f);
			int iPosPixel = int(y * g_iOCWidth) + x;

			if (iPosPixel > g_iOCcountPixels)
				int qwerty = 0;
			else
			{
				//если в буфере репроекции нет записей для текущего пикселя, либо записанная глубина меньше чем новая
				if (g_pOCarrDepthBufferReProjection[iPosPixel] >= 1.f || vNewPos.z > g_pOCarrDepthBufferReProjection[iPosPixel])
					g_pOCarrDepthBufferReProjection[iPosPixel] = vNewPos.z;
			}
		}
	}

	
	if (GetKeyState('N'))
	{
		D3DLOCKED_RECT  srect;
		g_pOCsurfDepthBuffer[2]->LockRect(&srect, 0, D3DLOCK_READONLY);

		memcpy(srect.pBits, g_pOCarrDepthBufferReProjection, sizeof(float) * g_iOCcountPixels);
		g_pOCsurfDepthBuffer[2]->UnlockRect();
		D3DXSaveSurfaceToFile("C:/1/g_pSurfDepthBufferOCreproj.jpg", D3DXIFF_JPG, g_pOCsurfDepthBuffer[2], NULL, NULL);
	}
}

inline void SwapFloat3(float3 &vA, float3 &vB)
{
	float3 vV;
	vV = vA;
	vA = vB;
	vB = vV;
}

inline void TriGetSphere(const float3 &vA, const float3 &vB, const float3 &vC, float &fRadius, float2 &vCenter)
{
	float x12 = vA.x - vB.x;
	float x23 = vB.x - vC.x;
	float x31 = vC.x - vA.x;

	float y12 = vA.y - vB.y;
	float y23 = vB.y - vC.y;
	float y31 = vC.y - vA.y;

	float z1 = (vA.x * vA.x) + (vA.y * vA.y);
	float z2 = (vB.x * vB.x) + (vB.y * vB.y);
	float z3 = (vC.x * vC.x) + (vC.y * vC.y);

	float zx = y12 * z3 + y23 * z1 + y31 * z2;
	float zy = x12 * z3 + x23 * z1 + x31 * z2;
	float z = x12 * y31 - y12 * x31;

	vCenter.x = -(zx / (2 * z));
	vCenter.y = zy / (2 * z);

	fRadius = sqrt(pow(vA.x - vCenter.x, 2) + (vA.y - vCenter.y, 2));
}

inline void QuadGetSphere(const float2 &vMin, const float2 &vMax, float &fRadius, float2 &vCenter)
{
	vCenter = (vMin + vMax) / 2;

	fRadius = SMVector2Length(vCenter - vMin);
}


inline bool OC_TriangleRasterize(const float4 &vA, const float4 &vB, const float4 &vC, bool isRasterize, const float3 &vNormal2, const float2_t &vNearFar)
{
	if (vA.z < 0.f && vB.z < 0.f && vC.z < 0.f)
		return false;

	float3 vNormal = SMVector3Normalize(SMVector3Cross(vC - vA, vB - vA));

	float3 vPointA = vA;
	float3 vPointB = vB;
	float3 vPointC = vC;

	/*int iWidth = g_oD3DAPP.BackBufferWidth * OC_SIZE_COEF;
	int iHeight = g_oD3DAPP.BackBufferHeight * OC_SIZE_COEF;*/

	//int iCountPixels = iWidth * iHeight;

	//для нахождения D достаточно использовать произвольюную точку треугольника
	float fD = -(vNormal.x * vPointA.x + vNormal.y * vPointA.y + vNormal.z * vPointA.z);

	/*
	получить интерполированное значение глубины можно так: -(N.x * x + N.y * y + D) / N.z
	N - нормаль
	x и y - текущие координаты на треугольнике
	*/

	//сортировка точек по убыванию координаты Y
	if (vPointB.y < vPointA.y)
		SwapFloat3(vPointA, vPointB);
	
	if (vPointC.y < vPointA.y)
		SwapFloat3(vPointA, vPointC);

	if (vPointB.y > vPointC.y)
		SwapFloat3(vPointB, vPointC);


	bool isVisible = false;

	//расширение треугольника, на случай неправильно округления, закрыло много багов
	vPointA.y -= g_fOCextTriangle;
	vPointC.y += g_fOCextTriangle;

	int iTotalHeight = vPointC.y - vPointA.y;

	if (iTotalHeight == 0)
		return false;

	int iSegmentHeight = vPointB.y - vPointA.y + 1;

	if (iSegmentHeight == 0)
		return false;

	int iStartY = clampf(vPointA.y, 0, g_iOCHeight - 1);
	int iFinishY = clampf(vPointB.y, 0, g_iOCHeight - 1);

	for (int y = iStartY; y <= iFinishY; ++y)
	{
		/*if (!(y >= 0 && y <= iHeight - 1))
			continue;*/

		float fA = (y - vPointA.y) / iTotalHeight;
		float fB = (y - vPointA.y) / iSegmentHeight;
		fA = vPointA.x + (vPointC.x - vPointA.x) * fA;
		fB = vPointA.x + (vPointB.x - vPointA.x) * fB;

		if (fA > fB)
			std::swap(fA, fB);

		fA = clampf(fA - g_fOCextTriangle, 0, g_iOCWidth - 1);
		fB = clampf(fB + g_fOCextTriangle, 0, g_iOCWidth - 1);

		//в оригинале отнимать и прибавлять единицу не надо, но пришлось сделать чтобы закрыть баги отсечения
		for (int x = fA; x <= fB; ++x)
		{
			/*if (x < 0 || x > iWidth - 1)
				continue;*/

			int iPosPixel = (y * g_iOCWidth) + x;
			if (iPosPixel < g_iOCcountPixels)
			{
				float fCurrDepth = (-(vNormal.x * float(x) + vNormal.y * float(y) + fD) / vNormal.z);
				
				/*if (isRasterize)
					g_pOCarrDepthBufferRasterize[iPosPixel] = 0;*/

				if (fCurrDepth >= 0.f && g_pOCarrDepthBufferReProjection[iPosPixel] >= (fCurrDepth - g_fOCbiasDEpth))
				{
					//if (!isRasterize)
						return true;
					/*else
						isVisible = true;*/
				}
			}
		}
	}


	iSegmentHeight = vPointC.y - vPointB.y + 1;

	if (iSegmentHeight == 0)
		return false;

	iStartY = clampf(vPointB.y, 0, g_iOCHeight - 1);
	iFinishY = clampf(vPointC.y, 0, g_iOCHeight - 1);

	for (int y = iStartY; y <= iFinishY; ++y)
	{
		/*if (!(y >= 0 && y <= iHeight - 1))
			continue;*/

		float fA = (y - vPointA.y) / iTotalHeight;
		float fB = (y - vPointB.y) / iSegmentHeight;
		fA = vPointA.x + (vPointC.x - vPointA.x) * fA;
		fB = vPointB.x + (vPointC.x - vPointB.x) * fB;
       
		if (fA > fB)
			std::swap(fA, fB);

		fA = clampf(fA - g_fOCextTriangle, 0, g_iOCWidth - 1);
		fB = clampf(fB + g_fOCextTriangle, 0, g_iOCWidth - 1);

		//в оригинале отнимать и прибавлять единицу не надо, но пришлось сделать чтобы закрыть баги отсечения
		for (int x = fA; x <= fB; ++x)
		{
			/*if (x < 0 || x > iWidth - 1)
				continue;*/

			int iPosPixel = (y * g_iOCWidth) + x;
			if (iPosPixel < g_iOCcountPixels)
			{
				float fCurrDepth = (-(vNormal.x * float(x) + vNormal.y * float(y) + fD) / vNormal.z);
				
				/*if (isRasterize)
					g_pOCarrDepthBufferRasterize[iPosPixel] = 0;*/

				if (fCurrDepth >= 0.f && g_pOCarrDepthBufferReProjection[iPosPixel] >= (fCurrDepth - g_fOCbiasDEpth))
				{
					//if (!isRasterize)
						return true;
					/*else
						isVisible = true;*/
				}
			}
        }
    }

	return isVisible;
}

inline bool OC_RasterizeQuad(const float3 &vA, const float3 &vB, const float3 &vC, const float3 &vD, const float2 &vNearFar)
{
	float3 vNormal = TriGetNormal(vA, vB, vC);
	return (OC_TriangleRasterize(vA, vB, vC, false, vNormal, vNearFar) || OC_TriangleRasterize(vB, vC, vD, false, vNormal, vNearFar));
}



inline bool OC_FrustumCulling(const float3 &vA, const float3 &vB, const float3 &vC)
{
	if (GetAsyncKeyState('P'))
		return true;
	++g_iOCcountFC;
	bool isVisible = (!g_pFrustum || g_pFrustum->polyInFrustum(&vA, &vB, &vC));
	if (!isVisible)
		++g_iOCcountFCfail;
	return isVisible;
}


SX_LIB_API bool SGCore_OC_IsVisible(const float3 *pMax, const float3 *pMin)
{
	SG_PRECOND(false);

	if (!pMax || !pMin)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - min or max is null!", GEN_MSG_LOCATION);
		return false;
	}

	if (!g_isOCenable)
		return true;

	float3 vObserverPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	if (!r_near || !r_far)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - cvar r_near or r_far is not found!", GEN_MSG_LOCATION);
		return false;
	}

	float3 vMax, vMin;
	vMax = *pMax;
	vMin = *pMin;

	//вычисление центра и радиуса по xz чтобы определить дистанцию до наблюдателя
	float3 vCenter = (vMin + vMax) * 0.5f;
	float fRadius = SMVector3Length2(float3(vCenter.x, 0.f, vCenter.z) - float3(vMax.x, 0, vMax.z));

	float fDist = SMVector3Length2(vObserverPos - vCenter);

	//если дистанция до наблюдаеля меньше либо равна радиусу бокса по xz значит наблюдатель в боксе
	if (fDist <= fRadius)
		return true;

	fDist -= fRadius;

	//если бокс в пределах ближнего расстояния неотсечения от наблюдателя, значит он виден
	if (fDist >= 0.f && fDist <= OC_DIST_NEAR_NOT_CULL*OC_DIST_NEAR_NOT_CULL)
		return true;

	//закрывало много багов ложного отсечения, однако на дальних дистанциях не отсекает как надо, возможно просто буфер глубины слишком уменьшенный
	vMax += g_cvOCext;
	vMin -= g_cvOCext;


	/*float fWidth = g_oD3DAPP.BackBufferWidth * OC_SIZE_COEF;
	float fHeight = g_oD3DAPP.BackBufferHeight * OC_SIZE_COEF;

	int iCountPixels = fWidth * fHeight;*/

	float4x4 mWorld, mView, mProjection;
	Core_RMatrixGet(G_RI_MATRIX_WORLD, &mWorld);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mProjection);

	float4x4 mWVP = mWorld * mView *mProjection;
	

	float4 aSSPoints[8];
	float4 aWPoints[8];
	aWPoints[0] = float4(vMax.x, vMax.y, vMax.z, 1.0f);
	aWPoints[1] = float4(vMax.x, vMax.y, vMin.z, 1.0f);
	aWPoints[2] = float4(vMax.x, vMin.y, vMax.z, 1.0f);
	aWPoints[3] = float4(vMin.x, vMax.y, vMax.z, 1.0f);
	aWPoints[4] = float4(vMax.x, vMin.y, vMin.z, 1.0f);
	aWPoints[5] = float4(vMin.x, vMin.y, vMax.z, 1.0f);
	aWPoints[6] = float4(vMin.x, vMax.y, vMin.z, 1.0f);
	aWPoints[7] = float4(vMin.x, vMin.y, vMin.z, 1.0f);

	for (int i = 0; i < 8; ++i)
	{
		aSSPoints[i] = SMVector4Transform(aWPoints[i], mWVP);

		aSSPoints[i].x /= abs(aSSPoints[i].w);
		aSSPoints[i].y /= abs(aSSPoints[i].w);
		aSSPoints[i].z = ((aSSPoints[i].z + (*r_near)) / (*r_far));// *sign(aSSPoints[i].w);

		//просчет линейной глубины из нелинейной
		//float fLinearDepth = (*r_near) / ((*r_far) + (*r_near) - aPoints[i].w * ((*r_far) - (*r_near)));
		
		aSSPoints[i].x = aSSPoints[i].x * 0.5 + 0.5;
		aSSPoints[i].y = aSSPoints[i].y * (-0.5) + 0.5;

		aSSPoints[i].x *= g_iOCWidth;
		aSSPoints[i].y *= g_iOCHeight;

		aSSPoints[i].x = int(aSSPoints[i].x);
		aSSPoints[i].y = int(aSSPoints[i].y);

		/*float2 vNewPos2;
		vNewPos2.x = float(int(aPoints[i].x * 1000) / 1000.f);
		vNewPos2.y = float(int(aPoints[i].y * 1000) / 1000.f);

		if (vNewPos2.x == 0.f || vNewPos2.x == 1.f)
			aPoints[i].x = vNewPos2.x;

		if (vNewPos2.y == 0.f || vNewPos2.y == 1.f)
			aPoints[i].y = vNewPos2.y;

		aPoints[i].x = int(floor(aPoints[i].x * fMapWidth + 0.5f));
		aPoints[i].y = int(floor(aPoints[i].y * fMapHeight + 0.5f));*/
	}

	float2 vNearFar((*r_near), (*r_far));

	/*float3 aCenters[6];

	//зад
	aCenters[0] = (aWPoints[7] + aWPoints[1]) * 0.5f;// SMVectorLerp(aWPoints[7], aWPoints[1], 0.5f);
	//перед
	aCenters[1] = (aWPoints[0] + aWPoints[5]) * 0.5f;// SMVectorLerp(aWPoints[0], aWPoints[5], 0.5f);

	//верх
	aCenters[2] = (aWPoints[0] + aWPoints[6]) * 0.5f;// SMVectorLerp(aWPoints[0], aWPoints[6], 0.5f);
	//низ
	aCenters[3] = (aWPoints[7] + aWPoints[2]) * 0.5f;// SMVectorLerp(aWPoints[7], aWPoints[2], 0.5f);

	//лево
	aCenters[4] = (aWPoints[7] + aWPoints[3]) * 0.5f;// SMVectorLerp(aWPoints[7], aWPoints[3], 0.5f);
	//право
	aCenters[5] = (aWPoints[0] + aWPoints[4]) * 0.5f;// SMVectorLerp(aWPoints[0], aWPoints[4], 0.5f);


	float aDist2[6];

	for (int i = 0; i < 6; ++i)
	{
		aDist2[i] = SMVector3Length2(aCenters[i] - vObserverPos);
	}*/



	//bool isVisible = (
		//(/*(aDist2[0] <= aDist2[1] || GetAsyncKeyState('F')*) &&*/ OC_RasterizeQuad(aSSPoints[6], aSSPoints[1], aSSPoints[4], aSSPoints[7], vNearFar)) ||

		//(/*(aDist2[1] <= aDist2[0] || GetAsyncKeyState('F')) &&*/ OC_RasterizeQuad(aSSPoints[0], aSSPoints[3], aSSPoints[5], aSSPoints[2], vNearFar)) ||

		//(/*(aDist2[2] <= aDist2[3] || GetAsyncKeyState('F')) &&*/ OC_RasterizeQuad(aSSPoints[3], aSSPoints[0], aSSPoints[1], aSSPoints[6], vNearFar)) ||

		//(/*(aDist2[3] <= aDist2[2] || GetAsyncKeyState('F')) &&*/ OC_RasterizeQuad(aSSPoints[7], aSSPoints[4], aSSPoints[2], aSSPoints[5], vNearFar)) ||

		//(/*(aDist2[4] <= aDist2[5] || GetAsyncKeyState('F')) &&*/ OC_RasterizeQuad(aSSPoints[3], aSSPoints[6], aSSPoints[7], aSSPoints[5], vNearFar)) ||

		//(/*(aDist2[5] <= aDist2[4] || GetAsyncKeyState('F')) &&*/ OC_RasterizeQuad(aSSPoints[1], aSSPoints[0], aSSPoints[2], aSSPoints[4], vNearFar))
		//);

	/*bool isVisible = (
		((aDist2[0] <= aDist2[1] || GetAsyncKeyState('F')) && 
		OC_TriangleRasterize(aSSPoints[6], aSSPoints[1], aSSPoints[4], false, aSSPoints[7], vNearFar) && 
		OC_TriangleRasterize(aSSPoints[6], aSSPoints[4], aSSPoints[7], false, aSSPoints[7], vNearFar)) ||

		((aDist2[1] <= aDist2[0] || GetAsyncKeyState('F')) && 
		OC_TriangleRasterize(aSSPoints[0], aSSPoints[3], aSSPoints[5], false, aSSPoints[2], vNearFar) &&
		OC_TriangleRasterize(aSSPoints[0], aSSPoints[5], aSSPoints[2], false, aSSPoints[2], vNearFar)) ||

		((aDist2[2] <= aDist2[3] || GetAsyncKeyState('F')) && 
		OC_TriangleRasterize(aSSPoints[3], aSSPoints[0], aSSPoints[1], false, aSSPoints[6], vNearFar) &&
		OC_TriangleRasterize(aSSPoints[3], aSSPoints[1], aSSPoints[6], false, aSSPoints[6], vNearFar)) ||

		((aDist2[3] <= aDist2[2] || GetAsyncKeyState('F')) && 
		OC_TriangleRasterize(aSSPoints[7], aSSPoints[4], aSSPoints[2], false, aSSPoints[5], vNearFar) &&
		OC_TriangleRasterize(aSSPoints[7], aSSPoints[2], aSSPoints[5], false, aSSPoints[5], vNearFar)) ||

		((aDist2[4] <= aDist2[5] || GetAsyncKeyState('F')) && 
		OC_TriangleRasterize(aSSPoints[3], aSSPoints[6], aSSPoints[7], false, aSSPoints[5], vNearFar) &&
		OC_TriangleRasterize(aSSPoints[3], aSSPoints[7], aSSPoints[5], false, aSSPoints[5], vNearFar)) ||

		((aDist2[5] <= aDist2[4] || GetAsyncKeyState('F')) && 
		OC_TriangleRasterize(aSSPoints[1], aSSPoints[0], aSSPoints[2], false, aSSPoints[4], vNearFar) &&
		OC_TriangleRasterize(aSSPoints[1], aSSPoints[2], aSSPoints[4], false, aSSPoints[4], vNearFar))
		);*/


	//зад
	/*TriangleRasterize((float3)aPoints[7], (float3)aPoints[6], (float3)aPoints[1], aNormals[0], vNearFar);
	TriangleRasterize((float3)aPoints[7], (float3)aPoints[1], (float3)aPoints[4], aNormals[0], vNearFar);

	//перед
	TriangleRasterize((float3)aPoints[0], (float3)aPoints[5], (float3)aPoints[2], aNormals[0], vNearFar);
	TriangleRasterize((float3)aPoints[0], (float3)aPoints[3], (float3)aPoints[5], aNormals[0], vNearFar);

	//----

	//верх
	TriangleRasterize((float3)aPoints[0], (float3)aPoints[1], (float3)aPoints[3], aNormals[0], vNearFar);
	TriangleRasterize((float3)aPoints[1], (float3)aPoints[6], (float3)aPoints[3], aNormals[0], vNearFar);

	//низ
	TriangleRasterize((float3)aPoints[5], (float3)aPoints[4], (float3)aPoints[2], aNormals[0], vNearFar);
	TriangleRasterize((float3)aPoints[4], (float3)aPoints[5], (float3)aPoints[7], aNormals[0], vNearFar);

	//----

	//лево
	TriangleRasterize((float3)aPoints[0], (float3)aPoints[2], (float3)aPoints[1], aNormals[0], vNearFar);
	TriangleRasterize((float3)aPoints[1], (float3)aPoints[2], (float3)aPoints[4], aNormals[0], vNearFar);

	//право
	TriangleRasterize((float3)aPoints[6], (float3)aPoints[5], (float3)aPoints[3], aNormals[0], vNearFar);
	TriangleRasterize((float3)aPoints[5], (float3)aPoints[6], (float3)aPoints[7], aNormals[0], vNearFar);*/

	//float3 aNormals[12];
	/*aNormals[0] = TriGetNormal(aWPoints[7], aWPoints[6], aWPoints[1]);
	aNormals[1] = TriGetNormal(aWPoints[7], aWPoints[1], aWPoints[4]);
	aNormals[2] = TriGetNormal(aWPoints[0], aWPoints[5], aWPoints[2]);
	aNormals[3] = TriGetNormal(aWPoints[0], aWPoints[3], aWPoints[5]);
	aNormals[4] = TriGetNormal(aWPoints[0], aWPoints[1], aWPoints[3]);
	aNormals[5] = TriGetNormal(aWPoints[1], aWPoints[6], aWPoints[3]);
	aNormals[6] = TriGetNormal(aWPoints[5], aWPoints[4], aWPoints[2]);
	aNormals[7] = TriGetNormal(aWPoints[4], aWPoints[5], aWPoints[7]);
	aNormals[8] = TriGetNormal(aWPoints[0], aWPoints[2], aWPoints[1]);
	aNormals[9] = TriGetNormal(aWPoints[1], aWPoints[2], aWPoints[4]);
	aNormals[10] = TriGetNormal(aWPoints[6], aWPoints[5], aWPoints[3]);
	aNormals[11] = TriGetNormal(aWPoints[5], aWPoints[6], aWPoints[7]);

	float3 vObserverDir;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_DIRECTION, &vObserverDir);*/

	//-13.92, 0.25, -52.76
	/*if (GetAsyncKeyState('C') 
		)
	{
		if (aNormals[0].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[7]), float3_t(aWPoints[6]), float3_t(aWPoints[1]) });

		if (aNormals[1].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[7]), float3_t(aWPoints[1]), float3_t(aWPoints[4]) });

		if (aNormals[2].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[0]), float3_t(aWPoints[5]), float3_t(aWPoints[2]) });

		if (aNormals[3].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[0]), float3_t(aWPoints[3]), float3_t(aWPoints[5]) });

		if (aNormals[4].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[0]), float3_t(aWPoints[1]), float3_t(aWPoints[3]) });

		if (aNormals[5].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[1]), float3_t(aWPoints[6]), float3_t(aWPoints[3]) });

		if (aNormals[6].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[5]), float3_t(aWPoints[4]), float3_t(aWPoints[2]) });

		if (aNormals[7].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[4]), float3_t(aWPoints[5]), float3_t(aWPoints[7]) });

		if (aNormals[8].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[0]), float3_t(aWPoints[2]), float3_t(aWPoints[1]) });

		if (aNormals[9].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[1]), float3_t(aWPoints[2]), float3_t(aWPoints[4]) });

		if (aNormals[10].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[6]), float3_t(aWPoints[5]), float3_t(aWPoints[3]) });

		if (aNormals[11].z > 0.f)
			g_aVetrs.push_back({ float3_t(aWPoints[5]), float3_t(aWPoints[6]), float3_t(aWPoints[7]) });
	}*/

	/*if (g_aVetrs.size() > 0)
	{
		SGCore_ShaderUnBind();

		g_pDXDevice->SetTransform(D3DTS_WORLD, &((D3DXMATRIX)SMMatrixIdentity()));
		g_pDXDevice->SetTransform(D3DTS_VIEW, &((D3DXMATRIX)mView));
		g_pDXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)mProjection));
		g_pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		g_pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		g_pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		//g_pDXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		g_pDXDevice->SetTexture(0, 0);
		g_pDXDevice->SetFVF(D3DFVF_XYZ);
		g_pDXDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, g_aVetrs.size(), &(g_aVetrs[0]), sizeof(float3_t));
		//g_pDXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}*/


	/*bool isVisible = (
		((SMVector3Dot(aNormals[0], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[7], aSSPoints[6], aSSPoints[1], false, vNearFar)) ||
		((SMVector3Dot(aNormals[1], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[7], aSSPoints[1], aSSPoints[4], false, vNearFar)) ||
		((SMVector3Dot(aNormals[2], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[0], aSSPoints[5], aSSPoints[2], false, vNearFar)) ||
		((SMVector3Dot(aNormals[3], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[0], aSSPoints[3], aSSPoints[5], false, vNearFar)) ||
		((SMVector3Dot(aNormals[4], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[0], aSSPoints[1], aSSPoints[3], false, vNearFar)) ||
		((SMVector3Dot(aNormals[5], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[1], aSSPoints[6], aSSPoints[3], false, vNearFar)) ||
		((SMVector3Dot(aNormals[6], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[5], aSSPoints[4], aSSPoints[2], false, vNearFar)) ||
		((SMVector3Dot(aNormals[7], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[4], aSSPoints[5], aSSPoints[7], false, vNearFar)) ||
		((SMVector3Dot(aNormals[8], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[0], aSSPoints[2], aSSPoints[1], false, vNearFar)) ||
		((SMVector3Dot(aNormals[9], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[1], aSSPoints[2], aSSPoints[4], false, vNearFar)) ||
		((SMVector3Dot(aNormals[10], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[6], aSSPoints[5], aSSPoints[3], false, vNearFar)) ||
		((SMVector3Dot(aNormals[11], vObserverDir) <= 0.f || GetAsyncKeyState('F')) && OC_TriangleRasterize(aSSPoints[5], aSSPoints[6], aSSPoints[7], false, vNearFar))
		);*/

	bool isVisible = (
		(OC_FrustumCulling(aWPoints[7], aWPoints[6], aWPoints[1]) && OC_TriangleRasterize(aSSPoints[7], aSSPoints[6], aSSPoints[1], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[7], aWPoints[1], aWPoints[4]) && OC_TriangleRasterize(aSSPoints[7], aSSPoints[1], aSSPoints[4], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[0], aWPoints[5], aWPoints[2]) && OC_TriangleRasterize(aSSPoints[0], aSSPoints[5], aSSPoints[2], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[0], aWPoints[3], aWPoints[5]) && OC_TriangleRasterize(aSSPoints[0], aSSPoints[3], aSSPoints[5], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[0], aWPoints[1], aWPoints[3]) && OC_TriangleRasterize(aSSPoints[0], aSSPoints[1], aSSPoints[3], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[1], aWPoints[6], aWPoints[3]) && OC_TriangleRasterize(aSSPoints[1], aSSPoints[6], aSSPoints[3], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[5], aWPoints[4], aWPoints[2]) && OC_TriangleRasterize(aSSPoints[5], aSSPoints[4], aSSPoints[2], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[4], aWPoints[5], aWPoints[7]) && OC_TriangleRasterize(aSSPoints[4], aSSPoints[5], aSSPoints[7], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[0], aWPoints[2], aWPoints[1]) && OC_TriangleRasterize(aSSPoints[0], aSSPoints[2], aSSPoints[1], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[1], aWPoints[2], aWPoints[4]) && OC_TriangleRasterize(aSSPoints[1], aSSPoints[2], aSSPoints[4], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[6], aWPoints[5], aWPoints[3]) && OC_TriangleRasterize(aSSPoints[6], aSSPoints[5], aSSPoints[3], false, aSSPoints[0], vNearFar)) ||
		(OC_FrustumCulling(aWPoints[5], aWPoints[6], aWPoints[7]) && OC_TriangleRasterize(aSSPoints[5], aSSPoints[6], aSSPoints[7], false, aSSPoints[0], vNearFar))
		);

	return isVisible;
}

//##########################################################################

SX_LIB_API ID SGCore_ShaderLoad(SHADER_TYPE type_shader, const char *szPath, const char *szName, SHADER_CHECKDOUBLE check_double, D3DXMACRO *pMacro)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->preLoad(type_shader, szPath, szName, check_double, pMacro);
}

SX_LIB_API void SGCore_ShaderAllLoad()
{
	SG_PRECOND(_VOID);

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
