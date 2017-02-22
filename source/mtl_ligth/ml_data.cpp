
//#define HDR_NUM_TONEMAP_TEXTURES 4

namespace MLSet
{
	IDirect3DDevice9* DXDevice = 0;
	float3 ConstCurrCamPos;
	float2_t WinSize = float2_t(1024, 768);
	DWORD CountTimeDelta = 0;
	char StdPathMaterial[1024];
	char StdPathMesh[1024];
	
	//размер текстуры глубины дл€ локальных источников света
	float2_t SizeTexDepthGlobal = float2_t(1024, 768);

	//размер текстуры глубины дл€ локальных источников света
	float2_t SizeTexDepthLocal = float2_t(1024, 768);

	//дистанции дл€ 4 сплитов дл€ глоальных теней PSSM
	float4_t DistForPSSM = float4_t(10.f, 40.f, 100.f, 200.f);

	//коэфициент размера текстур дл€ карт глубин локальных источников света
	float CoefSizeDepthMapForLocal = 1;

	//коэфициент размера текстур дл€ карт глубин глобального источника света
	float CoefSizeDepthMapForGlobal = 1;

	float2_t SizeTexReflection = float2_t(512, 512);

	void ReCalcSize()
	{
		SizeTexDepthGlobal.x = WinSize.x * CoefSizeDepthMapForGlobal;
		SizeTexDepthGlobal.y = WinSize.y * CoefSizeDepthMapForGlobal;

		SizeTexDepthLocal.x = WinSize.x * CoefSizeDepthMapForLocal;
		SizeTexDepthLocal.y = WinSize.y * CoefSizeDepthMapForLocal;
	}

	void GetArrDownScale4x4(DWORD width, DWORD height, float2 arr[]);

	//fov and ration esesno
	float ProjFov = SM_PI / 4.f;
	float ProjRatio = WinSize.x / WinSize.y;

	//ближн€€ и дальн€€ плоскости
	float2_t NearFar = float2_t(0.05f, 400.f);

	float4x4 MCamView;

	bool IsHalfGenPCFShadowLocal = false;

	//ориентаци€ и верхний вектор дл€ рендера в кубическую текстуру
	float3 OrientedCube[6] = { float3(1, 0, 0), float3(-1, 0, 0), float3(0, 1, 0), float3(0, -1, 0), float3(0, 0, 1), float3(0, 0, -1) };
	float3 UpVectorsCube[6] = { float3(0, 1, 0), float3(0, 1, 0), float3(0, 0, -1), float3(0, 0, 1), float3(0, 1, 0), float3(0, 1, 0) };

	float2 HDRSampleOffsets[16];

	namespace IDsShaders
	{
		namespace VS
		{
			ID PPQuadRender;
			ID ResPosDepth;
			

			ID ScreenOut;

			ID SMDepthGeomPSSMDirect;
			ID SMDepthGeomCube;

			ID SMDepthGreenPSSMDirect;
			ID SMDepthGreenCube;
		};

		namespace PS
		{
			ID SMDepthGeomPSSMDirect;
			ID SMDepthGeomCube;

			ID SMDepthGreenPSSMDirect;
			ID SMDepthGreenCube;

			ID PPBlurDepthBasedNoise;
			ID PSSM4;
			ID PSSM3;
			ID PPBlurDepthBased;
			ID GenShadowDirect4;
			ID GenShadowDirect9;
			
			ID GenShadowCube1;
			ID GenShadowCube6;

			ID CalcAdaptedLum;;
			ID SampleLumInit;
			ID SampleLumIterative;
			ID SampleLumFinal;

			ID ScreenOut;
		};
	};

	namespace IDsRenderTargets
	{
		/*DWORD Tex_DSDepthLinear;
		DWORD Tex_DSDepthLinearD2;
		DWORD Tex_DSDepthLinearD4;*/

		ID DSComLight;

		ID ColorScene;//цвет (текстуры)
		ID ColorScene2;//цвет (текстуры)
		ID NormalScene;//номрали + микрорельеф
		ID NormalScene2;//номрали + микрорельеф
		ID ParamsScene;//параметры освещени€
		ID ParamsScene2;//параметры освещени€
		ID DepthScene;
		ID DepthScene2;

		ID LightAmbientDiff;
		ID LightAmbientDiff2;
		ID LightSpecular;
		ID LightSpecular2;

		Array<ID> ToneMaps;
		Array<LPDIRECT3DSURFACE9> SurfToneMap;
		int CountArrToneMaps = 0;
		////
		ID AdaptLumCurr;
		ID AdaptLumLast;

		int HowAdaptedLum = 0;
		inline ID GetCurrAdaptedLum(){ if (HowAdaptedLum == 0) return AdaptLumCurr; else return  AdaptLumLast; };
		inline ID GetLastAdaptedLum(){ if (HowAdaptedLum == 1) return AdaptLumCurr; else return  AdaptLumLast; };
		void IncrAdaptedLum(){ if (HowAdaptedLum >= 1) HowAdaptedLum = 0; else HowAdaptedLum = 1; };
		////

		ID LigthCom;
		ID LigthCom2;

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

void MLInit(IDirect3DDevice9* device, const char* std_path_material, const char* std_path_mesh, float2_t* winsize, float projfov)
{
	MLSet::DXDevice = device;

	if (std_path_material)
		sprintf(MLSet::StdPathMaterial, "%s", std_path_material);
	else
		MLSet::StdPathMaterial[0] = 0;

	if (std_path_material)
		sprintf(MLSet::StdPathMesh, "%s", std_path_mesh);
	else
		MLSet::StdPathMesh[0] = 0;

	MLSet::WinSize = *winsize;
	MLSet::ProjFov = projfov;
	MLSet::IDsTexs::Tex_NoiseTex = SGCore_LoadTexAddName("noise_rottex.dds");



	IDirect3DTexture9* NullMaterial;
	MLSet::DXDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &NullMaterial, NULL);
	D3DLOCKED_RECT LockedRect;
	uint32_t tmpColor = D3DCOLOR_ARGB(0, 250, 2, 0);

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

	MLSet::IDsShaders::VS::PPQuadRender = SGCore_ShaderLoad(0, "pp_quad_render.vs", "pp_quad_render", true);


	MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect = SGCore_ShaderLoad(0, "sm_depth_geom_pssm_direct.vs", "sm_depth_geom_pssm_direct", true);
	MLSet::IDsShaders::PS::SMDepthGeomPSSMDirect = SGCore_ShaderLoad(1, "sm_depth_geom_pssm_direct.ps", "sm_depth_geom_pssm_direct", true);

	MLSet::IDsShaders::VS::SMDepthGeomCube = SGCore_ShaderLoad(0, "sm_depth_geom_cube.vs", "sm_depth_geom_cube", true);
	MLSet::IDsShaders::PS::SMDepthGeomCube = SGCore_ShaderLoad(1, "sm_depth_geom_cube.ps", "sm_depth_geom_cube", true);

	MLSet::IDsShaders::VS::SMDepthGreenPSSMDirect = SGCore_ShaderLoad(0, "sm_depth_green_pssm_direct.vs", "sm_depth_green_pssm_direct", true);
	MLSet::IDsShaders::PS::SMDepthGreenPSSMDirect = SGCore_ShaderLoad(1, "sm_depth_green_pssm_direct.ps", "sm_depth_green_pssm_direct", true);

	MLSet::IDsShaders::VS::SMDepthGreenCube = SGCore_ShaderLoad(0, "sm_depth_green_cube.vs", "sm_depth_green_cube", true);
	MLSet::IDsShaders::PS::SMDepthGreenCube = SGCore_ShaderLoad(1, "sm_depth_green_cube.ps", "sm_depth_green_cube", true);


	MLSet::IDsShaders::VS::ResPosDepth = SGCore_ShaderLoad(0, "pp_quad_render_res_pos.vs", "pp_quad_render_res_pos", true);


	MLSet::IDsShaders::PS::PSSM4 = SGCore_ShaderLoad(1, "ppgensm_pssm.ps", "ppgensm_pssm", true);

	D3DXMACRO Defines_SPLITS3[] = { { "SPLITS3", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::PSSM3 = SGCore_ShaderLoad(1, "ppgensm_pssm.ps", "ppgensm_pssm3split", true, Defines_SPLITS3);

	MLSet::IDsShaders::PS::GenShadowDirect4 = SGCore_ShaderLoad(1, "ppgensm_direct.ps", "ppgensm_direct", true);
	D3DXMACRO Defines_GSD_9[] = { { "GSD_9", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::GenShadowDirect9 = SGCore_ShaderLoad(1, "ppgensm_direct.ps", "ppgensm_direct", true, Defines_GSD_9);
	MLSet::IDsShaders::PS::GenShadowCube1 = SGCore_ShaderLoad(1, "ppgensm_point.ps", "ppgensm_point", true);
	D3DXMACRO Defines_GSC_6[] = { { "GSC_6", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::GenShadowCube6 = SGCore_ShaderLoad(1, "ppgensm_point.ps", "ppgensm_point", true, Defines_GSC_6);

	MLSet::IDsShaders::PS::PPBlurDepthBased = SGCore_ShaderLoad(1, "pp_blur_depth_based.ps", "pp_blur_depth_based", true);
	MLSet::IDsShaders::PS::PPBlurDepthBasedNoise = SGCore_ShaderLoad(1, "pp_blur_depth_based_noise.ps", "pp_blur_depth_based_noise", true);

	MLSet::IDsShaders::PS::CalcAdaptedLum = SGCore_ShaderLoad(1, "pphdr_calc_adapted_lum.ps", "pp_hdr_calc_adapted_lum", true);
	MLSet::IDsShaders::PS::SampleLumInit = SGCore_ShaderLoad(1, "pphdr_lum_init.ps", "pp_hdr_lum_init", true);
	MLSet::IDsShaders::PS::SampleLumIterative = SGCore_ShaderLoad(1, "pphdr_lum_iterative.ps", "pp_hdr_lum_iterative", true);
	MLSet::IDsShaders::PS::SampleLumFinal = SGCore_ShaderLoad(1, "pphdr_lum_final.ps", "pp_hdr_lum_final", true);

	MLSet::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(0, "pp_quad_render.vs", "pp_quad_render", true);
	MLSet::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(1, "pp_quad_render.ps", "pp_quad_render", true);

	//////////

	//цвет (текстуры)
	MLSet::IDsRenderTargets::ColorScene = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_color", 1);
	MLSet::IDsRenderTargets::ColorScene2 = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_color2", 1);
	//номрали + микрорельеф
	MLSet::IDsRenderTargets::NormalScene = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_normal", 1);
	MLSet::IDsRenderTargets::NormalScene2 = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "ds_normal2", 1);
	//параметры освещени€
	MLSet::IDsRenderTargets::ParamsScene = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, "ds_param", 1);
	MLSet::IDsRenderTargets::ParamsScene2 = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, "ds_param2", 1);

	MLSet::IDsRenderTargets::DepthScene = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_depth", 1);
	MLSet::IDsRenderTargets::DepthScene2 = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_depth2", 1);

	MLSet::IDsRenderTargets::LightAmbientDiff = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_ambient", 1);
	MLSet::IDsRenderTargets::LightSpecular = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_specdiff", 1);

	MLSet::IDsRenderTargets::LightAmbientDiff2 = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_ambient2", 1);
	MLSet::IDsRenderTargets::LightSpecular2 = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "ds_specdiff2", 1);


	MLSet::IDsRenderTargets::ToneMaps.clear();
	MLSet::IDsRenderTargets::SurfToneMap.clear();
	int tmpcount = 0;
	while (true)
	{
		int tmpsize = 1 << (2 * tmpcount);
		if (tmpsize >= MLSet::WinSize.x*0.25 || tmpsize > MLSet::WinSize.y*0.25)
			break;
		MLSet::IDsRenderTargets::ToneMaps[tmpcount] = SGCore_RTAdd(tmpsize, tmpsize, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "qq", 0);
		MLSet::IDsRenderTargets::SurfToneMap[tmpcount] = 0;
		++tmpcount;
	}
	MLSet::IDsRenderTargets::CountArrToneMaps = tmpcount;

	MLSet::IDsRenderTargets::AdaptLumCurr = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "", 0);
	MLSet::IDsRenderTargets::AdaptLumLast = SGCore_RTAdd(1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, "", 0);

	MLSet::IDsRenderTargets::LigthCom = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcom", 1);
	MLSet::IDsRenderTargets::LigthCom2 = SGCore_RTAdd(MLSet::WinSize.x, MLSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcom2", 1);
	
	MLSet::IDsRenderTargets::LigthComScaled = SGCore_RTAdd(MLSet::WinSize.x*0.25f, MLSet::WinSize.y*0.25f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, "ds_lightcomscaled", 0.25);
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