#include "RenderPipeline.h"

#include <core/sxcore.h>

#include "render_func.h"

#include "RenderableVisibility.h"

namespace gdata
{
	extern ICamera *pCamera;
};

//! При изменении базовых шейдеров отредактировать https://wiki.skyxengine.com/index.php?title=Стандартные_шейдеры_материалов

CRenderPipeline::CRenderPipeline(IGXDevice *pDevice):
	m_pDevice(pDevice)
{
	// load renderables
	IXCore *pCore = Core_GetIXCore();
	IPluginManager *pPluginManager = pCore->getPluginManager();
	m_pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);

	Core_0RegisterCVarBool("dev_lpv_cubes", false, "Отображать сетку LPV");
	Core_0RegisterCVarBool("dev_lpv_points", false, "Отображать VPL при инъекции в LPV");
	
	XVertexOutputElement voelGeneric[] = {
		{"vPosition", GXDECLTYPE_FLOAT4, GXDECLUSAGE_POSITION},
		{"vTexUV", GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		{"vNormal", GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD1},
		{"vPos", GXDECLTYPE_FLOAT4, GXDECLUSAGE_TEXCOORD2},
		XVERTEX_OUTPUT_DECL_END()
	};
	XVertexFormatHandler *pVertexFormatSceneGeneric = m_pMaterialSystem->registerVertexFormat("xSceneGeneric", voelGeneric);

	XVertexOutputElement voelPostprocess[] = {
		{"vPosition", GXDECLTYPE_FLOAT4, GXDECLUSAGE_POSITION},
		{"vTexUV", GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		{"vEyeRay", GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD1},
		{"vWorldRay", GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD2},
		XVERTEX_OUTPUT_DECL_END()
	};
	XVertexFormatHandler *pVertexFormatPostprocess = m_pMaterialSystem->registerVertexFormat("xPostprocess", voelPostprocess);
	
	XVertexOutputElement voelSky[] = {
		{"vPosition", GXDECLTYPE_FLOAT4, GXDECLUSAGE_POSITION},
		{"vTexUV", GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD},
		{"vNormal", GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD1},
		{"vPos", GXDECLTYPE_FLOAT4, GXDECLUSAGE_TEXCOORD2},
		XVERTEX_OUTPUT_DECL_END()
	};
	XVertexFormatHandler *pVertexFormatSkyBox = m_pMaterialSystem->registerVertexFormat("xSky", voelSky);

	m_pMaterialSystem->registerVertexShader(pVertexFormatPostprocess, "base/post.vs");
	{}

	{
		XRenderPassSamplersElement pSamplers[] = {
			{"Scene default", "sScene", 0},
			XRENDER_PASS_SAMPLERS_LIST_END()
		};

		XRenderPassOutputElement pOutput[] = {
			{"Base color", "vBaseColor", GXDECLTYPE_FLOAT3, "float3(1.0f, 0.0f, 0.0f)"},
			{"Normal", "vNormal", GXDECLTYPE_FLOAT3, "float3(0.0f, 0.0f, 0.0f)"},
			{"AO", "fAO", GXDECLTYPE_FLOAT1, "0.0f"},
			{"f0", "f0", GXDECLTYPE_FLOAT1, "0.04f"},
			{"Roughness", "fRoughness", GXDECLTYPE_FLOAT1, "0.5f"},
			{"Metallic", "fMetallic", GXDECLTYPE_FLOAT1, "0.5f"},
			{"Thickness", "fThickness", GXDECLTYPE_FLOAT1, "1.0f"},
			{"Lighting Coefficient", "fLightCoeff", GXDECLTYPE_FLOAT1, "1.0f"},

			// {"Depth", "fDepth", GXDECLTYPE_FLOAT1, "IN.vPos.z"},

			{"Height", "fHeight", GXDECLTYPE_FLOAT1, "1.0f"},
			XRENDER_PASS_OUTPUT_LIST_END()
		};

		m_pRenderPassGBuffer = m_pMaterialSystem->registerRenderPass("xGBuffer", "material/gbuffer.ps", NULL, pSamplers, pOutput);
	}

	{
		XRenderPassTexturesElement pTextures[] = {
			{"GBuffer color(rgb) light(a)", "txGBufferC3L1", 0},
			{"GBuffer normals(rgb) f0(a)", "txGBufferN3F1", 1},
			{"GBuffer depth(r)", "txGBufferD1", 2},
			{"", "", 3}, // reserved slot
			// {"GBuffer roughness(r) metallic(g) thickness(b) AO(a)", "g_txGBufferR1M1T1AO1", 3},
			{"Lighted scene", "txScene", 4},
			XRENDER_PASS_TEXTURES_LIST_END()
		}; 
		
		XRenderPassSamplersElement pSamplers[] = {
			{"Scene default", "sScene", 0},
			{"Point clamp", "sPointClamp", 1},
			XRENDER_PASS_SAMPLERS_LIST_END()
		};

		XRenderPassOutputElement pOutput[] = {
			{"Base color", "vBaseColor", GXDECLTYPE_FLOAT4, "float4(1.0f, 0.0f, 0.0f, 0.5f)"},
			{"Normal", "vNormal", GXDECLTYPE_FLOAT3, "float3(0.0f, 0.0f, 0.0f)"},
			{"AO", "fAO", GXDECLTYPE_FLOAT1, "0.0f"},
			{"f0", "f0", GXDECLTYPE_FLOAT1, "0.04f"},
			{"Roughness", "fRoughness", GXDECLTYPE_FLOAT1, "0.5f"},
			{"Metallic", "fMetallic", GXDECLTYPE_FLOAT1, "0.5f"},
			{"Thickness", "fThickness", GXDECLTYPE_FLOAT1, "1.0f"},
			{"Lighting Coefficient", "fLightCoeff", GXDECLTYPE_FLOAT1, "1.0f"},
			{"Emissive color", "fEmissiveColor", GXDECLTYPE_FLOAT3, "float3(0.0f, 0.0f, 0.0f)"},
			XRENDER_PASS_OUTPUT_LIST_END()
		};

		m_pRenderPassTransparency = m_pMaterialSystem->registerRenderPass("xTransparency", "material/transparent.ps", pTextures, pSamplers, pOutput);
	}

	{
		XRenderPassTexturesElement pTextures[] = {
			{"GBuffer color(rgb) light(a)", "g_txGBufferC3L1", 0},
			{"GBuffer normals(rgb) f0(a)", "g_txGBufferN3F1", 1},
			{"GBuffer depth(r)", "g_txGBufferD1", 2},
			{"", "", 3}, // reserved slot
			// {"GBuffer roughness(r) metallic(g) thickness(b) AO(a)", "g_txGBufferR1M1T1AO1", 3},
			{"Lighted scene", "g_txScene", 4},
			XRENDER_PASS_TEXTURES_LIST_END()
		};

		XRenderPassSamplersElement pSamplers[] = {
			{"Scene default", "sScene", 0},
			{"Point clamp", "sPointClamp", 1},
			XRENDER_PASS_SAMPLERS_LIST_END()
		};

		XRenderPassOutputElement pOutput[] = {
			{"Emissive color", "vEmissiveColor", GXDECLTYPE_FLOAT3, "float3(0.0f, 0.0f, 0.0f)"},
			{"Normal", "vNormal", GXDECLTYPE_FLOAT3, "float3(0.0f, 0.0f, 0.0f)"},
			XRENDER_PASS_OUTPUT_LIST_END()
		};

		m_pRenderPassIllumination = m_pMaterialSystem->registerRenderPass("xIllumination", "material/illum.ps", pTextures, pSamplers, pOutput);
	}

	{
		XRenderPassTexturesElement pTextures[] = {
			{"GBuffer color(rgb) light(a)", "g_txGBufferC3L1", 0},
			{"GBuffer normals(rgb) f0(a)", "g_txGBufferN3F1", 1},
			{"GBuffer depth(r)", "g_txGBufferD1", 2},
			{"", "", 3}, // reserved slot
			// {"GBuffer roughness(r) metallic(g) thickness(b) AO(a)", "g_txGBufferR1M1T1AO1", 3},
			{"Lighted scene", "g_txScene", 4},
			XRENDER_PASS_TEXTURES_LIST_END()
		};

		XRenderPassSamplersElement pSamplers[] = {
			{"Scene default", "sScene", 0},
			{"Point clamp", "sPointClamp", 1},
			XRENDER_PASS_SAMPLERS_LIST_END()
		};

		XRenderPassOutputElement pOutput[] = {
			{"Color", "vColor", GXDECLTYPE_FLOAT3, "float3(1.0f, 0.0f, 0.0f)"},
			XRENDER_PASS_OUTPUT_LIST_END()
		};

		m_pRenderPassPostprocess = m_pMaterialSystem->registerRenderPass("xPostprocess", "material/post.ps", pTextures, pSamplers, pOutput);
	}

	{
		XRenderPassSamplersElement pSamplers[] = {
			{"Scene default", "sScene", 0},
			// {"Point clamp", "sPointClamp", 1},
			XRENDER_PASS_SAMPLERS_LIST_END()
		};

		XRenderPassOutputElement pOutput[] = {
			{"Base color", "vBaseColor", GXDECLTYPE_FLOAT4, "float4(0.0f, 0.0f, 0.0f, 0.0f)"},
			{"Normal", "vNormal", GXDECLTYPE_FLOAT3, "float3(0.0f, 0.0f, 0.0f)"},
			XRENDER_PASS_OUTPUT_LIST_END()
		};

		GXMacro pVariant1[] = {
			{"IS_CUBEMAP", "1"},
			{NULL, NULL}
		}; 
		GXMacro pVariant2[] = {
			{"ENABLE_RSM", "1"},
			{NULL, NULL}
		};
		GXMacro pVariant3[] = {
			{"IS_CUBEMAP", "1"},
			{"ENABLE_RSM", "1"},
			{NULL, NULL}
		};
		XRenderPassVariantElement pVariants[] = {
			pVariant1,
			pVariant2,
			pVariant3,
			NULL
		};

		m_pRenderPassShadow = m_pMaterialSystem->registerRenderPass("xShadow", "material/shadow.ps", NULL, pSamplers, pOutput, pVariants);
	}

	{
		XMaterialShaderSampler pSamplers[] = {
			{"sDefault", GXSamplerDesc()},
			XMATERIAL_SHADER_SAMPLER_LIST_END()
		};
		
		XMaterialProperty pProperties[] = {
			XMATERIAL_PARAM_TEXTURE_OPT("Base texture", "txBase", NULL), // will override generic parameter if supplied
		//	XMATERIAL_PARAM_COLOR("Some color", "vColor", float4_t(1.0f, 1.0f, 1.0f, 1.0f)),
			XMATERIAL_PARAM_FLAG("Use f0 texture", "has_f0_texture", "HAS_F0MAP"),
			XMATERIAL_PARAM_GROUP(NULL, "HAS_F0MAP"),
				XMATERIAL_PARAM_TEXTURE("f0 texture", "txF0"),
			//	XMATERIAL_PARAM_FLAG("Use f0 texture", "has_f0_texture", "HAS_F0MAP2"),
			XMATERIAL_PARAM_GROUP_END(),
			XMATERIAL_PARAM_GROUP(NULL, "!HAS_F0MAP"),
				// XMATERIAL_PARAM_FLAG("Use f0 texture", "has_f0_texture", "HAS_F0MAP2"),
				XMATERIAL_PARAM_RANGE("f0", "pbr_f0", 0.0f, 1.0f, 0.04f),
			XMATERIAL_PARAM_GROUP_END(),
			XMATERIAL_PROPERTY_LIST_END()
		};

		XMaterialShaderPass pPasses[] = {
			{m_pRenderPassGBuffer, "default/default.ps", "MainGBuffer", NULL, pSamplers, pProperties},
			{m_pRenderPassIllumination, "default/default.ps", "MainIllimination", NULL, NULL, NULL},
			{m_pRenderPassShadow, "default/default.ps", "MainShadow", NULL, NULL, NULL},
			XMATERIAL_SHADER_PASS_LIST_END()
		};

		XMaterialProperty pGenericProperties[] = {
			// parameter name, texture name
			XMATERIAL_PARAM_TEXTURE("Base texture", "txBase"),
			// parameter name, define_if_supplied
			XMATERIAL_PARAM_TEXTURE_OPT("Normal map", "txNormals", "HAS_NORMALMAP"),
			XMATERIAL_PARAM_GROUP(NULL, "HAS_NORMALMAP"),
				XMATERIAL_PARAM_RANGE("Normalmap weight", "nm_weight", -1.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_GROUP_END(),
			// parameter name, material parameter name, define_if_set
			XMATERIAL_PARAM_FLAG("Use param texture", "has_parameter_texture", "HAS_PARAMMAP"),
			// group name (optional), condition (define)
			XMATERIAL_PARAM_GROUP(NULL, "HAS_PARAMMAP"),
				XMATERIAL_PARAM_TEXTURE("Param texture", "txParameters"),
			XMATERIAL_PARAM_GROUP_END(),
			XMATERIAL_PARAM_GROUP(NULL, "!HAS_PARAMMAP"),
				// name, min, max, material parameter name
				XMATERIAL_PARAM_RANGE("Roughness", "pbr_roughness", 0.0f, 1.0f, 0.5f),
				XMATERIAL_PARAM_RANGE("Metallic", "pbr_metallic", 0.0f, 1.0f, 0.0f),
				XMATERIAL_PARAM_RANGE("Thickness", "pbr_thickness", 0.0f, 1.0f, 1.0f),
				XMATERIAL_PARAM_RANGE("AO", "pbr_ao", 0.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_GROUP_END(),

			XMATERIAL_PARAM_FLAG("Enable alphatest", "has_alphatest", "HAS_ALPHATEST"),
			XMATERIAL_PARAM_GROUP(NULL, "HAS_ALPHATEST"),
				XMATERIAL_PARAM_RANGE("Alphatest threshold", "pbr_alphatest_threshold", 0.0f, 1.0f, 0.8f),
			XMATERIAL_PARAM_GROUP_END(),
			XMATERIAL_PROPERTY_LIST_END()
		};

		m_pMaterialSystem->registerMaterialShader("Default", pVertexFormatSceneGeneric, pPasses, pGenericProperties);
	}

	{
		GXMacro pMacro[] = {
			{"WANT_WRITE_DEPTH", "1"},
			GX_MACRO_END()
		};

		XMaterialShaderPass pPasses[] = {
			{m_pRenderPassGBuffer, "default/sky.ps", "MainGBuffer", pMacro, NULL, NULL},
			//{m_pRenderPassIllumination, "default/sky.ps", "MainIllimination", NULL, NULL, NULL},
			XMATERIAL_SHADER_PASS_LIST_END()
		};

		XMaterialProperty pGenericProperties[] = {
			// parameter name, texture name
			XMATERIAL_PARAM_TEXTURE_CUBE("Base texture", "txBase"),
			// parameter name, define_if_supplied
			XMATERIAL_PROPERTY_LIST_END()
		};

		m_pMaterialSystem->registerMaterialShader("Sky", pVertexFormatSkyBox, pPasses, pGenericProperties);
	}

	{
		XMaterialShaderPass pPasses[] = {
			{m_pRenderPassTransparency, "default/transparent.ps", "MainTransparency", NULL, NULL, NULL},
			{m_pRenderPassIllumination, "default/transparent.ps", "MainIllimination", NULL, NULL, NULL},
			{m_pRenderPassShadow, "default/transparent.ps", "MainShadow", NULL, NULL, NULL},
			XMATERIAL_SHADER_PASS_LIST_END()
		};

		XMaterialProperty pGenericProperties[] = {
			XMATERIAL_STATIC_FLAG("transparent", true),
			// parameter name, texture name
			XMATERIAL_PARAM_TEXTURE("Base texture", "txBase"),
			// parameter name, define_if_supplied
			XMATERIAL_PARAM_TEXTURE_OPT("Normal map", "txNormals", "HAS_NORMALMAP"),
			XMATERIAL_PARAM_GROUP(NULL, "HAS_NORMALMAP"),
				XMATERIAL_PARAM_RANGE("Normalmap weight", "nm_weight", -1.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_GROUP_END(),
			// parameter name, material parameter name, define_if_set
			XMATERIAL_PARAM_FLAG("Refraction", "refractive", "HAS_REFRACTION"),
			XMATERIAL_PARAM_FLAG("Use param texture", "has_parameter_texture", "HAS_PARAMMAP"),
			// group name (optional), condition (define)
			XMATERIAL_PARAM_GROUP(NULL, "HAS_PARAMMAP"),
				XMATERIAL_PARAM_TEXTURE("Param texture", "txParameters"),
			XMATERIAL_PARAM_GROUP_END(),
			XMATERIAL_PARAM_GROUP(NULL, "!HAS_PARAMMAP"),
				// name, min, max, material parameter name
				XMATERIAL_PARAM_RANGE("Roughness", "pbr_roughness", 0.0f, 1.0f, 0.5f),
				XMATERIAL_PARAM_RANGE("Metallic", "pbr_metallic", 0.0f, 1.0f, 0.0f),
				XMATERIAL_PARAM_RANGE("Thickness", "pbr_thickness", 0.0f, 1.0f, 1.0f),
				XMATERIAL_PARAM_RANGE("AO", "pbr_ao", 0.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_GROUP_END(),

			XMATERIAL_PROPERTY_LIST_END()
		};

		m_pMaterialSystem->registerMaterialShader("Transparent", pVertexFormatSceneGeneric, pPasses, pGenericProperties);
	}

	{
		XMaterialShaderPass pPasses[] = {
			{m_pRenderPassPostprocess, "default/postprocess.ps", "MainPostprocess", NULL, NULL, NULL},
			XMATERIAL_SHADER_PASS_LIST_END()
		};

		m_pMaterialSystem->registerMaterialShader("Postprocess", pVertexFormatPostprocess, pPasses, NULL);
	}

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
	dsDesc.cmpFuncDepth = GXCMP_GREATER_EQUAL;
	m_pDepthStencilStateDefault = m_pDevice->createDepthStencilState(&dsDesc);

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

	m_pLPVcentersShaderData = m_pDevice->createConstantBuffer(sizeof(m_lpvCentersShaderData.vs));
	m_pLPVcurrentCascadeShaderData = m_pDevice->createConstantBuffer(sizeof(float4_t));

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
	for(UINT i = 0; i < 3; ++i)
	{
		m_aLPVs[i].pGIAccumRed = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
		m_aLPVs[i].pGIAccumGreen = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
		m_aLPVs[i].pGIAccumBlue = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
		
		m_aLPVs[i].pGIAccumRed2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
		m_aLPVs[i].pGIAccumGreen2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);
		m_aLPVs[i].pGIAccumBlue2 = m_pDevice->createTexture3D(32, 32, 32, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_UNORDERED_ACCESS, GXFMT_A32B32G32R32F);

	}
//#undef TIDX

	m_idLightBoundShader = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "lighting_bound.vs"), -1);
	// m_idLightBoundShader = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "lighting_bound.vs"), 
	// 	SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_bound_debug.ps"));

	m_idLPVPropagateShader = SGCore_ShaderCreateKit(-1, -1, -1, SGCore_ShaderLoad(SHADER_TYPE_COMPUTE, "gi_propagation.cs"));

	m_pShadowCache = new CShadowCache(this, m_pMaterialSystem);
	m_pShadowShaderDataVS = m_pDevice->createConstantBuffer(sizeof(m_shadowShaderData.vs));

	
	newVisData(&m_pMainCameraVisibility);
	m_pMainCameraOcclusionCuller = new COcclusionCuller();
	//m_pMainCameraVisibility->setOcclusionCuller(m_pMainCameraOcclusionCuller);


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

	mem_release(m_pMainCameraOcclusionCuller);
	mem_release(m_pMainCameraVisibility);

	//mem_release(m_pSceneShaderDataVS);
	mem_release(m_pSceneShaderDataPS);

	mem_release(m_pCameraShaderDataVS);

	mem_release(m_pLPVcentersShaderData);
	mem_release(m_pLPVcurrentCascadeShaderData);

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

	for(UINT i = 0; i < 3; ++i)
	{
		mem_release(m_aLPVs[i].pGIAccumRed);
		mem_release(m_aLPVs[i].pGIAccumGreen);
		mem_release(m_aLPVs[i].pGIAccumBlue);

		mem_release(m_aLPVs[i].pGIAccumRed2);
		mem_release(m_aLPVs[i].pGIAccumGreen2);
		mem_release(m_aLPVs[i].pGIAccumBlue2);
	}

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

	IGXContext *pCtx = m_pDevice->getThreadContext();

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
	const bool *dev_lpv_cubes = GET_PCVAR_BOOL("dev_lpv_cubes");
	if(*dev_lpv_cubes)
	{
		showGICubes();
	}
	
	showFrameStats();

	getXUI()->render();
}
void CRenderPipeline::endFrame()
{
	m_pDevice->swapBuffers();
	getXUI()->present();
}

void CRenderPipeline::updateVisibility()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	// m_pMainCameraOcclusionCuller->update(gdata::pCamera, gdata::fProjFov, (float)*r_win_width / (float)*r_win_height, *r_far);
	m_pMainCameraVisibility->updateForCamera(gdata::pCamera);

	if(m_pLightSystem)
	{
		float3 vCamPos;
		gdata::pCamera->getPosition(&vCamPos);
		//! @todo: fix grid center pos!
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
	IGXContext *pCtx = m_pDevice->getThreadContext();

	pCtx->setPrimitiveTopology(GXPT_POINTLIST);
	pCtx->setRenderBuffer(m_pGICubesRB);
	SGCore_ShaderBind(m_idGICubesShader);
	pCtx->setGSConstant(m_pLightingShaderDataVS, 1);
	pCtx->setDepthStencilState(m_pDepthStencilStateDefault);
	pCtx->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);

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

void CRenderPipeline::renderPrepare()
{
	renderStage(XRS_PREPARE);
}
void CRenderPipeline::renderGBuffer()
{
	IGXContext *pCtx = m_pDevice->getThreadContext();

	m_pMaterialSystem->bindRenderPass(m_pRenderPassGBuffer);

	pCtx->setRasterizerState(NULL);
	pCtx->setDepthStencilState(m_pDepthStencilStateDefault);
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
	pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, RENDER_DEFAUL_BACKGROUND_COLOR, 0.0f);

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

	/*if(GetAsyncKeyState('U') < 0)
	{
		m_pDevice->saveTextureToFile("sm_color.dds", m_pGBufferColor);
		m_pDevice->saveTextureToFile("sm_shadow.dds", m_pGBufferDepth);
	}*/
}
void CRenderPipeline::renderShadows()
{
	IGXContext *pCtx = m_pDevice->getThreadContext();

	rfunc::SetRenderSceneFilter();
	pCtx->setSamplerState(gdata::rstates::pSamplerPointClamp, 1);
}
void CRenderPipeline::renderGI()
{
	if(!m_pLightSystem)
	{
		return;
	}

	m_pMaterialSystem->bindRenderPass(m_pRenderPassShadow);

	IGXContext *pCtx = m_pDevice->getThreadContext();

	IGXDepthStencilSurface *pOldDSSurface = pCtx->getDepthStencilSurface();

	UINT uCounts[LIGHT_TYPE__COUNT] = {0};
	for(int i = 0, l = m_pLightSystem->getCount(); i < l; ++i)
	{
		++uCounts[m_pLightSystem->getLight(i)->getType()];
	}
	m_pShadowCache->setLightsCount(uCounts[LIGHT_TYPE_POINT], uCounts[LIGHT_TYPE_SPOT], uCounts[LIGHT_TYPE_SUN] != 0);
	m_pShadowCache->setRSMLightsCount(uCounts[LIGHT_TYPE_POINT], uCounts[LIGHT_TYPE_SPOT], uCounts[LIGHT_TYPE_SUN] != 0);

	m_pShadowCache->setObserverCamera(gdata::pCamera);

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

	float3 vCamDir;
	gdata::pCamera->getLook(&vCamDir);

	const float c_aLPVsizes[] = {
		//0.5f,
		//1.0f,
		//2.0f

		1.0f,
		2.0f,
		4.0f
	};

	m_lpvCentersShaderData.vs.vCenterSize[0] = float4(gdata::vConstCurrCamPos + vCamDir * (LPV_GRID_SIZE / 2 - LPV_STEP_COUNT) * c_aLPVsizes[0], c_aLPVsizes[0]);
	m_lpvCentersShaderData.vs.vCenterSize[1] = float4(gdata::vConstCurrCamPos + vCamDir * (LPV_GRID_SIZE / 2 - LPV_STEP_COUNT) * c_aLPVsizes[1], c_aLPVsizes[1]);
	m_lpvCentersShaderData.vs.vCenterSize[2] = float4(gdata::vConstCurrCamPos + vCamDir * (LPV_GRID_SIZE / 2 - LPV_STEP_COUNT) * c_aLPVsizes[2], c_aLPVsizes[2]);
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
	for(int i = 0, l = m_pLightSystem->getCount(); i < l; ++i)
	{
		pLight = m_pLightSystem->getLight(i);

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

	pCtx->setRasterizerState(gdata::rstates::pRasterizerConservative);

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
			
			//если не глобальный источник
			if(pLight->getType() != LIGHT_TYPE_SUN)
			{
				//помечаем в стенсил буфере пиксели  которые входят в ограничивающий объем света, чтобы их осветить
				pCtx->setRasterizerState(gdata::rstates::pRasterizerCullNone);
				pCtx->setStencilRef(0);
				pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateLightBound);

				//отрисовка ограничивающего объема
				SGCore_ShaderBind(m_idLightBoundShader);
				pLight->drawShape(m_pDevice);
				
				pCtx->setStencilRef(255);
				pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateLightShadowNonGlobal);
			}
			else
			{
				//иначе это глобальный источник, отключаем стенсил тест
				pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateLightShadowGlobal);
			}

			pCtx->setRasterizerState(NULL);



			//pCtx->setVSConstant(m_pShadowShaderDataVS, 1);
			pCtx->setVSConstant(m_pLightingShaderDataVS, 1);

			pCtx->setBlendState(gdata::rstates::pBlendRed);
			pShadow->genShadow(m_pShadow, m_pGBufferDepth, m_pGBufferNormals);
			pCtx->setBlendState(gdata::rstates::pBlendAlphaOneOne);
			pCtx->setColorTarget(pAmbientSurf);
			pCtx->setColorTarget(pSpecDiffSurf, 1);

		//	pCtx->setPSTexture(m_pShadow, 4);
			//idshaderkit = gdata::shaders_id::kit::idComLightingShadow;
			ID idshaderkit = -1;
			switch(pLight->getType())
			{
			case LIGHT_TYPE_SPOT:
				idshaderkit = gdata::shaders_id::kit::idComLightingSpotShadow;
				break;
			case LIGHT_TYPE_POINT: 
				idshaderkit = gdata::shaders_id::kit::idComLightingShadow;
				break;
			case LIGHT_TYPE_SUN:
				idshaderkit = gdata::shaders_id::kit::idComLightingPSSMShadow;
				break;
			default:
				assert(!"Unknown light type!");
			}

			pCtx->setVSConstant(m_pLightingShaderDataVS, 1);
			
			//теперь когда будем считать освещение надо сбросить значения в стенсил буфере, чтобы каждый кадр не чистить
			//если стенсил тест прошел успешно, устанавливаем значнеие в нуль
			if(pLight->getType() != LIGHT_TYPE_SUN)
			{
				pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateLightClear);
			}

			IGXConstantBuffer *pLightConstants = pLight->getConstants(m_pDevice);
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
		pCtx->setBlendState(gdata::rstates::pBlendAlphaOneOne);

		pCtx->setVSConstant(m_pLightingShaderDataVS, 1);
		pCtx->setPSConstant(m_pLightingShaderDataPS, 1);

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
			IGXSurface *pLPVRed =   m_aLPVs[i].pGIAccumRed->asRenderTarget();
			IGXSurface *pLPVGreen = m_aLPVs[i].pGIAccumGreen->asRenderTarget();
			IGXSurface *pLPVBlue =  m_aLPVs[i].pGIAccumBlue->asRenderTarget();

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

	pCtx->setVSConstant(m_pLightingShaderDataVS, 1);
	pCtx->setPSConstant(m_pLightingShaderDataPS, 1);

	{
		SGCore_ShaderBind(m_idLPVPropagateShader);
		UINT uStepCount[] = {4, 6, 8};
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

				pCtx->computeDispatch(2, 16, 32);

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

				pCtx->computeDispatch(2, 16, 32);

				pCtx->setCSUnorderedAccessView(NULL, 0);
				pCtx->setCSUnorderedAccessView(NULL, 1);
				pCtx->setCSUnorderedAccessView(NULL, 2);

				pCtx->setCSTexture(NULL, 0);
				pCtx->setCSTexture(NULL, 1);
				pCtx->setCSTexture(NULL, 2);
			}
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

	//	pCtx->setPSConstant(m_pCameraShaderDataVS, 8);

		SGCore_ShaderBind(idshaderkit);

		pCtx->setSamplerState(gdata::rstates::pSamplerPointClamp, 0);
		pCtx->setSamplerState(gdata::rstates::pSamplerLinearBorder, 1);

		pCtx->setPSTexture(m_pGBufferDepth);
		pCtx->setPSTexture(m_pGBufferNormals, 1);
		for(UINT i = 0; i < 3; ++i)
		{
			pCtx->setPSTexture(m_aLPVs[i].pGIAccumRed,   2 + i);
			pCtx->setPSTexture(m_aLPVs[i].pGIAccumGreen, 5 + i);
			pCtx->setPSTexture(m_aLPVs[i].pGIAccumBlue,  8 + i);
		}
		//r2  |0
		//r3  |1
		//r4  |2
		//g5  |3
		//g6  |4
		//g7  |5
		//b8  |6
		//b9  |7
		//b10 |8

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

	//Sleep(16);
}
void CRenderPipeline::renderPostprocessMain()
{
	rfunc::SetRenderSceneFilter();

	m_pMaterialSystem->bindRenderPass(m_pRenderPassPostprocess);

	renderStage(XRS_POSTPROCESS_MAIN);
}
void CRenderPipeline::renderTransparent()
{
	assert(m_pMainCameraVisibility && m_pMainCameraVisibility->getPluginId() == -1);
	CRenderableVisibility *pVis = (CRenderableVisibility*)m_pMainCameraVisibility;

	IGXContext *pCtx = m_pDevice->getThreadContext();

	//pCtx->setPSConstant(m_pSceneShaderDataPS, SCR_SCENE);
	pCtx->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);
	rfunc::SetRenderSceneFilter();

	m_pMaterialSystem->bindRenderPass(m_pRenderPassTransparency);

//	pCtx->setRasterizerState(NULL);

	pCtx->setDepthStencilState(m_pDepthStencilStateNoZWrite);
	pCtx->setBlendState(m_pBlendStateAlpha);
	pCtx->setSamplerState(m_pRefractionScene, 1);
	//m_pDevice->setTexture(m_pSceneTexture, 11);
	pCtx->setPSTexture(m_pGBufferColor);
	pCtx->setPSTexture(m_pGBufferNormals, 1);
	pCtx->setPSTexture(m_pGBufferDepth, 2);
	pCtx->setPSTexture(m_pGBufferParams, 3);

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

	if(aNodes.size())
	{
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
	}

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

	IGXContext *pCtx = m_pDevice->getThreadContext();

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
			pCtx->setPSTexture(m_pRefractiveTextureRead, 4);

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
	rfunc::SetRenderSceneFilter();

	m_pMaterialSystem->bindRenderPass(m_pRenderPassPostprocess);

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
	m_pDevice->getThreadContext()->setVSConstant(m_pCameraShaderDataVS, SCR_CAMERA);

	renderStage(XRS_EDITOR_2D);
}

IXUI* CRenderPipeline::getXUI()
{
	if(!m_pXUI)
	{
		m_pXUI = (IXUI*)Core_GetIXCore()->getPluginManager()->getInterface(IXUI_GUID);
	}

	return(m_pXUI);
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
	IGXContext *pCtx = m_pDevice->getThreadContext();

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
