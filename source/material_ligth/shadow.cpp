
#pragma once

#include <material_ligth\\shadow.h>
	
PSSM::PSSM()
{
	BlurPixel = 2;
	Generating4Slits = true;

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
			mem_release_del(DepthMaps[i]);
			mem_release_del(DepthSurfaces[i]);
			mem_delete(Frustums[i]);
		}

	mem_release_del(DepthStencilSurface);
	mem_release_del(ShadowMap);
	mem_release_del(ShadowMap2);
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
			mem_release_del(DepthMaps[i]);
			mem_release_del(DepthSurfaces[i]);
		}

	mem_release_del(DepthStencilSurface);
	mem_release_del(ShadowMap);
	mem_release_del(ShadowMap2);
}

void PSSM::OnResetDevice()
{
		for(int i=0;i<5;i++)
		{
			IsUpdate[i] = 0;

			HRESULT hr = MLSet::DXDevice->CreateTexture(MLSet::SizeTexDepthGlobal.x*(i==4?1:1), MLSet::SizeTexDepthGlobal.y*(i==4?1:1), 1, /*D3DUSAGE_DEPTHSTENCIL*/D3DUSAGE_RENDERTARGET, /*FOURCC_INTZ*//*D3DFMT_A8R8G8B8*/D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMaps[i]), NULL);

				if(FAILED(hr))
					reportf(-1,"Ќе удалось создать текстуру глубины PSSM");
			
			DepthSurfaces[i] = 0;
		}

	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthGlobal.x, MLSet::SizeTexDepthGlobal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowGlobal.x, MLSet::SizeTexShadowGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowGlobal.x, MLSet::SizeTexShadowGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);

	FovRatio.x = MLSet::ProjFov;
	FovRatio.y = MLSet::ProjRatio;

	float2 fOffset = float2(0.5,0.5) + (float2(0.5f,0.5)/MLSet::SizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}

void PSSM::Init()
{
	FovRatio.x = MLSet::ProjFov;
	FovRatio.y = MLSet::ProjRatio;

	HowShadow = 0;

		for(int i=0;i<5;i++)
		{
			IsUpdate[i] = 0;

			HRESULT hr = MLSet::DXDevice->CreateTexture(MLSet::SizeTexDepthGlobal.x*(i==4?1:1), MLSet::SizeTexDepthGlobal.y*(i==4?1:1), 1, /*D3DUSAGE_DEPTHSTENCIL*/D3DUSAGE_RENDERTARGET, /*FOURCC_INTZ*//*D3DFMT_A8R8G8B8*/D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMaps[i]), NULL);
				
				if(FAILED(hr))
					reportf(-1,"Ќе удалось создать текстуру глубины PSSM");

			
			DepthSurfaces[i] = 0;

			Frustums[i] = SGCore_CrFrustum();
		}

	NearFar[0].x = MLSet::NearFar.x;
	NearFar[0].y = MLSet::DistForPSSM.x;

	NearFar[1].x = MLSet::DistForPSSM.x;
	NearFar[1].y = MLSet::DistForPSSM.y;

	NearFar[2].x = MLSet::DistForPSSM.y;
	NearFar[2].y = MLSet::DistForPSSM.z;

	NearFar[3].x = MLSet::DistForPSSM.z;
	NearFar[3].y = MLSet::DistForPSSM.w;


	NearFar[4].x = MLSet::NearFar.x;
	NearFar[4].y = MLSet::NearFar.y;

	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthGlobal.x, MLSet::SizeTexDepthGlobal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowGlobal.x, MLSet::SizeTexShadowGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowGlobal.x, MLSet::SizeTexShadowGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);

	float2 fOffset = float2(0.5,0.5) + (float2(0.5f,0.5)/MLSet::SizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}


void PSSM::Update(DWORD split,float3* poscam,float3* dircam)
{
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

	float2 WorldUnitsPerTexel = float2(LengthDiagonal,LengthDiagonal) / float2(MLSet::SizeTexDepthGlobal.x,MLSet::SizeTexDepthGlobal.y);

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
	Flickering(&ViewProj[split],MLSet::SizeTexDepthGlobal.x,MLSet::SizeTexDepthGlobal.y);
	
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&(ViewProj[split].operator D3DXMATRIX()));
	//MLSet::DXDevice->SetTransform(D3DTS_WORLD2,&(SMMatrixTranspose(ViewProj[split]).operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_VIEW,&(Views[split].operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&(Projs[split].operator D3DXMATRIX()));

	SGCore_ShaderSetVRF(0, MLSet::IDsShaders::VS::ShadowCreatePSSM_Direct, "WorldViewProjection", &SMMatrixTranspose(ViewProj[split]));
	SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ShadowCreatePSSM_Direct);
	SGCore_ShaderBind(1, MLSet::IDsShaders::PS::ShadowCreatePSSM_Direct);

	mem_release_del(DepthSurfaces[split]);
	DepthMaps[split]->GetSurfaceLevel(0, &(DepthSurfaces[split]));
	
	MLSet::DXDevice->SetRenderTarget(0, DepthSurfaces[split]);
	
	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);
}

void PSSM::Begin()
{
	HowShadow = 0;

	MLSet::DXDevice->GetDepthStencilSurface(&OldDepthStencilSurface);
	MLSet::DXDevice->SetDepthStencilSurface(DepthStencilSurface);
	MLSet::DXDevice->GetRenderTarget(0, &OldColorSurface);

	MLSet::DXDevice->GetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->GetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->GetTransform(D3DTS_WORLD1,&OldViewProj);
}

void PSSM::End()
{
	MLSet::DXDevice->SetDepthStencilSurface(OldDepthStencilSurface);
	MLSet::DXDevice->SetRenderTarget(0, OldColorSurface);

	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);

	MLSet::DXDevice->SetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&OldViewProj);

	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow0.bmp", D3DXIFF_PNG, DepthMaps[0], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow1.bmp", D3DXIFF_PNG, DepthMaps[1], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow2.bmp", D3DXIFF_PNG, DepthMaps[2], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow3.bmp", D3DXIFF_PNG, DepthMaps[3], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow4.bmp", D3DXIFF_PNG, DepthMaps[4], NULL);
	}
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
	MLSet::DXDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	MLSet::DXDevice->SetRenderTarget(0,RenderSurf);

	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);
		
	MLSet::DXDevice->SetRenderTarget(0,BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

void PSSM::GenShadow()
{
	HowShadow = 0;
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

	ShadowMap->GetSurfaceLevel(0, &RenderSurf);
	MLSet::DXDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	MLSet::DXDevice->SetRenderTarget(0,RenderSurf);

	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

	SGCore_SetSamplerFilter2(0, 6, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress2(0, 6, D3DTADDRESS_CLAMP);

	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinear));

	float4x4 MatrixTexture;
	char mattex[16];
		for(int i=0;i<5;i++)
		{
			MLSet::DXDevice->SetTexture(1+i,DepthMaps[i]);
			sprintf(mattex,"MatrixTexture%d",1+i);
			MatrixTexture = ViewProj[i] * ScaleBiasMat;
			MatrixTexture = SMMatrixTranspose(MatrixTexture);
				if(Generating4Slits)
					SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PSSM4,mattex,&MatrixTexture);
				else
					SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::PSSM3, mattex, &MatrixTexture);
		}

		SGCore_ShaderBind(0, MLSet::IDsShaders::VS::ResPosDepth);
		if(Generating4Slits)
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::PSSM4);
		else
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::PSSM3);
	
		SGCore_ShaderSetVRF(0, MLSet::IDsShaders::VS::ResPosDepth, "ParamProj", &float3_t(MLSet::WinSize.x, MLSet::WinSize.y, MLSet::ProjFov));
	SGCore_ShaderSetVRF(0,MLSet::IDsShaders::VS::ResPosDepth,"NearFar",&MLSet::NearFar);

	/*float3 CameraPosition;
	Core::Data::ObjCamera->GetPosition(&CameraPosition);*/
		if(Generating4Slits)
		{
			SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::PSSM4, "PosCam", &MLSet::ConstCurrCamPos);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PSSM4,"PixelSize",&float2(BlurPixel/MLSet::SizeTexDepthGlobal.x,BlurPixel/MLSet::SizeTexDepthGlobal.y));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PSSM4,"NearFar",&MLSet::NearFar);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PSSM4,"DistSplit",&float4(NearFar[0].y,NearFar[1].y,NearFar[2].y,NearFar[3].y));
		}
		else
		{
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PSSM3,"PosCam",&MLSet::ConstCurrCamPos);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PSSM3,"PixelSize",&float2(BlurPixel/MLSet::SizeTexDepthGlobal.x,BlurPixel/MLSet::SizeTexDepthGlobal.y));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PSSM3,"NearFar",&MLSet::NearFar);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PSSM3,"DistSplit",&float4(NearFar[0].y,NearFar[1].y,NearFar[2].y,NearFar[3].y));
		}

	float determ=0;
	float4x4 ViewInv = SMMatrixInverse(&determ, MLSet::MCamView);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(0,MLSet::IDsShaders::VS::ResPosDepth,"ViewInv",&ViewInv);

	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);
		
	MLSet::DXDevice->SetRenderTarget(0,BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

void PSSM::SoftShadow(bool randomsam,float size)
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(HowShadow == 0)
			ShadowMap2->GetSurfaceLevel(0, &RenderSurf);
		else
			ShadowMap->GetSurfaceLevel(0, &RenderSurf);

	MLSet::DXDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	MLSet::DXDevice->SetRenderTarget(0,RenderSurf);

	//MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);

	SGCore_SetSamplerFilter2(0, 2, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress2(0, 2, D3DTADDRESS_CLAMP);

	MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinear));
	
		if(HowShadow == 0)
			MLSet::DXDevice->SetTexture(1,ShadowMap);
		else
			MLSet::DXDevice->SetTexture(1,ShadowMap2);
	
	SGCore_ShaderBind(0,MLSet::IDsShaders::VS::PPQuadRender);
		if(randomsam)
		{
			SGCore_ShaderBind(1, MLSet::IDsShaders::PS::PPBlurDepthBasedNoise);
			MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));
			SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
			SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);
		}
		else
			SGCore_ShaderBind(1, MLSet::IDsShaders::PS::PPBlurDepthBased);
	
		if(randomsam)
		{
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise,"PixelSize",&float2_t(size/MLSet::SizeTexShadowGlobal.x,size/MLSet::SizeTexShadowGlobal.y));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise,"NearFar",&MLSet::NearFar);
		}
		else
		{
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBased,"PixelSize",&float2_t(size/MLSet::SizeTexShadowGlobal.x,size/MLSet::SizeTexShadowGlobal.y));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBased,"NearFar",&MLSet::NearFar);
		}

	
	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);
		
	MLSet::DXDevice->SetRenderTarget(0,BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);

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
	mem_release_del(DepthMap);
	mem_release_del(DepthStencilSurface);
	mem_release_del(ShadowMap);
	mem_release_del(ShadowMap2);

	mem_release_del(DepthSurface);
}

void ShadowMapTech::OnResetDevice()
{
	IsStatic = true;
	IsUpdate = false;
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, 1, /*D3DUSAGE_DEPTHSTENCIL*/D3DUSAGE_RENDERTARGET, /*FOURCC_INTZ*/D3DFMT_R32F, D3DPOOL_DEFAULT, &(DepthMap), NULL);
	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowLocal.x, MLSet::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F, D3DPOOL_DEFAULT, &ShadowMap, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowLocal.x, MLSet::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);
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
	mem_delete(Frustum);

	mem_release_del(DepthMap);
	mem_release_del(DepthStencilSurface);
	mem_release_del(ShadowMap);
	mem_release_del(ShadowMap2);

	mem_release_del(DepthSurface);
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

inline float ShadowMapTech::GetBlurPixel()
{
	return BlurPixel;
}

void ShadowMapTech::Init()
{
	Frustum = SGCore_CrFrustum();
	HowShadow = 0;

	DepthSurface = 0;
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, 1, /*D3DUSAGE_DEPTHSTENCIL*/D3DUSAGE_RENDERTARGET, /*FOURCC_INTZ*/D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMap), NULL);
	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowLocal.x, MLSet::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowLocal.x, MLSet::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);

	float fOffset = 0.5f + (0.5f/MLSet::SizeTexDepthLocal.x);
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

inline void ShadowMapTech::SetNear(float snear)
{
	AngleNearFar.y = snear;
}

inline float ShadowMapTech::GetNear()
{
	return AngleNearFar.y;
}

inline void ShadowMapTech::SetAngle(float sangle)
{
	AngleNearFar.x = sangle;
}

inline float ShadowMapTech::GetAngle()
{
	return AngleNearFar.x;
}

void ShadowMapTech::Begin()
{
	HowShadow = 0;

	MLSet::DXDevice->GetDepthStencilSurface(&OldDepthStencilSurface);
	MLSet::DXDevice->SetDepthStencilSurface(DepthStencilSurface);
	MLSet::DXDevice->GetRenderTarget(0, &OldColorSurface);

	MLSet::DXDevice->GetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->GetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->GetTransform(D3DTS_WORLD1,&OldViewProj);

	View = SMMatrixLookAtLH(Position, Position+Direction, float3(0,1,0));
	Proj = SMMatrixPerspectiveFovLH(AngleNearFar.x,MLSet::SizeTexDepthLocal.x / MLSet::SizeTexDepthLocal.y,AngleNearFar.y,AngleNearFar.z);

	MLSet::DXDevice->SetTransform(D3DTS_VIEW,&(View.operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&(Proj.operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&(View * Proj).operator D3DXMATRIX());

	Frustum->Update(&(View),&(Proj));

	SGCore_ShaderSetVRF(0,MLSet::IDsShaders::VS::ShadowCreatePSSM_Direct,"WorldViewProjection",&SMMatrixTranspose(View * Proj));
	SGCore_ShaderBind(0,MLSet::IDsShaders::VS::ShadowCreatePSSM_Direct);
	SGCore_ShaderBind(1, MLSet::IDsShaders::PS::ShadowCreatePSSM_Direct);

	mem_release_del(DepthSurface);
	DepthMap->GetSurfaceLevel(0, &(DepthSurface));
	
	MLSet::DXDevice->SetRenderTarget(0, DepthSurface);
	
	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);
}

void ShadowMapTech::End()
{
	MLSet::DXDevice->SetDepthStencilSurface(OldDepthStencilSurface);
	MLSet::DXDevice->SetRenderTarget(0, OldColorSurface);

	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);

	MLSet::DXDevice->SetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&OldViewProj);

	IsUpdate = true;
}

void ShadowMapTech::GenShadow()
{
	HowShadow = 0;
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

	ShadowMap->GetSurfaceLevel(0, &RenderSurf);
	MLSet::DXDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	MLSet::DXDevice->SetRenderTarget(0,RenderSurf);

	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0);

	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);

		if(MLSet::CoefSizeShadowMapForLocal == 1)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinear));
		else if(MLSet::CoefSizeShadowMapForLocal == 0.5f)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinearD2));
		else if(MLSet::CoefSizeShadowMapForLocal == 0.25)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinearD4));

	float4x4 MatrixTexture;

	MLSet::DXDevice->SetTexture(1,DepthMap);

	MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));
	MatrixTexture = View * Proj * ScaleBiasMat;
	MatrixTexture = SMMatrixTranspose(MatrixTexture);

		if(MLSet::IsHalfGenPCFShadowLocal)
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowDirect4,"MatrixTexture",&MatrixTexture);
		else
			SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::GenShadowDirect9, "MatrixTexture", &MatrixTexture);

	SGCore_ShaderBind(0,MLSet::IDsShaders::VS::ResPosDepth);

		if(MLSet::IsHalfGenPCFShadowLocal)
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::GenShadowDirect4);
		else
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::GenShadowDirect9);
	
	SGCore_ShaderSetVRF(0,MLSet::IDsShaders::VS::ResPosDepth,"ParamProj",&float3_t(MLSet::WinSize.x,MLSet::WinSize.y,D3DX_PI / 4.0f));
	SGCore_ShaderSetVRF(0,MLSet::IDsShaders::VS::ResPosDepth,"NearFar",&MLSet::NearFar);

	//float factor_shadow = 0.0f;
	//float3 CameraPosition;
	//Core::Data::ObjCamera->GetPosition(&CameraPosition);
		if(MLSet::IsHalfGenPCFShadowLocal)
		{
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowDirect4,"PosCam",&MLSet::ConstCurrCamPos);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowDirect4,"PixelSize",&float2(BlurPixel/MLSet::SizeTexDepthLocal.x,BlurPixel/MLSet::SizeTexDepthLocal.y));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowDirect4,"SizeMapBias",&float3(MLSet::SizeTexDepthLocal.x,MLSet::SizeTexDepthLocal.y,Bias));
		}
		else
		{
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowDirect9,"PosCam",&MLSet::ConstCurrCamPos);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowDirect9,"PixelSize",&float2(BlurPixel/MLSet::SizeTexDepthLocal.x,BlurPixel/MLSet::SizeTexDepthLocal.y));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowDirect9,"SizeMapBias",&float3(MLSet::SizeTexDepthLocal.x,MLSet::SizeTexDepthLocal.y,Bias));
		}

	float determ=0;
	float4x4 ViewInv = SMMatrixInverse(&determ,MLSet::MCamView);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(0,MLSet::IDsShaders::VS::ResPosDepth,"ViewInv",&ViewInv);

	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);
		
	MLSet::DXDevice->SetRenderTarget(0,BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

void ShadowMapTech::SoftShadow(bool randomsam,float size)
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(HowShadow == 0)
			ShadowMap2->GetSurfaceLevel(0, &RenderSurf);
		else
			ShadowMap->GetSurfaceLevel(0, &RenderSurf);
	MLSet::DXDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	MLSet::DXDevice->SetRenderTarget(0,RenderSurf);

	//MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,0,0,0), 1.0f, 0);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);

		if(MLSet::CoefSizeShadowMapForLocal == 1)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinear));
		else if(MLSet::CoefSizeShadowMapForLocal == 0.5f)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinearD2));
		else if(MLSet::CoefSizeShadowMapForLocal == 0.25f)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinearD4));
	
		if(HowShadow == 0)
			MLSet::DXDevice->SetTexture(1,ShadowMap);
		else
			MLSet::DXDevice->SetTexture(1,ShadowMap2);

	
	SGCore_ShaderBind(0,MLSet::IDsShaders::VS::PPQuadRender);
		
		if(randomsam)
		{
			SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
			SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);
			MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise);
		}
		else
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::PPBlurDepthBased);
	
		if(randomsam)
		{
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise,"PixelSize",&float2_t(size/MLSet::SizeTexShadowLocal.x,size/MLSet::SizeTexShadowLocal.y));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise,"NearFar",&MLSet::NearFar);
		}
		else
		{
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBased,"PixelSize",&float2_t(size/MLSet::SizeTexShadowLocal.x,size/MLSet::SizeTexShadowLocal.y));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBased,"NearFar",&MLSet::NearFar);
		}

	
	SGCore_ScreenQuadDraw();


	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

		
	MLSet::DXDevice->SetRenderTarget(0,BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);

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
	mem_release_del(DepthMap);
	mem_release_del(DepthStencilSurface);
	mem_release_del(ShadowMap);
	mem_release_del(ShadowMap2);

	mem_release_del(DepthSurface[0]);
	mem_release_del(DepthSurface[1]);
	mem_release_del(DepthSurface[2]);
	mem_release_del(DepthSurface[3]);
	mem_release_del(DepthSurface[4]);
	mem_release_del(DepthSurface[5]);
}

void ShadowMapCubeTech::OnResetDevice()
{
	IsStatic = true;
	IsUpdate = false;
	D3DXCreateCubeTexture(MLSet::DXDevice, MLSet::SizeTexDepthLocal.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &DepthMap);
	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.x, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowLocal.x, MLSet::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowLocal.x, MLSet::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);
}

ShadowMapCubeTech::~ShadowMapCubeTech()
{
	mem_delete(Frustum);

	mem_release_del(DepthMap);
	mem_release_del(DepthStencilSurface);
	mem_release_del(ShadowMap);
	mem_release_del(ShadowMap2);

	mem_release_del(DepthSurface[0]);
	mem_release_del(DepthSurface[1]);
	mem_release_del(DepthSurface[2]);
	mem_release_del(DepthSurface[3]);
	mem_release_del(DepthSurface[4]);
	mem_release_del(DepthSurface[5]);
}

ShadowMapCubeTech::ShadowMapCubeTech()
{
	IsStatic = true;
	IsUpdate = false;

	EnableEdge[0] = EnableEdge[1] = EnableEdge[2] = EnableEdge[3] = EnableEdge[4] = EnableEdge[5] = true;

	CountRender = 0;

	Bias = 0.007;
	BlurPixel = 64;
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

void ShadowMapCubeTech::SetNear(float val)
{
	NearFar.x = val;
}

float ShadowMapCubeTech::GetNear()
{
	return NearFar.x;
}


void ShadowMapCubeTech::SetFar(float val)
{
	NearFar.y = val;
}

float ShadowMapCubeTech::GetFar()
{
	return NearFar.y;
}


void ShadowMapCubeTech::Init()
{
	Frustum = SGCore_CrFrustum();
	HowShadow = 0;

	DepthSurface[0] = 0;
	DepthSurface[1] = 0;
	DepthSurface[2] = 0;
	DepthSurface[3] = 0;
	DepthSurface[4] = 0;
	DepthSurface[5] = 0;

	D3DXCreateCubeTexture(MLSet::DXDevice, MLSet::SizeTexDepthLocal.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &DepthMap);
	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.x, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowLocal.x, MLSet::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap, NULL);
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexShadowLocal.x, MLSet::SizeTexShadowLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R16F,D3DPOOL_DEFAULT, &ShadowMap2, NULL);
}

void ShadowMapCubeTech::Begin()
{
	MLSet::DXDevice->GetDepthStencilSurface(&OldDepthStencilSurface);
	MLSet::DXDevice->SetDepthStencilSurface(DepthStencilSurface);
	MLSet::DXDevice->GetRenderTarget(0, &OldColorSurface);

	MLSet::DXDevice->GetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->GetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->GetTransform(D3DTS_WORLD1,&OldViewProj);
}

void ShadowMapCubeTech::Pre(int cube)
{
	View[cube] = SMMatrixLookAtLH(Position, MLSet::OrientedCube[cube] + Position, MLSet::UpVectorsCube[cube]);

	Proj[cube] = SMMatrixPerspectiveFovLH(SM_PI/2.f,1,NearFar.x,NearFar.y);

	MLSet::DXDevice->SetTransform(D3DTS_VIEW,&(View[cube].operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&(Proj[cube].operator D3DXMATRIX()));

	Frustum->Update(&(View[cube]),&(Proj[cube]));

	float4x4 vp = View[cube] * Proj[cube];

	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&(vp.operator D3DXMATRIX()));
	vp = SMMatrixTranspose(vp);

	mem_release_del(DepthSurface[cube]);
	
    DepthMap->GetCubeMapSurface( ( D3DCUBEMAP_FACES ) cube, 0, &DepthSurface[cube] );
	MLSet::DXDevice->SetRenderTarget( 0, DepthSurface[cube] );
	
	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

	SGCore_ShaderBind(0,MLSet::IDsShaders::VS::ShadowCreateCube);
	SGCore_ShaderBind(1, MLSet::IDsShaders::PS::ShadowCreateCube);

	SGCore_ShaderSetVRF(0, MLSet::IDsShaders::VS::ShadowCreateCube, "LightPos", &Position);
	SGCore_ShaderSetVRF(0, MLSet::IDsShaders::VS::ShadowCreateCube, "WorldViewProjection", &vp);
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
	MLSet::DXDevice->SetDepthStencilSurface(OldDepthStencilSurface);
	MLSet::DXDevice->SetRenderTarget(0, OldColorSurface);
	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);

	MLSet::DXDevice->SetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&OldViewProj);

	
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
	MLSet::DXDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	MLSet::DXDevice->SetRenderTarget(0,RenderSurf);

	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,0,0), 1.0f, 0);

	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(2, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);

		if(MLSet::CoefSizeShadowMapForLocal == 1)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinear));
		else if(MLSet::CoefSizeShadowMapForLocal == 0.5f)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinearD2));
		else if(MLSet::CoefSizeShadowMapForLocal == 0.25f)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinearD4));

	MLSet::DXDevice->SetTexture(1,DepthMap);
	MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));

	SGCore_ShaderBind(0,MLSet::IDsShaders::VS::ResPosDepth);
		if(MLSet::IsHalfGenPCFShadowLocal)
			SGCore_ShaderBind(1, MLSet::IDsShaders::PS::GenShadowCube1);
		else
			SGCore_ShaderBind(1, MLSet::IDsShaders::PS::GenShadowCube6);
	
	SGCore_ShaderSetVRF(0,MLSet::IDsShaders::VS::ResPosDepth,"ParamProj",&float3_t(MLSet::WinSize.x,MLSet::WinSize.y,D3DX_PI / 4.0f));
	SGCore_ShaderSetVRF(0,MLSet::IDsShaders::VS::ResPosDepth,"NearFar",&MLSet::NearFar);

	float pixel_size = BlurPixel/MLSet::SizeTexDepthLocal.x;
	
		if(MLSet::IsHalfGenPCFShadowLocal)
		{
			SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::GenShadowCube1, "PosCam", &MLSet::ConstCurrCamPos);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowCube1,"LightPos",&Position);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowCube1,"SizeMapBias",&float2(MLSet::SizeTexDepthLocal.x,Bias));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowCube1,"PixelSize",&pixel_size);
		}
		else
		{
			SGCore_ShaderSetVRF(1, MLSet::IDsShaders::PS::GenShadowCube6, "PosCam", &MLSet::ConstCurrCamPos);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowCube6,"LightPos",&Position);
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowCube6,"SizeMapBias",&float2(MLSet::SizeTexDepthLocal.x,Bias));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::GenShadowCube6,"PixelSize",&pixel_size);
		}
	float determ=0;
	float4x4 ViewInv = SMMatrixInverse(&determ, MLSet::MCamView);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(0,MLSet::IDsShaders::VS::ResPosDepth,"ViewInv",&ViewInv);

	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);
		
	MLSet::DXDevice->SetRenderTarget(0,BackBuf);
	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

void ShadowMapCubeTech::SoftShadow(bool randomsam,float size)
{
	LPDIRECT3DSURFACE9 RenderSurf,BackBuf;

		if(HowShadow == 0)
			ShadowMap2->GetSurfaceLevel(0, &RenderSurf);
		else
			ShadowMap->GetSurfaceLevel(0, &RenderSurf);

	MLSet::DXDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&BackBuf);
	MLSet::DXDevice->SetRenderTarget(0,RenderSurf);

	//MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.0f, 0);


	SGCore_SetSamplerFilter(0, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);


		if (MLSet::CoefSizeShadowMapForLocal == 1)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinear));
		else if(MLSet::CoefSizeShadowMapForLocal == 0.5f)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinearD2));
		else if(MLSet::CoefSizeShadowMapForLocal == 0.25f)
			MLSet::DXDevice->SetTexture(0,SGCore_RTGetTexture(MLSet::IDsRenderTargets::Tex_DSDepthLinearD4));
	
		if(HowShadow == 0)
			MLSet::DXDevice->SetTexture(1,ShadowMap);
		else
			MLSet::DXDevice->SetTexture(1,ShadowMap2);
	
	SGCore_ShaderBind(0,MLSet::IDsShaders::VS::PPQuadRender);

		if(randomsam)
		{
			SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
			SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);
			MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise);
		}
		else
			SGCore_ShaderBind(1,MLSet::IDsShaders::PS::PPBlurDepthBased);
	
		if(randomsam)
		{
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise,"PixelSize",&float2_t(size/MLSet::SizeTexShadowLocal.x,size/MLSet::SizeTexShadowLocal.x));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBasedNoise,"NearFar",&MLSet::NearFar);
		}
		else
		{
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBased,"PixelSize",&float2_t(size/MLSet::SizeTexShadowLocal.x,size/MLSet::SizeTexShadowLocal.x));
			SGCore_ShaderSetVRF(1,MLSet::IDsShaders::PS::PPBlurDepthBased,"NearFar",&MLSet::NearFar);
		}

	
	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	MLSet::DXDevice->SetRenderTarget(0,BackBuf);
	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);

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
