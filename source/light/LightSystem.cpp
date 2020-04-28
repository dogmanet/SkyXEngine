#include "LightSystem.h"

class CLevelLoadListener: public IEventListener<XEventLevel>
{
public:
	CLevelLoadListener(CLightSystem *pLightSystem, IXCore *pCore):
		m_pLightSystem(pLightSystem)
	{
		m_pLevelSizeChannel = pCore->getEventChannel<XEventLevelSize>(EVENT_LEVEL_GET_SIZE_GUID);
	}

	void onEvent(const XEventLevel *pEvent) override
	{
		switch(pEvent->type)
		{
		case XEventLevel::TYPE_LOAD_END:
			XEventLevelSize levelSize;
			m_pLevelSizeChannel->broadcastEvent(&levelSize);

			m_pLightSystem->setLevelSize(levelSize.vMin, levelSize.vMax);
			break;
		}
	}

private:
	CLightSystem *m_pLightSystem;
	IEventChannel<XEventLevelSize> *m_pLevelSizeChannel = NULL;
};

//##########################################################################

CLightSystem::CLightSystem(IXCore *pCore):
	m_pCore(pCore)
{
	m_pLevelListener = new CLevelLoadListener(this, pCore);

	m_pLevelChannel = pCore->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID);
	m_pLevelChannel->addListener(m_pLevelListener);

	m_pDevice = SGCore_GetDXDevice();

	m_pMaterialSystem = (IXMaterialSystem*)pCore->getPluginManager()->getInterface(IXMATERIALSYSTEM_GUID);

	if(m_pDevice)
	{
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


		m_pLightLuminance = m_pDevice->createTexture2D(LUMINANCE_BUFFER_SIZE, LUMINANCE_BUFFER_SIZE, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R16F);

		m_pLightLuminance32 = m_pDevice->createTexture2D(32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_R16F);
		m_pLightLuminance1 = m_pDevice->createTexture2D(1, 1, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_R16F);
		m_pAdaptedLuminance[0] = m_pDevice->createTexture2D(1, 1, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_R16F);
		m_pAdaptedLuminance[1] = m_pDevice->createTexture2D(1, 1, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_R16F);
		
		
		m_pLPVcentersShaderData = m_pDevice->createConstantBuffer(sizeof(m_lpvCentersShaderData.vs));
		m_pLPVcurrentCascadeShaderData = m_pDevice->createConstantBuffer(sizeof(float4_t));
		
		m_pToneMappingShaderData = m_pDevice->createConstantBuffer(sizeof(m_toneMappingShaderData));

		for(UINT i = 0; i < 3; ++i)
		{
			m_aLPVs[i].pGIAccumRed = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
			m_aLPVs[i].pGIAccumGreen = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
			m_aLPVs[i].pGIAccumBlue = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);

			m_aLPVs[i].pGIAccumRed2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
			m_aLPVs[i].pGIAccumGreen2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
			m_aLPVs[i].pGIAccumBlue2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
		}

		m_idLightBoundShader = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "lighting_bound.vs"), -1);

		m_idLPVPropagateShader = SGCore_ShaderCreateKit(-1, -1, -1, SGCore_ShaderLoad(SHADER_TYPE_COMPUTE, "gi_propagation.cs"));

		m_idLuminanceReductionShader = SGCore_ShaderCreateKit(-1, -1, -1, SGCore_ShaderLoad(SHADER_TYPE_COMPUTE, "hdr_reduction.cs"));

		
		GXSamplerDesc samplerDesc;

		samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
		m_pSamplerPointClamp = m_pDevice->createSamplerState(&samplerDesc);

		samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_WRAP;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
		m_pSamplerLinearWrap = m_pDevice->createSamplerState(&samplerDesc);

		samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
		m_pSamplerLinearClamp = m_pDevice->createSamplerState(&samplerDesc);

		samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_BORDER;
		samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.f4BorderColor = float4_t(0.0f, 0.0f, 0.0f, 0.0f);
		m_pSamplerLinearBorder = m_pDevice->createSamplerState(&samplerDesc);

		m_pShadowShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_shadowShaderData.vs));


		GXRasterizerDesc rasterizerDesc;

		//rasterizerDesc.useConservativeRasterization = true;
		m_pRasterizerConservative = m_pDevice->createRasterizerState(&rasterizerDesc);

		//rasterizerDesc.useConservativeRasterization = false;
		rasterizerDesc.cullMode = GXCULL_NONE;
		m_pRasterizerCullNone = m_pDevice->createRasterizerState(&rasterizerDesc);



		GXBlendDesc blendDesc;

		blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_RED;
		m_pBlendRed = m_pDevice->createBlendState(&blendDesc);

		blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_ALL;
		blendDesc.renderTarget[0].useBlend = TRUE;
		blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_ONE;
		blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_ONE;
		m_pBlendAlphaOneOne = m_pDevice->createBlendState(&blendDesc);




		GXDepthStencilDesc dsDesc;
		dsDesc.cmpFuncDepth = GXCMP_GREATER_EQUAL;
		m_pDepthStencilStateDefault = m_pDevice->createDepthStencilState(&dsDesc);

		dsDesc.useDepthTest = FALSE;
		dsDesc.useDepthWrite = FALSE;
		m_pDepthStencilStateNoZ = m_pDevice->createDepthStencilState(&dsDesc);

		dsDesc.useStencilTest = TRUE;
		dsDesc.useDepthTest = TRUE;
		dsDesc.stencilTestFront.cmpFunc = GXCMP_ALWAYS;
		dsDesc.stencilTestFront.opDepthFail = GXSTENCIL_OP_INCR;
		dsDesc.stencilTestFront.opPass = GXSTENCIL_OP_KEEP;
		dsDesc.stencilTestBack.opDepthFail = GXSTENCIL_OP_DECR;
		m_pDepthStencilStateLightBound = m_pDevice->createDepthStencilState(&dsDesc);

		dsDesc.useDepthTest = FALSE;
		dsDesc.stencilTestFront.cmpFunc = GXCMP_EQUAL;
		dsDesc.stencilTestFront.opDepthFail = GXSTENCIL_OP_ZERO;
		dsDesc.stencilTestFront.opFail = GXSTENCIL_OP_ZERO;
		dsDesc.stencilTestFront.opPass = GXSTENCIL_OP_KEEP;
		m_pDepthStencilStateLightShadowNonGlobal = m_pDevice->createDepthStencilState(&dsDesc);

		dsDesc.stencilTestFront.opPass = GXSTENCIL_OP_ZERO;
		m_pDepthStencilStateLightClear = m_pDevice->createDepthStencilState(&dsDesc);

		dsDesc.useStencilTest = FALSE;
		m_pDepthStencilStateLightShadowGlobal = m_pDevice->createDepthStencilState(&dsDesc);



		ID idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs");
		ID idResPos = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs");

		GXMacro Defines_IS_SHADOWED[] = {{"IS_SHADOWED", ""}, {0, 0}};
		GXMacro Defines_IS_SPOT_SHADOWED[] = {{"IS_SHADOWED", ""}, {"IS_SPOT", ""}, {0, 0}};
		GXMacro Defines_IS_PSSM_SHADOWED[] = {{"IS_SHADOWED", ""}, {"IS_PSSM", ""}, {0, 0}};

		m_idBlendAmbientSpecDiffColor = SGCore_ShaderCreateKit(idResPos, SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_blend.ps"));
		m_idComLightingShadow = SGCore_ShaderCreateKit(idResPos, SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_shadow.ps", Defines_IS_SHADOWED));
		m_idComLightingSpotShadow = SGCore_ShaderCreateKit(idResPos, SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_spot_shadow.ps", Defines_IS_SPOT_SHADOWED));
		m_idComLightingPSSMShadow = SGCore_ShaderCreateKit(idResPos, SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_pssm_shadow.ps", Defines_IS_PSSM_SHADOWED));

		m_idComLightingGI = SGCore_ShaderCreateKit(idResPos, SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_gi.ps"));

		m_idHDRinitLuminance = SGCore_ShaderCreateKit(idScreenOut, SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "hdr_luminance.ps"));
		m_idHDRAdaptLuminance = SGCore_ShaderCreateKit(idScreenOut, SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "hdr_adapt.ps"));
		m_idHDRToneMapping = SGCore_ShaderCreateKit(idScreenOut, SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "hdr_tonemapping.ps"));
	}
	else
	{
		m_pAdaptedLuminance[0] = NULL;
		m_pAdaptedLuminance[1] = NULL;

		for(UINT i = 0; i < 3; ++i)
		{
			m_aLPVs[i].pGIAccumRed = NULL;
			m_aLPVs[i].pGIAccumGreen = NULL;
			m_aLPVs[i].pGIAccumBlue = NULL;

			m_aLPVs[i].pGIAccumRed2 = NULL;
			m_aLPVs[i].pGIAccumGreen2 = NULL;
			m_aLPVs[i].pGIAccumBlue2 = NULL;
		}
	}
}

CLightSystem::~CLightSystem()
{
	m_pLevelChannel->removeListener(m_pLevelListener);
	mem_delete(m_pLevelListener);
	mem_delete(m_pSun);
	mem_release(m_pShapeSphere);
	mem_release(m_pShapeCone);
	mem_release(m_pRenderPipeline);

	mem_release(m_pGICubesRB);

	mem_release(m_pLightLuminance);
	mem_release(m_pLightLuminance32);
	mem_release(m_pLightLuminance1);
	mem_release(m_pAdaptedLuminance[0]);
	mem_release(m_pAdaptedLuminance[1]);

	mem_release(m_pLPVcentersShaderData);
	mem_release(m_pLPVcurrentCascadeShaderData);
	mem_release(m_pToneMappingShaderData);


	for(UINT i = 0; i < 3; ++i)
	{
		mem_release(m_aLPVs[i].pGIAccumRed);
		mem_release(m_aLPVs[i].pGIAccumGreen);
		mem_release(m_aLPVs[i].pGIAccumBlue);

		mem_release(m_aLPVs[i].pGIAccumRed2);
		mem_release(m_aLPVs[i].pGIAccumGreen2);
		mem_release(m_aLPVs[i].pGIAccumBlue2);
	}

	mem_release(m_pDepthStencilStateDefault);

	mem_release(m_pSamplerPointClamp);
	mem_release(m_pSamplerLinearWrap);
	mem_release(m_pSamplerLinearClamp);
	mem_release(m_pSamplerLinearBorder);

	mem_release(m_pShadowShaderDataVS);

	mem_delete(m_pShadowCache);

	mem_release(m_pRasterizerConservative);
	mem_release(m_pRasterizerCullNone);

	mem_release(m_pBlendRed);
	mem_release(m_pBlendAlphaOneOne);

	mem_release(m_pDepthStencilStateNoZ);
	mem_release(m_pDepthStencilStateLightBound);
	mem_release(m_pDepthStencilStateLightClear);
	mem_release(m_pDepthStencilStateLightShadowNonGlobal);
	mem_release(m_pDepthStencilStateLightShadowGlobal);

	mem_release(m_pGBufferColor);
	mem_release(m_pGBufferNormals);
	mem_release(m_pGBufferParams);
	mem_release(m_pGBufferDepth);
}

void CLightSystem::setLevelSize(const float3 &vMin, const float3 &vMax)
{
	m_vLevelMin = vMin;
	m_vLevelMax = vMax;
	m_fLevelDimensions = SMVector3Length(vMin - vMax);
	if(m_fLevelDimensions < 100.0f)
	{
		m_fLevelDimensions = 100.0f;
	}
	if(m_pSun)
	{
		m_pSun->setMaxDistance(m_fLevelDimensions);
	}
}

IXLightSun* XMETHODCALLTYPE CLightSystem::newSun()
{
	if(m_pSun)
	{
		m_pSun->AddRef();
	}
	else
	{
		m_pSun = new CXLightSun(this);
		m_pSun->setMaxDistance(m_fLevelDimensions);
		m_aLights.push_back(m_pSun);
	}
	return(m_pSun);
}

IXLightSun* XMETHODCALLTYPE CLightSystem::getSun()
{
	if(m_pSun)
	{
		m_pSun->AddRef();
	}
	return(m_pSun);
}

IXLightPoint* XMETHODCALLTYPE CLightSystem::newPoint()
{
	CXLightPoint *pLight = m_poolPoint.Alloc(this);
	m_aLights.push_back(pLight);
	return(pLight);
}
IXLightSpot* XMETHODCALLTYPE CLightSystem::newSpot()
{
	CXLightSpot *pLight = m_poolSpot.Alloc(this);
	m_aLights.push_back(pLight);
	return(pLight);
}

UINT XMETHODCALLTYPE CLightSystem::getCount()
{
	return(m_aLights.size());
}
IXLight* XMETHODCALLTYPE CLightSystem::getLight(ID id)
{
	assert(ID_VALID(id) && id < (ID)m_aLights.size());

	return(m_aLights[id]);
}

void CLightSystem::destroySun(IXLightSun *pLight)
{
	assert(m_pSun == pLight);
	_deleteLight(m_pSun);
	mem_delete(m_pSun);
}
void CLightSystem::destroyPoint(IXLightPoint *_pLight)
{
	CXLightPoint *pLight = dynamic_cast<CXLightPoint*>(_pLight);
	_deleteLight(pLight);
	m_poolPoint.Delete(pLight);
}
void CLightSystem::destroySpot(IXLightSpot *_pLight)
{
	CXLightSpot *pLight = dynamic_cast<CXLightSpot*>(_pLight);
	_deleteLight(pLight);
	m_poolSpot.Delete(pLight);
}

void CLightSystem::_deleteLight(CXLight *pLight)
{
	for(UINT i = 0, l = m_aLights.size(); i < l; ++i)
	{
		if(m_aLights[i] == pLight)
		{
			m_aLights.erase(i);
			break;
		}
	}
}

IMesh* CLightSystem::getShapeSphere()
{
	if(!m_pShapeSphere)
	{
		SGCore_FCreateSphere(1.0f, 16, 16, &m_pShapeSphere);
	}
	m_pShapeSphere->AddRef();
	return(m_pShapeSphere);
}

IMesh* CLightSystem::getShapeCone()
{
	if(!m_pShapeCone)
	{
		SGCore_FCreateCone(0.0f, 1.0f, 1.0f, &m_pShapeCone, 16);
	}
	m_pShapeCone->AddRef();
	return(m_pShapeCone);
}

void XMETHODCALLTYPE CLightSystem::updateVisibility()
{
	assert(m_pMainCamera);

	float3 vCamPos;
	m_pMainCamera->getPosition(&vCamPos);

	//! @todo fix this values!
	float3 vLPVmin = float3(-16.0f, -16.0f, -16.0f) + vCamPos;
	float3 vLPVmax = float3(16.0f, 16.0f, 16.0f) + vCamPos;

	for(UINT i = 0, l = m_aLights.size(); i < l; ++i)
	{
		if(m_aLights[i]->isEnabled())
		{
			m_aLights[i]->updateVisibility(m_pMainCamera, vLPVmin, vLPVmax);
		}
	}
}

void XMETHODCALLTYPE CLightSystem::setFrameObserverCamera(ICamera *pMainCamera)
{
	//! @todo uncomment me!
	// pMainCamera->AddRef();
	m_pMainCamera = pMainCamera;
}

void XMETHODCALLTYPE CLightSystem::setGBuffer(IGXTexture2D *pColor, IGXTexture2D *pNormals, IGXTexture2D *pParams, IGXTexture2D *pDepth)
{
	pColor->AddRef();
	pNormals->AddRef();
	pParams->AddRef();
	pDepth->AddRef();
	m_pGBufferColor = pColor;
	m_pGBufferNormals = pNormals;
	m_pGBufferParams = pParams;
	m_pGBufferDepth = pDepth;
}

void XMETHODCALLTYPE CLightSystem::setRenderPipeline(IXRenderPipeline *pRenderPipeline)
{
	mem_release(m_pRenderPipeline);
	m_pRenderPipeline = pRenderPipeline;
	m_pRenderPipeline->AddRef();
	if(!m_pShadowCache)
	{
		m_pShadowCache = new CShadowCache(m_pRenderPipeline, m_pMaterialSystem);
	}
}

void XMETHODCALLTYPE CLightSystem::renderGI(IGXTexture2D *pLightTotal, IGXTexture2D *pTempBuffer)
{
	IGXContext *pCtx = m_pDevice->getThreadContext();

	IGXDepthStencilSurface *pOldDSSurface = pCtx->getDepthStencilSurface();

	UINT uCounts[LIGHT_TYPE__COUNT] = {0};
	for(int i = 0, l = getCount(); i < l; ++i)
	{
		++uCounts[getLight(i)->getType()];
	}
	m_pShadowCache->setLightsCount(uCounts[LIGHT_TYPE_POINT], uCounts[LIGHT_TYPE_SPOT], uCounts[LIGHT_TYPE_SUN] != 0);
	m_pShadowCache->setRSMLightsCount(uCounts[LIGHT_TYPE_POINT], uCounts[LIGHT_TYPE_SPOT], uCounts[LIGHT_TYPE_SUN] != 0);

	m_pShadowCache->setObserverCamera(m_pMainCamera);

	m_pShadowCache->nextFrame();

	IGXSurface *pAmbientSurf, *pBackBuf;
	pAmbientSurf = pTempBuffer->getMipmap();

	pBackBuf = pCtx->getColorTarget();

	pCtx->setColorTarget(pAmbientSurf);

	//очищаем рт и стенсил
	pCtx->clear(GX_CLEAR_COLOR);
	//	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_STENCIL);
	
	m_pRenderPipeline->renderGI();

	float3 vCamDir;
	m_pMainCamera->getLook(&vCamDir);
	float3 vCamPos;
	m_pMainCamera->getPosition(&vCamPos);


	const float c_aLPVsizes[] = {
		//0.5f,
		//1.0f,
		//2.0f

		1.0f,
		2.0f,
		4.0f
	};

	m_lpvCentersShaderData.vs.vCenterSize[0] = float4(vCamPos + vCamDir * (LPV_GRID_SIZE / 2 - LPV_STEP_COUNT) * c_aLPVsizes[0], c_aLPVsizes[0]);
	m_lpvCentersShaderData.vs.vCenterSize[1] = float4(vCamPos + vCamDir * (LPV_GRID_SIZE / 2 - LPV_STEP_COUNT) * c_aLPVsizes[1], c_aLPVsizes[1]);
	m_lpvCentersShaderData.vs.vCenterSize[2] = float4(vCamPos + vCamDir * (LPV_GRID_SIZE / 2 - LPV_STEP_COUNT) * c_aLPVsizes[2], c_aLPVsizes[2]);
	m_pLPVcentersShaderData->update(&m_lpvCentersShaderData.vs);

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
	for(int i = 0, l = getCount(); i < l; ++i)
	{
		pLight = getLight(i);

		//если свет виден фрустуму камеры (это надо было заранее просчитать) и если свет включен
		if(pLight->isEnabled() && pLight->getRenderType() != LRT_NONE)
		{
			if(pLight->getRenderType() & LRT_LPV)
			{
				m_pShadowCache->addRSMLight(pLight);
			}
			if(pLight->getRenderType() & LRT_LIGHT)
			{
				m_pShadowCache->addLight(pLight);
			}
		}
	}

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");
	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	// 	float4x4 &mCamView = gdata::mCamView;
	// 	// Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mCamView);
	// 	m_shadowShaderData.vs.mViewInv = SMMatrixTranspose(SMMatrixInverse(NULL, mCamView));
	// 	m_shadowShaderData.vs.vNearFar = gdata::vNearFar; // float2(*r_near, *r_far);
	// 	m_shadowShaderData.vs.vParamProj = float3((float)m_uOutWidth, (float)m_uOutHeight, gdata::fProjFov); // *r_default_fov);
	// 	m_pShadowShaderDataVS->update(&m_shadowShaderData.vs);

	//@TODO: убрать это
	//Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &m_shadowShaderData.ps.vPosCam);
	//m_pShadowShaderDataPS->update(&m_shadowShaderData.ps);


	//m_pDevice->setPixelShaderConstant(m_pShadowShaderDataPS, 7);

	pCtx->setRasterizerState(m_pRasterizerConservative);

	//pCtx->setVSConstant(m_pCameraShaderDataVS, 8);
	//pCtx->setPSConstant(m_pCameraShaderDataVS, 8);

	UINT uShadowCount = 0;
	while((uShadowCount = m_pShadowCache->processNextBunch()))
	{
		pCtx->setColorTarget(pAmbientSurf);
		pCtx->setDepthStencilSurface(pOldDSSurface);
		pCtx->setBlendState(m_pBlendAlphaOneOne);

	//	pCtx->setVSConstant(m_pLightingShaderDataVS, 1);
	//	pCtx->setPSConstant(m_pLightingShaderDataPS, 1);

		IBaseShadowMap *pShadow = NULL;

		//render direct light with shadows
		for(UINT i = 0; i < uShadowCount; ++i)
		{
			pLight = m_pShadowCache->getLight(i);
			pShadow = m_pShadowCache->getShadow(i);

			//если не глобальный источник
			if(pLight->getType() != LIGHT_TYPE_SUN)
			{
				//помечаем в стенсил буфере пиксели  которые входят в ограничивающий объем света, чтобы их осветить
				pCtx->setRasterizerState(m_pRasterizerCullNone);
				pCtx->setStencilRef(0);
				pCtx->setDepthStencilState(m_pDepthStencilStateLightBound);

				//отрисовка ограничивающего объема
				SGCore_ShaderBind(m_idLightBoundShader);
				pLight->drawShape(m_pDevice);

				pCtx->setStencilRef(255);
				pCtx->setDepthStencilState(m_pDepthStencilStateLightShadowNonGlobal);
			}
			else
			{
				//иначе это глобальный источник, отключаем стенсил тест
				pCtx->setDepthStencilState(m_pDepthStencilStateLightShadowGlobal);
			}

			pCtx->setRasterizerState(NULL);



			//pCtx->setVSConstant(m_pShadowShaderDataVS, 1);
		//	pCtx->setVSConstant(m_pLightingShaderDataVS, 1);

			pCtx->setBlendState(m_pBlendRed);
			pShadow->genShadow(m_pGBufferDepth, m_pGBufferNormals);
			pCtx->setBlendState(m_pBlendAlphaOneOne);
			pCtx->setColorTarget(pAmbientSurf);

			//	pCtx->setPSTexture(m_pShadow, 4);
			//idshaderkit = gdata::shaders_id::kit::idComLightingShadow;
			ID idshaderkit = -1;
			switch(pLight->getType())
			{
			case LIGHT_TYPE_SPOT:
				idshaderkit = m_idComLightingSpotShadow;
				break;
			case LIGHT_TYPE_POINT:
				idshaderkit = m_idComLightingShadow;
				break;
			case LIGHT_TYPE_SUN:
				idshaderkit = m_idComLightingPSSMShadow;
				break;
			default:
				assert(!"Unknown light type!");
			}

		//	pCtx->setVSConstant(m_pLightingShaderDataVS, 1);

			//теперь когда будем считать освещение надо сбросить значения в стенсил буфере, чтобы каждый кадр не чистить
			//если стенсил тест прошел успешно, устанавливаем значнеие в нуль
			if(pLight->getType() != LIGHT_TYPE_SUN)
			{
				pCtx->setDepthStencilState(m_pDepthStencilStateLightClear);
			}

			IGXConstantBuffer *pLightConstants = pLight->getConstants(m_pDevice);
			pCtx->setPSConstant(pLightConstants);
			mem_release(pLightConstants);

			SGCore_ShaderBind(idshaderkit);

			pCtx->setSamplerState(m_pSamplerPointClamp, 0);

			pCtx->setPSTexture(m_pGBufferColor);
			pCtx->setPSTexture(m_pGBufferNormals, 1);
			pCtx->setPSTexture(m_pGBufferParams, 2);
			pCtx->setPSTexture(m_pGBufferDepth, 3);
			//m_pDevice->setTexture(SGCore_GbufferGetRT(DS_RT_ADAPTEDLUM), 5);

			SGCore_ScreenQuadDraw();
		}


		//pCtx->setSamplerState(gdata::rstates::pSamplerLinearWrap, 0);
		//pCtx->setSamplerState(gdata::rstates::pSamplerLinearWrap, 1);
	}

	pCtx->setVSConstant(m_pLPVcentersShaderData, 9);
	pCtx->setPSConstant(m_pLPVcentersShaderData, 9);

	bool isFirstRun[3] = {true, true, true};
	while((uShadowCount = m_pShadowCache->processNextRSMBunch()))
	{
		pCtx->setVSConstant(m_pLPVcurrentCascadeShaderData, 10);
		pCtx->setPSConstant(m_pLPVcurrentCascadeShaderData, 10);

		pCtx->setDepthStencilSurface(pOldDSSurface);
		pCtx->setBlendState(m_pBlendAlphaOneOne);

	//	pCtx->setVSConstant(m_pLightingShaderDataVS, 1);
	//	pCtx->setPSConstant(m_pLightingShaderDataPS, 1);

		IGXDepthStencilSurface *pOldSurface = pCtx->getDepthStencilSurface();
		pCtx->unsetDepthStencilSurface();

		IBaseReflectiveShadowMap *pShadow = NULL;

		for(UINT i = 0; i < 3; ++i)
		{
			float4_t vTmp((float)i + 0.5f); // just to be sure
			m_pLPVcurrentCascadeShaderData->update(&vTmp);

			IGXSurface *pLPVRed = m_aLPVs[i].pGIAccumRed->asRenderTarget();
			IGXSurface *pLPVGreen = m_aLPVs[i].pGIAccumGreen->asRenderTarget();
			IGXSurface *pLPVBlue = m_aLPVs[i].pGIAccumBlue->asRenderTarget();

			//m_pDevice->setColorTarget(pAmbientSurf);
			pCtx->setColorTarget(pLPVRed);
			pCtx->setColorTarget(pLPVGreen, 1);
			pCtx->setColorTarget(pLPVBlue, 2);

			mem_release(pLPVRed);
			mem_release(pLPVGreen);
			mem_release(pLPVBlue);

			if(isFirstRun[i])
			{
				pCtx->clear(GX_CLEAR_COLOR);
				isFirstRun[i] = false;
			}

			//inject VPLs into LPV grid
			for(UINT j = 0; j < uShadowCount; ++j)
			{
				pShadow = m_pShadowCache->getRSMShadow(j);
				pShadow->genLPV();
			}

			pCtx->setColorTarget(NULL);
			pCtx->setColorTarget(NULL, 1);
			pCtx->setColorTarget(NULL, 2);
		}

		const bool *dev_lpv_points = GET_PCVAR_BOOL("dev_lpv_points");
		if(*dev_lpv_points)
		{
			auto pTarget = m_pGBufferColor->asRenderTarget();
			pCtx->setColorTarget(pAmbientSurf);
			mem_release(pTarget);
			for(UINT i = 0; i < uShadowCount; ++i)
			{
				pShadow = m_pShadowCache->getRSMShadow(i);
				pShadow->genLPV(true);
			}
			pCtx->setColorTarget(NULL);
		}

		pCtx->setDepthStencilSurface(pOldSurface);
		mem_release(pOldSurface);

		//break;
	}
	for(UINT i = 0; i < 3; ++i)
	{
		if(isFirstRun[i])
		{
			IGXSurface *pLPVRed = m_aLPVs[i].pGIAccumRed->asRenderTarget();
			IGXSurface *pLPVGreen = m_aLPVs[i].pGIAccumGreen->asRenderTarget();
			IGXSurface *pLPVBlue = m_aLPVs[i].pGIAccumBlue->asRenderTarget();

			pCtx->setColorTarget(pLPVRed);
			pCtx->setColorTarget(pLPVGreen, 1);
			pCtx->setColorTarget(pLPVBlue, 2);

			mem_release(pLPVRed);
			mem_release(pLPVGreen);
			mem_release(pLPVBlue);

			pCtx->clear(GX_CLEAR_COLOR);
			isFirstRun[i] = false;

			pCtx->setColorTarget(NULL);
			pCtx->setColorTarget(NULL, 1);
			pCtx->setColorTarget(NULL, 2);
		}
	}

	SGCore_ShaderUnBind();

	mem_release(pOldDSSurface);

	//pCtx->setVSConstant(m_pLightingShaderDataVS, 1);
	//pCtx->setPSConstant(m_pLightingShaderDataPS, 1);

	{
		SGCore_ShaderBind(m_idLPVPropagateShader);
		UINT uStepCount[] = {4, 6, 8};

		//for(UINT k = 0; k < 1000; ++k)
		//{
		for(UINT j = 0; j < 3; ++j)
		{
			for(UINT i = 0; i < uStepCount[j]; ++i)
			{
				pCtx->setCSTexture(m_aLPVs[j].pGIAccumRed, 0);
				pCtx->setCSTexture(m_aLPVs[j].pGIAccumGreen, 1);
				pCtx->setCSTexture(m_aLPVs[j].pGIAccumBlue, 2);

				pCtx->setCSUnorderedAccessView(m_aLPVs[j].pGIAccumRed2, 0);
				pCtx->setCSUnorderedAccessView(m_aLPVs[j].pGIAccumGreen2, 1);
				pCtx->setCSUnorderedAccessView(m_aLPVs[j].pGIAccumBlue2, 2);

				pCtx->computeDispatch(4, 4, 32);

				pCtx->setCSUnorderedAccessView(NULL, 0);
				pCtx->setCSUnorderedAccessView(NULL, 1);
				pCtx->setCSUnorderedAccessView(NULL, 2);

				pCtx->setCSTexture(NULL, 0);
				pCtx->setCSTexture(NULL, 1);
				pCtx->setCSTexture(NULL, 2);


				pCtx->setCSTexture(m_aLPVs[j].pGIAccumRed2, 0);
				pCtx->setCSTexture(m_aLPVs[j].pGIAccumGreen2, 1);
				pCtx->setCSTexture(m_aLPVs[j].pGIAccumBlue2, 2);

				pCtx->setCSUnorderedAccessView(m_aLPVs[j].pGIAccumRed, 0);
				pCtx->setCSUnorderedAccessView(m_aLPVs[j].pGIAccumGreen, 1);
				pCtx->setCSUnorderedAccessView(m_aLPVs[j].pGIAccumBlue, 2);

				pCtx->computeDispatch(4, 4, 32);

				pCtx->setCSUnorderedAccessView(NULL, 0);
				pCtx->setCSUnorderedAccessView(NULL, 1);
				pCtx->setCSUnorderedAccessView(NULL, 2);

				pCtx->setCSTexture(NULL, 0);
				pCtx->setCSTexture(NULL, 1);
				pCtx->setCSTexture(NULL, 2);
			}
		}
		//}

		SGCore_ShaderUnBind();
	}


	{
		pCtx->setColorTarget(pAmbientSurf);
		//gdata::pDXDevice->setRasterizerState(NULL);
		pCtx->setRasterizerState(m_pRasterizerCullNone);
		pCtx->setBlendState(m_pBlendAlphaOneOne);
		pCtx->setDepthStencilState(m_pDepthStencilStateLightShadowGlobal);

		//pCtx->setPSConstant(m_pLightingShaderDataPS, 1);

		ID idshaderkit = m_idComLightingGI;

		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "g_vViewPos", &gdata::vConstCurrCamPos);

		//	pCtx->setPSConstant(m_pCameraShaderDataVS, 8);

		SGCore_ShaderBind(idshaderkit);

		pCtx->setSamplerState(m_pSamplerPointClamp, 0);
		pCtx->setSamplerState(m_pSamplerLinearBorder, 1);

		pCtx->setPSTexture(m_pGBufferDepth);
		pCtx->setPSTexture(m_pGBufferNormals, 1);
		pCtx->setPSTexture(m_pGBufferColor, 2);
		pCtx->setPSTexture(m_pGBufferParams, 3);
		for(UINT i = 0; i < 3; ++i)
		{
			pCtx->setPSTexture(m_aLPVs[i].pGIAccumRed, 4 + i);
			pCtx->setPSTexture(m_aLPVs[i].pGIAccumGreen, 7 + i);
			pCtx->setPSTexture(m_aLPVs[i].pGIAccumBlue, 10 + i);
		}

		SGCore_ScreenQuadDraw();

		pCtx->setPSTexture(NULL, 2);
		pCtx->setPSTexture(NULL, 3);
		pCtx->setPSTexture(NULL, 4);

		SGCore_ShaderUnBind();
		pCtx->setSamplerState(m_pSamplerLinearClamp, 1);
	}



	pCtx->setDepthStencilState(m_pDepthStencilStateNoZ);
	pCtx->setRasterizerState(NULL);
	pCtx->setBlendState(NULL);

	//pCtx->setColorTarget(NULL, 1);

	mem_release(pAmbientSurf);

	//-------------------------------

	//теперь необходимо все смешать чтобы получить итоговую освещенную картинку
	//{{

	pCtx->setSamplerState(m_pSamplerPointClamp, 0);
	pCtx->setSamplerState(m_pSamplerLinearClamp, 1);

	IGXSurface *pComLightSurf = pLightTotal->getMipmap();
	pCtx->setColorTarget(pComLightSurf);

	//очищаем рт (в старой версии было многопроходное смешивание)
	//	pCtx->clear(GX_CLEAR_COLOR);

	pCtx->setPSTexture(m_pGBufferColor);
	pCtx->setPSTexture(pTempBuffer, 1);
	//pCtx->setPSTexture(m_pLightSpecular, 2);
	pCtx->setPSTexture(m_pGBufferNormals, 3);
	pCtx->setPSTexture(m_pGBufferDepth, 4);
	//gdata::pDXDevice->setTexture(SGCore_GbufferGetRT(DS_RT_ADAPTEDLUM), 4);
	pCtx->setPSTexture(m_pGBufferParams, 5);

	SGCore_ShaderBind(m_idBlendAmbientSpecDiffColor);

	SGCore_ScreenQuadDraw();

	mem_release(pComLightSurf);
	//}}

	SGCore_ShaderUnBind();

	pCtx->setColorTarget(pBackBuf);
	mem_release(pBackBuf);

	//Sleep(16);
}
void XMETHODCALLTYPE CLightSystem::renderToneMapping(IGXTexture2D *pSourceLight)
{
	Core_PStartSection(PERF_SECTION_TONEMAPPING);
	static const float *hdr_adapted_coef = GET_PCVAR_FLOAT("hdr_adapted_coef");
	static const float *hdr_base_value = GET_PCVAR_FLOAT("hdr_base_value");

	//! @todo update only on change
	m_toneMappingShaderData.fAdaptationSpeed = *hdr_adapted_coef;
	m_toneMappingShaderData.fBaseValue = *hdr_base_value;
	m_pToneMappingShaderData->update(&m_toneMappingShaderData);

	//	SGCore_ToneMappingCom(timeDelta, (hdr_adapted_coef ? (*hdr_adapted_coef) : 0.03f));
	IGXContext *pCtx = m_pDevice->getThreadContext();

	pCtx->setPSConstant(m_pToneMappingShaderData, 8);

	IGXSurface *pBackBuf = pCtx->getColorTarget();

	IGXSurface *pRT = m_pLightLuminance->asRenderTarget();
	pCtx->setColorTarget(pRT);
	mem_release(pRT);

	pCtx->setPSTexture(pSourceLight);
	pCtx->setSamplerState(m_pSamplerLinearClamp, 0);

	IGXDepthStencilSurface *pDSSurface = pCtx->getDepthStencilSurface();
	pCtx->unsetDepthStencilSurface();

	SGCore_ShaderBind(m_idHDRinitLuminance);
	SGCore_ScreenQuadDraw();
	SGCore_ShaderUnBind();

	pCtx->setColorTarget(pBackBuf);

	// Reduction
	{
		SGCore_ShaderBind(m_idLuminanceReductionShader);

		pCtx->setCSTexture(m_pLightLuminance, 0);
		pCtx->setCSUnorderedAccessView(m_pLightLuminance32, 0);
		pCtx->computeDispatch(32, 32, 1);
		pCtx->setCSUnorderedAccessView(NULL, 0);
		pCtx->setCSTexture(NULL, 0);

		pCtx->setCSTexture(m_pLightLuminance32, 0);
		pCtx->setCSUnorderedAccessView(m_pLightLuminance1, 0);
		pCtx->computeDispatch(1, 1, 1);
		pCtx->setCSUnorderedAccessView(NULL, 0);
		pCtx->setCSTexture(NULL, 0);

		SGCore_ShaderUnBind();
	}

	// Adaptation
	{
		// m_pLightLuminance1
		pCtx->setPSTexture(m_pAdaptedLuminance[!m_uCurrAdaptedLuminanceTarget]);
		pCtx->setPSTexture(m_pLightLuminance1, 1);
		IGXSurface *pAdaptedLuminanceBuf = m_pAdaptedLuminance[m_uCurrAdaptedLuminanceTarget]->asRenderTarget();
		pCtx->setColorTarget(pAdaptedLuminanceBuf);
		mem_release(pAdaptedLuminanceBuf);

		SGCore_ShaderBind(m_idHDRAdaptLuminance);

		SGCore_ScreenQuadDraw();

		SGCore_ShaderUnBind();
	}


	pCtx->setColorTarget(pBackBuf);
	mem_release(pBackBuf);

	{
		pCtx->setSamplerState(m_pSamplerPointClamp, 0);
		pCtx->setPSTexture(pSourceLight);
		pCtx->setPSTexture(m_pAdaptedLuminance[m_uCurrAdaptedLuminanceTarget], 1);
		pCtx->setPSTexture(m_pLightLuminance1, 2);
		SGCore_ShaderBind(m_idHDRToneMapping);

		SGCore_ScreenQuadDraw();

		SGCore_ShaderUnBind();
	}

	pCtx->setDepthStencilSurface(pDSSurface);
	m_uCurrAdaptedLuminanceTarget = !m_uCurrAdaptedLuminanceTarget;

	Core_PEndSection(PERF_SECTION_TONEMAPPING);
}
void XMETHODCALLTYPE CLightSystem::renderDebug()
{
	const bool *dev_lpv_cubes = GET_PCVAR_BOOL("dev_lpv_cubes");
	if(*dev_lpv_cubes)
	{
		showGICubes();
	}
}

void CLightSystem::showGICubes()
{
	IGXContext *pCtx = m_pDevice->getThreadContext();

	pCtx->setPrimitiveTopology(GXPT_POINTLIST);
	pCtx->setRenderBuffer(m_pGICubesRB);
	SGCore_ShaderBind(m_idGICubesShader);
	//pCtx->setGSConstant(m_pLightingShaderDataVS, 1);
	pCtx->setDepthStencilState(m_pDepthStencilStateDefault);
	pCtx->setSamplerState(m_pSamplerPointClamp, 0);

	pCtx->setVSConstant(m_pLPVcurrentCascadeShaderData, 10);
	pCtx->setGSConstant(m_pLPVcurrentCascadeShaderData, 10);
	pCtx->setPSConstant(m_pLPVcurrentCascadeShaderData, 10);

	for(UINT i = 2; i < 3; ++i)
	{
		float4_t vTmp((float)i + 0.5f);  // just to be sure
		m_pLPVcurrentCascadeShaderData->update(&vTmp);

		pCtx->setPSTexture(m_aLPVs[i].pGIAccumRed2, 0);
		pCtx->setPSTexture(m_aLPVs[i].pGIAccumGreen2, 1);
		pCtx->setPSTexture(m_aLPVs[i].pGIAccumBlue2, 2);

		pCtx->drawPrimitive(0, m_uGICubesCount);
	}


	pCtx->setPrimitiveTopology(GXPT_TRIANGLELIST);

	pCtx->setPSTexture(NULL, 0);
	pCtx->setPSTexture(NULL, 1);
	pCtx->setPSTexture(NULL, 2);
}
