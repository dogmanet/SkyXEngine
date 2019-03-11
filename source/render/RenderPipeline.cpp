#include "RenderPipeline.h"

#include <core/sxcore.h>

#include "render_func.h"

CRenderPipeline::CRenderPipeline(IGXContext *pDevice):
	m_pDevice(pDevice)
{
	// load renderables
	IXCore *pCore = Core_GetIXCore();
	IPluginManager *pPluginManager = pCore->getPluginManager();
	IXMaterialSystem *pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);

	IXRenderable *pRenderable;
	UINT ic = 0;
	while((pRenderable = (IXRenderable*)pPluginManager->getInterface(IXRENDERABLE_GUID, ic++)))
	{
		if(pRenderable->getVersion() == IXRENDERABLE_VERSION)
		{
			pRenderable->startup(m_pDevice, pMaterialSystem);
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


	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferColor = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8);
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferNormals = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8);
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferParams = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_A8R8G8B8);
	//GXFMT_G32R32F; // 64bpp; GXFMT_R32F
	m_pGBufferDepth = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXUSAGE_RENDERTARGET | GX_TEXUSAGE_AUTORESIZE, GXFMT_R32F);

	GXDEPTH_STENCIL_DESC dsDesc;

	dsDesc.bDepthEnable = FALSE;
	dsDesc.bEnableDepthWrite = FALSE;
	m_pDepthStencilStateNoZ = m_pDevice->createDepthStencilState(&dsDesc);

	m_pSceneShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_sceneShaderData.vs));
	m_pSceneShaderDataPS = m_pDevice->createConstantBuffer(sizeof(m_sceneShaderData));

}
CRenderPipeline::~CRenderPipeline()
{
	mem_release(m_pSceneShaderDataVS);
	mem_release(m_pSceneShaderDataPS);
	mem_release(m_pGBufferColor);
	mem_release(m_pGBufferNormals);
	mem_release(m_pGBufferParams);
	mem_release(m_pGBufferDepth);

	mem_release(m_pDepthStencilStateNoZ);
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
		break;
	case DS_RT_NORMAL:
		showTexture(m_pGBufferNormals);
		goto end;
		break;
	case DS_RT_PARAM:
		showTexture(m_pGBufferParams);
		goto end;
		break;
	case DS_RT_DEPTH:
		showTexture(m_pGBufferDepth);
		goto end;
	}

#if 0
	Core_PStartSection(PERF_SECTION_SHADOW_UPDATE);
	SRender_UpdateShadow(timeDelta);
	Core_PEndSection(PERF_SECTION_SHADOW_UPDATE);



	Core_PStartSection(PERF_SECTION_LIGHTING);
//	SRender_ComLighting(timeDelta);

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



void CRenderPipeline::renderPrepare()
{
}
void CRenderPipeline::renderGBuffer()
{
	m_pDevice->setRasterizerState(NULL);
	m_pDevice->setDepthStencilState(NULL);
	m_pDevice->setBlendState(NULL);
	rfunc::SetRenderSceneFilter();

	//?
	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_MATERIAL);

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
