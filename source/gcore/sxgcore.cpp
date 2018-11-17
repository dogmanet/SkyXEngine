
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
#include <gcore/oc.h>

#include <gcore/loader_static.h>
#include <gcore/sky.h>

#include <gcore/gcore_utils.h>
#include <gcore/gcore_data.h>


#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

//##########################################################################

#define SXGCORE_VERSION 1


IDirect3DDevice9 *g_pDXDevice = 0;
D3DPRESENT_PARAMETERS g_oD3DAPP;
IDirect3D9 *g_pD3D9 = 0;
ID3DXFont *g_pFPStext = 0;
Array<DEVMODE> g_aModes;

IDirect3DVertexDeclaration9 *g_pStaticVertexDecl = 0;

CShaderManager *g_pManagerShaders = 0;
CreatorTextures *g_pManagerRenderTargets = 0;
ÑLoaderTextures *g_pManagerTextures = 0;
ID3DXMesh *g_pScreenTexture = 0;
CSkyBox *g_pSkyBox = 0;
CSkyClouds *g_pSkyClouds = 0;
COcclusionCulling *g_pOC = 0;

//##########################################################################

void StdDrawIndexedPrimitive(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{
	Core_RIntSet(G_RI_INT_COUNT_DIP, Core_RIntGet(G_RI_INT_COUNT_DIP) + 1);
	g_pDXDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)type_primitive, base_vertexIndex, min_vertex_index, num_vertices, start_index, prim_count);
}

void StdMtlSet(ID id, float4x4 *pWorld)
{

	g_pDXDevice->SetTexture(0, SGCore_LoadTexGetTex(id));
}

ID StdMtlLoad(const char *szName, int iMtlType)
{
	return SGCore_LoadTexAddName(szName, LOAD_TEXTURE_TYPE_LOAD);
}

bool StdMtlIsTransparency(ID id)
{
	return false;
}

int StdMtlGetPhysicType(ID id)
{
	return 0;
}

bool StdMtlGroupIsSyngly(ID id)
{
	return false;
}

g_func_dip g_fnDIP = StdDrawIndexedPrimitive;
g_func_mtl_set g_fnMtlSet = StdMtlSet;
g_func_mtl_load g_fnMtlLoad = StdMtlLoad;
g_func_mtl_is_transparency g_fnMtlIsTransparency = StdMtlIsTransparency;
g_func_mtl_get_physic_type g_fnMtlGetPhysicType = StdMtlGetPhysicType;
g_func_mtl_group_render_is_singly g_fnMtlGroupRenderIsSingly = StdMtlGroupIsSyngly;

//##########################################################################

#define SG_PRECOND(retval) if(!g_pDXDevice){ LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxgcore is not init", GEN_MSG_LOCATION); return retval;}
#define SG_PRECOND_SKY_BOX(retval) SG_PRECOND(retval _VOID); if(!g_pSkyBox){ LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sky_box is not init", GEN_MSG_LOCATION); return retval;}
#define SG_PRECOND_SKY_CLOUDS(retval) SG_PRECOND(retval _VOID); if(!g_pSkyClouds){ LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sky_clouds is not init", GEN_MSG_LOCATION); return retval;}
#define SG_PRECOND_SKY_OC(retval) SG_PRECOND(retval _VOID); if(!g_pOC){ LibReport(REPORT_MSG_LEVEL_ERROR, "%s - occlusion culling is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

void GCoreInit(HWND hWnd, int iWidth, int iHeight, bool isWindowed, DWORD dwFlags)
{
	InitDevice(hWnd, iWidth, iHeight, isWindowed, dwFlags);
	InitFPStext();
	InitFullScreenQuad();

	g_pManagerShaders = new CShaderManager();
	g_pManagerRenderTargets = new CreatorTextures();
	g_pManagerTextures = new ÑLoaderTextures();
	g_pOC = new COcclusionCulling();
	g_pOC->init(g_oD3DAPP.BackBufferWidth, g_oD3DAPP.BackBufferHeight);

	D3DVERTEXELEMENT9 oLayoutStatic[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		D3DDECL_END()
	};

	g_pDXDevice->CreateVertexDeclaration(oLayoutStatic, &g_pStaticVertexDecl);

	InitArrModes();
	InitRT4Gbuffer();
	LoadShaders();
}

//##########################################################################

SX_LIB_API ID SGCore_GbufferGetRT_ID(DS_RT type)
{
	SG_PRECOND(-1);

	if (type == DS_RT_COLOR)
		return gcore_data::rt_id::idColorScene;
	else if (type == DS_RT_NORMAL)
		return gcore_data::rt_id::idNormalScene;
	else if (type == DS_RT_PARAM)
		return gcore_data::rt_id::idParamsScene;
	else if (type == DS_RT_DEPTH)
		return gcore_data::rt_id::idDepthScene;
	else if (type == DS_RT_DEPTH0)
		return gcore_data::rt_id::idDepthScene0;
	else if (type == DS_RT_DEPTH1)
		return gcore_data::rt_id::idDepthScene1;

	else if (type == DS_RT_AMBIENTDIFF)
		return gcore_data::rt_id::idLightAmbientDiff;
	else if (type == DS_RT_SPECULAR)
		return gcore_data::rt_id::idLightSpecular;

	else if (type == DS_RT_SCENELIGHT)
		return gcore_data::rt_id::idLigthCom;
	else if (type == DS_RT_SCENELIGHT2)
		return gcore_data::rt_id::idLigthCom2;

	else if (type == DS_RT_ADAPTEDLUM)
		return gcore_data::rt_id::GetCurrAdaptedLum();

	return -1;
}

SX_LIB_API IDirect3DTexture9* SGCore_GbufferGetRT(DS_RT type)
{
	SG_PRECOND(0);

	ID id = SGCore_GbufferGetRT_ID(type);
	if (id >= 0)
		return SGCore_RTGetTexture(id);
	else
		return 0;
}

SX_LIB_API void SGCore_ToneMappingCom(DWORD timeDelta, float fFactorAdapted)
{
	ToneMappingCom(timeDelta, fFactorAdapted);
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

SX_LIB_API const DEVMODE* SGCore_GetModes(int *pCount)
{
	SG_PRECOND(0);

	if (pCount)
		*pCount = g_aModes.size();

	return &(g_aModes[0]);
}

SX_LIB_API HWND SGCore_GetHWND()
{
	return g_oD3DAPP.hDeviceWindow;
}

SX_LIB_API void SGCore_AKill()
{
	SG_PRECOND(_VOID);

	mem_delete(g_pOC);

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

	g_pOC->onLostDevice();
}

SX_LIB_API bool SGCore_OnDeviceReset(int iWidth, int iHeight, bool isWindowed)
{
	SG_PRECOND(false);

	g_oD3DAPP.BackBufferWidth = iWidth;
	g_oD3DAPP.BackBufferHeight = iHeight;
	g_oD3DAPP.Windowed = isWindowed;

	return (FAILED(g_pDXDevice->Reset(&g_oD3DAPP)));
}

SX_LIB_API void SGCore_OnResetDevice()
{
	SG_PRECOND(_VOID);

	g_pFPStext->OnResetDevice();
	g_pManagerRenderTargets->OnResetDevice();
	g_pOC->onResetDevice(g_oD3DAPP.BackBufferWidth, g_oD3DAPP.BackBufferHeight);
	
	struct  VERTEX_SCREEN_TEXTURE { float x, y, z, tx, ty, tz; };

	const float fOffsetPixelX = 1.0f / float(g_oD3DAPP.BackBufferWidth);
	const float fOffsetPixelY = 1.0f / float(g_oD3DAPP.BackBufferHeight);

	VERTEX_SCREEN_TEXTURE aVertices[] =
	{
		{ -1.0f - fOffsetPixelX, -1.0f + fOffsetPixelY, 1.0f, 0.0f, 1.0f, 0 },
		{ -1.0f - fOffsetPixelX,  1.0f + fOffsetPixelY, 1.0f, 0.0f, 0.0f, 1 },
		{  1.0f - fOffsetPixelX,  1.0f + fOffsetPixelY, 1.0f, 1.0f, 0.0f, 2 },
		{  1.0f - fOffsetPixelX, -1.0f + fOffsetPixelY, 1.0f, 1.0f, 1.0f, 3 },
	};

	void * pVertices;
	if (!FAILED(g_pScreenTexture->LockVertexBuffer(0, (void**)&pVertices)))
	{
		memcpy(pVertices, aVertices, sizeof(aVertices));
		g_pScreenTexture->UnlockVertexBuffer();
	}

	gcore_data::rt_id::iHowAdaptedLum = 0;
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
	g_fnDIP(uiTypePrimitive, lBaseVertexIndex, uiMinVertexIndex, uiNumVertices, uiStartIndex, uiPrimitiveCount);
}

SX_LIB_API void SGCore_MtlSet(ID id, float4x4 *pWorld)
{
	SG_PRECOND(_VOID);
	g_fnMtlSet(id, pWorld);
}

SX_LIB_API ID SGCore_MtlLoad(const char *szName, int iMtlType)
{
	SG_PRECOND(-1);
	return g_fnMtlLoad(szName, iMtlType);
}

SX_LIB_API bool SGCore_MtlIsTransparency(ID id)
{
	SG_PRECOND(false);
	return g_fnMtlIsTransparency(id);
}

SX_LIB_API int SGCore_MtlGetPhysicType(ID id)
{
	SG_PRECOND(-1);
	return g_fnMtlGetPhysicType(id);
}

SX_LIB_API bool SGCore_MtlGroupRenderIsSingly(ID id)
{
	SG_PRECOND(false);
	return g_fnMtlGroupRenderIsSingly(id);
}


SX_LIB_API void SGCore_SetFunc_DIP(g_func_dip fnFunc)
{
	SG_PRECOND(_VOID);
	g_fnDIP = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlSet(g_func_mtl_set fnFunc)
{
	SG_PRECOND(_VOID);
	g_fnMtlSet = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlLoad(g_func_mtl_load fnFunc)
{
	SG_PRECOND(_VOID);
	g_fnMtlLoad = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlIsTransparency(g_func_mtl_is_transparency fnFunc)
{
	SG_PRECOND(_VOID);
	g_fnMtlIsTransparency = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlGetPhysicType(g_func_mtl_get_physic_type fnFunc)
{
	SG_PRECOND(_VOID);
	g_fnMtlGetPhysicType = fnFunc;
}

SX_LIB_API void SGCore_SetFunc_MtlGroupRenderIsSingly(g_func_mtl_group_render_is_singly fnFunc)
{
	SG_PRECOND(_VOID);
	g_fnMtlGroupRenderIsSingly = fnFunc;
}

//##########################################################################

SX_LIB_API void SGCore_OC_SetEnable(bool isEnable)
{
	SG_PRECOND_SKY_OC(_VOID);
	g_pOC->setEnable(isEnable);
}

SX_LIB_API void SGCore_OC_Update(ID idDepthMap, const IFrustum *pFrustum)
{
	SG_PRECOND_SKY_OC(_VOID);

	g_pOC->update(idDepthMap, pFrustum);
}

SX_LIB_API void SGCore_OC_Reprojection()
{
	SG_PRECOND_SKY_OC(_VOID);

	g_pOC->reprojection();
}

SX_LIB_API bool SGCore_OC_IsVisible(const float3 *pMax, const float3 *pMin)
{
	SG_PRECOND_SKY_OC(false);

	return g_pOC->comVisible(pMax, pMin);
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

SX_LIB_API void SGCore_ShaderSetVRFN(SHADER_TYPE type_shader, const char *szNameShader, const char *szNameVar, void *pData, int iCountFloat4)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->setValueRegisterF(type_shader, szNameShader, szNameVar, pData, iCountFloat4);
}

SX_LIB_API void SGCore_ShaderSetVRF(SHADER_TYPE type_shader, ID idShader, const char *szNameVar, void *pData, int iCountFloat4)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->setValueRegisterF(type_shader, idShader, szNameVar, pData, iCountFloat4);
}

SX_LIB_API void SGCore_ShaderSetVRIN(SHADER_TYPE type_shader, const char *szNameShader, const char *szNameVar, void *pData, int iCountInt4)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->setValueRegisterI(type_shader, szNameShader, szNameVar, pData, iCountInt4);
}

SX_LIB_API void SGCore_ShaderSetVRI(SHADER_TYPE type_shader, ID idShader, const char* szNameVar, void *pData, int iCountInt4)
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->setValueRegisterI(type_shader, idShader, szNameVar, pData, iCountInt4);
}


SX_LIB_API ID SGCore_ShaderExistsName(SHADER_TYPE type_shader, const char *szName)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->existsName(type_shader, szName);
}

SX_LIB_API ID SGCore_ShaderExistsPath(SHADER_TYPE type_shader, const char *szName)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->existsName(type_shader, szName);
}

SX_LIB_API bool SGCore_ShaderIsValidated(SHADER_TYPE type_shader, ID idShader)
{
	SG_PRECOND(0);

	return g_pManagerShaders->isValidated(type_shader, idShader);
}

SX_LIB_API void SGCore_ShaderGetPath(SHADER_TYPE type_shader, ID idShader, char *szPath)
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->getPath(type_shader, idShader, szPath);
}

SX_LIB_API void SGCore_ShaderGetName(SHADER_TYPE type_shader, ID idShader, char *szName)
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->getName(type_shader, idShader, szName);
}

SX_LIB_API bool SGCore_ShaderFileExists(const char *szName)
{
	SG_PRECOND(false);

	return g_pManagerShaders->existsFile(szName);
}

//##########################################################################

SX_LIB_API bool SGCore_LoadTexFileExists(const char *szName)
{
	SG_PRECOND(false);

	return g_pManagerTextures->fileExists(szName);
}

SX_LIB_API void SGCore_LoadTexClearLoaded()
{
	SG_PRECOND(_VOID);

	g_pManagerTextures->clearLoaded();
}

SX_LIB_API void SGCore_LoadTexDelete(ID id)
{
	SG_PRECOND(_VOID);

	g_pManagerTextures->deleteTexture(id);
}

SX_LIB_API ID SGCore_LoadTexAddName(const char *szName, LOAD_TEXTURE_TYPE type)
{
	SG_PRECOND(-1);

	return g_pManagerTextures->addName(szName, type);
}

SX_LIB_API ID SGCore_LoadTexAddConstAllInDir(const char *szDir)
{
	SG_PRECOND(false);

	return g_pManagerTextures->addConstAllInDir(szDir);
}

SX_LIB_API ID SGCore_LoadTexGetID(const char *szName)
{
	SG_PRECOND(-1);

	return g_pManagerTextures->getID(szName);
}

SX_LIB_API void SGCore_LoadTexGetName(ID id, char *szName)
{
	SG_PRECOND(_VOID);

	return g_pManagerTextures->getName(id, szName);
}

SX_LIB_API ID SGCore_LoadTexCreate(const char *szName, IDirect3DTexture9 *pTex)
{
	SG_PRECOND(-1);

	return g_pManagerTextures->create(szName, pTex);
}

SX_LIB_API ID SGCore_LoadTexUpdateN(const char *szName, LOAD_TEXTURE_TYPE type)
{
	SG_PRECOND(-1);

	return g_pManagerTextures->update(szName, type);
}

SX_LIB_API void SGCore_LoadTexUpdate(ID id)
{
	SG_PRECOND(_VOID);

	return g_pManagerTextures->update(id);
}

SX_LIB_API IDirect3DTexture9* SGCore_LoadTexGetTex(ID idTexture)
{
	SG_PRECOND(0);

	return g_pManagerTextures->getTexture2d(idTexture);
}

SX_LIB_API IDirect3DCubeTexture9* SGCore_LoadTexGetTexCube(ID idTexture)
{
	SG_PRECOND(0);

	return g_pManagerTextures->getTextureCube(idTexture);
}

SX_LIB_API void SGCore_LoadTexAllLoad()
{
	SG_PRECOND(_VOID);

	return g_pManagerTextures->loadTextures();
}

//##########################################################################

SX_LIB_API ID SGCore_RTAdd(UINT uiWidth, UINT uiHeight, UINT uiLevels, DWORD dwUsage, D3DFORMAT format, D3DPOOL pool, const char *szName, float fCoefFullScreen)
{
	SG_PRECOND(-1);

	return g_pManagerRenderTargets->Add(uiWidth, uiHeight, uiLevels, dwUsage, format, pool, szName, fCoefFullScreen);
}

SX_LIB_API void SGCore_RTDeleteN(const char *szName)
{
	SG_PRECOND(_VOID);

	return g_pManagerRenderTargets->Delete(szName);
}

SX_LIB_API void SGCore_RTDelete(ID id)
{
	SG_PRECOND(_VOID);

	return g_pManagerRenderTargets->Delete(id);
}

SX_LIB_API ID SGCore_RTGetId(const char *szName)
{
	SG_PRECOND(-1);

	return g_pManagerRenderTargets->GetNum(szName);
}

SX_LIB_API IDirect3DTexture9* SGCore_RTGetTextureN(const char *szName)
{
	SG_PRECOND(0);

	return g_pManagerRenderTargets->GetTexture(szName);
}

SX_LIB_API IDirect3DTexture9* SGCore_RTGetTexture(ID id)
{
	SG_PRECOND(0);

	return g_pManagerRenderTargets->GetTexture(id);
}

//##########################################################################

SX_LIB_API void SGCore_FCreateCone(float fTopRadius, float fBottomRadius, float fHeight, ID3DXMesh ** ppMesh, UINT iSideCount)
{
	SG_PRECOND(_VOID);

	CreateCone(fTopRadius, fBottomRadius, fHeight, ppMesh, g_pDXDevice, iSideCount);
}

SX_LIB_API void SGCore_FCompBoundBox(IDirect3DVertexBuffer9 *pVertexBuffer, ISXBound **ppBound, DWORD dwCountVertices, DWORD dwBytesPerVertex)
{
	SG_PRECOND(_VOID);

	ComputeBoundingBox(pVertexBuffer, ppBound, dwCountVertices, dwBytesPerVertex);
}

SX_LIB_API void SGCore_FCompBoundBox2(IDirect3DVertexBuffer9 *pVertexBuffer, ISXBound *pBound, DWORD dwCountVertices, DWORD dwBytesPerVertex)
{
	SG_PRECOND(_VOID);

	ComputeBoundingBox2(pVertexBuffer, pBound, dwCountVertices, dwBytesPerVertex);
}

SX_LIB_API void SGCore_FCreateBoundingBoxMesh(const float3 *pMin, const float3 *pMax, ID3DXMesh **ppBBmesh)
{
	SG_PRECOND(_VOID);

	CreateBoundingBoxMesh(pMin, pMax, ppBBmesh, g_pDXDevice);
}

SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint16(uint16_t *pIndecesBuffer, uint16_t numFaces, uint16_t numVerts)
{
	SG_PRECOND(_VOID);

	OptimizeIndecesInSubsetUint16(pIndecesBuffer, numFaces, numVerts);
}

SX_LIB_API void SGCore_OptimizeIndecesInSubsetUint32(uint32_t *pIndecesBuffer, uint32_t numFaces, uint32_t numVerts)
{
	SG_PRECOND(_VOID);

	OptimizeIndecesInSubsetUint32(pIndecesBuffer, numFaces, numVerts);
}

//##########################################################################

SX_LIB_API bool SGCore_0InPos2D(const float3* min, const float3* max, const float3* pos)
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

SX_LIB_API IFrustum* SGCore_CrFrustum()
{
	return new CFrustum();
}

SX_LIB_API ICamera* SGCore_CrCamera()
{
	return new CCamera();
}

SX_LIB_API ITransObject* SGCore_CrTransObject()
{
	return new CTransObject();
}

SX_LIB_API ISXBound* SGCore_CrBound()
{
	return new CBound();
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
	g_pSkyBox->loadTexture(texture);
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
	g_pSkyClouds->loadTexture(texture);
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
