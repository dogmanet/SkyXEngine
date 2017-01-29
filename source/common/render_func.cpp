
#include <common\\render_func.h>

HRESULT GetSampleOffsets_DownScale4x4(DWORD dwWidth, DWORD dwHeight, float2 avSampleOffsets[])
{
	if (NULL == avSampleOffsets)
		return E_INVALIDARG;

	float tU = 1.0f / dwWidth;
	float tV = 1.0f / dwHeight;

	// Sample from the 16 surrounding points. Since the center point will be in
	// the exact center of 16 texels, a 0.5f offset is needed to specify a texel
	// center.
	int index = 0;
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			avSampleOffsets[index].x = (x - 1.5f) * tU;
			avSampleOffsets[index].y = (y - 1.5f) * tV;

			index++;
		}
	}

	return S_OK;
}



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
	mem_release_del(GData::ComLightSurf1x1);
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
			GData::DXDevice->CreateOffscreenPlainSurface(1, 1, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &GData::ComLightSurf1x1, 0);
		
			g_pTexAdaptedLuminanceLast = SGCore_RTGetTexture(GData::IDSRenderTargets::AdaptLumLast);
			g_pTexAdaptedLuminanceCur = SGCore_RTGetTexture(GData::IDSRenderTargets::AdaptLumCurr);
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

	//SetRenderFilter();
	SetSamplerFilter(0, 16, D3DTEXF_LINEAR);
	SetSamplerAddress(0, 16, D3DTADDRESS_WRAP);

	LPDIRECT3DSURFACE9 BackBuf, ColorSurf, NormalSurf, ParamSurf, DepthMapLinearSurf;

	SGCore_RTGetTexture(GData::IDSRenderTargets::ColorScene)->GetSurfaceLevel(0, &ColorSurf);
	SGCore_RTGetTexture(GData::IDSRenderTargets::NormalScene)->GetSurfaceLevel(0, &NormalSurf);
	SGCore_RTGetTexture(GData::IDSRenderTargets::ParamsScene)->GetSurfaceLevel(0, &ParamSurf);
	SGCore_RTGetTexture(GData::IDSRenderTargets::DepthScene)->GetSurfaceLevel(0, &DepthMapLinearSurf);


	GData::DXDevice->SetRenderTarget(3, DepthMapLinearSurf);
	GData::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
	GData::DXDevice->SetRenderTarget(3, 0);

	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, ColorSurf);
	GData::DXDevice->SetRenderTarget(1, NormalSurf);
	GData::DXDevice->SetRenderTarget(2, ParamSurf);

	GData::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	GData::DXDevice->SetRenderTarget(3, DepthMapLinearSurf);

	SGCore_ShaderBind(0, GData::IDsShaders::VS::FreeGeometry);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::FreeGeometry);


	SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::FreeGeometry, "WorldViewProjection", &(SMMatrixTranspose(GData::MCamView * GData::MLightProj)));
	SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::FreeGeometry, "World", &(SMMatrixTranspose(SMMatrixIdentity())));
	SGCore_ShaderSetVRF(1, GData::IDsShaders::PS::FreeGeometry, "NearFar", &GData::NearFar);
	SGeom_ModelsRender(0);
	SGCore_ShaderUnBind();
	
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
	//SetRenderFilterUn();
}


void SXRenderFunc::UpdateShadow(DWORD timeDelta)
{
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
		if (SML_LigthsIsShadow(i) && (SML_LigthsComVisibleForFrustum(i, GData::ObjCamera->ObjFrustum) && SML_LigthsIsEnable(i)) /*|| (Data::Level::LightManager->Arr[i]->ShadowCube && Data::Level::LightManager->Arr[i]->ShadowCube->GetStatic() && !Data::Level::LightManager->Arr[i]->ShadowCube->GetUpdate())*/)
		{
			if (SML_LigthsGetType(i) == LIGHTS_TYPE_GLOBAL)
			{
				//Data::Level::LightManager->Arr[i]->ShadowPSSM->Set4Or3Splits(Core::Data::Settings::IsRender4Or3SplitsForPSSM);
				SML_LigthsInRenderBegin(i);
					float4 viewpos;
					SML_LigthsGetPosW(i, &viewpos);
					GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
					for (int k = 0; k<4; k++)
					{
						SML_LigthsUpdateG(i, k, &GData::ConstCurrCamPos, &GData::ConstCurrCamDir);
						SGeom_ModelsComVisible(SML_LigthsGetFrustumG(i, k), &GData::ConstCurrCamPos);
						SGeom_ModelsRender(timeDelta);
					}
					GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
					SML_LigthsInRenderEnd(i);
			}
			else if (SML_LigthsGetType(i) == LIGHTS_TYPE_DIRECTION)
			{
				float4 viewpos;
				SML_LigthsGetPosW(i, &viewpos);

				SML_LigthsInRenderBegin(i);

				SGeom_ModelsComVisible(SML_LigthsGetFrustum(i), &GData::ConstCurrCamPos);
				SGeom_ModelsRender(timeDelta);

				SML_LigthsInRenderEnd(i);
			}
			else if (SML_LigthsGetType(i) == LIGHTS_TYPE_POINT)
			{
				SML_LigthsInRenderBegin(i);
				float4 viewpos;
				SML_LigthsGetPosW(i, &viewpos);

				for (int k = 0; k<6; k++)
				{
					SML_LigthsInRenderPre(i, k);

					SGeom_ModelsComVisible(SML_LigthsGetFrustum(i), &GData::ConstCurrCamPos);
					SGeom_ModelsRender(timeDelta);

					SML_LigthsInRenderPost(i, k);
				}

				SML_LigthsInRenderEnd(i);
			}
		}
	}
	GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}

void SXRenderFunc::RenderShadow(DWORD timeDelta)
{
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED);

	for (int i = 0; i<SML_LigthsGetCount(); i++)
	{
		if (SML_LigthsIsShadow(i) && (SML_LigthsComVisibleForFrustum(i, GData::ObjCamera->ObjFrustum) && SML_LigthsIsEnable(i)) /*|| (Data::Level::LightManager->Arr[i]->ShadowCube && Data::Level::LightManager->Arr[i]->ShadowCube->GetStatic() && !Data::Level::LightManager->Arr[i]->ShadowCube->GetUpdate())*/)
		{
			/*if (SML_LigthsGetType(i) == LIGHTS_TYPE_DIRECTION)
			{	
				SML_LigthsGenShadow(i);		
			}
			else if (SML_LigthsGetType(i) == LIGHTS_TYPE_POINT)
			{*/
				SML_LigthsGenShadow(i);
				//SML_LigthsSoftShadow(i, true, 5);
				//SML_LigthsSoftShadow(i, false, 2);
			//}
		}
	}
	GData::DXDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
}


void SXRenderFunc::ComLighting(DWORD timeDelta, bool render_sky, bool blend_in_old)
{
	SGCore_ShaderUnBind();

	GData::DXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	LPDIRECT3DSURFACE9 AmbientSurf, SpecDiffSurf, BackBuf;
	SGCore_RTGetTexture(GData::IDSRenderTargets::LightAmbient)->GetSurfaceLevel(0, &AmbientSurf);
	SGCore_RTGetTexture(GData::IDSRenderTargets::LightSpecDiff)->GetSurfaceLevel(0, &SpecDiffSurf);

	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, AmbientSurf);
	GData::DXDevice->SetRenderTarget(1, SpecDiffSurf);

	SetSamplerFilter(0, 6, D3DTEXF_LINEAR);
	SetSamplerAddress(0, 6, D3DTADDRESS_CLAMP);

	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::ColorScene));
	GData::DXDevice->SetTexture(1, SGCore_RTGetTexture(GData::IDSRenderTargets::NormalScene));
	GData::DXDevice->SetTexture(2, SGCore_RTGetTexture(GData::IDSRenderTargets::ParamsScene));
	GData::DXDevice->SetTexture(3, SGCore_RTGetTexture(GData::IDSRenderTargets::DepthScene));

	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	GData::DXDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	for (int i = 0; i<SML_LigthsGetCount(); i++)
	{
		if (true
			//(Data::Level::LightManager->Arr[i]->GetVisible() && SML_LigthsIsEnable(i))
			)
		{
			DWORD idshader = GData::IDsShaders::PS::ComLightingNonShadow;
			if (SML_LigthsGetType(i) != LIGHTS_TYPE_GLOBAL)
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

				SML_LigthsRender(i, 0);
				//Core::Data::Statistics::CountPoly += Data::Level::LightManager->Arr[i]->Mesh->GetNumFaces();
				//SkyXEngine::Core::Data::Statistics::CountDips += 1;


				GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

				GData::DXDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
				GData::DXDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);
				GData::DXDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);
				GData::DXDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_ZERO);

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

			GData::DXDevice->SetTexture(4, SML_LigthsGetShadow(i));

			/*if (Data::Level::LightManager->Arr[i]->IsShadow)
			{
				if (
					(SkyXEngine::Core::Data::Settings::UseShadowGlobal && Data::Level::LightManager->Arr[i]->IsGlobal) ||
					(SkyXEngine::Core::Data::Settings::UseShadowLocal && !Data::Level::LightManager->Arr[i]->IsGlobal)
					)
				{
					GData::DXDevice->SetTexture(4, Data::Level::LightManager->Arr[i]->GetShadow());
					idshader = Data::EngineID::PS_ComLigthingShadowed;
				}
				else
				{
					SkyXEngine::GData::DXDevice->SetTexture(4, Core::Data::LoadedTextures->GetTexture(SkyXEngine::Core::Data::EngineID::Tex_White));
				}
			}*/

			GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
			float determ = 0;
			float4x4 ViewInv = SMMatrixInverse(&determ, GData::MCamView);
			ViewInv = SMMatrixTranspose(ViewInv);

			SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::ResPos, "ViewInv", &ViewInv);
			SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::ResPos, "NearFar", &GData::NearFar);

			SGCore_ShaderSetVRF(0, GData::IDsShaders::VS::ResPos, "ParamProj", &float3_t(GData::WinSize.x, GData::WinSize.y, D3DX_PI / 4.0f));

			//float MinIllumination = 0.1f;

			float4 tmpPosition;
			float3 tmpColor;// = float4(Data::Level::LightManager->Arr[i]->Color.x, Data::Level::LightManager->Arr[i]->Color.y, Data::Level::LightManager->Arr[i]->Color.z, Data::Level::LightManager->Arr[i]->Color.w/**Data::Level::LightManager->Arr[2]->Color.w*/);
			SML_LigthsGetColor(i, &tmpColor);
			SML_LigthsGetPosW(i, &tmpPosition);

			/*if(Data::Level::LightManager->Arr[i]->IsGlobal)
			MinIllumination = tmpColor.w * (0.5 + (1.f-tmpColor.w)*0.3);*/
			float MinIllumination = 0.3;
			SGCore_ShaderSetVRF(1, idshader, "ViewPos", &GData::ConstCurrCamPos);
			SGCore_ShaderSetVRF(1, idshader, "LightPos", &(tmpPosition));
			SGCore_ShaderSetVRF(1, idshader, "LightColor", &tmpColor);
			SGCore_ShaderSetVRF(1, idshader, "MinIllumination", &(MinIllumination));

			SGCore_ShaderBind(0, GData::IDsShaders::VS::ResPos);
			SGCore_ShaderBind(1, idshader);

			GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

			SGCore_ScreenQuadDraw();

			GData::DXDevice->SetVertexShader(0);
			GData::DXDevice->SetPixelShader(0);

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

	//LPDIRECT3DSURFACE9 ComLightSurf1x1;
	//SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom_1x1)->GetSurfaceLevel(0, &ComLightSurf1x1);
	//SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom)->GetSurfaceLevel(SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom)->GetLevelCount() - 1, &ComLightSurf1x1);
	//SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom)->SetLOD()

	/*LPDIRECT3DSURFACE9 SurfD;

	SGCore_RTGetTexture(GData::IDSRenderTargets::LightSpecDiffD2)->GetSurfaceLevel(0, &SurfD);

	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, SurfD);
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::ScreenOut);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::LightSpecDiff));
	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();
	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);
	mem_release(SurfD);*/





	/*SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom_1x1)->GetSurfaceLevel(0, &SurfD);

	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, SurfD);

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::ScreenOut);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::LigthComD2));
	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);
	mem_release(SurfD);*/



	LPDIRECT3DSURFACE9	DepthInvSurf;
	HRESULT hr = SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[0])->GetSurfaceLevel(0, &DepthInvSurf);
	
	hr = GData::DXDevice->GetRenderTargetData(DepthInvSurf, GData::ComLightSurf1x1);
	if (hr == D3DERR_DRIVERINTERNALERROR)
		int qwert = 0;
	else if (hr == D3DERR_DEVICELOST)
		int qwert = 0;
	else if (hr == D3DERR_INVALIDCALL)
		int qwert = 0;
	D3DSURFACE_DESC tmpdesc;
	SGCore_RTGetTexture(GData::IDSRenderTargets::LightSpecDiffD2)->GetLevelDesc(0, &tmpdesc);

	
	float allcolor = 0;
	D3DLOCKED_RECT  srect;
	GData::ComLightSurf1x1->LockRect(&srect, 0, D3DLOCK_READONLY);

	allcolor = *(((float*)srect.pBits) + 0*tmpdesc.Width + 0);

	/*for (int x = 0; x < tmpdesc.Width; ++x)
	{
		for (int y = 0; y < tmpdesc.Height; ++y)
		{
			
			allcolor += *(((float*)srect.pBits) + y*tmpdesc.Width + x);
			++countpix;
		}
	}*/
	//memcpy(&allcolor, srect.pBits, sizeof(DWORD));
	GData::ComLightSurf1x1->UnlockRect();
	//allcolor /= (float(countpix));

	DepthInvSurf->Release();
	






	float2 avSampleOffsets[16];
	GetSampleOffsets_DownScale4x4(GData::WinSize.x, GData::WinSize.y, avSampleOffsets);

	LPDIRECT3DSURFACE9 SurfSceneScale;

	SGCore_RTGetTexture(GData::IDSRenderTargets::LigthComScaled)->GetSurfaceLevel(0, &SurfSceneScale);

	GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	GData::DXDevice->SetRenderTarget(0, SurfSceneScale);
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::SampleLumIterative);
	SGCore_ShaderSetVRF(1, GData::IDsShaders::PS::SampleLumIterative, "g_avSampleOffsets", &(avSampleOffsets));

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom));
	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();

	mem_release(SurfSceneScale);
	/*GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);
	mem_release(SurfSceneScale);*/





#define NUM_TONEMAP_TEXTURES 4

	DWORD dwCurTexture = NUM_TONEMAP_TEXTURES - 1;

	// Sample log average luminance
	PDIRECT3DSURFACE9 apSurfToneMap[NUM_TONEMAP_TEXTURES] = { 0 };

	// Retrieve the tonemap surfaces
	for (int i = 0; i < NUM_TONEMAP_TEXTURES; i++)
	{
		SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[i])->GetSurfaceLevel(0, &apSurfToneMap[i]);
	}

	D3DSURFACE_DESC desc;
	SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[dwCurTexture])->GetLevelDesc(0, &desc);


	// Initialize the sample offsets for the initial luminance pass.
	float tU, tV;
	tU = 1.0f / (3.0f * desc.Width);
	tV = 1.0f / (3.0f * desc.Height);

	int index = 0;
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			avSampleOffsets[index].x = x * tU;
			avSampleOffsets[index].y = y * tV;

			index++;
		}
	}

	GData::DXDevice->SetRenderTarget(0, apSurfToneMap[dwCurTexture]);
	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::LigthComScaled));
	GData::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GData::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GData::DXDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	GData::DXDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	//GData::DXDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &BackBuf);
	//GData::DXDevice->SetRenderTarget(0, SurfSceneScale);
	//GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::SampleLumInit);
	SGCore_ShaderSetVRF(1, GData::IDsShaders::PS::SampleLumInit, "g_avSampleOffsets", &(avSampleOffsets));

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();
	mem_release(apSurfToneMap[dwCurTexture]);

	--dwCurTexture;
	while (dwCurTexture > 0)
	{
		SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[dwCurTexture + 1])->GetLevelDesc(0, &desc);
		GetSampleOffsets_DownScale4x4(desc.Width, desc.Height, avSampleOffsets);

		GData::DXDevice->SetRenderTarget(0, apSurfToneMap[dwCurTexture]);
		GData::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		GData::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);


		SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
		SGCore_ShaderBind(1, GData::IDsShaders::PS::SampleLumIterative);
		SGCore_ShaderSetVRF(1, GData::IDsShaders::PS::SampleLumIterative, "g_avSampleOffsets", &(avSampleOffsets));

		GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[dwCurTexture + 1]));
		SGCore_ScreenQuadDraw();

		GData::DXDevice->SetVertexShader(0);
		GData::DXDevice->SetPixelShader(0);

		SGCore_ShaderUnBind();
		//mem_release(apSurfToneMap[dwCurTexture]);
		dwCurTexture--;
	}


	SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[1])->GetLevelDesc(0, &desc);
	GetSampleOffsets_DownScale4x4(desc.Width, desc.Height, avSampleOffsets);


	GData::DXDevice->SetRenderTarget(0, apSurfToneMap[0]);
	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[1]));
	GData::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	GData::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::SampleLumFinal);
	SGCore_ShaderSetVRF(1, GData::IDsShaders::PS::SampleLumFinal, "g_avSampleOffsets", &(avSampleOffsets));

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();


	for (int i = 0; i < NUM_TONEMAP_TEXTURES; i++)
	{
		mem_release(apSurfToneMap[i]);
	}






	
	

	PDIRECT3DTEXTURE9 pTexSwap = g_pTexAdaptedLuminanceLast;
	g_pTexAdaptedLuminanceLast = g_pTexAdaptedLuminanceCur;
	g_pTexAdaptedLuminanceCur = pTexSwap;

	PDIRECT3DSURFACE9 pSurfAdaptedLum = NULL;
	g_pTexAdaptedLuminanceCur->GetSurfaceLevel(0, &pSurfAdaptedLum);

	GData::DXDevice->SetRenderTarget(0, pSurfAdaptedLum);
	GData::DXDevice->SetTexture(0, g_pTexAdaptedLuminanceLast);
	GData::DXDevice->SetTexture(1, SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[0]));
	GData::DXDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	GData::DXDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	GData::DXDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	GData::DXDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_POINT);

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::CalcAdaptedLum);
	
	float ElapsedTime = float(timeDelta) * 0.001f;
	SGCore_ShaderSetVRF(1, GData::IDsShaders::PS::CalcAdaptedLum, "g_fElapsedTime", &(ElapsedTime));

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();
	mem_release(pSurfAdaptedLum);

	GData::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release(BackBuf);
	






	


	LPDIRECT3DSURFACE9 ComLightSurf;
	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom)->GetSurfaceLevel(0, &ComLightSurf);

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

	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::ColorScene));
	GData::DXDevice->SetTexture(1, SGCore_RTGetTexture(GData::IDSRenderTargets::LightAmbient));
	GData::DXDevice->SetTexture(2, SGCore_RTGetTexture(GData::IDSRenderTargets::LightSpecDiff));
	GData::DXDevice->SetTexture(3, g_pTexAdaptedLuminanceCur/*SGCore_RTGetTexture(GData::IDSRenderTargets::ToneMaps[0])*/);

	//Data::ShadersManager->SetValueRegisterF(1,Data::EngineID::PS_BlendAmbientSpecDiffcolor,"MinIllumination",&(MinIllumination));

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::BlendAmbientSpecDiffcolor);
	//SGCore_ShaderSetVRF(1, GData::IDsShaders::PS::BlendAmbientSpecDiffcolor, "allcolor", &(allcolor));

	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);
	mem_release(ComLightSurf);



	/*SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom)->GetSurfaceLevel(0, &ComLightSurf);
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GData::DXDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	GData::DXDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
	GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GData::DXDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	GData::DXDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 255), 1.0f, 0);

	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom2));

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::ScreenOut);

	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);
	GData::DXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/
	/*if (render_sky && Data::EngineID::Light_Global != -1)
	{
		GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
		SkyXEngine::Core::Render::RenderSky(timeDelta);
		SkyXEngine::Core::Data::Statistics::CountDips += 1;
	}*/

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

	
	RenderShadow(timeDelta);

	ComLighting(timeDelta, false, false);

	GData::DXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	GData::DXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	SGCore_ShaderBind(0, GData::IDsShaders::VS::ScreenOut);
	SGCore_ShaderBind(1, GData::IDsShaders::PS::ScreenOut);

	GData::DXDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	GData::DXDevice->SetTexture(0, SGCore_RTGetTexture(GData::IDSRenderTargets::LigthCom));
	SGCore_ScreenQuadDraw();

	GData::DXDevice->SetVertexShader(0);
	GData::DXDevice->SetPixelShader(0);

	SGCore_ShaderUnBind();
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

	if (SGeom_GreenGetCount() > 0)
	{
		GData::DXDevice->SetClipPlane(0, GData::PlaneZCullingShader);
		GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
		SGeom_GreenRender(timeDelta);
	}

	SXRenderFunc::OutputDebugInfo(timeDelta);

	GData::DXDevice->EndScene();	//заканчиваем отрисовку

	GData::DXDevice->Present(0, 0, 0, 0);	//выводим картинку в окно
}