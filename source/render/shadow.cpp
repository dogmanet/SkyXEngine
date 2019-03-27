#include "shadow.h"

CShadowMap::CShadowMap()
{
}

CShadowMap::~CShadowMap()
{
	mem_release(m_pDepthMap);
	mem_release(m_pNormalMap);
	mem_release(m_pFluxMap);
	ReleaseDepthStencilSurface();
}

IGXDepthStencilSurface *CShadowMap::ms_pDepthStencilSurface = NULL;
UINT CShadowMap::ms_uDepthStencilSurfaceRefCount = 0;

void CShadowMap::InitDepthStencilSurface(IGXContext *pContext, UINT uSize)
{
	if(!ms_pDepthStencilSurface)
	{
		ms_pDepthStencilSurface = pContext->createDepthStencilSurface(uSize, uSize, GXFMT_D24S8, GXMULTISAMPLE_NONE, true);
	}
	++ms_uDepthStencilSurfaceRefCount;
}
void CShadowMap::ReleaseDepthStencilSurface()
{
	if(--ms_uDepthStencilSurfaceRefCount == 0)
	{
		mem_release(ms_pDepthStencilSurface);
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
	

	pRenderPipeline->renderStage(XRS_SHADOWS);

	m_pDevice->setColorTarget(NULL);
	m_pDevice->setColorTarget(NULL, 1);
	m_pDevice->setColorTarget(NULL, 2);

	if(GetAsyncKeyState('U'))
	{
	//	m_pDevice->setColorTarget(NULL);
	//	SGCore_ScreenQuadDraw();
		m_pDevice->saveTextureToFile("sm_depth.dds", m_pDepthMap);
		m_pDevice->saveTextureToFile("sm_normal.dds", m_pNormalMap);
		m_pDevice->saveTextureToFile("sm_flux.dds", m_pFluxMap);
	}
}

void CShadowMap::genShadow(IGXTexture2D *pShadowMap)
{
#if 0
	const float c_fTexWidth = (float)pShadowMap->getWidth();
	const float c_fTexHeight = (float)pShadowMap->getHeight();

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	IGXSurface *RenderSurf, *BackBuf;
	RenderSurf = pShadowMap->getMipmap(0);
	BackBuf = light_data::pDXDevice->getColorTarget();
	light_data::pDXDevice->setColorTarget(RenderSurf);

	//light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	light_data::pDXDevice->setSamplerState(light_data::pSamplerPointClamp, 0);
	light_data::pDXDevice->setSamplerState(light_data::pSamplerLinearClamp, 1);
	light_data::pDXDevice->setSamplerState(light_data::pSamplerPointWrap, 2);
	
	light_data::pDXDevice->setTexture(SGCore_GbufferGetRT(DS_RT_DEPTH));

	float4x4 MatrixTexture;

	light_data::pDXDevice->setTexture(DepthMap, 1);

	light_data::pDXDevice->setTexture(SGCore_LoadTexGetTex(light_data::texture_id::idNoiseTex), 2);
	MatrixTexture = View * Proj * ScaleBiasMat;
	MatrixTexture = SMMatrixTranspose(MatrixTexture);


	if(light_data::isHalfGenPCFShadowLocal)
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect4, "g_mMatrixTexture", &MatrixTexture);
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowDirect4);
	}
	else
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "g_mMatrixTexture", &MatrixTexture);
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowDirect9);
	}

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vParamProj", &float3_t(c_fTexWidth, c_fTexHeight, (float)*r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vNearFar", &float2_t(*r_near, *r_far));

	float3 observerpos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &observerpos);

	if(light_data::isHalfGenPCFShadowLocal)
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect4, "g_vPosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect4, "g_vPixelSize", &float2(m_fBlurPixel / m_fSize, m_fBlurPixel / m_fSize));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect4, "g_vSizeMapBias", &float3(m_fSize, m_fSize, m_fBias));
	}
	else
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "g_vPosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "g_vPixelSize", &float2(m_fBlurPixel / m_fSize, m_fBlurPixel / m_fSize));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "g_vSizeMapBias", &float3(m_fSize, m_fSize, m_fBias));
	}

	float4x4 camview;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &camview);

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, camview);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_mViewInv", &ViewInv);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	light_data::pDXDevice->setColorTarget(BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
#endif
}

