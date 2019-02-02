
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "gdata.h"

namespace gdata
{
	IGXContext *pDXDevice = 0;

	HWND hHandleParent3D = 0;
	HWND hHandle3D = 0;	

	DS_RT FinalImage = DS_RT_SCENELIGHT;
	ICamera *pCamera = 0;
	ID idSelectTex = -1;
	
	float2_t vNearFar = float2_t(0.025, 400);
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
			ID idComLightingShadow;

			ID idBlendAmbientSpecDiffColor;

			ID idStencilStr;
			ID idStencilColumn;
			ID idStencilStrColumn;
			ID idUnionAlpha;
		};

		namespace kit
		{
			ID idScreenOut;
			ID idStencilStr;
			ID idStencilColumn;
			ID idStencilStrColumn;
			ID idBlendAmbientSpecDiffColor;
			ID idUnionAlpha;
			ID idComLightingNonShadow;
			ID idComLightingShadow;
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
		IGXSamplerState *pSamplerAnisotopicClamp;
		IGXSamplerState *pSamplerAnisotopicWrap;

		IGXSamplerState *pSamplerScene = NULL;

		IGXBlendState *pBlendNoColor;
		IGXBlendState *pBlendRed;
		IGXBlendState *pBlendAlpha;
		IGXBlendState *pBlendAlphaOneOne;
		IGXBlendState *pBlendAlphaSky;

		IGXRasterizerState *pRasterizerCullFront;
		IGXRasterizerState *pRasterizerCullNone;
	};

	namespace Editors
	{
		bool canMoveCamera = true;
		CSimulationModel *pSimModel = 0;
		CGrid *pGrid = 0;
		CAxesStatic *pAxesStatic = 0;

		bool canRenderGrid = false;
		bool canRenderAxesStatic = false;
	}
};

//##########################################################################

void gdata::InitAllMatrix()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");
	static float2_t vWinSize;
	vWinSize = float2(*r_win_width, *r_win_height);

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");
	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	gdata::vNearFar.x = *r_near;
	gdata::vNearFar.y = *r_far;
	gdata::fProjFov = *r_default_fov;

	gdata::mCamProj = SMMatrixPerspectiveFovLH(gdata::fProjFov, vWinSize.x / vWinSize.y, gdata::vNearFar.x, gdata::vNearFar.y);
	//gdata::MCamProj = SMMatrixOrthographicLH(vWinSize.x / 4, vWinSize.y / 4, gdata::NearFar.x, gdata::NearFar.y);
	gdata::mLightProj = SMMatrixPerspectiveFovLH(gdata::fProjFov, vWinSize.x / vWinSize.y, gdata::vNearFar.x, G_DATA_LIGHT_FAR);
	//gdata::MLightProj = SMMatrixOrthographicLH(vWinSize.x / 4, vWinSize.y / 4, gdata::NearFar.x, G_DATA_LIGHT_FAR);

	gdata::mRefPlaneSkyProj = SMMatrixPerspectiveFovLH(gdata::fProjFov, vWinSize.x / vWinSize.y, gdata::vNearFar.x, G_DATA_LIGHT_FAR);
	gdata::mRefCubeSkyProj = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, gdata::vNearFar.x, G_DATA_LIGHT_FAR);

	Core_RMatrixSet(G_RI_MATRIX_OBSERVER_PROJ, &gdata::mCamProj);
	Core_RMatrixSet(G_RI_MATRIX_LIGHT_PROJ, &gdata::mLightProj);

	/*Core_RFloatSet(G_RI_FLOAT_OBSERVER_NEAR, gdata::NearFar.x);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FAR, gdata::NearFar.y);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FOV, gdata::ProjFov);*/
}

void gdata::shaders_id::InitAllShaders()
{
	gdata::shaders_id::vs::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	gdata::shaders_id::ps::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);

	gdata::shaders_id::vs::idResPos = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs", SHADER_CHECKDOUBLE_PATH);

	gdata::shaders_id::ps::idComLightingNonShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_nonshadow.ps", SHADER_CHECKDOUBLE_NAME);
	GXMACRO Defines_IS_SHADOWED[] = { { "IS_SHADOWED", "" }, { 0, 0 } };
	gdata::shaders_id::ps::idComLightingShadow = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_com.ps", "lighting_com_shadow.ps", SHADER_CHECKDOUBLE_NAME, Defines_IS_SHADOWED);
	gdata::shaders_id::ps::idBlendAmbientSpecDiffColor = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_blend.ps", "lighting_blend.ps", SHADER_CHECKDOUBLE_PATH);

	gdata::shaders_id::ps::idUnionAlpha = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_union_alpha.ps", "pp_union_alpha.ps", SHADER_CHECKDOUBLE_PATH);

	GXMACRO Defines_STR[] = { { "_STR_", "" }, { 0, 0 } };
	gdata::shaders_id::ps::idStencilStr = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str.ps", SHADER_CHECKDOUBLE_NAME, Defines_STR);
	GXMACRO Defines_COLUMN[] = { { "_COLUMN_", "" }, { 0, 0 } };
	gdata::shaders_id::ps::idStencilColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_column.ps", SHADER_CHECKDOUBLE_NAME, Defines_COLUMN);
	GXMACRO Defines_COLUMN_STR[] = { { "_COLUMN_STR_", "" }, { 0, 0 } };
	gdata::shaders_id::ps::idStencilStrColumn = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_alpha_stencil_mark.ps", "pp_stencil_str_column.ps", SHADER_CHECKDOUBLE_NAME, Defines_COLUMN_STR);

	gdata::shaders_id::kit::idScreenOut = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idScreenOut);
	gdata::shaders_id::kit::idStencilStr = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idStencilStr);
	gdata::shaders_id::kit::idStencilColumn = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idStencilColumn);
	gdata::shaders_id::kit::idStencilStrColumn = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idStencilStrColumn);
	gdata::shaders_id::kit::idBlendAmbientSpecDiffColor = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idBlendAmbientSpecDiffColor);
	gdata::shaders_id::kit::idUnionAlpha = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idScreenOut, gdata::shaders_id::ps::idUnionAlpha);
	gdata::shaders_id::kit::idComLightingNonShadow = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idResPos, gdata::shaders_id::ps::idComLightingNonShadow);
	gdata::shaders_id::kit::idComLightingShadow = SGCore_ShaderCreateKit(gdata::shaders_id::vs::idResPos, gdata::shaders_id::ps::idComLightingShadow);


	GXDEPTH_STENCIL_DESC dsDesc;

	dsDesc.bDepthEnable = FALSE;
	dsDesc.bEnableDepthWrite = FALSE;
	gdata::rstates::pDepthStencilStateNoZ = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.bStencilEnable = TRUE;
	dsDesc.depthFunc = GXCOMPARISON_ALWAYS;
	dsDesc.stencilPassOp = GXSTENCIL_OP_REPLACE;
	gdata::rstates::pDepthStencilStateMrtStage0 = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.bDepthEnable = TRUE;
	dsDesc.depthFunc = GXCOMPARISON_LESS_EQUAL;
	dsDesc.stencilFunc = GXCOMPARISON_NOT_EQUAL;
	dsDesc.stencilPassOp = GXSTENCIL_OP_DECR;
	gdata::rstates::pDepthStencilStateMrtStage1 = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.bEnableDepthWrite = FALSE;
	dsDesc.stencilFunc = GXCOMPARISON_ALWAYS;
	dsDesc.stencilDepthFailOp = GXSTENCIL_OP_INCR;
	dsDesc.stencilPassOp = GXSTENCIL_OP_KEEP;
	dsDesc.stencilBackDepthFailOp = GXSTENCIL_OP_DECR;
	gdata::rstates::pDepthStencilStateLightBound = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.bDepthEnable = FALSE;
	dsDesc.bEnableDepthWrite = FALSE;
	dsDesc.stencilFunc = GXCOMPARISON_EQUAL;
	dsDesc.stencilDepthFailOp = GXSTENCIL_OP_ZERO;
	dsDesc.stencilFailOp = GXSTENCIL_OP_ZERO;
	dsDesc.stencilPassOp = GXSTENCIL_OP_KEEP;
	gdata::rstates::pDepthStencilStateLightShadowNonGlobal = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.stencilPassOp = GXSTENCIL_OP_ZERO;
	gdata::rstates::pDepthStencilStateLightClear = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.bStencilEnable = FALSE;
	gdata::rstates::pDepthStencilStateLightShadowGlobal = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.bDepthEnable = TRUE;
	dsDesc.bEnableDepthWrite = FALSE;
	gdata::rstates::pDepthStencilStateParticles = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	

	GXSAMPLER_DESC samplerDesc;

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

	

	GXBLEND_DESC blendDesc;

	blendDesc.renderTarget[0].u8RenderTargetWriteMask = 0;
	gdata::rstates::pBlendNoColor = gdata::pDXDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_RED;
	gdata::rstates::pBlendRed = gdata::pDXDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].u8RenderTargetWriteMask = GXCOLOR_WRITE_ENABLE_ALL;
	blendDesc.renderTarget[0].bBlendEnable = TRUE;
	blendDesc.renderTarget[0].srcBlend = blendDesc.renderTarget[0].srcBlendAlpha = GXBLEND_INV_DEST_ALPHA;
	blendDesc.renderTarget[0].destBlend = blendDesc.renderTarget[0].destBlendAlpha = GXBLEND_DEST_ALPHA;
	gdata::rstates::pBlendAlpha = gdata::pDXDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].srcBlend = blendDesc.renderTarget[0].srcBlendAlpha = GXBLEND_ONE;
	blendDesc.renderTarget[0].destBlend = blendDesc.renderTarget[0].destBlendAlpha = GXBLEND_ONE;
	gdata::rstates::pBlendAlphaOneOne = gdata::pDXDevice->createBlendState(&blendDesc);

	blendDesc.renderTarget[0].srcBlend = blendDesc.renderTarget[0].srcBlendAlpha = GXBLEND_SRC_ALPHA;
	blendDesc.renderTarget[0].destBlend = blendDesc.renderTarget[0].destBlendAlpha = GXBLEND_INV_SRC_ALPHA;
	gdata::rstates::pBlendAlphaSky = gdata::pDXDevice->createBlendState(&blendDesc);
	


	GXRASTERIZER_DESC rasterizerDesc;

	rasterizerDesc.cullMode = GXCULL_FRONT;
	gdata::rstates::pRasterizerCullFront = gdata::pDXDevice->createRasterizerState(&rasterizerDesc);

	rasterizerDesc.cullMode = GXCULL_NONE;
	gdata::rstates::pRasterizerCullNone = gdata::pDXDevice->createRasterizerState(&rasterizerDesc);
}
