#include "shadow.h"

CShadowMap::CShadowMap(IGXContext *pDevice):
	m_pDevice(pDevice)
{
}

CShadowMap::~CShadowMap()
{
	mem_release(m_pDepthMap);
	mem_release(m_pNormalMap);
	mem_release(m_pFluxMap);
}

UINT CShadowMap::GetMapMemory(UINT uSize)
{
	//GXFMT_R32F - shadow
	//GXFMT_A8R8G8B8 - xyz:color; w:???
	//GXFMT_A8R8G8B8 - xyz:normals; w:???

	return(uSize * uSize * 12);
}

void CShadowMap::init(UINT uSize)
{
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
}

void CShadowMap::process(IXRenderPipeline *pRenderPipeline)
{
	float3 upvec = float3(0, 1, 0);

	if(Direction.x == 0 && Direction.y == -1 && Direction.z == 0)
		upvec = SMVector3Transform(Direction, SMMatrixRotationZ(1.57f));

	View = SMMatrixLookAtLH(Position, Position + Direction, upvec);
	Proj = SMMatrixPerspectiveFovLH(AngleNearFar.x, m_fSize / m_fSize, AngleNearFar.y, AngleNearFar.z);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &View);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &Proj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(View * Proj));

	Frustum->update(&(View), &(Proj));

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSMDirect, "g_mWVP", &SMMatrixTranspose(View * Proj));
	SGCore_ShaderBind(light_data::shader_id::kit::idSMDepthGeomPSSMDirect);

	mem_release_del(DepthSurface);
	DepthSurface = DepthMap->getMipmap();

	light_data::pDXDevice->setColorTarget(DepthSurface);

	light_data::pDXDevice->clear(GXCLEAR_COLOR | GXCLEAR_DEPTH, GXCOLOR_ARGB(255, 255, 255, 255));


	pRenderPipeline->renderStage(XRS_SHADOWS);
}

void CShadowMap::genShadow(IGXTexture2D *pShadowMap)
{
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
}

