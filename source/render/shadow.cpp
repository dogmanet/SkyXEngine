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
	mem_release(m_pNormalMap);
	mem_release(m_pFluxMap);
	mem_release(m_pDepthMap32);
	mem_release(m_pNormalMap32);
	mem_release(m_pFluxMap32);
	ReleaseDepthStencilSurface();
}

IGXDepthStencilSurface *CShadowMap::ms_pDepthStencilSurface = NULL;
IGXDepthStencilSurface *CShadowMap::ms_pDepthStencilSurface32 = NULL;
UINT CShadowMap::ms_uDepthStencilSurfaceRefCount = 0;
IGXSamplerState *CShadowMap::ms_pSamplerPointWrap = NULL;
IGXSamplerState *CShadowMap::ms_pSamplerPointClamp = NULL;
IGXSamplerState *CShadowMap::ms_pSamplerLinearClamp = NULL;
IGXSamplerState *CShadowMap::ms_pSamplerComparisonLinearClamp = NULL;
ID CShadowMap::ms_idShader = -1;
ID CShadowMap::ms_idInjectShader = -1;
ID CShadowMap::ms_idInjectDebugShader = -1;
ID CShadowMap::ms_idInjectShader32 = -1;
ID CShadowMap::ms_idInjectDebugShader32 = -1;
IGXTexture2D *CShadowMap::ms_pRandomTexture = NULL;

void CShadowMap::InitDepthStencilSurface(IGXDevice *pContext, UINT uSize)
{
	if(!ms_pDepthStencilSurface)
	{
		ms_pDepthStencilSurface = pContext->createDepthStencilSurface(uSize, uSize, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);
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

		samplerDesc.comparisonFunc = GXCMP_LESS;
		samplerDesc.filter = GXFILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		ms_pSamplerComparisonLinearClamp = pContext->createSamplerState(&samplerDesc);
		

		ID idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs");
		GXMacro Defines_GSD_9[] = {{"GSD_9", ""}, {0, 0}};
		ID idGenShadowDirect9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct_9.ps", Defines_GSD_9);
		ms_idShader = SGCore_ShaderCreateKit(idResPosDepth, idGenShadowDirect9);

		uint32_t aRndColors[16];// = D3DCOLOR_ARGB(0, 250, 2, 255);

		for(int i = 0; i < 16; ++i)
			aRndColors[i] = GX_COLOR_ARGB(255, rand() % 255, rand() % 255, rand() % 255);

		ms_pRandomTexture = pContext->createTexture2D(4, 4, 1, 0, GXFMT_A8R8G8B8, aRndColors);

		static char tmp[32];
		sprintf(tmp, "%u", uSize);

		GXMacro definesInject[] = {{"LPV_POINT_COUNT", "256"}, {"LPV_MAP_SIZE", tmp}, {0, 0}};
		ms_idInjectShader = SGCore_ShaderCreateKit(
			SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject.vs", 0, definesInject),
			SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInject),
			SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, "gi_inject.gs", 0, definesInject)
			);

		GXMacro definesInjectDebug[] = {{"_DEBUG", "1"}, {"LPV_POINT_COUNT", "256"}, {"LPV_MAP_SIZE", tmp}, {0, 0}};
		ms_idInjectDebugShader = SGCore_ShaderCreateKit(
			SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject.vs", 0, definesInjectDebug),
			SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInjectDebug)
			);
		
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
void CShadowMap::ReleaseDepthStencilSurface()
{
	if(--ms_uDepthStencilSurfaceRefCount == 0)
	{
		mem_release(ms_pSamplerComparisonLinearClamp);
		mem_release(ms_pDepthStencilSurface);
		mem_release(ms_pDepthStencilSurface32);
		mem_release(ms_pSamplerPointWrap);
		mem_release(ms_pSamplerPointClamp);
		mem_release(ms_pSamplerLinearClamp);
		mem_release(ms_pRandomTexture);
	}
}

UINT CShadowMap::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow
	//GXFMT_A8R8G8B8 - xyz:color; w:???
	//GXFMT_A8R8G8B8 - xyz:normals; w:???

	return(uSize * uSize * 12);
}

void CShadowMap::init(IGXDevice *pContext, UINT uSize)
{
	m_pDevice = pContext;

	//GXFMT_A8R8G8B8
	m_pNormalMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_A8R8G8B8
	m_pFluxMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_R32F
	m_pDepthMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);

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

	pCtx->setDepthStencilSurface((m_pLight->getRenderType() & LRT_LIGHT) ? ms_pDepthStencilSurface : ms_pDepthStencilSurface32);

	pCtx->setBlendState(NULL);

	IGXSurface *pDepthSurface = NULL;
	IGXSurface *pNormalSurface = NULL;
	IGXSurface *pFluxSurface = NULL;
	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		pDepthSurface = m_pDepthMap->asRenderTarget();
		if(m_pLight->getRenderType() & LRT_LPV)
		{
			pNormalSurface = m_pNormalMap->asRenderTarget();
			pFluxSurface = m_pFluxMap->asRenderTarget();
		}
	}
	else
	{
		pDepthSurface = m_pDepthMap32->asRenderTarget();
		pNormalSurface = m_pNormalMap32->asRenderTarget();
		pFluxSurface = m_pFluxMap32->asRenderTarget();
	}

	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, GX_COLOR_ARGB(255, 255, 255, 255));
	
//	pCtx->setColorTarget(pFluxSurface, 0);
//	pCtx->setColorTarget(pNormalSurface, 1);
//	pCtx->clear(GX_CLEAR_COLOR);
//	
//	pCtx->setColorTarget(pDepthSurface);
//	pCtx->setColorTarget(pNormalSurface, 1);
//	pCtx->setColorTarget(pFluxSurface, 2);

	mem_release(pDepthSurface);
	mem_release(pNormalSurface);
	mem_release(pFluxSurface);
	
	//m_pLight->setPSConstants(m_pDevice, 5);

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

void CShadowMap::genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals)
{
	if(!m_pDevice)
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	const float c_fTexWidth = (float)pShadowMap->getWidth();
	const float c_fTexHeight = (float)pShadowMap->getHeight();

	IGXSurface *pRenderSurf, *pBackBuf;
	pRenderSurf = pShadowMap->getMipmap(0);
	pBackBuf = pCtx->getColorTarget();
	pCtx->setColorTarget(pRenderSurf);
	pCtx->setColorTarget(NULL, 1);

	pCtx->clear(GX_CLEAR_COLOR);

	pCtx->setSamplerState(ms_pSamplerPointClamp, 0);
	//m_pDevice->setSamplerState(ms_pSamplerLinearClamp, 1);
	pCtx->setSamplerState(ms_pSamplerComparisonLinearClamp, 1);
	pCtx->setSamplerState(ms_pSamplerPointWrap, 2);
	
	pCtx->setPSTexture(pGBufferDepth);
	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		pCtx->setPSTexture(m_pDepthMap, 1);
	}
	else
	{
		pCtx->setPSTexture(m_pDepthMap32, 1);
	}
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

	SGCore_ShaderBind(ms_idShader);
#if 0
	if(light_data::isHalfGenPCFShadowLocal)
	{
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowDirect4);
	}
	else
	{
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowDirect9);

	}
#endif

		

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pCtx->setColorTarget(pBackBuf);

	mem_release(pRenderSurf);
	mem_release(pBackBuf);

	/*if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_shadow.dds", pShadowMap);
	}*/
}

void CShadowMap::genLPV(bool isDebug)
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

	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		pCtx->setVSTexture(m_pDepthMap);
		pCtx->setVSTexture(m_pNormalMap, 1);
		pCtx->setVSTexture(m_pFluxMap, 2);
	}
	else
	{
		pCtx->setVSTexture(m_pDepthMap32);
		pCtx->setVSTexture(m_pNormalMap32, 1);
		pCtx->setVSTexture(m_pFluxMap32, 2);
	}

	m_pShaderDataInjectVS->update(&SMMatrixTranspose(SMMatrixInverse(NULL, m_mView * m_mProj)));
	pCtx->setVSConstant(m_pShaderDataInjectVS, 6);

	IGXConstantBuffer *pLightConstant = m_pLight->getConstants(m_pDevice);
	pCtx->setVSConstant(pLightConstant, 7);
	mem_release(pLightConstant);

	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader : ms_idInjectShader);
		pCtx->drawPrimitive(0, 256 * 256);
	}
	else
	{
		SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader32 : ms_idInjectShader32);
		pCtx->drawPrimitive(0, 32 * 32);
	}

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

	mem_release(m_pCameraShaderDataVS);
	mem_release(m_pShaderDataInjectVS);
	mem_release(m_pShaderDataPS);

	for(UINT i = 0; i < PSSM_MAX_SPLITS; ++i)
	{
		mem_release(m_splits[i].pDepthMap);
		mem_release(m_splits[i].pNormalMap);
		mem_release(m_splits[i].pFluxMap);
	}
}

UINT CShadowPSSM::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow
	//GXFMT_A8R8G8B8 - xyz:color; w:???
	//GXFMT_A8R8G8B8 - xyz:normals; w:???

	return(uSize * uSize * 12 * PSSM_MAX_SPLITS);
}

void CShadowPSSM::init(IGXDevice *pContext, UINT uSize)
{
	m_pDevice = pContext;

	for(UINT i = 0; i < PSSM_MAX_SPLITS; ++i)
	{
		//GXFMT_A8R8G8B8
		m_splits[i].pNormalMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
		//GXFMT_A8R8G8B8
		m_splits[i].pFluxMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
		//GXFMT_R32F
		m_splits[i].pDepthMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);
	}

	float fOffset = 0.5f + (0.5f / (float)uSize);
	float fRange = 1.0f;
	float fBias = 0.0f;
	m_mScaleBiasMat = float4x4(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, fRange, 0.0f,
		fOffset, fOffset, fBias, 1.0f);

	m_fSize = (float)uSize;

	m_pDepthStencilSurface = pContext->createDepthStencilSurface(uSize, uSize, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);

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


	ID idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs");
	GXMacro Defines_GSD_9[] = {{"GSD_9", ""}, {0, 0}};
	ID idGenShadowDirect9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct_9.ps", Defines_GSD_9);
	m_idShader = SGCore_ShaderCreateKit(idResPosDepth, idGenShadowDirect9);

	static char tmp[32];
	sprintf(tmp, "%u", uSize);

	GXMacro definesInject[] = {{"LPV_POINT_COUNT", "256"}, {"LPV_MAP_SIZE", tmp}, {0, 0}};
	m_idInjectShader = SGCore_ShaderCreateKit(
		SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject.vs", 0, definesInject),
		SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInject),
		SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, "gi_inject.gs", 0, definesInject)
		);

	GXMacro definesInjectDebug[] = {{"_DEBUG", "1"}, {"LPV_POINT_COUNT", "256"}, {"LPV_MAP_SIZE", tmp}, {0, 0}};
	m_idInjectDebugShader = SGCore_ShaderCreateKit(
		SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject.vs", 0, definesInjectDebug),
		SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInjectDebug)
		);

	m_pShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.ps));
	m_pShaderDataInjectVS = m_pDevice->createConstantBuffer(sizeof(SMMATRIX));
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
#if 0

	IXLightSun *pSunLight = m_pLight->asSun();

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
	m_pDevice->setVertexShaderConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	m_pDevice->setDepthStencilSurface(m_pDepthStencilSurface);

	m_pDevice->setBlendState(NULL);

	IGXSurface *pDepthSurface = NULL;
	IGXSurface *pNormalSurface = NULL;
	IGXSurface *pFluxSurface = NULL;
	
	pDepthSurface = m_pDepthMap->asRenderTarget();
	pNormalSurface = m_pNormalMap->asRenderTarget();
	pFluxSurface = m_pFluxMap->asRenderTarget();

	m_pDevice->setColorTarget(pDepthSurface);
	m_pDevice->setColorTarget(NULL, 1);
	m_pDevice->setColorTarget(NULL, 2);
	m_pDevice->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, GX_COLOR_ARGB(255, 255, 255, 255));

	m_pDevice->setColorTarget(pFluxSurface, 0);
	m_pDevice->setColorTarget(pNormalSurface, 1);
	m_pDevice->clear(GX_CLEAR_COLOR);

	m_pDevice->setColorTarget(pDepthSurface);
	m_pDevice->setColorTarget(pNormalSurface, 1);
	m_pDevice->setColorTarget(pFluxSurface, 2);

	mem_release(pDepthSurface);
	mem_release(pNormalSurface);
	mem_release(pFluxSurface);

	//m_pLight->setPSConstants(m_pDevice, 5);

	pRenderPipeline->renderStage(XRS_SHADOWS, m_pLight->getVisibility());

	m_pDevice->setColorTarget(NULL);
	m_pDevice->setColorTarget(NULL, 1);
	m_pDevice->setColorTarget(NULL, 2);

	/*if(GetAsyncKeyState('U') < 0)
	{
	m_pDevice->saveTextureToFile("sm_depth.dds", m_pDepthMap);
	m_pDevice->saveTextureToFile("sm_normal.dds", m_pNormalMap);
	m_pDevice->saveTextureToFile("sm_flux.dds", m_pFluxMap);
	}*/
#endif
}

void CShadowPSSM::genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals)
{
	if(!m_pDevice)
	{
		return;
	}
#if 0
	const float c_fTexWidth = (float)pShadowMap->getWidth();
	const float c_fTexHeight = (float)pShadowMap->getHeight();

	IGXSurface *pRenderSurf, *pBackBuf;
	pRenderSurf = pShadowMap->getMipmap(0);
	pBackBuf = m_pDevice->getColorTarget();
	m_pDevice->setColorTarget(pRenderSurf);
	m_pDevice->setColorTarget(NULL, 1);

	m_pDevice->clear(GX_CLEAR_COLOR);

	m_pDevice->setSamplerState(m_pSamplerPointClamp, 0);
	//m_pDevice->setSamplerState(ms_pSamplerLinearClamp, 1);
	m_pDevice->setSamplerState(m_pSamplerComparisonLinearClamp, 1);
	m_pDevice->setSamplerState(m_pSamplerPointWrap, 2);

	m_pDevice->setTexture(pGBufferDepth);
	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		m_pDevice->setTexture(m_pDepthMap, 1);
	}
	else
	{
		m_pDevice->setTexture(m_pDepthMap32, 1);
	}
	m_pDevice->setTexture(pGBufferNormals, 3);

	m_shaderData.ps.mMatrixTexture = SMMatrixTranspose(m_mView * m_mProj * m_mScaleBiasMat);
	m_shaderData.ps.vPixelMapSizeBias = float3(m_fBlurPixel / m_fSize, m_fSize, m_fBias);
	m_pShaderDataPS->update(&m_shaderData.ps);
	m_pDevice->setPixelShaderConstant(m_pShaderDataPS, 6);

	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	m_pDevice->setPixelShaderConstant(pLightConstants);
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

	SGCore_ShaderBind(m_idShader);


	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	m_pDevice->setColorTarget(pBackBuf);

	mem_release(pRenderSurf);
	mem_release(pBackBuf);
#endif
	/*if(GetAsyncKeyState('U') < 0)
	{
	m_pDevice->saveTextureToFile("sm_shadow.dds", pShadowMap);
	}*/
}

void CShadowPSSM::genLPV(bool isDebug)
{
	if(!m_pDevice || !(m_pLight->getRenderType() & LRT_LPV))
	{
		return;
	}
#if 0
	m_pDevice->setRenderBuffer(NULL);
	m_pDevice->setIndexBuffer(NULL);
	m_pDevice->setPrimitiveTopology(GXPT_POINTLIST);
	//SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader : ms_idInjectShader);

	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		m_pDevice->setTextureVS(m_pDepthMap);
		m_pDevice->setTextureVS(m_pNormalMap, 1);
		m_pDevice->setTextureVS(m_pFluxMap, 2);
	}
	else
	{
		m_pDevice->setTextureVS(m_pDepthMap32);
		m_pDevice->setTextureVS(m_pNormalMap32, 1);
		m_pDevice->setTextureVS(m_pFluxMap32, 2);
	}

	m_pShaderDataInjectVS->update(&SMMatrixTranspose(SMMatrixInverse(NULL, m_mView * m_mProj)));
	m_pDevice->setVertexShaderConstant(m_pShaderDataInjectVS, 6);

	IGXConstantBuffer *pLightConstant = m_pLight->getConstants(m_pDevice);
	m_pDevice->setVertexShaderConstant(pLightConstant, 7);
	mem_release(pLightConstant);

	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader : ms_idInjectShader);
		m_pDevice->drawPrimitive(0, 256 * 256);
	}
	else
	{
		SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader32 : ms_idInjectShader32);
		m_pDevice->drawPrimitive(0, 32 * 32);
	}

	SGCore_ShaderUnBind();
	m_pDevice->setPrimitiveTopology(GXPT_TRIANGLELIST);

	m_pDevice->setTextureVS(NULL);
	m_pDevice->setTextureVS(NULL, 1);
	m_pDevice->setTextureVS(NULL, 2);
#endif
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
	mem_release(m_pNormalMap);
	mem_release(m_pFluxMap);
	mem_release(m_pDepthMap32);
	mem_release(m_pNormalMap32);
	mem_release(m_pFluxMap32);
	ReleaseDepthStencilSurface();
}

IGXDepthStencilSurface *CShadowCubeMap::ms_pDepthStencilSurface = NULL;
IGXDepthStencilSurface *CShadowCubeMap::ms_pDepthStencilSurface32 = NULL;
UINT CShadowCubeMap::ms_uDepthStencilSurfaceRefCount = 0;
IGXSamplerState *CShadowCubeMap::ms_pSamplerPointWrap = NULL;
IGXSamplerState *CShadowCubeMap::ms_pSamplerPointClamp = NULL;
IGXSamplerState *CShadowCubeMap::ms_pSamplerLinearClamp = NULL;
IGXSamplerState *CShadowCubeMap::ms_pSamplerComparisonLinearClamp = NULL;
ID CShadowCubeMap::ms_idShader = -1;
ID CShadowCubeMap::ms_idInjectShader = -1;
ID CShadowCubeMap::ms_idInjectShader32 = -1;
ID CShadowCubeMap::ms_idInjectDebugShader = -1;
ID CShadowCubeMap::ms_idInjectDebugShader32 = -1;

void CShadowCubeMap::InitDepthStencilSurface(IGXDevice *pContext, UINT uSize)
{
	if(!ms_pDepthStencilSurface)
	{
		ms_pDepthStencilSurface = pContext->createDepthStencilSurfaceCube(uSize, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);
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

		samplerDesc.comparisonFunc = GXCMP_LESS;
		samplerDesc.filter = GXFILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
		ms_pSamplerComparisonLinearClamp = pContext->createSamplerState(&samplerDesc);

		ID idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs");
		GXMacro Defines_GSC_6[] = {{"GSC_6", ""}, {0, 0}};
		ID idGenShadowPoint6 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point_6.ps", Defines_GSC_6);
		ms_idShader = SGCore_ShaderCreateKit(idResPosDepth, idGenShadowPoint6);

		static char tmp[32];
		sprintf(tmp, "%u", uSize);

		GXMacro definesInject[] = {{"IS_CUBEMAP", "1"}, {"LPV_POINT_COUNT", "256"}, {"LPV_MAP_SIZE", tmp}, {0, 0}};
		ms_idInjectShader = SGCore_ShaderCreateKit(
			SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject_cube.vs", 0, definesInject),
			SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInject),
			SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, "gi_inject.gs", 0, definesInject)
			);

		GXMacro definesInjectDebug[] = {{"_DEBUG", "1"}, {"IS_CUBEMAP", "1"}, {"LPV_POINT_COUNT", "256"}, {"LPV_MAP_SIZE", tmp}, {0, 0}};
		ms_idInjectDebugShader = SGCore_ShaderCreateKit(
			SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject_cube.vs", 0, definesInjectDebug),
			SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps", 0, definesInjectDebug)
			);

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
void CShadowCubeMap::ReleaseDepthStencilSurface()
{
	if(--ms_uDepthStencilSurfaceRefCount == 0)
	{
		mem_release(ms_pSamplerComparisonLinearClamp);
		mem_release(ms_pDepthStencilSurface);
		mem_release(ms_pDepthStencilSurface32);
		mem_release(ms_pSamplerPointWrap);
		mem_release(ms_pSamplerPointClamp);
		mem_release(ms_pSamplerLinearClamp);
	}
}

UINT CShadowCubeMap::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow
	//GXFMT_A8R8G8B8 - xyz:color; w:???
	//GXFMT_A8R8G8B8 - xyz:normals; w:???

	return(uSize * uSize * 12 * 6);
}

void CShadowCubeMap::init(IGXDevice *pDevice, UINT uSize)
{
	m_pDevice = pDevice;

	//GXFMT_A8R8G8B8
	m_pNormalMap = m_pDevice->createTextureCube(uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_A8R8G8B8
	m_pFluxMap = m_pDevice->createTextureCube(uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_R32F
	m_pDepthMap = m_pDevice->createTextureCube(uSize, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);

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
	
	pCtx->setDepthStencilSurface((m_pLight->getRenderType() & LRT_LIGHT) ? ms_pDepthStencilSurface : ms_pDepthStencilSurface32);

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

	IGXSurface *pDepthSurface = NULL;
	IGXSurface *pNormalSurface = NULL;
	IGXSurface *pFluxSurface = NULL;
	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		pDepthSurface = m_pDepthMap->asRenderTarget();
		if(m_pLight->getRenderType() & LRT_LPV)
		{
			pNormalSurface = m_pNormalMap->asRenderTarget();
			pFluxSurface = m_pFluxMap->asRenderTarget();
		}
	}
	else
	{
		pDepthSurface = m_pDepthMap32->asRenderTarget();
		pNormalSurface = m_pNormalMap32->asRenderTarget();
		pFluxSurface = m_pFluxMap32->asRenderTarget();
	}

	pCtx->setColorTarget(pDepthSurface);
	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, GX_COLOR_ARGB(255, 255, 255, 255));
	
//	pCtx->setColorTarget(pFluxSurface, 0);
//	pCtx->setColorTarget(pNormalSurface, 1);
//	pCtx->clear(GX_CLEAR_COLOR);
//
//	pCtx->setColorTarget(pDepthSurface);
//	pCtx->setColorTarget(pNormalSurface, 1);
//	pCtx->setColorTarget(pFluxSurface, 2);

	mem_release(pDepthSurface);
	mem_release(pNormalSurface);
	mem_release(pFluxSurface);
	
	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	pCtx->setGSConstant(m_pShaderDataGS, SCR_CAMERA);
	pCtx->setGSConstant(pLightConstants, SCR_OBJECT);
	mem_release(pLightConstants);

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

void CShadowCubeMap::genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth, IGXTexture2D *pGBufferNormals)
{
	if(!m_pDevice)
	{
		return;
	}
	const float c_fTexWidth = (float)pShadowMap->getWidth();
	const float c_fTexHeight = (float)pShadowMap->getHeight();

	IGXContext *pCtx = m_pDevice->getThreadContext();

	IGXSurface *pRenderSurf, *pBackBuf;
	pRenderSurf = pShadowMap->getMipmap(0);
	pBackBuf = pCtx->getColorTarget();
	pCtx->setColorTarget(pRenderSurf);

	pCtx->clear(GX_CLEAR_COLOR);

	pCtx->setSamplerState(ms_pSamplerPointClamp, 0);
	pCtx->setSamplerState(ms_pSamplerComparisonLinearClamp, 1);
	//m_pDevice->setSamplerState(ms_pSamplerLinearClamp, 1);

	pCtx->setPSTexture(pGBufferDepth);
	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		pCtx->setPSTexture(m_pDepthMap, 1);
	}
	else
	{
		pCtx->setPSTexture(m_pDepthMap32, 1);
	}
	pCtx->setPSTexture(pGBufferNormals, 3);

	m_shaderData.ps.vPixelSizeBias = float2(m_fBlurPixel / m_fSize, m_fBias);
	m_pShaderDataPS->update(&m_shaderData.ps);
	pCtx->setPSConstant(m_pShaderDataPS, 6);

	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	pCtx->setPSConstant(pLightConstants);
	mem_release(pLightConstants);
	
	SGCore_ShaderBind(ms_idShader);
#if 0
	if(light_data::isHalfGenPCFShadowLocal)
	{
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowDirect4);
	}
	else
	{
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowDirect9);

	}
#endif



	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pCtx->setColorTarget(pBackBuf);

	mem_release(pRenderSurf);
	mem_release(pBackBuf);

	/*if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_shadow.dds", pShadowMap);
	}*/
}

void CShadowCubeMap::genLPV(bool isDebug)
{
	if(!m_pDevice || !(m_pLight->getRenderType() & LRT_LPV))
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getThreadContext();

	pCtx->setRenderBuffer(NULL);
	pCtx->setIndexBuffer(NULL);
	pCtx->setPrimitiveTopology(GXPT_POINTLIST);

	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		pCtx->setVSTexture(m_pDepthMap);
		pCtx->setVSTexture(m_pNormalMap, 1);
		pCtx->setVSTexture(m_pFluxMap, 2);
	}
	else
	{
		pCtx->setVSTexture(m_pDepthMap32);
		pCtx->setVSTexture(m_pNormalMap32, 1);
		pCtx->setVSTexture(m_pFluxMap32, 2);
	}

	IGXConstantBuffer *pLightConstant = m_pLight->getConstants(m_pDevice);
	pCtx->setVSConstant(pLightConstant, 7);
	mem_release(pLightConstant);

	if(m_pLight->getRenderType() & LRT_LIGHT)
	{
		SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader : ms_idInjectShader);
		pCtx->drawPrimitive(0, 256 * 256 * 6);
	}
	else
	{
		SGCore_ShaderBind(isDebug ? ms_idInjectDebugShader32 : ms_idInjectShader32);
		pCtx->drawPrimitive(0, 32 * 32 * 6);
	}

	SGCore_ShaderUnBind();
	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);

	pCtx->setVSTexture(NULL);
	pCtx->setVSTexture(NULL, 1);
	pCtx->setVSTexture(NULL, 2);
}
