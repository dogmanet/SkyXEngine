#include "RenderPipeline.h"

#include <core/sxcore.h>

#include "render_func.h"

CRenderPipeline::CRenderPipeline(IGXContext *pDevice):
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
			X_RENDER_STAGE stages = pRenderable->getStages();

			for(UINT idx = 0; idx < sizeof(UINT) * 8; ++idx)
			{
				X_RENDER_STAGE stage = (X_RENDER_STAGE)(1 << idx);

				if(stages & stage)
				{
					_render_sys rs;
					rs.pRenderable = pRenderable;
					rs.uPriority = pRenderable->getPriorityForStage(stage);

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
	m_pGBufferColor = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8);
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferNormals = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8);
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferParams = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8);
	//GXFMT_G32R32F; // 64bpp; GXFMT_R32F
	m_pGBufferDepth = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_R32F);


	m_pLightAmbientDiffuse = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A16B16G16R16F);
	m_pLightSpecular = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_R16F);

	m_pLightTotal = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A16B16G16R16F);
	
	
	m_pShadow = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_R16F);
	//m_pShadow = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8);


	GXDEPTH_STENCIL_DESC dsDesc;

	dsDesc.bDepthEnable = FALSE;
	dsDesc.bEnableDepthWrite = FALSE;
	m_pDepthStencilStateNoZ = m_pDevice->createDepthStencilState(&dsDesc);

	m_pSceneShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_sceneShaderData.vs));
	m_pSceneShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_sceneShaderData));

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
					pData[x + y * uSize + z * uSize * uSize] = (float3)(float3(x, y, z) - float3(fHalf));
				}
			}
		}		
		IGXVertexBuffer *pVB = m_pDevice->createVertexBuffer(sizeof(float3_t) * m_uGICubesCount, GX_BUFFER_USAGE_STATIC, pData);
		mem_delete_a(pData);

		GXVERTEXELEMENT oLayout[] =
		{
			{0, 0, GXDECLTYPE_FLOAT3, GXDECLUSAGE_POSITION},
			GXDECL_END()
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
	m_pGIAccumRed = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
	m_pGIAccumGreen = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
	m_pGIAccumBlue = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);

	m_pGIAccumRed2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
	m_pGIAccumGreen2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
	m_pGIAccumBlue2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
//#undef TIDX

	m_idLightBoundShader = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "lighting_bound.vs"), -1);

	m_idLPVPropagateShader = SGCore_ShaderCreateKit(-1, -1, -1, SGCore_ShaderLoad(SHADER_TYPE_COMPUTE, "gi_propagation.cs"));

	m_pShadowCache = new CShadowCache(this, m_pMaterialSystem);
	m_pShadowShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_shadowShaderData.vs));
}
CRenderPipeline::~CRenderPipeline()
{
	mem_release(m_pSceneShaderDataVS);
	mem_release(m_pSceneShaderDataPS);

	mem_release(m_pLightingShaderDataVS);
	mem_release(m_pLightingShaderDataPS);

	mem_release(m_pGBufferColor);
	mem_release(m_pGBufferNormals);
	mem_release(m_pGBufferParams);
	mem_release(m_pGBufferDepth);

	mem_release(m_pLightAmbientDiffuse);
	mem_release(m_pLightSpecular);

	mem_release(m_pLightTotal);

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

	// m_sceneShaderData.vNearFarLayers = 

	m_pDevice->setVertexShaderConstant(m_pSceneShaderDataVS, SCR_SCENE);
	m_pDevice->setPixelShaderConstant(m_pSceneShaderDataPS, SCR_SCENE);

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

	switch(*r_final_image)
	{
	case DS_RT_AMBIENTDIFF:
		showTexture(m_pLightAmbientDiffuse);
		goto end;
	case DS_RT_SPECULAR:
		showTexture(m_pLightSpecular);
		goto end;
	}

	showTexture(m_pLightTotal);

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
	showGICubes();
	showFrameStats();
}
void CRenderPipeline::endFrame()
{
	m_pDevice->swapBuffers();
}

void CRenderPipeline::renderStage(X_RENDER_STAGE stage)
{
	auto &list = m_apRenderStages[getIndexForStage(stage)].aSystems;

	for(UINT i = 0, l = list.size(); i < l; ++i)
	{
		list[i].pRenderable->renderStage(stage, NULL);
	}
}

IGXContext *CRenderPipeline::getDevice()
{
	return(m_pDevice);
}

void CRenderPipeline::showGICubes()
{
	m_pDevice->setPrimitiveTopology(GXPT_POINTLIST);
	m_pDevice->setRenderBuffer(m_pGICubesRB);
	SGCore_ShaderBind(m_idGICubesShader);
	m_pDevice->setGeometryShaderConstant(m_pLightingShaderDataVS, 1);
	m_pDevice->setDepthStencilState(NULL);
	m_pDevice->setTexture(m_pGIAccumRed2, 0);
	m_pDevice->setTexture(m_pGIAccumGreen2, 1);
	m_pDevice->setTexture(m_pGIAccumBlue2, 2);
	m_pDevice->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);

	m_pDevice->drawPrimitive(0, m_uGICubesCount);

	m_pDevice->setPrimitiveTopology(GXPT_TRIANGLELIST);

	m_pDevice->setTexture(NULL, 0);
	m_pDevice->setTexture(NULL, 1);
	m_pDevice->setTexture(NULL, 2);
}

void CRenderPipeline::renderPrepare()
{
	renderStage(XRS_PREPARE);
}
void CRenderPipeline::renderGBuffer()
{
	m_pDevice->setRasterizerState(NULL);
	m_pDevice->setDepthStencilState(NULL);
	m_pDevice->setBlendState(NULL);
	rfunc::SetRenderSceneFilter();

	//?
	//Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_MATERIAL);

	IGXSurface *pBackBuf, *pColorSurf, *pNormalSurf, *pParamSurf, *pDepthMapLinearSurf;

	pColorSurf = m_pGBufferColor->getMipmap();
	pNormalSurf = m_pGBufferNormals->getMipmap();
	pParamSurf = m_pGBufferParams->getMipmap();
	pDepthMapLinearSurf = m_pGBufferDepth->getMipmap();
	pBackBuf = gdata::pDXDevice->getColorTarget();

	//очищаем рт глубины  максимальным значением
	//чтобы там где нет окружения к примеру был скайбокс, а значит в рт глубины было максимальное значение - максимальная отдаленность
	m_pDevice->setColorTarget(pDepthMapLinearSurf);
	m_pDevice->clear(GXCLEAR_COLOR, GXCOLOR_ARGB(255, 255, 255, 255));

	m_pDevice->setColorTarget(pParamSurf);
	m_pDevice->setColorTarget(pNormalSurf, 1);
	m_pDevice->clear(GXCLEAR_COLOR);
	m_pDevice->setColorTarget(NULL, 1);

	m_pDevice->setColorTarget(pColorSurf);
	m_pDevice->clear(GXCLEAR_COLOR | GXCLEAR_DEPTH | GXCLEAR_STENCIL, RENDER_DEFAUL_BACKGROUND_COLOR);

	m_pDevice->setColorTarget(pNormalSurf, 1);
	m_pDevice->setColorTarget(pParamSurf, 2);
	m_pDevice->setColorTarget(pDepthMapLinearSurf, 3);	//ставим рт глубины

	renderStage(XRS_GBUFFER);
	//SXAnim_Render();
	//SXDecals_Render();

	m_pDevice->setColorTarget(NULL, 1);
	m_pDevice->setColorTarget(NULL, 2);
	m_pDevice->setColorTarget(NULL, 3);
	m_pDevice->setColorTarget(pBackBuf);
	
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
		showTexture(m_pGBufferColor);
		return;
	}

	IGXDepthStencilSurface *pOldDSSurface = m_pDevice->getDepthStencilSurface();

	UINT uCounts[LIGHT_TYPE__COUNT] = {0};
	for(int i = 0, l = m_pLightSystem->getCount(); i < l; ++i)
	{
		++uCounts[m_pLightSystem->getLight(i)->getType()];
	}
	m_pShadowCache->setLightsCount(uCounts[LIGHT_TYPE_POINT], uCounts[LIGHT_TYPE_SPOT], uCounts[LIGHT_TYPE_SUN]);

	m_pShadowCache->nextFrame();

	IGXSurface *pAmbientSurf, *pSpecDiffSurf, *pBackBuf;
	pAmbientSurf = m_pLightAmbientDiffuse->getMipmap();
	pSpecDiffSurf = m_pLightSpecular->getMipmap();

	pBackBuf = m_pDevice->getColorTarget();

	m_pDevice->setColorTarget(pAmbientSurf);
	m_pDevice->setColorTarget(pSpecDiffSurf, 1);

	//очищаем рт и стенсил
	m_pDevice->clear(GXCLEAR_COLOR | GXCLEAR_STENCIL);

	m_lightingShaderData.vs.mViewInv = SMMatrixTranspose(SMMatrixInverse(NULL, gdata::mCamView));
	m_lightingShaderData.vs.mVP = SMMatrixTranspose(gdata::mCamView * gdata::mCamProj);
	m_lightingShaderData.vs.vNearFar = gdata::vNearFar;
	m_lightingShaderData.vs.vParamProj = float3_t(m_uOutWidth, m_uOutHeight, gdata::fProjFov);
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
		if(/*SLight_GetVisibleForFrustum(i) && */pLight->isEnabled())
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

	UINT uShadowCount = 0;
	while((uShadowCount = m_pShadowCache->processNextBunch()))
	{
		m_pDevice->setColorTarget(pAmbientSurf);
		m_pDevice->setColorTarget(pSpecDiffSurf, 1);
		m_pDevice->setDepthStencilSurface(pOldDSSurface);
		m_pDevice->setBlendState(gdata::rstates::pBlendAlphaOneOne);

		m_pDevice->setVertexShaderConstant(m_pLightingShaderDataVS, 1);
		m_pDevice->setPixelShaderConstant(m_pLightingShaderDataPS, 1);

		IBaseShadowMap *pShadow = NULL;

		//render direct light with shadows
		for(UINT i = 0; i < uShadowCount; ++i)
		{
			pLight = m_pShadowCache->getLight(i);
			pShadow = m_pShadowCache->getShadow(i);

			//пока что назначаем шейдер без теней
			ID idshaderkit = pLight->getType() == LIGHT_TYPE_SPOT ? gdata::shaders_id::kit::idComLightingSpotNonShadow : gdata::shaders_id::kit::idComLightingNonShadow;

			//если не глобальный источник
			if(pLight->getType() != LIGHT_TYPE_SUN)
			{
				//помечаем в стенсил буфере пиксели  которые входят в ограничивающий объем света, чтобы их осветить
				m_pDevice->setRasterizerState(gdata::rstates::pRasterizerCullNone);
				m_pDevice->setStencilRef(0);
				m_pDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateLightBound);

				//отрисовка ограничивающего объема
				SGCore_ShaderBind(m_idLightBoundShader);
				pLight->drawShape(m_pDevice);
				
				m_pDevice->setStencilRef(255);
				m_pDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateLightShadowNonGlobal);
			}
			else
			{
				//иначе это глобальный источник, отключаем стенсил тест
				m_pDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateLightShadowGlobal);
			}

			m_pDevice->setRasterizerState(NULL);



			m_pDevice->setVertexShaderConstant(m_pShadowShaderDataVS, 1);

			gdata::pDXDevice->setBlendState(gdata::rstates::pBlendRed);
			pShadow->genShadow(m_pShadow, m_pGBufferDepth);
			gdata::pDXDevice->setBlendState(gdata::rstates::pBlendAlphaOneOne);
			gdata::pDXDevice->setColorTarget(pAmbientSurf);
			gdata::pDXDevice->setColorTarget(pSpecDiffSurf, 1);

			gdata::pDXDevice->setTexture(m_pShadow, 4);
			idshaderkit = gdata::shaders_id::kit::idComLightingShadow;
			idshaderkit = pLight->getType() == LIGHT_TYPE_SPOT ? gdata::shaders_id::kit::idComLightingSpotShadow : gdata::shaders_id::kit::idComLightingShadow;

			m_pDevice->setVertexShaderConstant(m_pLightingShaderDataVS, 1);

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
				gdata::pDXDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateLightClear);
			}

			IGXConstantBuffer *pLightConstants = pLight->getConstants(m_pDevice);
			m_pDevice->setPixelShaderConstant(pLightConstants);
			mem_release(pLightConstants);

			SGCore_ShaderBind(idshaderkit);

			m_pDevice->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);
			
			m_pDevice->setTexture(m_pGBufferColor);
			m_pDevice->setTexture(m_pGBufferNormals, 1);
			m_pDevice->setTexture(m_pGBufferParams, 2);
			m_pDevice->setTexture(m_pGBufferDepth, 3);
			//m_pDevice->setTexture(SGCore_GbufferGetRT(DS_RT_ADAPTEDLUM), 5);

			SGCore_ScreenQuadDraw();
		}


		IGXSurface *pLPVRed = m_pGIAccumRed->asRenderTarget();
		IGXSurface *pLPVGreen = m_pGIAccumGreen->asRenderTarget();
		IGXSurface *pLPVBlue = m_pGIAccumBlue->asRenderTarget();

		//m_pDevice->setColorTarget(pAmbientSurf);
		m_pDevice->setColorTarget(pLPVRed);
		m_pDevice->setColorTarget(pLPVGreen, 1);
		m_pDevice->setColorTarget(pLPVBlue, 2);

		mem_release(pLPVRed);
		mem_release(pLPVGreen);
		mem_release(pLPVBlue);

		IGXDepthStencilSurface *pOldSurface = m_pDevice->getDepthStencilSurface();
		m_pDevice->setDepthStencilSurfaceNULL();

		m_pDevice->clear(GXCLEAR_COLOR);

		//inject VPLs into LPV grid
		for(UINT i = 0; i < uShadowCount; ++i)
		{
			pShadow = m_pShadowCache->getShadow(i);
			pShadow->genLPV();
		}

		m_pDevice->setDepthStencilSurface(pOldSurface);
		mem_release(pOldSurface);

		m_pDevice->setColorTarget(NULL);
		m_pDevice->setColorTarget(NULL, 1);
		m_pDevice->setColorTarget(NULL, 2);

		break;
	}

	SGCore_ShaderUnBind();

	mem_release(pOldDSSurface);

	{
		SGCore_ShaderBind(m_idLPVPropagateShader);

		for(UINT i = 0; i < 16; ++i)
		{
			m_pDevice->setTextureCS(m_pGIAccumRed, 0);
			m_pDevice->setTextureCS(m_pGIAccumGreen, 1);
			m_pDevice->setTextureCS(m_pGIAccumBlue, 2);

			m_pDevice->setUnorderedAccessVeiwCS(m_pGIAccumRed2, 0);
			m_pDevice->setUnorderedAccessVeiwCS(m_pGIAccumGreen2, 1);
			m_pDevice->setUnorderedAccessVeiwCS(m_pGIAccumBlue2, 2);

			m_pDevice->computeDispatch(2, 16, 32);

			m_pDevice->setUnorderedAccessVeiwCS(NULL, 0);
			m_pDevice->setUnorderedAccessVeiwCS(NULL, 1);
			m_pDevice->setUnorderedAccessVeiwCS(NULL, 2);

			m_pDevice->setTextureCS(NULL, 0);
			m_pDevice->setTextureCS(NULL, 1);
			m_pDevice->setTextureCS(NULL, 2);




			m_pDevice->setTextureCS(m_pGIAccumRed2, 0);
			m_pDevice->setTextureCS(m_pGIAccumGreen2, 1);
			m_pDevice->setTextureCS(m_pGIAccumBlue2, 2);

			m_pDevice->setUnorderedAccessVeiwCS(m_pGIAccumRed, 0);
			m_pDevice->setUnorderedAccessVeiwCS(m_pGIAccumGreen, 1);
			m_pDevice->setUnorderedAccessVeiwCS(m_pGIAccumBlue, 2);

			m_pDevice->computeDispatch(2, 16, 32);

			m_pDevice->setUnorderedAccessVeiwCS(NULL, 0);
			m_pDevice->setUnorderedAccessVeiwCS(NULL, 1);
			m_pDevice->setUnorderedAccessVeiwCS(NULL, 2);

			m_pDevice->setTextureCS(NULL, 0);
			m_pDevice->setTextureCS(NULL, 1);
			m_pDevice->setTextureCS(NULL, 2);
		}

		SGCore_ShaderUnBind();
	}
	
	{
		m_pDevice->setColorTarget(pAmbientSurf);
		m_pDevice->setColorTarget(pSpecDiffSurf, 1);
		//gdata::pDXDevice->setRasterizerState(NULL);
		gdata::pDXDevice->setRasterizerState(gdata::rstates::pRasterizerCullNone);
		gdata::pDXDevice->setBlendState(gdata::rstates::pBlendAlphaOneOne);
		gdata::pDXDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateLightShadowGlobal);

		m_pDevice->setPixelShaderConstant(m_pLightingShaderDataPS, 1);

		//ID idshader = gdata::shaders_id::ps::idComLightingGI;
		ID idshaderkit = gdata::shaders_id::kit::idComLightingGI;

		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "g_vViewPos", &gdata::vConstCurrCamPos);

		SGCore_ShaderBind(idshaderkit);

		m_pDevice->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);
		m_pDevice->setSamplerState(gdata::rstates::pSamplerLinearClamp, 1);

		gdata::pDXDevice->setTexture(m_pGBufferDepth);
		gdata::pDXDevice->setTexture(m_pGBufferNormals, 1);
		gdata::pDXDevice->setTexture(m_pGIAccumRed2, 2);
		gdata::pDXDevice->setTexture(m_pGIAccumGreen2, 3);
		gdata::pDXDevice->setTexture(m_pGIAccumBlue2, 4);

		SGCore_ScreenQuadDraw();

		gdata::pDXDevice->setTexture(NULL, 2);
		gdata::pDXDevice->setTexture(NULL, 3);
		gdata::pDXDevice->setTexture(NULL, 4);

		SGCore_ShaderUnBind();
	}

	m_pDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateNoZ);
	m_pDevice->setRasterizerState(NULL);
	m_pDevice->setBlendState(NULL);
	
	m_pDevice->setColorTarget(NULL, 1);
	
	mem_release(pAmbientSurf);
	mem_release(pSpecDiffSurf);

	//-------------------------------

	//теперь необходимо все смешать чтобы получить итоговую освещенную картинку
	//{{
	
	gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);
	
	IGXSurface *pComLightSurf = m_pLightTotal->getMipmap();
	gdata::pDXDevice->setColorTarget(pComLightSurf);

	//очищаем рт (в старой версии было многопроходное смешивание)
	gdata::pDXDevice->clear(GXCLEAR_COLOR);

	gdata::pDXDevice->setTexture(m_pGBufferColor);
	gdata::pDXDevice->setTexture(m_pLightAmbientDiffuse, 1);
	gdata::pDXDevice->setTexture(m_pLightSpecular, 2);
	gdata::pDXDevice->setTexture(m_pGBufferNormals, 3);
	//gdata::pDXDevice->setTexture(SGCore_GbufferGetRT(DS_RT_ADAPTEDLUM), 4);
	gdata::pDXDevice->setTexture(m_pGBufferParams, 5);

	SGCore_ShaderBind(gdata::shaders_id::kit::idBlendAmbientSpecDiffColor);

	SGCore_ScreenQuadDraw();

	mem_release(pComLightSurf);
	//}}

	SGCore_ShaderUnBind();

	gdata::pDXDevice->setColorTarget(pBackBuf);
	mem_release(pBackBuf);
}
void CRenderPipeline::renderPostprocessMain()
{
}
void CRenderPipeline::renderTransparent()
{
}
void CRenderPipeline::renderPostprocessFinal()
{
}
void CRenderPipeline::renderEditor2D()
{
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
	m_pDevice->setDepthStencilState(m_pDepthStencilStateNoZ);
	m_pDevice->setTexture(pTexture);
	SGCore_ShaderBind(gdata::shaders_id::kit::idScreenOut);
	SGCore_ScreenQuadDraw();
}

void CRenderPipeline::showFrameStats()
{
	Core_PStartSection(PERF_SECTION_RENDER_INFO);
	//@FIXME: Пока так
	SGame_Render();
	Core_PEndSection(PERF_SECTION_RENDER_INFO);
}
