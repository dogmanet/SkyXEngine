
#include "ml_data.h"

namespace MLSet
{
	void MLInit();
	IDirect3DDevice9* DXDevice = 0;
	//float3 ConstCurrCamPos;
	//float2_t WinSize = float2_t(1024, 768);
	//DWORD CountTimeDelta = 0;
	//char StdPathMaterial[1024];
	//char StdPathMesh[1024];

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
		SizeTexDepthGlobal.x = Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH) * CoefSizeDepthMapForGlobal;
		SizeTexDepthGlobal.y = Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT) * CoefSizeDepthMapForGlobal;

		SizeTexDepthLocal.x = Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH) * CoefSizeDepthMapForLocal;
		SizeTexDepthLocal.y = Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT) * CoefSizeDepthMapForLocal;
	}

	void GetArrDownScale4x4(DWORD width, DWORD height, float2 arr[]);

	//fov and ration esesno
	//float ProjFov = SM_PI * 0.25f;
	//float ProjRatio = WinSize.x / WinSize.y;

	//ближн¤¤ и дальн¤¤ плоскости
	//float2_t NearFar = float2_t(0.25f, 400.f);

	//float4x4 MCamView;

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
		ID ParamLight;
		ID NullMaterial;
		ID NullingTex;
	};
};

void MLSet::MLInit()
{
	MLSet::DXDevice = SGCore_GetDXDevice();

	/*sprintf(MLSet::StdPathMaterial, "%s", Core_RStringGet(G_RI_STRING_PATH_GS_MTRLS));
	sprintf(MLSet::StdPathMesh, "%s", Core_RStringGet(G_RI_STRING_PATH_GS_MESHES));

	MLSet::WinSize = *winsize;
	MLSet::ProjFov = projfov;*/
	MLSet::IDsTexs::Tex_NoiseTex = SGCore_LoadTexAddName("noise_rottex.dds", LoadTexType::ltt_const);



	IDirect3DTexture9* NullMaterial;
	MLSet::DXDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &NullMaterial, NULL);
	D3DLOCKED_RECT LockedRect;
	uint32_t tmpColor = D3DCOLOR_ARGB(0, 250, 2, 255);

	NullMaterial->LockRect(0, &LockedRect, 0, 0);

	uint32_t* tmpOldColor = (uint32_t*)LockedRect.pBits + 0 * LockedRect.Pitch + 0 * sizeof(uint32_t);
	memcpy(tmpOldColor, &tmpColor, sizeof(uint32_t));

	NullMaterial->UnlockRect(0);

	SGCore_LoadTexLoadTextures();
	MLSet::IDsTexs::NullMaterial = SGCore_LoadTexCreate("null_material__", NullMaterial);



	IDirect3DTexture9* ParamLightModelTex;
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

	SGCore_LoadTexLoadTextures();
	MLSet::IDsTexs::ParamLight = SGCore_LoadTexCreate("param_light__", ParamLightModelTex);



	IDirect3DTexture9* NullingTex;
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

	SGCore_LoadTexLoadTextures();
	MLSet::IDsTexs::NullingTex = SGCore_LoadTexCreate("nulling_tex__", NullingTex);


	MLSet::IDsShaders::VS::SMDepthSkinPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_pssm_direct.vs", "sm_depth_skin_pssm_direct", ShaderCheckDouble::scd_path);
	MLSet::IDsShaders::PS::SMDepthSkinPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_pssm_direct.ps", "sm_depth_skin_pssm_direct", ShaderCheckDouble::scd_path);

	MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_pssm_direct.vs", "sm_depth_geom_pssm_direct", ShaderCheckDouble::scd_path);
	MLSet::IDsShaders::PS::SMDepthGeomPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_pssm_direct.ps", "sm_depth_geom_pssm_direct", ShaderCheckDouble::scd_path);


	MLSet::IDsShaders::VS::SMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_geom_cube.vs", "sm_depth_geom_cube", ShaderCheckDouble::scd_path);
	MLSet::IDsShaders::PS::SMDepthGeomCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_geom_cube.ps", "sm_depth_geom_cube", ShaderCheckDouble::scd_path);

	MLSet::IDsShaders::VS::SMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_skin_cube.vs", "sm_depth_skin_cube", ShaderCheckDouble::scd_path);
	MLSet::IDsShaders::PS::SMDepthSkinCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_skin_cube.ps", "sm_depth_skin_cube", ShaderCheckDouble::scd_path);

	
	MLSet::IDsShaders::VS::SMDepthTreePSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm_direct.vs", "sm_depth_tree_pssm_direct", ShaderCheckDouble::scd_name);
	
	D3DXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	MLSet::IDsShaders::VS::SMDepthGrassPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_pssm_direct.vs", "sm_depth_grass_pssm_direct", ShaderCheckDouble::scd_name, Defines_GRASS);

	MLSet::IDsShaders::PS::SMDepthGreenPSSMDirect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_pssm_direct.ps", "sm_depth_green_pssm_direct", ShaderCheckDouble::scd_path);


	MLSet::IDsShaders::VS::SMDepthTreeCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_tree_cube", ShaderCheckDouble::scd_name);

	MLSet::IDsShaders::VS::SMDepthGrassCube = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "sm_depth_green_cube.vs", "sm_depth_green_cube", ShaderCheckDouble::scd_name, Defines_GRASS);
	
	MLSet::IDsShaders::PS::SMDepthGreenCube = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "sm_depth_green_cube.ps", "sm_depth_green_cube", ShaderCheckDouble::scd_path);


	MLSet::IDsShaders::VS::ResPosDepth = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos", ShaderCheckDouble::scd_path);


	MLSet::IDsShaders::PS::PSSM4 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps", "ppgensm_pssm", ShaderCheckDouble::scd_name);

	D3DXMACRO Defines_SPLITS3[] = { { "SPLITS3", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::PSSM3 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_pssm.ps", "ppgensm_pssm3split", ShaderCheckDouble::scd_name, Defines_SPLITS3);

	MLSet::IDsShaders::PS::GenShadowDirect4 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct", ShaderCheckDouble::scd_name);
	D3DXMACRO Defines_GSD_9[] = { { "GSD_9", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::GenShadowDirect9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_direct.ps", "ppgensm_direct_9", ShaderCheckDouble::scd_name, Defines_GSD_9);
	MLSet::IDsShaders::PS::GenShadowCube1 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point", ShaderCheckDouble::scd_name);
	D3DXMACRO Defines_GSC_9[] = { { "GSC_9", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::GenShadowCube6 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppgensm_point.ps", "ppgensm_point_9", ShaderCheckDouble::scd_name, Defines_GSC_9);

	MLSet::IDsShaders::PS::PPBlurDepthBased = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based.ps", "pp_blur_depth_based", ShaderCheckDouble::scd_path);
	MLSet::IDsShaders::PS::PPBlurDepthBasedNoise = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based_noise.ps", "pp_blur_depth_based_noise", ShaderCheckDouble::scd_path);

	MLSet::IDsShaders::PS::CalcAdaptedLum = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_calc_adapted_lum.ps", "pptm_calc_adapted_lum", ShaderCheckDouble::scd_path);
	MLSet::IDsShaders::PS::SampleLumInit = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_lum_init.ps", "pptm_lum_init", ShaderCheckDouble::scd_path);
	MLSet::IDsShaders::PS::SampleLumIterative = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pptm_lum_iterative.ps", "pptm_lum_iterative", ShaderCheckDouble::scd_path);
	
	MLSet::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render", ShaderCheckDouble::scd_path);
	MLSet::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render", ShaderCheckDouble::scd_path);


	MLSet::IDsShaders::VS::StdGeom = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_geom.vs", "stdr_geom", ShaderCheckDouble::scd_name);
	MLSet::IDsShaders::PS::StdGeom = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom", ShaderCheckDouble::scd_name);

	D3DXMACRO Defines_CP[] = { { "_CLIP_PLANE_", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::StdGeomCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_geom.ps", "stdr_geom_cp", ShaderCheckDouble::scd_name, Defines_CP);

	//D3DXMACRO Defines_GRASS[] = { { "_GRASS_", "" }, { 0, 0 } };
	MLSet::IDsShaders::VS::StdGrass = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_grass", ShaderCheckDouble::scd_name, Defines_GRASS);
	MLSet::IDsShaders::VS::StdTree = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_green.vs", "stdr_tree", ShaderCheckDouble::scd_name);

	MLSet::IDsShaders::PS::StdGreen = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green", ShaderCheckDouble::scd_name);
	MLSet::IDsShaders::PS::StdGreenCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_green.ps", "stdr_green_cp", ShaderCheckDouble::scd_name, Defines_CP);


	MLSet::IDsShaders::VS::StdSkin = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "stdr_skin.vs", "stdr_skin", ShaderCheckDouble::scd_name);
	MLSet::IDsShaders::PS::StdSkin = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin", ShaderCheckDouble::scd_name);

	MLSet::IDsShaders::PS::StdSkinCP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "stdr_skin.ps", "stdr_skin_cp", ShaderCheckDouble::scd_name, Defines_CP);


	//////////
	float tmpcoefsizert = 1;
	float2_t tmp_sizert = float2_t(Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH) * tmpcoefsizert, Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT) * tmpcoefsizert);

	//цвет (текстуры)
	MLSet::IDsRenderTargets::ColorScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_color", tmpcoefsizert);
	//номрали + микрорельеф
	MLSet::IDsRenderTargets::NormalScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_normal", tmpcoefsizert);
	//параметры освещени¤
	MLSet::IDsRenderTargets::ParamsScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_param", tmpcoefsizert);
	
	MLSet::IDsRenderTargets::DepthScene = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_depth", tmpcoefsizert);
	MLSet::IDsRenderTargets::DepthScene0 = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_depth_0", tmpcoefsizert);
	MLSet::IDsRenderTargets::DepthScene1 = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_depth_1", tmpcoefsizert);

	MLSet::IDsRenderTargets::LightAmbientDiff = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_ambient", tmpcoefsizert);
	MLSet::IDsRenderTargets::LightSpecular = SGCore_RTAdd(tmp_sizert.x, tmp_sizert.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_specdiff", tmpcoefsizert);


	MLSet::IDsRenderTargets::ToneMaps.clear();
	MLSet::IDsRenderTargets::SurfToneMap.clear();
	int tmpcount = 0;
	while (true)
	{
		int tmpsize = 1 << (2 * tmpcount);
		if (tmpsize >= Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH)*0.25 || tmpsize > Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT)*0.25)
			break;
		MLSet::IDsRenderTargets::ToneMaps[tmpcount] = SGCore_RTAdd(tmpsize, tmpsize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "qq", 0);
		MLSet::IDsRenderTargets::SurfToneMap[tmpcount] = 0;
		++tmpcount;
	}
	MLSet::IDsRenderTargets::CountArrToneMaps = tmpcount;

	MLSet::IDsRenderTargets::AdaptLumCurr = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "", 0);
	MLSet::IDsRenderTargets::AdaptLumLast = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "", 0);

	MLSet::IDsRenderTargets::LigthCom = SGCore_RTAdd(Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH), Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcom", 1);
	MLSet::IDsRenderTargets::LigthCom2 = SGCore_RTAdd(Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH), Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcom2", 1);
	MLSet::IDsRenderTargets::LigthCom3 = SGCore_RTAdd(Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH), Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_lightcom3", 1);
	
	MLSet::IDsRenderTargets::LigthComScaled = SGCore_RTAdd(Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH)*0.25f, Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT)*0.25f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcomscaled", 0.25);

	MLSet::RefMProjPlane = SMMatrixPerspectiveFovLH(Core_RFloatGet(G_RI_FLOAT_OBSERVER_FOV), Core_RFloatGet(G_RI_FLOAT_WINSIZE_WIDTH) / Core_RFloatGet(G_RI_FLOAT_WINSIZE_HEIGHT), MTl_REF_PROJ_NEAR, MTl_REF_PROJ_FAR);
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