
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define SXPP_VERSION 1

#include "sxpp.h"

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

void SPP_ComEdgeDetected();

namespace PPSet
{
	IDirect3DDevice9* DXDevice = 0;
	float3 ConstCurrCamPos;
	float3 ConstCurrCamDir;
	float2_t WinSize = float2_t(640, 480);

	bool SunExists = false;
	float4 SunPos;

	//fov and ration esesno
	float ProjFov = D3DX_PI * 0.25f;
	float ProjRatio = WinSize.x / WinSize.y;

	float3_t ArrRndVecSSAO[24];

	//ближн¤¤ и дальн¤¤ плоскости
	float2_t NearFar = float2_t(0.25f, 400.f);

	float4x4 MCamView;
	float4x4 MCamViewPrev;
	float4x4 MCamProj;

	void Init();
	
	namespace IDsShaders
	{
		namespace VS
		{
			ID ScreenOut;
			ID ResPos;
		};

		namespace PS
		{
			ID ScreenOut;
			ID FogLinear;
			ID SSAO_Q_1;
			ID SSAO_Q_2;
			ID SSAO_Q_3;
			ID SSAOBlur1;
			ID SSAOBlur2;
			ID WhiteBlack;
			ID Sepia;
			ID CBG;
			ID SunRender;
			ID BloomBP;
			ID BloomFinal;
			ID LensFlare0;
			ID LensFlare2;
			ID MotionBlur;

			ID GaussBlurH;
			ID GaussBlurW;
			ID FreeBlur3x3;

			ID DOF;

			ID DepthEdgeDetect;
			ID NFAA;
			ID DLAA_Small;
			ID DLAA_Long;

			ID idClip;
		};
	};

	namespace IDsRenderTargets
	{
		ID Normal;
		ID Depth0;
		ID Depth1;

		ID DepthD2;

		ID Input;
		ID Output;

		ID EdgeDetected;
		ID EdgeDetected2;

		ID IntermediateWinSize;
		ID IntermediateWinSize2;

		ID IntermediateWinSizeD2;
		ID IntermediateWinSize2D2;

		ID Bright;
		ID Bright2;

		int HowRenderRT = 0;
		inline ID GetRenderRT(){ if (HowRenderRT == 1) return Input; else return  Output; };
		inline ID GetSendRT(){ if (HowRenderRT == 1) return Output; else return  Input; };
		void IncrRT(){ if (HowRenderRT >= 1) HowRenderRT = 0; else HowRenderRT = 1; };
		void NullRT(){ HowRenderRT = 0; };
	};

	namespace IDsTexs
	{
		ID Noise = -1;
		ID Sun = -1;
	};
}

void PPSet::Init()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	PPSet::DXDevice = SGCore_GetDXDevice();
	PPSet::WinSize.x = *r_win_width;
	PPSet::WinSize.y = *r_win_height;
	Core_SetOutPtr();
	PPSet::IDsShaders::VS::ResPos = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs", SHADER_CHECKDOUBLE_PATH);
	
	PPSet::IDsShaders::VS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	PPSet::IDsShaders::PS::ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::FogLinear = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_fog_linear.ps", "ppe_fog_linear.ps", SHADER_CHECKDOUBLE_PATH);
	
	D3DXMACRO Defines_SSAO_Q_3[] = { { "SSAO_Q_3", "" }, { 0, 0 } };
	PPSet::IDsShaders::PS::SSAO_Q_3 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_ssao.ps", "ppe_ssao_q_3.ps", SHADER_CHECKDOUBLE_NAME, Defines_SSAO_Q_3);

	D3DXMACRO Defines_SSAO_Q_2[] = { { "SSAO_Q_2", "" }, { 0, 0 } };
	PPSet::IDsShaders::PS::SSAO_Q_2 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_ssao.ps", "ppe_ssao_q_2.ps", SHADER_CHECKDOUBLE_NAME, Defines_SSAO_Q_2);

	D3DXMACRO Defines_SSAO_Q_1[] = { { "SSAO_Q_1", "" }, { 0, 0 } };
	PPSet::IDsShaders::PS::SSAO_Q_1 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_ssao.ps", "ppe_ssao_q_1.ps", SHADER_CHECKDOUBLE_NAME, Defines_SSAO_Q_1);

	PPSet::IDsShaders::PS::SSAOBlur1 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_ssao_blur.ps", "ppe_ssao_blur.ps", SHADER_CHECKDOUBLE_NAME);
	D3DXMACRO Defines_SSAO_BLEND_COLOR[] = { { "_BLEND_COLOR_", "" }, { 0, 0 } };
	PPSet::IDsShaders::PS::SSAOBlur2 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_ssao_blur.ps", "ppe_ssao_blur_color.ps", SHADER_CHECKDOUBLE_NAME, Defines_SSAO_BLEND_COLOR);

	PPSet::IDsShaders::PS::WhiteBlack = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_black_white.ps", "ppe_black_white.ps", SHADER_CHECKDOUBLE_PATH);
	PPSet::IDsShaders::PS::Sepia = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_sepia.ps", "ppe_sepia.ps", SHADER_CHECKDOUBLE_PATH);
	PPSet::IDsShaders::PS::CBG = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_count_cbg.ps", "ppe_count_cbg.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::SunRender = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_sun_render.ps", "ppe_sun_render.ps", SHADER_CHECKDOUBLE_PATH);

	D3DXMACRO Defines_GaussBlur_H[] = { { "_H_", "" }, { 0, 0 } };
	PPSet::IDsShaders::PS::GaussBlurH = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_gauss_blur.ps", "ppe_gauss_blur_h.ps", SHADER_CHECKDOUBLE_NAME, Defines_GaussBlur_H);
	PPSet::IDsShaders::PS::GaussBlurW = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_gauss_blur.ps", "ppe_gauss_blur_w.ps", SHADER_CHECKDOUBLE_NAME);
	PPSet::IDsShaders::PS::FreeBlur3x3 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_free_blur_3x3.ps", "ppe_free_blur_3x3.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::DOF = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_depth_of_field.ps", "ppe_depth_of_field.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::BloomBP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_bloom_bp.ps", "ppe_bloom_bp.ps", SHADER_CHECKDOUBLE_PATH);
	PPSet::IDsShaders::PS::BloomFinal = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_bloom_final.ps", "ppe_bloom_final.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::LensFlare0 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_lens_flare0.ps", "ppe_lens_flare0.ps", SHADER_CHECKDOUBLE_PATH);
	PPSet::IDsShaders::PS::LensFlare2 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_lens_flare2.ps", "ppe_lens_flare2.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::MotionBlur = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_motion_blur.ps", "ppe_motion_blur.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::NFAA = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_nfaa.ps", "ppe_nfaa.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::DepthEdgeDetect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_depth_edge_detected.ps", "ppe_depth_edge_detected.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::DLAA_Small = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_dlaa_small.ps", "ppe_dlaa_small.ps", SHADER_CHECKDOUBLE_PATH);
	PPSet::IDsShaders::PS::DLAA_Long = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_dlaa_long.ps", "ppe_dlaa_long.ps", SHADER_CHECKDOUBLE_PATH);

	PPSet::IDsShaders::PS::idClip = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_clip.ps", "pp_clip.ps", SHADER_CHECKDOUBLE_NAME);

	//PPSet::IDsTexs::Noise = SGCore_LoadTexAddName("noise_rottex.dds", LOAD_TEXTURE_TYPE_CONST);

	D3DLOCKED_RECT LockedRect;
	uint32_t* tmpOldColor;
	IDirect3DTexture9 *pRnsSampler;
	PPSet::DXDevice->CreateTexture(*r_win_width, *r_win_height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pRnsSampler, NULL);

	//uint32_t aRndColors[16];// = D3DCOLOR_ARGB(0, 250, 2, 255);

	

	pRnsSampler->LockRect(0, &LockedRect, 0, 0);
	
	float3 vRnd;

	for (int i = 0; i < (*r_win_width) * (*r_win_height); ++i)
	{
		vRnd.x = randf(0, 1);
		vRnd.y = randf(0, 1);
		vRnd.z = randf(0, 1);
		vRnd = SMVector3Normalize(vRnd);

		((DWORD*)LockedRect.pBits)[i] = D3DCOLOR_ARGB(255, DWORD(vRnd.x * 255.0), DWORD(vRnd.y * 255.0), DWORD(vRnd.z * 255.0));
	}

	pRnsSampler->UnlockRect(0);

	//SGCore_LoadTexLoadTextures();
	PPSet::IDsTexs::Noise = SGCore_LoadTexCreate("noise_rottex_fullscreen__", pRnsSampler);

	PPSet::IDsTexs::Sun = -1;

	

	for (int i = 0; i<24; i++)
	{
		PPSet::ArrRndVecSSAO[i].x = randf(-1.f, 1.f);
		PPSet::ArrRndVecSSAO[i].z = randf(-1.f, 1.f);
		PPSet::ArrRndVecSSAO[i].y = randf(-1.f, 1.f);

		PPSet::ArrRndVecSSAO[i] = SMVector3Normalize(PPSet::ArrRndVecSSAO[i]);

		float scale = (float)i / 24.f;
		scale = vlerp(0.1f, 1.0f, scale);
		PPSet::ArrRndVecSSAO[i].x *= scale;
		PPSet::ArrRndVecSSAO[i].y *= scale;
		PPSet::ArrRndVecSSAO[i].z *= scale;
	}

	PPSet::IDsRenderTargets::DepthD2 = SGCore_RTAdd(PPSet::WinSize.x * 0.5f, PPSet::WinSize.y * 0.5f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "pp_depth_d2", 0.5);
	PPSet::IDsRenderTargets::EdgeDetected = SGCore_RTAdd(PPSet::WinSize.x, PPSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, "pp_edge_detected", 1);
	PPSet::IDsRenderTargets::EdgeDetected2 = SGCore_RTAdd(PPSet::WinSize.x, PPSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, "pp_edge_detected2", 1);

	PPSet::IDsRenderTargets::IntermediateWinSize = SGCore_RTAdd(PPSet::WinSize.x, PPSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_intermidiatews", 1);
	PPSet::IDsRenderTargets::IntermediateWinSize2 = SGCore_RTAdd(PPSet::WinSize.x, PPSet::WinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_intermidiatews2", 1);

	PPSet::IDsRenderTargets::IntermediateWinSizeD2 = SGCore_RTAdd(PPSet::WinSize.x * 0.5, PPSet::WinSize.y * 0.5, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_intermidiatews_d2", 0.5);
	PPSet::IDsRenderTargets::IntermediateWinSize2D2 = SGCore_RTAdd(PPSet::WinSize.x * 0.5, PPSet::WinSize.y * 0.5, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_intermidiatews2_d2", 0.5);

	PPSet::IDsRenderTargets::Bright = SGCore_RTAdd(PPSet::WinSize.x * 0.5, PPSet::WinSize.y * 0.5, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_bright", 0.5);
	PPSet::IDsRenderTargets::Bright2 = SGCore_RTAdd(PPSet::WinSize.x * 0.5, PPSet::WinSize.y * 0.5, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_bright2", 0.5);
}

#define PP_PRECOND(retval) if(!PPSet::DXDevice){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - post process is not init", GEN_MSG_LOCATION); return retval;}
#define PP_PRECOND_SECOND(retval) if(PPSet::IDsRenderTargets::Input < 0 || PPSet::IDsRenderTargets::Output < 0){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - post process is not init output textures", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SPP_0GetVersion()
{
	return SXPP_VERSION;
}

SX_LIB_API void SPP_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}


SX_LIB_API void SPP_0Create(const char* name, bool is_unic)
{
	if (name && strlen(name) > 1)
	{
		if (is_unic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, name);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
			}
			else
			{
				PPSet::Init();
			}
		}
		else
		{
			PPSet::Init();
		}
	}
	else
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - not init argument [name]", GEN_MSG_LOCATION);
}

SX_LIB_API void SPP_OnLostDevice()
{

}

SX_LIB_API void SPP_OnDeviceReset()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	D3DLOCKED_RECT LockedRect;
	uint32_t* tmpOldColor;
	//ID idTex = SGCore_LoadTexGetID("noise_rottex_fullscreen__");
	IDirect3DTexture9 *pRnsSampler = SGCore_LoadTexGetTex(PPSet::IDsTexs::Noise);
	mem_release_del(pRnsSampler);
	PPSet::DXDevice->CreateTexture(*r_win_width, *r_win_height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pRnsSampler, NULL);

	pRnsSampler->LockRect(0, &LockedRect, 0, 0);

	float3 vRnd;

	for (int i = 0; i < (*r_win_width) * (*r_win_height); ++i)
	{
		vRnd.x = randf(0, 1);
		vRnd.y = randf(0, 1);
		vRnd.z = randf(0, 1);
		vRnd = SMVector3Normalize(vRnd);

		((DWORD*)LockedRect.pBits)[i] = D3DCOLOR_ARGB(255, DWORD(vRnd.x * 255.0), DWORD(vRnd.y * 255.0), DWORD(vRnd.z * 255.0));
	}

	pRnsSampler->UnlockRect(0);

	//SGCore_LoadTexLoadTextures();
	PPSet::IDsTexs::Noise = SGCore_LoadTexCreate("noise_rottex_fullscreen__", pRnsSampler);
}

SX_LIB_API void SPP_0Kill()
{
	PPSet::DXDevice = 0;
}

SX_LIB_API void SPP_RTSetDepth0(ID rt)
{
	PP_PRECOND(_VOID);

	PPSet::IDsRenderTargets::Depth0 = rt;
}

SX_LIB_API void SPP_RTSetDepth1(ID rt)
{
	PP_PRECOND(_VOID);

	PPSet::IDsRenderTargets::Depth1 = rt;
}

SX_LIB_API void SPP_RTSetNormal(ID rt)
{
	PP_PRECOND(_VOID);

	PPSet::IDsRenderTargets::Normal = rt;
}

SX_LIB_API void SPP_RTSetInput(ID rt)
{
	PP_PRECOND(_VOID);

	PPSet::IDsRenderTargets::Input = rt;
}

SX_LIB_API void SPP_RTSetOutput(ID rt)
{
	PP_PRECOND(_VOID);

	PPSet::IDsRenderTargets::Output = rt;
}

SX_LIB_API ID SPP_RTGetCurrRender()
{
	PP_PRECOND(-1);
	PP_PRECOND_SECOND(-1);

	return PPSet::IDsRenderTargets::GetRenderRT();
}

SX_LIB_API ID SPP_RTGetCurrSend()
{
	PP_PRECOND(-1);
	PP_PRECOND_SECOND(-1);
	
	return PPSet::IDsRenderTargets::GetSendRT();
}

SX_LIB_API void SPP_RTIncr()
{
	PP_PRECOND(_VOID);
	PP_PRECOND_SECOND(_VOID);

	PPSet::IDsRenderTargets::IncrRT();
}

SX_LIB_API void SPP_RTNull()
{
	PP_PRECOND(_VOID);
	PP_PRECOND_SECOND(_VOID);

	PPSet::IDsRenderTargets::NullRT();
	SPP_ComEdgeDetected();
}

//##########################################################################

SX_LIB_API void SPP_Update()
{
	PP_PRECOND(_VOID);
	
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");


	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &PPSet::ConstCurrCamPos);
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_DIRECTION, &PPSet::ConstCurrCamDir);

	PPSet::MCamViewPrev = PPSet::MCamView;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &PPSet::MCamView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &PPSet::MCamProj);

	PPSet::WinSize.x = *r_win_width;
	PPSet::WinSize.y = *r_win_height;

	PPSet::ProjRatio = PPSet::WinSize.x / PPSet::WinSize.y;

	PPSet::NearFar.x = *r_near;
	PPSet::NearFar.y = *r_far;

	PPSet::ProjFov = *r_default_fov;
}

//##########################################################################

SX_LIB_API void SPP_RenderFogLinear(float3_t* color, float density)
{
	PP_PRECOND(_VOID);
	PP_PRECOND_SECOND(_VOID);

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);
	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	PPSet::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	PPSet::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	PPSet::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	PPSet::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth0));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::FogLinear, "g_vFogColor", color);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::FogLinear, "g_fFogDenisty", &density);
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::FogLinear, "NearFar", &PPSet::NearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::FogLinear);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

SX_LIB_API void SPP_RenderSSAO(float4_t* param, int quality)
{
	PP_PRECOND(_VOID);
	PP_PRECOND_SECOND(_VOID);

	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::IntermediateWinSize)/*SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT())*/->GetSurfaceLevel(0, &RenderSurf);
	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	PPSet::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);

	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth1));
	PPSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Normal));
	PPSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(PPSet::IDsTexs::Noise));

	ID idpsshader = PPSet::IDsShaders::PS::SSAO_Q_1;

	if (quality == 3)
		idpsshader = PPSet::IDsShaders::PS::SSAO_Q_3;
	else if (quality == 2)
		idpsshader = PPSet::IDsShaders::PS::SSAO_Q_2;

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_vNearFar", &PPSet::NearFar);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_vParams", &float4(param->x, param->y, param->z, param->w));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_aRndVectors", &PPSet::ArrRndVecSSAO);
	//float scale_rnd = PPSet::WinSize.x * 0.25f;
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "Ratio", &float2_t(PPSet::WinSize.x / PPSet::WinSize.y, 1));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_vViewPos", &PPSet::ConstCurrCamPos);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_vViewProj", &SMMatrixTranspose(PPSet::MCamView * PPSet::MCamProj));

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, PPSet::MCamView);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ResPos, "g_mViewInv", &ViewInv);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ResPos, "g_vNearFar", &PPSet::NearFar);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ResPos, "g_vParamProj", &float3_t(PPSet::WinSize.x, PPSet::WinSize.y, PPSet::ProjFov));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ResPos);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, idpsshader);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//PPSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);
	//mem_release(BackBuf);




	/*SGCore_RTGetTexture(PPSet::IDsRenderTargets::IntermediateWinSize2)->GetSurfaceLevel(0, &RenderSurf);
	//PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth1));
	PPSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(PPSet::IDsRenderTargets::IntermediateWinSize));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SSAOBlur1, "g_vPixelSize", &float2_t(2.0f / PPSet::WinSize.x, 2.0f / PPSet::WinSize.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SSAOBlur1, "g_vNearFar", &PPSet::NearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SSAOBlur1);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//PPSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);*/
	//mem_release(BackBuf);
	
	PPSet::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);
	//PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth1));
	PPSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(PPSet::IDsRenderTargets::IntermediateWinSize));
	PPSet::DXDevice->SetTexture(2, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SSAOBlur2, "g_vPixelSize", &float2_t(1.f / PPSet::WinSize.x, 1.f / PPSet::WinSize.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SSAOBlur2, "g_vNearFar", &PPSet::NearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SSAOBlur2);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	PPSet::IDsRenderTargets::IncrRT();

	/*if (GetKeyState('N'))
	{
		//MessageBox(0,"PP GetKeyState",0,0);
		char tmppath[1024];
		sprintf(tmppath, "%sssao.jpg", "C:\\1\\");
		D3DXSaveTextureToFile(tmppath, D3DXIFF_JPG, SGCore_RTGetTexture(PPSet::IDsRenderTargets::IntermediateWinSize), NULL);
	}*/
}


SX_LIB_API void SPP_RenderWhiteBlack(float coef)
{
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::WhiteBlack, "g_fCoef", &coef);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::WhiteBlack);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::IDsRenderTargets::IncrRT();
}

SX_LIB_API void SPP_RenderSepia(float coef)
{
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::Sepia, "g_vCountSepia", &coef);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::Sepia, "g_vLightColor", &PP_SEPIA_LIGHT_COLOR);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::Sepia, "g_vDarkColor", &PP_SEPIA_DARK_COLOR);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::Sepia, "g_vLumWeights", &PP_SEPIA_WEIGHTS_COLOR);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::Sepia);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::IDsRenderTargets::IncrRT();
}

SX_LIB_API void SPP_RenderCBG(float3_t* param)
{
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::CBG, "g_vParam", &param);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::CBG);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::IDsRenderTargets::IncrRT();
}

//##########################################################################

SX_LIB_API void SPP_RenderDOF(float4_t* param, float sky_blur)
{
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT())->GetLevelDesc(0, &desc);
	float2_t tmpSizeMap = float2_t(desc.Width, desc.Height);

	float tmpcoefblurbloom = 1.f;
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurH, "g_fCoefBlur", &tmpcoefblurbloom);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurH, "g_vSizeMap", &tmpSizeMap);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurH);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	//mem_release(BackBuf);



	SGCore_RTGetTexture(PPSet::IDsRenderTargets::IntermediateWinSize)->GetSurfaceLevel(0, &RenderSurf);

	//PPSet::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurW, "g_fCoefBlur", &tmpcoefblurbloom);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurW, "g_vSizeMap", &tmpSizeMap);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurW);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	//mem_release(BackBuf);



	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	//PPSet::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress2(0, 4, D3DTADDRESS_CLAMP);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));
	PPSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth0));
	PPSet::DXDevice->SetTexture(2, SGCore_RTGetTexture(PPSet::IDsRenderTargets::IntermediateWinSize));

	float tmpskyblur = 0.0f;
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DOF, "g_vParam", param);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DOF, "g_fSkyBlur", &sky_blur);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DOF, "g_vNearFar", &PPSet::NearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DOF);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::IDsRenderTargets::IncrRT();
}


SX_LIB_API void SPP_UpdateSun(float3* sunpos)
{
	if (sunpos == 0)
	{
		PPSet::SunExists = false;
		return;
	}
	else
		PPSet::SunExists = true;

	float3 sunPos = *sunpos;

	sunPos = SMVector4Transform(sunPos, PPSet::MCamView * PPSet::MCamProj);

	sunPos.x /= sunPos.w;
	sunPos.y /= sunPos.w;
	sunPos.z /= sunPos.w;
	sunPos.w /= sunPos.w;

	sunPos.x *= 0.5;
	sunPos.y *= -0.5;

	sunPos.x += 0.5;
	sunPos.y += 0.5;

	float3 tmpVec(PPSet::ConstCurrCamDir.x, PPSet::ConstCurrCamDir.y, PPSet::ConstCurrCamDir.z);
	tmpVec = SMVector3Normalize(tmpVec);

	float3 dirVec = (*sunpos) - PPSet::ConstCurrCamPos;

	dirVec = SMVector3Normalize(dirVec);
	sunPos.w = abs(acosf(SMVector3Dot(tmpVec, dirVec)) * sign(SMVector3Cross(tmpVec, dirVec).y));
		
	PPSet::SunPos = sunPos;
}

SX_LIB_API void SPP_ChangeTexSun(const char* str)
{
	PPSet::IDsTexs::Sun = SGCore_LoadTexAddName(str, LOAD_TEXTURE_TYPE_CONST);
}

SX_LIB_API void SPP_RenderSun(float4_t* sun_color)
{
	if (!PPSet::SunExists)
		return;

	if (PPSet::SunPos.w > PP_MAX_ANGLE_VISIBLE_SUN)
		return;

	if (PPSet::IDsTexs::Sun < 0 || !SGCore_LoadTexGetTex(PPSet::IDsTexs::Sun))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - sun texture is not init, process can not be started\n", GEN_MSG_LOCATION);
		return;
	}

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);
	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter2(0, 4, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress2(0, 4, D3DTADDRESS_CLAMP);

	D3DSURFACE_DESC desc;
	SGCore_LoadTexGetTex(PPSet::IDsTexs::Sun)->GetLevelDesc(0, &desc);

	float2_t SizeMapSun = float2_t(desc.Width, desc.Height);

	RECT rectscissor;
	rectscissor.top = PPSet::SunPos.y * PPSet::WinSize.y - desc.Height*0.5f;
	rectscissor.left = PPSet::SunPos.x * PPSet::WinSize.x - desc.Width*0.5f;
	rectscissor.bottom = rectscissor.top + desc.Height;
	rectscissor.right = rectscissor.left + desc.Width;

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//PPSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);



	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);
	//PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth0));
	PPSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(PPSet::IDsTexs::Sun));
	PPSet::DXDevice->SetTexture(3, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT()));


	PPSet::DXDevice->SetScissorRect(&rectscissor);
	PPSet::DXDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	PPSet::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	PPSet::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	PPSet::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	PPSet::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SunRender, "g_vLightPos", &PPSet::SunPos);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SunRender, "g_vSizeMap", &(PPSet::WinSize));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SunRender, "g_vSizeTexSun", &SizeMapSun);
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SunRender, "PixelSize", &float2_t(1.f / PPSet::WinSize.x, 1.f / PPSet::WinSize.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SunRender, "g_vLightColor", sun_color);
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SunRender, "NearFar", &PPSet::NearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SunRender);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//PPSet::IDsRenderTargets::IncrRT();
}

SX_LIB_API void SPP_RenderLensFlare(float3_t* param, float4_t* sun_color, bool use_bloom)
{
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	PPSet::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	PPSet::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	PPSet::DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright)->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	if (PPSet::SunExists && PPSet::SunPos.w < PP_MAX_ANGLE_VISIBLE_SUN && sun_color->w > 0.3)
	{
		if (!use_bloom)
			PPSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		SGCore_SetSamplerFilter(0, D3DTADDRESS_MIRROR);
		PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth0));

		D3DSURFACE_DESC desc;
		SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright)->GetLevelDesc(0, &desc);
		float2_t tmpSizeMap = float2_t(desc.Width, desc.Height);

		float LensFlareSunRadius = PP_SUN_RADIUS;
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::LensFlare0, "g_vLightPos", &PPSet::SunPos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::LensFlare0, "g_vSizeMap", &tmpSizeMap);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::LensFlare0, "g_fRadiusSun", &LensFlareSunRadius);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::LensFlare0, "g_vColor", sun_color);
		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::LensFlare0, "NearFar", &PPSet::NearFar);
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::LensFlare0);

		SGCore_ScreenQuadDraw();

		SGCore_ShaderUnBind();
	}
	else
	{
		if (!use_bloom)
			PPSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	}

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	/*if (GetKeyState('N'))
	{
		//MessageBox(0,"PP GetKeyState",0,0);
		char tmppath[1024];
		sprintf(tmppath, "%sbp.jpg", "C:\\1\\");
		D3DXSaveTextureToFile(tmppath, D3DXIFF_JPG, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright), NULL);
	}*/


	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTADDRESS_MIRROR);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::LensFlare2, "g_vLensFlareParam", param);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::LensFlare2);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

SX_LIB_API void SPP_RenderBloom(float3_t* param)
{
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright)->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::BloomBP, "g_vParam", param);
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::BloomBP);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);

	/*if (GetKeyState('N'))
	{
		D3DXSaveTextureToFile("C:\\1\\sbp.png", D3DXIFF_PNG, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright), NULL);
	}*/


	SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright2)->GetSurfaceLevel(0, &RenderSurf);

	//PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright));

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright)->GetLevelDesc(0, &desc);
	float2_t tmpSizeMap = float2_t(desc.Width, desc.Height);

	float tmpcoefblurbloom = 2.f;
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurH, "g_fCoefBlur", &tmpcoefblurbloom);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurH, "g_vSizeMap", &tmpSizeMap);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurH);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);



	SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright)->GetSurfaceLevel(0, &RenderSurf);

	//PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright2));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurW, "g_fCoefBlur", &tmpcoefblurbloom);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurW, "g_vSizeMap", &tmpSizeMap);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::GaussBlurW);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);

	/*if (SSInput_GetKeyState(DIK_M))
	{
		char tmppath[1024];
		sprintf(tmppath, "%sbp_blur.png", GData::Pathes::WorkingTex);
		D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright), NULL);
	}*/

	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	PPSet::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	PPSet::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	PPSet::DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);

	//PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Bright));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::BloomFinal);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//##########################################################################

void SPP_ComEdgeDetected()
{
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	/*SGCore_RTGetTexture(PPSet::IDsRenderTargets::DepthD2)->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);
	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth1));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	mem_release(RenderSurf);*/

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);
	//SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);

	SGCore_RTGetTexture(PPSet::IDsRenderTargets::EdgeDetected)->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);
	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth1));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DepthEdgeDetect, "g_vPixelSize", &float2_t(1.f / PPSet::WinSize.x, 1.f / PPSet::WinSize.y));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DepthEdgeDetect);

	SGCore_ScreenQuadDraw();

	mem_release(RenderSurf);
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::EdgeDetected2)->GetSurfaceLevel(0, &RenderSurf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	//PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth1));
	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::EdgeDetected));

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::EdgeDetected)->GetLevelDesc(0, &desc);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::FreeBlur3x3, "g_vPixelSize", &float2_t(1.0f / desc.Width, 1.0f / desc.Height));
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::SSAOBlur1, "g_vNearFar", &PPSet::NearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::FreeBlur3x3);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(RenderSurf);
	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);



	PPSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	PPSet::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);

	PPSet::DXDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f, 0);

	PPSet::DXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	PPSet::DXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	PPSet::DXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	PPSet::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	PPSet::DXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	PPSet::DXDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	PPSet::DXDevice->SetRenderState(D3DRS_STENCILREF, 1);
	PPSet::DXDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
	PPSet::DXDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);

	

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::idClip);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::EdgeDetected2));
	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);

	SGCore_ScreenQuadDraw();

	PPSet::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	if (GetAsyncKeyState('N'))
	{
		D3DXSaveTextureToFile("C:\\1\\EdgeDetected.jpg", D3DXIFF_JPG, SGCore_RTGetTexture(PPSet::IDsRenderTargets::EdgeDetected), NULL);
		D3DXSaveTextureToFile("C:\\1\\EdgeDetected2.jpg", D3DXIFF_JPG, SGCore_RTGetTexture(PPSet::IDsRenderTargets::EdgeDetected2), NULL);
	}
}

SX_LIB_API void SPP_RenderNFAA(float3_t* param)
{
	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	//**

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::NFAA, "g_vParam", param);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::NFAA, "g_vPixelSize", &float2_t(1.f / PPSet::WinSize.x, 1.f / PPSet::WinSize.y));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::NFAA);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::IDsRenderTargets::IncrRT();

	PPSet::DXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

SX_LIB_API void SPP_RenderDLAA()
{
	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	PPSet::DXDevice->GetRenderTarget(0, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	//**

	PPSet::DXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	PPSet::DXDevice->SetRenderState(D3DRS_STENCILREF, 1);
	PPSet::DXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	PPSet::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));
	
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DLAA_Long);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DLAA_Long, "g_vPixelSize", &float2_t(1.f / PPSet::WinSize.x, 1.f / PPSet::WinSize.y));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(RenderSurf);

	//**

	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DLAA_Small, "g_vPixelSize", &float2_t(1.f / PPSet::WinSize.x, 1.f / PPSet::WinSize.y));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::DLAA_Small);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::DXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//##########################################################################

SX_LIB_API void SPP_RenderMotionBlur(float coef, DWORD timeDelta)
{
	PPSet::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	PPSet::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	PPSet::DXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress2(0, 2, D3DTADDRESS_CLAMP);

	PPSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(PPSet::IDsRenderTargets::GetSendRT()));
	PPSet::DXDevice->SetTexture(1, SGCore_RTGetTexture(PPSet::IDsRenderTargets::Depth0));

	float determ = 0;
	float4x4 ViewInv = SMMatrixTranspose(SMMatrixInverse(&determ, PPSet::MCamView));
	//float4x4 ViewPrevInv = SMMatrixTranspose(SMMatrixInverse(&determ, PPSet::MCamViewPrev));

	//float4x4 ViewProjInv = SMMatrixTranspose(SMMatrixInverse(&determ, PPSet::MCamView * PPSet::MCamProj));
	float4x4 ViewProjPrev = (SMMatrixTranspose(PPSet::MCamViewPrev * PPSet::MCamProj));

	//float4x4 tmpProj = SMMatrixTranspose(PPSet::MCamView*PPSet::MCamProj);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ResPos, "g_mViewInv", &ViewInv);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ResPos, "g_vNearFar", &PPSet::NearFar);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ResPos, "g_vParamProj", &float3_t(PPSet::WinSize.x, PPSet::WinSize.y, PPSet::ProjFov));
	
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::MotionBlur, "g_vViewPos", &PPSet::ConstCurrCamPos);
	float tmpcoefblur = float(timeDelta) * 0.001f * ((coef)*10.f);// 0.3f;// *(float(timeDelta) * 0.001f);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::MotionBlur, "g_fCoefBlur", &tmpcoefblur);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::MotionBlur, "g_vNearFar", &PPSet::NearFar);

	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::MotionBlur, "InvViewProj", &ViewProjInv);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::MotionBlur, "g_mVPprev", &ViewProjPrev);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, PPSet::IDsShaders::VS::ResPos);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, PPSet::IDsShaders::PS::MotionBlur);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	PPSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	PPSet::IDsRenderTargets::IncrRT();
}
