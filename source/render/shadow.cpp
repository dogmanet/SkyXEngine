#include "shadow.h"
#include <mtrl/IXMaterialSystem.h>

CShadowMap::CShadowMap()
{
}

CShadowMap::~CShadowMap()
{
	mem_release(m_pCameraShaderDataVS);
	mem_release(m_pShaderDataInjectVS);
	mem_release(m_pShaderDataPS);
	mem_release(m_pDepthMap);
	ReleaseDepthStencilSurface();
}

IGXDepthStencilSurface *CShadowMap::ms_pDepthStencilSurface = NULL;
UINT CShadowMap::ms_uDepthStencilSurfaceRefCount = 0;
IGXSamplerState *CShadowMap::ms_pSamplerPointWrap = NULL;
IGXSamplerState *CShadowMap::ms_pSamplerPointClamp = NULL;
IGXSamplerState *CShadowMap::ms_pSamplerLinearClamp = NULL;
IGXSamplerState *CShadowMap::ms_pSamplerComparisonLinearClamp = NULL;

IGXTexture2D *CShadowMap::ms_pRandomTexture = NULL;

void CShadowMap::InitDepthStencilSurface(IGXDevice *pContext, UINT uSize)
{
	if(!ms_pDepthStencilSurface)
	{
		ms_pDepthStencilSurface = pContext->createDepthStencilSurface(uSize, uSize, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);

		GXSamplerDesc samplerDesc;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointWrap = pContext->createSamplerState(&samplerDesc);

		samplerDesc.addressU = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressV = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointClamp = pContext->createSamplerState(&samplerDesc);

		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
		ms_pSamplerLinearClamp = pContext->createSamplerState(&samplerDesc);

		samplerDesc.comparisonFunc = GXCMP_LESS;
		samplerDesc.filter = GXFILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		ms_pSamplerComparisonLinearClamp = pContext->createSamplerState(&samplerDesc);
		

	//	ID idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs");
	//	GXMacro Defines_GSD_9[] = {{"GSD_9", ""}, {0, 0}};
	//	ID idGenShadowDirect9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct_9.ps", Defines_GSD_9);
	//	ms_idShader = SGCore_ShaderCreateKit(idResPosDepth, idGenShadowDirect9);

		uint32_t aRndColors[16];// = D3DCOLOR_ARGB(0, 250, 2, 255);

		for(int i = 0; i < 16; ++i)
			aRndColors[i] = GX_COLOR_ARGB(255, rand() % 255, rand() % 255, rand() % 255);

		ms_pRandomTexture = pContext->createTexture2D(4, 4, 1, 0, GXFMT_A8R8G8B8, aRndColors);

		static char tmp[32];
		sprintf(tmp, "%u", uSize);
	}
	++ms_uDepthStencilSurfaceRefCount;
}
void CShadowMap::ReleaseDepthStencilSurface()
{
	if(--ms_uDepthStencilSurfaceRefCount == 0)
	{
		mem_release(ms_pSamplerComparisonLinearClamp);
		mem_release(ms_pDepthStencilSurface);
		mem_release(ms_pSamplerPointWrap);
		mem_release(ms_pSamplerPointClamp);
		mem_release(ms_pSamplerLinearClamp);
		mem_release(ms_pRandomTexture);
	}
}

UINT CShadowMap::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow

	return(uSize * uSize * 4);
}

void CShadowMap::init(IGXDevice *pContext, UINT uSize)
{
	m_pDevice = pContext;

	//GXFMT_R32F
	m_pDepthMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);

	float fOffset = 0.5f + (0.5f / (float)uSize);
	float fRange = 1.0f;
	float fBias = 0.0f;
	m_mScaleBiasMat = float4x4(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, 0.0f,
		fOffset, fOffset, fBias, 1.0f);

	m_fSize = (float)uSize;

	InitDepthStencilSurface(pContext, uSize);

	m_pShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.ps));
	m_pShaderDataInjectVS = m_pDevice->createConstantBuffer(sizeof(SMMATRIX));
	m_pCameraShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_cameraShaderData.vs));
}

void CShadowMap::setLight(IXLight *pLight)
{
	m_pLight = pLight;
}

void CShadowMap::process(IXRenderPipeline *pRenderPipeline)
{
	assert(m_pLight && m_pLight->getType() == LIGHT_TYPE_SPOT);
	if(!m_pDevice)
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	IXLightSpot *pSpotLight = m_pLight->asSpot();

	//! @todo remove double with Light::updateFrustum()
	float3 vPos = pSpotLight->getPosition();
	float3 vDir = pSpotLight->getDirection() * LIGHTS_DIR_BASE;
	float3 vUp = pSpotLight->getDirection() * float3(0.0f, 0.0f, 1.0f);

	m_mView = SMMatrixLookAtLH(vPos, vPos + vDir, vUp);
	m_mProj = SMMatrixPerspectiveFovLH(pSpotLight->getOuterAngle(), 1.0f, 0.025f, pSpotLight->getMaxDistance());

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mProj);

	m_cameraShaderData.vs.mVP = SMMatrixTranspose(m_mView * m_mProj);
	m_cameraShaderData.vs.vPosCam = vPos;
	m_pCameraShaderDataVS->update(&m_cameraShaderData.vs);
	pCtx->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	pCtx->setDepthStencilSurface(ms_pDepthStencilSurface);

	pCtx->setBlendState(NULL);

	IGXSurface *pDepthSurface = NULL;
	
	pDepthSurface = m_pDepthMap->asRenderTarget();

	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, GX_COLOR_ARGB(255, 255, 255, 255));
	
	mem_release(pDepthSurface);
	
	//m_pLight->setPSConstants(m_pDevice, 5);

	pRenderPipeline->renderShadows();
	pRenderPipeline->renderStage(XRS_SHADOWS, m_pLight->getVisibility());

	pCtx->setColorTarget(NULL);

	/*if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_depth.dds", m_pDepthMap);
		m_pDevice->saveTextureToFile("sm_normal.dds", m_pNormalMap);
		m_pDevice->saveTextureToFile("sm_flux.dds", m_pFluxMap);
	}*/
}

void CShadowMap::genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals)
{
	if(!m_pDevice)
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	const float c_fTexWidth = (float)pShadowMap->getWidth();
	const float c_fTexHeight = (float)pShadowMap->getHeight();

//	IGXSurface *pRenderSurf, *pBackBuf;
//	pRenderSurf = pShadowMap->getMipmap(0);
//	pBackBuf = pCtx->getColorTarget();
//	pCtx->setColorTarget(pRenderSurf);
//	pCtx->setColorTarget(NULL, 1);
//
//	pCtx->clear(GX_CLEAR_COLOR);

	pCtx->setSamplerState(ms_pSamplerPointClamp, 0);
	//pCtx->setSamplerState(ms_pSamplerLinearClamp, 1);
	pCtx->setSamplerState(ms_pSamplerComparisonLinearClamp, 1);
	pCtx->setSamplerState(ms_pSamplerPointWrap, 2);
	
	pCtx->setPSTexture(pGBufferDepth);
	pCtx->setPSTexture(m_pDepthMap, 4);
	pCtx->setPSTexture(ms_pRandomTexture, 2);
	pCtx->setPSTexture(pGBufferNormals, 3);

	m_shaderData.ps.mMatrixTexture = SMMatrixTranspose(m_mView * m_mProj * m_mScaleBiasMat);
	m_shaderData.ps.vPixelMapSizeBias = float3(m_fBlurPixel / m_fSize, m_fSize, m_fBias);
	m_pShaderDataPS->update(&m_shaderData.ps);
	pCtx->setPSConstant(m_pShaderDataPS, 6);

	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	pCtx->setPSConstant(pLightConstants);
	mem_release(pLightConstants);
	/*
	vs:
		cbuffer(1)
		{
			- g_vParamProj // per camera
			- g_mViewInv   // per camera
			- g_vNearFar   // per frame
		}


	ps:
		cbuffer(6)
		{
			- g_mMatrixTexture // per shadow
			- g_vPixelSize     // per shadow
			- g_vSizeMapBias   // per shadow
		}

		cbuffer(1)
		{
			- g_vPosCam        // per camera
		}
	*/


	//m_pDevice->setVertexShaderConstant(m_pLightingShaderDataVS, 1);

//	SGCore_ShaderBind(ms_idShader);


		

//	SGCore_ScreenQuadDraw();

//	SGCore_ShaderUnBind();
//
//	pCtx->setColorTarget(pBackBuf);
//
//	mem_release(pRenderSurf);
//	mem_release(pBackBuf);

	/*if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_shadow.dds", pShadowMap);
	}*/
}

//##########################################################################


CReflectiveShadowMap::CReflectiveShadowMap()
{}

CReflectiveShadowMap::~CReflectiveShadowMap()
{
	mem_release(m_pCameraShaderDataVS);
	mem_release(m_pShaderDataInjectVS);
	mem_release(m_pShaderDataPS);
	mem_release(m_pDepthMap32);
	mem_release(m_pNormalMap32);
	mem_release(m_pFluxMap32);
	ReleaseDepthStencilSurface();
}

IGXDepthStencilSurface *CReflectiveShadowMap::ms_pDepthStencilSurface32 = NULL;
UINT CReflectiveShadowMap::ms_uDepthStencilSurfaceRefCount = 0;
IGXSamplerState *CReflectiveShadowMap::ms_pSamplerPointWrap = NULL;
IGXSamplerState *CReflectiveShadowMap::ms_pSamplerPointClamp = NULL;
IGXSamplerState *CReflectiveShadowMap::ms_pSamplerLinearClamp = NULL;
ID CReflectiveShadowMap::ms_idInjectShader32 = -1;
ID CReflectiveShadowMap::ms_idInjectDebugShader32 = -1;

void CReflectiveShadowMap::InitDepthStencilSurface(IGXDevice *pContext, UINT uSize)
{
	if(!ms_pDepthStencilSurface32)
	{
		ms_pDepthStencilSurface32 = pContext->createDepthStencilSurface(32, 32, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);

		GXSamplerDesc samplerDesc;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointWrap = pContext->createSamplerState(&samplerDesc);

		samplerDesc.addressU = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressV = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointClamp = pContext->createSamplerState(&samplerDesc);

		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
		ms_pSamplerLinearClamp = pContext->createSamplerState(&samplerDesc);

		static char tmp[32];
		sprintf(tmp, "%u", uSize);

		GXMacro definesInject32[] = {{"LPV_POINT_COUNT", "32"}, {"LPV_MAP_SIZE", "32"}, {0, 0}};
		ms_idInjectShader32 = SGCore_ShaderCreateKit(
			SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject.vs", 0, definesInject32),
			SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInject32),
			SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, "gi_inject.gs", 0, definesInject32)
			);

		GXMacro definesInjectDebug32[] = {{"_DEBUG", "1"}, {"LPV_POINT_COUNT", "32"}, {"LPV_MAP_SIZE", "32"}, {0, 0}};
		ms_idInjectDebugShader32 = SGCore_ShaderCreateKit(
			SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject.vs", 0, definesInjectDebug32),
			SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInjectDebug32)
			);
	}
	++ms_uDepthStencilSurfaceRefCount;
}
void CReflectiveShadowMap::ReleaseDepthStencilSurface()
{
	if(--ms_uDepthStencilSurfaceRefCount == 0)
	{
		mem_release(ms_pDepthStencilSurface32);
		mem_release(ms_pSamplerPointWrap);
		mem_release(ms_pSamplerPointClamp);
		mem_release(ms_pSamplerLinearClamp);
	}
}

UINT CReflectiveShadowMap::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow
	//GXFMT_A8R8G8B8 - xyz:color; w:???
	//GXFMT_A8R8G8B8 - xyz:normals; w:???

	return(32 * 32 * 12);
}

void CReflectiveShadowMap::init(IGXDevice *pContext, UINT uSize)
{
	m_pDevice = pContext;

	//GXFMT_A8R8G8B8
	m_pNormalMap32 = m_pDevice->createTexture2D(32, 32, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_A8R8G8B8
	m_pFluxMap32 = m_pDevice->createTexture2D(32, 32, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_R32F
	m_pDepthMap32 = m_pDevice->createTexture2D(32, 32, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);

	float fOffset = 0.5f + (0.5f / (float)uSize);
	float fRange = 1.0f;
	float fBias = 0.0f;
	m_mScaleBiasMat = float4x4(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, 0.0f,
		fOffset, fOffset, fBias, 1.0f);

	m_fSize = (float)uSize;

	InitDepthStencilSurface(pContext, uSize);

	m_pShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.ps));
	m_pShaderDataInjectVS = m_pDevice->createConstantBuffer(sizeof(SMMATRIX));
	m_pCameraShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_cameraShaderData.vs));
}

void CReflectiveShadowMap::setLight(IXLight *pLight)
{
	m_pLight = pLight;
}

void CReflectiveShadowMap::process(IXRenderPipeline *pRenderPipeline)
{
	assert(m_pLight && m_pLight->getType() == LIGHT_TYPE_SPOT);
	if(!m_pDevice)
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	IXLightSpot *pSpotLight = m_pLight->asSpot();

	//! @todo remove double with Light::updateFrustum()
	float3 vPos = pSpotLight->getPosition();
	float3 vDir = pSpotLight->getDirection() * LIGHTS_DIR_BASE;
	float3 vUp = pSpotLight->getDirection() * float3(0.0f, 0.0f, 1.0f);

	m_mView = SMMatrixLookAtLH(vPos, vPos + vDir, vUp);
	m_mProj = SMMatrixPerspectiveFovLH(pSpotLight->getOuterAngle(), 1.0f, 0.025f, pSpotLight->getMaxDistance());

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mProj);

	m_cameraShaderData.vs.mVP = SMMatrixTranspose(m_mView * m_mProj);
	m_cameraShaderData.vs.vPosCam = vPos;
	m_pCameraShaderDataVS->update(&m_cameraShaderData.vs);
	pCtx->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	pCtx->setDepthStencilSurface(ms_pDepthStencilSurface32);

	pCtx->setBlendState(NULL);

	IGXSurface *pDepthSurface = m_pDepthMap32->asRenderTarget();
	IGXSurface *pNormalSurface = m_pNormalMap32->asRenderTarget();
	IGXSurface *pFluxSurface = m_pFluxMap32->asRenderTarget();

	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, GX_COLOR_ARGB(255, 255, 255, 255));

	pCtx->setColorTarget(pFluxSurface, 0);
	pCtx->setColorTarget(pNormalSurface, 1);
	pCtx->clear(GX_CLEAR_COLOR);
	
	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(pNormalSurface, 1);
	pCtx->setColorTarget(pFluxSurface, 2);

	mem_release(pDepthSurface);
	mem_release(pNormalSurface);
	mem_release(pFluxSurface);

	//m_pLight->setPSConstants(m_pDevice, 5);

	pRenderPipeline->renderShadows();
	pRenderPipeline->renderStage(XRS_SHADOWS, m_pLight->getVisibility());

	pCtx->setColorTarget(NULL);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);

	/*if(GetAsyncKeyState('U') < 0)
	{
	m_pDevice->saveTextureToFile("sm_depth.dds", m_pDepthMap);
	m_pDevice->saveTextureToFile("sm_normal.dds", m_pNormalMap);
	m_pDevice->saveTextureToFile("sm_flux.dds", m_pFluxMap);
	}*/
}

void CReflectiveShadowMap::genLPV(bool isDebug)
{
	if(!m_pDevice || !(m_pLight->getRenderType() & LRT_LPV))
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	pCtx->setRenderBuffer(NULL);
	pCtx->setIndexBuffer(NULL);
	pCtx->setPrimitiveTopology(GXPT_POINTLIST);
	//SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader : ms_idInjectShader);

	pCtx->setVSTexture(m_pDepthMap32);
	pCtx->setVSTexture(m_pNormalMap32, 1);
	pCtx->setVSTexture(m_pFluxMap32, 2);

	m_pShaderDataInjectVS->update(&SMMatrixTranspose(SMMatrixInverse(NULL, m_mView * m_mProj)));
	pCtx->setVSConstant(m_pShaderDataInjectVS, 6);

	IGXConstantBuffer *pLightConstant = m_pLight->getConstants(m_pDevice);
	pCtx->setVSConstant(pLightConstant, 7);
	mem_release(pLightConstant);

	SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader32 : ms_idInjectShader32);
	pCtx->drawPrimitive(0, 32 * 32);

	SGCore_ShaderUnBind();
	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);

	pCtx->setVSTexture(NULL);
	pCtx->setVSTexture(NULL, 1);
	pCtx->setVSTexture(NULL, 2);
}

//##########################################################################


CShadowPSSM::CShadowPSSM()
{
}

CShadowPSSM::~CShadowPSSM()
{
	mem_release(m_pSamplerComparisonLinearClamp);
	mem_release(m_pDepthStencilSurface);
	mem_release(m_pSamplerPointWrap);
	mem_release(m_pSamplerPointClamp);
	mem_release(m_pSamplerLinearClamp);

	mem_release(m_pDepthMap);

	mem_release(m_pCameraShaderDataVS);
	//mem_release(m_pShaderDataInjectVS);
	mem_release(m_pShaderDataPS);
	mem_release(m_pShaderDataGS);
}

UINT CShadowPSSM::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow
	return(uSize * uSize * 4 * PSSM_MAX_SPLITS);
}

void CShadowPSSM::setObserverCamera(ICamera *pCamera)
{
	m_pCamera = pCamera;
}

void CShadowPSSM::init(IGXDevice *pContext, UINT uSize)
{
	m_pDevice = pContext;

	m_pDepthMap = m_pDevice->createTextureCube(uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);
	m_pDepthStencilSurface = pContext->createDepthStencilSurfaceCube(uSize, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);

	float fOffset = 0.5f + (0.5f / (float)uSize);
	float fRange = 1.0f;
	float fBias = 0.0f;
	m_mScaleBiasMat = float4x4(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, 0.0f,
		fOffset, fOffset, fBias, 1.0f);

	m_fSize = (float)uSize;

	GXSamplerDesc samplerDesc;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
	m_pSamplerPointWrap = pContext->createSamplerState(&samplerDesc);

	samplerDesc.addressU = GXTEXTURE_ADDRESS_CLAMP;
	samplerDesc.addressV = GXTEXTURE_ADDRESS_CLAMP;
	samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
	m_pSamplerPointClamp = pContext->createSamplerState(&samplerDesc);

	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
	m_pSamplerLinearClamp = pContext->createSamplerState(&samplerDesc);

	samplerDesc.comparisonFunc = GXCMP_LESS;
	samplerDesc.filter = GXFILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	m_pSamplerComparisonLinearClamp = pContext->createSamplerState(&samplerDesc);

	
	m_pShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.ps));
	m_pShaderDataGS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.gs));
	m_pCameraShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_cameraShaderData.vs));
}

void CShadowPSSM::setLight(IXLight *pLight)
{
	m_pLight = pLight;
}

void CShadowPSSM::process(IXRenderPipeline *pRenderPipeline)
{
	assert(m_pLight && m_pLight->getType() == LIGHT_TYPE_SUN);
	if(!m_pDevice)
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	IXLightSun *pSunLight = m_pLight->asSun();


	//! @todo remove double with Light::updateFrustum()
	float3 vPos = pSunLight->getPosition();
	float3 vDir = pSunLight->getDirection() * LIGHTS_DIR_BASE;
	float3 vUp = pSunLight->getDirection() * float3(0.0f, 0.0f, 1.0f);

	updateFrustums();

	pCtx->setDepthStencilSurface(m_pDepthStencilSurface);

	pCtx->setBlendState(NULL);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &SMMatrixIdentity());

	m_cameraShaderData.vs.vPosCam = vPos;
	m_pCameraShaderDataVS->update(&m_cameraShaderData.vs);
	pCtx->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	IGXSurface *pDepthSurface = m_pDepthMap->asRenderTarget();

	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, GX_COLOR_ARGB(255, 255, 255, 255));

	mem_release(pDepthSurface);

	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	pCtx->setGSConstant(m_pShaderDataGS, SCR_CAMERA);
	pCtx->setGSConstant(pLightConstants, SCR_OBJECT);
	mem_release(pLightConstants);

	pRenderPipeline->renderShadows();
	pRenderPipeline->renderStage(XRS_SHADOWS, NULL); // m_pLight->getVisibility());

	pCtx->setColorTarget(NULL);

	if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("pssm_depth.dds", m_pDepthMap);
	}
}

#define PSSM_LIGHT_NEAR 0.1f
#define PSSM_LIGHT_FAR 5000.0f

void CShadowPSSM::updateFrustums()
{
	assert(m_pCamera);

	IXLightSun *pSunLight = m_pLight->asSun();
	float3 vLightDir = pSunLight->getDirection() * LIGHTS_DIR_BASE;
	float3 vUpDir = pSunLight->getDirection() * float3(1.0f, 0.0f, 0.0f);

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");
	static const float *r_pssm_max_distance = GET_PCVAR_FLOAT("r_pssm_max_distance");
	
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");
	static const float *r_effective_fov = GET_PCVAR_FLOAT("r_default_fov");
	static const int *r_pssm_splits = GET_PCVAR_INT("r_pssm_splits");
	if(*r_pssm_splits < 1)
	{
		Core_0SetCVarInt("r_pssm_splits", 1);
	}
	else if(*r_pssm_splits > PSSM_MAX_SPLITS)
	{
		Core_0SetCVarInt("r_pssm_splits", PSSM_MAX_SPLITS);
	}

	float fSplitWeight = 0.8f;
	float fShadowDistance = min(*r_pssm_max_distance, *r_far);

	float fMaxDistanceSun = pSunLight->getMaxDistance();
	if(fShadowDistance > fMaxDistanceSun)
	{
		fShadowDistance = fMaxDistanceSun;
	}

	float aSplitDistances[PSSM_MAX_SPLITS];
	for(int i = 0; i < *r_pssm_splits; ++i)
	{
		float f = (i + 1.0f) / *r_pssm_splits;
		float fLogDistance = *r_near * pow(fShadowDistance / *r_near, f);
		float fUniformDistance = *r_near + (fShadowDistance - *r_near) * f;
		aSplitDistances[i] = lerpf(fUniformDistance, fLogDistance, fSplitWeight);

		if(i == 0)
		{
			m_splits[i].vNearFar = float2(*r_near, aSplitDistances[i]);
		}
		else
		{
			m_splits[i].vNearFar = float2(aSplitDistances[i - 1], aSplitDistances[i]);
		}
	}


	float3 vStart;
	m_pCamera->getPosition(&vStart);
	float3 vDir;
	m_pCamera->getLook(&vDir);
	vDir = SMVector3Normalize(vDir);
	float3 vRight;
	m_pCamera->getRight(&vRight);
	float3 vUp;
	m_pCamera->getUp(&vUp);
	float fAspectRatio = (float)*r_win_width / (float)*r_win_height;
	float fFovTan = tanf(*r_effective_fov * 0.5f);
	for(int i = 0; i < *r_pssm_splits; ++i)
	{
		Split &split = m_splits[i];

		float3 vNearCenter = vStart + vDir * split.vNearFar.x;
		float3 vFarCenter = vStart + vDir * split.vNearFar.y;

		float fNearHalfHeight = fFovTan * split.vNearFar.x;
		float fFarHalfHeight = fFovTan * split.vNearFar.y;

		float fNearHalfWidth = fNearHalfHeight * fAspectRatio;
		float fFarHalfWidth = fFarHalfHeight * fAspectRatio;

		float3 vA = vNearCenter - vUp * fNearHalfHeight + vRight * fNearHalfWidth;
		float3 vB = vFarCenter + vUp * fFarHalfHeight - vRight * fFarHalfWidth;
		float3 vC = vFarCenter - vUp * fFarHalfHeight + vRight * fFarHalfWidth;

		float3 vCenter = SMTriangleCircumcenter3(vA, vB, vC);
 

		float fRadius = SMVector3Length(vCenter - vA);
		//float fRadius1 = SMVector3Length(vCenter - vB);
		//float fRadius2 = SMVector3Length(vCenter - vC);
		
		// vCenter.mmv = _mm_round_ps(vCenter, _MM_ROUND_NEAREST);

		SMMATRIX mLight(SMMatrixTranspose(SMMATRIX(
			float4(SMVector3Cross(vUpDir, vLightDir)),
			float4(vUpDir),
			float4(vLightDir),
			float4(0.0f, 0.0f, 0.0f, 1.0f)
			)));
		SMMATRIX mLightInv = SMMatrixInverse(NULL, mLight);

		vCenter = SMVector3Transform(vCenter, mLight);
		float fStep = (fRadius * 2.0f / m_fSize);
		vCenter.x -= fmodf(vCenter.x, fStep);
		vCenter.y -= fmodf(vCenter.y, fStep);

		vCenter = SMVector3Transform(vCenter, mLightInv);

		float fMaxDistance = PSSM_LIGHT_FAR;

		split.mProj = SMMatrixOrthographicLH(fRadius * 2.0f, fRadius * 2.0f, PSSM_LIGHT_NEAR, fMaxDistance);
		split.mView = SMMatrixLookToLH(vCenter - vLightDir * (fMaxDistance /*- fRadius * 2*/ * 0.5f), vLightDir, vUpDir);

		m_shaderData.gs.mVP[i] = SMMatrixTranspose(split.mView * split.mProj);
	}
	m_pShaderDataGS->update(&m_shaderData.gs);
}

void CShadowPSSM::genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals)
{
	if(!m_pDevice)
	{
		return;
	}

	// static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");
	// static const float *r_near = GET_PCVAR_FLOAT("r_near");
	// static const float *r_far = GET_PCVAR_FLOAT("r_far");

	IGXContext *pCtx = m_pDevice->getThreadContext();

	const float c_fTexWidth = (float)pShadowMap->getWidth();
	const float c_fTexHeight = (float)pShadowMap->getHeight();

	//	IGXSurface *pRenderSurf, *pBackBuf;
	//	pRenderSurf = pShadowMap->getMipmap(0);
	//	pBackBuf = pCtx->getColorTarget();
	//	pCtx->setColorTarget(pRenderSurf);
	//	pCtx->setColorTarget(NULL, 1);
	//
	//	pCtx->clear(GX_CLEAR_COLOR);

	pCtx->setSamplerState(m_pSamplerPointClamp, 0);
	//m_pDevice->setSamplerState(ms_pSamplerLinearClamp, 1);
	pCtx->setSamplerState(m_pSamplerComparisonLinearClamp, 1);
	pCtx->setSamplerState(m_pSamplerPointWrap, 2);

	pCtx->setPSTexture(pGBufferDepth);
	pCtx->setPSTexture(m_pDepthMap, 4);
	//pCtx->setPSTexture(ms_pRandomTexture, 2);
	pCtx->setPSTexture(pGBufferNormals, 3);

	SMMATRIX tmp = SMMatrixTranspose(m_mScaleBiasMat);
	for(int i = 0; i < PSSM_MAX_SPLITS; ++i)
	{
		//m_shaderData.ps.mMatrixTextureV[i] = tmp * m_shaderData.gs.mVP[i];
		m_shaderData.ps.mMatrixTextureV[i] = SMMatrixTranspose(m_splits[i].mView);
		m_shaderData.ps.mMatrixTextureP[i] = SMMatrixTranspose(m_splits[i].mProj * m_mScaleBiasMat);
	}
	//m_shaderData.ps.vPixelMapSizeBias = float3(m_fBlurPixel / m_fSize, m_fSize, m_fBias);
	m_shaderData.ps.vSizeBoundNearFar = float4(m_fSize, PSSM_LIGHT_NEAR * tanf(0.001f), PSSM_LIGHT_NEAR, PSSM_LIGHT_FAR);
	m_pShaderDataPS->update(&m_shaderData.ps);
	pCtx->setPSConstant(m_pShaderDataPS, 6);

	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	pCtx->setPSConstant(pLightConstants);
	mem_release(pLightConstants);
}

//##########################################################################

CReflectiveShadowSun::CReflectiveShadowSun()
{
}

CReflectiveShadowSun::~CReflectiveShadowSun()
{
	mem_release(m_pSamplerComparisonLinearClamp);
	mem_release(m_pDepthStencilSurface);
	mem_release(m_pSamplerPointWrap);
	mem_release(m_pSamplerPointClamp);
	mem_release(m_pSamplerLinearClamp);

	mem_release(m_pCameraShaderDataVS);
	mem_release(m_pShaderDataInjectVS);
	mem_release(m_pShaderDataPS);

	mem_release(m_pDepthMap);
	mem_release(m_pNormalMap);
	mem_release(m_pFluxMap);
}

UINT CReflectiveShadowSun::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow
	//GXFMT_A8R8G8B8 - xyz:color; w:???
	//GXFMT_A8R8G8B8 - xyz:normals; w:???

	return(RSM_SUN_SIZE * RSM_SUN_SIZE * 12 * PSSM_MAX_SPLITS);
}

void CReflectiveShadowSun::setObserverCamera(ICamera *pCamera)
{
	m_pCamera = pCamera;
}

void CReflectiveShadowSun::init(IGXDevice *pContext, UINT uSize)
{
	m_pDevice = pContext;

	//GXFMT_A8R8G8B8
	m_pNormalMap = m_pDevice->createTexture2D(RSM_SUN_SIZE, RSM_SUN_SIZE, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_A8R8G8B8
	m_pFluxMap = m_pDevice->createTexture2D(RSM_SUN_SIZE, RSM_SUN_SIZE, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_R32F
	m_pDepthMap = m_pDevice->createTexture2D(RSM_SUN_SIZE, RSM_SUN_SIZE, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);

	float fOffset = 0.5f + (0.5f / (float)RSM_SUN_SIZE);
	float fRange = 1.0f;
	float fBias = 0.0f;
	m_mScaleBiasMat = float4x4(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, 0.0f,
		fOffset, fOffset, fBias, 1.0f);

	m_fSize = (float)RSM_SUN_SIZE;

	m_pDepthStencilSurface = pContext->createDepthStencilSurface(RSM_SUN_SIZE, RSM_SUN_SIZE, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);

	GXSamplerDesc samplerDesc;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
	m_pSamplerPointWrap = pContext->createSamplerState(&samplerDesc);

	samplerDesc.addressU = GXTEXTURE_ADDRESS_CLAMP;
	samplerDesc.addressV = GXTEXTURE_ADDRESS_CLAMP;
	samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
	m_pSamplerPointClamp = pContext->createSamplerState(&samplerDesc);

	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
	m_pSamplerLinearClamp = pContext->createSamplerState(&samplerDesc);

	samplerDesc.comparisonFunc = GXCMP_LESS;
	samplerDesc.filter = GXFILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	m_pSamplerComparisonLinearClamp = pContext->createSamplerState(&samplerDesc);

	GXMacro definesInject[] = {{"IS_SUN", "1"}, {"LPV_POINT_COUNT", MACRO_TEXT(RSM_SUN_POINTS)}, {"LPV_MAP_SIZE", MACRO_TEXT(RSM_SUN_SIZE)}, {0, 0}};
	m_idInjectShader = SGCore_ShaderCreateKit(
		SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject.vs", 0, definesInject),
		SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInject),
		SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, "gi_inject.gs", 0, definesInject)
		);

	GXMacro definesInjectDebug[] = {{"IS_SUN", "1"}, {"_DEBUG", "1"}, {"LPV_POINT_COUNT", MACRO_TEXT(RSM_SUN_POINTS)}, {"LPV_MAP_SIZE", MACRO_TEXT(RSM_SUN_SIZE)}, {0, 0}};
	m_idInjectDebugShader = SGCore_ShaderCreateKit(
		SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject.vs", 0, definesInjectDebug),
		SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInjectDebug)
		);

	m_pShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.ps));
	m_pShaderDataInjectVS = m_pDevice->createConstantBuffer(sizeof(SMMATRIX));
	m_pCameraShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_cameraShaderData.vs));
}

void CReflectiveShadowSun::setLight(IXLight *pLight)
{
	m_pLight = pLight;
}

void CReflectiveShadowSun::process(IXRenderPipeline *pRenderPipeline)
{
	assert(m_pLight && m_pLight->getType() == LIGHT_TYPE_SUN);
	if(!m_pDevice)
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	IXLightSun *pSunLight = m_pLight->asSun();


	//! @todo remove double with Light::updateFrustum()
	float3 vPos = pSunLight->getPosition();
	float3 vDir = pSunLight->getDirection() * LIGHTS_DIR_BASE;
	float3 vUp = pSunLight->getDirection() * float3(0.0f, 0.0f, 1.0f);


	updateFrustum();

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mProj);

	m_cameraShaderData.vs.mVP = SMMatrixTranspose(m_mView * m_mProj);
	m_cameraShaderData.vs.vPosCam = vPos;
	m_pCameraShaderDataVS->update(&m_cameraShaderData.vs);
	pCtx->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	pCtx->setDepthStencilSurface(m_pDepthStencilSurface);

	pCtx->setBlendState(NULL);

	IGXSurface *pDepthSurface = m_pDepthMap->asRenderTarget();
	IGXSurface *pNormalSurface = m_pNormalMap->asRenderTarget();
	IGXSurface *pFluxSurface = m_pFluxMap->asRenderTarget();

	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, GX_COLOR_ARGB(255, 255, 255, 255));

	pCtx->setColorTarget(pFluxSurface, 0);
	pCtx->setColorTarget(pNormalSurface, 1);
	pCtx->clear(GX_CLEAR_COLOR);

	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(pNormalSurface, 1);
	pCtx->setColorTarget(pFluxSurface, 2);

	mem_release(pDepthSurface);
	mem_release(pNormalSurface);
	mem_release(pFluxSurface);

	//m_pLight->setPSConstants(m_pDevice, 5);
	pRenderPipeline->renderShadows();
	pRenderPipeline->renderStage(XRS_SHADOWS, NULL); // m_pLight->getVisibility());

	pCtx->setColorTarget(NULL);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);

	/*if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_depth.dds", m_pDepthMap);
		m_pDevice->saveTextureToFile("sm_normal.dds", m_pNormalMap);
		m_pDevice->saveTextureToFile("sm_flux.dds", m_pFluxMap);
	}*/
}

void CReflectiveShadowSun::updateFrustum()
{
	assert(m_pCamera);

	IXLightSun *pSunLight = m_pLight->asSun();
	float3 vLightDir = pSunLight->getDirection() * LIGHTS_DIR_BASE;
	float3 vUpDir = pSunLight->getDirection() * float3(1.0f, 0.0f, 0.0f);

	float3 vStart;
	m_pCamera->getPosition(&vStart);
	float3 vDir;
	m_pCamera->getLook(&vDir);
	vDir = SMVector3Normalize(vDir);

	//! @todo: fix grid center pos!
	float3 vGridCenter = vStart;
	//float fGridRadius = sqrtf(16.0f * 16.0f * 3.0f);
	float fGridRadius = sqrtf(64.0f * 64.0f * 3.0f);

	SMMATRIX mLight(SMMatrixTranspose(SMMATRIX(
		float4(SMVector3Cross(vUpDir, vLightDir)),
		float4(vUpDir),
		float4(vLightDir),
		float4(0.0f, 0.0f, 0.0f, 1.0f)
		)));
	SMMATRIX mLightInv = SMMatrixInverse(NULL, mLight);

	vGridCenter = SMVector3Transform(vGridCenter, mLight);
	float fStep = (fGridRadius * 2.0f / m_fSize);
	vGridCenter.x -= fmodf(vGridCenter.x, fStep);
	vGridCenter.y -= fmodf(vGridCenter.y, fStep);

	vGridCenter = SMVector3Transform(vGridCenter, mLightInv);

	float fMaxDistance = PSSM_LIGHT_FAR;

	m_mProj = SMMatrixOrthographicLH(fGridRadius * 2.0f, fGridRadius * 2.0f, PSSM_LIGHT_NEAR, fMaxDistance);
	m_mView = SMMatrixLookToLH(vGridCenter - vLightDir * (fMaxDistance /*- fRadius * 2*/ * 0.5f), vLightDir, vUpDir);
}


void CReflectiveShadowSun::genLPV(bool isDebug)
{
	if(!m_pDevice || !(m_pLight->getRenderType() & LRT_LPV))
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	pCtx->setRenderBuffer(NULL);
	pCtx->setIndexBuffer(NULL);
	pCtx->setPrimitiveTopology(GXPT_POINTLIST);
	//SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader : ms_idInjectShader);

	pCtx->setVSTexture(m_pDepthMap);
	pCtx->setVSTexture(m_pNormalMap, 1);
	pCtx->setVSTexture(m_pFluxMap, 2);

	m_pShaderDataInjectVS->update(&SMMatrixTranspose(SMMatrixInverse(NULL, m_mView * m_mProj)));
	pCtx->setVSConstant(m_pShaderDataInjectVS, 6);

	IGXConstantBuffer *pLightConstant = m_pLight->getConstants(m_pDevice);
	pCtx->setVSConstant(pLightConstant, 7);
	mem_release(pLightConstant);

	SGCore_ShaderBind(isDebug ? m_idInjectDebugShader : m_idInjectShader);
	pCtx->drawPrimitive(0, RSM_SUN_POINTS * RSM_SUN_POINTS);

	SGCore_ShaderUnBind();
	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);

	pCtx->setVSTexture(NULL);
	pCtx->setVSTexture(NULL, 1);
	pCtx->setVSTexture(NULL, 2);
}

//##########################################################################

CShadowCubeMap::CShadowCubeMap()
{
}

CShadowCubeMap::~CShadowCubeMap()
{
	mem_release(m_pCameraShaderDataVS);
	mem_release(m_pShaderDataPS);
	mem_release(m_pShaderDataGS);
	mem_release(m_pDepthMap);
	ReleaseDepthStencilSurface();
}

IGXDepthStencilSurface *CShadowCubeMap::ms_pDepthStencilSurface = NULL;
UINT CShadowCubeMap::ms_uDepthStencilSurfaceRefCount = 0;
IGXSamplerState *CShadowCubeMap::ms_pSamplerPointWrap = NULL;
IGXSamplerState *CShadowCubeMap::ms_pSamplerPointClamp = NULL;
IGXSamplerState *CShadowCubeMap::ms_pSamplerLinearClamp = NULL;
IGXSamplerState *CShadowCubeMap::ms_pSamplerComparisonLinearClamp = NULL;

void CShadowCubeMap::InitDepthStencilSurface(IGXDevice *pContext, UINT uSize)
{
	if(!ms_pDepthStencilSurface)
	{
		ms_pDepthStencilSurface = pContext->createDepthStencilSurfaceCube(uSize, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);

		GXSamplerDesc samplerDesc;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointWrap = pContext->createSamplerState(&samplerDesc);

		samplerDesc.addressU = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressV = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointClamp = pContext->createSamplerState(&samplerDesc);

		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
		ms_pSamplerLinearClamp = pContext->createSamplerState(&samplerDesc);

		samplerDesc.comparisonFunc = GXCMP_LESS;
		samplerDesc.filter = GXFILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		ms_pSamplerComparisonLinearClamp = pContext->createSamplerState(&samplerDesc);

	//	ID idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs");
	//	GXMacro Defines_GSC_6[] = {{"GSC_6", ""}, {0, 0}};
	//	ID idGenShadowPoint6 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point_6.ps", Defines_GSC_6);
	//	ms_idShader = SGCore_ShaderCreateKit(idResPosDepth, idGenShadowPoint6);
	}
	++ms_uDepthStencilSurfaceRefCount;
}
void CShadowCubeMap::ReleaseDepthStencilSurface()
{
	if(--ms_uDepthStencilSurfaceRefCount == 0)
	{
		mem_release(ms_pSamplerComparisonLinearClamp);
		mem_release(ms_pDepthStencilSurface);
		mem_release(ms_pSamplerPointWrap);
		mem_release(ms_pSamplerPointClamp);
		mem_release(ms_pSamplerLinearClamp);
	}
}

UINT CShadowCubeMap::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow

	return(uSize * uSize * 4 * 6);
}

void CShadowCubeMap::init(IGXDevice *pDevice, UINT uSize)
{
	m_pDevice = pDevice;

	//GXFMT_R32F
	m_pDepthMap = m_pDevice->createTextureCube(uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);

	float fOffset = 0.5f + (0.5f / (float)uSize);
	float fRange = 1.0f;
	float fBias = 0.0f;
	m_mScaleBiasMat = float4x4(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, 0.0f,
		fOffset, fOffset, fBias, 1.0f);

	m_fSize = (float)uSize;

	InitDepthStencilSurface(m_pDevice, uSize);

	m_pShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.ps));
	m_pShaderDataGS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.gs));
	m_pCameraShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_cameraShaderData.vs));
}

void CShadowCubeMap::setLight(IXLight *pLight)
{
	m_pLight = pLight;
}

void CShadowCubeMap::process(IXRenderPipeline *pRenderPipeline)
{
	assert(m_pLight && m_pLight->getType() == LIGHT_TYPE_POINT);
	if(!m_pDevice)
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	IXLightPoint *pPointLight = m_pLight->asPoint();

	float3 vPos = pPointLight->getPosition();

	static const float3 sc_vOrientedCube[6] = {float3(1, 0, 0), float3(-1, 0, 0), float3(0, 1, 0), float3(0, -1, 0), float3(0, 0, 1), float3(0, 0, -1)};
	static const float3 sc_vUpVectorsCube[6] = {float3(0, 1, 0), float3(0, 1, 0), float3(0, 0, -1), float3(0, 0, 1), float3(0, 1, 0), float3(0, 1, 0)};

	m_mProj = SMMatrixPerspectiveFovLH(SM_PIDIV2, 1.0f, 0.025f, pPointLight->getMaxDistance());
	for(UINT i = 0; i < 6; ++i)
	{
		m_mView[i] = SMMatrixLookAtLH(vPos, vPos + sc_vOrientedCube[i], sc_vUpVectorsCube[i]);
		m_shaderData.gs.mVP[i] = SMMatrixTranspose(m_mView[i] * m_mProj);
	}
	m_pShaderDataGS->update(&m_shaderData.gs);
	
	pCtx->setDepthStencilSurface(ms_pDepthStencilSurface);

	pCtx->setBlendState(NULL);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &SMMatrixIdentity());

	//m_lightingShaderData.vs.mViewInv = SMMatrixTranspose(SMMatrixInverse(NULL, m_mView));
	////m_lightingShaderData.vs.mVP = SMMatrixIdentity();
	//m_lightingShaderData.vs.vNearFar = float2(0.025f, pPointLight->getMaxDistance());
	//m_lightingShaderData.vs.vParamProj = float3_t(m_fSize, m_fSize, SM_PIDIV2);
	//m_pLightingShaderDataVS->update(&m_lightingShaderData.vs);

	//m_cameraShaderData.vs.mVP = SMMatrixIdentity();
	m_cameraShaderData.vs.vPosCam = vPos;
	m_pCameraShaderDataVS->update(&m_cameraShaderData.vs);
	pCtx->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	IGXSurface *pDepthSurface = m_pDepthMap->asRenderTarget();

	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, GX_COLOR_ARGB(255, 255, 255, 255));
	
	mem_release(pDepthSurface);
	
	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	pCtx->setGSConstant(m_pShaderDataGS, SCR_CAMERA);
	pCtx->setGSConstant(pLightConstants, SCR_OBJECT);
	mem_release(pLightConstants);

	pRenderPipeline->renderShadows();
	pRenderPipeline->renderStage(XRS_SHADOWS, m_pLight->getVisibility());

	pCtx->setColorTarget(NULL);

 	/*if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_depth.dds", m_pDepthMap);
		m_pDevice->saveTextureToFile("sm_normal.dds", m_pNormalMap);
		m_pDevice->saveTextureToFile("sm_flux.dds", m_pFluxMap);
	}*/
}

void CShadowCubeMap::genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals)
{
	if(!m_pDevice)
	{
		return;
	}
	const float c_fTexWidth = (float)pShadowMap->getWidth();
	const float c_fTexHeight = (float)pShadowMap->getHeight();

	IGXContext *pCtx = m_pDevice->getThreadContext();

//	IGXSurface *pRenderSurf, *pBackBuf;
//	pRenderSurf = pShadowMap->getMipmap(0);
//	pBackBuf = pCtx->getColorTarget();
//	pCtx->setColorTarget(pRenderSurf);

//	pCtx->clear(GX_CLEAR_COLOR);

	pCtx->setSamplerState(ms_pSamplerPointClamp, 0);
	pCtx->setSamplerState(ms_pSamplerComparisonLinearClamp, 1);
	//m_pDevice->setSamplerState(ms_pSamplerLinearClamp, 1);

	pCtx->setPSTexture(pGBufferDepth);
	pCtx->setPSTexture(m_pDepthMap, 4);
	pCtx->setPSTexture(pGBufferNormals, 3);

	m_shaderData.ps.vPixelSizeBias = float2(m_fBlurPixel / m_fSize, m_fBias);
	m_pShaderDataPS->update(&m_shaderData.ps);
	pCtx->setPSConstant(m_pShaderDataPS, 6);

	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	pCtx->setPSConstant(pLightConstants);
	mem_release(pLightConstants);
	
//	SGCore_ShaderBind(ms_idShader);




//	SGCore_ScreenQuadDraw();

//	SGCore_ShaderUnBind();

//	pCtx->setColorTarget(pBackBuf);
//
//	mem_release(pRenderSurf);
//	mem_release(pBackBuf);

	/*if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_shadow.dds", pShadowMap);
	}*/
}

//##########################################################################

CReflectiveShadowCubeMap::CReflectiveShadowCubeMap()
{}

CReflectiveShadowCubeMap::~CReflectiveShadowCubeMap()
{
	mem_release(m_pCameraShaderDataVS);
	mem_release(m_pShaderDataPS);
	mem_release(m_pShaderDataGS);
	mem_release(m_pDepthMap32);
	mem_release(m_pNormalMap32);
	mem_release(m_pFluxMap32);
	ReleaseDepthStencilSurface();
}

IGXDepthStencilSurface *CReflectiveShadowCubeMap::ms_pDepthStencilSurface32 = NULL;
UINT CReflectiveShadowCubeMap::ms_uDepthStencilSurfaceRefCount = 0;
IGXSamplerState *CReflectiveShadowCubeMap::ms_pSamplerPointWrap = NULL;
IGXSamplerState *CReflectiveShadowCubeMap::ms_pSamplerPointClamp = NULL;
IGXSamplerState *CReflectiveShadowCubeMap::ms_pSamplerLinearClamp = NULL;
ID CReflectiveShadowCubeMap::ms_idInjectShader32 = -1;
ID CReflectiveShadowCubeMap::ms_idInjectDebugShader32 = -1;

void CReflectiveShadowCubeMap::InitDepthStencilSurface(IGXDevice *pContext, UINT uSize)
{
	if(!ms_pDepthStencilSurface32)
	{
		ms_pDepthStencilSurface32 = pContext->createDepthStencilSurfaceCube(32, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);

		GXSamplerDesc samplerDesc;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointWrap = pContext->createSamplerState(&samplerDesc);

		samplerDesc.addressU = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressV = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointClamp = pContext->createSamplerState(&samplerDesc);

		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
		ms_pSamplerLinearClamp = pContext->createSamplerState(&samplerDesc);

		GXMacro definesInject32[] = {{"IS_CUBEMAP", "1"}, {"LPV_POINT_COUNT", "32"}, {"LPV_MAP_SIZE", "32"}, {0, 0}};
		ms_idInjectShader32 = SGCore_ShaderCreateKit(
			SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject_cube.vs", 0, definesInject32),
			SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInject32),
			SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, "gi_inject.gs", 0, definesInject32)
			);

		GXMacro definesInjectDebug32[] = {{"_DEBUG", "1"}, {"IS_CUBEMAP", "1"}, {"LPV_POINT_COUNT", "32"}, {"LPV_MAP_SIZE", "32"}, {0, 0}};
		ms_idInjectDebugShader32 = SGCore_ShaderCreateKit(
			SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject_cube.vs", 0, definesInjectDebug32),
			SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInjectDebug32)
			);
	}
	++ms_uDepthStencilSurfaceRefCount;
}
void CReflectiveShadowCubeMap::ReleaseDepthStencilSurface()
{
	if(--ms_uDepthStencilSurfaceRefCount == 0)
	{
		mem_release(ms_pDepthStencilSurface32);
		mem_release(ms_pSamplerPointWrap);
		mem_release(ms_pSamplerPointClamp);
		mem_release(ms_pSamplerLinearClamp);
	}
}

UINT CReflectiveShadowCubeMap::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow
	//GXFMT_A8R8G8B8 - xyz:color; w:???
	//GXFMT_A8R8G8B8 - xyz:normals; w:???

	return(32 * 32 * 12 * 6);
}

void CReflectiveShadowCubeMap::init(IGXDevice *pDevice, UINT uSize)
{
	m_pDevice = pDevice;

	//GXFMT_A8R8G8B8
	m_pNormalMap32 = m_pDevice->createTextureCube(32, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_A8R8G8B8
	m_pFluxMap32 = m_pDevice->createTextureCube(32, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_R32F
	m_pDepthMap32 = m_pDevice->createTextureCube(32, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);

	float fOffset = 0.5f + (0.5f / (float)uSize);
	float fRange = 1.0f;
	float fBias = 0.0f;
	m_mScaleBiasMat = float4x4(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, 0.0f,
		fOffset, fOffset, fBias, 1.0f);

	m_fSize = (float)uSize;

	InitDepthStencilSurface(m_pDevice, uSize);

	m_pShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.ps));
	m_pShaderDataGS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.gs));
	m_pCameraShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_cameraShaderData.vs));
}

void CReflectiveShadowCubeMap::setLight(IXLight *pLight)
{
	m_pLight = pLight;
}

void CReflectiveShadowCubeMap::process(IXRenderPipeline *pRenderPipeline)
{
	assert(m_pLight && m_pLight->getType() == LIGHT_TYPE_POINT);
	if(!m_pDevice)
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	IXLightPoint *pPointLight = m_pLight->asPoint();

	float3 vPos = pPointLight->getPosition();

	static const float3 sc_vOrientedCube[6] = {float3(1, 0, 0), float3(-1, 0, 0), float3(0, 1, 0), float3(0, -1, 0), float3(0, 0, 1), float3(0, 0, -1)};
	static const float3 sc_vUpVectorsCube[6] = {float3(0, 1, 0), float3(0, 1, 0), float3(0, 0, -1), float3(0, 0, 1), float3(0, 1, 0), float3(0, 1, 0)};

	m_mProj = SMMatrixPerspectiveFovLH(SM_PIDIV2, 1.0f, 0.025f, pPointLight->getMaxDistance());
	for(UINT i = 0; i < 6; ++i)
	{
		m_mView[i] = SMMatrixLookAtLH(vPos, vPos + sc_vOrientedCube[i], sc_vUpVectorsCube[i]);
		m_shaderData.gs.mVP[i] = SMMatrixTranspose(m_mView[i] * m_mProj);
	}
	m_pShaderDataGS->update(&m_shaderData.gs);

	pCtx->setDepthStencilSurface(ms_pDepthStencilSurface32);

	pCtx->setBlendState(NULL);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &SMMatrixIdentity());

	//m_lightingShaderData.vs.mViewInv = SMMatrixTranspose(SMMatrixInverse(NULL, m_mView));
	////m_lightingShaderData.vs.mVP = SMMatrixIdentity();
	//m_lightingShaderData.vs.vNearFar = float2(0.025f, pPointLight->getMaxDistance());
	//m_lightingShaderData.vs.vParamProj = float3_t(m_fSize, m_fSize, SM_PIDIV2);
	//m_pLightingShaderDataVS->update(&m_lightingShaderData.vs);

	//m_cameraShaderData.vs.mVP = SMMatrixIdentity();
	m_cameraShaderData.vs.vPosCam = vPos;
	m_pCameraShaderDataVS->update(&m_cameraShaderData.vs);
	pCtx->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	IGXSurface *pDepthSurface = m_pDepthMap32->asRenderTarget();
	IGXSurface *pNormalSurface = m_pNormalMap32->asRenderTarget();
	IGXSurface *pFluxSurface = m_pFluxMap32->asRenderTarget();
	
	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, GX_COLOR_ARGB(255, 255, 255, 255));

	pCtx->setColorTarget(pFluxSurface, 0);
	pCtx->setColorTarget(pNormalSurface, 1);
	pCtx->clear(GX_CLEAR_COLOR);
	
	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(pNormalSurface, 1);
	pCtx->setColorTarget(pFluxSurface, 2);

	mem_release(pDepthSurface);
	mem_release(pNormalSurface);
	mem_release(pFluxSurface);

	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	pCtx->setGSConstant(m_pShaderDataGS, SCR_CAMERA);
	pCtx->setGSConstant(pLightConstants, SCR_OBJECT);
	mem_release(pLightConstants);

	pRenderPipeline->renderShadows();
	pRenderPipeline->renderStage(XRS_SHADOWS, m_pLight->getVisibility());

	pCtx->setColorTarget(NULL);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);

	/*if(GetAsyncKeyState('U') < 0)
	{
	m_pDevice->saveTextureToFile("sm_depth.dds", m_pDepthMap);
	m_pDevice->saveTextureToFile("sm_normal.dds", m_pNormalMap);
	m_pDevice->saveTextureToFile("sm_flux.dds", m_pFluxMap);
	}*/
}

void CReflectiveShadowCubeMap::genLPV(bool isDebug)
{
	if(!m_pDevice || !(m_pLight->getRenderType() & LRT_LPV))
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	pCtx->setRenderBuffer(NULL);
	pCtx->setIndexBuffer(NULL);
	pCtx->setPrimitiveTopology(GXPT_POINTLIST);

	pCtx->setVSTexture(m_pDepthMap32);
	pCtx->setVSTexture(m_pNormalMap32, 1);
	pCtx->setVSTexture(m_pFluxMap32, 2);

	IGXConstantBuffer *pLightConstant = m_pLight->getConstants(m_pDevice);
	pCtx->setVSConstant(pLightConstant, 7);
	mem_release(pLightConstant);

	SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader32 : ms_idInjectShader32);
	pCtx->drawPrimitive(0, 32 * 32 * 6);

	SGCore_ShaderUnBind();
	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);

	pCtx->setVSTexture(NULL);
	pCtx->setVSTexture(NULL, 1);
	pCtx->setVSTexture(NULL, 2);
}
