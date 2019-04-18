
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "ml_data.h"

namespace light_data
{
	void Init();

	void ReCalcSize();

	IGXContext *pDXDevice = 0;

	//размер текстуры глубины дл¤ локальных источников света
	float2_t vSizeTexDepthGlobal = float2_t(1024, 768);

	//размер текстуры глубины дл¤ локальных источников света
	float2_t vSizeTexDepthLocal = float2_t(1024, 768);

	//дистанции дл¤ 4 сплитов дл¤ глоальных теней PSSM
	float4_t vDistForPSSM = float4_t(10.f, 40.f, 100.f, 200.f);

	//коэфициент размера текстур дл¤ карт глубин локальных источников света
	float fCoefSizeDepthMapForLocal = 2;

	//коэфициент размера текстур дл¤ карт глубин глобального источника света
	float fCoefSizeDepthMapForGlobal = 1;

	bool isHalfGenPCFShadowLocal = false;

	//ориентаци¤ и верхний вектор дл¤ рендера в кубическую текстуру
	float3 vOrientedCube[6] = { float3(1, 0, 0), float3(-1, 0, 0), float3(0, 1, 0), float3(0, -1, 0), float3(0, 0, 1), float3(0, 0, -1) };
	float3 vUpVectorsCube[6] = { float3(0, 1, 0), float3(0, 1, 0), float3(0, 0, -1), float3(0, 0, 1), float3(0, 1, 0), float3(0, 1, 0) };

	float4x4 mRefProjPlane;
	float4x4 mRefProjCube;

	IGXSamplerState *pSamplerPointClamp;
	IGXSamplerState *pSamplerLinearClamp;
	IGXSamplerState *pSamplerPointWrap;

	namespace shader_id
	{
		namespace vs
		{
			ID idResPosDepth;
			
			ID idScreenOut;

			ID idSMDepthGeomPSSMDirect;
			ID idSMDepthGeomCube;

			ID idSMDepthGrassPSSMDirect;
			ID idSMDepthGrassCube;

			ID idSMDepthTreePSSMDirect;
			ID idSMDepthTreeCube;

			ID idSMDepthSkinPSSMDirect;
			ID idSMDepthSkinCube;

			ID idStdGeom;
			ID idStdTree;
			ID idStdGrass;
			ID idStdSkin;

			ID idLightBound;
		};

		namespace ps
		{
			ID idSMDepthGeomPSSMDirect;
			ID idSMDepthGeomCube;

			ID idSMDepthGreenPSSMDirect;
			ID idSMDepthGreenCube;

			ID idSMDepthSkinPSSMDirect;
			ID idSMDepthSkinCube;

			ID idPPBlurDepthBasedNoise;
			ID idPSSM4;
			ID idPSSM3;
			ID idPPBlurDepthBased;
			ID idGenShadowDirect4;
			ID idGenShadowDirect9;

			ID idGenShadowCube1;
			ID idGenShadowCube6;

			ID idScreenOut;

			ID idStdGeom;
			ID idStdGeomCP;
			ID idStdGreen;
			ID idStdGreenCP;
			ID idStdSkin;
			ID idStdSkinCP;

			ID idLightBound;
		};

		namespace kit
		{
			ID idSMDepthGeomPSSMDirect;
			ID idResPosDepthPSSM3;
			ID idResPosDepthPSSM4;
			ID idResPosDepthGenShadowDirect4;
			ID idResPosDepthGenShadowDirect9;
			ID idSMDepthGeomCube;
			ID idResPosDepthGenShadowCube1;
			ID idResPosDepthGenShadowCube6;
			ID idSMDepthSkinPSSMDirect;
			ID idSMDepthTreePSSMDirect;
			ID idSMDepthGrassPSSMDirect;
			ID idSMDepthGrassCube;
			ID idSMDepthTreeCube;
			ID idSMDepthSkinCube;
			ID idPPBlurDepthBasedNoise;
			ID idPPBlurDepthBased;

			ID idLightBound;
		};
	};

	namespace texture_id
	{
		ID idNoiseTex;
	};
};

void light_data::ReCalcSize()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	vSizeTexDepthGlobal.x = float(*r_win_width) * light_data::fCoefSizeDepthMapForGlobal;
	vSizeTexDepthGlobal.y = float(*r_win_height) * light_data::fCoefSizeDepthMapForGlobal;

	vSizeTexDepthLocal.x = float(*r_win_width) * light_data::fCoefSizeDepthMapForLocal;
	vSizeTexDepthLocal.y = float(*r_win_height) * light_data::fCoefSizeDepthMapForLocal;
}

void light_data::Init()
{
	light_data::pDXDevice = SGCore_GetDXDevice();
		
	GXSAMPLER_DESC samplerDesc;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
	pSamplerPointWrap = pDXDevice->createSamplerState(&samplerDesc);
	 
	samplerDesc.addressU = GXTEXTURE_ADDRESS_CLAMP;
	samplerDesc.addressV = GXTEXTURE_ADDRESS_CLAMP;
	samplerDesc.addressW = GXTEXTURE_ADDRESS_CLAMP;
	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
	pSamplerPointClamp = pDXDevice->createSamplerState(&samplerDesc);

	samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
	pSamplerLinearClamp = pDXDevice->createSamplerState(&samplerDesc);


	//SGCore_LoadTexLoadTextures();

	shader_id::vs::idSMDepthSkinPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_pssm_direct.vs");
	shader_id::ps::idSMDepthSkinPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_pssm_direct.ps");
	shader_id::kit::idSMDepthSkinPSSMDirect = SGCore_ShaderCreateKit(shader_id::vs::idSMDepthSkinPSSMDirect, shader_id::ps::idSMDepthSkinPSSMDirect);

	shader_id::vs::idSMDepthGeomPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_pssm_direct.vs");
	shader_id::ps::idSMDepthGeomPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_pssm_direct.ps");
	shader_id::kit::idSMDepthGeomPSSMDirect = SGCore_ShaderCreateKit(shader_id::vs::idSMDepthGeomPSSMDirect, shader_id::ps::idSMDepthGeomPSSMDirect);


	shader_id::vs::idSMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_cube.vs");
	shader_id::ps::idSMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_cube.ps");
	shader_id::kit::idSMDepthGeomCube = SGCore_ShaderCreateKit(shader_id::vs::idSMDepthGeomCube, shader_id::ps::idSMDepthGeomCube);

	shader_id::vs::idSMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_cube.vs");
	shader_id::ps::idSMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_cube.ps");
	shader_id::kit::idSMDepthSkinCube = SGCore_ShaderCreateKit(shader_id::vs::idSMDepthSkinCube, shader_id::ps::idSMDepthSkinCube);

	
	shader_id::vs::idSMDepthTreePSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm_direct.vs", "sm_depth_tree_pssm_direct.vs");
	
	GXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	shader_id::vs::idSMDepthGrassPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm_direct.vs", "sm_depth_grass_pssm_direct.vs", Defines_GRASS);

	shader_id::ps::idSMDepthGreenPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_pssm_direct.ps");


	shader_id::kit::idSMDepthTreePSSMDirect = SGCore_ShaderCreateKit(shader_id::vs::idSMDepthTreePSSMDirect, shader_id::ps::idSMDepthGreenPSSMDirect);
	shader_id::kit::idSMDepthGrassPSSMDirect = SGCore_ShaderCreateKit(shader_id::vs::idSMDepthGrassPSSMDirect, shader_id::ps::idSMDepthGreenPSSMDirect);


	shader_id::vs::idSMDepthTreeCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_tree_cube.vs");

	shader_id::vs::idSMDepthGrassCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_grass_cube.vs", Defines_GRASS);
	
	shader_id::ps::idSMDepthGreenCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_cube.ps");

	shader_id::kit::idSMDepthGrassCube = SGCore_ShaderCreateKit(shader_id::vs::idSMDepthGrassCube, shader_id::ps::idSMDepthGreenCube);
	shader_id::kit::idSMDepthTreeCube = SGCore_ShaderCreateKit(shader_id::vs::idSMDepthTreeCube, shader_id::ps::idSMDepthGreenCube);


	shader_id::vs::idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs");

	shader_id::ps::idPSSM4 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps");
	shader_id::kit::idResPosDepthPSSM4 = SGCore_ShaderCreateKit(shader_id::vs::idResPosDepth, shader_id::ps::idPSSM4);

	GXMACRO Defines_SPLITS3[] = { { "SPLITS3", "" }, { 0, 0 } };
	shader_id::ps::idPSSM3 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps", "ppgensm_pssm3split.ps", Defines_SPLITS3);
	shader_id::kit::idResPosDepthPSSM3 = SGCore_ShaderCreateKit(shader_id::vs::idResPosDepth, shader_id::ps::idPSSM3);

	shader_id::ps::idGenShadowDirect4 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps");
	shader_id::kit::idResPosDepthGenShadowDirect4 = SGCore_ShaderCreateKit(shader_id::vs::idResPosDepth, shader_id::ps::idGenShadowDirect4);
	GXMACRO Defines_GSD_9[] = { { "GSD_9", "" }, { 0, 0 } };
	shader_id::ps::idGenShadowDirect9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct_9.ps", Defines_GSD_9);
	shader_id::kit::idResPosDepthGenShadowDirect9 = SGCore_ShaderCreateKit(shader_id::vs::idResPosDepth, shader_id::ps::idGenShadowDirect9);

	shader_id::ps::idGenShadowCube1 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps");
	shader_id::kit::idResPosDepthGenShadowCube1 = SGCore_ShaderCreateKit(shader_id::vs::idResPosDepth, shader_id::ps::idGenShadowCube1);
	GXMACRO Defines_GSC_6[] = { { "GSC_6", "" }, { 0, 0 } };
	shader_id::ps::idGenShadowCube6 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point_6.ps", Defines_GSC_6);
	shader_id::kit::idResPosDepthGenShadowCube6 = SGCore_ShaderCreateKit(shader_id::vs::idResPosDepth, shader_id::ps::idGenShadowCube6);

	shader_id::ps::idPPBlurDepthBased = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based.ps");
	shader_id::ps::idPPBlurDepthBasedNoise = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based_noise.ps");

	shader_id::vs::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs");
	shader_id::ps::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps");


	shader_id::kit::idPPBlurDepthBasedNoise = SGCore_ShaderCreateKit(shader_id::vs::idScreenOut, shader_id::ps::idPPBlurDepthBasedNoise);
	shader_id::kit::idPPBlurDepthBased = SGCore_ShaderCreateKit(shader_id::vs::idScreenOut, shader_id::ps::idPPBlurDepthBased);


	shader_id::vs::idStdGeom = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_geom.vs");
	shader_id::ps::idStdGeom = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps");

	GXMACRO Defines_CP[] = { { "_CLIP_PLANE_", "" }, { 0, 0 } };
	shader_id::ps::idStdGeomCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom_cp.ps", Defines_CP);

	//D3DXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	shader_id::vs::idStdGrass = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_grass.vs", Defines_GRASS);
	shader_id::vs::idStdTree = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_tree.vs");

	shader_id::ps::idStdGreen = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps");
	shader_id::ps::idStdGreenCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green_cp.ps", Defines_CP);

	shader_id::vs::idStdSkin = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_skin.vs");
	shader_id::ps::idStdSkin = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps");

	shader_id::ps::idStdSkinCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin_cp.ps", Defines_CP);

	shader_id::vs::idLightBound = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "lighting_bound.vs");
	//shader_id::ps::idLightBound = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "lighting_bound.ps");
	//shader_id::kit::idLightBound = SGCore_ShaderCreateKit(shader_id::vs::idLightBound, shader_id::ps::idLightBound);
	shader_id::kit::idLightBound = SGCore_ShaderCreateKit(shader_id::vs::idLightBound, -1);
}
