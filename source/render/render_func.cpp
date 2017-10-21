
#include "render_func.h"


namespace SXRenderFunc
{
	namespace Delay
	{
		int64_t UpdateVisibleForCamera = 0;
		int64_t UpdateVisibleForLight = 0;
		int64_t UpdateVisibleForReflection = 0;

		int64_t UpdateShadow = 0;
		int64_t UpdateParticles = 0;
		int64_t RenderMRT = 0;
		int64_t ComLighting = 0;
		int64_t PostProcess = 0;
		int64_t ComReflection = 0;
		int64_t GeomSortGroup = 0;

		int64_t Present = 0;

		int64_t FreeVal = 0;
		float FreeValF1 = 0;
		float FreeValF2 = 0;
		float FreeValF3 = 0;
	};
};


inline void SXRenderFunc::SetSamplerFilter(DWORD id, DWORD value)
{
	GData::DXDevice->SetSamplerState(id, D3DSAMP_MAGFILTER, value);
	GData::DXDevice->SetSamplerState(id, D3DSAMP_MINFILTER, value);
	GData::DXDevice->SetSamplerState(id, D3DSAMP_MIPFILTER, value);
}

inline void SXRenderFunc::SetSamplerAddress(DWORD id, DWORD value)
{
	GData::DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSU, value);
	GData::DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSV, value);
	GData::DXDevice->SetSamplerState(id, D3DSAMP_ADDRESSW, value);
}

inline void SXRenderFunc::SetSamplerFilter(DWORD begin_id, DWORD end_id, DWORD value)
{
	if (begin_id >= 0 && end_id <= 16)
	{
		for (int i = begin_id; i<=end_id; i++)
			SXRenderFunc::SetSamplerFilter(i, value);
	}
}

inline void SXRenderFunc::SetSamplerAddress(DWORD begin_id, DWORD end_id, DWORD value)
{
	if (begin_id >= 0 && end_id <= 16)
	{
		for (DWORD i = begin_id; i<=end_id; i++)
			SXRenderFunc::SetSamplerAddress(i, value);
	}
}

void SXRenderFunc::SetRenderSceneFilter()
{
	static const int * r_s_filter = GET_PCVAR_INT("r_s_filter");
	static const int * r_s_max_anisotropy = GET_PCVAR_INT("r_s_max_anisotropy");
	static const int * r_s_max_miplevel = GET_PCVAR_INT("r_s_max_miplevel");

	static int r_s_filter2 = 1;
	static int r_s_max_anisotropy2 = 0;
	static int r_s_max_miplevel2 = 0;

	if (r_s_filter)
		r_s_filter2 = (*r_s_filter);
	else
		r_s_filter2 = 1;

	if (r_s_max_anisotropy)
		r_s_max_anisotropy2 = (*r_s_max_anisotropy);
	else
		r_s_max_anisotropy2 = 1;

	if (r_s_max_miplevel)
		r_s_max_miplevel2 = (*r_s_max_miplevel);
	else
		r_s_max_miplevel2 = 1;
	
	if (r_s_filter2 == 0)
		SetSamplerFilter(0, 16, D3DTEXF_POINT);
	else if (r_s_filter2 == 2)
	{
		for (int i = 0; i<16; ++i)
			GData::DXDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, r_s_max_anisotropy2);
		SetSamplerFilter(0, 16, D3DTEXF_ANISOTROPIC);
	}
	else
		SetSamplerFilter(0, 16, D3DTEXF_LINEAR);

	GData::DXDevice->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, r_s_max_miplevel2);

	SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);
}

void SXRenderFunc::SetRenderSceneFilterUn()
{
	GData::DXDevice->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, 0);
}

//##########################################################################

void SXRenderFunc::ComDeviceLost()
{
	static int *resize = (int*)GET_PCVAR_INT("resize");

	static int *winr_width = (int*)GET_PCVAR_INT("winr_width");
	static int *winr_height = (int*)GET_PCVAR_INT("winr_height");
	static const bool *winr_windowed = GET_PCVAR_BOOL("winr_windowed");

	if (*resize != RENDER_RESIZE_CHANGE)
	{
		//получаем текущий размер окна в которое рисовали
		RECT rect_scene;
		GetClientRect(GData::Handle3D, &rect_scene);

		*winr_width = rect_scene.right;
		*winr_height = rect_scene.bottom;
	}

	//сбрасываем все что необходимо для восстановления устройства
	SGCore_OnLostDevice();
	SGeom_OnLostDevice();
	SML_OnLostDevice();
	SPE_OnLostDevice();

	SXRenderFunc::InitModeWindow();
	bool bf = SGCore_OnDeviceReset(*winr_width, *winr_height, *winr_windowed);
	
	if (bf)
	{
		//если все-таки функция зашла сюда значит что-то было неосвобождено
		g_fnReportf(REPORT_MSG_LEVEL_ERROR, "reset device is failed ... \n");
	}
	else
	{
		GData::InitAllMatrix();
		*resize = RENDER_RESIZE_NONE;
		SGCore_OnResetDevice();
		SML_OnResetDevice();
		SGeom_OnResetDevice();
		SPE_OnResetDevice();


		GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	}
}


void SXRenderFunc::ComVisibleForLight()
{
	for (int i = 0; i<SML_LigthsGetCount(); i++)
	{
		if (!SML_LigthsGetExists(i))
			continue;

		if (SML_LigthsGetShadowed(i) && (SML_LigthsComVisibleForFrustum(i, GData::ObjCamera->ObjFrustum) && SML_LigthsGetEnable(i)) /*|| (Data::Level::LightManager->Arr[i]->ShadowCube && Data::Level::LightManager->Arr[i]->ShadowCube->GetStatic() && !Data::Level::LightManager->Arr[i]->ShadowCube->GetUpdate())*/)
		{
			if (SML_LigthsGetType(i) == LTYPE_LIGHT_GLOBAL)
			{
				for (int k = 0; k<4; k++)
				{
					if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, k) <= -1)
						SML_LigthsSetIDArr(i, RENDER_IDARRCOM_GEOM, k, SGeom_ModelsAddArrForCom());

					if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, k) <= -1)
						SML_LigthsSetIDArr(i, RENDER_IDARRCOM_GREEN, k, SGeom_GreenAddArrForCom());

					if(SML_LigthsGetIDArr(i, RENDER_IDARRCOM_ANIM, k) <= -1)
						SML_LigthsSetIDArr(i, RENDER_IDARRCOM_ANIM, k, SXAnim_ModelsAddArrForCom());

					if (SML_LigthsCountUpdateUpdate(i, &GData::ConstCurrCamPos, k))
					{
						SML_LigthsUpdateGFrustums(i, k, &GData::ConstCurrCamPos, &GData::ConstCurrCamDir);
						SGeom_ModelsComVisible(SML_LigthsGetFrustum(i, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, k));
						SGeom_GreenComVisible(SML_LigthsGetFrustum(i, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, k));
						SXAnim_ModelsComVisible(SML_LigthsGetFrustum(i, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_ANIM, k));
					}
				}
			}
			else if (SML_LigthsGetType(i) == LTYPE_LIGHT_DIR)
			{
				if (SML_LigthsGetTypeShadowed(i) == LTYPE_SHADOW_DYNAMIC)
				{
					if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, 0) <= -1)
						SML_LigthsSetIDArr(i, RENDER_IDARRCOM_GEOM, 0, SGeom_ModelsAddArrForCom());

					if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, 0) <= -1)
						SML_LigthsSetIDArr(i, RENDER_IDARRCOM_GREEN, 0, SGeom_GreenAddArrForCom());

					if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_ANIM, 0) <= -1)
						SML_LigthsSetIDArr(i, RENDER_IDARRCOM_ANIM, 0, SXAnim_ModelsAddArrForCom());

					SGeom_ModelsComVisible(SML_LigthsGetFrustum(i, 0), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, 0));
					SGeom_GreenComVisible(SML_LigthsGetFrustum(i, 0), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, 0));
					SXAnim_ModelsComVisible(SML_LigthsGetFrustum(i, 0), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_ANIM, 0));
				}
			}
			else if (SML_LigthsGetType(i) == LTYPE_LIGHT_POINT)
			{
				if (SML_LigthsGetTypeShadowed(i) == LTYPE_SHADOW_DYNAMIC)
				{
					for (int k = 0; k < 6; k++)
					{
						ID idarr = -1;

						idarr = SGeom_ModelsAddArrForCom();
						if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, k) <= -1)
							SML_LigthsSetIDArr(i, RENDER_IDARRCOM_GEOM, k, idarr);

						if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, k) <= -1)
							SML_LigthsSetIDArr(i, RENDER_IDARRCOM_GREEN, k, SGeom_GreenAddArrForCom());

						if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_ANIM, k) <= -1)
							SML_LigthsSetIDArr(i, RENDER_IDARRCOM_ANIM, k, SXAnim_ModelsAddArrForCom());

						if (SML_LigthsGetCubeEdgeEnable(i, k))
						{
							SGeom_ModelsComVisible(SML_LigthsGetFrustum(i, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, k));
							SGeom_GreenComVisible(SML_LigthsGetFrustum(i, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, k));
							SXAnim_ModelsComVisible(SML_LigthsGetFrustum(i, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_ANIM, k));
						}
					}
				}
			}
		}
	}

	long tmpidarr = -1;
	for (long i = 0; i < SML_LigthsDelGetCount(); ++i)
	{
		if (SML_LigthsDelGetType(i) == LTYPE_LIGHT_GLOBAL)
		{
			for (int k = 0; k<4; k++)
			{
				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GEOM, k)) >= 0 && SGeom_ModelsExistsArrForCom(tmpidarr))
					SGeom_ModelsDelArrForCom(tmpidarr);

				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GREEN, k)) >= 0 && SGeom_GreenExistsArrForCom(tmpidarr))
					SGeom_GreenDelArrForCom(tmpidarr);

				if((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_ANIM, k)) >= 0)
					SXAnim_ModelsDelArrForCom(tmpidarr);
			}
		}
		else if (SML_LigthsDelGetType(i) == LTYPE_LIGHT_DIR)
		{
			if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GEOM, 0)) >= 0 && SGeom_ModelsExistsArrForCom(tmpidarr))
				SGeom_ModelsDelArrForCom(tmpidarr);

			if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GREEN, 0)) >= 0 && SGeom_GreenExistsArrForCom(tmpidarr))
				SGeom_GreenDelArrForCom(tmpidarr);

			if((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_ANIM, 0)) >= 0)
				SXAnim_ModelsDelArrForCom(tmpidarr);

		}
		else if (SML_LigthsDelGetType(i) == LTYPE_LIGHT_POINT)
		{
			for (int k = 0; k<6; k++)
			{
				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GEOM, k)) >= 0 && SGeom_ModelsExistsArrForCom(tmpidarr))
					SGeom_ModelsDelArrForCom(tmpidarr);

				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GREEN, k)) >= 0 && SGeom_GreenExistsArrForCom(tmpidarr))
					SGeom_GreenDelArrForCom(tmpidarr);

				if((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_ANIM, k)) >= 0)
					SXAnim_ModelsDelArrForCom(tmpidarr);
			}
		}

		SML_LigthsDelDel(i);
	}
}

void SXRenderFunc::ComVisibleForCamera()
{
	if (SGeom_ModelsGetCount() > 0)
		SGeom_ModelsComVisible(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);

	if (SGeom_GreenGetCount() > 0)
		SGeom_GreenComVisible(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);

	SXAnim_ModelsComVisible(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);
}

void SXRenderFunc::ComVisibleReflection()
{
	for (int i = 0; i < SGeom_ModelsGetCount(); ++i)
	{
		for (int k = 0; k < SGeom_ModelsMGetCountGroups(i); ++k)
		{
			ID idmat = SGeom_ModelsMGetGroupIDMat(i, k);
			MTLTYPE_REFLECT typeref = SML_MtlGetTypeReflection(SGeom_ModelsMGetGroupIDMat(i, k));
			float3_t center;

			if (typeref == MTLTYPE_REFLECT_PLANE)
			{
				if (!SML_MtlRefIsAllowedRender(idmat))
					continue;

				SGeom_ModelsMGetGroupCenter(i, k, &center);
				
				if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, 0) < 0)
					SML_MtlRefSetIDArr(idmat, RENDER_IDARRCOM_GEOM, 0, SGeom_ModelsAddArrForCom());

				if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, 0) < 0)
					SML_MtlRefSetIDArr(idmat, RENDER_IDARRCOM_GREEN, 0, SGeom_GreenAddArrForCom());

				if(SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_ANIM, 0) < 0)
					SML_MtlRefSetIDArr(idmat, RENDER_IDARRCOM_ANIM, 0, SXAnim_ModelsAddArrForCom());

				SGeom_ModelsComVisible(SML_MtlRefGetfrustum(idmat, 0), &float3(center), SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, 0));
				SGeom_GreenComVisible(SML_MtlRefGetfrustum(idmat, 0), &float3(center), SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, 0));
				SXAnim_ModelsComVisible(SML_MtlRefGetfrustum(idmat, 0), &float3(center), SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_ANIM, 0));
			}
			else if (typeref == MTLTYPE_REFLECT_CUBE_DYNAMIC)
			{
				SGeom_ModelsMGetGroupCenter(i, k, &center);

				if (!SML_MtlRefIsAllowedRender(idmat))
					continue;

				for (int j = 0; j<6; j++)
				{
					if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, k) < 0)
						SML_MtlRefSetIDArr(idmat, RENDER_IDARRCOM_GEOM, k, SGeom_ModelsAddArrForCom());

					if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, k) < 0)
						SML_MtlRefSetIDArr(idmat, RENDER_IDARRCOM_GREEN, k, SGeom_GreenAddArrForCom());

					if(SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_ANIM, k) < 0)
						SML_MtlRefSetIDArr(idmat, RENDER_IDARRCOM_ANIM, k, SXAnim_ModelsAddArrForCom());
					
					SGeom_ModelsComVisible(SML_MtlRefGetfrustum(idmat, j), &float3(center), SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, k));
					SGeom_GreenComVisible(SML_MtlRefGetfrustum(idmat, j), &float3(center), SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, k));
					SXAnim_ModelsComVisible(SML_MtlRefGetfrustum(idmat, j), &float3(center), SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_ANIM, k));
				}
			}
		}
	}
}

void SXRenderFunc::SaveScreenShot()
{
	char tmppath[1024];
	static int numscreen = 0;
	
	if (!Core_0FileExists(Core_RStringGet(G_RI_STRING_PATH_SCREENSHOTS)))
	{
		CreateDirectoryA(Core_RStringGet(G_RI_STRING_PATH_SCREENSHOTS), NULL);
	}

	do
	{
		++numscreen;
		sprintf(tmppath, "%sscreen_skyxengine_build_%d.jpg", Core_RStringGet(G_RI_STRING_PATH_SCREENSHOTS), numscreen);
	} while (Core_0FileExists(tmppath));

	LPDIRECT3DSURFACE9 BackBuf;
	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	D3DXSaveSurfaceToFile(tmppath, D3DXIFF_JPG, BackBuf, NULL, NULL);
	mem_release(BackBuf);
}

void SXRenderFunc::SaveWorkTex()
{
	char tmppath[1024];
	sprintf(tmppath, "%scolor.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT_COLOR), NULL);

	sprintf(tmppath, "%snormal.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT_NORMAL), NULL);

	sprintf(tmppath, "%sdepth.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT_DEPTH), NULL);

	sprintf(tmppath, "%sdepth0.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT_DEPTH0), NULL);

	sprintf(tmppath, "%sparam.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT_PARAM), NULL);

	sprintf(tmppath, "%sambient_diff.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT_AMBIENTDIFF), NULL);

	sprintf(tmppath, "%sspecular.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT_SPECULAR), NULL);


	sprintf(tmppath, "%slight_com_1.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT_SCENELIGHT), NULL);

	sprintf(tmppath, "%slight_com_2.png", Core_RStringGet(G_RI_STRING_PATH_WORKTEX));
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT_SCENELIGHT2), NULL);
}

void SXRenderFunc::InitModeWindow()
{
	static const bool *winr_windowed = GET_PCVAR_BOOL("winr_windowed");

	if (winr_windowed == NULL)
		return;

	if (!(*winr_windowed))
	{
		SetWindowLong(GData::Handle3D, GWL_STYLE, WS_POPUP);
		ShowWindow(GData::Handle3D, SW_MAXIMIZE);
	}
	else
	{
		SetWindowLong(GData::Handle3D, GWL_STYLE, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

		RECT rc;
		GetWindowRect(GData::Handle3D, &rc);
		SetWindowPos(GData::Handle3D, HWND_NOTOPMOST, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
	}
}

void SXRenderFunc::ChangeModeWindow()
{
	/*static int * winr_width = (int*)GET_PCVAR_INT("winr_width");
	*winr_width = 1280;

	static int * winr_height = (int*)GET_PCVAR_INT("winr_height");
	*winr_height = 768;

	return;*/

	static bool *winr_windowed = (bool*)GET_PCVAR_BOOL("winr_windowed");

	if (winr_windowed == NULL)
		return;

	*winr_windowed = !(*winr_windowed);

	SXRenderFunc::InitModeWindow();

	static int *resize = (int*)GET_PCVAR_INT("resize");
	*resize = RENDER_RESIZE_CHANGE;
}

//##########################################################################

void SXRenderFunc::UpdateView()
{
	GData::InitAllMatrix();

	GData::ObjCamera->GetViewMatrix(&GData::MCamView);
	
	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &GData::MCamView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &GData::MLightProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(GData::MCamView * GData::MLightProj));
	Core_RMatrixSet(G_RI_MATRIX_TRANSP_VIEWPROJ, &SMMatrixTranspose(GData::MCamView * GData::MLightProj));

	GData::ObjCamera->GetPosition(&GData::ConstCurrCamPos);
	GData::ObjCamera->GetLook(&GData::ConstCurrCamDir);

	Core_RFloat3Set(G_RI_FLOAT3_OBSERVER_POSITION, &GData::ConstCurrCamPos);
	Core_RFloat3Set(G_RI_FLOAT3_OBSERVER_DIRECTION, &GData::ConstCurrCamDir);

	Core_RMatrixSet(G_RI_MATRIX_OBSERVER_VIEW, &GData::MCamView);
	Core_RMatrixSet(G_RI_MATRIX_OBSERVER_PROJ, &GData::MCamProj);
	Core_RMatrixSet(G_RI_MATRIX_LIGHT_PROJ, &GData::MLightProj);

	Core_RFloatSet(G_RI_FLOAT_OBSERVER_NEAR, GData::NearFar.x);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FAR, GData::NearFar.y);
	Core_RFloatSet(G_RI_FLOAT_OBSERVER_FOV, GData::ProjFov);

	GData::ObjCamera->ObjFrustum->Update(&(GData::MCamView), &(GData::MCamProj));

	if (GData::DefaultGeomIDArr < 0)
		GData::DefaultGeomIDArr = SGeom_ModelsAddArrForCom();

	if (GData::DefaultGreenIDArr < 0)
		GData::DefaultGreenIDArr = SGeom_GreenAddArrForCom();

	if (GData::DefaultAnimIDArr < 0)
		GData::DefaultAnimIDArr = SXAnim_ModelsAddArrForCom();
}

void SXRenderFunc::OutputDebugInfo(DWORD timeDelta, bool needGameTime)
{
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

	static DWORD FrameCount=0;
	static float TimeElapsed=0;
	static float FpsValue = 0;
	static char FpsStr[1024];
	static char debugstr[SXGC_STR_SIZE_DBG_MSG];

	++FrameCount;
	TimeElapsed += ((float)timeDelta) * 0.001f;
	static const int * rs_stats = GET_PCVAR_INT("rs_stats");
		
		if (TimeElapsed >= 1.0f && rs_stats)
		{
			FpsValue	= (float)FrameCount / TimeElapsed;

			if ((*rs_stats) > 0)
				sprintf(debugstr, "FPS %.1f\n", FpsValue);

			if (needGameTime)
			{
				tm g_tm;
				time_t g_time = Core_TimeUnixCurrGet(Core_RIntGet(G_RI_INT_TIMER_GAME));
				localtime_s(&g_tm, &g_time);

				sprintf(debugstr + strlen(debugstr), "\nGame time : %d %d %d %d %d %d\n", 1900 + g_tm.tm_year, g_tm.tm_mon, g_tm.tm_mday, g_tm.tm_hour, g_tm.tm_min, g_tm.tm_sec);
			}

			if ((*rs_stats) == 2)
			{
				sprintf(debugstr + strlen(debugstr), "\ncount poly : %d\n", Core_RIntGet(G_RI_INT_COUNT_POLY) / FrameCount);
				sprintf(debugstr + strlen(debugstr), "count DIPs : %d\n\n", Core_RIntGet(G_RI_INT_COUNT_DIP) / FrameCount);
				sprintf(debugstr + strlen(debugstr), "Pos camera : [%.2f, %.2f, %.2f]\n", GData::ConstCurrCamPos.x, GData::ConstCurrCamPos.y, GData::ConstCurrCamPos.z);
				sprintf(debugstr + strlen(debugstr), "Dir camera : [%.2f, %.2f, %.2f]\n", GData::ConstCurrCamDir.x, GData::ConstCurrCamDir.y, GData::ConstCurrCamDir.z);

				/*sprintf(debugstr + strlen(debugstr), "\nDELAY:\n");
				sprintf(debugstr + strlen(debugstr), "\tUpdateShadow : %.3f\n", float(SXRenderFunc::Delay::UpdateShadow) / float(FrameCount) * 0.001f);
				sprintf(debugstr + strlen(debugstr), "\tRenderMRT : %.3f\n", float(SXRenderFunc::Delay::RenderMRT) / float(FrameCount) * 0.001f);
				sprintf(debugstr + strlen(debugstr), "\tComLighting : %.3f\n", float(SXRenderFunc::Delay::ComLighting) / float(FrameCount) * 0.001f);
				sprintf(debugstr + strlen(debugstr), "\tPostProcess : %.3f\n", float(SXRenderFunc::Delay::PostProcess) / float(FrameCount) * 0.001f);
				sprintf(debugstr + strlen(debugstr), "\tComReflection : %.3f\n", float(SXRenderFunc::Delay::ComReflection) / float(FrameCount) * 0.001f);
				sprintf(debugstr + strlen(debugstr), "\tGeomSortGroup : %.3f\n", float(SXRenderFunc::Delay::GeomSortGroup) / float(FrameCount) * 0.001f);
				sprintf(debugstr + strlen(debugstr), "\tUpdateParticles : %.3f\n", float(SXRenderFunc::Delay::UpdateParticles) / float(FrameCount) * 0.001f);

				sprintf(debugstr + strlen(debugstr), "\n\tUpdateVisibleFor\n");
				sprintf(debugstr + strlen(debugstr), "\t\tCamera\t: %.3f\n", float(SXRenderFunc::Delay::UpdateVisibleForCamera) / float(FrameCount) * 0.001f);
				sprintf(debugstr + strlen(debugstr), "\t\tLight\t: %.3f\n", float(SXRenderFunc::Delay::UpdateVisibleForLight) / float(FrameCount) * 0.001f);
				sprintf(debugstr + strlen(debugstr), "\t\tReflection\t: %.3f\n", float(SXRenderFunc::Delay::UpdateVisibleForReflection) / float(FrameCount) * 0.001f);

				sprintf(debugstr + strlen(debugstr), "\n\tPresent : %.3f\n", float(SXRenderFunc::Delay::Present) / float(FrameCount) * 0.001f);

				sprintf(debugstr + strlen(debugstr), "\nFreeVal : %d\n", SXRenderFunc::Delay::FreeVal);
				sprintf(debugstr + strlen(debugstr), "\nFreeValF1 : %f\n", SXRenderFunc::Delay::FreeValF1);
				sprintf(debugstr + strlen(debugstr), "\nFreeValF2 : %f\n", SXRenderFunc::Delay::FreeValF2);
				sprintf(debugstr + strlen(debugstr), "\nFreeValF3 : %f\n", SXRenderFunc::Delay::FreeValF3);*/
			}

			Core_RIntSet(G_RI_INT_COUNT_POLY, 0);
			Core_RIntSet(G_RI_INT_COUNT_DIP, 0);
			TimeElapsed		= 0.0f;
			FrameCount		= 0;

			SXRenderFunc::Delay::UpdateShadow = 0;
			SXRenderFunc::Delay::RenderMRT = 0;
			SXRenderFunc::Delay::ComLighting = 0;
			SXRenderFunc::Delay::PostProcess = 0;
			SXRenderFunc::Delay::ComReflection = 0;
			SXRenderFunc::Delay::GeomSortGroup = 0;
			SXRenderFunc::Delay::UpdateVisibleForCamera = 0;
			SXRenderFunc::Delay::UpdateVisibleForLight = 0;
			SXRenderFunc::Delay::UpdateVisibleForReflection = 0;
			SXRenderFunc::Delay::Present = 0;
		}
			
	if (rs_stats && (*rs_stats) > 0)
		SGCore_DbgMsg(debugstr);
}

//##########################################################################

void SXRenderFunc::BuildMRT(DWORD timeDelta, bool isRenderSimulation)
{
	static const int *winr_width = GET_PCVAR_INT("winr_width");
	static const int *winr_height = GET_PCVAR_INT("winr_height");
	static float2_t vWinSize;
	vWinSize = float2(*winr_width, *winr_height);

	SGCore_ShaderUnBind();

	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_MATERIAL);
	
	Core_RMatrixGet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	SXRenderFunc::SetRenderSceneFilter();

	LPDIRECT3DSURFACE9 BackBuf, ColorSurf, NormalSurf, ParamSurf, DepthMapLinearSurf;

	SML_DSGetRT(DS_RT_COLOR)->GetSurfaceLevel(0, &ColorSurf);
	SML_DSGetRT(DS_RT_NORMAL)->GetSurfaceLevel(0, &NormalSurf);
	SML_DSGetRT(DS_RT_PARAM)->GetSurfaceLevel(0, &ParamSurf);
	SML_DSGetRT(DS_RT_DEPTH)->GetSurfaceLevel(0, &DepthMapLinearSurf);

	//очищаем рт глубины  максимальным значением
	//чтобы там где нет окружения к примеру был скайбокс, а значит в рт глубины было максимальное значение - максимальная отдаленность
	GData::DXDevice->SetRenderTarget(3, DepthMapLinearSurf);
	GData::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
	GData::DXDevice->SetRenderTarget(3, 0);	//убираем рт глубины

	GData::DXDevice->GetRenderTarget(0, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, ColorSurf);
	GData::DXDevice->SetRenderTarget(1, NormalSurf);
	GData::DXDevice->SetRenderTarget(2, ParamSurf);

	GData::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, RENDER_DEFAUL_BACKGROUND_COLOR/*D3DCOLOR_ARGB(0, 0, 0, 0)*/, 1.0f, 0);
	GData::DXDevice->SetRenderTarget(3, DepthMapLinearSurf);	//ставим рт глубины

	SML_MtlNullingCurrCountSurf();
	SML_MtlSetCurrCountSurf(RENDER_LAYER_UNTRANSPARENT);


	if (!isRenderSimulation)
	{
		//SXDecals_Render();
		if (SGeom_ModelsGetCount() > 0)
			SGeom_ModelsRender(timeDelta, MTLTYPE_TRANSPARENCY_NONE);

		SXAnim_Render();

		if (SGeom_GreenGetCount() > 0)
			SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GREEN_TYPE_ALL);

		SGCore_ShaderUnBind();
	}

	//копируем глубину нулевого слоя (непрозрачной геометрии) в рт непрозрачной глубины
	//{
	//mem_release_del(DepthMapLinearSurf);
	//GData::DXDevice->SetRenderTarget(0, BackBuf);
	GData::DXDevice->SetRenderTarget(1, 0);
	GData::DXDevice->SetRenderTarget(2, 0);
	GData::DXDevice->SetRenderTarget(3, 0);

	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	LPDIRECT3DSURFACE9 DepthSurf2;
	SML_DSGetRT(DS_RT_DEPTH0)->GetSurfaceLevel(0, &DepthSurf2);

	GData::DXDevice->SetRenderTarget(0, DepthSurf2);

	GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_DEPTH));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();


	SXRenderFunc::SetRenderSceneFilter();
	//SML_DSGetRT(DS_RT_DEPTH)->GetSurfaceLevel(0, &DepthMapLinearSurf);
	GData::DXDevice->SetRenderTarget(0, ColorSurf);
	GData::DXDevice->SetRenderTarget(1, NormalSurf);
	GData::DXDevice->SetRenderTarget(2, ParamSurf);
	GData::DXDevice->SetRenderTarget(3, DepthMapLinearSurf);

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

	mem_release_del(DepthSurf2);
	//}}

	if (!isRenderSimulation)
	{
		//если есть что к отрисовке из полупрозрачной геометрии
		if (SGeom_ModelsSortExistsForRender(MTLTYPE_TRANSPARENCY_ALPHA_LIGHT))
		{
			//тут такая ситуация ... есть два рабочих варианта, причем работают чутка по разному, возможно я изработался и не могу сообразить что да как ...
			//первый вариант, чистим в 4, метим 3 раза начиная с нуля (первый раз 0, второй 1 третий 2 НЕ ИНКРЕМЕНТ а метка)
			//рисуем с сотрировкой front to back один раз, значение 1 функция D3DCMP_LESSEQUAL, с декрементом в случае успеха стенсил теста
			//но этот вариант не совсем понятно почему работает стабильно для трех ппрозрачных поверхностей, причем миганий перехода на передний план у двух (ппповерхностей) последних не наблюдалось
			//второй вариант, чистим в 0, метим 3 раза начиная с 1 (первый 1, второй 2, третий 3)
			//рисуем с сотрировкой front to back один раз, значение 0 функция D3DCMP_NOTEQUAL, с декрементом в случае успеха стенсил теста
			//второй случай логичнее, однако на двух дальних плоскостях иногда наблюдается переход одной плоскости на передний план, что слегка заметно
			//пересмотреть этот момент как будет время, а пока оставить второй вариант как наиболее логичный

			GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);
			GData::DXDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f, 0);

			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

			GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

			GData::DXDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			GData::DXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
			GData::DXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

			GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 1);
			GData::DXDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
			GData::DXDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::StencilStr);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::StencilStr, "WinSize", &vWinSize);

			SGCore_ScreenQuadDraw();

			GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
			GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 2);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::StencilColumn);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::StencilColumn, "WinSize", &vWinSize);

			SGCore_ScreenQuadDraw();

			GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
			GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 3);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::StencilStrColumn);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::StencilStrColumn, "WinSize", &vWinSize);

			SGCore_ScreenQuadDraw();

			GData::DXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
			GData::DXDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

			GData::DXDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

			GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

			SGCore_ShaderUnBind();

			SML_MtlSetForceblyAlphaTest(true);

			GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 0);
			GData::DXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
			GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);

			if (SGeom_ModelsGetCount() > 0)
			{
				SML_MtlSetIsIncrCountSurf(true);
				SML_MtlSetCurrCountSurf(RENDER_LAYER_TRANSPARENT);

				SGeom_ModelsRender(timeDelta, MTLTYPE_TRANSPARENCY_ALPHA_LIGHT, 0, true);
			}


			SML_MtlSetForceblyAlphaTest(false);
			GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

			SGCore_ShaderUnBind();

			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		}

		SXDecals_Render();
	}
	else
	{
		if (SML_MtlGetTypeTransparency(GData::Editors::SimModel->GetIDMtl()) != MTLTYPE_TRANSPARENCY_NONE)
			SML_MtlSetForceblyAlphaTest(true);
		GData::Editors::SimModel->Render(timeDelta);
		if (SML_MtlGetTypeTransparency(GData::Editors::SimModel->GetIDMtl()) != MTLTYPE_TRANSPARENCY_NONE)
			SML_MtlSetForceblyAlphaTest(false);
	}

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	GData::DXDevice->SetRenderTarget(1, 0);
	GData::DXDevice->SetRenderTarget(2, 0);
	GData::DXDevice->SetRenderTarget(3, 0);

	mem_release(BackBuf);

	mem_release(ColorSurf);
	mem_release(NormalSurf);
	mem_release(ParamSurf);
	mem_release(DepthMapLinearSurf);

	SXRenderFunc::SetRenderSceneFilterUn();
}


void SXRenderFunc::UpdateShadow(DWORD timeDelta)
{
	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_SHADOW);
	SML_LigthsComVisibleFrustumDistFor(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	SetSamplerFilter(0, D3DTEXF_LINEAR);
	SetSamplerAddress(0, D3DTADDRESS_WRAP);

	GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);

	for (int i = 0; i<SML_LigthsGetCount(); i++)
	{
		if (!SML_LigthsGetExists(i))
			continue;

		Core_RIntSet(G_RI_INT_CURRIDLIGHT, i);
		if (SML_LigthsGetShadowed(i) && (SML_LigthsComVisibleForFrustum(i, GData::ObjCamera->ObjFrustum) && SML_LigthsGetEnable(i)) /*|| (Data::Level::LightManager->Arr[i]->ShadowCube && Data::Level::LightManager->Arr[i]->ShadowCube->GetStatic() && !Data::Level::LightManager->Arr[i]->ShadowCube->GetUpdate())*/)
		{
			if (SML_LigthsGetType(i) == LTYPE_LIGHT_GLOBAL)
			{
				SML_LigthsShadowRenderBegin(i);
				int countsplits = (SML_LigthsGet4Or3SplitsG(i) ? 4 : 3);
					for (int k = 0; k<countsplits; k++)
					{
						if (SML_LigthsCountUpdateAllowed(i, k))
						{
							SML_LigthsShadowRenderPre(i, k);

							if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, k) > -1)
								SGeom_ModelsRender(timeDelta, MTLTYPE_TRANSPARENCY_NONE, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, k));

							if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, k) > -1)
								SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GREEN_TYPE_TREE, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, k));

							SXAnim_Render();
						}
					}

				//КОГДА ИСТОЧНИК БЛИЗОК К ГОРИЗОНТУ ИЗ-ЗА ОБЛАКОВ ВОЗНИКАЕТ БАГ С ТЕНЯМИ В ВИДЕ ФЕЙКОВЫХ ТЕНЕЙ
				if (SGCore_SkyCloudsIsCr())
				{
					SML_LigthsUpdateGFrustums(i, 4, &GData::ConstCurrCamPos, &GData::ConstCurrCamDir);
					SML_LigthsShadowRenderPre(i, 4);
					GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
					GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
					SetSamplerFilter(0, D3DTEXF_LINEAR);
					SetSamplerFilter(1, D3DTEXF_LINEAR);
					SetSamplerAddress(0, D3DTADDRESS_MIRROR);
					SetSamplerAddress(1, D3DTADDRESS_MIRROR);

					if (SGCore_SkyCloudsIsLoadTex())
						SGCore_SkyCloudsRender(timeDelta, &float3(GData::ConstCurrCamPos.x, GData::ConstCurrCamPos.y + 150, GData::ConstCurrCamPos.z), true);
					else
						GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);

					SetSamplerAddress(0, D3DTADDRESS_WRAP);
					SetSamplerAddress(1, D3DTADDRESS_WRAP);
				}
				SML_LigthsShadowRenderEnd(i);
			}
			else if (SML_LigthsGetType(i) == LTYPE_LIGHT_DIR)
			{
				if (SML_LigthsCountUpdateUpdate(i, &GData::ConstCurrCamPos))
				{
					SML_LigthsShadowRenderBegin(i);

					if (SML_LigthsGetTypeShadowed(i) == LTYPE_SHADOW_DYNAMIC)
					{
						if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, 0) > -1)
							SGeom_ModelsRender(timeDelta, MTLTYPE_TRANSPARENCY_NONE, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, 0));
							
						if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, 0) > -1)
							SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GREEN_TYPE_TREE, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, 0));

						SXAnim_Render();
					}
					else
					{
						SGeom_ModelsComVisible(SML_LigthsGetFrustum(i, 0), &GData::ConstCurrCamPos, GData::DefaultGeomIDArr);
						SGeom_ModelsRender(timeDelta, -1, GData::DefaultGeomIDArr);
						SGeom_GreenComVisible(SML_LigthsGetFrustum(i, 0), &GData::ConstCurrCamPos, GData::DefaultGreenIDArr);
						SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GREEN_TYPE_TREE, GData::DefaultGreenIDArr);
					}

					SML_LigthsShadowRenderEnd(i);
				}
			}
			else if (SML_LigthsGetType(i) == LTYPE_LIGHT_POINT)
			{
				if (SML_LigthsGetTypeShadowed(i) != LTYPE_SHADOW_NONE && SML_LigthsCountUpdateUpdate(i, &GData::ConstCurrCamPos))
				{
					SML_LigthsShadowRenderBegin(i);

					for (int k = 0; k < 6; k++)
					{
						if (SML_LigthsGetCubeEdgeEnable(i, k))
						{
							SML_LigthsShadowRenderPre(i, k);

							if (SML_LigthsGetTypeShadowed(i) == LTYPE_SHADOW_DYNAMIC)
							{
								if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, k) > -1)
									SGeom_ModelsRender(timeDelta, MTLTYPE_TRANSPARENCY_NONE, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GEOM, k));

								if (SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, k) > -1)
									SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GREEN_TYPE_TREE, SML_LigthsGetIDArr(i, RENDER_IDARRCOM_GREEN, k));

								SXAnim_Render();
							}
							else
							{
								SGeom_ModelsComVisible(SML_LigthsGetFrustum(i, k), &GData::ConstCurrCamPos, GData::DefaultGeomIDArr);
								SGeom_ModelsRender(timeDelta, -1, GData::DefaultGeomIDArr);
								SGeom_GreenComVisible(SML_LigthsGetFrustum(i, k), &GData::ConstCurrCamPos, GData::DefaultGreenIDArr);
								SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GREEN_TYPE_TREE, GData::DefaultGreenIDArr);
							}
							
							SML_LigthsShadowRenderPost(i, k);
						}
					}

					SML_LigthsShadowRenderEnd(i);
				}
			}
		}
	}
	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_FREE);
	GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

void SXRenderFunc::RenderSky(DWORD timeDelta)
{
	LPDIRECT3DSURFACE9 ColorSurf, BackBuf;
	SML_DSGetRT(DS_RT_SCENELIGHT)->GetSurfaceLevel(0, &ColorSurf);
	GData::DXDevice->GetRenderTarget(0, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, ColorSurf);

	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, RENDER_DEFAUL_BACKGROUND_COLOR, 1.0f, 0);

	SetSamplerFilter(0, 2, D3DTEXF_ANISOTROPIC);

	if (SGCore_SkyBoxIsCr() && SGCore_SkyBoxIsLoadTex())
	{
		GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		SetSamplerAddress(0, 2, D3DTADDRESS_CLAMP);
		SGCore_SkyBoxRender(timeDelta, &float3(GData::ConstCurrCamPos.x, GData::ConstCurrCamPos.y + 40, GData::ConstCurrCamPos.z));
	}

	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (SGCore_SkyCloudsIsCr() && SGCore_SkyCloudsIsLoadTex())
	{
		SetSamplerAddress(0, 2, D3DTADDRESS_MIRROR);
		SGCore_SkyCloudsRender(timeDelta, &float3(GData::ConstCurrCamPos.x, GData::ConstCurrCamPos.y + 150, GData::ConstCurrCamPos.z), false);
	}

	GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_SCENELIGHT2));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(ColorSurf);

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);

	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void SXRenderFunc::ComLighting(DWORD timeDelta)
{
	static int *winr_width = (int*)GET_PCVAR_INT("winr_width");
	static int *winr_height = (int*)GET_PCVAR_INT("winr_height");

	SGCore_ShaderUnBind();

	LPDIRECT3DSURFACE9 AmbientSurf, SpecDiffSurf, BackBuf;
	SML_DSGetRT(DS_RT_AMBIENTDIFF)->GetSurfaceLevel(0, &AmbientSurf);
	SML_DSGetRT(DS_RT_SPECULAR)->GetSurfaceLevel(0, &SpecDiffSurf);

	GData::DXDevice->GetRenderTarget(0, &BackBuf);

	GData::DXDevice->SetRenderTarget(0, AmbientSurf);
	GData::DXDevice->SetRenderTarget(1, SpecDiffSurf);

	//очищаем рт и стенсил
	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	//устанавка аддитивного смешивания
	//когда к уже записанному будет прибавляться то что хотим записать
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	GData::DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	GData::DXDevice->SetTransform(D3DTS_WORLD, &((D3DXMATRIX)SMMatrixIdentity()));
	GData::DXDevice->SetTransform(D3DTS_VIEW, &((D3DXMATRIX)GData::MCamView));
	GData::DXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)GData::MLightProj));

	//проходимся циклом по всем источникам света
	for (int i = 0; i<SML_LigthsGetCount(); i++)
	{
		if (!SML_LigthsGetExists(i))
			continue;

		//если свет виден фрустуму камеры (это надо было заранее просчитать) и если свет включен
		if (SML_LigthsGetVisibleForFrustum(i) && SML_LigthsGetEnable(i))
		{
			//пока что назначаем шейдер без теней
			ID idshader = GData::IDsShaders::PS::ComLightingNonShadow;

			//если не глобальный источник
			if (SML_LigthsGetType(i) != LTYPE_LIGHT_GLOBAL)
			{
				//помечаем в стенсил буфере пиксели  которые входят в ограничивающий объем света, чтобы их осветить

				//отключаем вывод цвета
				GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);

				//установка стенсил теста, причем и двухстороннего тоже
				GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
				GData::DXDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);

				//вклчить тест глубины, но запись выключить, установить стандартную функцию проверки глубины
				GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
				GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
				GData::DXDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

				//стенсил тест проходит всегда удачно, при провале теста глубины инкрементируем значение в стенсиле
				GData::DXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
				GData::DXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
				//при удачно проходе, игнорируем
				GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

				//стенсил тест для обратной глубины проходит всегда удачно, при провале теста глубины декрементируем значение в стенсиле
				GData::DXDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
				GData::DXDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_DECR);
				//при удачно проходе, игнорируем
				GData::DXDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP);

				//установка значений для записи
				GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 0x0);
				GData::DXDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
				GData::DXDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
				GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//включение двухсторонней отрисовки

				GData::DXDevice->SetTransform(D3DTS_WORLD, &((D3DXMATRIX)SMMatrixIdentity()));
				GData::DXDevice->SetTransform(D3DTS_VIEW, &((D3DXMATRIX)GData::MCamView));
				GData::DXDevice->SetTransform(D3DTS_PROJECTION, &((D3DXMATRIX)GData::MLightProj));

				//отрисовка ограничивающего объема
				SML_LigthsRender(i, 0);

				//
				GData::DXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
				GData::DXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);
				GData::DXDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);
				GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

				GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 255);

				//включаем вывод цвета
				GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
			}
			else
			{
				//иначе это глобальный источник, отключаем стенсил тест
				GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
				GData::DXDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
			}

			//отключаем тест глубины ибо будем теперь пост процессом обрабатывать полученные данные
			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

			GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			//если свет отбрасывает тени
			if (SML_LigthsGetShadowed(i))
			{
				//генерация теней для текущего света
				//{{
				//так как нам нужно провести очистку рт то убираем оба рт
				GData::DXDevice->SetRenderTarget(0, 0);
				GData::DXDevice->SetRenderTarget(1, 0);

				//отключаем смешивание, нам не нужен хлам в рт
				GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
				GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);

				SML_LigthsShadowNull();	//очищаем рт генерации теней
				SML_LigthsShadowGen(i);	//генерируем тень для света

				static const int * shadow_soft = GET_PCVAR_INT("shadow_soft");

				if (shadow_soft)
				{
					if ((*shadow_soft) == 1)
						SML_LigthsShadowSoft(false, 2);
					else if ((*shadow_soft) == 2)
					{
						SML_LigthsShadowSoft(false, 2);
						SML_LigthsShadowSoft(false, 2);
					}
				}

				GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
				//}}
			
				//включаем смешивание для освещения
				GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

				//опять назначаем второй рт
				GData::DXDevice->SetRenderTarget(0, AmbientSurf);
				GData::DXDevice->SetRenderTarget(1, SpecDiffSurf);

				//устанавливаем текстуру с тенями и переназначаем шейдер, теперь уже с тенями
				GData::DXDevice->SetTexture(4, SML_LigthsGetShadow());
				idshader = GData::IDsShaders::PS::ComLightingShadow;
			}

			SGCore_ShaderUnBind();

			//теперь когда будем считать освещение надо сбросить значения в стенсил буфере, чтобы каждый кадр не чистить
			//если стенсил тест прошел успешно, устанавливаем значнеие в нуль
			if (SML_LigthsGetType(i) != LTYPE_LIGHT_GLOBAL)
				GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);

			float determ = 0;
			float4x4 ViewInv = SMMatrixInverse(&determ, GData::MCamView);
			ViewInv = SMMatrixTranspose(ViewInv);

			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ResPos, "ViewInv", &ViewInv);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ResPos, "NearFar", &GData::NearFar);
			SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ResPos, "ParamProj", &float3_t(*winr_width, *winr_height, GData::ProjFov));

			float3 tmpPosition;
			float2 tmpPowerDist;
			float3 tmpColor;
			SML_LigthsGetColor(i, &tmpColor);
			SML_LigthsGetPos(i, &tmpPosition, true);
			tmpPowerDist.x = SML_LigthsGetPower(i);
			tmpPowerDist.y = SML_LigthsGetDist(i);
			
			if (SML_LigthsGetType(i) != LTYPE_LIGHT_GLOBAL)
			{
				tmpColor.w = 0.f;
				ID gl_id = -1;
				if ((gl_id = SML_LigthsGetGlobal()) >= 0)
				{
					float gl_power = 0.f;
					if (SML_LigthsGetEnable(gl_id))
						gl_power = SML_LigthsGetPower(gl_id);

					float f_dep_coef = clampf(1.f - gl_power, 0.25f, 1.f);
					tmpPowerDist.x *= f_dep_coef;
					tmpPowerDist.y *= f_dep_coef;
				}
			}
			else
				tmpColor.w = 1.f;

			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "ViewPos", &GData::ConstCurrCamPos);
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "LightPos", &(tmpPosition));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "LightPowerDist", &(tmpPowerDist));
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "LightColor", &tmpColor);
			//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idshader, "NearFar", &GData::NearFar);

			SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ResPos);
			SGCore_ShaderBind(SHADER_TYPE_PIXEL, idshader);

			SetSamplerFilter(0, 5, D3DTEXF_NONE);
			SetSamplerAddress(0, 5, D3DTADDRESS_CLAMP);
			
			GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_COLOR));
			GData::DXDevice->SetTexture(1, SML_DSGetRT(DS_RT_NORMAL));
			GData::DXDevice->SetTexture(2, SML_DSGetRT(DS_RT_PARAM));
			GData::DXDevice->SetTexture(3, SML_DSGetRT(DS_RT_DEPTH));
			GData::DXDevice->SetTexture(5, SML_DSGetRT(DS_RT_ADAPTEDLUM));

			SGCore_ScreenQuadDraw();

			SGCore_ShaderUnBind();
		}
	}

	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	GData::DXDevice->SetRenderTarget(1, 0);

	mem_release(BackBuf);

	mem_release(AmbientSurf);
	mem_release(SpecDiffSurf);

	//-------------------------------
	
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	
	//теперь необходимо все смешать чтобы получить итоговую освещенную картинку
	//{{
	SetSamplerFilter(0, 5, D3DTEXF_NONE);
	SetSamplerAddress(0, 5, D3DTADDRESS_CLAMP);
	LPDIRECT3DSURFACE9 ComLightSurf;

	SML_DSGetRT(DS_RT_SCENELIGHT)->GetSurfaceLevel(0, &ComLightSurf);

	GData::DXDevice->GetRenderTarget(0, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, ComLightSurf);

	//очищаем рт (в старой версии было многопроходное смешивание)
	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

	GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_COLOR));
	GData::DXDevice->SetTexture(1, SML_DSGetRT(DS_RT_AMBIENTDIFF));
	GData::DXDevice->SetTexture(2, SML_DSGetRT(DS_RT_SPECULAR));
	GData::DXDevice->SetTexture(3, SML_DSGetRT(DS_RT_NORMAL));
	GData::DXDevice->SetTexture(4, SML_DSGetRT(DS_RT_ADAPTEDLUM));
	GData::DXDevice->SetTexture(5, SML_DSGetRT(DS_RT_PARAM));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::BlendAmbientSpecDiffColor);
	
	SGCore_ScreenQuadDraw();

	mem_release(ComLightSurf);
	//}}

	SGCore_ShaderUnBind();

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);
}

void SXRenderFunc::UnionLayers()
{
	static const int *winr_width = GET_PCVAR_INT("winr_width");
	static const int *winr_height = GET_PCVAR_INT("winr_height");

	LPDIRECT3DSURFACE9 BackBuf, ComLightSurf;
	GData::DXDevice->GetRenderTarget(0, &BackBuf);

	static int CurrCountTransparencySurf;

#if defined(SXMATERIAL_EDITOR)
	CurrCountTransparencySurf = 3;
#else
	CurrCountTransparencySurf = SML_MtlGetCurrCountSurf();
#endif

	if (CurrCountTransparencySurf >= RENDER_LAYER_TRANSPARENT)
	{
		LPDIRECT3DSURFACE9 ColorSurf, Color2Surf, DepthSurf;
		SML_DSGetRT(DS_RT_SCENELIGHT2)->GetSurfaceLevel(0, &ColorSurf);
		GData::DXDevice->SetRenderTarget(0, ColorSurf);
		SML_DSGetRT(DS_RT_DEPTH0)->GetSurfaceLevel(0, &DepthSurf);
		GData::DXDevice->SetRenderTarget(1, DepthSurf);
		SML_DSGetRT(DS_RT_DEPTH1)->GetSurfaceLevel(0, &Color2Surf);
		GData::DXDevice->SetRenderTarget(2, Color2Surf);
		GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_COLOR));
		GData::DXDevice->SetTexture(1, SML_DSGetRT(DS_RT_SCENELIGHT));
		GData::DXDevice->SetTexture(2, SML_DSGetRT(DS_RT_DEPTH));
		GData::DXDevice->SetTexture(3, SML_DSGetRT(DS_RT_NORMAL));
		GData::DXDevice->SetTexture(4, SML_DSGetRT(DS_RT_PARAM));

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::UnionAlpha);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::UnionAlpha, "WinSize", &float4_t(*winr_width, *winr_height, 1.f / float(*winr_width), 1.f / float(*winr_height)));

		SGCore_ScreenQuadDraw();

		GData::DXDevice->SetRenderTarget(1, 0);
		GData::DXDevice->SetRenderTarget(2, 0);
		mem_release(ColorSurf);
		mem_release(DepthSurf);
		mem_release(Color2Surf);
	}
	else
	{
		//копируем значения только для ds_rt_depth1, а ds_rt_depth0 уже заполнен

		LPDIRECT3DSURFACE9 DepthSurf1;

		SML_DSGetRT(DS_RT_DEPTH1)->GetSurfaceLevel(0, &DepthSurf1);
		GData::DXDevice->SetRenderTarget(0, DepthSurf1);

		GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_DEPTH));

		SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::ScreenOut);

		SGCore_ScreenQuadDraw();

		GData::DXDevice->SetRenderTarget(0, 0);
		mem_release(DepthSurf1);
	}

	//копируем итоговую сцену
	SML_DSGetRT((CurrCountTransparencySurf >= RENDER_LAYER_TRANSPARENT ? DS_RT_SCENELIGHT : DS_RT_SCENELIGHT2))->GetSurfaceLevel(0, &ComLightSurf);
	GData::DXDevice->SetRenderTarget(0, ComLightSurf);
	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	GData::DXDevice->SetTexture(0, SML_DSGetRT((CurrCountTransparencySurf >= RENDER_LAYER_TRANSPARENT ? DS_RT_SCENELIGHT2 : DS_RT_SCENELIGHT)));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	mem_release(ComLightSurf);

	SGCore_ShaderUnBind();

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);
}

void SXRenderFunc::ApplyToneMapping()
{
	LPDIRECT3DSURFACE9 BackBuf, ComLightSurf;
	SML_DSGetRT(DS_RT_SCENELIGHT2)->GetSurfaceLevel(0, &ComLightSurf);
	GData::DXDevice->GetRenderTarget(0, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, ComLightSurf);

	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, RENDER_DEFAUL_BACKGROUND_COLOR, 1.0f, 0);

	SetSamplerFilter(0, 5, D3DTEXF_NONE);
	SetSamplerAddress(0, 5, D3DTADDRESS_CLAMP);

	GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_SCENELIGHT));
	GData::DXDevice->SetTexture(1, SML_DSGetRT(DS_RT_ADAPTEDLUM));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::ToneMapping);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(ComLightSurf);



	SML_DSGetRT(DS_RT_SCENELIGHT)->GetSurfaceLevel(0, &ComLightSurf);
	GData::DXDevice->SetRenderTarget(0, ComLightSurf);
	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_SCENELIGHT2));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, GData::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	mem_release(ComLightSurf);

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);


	SGCore_ShaderUnBind();
}

void SXRenderFunc::ComToneMapping(DWORD timeDelta)
{
	//обработка tone mapping
	static const float * hdr_adapted_coef = GET_PCVAR_FLOAT("hdr_adapted_coef");
	SML_LigthsToneMappingCom(timeDelta, (hdr_adapted_coef ? (*hdr_adapted_coef) : 0.03f));
}



void SXRenderFunc::RenderParticles(DWORD timeDelta)
{
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHAREF, RENDER_PARTICLES_ALPHATEST_VALUE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	SetSamplerFilter(0, 3, D3DTEXF_LINEAR);
	SetSamplerAddress(0, 3, D3DTADDRESS_WRAP);

	SPE_EffectRenderAll(timeDelta);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
}

void SXRenderFunc::RenderPostProcess(DWORD timeDelta)
{
	static const int *winr_width = GET_PCVAR_INT("winr_width");
	static const int *winr_height = GET_PCVAR_INT("winr_height");
	static float2_t vWinSize;
	vWinSize = float2(*winr_width, *winr_height);

	SPP_RTNull();

	static const int * pp_ssao = GET_PCVAR_INT("pp_ssao");
	if (pp_ssao && (*pp_ssao) > 0)
		SPP_RenderSSAO(&float4_t(0.3f, 0.1f, 0.8f, 0.3f / GData::NearFar.y), (*pp_ssao));

	//создаем статический вектор цвета тумана, затем получаем квар который int типа который будет содеражть указатель на этот вектор, и в него записываем указатель на вектор цвета тумана
	//static float3_t fog_color(0.5, 0.5, 0.5);
	static UINT_PTR *pp_fog_color = GET_PCVAR_POINTER("pp_fog_color");
	static float3_t *fog_color; 
	fog_color = (float3_t*)(*pp_fog_color);

	static const float * pp_fog_density = GET_PCVAR_FLOAT("pp_fog_density");
	static const float * pp_fog_sky = GET_PCVAR_FLOAT("pp_fog_sky");
	static const float * pp_fog_min = GET_PCVAR_FLOAT("pp_fog_min");
	static const float * pp_fog_max = GET_PCVAR_FLOAT("pp_fog_max");

	if (pp_fog_density && *pp_fog_density > 0.f && pp_fog_sky && pp_fog_min && pp_fog_max)
		SPP_RenderFogLinear(fog_color, &float4_t(*pp_fog_density, *pp_fog_sky, *pp_fog_min, *pp_fog_max));
	//SPP_RenderWhiteBlack(1);

	static const bool * pp_bloom = GET_PCVAR_BOOL("pp_bloom");
	if (pp_bloom && (*pp_bloom))
		SPP_RenderBloom(&float3_t(1, 0.7, 0.1));

	SPP_Update();

	float3 tmpPosition;
	float3 tmpColor;

	ID GlobalLight = SML_LigthsGetGlobal();
	if (GlobalLight > -1)
	{
		SML_LigthsGetColor(GlobalLight, &tmpColor);
		SML_LigthsGetPos(GlobalLight, &tmpPosition, true);

		SPP_UpdateSun((SML_LigthsGetEnable(GlobalLight) ? &tmpPosition : 0));

		if (!SML_LigthsGetCastGlobalShadow())
			SPP_RenderSun(&float4_t(tmpColor.x, tmpColor.y, tmpColor.z, SML_LigthsGetPower(GlobalLight)));
	}
	else
		SPP_UpdateSun(0);

	static const bool * pp_lensflare = GET_PCVAR_BOOL("pp_lensflare");
	static const bool * pp_lensflare_usebloom = GET_PCVAR_BOOL("pp_lensflare_usebloom");
	if (pp_lensflare && (*pp_lensflare))
		SPP_RenderLensFlare(&float3_t(0.25f, 0.3f, 0.9f), &float4_t(tmpColor.x, tmpColor.y, tmpColor.z, (SML_LigthsGetCastGlobalShadow() ? 0 : SML_LigthsGetPower(GlobalLight))), (pp_lensflare_usebloom ? (*pp_lensflare_usebloom) : false));


	SPP_RenderDOF(&float4_t(0, 200, 0, 100), 0);

	static const bool * pp_dlaa = GET_PCVAR_BOOL("pp_dlaa");
	if (pp_dlaa && (*pp_dlaa))
		SPP_RenderDLAA();

	static const bool * pp_nfaa = GET_PCVAR_BOOL("pp_dlaa");
	if (pp_nfaa && (*pp_nfaa))
		SPP_RenderNFAA(&float3_t(1, 1, 0));

	static const bool * pp_motionblur = GET_PCVAR_BOOL("pp_motionblur");
	static const float * pp_motionblur_coef = GET_PCVAR_FLOAT("pp_motionblur_coef");
	if (pp_motionblur && (*pp_motionblur))
		SPP_RenderMotionBlur((pp_motionblur_coef ? (*pp_motionblur_coef) : 0.1), timeDelta);
}

void SXRenderFunc::ShaderRegisterData()
{
	static float4_t tmpnull[256];
	memset(tmpnull, 0, sizeof(float4_t)* 256);
	GData::DXDevice->SetVertexShaderConstantF(0, (float*)&tmpnull, 256);
	GData::DXDevice->SetPixelShaderConstantF(0, (float*)&tmpnull, 256);
}


void SXRenderFunc::UpdateReflectionScene(DWORD timeDelta)
{
	for (int i = 0; i < SGeom_ModelsGetCount(); ++i)
	{
		for (int k = 0; k < SGeom_ModelsMGetCountGroups(i); ++k)
		{
			ID idmat = SGeom_ModelsMGetGroupIDMat(i, k);
			MTLTYPE_REFLECT typeref = SML_MtlGetTypeReflection(SGeom_ModelsMGetGroupIDMat(i, k));
			D3DXPLANE plane;
			float3_t center;
			if (typeref == MTLTYPE_REFLECT_PLANE)
			{
				if (!SML_MtlRefIsAllowedRender(idmat))
				{
					SML_MtlRefUpdateCountUpdate(idmat, &((float3_t)GData::ConstCurrCamPos));
					continue;
				}

				SGeom_ModelsMGetGroupPlane(i, k, &plane);
				SGeom_ModelsMGetGroupCenter(i, k, &center);
				SML_MtlRefPreRenderPlane(idmat, &plane);
				SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
				SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);


				Core_RBoolSet(G_RI_BOOL_CLIPPLANE0, true);

				Core_RFloat3Set(G_RI_FLOAT3_CLIPPLANE0_NORMAL, &float3(plane.a, plane.b, plane.c));
				Core_RFloat3Set(G_RI_FLOAT3_CLIPPLANE0_POINT, &float3(center));

				if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, 0) >= 0)
					SGeom_ModelsRender(timeDelta, MTLTYPE_TRANSPARENCY_NONE, SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, 0), false, i, k);

				if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, 0) >= 0)
					SGeom_GreenRender(timeDelta, &float3(center), GREEN_TYPE_ALL, SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, 0));

				SXAnim_Render();

				SGCore_ShaderUnBind();


				GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

				GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
				GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

				GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
				GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

				GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
				GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
				Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &GData::MRefPlaneSkyProj);
				SGCore_SkyBoxRender(timeDelta, &float3(center));
				GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
				GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
				GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

				SML_MtlRefPostRenderPlane(idmat);
			}
			else if (typeref == MTLTYPE_REFLECT_CUBE_STATIC || typeref == MTLTYPE_REFLECT_CUBE_DYNAMIC)
			{
				SGeom_ModelsMGetGroupCenter(i, k, &center);
				float3_t min, max;
				SGeom_ModelsMGetGroupMin(i, k, &min);
				SGeom_ModelsMGetGroupMax(i, k, &max);
				SML_MtlRefSetMinMax(idmat, &min, &max);

				if (!SML_MtlRefIsAllowedRender(idmat))
				{
					SML_MtlRefUpdateCountUpdate(idmat, &((float3_t)GData::ConstCurrCamPos));
					continue;
				}

				SML_MtlRefCubeBeginRender(idmat, &center);

				for (int j = 0; j<6; j++)
				{
					Core_RMatrixGet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

					SML_MtlRefCubePreRender(idmat, j, &(SMMatrixIdentity()));
					SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
					SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

					//если статические кубические отражения
					if (SML_MtlGetTypeReflection(idmat) == MTLTYPE_REFLECT_CUBE_STATIC)
					{
						//тогда считаем в массив камеры
						SGeom_ModelsComVisible(SML_MtlRefGetfrustum(idmat, j), &float3(center), GData::DefaultGeomIDArr);
						SGeom_ModelsRender(timeDelta, MTLTYPE_TRANSPARENCY_NONE, GData::DefaultGeomIDArr, false, i, k);

						SGeom_GreenComVisible(SML_MtlRefGetfrustum(idmat, j), &float3(center), GData::DefaultGreenIDArr);
						SGeom_GreenRender(timeDelta, &float3(center), GREEN_TYPE_ALL, GData::DefaultGreenIDArr);
					}
					else
					{
						if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, k) >= 0)
							SGeom_ModelsRender(timeDelta, MTLTYPE_TRANSPARENCY_NONE, SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, k), false, i, k);

						if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, k) >= 0)
							SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GREEN_TYPE_ALL, SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, k));

						SXAnim_Render();
					}

					GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

					GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
					GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

					GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
					GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

					GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
					GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

					GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					SetSamplerAddress(0, 2, D3DTADDRESS_CLAMP);

					Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &GData::MRefCubeSkyProj);

					SGCore_SkyBoxRender(timeDelta, &float3(center));
					GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

					SML_MtlRefCubePostRender(idmat, j);

					GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
					GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
				}
				SML_MtlRefCubeEndRender(idmat, &((float3_t)GData::ConstCurrCamPos));
			}
		}
	}
}

void SXRenderFunc::UpdateReflectionSimModel(DWORD timeDelta)
{
	ID idmat = GData::Editors::SimModel->GetIDMtl();
	MTLTYPE_REFLECT typeref = SML_MtlGetTypeReflection(idmat);
	D3DXPLANE plane;
	float3_t center;
	GData::Editors::SimModel->GetCenter(&center);
	SGCore_ShaderUnBind();

	if (typeref == MTLTYPE_REFLECT_PLANE)
	{
		GData::Editors::SimModel->GetPlane(&plane);
		SML_MtlRefPreRenderPlane(idmat, &plane);
		SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
		SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

		GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &GData::MRefPlaneSkyProj);
		SGCore_SkyBoxRender(timeDelta, &float3(center));
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

		SML_MtlRefPostRenderPlane(idmat);
	}
	else if (typeref == MTLTYPE_REFLECT_CUBE_STATIC || typeref == MTLTYPE_REFLECT_CUBE_DYNAMIC)
	{
		SML_MtlRefCubeBeginRender(idmat, &center);

		for (int j = 0; j<6; j++)
		{
			Core_RMatrixGet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

			SML_MtlRefCubePreRender(idmat, j, &(SMMatrixIdentity()));

			GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
			Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &GData::MRefCubeSkyProj);
			SGCore_SkyBoxRender(timeDelta, &float3(center));

			GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
			SML_MtlRefCubePostRender(idmat, j);
			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		}
		SML_MtlRefCubeEndRender(idmat, &((float3_t)GData::ConstCurrCamPos));
	}
}

void SXRenderFunc::UpdateReflection(DWORD timeDelta, bool isRenderSimulation)
{
	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_FREE);
	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &GData::MCamView);

	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

	if (!isRenderSimulation)
		SXRenderFunc::UpdateReflectionScene(timeDelta);
	else
		SXRenderFunc::UpdateReflectionSimModel(timeDelta);

}

//##########################################################################

void SXRenderFunc::RenderEditorMain()
{
	if (GData::Editors::ObjGrid && GData::Editors::RenderGrid)
	{
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		GData::Editors::ObjGrid->render();
	}

	if (GData::Editors::ObjAxesStatic && GData::Editors::RenderAxesStatic)
	{
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		GData::Editors::ObjAxesStatic->render();
	}
}
