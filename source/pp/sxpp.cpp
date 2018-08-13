
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

namespace pp_data
{
	IDirect3DDevice9 *pDXDevice = 0;
	float3 vConstCurrCamPos;
	float3 vConstCurrCamDir;
	float2_t vWinSize = float2_t(640, 480);

	bool existsSun = false;
	float4 vSunPos;

	//fov and ration esesno
	float fProjFov = D3DX_PI * 0.25f;
	float fProjRatio = vWinSize.x / vWinSize.y;

	float3_t aRndVecSSAO[24];

	//ближняя и дальняя плоскости
	float2_t vNearFar = float2_t(0.25f, 400.f);

	float4x4 mCamView;
	float4x4 mCamViewPrev;
	float4x4 mCamProj;

	void Init();
	void InitNoiseTex();
	
	namespace shaders_id
	{
		namespace vs
		{
			ID idScreenOut = -1;
			ID idResPos = -1;
		};

		namespace ps
		{
			ID idScreenOut = -1;
			ID idFogLinear = -1;
			ID idSSAO_Q_1 = -1;
			ID idSSAO_Q_2 = -1;
			ID idSSAO_Q_3 = -1;
			ID idBlurDepthBased25BlendColor = -1;
			ID idWhiteBlack = -1;
			ID idSepia = -1;
			ID idCBG = -1;
			ID idSunRender = -1;
			ID idBloomBP = -1;
			ID idBloomFinal = -1;
			ID idLensFlare0 = -1;
			ID idLensFlare2 = -1;
			ID idMotionBlur = -1;

			ID idGaussBlurH = -1;
			ID idGaussBlurW = -1;
			ID idFreeBlur3x3 = -1;

			ID idDOF = -1;

			ID idDepthEdgeDetect = -1;
			ID idNFAA = -1;
			ID idDLAA_Small = -1;
			ID idDLAA_Long = -1;

			ID idClip = -1;
		};
	};

	namespace rt_id
	{
		ID idNormal = -1;
		ID idDepth0 = -1;
		ID idDepth1 = -1;

		ID idDepthD2 = -1;

		ID idInput = -1;
		ID idOutput = -1;

		ID idEdgeDetected = -1;
		ID idEdgeDetected2 = -1;

		ID idIntermediateWinSize = -1;
		ID idIntermediateWinSize2 = -1;

		//ID IntermediateWinSizeD2 = -1;
		//ID IntermediateWinSize2D2 = -1;

		ID idBright = -1;
		ID idBright2 = -1;

		int HowRenderRT = 0;
		inline ID GetRenderRT(){ if (HowRenderRT == 1) return idInput; else return idOutput; };
		inline ID GetSendRT(){ if (HowRenderRT == 1) return idOutput; else return idInput; };
		void IncrRT(){ if (HowRenderRT >= 1) HowRenderRT = 0; else HowRenderRT = 1; };
		void NullRT(){ HowRenderRT = 0; };
	};

	namespace tex_id
	{
		ID idNoise = -1;
		ID idSun = -1;
	};
}

void pp_data::Init()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	pp_data::pDXDevice = SGCore_GetDXDevice();
	pp_data::vWinSize.x = *r_win_width;
	pp_data::vWinSize.y = *r_win_height;
	Core_SetOutPtr();
	pp_data::shaders_id::vs::idResPos = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_res_pos.vs", "pp_quad_render_res_pos.vs", SHADER_CHECKDOUBLE_PATH);
	
	pp_data::shaders_id::vs::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	pp_data::shaders_id::ps::idScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idFogLinear = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_fog_linear.ps", "ppe_fog_linear.ps", SHADER_CHECKDOUBLE_PATH);
	
	D3DXMACRO Defines_SSAO_Q_3[] = { { "SSAO_Q_3", "" }, { 0, 0 } };
	pp_data::shaders_id::ps::idSSAO_Q_3 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_ssao.ps", "ppe_ssao_q_3.ps", SHADER_CHECKDOUBLE_NAME, Defines_SSAO_Q_3);

	D3DXMACRO Defines_SSAO_Q_2[] = { { "SSAO_Q_2", "" }, { 0, 0 } };
	pp_data::shaders_id::ps::idSSAO_Q_2 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_ssao.ps", "ppe_ssao_q_2.ps", SHADER_CHECKDOUBLE_NAME, Defines_SSAO_Q_2);

	D3DXMACRO Defines_SSAO_Q_1[] = { { "SSAO_Q_1", "" }, { 0, 0 } };
	pp_data::shaders_id::ps::idSSAO_Q_1 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_ssao.ps", "ppe_ssao_q_1.ps", SHADER_CHECKDOUBLE_NAME, Defines_SSAO_Q_1);

	D3DXMACRO Defines_SAMPLES25_BLENDCOLOR[] = { { "SAMPLES_25", "" }, { "BLEND_COLOR", "" }, { 0, 0 } };
	pp_data::shaders_id::ps::idBlurDepthBased25BlendColor = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_blur_depth_based.ps", "pp_blur_depth_based_25_blend_color.ps", SHADER_CHECKDOUBLE_NAME, Defines_SAMPLES25_BLENDCOLOR);
	
	pp_data::shaders_id::ps::idWhiteBlack = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_black_white.ps", "ppe_black_white.ps", SHADER_CHECKDOUBLE_PATH);
	pp_data::shaders_id::ps::idSepia = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_sepia.ps", "ppe_sepia.ps", SHADER_CHECKDOUBLE_PATH);
	pp_data::shaders_id::ps::idCBG = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_count_cbg.ps", "ppe_count_cbg.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idSunRender = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_sun_render.ps", "ppe_sun_render.ps", SHADER_CHECKDOUBLE_PATH);

	D3DXMACRO Defines_GaussBlur_H[] = { { "_H_", "" }, { 0, 0 } };
	pp_data::shaders_id::ps::idGaussBlurH = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_gauss_blur.ps", "ppe_gauss_blur_h.ps", SHADER_CHECKDOUBLE_NAME, Defines_GaussBlur_H);
	pp_data::shaders_id::ps::idGaussBlurW = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_gauss_blur.ps", "ppe_gauss_blur_w.ps", SHADER_CHECKDOUBLE_NAME);
	pp_data::shaders_id::ps::idFreeBlur3x3 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_free_blur_3x3.ps", "ppe_free_blur_3x3.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idDOF = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_depth_of_field.ps", "ppe_depth_of_field.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idBloomBP = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_bloom_bp.ps", "ppe_bloom_bp.ps", SHADER_CHECKDOUBLE_PATH);
	pp_data::shaders_id::ps::idBloomFinal = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_bloom_final.ps", "ppe_bloom_final.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idLensFlare0 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_lens_flare0.ps", "ppe_lens_flare0.ps", SHADER_CHECKDOUBLE_PATH);
	pp_data::shaders_id::ps::idLensFlare2 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_lens_flare2.ps", "ppe_lens_flare2.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idMotionBlur = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_motion_blur.ps", "ppe_motion_blur.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idNFAA = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_nfaa.ps", "ppe_nfaa.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idDepthEdgeDetect = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_depth_edge_detected.ps", "ppe_depth_edge_detected.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idDLAA_Small = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_dlaa_small.ps", "ppe_dlaa_small.ps", SHADER_CHECKDOUBLE_PATH);
	pp_data::shaders_id::ps::idDLAA_Long = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "ppe_dlaa_long.ps", "ppe_dlaa_long.ps", SHADER_CHECKDOUBLE_PATH);

	pp_data::shaders_id::ps::idClip = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_clip.ps", "pp_clip.ps", SHADER_CHECKDOUBLE_NAME);

	
	pp_data::InitNoiseTex();

	pp_data::tex_id::idSun = -1;

	for (int i = 0; i<24; i++)
	{
		pp_data::aRndVecSSAO[i].x = randf(-1.f, 1.f);
		pp_data::aRndVecSSAO[i].z = randf(-1.f, 1.f);
		pp_data::aRndVecSSAO[i].y = randf(-1.f, 1.f);

		pp_data::aRndVecSSAO[i] = SMVector3Normalize(pp_data::aRndVecSSAO[i]);

		float scale = (float)i / 24.f;
		scale = vlerp(0.1f, 1.0f, scale);
		pp_data::aRndVecSSAO[i].x *= scale;
		pp_data::aRndVecSSAO[i].y *= scale;
		pp_data::aRndVecSSAO[i].z *= scale;
	}

	pp_data::rt_id::idDepthD2 = SGCore_RTAdd(pp_data::vWinSize.x * 0.5f, pp_data::vWinSize.y * 0.5f, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "pp_depth_d2", 0.5);
	pp_data::rt_id::idEdgeDetected = SGCore_RTAdd(pp_data::vWinSize.x, pp_data::vWinSize.y, 1, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, "pp_edge_detected", 1);
	pp_data::rt_id::idEdgeDetected2 = SGCore_RTAdd(pp_data::vWinSize.x, pp_data::vWinSize.y, 1, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, "pp_edge_detected2", 1);

	pp_data::rt_id::idIntermediateWinSize = SGCore_RTAdd(pp_data::vWinSize.x, pp_data::vWinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_intermidiatews", 1);
	pp_data::rt_id::idIntermediateWinSize2 = SGCore_RTAdd(pp_data::vWinSize.x, pp_data::vWinSize.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_intermidiatews2", 1);

	//pp_data::rt_id::IntermediateWinSizeD2 = SGCore_RTAdd(pp_data::vWinSize.x * 0.5, pp_data::vWinSize.y * 0.5, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_intermidiatews_d2", 0.5);
	//pp_data::rt_id::IntermediateWinSize2D2 = SGCore_RTAdd(pp_data::vWinSize.x * 0.5, pp_data::vWinSize.y * 0.5, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_intermidiatews2_d2", 0.5);

	pp_data::rt_id::idBright = SGCore_RTAdd(pp_data::vWinSize.x * 0.5, pp_data::vWinSize.y * 0.5, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_bright", 0.5);
	pp_data::rt_id::idBright2 = SGCore_RTAdd(pp_data::vWinSize.x * 0.5, pp_data::vWinSize.y * 0.5, 0, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, "pp_bright2", 0.5);
}

void pp_data::InitNoiseTex()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	D3DLOCKED_RECT oLockedRect;

	IDirect3DTexture9 *pRnsSampler = 0;

	if (pp_data::tex_id::idNoise != -1)
	{
		pRnsSampler = SGCore_LoadTexGetTex(pp_data::tex_id::idNoise);
		mem_release_del(pRnsSampler);
	}

	pp_data::pDXDevice->CreateTexture(*r_win_width, *r_win_height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pRnsSampler, NULL);

	pRnsSampler->LockRect(0, &oLockedRect, 0, 0);

	float3 vRnd;

	for (int i = 0; i < (*r_win_width) * (*r_win_height); ++i)
	{
		vRnd.x = randf(0, 1);
		vRnd.y = randf(0, 1);
		vRnd.z = randf(0, 1);
		vRnd = SMVector3Normalize(vRnd);

		((DWORD*)oLockedRect.pBits)[i] = D3DCOLOR_ARGB(255, DWORD(vRnd.x * 255.0), DWORD(vRnd.y * 255.0), DWORD(vRnd.z * 255.0));
	}

	pRnsSampler->UnlockRect(0);

	pp_data::tex_id::idNoise = SGCore_LoadTexCreate("noise_rottex_fullscreen__", pRnsSampler);
}

#define PP_PRECOND(retval) if(!pp_data::pDXDevice){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - post process is not init", GEN_MSG_LOCATION); return retval;}
#define PP_PRECOND_SECOND(retval) if(pp_data::rt_id::idInput < 0 || pp_data::rt_id::idOutput < 0){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - post process is not init output textures", GEN_MSG_LOCATION); return retval;}

//##########################################################################

SX_LIB_API long SPP_0GetVersion()
{
	return SXPP_VERSION;
}

SX_LIB_API void SPP_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}


SX_LIB_API void SPP_0Create(const char *szName, bool isUnic)
{
	if (szName && strlen(szName) > 1)
	{
		if (isUnic)
		{
			HANDLE hMutex = CreateMutex(NULL, FALSE, szName);
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				CloseHandle(hMutex);
				LibReport(REPORT_MSG_LEVEL_ERROR, "%s - none unic name", GEN_MSG_LOCATION);
			}
			else
			{
				pp_data::Init();
			}
		}
		else
		{
			pp_data::Init();
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
	pp_data::InitNoiseTex();
}

SX_LIB_API void SPP_0Kill()
{
	pp_data::pDXDevice = 0;
}

SX_LIB_API void SPP_RTSetDepth0(ID rt)
{
	PP_PRECOND(_VOID);

	pp_data::rt_id::idDepth0 = rt;
}

SX_LIB_API void SPP_RTSetDepth1(ID rt)
{
	PP_PRECOND(_VOID);

	pp_data::rt_id::idDepth1 = rt;
}

SX_LIB_API void SPP_RTSetNormal(ID rt)
{
	PP_PRECOND(_VOID);

	pp_data::rt_id::idNormal = rt;
}

SX_LIB_API void SPP_RTSetInput(ID rt)
{
	PP_PRECOND(_VOID);

	pp_data::rt_id::idInput = rt;
}

SX_LIB_API void SPP_RTSetOutput(ID rt)
{
	PP_PRECOND(_VOID);

	pp_data::rt_id::idOutput = rt;
}

SX_LIB_API ID SPP_RTGetCurrRender()
{
	PP_PRECOND(-1);
	PP_PRECOND_SECOND(-1);

	return pp_data::rt_id::GetRenderRT();
}

SX_LIB_API ID SPP_RTGetCurrSend()
{
	PP_PRECOND(-1);
	PP_PRECOND_SECOND(-1);
	
	return pp_data::rt_id::GetSendRT();
}

SX_LIB_API void SPP_RTIncr()
{
	PP_PRECOND(_VOID);
	PP_PRECOND_SECOND(_VOID);

	pp_data::rt_id::IncrRT();
}

SX_LIB_API void SPP_RTNull()
{
	PP_PRECOND(_VOID);
	PP_PRECOND_SECOND(_VOID);

	pp_data::rt_id::NullRT();
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


	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &pp_data::vConstCurrCamPos);
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_DIRECTION, &pp_data::vConstCurrCamDir);

	pp_data::mCamViewPrev = pp_data::mCamView;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &pp_data::mCamView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &pp_data::mCamProj);

	pp_data::vWinSize.x = *r_win_width;
	pp_data::vWinSize.y = *r_win_height;

	pp_data::fProjRatio = pp_data::vWinSize.x / pp_data::vWinSize.y;

	pp_data::vNearFar.x = *r_near;
	pp_data::vNearFar.y = *r_far;

	pp_data::fProjFov = *r_default_fov;
}

//##########################################################################

SX_LIB_API void SPP_RenderFogLinear(float3_t* color, float density)
{
	PP_PRECOND(_VOID);
	PP_PRECOND_SECOND(_VOID);

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);
	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pp_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pp_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	pp_data::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pp_data::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idDepth0));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idFogLinear, "g_vFogColor", color);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idFogLinear, "g_fFogDenisty", &density);
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::FogLinear, "vNearFar", &pp_data::vNearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idFogLinear);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

SX_LIB_API void SPP_RenderSSAO(float4_t* param, int quality)
{
	PP_PRECOND(_VOID);
	PP_PRECOND_SECOND(_VOID);

	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::idIntermediateWinSize)->GetSurfaceLevel(0, &RenderSurf);
	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	pp_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	
	SGCore_SetSamplerFilter2(0, 3, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress2(0, 3, D3DTADDRESS_CLAMP);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idDepth1));
	pp_data::pDXDevice->SetTexture(1, SGCore_RTGetTexture(pp_data::rt_id::idNormal));
	pp_data::pDXDevice->SetTexture(2, SGCore_LoadTexGetTex(pp_data::tex_id::idNoise));

	ID idpsshader = pp_data::shaders_id::ps::idSSAO_Q_1;

	if (quality == 3)
		idpsshader = pp_data::shaders_id::ps::idSSAO_Q_3;
	else if (quality == 2)
		idpsshader = pp_data::shaders_id::ps::idSSAO_Q_2;

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_vNearFar", &pp_data::vNearFar);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_vParams", &float4(param->x, param->y, param->z, param->w));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_aRndVectors", &pp_data::aRndVecSSAO);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_vViewPos", &pp_data::vConstCurrCamPos);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idpsshader, "g_vViewProj", &SMMatrixTranspose(pp_data::mCamView * pp_data::mCamProj));

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, pp_data::mCamView);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idResPos, "g_mViewInv", &ViewInv);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idResPos, "g_vNearFar", &pp_data::vNearFar);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idResPos, "g_vParamProj", &float3_t(pp_data::vWinSize.x, pp_data::vWinSize.y, pp_data::fProjFov));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idResPos);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, idpsshader);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(RenderSurf);


	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idDepth1));
	pp_data::pDXDevice->SetTexture(1, SGCore_RTGetTexture(pp_data::rt_id::idIntermediateWinSize));
	pp_data::pDXDevice->SetTexture(2, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idBlurDepthBased25BlendColor, "g_vPixelSize", &float2_t(1.f / pp_data::vWinSize.x, 1.f / pp_data::vWinSize.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idBlurDepthBased25BlendColor, "g_vNearFar", &pp_data::vNearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idBlurDepthBased25BlendColor);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pp_data::rt_id::IncrRT();

	/*if (GetKeyState('N'))
	{
		//MessageBox(0,"PP GetKeyState",0,0);
		char tmppath[1024];
		sprintf(tmppath, "%sssao.jpg", "C:\\1\\");
		D3DXSaveTextureToFile(tmppath, D3DXIFF_JPG, SGCore_RTGetTexture(pp_data::rt_id::IntermediateWinSize), NULL);
	}*/
}


SX_LIB_API void SPP_RenderWhiteBlack(float coef)
{
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idWhiteBlack, "g_fCoef", &coef);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idWhiteBlack);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::rt_id::IncrRT();
}

SX_LIB_API void SPP_RenderSepia(float coef)
{
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSepia, "g_vCountSepia", &coef);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSepia, "g_vLightColor", &PP_SEPIA_LIGHT_COLOR);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSepia, "g_vDarkColor", &PP_SEPIA_DARK_COLOR);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSepia, "g_vLumWeights", &PP_SEPIA_WEIGHTS_COLOR);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSepia);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::rt_id::IncrRT();
}

SX_LIB_API void SPP_RenderCBG(float3_t* param)
{
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idCBG, "g_vParam", &param);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idCBG);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::rt_id::IncrRT();
}

//##########################################################################

SX_LIB_API void SPP_RenderDOF(float4_t* param, float sky_blur)
{
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(pp_data::rt_id::GetSendRT())->GetLevelDesc(0, &desc);
	float2_t tmpSizeMap = float2_t(desc.Width, desc.Height);

	float tmpcoefblurbloom = 1.f;
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurH, "g_fCoefBlur", &tmpcoefblurbloom);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurH, "g_vSizeMap", &tmpSizeMap);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurH);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	//mem_release(BackBuf);



	SGCore_RTGetTexture(pp_data::rt_id::idIntermediateWinSize)->GetSurfaceLevel(0, &RenderSurf);

	//pp_data::pDXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurW, "g_fCoefBlur", &tmpcoefblurbloom);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurW, "g_vSizeMap", &tmpSizeMap);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurW);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	//mem_release(BackBuf);



	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	//pp_data::pDXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress2(0, 4, D3DTADDRESS_CLAMP);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));
	pp_data::pDXDevice->SetTexture(1, SGCore_RTGetTexture(pp_data::rt_id::idDepth0));
	pp_data::pDXDevice->SetTexture(2, SGCore_RTGetTexture(pp_data::rt_id::idIntermediateWinSize));

	float tmpskyblur = 0.0f;
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDOF, "g_vParam", param);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDOF, "g_fSkyBlur", &sky_blur);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDOF, "g_vNearFar", &pp_data::vNearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDOF);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::rt_id::IncrRT();
}


SX_LIB_API void SPP_UpdateSun(float3* sunpos)
{
	if (sunpos == 0)
	{
		pp_data::existsSun = false;
		return;
	}
	else
		pp_data::existsSun = true;

	float3 sunPos = *sunpos;

	sunPos = SMVector4Transform(sunPos, pp_data::mCamView * pp_data::mCamProj);

	sunPos.x /= sunPos.w;
	sunPos.y /= sunPos.w;
	sunPos.z /= sunPos.w;
	sunPos.w /= sunPos.w;

	sunPos.x *= 0.5;
	sunPos.y *= -0.5;

	sunPos.x += 0.5;
	sunPos.y += 0.5;

	float3 tmpVec(pp_data::vConstCurrCamDir.x, pp_data::vConstCurrCamDir.y, pp_data::vConstCurrCamDir.z);
	tmpVec = SMVector3Normalize(tmpVec);

	float3 dirVec = (*sunpos) - pp_data::vConstCurrCamPos;

	dirVec = SMVector3Normalize(dirVec);
	sunPos.w = abs(acosf(SMVector3Dot(tmpVec, dirVec)) * sign(SMVector3Cross(tmpVec, dirVec).y));
		
	pp_data::vSunPos = sunPos;
}

SX_LIB_API void SPP_ChangeTexSun(const char* str)
{
	pp_data::tex_id::idSun = SGCore_LoadTexAddName(str, LOAD_TEXTURE_TYPE_CONST);
}

SX_LIB_API void SPP_RenderSun(float4_t* sun_color)
{
	if (!pp_data::existsSun)
		return;

	if (pp_data::vSunPos.w > PP_MAX_ANGLE_VISIBLE_SUN)
		return;

	if (pp_data::tex_id::idSun < 0 || !SGCore_LoadTexGetTex(pp_data::tex_id::idSun))
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - sun texture is not init, process can not be started\n", GEN_MSG_LOCATION);
		return;
	}

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);
	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter2(0, 4, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress2(0, 4, D3DTADDRESS_CLAMP);

	D3DSURFACE_DESC desc;
	SGCore_LoadTexGetTex(pp_data::tex_id::idSun)->GetLevelDesc(0, &desc);

	float2_t SizeMapSun = float2_t(desc.Width, desc.Height);

	RECT rectscissor;
	rectscissor.top = pp_data::vSunPos.y * pp_data::vWinSize.y - desc.Height*0.5f;
	rectscissor.left = pp_data::vSunPos.x * pp_data::vWinSize.x - desc.Width*0.5f;
	rectscissor.bottom = rectscissor.top + desc.Height;
	rectscissor.right = rectscissor.left + desc.Width;

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idScreenOut);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//pp_data::pDXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);



	SGCore_RTGetTexture(pp_data::rt_id::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);
	//pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idDepth0));
	pp_data::pDXDevice->SetTexture(2, SGCore_LoadTexGetTex(pp_data::tex_id::idSun));
	pp_data::pDXDevice->SetTexture(3, SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT()));


	pp_data::pDXDevice->SetScissorRect(&rectscissor);
	pp_data::pDXDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pp_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pp_data::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	pp_data::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pp_data::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSunRender, "g_vLightPos", &pp_data::vSunPos);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSunRender, "g_vSizeMap", &(pp_data::vWinSize));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSunRender, "g_vSizeTexSun", &SizeMapSun);
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::SunRender, "PixelSize", &float2_t(1.f / pp_data::vWinSize.x, 1.f / pp_data::vWinSize.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSunRender, "g_vLightColor", sun_color);
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::SunRender, "vNearFar", &pp_data::vNearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idSunRender);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);

	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//pp_data::rt_id::IncrRT();
}

SX_LIB_API void SPP_RenderLensFlare(float3_t* param, float4_t* sun_color, bool use_bloom)
{
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pp_data::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pp_data::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pp_data::pDXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::idBright)->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	if (pp_data::existsSun && pp_data::vSunPos.w < PP_MAX_ANGLE_VISIBLE_SUN && sun_color->w > 0.3)
	{
		if (!use_bloom)
			pp_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		SGCore_SetSamplerFilter(0, D3DTADDRESS_MIRROR);
		pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idDepth0));

		D3DSURFACE_DESC desc;
		SGCore_RTGetTexture(pp_data::rt_id::idBright)->GetLevelDesc(0, &desc);
		float2_t tmpSizeMap = float2_t(desc.Width, desc.Height);

		float LensFlareSunRadius = PP_SUN_RADIUS;
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idLensFlare0, "g_vLightPos", &pp_data::vSunPos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idLensFlare0, "g_vSizeMap", &tmpSizeMap);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idLensFlare0, "g_fRadiusSun", &LensFlareSunRadius);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idLensFlare0, "g_vColor", sun_color);
		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::LensFlare0, "vNearFar", &pp_data::vNearFar);
		SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idLensFlare0);

		SGCore_ScreenQuadDraw();

		SGCore_ShaderUnBind();
	}
	else
	{
		if (!use_bloom)
			pp_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	}

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	/*if (GetKeyState('N'))
	{
		//MessageBox(0,"PP GetKeyState",0,0);
		char tmppath[1024];
		sprintf(tmppath, "%sbp.jpg", "C:\\1\\");
		D3DXSaveTextureToFile(tmppath, D3DXIFF_JPG, SGCore_RTGetTexture(pp_data::rt_id::Bright), NULL);
	}*/


	SGCore_RTGetTexture(pp_data::rt_id::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerFilter(0, D3DTADDRESS_MIRROR);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idBright));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idLensFlare2, "g_vLensFlareParam", param);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idLensFlare2);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

SX_LIB_API void SPP_RenderBloom(float3_t* param)
{
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::idBright)->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idBloomBP, "g_vParam", param);
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idBloomBP);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);

	/*if (GetKeyState('N'))
	{
		D3DXSaveTextureToFile("C:\\1\\sbp.png", D3DXIFF_PNG, SGCore_RTGetTexture(pp_data::rt_id::Bright), NULL);
	}*/


	SGCore_RTGetTexture(pp_data::rt_id::idBright2)->GetSurfaceLevel(0, &RenderSurf);

	//pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idBright));

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(pp_data::rt_id::idBright)->GetLevelDesc(0, &desc);
	float2_t tmpSizeMap = float2_t(desc.Width, desc.Height);

	float tmpcoefblurbloom = 2.f;
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurH, "g_fCoefBlur", &tmpcoefblurbloom);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurH, "g_vSizeMap", &tmpSizeMap);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurH);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);



	SGCore_RTGetTexture(pp_data::rt_id::idBright)->GetSurfaceLevel(0, &RenderSurf);

	//pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idBright2));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurW, "g_fCoefBlur", &tmpcoefblurbloom);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurW, "g_vSizeMap", &tmpSizeMap);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idGaussBlurW);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	//pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);

	/*if (SSInput_GetKeyState(DIK_M))
	{
		char tmppath[1024];
		sprintf(tmppath, "%sbp_blur.png", GData::Pathes::WorkingTex);
		D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_RTGetTexture(pp_data::rt_id::Bright), NULL);
	}*/

	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pp_data::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pp_data::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	pp_data::pDXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	SGCore_RTGetTexture(pp_data::rt_id::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);

	//pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idBright));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idBloomFinal);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//##########################################################################

void SPP_ComEdgeDetected()
{
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	/*SGCore_RTGetTexture(pp_data::rt_id::DepthD2)->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);
	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::Depth1));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::ScreenOut);

	SGCore_ScreenQuadDraw();

	mem_release(RenderSurf);*/

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);
	//SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);

	SGCore_RTGetTexture(pp_data::rt_id::idEdgeDetected)->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress(0, D3DTADDRESS_MIRROR);
	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idDepth1));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDepthEdgeDetect, "g_vPixelSize", &float2_t(1.f / pp_data::vWinSize.x, 1.f / pp_data::vWinSize.y));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDepthEdgeDetect);

	SGCore_ScreenQuadDraw();

	mem_release(RenderSurf);
	SGCore_RTGetTexture(pp_data::rt_id::idEdgeDetected2)->GetSurfaceLevel(0, &RenderSurf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	//pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::Depth1));
	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idEdgeDetected));

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(pp_data::rt_id::idEdgeDetected)->GetLevelDesc(0, &desc);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idFreeBlur3x3, "g_vPixelSize", &float2_t(1.0f / desc.Width, 1.0f / desc.Height));
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::SSAOBlur1, "g_vNearFar", &pp_data::vNearFar);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idFreeBlur3x3);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(RenderSurf);
	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);



	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pp_data::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);

	pp_data::pDXDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f, 0);

	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);

	

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idClip);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::idEdgeDetected2));
	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);

	SGCore_ScreenQuadDraw();

	pp_data::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	if (GetAsyncKeyState('N'))
	{
		D3DXSaveTextureToFile("C:\\1\\EdgeDetected.jpg", D3DXIFF_JPG, SGCore_RTGetTexture(pp_data::rt_id::idEdgeDetected), NULL);
		D3DXSaveTextureToFile("C:\\1\\EdgeDetected2.jpg", D3DXIFF_JPG, SGCore_RTGetTexture(pp_data::rt_id::idEdgeDetected2), NULL);
	}
}

SX_LIB_API void SPP_RenderNFAA(float3_t* param)
{
	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idScreenOut);

	SGCore_ScreenQuadDraw();

	//**

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idNFAA, "g_vParam", param);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idNFAA, "g_vPixelSize", &float2_t(1.f / pp_data::vWinSize.x, 1.f / pp_data::vWinSize.y));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idNFAA);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::rt_id::IncrRT();

	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

SX_LIB_API void SPP_RenderDLAA()
{
	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	pp_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idScreenOut);

	SGCore_ScreenQuadDraw();

	//**

	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILREF, 1);
	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));
	
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDLAA_Long);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDLAA_Long, "g_vPixelSize", &float2_t(1.f / pp_data::vWinSize.x, 1.f / pp_data::vWinSize.y));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(RenderSurf);

	//**

	SGCore_RTGetTexture(pp_data::rt_id::GetSendRT())->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT()));

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDLAA_Small, "g_vPixelSize", &float2_t(1.f / pp_data::vWinSize.x, 1.f / pp_data::vWinSize.y));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idDLAA_Small);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//##########################################################################

SX_LIB_API void SPP_RenderMotionBlur(float coef, DWORD timeDelta)
{
	pp_data::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	SGCore_RTGetTexture(pp_data::rt_id::GetRenderRT())->GetSurfaceLevel(0, &RenderSurf);

	pp_data::pDXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	pp_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	SGCore_SetSamplerAddress2(0, 2, D3DTADDRESS_CLAMP);

	pp_data::pDXDevice->SetTexture(0, SGCore_RTGetTexture(pp_data::rt_id::GetSendRT()));
	pp_data::pDXDevice->SetTexture(1, SGCore_RTGetTexture(pp_data::rt_id::idDepth0));

	float determ = 0;
	float4x4 ViewInv = SMMatrixTranspose(SMMatrixInverse(&determ, pp_data::mCamView));
	//float4x4 ViewPrevInv = SMMatrixTranspose(SMMatrixInverse(&determ, pp_data::mCamViewPrev));

	//float4x4 ViewProjInv = SMMatrixTranspose(SMMatrixInverse(&determ, pp_data::mCamView * pp_data::mCamProj));
	float4x4 ViewProjPrev = (SMMatrixTranspose(pp_data::mCamViewPrev * pp_data::mCamProj));

	//float4x4 tmpProj = SMMatrixTranspose(pp_data::mCamView*pp_data::mCamProj);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idResPos, "g_mViewInv", &ViewInv);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idResPos, "g_vNearFar", &pp_data::vNearFar);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idResPos, "g_vParamProj", &float3_t(pp_data::vWinSize.x, pp_data::vWinSize.y, pp_data::fProjFov));
	
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idMotionBlur, "g_vViewPos", &pp_data::vConstCurrCamPos);
	float tmpcoefblur = float(timeDelta) * 0.001f * ((coef)*10.f);// 0.3f;// *(float(timeDelta) * 0.001f);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idMotionBlur, "g_fCoefBlur", &tmpcoefblur);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idMotionBlur, "g_vNearFar", &pp_data::vNearFar);

	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::MotionBlur, "InvViewProj", &ViewProjInv);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idMotionBlur, "g_mVPprev", &ViewProjPrev);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, pp_data::shaders_id::vs::idResPos);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, pp_data::shaders_id::ps::idMotionBlur);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	pp_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(RenderSurf);
	mem_release(BackBuf);

	pp_data::rt_id::IncrRT();
}
