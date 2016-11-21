
#pragma once

namespace SkyXEngine
{
namespace Graphics
{
namespace ThreeD
{
	
PSSM::PSSM()
{
	/*PosLigthMaxValue = 1000;
	AngleY = 45;
	AngleX = 90;*/
	BlurPixel = 2;
	Generating4Slits = false;

		for(int i=0;i<5;i++)
		{
			DepthSurfaces[i] = 0;
			DepthMaps[i] = 0;

			IsUpdate[i] = 0;
		}

	ShadowMap = 0;
	ShadowMap2 = 0;
	OldDepthStencilSurface = 0;
	OldColorSurface = 0;
}

PSSM::~PSSM()
{
		for(int i=0;i<5;i++)
		{
			ndx_release(DepthMaps[i]);
			ndx_release(DepthSurfaces[i]);
			nmem_delete(Frustums[i]);
		}

	ndx_release(DepthStencilSurface);
	ndx_release(ShadowMap);
	ndx_release(ShadowMap2);
}

inline void PSSM::SetBlurPixel(float blur_pixel)
{
	BlurPixel = blur_pixel;
}

inline bool PSSM::GetBlurPixel()
{
	return BlurPixel;
}

void PSSM::OnLostDevice()
{
		for(int i=0;i<5;i++)
		{
			ndx_release(DepthMaps[i]);
			ndx_release(DepthSurfaces[i]);
		}

	ndx_release(DepthStencilSurface);
	ndx_release(ShadowMap);
	ndx_release(ShadowMap2);
}

void PSSM::OnResetDevice()
{
		for(int i=0;i<5;i++)
		{
			IsUpdate[i] = 0;

			HRESULT hr = Core::Data::Device->CreateTexture(Core::Data::SizeTexDepthGlobal.x*(i==4?1:1), Core::Data::SizeTexDepthGlobal.y*(i==4?1:1), 1, /*D3DUSAGE_DEPTHSTENCIL*/D3DUSAGE_RENDERTARGET, /*FOURCC_INTZ*//*D3DFMT_A8R8G8B8*/D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMaps[i]), NULL);

				if(FAILED(hr))
					Core::InError("Ќе удалось создать текстуру глубины PSSM");
			
			DepthSurfaces[i] = 0;
		}

	Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexDepthGlobal.x, Core::Data::SizeTexDepthGlobal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowGlobal.x, Core::Data::SizeTexShadowGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowGlobal.x, Core::Data::SizeTexShadowGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);

	FovRatio.x = Core::Data::Settings::ProjFov;
	FovRatio.y = Core::Data::Settings::ProjRatio;

	float2 fOffset = float2(0.5,0.5) + (float2(0.5f,0.5)/Core::Data::SizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}

void PSSM::Init()
{
	FovRatio.x = Core::Data::Settings::ProjFov;
	FovRatio.y = Core::Data::Settings::ProjRatio;

	HowShadow = 0;

		for(int i=0;i<5;i++)
		{
			IsUpdate[i] = 0;

			HRESULT hr = Core::Data::Device->CreateTexture(Core::Data::SizeTexDepthGlobal.x*(i==4?1:1), Core::Data::SizeTexDepthGlobal.y*(i==4?1:1), 1, /*D3DUSAGE_DEPTHSTENCIL*/D3DUSAGE_RENDERTARGET, /*FOURCC_INTZ*//*D3DFMT_A8R8G8B8*/D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMaps[i]), NULL);
				
				if(FAILED(hr))
					Core::InError("Ќе удалось создать текстуру глубины PSSM");

			
			DepthSurfaces[i] = 0;

			Frustums[i] = new Core::ControllMoving::Frustum();
		}

	NearFar[0].x = Core::Data::NearFar.x;
	NearFar[0].y = Core::Data::Settings::DistForPSSM.x;

	NearFar[1].x = Core::Data::Settings::DistForPSSM.x;
	NearFar[1].y = Core::Data::Settings::DistForPSSM.y;

	NearFar[2].x = Core::Data::Settings::DistForPSSM.y;
	NearFar[2].y = Core::Data::Settings::DistForPSSM.z;

	NearFar[3].x = Core::Data::Settings::DistForPSSM.z;
	NearFar[3].y = Core::Data::Settings::DistForPSSM.w;


	NearFar[4].x = Core::Data::NearFar.x;
	NearFar[4].y = Core::Data::NearFar.y;

	Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexDepthGlobal.x, Core::Data::SizeTexDepthGlobal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowGlobal.x, Core::Data::SizeTexShadowGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowGlobal.x, Core::Data::SizeTexShadowGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);

	float2 fOffset = float2(0.5,0.5) + (float2(0.5f,0.5)/Core::Data::SizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}


void PSSM::Update(DWORD split,float3* poscam,float3* dircam)
{
	/*if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
	{	
		AngleY +=1;
			if(AngleY >= 360)
				AngleY = 0;
	}

	if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
	{	
		AngleY -=1;
			if(AngleY <= 0)
				AngleY = 360;
	}

	if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
	{	
		AngleX +=1;
			if(AngleX >= 360)
				AngleX = 0;
	}

	if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
	{	
		AngleX -=1;
			if(AngleX <= 0)
				AngleX = 360;
	}*/

	/*float OneAngle = PosLigthMaxValue / 90.0;

		if(AngleX >= 0 && AngleX <= 90)
		{
			Position.x = OneAngle * AngleX;
			Position.z = PosLigthMaxValue - OneAngle * AngleX;
		}
		else if(AngleX >= 90 && AngleX <= 180)
		{
			Position.x = OneAngle * (180.0 - AngleX);
			Position.z = -(PosLigthMaxValue-OneAngle * (180.0 - AngleX));
		}
		else if(AngleX >= 180 && AngleX <= 270)
		{
			Position.x = -(PosLigthMaxValue-OneAngle * (270.0 - AngleX));
			Position.z = -OneAngle * (270.0 - AngleX);
		}
		else if(AngleX >= 270 && AngleX <= 360)
		{
			Position.x = -(OneAngle * (360.0 - AngleX));
			Position.z = PosLigthMaxValue - OneAngle * (360.0 - AngleX);
		}


		if(AngleY >= 0 && AngleY <= 90)
		{
			Position.y = OneAngle * AngleY;

			float coef_ck = (1.0 / PosLigthMaxValue) * ((PosLigthMaxValue / 90.0) * AngleY);
			Position.x *= 1.0 - coef_ck;
			Position.z *= 1.0 - coef_ck;
		}
		else if(AngleY >= 90 && AngleY <= 180)
		{
			Position.y = OneAngle * (180.0 - AngleY);

			float coef_ck = (1.0 / PosLigthMaxValue) * ((PosLigthMaxValue / 90.0) * (180.0 - AngleY));
			Position.x *= coef_ck - 1.0;
			Position.z *= coef_ck - 1.0;
		}
		else if(AngleY >= 180 && AngleY <= 270)
		{
			Position.y = -(PosLigthMaxValue-OneAngle * (270.0 - AngleY));

			float coef_ck = (1.0 / PosLigthMaxValue) * ((PosLigthMaxValue / 90.0) * (270.0 - AngleY));
			Position.x *= - coef_ck;
			Position.z *= - coef_ck;
		}
		else if(AngleY >= 270 && AngleY <= 360)
		{
			Position.y = -(OneAngle * (360.0 - AngleY));

			float coef_ck = (1.0 / PosLigthMaxValue) * ((PosLigthMaxValue / 90.0) * (360.0 - AngleY));
			Position.x *= 1.0 - (coef_ck);
			Position.z *= 1.0 - (coef_ck);
		}*/

	Frustums[split]->Update(&(Views[split]),&(Projs[split]));


	float3 up(0.0f, 1.0f, 0.0f);
	float3 right = SMVector3Normalize(SMVector3Cross((*dircam),up));

	float3 fc = (*poscam) + (*dircam)*NearFar[split].y;
	float3 nc = (*poscam) + (*dircam)*NearFar[split].x;

	up = SMVector3Normalize(SMVector3Cross(right,(*dircam)));

	float near_height = tan(FovRatio.x/2.f) * NearFar[split].x;
	float near_width = near_height * FovRatio.y;
	float far_height = tan(FovRatio.x/2.f) * NearFar[split].y;
	float far_width = far_height * FovRatio.y;

	Frustums[split]->Point[0] = nc - up*near_height - right*near_width;
	Frustums[split]->Point[1] = nc + up*near_height - right*near_width;
	Frustums[split]->Point[2] = nc + up*near_height + right*near_width;
	Frustums[split]->Point[3] = nc - up*near_height + right*near_width;

	Frustums[split]->Point[4] = fc - up*far_height - right*far_width;
	Frustums[split]->Point[5] = fc + up*far_height - right*far_width;
	Frustums[split]->Point[6] = fc + up*far_height + right*far_width;
	Frustums[split]->Point[7] = fc - up*far_height + right*far_width;

	float3 vCenter(0,0,0);
		for (int i = 0; i < 8; i++)
			vCenter += Frustums[split]->Point[i];
	vCenter /= 8;
	Frustums[split]->Center = vCenter;






	float dist	=1;
	float3 DirL	= Position;
	float3 TarG = float3(Frustums[split]->Center.x,Frustums[split]->Center.y,Frustums[split]->Center.z);

	float3 LightPos = TarG+DirL*dist;
	float3 LightPos2 = DirL;
	float3 LightTarget	= TarG;
	float3	Lightup(0.0f, 1.0f, 0.0f);
	Views[split] = SMMatrixLookAtLH(LightPos, LightTarget, Lightup);

	float minX=0;
	float minY=0;
	float minZ=0;
	float maxX=0;
	float maxY=0;
	float maxZ=0;

	float4 trans0;
	float4 transform0(Frustums[split]->Point[0].x,Frustums[split]->Point[0].y,Frustums[split]->Point[0].z,1);
	trans0 = SMVector4Transform(transform0,Views[split]);

	minX=trans0.x;maxX=trans0.x;
	minY=trans0.y;maxY=trans0.y;
	maxZ=trans0.z;

		for (int i=0; i<8; i++)
		{
			float4 trans;
			float4 transform(Frustums[split]->Point[i].x,Frustums[split]->Point[i].y,Frustums[split]->Point[i].z,1);

			trans = SMVector4Transform(transform,Views[split]);

				if (minX>trans.x) 
					minX=trans.x;
				if (maxX<trans.x) 
					maxX=trans.x;
				if (minY>trans.y) 
					minY=trans.y;
				if (maxY<trans.y) 
					maxY=trans.y;
				if (maxZ<trans.z) 
					maxZ=trans.z;
				if (minZ>trans.z) 
					minZ=trans.z;
		}


	float2 OrtMax = float2(maxX,maxY);
	float2 OrtMin = float2(minX,minY);

	float3 Diagonal = Frustums[split]->Point[0] - Frustums[split]->Point[6];
	float LengthDiagonal = SMVector3Length(Diagonal);

	float2 BoarderOffset = (float2(LengthDiagonal,LengthDiagonal) - (OrtMax - OrtMin)) * 0.5;

	OrtMax += BoarderOffset;
	OrtMin -= BoarderOffset;

	float2 WorldUnitsPerTexel = float2(LengthDiagonal,LengthDiagonal) / float2(Core::Data::SizeTexDepthGlobal.x,Core::Data::SizeTexDepthGlobal.y);

	OrtMin /= WorldUnitsPerTexel;
	OrtMin.x = floor(OrtMin.x);
	OrtMin.y = floor(OrtMin.y);
	OrtMin *= WorldUnitsPerTexel;
            
	OrtMax /= WorldUnitsPerTexel;
	OrtMax.x = floor(OrtMax.x);
	OrtMax.y = floor(OrtMax.y);
	OrtMax *= WorldUnitsPerTexel;

	D3DXMATRIX tmpproj;
	D3DXMatrixOrthoOffCenterLH(&tmpproj, OrtMin.x,OrtMax.x,OrtMin.y,OrtMax.y, minZ, maxZ);
	Projs[split] = float4x4(tmpproj);

	ViewProj[split] = Views[split] * Projs[split];
	Flickering(&ViewProj[split],Core::Data::SizeTexDepthGlobal.x,Core::Data::SizeTexDepthGlobal.y);
	
	Core::Data::Device->SetTransform(D3DTS_WORLD1,&(ViewProj[split].operator D3DXMATRIX()));
	//Core::Data::Device->SetTransform(D3DTS_WORLD2,&(SMMatrixTranspose(ViewProj[split]).operator D3DXMATRIX()));
	Core::Data::Device->SetTransform(D3DTS_VIEW,&(Views[split].operator D3DXMATRIX()));
	Core::Data::Device->SetTransform(D3DTS_PROJECTION,&(Projs[split].operator D3DXMATRIX()));

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ShadowCreatePSSM_Direct,"WorldViewProjection",&SMMatrixTranspose(ViewProj[split]));
	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ShadowCreatePSSM_Direct);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_ShadowCreatePSSM_Direct);

	ndx_release(DepthSurfaces[split]);
	DepthMaps[split]->GetSurfaceLevel(0, &(DepthSurfaces[split]));
	
	Core::Data::Device->SetRenderTarget(0, DepthSurfaces[split]);
	
	Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);
}

void PSSM::Begin()
{
	HowShadow = 0;

	Core::Data::Device->GetDepthStencilSurface(&OldDepthStencilSurface);
	Core::Data::Device->SetDepthStencilSurface(DepthStencilSurface);
	Core::Data::Device->GetRenderTarget(0, &OldColorSurface);

	Core::Data::Device->GetTransform(D3DTS_VIEW,&OldView);
	Core::Data::Device->GetTransform(D3DTS_PROJECTION,&OldProj);
	Core::Data::Device->GetTransform(D3DTS_WORLD1,&OldViewProj);
}

void PSSM::End()
{
	Core::Data::Device->SetDepthStencilSurface(OldDepthStencilSurface);
	Core::Data::Device->SetRenderTarget(0, OldColorSurface);

	ndx_release(OldDepthStencilSurface);
	ndx_release(OldColorSurface);

	Core::Data::Device->SetTransform(D3DTS_VIEW,&OldView);
	Core::Data::Device->SetTransform(D3DTS_PROJECTION,&OldProj);
	Core::Data::Device->SetTransform(D3DTS_WORLD1,&OldViewProj);
}

void PSSM::Flickering(float4x4 *matLVP,float size_x,float size_y)
{
	float3 ptOriginShadow(0,0,0);
	ptOriginShadow = SMVector3Transform(ptOriginShadow,*matLVP);

	float texCoordX = ptOriginShadow.x * size_x * 0.5f;
	float texCoordY = ptOriginShadow.y * size_y * 0.5f;

		if(texCoordX >= 0)
			texCoordX += 0.5f;
		else
			texCoordX -= 0.5f;

		if(texCoordY >= 0)
			texCoordY += 0.5f;
		else
			texCoordY -= 0.5f;

	float texCoordRoundedX = floor(texCoordX);
	float texCoordRoundedY = floor(texCoordY);

	float dx = texCoordRoundedX - texCoordX;
	float dy = texCoordRoundedY - texCoordY;

	dx /= size_x * 0.5f;
	dy /= size_y * 0.5f;

	float4x4 xRounding = SMMatrixTranslation(dx, dy, 0); 

	*matLVP = SMMatrixMultiply(*matLVP,xRounding);
}

void PSSM::NullingShadow()
{
	HowShadow = 0;
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

	ShadowMap->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);
		
	Core::Data::Device->SetRenderTarget(0,BackBuf);

	ndx_release(RenderSurf);
	ndx_release(BackBuf);
}

void PSSM::GenShadow()
{
	HowShadow = 0;
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

	ShadowMap->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

	Core::Render::SetSamplerFilter(0,6,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(0,6,D3DTADDRESS_CLAMP);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));

	float4x4 MatrixTexture;
	char mattex[16];
		for(int i=0;i<5;i++)
		{
			Core::Data::Device->SetTexture(1+i,DepthMaps[i]);
			sprintf(mattex,"MatrixTexture%d",1+i);
			MatrixTexture = ViewProj[i] * ScaleBiasMat;
			MatrixTexture = SMMatrixTranspose(MatrixTexture);
				if(Generating4Slits)
					Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM4,mattex,&MatrixTexture);
				else
					Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM3,mattex,&MatrixTexture);
		}

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ResPosDepth);
		if(Generating4Slits)
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PSSM4);
		else
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PSSM3);
	
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ParamProj",&float3_t(Core::Data::WidthHeight.x,Core::Data::WidthHeight.y,D3DX_PI / 4.0f));
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"NearFar",&Core::Data::NearFar);

	/*float3 CameraPosition;
	Core::Data::ObjCamera->GetPosition(&CameraPosition);*/
		if(Generating4Slits)
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM4,"PosCam",&Core::Data::ConstCurrentCameraPosition);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM4,"PixelSize",&float2(BlurPixel/Core::Data::SizeTexDepthGlobal.x,BlurPixel/Core::Data::SizeTexDepthGlobal.y));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM4,"NearFar",&Core::Data::NearFar);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM4,"DistSplit",&float4(NearFar[0].y,NearFar[1].y,NearFar[2].y,NearFar[3].y));
		}
		else
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM3,"PosCam",&Core::Data::ConstCurrentCameraPosition);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM3,"PixelSize",&float2(BlurPixel/Core::Data::SizeTexDepthGlobal.x,BlurPixel/Core::Data::SizeTexDepthGlobal.y));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM3,"NearFar",&Core::Data::NearFar);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PSSM3,"DistSplit",&float4(NearFar[0].y,NearFar[1].y,NearFar[2].y,NearFar[3].y));
		}

	float determ=0;
	float4x4 ViewInv = SMMatrixInverse(&determ,Core::Data::View);
	ViewInv = SMMatrixTranspose(ViewInv);

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ViewInv",&ViewInv);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);
		
	Core::Data::Device->SetRenderTarget(0,BackBuf);

	ndx_release(RenderSurf);
	ndx_release(BackBuf);
}

void PSSM::SoftShadow(bool randomsam,float size)
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(HowShadow == 0)
			ShadowMap2->GetSurfaceLevel(0, &RenderSurf);
		else
			ShadowMap->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	//Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

	Core::Render::SetSamplerFilter(0,2,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(0,2,D3DTADDRESS_CLAMP);

	Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
	
		if(HowShadow == 0)
			Core::Data::Device->SetTexture(1,ShadowMap);
		else
			Core::Data::Device->SetTexture(1,ShadowMap2);
	
	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
		if(randomsam)
		{
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPBlurDepthBasedNoise);
			Core::Data::Device->SetTexture(2,SkyXEngine::Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_NoiseTex));
			Core::Render::SetSamplerFilter(2,D3DTEXF_POINT);
			Core::Render::SetSamplerAddress(2,D3DTADDRESS_WRAP);
		}
		else
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPBlurDepthBased);
	
		if(randomsam)
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBasedNoise,"PixelSize",&float2_t(size/Core::Data::SizeTexShadowGlobal.x,size/Core::Data::SizeTexShadowGlobal.y));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBasedNoise,"NearFar",&Core::Data::NearFar);
		}
		else
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBased,"PixelSize",&float2_t(size/Core::Data::SizeTexShadowGlobal.x,size/Core::Data::SizeTexShadowGlobal.y));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBased,"NearFar",&Core::Data::NearFar);
		}

	
	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);
		
	Core::Data::Device->SetRenderTarget(0,BackBuf);

	ndx_release(RenderSurf);
	ndx_release(BackBuf);

		if(HowShadow == 1)
			HowShadow = 0;
		else
			HowShadow = 1;
}

IDirect3DTexture9* PSSM::GetShadow()
{
		if(HowShadow == 0)
			return ShadowMap;
		else
			return ShadowMap2;
}


///////////////////////////////

void ShadowMapTech::OnLostDevice()
{
	ndx_release(DepthMap);
	ndx_release(DepthStencilSurface);
	ndx_release(ShadowMap);
	ndx_release(ShadowMap2);

	ndx_release(DepthSurface);
}

void ShadowMapTech::OnResetDevice()
{
	IsStatic = true;
	IsUpdate = false;
	Core::Data::Device->CreateTexture(Core::Data::SizeTexDepthLocal.x, Core::Data::SizeTexDepthLocal.y, 1, /*D3DUSAGE_DEPTHSTENCIL*/D3DUSAGE_RENDERTARGET, /*FOURCC_INTZ*/D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMap), NULL);
	Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexDepthLocal.x, Core::Data::SizeTexDepthLocal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowLocal.x, Core::Data::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowLocal.x, Core::Data::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);
}

ShadowMapTech::ShadowMapTech()
{
	IsStatic = true;
	IsUpdate = false;

	Bias = 0.0001;
	BlurPixel = 0.5;
	IsRenderGreen = false;
}

ShadowMapTech::~ShadowMapTech()
{
	nmem_delete(Frustum);

	ndx_release(DepthMap);
	ndx_release(DepthStencilSurface);
	ndx_release(ShadowMap);
	ndx_release(ShadowMap2);

	ndx_release(DepthSurface);
}

void ShadowMapTech::SetPosition(float3* pos)
{
	Position = *pos;
}

void ShadowMapTech::GetPosition(float3* pos)
{
	*pos = Position;
}

void ShadowMapTech::SetDirection(float3* dir)
{
	Direction = *dir;
}

void ShadowMapTech::GetDirection(float3* dir)
{
	*dir = Direction;
}

void ShadowMapTech::SetAngleNearFar(float3* anf)
{
	AngleNearFar = *anf;
}

void ShadowMapTech::GetAngleNearFar(float3* anf)
{
	*anf = AngleNearFar;
}

inline void ShadowMapTech::SetBias(float bias)
{
	Bias = bias;
}

inline float ShadowMapTech::GetBias()
{
	return Bias;
}


inline void ShadowMapTech::SetBlurPixel(float blur_pixel)
{
	BlurPixel = blur_pixel;
}

inline bool ShadowMapTech::GetBlurPixel()
{
	return BlurPixel;
}

void ShadowMapTech::Init()
{
	Frustum = new Core::ControllMoving::Frustum();
	HowShadow = 0;

	DepthSurface = 0;
	Core::Data::Device->CreateTexture(Core::Data::SizeTexDepthLocal.x, Core::Data::SizeTexDepthLocal.y, 1, /*D3DUSAGE_DEPTHSTENCIL*/D3DUSAGE_RENDERTARGET, /*FOURCC_INTZ*/D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMap), NULL);
	Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexDepthLocal.x, Core::Data::SizeTexDepthLocal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowLocal.x, Core::Data::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowLocal.x, Core::Data::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);

	float fOffset = 0.5f + (0.5f/Core::Data::SizeTexDepthLocal.x);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset,	fOffset,	fBias,		1.0f);
}

inline void ShadowMapTech::SetFar(float sfar)
{
	AngleNearFar.z = sfar;
}

inline float ShadowMapTech::GetFar()
{
	return AngleNearFar.z;
}

void ShadowMapTech::Begin()
{
	HowShadow = 0;

	Core::Data::Device->GetDepthStencilSurface(&OldDepthStencilSurface);
	Core::Data::Device->SetDepthStencilSurface(DepthStencilSurface);
	Core::Data::Device->GetRenderTarget(0, &OldColorSurface);

	Core::Data::Device->GetTransform(D3DTS_VIEW,&OldView);
	Core::Data::Device->GetTransform(D3DTS_PROJECTION,&OldProj);
	Core::Data::Device->GetTransform(D3DTS_WORLD1,&OldViewProj);

	View = SMMatrixLookAtLH(Position, Position+Direction, float3(0,1,0));
	Proj = SMMatrixPerspectiveFovLH(AngleNearFar.x,Core::Data::SizeTexDepthLocal.x / Core::Data::SizeTexDepthLocal.y,AngleNearFar.y,AngleNearFar.z);

	Core::Data::Device->SetTransform(D3DTS_VIEW,&(View.operator D3DXMATRIX()));
	Core::Data::Device->SetTransform(D3DTS_PROJECTION,&(Proj.operator D3DXMATRIX()));
	Core::Data::Device->SetTransform(D3DTS_WORLD1,&(View * Proj).operator D3DXMATRIX());

	Frustum->Update(&(View),&(Proj));

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ShadowCreatePSSM_Direct,"WorldViewProjection",&SMMatrixTranspose(View * Proj));
	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ShadowCreatePSSM_Direct);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_ShadowCreatePSSM_Direct);

	ndx_release(DepthSurface);
	DepthMap->GetSurfaceLevel(0, &(DepthSurface));
	
	Core::Data::Device->SetRenderTarget(0, DepthSurface);
	
	Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);
}

void ShadowMapTech::End()
{
	Core::Data::Device->SetDepthStencilSurface(OldDepthStencilSurface);
	Core::Data::Device->SetRenderTarget(0, OldColorSurface);

	ndx_release(OldDepthStencilSurface);
	ndx_release(OldColorSurface);

	Core::Data::Device->SetTransform(D3DTS_VIEW,&OldView);
	Core::Data::Device->SetTransform(D3DTS_PROJECTION,&OldProj);
	Core::Data::Device->SetTransform(D3DTS_WORLD1,&OldViewProj);

	IsUpdate = true;
}

void ShadowMapTech::GenShadow()
{
	HowShadow = 0;
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

	ShadowMap->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0);

	Core::Render::SetSamplerFilter(0,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(0,D3DTADDRESS_CLAMP);
	Core::Render::SetSamplerFilter(1,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(1,D3DTADDRESS_CLAMP);
	Core::Render::SetSamplerFilter(2,D3DTEXF_POINT);
	Core::Render::SetSamplerAddress(2,D3DTADDRESS_WRAP);

		if(Core::Data::Settings::CoefSizeShadowMapForLocal == 1)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
		else if(Core::Data::Settings::CoefSizeShadowMapForLocal == 0.5f)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinearD2));
		else if(Core::Data::Settings::CoefSizeShadowMapForLocal == 0.25)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinearD4));

	float4x4 MatrixTexture;

	Core::Data::Device->SetTexture(1,DepthMap);

	Core::Data::Device->SetTexture(2,SkyXEngine::Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_NoiseTex));
	MatrixTexture = View * Proj * ScaleBiasMat;
	MatrixTexture = SMMatrixTranspose(MatrixTexture);

		if(Core::Data::Settings::IsHalfGenPCFShadowLocal)
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowDirect4,"MatrixTexture",&MatrixTexture);
		else
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowDirect9,"MatrixTexture",&MatrixTexture);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ResPosDepth);

		if(Core::Data::Settings::IsHalfGenPCFShadowLocal)
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_GenShadowDirect4);
		else
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_GenShadowDirect9);
	
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ParamProj",&float3_t(Core::Data::WidthHeight.x,Core::Data::WidthHeight.y,D3DX_PI / 4.0f));
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"NearFar",&Core::Data::NearFar);

	//float factor_shadow = 0.0f;
	//float3 CameraPosition;
	//Core::Data::ObjCamera->GetPosition(&CameraPosition);
		if(Core::Data::Settings::IsHalfGenPCFShadowLocal)
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowDirect4,"PosCam",&Core::Data::ConstCurrentCameraPosition);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowDirect4,"PixelSize",&float2(BlurPixel/Core::Data::SizeTexDepthLocal.x,BlurPixel/Core::Data::SizeTexDepthLocal.y));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowDirect4,"SizeMapBias",&float3(Core::Data::SizeTexDepthLocal.x,Core::Data::SizeTexDepthLocal.y,Bias));
		}
		else
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowDirect9,"PosCam",&Core::Data::ConstCurrentCameraPosition);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowDirect9,"PixelSize",&float2(BlurPixel/Core::Data::SizeTexDepthLocal.x,BlurPixel/Core::Data::SizeTexDepthLocal.y));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowDirect9,"SizeMapBias",&float3(Core::Data::SizeTexDepthLocal.x,Core::Data::SizeTexDepthLocal.y,Bias));
		}

	float determ=0;
	float4x4 ViewInv = SMMatrixInverse(&determ,Core::Data::View);
	ViewInv = SMMatrixTranspose(ViewInv);

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ViewInv",&ViewInv);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);
		
	Core::Data::Device->SetRenderTarget(0,BackBuf);

	ndx_release(RenderSurf);
	ndx_release(BackBuf);
}

void ShadowMapTech::SoftShadow(bool randomsam,float size)
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(HowShadow == 0)
			ShadowMap2->GetSurfaceLevel(0, &RenderSurf);
		else
			ShadowMap->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	//Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0);

	Core::Render::SetSamplerFilter(0,D3DTEXF_NONE);
	Core::Render::SetSamplerAddress(0,D3DTADDRESS_CLAMP);
	Core::Render::SetSamplerFilter(1,D3DTEXF_NONE);
	Core::Render::SetSamplerAddress(1,D3DTADDRESS_CLAMP);

		if(Core::Data::Settings::CoefSizeShadowMapForLocal == 1)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
		else if(Core::Data::Settings::CoefSizeShadowMapForLocal == 0.5f)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinearD2));
		else if(Core::Data::Settings::CoefSizeShadowMapForLocal == 0.25f)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinearD4));
	
		if(HowShadow == 0)
			Core::Data::Device->SetTexture(1,ShadowMap);
		else
			Core::Data::Device->SetTexture(1,ShadowMap2);

	
	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);
		
		if(randomsam)
		{
			Core::Render::SetSamplerFilter(2,D3DTEXF_POINT);
			Core::Render::SetSamplerAddress(2,D3DTADDRESS_WRAP);
			Core::Data::Device->SetTexture(2,Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_NoiseTex));
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPBlurDepthBasedNoise);
		}
		else
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPBlurDepthBased);
	
		if(randomsam)
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBasedNoise,"PixelSize",&float2_t(size/Core::Data::SizeTexShadowLocal.x,size/Core::Data::SizeTexShadowLocal.y));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBasedNoise,"NearFar",&Core::Data::NearFar);
		}
		else
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBased,"PixelSize",&float2_t(size/Core::Data::SizeTexShadowLocal.x,size/Core::Data::SizeTexShadowLocal.y));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBased,"NearFar",&Core::Data::NearFar);
		}

	
	Core::Data::ScreenTexture->DrawSubset(0);


	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

		
	Core::Data::Device->SetRenderTarget(0,BackBuf);

	ndx_release(RenderSurf);
	ndx_release(BackBuf);

		if(HowShadow == 1)
			HowShadow = 0;
		else
			HowShadow = 1;
}

IDirect3DTexture9* ShadowMapTech::GetShadow()
{
		if(HowShadow == 0)
			return ShadowMap;
		else
			return ShadowMap2;
}



///////////////////////////////

void ShadowMapCubeTech::OnLostDevice()
{
	ndx_release(DepthMap);
	ndx_release(DepthStencilSurface);
	ndx_release(ShadowMap);
	ndx_release(ShadowMap2);

	ndx_release(DepthSurface[0]);
	ndx_release(DepthSurface[1]);
	ndx_release(DepthSurface[2]);
	ndx_release(DepthSurface[3]);
	ndx_release(DepthSurface[4]);
	ndx_release(DepthSurface[5]);
}

void ShadowMapCubeTech::OnResetDevice()
{
	IsStatic = true;
	IsUpdate = false;
	D3DXCreateCubeTexture(Core::Data::Device, Core::Data::SizeTexDepthLocal.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &DepthMap);
	Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexDepthLocal.x, Core::Data::SizeTexDepthLocal.x, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowLocal.x, Core::Data::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowLocal.x, Core::Data::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);
}

ShadowMapCubeTech::~ShadowMapCubeTech()
{
	nmem_delete(Frustum);

	ndx_release(DepthMap);
	ndx_release(DepthStencilSurface);
	ndx_release(ShadowMap);
	ndx_release(ShadowMap2);

	ndx_release(DepthSurface[0]);
	ndx_release(DepthSurface[1]);
	ndx_release(DepthSurface[2]);
	ndx_release(DepthSurface[3]);
	ndx_release(DepthSurface[4]);
	ndx_release(DepthSurface[5]);
}

ShadowMapCubeTech::ShadowMapCubeTech()
{
	IsStatic = true;
	IsUpdate = false;

	EnableEdge[0] = EnableEdge[1] = EnableEdge[2] = EnableEdge[3] = EnableEdge[4] = EnableEdge[5] = true;

	CountRender = 0;

	Bias = 0.07;
	BlurPixel = 5;
	IsRenderGreen = false;
}

inline void ShadowMapCubeTech::SetEnableCubeEdge(int edge,bool enable)
{
		if(edge >= 0 && edge <= 5)
			EnableEdge[edge] = enable;
}

inline bool ShadowMapCubeTech::GetEnableCubeEdge(int edge)
{
		if(edge >= 0 && edge <= 5)
			return EnableEdge[edge];
}

void ShadowMapCubeTech::SetPosition(float3* pos)
{
	Position = *pos;
}

void ShadowMapCubeTech::GetPosition(float3* pos)
{
	*pos = Position;
}

void ShadowMapCubeTech::SetNearFar(float2* nf)
{
	NearFar = *nf;
}

void ShadowMapCubeTech::GetNearFar(float2* nf)
{
	*nf = NearFar;
}

void ShadowMapCubeTech::Init()
{
	Frustum = new Core::ControllMoving::Frustum();
	HowShadow = 0;

	DepthSurface[0] = 0;
	DepthSurface[1] = 0;
	DepthSurface[2] = 0;
	DepthSurface[3] = 0;
	DepthSurface[4] = 0;
	DepthSurface[5] = 0;

	D3DXCreateCubeTexture(Core::Data::Device, Core::Data::SizeTexDepthLocal.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &DepthMap);
	Core::Data::Device->CreateDepthStencilSurface(Core::Data::SizeTexDepthLocal.x, Core::Data::SizeTexDepthLocal.x, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowLocal.x, Core::Data::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	Core::Data::Device->CreateTexture(Core::Data::SizeTexShadowLocal.x, Core::Data::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);
}

void ShadowMapCubeTech::Begin()
{
	Core::Data::Device->GetDepthStencilSurface(&OldDepthStencilSurface);
	Core::Data::Device->SetDepthStencilSurface(DepthStencilSurface);
	Core::Data::Device->GetRenderTarget(0, &OldColorSurface);

	Core::Data::Device->GetTransform(D3DTS_VIEW,&OldView);
	Core::Data::Device->GetTransform(D3DTS_PROJECTION,&OldProj);
	Core::Data::Device->GetTransform(D3DTS_WORLD1,&OldViewProj);
}

void ShadowMapCubeTech::Pre(int cube)
{
	View[cube] = SMMatrixLookAtLH(Position, Core::Data::OrientedCube[cube] + Position, Core::Data::UpVectorsCube[cube]);

	Proj[cube] = SMMatrixPerspectiveFovLH(SM_PI/2.f,1,NearFar.x,NearFar.y);

	Core::Data::Device->SetTransform(D3DTS_VIEW,&(View[cube].operator D3DXMATRIX()));
	Core::Data::Device->SetTransform(D3DTS_PROJECTION,&(Proj[cube].operator D3DXMATRIX()));

	Frustum->Update(&(View[cube]),&(Proj[cube]));

	float4x4 vp = View[cube] * Proj[cube];

	Core::Data::Device->SetTransform(D3DTS_WORLD1,&(vp.operator D3DXMATRIX()));
	vp = SMMatrixTranspose(vp);

	ndx_release(DepthSurface[cube]);
	
    DepthMap->GetCubeMapSurface( ( D3DCUBEMAP_FACES ) cube, 0, &DepthSurface[cube] );
	Core::Data::Device->SetRenderTarget( 0, DepthSurface[cube] );
	
	Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ShadowCreateCube);
	Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_ShadowCreateCube);

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ShadowCreateCube,"LightPos",&Position);
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ShadowCreateCube,"WorldViewProjection",&vp);
}

void ShadowMapCubeTech::Post(int cube)
{
		if(GetAsyncKeyState(VK_NUMPAD5))
		{
			char tmpstr[1024];
			sprintf(tmpstr,"C:\\1\\shadowcube_%d.bmp",cube);
			D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_PNG, DepthSurface[cube], NULL,0);
		}
}

void ShadowMapCubeTech::End()
{
	Core::Data::Device->SetDepthStencilSurface(OldDepthStencilSurface);
	Core::Data::Device->SetRenderTarget(0, OldColorSurface);
	ndx_release(OldDepthStencilSurface);
	ndx_release(OldColorSurface);

	Core::Data::Device->SetTransform(D3DTS_VIEW,&OldView);
	Core::Data::Device->SetTransform(D3DTS_PROJECTION,&OldProj);
	Core::Data::Device->SetTransform(D3DTS_WORLD1,&OldViewProj);

	
		if(CountRender > 2)
			IsUpdate = true;
		else
			CountRender++;
}

void ShadowMapCubeTech::GenShadow()
{
	HowShadow = 0;
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

	ShadowMap->GetSurfaceLevel(0, &RenderSurf);
	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,0,0), 1.0f, 0);

	Core::Render::SetSamplerFilter(0,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(0,D3DTADDRESS_CLAMP);
	Core::Render::SetSamplerFilter(1,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(1,D3DTADDRESS_CLAMP);
	Core::Render::SetSamplerFilter(2,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(2,D3DTADDRESS_WRAP);

		if(Core::Data::Settings::CoefSizeShadowMapForLocal == 1)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
		else if(Core::Data::Settings::CoefSizeShadowMapForLocal == 0.5f)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinearD2));
		else if(Core::Data::Settings::CoefSizeShadowMapForLocal == 0.25f)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinearD4));

	Core::Data::Device->SetTexture(1,DepthMap);
	Core::Data::Device->SetTexture(2,SkyXEngine::Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_NoiseTex));

	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_ResPosDepth);
		if(Core::Data::Settings::IsHalfGenPCFShadowLocal)
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_GenShadowCube1);
		else
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_GenShadowCube6);
	
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ParamProj",&float3_t(Core::Data::WidthHeight.x,Core::Data::WidthHeight.y,D3DX_PI / 4.0f));
	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"NearFar",&Core::Data::NearFar);

	float pixel_size = BlurPixel/Core::Data::SizeTexDepthLocal.x;
	float3 CameraPosition;
	Core::Data::ObjCamera->GetPosition(&CameraPosition);
		if(Core::Data::Settings::IsHalfGenPCFShadowLocal)
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowCube1,"PosCam",&CameraPosition);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowCube1,"LightPos",&Position);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowCube1,"SizeMapBias",&float2(Core::Data::SizeTexDepthLocal.x,Bias));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowCube1,"PixelSize",&pixel_size);
		}
		else
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowCube6,"PosCam",&CameraPosition);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowCube6,"LightPos",&Position);
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowCube6,"SizeMapBias",&float2(Core::Data::SizeTexDepthLocal.x,Bias));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_GenShadowCube6,"PixelSize",&pixel_size);
		}
	float determ=0;
	float4x4 ViewInv = SMMatrixInverse(&determ,Core::Data::View);
	ViewInv = SMMatrixTranspose(ViewInv);

	Core::Data::ShadersManager->SetValueRegisterF(0,Core::Data::EngineID::VS_ResPosDepth,"ViewInv",&ViewInv);

	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);
		
	Core::Data::Device->SetRenderTarget(0,BackBuf);
	ndx_release(RenderSurf);
	ndx_release(BackBuf);
}

void ShadowMapCubeTech::SoftShadow(bool randomsam,float size)
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(HowShadow == 0)
			ShadowMap2->GetSurfaceLevel(0, &RenderSurf);
		else
			ShadowMap->GetSurfaceLevel(0, &RenderSurf);

	Core::Data::Device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	Core::Data::Device->SetRenderTarget(0,RenderSurf);

	//Core::Data::Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);


	Core::Render::SetSamplerFilter(0,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(0,D3DTADDRESS_CLAMP);
	Core::Render::SetSamplerFilter(1,D3DTEXF_LINEAR);
	Core::Render::SetSamplerAddress(1,D3DTADDRESS_CLAMP);


		if(Core::Data::Settings::CoefSizeShadowMapForLocal == 1)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinear));
		else if(Core::Data::Settings::CoefSizeShadowMapForLocal == 0.5f)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinearD2));
		else if(Core::Data::Settings::CoefSizeShadowMapForLocal == 0.25f)
			Core::Data::Device->SetTexture(0,Core::Data::CreateTexrures->GetTexture(Core::Data::EngineID::Tex_DSDepthLinearD4));
	
		if(HowShadow == 0)
			Core::Data::Device->SetTexture(1,ShadowMap);
		else
			Core::Data::Device->SetTexture(1,ShadowMap2);
	
	Core::Data::ShadersManager->Bind(0,Core::Data::EngineID::VS_PPQuadRender);

		if(randomsam)
		{
			Core::Render::SetSamplerFilter(2,D3DTEXF_POINT);
			Core::Render::SetSamplerAddress(2,D3DTADDRESS_WRAP);
			Core::Data::Device->SetTexture(2,Core::Data::LoadedTextures->GetTexture(Core::Data::EngineID::Tex_NoiseTex));
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPBlurDepthBasedNoise);
		}
		else
			Core::Data::ShadersManager->Bind(1,Core::Data::EngineID::PS_PPBlurDepthBased);
	
		if(randomsam)
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBasedNoise,"PixelSize",&float2_t(size/Core::Data::SizeTexShadowLocal.x,size/Core::Data::SizeTexShadowLocal.x));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBasedNoise,"NearFar",&Core::Data::NearFar);
		}
		else
		{
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBased,"PixelSize",&float2_t(size/Core::Data::SizeTexShadowLocal.x,size/Core::Data::SizeTexShadowLocal.x));
			Core::Data::ShadersManager->SetValueRegisterF(1,Core::Data::EngineID::PS_PPBlurDepthBased,"NearFar",&Core::Data::NearFar);
		}

	
	Core::Data::ScreenTexture->DrawSubset(0);

	Core::Data::Device->SetVertexShader(0);
	Core::Data::Device->SetPixelShader(0);

	Core::Data::Device->SetRenderTarget(0,BackBuf);
	ndx_release(RenderSurf);
	ndx_release(BackBuf);

		if(HowShadow == 1)
			HowShadow = 0;
		else
			HowShadow = 1;
}

IDirect3DTexture9* ShadowMapCubeTech::GetShadow()
{
		if(HowShadow == 0)
			return ShadowMap;
		else
			return ShadowMap2;
}


};
};
};