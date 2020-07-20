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

	Core_0RegisterCVarBool("r_clear_color", false, "Очищать буфер перед выводом кадра");

	Core_0RegisterCVarFloat("hdr_adapted_coef", 0.3f, "Коэфициент привыкания к освещению (0,1] (медлено, быстро)");
	Core_0RegisterCVarFloat("hdr_base_value", 0.2f, "Базовое значение для тонмаппинга  (0,0.5] (темно, ярко)");
	
	XVertexOutputElement voelGeneric[] = {
		{"vPosition", GXDECLTYPE_FLOAT4, GXDECLUSAGE_POSITION},
		{"vTexUV", GXDECLTYPE_FLOAT2, GXDECLUSAGE_TEXCOORD},
		{"vNormal", GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD1},
		{"vPos", GXDECLTYPE_FLOAT4, GXDECLUSAGE_TEXCOORD2},
		{"vTangent", GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD3},
		{"vBinormal", GXDECLTYPE_FLOAT3, GXDECLUSAGE_TEXCOORD4},
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
			// {"GBuffer normals(rgb) f0(a)", "txGBufferN3F1", 1},
			{"GBuffer depth(r)", "txGBufferD1", 2},
			// {"", "3", 3}, // reserved slot
			// {"GBuffer roughness(r) metallic(g) thickness(b) AO(a)", "g_txGBufferR1M1T1AO1", 3},
			{"Lighted scene", "txScene", 4},

			{"LPV Red cascades", "txLightVolumeRed[3]", 5, GXTEXTURE_TYPE_3D},
			{"", "6", 6},
			{"", "7", 7},
			{"LPV Green cascades", "txLightVolumeGreen[3]", 8, GXTEXTURE_TYPE_3D},
			{"", "9", 9},
			{"", "10", 10},
			{"LPV Red cascades", "txLightVolumeBlue[3]", 11, GXTEXTURE_TYPE_3D},
			{"", "12", 12},
			{"", "13", 13},
			XRENDER_PASS_TEXTURES_LIST_END()
		}; 
		
		XRenderPassSamplersElement pSamplers[] = {
			{"Scene default", "sScene", 0},
			{"Point clamp", "sPointClamp", 1},
			{"Linear clamp", "sLinearClamp", 2},
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
		//	{"GBuffer color(rgb) light(a)", "g_txGBufferC3L1", 0},
		//	{"GBuffer normals(rgb) f0(a)", "g_txGBufferN3F1", 1},
		//	{"GBuffer depth(r)", "g_txGBufferD1", 2},
		//	{"", "", 3}, // reserved slot
		//	// {"GBuffer roughness(r) metallic(g) thickness(b) AO(a)", "g_txGBufferR1M1T1AO1", 3},
		//	{"Lighted scene", "g_txScene", 4},
			XRENDER_PASS_TEXTURES_LIST_END()
		};

		XRenderPassSamplersElement pSamplers[] = {
			{"Scene default", "sScene", 0},
		//	{"Point clamp", "sPointClamp", 1},
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

			XMATERIAL_PARAM_FLAG("Emissive", "emissive", "HAS_EMISSION"),
			XMATERIAL_PARAM_GROUP(NULL, "HAS_EMISSION"),
				XMATERIAL_PARAM_TEXTURE_OPT("Emissive map", "txEmissive", "HAS_EMISSIVE_MAP"),
				XMATERIAL_PARAM_RANGE("Emissive multiplier", "em_multiplier", 0.0f, 1000.0f, 16.0f),
			XMATERIAL_PARAM_GROUP_END(),
			XMATERIAL_PROPERTY_LIST_END()
		};

		m_pMaterialSystem->registerMaterialShader("Default", pVertexFormatSceneGeneric, pPasses, pGenericProperties);
	}

	{
		XMaterialShaderSampler pSamplers[] = {
			{"sDefault", GXSamplerDesc()},
			XMATERIAL_SHADER_SAMPLER_LIST_END()
		};

		XMaterialShaderPass pPasses[] = {
			{m_pRenderPassGBuffer, "default/terrain.ps", "MainGBuffer", NULL, pSamplers, NULL},
			{m_pRenderPassShadow, "default/terrain.ps", "MainShadow", NULL, NULL, NULL},
			XMATERIAL_SHADER_PASS_LIST_END()
		};

		XMaterialProperty pGenericProperties[] = {
			// parameter name, texture name
			XMATERIAL_PARAM_TEXTURE("Base texture", "txBase"),

			XMATERIAL_PARAM_TEXTURE("Mask texture", "txMask"),
			XMATERIAL_PARAM_TEXTURE("Detail texture 0", "txDetail0"),
			XMATERIAL_PARAM_TEXTURE("Detail texture 1", "txDetail1"),
			XMATERIAL_PARAM_TEXTURE("Detail texture 2", "txDetail2"),
			XMATERIAL_PARAM_TEXTURE("Detail texture 3", "txDetail3"),

			XMATERIAL_PARAM_RANGE("Base scale", "terrain_base_scale", 0.0f, 100.0f, 2.0f),
			XMATERIAL_PARAM_RANGE("Detail scale", "terrain_detail_scale", 0.0f, 100.0f, 10.0f),
			XMATERIAL_PARAM_RANGE("Mask scale", "terrain_mask_scale", 0.0f, 100.0f, 1.0f),

			XMATERIAL_PARAM_RANGE("Detail texture 0 multiplier", "terrain_mask_mult_0", 0.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_RANGE("Detail texture 1 multiplier", "terrain_mask_mult_1", 0.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_RANGE("Detail texture 2 multiplier", "terrain_mask_mult_2", 0.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_RANGE("Detail texture 3 multiplier", "terrain_mask_mult_3", 0.0f, 1.0f, 1.0f),

			//XMATERIAL_PARAM_TEXTURE_OPT("Normal map", "txNormals", "HAS_NORMALMAP"),
			//XMATERIAL_PARAM_GROUP(NULL, "HAS_NORMALMAP"),
			//	XMATERIAL_PARAM_RANGE("Normalmap weight", "nm_weight", -1.0f, 1.0f, 1.0f),
			//XMATERIAL_PARAM_GROUP_END(),

		//	XMATERIAL_PARAM_FLAG("Use param texture", "has_parameter_texture", "HAS_PARAMMAP"),
		//	// group name (optional), condition (define)
		//	XMATERIAL_PARAM_GROUP(NULL, "HAS_PARAMMAP"),
		//		XMATERIAL_PARAM_TEXTURE("Param texture", "txParameters"),
		//	XMATERIAL_PARAM_GROUP_END(),
		//	XMATERIAL_PARAM_GROUP(NULL, "!HAS_PARAMMAP"),
				// name, min, max, material parameter name
				XMATERIAL_PARAM_RANGE("Roughness", "pbr_roughness", 0.0f, 1.0f, 0.9f),
				XMATERIAL_PARAM_RANGE("Metallic", "pbr_metallic", 0.0f, 1.0f, 0.0f),
				XMATERIAL_PARAM_RANGE("Thickness", "pbr_thickness", 0.0f, 1.0f, 1.0f),
				XMATERIAL_PARAM_RANGE("AO", "pbr_ao", 0.0f, 1.0f, 1.0f),
		//	XMATERIAL_PARAM_GROUP_END(),


			XMATERIAL_PARAM_TEXTURE_OPT("Normal map 0", "txNormals0", "HAS_NORMALMAP_0"),
			XMATERIAL_PARAM_GROUP(NULL, "HAS_NORMALMAP_0"),
				XMATERIAL_PARAM_RANGE("Normalmap 0 weight", "nm_weight_0", -1.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_GROUP_END(),

			// parameter name, material parameter name, define_if_set
		//	XMATERIAL_PARAM_FLAG("Use param texture 0", "has_parameter_texture_0", "HAS_PARAMMAP_0"),
		//	// group name (optional), condition (define)
		//	XMATERIAL_PARAM_GROUP(NULL, "HAS_PARAMMAP_0"),
		//		XMATERIAL_PARAM_TEXTURE("Param texture 0", "txParameters0"),
		//	XMATERIAL_PARAM_GROUP_END(),
		//	XMATERIAL_PARAM_GROUP(NULL, "!HAS_PARAMMAP_0"),
				// name, min, max, material parameter name
				XMATERIAL_PARAM_RANGE("Roughness 0", "pbr_roughness_0", 0.0f, 1.0f, 0.9f),
				XMATERIAL_PARAM_RANGE("Metallic 0", "pbr_metallic_0", 0.0f, 1.0f, 0.0f),
				XMATERIAL_PARAM_RANGE("Thickness 0", "pbr_thickness_0", 0.0f, 1.0f, 1.0f),
				XMATERIAL_PARAM_RANGE("AO 0", "pbr_ao_0", 0.0f, 1.0f, 1.0f),
		//	XMATERIAL_PARAM_GROUP_END(),


			XMATERIAL_PARAM_TEXTURE_OPT("Normal map 1", "txNormals1", "HAS_NORMALMAP_1"),
			XMATERIAL_PARAM_GROUP(NULL, "HAS_NORMALMAP_1"),
				XMATERIAL_PARAM_RANGE("Normalmap 1 weight", "nm_weight_1", -1.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_GROUP_END(),

			// parameter name, material parameter name, define_if_set
		//	XMATERIAL_PARAM_FLAG("Use param texture 1", "has_parameter_texture_1", "HAS_PARAMMAP_1"),
		//	// group name (optional), condition (define)
		//	XMATERIAL_PARAM_GROUP(NULL, "HAS_PARAMMAP_1"),
		//		XMATERIAL_PARAM_TEXTURE("Param texture 1", "txParameters1"),
		//	XMATERIAL_PARAM_GROUP_END(),
		//	XMATERIAL_PARAM_GROUP(NULL, "!HAS_PARAMMAP_1"),
				// name, min, max, material parameter name
				XMATERIAL_PARAM_RANGE("Roughness 1", "pbr_roughness_1", 0.0f, 1.0f, 0.9f),
				XMATERIAL_PARAM_RANGE("Metallic 1", "pbr_metallic_1", 0.0f, 1.0f, 0.0f),
				XMATERIAL_PARAM_RANGE("Thickness 1", "pbr_thickness_1", 0.0f, 1.0f, 1.0f),
				XMATERIAL_PARAM_RANGE("AO 1", "pbr_ao_1", 0.0f, 1.0f, 1.0f),
		//	XMATERIAL_PARAM_GROUP_END(),



			XMATERIAL_PARAM_TEXTURE_OPT("Normal map 2", "txNormals2", "HAS_NORMALMAP_2"),
			XMATERIAL_PARAM_GROUP(NULL, "HAS_NORMALMAP_2"),
				XMATERIAL_PARAM_RANGE("Normalmap 2 weight", "nm_weight_2", -1.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_GROUP_END(),
			
			// parameter name, material parameter name, define_if_set
		//	XMATERIAL_PARAM_FLAG("Use param texture 2", "has_parameter_texture_2", "HAS_PARAMMAP_2"),
		//	// group name (optional), condition (define)
		//	XMATERIAL_PARAM_GROUP(NULL, "HAS_PARAMMAP_2"),
		//		XMATERIAL_PARAM_TEXTURE("Param texture 2", "txParameters2"),
		//	XMATERIAL_PARAM_GROUP_END(),
		//	XMATERIAL_PARAM_GROUP(NULL, "!HAS_PARAMMAP_2"),
				// name, min, max, material parameter name
				XMATERIAL_PARAM_RANGE("Roughness 2", "pbr_roughness_2", 0.0f, 1.0f, 0.9f),
				XMATERIAL_PARAM_RANGE("Metallic 2", "pbr_metallic_2", 0.0f, 1.0f, 0.0f),
				XMATERIAL_PARAM_RANGE("Thickness 2", "pbr_thickness_2", 0.0f, 1.0f, 1.0f),
				XMATERIAL_PARAM_RANGE("AO 2", "pbr_ao_2", 0.0f, 1.0f, 1.0f),
		//	XMATERIAL_PARAM_GROUP_END(),
			

			XMATERIAL_PARAM_TEXTURE_OPT("Normal map 3", "txNormals3", "HAS_NORMALMAP_3"),
			XMATERIAL_PARAM_GROUP(NULL, "HAS_NORMALMAP_3"),
				XMATERIAL_PARAM_RANGE("Normalmap 3 weight", "nm_weight_3", -1.0f, 1.0f, 1.0f),
			XMATERIAL_PARAM_GROUP_END(),
			
			// parameter name, material parameter name, define_if_set
		//	XMATERIAL_PARAM_FLAG("Use param texture 3", "has_parameter_texture_3", "HAS_PARAMMAP_3"),
		//	// group name (optional), condition (define)
		//	XMATERIAL_PARAM_GROUP(NULL, "HAS_PARAMMAP_3"),
		//		XMATERIAL_PARAM_TEXTURE("Param texture 3", "txParameters3"),
		//	XMATERIAL_PARAM_GROUP_END(),
		//	XMATERIAL_PARAM_GROUP(NULL, "!HAS_PARAMMAP_3"),
				// name, min, max, material parameter name
				XMATERIAL_PARAM_RANGE("Roughness 3", "pbr_roughness_3", 0.0f, 1.0f, 0.9f),
				XMATERIAL_PARAM_RANGE("Metallic 3", "pbr_metallic_3", 0.0f, 1.0f, 0.0f),
				XMATERIAL_PARAM_RANGE("Thickness 3", "pbr_thickness_3", 0.0f, 1.0f, 1.0f),
				XMATERIAL_PARAM_RANGE("AO 3", "pbr_ao_3", 0.0f, 1.0f, 1.0f),
		//	XMATERIAL_PARAM_GROUP_END(),

			XMATERIAL_PROPERTY_LIST_END()
		};

		m_pMaterialSystem->registerMaterialShader("Terrain", pVertexFormatSceneGeneric, pPasses, pGenericProperties);
	}

	{
		GXMacro pMacro[] = {
			{"WANT_WRITE_DEPTH", "1"},
			GX_MACRO_END()
		};

		XMaterialShaderPass pPasses[] = {
			{m_pRenderPassGBuffer, "default/sky.ps", "MainGBuffer", pMacro, NULL, NULL},
			{m_pRenderPassIllumination, "default/sky.ps", "MainIllimination", NULL, NULL, NULL},
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
			XMATERIAL_PARAM_RANGE("Transparency multiplier", "tp_multiplier", 0.0f, 1.0f, 1.0f),
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
	m_pGBufferColor = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8B8G8R8);
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferNormals = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8B8G8R8);
	//GXFMT_A16B16G16R16F; // 64bpp; GXFMT_A8R8G8B8
	m_pGBufferParams = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A8B8G8R8);
	//GXFMT_G32R32F; // 64bpp; GXFMT_R32F
	m_pGBufferDepth = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R32F);

	if(m_pLightSystem)
	{
		m_pLightSystem->setRenderPipeline(this);
		m_pLightSystem->setGBuffer(m_pGBufferColor, m_pGBufferNormals, m_pGBufferParams, m_pGBufferDepth);
	}

	m_pLightAmbientDiffuse = m_pDevice->createTexture2D(m_uOutWidth, m_uOutHeight, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_A16B16G16R16F);
	
	
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

	m_pCameraShaderData = m_pDevice->createConstantBuffer(sizeof(m_cameraShaderData));

	m_pFrameShaderData = m_pDevice->createConstantBuffer(sizeof(m_frameShaderData));

	
	
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

	mem_release(m_pCameraShaderData);

	mem_release(m_pGBufferColor);
	mem_release(m_pGBufferNormals);
	mem_release(m_pGBufferParams);
	mem_release(m_pGBufferDepth);

	mem_release(m_pLightAmbientDiffuse);


	mem_release(m_pFrameShaderData);

	mem_release(m_pLightTotal);

	mem_release(m_pBlendStateAlpha);

	mem_release(m_pDepthStencilStateNoZWrite);
	mem_release(m_pDepthStencilStateNoZ);
}

void CRenderPipeline::resize(UINT uWidth, UINT uHeight, bool isWindowed)
{
	m_pDevice->resize(uWidth, uHeight, isWindowed);
	m_uOutWidth = uWidth;
	m_uOutHeight = uHeight;
	m_isWindowed = isWindowed;
}

void CRenderPipeline::renderFrame(float fDeltaTime)
{
	static const int *r_final_image = GET_PCVAR_INT("r_final_image");

	IGXContext *pCtx = m_pDevice->getThreadContext();
	IGXSurface *pBackBuf = pCtx->getColorTarget();
	IGXSurface *pSceneBuf = NULL;

	m_frameShaderData.fFrameTime = fDeltaTime;
	m_pFrameShaderData->update(&m_frameShaderData);

	pCtx->setVSConstant(m_pFrameShaderData, SCR_FRAME);
	pCtx->setPSConstant(m_pFrameShaderData, SCR_FRAME);

	m_sceneShaderData.vNearFarInvWinSize = float4(gdata::vNearFar, 1.0f / (float)m_uOutWidth, 1.0f / (float)m_uOutHeight);
	m_pSceneShaderDataPS->update(&m_sceneShaderData);
	//m_pDevice->setVertexShaderConstant(m_pSceneShaderDataVS, SCR_SCENE);
	pCtx->setPSConstant(m_pSceneShaderDataPS, SCR_SCENE);

	m_cameraShaderData.mVP = SMMatrixTranspose(gdata::mCamView * gdata::mCamProj);
	m_cameraShaderData.mInvVP = SMMatrixInverse(NULL, m_cameraShaderData.mVP);
	m_cameraShaderData.mInvV = SMMatrixTranspose(SMMatrixInverse(NULL, gdata::mCamView));
	m_cameraShaderData.vPosCam = gdata::vConstCurrCamPos;
	m_cameraShaderData.vNearFar = gdata::vNearFar;
	m_cameraShaderData.vParamProj = float3_t((float)m_uOutWidth, (float)m_uOutHeight, gdata::fProjFov);
	m_pCameraShaderData->update(&m_cameraShaderData);
	pCtx->setVSConstant(m_pCameraShaderData, SCR_CAMERA);
	pCtx->setPSConstant(m_pCameraShaderData, SCR_CAMERA);
	pCtx->setVSConstant(m_pCameraShaderData, SCR_OBSERVER_CAMERA);
	pCtx->setGSConstant(m_pCameraShaderData, SCR_OBSERVER_CAMERA);
	pCtx->setPSConstant(m_pCameraShaderData, SCR_OBSERVER_CAMERA);
	
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

	if(m_pLightSystem)
	{
		m_pLightSystem->renderGI(m_pLightTotal, m_pLightAmbientDiffuse);

		if(*r_final_image == DS_RT_AMBIENTDIFF)
		{
			showTexture(m_pLightAmbientDiffuse);
			goto end;
		}
		
		//m_pSceneTexture = m_pLightAmbientDiffuse;
		m_pSceneTexture = m_pLightTotal;

		pSceneBuf = m_pLightAmbientDiffuse->asRenderTarget();
		pCtx->setColorTarget(pSceneBuf);
	}
	else
	{
		m_pSceneTexture = m_pGBufferColor;
	}

	showTexture(m_pSceneTexture);

	renderPostprocessMain();
	renderTransparent();
	renderPostprocessFinal();

	if(m_pLightSystem)
	{
		pCtx->setColorTarget(pBackBuf);

		m_pLightSystem->renderToneMapping(m_pLightAmbientDiffuse);

	//! @todo reimplement me!
	//	if(*r_final_image == DS_RT_LUMINANCE)
	//	{
	//		showTexture(m_pLightLuminance);
	//		// showTexture(m_pLightLuminance32);
	//		goto end;
	//	}

		//showTexture(m_pLightAmbientDiffuse);
	}

end:
	mem_release(pBackBuf);
	mem_release(pSceneBuf);

	if(m_pLightSystem)
	{
		m_pLightSystem->renderDebug();
	}

	Core_PStartSection(PERF_SECTION_RENDER_INFO);
	//@FIXME: пока так
	SGame_RenderHUD();
	Core_PEndSection(PERF_SECTION_RENDER_INFO);
	
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
		//! @todo move to CRenderPipeline::setFrameObserverCamera() method
		m_pLightSystem->setFrameObserverCamera(gdata::pCamera);
		m_pLightSystem->updateVisibility();
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


void CRenderPipeline::renderPrepare()
{
	renderStage(XRS_PREPARE);
}
void CRenderPipeline::renderGBuffer()
{
	IGXContext *pCtx = m_pDevice->getThreadContext();

	static const bool *r_clear_color = GET_PCVAR_BOOL("r_clear_color");

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
//	pCtx->clear(GX_CLEAR_COLOR, GX_COLOR_ARGB(255, 255, 255, 255));

	pCtx->setColorTarget(pParamSurf);
	pCtx->setColorTarget(pNormalSurf, 1);
//	pCtx->clear(GX_CLEAR_COLOR);
	pCtx->setColorTarget(NULL, 1);

	pCtx->setColorTarget(pColorSurf);
	//pCtx->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH | GX_CLEAR_STENCIL, RENDER_DEFAUL_BACKGROUND_COLOR, 0.0f);
	pCtx->clear(GX_CLEAR_DEPTH | GX_CLEAR_STENCIL | (*r_clear_color ? GX_CLEAR_COLOR : 0), RENDER_DEFAUL_BACKGROUND_COLOR, 0.0f);

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
	//m_pMaterialSystem->bindRenderPass(m_pRenderPassShadow);
	rfunc::SetRenderSceneFilter();
	pCtx->setSamplerState(gdata::rstates::pSamplerPointClamp, 1);
}
void CRenderPipeline::renderGI()
{
	IGXContext *pCtx = m_pDevice->getThreadContext();
	rfunc::SetRenderSceneFilter();
	m_pMaterialSystem->bindRenderPass(m_pRenderPassIllumination);
	//pCtx->setDepthStencilState(gdata::rstates::pDepthStencilStateNoZ);
	renderStage(XRS_GI, m_pMainCameraVisibility);
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

	IGXSurface *pBackBuff = pCtx->getColorTarget();

	//pCtx->setPSConstant(m_pSceneShaderDataPS, SCR_SCENE);
	pCtx->setVSConstant(m_pCameraShaderData, SCR_CAMERA);
	rfunc::SetRenderSceneFilter();

	m_pMaterialSystem->bindRenderPass(m_pRenderPassTransparency);

//	pCtx->setRasterizerState(NULL);

	pCtx->setDepthStencilState(m_pDepthStencilStateNoZWrite);
	pCtx->setBlendState(m_pBlendStateAlpha);
	pCtx->setSamplerState(m_pRefractionScene, 1);
	pCtx->setSamplerState(gdata::rstates::pSamplerLinearClamp, 2);
	//m_pDevice->setTexture(m_pSceneTexture, 11);
	pCtx->setPSTexture(m_pGBufferColor);
//	pCtx->setPSTexture(m_pGBufferNormals, 1);
	pCtx->setPSTexture(m_pGBufferDepth, 2);
//	pCtx->setPSTexture(m_pGBufferParams, 3);

	//! @todo implement me!
	// for(UINT i = 0; i < 3; ++i)
	// {
	// 	pCtx->setPSTexture(m_aLPVs[i].pGIAccumRed, 5 + i);
	// 	pCtx->setPSTexture(m_aLPVs[i].pGIAccumGreen, 8 + i);
	// 	pCtx->setPSTexture(m_aLPVs[i].pGIAccumBlue, 11 + i);
	// }

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

		float3 vCamPos = gdata::pCamera->getPosition();

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
	pCtx->setColorTarget(pBackBuff);
	mem_release(pBackBuff);
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
	if(pNode->iPSP >= 0 && uPlaneCount < MAX_TRANSPARENCY_CLIP_PANES)
	{
		pPSPs[pNode->iPSP].isRenderFront = !isInFront;
		puPlanesStack[uPlaneCount++] = (UINT)pNode->iPSP;
		// assert(uPlaneCount <= MAX_TRANSPARENCY_CLIP_PANES);
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
}
void CRenderPipeline::renderEditor2D(IXRenderableVisibility *pVisibility)
{
	if(!pVisibility)
	{
		pVisibility = m_pMainCameraVisibility;
	}

	SMMATRIX mVP;
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &mVP);
	float3 vCamPos = SRender_GetCamera()->getPosition();

	m_cameraShaderData.mVP = SMMatrixTranspose(mVP);
	m_cameraShaderData.vPosCam = vCamPos;
	m_pCameraShaderData->update(&m_cameraShaderData);
	m_pDevice->getThreadContext()->setVSConstant(m_pCameraShaderData, SCR_CAMERA);
	m_pDevice->getThreadContext()->setVSConstant(m_pCameraShaderData, SCR_OBSERVER_CAMERA);
	m_pDevice->getThreadContext()->setPSConstant(m_pCameraShaderData, SCR_OBSERVER_CAMERA);
	m_pDevice->getThreadContext()->setGSConstant(m_pCameraShaderData, SCR_OBSERVER_CAMERA);

	renderStage(XRS_EDITOR_2D, pVisibility);
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
