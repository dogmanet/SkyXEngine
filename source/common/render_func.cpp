
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
	GData::StaticGreen->OnLostDevice();

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
			GData::StaticGreen->OnResetDevice();
			GData::DXDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
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
	float4x4 wmat = (GData::MCamView*GData::MCamView);

	float determ = 0;
	wmat = SMMatrixTranspose(wmat);
	wmat = SMMatrixInverse(&determ, wmat);

	D3DXPlaneTransform(&GData::PlaneZCullingShader, &GData::PlaneZCullingShader, &(wmat.operator D3DXMATRIX()));
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

	GData::Geometry->CPUFillingArrIndeces(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);
	GData::StaticGreen->CPUFillingArrIndeces(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);

	GData::DXDevice->SetClipPlane(0, GData::PlaneZCulling);
	GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
	GData::Geometry->GPURender(timeDelta);

	GData::DXDevice->SetClipPlane(0, GData::PlaneZCullingShader);
	GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
	GData::StaticGreen->GPURender(timeDelta);

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

	if (GData::Geometry->GetCountModel() > 0)
		GData::Geometry->CPUFillingArrIndeces(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);
	
	GData::StaticGreen->CPUFillingArrIndeces(GData::ObjCamera->ObjFrustum, &GData::ConstCurrCamPos);

	GData::DXDevice->SetClipPlane(0, GData::PlaneZCulling);
	GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);

	if (GData::Geometry->GetCountModel() > 0)
		GData::Geometry->GPURender/*Models*/(timeDelta);

	GData::DXDevice->SetClipPlane(0, GData::PlaneZCullingShader);
	GData::DXDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
	GData::StaticGreen->GPURender(timeDelta);

	SXRenderFunc::OutputDebugInfo(timeDelta);

	GData::DXDevice->EndScene();	//заканчиваем отрисовку

	//@@@


	GData::DXDevice->Present(0, 0, 0, 0);	//выводим картинку в окно
}