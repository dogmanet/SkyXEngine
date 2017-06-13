
#include <managed_render\\render_func.h>

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
		for (DWORD i = begin_id; i<end_id; i++)
			SXRenderFunc::SetSamplerFilter(i, value);
	}
}

inline void SXRenderFunc::SetSamplerAddress(DWORD begin_id, DWORD end_id, DWORD value)
{
	if (begin_id >= 0 && end_id <= 16)
	{
		for (DWORD i = begin_id; i<end_id; i++)
			SXRenderFunc::SetSamplerAddress(i, value);
	}
}

//////

void SXRenderFunc::ComDeviceLost()
{
	if (GData::ReSize != 2)
	{
		//получаем текущий размер окна в которое рисовали
		RECT rect_scene;
		GetClientRect(GData::Handle3D, &rect_scene);


		GData::WinSize.x = rect_scene.right;
		GData::WinSize.y = rect_scene.bottom;
	}

	//сбрасываем все что необходимо для восстановления устройства
	SGCore_OnLostDevice();
	SGeom_OnLostDevice();
	SML_OnLostDevice();
	SPE_OnLostDevice();

	bool bf = SGCore_OnDeviceReset(GData::WinSize.x, GData::WinSize.y,GData::IsWindowed);
		if (bf)
		{
			//если всетаки функция зашла сюда значит чтото было неосвобождено
			printflog(REPORT_MSG_LEVEL_ERROR, "reset device is failed ... \n");
		}
		else
		{
			GData::InitAllMatrix();
			GData::ReSize = 0;
			SGCore_OnResetDevice();
			SML_OnResetDevice(GData::WinSize.x, GData::WinSize.y, GData::ProjFov);
			SGeom_OnResetDevice();
			SPE_OnResetDevice();


			GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		}
}

void SXRenderFunc::ComVisibleForLight()
{
	for (int i = 0; i<SML_LigthsGetCount(); i++)
	{
		long tmpid = SML_LigthsGetIDOfKey(i);
		if (SML_LigthsIsShadow(tmpid) && (SML_LigthsComVisibleForFrustum(tmpid, GData::ObjCamera->ObjFrustum) && SML_LigthsIsEnable(tmpid)) /*|| (Data::Level::LightManager->Arr[i]->ShadowCube && Data::Level::LightManager->Arr[i]->ShadowCube->GetStatic() && !Data::Level::LightManager->Arr[i]->ShadowCube->GetUpdate())*/)
		{
			if (SML_LigthsGetType(tmpid) == LightsTypeLight::ltl_global)
			{
				for (int k = 0; k<4; k++)
				{
					if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k, SGeom_ModelsAddArrForCom());

					if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k, SGeom_GreenAddArrForCom());

					if(SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_ANIM, k) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_ANIM, k, SXAnim_ModelsAddArrForCom());

					if (SML_LigthsUpdateCountUpdate(tmpid, &GData::ConstCurrCamPos, k))
					{
						SML_LigthsUpdateFrustumsG(tmpid, k, &GData::ConstCurrCamPos, &GData::ConstCurrCamDir);
						SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k));
						SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k));
						SXAnim_ModelsComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_ANIM, k));
					}
				}
			}
			else if (SML_LigthsGetType(tmpid) == LightsTypeLight::ltl_direction)
			{
				if (SML_LigthsGetTypeShadowed(tmpid) == LightsTypeShadow::lts_dynamic)
				{
					if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0, SGeom_ModelsAddArrForCom());

					if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0, SGeom_GreenAddArrForCom());

					if(SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_ANIM, 0) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_ANIM, 0, SXAnim_ModelsAddArrForCom());

					SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, 0), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0));
					SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, 0), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0));
					SXAnim_ModelsComVisible(SML_LigthsGetFrustum(tmpid, 0), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_ANIM, 0));
				}
			}
			else if (SML_LigthsGetType(tmpid) == LightsTypeLight::ltl_point)
			{
				if (SML_LigthsGetTypeShadowed(tmpid) == LightsTypeShadow::lts_dynamic)
				{
					for (int k = 0; k < 6; k++)
					{
						if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k) <= -1)
							SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k, SGeom_ModelsAddArrForCom());

						if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k) <= -1)
							SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k, SGeom_GreenAddArrForCom());

						if(SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_ANIM, k) <= -1)
							SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_ANIM, k, SXAnim_ModelsAddArrForCom());

						if (SML_LigthsGetEnableCubeEdge(tmpid, k))
						{
							SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k));
							SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k));
							SXAnim_ModelsComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_ANIM, k));
						}
					}
				}
			}
		}
	}

	long tmpidarr = -1;
	for (long i = 0; i < SML_LigthsDelGetCount(); ++i)
	{
		if (SML_LigthsDelGetType(i) == LightsTypeLight::ltl_global)
		{
			for (int k = 0; k<4; k++)
			{
				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GEOM, k)) >= 0)
					SGeom_ModelsDelArrForCom(tmpidarr);

				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GREEN, k)) >= 0)
					SGeom_GreenDelArrForCom(tmpidarr);

				if((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_ANIM, k)) >= 0)
					SXAnim_ModelsDelArrForCom(tmpidarr);
			}
		}
		else if (SML_LigthsDelGetType(i) == LightsTypeLight::ltl_direction)
		{
			if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GEOM, 0)) >= 0)
				SGeom_ModelsDelArrForCom(tmpidarr);

			if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GREEN, 0)) >= 0)
				SGeom_GreenDelArrForCom(tmpidarr);

			if((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_ANIM, 0)) >= 0)
				SXAnim_ModelsDelArrForCom(tmpidarr);

		}
		else if (SML_LigthsDelGetType(i) == LightsTypeLight::ltl_point)
		{
			for (int k = 0; k<6; k++)
			{
				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GEOM, k)) >= 0)
					SGeom_ModelsDelArrForCom(tmpidarr);

				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GREEN, k)) >= 0)
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
			MtlTypeReflect typeref = SML_MtlGetTypeReflection(SGeom_ModelsMGetGroupIDMat(i, k));
			float3_t center;

			if (typeref == MtlTypeReflect::mtr_plane)
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
			else if (typeref == MtlTypeReflect::mtr_cube_dynamic)
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
	
	if(!Core_0FileExists(GData::Pathes::Screenshots))
	{
		CreateDirectoryA(GData::Pathes::Screenshots, NULL);
	}

	do
	{
		++numscreen;
		sprintf(tmppath, "%sscreen_skyxengine_build_%d.jpg", GData::Pathes::Screenshots, numscreen);
	} while (Core_0FileExists(tmppath));

	LPDIRECT3DSURFACE9 BackBuf;
	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	D3DXSaveSurfaceToFile(tmppath, D3DXIFF_JPG, BackBuf, NULL, NULL);
	mem_release(BackBuf);
}

void SXRenderFunc::SaveWorkTex()
{
	char tmppath[1024];
	sprintf(tmppath, "%scolor.png", GData::Pathes::WorkingTex);
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT::ds_rt_color), NULL);

	sprintf(tmppath, "%snormal.png", GData::Pathes::WorkingTex);
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT::ds_rt_normal), NULL);

	sprintf(tmppath, "%sdepth.png", GData::Pathes::WorkingTex);
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT::ds_rt_depth), NULL);

	sprintf(tmppath, "%sdepth0.png", GData::Pathes::WorkingTex);
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT::ds_rt_depth0), NULL);

	sprintf(tmppath, "%sparam.png", GData::Pathes::WorkingTex);
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT::ds_rt_param), NULL);

	sprintf(tmppath, "%sambient_diff.png", GData::Pathes::WorkingTex);
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT::ds_rt_ambient_diff), NULL);

	sprintf(tmppath, "%sspecular.png", GData::Pathes::WorkingTex);
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT::ds_rt_specular), NULL);


	sprintf(tmppath, "%slight_com_1.png", GData::Pathes::WorkingTex);
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT::ds_rt_scene_light_com), NULL);

	sprintf(tmppath, "%slight_com_2.png", GData::Pathes::WorkingTex);
	D3DXSaveTextureToFile(tmppath, D3DXIFF_PNG, SML_DSGetRT(DS_RT::ds_rt_scene_light_com2), NULL);
}

void SXRenderFunc::ChangeModeWindow()
{
	if (GData::IsWindowed)
	{
		SetWindowLong(GData::Handle3D, GWL_STYLE, WS_POPUP);
		ShowWindow(GData::Handle3D, SW_MAXIMIZE);
	}
	else
	{
		SetWindowLong(GData::Handle3D, GWL_STYLE, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
		SetWindowPos(GData::Handle3D, HWND_NOTOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
	}

	GData::IsWindowed = !GData::IsWindowed;
	GData::ReSize = 2;
}

/////

void SXRenderFunc::UpdateView()
{
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
}

void SXRenderFunc::OutputDebugInfo(DWORD timeDelta)
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

		if(TimeElapsed >= 1.0f)
		{
			
			FpsValue	= (float)FrameCount / TimeElapsed;
			sprintf(debugstr, "FPS %.1f\n", FpsValue);

#if !defined(SX_MATERIAL_EDITOR) && !defined(SX_LEVEL_EDITOR) && !defined(SX_PARTICLES_EDITOR)
			sprintf(debugstr + strlen(debugstr), "\ncount poly : %d\n", Core_RIntGet(G_RI_INT_COUNT_POLY) / FrameCount);
			sprintf(debugstr + strlen(debugstr), "count DIPs : %d\n\n", Core_RIntGet(G_RI_INT_COUNT_DIP) / FrameCount);
			sprintf(debugstr + strlen(debugstr), "Pos camera : [%.2f, %.2f, %.2f]\n", GData::ConstCurrCamPos.x, GData::ConstCurrCamPos.y, GData::ConstCurrCamPos.z);
			sprintf(debugstr + strlen(debugstr), "Dir camera : [%.2f, %.2f, %.2f]\n", GData::ConstCurrCamDir.x, GData::ConstCurrCamDir.y, GData::ConstCurrCamDir.z);

			sprintf(debugstr + strlen(debugstr), "\nDELAY:\n");
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

			sprintf(debugstr + strlen(debugstr), "\n\FreeVal : %d\n", SXRenderFunc::Delay::FreeVal);
#endif
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
			
	SGCore_DbgMsg(debugstr);
}

////////

void SXRenderFunc::RenderInMRT(DWORD timeDelta)
{
	SGCore_ShaderUnBind();

	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_MATERIAL);
	
	Core_RMatrixGet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
	SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

	LPDIRECT3DSURFACE9 BackBuf, ColorSurf, NormalSurf, ParamSurf, DepthMapLinearSurf;

	SML_DSGetRT(DS_RT::ds_rt_color)->GetSurfaceLevel(0, &ColorSurf);
	SML_DSGetRT(DS_RT::ds_rt_normal)->GetSurfaceLevel(0, &NormalSurf);
	SML_DSGetRT(DS_RT::ds_rt_param)->GetSurfaceLevel(0, &ParamSurf);
	SML_DSGetRT(DS_RT::ds_rt_depth)->GetSurfaceLevel(0, &DepthMapLinearSurf);

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
//если не объявлен флаг редактора материалов (для него немного другой рендер)
#if !defined(SX_MATERIAL_EDITOR)
	//SXDecals_Render();
	if (SGeom_ModelsGetCount() > 0)
	{
		/*if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
			SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_none, 0, false, GData::Editors::ActiveElement);
		else*/
		SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_none);
	}

	SXAnim_Render();

	if (SGeom_GreenGetCount() > 0)
		SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GeomGreenType::ggtr_all);

	SGCore_ShaderUnBind();
	
	SML_LigthsRenderSource(-1, true, timeDelta);

	SGCore_ShaderUnBind();
#endif

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
	SML_DSGetRT(DS_RT::ds_rt_depth0)->GetSurfaceLevel(0, &DepthSurf2);

	GData::DXDevice->SetRenderTarget(0, DepthSurf2);

	GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT::ds_rt_depth));

	SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();


	SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
	SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);
	//SML_DSGetRT(DS_RT::ds_rt_depth)->GetSurfaceLevel(0, &DepthMapLinearSurf);
	GData::DXDevice->SetRenderTarget(0, ColorSurf);
	GData::DXDevice->SetRenderTarget(1, NormalSurf);
	GData::DXDevice->SetRenderTarget(2, ParamSurf);
	GData::DXDevice->SetRenderTarget(3, DepthMapLinearSurf);

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

	mem_release_del(DepthSurf2);
	//}}

#if !defined(SX_MATERIAL_EDITOR)

	//если есть что к отрисовке из полупрозрачной геометрии
	if (SGeom_ModelsSortExistsForRender(MtlTypeTransparency::mtt_alpha_lighting))
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

		SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::StencilStr);
		SGCore_ShaderSetVRF(ShaderType::st_pixel, GData::IDsShaders::PS::StencilStr, "WinSize", &GData::WinSize);

		SGCore_ScreenQuadDraw();

		GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 2);

		SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::StencilColumn);
		SGCore_ShaderSetVRF(ShaderType::st_pixel, GData::IDsShaders::PS::StencilColumn, "WinSize", &GData::WinSize);

		SGCore_ScreenQuadDraw();

		GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 3);

		SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::StencilStrColumn);
		SGCore_ShaderSetVRF(ShaderType::st_pixel, GData::IDsShaders::PS::StencilStrColumn, "WinSize", &GData::WinSize);

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

			/*if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
				SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_alpha_lighting, 0, true, GData::Editors::ActiveElement);
				else*/
			SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_alpha_lighting, 0, true);
		}


		SML_MtlSetForceblyAlphaTest(false);
		GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

		SGCore_ShaderUnBind();

		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	}

	SXDecals_Render();

#else
	if(SML_MtlGetTypeTransparency(GData::Editors::SimModel->GetIDMtl()) != MtlTypeTransparency::mtt_none)
		SML_MtlSetForceblyAlphaTest(true);
	GData::Editors::SimModel->Render(timeDelta);
	if (SML_MtlGetTypeTransparency(GData::Editors::SimModel->GetIDMtl()) != MtlTypeTransparency::mtt_none)
		SML_MtlSetForceblyAlphaTest(false);
#endif

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	GData::DXDevice->SetRenderTarget(1, 0);
	GData::DXDevice->SetRenderTarget(2, 0);
	GData::DXDevice->SetRenderTarget(3, 0);

	mem_release(BackBuf);

	mem_release(ColorSurf);
	mem_release(NormalSurf);
	mem_release(ParamSurf);
	mem_release(DepthMapLinearSurf);
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
		long tmpid = SML_LigthsGetIDOfKey(i);
		Core_RIntSet(G_RI_INT_CURRIDLIGHT, tmpid);
		if (SML_LigthsIsShadow(tmpid) && (SML_LigthsComVisibleForFrustum(tmpid, GData::ObjCamera->ObjFrustum) && SML_LigthsIsEnable(tmpid)) /*|| (Data::Level::LightManager->Arr[i]->ShadowCube && Data::Level::LightManager->Arr[i]->ShadowCube->GetStatic() && !Data::Level::LightManager->Arr[i]->ShadowCube->GetUpdate())*/)
		{
			if (SML_LigthsGetType(tmpid) == LightsTypeLight::ltl_global)
			{
				SML_LigthsInRenderBegin(tmpid);
				int countsplits = (SML_LigthsGet4Or3SplitsG(tmpid) ? 4 : 3);
					for (int k = 0; k<countsplits; k++)
					{
						if (SML_LigthsAllowedRender(tmpid, k))
						{
							SML_LigthsInRenderPre(tmpid, k);

							if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k) > -1)
								SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_none, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k));

							if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k) > -1)
								SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GeomGreenType::ggt_tree, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k));

							SML_LigthsRenderAllExceptGroupSource(tmpid, timeDelta);

							SXAnim_Render();
						}
					}

				//КОГДА ИСТОЧНИК БЛИЗОК К ГОРИЗОНТУ ИЗ-ЗА ОБЛАКОВ ВОЗНИКАЕТ БАГ С ТЕНЯМИ В ВИДЕ ФЕЙКОВЫХ ТЕНЕЙ
				if (SGCore_SkyCloudsIsCr() && SGCore_SkyCloudsIsLoadTex())
				{
					SML_LigthsUpdateFrustumsG(tmpid, 4, &GData::ConstCurrCamPos, &GData::ConstCurrCamDir);
					SML_LigthsInRenderPre(tmpid, 4);
					GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
					GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
					SetSamplerFilter(0, D3DTEXF_LINEAR);
					SetSamplerFilter(1, D3DTEXF_LINEAR);
					SetSamplerAddress(0, D3DTADDRESS_MIRROR);
					SetSamplerAddress(1, D3DTADDRESS_MIRROR);

					SGCore_SkyCloudsRender(timeDelta, &float3(GData::ConstCurrCamPos.x, GData::ConstCurrCamPos.y + 150, GData::ConstCurrCamPos.z), true);

					SetSamplerAddress(0, D3DTADDRESS_WRAP);
					SetSamplerAddress(1, D3DTADDRESS_WRAP);
				}
				SML_LigthsInRenderEnd(tmpid);
			}
			else if (SML_LigthsGetType(tmpid) == LightsTypeLight::ltl_direction)
			{
				if (SML_LigthsUpdateCountUpdate(tmpid, &GData::ConstCurrCamPos))
				{
					SML_LigthsInRenderBegin(tmpid);

					if (SML_LigthsGetTypeShadowed(tmpid) == LightsTypeShadow::lts_dynamic)
					{
						if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0) > -1)
							SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_none, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0));
							
						if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0) > -1)
							SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GeomGreenType::ggt_tree, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0));

						SXAnim_Render();
					}
					else
					{
						SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, 0), &GData::ConstCurrCamPos, GData::DefaultGeomIDArr);
						SGeom_ModelsRender(timeDelta, -1, GData::DefaultGeomIDArr);
						SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, 0), &GData::ConstCurrCamPos, GData::DefaultGreenIDArr);
						SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GeomGreenType::ggt_tree, GData::DefaultGreenIDArr);
					}

					SML_LigthsRenderAllExceptGroupSource(tmpid, timeDelta);
					SML_LigthsInRenderEnd(tmpid);
				}
			}
			else if (SML_LigthsGetType(tmpid) == LightsTypeLight::ltl_point)
			{
				if (SML_LigthsUpdateCountUpdate(tmpid, &GData::ConstCurrCamPos))
				{
					SML_LigthsInRenderBegin(tmpid);

					for (int k = 0; k < 6; k++)
					{
						if (SML_LigthsGetEnableCubeEdge(tmpid, k))
						{
							SML_LigthsInRenderPre(tmpid, k);

							if (SML_LigthsGetTypeShadowed(tmpid) == LightsTypeShadow::lts_dynamic)
							{
								if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k) > -1)
									SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_none, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k));

								if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k) > -1)
									SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GeomGreenType::ggt_tree, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k));

								SXAnim_Render();
							}
							else
							{
								SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, GData::DefaultGeomIDArr);
								SGeom_ModelsRender(timeDelta, -1, GData::DefaultGeomIDArr);
								SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, GData::DefaultGreenIDArr);
								SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GeomGreenType::ggt_tree, GData::DefaultGreenIDArr);
							}
							
							SML_LigthsRenderAllExceptGroupSource(tmpid, timeDelta);
							SML_LigthsInRenderPost(tmpid, k);
						}
					}

					SML_LigthsInRenderEnd(tmpid);
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
	SML_DSGetRT(DS_RT::ds_rt_scene_light_com)->GetSurfaceLevel(0, &ColorSurf);
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

	GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT::ds_rt_scene_light_com2));

	SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	mem_release(ColorSurf);

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);

	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void SXRenderFunc::ComLighting(DWORD timeDelta, bool render_sky)
{
	SGCore_ShaderUnBind();

	LPDIRECT3DSURFACE9 AmbientSurf, SpecDiffSurf, BackBuf;
	SML_DSGetRT(DS_RT::ds_rt_ambient_diff)->GetSurfaceLevel(0, &AmbientSurf);
	SML_DSGetRT(DS_RT::ds_rt_specular)->GetSurfaceLevel(0, &SpecDiffSurf);

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
		//поулчаем идентификатор света по ключу
		ID tmpid = SML_LigthsGetIDOfKey(i);

		//если свет виден фрустуму камеры (это надо было заранее просчитать) и если свет включен
		if (SML_LigthsGetVisibleForFrustum(tmpid) && SML_LigthsIsEnable(tmpid))
		{
			//пока что назначаем шейдер без теней
			ID idshader = GData::IDsShaders::PS::ComLightingNonShadow;

			//если не глобальный источник
			if (SML_LigthsGetType(tmpid) != LightsTypeLight::ltl_global)
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
				SML_LigthsRender(tmpid, 0);

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
			if (SML_LigthsIsShadow(tmpid))
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

				SML_LigthsNullingShadow();	//очищаем рт генерации теней
				SML_LigthsGenShadow(tmpid);	//генерируем тень для света

				//сглаживаем
				SML_LigthsSoftShadow(false, 4);
				SML_LigthsSoftShadow(false, 2);

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
			if (SML_LigthsGetType(tmpid) != LightsTypeLight::ltl_global)
				GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);

			float determ = 0;
			float4x4 ViewInv = SMMatrixInverse(&determ, GData::MCamView);
			ViewInv = SMMatrixTranspose(ViewInv);

			SGCore_ShaderSetVRF(ShaderType::st_vertex, GData::IDsShaders::VS::ResPos, "ViewInv", &ViewInv);
			SGCore_ShaderSetVRF(ShaderType::st_vertex, GData::IDsShaders::VS::ResPos, "NearFar", &GData::NearFar);
			SGCore_ShaderSetVRF(ShaderType::st_vertex, GData::IDsShaders::VS::ResPos, "ParamProj", &float3_t(GData::WinSize.x, GData::WinSize.y, GData::ProjFov));

			float3 tmpPosition;
			float2 tmpPowerDist;
			float3 tmpColor;
			SML_LigthsGetColor(tmpid, &tmpColor);
			SML_LigthsGetPos(tmpid, &tmpPosition, false, true);
			tmpPowerDist.x = SML_LigthsGetPowerDiv(tmpid);
			tmpPowerDist.y = SML_LigthsGetDist(tmpid);

			SGCore_ShaderSetVRF(ShaderType::st_pixel, idshader, "ViewPos", &GData::ConstCurrCamPos);
			SGCore_ShaderSetVRF(ShaderType::st_pixel, idshader, "LightPos", &(tmpPosition));
			SGCore_ShaderSetVRF(ShaderType::st_pixel, idshader, "LightPowerDist", &(tmpPowerDist));
			SGCore_ShaderSetVRF(ShaderType::st_pixel, idshader, "LightColor", &tmpColor);
			//SGCore_ShaderSetVRF(ShaderType::st_pixel, idshader, "NearFar", &GData::NearFar);

			SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ResPos);
			SGCore_ShaderBind(ShaderType::st_pixel, idshader);

			SetSamplerFilter(0, 4, D3DTEXF_NONE);
			SetSamplerAddress(0, 4, D3DTADDRESS_CLAMP);
			
			GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT::ds_rt_color));
			GData::DXDevice->SetTexture(1, SML_DSGetRT(DS_RT::ds_rt_normal));
			GData::DXDevice->SetTexture(2, SML_DSGetRT(DS_RT::ds_rt_param));
			GData::DXDevice->SetTexture(3, SML_DSGetRT(DS_RT::ds_rt_depth));

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
	//обратаываем hdr (а если точнее именно tone mapping)
	SML_LigthsComHDR(timeDelta,30);
	
	//теперь необходимо все смешать чтобы получить итоговую освещенную картинку
	//{{
	SetSamplerFilter(0, 5, D3DTEXF_NONE);
	SetSamplerAddress(0, 5, D3DTADDRESS_CLAMP);
	LPDIRECT3DSURFACE9 ComLightSurf;

	SML_DSGetRT(DS_RT::ds_rt_scene_light_com)->GetSurfaceLevel(0, &ComLightSurf);

	GData::DXDevice->GetRenderTarget(0, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, ComLightSurf);

	//очищаем рт (в старой версии было многопроходное смешинваие)
	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

	GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT::ds_rt_color));
	GData::DXDevice->SetTexture(1, SML_DSGetRT(DS_RT::ds_rt_ambient_diff));
	GData::DXDevice->SetTexture(2, SML_DSGetRT(DS_RT::ds_rt_specular));
	GData::DXDevice->SetTexture(3, SML_DSGetRT(DS_RT::ds_rt_normal));
	GData::DXDevice->SetTexture(4, SML_DSGetRT(DS_RT::ds_rt_adapted_lum_curr));
	GData::DXDevice->SetTexture(5, SML_DSGetRT(DS_RT::ds_rt_param));

	SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::BlendAmbientSpecDiffColor);
	
	SGCore_ScreenQuadDraw();

	mem_release(ComLightSurf);
	//}}

	//теперь надо смешать все полупрозрачные слои (если есть) с непрозрачными
	//{{
	static int CurrCountTransparencySurf;
#if defined(SXMATERIAL_EDITOR)
	CurrCountTransparencySurf = 3;
#else
	CurrCountTransparencySurf = SML_MtlGetCurrCountSurf();
#endif
	if (CurrCountTransparencySurf >= RENDER_LAYER_TRANSPARENT)
	{
		LPDIRECT3DSURFACE9 ColorSurf, Color2Surf, DepthSurf;
		SML_DSGetRT(DS_RT::ds_rt_scene_light_com2)->GetSurfaceLevel(0, &ColorSurf);
		GData::DXDevice->SetRenderTarget(0, ColorSurf);
		SML_DSGetRT(DS_RT::ds_rt_depth0)->GetSurfaceLevel(0, &DepthSurf);
		GData::DXDevice->SetRenderTarget(1, DepthSurf);
		SML_DSGetRT(DS_RT::ds_rt_depth1)->GetSurfaceLevel(0, &Color2Surf);
		GData::DXDevice->SetRenderTarget(2, Color2Surf);
		GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT::ds_rt_color));
		GData::DXDevice->SetTexture(1, SML_DSGetRT(DS_RT::ds_rt_scene_light_com));
		GData::DXDevice->SetTexture(2, SML_DSGetRT(DS_RT::ds_rt_depth));
		GData::DXDevice->SetTexture(3, SML_DSGetRT(DS_RT::ds_rt_normal));
		GData::DXDevice->SetTexture(4, SML_DSGetRT(DS_RT::ds_rt_param));

		SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::UnionAlpha);
		SGCore_ShaderSetVRF(ShaderType::st_pixel, GData::IDsShaders::PS::UnionAlpha, "WinSize", &float4_t(GData::WinSize.x, GData::WinSize.y, 1.f / GData::WinSize.x, 1.f / GData::WinSize.y));

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

		SML_DSGetRT(DS_RT::ds_rt_depth1)->GetSurfaceLevel(0, &DepthSurf1);
		GData::DXDevice->SetRenderTarget(0, DepthSurf1);

		GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT::ds_rt_depth));

		SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::ScreenOut);

		SGCore_ScreenQuadDraw();

		GData::DXDevice->SetRenderTarget(0, 0);
		mem_release(DepthSurf1);
	}

	//копируем итоговую сцену
	SML_DSGetRT((CurrCountTransparencySurf >= RENDER_LAYER_TRANSPARENT ? DS_RT::ds_rt_scene_light_com : DS_RT::ds_rt_scene_light_com2))->GetSurfaceLevel(0, &ComLightSurf);
	GData::DXDevice->SetRenderTarget(0, ComLightSurf);
	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
	GData::DXDevice->SetTexture(0, SML_DSGetRT((CurrCountTransparencySurf >= RENDER_LAYER_TRANSPARENT ? DS_RT::ds_rt_scene_light_com2 : DS_RT::ds_rt_scene_light_com)));

	SGCore_ShaderBind(ShaderType::st_vertex, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(ShaderType::st_pixel, GData::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	mem_release(ComLightSurf);

	//}}

	SGCore_ShaderUnBind();

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);

	//если надо то рисуем небо
	if (render_sky && SGCore_SkyBoxIsCr())
		SXRenderFunc::RenderSky(timeDelta);

	SGCore_ShaderUnBind();
}

void SXRenderFunc::RenderParticles(DWORD timeDelta)
{
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

#if defined(SX_PARTICLES_EDITOR)
	//GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 128, 128, 128), 1.0f, 0);

	if (GData::Editors::RenderBound && SXParticlesEditor::SelEffID != -1 && SXParticlesEditor::SelEmitterID != -1 && SPE_EmitterGet(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID, BoundType) != ParticlesBoundType::pbt_none)
	{
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

		ParticlesData* pdata = SPE_EmitterGetData(SXParticlesEditor::SelEffID, SXParticlesEditor::SelEmitterID);
		GData::DXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		if (pdata->BoundType == ParticlesBoundType::pbt_box)
		{
			GData::DXDevice->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)(SMMatrixScaling(float3(pdata->BoundVec2 - pdata->BoundVec1)) * SMMatrixTranslation(float3(pdata->BoundVec2 + pdata->BoundVec1)*0.5f)));
			GData::Editors::FigureBox->DrawSubset(0);
		}
		else if (pdata->BoundType == ParticlesBoundType::pbt_sphere)
		{
			GData::DXDevice->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)(SMMatrixScaling(pdata->BoundVec1.w, pdata->BoundVec1.w, pdata->BoundVec1.w) * SMMatrixTranslation(float3(pdata->BoundVec1))));
			GData::Editors::FigureSphere->DrawSubset(0);
		}
		else if (pdata->BoundType == ParticlesBoundType::pbt_cone)
		{
			if(GData::Editors::FigureConeParam.x != pdata->BoundVec2.w || GData::Editors::FigureConeParam.y != pdata->BoundVec1.w || GData::Editors::FigureConeParam.z != pdata->BoundVec2.y - pdata->BoundVec1.y)
			{
				GData::Editors::FigureConeParam.x = pdata->BoundVec2.w;
				GData::Editors::FigureConeParam.y = pdata->BoundVec1.w;
				GData::Editors::FigureConeParam.z = pdata->BoundVec2.y - pdata->BoundVec1.y;

				SGCore_FCreateCone(GData::Editors::FigureConeParam.x, GData::Editors::FigureConeParam.y, GData::Editors::FigureConeParam.z, &GData::Editors::FigureCone, 20);
			}

			GData::DXDevice->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)SMMatrixTranslation(pdata->BoundVec1.x, pdata->BoundVec2.y, pdata->BoundVec1.z));
			GData::Editors::FigureCone->DrawSubset(0);
		}

		GData::DXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		GData::DXDevice->SetTransform(D3DTS_WORLD, &(D3DXMATRIX)SMMatrixIdentity());
	}

#endif


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
	SPP_RTNull();

	if (!SSInput_GetKeyState(SIK_X))
		SPP_RenderSSAO(&float4_t(0.3f, 0.1f, 0.8f, 0.3f / GData::NearFar.y));

	if (!SSInput_GetKeyState(SIK_L))
		SPP_RenderFog(&float3_t(0.5, 0.5, 0.5), &float4_t(0.8, 0, 0.1, 0.9));
	//SPP_RenderWhiteBlack(1);
	SPP_RenderBloom(&float3_t(1, 0.9, 1));
	SPP_RenderLensFlare(&float3_t(0.25f, 0.3f, 0.9f), true);
	SPP_Update(&(float3_t)GData::ConstCurrCamPos, &(float3_t)GData::ConstCurrCamDir, &GData::MCamView, &GData::MCamProj, &GData::WinSize, &GData::NearFar, GData::ProjFov);

	float3 tmpPosition;
	float3 tmpColor;

	ID GlobalLight = SML_LigthsGetGlobal();
	if (GlobalLight > -1)
	{
		SML_LigthsGetColor(GlobalLight, &tmpColor);
		SML_LigthsGetPos(GlobalLight, &tmpPosition, false, true);

		SPP_UpdateSun(&tmpPosition);

		SPP_RenderSun(&float4_t(tmpColor.x, tmpColor.y, tmpColor.z, SML_LigthsGetPowerDiv(0)));
	}

	SPP_RenderDOF(&float4_t(0, 200, 0, 100), 0);

	if (!SSInput_GetKeyState(SIK_Z))
		SPP_RenderDLAA();

	/*if (!SSInput_GetKeyState(SIK_C))
		SPP_RenderNFAA(&float3_t(1, 1, 0));*/

	//SPP_RenderMotionBlur(0.1, timeDelta);
}

void SXRenderFunc::ShaderRegisterData()
{
	static float4_t tmpnull[256];
	GData::DXDevice->SetVertexShaderConstantF(0, (float*)&tmpnull, 256);
	GData::DXDevice->SetPixelShaderConstantF(0, (float*)&tmpnull, 256);
}

void SXRenderFunc::UpdateReflection(DWORD timeDelta)
{
	Core_RIntSet(G_RI_INT_RENDERSTATE, RENDER_STATE_FREE);
	Core_RMatrixSet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &GData::MCamView);

	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

//если не объявлен флаг редактора материалов (для него немного другой рендер)
#if !defined(SX_MATERIAL_EDITOR)
	for (int i = 0; i < SGeom_ModelsGetCount(); ++i)
	{
		for (int k = 0; k < SGeom_ModelsMGetCountGroups(i); ++k)
		{
			ID idmat = SGeom_ModelsMGetGroupIDMat(i, k);
			MtlTypeReflect typeref = SML_MtlGetTypeReflection(SGeom_ModelsMGetGroupIDMat(i, k));
			D3DXPLANE plane;
			float3_t center;
			if (typeref == MtlTypeReflect::mtr_plane)
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
					SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_none, SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, 0), false, i, k);
				
				if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, 0) >= 0)
					SGeom_GreenRender(timeDelta, &float3(center), GeomGreenType::ggtr_all, SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, 0));

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
			else if (typeref == MtlTypeReflect::mtr_cube_static || typeref == MtlTypeReflect::mtr_cube_dynamic)
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
					if (SML_MtlGetTypeReflection(idmat) == MtlTypeReflect::mtr_cube_static)
					{
						//тогда считаем в массив камеры
						SGeom_ModelsComVisible(SML_MtlRefGetfrustum(idmat, j), &float3(center), GData::DefaultGeomIDArr);
						SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_none, GData::DefaultGeomIDArr, false, i, k);

						SGeom_GreenComVisible(SML_MtlRefGetfrustum(idmat, j), &float3(center), GData::DefaultGreenIDArr);
						SGeom_GreenRender(timeDelta, &float3(center), GeomGreenType::ggtr_all, GData::DefaultGreenIDArr);
					}
					else
					{
						if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, k) >= 0)
							SGeom_ModelsRender(timeDelta, MtlTypeTransparency::mtt_none, SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GEOM, k), false, i, k);
						
						if (SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, k) >= 0)
							SGeom_GreenRender(timeDelta, &GData::ConstCurrCamPos, GeomGreenType::ggtr_all, SML_MtlRefGetIDArr(idmat, RENDER_IDARRCOM_GREEN, k));

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
#else
	ID idmat = GData::Editors::SimModel->GetIDMtl();
	MtlTypeReflect typeref = SML_MtlGetTypeReflection(idmat);
	D3DXPLANE plane;
	float3_t center;
	GData::Editors::SimModel->GetCenter(&center);

	if (typeref == MtlTypeReflect::mtr_plane)
	{
		GData::Editors::SimModel->GetPlane(&plane);
		
		SML_MtlRefPreRenderPlane(idmat, &plane);
		SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
		SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

		GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
		GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &GData::MRefPlaneSkyProj);
		SGCore_SkyBoxRender(timeDelta, &float3(center.x, center.y, center.z));
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		SML_MtlRefPostRenderPlane(idmat);
	}
	else if (typeref == MtlTypeReflect::mtr_cube_static || typeref == MtlTypeReflect::mtr_cube_dynamic)
	{
		SML_MtlRefCubeBeginRender(idmat, &center);

		for (int j = 0; j<6; j++)
		{
			Core_RMatrixGet(G_RI_MATRIX_WORLD, &SMMatrixIdentity());

			SML_MtlRefCubePreRender(idmat, j, &(SMMatrixIdentity()));

			GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

			GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
			GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
			Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &GData::MRefCubeSkyProj);
			SGCore_SkyBoxRender(timeDelta, &float3(0, 0, 0));
			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
			GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

			SML_MtlRefCubePostRender(idmat, j);
		}
		SML_MtlRefCubeEndRender(idmat, &((float3_t)GData::ConstCurrCamPos));
	}
#endif
}

////////

void SXRenderFunc::RenderEditorMain()
{
#if !defined(SX_GAME)
	if (GData::Editors::ObjGrid && GData::Editors::RenderGrid)
	{
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		GData::Editors::ObjGrid->Render();
	}

	if (GData::Editors::ObjAxesStatic && GData::Editors::RenderAxesStatic)
	{
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		GData::Editors::ObjAxesStatic->Render();
	}
#endif
}

void SXRenderFunc::RenderEditorLE(DWORD timeDelta)
{
#if defined(SX_LEVEL_EDITOR)
	if (GData::Editors::SelSelection)
	{
		if (GData::Editors::SelZTest)
		{
			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		}
		else
		{
			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		}

		if (GData::Editors::SelMesh)
			GData::DXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		if (GData::Editors::SelBackFacesCull)
			GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_LIGHT)
		{
			if (GData::Editors::ActiveElement > -1)
			{
				GData::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(GData::IDSelectTex));
				SML_LigthsRender(GData::Editors::ActiveElement, timeDelta);
			}
		}

		if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GEOM)
		{
			if (GData::Editors::ActiveElement > -1)
			{
				GData::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(GData::IDSelectTex));
				SGeom_ModelsRenderSingly(timeDelta, GData::Editors::ActiveElement, SML_MtlGetStdMtl(MtlTypeModel::tms_static));
			}
		}

		if (GData::Editors::ActiveGroupType == EDITORS_LEVEL_GROUPTYPE_GREEN)
		{
			if (GData::Editors::ActiveElement > -1)
			{
				if (GData::Editors::ActiveGreenSplit >= 0 && GData::Editors::ActiveGreenObject >= 0)
				{
					GData::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(GData::IDSelectTex));
					SGeom_GreenRenderObject(timeDelta, &GData::ConstCurrCamPos, GData::Editors::ActiveElement, GData::Editors::ActiveGreenSplit, GData::Editors::ActiveGreenObject, SML_MtlGetStdMtl(MtlTypeModel::tms_tree));
				}
				else
				{
					GData::DXDevice->SetTexture(0, SGCore_LoadTexGetTex(GData::IDSelectTex));
					SGeom_GreenRenderSingly(timeDelta, &GData::ConstCurrCamPos, GData::Editors::ActiveElement, SML_MtlGetStdMtl(MtlTypeModel::tms_tree));
				}
			}
		}

		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);

		if (GData::Editors::SelMesh)
			GData::DXDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		if (GData::Editors::SelBackFacesCull)
			GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
#endif
}

///////

void SXRenderFunc::RFuncDIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{

}

void SXRenderFunc::RFuncMtlSet(ID id, float4x4* world)
{
	switch(Core_RIntGet(G_RI_INT_RENDERSTATE))
	{
	case RENDER_STATE_SHADOW:
		SML_MtlSetMainTexture(0, id);
		SML_LigthsShadowSetShaderOfTypeMat(Core_RIntGet(G_RI_INT_CURRIDLIGHT), SML_MtlGetTypeModel(id), world);
		break;

	case RENDER_STATE_FREE:
		SML_MtlSetMainTexture(0, id);
		Core_RMatrixSet(G_RI_MATRIX_WORLD, &(world ? (*world) : SMMatrixIdentity()));
		//SGCore_ShaderUnBind();
		SML_MtlRenderStd(SML_MtlGetTypeModel(id), world, 0, id);
		break;

	case RENDER_STATE_MATERIAL:
		SML_MtlRender(id, world);
		break;
	}
}

ID SXRenderFunc::RFuncMtlLoad(const char* name, int mtl_type)
{
	return SML_MtlLoad(name, (MtlTypeModel)mtl_type);
}