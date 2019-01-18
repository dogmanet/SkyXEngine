
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
		
	uint32_t aRndColors[16];// = D3DCOLOR_ARGB(0, 250, 2, 255);

	for (int i = 0; i < 16; ++i)
		aRndColors[i] = GXCOLOR_ARGB(255, rand() % 255, rand() % 255, rand() % 255);

	IGXTexture2D *pRnsSampler = light_data::pDXDevice->createTexture2D(4, 4, 1, 0, GXFMT_A8R8G8B8, aRndColors);


	//SGCore_LoadTexLoadTextures();
	light_data::texture_id::idNoiseTex = SGCore_LoadTexCreate("noise_rottex__", pRnsSampler);

	light_data::shader_id::vs::idSMDepthSkinPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_pssm_direct.vs", "sm_depth_skin_pssm_direct.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthSkinPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_pssm_direct.ps", "sm_depth_skin_pssm_direct.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idSMDepthGeomPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_pssm_direct.vs", "sm_depth_geom_pssm_direct.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthGeomPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_pssm_direct.ps", "sm_depth_geom_pssm_direct.ps", SHADER_CHECKDOUBLE_PATH);


	light_data::shader_id::vs::idSMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_cube.vs", "sm_depth_geom_cube.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_cube.ps", "sm_depth_geom_cube.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idSMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_cube.vs", "sm_depth_skin_cube.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_cube.ps", "sm_depth_skin_cube.ps", SHADER_CHECKDOUBLE_PATH);

	
	light_data::shader_id::vs::idSMDepthTreePSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm_direct.vs", "sm_depth_tree_pssm_direct.vs", SHADER_CHECKDOUBLE_NAME);
	
	GXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	light_data::shader_id::vs::idSMDepthGrassPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm_direct.vs", "sm_depth_grass_pssm_direct.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);

	light_data::shader_id::ps::idSMDepthGreenPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_pssm_direct.ps", "sm_depth_green_pssm_direct.ps", SHADER_CHECKDOUBLE_PATH);


	light_data::shader_id::vs::idSMDepthTreeCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_tree_cube.vs", SHADER_CHECKDOUBLE_NAME);

	light_data::shader_id::vs::idSMDepthGrassCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_green_cube.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);
	
	light_data::shader_id::ps::idSMDepthGreenCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_cube.ps", "sm_depth_green_cube.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::ps::idPSSM4 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps", "ppgensm_pssm.ps", SHADER_CHECKDOUBLE_NAME);

	GXMACRO Defines_SPLITS3[] = { { "SPLITS3", "" }, { 0, 0 } };
	light_data::shader_id::ps::idPSSM3 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps", "ppgensm_pssm3split.ps", SHADER_CHECKDOUBLE_NAME, Defines_SPLITS3);

	light_data::shader_id::ps::idGenShadowDirect4 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct.ps", SHADER_CHECKDOUBLE_NAME);
	GXMACRO Defines_GSD_9[] = { { "GSD_9", "" }, { 0, 0 } };
	light_data::shader_id::ps::idGenShadowDirect9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct_9.ps", SHADER_CHECKDOUBLE_NAME, Defines_GSD_9);
	light_data::shader_id::ps::idGenShadowCube1 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point.ps", SHADER_CHECKDOUBLE_NAME);
	GXMACRO Defines_GSC_6[] = { { "GSC_6", "" }, { 0, 0 } };
	light_data::shader_id::ps::idGenShadowCube6 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point_6.ps", SHADER_CHECKDOUBLE_NAME, Defines_GSC_6);

	light_data::shader_id::ps::idPPBlurDepthBased = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based.ps", "pp_blur_depth_based.ps", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idPPBlurDepthBasedNoise = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based_noise.ps", "pp_blur_depth_based_noise.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idStdGeom = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_geom.vs", "stdr_geom.vs", SHADER_CHECKDOUBLE_NAME);
	light_data::shader_id::ps::idStdGeom = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom.ps", SHADER_CHECKDOUBLE_NAME);

	GXMACRO Defines_CP[] = { { "_CLIP_PLANE_", "" }, { 0, 0 } };
	light_data::shader_id::ps::idStdGeomCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);

	//D3DXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	light_data::shader_id::vs::idStdGrass = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_grass.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);
	light_data::shader_id::vs::idStdTree = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_tree.vs", SHADER_CHECKDOUBLE_NAME);

	light_data::shader_id::ps::idStdGreen = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green.ps", SHADER_CHECKDOUBLE_NAME);
	light_data::shader_id::ps::idStdGreenCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);

	light_data::shader_id::vs::idStdSkin = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_skin.vs", "stdr_skin.vs", SHADER_CHECKDOUBLE_NAME);
	light_data::shader_id::ps::idStdSkin = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin.ps", SHADER_CHECKDOUBLE_NAME);

	light_data::shader_id::ps::idStdSkinCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);
}
