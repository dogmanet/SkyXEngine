
#include <common\\render_func.h>

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
	//получаем текущий размер окна в которое рисовали
	RECT rect_scene;
	GetClientRect(GData::Handle3D, &rect_scene);

	GData::WinSize.x = rect_scene.right;
	GData::WinSize.y = rect_scene.bottom;

	//сбрасываем все что необходимо для восстановления устройства
	SGCore_OnLostDevice();
	SGeom_OnLostDevice();
	SML_OnLostDevice();

	bool bf = SGCore_OnDeviceReset(GData::WinSize.x, GData::WinSize.y,GData::IsWindowed);
		if (bf)
		{
			//если всетаки функция зашла сюда значит чтото было неосвобождено
			//InLog("Reset device is failed ... %d \n", DWORD(hr));
		}
		else
		{
			//переделываем матрицы проекции
			GData::ProjRatio = GData::WinSize.x / GData::WinSize.y;
			GData::MCamProj = SMMatrixPerspectiveFovLH(GData::ProjFov, GData::ProjRatio, GData::NearFar.x, GData::NearFar.y);
			GData::MLightProj = SMMatrixPerspectiveFovLH(GData::ProjFov, GData::ProjRatio, GData::NearFar.x, 10000);
			GData::ReSize = false;;
			SGCore_OnResetDevice();
			SML_OnResetDevice(GData::WinSize.x, GData::WinSize.y, GData::ProjFov);
			SGeom_OnResetDevice();
			GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		}
}

void SXRenderFunc::UpdateLight()
{
	for (int i = 0; i<SML_LigthsGetCount(); i++)
	{
		long tmpid = SML_LigthsGetIDOfKey(i);
		if (SML_LigthsIsShadow(tmpid) && (SML_LigthsComVisibleForFrustum(tmpid, GData::ObjCamera->ObjFrustum) && SML_LigthsIsEnable(tmpid)) /*|| (Data::Level::LightManager->Arr[i]->ShadowCube && Data::Level::LightManager->Arr[i]->ShadowCube->GetStatic() && !Data::Level::LightManager->Arr[i]->ShadowCube->GetUpdate())*/)
		{
			if (SML_LigthsGetType(tmpid) == LIGHTS_TYPE_GLOBAL)
			{
				for (int k = 0; k<4; k++)
				{
					if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k, SGeom_ModelsAddArrForCom());

					if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k, SGeom_GreenAddArrForCom());

					if (SML_LigthsUpdateCountUpdate(tmpid, &GData::ConstCurrCamPos, k))
					{
						SML_LigthsUpdateFrustumsG(tmpid, k, &GData::ConstCurrCamPos, &GData::ConstCurrCamDir);
						SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k));
						SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k));
					}
				}
			}
			else if (SML_LigthsGetType(tmpid) == LIGHTS_TYPE_DIRECTION)
			{
				if (SML_LigthsGetTypeShadowed(tmpid) == LIGHTS_TYPE_SHADOWED_DYNAMIC)
				{
					if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0, SGeom_ModelsAddArrForCom());

					if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0) <= -1)
						SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0, SGeom_GreenAddArrForCom());

					SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, 0), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0));
					SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, 0), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0));
				}
			}
			else if (SML_LigthsGetType(tmpid) == LIGHTS_TYPE_POINT)
			{
				if (SML_LigthsGetTypeShadowed(tmpid) == LIGHTS_TYPE_SHADOWED_DYNAMIC)
				{
					for (int k = 0; k < 6; k++)
					{
						if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k) <= -1)
							SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k, SGeom_ModelsAddArrForCom());

						if (SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k) <= -1)
							SML_LigthsSetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k, SGeom_GreenAddArrForCom());

						if (SML_LigthsGetEnableCubeEdge(tmpid, k))
						{
							SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k));
							SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0));
						}
					}
				}
			}
		}
	}

	long tmpidarr = -1;
	for (long i = 0; i < SML_LigthsDelGetCount(); ++i)
	{
		if (SML_LigthsDelGetType(i) == LIGHTS_TYPE_GLOBAL)
		{
			for (int k = 0; k<4; k++)
			{
				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GEOM, k)) >= 0)
					SGeom_ModelsDelArrForCom(tmpidarr);

				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GREEN, k)) >= 0)
					SGeom_GreenDelArrForCom(tmpidarr);
			}
		}
		else if (SML_LigthsDelGetType(i) == LIGHTS_TYPE_DIRECTION)
		{
			if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GEOM, 0)) >= 0)
				SGeom_ModelsDelArrForCom(tmpidarr);

			if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GREEN, 0)) >= 0)
				SGeom_GreenDelArrForCom(tmpidarr);
		}
		else if (SML_LigthsDelGetType(i) == LIGHTS_TYPE_POINT)
		{
			for (int k = 0; k<6; k++)
			{
				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GEOM, k)) >= 0)
					SGeom_ModelsDelArrForCom(tmpidarr);

				if ((tmpidarr = SML_LigthsDelGetIDArr(i, RENDER_IDARRCOM_GREEN, k)) >= 0)
					SGeom_GreenDelArrForCom(tmpidarr);
			}
		}

		SML_LigthsDelDel(i);
	}
}

/////

void SXRenderFunc::UpdateView(DWORD timeDelta)
{
	GData::DXDevice->SetTransform(D3DTS_WORLD, &(SMMatrixIdentity().operator D3DXMATRIX()));
	GData::ObjCamera->GetViewMatrix(&GData::MCamView);
	GData::DXDevice->SetTransform(D3DTS_VIEW, &(GData::MCamView.operator D3DXMATRIX()));
	GData::DXDevice->SetTransform(D3DTS_PROJECTION, &(GData::MLightProj.operator D3DXMATRIX()));
	GData::DXDevice->SetTransform(D3DTS_WORLD1, &((GData::MCamView * GData::MLightProj).operator D3DXMATRIX()));
	GData::DXDevice->SetTransform(D3DTS_WORLD2, &(SMMatrixTranspose((GData::MCamView * GData::MLightProj)).operator D3DXMATRIX()));

	if (!GetAsyncKeyState('Q'))
		GData::ObjCamera->ObjFrustum->Update(&(GData::MCamView), &(GData::MCamProj));

	GData::PlaneZCulling.a = 0;
	GData::PlaneZCulling.b = 0;
	GData::PlaneZCulling.c = 0;
	GData::PlaneZCulling.d = 0;

	GData::PlaneZCullingShader.a = 0;
	GData::PlaneZCullingShader.b = 0;
	GData::PlaneZCullingShader.c = 0;
	GData::PlaneZCullingShader.d = 0;

	GData::DXDevice->SetClipPlane(0, GData::PlaneZCulling);

	GData::ConstCurrCamDir = SMVector3Normalize(GData::ConstCurrCamDir);
	float3 pointplane = (GData::ConstCurrCamPos + ((GData::NearFar.y - 1.f) * GData::ConstCurrCamDir));
	D3DXPlaneFromPointNormal(
		&GData::PlaneZCulling,
		&((D3DXVECTOR3)pointplane),
		&D3DXVECTOR3(-GData::ConstCurrCamDir.x, -GData::ConstCurrCamDir.y, -GData::ConstCurrCamDir.z));
	float4x4 wmat = (GData::MCamView*GData::MLightProj);

	float determ = 0;
	wmat = SMMatrixTranspose(wmat);
	wmat = SMMatrixInverse(&determ, wmat);

	D3DXPlaneTransform(&GData::PlaneZCullingShader, &GData::PlaneZCulling, &(wmat.operator D3DXMATRIX()));
}

void SXRenderFunc::OutputDebugInfo(DWORD timeDelta)
{
	static DWORD FrameCount=0;
	static float TimeElapsed=0;
	static float FpsValue = 0;
	static char FpsStr[1024];
	static char debugstr[SXGC_STR_SIZE_DBG_MSG];

	FrameCount++;
	TimeElapsed += ((float)timeDelta) * 0.001f;

		if(TimeElapsed >= 1.0f)
		{
			
			FpsValue	= (float)FrameCount / TimeElapsed;
			sprintf(debugstr, "FPS %.1f\n", FpsValue);
			sprintf(debugstr + strlen(debugstr), "\ncount poly %d\n", Core_RIntGet(SGCORE_RI_INT_COUNT_POLY) / FrameCount);
			sprintf(debugstr + strlen(debugstr), "count DIPs %d\n\n", Core_RIntGet(SGCORE_RI_INT_COUNT_DIP) / FrameCount);
			sprintf(debugstr + strlen(debugstr), "Pos camera : [%.2f, %.2f, %.2f]\n", GData::ConstCurrCamPos.x, GData::ConstCurrCamPos.y, GData::ConstCurrCamPos.z);
			sprintf(debugstr + strlen(debugstr), "Dir camera : [%.2f, %.2f, %.2f]\n", GData::ConstCurrCamDir.x, GData::ConstCurrCamDir.y, GData::ConstCurrCamDir.z);

			TimeElapsed		= 0.0f;
			FrameCount		= 0;
		}
			
	SGCore_DbgMsg(debugstr);
}

////////

void SXRenderFunc::RenderInMRT(DWORD timeDelta)
{
	Core_RIntSet(RENDER_RI_INT_RENDERSTATE, RENDER_STATE_MATERIAL);
	GData::DXDevice->SetTransform(D3DTS_WORLD, &(SMMatrixIdentity().operator D3DXMATRIX()));

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	GData::DXDevice->SetClipPlane(0, GData::PlaneZCullingShader);
	GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);

	SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
	SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

	LPDIRECT3DSURFACE9 BackBuf, ColorSurf, NormalSurf, ParamSurf, DepthMapLinearSurf;

	SML_DSGetRT(DS_RT_COLOR)->GetSurfaceLevel(0, &ColorSurf);
	SML_DSGetRT(DS_RT_NORMAL)->GetSurfaceLevel(0, &NormalSurf);
	SML_DSGetRT(DS_RT_PARAM)->GetSurfaceLevel(0, &ParamSurf);
	SML_DSGetRT(DS_RT_DEPTH)->GetSurfaceLevel(0, &DepthMapLinearSurf);


	GData::DXDevice->SetRenderTarget(3, DepthMapLinearSurf);
	GData::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
	GData::DXDevice->SetRenderTarget(3, 0);

	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, ColorSurf);
	GData::DXDevice->SetRenderTarget(1, NormalSurf);
	GData::DXDevice->SetRenderTarget(2, ParamSurf);

	GData::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	GData::DXDevice->SetRenderTarget(3, DepthMapLinearSurf);

	/*SGCore_ShaderBind(0, GData::IDsShaders::VS::FreeGeometry);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::FreeGeometry);


	SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::FreeGeometry, "WorldViewProjection", &(SMMatrixTranspose(GData::MCamView * GData::MLightProj)));
	SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::FreeGeometry, "World", &(SMMatrixTranspose(SMMatrixIdentity())));
	SGCore_ShaderSetVRF(1, GData::IDsShaders::PS::FreeGeometry, "NearFar", &GData::NearFar);*/
	SGeom_ModelsRender(0);

	//SGCore_ShaderUnBind();

	if (SGeom_GreenGetCount() > 0)
	{
		GData::DXDevice->SetClipPlane(0, GData::PlaneZCullingShader);
		GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
		SGeom_GreenRender(timeDelta);
	}
	
	SML_LigthsRenderSource(-1, true, timeDelta);
	//GData::StaticGreen->GPURender(0);


	GData::DXDevice->SetRenderTarget(0, BackBuf);
	GData::DXDevice->SetRenderTarget(1, 0);
	GData::DXDevice->SetRenderTarget(2, 0);
	GData::DXDevice->SetRenderTarget(3, 0);

	mem_release(BackBuf);

	mem_release(ColorSurf);
	mem_release(NormalSurf);
	mem_release(ParamSurf);
	mem_release(DepthMapLinearSurf);

	GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
}


void SXRenderFunc::UpdateShadow(DWORD timeDelta)
{
	Core_RIntSet(RENDER_RI_INT_RENDERSTATE, RENDER_STATE_SHADOW);
	SML_LigthsComVisibleFrustumDistFor(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	SetSamplerFilter(0, D3DTEXF_LINEAR);
	SetSamplerAddress(0, D3DTADDRESS_WRAP);

	GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);


	for (int i = 0; i<SML_LigthsGetCount(); i++)
	{
		long tmpid = SML_LigthsGetIDOfKey(i);
		Core_RIntSet(RENDER_RI_INT_CURRIDLIGHT, tmpid);
		if (SML_LigthsIsShadow(tmpid) && (SML_LigthsComVisibleForFrustum(tmpid, GData::ObjCamera->ObjFrustum) && SML_LigthsIsEnable(tmpid)) /*|| (Data::Level::LightManager->Arr[i]->ShadowCube && Data::Level::LightManager->Arr[i]->ShadowCube->GetStatic() && !Data::Level::LightManager->Arr[i]->ShadowCube->GetUpdate())*/)
		{
			if (SML_LigthsGetType(tmpid) == LIGHTS_TYPE_GLOBAL)
			{
				//Data::Level::LightManager->Arr[i]->ShadowPSSM->Set4Or3Splits(Core::Data::Settings::IsRender4Or3SplitsForPSSM);
				SML_LigthsInRenderBegin(tmpid);
				int countsplits = (SML_LigthsGet4Or3SplitsG(tmpid) ? 4 : 3);
					for (int k = 0; k<countsplits; k++)
					{
						if (SML_LigthsAllowedRender(tmpid, k) && SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k) > -1)
						{
							SML_LigthsInRenderPre(tmpid, k);
							SGeom_ModelsRender(timeDelta, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k));
							SGeom_GreenRender(timeDelta, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k));

							SML_LigthsRenderAllExceptGroupSource(tmpid, timeDelta);
						}
					}

				//КОГДА ИСТОЧНИК БЛИЗОК К ГОРИЗОНТУ ИЗ-ЗА ОБЛАКОВ ВОЗНИКАЕТ БАГ С ТЕНЯМИ В ВИДЕ ФЕЙКОВЫХ ТЕНЕЙ
				if (true)
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
			else if (SML_LigthsGetType(tmpid) == LIGHTS_TYPE_DIRECTION)
			{
				if (SML_LigthsUpdateCountUpdate(tmpid, &GData::ConstCurrCamPos))
				{
					SML_LigthsInRenderBegin(tmpid);

					if (SML_LigthsGetTypeShadowed(tmpid) == LIGHTS_TYPE_SHADOWED_DYNAMIC && SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0) > -1)
					{
						SGeom_ModelsRender(timeDelta, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0));
							
						SGeom_GreenRender(timeDelta, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, 0));
					}
					else
					{
						SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, 0), &GData::ConstCurrCamPos);
						SGeom_ModelsRender(timeDelta);
						SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, 0), &GData::ConstCurrCamPos);
						SGeom_GreenRender(timeDelta);
					}

					SML_LigthsRenderAllExceptGroupSource(tmpid, timeDelta);
					SML_LigthsInRenderEnd(tmpid);
				}
			}
			else if (SML_LigthsGetType(tmpid) == LIGHTS_TYPE_POINT)
			{
				if (SML_LigthsUpdateCountUpdate(tmpid, &GData::ConstCurrCamPos))
				{
					SML_LigthsInRenderBegin(tmpid);

					for (int k = 0; k < 6; k++)
					{
						if (SML_LigthsGetEnableCubeEdge(tmpid, k))
						{
							SML_LigthsInRenderPre(tmpid, k);

							if (SML_LigthsGetTypeShadowed(tmpid) == LIGHTS_TYPE_SHADOWED_DYNAMIC && SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, 0) > -1)
							{
								SGeom_ModelsRender(timeDelta, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GEOM, k));
								SGeom_GreenRender(timeDelta, SML_LigthsGetIDArr(tmpid, RENDER_IDARRCOM_GREEN, k));
							}
							else
							{
								SGeom_ModelsComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos);
								SGeom_ModelsRender(timeDelta);
								SGeom_GreenComVisible(SML_LigthsGetFrustum(tmpid, k), &GData::ConstCurrCamPos);
								SGeom_GreenRender(timeDelta);
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
	Core_RIntSet(RENDER_RI_INT_RENDERSTATE, RENDER_STATE_FREE);
	GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

void SXRenderFunc::RenderSky(DWORD timeDelta)
{
	SetSamplerFilter(0, 2, D3DTEXF_ANISOTROPIC);

	

	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
	GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

	

	GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
	GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	GData::DXDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	GData::DXDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);

	GData::DXDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	GData::DXDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
	
	SGCore_SkyCloudsRender(timeDelta, &float3(GData::ConstCurrCamPos.x, GData::ConstCurrCamPos.y + 150, GData::ConstCurrCamPos.z), false);
	
	GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
	GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	SetSamplerAddress(0, 2, D3DTADDRESS_CLAMP);
	SGCore_SkyBoxRender(timeDelta, &float3(GData::ConstCurrCamPos.x, GData::ConstCurrCamPos.y + 40, GData::ConstCurrCamPos.z));



	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void SXRenderFunc::ComLighting(DWORD timeDelta, bool render_sky, bool blend_in_old)
{
	SGCore_ShaderUnBind();

	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	LPDIRECT3DSURFACE9 AmbientSurf, SpecDiffSurf, SpecDiffSurf2, BackBuf;
	SML_DSGetRT(DS_RT_AMBIENT_DIFF)->GetSurfaceLevel(0, &AmbientSurf);
	SML_DSGetRT(DS_RT_SPECULAR)->GetSurfaceLevel(0, &SpecDiffSurf);

	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, AmbientSurf);
	GData::DXDevice->SetRenderTarget(1, SpecDiffSurf);

	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	GData::DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	for (int i = 0; i<SML_LigthsGetCount(); i++)
	{
		long tmpid = SML_LigthsGetIDOfKey(i);
		if (SML_LigthsGetVisibleForFrustum(tmpid) && SML_LigthsIsEnable(tmpid))
		{
			DWORD idshader = GData::IDsShaders::PS::ComLightingNonShadow;

			if (SML_LigthsGetType(tmpid) != LIGHTS_TYPE_GLOBAL)
			{
				GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, FALSE);

				GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
				GData::DXDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);

				GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
				GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
				GData::DXDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

				GData::DXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
				GData::DXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
				GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

				GData::DXDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
				GData::DXDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_DECR);
				GData::DXDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP);

				GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 0x0);
				GData::DXDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
				GData::DXDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
				GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

				SML_LigthsRender(tmpid, 0);

				GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

				GData::DXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
				GData::DXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);
				GData::DXDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);
				GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

				GData::DXDevice->SetRenderState(D3DRS_STENCILREF, 255);
				GData::DXDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
				GData::DXDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
				GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
			}
			else
			{
				GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
				GData::DXDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
			}

			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

			GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			GData::DXDevice->GetRenderTarget(1, &SpecDiffSurf2);
			GData::DXDevice->SetRenderTarget(1, 0);
			mem_release_del(SpecDiffSurf2);
			mem_release_del(SpecDiffSurf);
			SetSamplerAddress(0, 6, D3DTADDRESS_CLAMP);
			GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);
			
			SML_LigthsNullingShadow();
			SML_LigthsGenShadow(tmpid);
			//SML_LigthsSoftShadow(false, 6);
			//GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
			SML_LigthsSoftShadow(false, 4);
			SML_LigthsSoftShadow(false, 2);
			//GData::DXDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
			GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			GData::DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

			SML_DSGetRT(DS_RT_SPECULAR)->GetSurfaceLevel(0, &SpecDiffSurf);
			GData::DXDevice->SetRenderTarget(1, SpecDiffSurf);

			if (SML_LigthsIsShadow(tmpid))
			{
				
				GData::DXDevice->SetTexture(4, SML_LigthsGetShadow());
				idshader = GData::IDsShaders::PS::ComLightingShadow;
			}

			
			GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);

			float determ = 0;
			float4x4 ViewInv = SMMatrixInverse(&determ, GData::MCamView);
			ViewInv = SMMatrixTranspose(ViewInv);

			SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::ResPos, "ViewInv", &ViewInv);
			SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::ResPos, "NearFar", &GData::NearFar);

			SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::ResPos, "ParamProj", &float3_t(GData::WinSize.x, GData::WinSize.y, GData::ProjFov));

			float3 tmpPosition;
			float2 tmpPowerDist;
			float3 tmpColor;
			SML_LigthsGetColor(tmpid, &tmpColor);
			SML_LigthsGetPos(tmpid, &tmpPosition, false);
			tmpPowerDist.x = SML_LigthsGetPowerDiv(tmpid);
			tmpPowerDist.y = SML_LigthsGetDist(tmpid);

			SGCore_ShaderSetVRF(1, idshader, "ViewPos", &GData::ConstCurrCamPos);
			SGCore_ShaderSetVRF(1, idshader, "LightPos", &(tmpPosition));
			SGCore_ShaderSetVRF(1, idshader, "LightPowerDist", &(tmpPowerDist));
			SGCore_ShaderSetVRF(1, idshader, "LightColor", &tmpColor);

			SGCore_ShaderBind(0, GData::IDsShaders::VS::ResPos);
			SGCore_ShaderBind(1, idshader);

			SetSamplerFilter(0, 6, D3DTEXF_LINEAR);
			SetSamplerAddress(0, 6, D3DTADDRESS_CLAMP);

			SetSamplerFilter(1, D3DTEXF_NONE);

			GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_COLOR));
			GData::DXDevice->SetTexture(1, SML_DSGetRT(DS_RT_NORMAL));
			GData::DXDevice->SetTexture(2, SML_DSGetRT(DS_RT_PARAM));
			GData::DXDevice->SetTexture(3, SML_DSGetRT(DS_RT_DEPTH));

			//GData::DXDevice->SetTexture(7, SGCore_RTGetTexture(GData::IDSRenderTargets::NormalScene));

			

			SGCore_ScreenQuadDraw();

			GData::DXDevice->SetVertexShader(0);
			GData::DXDevice->SetPixelShader(0);

			

			//GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);
			//SML_LigthsGenShadow2(i);
			//GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
			
			//////////////////////
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

	GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	SML_LigthsComHDR(timeDelta,30);
	GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);





	LPDIRECT3DSURFACE9 ComLightSurf;
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	SML_DSGetRT(DS_RT_SCENE_LIGHT_COM)->GetSurfaceLevel(0, &ComLightSurf);

	D3DSURFACE_DESC desc;
	ComLightSurf->GetDesc(&desc);

	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, ComLightSurf);

	/*if (blend_in_old)
	{
		GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

		GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else*/
		GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

	GData::DXDevice->SetTexture(0, SML_DSGetRT(DS_RT_COLOR));
	GData::DXDevice->SetTexture(1, SML_DSGetRT(DS_RT_AMBIENT_DIFF));
	GData::DXDevice->SetTexture(2, SML_DSGetRT(DS_RT_SPECULAR));
	//GData::DXDevice->SetTexture(3, SGCore_RTGetTexture(GData::IDSRenderTargets::ParamsScene));
	GData::DXDevice->SetTexture(4, SML_DSGetRT(DS_RT_ADAPTED_LUM_CURR)/*SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[0])*/);
	//GData::DXDevice->SetTexture(5, SML_LigthsGetShadow());

	//Data::ShadersManager->SetValueRegisterF(1,Data::EngineID::PS_BlendAmbientSpecDiffcolor,"MinIllumination",&(MinIllumination));

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::BlendAmbientSpecDiffcolor);
	//SGCore_ShaderSetVRF(1, GData::IDsShaders::PS::BlendAmbientSpecDiffcolor, "allcolor", &(allcolor));

	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	if (render_sky)
	{
		//DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		//DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		SXRenderFunc::RenderSky(timeDelta);
	}

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	/*if (SML_LigthsGetCount() > 0)
	{
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		GData::DXDevice->SetTransform(D3DTS_VIEW, &(GData::MCamView.operator D3DXMATRIX()));
		GData::DXDevice->SetTransform(D3DTS_PROJECTION, &(GData::MLightProj.operator D3DXMATRIX()));
		GData::DXDevice->SetTexture(0, 0);
		SML_LigthsRender(0, 0);
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	}*/

	mem_release(ComLightSurf);

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);

	
	
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void SXRenderFunc::GameRender(DWORD timeDelta)
{
	if (!IsIconic(GData::Handle3D))
		return;
	//@@@
	SSInput_Update();
	
	GData::ObjCamera->GetPosition(&GData::ConstCurrCamPos);
	GData::ObjCamera->GetLook(&GData::ConstCurrCamDir);

	//@@@
	CameraUpdate::UpdateEditorial(timeDelta);
	//@@@
	SXRenderFunc::UpdateView(timeDelta);

	GData::DXDevice->SetClipPlane(0, GData::PlaneZCulling);
	GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);

	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x888888, 1.0f, 0);
	GData::DXDevice->BeginScene();//подготовка к рисованию

	SGeom_ModelsComVisible(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);
	SGeom_GreenComVisible(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);

	GData::DXDevice->SetClipPlane(0, GData::PlaneZCulling);
	GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
	SGeom_ModelsRender(timeDelta);

	GData::DXDevice->SetClipPlane(0, GData::PlaneZCullingShader);
	GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
	SGeom_GreenRender(timeDelta);

	SXRenderFunc::OutputDebugInfo(timeDelta);

	GData::DXDevice->EndScene();	//заканчиваем отрисовку

	//@@@
	

	GData::DXDevice->Present(0, 0, 0, 0);	//выводим картинку в окно
}

////////

void SXRenderFunc::LevelEditorRender(DWORD timeDelta)
{
	Core_RIntSet(SGCORE_RI_INT_COUNT_POLY, 0);
	Core_RIntSet(SGCORE_RI_INT_COUNT_DIP, 0);

	if (GData::DXDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET || GData::ReSize)
	{
		if (!IsIconic(GData::Handle3D))
			SXRenderFunc::ComDeviceLost();
		return;
	}

	//@@@
	SSInput_Update();

	if (SSInput_GetKeyState(DIK_Y))
		SGCore_ShaderReloadAll();

	

	//@@@
	CameraUpdate::UpdateEditorial(timeDelta);

	GData::ObjCamera->GetPosition(&GData::ConstCurrCamPos);
	GData::ObjCamera->GetLook(&GData::ConstCurrCamDir);
	//@@@
	SXRenderFunc::UpdateView(timeDelta);
	SML_LigthsUpdate(&GData::ConstCurrCamPos,&GData::MCamView);
	//GData::DXDevice->SetClipPlane(0, GData::PlaneZCulling);
	//GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);

	//GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x888888, 1.0f, 0);
	GData::DXDevice->BeginScene();//подготовка к рисованию
	UpdateShadow(timeDelta);
	if (SGeom_ModelsGetCount() > 0)
		SGeom_ModelsComVisible(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);

	if (SGeom_GreenGetCount() > 0)
		SGeom_GreenComVisible(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);

	
	RenderInMRT(0);

	ComLighting(timeDelta, true, false);

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::ScreenOut);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GData::DXDevice->SetTexture(0, SML_DSGetRT((GetAsyncKeyState('Q') ? DS_RT_NORMAL : DS_RT_SCENE_LIGHT_COM)));
	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();

	/*DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	SXRenderFunc::RenderSky(timeDelta);*/

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);


	/*if (GData::Geometry->GetCountModel() > 0)
		GData::Geometry->GPURender(timeDelta);*/

	//reportf(0, "`````````````````````````\n");
	/*DXDevice->SetTexture(0, 0);
	DXDevice->SetStreamSource(0, vb, 0, sizeof(float3_t));
	DXDevice->SetIndices(ib);
	DXDevice->SetFVF(D3DFVF_XYZ);
	DXDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0, 0, count_vertex, 0, count_index/3);*/

	

	SXRenderFunc::OutputDebugInfo(timeDelta);

	GData::DXDevice->EndScene();	//заканчиваем отрисовку

	GData::DXDevice->Present(0, 0, 0, 0);	//выводим картинку в окно


	/*if (GetAsyncKeyState(VK_NUMPAD1))
	{
		D3DXSaveTextureToFile("C:\\1\\color", D3DXIFF_PNG, SGCore_RTGetTexture(GData::IDSRenderTargets::ColorScene), NULL);
		D3DXSaveTextureToFile("C:\\1\\normal", D3DXIFF_PNG, SGCore_RTGetTexture(GData::IDSRenderTargets::NormalScene), NULL);
		D3DXSaveTextureToFile("C:\\1\\depth", D3DXIFF_PNG, SGCore_RTGetTexture(GData::IDSRenderTargets::DepthScene), NULL);
		D3DXSaveTextureToFile("C:\\1\\param", D3DXIFF_PNG, SGCore_RTGetTexture(GData::IDSRenderTargets::ParamsScene), NULL);
	}*/

	SXRenderFunc::UpdateLight();
}

void SXRenderFunc::RFuncDIP(UINT type_primitive, long base_vertexIndex, UINT min_vertex_index, UINT num_vertices, UINT start_index, UINT prim_count)
{

}

void SXRenderFunc::RFuncSetMtl(UINT id, float4x4* world)
{
	if (Core_RIntGet(RENDER_RI_INT_RENDERSTATE) == RENDER_STATE_SHADOW)
	{
		SML_MtlSetMainTexture(0, id);
		SML_LigthsShadowSetShaderOfTypeMat(Core_RIntGet(RENDER_RI_INT_CURRIDLIGHT), SML_MtlGetType(id), world);
	}
	else if (Core_RIntGet(RENDER_RI_INT_RENDERSTATE) == RENDER_STATE_FREE)
	{
		SML_MtlSetMainTexture(0, id);
		GData::DXDevice->SetTransform(D3DTS_WORLD, &((world ? (*world) : SMMatrixIdentity()).operator D3DXMATRIX()));
	}
	else if (Core_RIntGet(RENDER_RI_INT_RENDERSTATE) == RENDER_STATE_MATERIAL)
		SML_MtlRender(id, world);
}

long SXRenderFunc::RFuncLoadMtl(const char* name, int mtl_type)
{
	return SML_MtlLoad(name, mtl_type);
}