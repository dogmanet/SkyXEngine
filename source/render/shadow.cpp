#include "shadow.h"
#include <mtrl/IXMaterialSystem.h>

CShadowMap::CShadowMap()
{
}

CShadowMap::~CShadowMap()
{
	mem_release(m_pShaderDataInjectVS);
	mem_release(m_pShaderDataPS);
	mem_release(m_pDepthMap);
	mem_release(m_pNormalMap);
	mem_release(m_pFluxMap);
	ReleaseDepthStencilSurface();
}

IGXDepthStencilSurface *CShadowMap::ms_pDepthStencilSurface = NULL;
UINT CShadowMap::ms_uDepthStencilSurfaceRefCount = 0;
IGXSamplerState *CShadowMap::ms_pSamplerPointWrap = NULL;
IGXSamplerState *CShadowMap::ms_pSamplerPointClamp = NULL;
IGXSamplerState *CShadowMap::ms_pSamplerLinearClamp = NULL;
ID CShadowMap::ms_idShader = -1;
ID CShadowMap::ms_idInjectShader = -1;
IGXTexture2D *CShadowMap::ms_pRandomTexture = NULL;

void CShadowMap::InitDepthStencilSurface(IGXContext *pContext, UINT uSize)
{
	if(!ms_pDepthStencilSurface)
	{
		ms_pDepthStencilSurface = pContext->createDepthStencilSurface(uSize, uSize, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);

		GXSAMPLER_DESC samplerDesc;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointWrap = pContext->createSamplerState(&samplerDesc);

		samplerDesc.addressU = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressV = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointClamp = pContext->createSamplerState(&samplerDesc);

		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
		ms_pSamplerLinearClamp = pContext->createSamplerState(&samplerDesc);

		ID idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs");
		GXMACRO Defines_GSD_9[] = {{"GSD_9", ""}, {0, 0}};
		ID idGenShadowDirect9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct_9.ps", Defines_GSD_9);
		ms_idShader = SGCore_ShaderCreateKit(idResPosDepth, idGenShadowDirect9);

		uint32_t aRndColors[16];// = D3DCOLOR_ARGB(0, 250, 2, 255);

		for(int i = 0; i < 16; ++i)
			aRndColors[i] = GXCOLOR_ARGB(255, rand() % 255, rand() % 255, rand() % 255);

		ms_pRandomTexture = pContext->createTexture2D(4, 4, 1, 0, GXFMT_A8R8G8B8, aRndColors);

		ms_idInjectShader = SGCore_ShaderCreateKit(
			SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_inject.vs"),
			SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_inject.ps"),
			SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, "gi_inject.gs")
			);
	}
	++ms_uDepthStencilSurfaceRefCount;
}
void CShadowMap::ReleaseDepthStencilSurface()
{
	if(--ms_uDepthStencilSurfaceRefCount == 0)
	{
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
	//GXFMT_A8R8G8B8 - xyz:color; w:???
	//GXFMT_A8R8G8B8 - xyz:normals; w:???

	return(uSize * uSize * 12);
}

void CShadowMap::init(IGXContext *pContext, UINT uSize)
{
	m_pDevice = pContext;

	//GXFMT_A8R8G8B8
	m_pNormalMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_A8R8G8B8
	m_pFluxMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_R32F
	m_pDepthMap = m_pDevice->createTexture2D(uSize, uSize, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_R32F);

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
}

void CShadowMap::setLight(IXLight *pLight)
{
	m_pLight = pLight;
}

void CShadowMap::process(IXRenderPipeline *pRenderPipeline)
{
	assert(m_pLight && m_pLight->getType() == LIGHT_TYPE_SPOT);
	IXLightSpot *pSpotLight = m_pLight->asSpot();

	float3 vPos = pSpotLight->getPosition();
	float3 vDir = pSpotLight->getDirection() * LIGHTS_DIR_BASE;
	float3 vUp = pSpotLight->getDirection() * float3(0.0f, 0.0f, 1.0f);

	m_mView = SMMatrixLookAtLH(vPos, vPos + vDir, vUp);
	m_mProj = SMMatrixPerspectiveFovLH(pSpotLight->getOuterAngle(), 1.0f, 0.0025f, pSpotLight->getMaxDistance());

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mProj);

	m_pDevice->setDepthStencilSurface(ms_pDepthStencilSurface);

	m_pDevice->setBlendState(NULL);

	IGXSurface *pDepthSurface = m_pDepthMap->getMipmap();
	IGXSurface *pNormalSurface = m_pNormalMap->getMipmap();
	IGXSurface *pFluxSurface = m_pFluxMap->getMipmap();

	m_pDevice->setColorTarget(pDepthSurface);
	m_pDevice->setColorTarget(NULL, 1);
	m_pDevice->setColorTarget(NULL, 2);
	m_pDevice->clear(GXCLEAR_COLOR | GXCLEAR_DEPTH | GXCLEAR_STENCIL, GXCOLOR_ARGB(255, 255, 255, 255));

	m_pDevice->setColorTarget(pFluxSurface, 0);
	m_pDevice->setColorTarget(pNormalSurface, 1);
	m_pDevice->clear(GXCLEAR_COLOR);

	m_pDevice->setColorTarget(pDepthSurface);
	m_pDevice->setColorTarget(pNormalSurface, 1);
	m_pDevice->setColorTarget(pFluxSurface, 2);

	mem_release(pDepthSurface);
	mem_release(pNormalSurface);
	mem_release(pFluxSurface);
	
	//m_pLight->setPSConstants(m_pDevice, 5);

	pRenderPipeline->renderStage(XRS_SHADOWS);

	m_pDevice->setColorTarget(NULL);
	m_pDevice->setColorTarget(NULL, 1);
	m_pDevice->setColorTarget(NULL, 2);

	if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_depth.dds", m_pDepthMap);
		m_pDevice->saveTextureToFile("sm_normal.dds", m_pNormalMap);
		m_pDevice->saveTextureToFile("sm_flux.dds", m_pFluxMap);
	}
}

void CShadowMap::genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth)
{

	const float c_fTexWidth = (float)pShadowMap->getWidth();
	const float c_fTexHeight = (float)pShadowMap->getHeight();

	IGXSurface *pRenderSurf, *pBackBuf;
	pRenderSurf = pShadowMap->getMipmap(0);
	pBackBuf = m_pDevice->getColorTarget();
	m_pDevice->setColorTarget(pRenderSurf);

	m_pDevice->clear(GXCLEAR_COLOR);

	m_pDevice->setSamplerState(ms_pSamplerPointClamp, 0);
	m_pDevice->setSamplerState(ms_pSamplerLinearClamp, 1);
	m_pDevice->setSamplerState(ms_pSamplerPointWrap, 2);
	
	m_pDevice->setTexture(pGBufferDepth);
	m_pDevice->setTexture(m_pDepthMap, 1);
	m_pDevice->setTexture(ms_pRandomTexture, 2);

	m_shaderData.ps.mMatrixTexture = SMMatrixTranspose(m_mView * m_mProj * m_mScaleBiasMat);
	m_shaderData.ps.vPixelMapSizeBias = float3(m_fBlurPixel / m_fSize, m_fSize, m_fBias);
	m_pShaderDataPS->update(&m_shaderData.ps);
	m_pDevice->setPixelShaderConstant(m_pShaderDataPS, 6);
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

	m_pDevice->setColorTarget(pBackBuf);

	mem_release(pRenderSurf);
	mem_release(pBackBuf);

	if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_shadow.dds", pShadowMap);
	}
}

void CShadowMap::genLPV()
{
	m_pDevice->setRenderBuffer(NULL);
	m_pDevice->setIndexBuffer(NULL);
	m_pDevice->setPrimitiveTopology(GXPT_POINTLIST);
	SGCore_ShaderBind(ms_idInjectShader);

	m_pDevice->setTextureVS(m_pDepthMap);
	m_pDevice->setTextureVS(m_pNormalMap, 1);
	m_pDevice->setTextureVS(m_pFluxMap, 2);

	m_pShaderDataInjectVS->update(&SMMatrixTranspose(SMMatrixInverse(NULL, m_mView * m_mProj)));
	m_pDevice->setVertexShaderConstant(m_pShaderDataInjectVS, 6);

	IGXConstantBuffer *pLightConstant = m_pLight->getConstants(m_pDevice);
	m_pDevice->setVertexShaderConstant(pLightConstant, 7);
	mem_release(pLightConstant);

	m_pDevice->drawPrimitive(0, 256 * 256);

	SGCore_ShaderUnBind();
	m_pDevice->setPrimitiveTopology(GXPT_TRIANGLELIST);

	m_pDevice->setTextureVS(NULL);
	m_pDevice->setTextureVS(NULL, 1);
	m_pDevice->setTextureVS(NULL, 2);
}

//##########################################################################

CShadowCubeMap::CShadowCubeMap()
{
}

CShadowCubeMap::~CShadowCubeMap()
{
	mem_release(m_pShaderDataPS);
	mem_release(m_pShaderDataGS);
	mem_release(m_pDepthMap);
	mem_release(m_pNormalMap);
	mem_release(m_pFluxMap);
	ReleaseDepthStencilSurface();
}

IGXDepthStencilSurface *CShadowCubeMap::ms_pDepthStencilSurface = NULL;
UINT CShadowCubeMap::ms_uDepthStencilSurfaceRefCount = 0;
IGXSamplerState *CShadowCubeMap::ms_pSamplerPointWrap = NULL;
IGXSamplerState *CShadowCubeMap::ms_pSamplerPointClamp = NULL;
IGXSamplerState *CShadowCubeMap::ms_pSamplerLinearClamp = NULL;
ID CShadowCubeMap::ms_idShader = -1;

void CShadowCubeMap::InitDepthStencilSurface(IGXContext *pContext, UINT uSize)
{
	if(!ms_pDepthStencilSurface)
	{
		ms_pDepthStencilSurface = pContext->createDepthStencilSurfaceCube(uSize, GXFMT_D24S8, GXMULTISAMPLE_NONE, false);

		GXSAMPLER_DESC samplerDesc;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointWrap = pContext->createSamplerState(&samplerDesc);

		samplerDesc.addressU = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressV = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
		ms_pSamplerPointClamp = pContext->createSamplerState(&samplerDesc);

		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
		ms_pSamplerLinearClamp = pContext->createSamplerState(&samplerDesc);

		ID idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs");
		GXMACRO Defines_GSC_6[] = {{"GSC_6", ""}, {0, 0}};
		ID idGenShadowPoint6 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point_6.ps", Defines_GSC_6);
		ms_idShader = SGCore_ShaderCreateKit(idResPosDepth, idGenShadowPoint6);
	}
	++ms_uDepthStencilSurfaceRefCount;
}
void CShadowCubeMap::ReleaseDepthStencilSurface()
{
	if(--ms_uDepthStencilSurfaceRefCount == 0)
	{
		mem_release(ms_pDepthStencilSurface);
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

void CShadowCubeMap::init(IGXContext *pContext, UINT uSize)
{
	m_pDevice = pContext;

	//GXFMT_A8R8G8B8
	m_pNormalMap = m_pDevice->createTextureCube(uSize, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_A8R8G8B8
	m_pFluxMap = m_pDevice->createTextureCube(uSize, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_A8R8G8B8);
	//GXFMT_R32F
	m_pDepthMap = m_pDevice->createTextureCube(uSize, 1, GX_TEXUSAGE_RENDERTARGET, GXFMT_R32F);

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
	m_pShaderDataGS = m_pDevice->createConstantBuffer(sizeof(m_shaderData.gs));
}

void CShadowCubeMap::setLight(IXLight *pLight)
{
	m_pLight = pLight;
}

void CShadowCubeMap::process(IXRenderPipeline *pRenderPipeline)
{
	assert(m_pLight && m_pLight->getType() == LIGHT_TYPE_POINT);
	IXLightPoint *pPointLight = m_pLight->asPoint();

	float3 vPos = pPointLight->getPosition();

	static const float3 sc_vOrientedCube[6] = {float3(1, 0, 0), float3(-1, 0, 0), float3(0, 1, 0), float3(0, -1, 0), float3(0, 0, 1), float3(0, 0, -1)};
	static const float3 sc_vUpVectorsCube[6] = {float3(0, 1, 0), float3(0, 1, 0), float3(0, 0, -1), float3(0, 0, 1), float3(0, 1, 0), float3(0, 1, 0)};

	m_mProj = SMMatrixPerspectiveFovLH(SM_PIDIV2, 1.0f, 0.0025f, pPointLight->getMaxDistance());
	for(UINT i = 0; i < 6; ++i)
	{
		m_mView[i] = SMMatrixLookAtLH(vPos, vPos + sc_vOrientedCube[i], sc_vUpVectorsCube[i]);
		m_shaderData.gs.mVP[i] = SMMatrixTranspose(m_mView[i] * m_mProj);
	}
	m_pShaderDataGS->update(&m_shaderData.gs);
	
	m_pDevice->setDepthStencilSurface(ms_pDepthStencilSurface);

	m_pDevice->setBlendState(NULL);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &SMMatrixIdentity());

	IGXSurface *pDepthSurface = m_pDepthMap->asRenderTarget();
	IGXSurface *pNormalSurface = m_pNormalMap->asRenderTarget();
	IGXSurface *pFluxSurface = m_pFluxMap->asRenderTarget();

	m_pDevice->setColorTarget(pDepthSurface);
	m_pDevice->setColorTarget(NULL, 1);
	m_pDevice->setColorTarget(NULL, 2);
	m_pDevice->clear(GXCLEAR_COLOR | GXCLEAR_DEPTH | GXCLEAR_STENCIL, GXCOLOR_ARGB(255, 255, 255, 255));

	m_pDevice->setColorTarget(pFluxSurface, 0);
	m_pDevice->setColorTarget(pNormalSurface, 1);
	m_pDevice->clear(GXCLEAR_COLOR);

	m_pDevice->setColorTarget(pDepthSurface);
	m_pDevice->setColorTarget(pNormalSurface, 1);
	m_pDevice->setColorTarget(pFluxSurface, 2);

	mem_release(pDepthSurface);
	mem_release(pNormalSurface);
	mem_release(pFluxSurface);
	
	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	m_pDevice->setGeometryShaderConstant(m_pShaderDataGS, SCR_CAMERA);
	m_pDevice->setGeometryShaderConstant(pLightConstants, SCR_OBJECT);
	mem_release(pLightConstants);

	pRenderPipeline->renderStage(XRS_SHADOWS);

	m_pDevice->setColorTarget(NULL);
	m_pDevice->setColorTarget(NULL, 1);
	m_pDevice->setColorTarget(NULL, 2);

 	if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_depth.dds", m_pDepthMap);
		m_pDevice->saveTextureToFile("sm_normal.dds", m_pNormalMap);
		m_pDevice->saveTextureToFile("sm_flux.dds", m_pFluxMap);
	}
}

void CShadowCubeMap::genShadow(IGXTexture2D *pShadowMap, IGXTexture2D *pGBufferDepth)
{

	const float c_fTexWidth = (float)pShadowMap->getWidth();
	const float c_fTexHeight = (float)pShadowMap->getHeight();

	IGXSurface *pRenderSurf, *pBackBuf;
	pRenderSurf = pShadowMap->getMipmap(0);
	pBackBuf = m_pDevice->getColorTarget();
	m_pDevice->setColorTarget(pRenderSurf);

	m_pDevice->clear(GXCLEAR_COLOR);

	m_pDevice->setSamplerState(ms_pSamplerPointClamp, 0);
	m_pDevice->setSamplerState(ms_pSamplerLinearClamp, 1);

	m_pDevice->setTexture(pGBufferDepth);
	m_pDevice->setTexture(m_pDepthMap, 1);

	m_shaderData.ps.vPixelSizeBias = float2(m_fBlurPixel / m_fSize, m_fBias);
	m_pShaderDataPS->update(&m_shaderData.ps);
	m_pDevice->setPixelShaderConstant(m_pShaderDataPS, 6);

	IGXConstantBuffer *pLightConstants = m_pLight->getConstants(m_pDevice);
	m_pDevice->setPixelShaderConstant(pLightConstants);
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

	m_pDevice->setColorTarget(pBackBuf);

	mem_release(pRenderSurf);
	mem_release(pBackBuf);

	if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_shadow.dds", pShadowMap);
	}
}

