
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "render_func.h"

inline void rfunc::SetSamplerFilter(DWORD dwId, DWORD dwValue)
{
	gdata::pDXDevice->SetSamplerState(dwId, D3DSAMP_MAGFILTER, dwValue);
	gdata::pDXDevice->SetSamplerState(dwId, D3DSAMP_MINFILTER, dwValue);
	gdata::pDXDevice->SetSamplerState(dwId, D3DSAMP_MIPFILTER, dwValue);
}

inline void rfunc::SetSamplerAddress(DWORD dwId, DWORD dwValue)
{
	gdata::pDXDevice->SetSamplerState(dwId, D3DSAMP_ADDRESSU, dwValue);
	gdata::pDXDevice->SetSamplerState(dwId, D3DSAMP_ADDRESSV, dwValue);
	gdata::pDXDevice->SetSamplerState(dwId, D3DSAMP_ADDRESSW, dwValue);
}

inline void rfunc::SetSamplerFilter(DWORD dwStartId, DWORD dwFinishEnd, DWORD dwValue)
{
	if (dwStartId >= 0 && dwFinishEnd <= 16)
	{
		for (DWORD i = dwStartId; i <= dwFinishEnd; ++i)
			rfunc::SetSamplerFilter(i, dwValue);
	}
}

inline void rfunc::SetSamplerAddress(DWORD dwStartId, DWORD dwFinishEnd, DWORD dwValue)
{
	if (dwStartId >= 0 && dwFinishEnd <= 16)
	{
		for (DWORD i = dwStartId; i <= dwFinishEnd; ++i)
			rfunc::SetSamplerAddress(i, dwValue);
	}
}

void rfunc::SetRenderSceneFilter()
{
	static const int *r_texfilter_type = GET_PCVAR_INT("r_texfilter_type");
	static const int *r_texfilter_max_anisotropy = GET_PCVAR_INT("r_texfilter_max_anisotropy");
	static const int *r_texfilter_max_miplevel = GET_PCVAR_INT("r_texfilter_max_miplevel");

	static int r_texfilter_type2 = 1;
	static int r_texfilter_max_anisotropy2 = 0;
	static int r_texfilter_max_miplevel2 = 0;

	if (r_texfilter_type)
		r_texfilter_type2 = (*r_texfilter_type);
	else
		r_texfilter_type2 = 1;

	if (r_texfilter_max_anisotropy)
		r_texfilter_max_anisotropy2 = (*r_texfilter_max_anisotropy);
	else
		r_texfilter_max_anisotropy2 = 1;

	if (r_texfilter_max_miplevel)
		r_texfilter_max_miplevel2 = (*r_texfilter_max_miplevel);
	else
		r_texfilter_max_miplevel2 = 1;

	if (r_texfilter_type2 == 0)
		SetSamplerFilter(0, 16, D3DTEXF_POINT);
	else if (r_texfilter_type2 == 2)
	{
		for (int i = 0; i<16; ++i)
			gdata::pDXDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, r_texfilter_max_anisotropy2);
		SetSamplerFilter(0, 16, D3DTEXF_ANISOTROPIC);
	}
	else
		SetSamplerFilter(0, 16, D3DTEXF_LINEAR);

	gdata::pDXDevice->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, r_texfilter_max_miplevel2);

	SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);
}

void rfunc::SetRenderSceneFilterUn()
{
	gdata::pDXDevice->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, 0);
}

//##########################################################################

void rfunc::ComDeviceLost(bool isSetWindowSize)
{
	static int *r_resize = (int*)GET_PCVAR_INT("r_resize");

	static int *r_win_width = (int*)GET_PCVAR_INT("r_win_width");
	static int *r_win_height = (int*)GET_PCVAR_INT("r_win_height");
	static const bool *r_win_windowed = GET_PCVAR_BOOL("r_win_windowed");

	if (isSetWindowSize && *r_resize != RENDER_RESIZE_CHANGE)
	{
		//получаем текущий размер окна в которое рисовали
		RECT rect_scene;
		GetClientRect(gdata::hHandle3D, &rect_scene);

		*r_win_width = rect_scene.right;
		*r_win_height = rect_scene.bottom;
	}

	//сбрасываем все что необходимо для восстановления устройства
	SGCore_OnLostDevice();
	SGeom_OnLostDevice();
	SGreen_OnLostDevice();
	SLight_OnLostDevice();
	SMtrl_OnLostDevice();
	SPE_OnLostDevice();
	SPP_OnLostDevice();
	SXGame_OnLostDevice();

	rfunc::InitModeWindow();
	bool bf = SGCore_OnDeviceReset(*r_win_width, *r_win_height, *r_win_windowed);
	LibReport(REPORT_MSG_LEVEL_WARNING, "r_win_width %d, r_win_height %d, r_win_windowed %d \n", *r_win_width, *r_win_height, *r_win_windowed);

	if (bf)
	{
		//если все-таки функция зашла сюда значит что-то было неосвобождено
		LibReport(REPORT_MSG_LEVEL_ERROR, "reset device is failed ... \n");
	}
	else
	{
		gdata::InitAllMatrix();
		*r_resize = RENDER_RESIZE_NONE;
		SGCore_OnResetDevice();
		SLight_OnResetDevice();
		SMtrl_OnResetDevice();
		SGreen_OnResetDevice();
		SGeom_OnResetDevice();
		SPE_OnResetDevice();
		SXGame_OnResetDevice();
		SPP_OnDeviceReset();

		gdata::pDXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
}

void rfunc::ComVisibleForLight()
{
	for (int i = 0; i<SLight_GetCount(); ++i)
	{
		if (!SLight_GetExists(i))
			continue;

		if (SLight_GetShadowed(i) && (SLight_ComVisibleForFrustum(i, gdata::pCamera->getFrustum()) && SLight_GetEnable(i)))
		{
			if (SLight_GetType(i) == LTYPE_LIGHT_GLOBAL)
			{
				for (int k = 0; k<4; ++k)
				{
					if (SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, k) <= -1)
						SLight_SetIDArr(i, RENDER_IDARRCOM_GEOM, k, SGeom_ModelsAddArrForCom());

					if (SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, k) <= -1)
						SLight_SetIDArr(i, RENDER_IDARRCOM_GREEN, k, SGreen_AddVisCaclObj());

					if (SLight_GetIDArr(i, RENDER_IDARRCOM_ANIM, k) <= -1)
						SLight_SetIDArr(i, RENDER_IDARRCOM_ANIM, k, SXAnim_ModelsAddArrForCom());

					if (SLight_CountUpdateUpdate(i, &gdata::vConstCurrCamPos, k))
					{
						SLight_UpdateGFrustums(i, k, &gdata::vConstCurrCamPos, &gdata::vConstCurrCamDir);
						SGeom_ModelsComVisible(SLight_GetFrustum(i, k), &gdata::vConstCurrCamPos, SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, k));
						SGreen_ComVisible(SLight_GetFrustum(i, k), &gdata::vConstCurrCamPos, SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, k));
						SXAnim_ModelsComVisible(SLight_GetFrustum(i, k), &gdata::vConstCurrCamPos, SLight_GetIDArr(i, RENDER_IDARRCOM_ANIM, k));
					}
				}
			}
			else if (SLight_GetType(i) == LTYPE_LIGHT_DIR)
			{
				if (SLight_GetTypeShadowed(i) == LTYPE_SHADOW_DYNAMIC)
				{
					if (SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, 0) <= -1)
						SLight_SetIDArr(i, RENDER_IDARRCOM_GEOM, 0, SGeom_ModelsAddArrForCom());

					if (SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, 0) <= -1)
						SLight_SetIDArr(i, RENDER_IDARRCOM_GREEN, 0, SGreen_AddVisCaclObj());

					if (SLight_GetIDArr(i, RENDER_IDARRCOM_ANIM, 0) <= -1)
						SLight_SetIDArr(i, RENDER_IDARRCOM_ANIM, 0, SXAnim_ModelsAddArrForCom());

					SGeom_ModelsComVisible(SLight_GetFrustum(i, 0), &gdata::vConstCurrCamPos, SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, 0));
					SGreen_ComVisible(SLight_GetFrustum(i, 0), &gdata::vConstCurrCamPos, SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, 0));
					SXAnim_ModelsComVisible(SLight_GetFrustum(i, 0), &gdata::vConstCurrCamPos, SLight_GetIDArr(i, RENDER_IDARRCOM_ANIM, 0));
				}
			}
			else if (SLight_GetType(i) == LTYPE_LIGHT_POINT)
			{
				if (SLight_GetTypeShadowed(i) == LTYPE_SHADOW_DYNAMIC)
				{
					for (int k = 0; k < 6; k++)
					{
						if (SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, k) <= -1)
							SLight_SetIDArr(i, RENDER_IDARRCOM_GEOM, k, SGeom_ModelsAddArrForCom());

						if (SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, k) <= -1)
							SLight_SetIDArr(i, RENDER_IDARRCOM_GREEN, k, SGreen_AddVisCaclObj());

						if (SLight_GetIDArr(i, RENDER_IDARRCOM_ANIM, k) <= -1)
							SLight_SetIDArr(i, RENDER_IDARRCOM_ANIM, k, SXAnim_ModelsAddArrForCom());

						if (SLight_GetCubeEdgeEnable(i, k))
						{
							SGeom_ModelsComVisible(SLight_GetFrustum(i, k), &gdata::vConstCurrCamPos, SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, k));
							SGreen_ComVisible(SLight_GetFrustum(i, k), &gdata::vConstCurrCamPos, SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, k));
							SXAnim_ModelsComVisible(SLight_GetFrustum(i, k), &gdata::vConstCurrCamPos, SLight_GetIDArr(i, RENDER_IDARRCOM_ANIM, k));
						}
					}
				}
			}
		}
	}

	ID idTempArr = -1;
	while (SLight_DelGetCount() > 0)
	{
		int iCurrKey = 0;
		if (SLight_DelGetType(iCurrKey) == LTYPE_LIGHT_GLOBAL)
		{
			for (int k = 0; k<4; ++k)
			{
				if ((idTempArr = SLight_DelGetIDArr(iCurrKey, RENDER_IDARRCOM_GEOM, k)) >= 0 && SGeom_ModelsExistsArrForCom(idTempArr))
					SGeom_ModelsDelArrForCom(idTempArr);

				if ((idTempArr = SLight_DelGetIDArr(iCurrKey, RENDER_IDARRCOM_GREEN, k)) >= 0 && SGreen_ExistsVisCaclObj(idTempArr))
					SGreen_DelVisCaclObj(idTempArr);

				if ((idTempArr = SLight_DelGetIDArr(iCurrKey, RENDER_IDARRCOM_ANIM, k)) >= 0)
					SXAnim_ModelsDelArrForCom(idTempArr);
			}
		}
		else if (SLight_DelGetType(iCurrKey) == LTYPE_LIGHT_DIR)
		{
			if ((idTempArr = SLight_DelGetIDArr(iCurrKey, RENDER_IDARRCOM_GEOM, 0)) >= 0 && SGeom_ModelsExistsArrForCom(idTempArr))
				SGeom_ModelsDelArrForCom(idTempArr);

			if ((idTempArr = SLight_DelGetIDArr(iCurrKey, RENDER_IDARRCOM_GREEN, 0)) >= 0 && SGreen_ExistsVisCaclObj(idTempArr))
				SGreen_DelVisCaclObj(idTempArr);

			if ((idTempArr = SLight_DelGetIDArr(iCurrKey, RENDER_IDARRCOM_ANIM, 0)) >= 0)
				SXAnim_ModelsDelArrForCom(idTempArr);

		}
		else if (SLight_DelGetType(iCurrKey) == LTYPE_LIGHT_POINT)
		{
			for (int k = 0; k<6; k++)
			{
				if ((idTempArr = SLight_DelGetIDArr(iCurrKey, RENDER_IDARRCOM_GEOM, k)) >= 0 && SGeom_ModelsExistsArrForCom(idTempArr))
					SGeom_ModelsDelArrForCom(idTempArr);

				if ((idTempArr = SLight_DelGetIDArr(iCurrKey, RENDER_IDARRCOM_GREEN, k)) >= 0 && SGreen_ExistsVisCaclObj(idTempArr))
					SGreen_DelVisCaclObj(idTempArr);

				if ((idTempArr = SLight_DelGetIDArr(iCurrKey, RENDER_IDARRCOM_ANIM, k)) >= 0)
					SXAnim_ModelsDelArrForCom(idTempArr);
			}
		}

		SLight_DelDel(iCurrKey);
	}
}

void rfunc::ComVisibleForCamera()
{
	if (SGeom_ModelsGetCount() > 0)
		SGeom_ModelsComVisible(gdata::pCamera->getFrustum(), &gdata::vConstCurrCamPos);

	if (SGreen_GetCount() > 0)
		SGreen_ComVisible(gdata::pCamera->getFrustum(), &gdata::vConstCurrCamPos);

	SXAnim_ModelsComVisible(gdata::pCamera->getFrustum(), &gdata::vConstCurrCamPos);
}

void rfunc::ComVisibleReflection()
{
	static const int *r_reflection_render = GET_PCVAR_INT("r_reflection_render");

	if (r_reflection_render && (*r_reflection_render) == REFLECTION_RENDER_ONLY_SKY)
		return;

	for (int i = 0; i < SGeom_ModelsGetCount(); ++i)
	{
		for (int k = 0; k < SGeom_ModelsMGetCountGroups(i); ++k)
		{
			ID idMat = SGeom_ModelsMGetGroupIDMat(i, k);
			MTLTYPE_REFLECT typeReflection = SMtrl_MtlGetTypeReflection(SGeom_ModelsMGetGroupIDMat(i, k));
			float3_t center;

			if (typeReflection == MTLTYPE_REFLECT_PLANE)
			{
				if (!SMtrl_RefIsAllowedRender(idMat))
					continue;

				SGeom_ModelsMGetGroupCenter(i, k, &center);

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
				{
					if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0) < 0)
						SMtrl_RefSetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0, SGeom_ModelsAddArrForCom());
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
					SGeom_ModelsComVisible(SMtrl_RefGetfrustum(idMat, 0), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0));

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GREEN)
					SGreen_ComVisible(SMtrl_RefGetfrustum(idMat, 0), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GREEN, 0));

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_ANIM)
					SXAnim_ModelsComVisible(SMtrl_RefGetfrustum(idMat, 0), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_ANIM, 0));
			}
			else if (typeReflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
			{
				SGeom_ModelsMGetGroupCenter(i, k, &center);

				if (!SMtrl_RefIsAllowedRender(idMat))
					continue;

				for (int j = 0; j<6; j++)
				{
					if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
					{
						if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, k) < 0)
							SMtrl_RefSetIDArr(idMat, RENDER_IDARRCOM_GEOM, k, SGeom_ModelsAddArrForCom());
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
						SGeom_ModelsComVisible(SMtrl_RefGetfrustum(idMat, j), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, k));

					if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GREEN)
						SGreen_ComVisible(SMtrl_RefGetfrustum(idMat, j), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, k));

					if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_ANIM)
						SXAnim_ModelsComVisible(SMtrl_RefGetfrustum(idMat, j), &float3(center), SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_ANIM, k));
				}
			}
		}
	}
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

	LPDIRECT3DSURFACE9 BackBuf;
	gdata::pDXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	D3DXSaveSurfaceToFile(tmppath, D3DXIFF_JPG, BackBuf, NULL, NULL);
	mem_release(BackBuf);
}

void rfunc::SaveWorkTex()
{
	char tmppath[1024];
	sprintf(tmppath, "%scolor.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_GbufferGetRT(DS_RT_COLOR), NULL);

	sprintf(tmppath, "%snormal.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_GbufferGetRT(DS_RT_NORMAL), NULL);

	sprintf(tmppath, "%sdepth.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_GbufferGetRT(DS_RT_DEPTH), NULL);

	sprintf(tmppath, "%sdepth0.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_GbufferGetRT(DS_RT_DEPTH0), NULL);

	sprintf(tmppath, "%sparam.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_GbufferGetRT(DS_RT_PARAM), NULL);

	sprintf(tmppath, "%sambient_diff.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_GbufferGetRT(DS_RT_AMBIENTDIFF), NULL);

	sprintf(tmppath, "%sspecular.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_GbufferGetRT(DS_RT_SPECULAR), NULL);


	sprintf(tmppath, "%slight_com_1.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_GbufferGetRT(DS_RT_SCENELIGHT), NULL);

	sprintf(tmppath, "%slight_com_2.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SGCore_GbufferGetRT(DS_RT_SCENELIGHT2), NULL);
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

	if (gdata::idDefaultGeomArr < 0)
		gdata::idDefaultGeomArr = SGeom_ModelsAddArrForCom();

	if (gdata::idDefaultGreenArr < 0)
		gdata::idDefaultGreenArr = SGreen_AddVisCaclObj();

	if (gdata::idDefaultAnimArr < 0)
		gdata::idDefaultAnimArr = SXAnim_ModelsAddArrForCom();
}

int rfunc::OutputDebugInfo(DWORD timeDelta, bool needGameTime, const char *szStr)
{
	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

	static int FrameCount = 0;
	static int FrameCount2 = 0;
	static float TimeElapsed = 0;
	static float FpsValue = 0;
	static char FpsStr[1024];
	static char debugstr[SXGC_STR_SIZE_DBG_MSG];

	++FrameCount;
	TimeElapsed += ((float)timeDelta) * 0.001f;
	static const int * r_stats = GET_PCVAR_INT("r_stats");

	static bool isNulled = false;

	if (TimeElapsed >= 1.0f && r_stats)
	{
		FpsValue = (float)FrameCount / TimeElapsed;

		debugstr[0] = 0;

		if ((*r_stats) > 0)
			sprintf(debugstr, "FPS %.1f\n", FpsValue);

		if ((*r_stats) > 0 && needGameTime)
		{
			tm g_tm;
			time_t g_time = Core_TimeUnixCurrGet(Core_RIntGet(G_RI_INT_TIMER_GAME));
			localtime_s(&g_tm, &g_time);

			sprintf(debugstr + strlen(debugstr), "\nGame time : %d %d %d %d %d %d\n", 1900 + g_tm.tm_year, g_tm.tm_mon, g_tm.tm_mday, g_tm.tm_hour, g_tm.tm_min, g_tm.tm_sec);
		}

		if ((*r_stats) == 2)
		{
			sprintf(debugstr + strlen(debugstr), szStr);
		}

		TimeElapsed = 0.0f;
		FrameCount2 = FrameCount;
		FrameCount = 0;

		isNulled = true;
	}
	else
		isNulled = false;

	if (r_stats && (*r_stats) > 0)
		SGCore_DbgMsg(debugstr);

	return (isNulled ? FrameCount2 : 0);
}

//##########################################################################

void rfunc::BuildMRT(DWORD timeDelta, bool isRenderSimulation)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");
	static float2_t vWinSize;
	vWinSize = float2(*r_win_width, *r_win_height);

	SGCore_ShaderUnBind();

	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_MATERIAL);

	Core_RMatrixGet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	rfunc::SetRenderSceneFilter();

	LPDIRECT3DSURFACE9 BackBuf, ColorSurf, NormalSurf, ParamSurf, DepthMapLinearSurf;

	SGCore_GbufferGetRT(DS_RT_COLOR)->GetSurfaceLevel(0, &ColorSurf);
	SGCore_GbufferGetRT(DS_RT_NORMAL)->GetSurfaceLevel(0, &NormalSurf);
	SGCore_GbufferGetRT(DS_RT_PARAM)->GetSurfaceLevel(0, &ParamSurf);
	SGCore_GbufferGetRT(DS_RT_DEPTH)->GetSurfaceLevel(0, &DepthMapLinearSurf);

	//очищаем рт глубины  максимальным значением
	//чтобы там где нет окружения к примеру был скайбокс, а значит в рт глубины было максимальное значение - максимальная отдаленность
	gdata::pDXDevice->SetRenderTarget(3, DepthMapLinearSurf);
	gdata::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
	gdata::pDXDevice->SetRenderTarget(3, 0);	//убираем рт глубины

	gdata::pDXDevice->SetRenderTarget(1, NormalSurf);
	gdata::pDXDevice->SetRenderTarget(2, ParamSurf);
	gdata::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	gdata::pDXDevice->SetRenderTarget(1, 0);
	gdata::pDXDevice->SetRenderTarget(2, 0);

	gdata::pDXDevice->GetRenderTarget(0, &BackBuf);
	gdata::pDXDevice->SetRenderTarget(0, ColorSurf);
	gdata::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, RENDER_DEFAUL_BACKGROUND_COLOR/*D3DCOLOR_ARGB(0, 0, 0, 0)*/, 1.0f, 0);

	gdata::pDXDevice->SetRenderTarget(1, NormalSurf);
	gdata::pDXDevice->SetRenderTarget(2, ParamSurf);
	gdata::pDXDevice->SetRenderTarget(3, DepthMapLinearSurf);	//ставим рт глубины

	SMtrl_MtlNullingCurrCountSurf();
	SMtrl_MtlSetCurrCountSurf(RENDER_LAYER_UNTRANSPARENT);


	if (!isRenderSimulation)
	{
		//SXDecals_Render();
		if (SGeom_ModelsGetCount() > 0)
			SGeom_ModelsRender(timeDelta, MTLSORT_OPAQUE);

		SXAnim_Render();

		if (SGreen_GetCount() > 0)
			SGreen_Render(timeDelta, &gdata::vConstCurrCamPos, GREEN_TYPE_ALL);

		SGCore_ShaderUnBind();
	}

	//копируем глубину нулевого слоя (непрозрачной геометрии) в рт непрозрачной глубины
	//{
	//mem_release_del(DepthMapLinearSurf);
	//gdata::pDXDevice->SetRenderTarget(0, BackBuf);
	gdata::pDXDevice->SetRenderTarget(1, 0);
	gdata::pDXDevice->SetRenderTarget(2, 0);
	gdata::pDXDevice->SetRenderTarget(3, 0);

	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	LPDIRECT3DSURFACE9 DepthSurf2;
	SGCore_GbufferGetRT(DS_RT_DEPTH0)->GetSurfaceLevel(0, &DepthSurf2);

	gdata::pDXDevice->SetRenderTarget(0, DepthSurf2);

	gdata::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_DEPTH));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idScreenOut);

	SGCore_ScreenQuadDraw();


	rfunc::SetRenderSceneFilter();
	//SGCore_GbufferGetRT(DS_RT_DEPTH)->GetSurfaceLevel(0, &DepthMapLinearSurf);
	gdata::pDXDevice->SetRenderTarget(0, ColorSurf);
	gdata::pDXDevice->SetRenderTarget(1, NormalSurf);
	gdata::pDXDevice->SetRenderTarget(2, ParamSurf);
	gdata::pDXDevice->SetRenderTarget(3, DepthMapLinearSurf);

	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

	mem_release_del(DepthSurf2);
	//}}

	if (!isRenderSimulation)
	{
		//если есть что к отрисовке из полупрозрачной геометрии
		if (SGeom_ModelsSortExistsForRender(MTLTYPE_TRANSPARENCY))
		{
			//тут такая ситуация ... есть два рабочих варианта, причем работают чутка по разному, возможно я изработался и не могу сообразить что да как ...
			//первый вариант, чистим в 4, метим 3 раза начиная с нуля (первый раз 0, второй 1 третий 2 НЕ ИНКРЕМЕНТ а метка)
			//рисуем с сотрировкой front to back один раз, значение 1 функция D3DCMP_LESSEQUAL, с декрементом в случае успеха стенсил теста
			//но этот вариант не совсем понятно почему работает стабильно для трех ппрозрачных поверхностей, причем миганий перехода на передний план у двух (ппповерхностей) последних не наблюдалось
			//второй вариант, чистим в 0, метим 3 раза начиная с 1 (первый 1, второй 2, третий 3)
			//рисуем с сотрировкой front to back один раз, значение 0 функция D3DCMP_NOTEQUAL, с декрементом в случае успеха стенсил теста
			//второй случай логичнее, однако на двух дальних плоскостях иногда наблюдается переход одной плоскости на передний план, что слегка заметно
			//пересмотреть этот момент как будет время, а пока оставить второй вариант как наиболее логичный

			gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			gdata::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);
			gdata::pDXDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f, 0);

			gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

			gdata::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

			gdata::pDXDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

			gdata::pDXDevice->SetRenderState(D3DRS_STENCILREF, 1);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idScreenOut);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idStencilStr);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idStencilStr, "g_vWinSize", &vWinSize);

			SGCore_ScreenQuadDraw();

			gdata::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILREF, 2);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idScreenOut);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idStencilColumn);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idStencilColumn, "g_vWinSize", &vWinSize);

			SGCore_ScreenQuadDraw();

			gdata::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILREF, 3);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idScreenOut);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idStencilStrColumn);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idStencilStrColumn, "g_vWinSize", &vWinSize);

			SGCore_ScreenQuadDraw();

			gdata::pDXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

			gdata::pDXDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

			gdata::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

			SGCore_ShaderUnBind();

			SMtrl_MtlSetForceblyAlphaTest(true);

			gdata::pDXDevice->SetRenderState(D3DRS_STENCILREF, 0);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);

			if (SGeom_ModelsGetCount() > 0)
			{
				SMtrl_MtlSetIsIncrCountSurf(true);
				SMtrl_MtlSetCurrCountSurf(RENDER_LAYER_TRANSPARENT);

				SGeom_ModelsRender(timeDelta, MTLTYPE_TRANSPARENCY, 0, true);
			}


			SMtrl_MtlSetForceblyAlphaTest(false);
			gdata::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

			SGCore_ShaderUnBind();

			gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		}

		SXDecals_Render();
	}
	else
	{
		if (SMtrl_MtlGetSort(gdata::Editors::pSimModel->getIdMtl()) != MTLSORT_OPAQUE)
			SMtrl_MtlSetForceblyAlphaTest(true);
		gdata::Editors::pSimModel->render(timeDelta);
		if (SMtrl_MtlGetSort(gdata::Editors::pSimModel->getIdMtl()) != MTLSORT_OPAQUE)
			SMtrl_MtlSetForceblyAlphaTest(false);
	}

	gdata::pDXDevice->SetRenderTarget(0, BackBuf);
	gdata::pDXDevice->SetRenderTarget(1, 0);
	gdata::pDXDevice->SetRenderTarget(2, 0);
	gdata::pDXDevice->SetRenderTarget(3, 0);

	mem_release(BackBuf);

	mem_release(ColorSurf);
	mem_release(NormalSurf);
	mem_release(ParamSurf);
	mem_release(DepthMapLinearSurf);

	rfunc::SetRenderSceneFilterUn();
}


void rfunc::UpdateShadow(DWORD timeDelta)
{
	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_SHADOW);
	SLight_ComVisibleFrustumDistFor(gdata::pCamera->getFrustum(), &gdata::vConstCurrCamPos);
	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	SetSamplerFilter(0, D3DTEXF_LINEAR);
	SetSamplerAddress(0, D3DTADDRESS_WRAP);

	gdata::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);

	for (int i = 0; i<SLight_GetCount(); i++)
	{
		if (!SLight_GetExists(i))
			continue;

		Core_RIntSet(G_RI_INT_CURRIDLIGHT, i);
		if (SLight_GetShadowed(i) && (SLight_ComVisibleForFrustum(i, gdata::pCamera->getFrustum()) && SLight_GetEnable(i)) /*|| (Data::Level::LightManager->Arr[i]->ShadowCube && Data::Level::LightManager->Arr[i]->ShadowCube->GetStatic() && !Data::Level::LightManager->Arr[i]->ShadowCube->GetUpdate())*/)
		{
			if (SLight_GetType(i) == LTYPE_LIGHT_GLOBAL)
			{
				SLight_ShadowRenderBegin(i);
				int countsplits = (SLight_Get4Or3SplitsG(i) ? 4 : 3);
				for (int k = 0; k<countsplits; k++)
				{
					if (SLight_CountUpdateAllowed(i, k))
					{
						SLight_ShadowRenderPre(i, k);

						if (SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, k) > -1)
							SGeom_ModelsRender(timeDelta, MTLSORT_OPAQUE, SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, k));

						if (SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, k) > -1)
							SGreen_Render(timeDelta, &gdata::vConstCurrCamPos, GREEN_TYPE_TREE, SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, k));

						SXAnim_Render();
					}
				}

				//КОГДА ИСТОЧНИК БЛИЗОК К ГОРИЗОНТУ ИЗ-ЗА ОБЛАКОВ ВОЗНИКАЕТ БАГ С ТЕНЯМИ В ВИДЕ ФЕЙКОВЫХ ТЕНЕЙ
				if (SGCore_SkyCloudsIsCr())
				{
					SLight_UpdateGFrustums(i, 4, &gdata::vConstCurrCamPos, &gdata::vConstCurrCamDir);
					SLight_ShadowRenderPre(i, 4);
					gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
					gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
					SetSamplerFilter(0, D3DTEXF_LINEAR);
					SetSamplerFilter(1, D3DTEXF_LINEAR);
					SetSamplerAddress(0, D3DTADDRESS_MIRROR);
					SetSamplerAddress(1, D3DTADDRESS_MIRROR);

					if (SGCore_SkyCloudsIsLoadTex())
						SGCore_SkyCloudsRender(timeDelta, &float3(gdata::vConstCurrCamPos.x, gdata::vConstCurrCamPos.y + 150, gdata::vConstCurrCamPos.z), true);
					else
						gdata::pDXDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);

					SetSamplerAddress(0, D3DTADDRESS_WRAP);
					SetSamplerAddress(1, D3DTADDRESS_WRAP);
				}
				SLight_ShadowRenderEnd(i);
			}
			else if (SLight_GetType(i) == LTYPE_LIGHT_DIR)
			{
				gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				if (SLight_CountUpdateUpdate(i, &gdata::vConstCurrCamPos))
				{
					SLight_ShadowRenderBegin(i);

					if (SLight_GetTypeShadowed(i) == LTYPE_SHADOW_DYNAMIC)
					{
						if (SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, 0) > -1)
							SGeom_ModelsRender(timeDelta, MTLSORT_OPAQUE, SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, 0));

						if (SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, 0) > -1)
							SGreen_Render(timeDelta, &gdata::vConstCurrCamPos, GREEN_TYPE_TREE, SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, 0));

						SXAnim_Render();
					}
					else
					{
						SGeom_ModelsComVisible(SLight_GetFrustum(i, 0), &gdata::vConstCurrCamPos, gdata::idDefaultGeomArr);
						SGeom_ModelsRender(timeDelta, -1, gdata::idDefaultGeomArr);
						SGreen_ComVisible(SLight_GetFrustum(i, 0), &gdata::vConstCurrCamPos, gdata::idDefaultGreenArr);
						SGreen_Render(timeDelta, &gdata::vConstCurrCamPos, GREEN_TYPE_TREE, gdata::idDefaultGreenArr);
					}

					SLight_ShadowRenderEnd(i);
				}
			}
			else if (SLight_GetType(i) == LTYPE_LIGHT_POINT)
			{
				gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				if (SLight_GetTypeShadowed(i) != LTYPE_SHADOW_NONE && SLight_CountUpdateUpdate(i, &gdata::vConstCurrCamPos))
				{
					SLight_ShadowRenderBegin(i);

					for (int k = 0; k < 6; k++)
					{
						if (SLight_GetCubeEdgeEnable(i, k))
						{
							SLight_ShadowRenderPre(i, k);

							if (SLight_GetTypeShadowed(i) == LTYPE_SHADOW_DYNAMIC)
							{
								if (SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, k) > -1)
									SGeom_ModelsRender(timeDelta, MTLSORT_OPAQUE, SLight_GetIDArr(i, RENDER_IDARRCOM_GEOM, k));

								if (SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, k) > -1)
									SGreen_Render(timeDelta, &gdata::vConstCurrCamPos, GREEN_TYPE_TREE, SLight_GetIDArr(i, RENDER_IDARRCOM_GREEN, k));

								SXAnim_Render();
							}
							else
							{
								SGeom_ModelsComVisible(SLight_GetFrustum(i, k), &gdata::vConstCurrCamPos, gdata::idDefaultGeomArr);
								SGeom_ModelsRender(timeDelta, -1, gdata::idDefaultGeomArr);
								SGreen_ComVisible(SLight_GetFrustum(i, k), &gdata::vConstCurrCamPos, gdata::idDefaultGreenArr);
								SGreen_Render(timeDelta, &gdata::vConstCurrCamPos, GREEN_TYPE_TREE, gdata::idDefaultGreenArr);
							}

							SLight_ShadowRenderPost(i, k);
						}
					}

					SLight_ShadowRenderEnd(i);
				}
			}
		}
	}
	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_FREE);
	gdata::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}

void rfunc::RenderSky(DWORD timeDelta)
{
	LPDIRECT3DSURFACE9 ColorSurf, BackBuf;
	SGCore_GbufferGetRT(DS_RT_SCENELIGHT)->GetSurfaceLevel(0, &ColorSurf);
	gdata::pDXDevice->GetRenderTarget(0, &BackBuf);
	gdata::pDXDevice->SetRenderTarget(0, ColorSurf);

	gdata::pDXDevice->Clear(0, 0, D3DCLEAR_TARGET, RENDER_DEFAUL_BACKGROUND_COLOR, 1.0f, 0);

	SetSamplerFilter(0, 2, D3DTEXF_ANISOTROPIC);

	if (SGCore_SkyBoxIsCr() && SGCore_SkyBoxIsLoadTex())
	{
		gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		SetSamplerAddress(0, 2, D3DTADDRESS_CLAMP);
		SGCore_SkyBoxRender(timeDelta, &float3(gdata::vConstCurrCamPos.x, gdata::vConstCurrCamPos.y + 40, gdata::vConstCurrCamPos.z));
	}

	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	gdata::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gdata::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (SGCore_SkyCloudsIsCr() && SGCore_SkyCloudsIsLoadTex())
	{
		SetSamplerAddress(0, 2, D3DTADDRESS_MIRROR);
		SGCore_SkyCloudsRender(timeDelta, &float3(gdata::vConstCurrCamPos.x, gdata::vConstCurrCamPos.y + 150, gdata::vConstCurrCamPos.z), false);
	}

	gdata::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_SCENELIGHT2));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idScreenOut);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(ColorSurf);

	gdata::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);

	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void rfunc::ComLighting(DWORD timeDelta)
{
	static int *r_win_width = (int*)GET_PCVAR_INT("r_win_width");
	static int *r_win_height = (int*)GET_PCVAR_INT("r_win_height");

	SGCore_ShaderUnBind();

	LPDIRECT3DSURFACE9 pAmbientSurf, pSpecDiffSurf, pBackBuf;
	SGCore_GbufferGetRT(DS_RT_AMBIENTDIFF)->GetSurfaceLevel(0, &pAmbientSurf);
	SGCore_GbufferGetRT(DS_RT_SPECULAR)->GetSurfaceLevel(0, &pSpecDiffSurf);

	gdata::pDXDevice->GetRenderTarget(0, &pBackBuf);

	gdata::pDXDevice->SetRenderTarget(0, pAmbientSurf);
	gdata::pDXDevice->SetRenderTarget(1, pSpecDiffSurf);

	//очищаем рт и стенсил
	gdata::pDXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	//устанавка аддитивного смешивания
	//когда к уже записанному будет прибавляться то что хотим записать
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	gdata::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	gdata::pDXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	gdata::pDXDevice->SetTransform(D3DTS_WORLD, &((D3DXMATRIX)SMMatrixIdentity()));
	gdata::pDXDevice->SetTransform(D3DTS_VIEW, &((D3DXMATRIX)gdata::mCamView));
	gdata::pDXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)gdata::mLightProj));

	//проходимся циклом по всем источникам света
	for (int i = 0; i<SLight_GetCount(); i++)
	{
		if (!SLight_GetExists(i))
			continue;

		//если свет виден фрустуму камеры (это надо было заранее просчитать) и если свет включен
		if (SLight_GetVisibleForFrustum(i) && SLight_GetEnable(i))
		{
			//пока что назначаем шейдер без теней
			ID idshader = gdata::shaders_id::ps::idComLightingNonShadow;

			//если не глобальный источник
			if (SLight_GetType(i) != LTYPE_LIGHT_GLOBAL)
			{
				//помечаем в стенсил буфере пиксели  которые входят в ограничивающий объем света, чтобы их осветить

				//отключаем вывод цвета
				gdata::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);

				//установка стенсил теста, причем и двухстороннего тоже
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
				gdata::pDXDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);

				//вклчить тест глубины, но запись выключить, установить стандартную функцию проверки глубины
				gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
				gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
				gdata::pDXDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

				//стенсил тест проходит всегда удачно, при провале теста глубины инкрементируем значение в стенсиле
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
				//при удачно проходе, игнорируем
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

				//стенсил тест для обратной глубины проходит всегда удачно, при провале теста глубины декрементируем значение в стенсиле
				gdata::pDXDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
				gdata::pDXDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_DECR);
				//при удачно проходе, игнорируем
				gdata::pDXDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP);

				//установка значений для записи
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILREF, 0x0);
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
				gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//включение двухсторонней отрисовки

				gdata::pDXDevice->SetTransform(D3DTS_WORLD, &((D3DXMATRIX)SMMatrixIdentity()));
				gdata::pDXDevice->SetTransform(D3DTS_VIEW, &((D3DXMATRIX)gdata::mCamView));
				gdata::pDXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)gdata::mLightProj));

				//отрисовка ограничивающего объема
				SLight_Render(i, 0);

				//
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

				gdata::pDXDevice->SetRenderState(D3DRS_STENCILREF, 255);

				//включаем вывод цвета
				gdata::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
			}
			else
			{
				//иначе это глобальный источник, отключаем стенсил тест
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
				gdata::pDXDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
			}

			//отключаем тест глубины ибо будем теперь пост процессом обрабатывать полученные данные
			gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

			gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			//если свет отбрасывает тени
			if (SLight_GetShadowed(i))
			{
				//генерация теней для текущего света
				//{{
				//так как нам нужно провести очистку рт то убираем оба рт
				gdata::pDXDevice->SetRenderTarget(0, 0);
				gdata::pDXDevice->SetRenderTarget(1, 0);

				//отключаем смешивание, нам не нужен хлам в рт
				gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
				gdata::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);

				SLight_ShadowNull();	//очищаем рт генерации теней
				SLight_ShadowGen(i);	//генерируем тень для света

				static const int * r_shadow_soft = GET_PCVAR_INT("r_shadow_soft");

				if (r_shadow_soft)
				{
					if ((*r_shadow_soft) == 1)
						SLight_ShadowSoft(false, 2);
					else if ((*r_shadow_soft) == 2)
					{
						SLight_ShadowSoft(false, 2);
						SLight_ShadowSoft(false, 2);
					}
				}

				gdata::pDXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
				//}}

				//включаем смешивание для освещения
				gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

				//опять назначаем второй рт
				gdata::pDXDevice->SetRenderTarget(0, pAmbientSurf);
				gdata::pDXDevice->SetRenderTarget(1, pSpecDiffSurf);

				//устанавливаем текстуру с тенями и переназначаем шейдер, теперь уже с тенями
				gdata::pDXDevice->SetTexture(4, SLight_GetShadow());
				idshader = gdata::shaders_id::ps::idComLightingShadow;

				//if (GetAsyncKeyState('Q'))
					//D3DXSaveTextureToFile((String("C:/1/SLight_GetShadow") + String(i) + ".jpg").c_str(), D3DXIFF_JPG, SLight_GetShadow(), NULL);

				/*if (i == SLight_GetGlobal())
				gdata::pDXDevice->SetTexture(4, SGCore_LoadTexGetTex(SGCore_LoadTexGetID("g_shadow")));*/
			}

			SGCore_ShaderUnBind();

			//теперь когда будем считать освещение надо сбросить значения в стенсил буфере, чтобы каждый кадр не чистить
			//если стенсил тест прошел успешно, устанавливаем значнеие в нуль
			if (SLight_GetType(i) != LTYPE_LIGHT_GLOBAL)
				gdata::pDXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);

			float determ = 0;
			float4x4 ViewInv = SMMatrixInverse(&determ, gdata::mCamView);
			ViewInv = SMMatrixTranspose(ViewInv);

			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idResPos, "g_mViewInv", &ViewInv);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idResPos, "g_vNearFar", &gdata::vNearFar);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idResPos, "g_vParamProj", &float3_t(*r_win_width, *r_win_height, gdata::fProjFov));

			float3 tmpPosition;
			float2 tmpPowerDist;
			float3 tmpColor;
			SLight_GetColor(i, &tmpColor);
			SLight_GetPos(i, &tmpPosition, true);
			tmpPowerDist.x = SLight_GetPower(i);
			tmpPowerDist.y = SLight_GetDist(i);

			if (SLight_GetType(i) != LTYPE_LIGHT_GLOBAL)
			{
				tmpColor.w = 0.f;
				ID gl_id = -1;
				if ((gl_id = SLight_GetGlobal()) >= 0)
				{
					float gl_power = 0.f;
					if (SLight_GetEnable(gl_id))
						gl_power = SLight_GetPower(gl_id);

					float f_dep_coef = clampf(1.f - gl_power, 0.25f, 1.f);
					tmpPowerDist.x *= f_dep_coef;
					tmpPowerDist.y *= f_dep_coef;
				}
			}
			else
				tmpColor.w = 1.f;

			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "g_vViewPos", &gdata::vConstCurrCamPos);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "g_vLightPos", &(tmpPosition));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "g_vLightPowerDist", &(tmpPowerDist));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "g_vLightColor", &tmpColor);
			//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "vNearFar", &gdata::vNearFar);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idResPos);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, idshader);

			SetSamplerFilter(0, 5, D3DTEXF_NONE);
			SetSamplerAddress(0, 5, D3DTADDRESS_CLAMP);

			gdata::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_COLOR));
			gdata::pDXDevice->SetTexture(1, SGCore_GbufferGetRT(DS_RT_NORMAL));
			gdata::pDXDevice->SetTexture(2, SGCore_GbufferGetRT(DS_RT_PARAM));
			gdata::pDXDevice->SetTexture(3, SGCore_GbufferGetRT(DS_RT_DEPTH));
			gdata::pDXDevice->SetTexture(5, SGCore_GbufferGetRT(DS_RT_ADAPTEDLUM));

			SGCore_ScreenQuadDraw();

			SGCore_ShaderUnBind();
		}
	}

	gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	gdata::pDXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);

	gdata::pDXDevice->SetRenderTarget(0, pBackBuf);
	gdata::pDXDevice->SetRenderTarget(1, 0);

	mem_release(pBackBuf);

	mem_release(pAmbientSurf);
	mem_release(pSpecDiffSurf);

	//-------------------------------

	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//теперь необходимо все смешать чтобы получить итоговую освещенную картинку
	//{{
	SetSamplerFilter(0, 5, D3DTEXF_NONE);
	SetSamplerAddress(0, 5, D3DTADDRESS_CLAMP);
	LPDIRECT3DSURFACE9 pComLightSurf;

	SGCore_GbufferGetRT(DS_RT_SCENELIGHT)->GetSurfaceLevel(0, &pComLightSurf);

	gdata::pDXDevice->GetRenderTarget(0, &pBackBuf);
	gdata::pDXDevice->SetRenderTarget(0, pComLightSurf);

	//очищаем рт (в старой версии было многопроходное смешивание)
	gdata::pDXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	gdata::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_COLOR));
	gdata::pDXDevice->SetTexture(1, SGCore_GbufferGetRT(DS_RT_AMBIENTDIFF));
	gdata::pDXDevice->SetTexture(2, SGCore_GbufferGetRT(DS_RT_SPECULAR));
	gdata::pDXDevice->SetTexture(3, SGCore_GbufferGetRT(DS_RT_NORMAL));
	gdata::pDXDevice->SetTexture(4, SGCore_GbufferGetRT(DS_RT_ADAPTEDLUM));
	gdata::pDXDevice->SetTexture(5, SGCore_GbufferGetRT(DS_RT_PARAM));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idBlendAmbientSpecDiffColor);

	SGCore_ScreenQuadDraw();

	mem_release(pComLightSurf);
	//}}

	SGCore_ShaderUnBind();

	gdata::pDXDevice->SetRenderTarget(0, pBackBuf);
	mem_release(pBackBuf);
}

void rfunc::UnionLayers()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	LPDIRECT3DSURFACE9 pBackBuf, pComLightSurf;
	gdata::pDXDevice->GetRenderTarget(0, &pBackBuf);

	static int iCurrCountTransparencySurf;

#if defined(SXMATERIAL_EDITOR)
	iCurrCountTransparencySurf = 3;
#else
	iCurrCountTransparencySurf = SMtrl_MtlGetCurrCountSurf();
#endif

	if (iCurrCountTransparencySurf >= RENDER_LAYER_TRANSPARENT)
	{
		LPDIRECT3DSURFACE9 pColorSurf, pColor2Surf, pDepthSurf;
		SGCore_GbufferGetRT(DS_RT_SCENELIGHT2)->GetSurfaceLevel(0, &pColorSurf);
		gdata::pDXDevice->SetRenderTarget(0, pColorSurf);
		SGCore_GbufferGetRT(DS_RT_DEPTH0)->GetSurfaceLevel(0, &pDepthSurf);
		gdata::pDXDevice->SetRenderTarget(1, pDepthSurf);
		SGCore_GbufferGetRT(DS_RT_DEPTH1)->GetSurfaceLevel(0, &pColor2Surf);
		gdata::pDXDevice->SetRenderTarget(2, pColor2Surf);
		gdata::pDXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		gdata::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_COLOR));
		gdata::pDXDevice->SetTexture(1, SGCore_GbufferGetRT(DS_RT_SCENELIGHT));
		gdata::pDXDevice->SetTexture(2, SGCore_GbufferGetRT(DS_RT_DEPTH));
		gdata::pDXDevice->SetTexture(3, SGCore_GbufferGetRT(DS_RT_NORMAL));
		gdata::pDXDevice->SetTexture(4, SGCore_GbufferGetRT(DS_RT_PARAM));

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idScreenOut);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idUnionAlpha);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idUnionAlpha, "g_vWinSize", &float4_t(*r_win_width, *r_win_height, 1.f / float(*r_win_width), 1.f / float(*r_win_height)));

		SGCore_ScreenQuadDraw();

		gdata::pDXDevice->SetRenderTarget(1, 0);
		gdata::pDXDevice->SetRenderTarget(2, 0);
		mem_release(pColorSurf);
		mem_release(pDepthSurf);
		mem_release(pColor2Surf);
	}
	else
	{
		//копируем значения только для ds_rt_depth1, а ds_rt_depth0 уже заполнен

		LPDIRECT3DSURFACE9 pDepthSurf1;

		SGCore_GbufferGetRT(DS_RT_DEPTH1)->GetSurfaceLevel(0, &pDepthSurf1);
		gdata::pDXDevice->SetRenderTarget(0, pDepthSurf1);

		gdata::pDXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		gdata::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_DEPTH));

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idScreenOut);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idScreenOut);

		SGCore_ScreenQuadDraw();

		gdata::pDXDevice->SetRenderTarget(0, 0);
		mem_release(pDepthSurf1);
	}

	//копируем итоговую сцену
	SGCore_GbufferGetRT((iCurrCountTransparencySurf >= RENDER_LAYER_TRANSPARENT ? DS_RT_SCENELIGHT : DS_RT_SCENELIGHT2))->GetSurfaceLevel(0, &pComLightSurf);
	gdata::pDXDevice->SetRenderTarget(0, pComLightSurf);
	gdata::pDXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0, 0);
	gdata::pDXDevice->SetTexture(0, SGCore_GbufferGetRT((iCurrCountTransparencySurf >= RENDER_LAYER_TRANSPARENT ? DS_RT_SCENELIGHT2 : DS_RT_SCENELIGHT)));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, gdata::shaders_id::vs::idScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, gdata::shaders_id::ps::idScreenOut);

	SGCore_ScreenQuadDraw();

	mem_release(pComLightSurf);

	SGCore_ShaderUnBind();

	gdata::pDXDevice->SetRenderTarget(0, pBackBuf);
	mem_release(pBackBuf);
}

void rfunc::RenderParticles(DWORD timeDelta)
{
	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHAREF, RENDER_PARTICLES_ALPHATEST_VALUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	SetSamplerFilter(0, 3, D3DTEXF_LINEAR);
	SetSamplerAddress(0, 3, D3DTADDRESS_WRAP);

	SPE_EffectRenderAll(timeDelta);

	gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
}

void rfunc::RenderMainPostProcess(DWORD timeDelta)
{
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
		SPP_RenderBloom(&float3_t(1, 0.7, 1.0));

	//**********************************************************************

	float3 vGLpos;
	float3 vGLcolor;

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
	if (pp_lensflare && (*pp_lensflare) && idGlobalLight >= 0)
		SPP_RenderLensFlare(&float3_t(0.25f, 0.3f, 0.2f), &float4_t(vGLcolor.x, vGLcolor.y, vGLcolor.z, (SLight_GetCastGlobalShadow() ? 0 : SLight_GetPower(idGlobalLight))), (pp_lensflare_usebloom ? (*pp_lensflare_usebloom) : false));

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
}

void rfunc::RenderFinalPostProcess(DWORD timeDelta)
{
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
}

void rfunc::ShaderRegisterData()
{
	static float4_t aNull[256];
	memset(aNull, 0, sizeof(float4_t)* 256);
	gdata::pDXDevice->SetVertexShaderConstantF(0, (float*)&aNull, 256);
	gdata::pDXDevice->SetPixelShaderConstantF(0, (float*)&aNull, 256);
}


void rfunc::UpdateReflectionScene(DWORD timeDelta)
{
	static const int *r_reflection_render = GET_PCVAR_INT("r_reflection_render");

	float3 vObserverPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

	for (int i = 0; i < SGeom_ModelsGetCount(); ++i)
	{
		for (int k = 0; k < SGeom_ModelsMGetCountGroups(i); ++k)
		{
			ID idMat = SGeom_ModelsMGetGroupIDMat(i, k);
			MTLTYPE_REFLECT typeReflection = SMtrl_MtlGetTypeReflection(SGeom_ModelsMGetGroupIDMat(i, k));
			D3DXPLANE oPlane;
			float3_t vCenter;

			if (typeReflection == MTLTYPE_REFLECT_PLANE)
			{
				if (!SMtrl_RefIsAllowedRender(idMat))
				{
					SMtrl_RefUpdateCountUpdate(idMat, &((float3_t)gdata::vConstCurrCamPos));
					continue;
				}

				SGeom_ModelsMGetGroupPlane(i, k, &oPlane);
				SGeom_ModelsMGetGroupCenter(i, k, &vCenter);
				SMtrl_RefPreRenderPlane(idMat, &oPlane);
				SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
				SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

				Core_RBoolSet(G_RI_BOOL_CLIPPLANE0, true);

				Core_RFloat3Set(G_RI_FLOAT3_CLIPPLANE0_NORMAL, &float3(oPlane.a, oPlane.b, oPlane.c));
				Core_RFloat3Set(G_RI_FLOAT3_CLIPPLANE0_POINT, &float3(vCenter));

				if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
				{
					if (SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0) >= 0)
						SGeom_ModelsRender(timeDelta, MTLSORT_OPAQUE, SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, 0), false, i, k);
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

				if (SGCore_SkyBoxIsCr())
				{
					gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

					gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

					gdata::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
					gdata::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

					gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					SetSamplerAddress(0, 2, D3DTADDRESS_CLAMP);

					gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
					gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
					Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &gdata::mRefPlaneSkyProj);
					SGCore_SkyBoxRender(timeDelta, &float3(vCenter));
					gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
					gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
					gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				}

				SMtrl_RefPostRenderPlane(idMat);
			}
			else if (typeReflection == MTLTYPE_REFLECT_CUBE_STATIC || typeReflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
			{
				SGeom_ModelsMGetGroupCenter(i, k, &vCenter);
				float3_t min, max;
				SGeom_ModelsMGetGroupMin(i, k, &min);
				SGeom_ModelsMGetGroupMax(i, k, &max);
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
					SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
					SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

					if (r_reflection_render && (*r_reflection_render) != 0)
					{
						//если статические кубические отражения
						if (SMtrl_MtlGetTypeReflection(idMat) == MTLTYPE_REFLECT_CUBE_STATIC)
						{
							//тогда считаем в массив камеры
							if (r_reflection_render && (*r_reflection_render) >= REFLECTION_RENDER_GEOM)
							{
								SGeom_ModelsComVisible(SMtrl_RefGetfrustum(idMat, j), &float3(vCenter), gdata::idDefaultGeomArr);
								SGeom_ModelsRender(timeDelta, MTLSORT_OPAQUE, gdata::idDefaultGeomArr, false, i, k);
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
									SGeom_ModelsRender(timeDelta, MTLSORT_OPAQUE, SMtrl_RefGetIDArr(idMat, RENDER_IDARRCOM_GEOM, k), false, i, k);
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

					if (SGCore_SkyBoxIsCr())
					{
						gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

						gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
						gdata::pDXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

						gdata::pDXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
						gdata::pDXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

						gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
						gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

						gdata::pDXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
						SetSamplerAddress(0, 2, D3DTADDRESS_CLAMP);

						Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &gdata::mRefCubeSkyProj);

						SGCore_SkyBoxRender(timeDelta, &float3(vCenter));
						gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

						SMtrl_RefCubePostRender(idMat, j);

						gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
						gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
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
	D3DXPLANE oPlane;
	float3_t vCenter;
	gdata::Editors::pSimModel->getCenter(&vCenter);
	SGCore_ShaderUnBind();

	if (typeReflection == MTLTYPE_REFLECT_PLANE)
	{
		gdata::Editors::pSimModel->getPlane(&oPlane);
		SMtrl_RefPreRenderPlane(idMat, &oPlane);
		SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
		SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

		gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &gdata::mRefPlaneSkyProj);
		SGCore_SkyBoxRender(timeDelta, &float3(vCenter));
		gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

		SMtrl_RefPostRenderPlane(idMat);
	}
	else if (typeReflection == MTLTYPE_REFLECT_CUBE_STATIC || typeReflection == MTLTYPE_REFLECT_CUBE_DYNAMIC)
	{
		SMtrl_RefCubeBeginRender(idMat, &vCenter);

		for (int j = 0; j<6; ++j)
		{
			Core_RMatrixGet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

			SMtrl_RefCubePreRender(idMat, j, &(SMMatrixIdentity()));

			gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
			Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &gdata::mRefCubeSkyProj);
			SGCore_SkyBoxRender(timeDelta, &float3(vCenter));

			gdata::pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
			SMtrl_RefCubePostRender(idMat, j);
			gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		}
		SMtrl_RefCubeEndRender(idMat, &((float3_t)gdata::vConstCurrCamPos));
	}
}

void rfunc::UpdateReflection(DWORD timeDelta, bool isRenderSimulation)
{
	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_FREE);
	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &gdata::mCamView);

	gdata::pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

	if (!isRenderSimulation)
		rfunc::UpdateReflectionScene(timeDelta);
	else
		rfunc::UpdateReflectionSimModel(timeDelta);

}

//##########################################################################

void rfunc::RenderEditorMain()
{
	if (gdata::Editors::pGrid && gdata::Editors::canRenderGrid)
	{
		gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		gdata::Editors::pGrid->render();
	}

	if (gdata::Editors::pAxesStatic && gdata::Editors::canRenderAxesStatic)
	{
		gdata::pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		gdata::pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		gdata::Editors::pAxesStatic->render();
	}
}
