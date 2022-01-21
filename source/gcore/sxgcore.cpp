
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sxgcore.h"

#include <gcore/shader.h>
#include <gcore/bound.h>
#include <gcore/camera.h>

#include <gcore/gcore_utils.h>


//##########################################################################

#define SXGCORE_VERSION 1


IGXDevice *g_pDevice = 0;
HMODULE m_hLibGXAPI = NULL;
Array<DEVMODE> g_aModes;
HWND g_hWnd = NULL;

IGXVertexDeclaration *g_pStaticVertexDecl = 0;

CShaderManager *g_pManagerShaders = 0;
IGXRenderBuffer *g_pScreenTextureRB = 0;

DECLARE_PROFILER_INTERNAL();

//##########################################################################

#define SG_PRECOND(retval) if(!g_pDevice){ LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxgcore is not init", GEN_MSG_LOCATION); return retval;}

//##########################################################################

void GCoreInit(SXWINDOW hWnd, int iWidth, int iHeight, bool isWindowed)
{
	InitDevice(hWnd, iWidth, iHeight, isWindowed);
	InitFullScreenQuad();

	g_pManagerShaders = new CShaderManager();

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

	Core_0RegisterConcmd("shader_reload", SGCore_ShaderReloadAll);
}

//##########################################################################

SX_LIB_API void SGCore_0Create(const char *szName, SXWINDOW hWnd, int iWidth, int iHeigth, bool isWindowed, bool isUnic)
{
	Core_SetOutPtr();
	INIT_PROFILER_INTERNAL();
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

	mem_release(g_pScreenTextureRB);

	//mem_release(g_pFPStext);

	mem_release(g_pDevice);
}

SX_LIB_API IGXDevice* SGCore_GetDXDevice()
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
}

SX_LIB_API void SGCore_ScreenQuadDraw()
{
	SG_PRECOND(_VOID);
	IGXContext *pCtx = g_pDevice->getThreadContext();
	pCtx->setRenderBuffer(g_pScreenTextureRB);
	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);
	pCtx->drawPrimitive(0, 2);
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
	XPROFILE_FUNCTION();

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

SX_LIB_API IXFrustum* SGCore_CrFrustum()
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
		IGXContext *pCtx = g_pDevice->getThreadContext();
		pCtx->setRenderBuffer(m_pRB);
		pCtx->setIndexBuffer(m_pIB);
		pCtx->drawIndexed(m_uVertexCount, m_uIndexCount / 3, 0, 0);
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
