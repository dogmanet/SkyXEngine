
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "ml_data.h"

namespace light_data
{
	void Init();

	void ReCalcSize();

	IDirect3DDevice9 *pDXDevice = 0;

	//размер текстуры глубины дл¤ локальных источников света
	float2_t vSizeTexDepthGlobal = float2_t(1024, 768);

	//размер текстуры глубины дл¤ локальных источников света
	float2_t vSizeTexDepthLocal = float2_t(1024, 768);

	//дистанции дл¤ 4 сплитов дл¤ глоальных теней PSSM
	float4_t vDistForPSSM = float4_t(10.f, 40.f, 100.f, 200.f);

	//коэфициент размера текстур дл¤ карт глубин локальных источников света
	float fCoefSizeDepthMapForLocal = 1;

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

			ID idSMDepthGeomPSSM;
			ID idSMDepthGeomDirect;
			ID idSMDepthGeomCube;

			ID idSMDepthGrassPSSM;
			ID idSMDepthGrassDirect;
			ID idSMDepthGrassCube;

			ID idSMDepthTreePSSM;
			ID idSMDepthTreeDirect;
			ID idSMDepthTreeCube;

			ID idSMDepthSkinPSSM;
			ID idSMDepthSkinDirect;
			ID idSMDepthSkinCube;

			ID idStdGeom;
			ID idStdTree;
			ID idStdGrass;
			ID idStdSkin;
		};

		namespace ps
		{
			ID idSMDepthGeomPSSM;
			ID idSMDepthGeomDirect;
			ID idSMDepthGeomCube;

			ID idSMDepthGreenPSSM;
			ID idSMDepthGreenDirect;
			ID idSMDepthGreenCube;

			ID idSMDepthSkinPSSM;
			ID idSMDepthSkinDirect;
			ID idSMDepthSkinCube;

			ID idPPBlurDepthBasedNoise;
			ID idPSSM4;
			ID idPSSM3;
			ID idPPBlurDepthBased;

			ID idGenShadowDirect;
			ID idGenShadowCube;

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
		ID idSurfaceDepthGlobalShadow;
		ID idSurfaceDepthPointShadow;
		ID idSurfaceDepthDirShadow;
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

	const int *r_win_width = GET_PCVAR_INT("r_win_width");
	const int *r_win_height = GET_PCVAR_INT("r_win_height");

	const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	D3DLOCKED_RECT LockedRect;
	uint32_t* tmpOldColor;
	IDirect3DTexture9 *pRnsSampler;
	light_data::pDXDevice->CreateTexture(4, 4, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pRnsSampler, NULL);
	
	uint32_t aRndColors[16];// = D3DCOLOR_ARGB(0, 250, 2, 255);

	for (int i = 0; i < 16; ++i)
		aRndColors[i] = D3DCOLOR_ARGB(255, rand() % 255, rand() % 255, rand() % 255);

	pRnsSampler->LockRect(0, &LockedRect, 0, 0);
	memcpy(LockedRect.pBits, aRndColors, sizeof(uint32_t));
	pRnsSampler->UnlockRect(0);

	//SGCore_LoadTexLoadTextures();
	light_data::texture_id::idNoiseTex = SGCore_LoadTexCreate("noise_rottex__", pRnsSampler);

	


	light_data::shader_id::vs::idSMDepthGeomPSSM = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_pssm.vs", "sm_depth_geom_pssm.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthGeomPSSM = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_pssm.ps", "sm_depth_geom_pssm.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idSMDepthGeomDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_direct.vs", "sm_depth_geom_direct.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthGeomDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_direct.ps", "sm_depth_geom_direct.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idSMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_cube.vs", "sm_depth_geom_cube.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_cube.ps", "sm_depth_geom_cube.ps", SHADER_CHECKDOUBLE_PATH);


	light_data::shader_id::vs::idSMDepthSkinPSSM = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_pssm.vs", "sm_depth_skin_pssm.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthSkinPSSM = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_pssm.ps", "sm_depth_skin_pssm.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idSMDepthSkinDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_direct.vs", "sm_depth_skin_direct.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthSkinDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_direct.ps", "sm_depth_skin_direct.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idSMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_cube.vs", "sm_depth_skin_cube.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_cube.ps", "sm_depth_skin_cube.ps", SHADER_CHECKDOUBLE_PATH);



	light_data::shader_id::vs::idSMDepthTreePSSM = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm.vs", "sm_depth_tree_pssm.vs", SHADER_CHECKDOUBLE_NAME);
	light_data::shader_id::vs::idSMDepthTreeDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_direct.vs", "sm_depth_tree_direct.vs", SHADER_CHECKDOUBLE_NAME);
	
	D3DXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	light_data::shader_id::vs::idSMDepthGrassPSSM = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm.vs", "sm_depth_grass_pssm.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);
	light_data::shader_id::vs::idSMDepthGrassDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_direct.vs", "sm_depth_grass_direct.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);

	light_data::shader_id::ps::idSMDepthGreenPSSM = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_pssm.ps", "sm_depth_green_pssm.ps", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idSMDepthGreenDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_direct.ps", "sm_depth_green_direct.ps", SHADER_CHECKDOUBLE_PATH);


	light_data::shader_id::vs::idSMDepthTreeCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_tree_cube.vs", SHADER_CHECKDOUBLE_NAME);

	light_data::shader_id::vs::idSMDepthGrassCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_green_cube.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);
	
	light_data::shader_id::ps::idSMDepthGreenCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_cube.ps", "sm_depth_green_cube.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::ps::idPSSM4 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps", "ppgensm_pssm.ps", SHADER_CHECKDOUBLE_NAME);

	D3DXMACRO Defines_SPLITS3[] = { { "SPLITS3", "" }, { 0, 0 } };
	light_data::shader_id::ps::idPSSM3 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps", "ppgensm_pssm3split.ps", SHADER_CHECKDOUBLE_NAME, Defines_SPLITS3);

	light_data::shader_id::ps::idGenShadowDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct.ps", SHADER_CHECKDOUBLE_NAME);
	
	light_data::shader_id::ps::idGenShadowCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point.ps", SHADER_CHECKDOUBLE_NAME);
	
	light_data::shader_id::ps::idPPBlurDepthBased = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based.ps", "pp_blur_depth_based.ps", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idPPBlurDepthBasedNoise = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based_noise.ps", "pp_blur_depth_based_noise.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	light_data::shader_id::ps::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);

	light_data::shader_id::vs::idStdGeom = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_geom.vs", "stdr_geom.vs", SHADER_CHECKDOUBLE_NAME);
	light_data::shader_id::ps::idStdGeom = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom.ps", SHADER_CHECKDOUBLE_NAME);

	D3DXMACRO Defines_CP[] = { { "_CLIP_PLANE_", "" }, { 0, 0 } };
	light_data::shader_id::ps::idStdGeomCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);

	//D3DXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	light_data::shader_id::vs::idStdGrass = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_grass.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);
	light_data::shader_id::vs::idStdTree = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_tree.vs", SHADER_CHECKDOUBLE_NAME);

	light_data::shader_id::ps::idStdGreen = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green.ps", SHADER_CHECKDOUBLE_NAME);
	light_data::shader_id::ps::idStdGreenCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);

	light_data::shader_id::vs::idStdSkin = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_skin.vs", "stdr_skin.vs", SHADER_CHECKDOUBLE_NAME);
	light_data::shader_id::ps::idStdSkin = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin.ps", SHADER_CHECKDOUBLE_NAME);

	light_data::shader_id::ps::idStdSkinCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);

	light_data::texture_id::idSurfaceDepthGlobalShadow = SGCore_RTcreateSurfaceDepth(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, D3DFMT_D24X8, "shadow_globa_depth_stencill", -1);
	light_data::texture_id::idSurfaceDepthPointShadow = SGCore_RTcreateSurfaceDepth(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthLocal.x, D3DFMT_D24X8, "shadow_point_depth_stencil", -1);
	light_data::texture_id::idSurfaceDepthDirShadow = SGCore_RTcreateSurfaceDepth(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.y, D3DFMT_D24X8, "shadow_dir_depth_stencil", -1);
}