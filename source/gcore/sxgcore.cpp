
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxgcore.h"

#include <gcore/shader.h>
#include <gcore/loadertextures.h>
#include <gcore/bound.h>
#include <gcore/camera.h>

#include <gcore/gcore_utils.h>
#include <gcore/gcore_data.h>


//##########################################################################

#define SXGCORE_VERSION 1


IGXContext *g_pDevice = 0;
HMODULE m_hLibGXAPI = NULL;
Array<DEVMODE> g_aModes;
HWND g_hWnd = NULL;

IGXVertexDeclaration *g_pStaticVertexDecl = 0;

CShaderManager *g_pManagerShaders = 0;
CLoaderTextures *g_pManagerTextures = 0;
IGXRenderBuffer *g_pScreenTextureRB = 0;
IGXBlendState *g_pToneMappingBS = NULL;
IGXSamplerState *g_pSamplerFilterPoint = NULL;
IGXSamplerState *g_pSamplerFilterLinear = NULL;



//##########################################################################

#define SG_PRECOND(retval) if(!g_pDevice){ LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxgcore is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

void GCoreInit(SXWINDOW hWnd, int iWidth, int iHeight, bool isWindowed)
{
	InitDevice(hWnd, iWidth, iHeight, isWindowed);
	InitFPStext();
	InitFullScreenQuad();

	g_pManagerShaders = new CShaderManager();
	g_pManagerTextures = new CLoaderTextures();
	InitToneMappingStates();

	GXVertexElement oLayoutStatic[] =
	{
		{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
		{0, 12, GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		{0, 20, GXDECLTYPE_FLOAT3, GXDECLUSAGE_NORMAL},
		{0, 32, GXDECLTYPE_FLOAT3, GXDECLUSAGE_TANGENT},
		{0, 44, GXDECLTYPE_FLOAT3, GXDECLUSAGE_BINORMAL},
		GX_DECL_END()
	};

	g_pStaticVertexDecl = g_pDevice->createVertexDeclaration(oLayoutStatic);

	InitArrModes();
	InitRT4Gbuffer();
	LoadShaders();

	Core_0RegisterConcmd("shader_reload", SGCore_ShaderReloadAll);
}

//##########################################################################

SX_LIB_API void SGCore_0Create(const char *szName, SXWINDOW hWnd, int iWidth, int iHeigth, bool isWindowed, bool isUnic)
{
	Core_SetOutPtr();
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
		g_hWnd = (HWND)hWnd;
		GCoreInit(hWnd, iWidth, iHeigth, isWindowed);
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

SX_LIB_API SXWINDOW SGCore_GetHWND()
{
	return(g_hWnd);
}

SX_LIB_API void SGCore_AKill()
{
	SG_PRECOND(_VOID);

	mem_delete(g_pManagerShaders);
	mem_delete(g_pManagerTextures);

	mem_release(g_pScreenTextureRB);

	mem_release(g_pSamplerFilterLinear);
	mem_release(g_pSamplerFilterPoint);

	//mem_release(g_pFPStext);

	mem_release(g_pDevice);
}

SX_LIB_API IGXContext* SGCore_GetDXDevice()
{
	SG_PRECOND(0);
	return g_pDevice;
}

SX_LIB_API void SGCore_OnResetDevice()
{
	SG_PRECOND(_VOID);

	//g_pFPStext->OnResetDevice();
//	g_pManagerRenderTargets->OnResetDevice();
//	g_pOC->onResetDevice(0/*g_oD3DAPP.BackBufferWidth*/, 9/*g_oD3DAPP.BackBufferHeight*/);
	InitFullScreenQuad();
	
	gcore_data::rt_id::iHowAdaptedLum = 0;
}

SX_LIB_API void SGCore_ScreenQuadDraw()
{
	SG_PRECOND(_VOID);
	g_pDevice->setRenderBuffer(g_pScreenTextureRB);
	g_pDevice->setPrimitiveTopology(GXPT_TRIANGLELIST);
	g_pDevice->drawPrimitive(0, 2);
}

//##########################################################################

SX_LIB_API ID SGCore_ShaderLoad(SHADER_TYPE type_shader, const char *szPath, const char *szName, GXMacro *pMacro)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->preLoad(type_shader, szPath, pMacro);
}

SX_LIB_API void SGCore_ShaderAllLoad()
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->allLoad();
}

SX_LIB_API void SGCore_ShaderReloadAll()
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->reloadAll();
}

SX_LIB_API ID SGCore_ShaderCreateKit(ID idVertexShader, ID idPixelShader, ID idGeometryShader, ID idComputeShader)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->createKit(idVertexShader, idPixelShader, idGeometryShader, idComputeShader);
}

SX_LIB_API void SGCore_ShaderBind(ID idShaderKit)
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->bind(idShaderKit);
}


SX_LIB_API void SGCore_ShaderUnBind()
{
	SG_PRECOND(_VOID);

	return g_pManagerShaders->unbind();
}

SX_LIB_API ID SGCore_ShaderExists(SHADER_TYPE type_shader, const char *szName, GXMacro *pMacro)
{
	SG_PRECOND(-1);

	return g_pManagerShaders->existsPathMacro(type_shader, szName, pMacro);
}

SX_LIB_API bool SGCore_ShaderIsLoaded(SHADER_TYPE type_shader, ID idShader)
{
	SG_PRECOND(0);

	return g_pManagerShaders->isLoaded(type_shader, idShader);
}

SX_LIB_API void SGCore_ShaderGetPath(SHADER_TYPE type_shader, ID idShader, char *szPath)
{
	SG_PRECOND(_VOID);

	g_pManagerShaders->getPath(type_shader, idShader, szPath);
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

SX_LIB_API ID SGCore_LoadTexCreate(const char *szName, IGXTexture2D *pTex)
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

SX_LIB_API IGXTexture2D* SGCore_LoadTexGetTex(ID idTexture)
{
	SG_PRECOND(0);

	return g_pManagerTextures->getTexture2d(idTexture);
}

SX_LIB_API void SGCore_LoadTexSetTex(ID idTexture, IGXTexture2D *pTexture)
{
	SG_PRECOND(_VOID);

	g_pManagerTextures->setTexture2d(idTexture, pTexture);
}

SX_LIB_API IGXTextureCube* SGCore_LoadTexGetTexCube(ID idTexture)
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

SX_LIB_API void SGCore_FCreateCone(float fTopRadius, float fBottomRadius, float fHeight, IMesh ** ppMesh, UINT iSideCount)
{
	SG_PRECOND(_VOID);

	CreateCone(fTopRadius, fBottomRadius, fHeight, ppMesh, g_pDevice, iSideCount);
}

SX_LIB_API void SGCore_FCreateSphere(float fRadius, UINT iSideCount, UINT iStackCount, IMesh ** ppMesh)
{
	SG_PRECOND(_VOID);

	CreateSphere(fRadius, iSideCount, iStackCount, ppMesh, g_pDevice);
}

SX_LIB_API void SGCore_FCreateBoundingBoxMesh(const float3 *pMin, const float3 *pMax, IMesh **ppBBmesh)
{
	SG_PRECOND(_VOID);

	CreateBoundingBoxMesh(pMin, pMax, ppBBmesh, g_pDevice);
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

//##########################################################################

struct CMesh: public IMesh
{
public:
	CMesh(UINT uVertexCount, UINT uIndexCount, float3_t *pVertices, USHORT *pIndices):
		m_uVertexCount(uVertexCount),
		m_uIndexCount(uIndexCount)
	{
		GXVertexElement oLayout[] =
		{
			{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
			GX_DECL_END()
		};

		IGXVertexDeclaration *pVD = g_pDevice->createVertexDeclaration(oLayout);

		m_pIB = g_pDevice->createIndexBuffer(sizeof(USHORT) * uIndexCount, GXBUFFER_USAGE_STATIC, GXIT_UINT16, pIndices);
		m_pVB = g_pDevice->createVertexBuffer(sizeof(float) * 3 * uVertexCount, GXBUFFER_USAGE_STATIC, pVertices);
		m_pRB = g_pDevice->createRenderBuffer(1, &m_pVB, pVD);

		mem_release(pVD);
	}
	~CMesh()
	{
		mem_release(m_pIB);
		mem_release(m_pVB);
		mem_release(m_pRB);
	}
	void draw()
	{
		g_pDevice->setRenderBuffer(m_pRB);
		g_pDevice->setIndexBuffer(m_pIB);
		g_pDevice->drawIndexed(m_uVertexCount, m_uIndexCount / 3, 0, 0);
	}
	IGXVertexBuffer *getVertexBuffer()
	{
		return(m_pVB);
	}
	IGXIndexBuffer *getIndexBuffer()
	{
		return(m_pIB);
	}
	void Release()
	{
		if(!--m_uRefCount)
		{
			delete this;
		}
	}
	void AddRef()
	{
		++m_uRefCount;
	}

protected:
	IGXVertexBuffer *m_pVB;
	IGXRenderBuffer *m_pRB;
	IGXIndexBuffer *m_pIB;
	UINT m_uVertexCount;
	UINT m_uIndexCount;
	UINT m_uRefCount = 1;
};

SX_LIB_API IMesh* SGCore_CrMesh(UINT uVertexCount, UINT uIndexCount, float3_t *pVertices, USHORT *pIndices)
{
	return(new CMesh(uVertexCount, uIndexCount, pVertices, pIndices));
}
