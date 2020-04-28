
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

		namespace kit
		{
			ID idScreenOut;
		};
	};

	namespace rstates
	{
		IGXDepthStencilState *pDepthStencilStateNoZ;
		IGXDepthStencilState *pDepthStencilStateParticles;

		IGXSamplerState *pSamplerPointClamp;
		IGXSamplerState *pSamplerLinearWrap;
		IGXSamplerState *pSamplerLinearClamp;

		IGXSamplerState *pSamplerScene = NULL;

		IGXBlendState *pBlendAlpha;

		IGXRasterizerState *pRasterizerCullNone;
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

	gdata::mCamProj = SMMatrixPerspectiveFovLH(gdata::fProjFov, vWinSize.x / vWinSize.y, gdata::vNearFar.y, gdata::vNearFar.x);

	Core_RMatrixSet(G_RI_MATRIX_OBSERVER_PROJ, &gdata::mCamProj);
	Core_RMatrixSet(G_RI_MATRIX_LIGHT_PROJ, &gdata::mLightProj);
}

void gdata::shaders_id::InitAllShaders()
{
	gdata::shaders_id::kit::idScreenOut = SGCore_ShaderCreateKit(SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs"), SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps"));
	
	GXDepthStencilDesc dsDesc;

	dsDesc.useDepthTest = FALSE;
	dsDesc.useDepthWrite = FALSE;
	gdata::rstates::pDepthStencilStateNoZ = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	dsDesc.cmpFuncDepth = GXCMP_GREATER_EQUAL;
	dsDesc.useStencilTest = FALSE;
	dsDesc.useDepthTest = TRUE;
	dsDesc.useDepthWrite = FALSE;
	gdata::rstates::pDepthStencilStateParticles = gdata::pDXDevice->createDepthStencilState(&dsDesc);

	

	GXSamplerDesc samplerDesc;

	samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
	gdata::rstates::pSamplerPointClamp = gdata::pDXDevice->createSamplerState(&samplerDesc);

	samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_WRAP;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
	gdata::rstates::pSamplerLinearWrap = gdata::pDXDevice->createSamplerState(&samplerDesc);

	samplerDesc.addressU = samplerDesc.addressV = samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
	gdata::rstates::pSamplerLinearClamp = gdata::pDXDevice->createSamplerState(&samplerDesc);


	GXBlendDesc blendDesc;

	blendDesc.renderTarget[0].useBlend = TRUE;
	blendDesc.renderTarget[0].blendSrcColor = blendDesc.renderTarget[0].blendSrcAlpha = GXBLEND_INV_DEST_ALPHA;
	blendDesc.renderTarget[0].blendDestColor = blendDesc.renderTarget[0].blendDestAlpha = GXBLEND_DEST_ALPHA;
	gdata::rstates::pBlendAlpha = gdata::pDXDevice->createBlendState(&blendDesc);



	GXRasterizerDesc rasterizerDesc;

	rasterizerDesc.cullMode = GXCULL_NONE;
	gdata::rstates::pRasterizerCullNone = gdata::pDXDevice->createRasterizerState(&rasterizerDesc);

}
