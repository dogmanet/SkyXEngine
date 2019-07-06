
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "render_func.h"

#include "RenderPipeline.h"

extern CRenderPipeline *g_pPipeline;

//##########################################################################

void rfunc::SetRenderSceneFilter()
{
	static const int *r_texfilter_type = GET_PCVAR_INT("r_texfilter_type");
	static const int *r_texfilter_max_anisotropy = GET_PCVAR_INT("r_texfilter_max_anisotropy");
	static const int *r_texfilter_max_miplevel = GET_PCVAR_INT("r_texfilter_max_miplevel");

	static int r_texfilter_type2 = r_texfilter_type ? *r_texfilter_type : 1;
	static int r_texfilter_max_anisotropy2 = r_texfilter_max_anisotropy ? *r_texfilter_max_anisotropy : 1;
	static int r_texfilter_max_miplevel2 = r_texfilter_max_miplevel ? *r_texfilter_max_miplevel : 1;

	bool bChanged = false;

	if(r_texfilter_type && *r_texfilter_type != r_texfilter_type2)
	{
		r_texfilter_type2 = (*r_texfilter_type);
		bChanged = true;
	}

	if(r_texfilter_max_anisotropy && *r_texfilter_max_anisotropy != r_texfilter_max_anisotropy2)
	{
		r_texfilter_max_anisotropy2 = (*r_texfilter_max_anisotropy);
		bChanged = true;
	}

	if(r_texfilter_max_miplevel && *r_texfilter_max_miplevel != r_texfilter_max_miplevel2)
	{
		r_texfilter_max_miplevel2 = (*r_texfilter_max_miplevel);
		bChanged = true;
	}

	if(bChanged)
	{
		mem_release(gdata::rstates::pSamplerScene);
	}

	if(!gdata::rstates::pSamplerScene)
	{
		GXSamplerDesc samplerDesc;
		samplerDesc.uMaxAnisotropy = r_texfilter_max_anisotropy2;
		switch(r_texfilter_type2)
		{
		case 1:
			samplerDesc.filter = GXFILTER_MIN_MAG_MIP_POINT;
			break;
		case 2:
			samplerDesc.filter = GXFILTER_ANISOTROPIC;
			break;
		default:
			samplerDesc.filter = GXFILTER_MIN_MAG_MIP_LINEAR;
			break;
		}
		samplerDesc.fMinLOD = r_texfilter_max_miplevel2;
		gdata::rstates::pSamplerScene = gdata::pDXDevice->createSamplerState(&samplerDesc);
	}

	gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerScene, 0);
}

void rfunc::SetRenderSceneFilterUn()
{
	gdata::pDXDevice->setSamplerState(NULL, 0);
//	gdata::pDXDevice->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, 0);
}

//##########################################################################

bool rfunc::ComDeviceLost(bool isSetWindowSize)
{
	gdata::InitAllMatrix();
	SGCore_OnResetDevice();

	return(true);
}

void rfunc::ComVisibleReflection()
{
#if 0
	static const int *r_reflection_render = GET_PCVAR_INT("r_reflection_render");

	if (r_reflection_render && (*r_reflection_render) == REFLECTION_RENDER_ONLY_SKY)
		return;

	for (int i = 0; i < SGeom_GetCountModels(); ++i)
	{
		for (int k = 0; k < SGeom_ModelGetCountGroups(i); ++k)
		{
			ID idMat = SGeom_ModelGetGroupMtrlID(i, k);
			MTLTYPE_REFLECT typeReflection = SMtrl_MtlGetTypeReflection(SGeom_ModelGetGroupMtrlID(i, k));
			float3_t center;

			if (typeReflection == MTLTYPE_REFLECT_PLANE)
			{
				if (!SMtrl_RefIsAllowedRender(idMat))
					continue;

				SGeom_ModelGetGroupCenter(i, k, &center);

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
				{
					if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0) < 0)
						SMtrl_RefSetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0, SGeom_VisCaclObjAdd());
				}

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GREEN)
				{
					if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GREEN, 0) < 0)
						SMtrl_RefSetIDArr(idMat, RENDER_IDARRCOM_GREEN, 0, SGreen_AddVisCaclObj());
				}

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_ANIM)
				{
					if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_ANIM, 0) < 0)
						SMtrl_RefSetIDArr(idMat, RENDER_IDARRCOM_ANIM, 0, SXAnim_ModelsAddArrForCom());
				}

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
					SGeom_ComVisible(SMtrl_RefGetfrustum(idMat, 0), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0));

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GREEN)
					SGreen_ComVisible(SMtrl_RefGetfrustum(idMat, 0), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GREEN, 0));

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_ANIM)
					SXAnim_ModelsComVisible(SMtrl_RefGetfrustum(idMat, 0), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_ANIM, 0));
			}
			else if (typeReflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
			{
				SGeom_ModelGetGroupCenter(i, k, &center);

				if (!SMtrl_RefIsAllowedRender(idMat))
					continue;

				for (int j = 0; j<6; j++)
				{
					if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
					{
						if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, k) < 0)
							SMtrl_RefSetIDArr(idMat, RENDER_IDARRCOM_GEOM, k, SGeom_VisCaclObjAdd());
					}

					if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GREEN)
					{
						if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GREEN, k) < 0)
							SMtrl_RefSetIDArr(idMat, RENDER_IDARRCOM_GREEN, k, SGreen_AddVisCaclObj());
					}

					if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_ANIM)
					{
						if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_ANIM, k) < 0)
							SMtrl_RefSetIDArr(idMat, RENDER_IDARRCOM_ANIM, k, SXAnim_ModelsAddArrForCom());
					}

					if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
						SGeom_ComVisible(SMtrl_RefGetfrustum(idMat, j), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, k));

					if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GREEN)
						SGreen_ComVisible(SMtrl_RefGetfrustum(idMat, j), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, k));

					if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_ANIM)
						SXAnim_ModelsComVisible(SMtrl_RefGetfrustum(idMat, j), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_ANIM, k));
				}
			}
		}
	}
#endif
}

void rfunc::SaveScreenShot()
{
	char tmppath[1024];
	static int numscreen = 0;

	if (!FileExistsFile(Core_RStringGet(G_RI_STRING_PATH_SCREENSHOTS)))
	{
		CreateDirectoryA(Core_RStringGet(G_RI_STRING_PATH_SCREENSHOTS), NULL);
	}

	do
	{
		++numscreen;
		sprintf(tmppath, "%sscreen_skyxengine_build_%d.jpg", Core_RStringGet(G_RI_STRING_PATH_SCREENSHOTS), numscreen);
	} while (FileExistsFile(tmppath));

#ifdef _GRAPHIX_API
	LPDIRECT3DSURFACE9 BackBuf;
	gdata::pDXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	D3DXSaveSurfaceToFile(tmppath, D3DXIFF_JPG, BackBuf, NULL, NULL);
	mem_release(BackBuf);
#endif
}

void rfunc::SaveWorkTex()
{
#if 0
	char tmppath[1024];
	sprintf(tmppath, "%scolor.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	gdata::pDXDevice->saveTextureToFile(tmppath, SGCore_GbufferGetRT(DS_RT_COLOR));

	sprintf(tmppath, "%snormal.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	gdata::pDXDevice->saveTextureToFile(tmppath, SGCore_GbufferGetRT(DS_RT_NORMAL));

	sprintf(tmppath, "%sdepth.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	gdata::pDXDevice->saveTextureToFile(tmppath, SGCore_GbufferGetRT(DS_RT_DEPTH));

	sprintf(tmppath, "%sdepth0.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	gdata::pDXDevice->saveTextureToFile(tmppath, SGCore_GbufferGetRT(DS_RT_DEPTH0));

	sprintf(tmppath, "%sparam.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	gdata::pDXDevice->saveTextureToFile(tmppath, SGCore_GbufferGetRT(DS_RT_PARAM));

	sprintf(tmppath, "%sambient_diff.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	gdata::pDXDevice->saveTextureToFile(tmppath, SGCore_GbufferGetRT(DS_RT_AMBIENTDIFF));

	sprintf(tmppath, "%sspecular.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	gdata::pDXDevice->saveTextureToFile(tmppath, SGCore_GbufferGetRT(DS_RT_SPECULAR));


	sprintf(tmppath, "%slight_com_1.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	gdata::pDXDevice->saveTextureToFile(tmppath, SGCore_GbufferGetRT(DS_RT_SCENELIGHT));

	sprintf(tmppath, "%slight_com_2.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	gdata::pDXDevice->saveTextureToFile(tmppath, SGCore_GbufferGetRT(DS_RT_SCENELIGHT2));
#endif
}

void rfunc::InitModeWindow()
{
	static int * r_win_width = (int*)GET_PCVAR_INT("r_win_width");
	static int * r_win_height = (int*)GET_PCVAR_INT("r_win_height");

	static const bool *r_win_windowed = GET_PCVAR_BOOL("r_win_windowed");

	static DWORD dwStyle = GetWindowLong(gdata::hHandle3D, GWL_STYLE);

	if (r_win_windowed == NULL)
		return;

	if (!(*r_win_windowed))
	{
		SetWindowLong(gdata::hHandle3D, GWL_STYLE, dwStyle | WS_POPUP);
		ShowWindow(gdata::hHandle3D, SW_MAXIMIZE);
	}
	else
	{
		SetWindowLong(gdata::hHandle3D, GWL_STYLE, dwStyle);

		/*RECT rc;
		GetWindowRect(gdata::hHandle3D, &rc);*/

		RECT rc2 = { 0, 0, *r_win_width, *r_win_height };
		AdjustWindowRect(&rc2, dwStyle, false);

		int iWidth = rc2.right - rc2.left;
		int iHeight = rc2.bottom - rc2.top;
		int iPosX = (GetSystemMetrics(SM_CXSCREEN) - iWidth) / 2;
		int iPosY = (GetSystemMetrics(SM_CYSCREEN) - iHeight) / 2;
		SetWindowPos(gdata::hHandle3D, HWND_NOTOPMOST, iPosX, iPosY, iWidth, iHeight, SWP_SHOWWINDOW);

		SetForegroundWindow(gdata::hHandle3D);
	}
}

void rfunc::ChangeModeWindow()
{
	static bool *r_win_windowed = (bool*)GET_PCVAR_BOOL("r_win_windowed");

	if (r_win_windowed == NULL)
		return;

	*r_win_windowed = !(*r_win_windowed);

	rfunc::InitModeWindow();

	static int *r_resize = (int*)GET_PCVAR_INT("r_resize");
	*r_resize = RENDER_RESIZE_CHANGE;
}

void rfunc::FullScreenChangeSizeAbs()
{
	static bool *r_win_windowed = (bool*)GET_PCVAR_BOOL("r_win_windowed");

	if (r_win_windowed == NULL || (*r_win_windowed))
		return;

	int iCountModes = 0;
	static const DEVMODE *pModes = SGCore_GetModes(&iCountModes);

	static int iFullScreenWidth = 800;
	static int iFullScreenHeight = 600;

	if (pModes)
	{
		iFullScreenWidth = pModes[iCountModes - 1].dmPelsWidth;
		iFullScreenHeight = pModes[iCountModes - 1].dmPelsHeight;
		pModes = 0;
	}

	static int * r_win_width = (int*)GET_PCVAR_INT("r_win_width");
	static int * r_win_height = (int*)GET_PCVAR_INT("r_win_height");

	if (!r_win_width || !r_win_height)
		return;

	static int r_win_width_old = *r_win_width;
	static int r_win_height_old = *r_win_height;

	if (*r_win_width == iFullScreenWidth && *r_win_height == iFullScreenHeight)
	{
		*r_win_width = r_win_width_old;
		*r_win_height = r_win_height_old;
	}
	else
	{
		r_win_width_old = *r_win_width;
		r_win_height_old = *r_win_height;

		*r_win_width = iFullScreenWidth;
		*r_win_height = iFullScreenHeight;

		//LibReport(REPORT_MSG_LEVEL_WARNING, "iFullScreenWidth %d, iFullScreenHeight %d \n", iFullScreenWidth, iFullScreenHeight);
	}

	static int *r_resize = (int*)GET_PCVAR_INT("r_resize");
	*r_resize = RENDER_RESIZE_CHANGE;
}

//##########################################################################

void rfunc::UpdateView()
{
	gdata::InitAllMatrix();

	gdata::pCamera->getViewMatrix(&gdata::mCamView);

	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &gdata::mCamView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &gdata::mLightProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(gdata::mCamView * gdata::mLightProj));
	Core_RMatrixSet(G_RI_MATRIX_TRANSP_VIEWPROJ, &SMMatrixTranspose(gdata::mCamView * gdata::mLightProj));

	gdata::pCamera->getPosition(&gdata::vConstCurrCamPos);
	gdata::pCamera->getLook(&gdata::vConstCurrCamDir);

	Core_RFloat3Set(G_RI_FLOAT3_OBSERVER_POSITION, &gdata::vConstCurrCamPos);
	Core_RFloat3Set(G_RI_FLOAT3_OBSERVER_DIRECTION, &gdata::vConstCurrCamDir);

	Core_RMatrixSet(G_RI_MATRIX_OBSERVER_VIEW, &gdata::mCamView);
	Core_RMatrixSet(G_RI_MATRIX_OBSERVER_PROJ, &gdata::mCamProj);
	Core_RMatrixSet(G_RI_MATRIX_LIGHT_PROJ, &gdata::mLightProj);

	/*Core_RFloatSet(G_RI_FLOAT_OBSERVER_NEAR, gdata::vNearFar.x);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FAR, gdata::vNearFar.y);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FOV, gdata::fProjFov);*/

	gdata::pCamera->updateFrustum(&gdata::mCamProj);

#if 0
	if (gdata::idDefaultGeomArr < 0 || !SGeom_VisCaclObjExists(gdata::idDefaultGeomArr))
		gdata::idDefaultGeomArr = SGeom_VisCaclObjAdd();

	if (gdata::idDefaultGreenArr < 0 || !SGreen_ExistsVisCaclObj(gdata::idDefaultGreenArr))
		gdata::idDefaultGreenArr = SGreen_AddVisCaclObj();

	if (gdata::idDefaultAnimArr < 0)
		gdata::idDefaultAnimArr = SXAnim_ModelsAddArrForCom();
#endif
}

//##########################################################################

void rfunc::RenderSky(DWORD timeDelta)
{
#if 0
	IGXSurface *ColorSurf, *BackBuf;
	ColorSurf = SGCore_GbufferGetRT(DS_RT_SCENELIGHT)->getMipmap();
	//ColorSurf = SGCore_GbufferGetRT(DS_RT_COLOR)->getMipmap();
	BackBuf = gdata::pDXDevice->getColorTarget();
	gdata::pDXDevice->setColorTarget(ColorSurf);

	gdata::pDXDevice->clear(GX_CLEAR_COLOR, RENDER_DEFAUL_BACKGROUND_COLOR);

//	SetSamplerFilter(0, 2, D3DTEXF_ANISOTROPIC);
	gdata::pDXDevice->setRasterizerState(NULL);
	for(UINT i = 0; i <= 2; ++i)
	{
		gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerAnisotopicClamp, i);
	}

	if (SGCore_SkyBoxIsCr() && SGCore_SkyBoxGetUse() && SGCore_SkyBoxIsLoadTex())
	{
	//	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//	SetSamplerAddress(0, 2, D3DTADDRESS_CLAMP);
		SGCore_SkyBoxRender(timeDelta, &float3(gdata::vConstCurrCamPos.x, gdata::vConstCurrCamPos.y/* + (SXGC_SKYBOX_SIZE * 0.5 - 10)*/, gdata::vConstCurrCamPos.z));
	}

//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

#ifdef _GRAPHIX_API
	gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
#endif

//	gdata::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	gdata::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	gdata::pDXDevice->setBlendState(gdata::rstates::pBlendAlphaSky);

//	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (SGCore_SkyCloudsIsCr() && SGCore_SkyCloudsGetUse() && SGCore_SkyCloudsIsLoadTex())
	{
		for(UINT i = 0; i <= 2; ++i)
		{
			gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerAnisotopicWrap, i);
		}
	//	SetSamplerAddress(0, 2, D3DTADDRESS_MIRROR);
		gdata::pDXDevice->setRasterizerState(gdata::rstates::pRasterizerCullNone);
		SGCore_SkyCloudsRender(timeDelta, &float3(gdata::vConstCurrCamPos.x, gdata::vConstCurrCamPos.y + 150, gdata::vConstCurrCamPos.z), false);
		gdata::pDXDevice->setRasterizerState(NULL);
	}

	gdata::pDXDevice->setTexture(SGCore_GbufferGetRT(DS_RT_SCENELIGHT2));

	SGCore_ShaderBind(gdata::shaders_id::kit::idScreenOut);

//	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(ColorSurf);

	gdata::pDXDevice->setColorTarget(BackBuf);
	mem_release(BackBuf);

	gdata::pDXDevice->setBlendState(NULL);
//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
#endif
}

void rfunc::RenderParticles(DWORD timeDelta)
{
#if 0
	gdata::pDXDevice->setRasterizerState(gdata::rstates::pRasterizerCullNone);
//	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
//#ifdef _GRAPHIX_API
//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHAREF, RENDER_PARTICLES_ALPHATEST_VALUE);
//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
//#endif

//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	gdata::pDXDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateParticles);

	for(UINT i = 0; i <= 3; ++i)
	{
		gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerLinearWrap, i);
	}
//	SetSamplerFilter(0, 3, D3DTEXF_LINEAR);
//	SetSamplerAddress(0, 3, D3DTADDRESS_WRAP);

	SPE_EffectRenderAll(timeDelta);

	gdata::pDXDevice->setRasterizerState(NULL);
//	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	gdata::pDXDevice->setDepthStencilState(NULL);
#endif
}

void rfunc::RenderMainPostProcess(DWORD timeDelta)
{
#if 0
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");
	static float2_t vWinSize;
	vWinSize = float2(*r_win_width, *r_win_height);

	SPP_Update();
	SPP_RTNull();

	//**********************************************************************

	static const int * pp_ssao = GET_PCVAR_INT("pp_ssao");
	if (pp_ssao && (*pp_ssao) > 0)
		SPP_RenderSSAO(&float4_t(0.3f, 0.1f, 0.8f, 0.3f / gdata::vNearFar.y), (*pp_ssao));

	//**********************************************************************

	//создаем статический вектор цвета тумана, затем получаем квар который int типа который будет содеражть указатель на этот вектор, и в него записываем указатель на вектор цвета тумана
	//static float3_t fog_color(0.5, 0.5, 0.5);
	static UINT_PTR *pp_fog_color = GET_PCVAR_POINTER("pp_fog_color");
	static float3_t *fog_color;
	fog_color = (float3_t*)(*pp_fog_color);

	//**********************************************************************

	static const float * pp_fog_density = GET_PCVAR_FLOAT("pp_fog_density");
	if (pp_fog_density && *pp_fog_density > 0.f)
		SPP_RenderFogLinear(fog_color, *pp_fog_density);

	//**********************************************************************

	static const bool * pp_bloom = GET_PCVAR_BOOL("pp_bloom");
	if (pp_bloom && (*pp_bloom))
		SPP_RenderBloom(&float3_t(1, 0.85, 1.4));

	//**********************************************************************

	float3 vGLpos;
	float3 vGLcolor;

//@TODO: Reimplement this!
#if 0
	ID idGlobalLight = SLight_GetGlobal();
	if (idGlobalLight > -1)
	{
		SLight_GetColor(idGlobalLight, &vGLcolor);
		SLight_GetPos(idGlobalLight, &vGLpos, true);

		SPP_UpdateSun((SLight_GetEnable(idGlobalLight) ? &vGLpos : 0));

		if (!SLight_GetCastGlobalShadow())
			SPP_RenderSun(&float4_t(vGLcolor.x, vGLcolor.y, vGLcolor.z, SLight_GetPower(idGlobalLight)));
	}
	else
		SPP_UpdateSun(0);


	//**********************************************************************

	static const bool * pp_lensflare = GET_PCVAR_BOOL("pp_lensflare");
	static const bool * pp_lensflare_usebloom = GET_PCVAR_BOOL("pp_lensflare_usebloom");

	//использовать ли блум, если сам блум вообще используется
	bool useBloom = ((pp_lensflare_usebloom ? (*pp_lensflare_usebloom) : false) && (pp_bloom && (*pp_bloom)));
	if (pp_lensflare && (*pp_lensflare) && idGlobalLight >= 0)
		SPP_RenderLensFlare(&float3_t(0.25f, 0.3f, 0.2f), &float4_t(vGLcolor.x, vGLcolor.y, vGLcolor.z, (SLight_GetCastGlobalShadow() ? 0 : SLight_GetPower(idGlobalLight))), useBloom);
#endif
	//**********************************************************************

	SPP_RenderDOF(&float4_t(0, 100, 0, 20), 0);

	//**********************************************************************

	static const bool * pp_nfaa = GET_PCVAR_BOOL("pp_nfaa");
	if (pp_nfaa && (*pp_nfaa))
		SPP_RenderNFAA(&float3_t(1, 1, 0));

	//**********************************************************************

	static const bool * pp_dlaa = GET_PCVAR_BOOL("pp_dlaa");
	if (pp_dlaa && (*pp_dlaa))
		SPP_RenderDLAA();

	//**********************************************************************

	static const bool * pp_motionblur = GET_PCVAR_BOOL("pp_motionblur");
	static const float * pp_motionblur_coef = GET_PCVAR_FLOAT("pp_motionblur_coef");
	if (pp_motionblur && (*pp_motionblur))
		SPP_RenderMotionBlur((pp_motionblur_coef ? (*pp_motionblur_coef) : 0.1), timeDelta);
#endif
}

void rfunc::RenderFinalPostProcess(DWORD timeDelta)
{
#if 0
	static const float * pp_whiteblack_coef = GET_PCVAR_FLOAT("pp_whiteblack_coef");

	if (pp_whiteblack_coef && (*pp_whiteblack_coef) > 0.f)
		SPP_RenderWhiteBlack(*pp_whiteblack_coef);

	//**********************************************************************

	static const float * pp_sepia_coef = GET_PCVAR_FLOAT("pp_sepia_coef");

	if (pp_sepia_coef && (*pp_sepia_coef) > 0.f)
		SPP_RenderSepia((*pp_sepia_coef));

	//**********************************************************************

	static const float * pp_contrast = GET_PCVAR_FLOAT("pp_contrast");
	static const float * pp_gamma = GET_PCVAR_FLOAT("pp_gamma");
	static const float * pp_bright = GET_PCVAR_FLOAT("pp_bright");

	if (pp_contrast && pp_gamma && pp_bright)
		SPP_RenderCBG(&float3_t((*pp_contrast), (*pp_gamma), (*pp_bright)));
#endif
}

#if 0
void rfunc::UpdateReflectionScene(DWORD timeDelta)
{
	static const int *r_reflection_render = GET_PCVAR_INT("r_reflection_render");

	float3 vObserverPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

	gdata::pDXDevice->setRasterizerState(NULL);

	for (int i = 0; i < SGeom_GetCountModels(); ++i)
	{
		for (int k = 0; k < SGeom_ModelGetCountGroups(i); ++k)
		{
			ID idMat = SGeom_ModelGetGroupMtrlID(i, k);
			MTLTYPE_REFLECT typeReflection = SMtrl_MtlGetTypeReflection(SGeom_ModelGetGroupMtrlID(i, k));
			SMPLANE oPlane;
			float3_t vCenter;

			if (typeReflection == MTLTYPE_REFLECT_PLANE)
			{
				if (!SMtrl_RefIsAllowedRender(idMat))
				{
					SMtrl_RefUpdateCountUpdate(idMat, &((float3_t)gdata::vConstCurrCamPos));
					continue;
				}

				SGeom_ModelGetGroupPlane(i, k, &oPlane);
				SGeom_ModelGetGroupCenter(i, k, &vCenter);
				SMtrl_RefPreRenderPlane(idMat, &oPlane);
			//	SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
			//	SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);
				for(UINT ii = 0; ii < 16; ++ii)
				{
					gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerLinearWrap, i);
				}

				Core_RBoolSet(G_RI_BOOL_CLIPPLANE0, true);

				Core_RFloat3Set(G_RI_FLOAT3_CLIPPLANE0_NORMAL, &float3(oPlane));
				Core_RFloat3Set(G_RI_FLOAT3_CLIPPLANE0_POINT, &float3(vCenter));

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
				{
					if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0) >= 0)
						SGeom_Render(timeDelta, GEOM_RENDER_TYPE_OPAQUE, SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0), i, k);
				}

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GREEN)
				{
					if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GREEN, 0) >= 0)
						SGreen_Render(timeDelta, &float3(vCenter), GREEN_TYPE_ALL, SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GREEN, 0));
				}

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_ANIM)
				{
					SXAnim_Render();
				}

				SGCore_ShaderUnBind();

				if (SGCore_SkyBoxIsCr() && SGCore_SkyBoxGetUse())
				{
				//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

#ifdef _GRAPHIX_API
					gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
#endif
					gdata::pDXDevice->setBlendState(gdata::rstates::pBlendAlpha);
				//	gdata::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
				//	gdata::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

				//	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					for(UINT ii = 0; ii <= 2; ++ii)
					{
						gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerLinearClamp, i);
					}
				//	SetSamplerAddress(0, 2, D3DTADDRESS_CLAMP);

					gdata::pDXDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateNoZ);
				//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
				//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
					Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &gdata::mRefPlaneSkyProj);
					SGCore_SkyBoxRender(timeDelta, &float3(vCenter));
					gdata::pDXDevice->setDepthStencilState(NULL);
				//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
				//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
				//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
					gdata::pDXDevice->setBlendState(NULL);
				}

				SMtrl_RefPostRenderPlane(idMat);
			}
			else if (typeReflection == MTLTYPE_REFLECT_CUBE_STATIC || typeReflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
			{
				SGeom_ModelGetGroupCenter(i, k, &vCenter);
				float3_t min, max;
				SGeom_ModelGetGroupMinMax(i, k, &min, &max);
				SMtrl_RefSetMinMax(idMat, &min, &max);

				if (!SMtrl_RefIsAllowedRender(idMat))
				{
					SMtrl_RefUpdateCountUpdate(idMat, &((float3_t)gdata::vConstCurrCamPos));
					continue;
				}

				SMtrl_RefCubeBeginRender(idMat, &vCenter);

				for (int j = 0; j < 6; j++)
				{
					Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

					SMtrl_RefCubePreRender(idMat, j, &(SMMatrixIdentity()));
				//	SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
				//	SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);
					for(UINT ii = 0; ii < 16; ++ii)
					{
						gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerLinearWrap, i);
					}

					if (r_reflection_render && (*r_reflection_render) != 0)
					{
						//если статические кубические отражения
						if (SMtrl_MtlGetTypeReflection(idMat) == MTLTYPE_REFLECT_CUBE_STATIC)
						{
							//тогда считаем в массив камеры
							if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
							{
								SGeom_ComVisible(SMtrl_RefGetfrustum(idMat, j), &float3(vCenter), gdata::idDefaultGeomArr);
								SGeom_Render(timeDelta, GEOM_RENDER_TYPE_OPAQUE, gdata::idDefaultGeomArr, i, k);
							}

							if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GREEN)
							{
								SGreen_ComVisible(SMtrl_RefGetfrustum(idMat, j), &float3(vCenter), gdata::idDefaultGreenArr);
								SGreen_Render(timeDelta, &float3(vCenter), GREEN_TYPE_ALL, gdata::idDefaultGreenArr);
							}
						}
						else
						{
							if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
							{
								if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, k) >= 0)
									SGeom_Render(timeDelta, GEOM_RENDER_TYPE_OPAQUE, SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, k), i, k);
							}

							if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GREEN)
							{
								if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GREEN, k) >= 0)
									SGreen_Render(timeDelta, &gdata::vConstCurrCamPos, GREEN_TYPE_ALL, SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GREEN, k));
							}

							if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_ANIM)
							{
								SXAnim_Render();
							}
						}
					}

					if (SGCore_SkyBoxIsCr() && SGCore_SkyBoxGetUse())
					{
					//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

#ifdef _GRAPHIX_API
						gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
						gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
#endif
						gdata::pDXDevice->setBlendState(gdata::rstates::pBlendAlpha);
					//	gdata::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
					//	gdata::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

						gdata::pDXDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateNoZ);
					//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
					//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

					//	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					//	SetSamplerAddress(0, 2, D3DTADDRESS_CLAMP);
						for(UINT ii = 0; ii <= 2; ++ii)
						{
							gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerLinearClamp, i);
						}

						Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &gdata::mRefCubeSkyProj);

						SGCore_SkyBoxRender(timeDelta, &float3(vCenter));
						gdata::pDXDevice->setBlendState(NULL);
					//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

						SMtrl_RefCubePostRender(idMat, j);

						gdata::pDXDevice->setDepthStencilState(NULL);
					//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
					//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
					}
				}
				SMtrl_RefCubeEndRender(idMat, &((float3_t)gdata::vConstCurrCamPos));
			}
		}
	}
}

void rfunc::UpdateReflectionSimModel(DWORD timeDelta)
{
	ID idMat = gdata::Editors::pSimModel->getIdMtl();
	MTLTYPE_REFLECT typeReflection = SMtrl_MtlGetTypeReflection(idMat);
	SMPLANE oPlane;
	float3_t vCenter;
	gdata::Editors::pSimModel->getCenter(&vCenter);
	SGCore_ShaderUnBind();

	gdata::pDXDevice->setBlendState(NULL);
	gdata::pDXDevice->setDepthStencilState(gdata::rstates::pDepthStencilStateNoZ);

	if (typeReflection == MTLTYPE_REFLECT_PLANE)
	{
		gdata::Editors::pSimModel->getPlane(&oPlane);
		SMtrl_RefPreRenderPlane(idMat, &oPlane);
		for(UINT i = 0; i < 16; ++i)
		{
			gdata::pDXDevice->setSamplerState(gdata::rstates::pSamplerLinearWrap, i);
		}
	//	SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
	//	SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

	//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &gdata::mRefPlaneSkyProj);
		SGCore_SkyBoxRender(timeDelta, &float3(vCenter));
	//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

		SMtrl_RefPostRenderPlane(idMat);
	}
	else if (typeReflection == MTLTYPE_REFLECT_CUBE_STATIC || typeReflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
	{
		SMtrl_RefCubeBeginRender(idMat, &vCenter);

		for (int j = 0; j<6; ++j)
		{
		//	Core_RMatrixGet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

			SMtrl_RefCubePreRender(idMat, j, &(SMMatrixIdentity()));

		//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
			Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &gdata::mRefCubeSkyProj);
			SGCore_SkyBoxRender(timeDelta, &float3(vCenter));

		//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
			SMtrl_RefCubePostRender(idMat, j);
		//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		}
		SMtrl_RefCubeEndRender(idMat, &((float3_t)gdata::vConstCurrCamPos));
	}

	gdata::pDXDevice->setDepthStencilState(NULL);
}


void rfunc::UpdateReflection(DWORD timeDelta, bool isRenderSimulation)
{
	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_FREE);
	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &gdata::mCamView);

//	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
//	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	gdata::pDXDevice->setDepthStencilState(NULL);

	if (!isRenderSimulation)
		rfunc::UpdateReflectionScene(timeDelta);
	else
		rfunc::UpdateReflectionSimModel(timeDelta);

}
#endif
