
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "ml_data.h"

namespace MLSet
{
	void MLInit();
	IDirect3DDevice9* DXDevice = 0;

	//размер текстуры глубины дл¤ локальных источников света
	float2_t SizeTexDepthGlobal = float2_t(1024, 768);

	//размер текстуры глубины дл¤ локальных источников света
	float2_t SizeTexDepthLocal = float2_t(1024, 768);

	//дистанции дл¤ 4 сплитов дл¤ глоальных теней PSSM
	float4_t DistForPSSM = float4_t(10.f, 40.f, 100.f, 200.f);

	//коэфициент размера текстур дл¤ карт глубин локальных источников света
	float CoefSizeDepthMapForLocal = 2;

	//коэфициент размера текстур дл¤ карт глубин глобального источника света
	float CoefSizeDepthMapForGlobal = 1;

	float2_t SizeTexReflection = float2_t(MTL_REF_TEX_SIZE, MTL_REF_TEX_SIZE);

	void ReCalcSize()
	{
		static const int *r_win_width = GET_PCVAR_INT("r_win_width");
		static const int *r_win_height = GET_PCVAR_INT("r_win_height");

		SizeTexDepthGlobal.x = float(*r_win_width) * CoefSizeDepthMapForGlobal;
		SizeTexDepthGlobal.y = float(*r_win_height) * CoefSizeDepthMapForGlobal;

		SizeTexDepthLocal.x = float(*r_win_width) * CoefSizeDepthMapForLocal;
		SizeTexDepthLocal.y = float(*r_win_height) * CoefSizeDepthMapForLocal;
	}

	void GetArrDownScale4x4(DWORD width, DWORD height, float2 arr[]);

	bool IsHalfGenPCFShadowLocal = false;

	//ориентаци¤ и верхний вектор дл¤ рендера в кубическую текстуру
	float3 OrientedCube[6] = { float3(1, 0, 0), float3(-1, 0, 0), float3(0, 1, 0), float3(0, -1, 0), float3(0, 0, 1), float3(0, 0, -1) };
	float3 UpVectorsCube[6] = { float3(0, 1, 0), float3(0, 1, 0), float3(0, 0, -1), float3(0, 0, 1), float3(0, 1, 0), float3(0, 1, 0) };

	float2 HDRSampleOffsets[16];

	float4x4 RefMProjPlane;
	float4x4 RefMProjCube;

	namespace IDsShaders
	{
		namespace VS
		{
			ID ResPosDepth;
			
			ID ScreenOut;

			ID SMDepthGeomPSSMDirect;
			ID SMDepthGeomCube;

			ID SMDepthGrassPSSMDirect;
			ID SMDepthGrassCube;

			ID SMDepthTreePSSMDirect;
			ID SMDepthTreeCube;

			ID SMDepthSkinPSSMDirect;
			ID SMDepthSkinCube;

			ID StdGeom;
			ID StdTree;
			ID StdGrass;
			ID StdSkin;
		};

		namespace PS
		{
			ID SMDepthGeomPSSMDirect;
			ID SMDepthGeomCube;

			ID SMDepthGreenPSSMDirect;
			ID SMDepthGreenCube;

			ID SMDepthSkinPSSMDirect;
			ID SMDepthSkinCube;

			ID PPBlurDepthBasedNoise;
			ID PSSM4;
			ID PSSM3;
			ID PPBlurDepthBased;
			ID GenShadowDirect4;
			ID GenShadowDirect9;

			ID GenShadowCube1;
			ID GenShadowCube6;

			ID CalcAdaptedLum;
			ID SampleLumInit;
			ID SampleLumIterative;

			ID ScreenOut;

			ID StdGeom;
			ID StdGeomCP;
			ID StdGreen;
			ID StdGreenCP;
			ID StdSkin;
			ID StdSkinCP;
		};
	};

	namespace IDsRenderTargets
	{
		ID DSComLight;

		ID ColorScene;//цвет (текстуры)
		ID NormalScene;//номрали + микрорельеф
		ID ParamsScene;//параметры освещени¤
		ID DepthScene;
		ID DepthScene0;
		ID DepthScene1;

		ID LightAmbientDiff;
		ID LightSpecular;

		Array<ID> ToneMaps;
		Array<LPDIRECT3DSURFACE9> SurfToneMap;
		int CountArrToneMaps = 0;
		////
		ID AdaptLumCurr;
		ID AdaptLumLast;

		int HowAdaptedLum = 0;
		ID GetCurrAdaptedLum(){ if (HowAdaptedLum == 0) return AdaptLumCurr; else return  AdaptLumLast; };
		ID GetLastAdaptedLum(){ if (HowAdaptedLum == 1) return AdaptLumCurr; else return  AdaptLumLast; };
		void IncrAdaptedLum(){ if (HowAdaptedLum >= 1) HowAdaptedLum = 0; else HowAdaptedLum = 1; };
		////

		ID LigthCom;
		ID LigthCom2;
		ID LigthCom3;

		ID LigthComScaled;
	};

	namespace IDsTexs
	{
		ID Tex_NoiseTex;
		//ID ParamLight;
		//ID NullMaterial;
		//ID NullingTex;
	};
};

void MLSet::MLInit()
{
	MLSet::DXDevice = SGCore_GetDXDevice();

	const int *r_win_width = GET_PCVAR_INT("r_win_width");
	const int *r_win_height = GET_PCVAR_INT("r_win_height");

	const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	//MLSet::IDsTexs::Tex_NoiseTex = SGCore_LoadTexAddName("noise_rottex.dds", LOAD_TEXTURE_TYPE_CONST);

	D3DLOCKED_RECT LockedRect;
	uint32_t* tmpOldColor;
	IDirect3DTexture9 *pRnsSampler;
	MLSet::DXDevice->CreateTexture(4, 4, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pRnsSampler, NULL);
	
	uint32_t aRndColors[16];// = D3DCOLOR_ARGB(0, 250, 2, 255);

	for (int i = 0; i < 16; ++i)
		aRndColors[i] = D3DCOLOR_ARGB(255, rand() % 255, rand() % 255, rand() % 255);

	pRnsSampler->LockRect(0, &LockedRect, 0, 0);
	memcpy(LockedRect.pBits, aRndColors, sizeof(uint32_t));
	pRnsSampler->UnlockRect(0);

	//SGCore_LoadTexLoadTextures();
	MLSet::IDsTexs::Tex_NoiseTex = SGCore_LoadTexCreate("noise_rottex__", pRnsSampler);
	


	/*IDirect3DTexture9* ParamLightModelTex;
	MLSet::DXDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &ParamLightModelTex, NULL);

	DWORD tmpcolorparamlight = D3DCOLOR_ARGB(0, 0, 0, 0);
	ParamLightModelTex->LockRect(0, &LockedRect, 0, 0);

	for (int x = 0; x<1; x++)
	{
		for (int y = 0; y<1; y++)
		{
			tmpOldColor = (uint32_t*)LockedRect.pBits + x*LockedRect.Pitch + y*sizeof(uint32_t);
			memcpy(tmpOldColor, &tmpcolorparamlight, sizeof(uint32_t));
		}
	}

	ParamLightModelTex->UnlockRect(0);

	//SGCore_LoadTexLoadTextures();
	MLSet::IDsTexs::ParamLight = SGCore_LoadTexCreate("param_light__", ParamLightModelTex);*/



	/*IDirect3DTexture9* NullingTex;
	MLSet::DXDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &NullingTex, NULL);

	DWORD tmpcolornulling = D3DCOLOR_ARGB(10, 10, 10, 10);
	NullingTex->LockRect(0, &LockedRect, 0, 0);

	for (int x = 0; x<1; x++)
	{
		for (int y = 0; y<1; y++)
		{
			tmpOldColor = (uint32_t*)LockedRect.pBits + x*LockedRect.Pitch + y*sizeof(uint32_t);
			memcpy(tmpOldColor, &tmpcolornulling, sizeof(uint32_t));
		}
	}

	NullingTex->UnlockRect(0);

	//SGCore_LoadTexLoadTextures();
	MLSet::IDsTexs::NullingTex = SGCore_LoadTexCreate("nulling_tex__", NullingTex);*/


	MLSet::IDsShaders::VS::SMDepthSkinPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_pssm_direct.vs", "sm_depth_skin_pssm_direct.vs", SHADER_CHECKDOUBLE_PATH);
	MLSet::IDsShaders::PS::SMDepthSkinPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_pssm_direct.ps", "sm_depth_skin_pssm_direct.ps", SHADER_CHECKDOUBLE_PATH);

	MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_pssm_direct.vs", "sm_depth_geom_pssm_direct.vs", SHADER_CHECKDOUBLE_PATH);
	MLSet::IDsShaders::PS::SMDepthGeomPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_pssm_direct.ps", "sm_depth_geom_pssm_direct.ps", SHADER_CHECKDOUBLE_PATH);


	MLSet::IDsShaders::VS::SMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_cube.vs", "sm_depth_geom_cube.vs", SHADER_CHECKDOUBLE_PATH);
	MLSet::IDsShaders::PS::SMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_cube.ps", "sm_depth_geom_cube.ps", SHADER_CHECKDOUBLE_PATH);

	MLSet::IDsShaders::VS::SMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_cube.vs", "sm_depth_skin_cube.vs", SHADER_CHECKDOUBLE_PATH);
	MLSet::IDsShaders::PS::SMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_cube.ps", "sm_depth_skin_cube.ps", SHADER_CHECKDOUBLE_PATH);

	
	MLSet::IDsShaders::VS::SMDepthTreePSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm_direct.vs", "sm_depth_tree_pssm_direct.vs", SHADER_CHECKDOUBLE_NAME);
	
	D3DXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	MLSet::IDsShaders::VS::SMDepthGrassPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm_direct.vs", "sm_depth_grass_pssm_direct.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);

	MLSet::IDsShaders::PS::SMDepthGreenPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_pssm_direct.ps", "sm_depth_green_pssm_direct.ps", SHADER_CHECKDOUBLE_PATH);


	MLSet::IDsShaders::VS::SMDepthTreeCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_tree_cube.vs", SHADER_CHECKDOUBLE_NAME);

	MLSet::IDsShaders::VS::SMDepthGrassCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_green_cube.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);
	
	MLSet::IDsShaders::PS::SMDepthGreenCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_cube.ps", "sm_depth_green_cube.ps", SHADER_CHECKDOUBLE_PATH);


	MLSet::IDsShaders::VS::ResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs", SHADER_CHECKDOUBLE_PATH);

	MLSet::IDsShaders::PS::PSSM4 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps", "ppgensm_pssm.ps", SHADER_CHECKDOUBLE_NAME);

	D3DXMACRO Defines_SPLITS3[] = { { "SPLITS3", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::PSSM3 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps", "ppgensm_pssm3split.ps", SHADER_CHECKDOUBLE_NAME, Defines_SPLITS3);

	MLSet::IDsShaders::PS::GenShadowDirect4 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct.ps", SHADER_CHECKDOUBLE_NAME);
	D3DXMACRO Defines_GSD_9[] = { { "GSD_9", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::GenShadowDirect9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct_9.ps", SHADER_CHECKDOUBLE_NAME, Defines_GSD_9);
	MLSet::IDsShaders::PS::GenShadowCube1 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point.ps", SHADER_CHECKDOUBLE_NAME);
	D3DXMACRO Defines_GSC_9[] = { { "GSC_9", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::GenShadowCube6 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point_9.ps", SHADER_CHECKDOUBLE_NAME, Defines_GSC_9);

	MLSet::IDsShaders::PS::PPBlurDepthBased = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based.ps", "pp_blur_depth_based.ps", SHADER_CHECKDOUBLE_PATH);
	MLSet::IDsShaders::PS::PPBlurDepthBasedNoise = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based_noise.ps", "pp_blur_depth_based_noise.ps", SHADER_CHECKDOUBLE_PATH);

	MLSet::IDsShaders::PS::CalcAdaptedLum = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_calc_adapted_lum.ps", "pptm_calc_adapted_lum.ps", SHADER_CHECKDOUBLE_PATH);
	MLSet::IDsShaders::PS::SampleLumInit = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_lum_init.ps", "pptm_lum_init.ps", SHADER_CHECKDOUBLE_PATH);
	MLSet::IDsShaders::PS::SampleLumIterative = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_lum_iterative.ps", "pptm_lum_iterative.ps", SHADER_CHECKDOUBLE_PATH);
	
	MLSet::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	MLSet::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);


	MLSet::IDsShaders::VS::StdGeom = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_geom.vs", "stdr_geom.vs", SHADER_CHECKDOUBLE_NAME);
	MLSet::IDsShaders::PS::StdGeom = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom.ps", SHADER_CHECKDOUBLE_NAME);

	D3DXMACRO Defines_CP[] = { { "_CLIP_PLANE_", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::StdGeomCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);

	//D3DXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	MLSet::IDsShaders::VS::StdGrass = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_grass.vs", SHADER_CHECKDOUBLE_NAME, Defines_GRASS);
	MLSet::IDsShaders::VS::StdTree = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_tree.vs", SHADER_CHECKDOUBLE_NAME);

	MLSet::IDsShaders::PS::StdGreen = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green.ps", SHADER_CHECKDOUBLE_NAME);
	MLSet::IDsShaders::PS::StdGreenCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);


	MLSet::IDsShaders::VS::StdSkin = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_skin.vs", "stdr_skin.vs", SHADER_CHECKDOUBLE_NAME);
	MLSet::IDsShaders::PS::StdSkin = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin.ps", SHADER_CHECKDOUBLE_NAME);

	MLSet::IDsShaders::PS::StdSkinCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin_cp.ps", SHADER_CHECKDOUBLE_NAME, Defines_CP);


	//////////
	float tmpcoefsizert = 1;
	float2_t tmp_sizert = float2_t(float(*r_win_width) * tmpcoefsizert, (*r_win_height) * tmpcoefsizert);

	//цвет (текстуры)
	MLSet::IDsRenderTargets::ColorScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_color", tmpcoefsizert);
	//номрали + микрорельеф
	MLSet::IDsRenderTargets::NormalScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8/*D3DFMT_A2R10G10B10*/, D3DPOOL_DEFAULT, "ds_normal", tmpcoefsizert);
	//параметры освещени¤
	MLSet::IDsRenderTargets::ParamsScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_param", tmpcoefsizert);
	
	MLSet::IDsRenderTargets::DepthScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "ds_depth", tmpcoefsizert);
	MLSet::IDsRenderTargets::DepthScene0 = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "ds_depth_0", tmpcoefsizert);
	MLSet::IDsRenderTargets::DepthScene1 = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "ds_depth_1", tmpcoefsizert);

	MLSet::IDsRenderTargets::LightAmbientDiff = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_ambient", tmpcoefsizert);
	MLSet::IDsRenderTargets::LightSpecular = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_specdiff", tmpcoefsizert);


	MLSet::IDsRenderTargets::ToneMaps.clear();
	MLSet::IDsRenderTargets::SurfToneMap.clear();

	char szNameRT[64];
	int tmpcount = 0;
	while (true)
	{
		int tmpsize = 1 << (2 * tmpcount);
		if (tmpsize >= float(*r_win_width)*0.25 || tmpsize > (*r_win_height)*0.25)
			break;

		sprintf(szNameRT, "tone_map_%dx%d", tmpsize, tmpsize);
		MLSet::IDsRenderTargets::ToneMaps[tmpcount] = SGCore_RTAdd(tmpsize, tmpsize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, szNameRT, 0);
		MLSet::IDsRenderTargets::SurfToneMap[tmpcount] = 0;
		++tmpcount;
	}
	MLSet::IDsRenderTargets::CountArrToneMaps = tmpcount;

	MLSet::IDsRenderTargets::AdaptLumCurr = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "adapted_lum_curr", 0);
	MLSet::IDsRenderTargets::AdaptLumLast = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "adapted_lum_last", 0);

	MLSet::IDsRenderTargets::LigthCom = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcom", 1);
	MLSet::IDsRenderTargets::LigthCom2 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcom2", 1);
	MLSet::IDsRenderTargets::LigthCom3 = SGCore_RTAdd(*r_win_width, *r_win_height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_lightcom3", 1);
	
	MLSet::IDsRenderTargets::LigthComScaled = SGCore_RTAdd(float(*r_win_width)*0.25f, float(*r_win_height)*0.25f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcomscaled", 0.25);

	MLSet::RefMProjPlane = SMMatrixPerspectiveFovLH(*r_default_fov, float(*r_win_width) / float(*r_win_height), MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
	MLSet::RefMProjCube = SMMatrixPerspectiveFovLH(SM_PI * 0.5f, 1, MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
}

void MLSet::GetArrDownScale4x4(DWORD width, DWORD height, float2 arr[])
{
	if (arr == 0)
		return;

	float tU = 1.0f / float(width);
	float tV = 1.0f / float(height);

	int index = 0;

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{
			arr[index].x = (x - 1.5f) * tU;
			arr[index].y = (y - 1.5f) * tV;

			++index;
		}
	}
}