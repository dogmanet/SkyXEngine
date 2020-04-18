
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "gdata.h"

namespace gdata
{
	IGXDevice *pDXDevice = 0;

	HWND hHandleParent3D = 0;
	HWND hHandle3D = 0;	

	DS_RT FinalImage = DS_RT_SCENELIGHT;
	ICamera *pCamera = 0;
	
	float2_t vNearFar = float2_t(0.025f, 400.0f);
	float fProjFov = SM_PIDIV4;	

	ID idDefaultGeomArr = -1;
	ID idDefaultGreenArr = -1;
	ID idDefaultAnimArr = -1;

	float3 vConstCurrCamPos;
	float3 vConstCurrCamDir;

	float4_t vCamWalkParamEditor = float4_t(
		10.f,	//!< простое движенеи вперед
		5.f,	//!< коэфициент ускорения
		0.7f,	//!< коэфициент от основного движения в стороны 
		0.5f	//!< коэфициент от основного движения назад
		);

	//матрицы
	void InitAllMatrix();
	float4x4 mCamView;
	float4x4 mCamProj;
	float4x4 mLightProj;
	float4x4 mRefPlaneSkyProj;
	float4x4 mRefCubeSkyProj;

	//**********************************************************************

	//! пространство имен с идентификаторами шейдеров
	namespace shaders_id
	{
		//! загрузка всех необходимых шейдеров
		void InitAllShaders();

		//! вершинные шейдеры
		namespace vs
		{
			ID idScreenOut;
			ID idResPos;
		};

		//! пиксельные шейдеры
		namespace ps
		{
			ID idScreenOut;
			ID idComLightingNonShadow;
			ID idComLightingSpotNonShadow;
			ID idComLightingGI;
			ID idComLightingShadow;
			ID idComLightingSpotShadow;
			ID idComLightingPSSMShadow;

			ID idBlendAmbientSpecDiffColor;

			ID idStencilStr;
			ID idStencilColumn;
			ID idStencilStrColumn;
			//ID idUnionAlpha;
		};

		namespace kit
		{
			ID idScreenOut;
			ID idStencilStr;
			ID idStencilColumn;
			ID idStencilStrColumn;
			ID idBlendAmbientSpecDiffColor;
			//ID idUnionAlpha;
			ID idComLightingNonShadow;
			ID idComLightingShadow;
			ID idComLightingSpotNonShadow;
			ID idComLightingSpotShadow;
			ID idComLightingPSSMShadow;
			ID idComLightingGI;
		};
	};

	namespace rstates
	{
		IGXDepthStencilState *pDepthStencilStateNoZ;
		IGXDepthStencilState *pDepthStencilStateMrtStage0;
		IGXDepthStencilState *pDepthStencilStateMrtStage1;
		IGXDepthStencilState *pDepthStencilStateLightBound;
		IGXDepthStencilState *pDepthStencilStateLightClear;
		IGXDepthStencilState *pDepthStencilStateLightShadowNonGlobal;
		IGXDepthStencilState *pDepthStencilStateLightShadowGlobal;
		IGXDepthStencilState *pDepthStencilStateParticles;

		IGXSamplerState *pSamplerPointClamp;
		IGXSamplerState *pSamplerLinearWrap;
		IGXSamplerState *pSamplerLinearMirror;
		IGXSamplerState *pSamplerLinearClamp;
		IGXSamplerState *pSamplerLinearBorder;
		IGXSamplerState *pSamplerAnisotopicClamp;
		IGXSamplerState *pSamplerAnisotopicWrap;

		IGXSamplerState *pSamplerScene = NULL;

		IGXBlendState *pBlendRed;
		IGXBlendState *pBlendAlpha;
		IGXBlendState *pBlendAlphaOneOne;
		IGXBlendState *pBlendAlphaSky;

		IGXRasterizerState *pRasterizerCullFront;
		IGXRasterizerState *pRasterizerCullNone;
		IGXRasterizerState *pRasterizerConservative;
	};

};

//##########################################################################

void gdata::InitAllMatrix()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");
	static float2_t vWinSize;
	vWinSize = float2((float)*r_win_width, (float)*r_win_height);

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");
	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	gdata::vNearFar.x = *r_near;
	gdata::vNearFar.y = *r_far;
	gdata::fProjFov = *r_default_fov;

	// gdata::mCamProj = SMMatrixPerspectiveFovLH(gdata::fProjFov, vWinSize.x / vWinSize.y, gdata::vNearFar.x, gdata::vNearFar.y);
	gdata::mCamProj = SMMatrixPerspectiveFovLH(gdata::fProjFov, vWinSize.x / vWinSize.y, gdata::vNearFar.y, gdata::vNearFar.x);
	//gdata::MCamProj = SMMatrixOrthographicLH(vWinSize.x / 4, vWinSize.y / 4, gdata::NearFar.x, gdata::NearFar.y);
//	gdata::mLightProj = SMMatrixPerspectiveFovLH(gdata::fProjFov, vWinSize.x / vWinSize.y, gdata::vNearFar.x, G_DATA_LIGHT_FAR);
	//gdata::MLightProj = SMMatrixOrthographicLH(vWinSize.x / 4, vWinSize.y / 4, gdata::NearFar.x, G_DATA_LIGHT_FAR);

//	gdata::mRefPlaneSkyProj = SMMatrixPerspectiveFovLH(gdata::fProjFov, vWinSize.x / vWinSize.y, gdata::vNearFar.x, G_DATA_LIGHT_FAR);
//	gdata::mRefCubeSkyProj = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, gdata::vNearFar.x, G_DATA_LIGHT_FAR);

	Core_RMatrixSet(G_RI_MATRIX_OBSERVER_PROJ, &gdata::mCamProj);
	Core_RMatrixSet(G_RI_MATRIX_LIGHT_PROJ, &gdata::mLightProj);

	/*Core_RFloatSet(G_RI_FLOAT_OBSERVER_NEAR, gdata::NearFar.x);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FAR, gdata::NearFar.y);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FOV, gdata::ProjFov);*/
}

void gdata::shaders_id::InitAllShaders()
{
	gdata::shaders_id::vs::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs");
	gdata::shaders_id::ps::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps");

	gdata::shaders_id::vs::idResPos = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs");

	gdata::shaders_id::ps::idComLightingNonShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_nonshadow.ps");
	GXMacro Defines_IS_SPOT[] = {{"IS_SPOT", ""}, {0, 0}};
	gdata::shaders_id::ps::idComLightingSpotNonShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_spot_nonshadow.ps", Defines_IS_SPOT);
	GXMacro Defines_IS_SHADOWED[] = { { "IS_SHADOWED", "" }, { 0, 0 } };
	gdata::shaders_id::ps::idComLightingShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_shadow.ps", Defines_IS_SHADOWED);
	GXMacro Defines_IS_SPOT_SHADOWED[] = {{"IS_SHADOWED", ""}, {"IS_SPOT", ""}, {0, 0}};
	gdata::shaders_id::ps::idComLightingSpotShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_spot_shadow.ps", Defines_IS_SPOT_SHADOWED);
	GXMacro Defines_IS_PSSM_SHADOWED[] = {{"IS_SHADOWED", ""}, {"IS_PSSM", ""}, {0, 0}};
	gdata::shaders_id::ps::idComLightingPSSMShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_pssm_shadow.ps", Defines_IS_PSSM_SHADOWED);
	gdata::shaders_id::ps::idBlendAmbientSpecDiffColor = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_blend.ps");

	//gdata::shaders_id::ps::idUnionAlpha = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_union_alpha.ps");

	GXMacro Defines_STR[] = { { "_STR_", "" }, { 0, 0 } };
	gdata::shaders_id::ps::idStencilStr = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str.ps", Defines_STR);
	GXMacro Defines_COLUMN[] = { { "_COLUMN_", "" }, { 0, 0 } };
	gdata::shaders_id::ps::idStencilColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_column.ps", Defines_COLUMN);
	GXMacro Defines_COLUMN_STR[] = { { "_COLUMN_STR_", "" }, { 0, 0 } };
	gdata::shaders_id::ps::idStencilStrColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str_column.ps", Defines_COLUMN_STR);

	gdata::shaders_id::kit::idScreenOut = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idScreenOut);
	gdata::shaders_id::kit::idStencilStr = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idStencilStr);
	gdata::shaders_id::kit::idStencilColumn = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idStencilColumn);
	gdata::shaders_id::kit::idStencilStrColumn = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idStencilStrColumn);
	gdata::shaders_id::kit::idBlendAmbientSpecDiffColor = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idBlendAmbientSpecDiffColor);
	//gdata::shaders_id::kit::idUnionAlpha = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idUnionAlpha);
	gdata::shaders_id::kit::idComLightingNonShadow = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idResPos, gdata::shaders_id::ps::idComLightingNonShadow);
	gdata::shaders_id::kit::idComLightingShadow = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idResPos, gdata::shaders_id::ps::idComLightingShadow);
	gdata::shaders_id::kit::idComLightingSpotNonShadow = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idResPos, gdata::shaders_id::ps::idComLightingSpotNonShadow);
	gdata::shaders_id::kit::idComLightingSpotShadow = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idResPos, gdata::shaders_id::ps::idComLightingSpotShadow);
	gdata::shaders_id::kit::idComLightingPSSMShadow = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idResPos, gdata::shaders_id::ps::idComLightingPSSMShadow);


	GXDepthStencilDesc dsDesc;

	dsDesc.useDepthTest = FALSE;
	dsDesc.useDepthWrite = FALSE;
	gdata::rstates::pDepthStencilStateNoZ = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.useStencilTest = TRUE;
	dsDesc.cmpFuncDepth = GXCMP_ALWAYS;
	dsDesc.stencilTestFront.opPass = GXSTENCIL_OP_REPLACE;
	gdata::rstates::pDepthStencilStateMrtStage0 = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.useDepthTest = TRUE;
	dsDesc.cmpFuncDepth = GXCMP_GREATER_EQUAL;
	dsDesc.stencilTestFront.cmpFunc = GXCMP_NOT_EQUAL;
	dsDesc.stencilTestFront.opPass = GXSTENCIL_OP_DECR;
	gdata::rstates::pDepthStencilStateMrtStage1 = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.useDepthWrite = FALSE;
	dsDesc.stencilTestFront.cmpFunc = GXCMP_ALWAYS;
	dsDesc.stencilTestFront.opDepthFail = GXSTENCIL_OP_INCR;
	dsDesc.stencilTestFront.opPass = GXSTENCIL_OP_KEEP;
	dsDesc.stencilTestBack.opDepthFail = GXSTENCIL_OP_DECR;
	gdata::rstates::pDepthStencilStateLightBound = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.useDepthTest = FALSE;
	dsDesc.useDepthWrite = FALSE;
	dsDesc.stencilTestFront.cmpFunc = GXCMP_EQUAL;
	dsDesc.stencilTestFront.opDepthFail = GXSTENCIL_OP_ZERO;
	dsDesc.stencilTestFront.opFail = GXSTENCIL_OP_ZERO;
	dsDesc.stencilTestFront.opPass = GXSTENCIL_OP_KEEP;
	gdata::rstates::pDepthStencilStateLightShadowNonGlobal = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.stencilTestFront.opPass = GXSTENCIL_OP_ZERO;
	gdata::rstates::pDepthStencilStateLightClear = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.useStencilTest = FALSE;
	gdata::rstates::pDepthStencilStateLightShadowGlobal = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.useDepthTest = TRUE;
	dsDesc.useDepthWrite = FALSE;
	gdata::rstates::pDepthStencilStateParticles = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	

	GXSamplerDesc samplerDesc;

	samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
	gdata::rstates::pSamplerPointClamp = gdata::pDXDevice->createSamplerState(&samplerDesc);

	samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_WRAP;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
	gdata::rstates::pSamplerLinearWrap = gdata::pDXDevice->createSamplerState(&samplerDesc);

	samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_MIRROR;
	gdata::rstates::pSamplerLinearMirror = gdata::pDXDevice->createSamplerState(&samplerDesc);

	samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
	gdata::rstates::pSamplerLinearClamp = gdata::pDXDevice->createSamplerState(&samplerDesc);

	samplerDesc.filter = GXFILTER_ANISOTROPIC;
	gdata::rstates::pSamplerAnisotopicClamp = gdata::pDXDevice->createSamplerState(&samplerDesc);

	samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_WRAP;
	gdata::rstates::pSamplerAnisotopicWrap = gdata::pDXDevice->createSamplerState(&samplerDesc);

	samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_BORDER;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.f4BorderColor = float4_t(0.0f, 0.0f, 0.0f, 0.0f);
	gdata::rstates::pSamplerLinearBorder = gdata::pDXDevice->createSamplerState(&samplerDesc);
	

	GXBlendDesc blendDesc;

	blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_RED;
	gdata::rstates::pBlendRed = gdata::pDXDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_ALL;
	blendDesc.renderTarget[0].useBlend = TRUE;
	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_INV_DEST_ALPHA;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_DEST_ALPHA;
	gdata::rstates::pBlendAlpha = gdata::pDXDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_ONE;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_ONE;
	gdata::rstates::pBlendAlphaOneOne = gdata::pDXDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_SRC_ALPHA;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_INV_SRC_ALPHA;
	gdata::rstates::pBlendAlphaSky = gdata::pDXDevice->createBlendState(&blendDesc);
	


	GXRasterizerDesc rasterizerDesc;

	//rasterizerDesc.useConservativeRasterization = true;
	gdata::rstates::pRasterizerConservative = gdata::pDXDevice->createRasterizerState(&rasterizerDesc);

	//rasterizerDesc.useConservativeRasterization = false;
	rasterizerDesc.cullMode = GXCULL_FRONT;
	gdata::rstates::pRasterizerCullFront = gdata::pDXDevice->createRasterizerState(&rasterizerDesc);

	rasterizerDesc.cullMode = GXCULL_NONE;
	gdata::rstates::pRasterizerCullNone = gdata::pDXDevice->createRasterizerState(&rasterizerDesc);



	gdata::shaders_id::ps::idComLightingGI = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_gi.ps");
	gdata::shaders_id::kit::idComLightingGI = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idResPos, gdata::shaders_id::ps::idComLightingGI);
}
