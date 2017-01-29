
namespace MLSet
{
	IDirect3DDevice9* DXDevice = 0;
	float3 ConstCurrCamPos;
	float2_t WinSize = float2_t(1024, 768);
	//размер текстуры теней для локальных источников света
	float2_t SizeTexShadowGlobal = float2_t(1024, 768);

	//размер текстуры глубины для локальных источников света
	float2_t SizeTexDepthGlobal = float2_t(1024, 768);

	//размер текстуры теней для локальных источников света
	float2_t SizeTexShadowLocal = float2_t(1024, 768);

	//размер текстуры глубины для локальных источников света
	float2_t SizeTexDepthLocal = float2_t(1024, 768);

	//дистанции для 4 сплитов для глоальных теней PSSM
	float4_t DistForPSSM = float4_t(10.f, 40.f, 100.f, 200.f);

	//коэфициент размера текстур для карт теней локальных источников света
	float CoefSizeShadowMapForLocal = 1;
	//коэфициент размера текстур для карт глубин локальных источников света
	float CoefSizeDepthMapForLocal = 1;

	//коэфициент размера текстур для карт теней глобального источника света
	float CoefSizeShadowMapForGlobal = 1;
	//коэфициент размера текстур для карт глубин глобального источника света
	float CoefSizeDepthMapForGlobal = 1;

	void ReCalcSize()
	{
		SizeTexShadowGlobal.x = WinSize.x * CoefSizeShadowMapForGlobal;
		SizeTexShadowGlobal.y = WinSize.y * CoefSizeShadowMapForGlobal;

		SizeTexDepthGlobal.x = WinSize.x * CoefSizeDepthMapForGlobal;
		SizeTexDepthGlobal.y = WinSize.y * CoefSizeDepthMapForGlobal;

		SizeTexShadowLocal.x = WinSize.x * CoefSizeShadowMapForLocal;
		SizeTexShadowLocal.y = WinSize.y * CoefSizeShadowMapForLocal;

		SizeTexDepthLocal.x = WinSize.x * CoefSizeDepthMapForLocal;
		SizeTexDepthLocal.y = WinSize.y * CoefSizeDepthMapForLocal;
	}

	//fov and ration esesno
	float ProjFov = SM_PI / 4.f;
	float ProjRatio = WinSize.x / WinSize.y;

	//ближняя и дальняя плоскости
	float2_t NearFar = float2_t(0.05f, 400.f);

	float4x4 MCamView;

	bool IsHalfGenPCFShadowLocal = false;

	//ориентация и верхний вектор для рендера в кубическую текстуру
	float3 OrientedCube[6] = { float3(1, 0, 0), float3(-1, 0, 0), float3(0, 1, 0), float3(0, -1, 0), float3(0, 0, 1), float3(0, 0, -1) };
	float3 UpVectorsCube[6] = { float3(0, 1, 0), float3(0, 1, 0), float3(0, 0, -1), float3(0, 0, 1), float3(0, 1, 0), float3(0, 1, 0) };

	namespace IDsShaders
	{
		namespace VS
		{
			DWORD ShadowCreatePSSM_Direct;
			DWORD PPQuadRender;
			DWORD ResPosDepth;
			DWORD ShadowCreateCube;
		};

		namespace PS
		{
			DWORD ShadowCreatePSSM_Direct;
			DWORD PPBlurDepthBasedNoise;
			DWORD PSSM4;
			DWORD PSSM3;
			DWORD PPBlurDepthBased;
			DWORD GenShadowDirect4;
			DWORD GenShadowDirect9;
			DWORD ShadowCreateCube;
			DWORD GenShadowCube1;
			DWORD GenShadowCube6;
		};
	};

	namespace IDsRenderTargets
	{
		DWORD Tex_DSDepthLinear;
		DWORD Tex_DSDepthLinearD2;
		DWORD Tex_DSDepthLinearD4;
	};

	namespace IDsTexs
	{
		DWORD Tex_NoiseTex;
	};
};

void MLInit(IDirect3DDevice9* device, float2_t* winsize, float projfov, DWORD id_depth, DWORD id_depth_d2, DWORD id_depth_d4)
{
	MLSet::DXDevice = device;
	MLSet::WinSize = *winsize;
	MLSet::ProjFov = projfov;
	MLSet::IDsTexs::Tex_NoiseTex = SGCore_LoadTexAddName("noise_rottex.dds");

	MLSet::IDsRenderTargets::Tex_DSDepthLinear = id_depth;
	MLSet::IDsRenderTargets::Tex_DSDepthLinearD2 = id_depth_d2;
	MLSet::IDsRenderTargets::Tex_DSDepthLinearD4 = id_depth_d4;

	MLSet::IDsShaders::VS::PPQuadRender = SGCore_ShaderLoad(0, "pp_quad_render.vs", "pp_quad_render", false);
	MLSet::IDsShaders::VS::ShadowCreatePSSM_Direct = SGCore_ShaderLoad(0, "shadowcreatedepth.vs", "shadowcreatedepth", false);
	MLSet::IDsShaders::PS::ShadowCreatePSSM_Direct = SGCore_ShaderLoad(1, "shadowcreatedepth.ps", "shadowcreatedepth", false);

	MLSet::IDsShaders::VS::ShadowCreateCube = SGCore_ShaderLoad(0, "shadowcreatedepthcube.vs", "shadowcreatedepthcube", false);
	MLSet::IDsShaders::PS::ShadowCreateCube = SGCore_ShaderLoad(1, "shadowcreatedepthcube.ps", "shadowcreatedepthcube", false);

	MLSet::IDsShaders::VS::ResPosDepth = SGCore_ShaderLoad(0, "pp_quad_render_res_pos.vs", "pp_quad_render_res_pos", false);


	MLSet::IDsShaders::PS::PSSM4 = SGCore_ShaderLoad(1, "pp_gen_shadow_map_pssm.ps", "pp_gen_shadow_map_pssm", false);

	D3DXMACRO Defines_SPLITS3[] = { { "SPLITS3", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::PSSM3 = SGCore_ShaderLoad(1, "pp_gen_shadow_map_pssm.ps", "pp_gen_shadow_map_pssm3split", false, Defines_SPLITS3);

	MLSet::IDsShaders::PS::GenShadowDirect4 = SGCore_ShaderLoad(1, "pp_gen_shadow_map_direct.ps", "pp_gen_shadow_map_direct", false);
	D3DXMACRO Defines_GSD_9[] = { { "GSD_9", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::GenShadowDirect9 = SGCore_ShaderLoad(1, "pp_gen_shadow_map_direct.ps", "pp_gen_shadow_map_direct", false, Defines_GSD_9);
	MLSet::IDsShaders::PS::GenShadowCube1 = SGCore_ShaderLoad(1, "pp_gen_shadow_map_point.ps", "pp_gen_shadow_map_point", false);
	D3DXMACRO Defines_GSC_6[] = { { "GSC_6", "" }, { 0, 0 } };
	MLSet::IDsShaders::PS::GenShadowCube6 = SGCore_ShaderLoad(1, "pp_gen_shadow_map_point.ps", "pp_gen_shadow_map_point", false, Defines_GSC_6);

	MLSet::IDsShaders::PS::PPBlurDepthBased = SGCore_ShaderLoad(1, "pp_blur_depth_based.ps", "pp_blur_depth_based", false);
	MLSet::IDsShaders::PS::PPBlurDepthBasedNoise = SGCore_ShaderLoad(1, "pp_blur_depth_based_noise.ps", "pp_blur_depth_based_noise", false);
}