#include "RenderPipeline.h"

#include <core/sxcore.h>

#include "render_func.h"

#include "RenderableVisibility.h"

namespace gdata
{
	extern ICamera *pCamera;
};

CRenderPipeline::CRenderPipeline(IGXDevice *pDevice):
	m_pDevice(pDevice)
{
	// load renderables
	IXCore *pCore = Core_GetIXCore();
	IPluginManager *pPluginManager = pCore->getPluginManager();
	m_pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);

	IXRenderable *pRenderable;
	UINT ic = 0;
	while((pRenderable = (IXRenderable*)pPluginManager->getInterface(IXRENDERABLE_GUID, ic++)))
	{
		if(pRenderable->getVersion() == IXRENDERABLE_VERSION)
		{
			pRenderable->startup(m_pDevice, m_pMaterialSystem);
			m_apRenderables.push_back(pRenderable);

			X_RENDER_STAGE stages = pRenderable->getStages();

			for(UINT idx = 0; idx < sizeof(UINT) * 8 /* bits in byte */; ++idx)
			{
				X_RENDER_STAGE stage = (X_RENDER_STAGE)(1 << idx);

				if(stages & stage)
				{
					_render_sys rs;
					rs.pRenderable = pRenderable;
					rs.uPriority = pRenderable->getPriorityForStage(stage);
					rs.uRenderableId = m_apRenderables.size() - 1;

					m_apRenderStages[idx].aSystems.push_back(rs);
				}
			}
		}
	}

	for(UINT i = 0, l = m_apRenderStages.size(); i < l; ++i)
	{
		m_apRenderStages[i].aSystems.quickSort([](const _render_sys &a, const _render_sys &b)
		{
			return(a.uPriority < b.uPriority);
		});
	}

	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");
	static const bool *r_win_windowed = GET_PCVAR_BOOL("r_win_windowed");
	m_uOutWidth = *r_win_width;
	m_uOutHeight = *r_win_height;
	m_isWindowed = *r_win_windowed;

	m_pLightSystem = (IXLightSystem*)pPluginManager->getInterface(IXLIGHTSYSTEM_GUID);


	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferColor = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8R8G8B8);
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferNormals = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8R8G8B8);
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferParams = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8R8G8B8);
	//GXFMT_G32R32F; // 64bpp; GXFMT_R32F
	m_pGBufferDepth = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R32F);


	m_pLightAmbientDiffuse = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A16B16G16R16F);
	m_pLightSpecular = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R16F);

	m_pLightTotal = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A16B16G16R16F);
	
	m_pRefractiveTextures[0] = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A16B16G16R16F);
	m_pRefractiveTextures[1] = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A16B16G16R16F);
	
	m_pShadow = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R16F);
	//m_pShadow = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8R8G8B8);


	GXDepthStencilDesc dsDesc;

	dsDesc.useDepthWrite = FALSE;
	m_pDepthStencilStateNoZWrite = m_pDevice->createDepthStencilState(&dsDesc);

	dsDesc.useDepthTest = FALSE;
	m_pDepthStencilStateNoZ = m_pDevice->createDepthStencilState(&dsDesc);


	GXBlendDesc blendDesc;
	blendDesc.renderTarget[0].useBlend = true;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_SRC_ALPHA;
	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_SRC_ALPHA;
	m_pBlendStateAlpha = m_pDevice->createBlendState(&blendDesc);

	//m_pSceneShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_sceneShaderData));
	m_pSceneShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_sceneShaderData));

	m_pCameraShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_cameraShaderData.vs));

	m_pLightingShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_lightingShaderData.vs));
	m_pLightingShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_lightingShaderData.ps));

	{
		const UINT uSize = 32;
		m_uGICubesCount = uSize * uSize * uSize;
		float3_t *pData = new float3_t[m_uGICubesCount];
		float fHalf = (float)uSize / 2.0f;
		for(UINT x = 0; x < uSize; ++x)
		{
			for(UINT y = 0; y < uSize; ++y)
			{
				for(UINT z = 0; z < uSize; ++z)
				{
					pData[x + y * uSize + z * uSize * uSize] = (float3)(float3((float)x, (float)y, (float)z) - float3(fHalf));
				}
			}
		}		
		IGXVertexBuffer *pVB = m_pDevice->createVertexBuffer(sizeof(float3_t) * m_uGICubesCount, GXBUFFER_USAGE_STATIC, pData);
		mem_delete_a(pData);

		GXVertexElement oLayout[] =
		{
			{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
			GX_DECL_END()
		};

		IGXVertexDeclaration *pVD = m_pDevice->createVertexDeclaration(oLayout);
		m_pGICubesRB = m_pDevice->createRenderBuffer(1, &pVB, pVD);
		mem_release(pVD);
		mem_release(pVB);

		ID idVS = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "gi_cubes.vs");
		ID idPS = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "gi_cubes.ps");
		ID idGS = SGCore_ShaderLoad(SHADER_TYPE_GEOMETRY, "gi_cubes.gs");
		m_idGICubesShader = SGCore_ShaderCreateKit(idVS, idPS, idGS);
	}

//#define TIDX(x, y, z) (x + y * 32 + z * 32 * 32)
	m_pGIAccumRed = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
	m_pGIAccumGreen = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
	m_pGIAccumBlue = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);

	m_pGIAccumRed2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
	m_pGIAccumGreen2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
	m_pGIAccumBlue2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
//#undef TIDX

	m_idLightBoundShader = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "lighting_bound.vs"), -1);

	m_idLPVPropagateShader = SGCore_ShaderCreateKit(-1, -1, -1, SGCore_ShaderLoad(SHADER_TYPE_COMPUTE, "gi_propagation.cs"));

	m_pShadowCache = new CShadowCache(this, m_pMaterialSystem);
	m_pShadowShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_shadowShaderData.vs));

	
	newVisData(&m_pMainCameraVisibility);


	m_pTransparencyShaderClipPlanes = m_pDevice->createConstantBuffer(sizeof(float4) * MAX_TRANSPARENCY_CLIP_PANES);

	GXSamplerDesc sampDesc;
	sampDesc.addressU = sampDesc.addressV = sampDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
	sampDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
	m_pRefractionScene = m_pDevice->createSamplerState(&sampDesc);
}
CRenderPipeline::~CRenderPipeline()
{
	for(UINT i = 0, l = m_apRenderables.size(); i < l; ++i)
	{
		m_apRenderables[i]->shutdown();
	}

	mem_release(m_pRefractiveTextures[0]);
	mem_release(m_pRefractiveTextures[1]);

	mem_release(m_pRefractionScene);

	mem_release(m_pTransparencyShaderClipPlanes);

	mem_release(m_pMainCameraVisibility);

	//mem_release(m_pSceneShaderDataVS);
	mem_release(m_pSceneShaderDataPS);

	mem_release(m_pCameraShaderDataVS);

	mem_release(m_pLightingShaderDataVS);
	mem_release(m_pLightingShaderDataPS);

	mem_release(m_pGBufferColor);
	mem_release(m_pGBufferNormals);
	mem_release(m_pGBufferParams);
	mem_release(m_pGBufferDepth);

	mem_release(m_pLightAmbientDiffuse);
	mem_release(m_pLightSpecular);

	mem_release(m_pLightTotal);

	mem_release(m_pBlendStateAlpha);

	mem_release(m_pDepthStencilStateNoZWrite);
	mem_release(m_pDepthStencilStateNoZ);

	mem_release(m_pGICubesRB);

	mem_release(m_pGIAccumRed);
	mem_release(m_pGIAccumGreen);
	mem_release(m_pGIAccumBlue);

	mem_delete(m_pShadowCache);
	mem_release(m_pShadowShaderDataVS);
}

void CRenderPipeline::resize(UINT uWidth, UINT uHeight, bool isWindowed)
{
	m_pDevice->resize(uWidth, uHeight, isWindowed);
	m_uOutWidth = uWidth;
	m_uOutHeight = uHeight;
	m_isWindowed = isWindowed;
}

void CRenderPipeline::renderFrame()
{
	UINT timeDelta = 16;
	static const int *r_final_image = GET_PCVAR_INT("r_final_image");

	IGXContext *pCtx = m_pDevice->getDirectContext();

	m_sceneShaderData.vNearFarInvWinSize = float4(gdata::vNearFar, 1.0f / (float)m_uOutWidth, 1.0f / (float)m_uOutHeight);
	m_pSceneShaderDataPS->update(&m_sceneShaderData);
	//m_pDevice->setVertexShaderConstant(m_pSceneShaderDataVS, SCR_SCENE);
	pCtx->setPSConstant(m_pSceneShaderDataPS, SCR_SCENE);

	m_cameraShaderData.vs.mVP = SMMatrixTranspose(gdata::mCamView * gdata::mCamProj);
	m_cameraShaderData.vs.vPosCam = gdata::vConstCurrCamPos;
	m_pCameraShaderDataVS->update(&m_cameraShaderData.vs);
	pCtx->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	renderPrepare();

	Core_PStartSection(PERF_SECTION_MRT);
	renderGBuffer();
	Core_PEndSection(PERF_SECTION_MRT);

	switch(*r_final_image)
	{
	case DS_RT_COLOR:
		showTexture(m_pGBufferColor);
		goto end;
	case DS_RT_NORMAL:
		showTexture(m_pGBufferNormals);
		goto end;
	case DS_RT_PARAM:
		showTexture(m_pGBufferParams);
		goto end;
	case DS_RT_DEPTH:
		showTexture(m_pGBufferDepth);
		goto end;
	}

	renderGI();

	if(m_pLightSystem)
	{
		switch(*r_final_image)
		{
		case DS_RT_AMBIENTDIFF:
			showTexture(m_pLightAmbientDiffuse);
			goto end;
		case DS_RT_SPECULAR:
			showTexture(m_pLightSpecular);
			goto end;
		}
		
		m_pSceneTexture = m_pLightTotal;
	}
	else
	{
		m_pSceneTexture = m_pGBufferColor;
	}

	showTexture(m_pSceneTexture);

	renderPostprocessMain();
	renderTransparent();
	renderPostprocessFinal();

#if 0
	Core_PStartSection(PERF_SECTION_SHADOW_UPDATE);
	SRender_UpdateShadow(timeDelta);
	Core_PEndSection(PERF_SECTION_SHADOW_UPDATE);



	Core_PStartSection(PERF_SECTION_LIGHTING);
	SRender_ComLighting(timeDelta);

	if(SGCore_SkyBoxIsCr())
	{
		Core_PStartSection(PERF_SECTION_SKYBOX);
		SRender_RenderSky(timeDelta);
		Core_PEndSection(PERF_SECTION_SKYBOX);
	}

	Core_PStartSection(PERF_SECTION_TONEMAPPING);
	static const float * r_hdr_adapted_coef = GET_PCVAR_FLOAT("r_hdr_adapted_coef");
//	SGCore_ToneMappingCom(timeDelta, (r_hdr_adapted_coef ? (*r_hdr_adapted_coef) : 0.03f));
	Core_PEndSection(PERF_SECTION_TONEMAPPING);

	Core_PEndSection(PERF_SECTION_LIGHTING);

	SGCore_ShaderUnBind();
#endif

end:
	//showGICubes();
	showFrameStats();
}
void CRenderPipeline::endFrame()
{
	m_pDevice->swapBuffers();
}

void CRenderPipeline::updateVisibility()
{
	m_pMainCameraVisibility->updateForCamera(gdata::pCamera);

	if(m_pLightSystem)
	{
		float3 vCamPos;
		gdata::pCamera->getPosition(&vCamPos);
		m_pLightSystem->updateVisibility(gdata::pCamera, float3(-16.0f, -16.0f, -16.0f) + vCamPos, float3(16.0f, 16.0f, 16.0f) + vCamPos);
	}
}

void CRenderPipeline::renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility)
{
	CRenderableVisibility *pVis = NULL;
	if(pVisibility)
	{
		assert(((IXRenderableVisibility*)pVisibility)->getPluginId() == -1);
		pVis = (CRenderableVisibility*)pVisibility;
	}

	auto &list = m_apRenderStages[getIndexForStage(stage)].aSystems;

	for(UINT i = 0, l = list.size(); i < l; ++i)
	{
		list[i].pRenderable->renderStage(stage, pVis ? pVis->getVisibility(list[i].uRenderableId) : NULL);
	}
}

IGXDevice *CRenderPipeline::getDevice()
{
	return(m_pDevice);
}

void CRenderPipeline::newVisData(IXRenderableVisibility **ppVisibility)
{
	CRenderableVisibility *pVisibility = new CRenderableVisibility(-1, m_apRenderables.size());
	for(UINT i = 0, l = m_apRenderables.size(); i < l; ++i)
	{
		IXRenderableVisibility *pVis = NULL;
		m_apRenderables[i]->newVisData(&pVis);
		pVisibility->setVisibility(i, pVis);
	}
	*ppVisibility = pVisibility;
}

void CRenderPipeline::showGICubes()
{
	IGXContext *pCtx = m_pDevice->getDirectContext();

	pCtx->setPrimitiveTopology(GXPT_POINTLIST);
	pCtx->setRenderBuffer(m_pGICubesRB);
	SGCore_ShaderBind(m_idGICubesShader);
	pCtx->setGSConstant(m_pLightingShaderDataVS, 1);
	pCtx->setDepthStencilState(NULL);
	pCtx->setPSTexture(m_pGIAccumRed2, 0);
	pCtx->setPSTexture(m_pGIAccumGreen2, 1);
	pCtx->setPSTexture(m_pGIAccumBlue2, 2);
	pCtx->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);

	pCtx->drawPrimitive(0, m_uGICubesCount);

	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);

	pCtx->setPSTexture(NULL, 0);
	pCtx->setPSTexture(NULL, 1);
	pCtx->setPSTexture(NULL, 2);
}

void CRenderPipeline::renderPrepare()
{
	renderStage(XRS_PREPARE);
}
void CRenderPipeline::renderGBuffer()
{
	IGXContext *pCtx = m_pDevice->getDirectContext();

	pCtx->setRasterizerState(NULL);
	pCtx->setDepthStencilState(NULL);
	pCtx->setBlendState(NULL);
	rfunc::SetRenderSceneFilter();

	//?
	//Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_MATERIAL);

	IGXSurface *pBackBuf, *pColorSurf, *pNormalSurf, *pParamSurf, *pDepthMapLinearSurf;

	pColorSurf = m_pGBufferColor->getMipmap();
	pNormalSurf = m_pGBufferNormals->getMipmap();
	pParamSurf = m_pGBufferParams->getMipmap();
	pDepthMapLinearSurf = m_pGBufferDepth->getMipmap();
	pBackBuf = pCtx->getColorTarget();

	//очищаем рт глубины  максимальным значением
	//чтобы там где нет окружения к примеру был скайбокс, а значит в рт глубины было максимальное значение - максимальная отдаленность
	pCtx->setColorTarget(pDepthMapLinearSurf);
	pCtx->clear(GX_CLEAR_COLOR, GX_COLOR_ARGB(255, 255, 255, 255));

	pCtx->setColorTarget(pParamSurf);
	pCtx->setColorTarget(pNormalSurf, 1);
	pCtx->clear(GX_CLEAR_COLOR);
	pCtx->setColorTarget(NULL, 1);

	pCtx->setColorTarget(pColorSurf);
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, RENDER_DEFAUL_BACKGROUND_COLOR);

	pCtx->setColorTarget(pNormalSurf, 1);
	pCtx->setColorTarget(pParamSurf, 2);
	pCtx->setColorTarget(pDepthMapLinearSurf, 3);	//ставим рт глубины

	renderStage(XRS_GBUFFER, m_pMainCameraVisibility);
	//SXAnim_Render();
	//SXDecals_Render();

	pCtx->setColorTarget(NULL, 1);
	pCtx->setColorTarget(NULL, 2);
	pCtx->setColorTarget(NULL, 3);
	pCtx->setColorTarget(pBackBuf);
	
	mem_release(pBackBuf);
	mem_release(pColorSurf);
	mem_release(pNormalSurf);
	mem_release(pParamSurf);
	mem_release(pDepthMapLinearSurf);
}
void CRenderPipeline::renderShadows()
{

}
void CRenderPipeline::renderGI()
{
	if(!m_pLightSystem)
	{
		return;
	}

	IGXContext *pCtx = m_pDevice->getDirectContext();

	IGXDepthStencilSurface *pOldDSSurface = pCtx->getDepthStencilSurface();

	UINT uCounts[LIGHT_TYPE__COUNT] = {0};
	for(int i = 0, l = m_pLightSystem->getCount(); i < l; ++i)
	{
		++uCounts[m_pLightSystem->getLight(i)->getType()];
	}
	m_pShadowCache->setLightsCount(uCounts[LIGHT_TYPE_POINT], uCounts[LIGHT_TYPE_SPOT], uCounts[LIGHT_TYPE_SUN] != 0);

	m_pShadowCache->nextFrame();

	IGXSurface *pAmbientSurf, *pSpecDiffSurf, *pBackBuf;
	pAmbientSurf = m_pLightAmbientDiffuse->getMipmap();
	pSpecDiffSurf = m_pLightSpecular->getMipmap();

	pBackBuf = pCtx->getColorTarget();

	pCtx->setColorTarget(pAmbientSurf);
	pCtx->setColorTarget(pSpecDiffSurf, 1);

	//очищаем рт и стенсил
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_STENCIL);

	m_lightingShaderData.vs.mViewInv = SMMatrixTranspose(SMMatrixInverse(NULL, gdata::mCamView));
	m_lightingShaderData.vs.mVP = SMMatrixTranspose(gdata::mCamView * gdata::mCamProj);
	m_lightingShaderData.vs.vNearFar = gdata::vNearFar;
	m_lightingShaderData.vs.vParamProj = float3_t((float)m_uOutWidth, (float)m_uOutHeight, gdata::fProjFov);
	m_pLightingShaderDataVS->update(&m_lightingShaderData.vs);

	m_lightingShaderData.ps.vViewPos = gdata::vConstCurrCamPos;
	m_pLightingShaderDataPS->update(&m_lightingShaderData.ps.vViewPos);


	/*
	vs:
		cbuffer0: (per light)
		- g_mW
		cbuffer1: (per frame)
		- g_mVP
		- g_mViewInv
		- g_vNearFar
		- g_vParamProj

	ps:
		cbuffer0: (per light)
		- g_vLightPos
		- g_vLightColor
		- g_vLightPowerDistShadow
		cbuffer1: (per frame)
		- g_vViewPos


	{
		render shadowmaps
		render direct light with shadows
		inject VPLs into LPV grid
	}
	*/
	
	// Определим список лампочек, которые будут участвовать в текущем кадре
	IXLight *pLight;
	for(int i = 0, l = m_pLightSystem->getCount(); i < l; ++i)
	{
		pLight = m_pLightSystem->getLight(i);

		//если свет виден фрустуму камеры (это надо было заранее просчитать) и если свет включен
		if(pLight->isEnabled() && pLight->getRenderType() != LRT_NONE)
		{
			m_pShadowCache->addLight(pLight);
		}
	}

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");
	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	float4x4 mCamView;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mCamView);
	m_shadowShaderData.vs.mViewInv = SMMatrixTranspose(SMMatrixInverse(NULL, mCamView));
	m_shadowShaderData.vs.vNearFar = float2(*r_near, *r_far);
	m_shadowShaderData.vs.vParamProj = float3((float)m_uOutWidth, (float)m_uOutHeight, *r_default_fov);
	m_pShadowShaderDataVS->update(&m_shadowShaderData.vs);

	//@TODO: убрать это
	//Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &m_shadowShaderData.ps.vPosCam);
	//m_pShadowShaderDataPS->update(&m_shadowShaderData.ps);
	

	//m_pDevice->setPixelShaderConstant(m_pShadowShaderDataPS, 7);


	pCtx->setVSConstant(m_pCameraShaderDataVS, 8);
	pCtx->setPSConstant(m_pCameraShaderDataVS, 8);

	UINT uShadowCount = 0;
	while((uShadowCount = m_pShadowCache->processNextBunch()))
	{
		pCtx->setColorTarget(pAmbientSurf);
		pCtx->setColorTarget(pSpecDiffSurf, 1);
		pCtx->setDepthStencilSurface(pOldDSSurface);
		pCtx->setBlendState(gdata::rstates::pBlendAlphaOneOne);

		pCtx->setVSConstant(m_pLightingShaderDataVS, 1);
		pCtx->setPSConstant(m_pLightingShaderDataPS, 1);

		IBaseShadowMap *pShadow = NULL;

		//render direct light with shadows
		for(UINT i = 0; i < uShadowCount; ++i)
		{
			pLight = m_pShadowCache->getLight(i);
			pShadow = m_pShadowCache->getShadow(i);

			//пока что назначаем шейдер без теней
			//ID idshaderkit = pLight->getType() == LIGHT_TYPE_SPOT ? gdata::shaders_id::kit::idComLightingSpotNonShadow : gdata::shaders_id::kit::idComLightingNonShadow;

			//если не глобальный источник
			if(pLight->getType() != LIGHT_TYPE_SUN)
			{
				//помечаем в стенсил буфере пиксели  которые входят в ограничивающий объем света, чтобы их осветить
				pCtx->setRasterizerState(gdata::rstates::pRasterizerCullNone);
				pCtx->setStencilRef(0);
				pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateLightBound);

				//отрисовка ограничивающего объема
				SGCore_ShaderBind(m_idLightBoundShader);
				pLight->drawShape(pCtx);
				
				pCtx->setStencilRef(255);
				pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateLightShadowNonGlobal);
			}
			else
			{
				//иначе это глобальный источник, отключаем стенсил тест
				pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateLightShadowGlobal);
			}

			pCtx->setRasterizerState(NULL);



			pCtx->setVSConstant(m_pShadowShaderDataVS, 1);

			pCtx->setBlendState(gdata::rstates::pBlendRed);
			pShadow->genShadow(m_pShadow, m_pGBufferDepth, m_pGBufferNormals);
			pCtx->setBlendState(gdata::rstates::pBlendAlphaOneOne);
			pCtx->setColorTarget(pAmbientSurf);
			pCtx->setColorTarget(pSpecDiffSurf, 1);

			pCtx->setPSTexture(m_pShadow, 4);
			//idshaderkit = gdata::shaders_id::kit::idComLightingShadow;
			ID idshaderkit = pLight->getType() == LIGHT_TYPE_SPOT ? gdata::shaders_id::kit::idComLightingSpotShadow : gdata::shaders_id::kit::idComLightingShadow;

			pCtx->setVSConstant(m_pLightingShaderDataVS, 1);

#if 0
			static const int * r_shadow_soft = GET_PCVAR_INT("r_shadow_soft");

			if(r_shadow_soft)
			{
				if((*r_shadow_soft) == 1)
					SLight_ShadowSoft(false, 2);
				else if((*r_shadow_soft) == 2)
				{
					SLight_ShadowSoft(false, 2);
					SLight_ShadowSoft(false, 2);
				}
			}
#endif

			//теперь когда будем считать освещение надо сбросить значения в стенсил буфере, чтобы каждый кадр не чистить
			//если стенсил тест прошел успешно, устанавливаем значнеие в нуль
			if(pLight->getType() != LIGHT_TYPE_SUN)
			{
				pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateLightClear);
			}

			IGXConstantBuffer *pLightConstants = pLight->getConstants(pCtx);
			pCtx->setPSConstant(pLightConstants);
			mem_release(pLightConstants);

			SGCore_ShaderBind(idshaderkit);

			pCtx->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);
			
			pCtx->setPSTexture(m_pGBufferColor);
			pCtx->setPSTexture(m_pGBufferNormals, 1);
			pCtx->setPSTexture(m_pGBufferParams, 2);
			pCtx->setPSTexture(m_pGBufferDepth, 3);
			//m_pDevice->setTexture(SGCore_GbufferGetRT(DS_RT_ADAPTEDLUM), 5);

			SGCore_ScreenQuadDraw();
		}
		
		IGXSurface *pLPVRed = m_pGIAccumRed->asRenderTarget();
		IGXSurface *pLPVGreen = m_pGIAccumGreen->asRenderTarget();
		IGXSurface *pLPVBlue = m_pGIAccumBlue->asRenderTarget();

		//m_pDevice->setColorTarget(pAmbientSurf);
		pCtx->setColorTarget(pLPVRed);
		pCtx->setColorTarget(pLPVGreen, 1);
		pCtx->setColorTarget(pLPVBlue, 2);

		mem_release(pLPVRed);
		mem_release(pLPVGreen);
		mem_release(pLPVBlue);

		IGXDepthStencilSurface *pOldSurface = pCtx->getDepthStencilSurface();
		pCtx->unsetDepthStencilSurface();

		pCtx->clear(GX_CLEAR_COLOR);

		//inject VPLs into LPV grid
		for(UINT i = 0; i < uShadowCount; ++i)
		{
			pShadow = m_pShadowCache->getShadow(i);
			pShadow->genLPV();
		}

		pCtx->setColorTarget(NULL);
		pCtx->setColorTarget(NULL, 1);
		pCtx->setColorTarget(NULL, 2);

#if 0
		auto pTarget = m_pGBufferColor->asRenderTarget();
		m_pDevice->setColorTarget(pAmbientSurf);
		mem_release(pTarget);
		for(UINT i = 0; i < uShadowCount; ++i)
		{
			pShadow = m_pShadowCache->getShadow(i);
			pShadow->genLPV(true);
		}
		m_pDevice->setColorTarget(NULL);
#endif

		pCtx->setDepthStencilSurface(pOldSurface);
		mem_release(pOldSurface);

		break;
	}

	SGCore_ShaderUnBind();

	mem_release(pOldDSSurface);

	pCtx->setVSConstant(m_pLightingShaderDataVS, 1);
	pCtx->setPSConstant(m_pLightingShaderDataPS, 1);

	{
		SGCore_ShaderBind(m_idLPVPropagateShader);

		for(UINT i = 0; i < 16; ++i)
		{
			pCtx->setCSTexture(m_pGIAccumRed, 0);
			pCtx->setCSTexture(m_pGIAccumGreen, 1);
			pCtx->setCSTexture(m_pGIAccumBlue, 2);

			pCtx->setCSUnorderedAccessView(m_pGIAccumRed2, 0);
			pCtx->setCSUnorderedAccessView(m_pGIAccumGreen2, 1);
			pCtx->setCSUnorderedAccessView(m_pGIAccumBlue2, 2);

			pCtx->computeDispatch(2, 16, 32);

			pCtx->setCSUnorderedAccessView(NULL, 0);
			pCtx->setCSUnorderedAccessView(NULL, 1);
			pCtx->setCSUnorderedAccessView(NULL, 2);

			pCtx->setCSTexture(NULL, 0);
			pCtx->setCSTexture(NULL, 1);
			pCtx->setCSTexture(NULL, 2);




			pCtx->setCSTexture(m_pGIAccumRed2, 0);
			pCtx->setCSTexture(m_pGIAccumGreen2, 1);
			pCtx->setCSTexture(m_pGIAccumBlue2, 2);
			
			pCtx->setCSUnorderedAccessView(m_pGIAccumRed, 0);
			pCtx->setCSUnorderedAccessView(m_pGIAccumGreen, 1);
			pCtx->setCSUnorderedAccessView(m_pGIAccumBlue, 2);

			pCtx->computeDispatch(2, 16, 32);

			pCtx->setCSUnorderedAccessView(NULL, 0);
			pCtx->setCSUnorderedAccessView(NULL, 1);
			pCtx->setCSUnorderedAccessView(NULL, 2);
			
			pCtx->setCSTexture(NULL, 0);
			pCtx->setCSTexture(NULL, 1);
			pCtx->setCSTexture(NULL, 2);
		}

		SGCore_ShaderUnBind();
	}
	
	{
		pCtx->setColorTarget(pAmbientSurf);
		pCtx->setColorTarget(pSpecDiffSurf, 1);
		//gdata::pDXDevice->setRasterizerState(NULL);
		pCtx->setRasterizerState(gdata::rstates::pRasterizerCullNone);
		pCtx->setBlendState(gdata::rstates::pBlendAlphaOneOne);
		pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateLightShadowGlobal);

		pCtx->setPSConstant(m_pLightingShaderDataPS, 1);

		//ID idshader = gdata::shaders_id::ps::idComLightingGI;
		ID idshaderkit = gdata::shaders_id::kit::idComLightingGI;

		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "g_vViewPos", &gdata::vConstCurrCamPos);

		SGCore_ShaderBind(idshaderkit);

		pCtx->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);
		pCtx->setSamplerState(gdata::rstates::pSamplerLinearBorder, 1);

		pCtx->setPSTexture(m_pGBufferDepth);
		pCtx->setPSTexture(m_pGBufferNormals, 1);
		pCtx->setPSTexture(m_pGIAccumRed, 2);
		pCtx->setPSTexture(m_pGIAccumGreen, 3);
		pCtx->setPSTexture(m_pGIAccumBlue, 4);

		SGCore_ScreenQuadDraw();

		pCtx->setPSTexture(NULL, 2);
		pCtx->setPSTexture(NULL, 3);
		pCtx->setPSTexture(NULL, 4);

		SGCore_ShaderUnBind();
		pCtx->setSamplerState(gdata::rstates::pSamplerLinearClamp, 1);
	}

	pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateNoZ);
	pCtx->setRasterizerState(NULL);
	pCtx->setBlendState(NULL);
	
	pCtx->setColorTarget(NULL, 1);
	
	mem_release(pAmbientSurf);
	mem_release(pSpecDiffSurf);

	//-------------------------------

	//теперь необходимо все смешать чтобы получить итоговую освещенную картинку
	//{{
	
	pCtx->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);
	
	IGXSurface *pComLightSurf = m_pLightTotal->getMipmap();
	pCtx->setColorTarget(pComLightSurf);

	//очищаем рт (в старой версии было многопроходное смешивание)
	pCtx->clear(GX_CLEAR_COLOR);

	pCtx->setPSTexture(m_pGBufferColor);
	pCtx->setPSTexture(m_pLightAmbientDiffuse, 1);
	pCtx->setPSTexture(m_pLightSpecular, 2);
	pCtx->setPSTexture(m_pGBufferNormals, 3);
	//gdata::pDXDevice->setTexture(SGCore_GbufferGetRT(DS_RT_ADAPTEDLUM), 4);
	pCtx->setPSTexture(m_pGBufferParams, 5);

	SGCore_ShaderBind(gdata::shaders_id::kit::idBlendAmbientSpecDiffColor);

	SGCore_ScreenQuadDraw();

	mem_release(pComLightSurf);
	//}}

	SGCore_ShaderUnBind();

	pCtx->setColorTarget(pBackBuf);
	mem_release(pBackBuf);
}
void CRenderPipeline::renderPostprocessMain()
{
	renderStage(XRS_POSTPROCESS_MAIN);
}
void CRenderPipeline::renderTransparent()
{
	assert(m_pMainCameraVisibility && m_pMainCameraVisibility->getPluginId() == -1);
	CRenderableVisibility *pVis = (CRenderableVisibility*)m_pMainCameraVisibility;

	IGXContext *pCtx = m_pDevice->getDirectContext();

	pCtx->setDepthStencilState(m_pDepthStencilStateNoZWrite);
	pCtx->setBlendState(m_pBlendStateAlpha);
	pCtx->setSamplerState(m_pRefractionScene, 1);
	//m_pDevice->setTexture(m_pSceneTexture, 11);
	pCtx->setPSTexture(m_pGBufferDepth, 12);

	m_iRefractiveSource = -1;
	IGXSurface *pSceneTarget = m_pSceneTexture->asRenderTarget();
	pCtx->setColorTarget(pSceneTarget);
	mem_release(pSceneTarget);
	//renderStage(XRS_TRANSPARENT, m_pMainCameraVisibility);

	auto &list = m_apRenderStages[getIndexForStage(XRS_TRANSPARENT)].aSystems;

	Array<XTransparentNode> &aNodes = m_aTmpNodes;
	Array<XTransparentPSP> &aPSPs = m_aTmpPSPs;

	for(UINT i = 0, l = list.size(); i < l; ++i)
	{
		IXRenderableVisibility *pVisibility = pVis ? pVis->getVisibility(list[i].uRenderableId) : NULL;
		UINT uCount = list[i].pRenderable->getTransparentCount(pVisibility);
		for(UINT j = 0; j < uCount; ++j)
		{
			XTransparentObject obj;
			list[i].pRenderable->getTransparentObject(pVisibility, j, &obj);

			XTransparentNode tpNode;
			tpNode.obj = obj;
			tpNode.uRenderable = i;
			tpNode.uObjectID = j;
			tpNode.pVisibility = pVisibility;
			aNodes.push_back(tpNode);

			if(obj.hasPSP)
			{
				XTransparentPSP psp;
				psp.psp = obj.psp;
				psp.uNode = aNodes.size() - 1;
				aPSPs.push_back(psp);
			}
		}
	}

	UINT uTotalPlanes = 0;
	// Определение секущих плоскостей
	for(UINT i = 0, l = aPSPs.size(); i < l; ++i)
	{
		XTransparentPSP *pPSP = &aPSPs[i];
		XTransparentNode *pPSPNode = &aNodes[pPSP->uNode];

		for(UINT j = 0, jl = aNodes.size(); j < jl; ++j)
		{
			if(j == pPSP->uNode)
			{
				continue;
			}
			
			XTransparentNode *pNode = &aNodes[j];

			if(SMAABBIntersectAABB(pNode->obj.vMin, pNode->obj.vMax, pPSPNode->obj.vMin, pPSPNode->obj.vMax)
				&& SMPlaneIntersectAABB(pPSP->psp, pNode->obj.vMin, pNode->obj.vMax))
			{
				pPSP->useMe = true;
				++uTotalPlanes;
				break;
			}
		}
	}

	// Определение одинаковых плоскостей
	for(UINT i = 0, l = aPSPs.size(); i < l; ++i)
	{
		XTransparentPSP *pPSP = &aPSPs[i];
		if(pPSP->useMe)
		{
			for(UINT j = i + 1; j < l; ++j)
			{
				XTransparentPSP *pPSP2 = &aPSPs[j];
				if(pPSP2->useMe && SMPlaneEqualEpsilon(pPSP2->psp, pPSP->psp, 0.01f))
				{
					pPSP2->iBasePSP = (int)i;
					--uTotalPlanes;
				}
			}
		}
	}

	XTransparentBSPObject *pRootObject = NULL, *pTailObject = NULL;
	for(UINT i = 0, l = aNodes.size(); i < l; ++i)
	{
		XTransparentBSPObject *pObj = m_poolTransparencyBSPobjects.Alloc();
		pObj->uNode = i;
		if(!pRootObject)
		{
			pRootObject = pObj;
			pTailObject = pObj;
		}
		else
		{
			pTailObject->pNext = pObj;
			pTailObject = pObj;
		}
	}
	//m_poolTransparencyBSPobjects

	float3 vCamPos;
	gdata::pCamera->getPosition(&vCamPos);

	// Построение дерева
	XTransparentBSPNode *pRootNode = m_poolTransparencyBSPnodes.Alloc();
	buildTransparencyBSP(&aPSPs[0], aPSPs.size(), 0, pRootNode, pRootObject, &aNodes[0], vCamPos);
	
	//float4 vPlanes[MAX_TRANSPARENCY_CLIP_PANES];
	//vPlanes[0] = SMPlaneNormalize(SMPlaneTransform(SMPLANE(0.0f, 1.0f, 0.0f, -1.0f), gdata::mCamView * gdata::mCamProj));
	//vPlanes[0] /= fabsf(vPlanes[0].w);
	//m_pTransparencyShaderClipPlanes->update(vPlanes);

	// Отрисовка дерева
	pCtx->setPSConstant(m_pTransparencyShaderClipPlanes, 6);

	SMMATRIX mTransInvVP = SMMatrixTranspose(SMMatrixInverse(NULL, gdata::mCamView * gdata::mCamProj));
	UINT puPlanesStack[MAX_TRANSPARENCY_CLIP_PANES];
	renderTransparencyBSP(pRootNode, &aPSPs[0], &aNodes[0], &list[0], vCamPos, puPlanesStack, 0, mTransInvVP);

	m_poolTransparencyBSPobjects.clearFast();
	m_poolTransparencyBSPnodes.clearFast();
	m_poolTransparencyBSPsplitPlanes.clearFast();
	m_aTmpNodes.clearFast();
	m_aTmpPSPs.clearFast();

	pCtx->setBlendState(NULL);
	pCtx->setColorTarget(NULL);
	if(m_iRefractiveSource == -1)
	{
		showTexture(m_pSceneTexture);
	}
	else
	{
		showTexture(m_pRefractiveTextures[m_iRefractiveSource]);
	}
}

void CRenderPipeline::renderTransparencyBSP(XTransparentBSPNode *pNode, XTransparentPSP *pPSPs, XTransparentNode *pObjectNodes, _render_sys *pRenderSystems, const float3 &vCamPos, UINT *puPlanesStack, UINT uPlaneCount, const SMMATRIX &mTransInvVP)
{
	XTransparentBSPNode *pFirst, *pSecond;

	IGXContext *pCtx = m_pDevice->getDirectContext();

	bool isInFront = (pNode->iPSP >= 0) && SMVector3Dot(pPSPs[pNode->iPSP].psp, vCamPos) > -pPSPs[pNode->iPSP].psp.w;
	if(isInFront)
	{
		pFirst = pNode->pBack;
		pSecond = pNode->pFront;
	}
	else
	{
		pFirst = pNode->pFront;
		pSecond = pNode->pBack;
	}
	if(pNode->iPSP >= 0)
	{
		pPSPs[pNode->iPSP].isRenderFront = !isInFront;
		puPlanesStack[uPlaneCount++] = (UINT)pNode->iPSP;
	}
	if(pFirst)
	{
		renderTransparencyBSP(pFirst, pPSPs, pObjectNodes, pRenderSystems, vCamPos, puPlanesStack, uPlaneCount, mTransInvVP);
	}

	static float4 vPlanes[MAX_TRANSPARENCY_CLIP_PANES];
	XTransparentBSPObject *pObj = pNode->pObjects;
	bool isRefractiveSwapped = false;
	while(pObj)
	{
		XTransparentNode *pNode = &pObjectNodes[pObj->uNode];

		XTransparentNodeSP *pCurSP = pNode->pSplitPlanes;
		for(UINT i = 0; i < MAX_TRANSPARENCY_CLIP_PANES; ++i)
		{
			if(pCurSP)
			{
				bool isFound = false;
				while(!isFound && pCurSP)
				{
					for(UINT j = 0; j < uPlaneCount; ++j)
					{
						if(puPlanesStack[j] == (UINT)pCurSP->iPSP)
						{
							isFound = true;
							break;
						}
					}
					if(!isFound)
					{
						pCurSP = pCurSP->pNext;
					}
				}

				if(isFound)
				{
					SMPLANE plane = pPSPs[pCurSP->iPSP].psp;
					if(!pPSPs[pCurSP->iPSP].isRenderFront)
					{
						plane *= -1.0f;
					}
					vPlanes[i] = SMPlaneNormalize(SMPlaneTransformTI(plane, mTransInvVP));
					vPlanes[i] /= fabsf(vPlanes[i].w);

					pCurSP = pCurSP->pNext;
					continue;
				}
			}
				
			vPlanes[i] = SMPLANE(0, 0, 0, 1.0f);
		}
		m_pTransparencyShaderClipPlanes->update(vPlanes);

		if(!isRefractiveSwapped && pNode->obj.pMaterial && pNode->obj.pMaterial->isRefractive())
		{
			if(m_iRefractiveSource == -1)
			{
				m_pRefractiveTextureRead = m_pSceneTexture;
				++m_iRefractiveSource;
				m_pRefractiveTextureWrite = m_pRefractiveTextures[m_iRefractiveSource];
			}
			else
			{
				m_pRefractiveTextureRead = m_pRefractiveTextureWrite;
				m_iRefractiveSource = (m_iRefractiveSource + 1) & 1;
				m_pRefractiveTextureWrite = m_pRefractiveTextures[m_iRefractiveSource];
			}
			// swap render targets
			pCtx->setPSTexture(m_pRefractiveTextureRead, 11);

			IGXSurface *pSceneTarget = m_pRefractiveTextureWrite->asRenderTarget();
			pCtx->setColorTarget(pSceneTarget);
			mem_release(pSceneTarget);

			pCtx->setBlendState(NULL);
			showTexture(m_pRefractiveTextureRead);
			pCtx->setBlendState(m_pBlendStateAlpha);

			isRefractiveSwapped = true;
		}

		pRenderSystems[pNode->uRenderable].pRenderable->renderTransparentObject(pNode->pVisibility, pNode->uObjectID, 0);
		pObj = pObj->pNext;
	}

	if(pNode->iPSP >= 0)
	{
		pPSPs[pNode->iPSP].isRenderFront = isInFront;
	}
	if(pSecond)
	{
		renderTransparencyBSP(pSecond, pPSPs, pObjectNodes, pRenderSystems, vCamPos, puPlanesStack, uPlaneCount, mTransInvVP);
	}
}

void CRenderPipeline::buildTransparencyBSP(XTransparentPSP *pPSPs, UINT uPSPcount, UINT uStartPSP, XTransparentBSPNode *pBSPNode, XTransparentBSPObject *pObjects, XTransparentNode *pObjectNodes, const float3 &vCamPos)
{
	XTransparentPSP *pPSP;
	UINT uPSPIndex = uStartPSP;

	XTransparentBSPObject *pRootObject = NULL, *pTailObject = NULL;

	do
	{
		pPSP = NULL;
		// Выберем плоскость для текущего рассечения
		for(; uPSPIndex < uPSPcount; ++uPSPIndex)
		{
			if(pPSPs[uPSPIndex].useMe && pPSPs[uPSPIndex].iBasePSP < 0)
			{
				pPSP = &pPSPs[uPSPIndex];
				pBSPNode->iPSP = (int)uPSPIndex;
				break;
			}
		}
		if(!pPSP)
		{
			XTransparentBSPObject *pCurObj = pObjects, *pNextObj;
			XTransparentNode *pNode;
			while(pCurObj)
			{
				pNextObj = pCurObj->pNext;
				pCurObj->pNext = NULL;

				pNode = &pObjectNodes[pCurObj->uNode];
				pCurObj->fCamDist = SMDistancePointAABB(vCamPos, pNode->obj.vMin, pNode->obj.vMax);
				


				XTransparentBSPObject *pTmpCur = pBSPNode->pObjects, *pTmpPrev = NULL;
				if(pTmpCur)
				{
					bool isFound = false;
					while(pTmpCur)
					{
						//pSplitPlane->pNext = pNode->pSplitPlanes;

						if(pTmpCur->fCamDist < pCurObj->fCamDist)
						{
							pCurObj->pNext = pTmpCur;

							if(pTmpPrev)
							{
								pTmpPrev->pNext = pCurObj;
							}
							else
							{
								pBSPNode->pObjects = pCurObj;
							}
							isFound = true;
							break;
						}

						pTmpPrev = pTmpCur;
						pTmpCur = pTmpCur->pNext;
					}

					if(!isFound)
					{
						if(pTmpPrev)
						{
							pTmpPrev->pNext = pCurObj;
						}
						else
						{
							pBSPNode->pObjects = pCurObj;
						}
					}
				}
				else
				{
					pBSPNode->pObjects = pCurObj;
				}



				pCurObj = pNextObj;
			}

			//pBSPNode->pObjects = pObjects;
			return;
		}

		// Найти объекты узла (совпадающие с плоскостью)
		// Для каждого оставшегося объекта:
		//     Если объект пересекает плоскость: то
		//         Если объект пересекается с одним из объектов, порождающих PSP: то добавляем плоскость рассечения в него, помещаем объект в оба новых списка
		//         Иначе: Помещаем объект в узел
		//     Иначе: Помещаем объект в правый или левый список, в зависимости от расположения относительно плоскости
		// Повторяем рекурсивно для правого и левого поддерева

		// Найти объекты узла (совпадающие с плоскостью)
		for(UINT i = 0; i < uPSPcount; ++i)
		{
			if(pPSPs[i].iBasePSP == pBSPNode->iPSP || i == pBSPNode->iPSP)
			{
				XTransparentBSPObject *pCur = pObjects, *pPrev = NULL;
				while(pCur)
				{
					if(pCur->uNode == pPSPs[i].uNode)
					{
						if(pPrev)
						{
							pPrev->pNext = pCur->pNext;
						}
						else
						{
							pObjects = pCur->pNext;
						}
						pCur->pNext = NULL;
						if(!pRootObject)
						{
							pRootObject = pTailObject = pCur;
						}
						else
						{
							pTailObject->pNext = pCur;
							pTailObject = pCur;
						}

						break;
					}

					pPrev = pCur;
					pCur = pCur->pNext;
				}

			}
		}
		++uPSPIndex;
	}
	while(!pRootObject);

	XTransparentBSPObject *pFrontRootObject = NULL, *pFrontTailObject = NULL;
	XTransparentBSPObject *pBackRootObject = NULL, *pBackTailObject = NULL;

	// Для каждого оставшегося объекта:
	XTransparentBSPObject *pCur = pObjects, *pNext;
	while(pCur)
	{
		pNext = pCur->pNext;

		XTransparentNode *pNode = &pObjectNodes[pCur->uNode];
		if(SMPlaneIntersectAABB(pPSP->psp, pNode->obj.vMin, pNode->obj.vMax))
		{
			bool isIntersects = false;
			for(UINT i = 0; i < uPSPcount; ++i)
			{
				XTransparentNode *pPSPNode = &pObjectNodes[pPSPs[i].uNode];
				if(pPSPs[i].useMe && SMAABBIntersectAABB(pNode->obj.vMin, pNode->obj.vMax, pPSPNode->obj.vMin, pPSPNode->obj.vMax))
				{
					isIntersects = true;
					break;
				}
			}

			pCur->pNext = NULL;

			if(isIntersects)
			{
				// добавляем плоскость рассечения pPSP в pCur

				XTransparentNodeSP *pSplitPlane = m_poolTransparencyBSPsplitPlanes.Alloc();
				pSplitPlane->fCamDist = SMDistancePointAABB(vCamPos, pNode->obj.vMin, pNode->obj.vMax);
				pSplitPlane->iPSP = pBSPNode->iPSP;

				XTransparentNodeSP *pTmpCur = pNode->pSplitPlanes, *pTmpPrev = NULL;
				if(pTmpCur)
				{
					bool isFound = false;
					while(pTmpCur)
					{
						//pSplitPlane->pNext = pNode->pSplitPlanes;

						if(pTmpCur->fCamDist < pSplitPlane->fCamDist)
						{
							pSplitPlane->pNext = pTmpCur;

							if(pTmpPrev)
							{
								//!!!
								pTmpPrev->pNext = pSplitPlane;
							}
							else
							{
								pNode->pSplitPlanes = pSplitPlane;
							}
							isFound = true;
							break;
						}

						pTmpPrev = pTmpCur;
						pTmpCur = pTmpCur->pNext;
					}

					if(!isFound)
					{
						if(pTmpPrev)
						{
							pTmpPrev->pNext = pSplitPlane;
						}
						else
						{
							pNode->pSplitPlanes = pSplitPlane;
						}
					}
				}
				else
				{
					pNode->pSplitPlanes = pSplitPlane;
				}

				++pNode->uSplitPlanes;


				if(!pFrontRootObject)
				{
					pFrontRootObject = pFrontTailObject = pCur;
				}
				else
				{
					pFrontTailObject->pNext = pCur;
					pFrontTailObject = pCur;
				}
				XTransparentBSPObject *pClone = m_poolTransparencyBSPobjects.Alloc();
				pClone->uNode = pCur->uNode;
				if(!pBackRootObject)
				{
					pBackRootObject = pBackTailObject = pClone;
				}
				else
				{
					pBackTailObject->pNext = pClone;
					pBackTailObject = pClone;
				}
			}
			else
			{
				if(!pRootObject)
				{
					pRootObject = pTailObject = pCur;
				}
				else
				{
					pTailObject->pNext = pCur;
					pTailObject = pCur;
				}
			}

		}
		else
		{
			pCur->pNext = NULL;

			//front
			if(SMVector3Dot(pNode->obj.vMin, pPSP->psp) > -pPSP->psp.w)
			{
				if(!pFrontRootObject)
				{
					pFrontRootObject = pFrontTailObject = pCur;
				}
				else
				{
					pFrontTailObject->pNext = pCur;
					pFrontTailObject = pCur;
				}
			}
			else // back
			{
				if(!pBackRootObject)
				{
					pBackRootObject = pBackTailObject = pCur;
				}
				else
				{
					pBackTailObject->pNext = pCur;
					pBackTailObject = pCur;
				}
			}

		}

		pCur = pNext;
	}

	if(pFrontRootObject)
	{
		XTransparentBSPNode *pFrontNode = m_poolTransparencyBSPnodes.Alloc();
		buildTransparencyBSP(pPSPs, uPSPcount, pBSPNode->iPSP + 1, pFrontNode, pFrontRootObject, pObjectNodes, vCamPos);
		if(pFrontNode->pObjects)
		{
			pBSPNode->pFront = pFrontNode;
		}
		else
		{
			m_poolTransparencyBSPnodes.Delete(pFrontNode);
		}
	}
	if(pBackRootObject)
	{
		XTransparentBSPNode *pBackNode = m_poolTransparencyBSPnodes.Alloc();
		buildTransparencyBSP(pPSPs, uPSPcount, pBSPNode->iPSP + 1, pBackNode, pBackRootObject, pObjectNodes, vCamPos);
		pBSPNode->pBack = pBackNode;
		if(pBackNode->pObjects)
		{
			pBSPNode->pBack = pBackNode;
		}
		else
		{
			m_poolTransparencyBSPnodes.Delete(pBackNode);
		}
	}
	pBSPNode->pObjects = pRootObject;
}

void CRenderPipeline::renderPostprocessFinal()
{
	Core_PStartSection(PERF_SECTION_RENDER_INFO);
	//@FIXME: пока так
	SGame_RenderHUD();
	Core_PEndSection(PERF_SECTION_RENDER_INFO);
}
void CRenderPipeline::renderEditor2D()
{
	SMMATRIX mVP;
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &mVP);
	float3 vCamPos;
	SRender_GetCamera()->getPosition(&vCamPos);

	m_cameraShaderData.vs.mVP = SMMatrixTranspose(mVP);
	m_cameraShaderData.vs.vPosCam = vCamPos;
	m_pCameraShaderDataVS->update(&m_cameraShaderData.vs);
	m_pDevice->getDirectContext()->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	renderStage(XRS_EDITOR_2D);
}

//##########################################################################

UINT CRenderPipeline::getIndexForStage(X_RENDER_STAGE stage)
{
	UINT idx = 0;
	UINT stg = stage;
	while(stg >>= 1)
	{
		idx++;
	}
	return(idx);
}

void CRenderPipeline::showTexture(IGXTexture2D *pTexture)
{
	IGXContext *pCtx = m_pDevice->getDirectContext();

	IGXDepthStencilState *pOldState = pCtx->getDepthStencilState();
	pCtx->setDepthStencilState(m_pDepthStencilStateNoZ);
	
	pCtx->setPSTexture(pTexture);
	SGCore_ShaderBind(gdata::shaders_id::kit::idScreenOut);
	SGCore_ScreenQuadDraw();

	pCtx->setDepthStencilState(pOldState);
	mem_release(pOldState);
}

void CRenderPipeline::showFrameStats()
{
	Core_PStartSection(PERF_SECTION_RENDER_INFO);
	//@FIXME: Пока так
	SGame_Render();
	Core_PEndSection(PERF_SECTION_RENDER_INFO);
}
