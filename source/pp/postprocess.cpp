
namespace SkyXEngine
{
namespace Graphics
{
namespace PostProcess
{

void RenderFog()
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Render::GetCurrentSendTexture()->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	Core::Data::Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	Core::Data::Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	Core::Data::Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Core::Data::Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

	Core::Render::SetSamplerFilter(0,2,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(0,2,D3DTADDRESS_CLAMP);
			
	//Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());
	Core::Data::Device->SetTexture(1,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ,Core::Data::View);
	ViewInv = SMMatrixTranspose(ViewInv);

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ViewInv",&ViewInv);
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"NearFar",&Core::Data::NearFar);

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ParamProj",&float3_t(Core::Data::WidthHeight.x,Core::Data::WidthHeight.y,D3DX_PI / 4.0f));

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPFogFree,"ViewPos",&Core::Data::ConstCurrentCameraPosition);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPFogFree,"NearFar",&Core::Data::NearFar);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPFogFree,"FogColor",&FogColor);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPFogFree,"Param",&FogParam);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ResPosDepth);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPFogFree);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);

	RenderSurf->Release();
	BackBuf->Release();

	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//Core::Render::SetIncrCurrentTex();
}

void UpdateSunEffects()
{
	SkyXEngine::Graphics::PostProcess::OrientationSun = 0.f;
		if(Core::Data::EngineID::Light_Global != -1)
		{
			float4 posglight;
			Core::Data::Level::LightManager->Arr[Core::Data::EngineID::Light_Global]->GetPositionSys(&posglight);
			float4 sun_pos(
								posglight.x+Core::Data::ConstCurrentCameraPosition.x,
								posglight.y+Core::Data::ConstCurrentCameraPosition.y,
								posglight.z+Core::Data::ConstCurrentCameraPosition.z,1.0);
			float3 sunPos;
			sunPos.x = posglight.x;
			sunPos.y = posglight.y;
			sunPos.z = posglight.z;

			float4x4 viewproj = Core::Data::View * Core::Data::Projection;

			sun_pos = SMVector4Transform(sun_pos,viewproj);
			//sunPos = sun_pos;

			sun_pos.x /= sun_pos.w;
			sun_pos.y /= sun_pos.w;
			sun_pos.z /= sun_pos.w;
			sun_pos.w /= sun_pos.w;

			sun_pos.x *= 0.5;
			sun_pos.y *= -0.5;

			sun_pos.x += 0.5;
			sun_pos.y += 0.5;

			OrientationSun = 0;

			float3 tmpVec(Core::Data::ConstCurrentCameraDirection.x, Core::Data::ConstCurrentCameraDirection.y, Core::Data::ConstCurrentCameraDirection.z);
			tmpVec = SMVector3Normalize(tmpVec);
			
			float3 dirVec = sunPos - Core::Data::ConstCurrentCameraPosition;
			//dirVec.y = 0;
			dirVec = SMVector3Normalize(dirVec);
			SkyXEngine::Graphics::PostProcess::OrientationSun = abs(acosf(SMVector3Dot(tmpVec, dirVec)) * sign(SMVector3Cross(tmpVec, dirVec).y));
			
			sun_pos.w = SkyXEngine::Graphics::PostProcess::OrientationSun;

			SunPos = sun_pos;
		}
}

void RenderSun()
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerFilter(0,4,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(0,4,D3DTADDRESS_CLAMP);

	D3DSURFACE_DESC desc;
	Core::Data::LoadedTextures->GetTexture(IDTexSun)->GetLevelDesc(0,&desc);

	float2_t SizeMapSun = float2_t(desc.Width,desc.Height);

	RECT rectscissor;
	rectscissor.top = SunPos.y * Core::Data::WidthHeight.y - desc.Height*0.5f;
	rectscissor.left = SunPos.x * Core::Data::WidthHeight.x - desc.Width*0.5f;
	rectscissor.bottom = rectscissor.top + desc.Height;
	rectscissor.right = rectscissor.left + desc.Width;

	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPQuadRender);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);

	RenderSurf->Release();
	BackBuf->Release();



	Core::Render::GetCurrentSendTexture()->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
	Core::Data::Device->SetTexture(2,Core::Data::LoadedTextures->GetTexture(IDTexSun));
	Core::Data::Device->SetTexture(3,Core::Render::GetCurrentRenderTexture());
	

	Core::Data::Device->SetScissorRect(&rectscissor);
	Core::Data::Device->SetRenderState(D3DRS_SCISSORTESTENABLE,TRUE);

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSunRender,"Color",&SunColor);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSunRender,"LightPos",&SunPos);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSunRender,"SizeMap",&(Core::Data::WidthHeight));
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSunRender,"SizeTexSun",&SizeMapSun);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSunRender,"PixelSize",&float2_t(1.f/Core::Data::WidthHeight.x,1.f/Core::Data::WidthHeight.y));
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSunRender,"LightColor",&Core::Data::Level::LightManager->Arr[Core::Data::EngineID::Light_Global]->Color);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPSunRender);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderState(D3DRS_SCISSORTESTENABLE,FALSE);

	Core::Data::Device->SetRenderTarget(0,BackBuf);

	RenderSurf->Release();
	BackBuf->Release();
	
	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	//Core::Render::SetIncrCurrentTex();
}

void RenderSSR()
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerFilter(0,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);
	Core::Render::SetSamplerFilter(1,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(1,D3DTADDRESS_CLAMP);
	Core::Render::SetSamplerFilter(2,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(2,D3DTADDRESS_WRAP);
	Core::Render::SetSamplerFilter(3,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(3,D3DTADDRESS_CLAMP);
			
	//Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());
	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());
	Core::Data::Device->SetTexture(1,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSNormal));
	Core::Data::Device->SetTexture(2,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
	Core::Data::Device->SetTexture(3,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSParam));

	float4x4 vp = SMMatrixTranspose(Core::Data::View * Core::Data::ProjectionStencil);
	float determ = 0;
	float4x4 ViewInv = SMMatrixTranspose(SMMatrixInverse(&determ,Core::Data::View));
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSSR,"CameraPosition",&Core::Data::ConstCurrentCameraPosition);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSSR,"ViewProjection",&vp);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSSR,"Param",&SSRParam);
	//Core::Data::ShadersManager->SetValueRegisterF(1,"pp_ssr","NearFar",&Core::Data::NearFar);

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ViewInv",&ViewInv);
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"NearFar",&Core::Data::NearFar);
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ParamProj",&float3_t(Core::Data::WidthHeight.x,Core::Data::WidthHeight.y,Core::Data::Settings::ProjFov));

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ResPosDepth);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPSSR);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);

	RenderSurf->Release();
	BackBuf->Release();


	Core::Render::GetCurrentSendTexture()->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentRenderTexture());

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPQuadRender);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);

	RenderSurf->Release();
	BackBuf->Release();
}

void RenderSSAO()
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Data::CreateTexrures->GetTexture("finalscene_1")->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

	Core::Render::SetSamplerFilter(0,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(0,D3DTADDRESS_CLAMP);
	Core::Render::SetSamplerFilter(1,D3DTEXF_POINT);
	Core::Render::SetSamplerAddress(1,D3DTADDRESS_CLAMP);
	Core::Render::SetSamplerFilter(2,D3DTEXF_POINT);
	Core::Render::SetSamplerAddress(2,D3DTADDRESS_WRAP);
			
	//Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());
	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
	Core::Data::Device->SetTexture(1,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSNormal));
	Core::Data::Device->SetTexture(2,SkyXEngine::Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_NoiseTex));

	DWORD how_id_ssao = 0;

		if(Core::Data::Settings::HowQualitySSAO == 2)
			how_id_ssao = Core::Data::EngineID::PS_PPSSAO24;
		else if(Core::Data::Settings::HowQualitySSAO == 1)
			how_id_ssao = Core::Data::EngineID::PS_PPSSAO16;
		else
			how_id_ssao = Core::Data::EngineID::PS_PPSSAO8;
	Core::Data::ShadersManager->SetValueRegisterF(1,how_id_ssao,"NearFar",&Core::Data::WidthHeight);
	Core::Data::ShadersManager->SetValueRegisterF(1,how_id_ssao,"Params",&float4(3.f/(Core::Data::WidthHeight.x + Core::Data::WidthHeight.y),SSAOParam.y,SSAOParam.z,SSAOParam.w));
	Core::Data::ShadersManager->SetValueRegisterF(1,how_id_ssao,"ArrRndVecSSAO",&Core::Data::ArrRndVecSSAO);
	float scale_rnd = Core::Data::WidthHeight.x * 0.25f;
	Core::Data::ShadersManager->SetValueRegisterF(1,how_id_ssao,"ScaleRnd",&scale_rnd);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,how_id_ssao);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);

	RenderSurf->Release();
	BackBuf->Release();




	Core::Data::CreateTexrures->GetTexture("finalscene_2")->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
	Core::Data::Device->SetTexture(1,Core::Data::CreateTexrures->GetTexture("finalscene_1"));
	Core::Data::Device->SetTexture(2,Core::Render::GetCurrentSendTexture());

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSSAOBlur1,"PixelSize",&float2_t(2.f/Core::Data::WidthHeight.x,2.f/Core::Data::WidthHeight.y));
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSSAOBlur1,"NearFar",&Core::Data::NearFar);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPSSAOBlur2);

	/*Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture("finalscene3"));

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPQuadRender);*/

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);

	RenderSurf->Release();
	BackBuf->Release();


	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
	Core::Data::Device->SetTexture(1,Core::Data::CreateTexrures->GetTexture("finalscene_2"));
	Core::Data::Device->SetTexture(2,Core::Render::GetCurrentSendTexture());

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSSAOBlur1,"PixelSize",&float2_t(1.f/Core::Data::WidthHeight.x,1.f/Core::Data::WidthHeight.y));
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSSAOBlur1,"NearFar",&Core::Data::NearFar);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPSSAOBlur1);

	/*Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture("finalscene3"));

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPQuadRender);*/

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);

	RenderSurf->Release();
	BackBuf->Release();

	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Core::Render::SetIncrCurrentTex();
}

void RenderMotionBlur(DWORD timeDelta)
{
	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,2,D3DTADDRESS_CLAMP);

	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());
	Core::Data::Device->SetTexture(1,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));

	float determ = 0;
	float4x4 ViewInv = SMMatrixTranspose(SMMatrixInverse(&determ,Core::Data::View));
	float4x4 ViewPrevInv = SMMatrixTranspose(SMMatrixInverse(&determ,Core::Data::ViewPrev));

	float4x4 ViewProjInv = SMMatrixTranspose(SMMatrixInverse(&determ,Core::Data::View * Core::Data::ProjectionStencil));
	float4x4 ViewProjPrevInv = (SMMatrixTranspose(Core::Data::ViewPrev * Core::Data::ProjectionStencil));

	float4x4 tmpProj = SMMatrixTranspose(Core::Data::View*Core::Data::ProjectionStencil);

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ViewInv",&ViewInv);
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"NearFar",&Core::Data::NearFar);
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ParamProj",&float3_t(Core::Data::WidthHeight.x,Core::Data::WidthHeight.y,D3DX_PI / 4.0f));

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPMotionBlur,"ViewPos",&Core::Data::ConstCurrentCameraPosition);
	float tmpcoefblur = MotionBlurCoef * (float(timeDelta) * 0.001f);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPMotionBlur,"CoefBlur",&MotionBlurCoef);

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPMotionBlur,"ViewProjectionInverseMatrix",&ViewProjInv);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPMotionBlur,"PreviousViewProjectionMatrix",&ViewProjPrevInv);

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPMotionBlur,"ViewProjection",&tmpProj);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ResPosDepth);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPMotionBlur);

	Core::Data::Device->BeginScene();
	Core::Data::ScreenTexture->DrawSubset(0);
	Core::Data::Device->EndScene();

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	Core::Render::SetIncrCurrentTex();
}

void RenderGodRays()
{
	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Data::CreateTexrures->GetTexture("ppgodrays")->GetSurfaceLevel(0, &RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_CLAMP);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPQuadRender);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();



	Core::Data::CreateTexrures->GetTexture("ppgodrays2")->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture("ppgodrays"));

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGodRays,"GRParams",&GodRaysParam);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGodRays,"LightPos",&SunPos);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGodRays,"SunColor",&GodRaysColor);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPGodRays);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();


		

	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	Core::Data::Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	Core::Data::Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Core::Data::Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	Core::Render::GetCurrentSendTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_CLAMP);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture("ppgodrays2"));

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPQuadRender);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void RenderBloom()
{
	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Data::CreateTexrures->GetTexture("ppbloom")->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);

	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBloomPP,"Param",&BloomParam);
	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPBloomPP);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();


		

	Core::Data::CreateTexrures->GetTexture("ppbloom2")->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture("ppbloom"));

	D3DSURFACE_DESC desc;
	Core::Data::CreateTexrures->GetTexture("ppbloom")->GetLevelDesc(0,&desc);
	float2_t tmpSizeMap = float2_t(desc.Width,desc.Height);

	float tmpcoefblurbloom = 2.f;
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGaussBlurH,"CoefBlur",&tmpcoefblurbloom);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGaussBlurH,"SizeMap",&tmpSizeMap);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPGaussBlurH);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();



	Core::Data::CreateTexrures->GetTexture("ppbloom")->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture("ppbloom2"));

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGaussBlurV,"CoefBlur",&tmpcoefblurbloom);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGaussBlurV,"SizeMap",&tmpSizeMap);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPGaussBlurV);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();



	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	Core::Data::Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	Core::Data::Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Core::Data::Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	Core::Render::GetCurrentSendTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_CLAMP);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture("ppbloom"));

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPQuadRender);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void RenderLensFlare()
{
	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	Core::Data::Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	Core::Data::Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	Core::Data::Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(Graphics::PostProcess::OrientationSun < 1.57)
		{
			Core::Data::CreateTexrures->GetTexture("ppbloom")->GetSurfaceLevel(0, &RenderSurf);

			Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
			Core::Data::Device->SetRenderTarget(0,RenderSurf);

				if(!Graphics::PostProcess::IsRenderBloom)
					Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

			Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));

			D3DSURFACE_DESC desc;
			Core::Data::CreateTexrures->GetTexture("ppbloom")->GetLevelDesc(0,&desc);
			float2_t tmpSizeMap = float2_t(desc.Width,desc.Height);
			
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPLensFlare0,"LightPos",&SunPos);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPLensFlare0,"SizeMap",&tmpSizeMap);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPLensFlare0,"RadiusSun",&LensFlareSunRadius);
			Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPLensFlare0);

			Core::Data::ScreenTexture->DrawSubset(0);

			Core::Data::Device->SetVertexShader(0);
			Core::Data::Device->SetPixelShader(0);

			Core::Data::Device->SetRenderTarget(0,BackBuf);
			RenderSurf->Release();
			BackBuf->Release();
		}



	Core::Render::GetCurrentSendTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture("ppbloom"));

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPLensFlare2,"LensFlareParam",&LensFlareParam);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPLensFlare2);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void RenderDOF()
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);

	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());

	D3DSURFACE_DESC desc;
	Core::Render::GetCurrentSendTexture()->GetLevelDesc(0,&desc);
	float2_t tmpSizeMap = float2_t(desc.Width,desc.Height);

	float tmpcoefblurbloom = 1.f;
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGaussBlurH,"CoefBlur",&tmpcoefblurbloom);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGaussBlurH,"SizeMap",&tmpSizeMap);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPGaussBlurH);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();



	Core::Data::CreateTexrures->GetTexture("finalscene3")->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);

	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentRenderTexture());

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGaussBlurV,"CoefBlur",&tmpcoefblurbloom);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPGaussBlurV,"SizeMap",&tmpSizeMap);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPGaussBlurV);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();



	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,4,D3DTADDRESS_CLAMP);

	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());
	Core::Data::Device->SetTexture(1,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
	Core::Data::Device->SetTexture(2,Core::Data::CreateTexrures->GetTexture("finalscene3"));

	float tmpskyblur = 0.0f;
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPDOF,"Param",&DOFParam);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPDOF,"SkyBlur",&DOFSkyBlur);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPDOF,"NearFar",&Core::Data::NearFar);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPDOF);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	Core::Render::SetIncrCurrentTex();
}

void RenderWhiteBlack()
{
	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);
	
	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPWhiteBlack,"CountPassWB",&BlackWhiteCoef);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPWhiteBlack);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	Core::Render::SetIncrCurrentTex();
}

void RenderSepia()
{
	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);
	
	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSepia,"CountSepia",&SepiaCoef);

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSepia,"LightColor",&SepiaLightColor);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSepia,"DarkColor",&SepiaDarkColor);
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPSepia,"LumWeights",&SepiaLumWeights);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPSepia);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	Core::Render::SetIncrCurrentTex();
}

void RenderCBG()
{
	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);
	
	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPCBG,"Param",&CBGParam);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPCBG);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	Core::Render::SetIncrCurrentTex();
}

void RenderAA()
{
	Core::Data::Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;
	Core::Render::GetCurrentRenderTexture()->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Render::SetSamplerAddress(0,D3DTADDRESS_MIRROR);
	
	Core::Data::Device->SetTexture(0,Core::Render::GetCurrentSendTexture());

	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPAA,"Param",&float4(1,3,1,0));
	Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPAA,"SizeMap",&Core::Data::WidthHeight);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPAA);

	Core::Data::Device->BeginScene();
	Core::Data::ScreenTexture->DrawSubset(0);
	Core::Data::Device->EndScene();

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	RenderSurf->Release();
	BackBuf->Release();

	Core::Render::SetIncrCurrentTex();
}

/*void UpdateFogColor(float3_t* color)
{
	HowRenderFog = 2;
	StartFogColor = FogColor;
	FinishFogColor = *color;
	HowLastDisplay = 0;
}

void Update(DWORD timeGetTime)
{
		if(HowRenderFog == 2)
		{
				if(HowLastDisplay >= TimeDisplay)
				{
					HowRenderFog == 1;
					FogColor = FinishFogColor;
				}
				else
				{
					float lerpfactor = float(HowLastDisplay) / float(TimeDisplay);
					FogColor.x = lerp(StartFogColor.x,FinishFogColor.x,lerpfactor);
					FogColor.y = lerp(StartFogColor.y,FinishFogColor.y,lerpfactor);
					FogColor.z = lerp(StartFogColor.z,FinishFogColor.z,lerpfactor);
				}
		}
		else if(HowRenderFog == 2)
		{
			
		}
}*/

};
};
};