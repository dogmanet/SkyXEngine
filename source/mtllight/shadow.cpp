
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "shadow.h"
	
PSSM::PSSM()
{
	BlurPixel = 2;
	Generating4Slits = true;

		for(int i=0;i<5;i++)
		{
			DepthSurfaces[i] = 0;
			DepthMaps[i] = 0;
			Frustums[i] = 0;
			IsUpdate[i] = 0;

			//IDArr[i] = -1;
		}

	OldDepthStencilSurface = 0;
	OldColorSurface = 0;

	DepthStencilSurface = 0;
}

PSSM::~PSSM()
{
	for(int i=0;i<5;i++)
	{
		mem_release_del(DepthMaps[i]);
		mem_release_del(DepthSurfaces[i]);
		mem_release_del(Frustums[i]);
	}

	for (int i = 0; i < IDArr.size(); i++)
	{
		mem_delete_a(IDArr[i]);
	}

	IDArr.clear();

	mem_release_del(DepthStencilSurface);
	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);
}

void PSSM::SetPosition(float3* pos)
{ 
	Position = *pos; 
}

void PSSM::GetPosition(float3* pos)
{ 
	*pos = Position; 
}

void PSSM::Set4Or3Splits(bool is4)
{ 
	Generating4Slits = is4; 
}

bool PSSM::Get4Or3Splits()
{ 
	return Generating4Slits; 
}

void PSSM::SetBlurPixel(float blur_pixel)
{
	BlurPixel = blur_pixel;
}

bool PSSM::GetBlurPixel()
{
	return BlurPixel;
}

void PSSM::SetIDArr(ID id, int split, ID idarr)
{
	if (id < 0 || !(split >= 0 && split < 5))
		return;

	if (id >= IDArr.size())
	{
		ID* tmparr = new ID[5];
		for (int i = 0; i < 5; ++i)
			tmparr[i] = -1;
		IDArr[id] = tmparr;
	}

	IDArr[id][split] = idarr;
}

long PSSM::GetCountIDArrs()
{
	return IDArr.size();
}

ID PSSM::GetIDArr(ID id, int split)
{
	if (id < 0 || !(split >= 0 && split < 5) || !(id < IDArr.size()))
		return -2;

	return IDArr[id][split];
}

void PSSM::OnLostDevice()
{
		for(int i=0;i<5;i++)
		{
			mem_release_del(DepthMaps[i]);
			mem_release_del(DepthSurfaces[i]);
		}

	mem_release_del(DepthStencilSurface);
}

void PSSM::OnResetDevice()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

		for(int i=0;i<5;i++)
		{
			IsUpdate[i] = 0;

			HRESULT hr = MLSet::DXDevice->CreateTexture(MLSet::SizeTexDepthGlobal.x, MLSet::SizeTexDepthGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMaps[i]), NULL);

				/*if(FAILED(hr))
					LibReport(REPORT_MSG_LEVEL_ERROR,"Ќе удалось создать текстуру глубины PSSM");*/
			
			DepthSurfaces[i] = 0;
		}

	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthGlobal.x, MLSet::SizeTexDepthGlobal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	
	FovRatio.x = *r_default_fov;
	FovRatio.y = float(*r_win_width) / float(*r_win_height);

	float2 fOffset = float2(0.5, 0.5) - (float2(0.5f, 0.5f) / MLSet::SizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.000f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}

void PSSM::Init()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	FovRatio.x = *r_default_fov;
	FovRatio.y = float(*r_win_width) / float(*r_win_height);

		for(int i=0;i<5;i++)
		{
			IsUpdate[i] = 0;

			MLSet::DXDevice->CreateTexture(MLSet::SizeTexDepthGlobal.x, MLSet::SizeTexDepthGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMaps[i]), NULL);

			DepthSurfaces[i] = 0;

			Frustums[i] = SGCore_CrFrustum();
		}

	NearFar[0].x = *r_near;
	NearFar[0].y = MLSet::DistForPSSM.x;

	NearFar[1].x = MLSet::DistForPSSM.x;
	NearFar[1].y = MLSet::DistForPSSM.y;

	NearFar[2].x = MLSet::DistForPSSM.y;
	NearFar[2].y = MLSet::DistForPSSM.z;

	NearFar[3].x = MLSet::DistForPSSM.z;
	NearFar[3].y = MLSet::DistForPSSM.w;


	NearFar[4].x = *r_near;
	NearFar[4].y = *r_far;

	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthGlobal.x, MLSet::SizeTexDepthGlobal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	
	float2 fOffset = float2(0.5,0.5) + (float2(0.5f,0.5)/MLSet::SizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}


void PSSM::UpdateFrustums(int split, const float3* poscam, const float3* dircam)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	FovRatio.x = *r_default_fov;
	FovRatio.y = float(*r_win_width) / float(*r_win_height);

	Frustums[split]->update(&(Views[split]), &(Projs[split]));

	float3 up(0.0f, 1.0f, 0.0f);
	float3 right = SMVector3Normalize(SMVector3Cross((*dircam), up));

	float3 fc = (*poscam) + (*dircam)*NearFar[split].y;
	float3 nc = (*poscam) + (*dircam)*NearFar[split].x;

	up = SMVector3Normalize(SMVector3Cross(right, (*dircam)));

	float near_height = tan(FovRatio.x / 2.f) * NearFar[split].x;
	float near_width = near_height * FovRatio.y;
	float far_height = tan(FovRatio.x / 2.f) * NearFar[split].y;
	float far_width = far_height * FovRatio.y;

	Frustums[split]->setPoint(0, &float3(nc - up*near_height - right*near_width));
	Frustums[split]->setPoint(1, &float3(nc + up*near_height - right*near_width));
	Frustums[split]->setPoint(2, &float3(nc + up*near_height + right*near_width));
	Frustums[split]->setPoint(3, &float3(nc - up*near_height + right*near_width));

	Frustums[split]->setPoint(4, &float3(fc - up*far_height - right*far_width));
	Frustums[split]->setPoint(5, &float3(fc + up*far_height - right*far_width));
	Frustums[split]->setPoint(6, &float3(fc + up*far_height + right*far_width));
	Frustums[split]->setPoint(7, &float3(fc - up*far_height + right*far_width));

	float3 vCenter(0, 0, 0);
	for (int i = 0; i < 8; i++)
		vCenter += Frustums[split]->getPoint(i);
	vCenter /= 8;
	Frustums[split]->setCenter(&vCenter);

	float dist = 1;
	float3 DirL = Position;
	float3 TarG = float3(Frustums[split]->getCenter());

	float3 LightPos = TarG + DirL*dist;
	float3 LightPos2 = DirL;
	float3 LightTarget = TarG;
	float3	Lightup(0.0f, 1.0f, 0.0f);
	Views[split] = SMMatrixLookAtLH(LightPos, LightTarget, Lightup);

	float minX = 0;
	float minY = 0;
	float minZ = 0;
	float maxX = 0;
	float maxY = 0;
	float maxZ = 0;

	float4 trans0;
	float4 transform0(Frustums[split]->getPoint(0), 1);
	trans0 = SMVector4Transform(transform0, Views[split]);

	minX = trans0.x; maxX = trans0.x;
	minY = trans0.y; maxY = trans0.y;
	maxZ = trans0.z;

	for (int i = 0; i<8; i++)
	{
		float4 trans;
		float4 transform(Frustums[split]->getPoint(i), 1);

		trans = SMVector4Transform(transform, Views[split]);

		if (minX>trans.x)
			minX = trans.x;
		if (maxX<trans.x)
			maxX = trans.x;
		if (minY>trans.y)
			minY = trans.y;
		if (maxY < trans.y)
			maxY = trans.y;
		if (maxZ<trans.z)
			maxZ = trans.z;
		if (minZ>trans.z)
			minZ = trans.z;
	}


	float2 OrtMax = float2(maxX, maxY);
	float2 OrtMin = float2(minX, minY);

	float3 Diagonal = Frustums[split]->getPoint(0) - Frustums[split]->getPoint(6);
	float LengthDiagonal = SMVector3Length(Diagonal);

	float2 BoarderOffset = (float2(LengthDiagonal, LengthDiagonal) - (OrtMax - OrtMin)) * 0.5;

	OrtMax += BoarderOffset;
	OrtMin -= BoarderOffset;

	float2 WorldUnitsPerTexel = float2(LengthDiagonal, LengthDiagonal) / float2(MLSet::SizeTexDepthGlobal.x, MLSet::SizeTexDepthGlobal.y);

	OrtMin /= WorldUnitsPerTexel;
	OrtMin.x = floor(OrtMin.x);
	OrtMin.y = floor(OrtMin.y);
	OrtMin *= WorldUnitsPerTexel;

	OrtMax /= WorldUnitsPerTexel;
	OrtMax.x = floor(OrtMax.x);
	OrtMax.y = floor(OrtMax.y);
	OrtMax *= WorldUnitsPerTexel;

	D3DXMATRIX tmpproj;
	D3DXMatrixOrthoOffCenterLH(&tmpproj, OrtMin.x, OrtMax.x, OrtMin.y, OrtMax.y, minZ, maxZ);
	Projs[split] = float4x4(tmpproj);

	ViewProj[split] = Views[split] * Projs[split];
	Flickering(&ViewProj[split], MLSet::SizeTexDepthGlobal.x, MLSet::SizeTexDepthGlobal.y);
}

void PSSM::PreRender(int split)
{
	/*MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&(ViewProj[split].operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_VIEW,&(Views[split].operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&(Projs[split].operator D3DXMATRIX()));*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &Views[split]);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &Projs[split]);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &ViewProj[split]);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect, "WorldViewProjection", &SMMatrixTranspose(ViewProj[split]));
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGeomPSSMDirect);

	mem_release_del(DepthSurfaces[split]);
	DepthMaps[split]->GetSurfaceLevel(0, &(DepthSurfaces[split]));
	
	MLSet::DXDevice->SetRenderTarget(0, DepthSurfaces[split]);
	
	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);
}

void PSSM::Begin()
{
	MLSet::DXDevice->GetDepthStencilSurface(&OldDepthStencilSurface);
	MLSet::DXDevice->SetDepthStencilSurface(DepthStencilSurface);
	MLSet::DXDevice->GetRenderTarget(0, &OldColorSurface);

	/*MLSet::DXDevice->GetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->GetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->GetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);
}

void PSSM::End()
{
	MLSet::DXDevice->SetDepthStencilSurface(OldDepthStencilSurface);
	MLSet::DXDevice->SetRenderTarget(0, OldColorSurface);

	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);

	/*MLSet::DXDevice->SetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);

	/*if (GetAsyncKeyState(VK_NUMPAD1))
	{
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow0.bmp", D3DXIFF_PNG, DepthMaps[0], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow1.bmp", D3DXIFF_PNG, DepthMaps[1], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow2.bmp", D3DXIFF_PNG, DepthMaps[2], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow3.bmp", D3DXIFF_PNG, DepthMaps[3], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow4.bmp", D3DXIFF_PNG, DepthMaps[4], NULL);
	}*/
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

void PSSM::GenShadow2(IDirect3DTexture9* shadowmap)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	shadowmap->GetSurfaceLevel(0, &RenderSurf);
	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, RenderSurf);

	//MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter2(1, 6, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress2(1, 6, D3DTADDRESS_CLAMP);

	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::DepthScene));

	float4x4 MatrixTexture;
	char mattex[16];
	for (int i = 0; i<5; i++)
	{
		MLSet::DXDevice->SetTexture(1 + i, DepthMaps[i]);
		sprintf(mattex, "MatrixTexture%d", 1 + i);
		MatrixTexture = ViewProj[i] * ScaleBiasMat;
		MatrixTexture = SMMatrixTranspose(MatrixTexture);
		if (Generating4Slits)
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM4, mattex, &MatrixTexture);
		else if (i != 3)
			SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM3, mattex, &MatrixTexture);
	}

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth);
	if (Generating4Slits)
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM4);
	else
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM3);

	float4x4 camview, mProjection;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &camview);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mProjection);

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, camview);
	ViewInv = SMMatrixTranspose(ViewInv);

	float4x4 mInvVP = SMMatrixTranspose(SMMatrixInverse(&determ, camview * mProjection));

	float3 observerpos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &observerpos);
	
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth, "ParamProj", &float3_t(*r_win_width, *r_win_height, *r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth, "NearFar", &float2_t(*r_near, *r_far));
	
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth, "ViewInv", &ViewInv);

	if (Generating4Slits)
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM4, "PosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM4, "PixelSize", &float2(BlurPixel / MLSet::SizeTexDepthGlobal.x, BlurPixel / MLSet::SizeTexDepthGlobal.y));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM4, "NearFar", &float2_t(*r_near, *r_far));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM4, "DistSplit", &float4(NearFar[0].y, NearFar[1].y, NearFar[2].y, NearFar[3].y));
	}
	else
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM3, "PosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM3, "PixelSize", &float2(BlurPixel / MLSet::SizeTexDepthGlobal.x, BlurPixel / MLSet::SizeTexDepthGlobal.y));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM3, "NearFar", &float2_t(*r_near, *r_far));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::PSSM3, "DistSplit", &float4(NearFar[0].y, NearFar[1].y, NearFar[2].y, NearFar[3].y));
	}

	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

void PSSM::GenShadowAll(IDirect3DTexture9* shadowmap)
{
	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	shadowmap->GetSurfaceLevel(0, &RenderSurf);
	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, RenderSurf);

	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

///////////////////////////////

void ShadowMapTech::OnLostDevice()
{
	mem_release_del(DepthMap);
	mem_release_del(DepthStencilSurface);
	mem_release_del(DepthSurface);
}

void ShadowMapTech::OnResetDevice()
{
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &(DepthMap), NULL);
	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
}

ShadowMapTech::ShadowMapTech()
{
	Bias = 0.0001;
	BlurPixel = 0.5;

	Frustum = 0;

	DepthMap = 0;
	DepthSurface = 0;
	DepthStencilSurface = 0;

	OldDepthStencilSurface = 0;
	OldColorSurface = 0;

	//IDArr = -1;
}

ShadowMapTech::~ShadowMapTech()
{
	mem_release_del(Frustum);

	mem_release_del(DepthMap);
	mem_release_del(DepthStencilSurface);
	
	mem_release_del(DepthSurface);

	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);
}

void ShadowMapTech::SetPosition(const float3* pos)
{
	Position = *pos;
}

void ShadowMapTech::GetPosition(float3* pos)
{
	*pos = Position;
}

void ShadowMapTech::SetDirection(const float3* dir)
{
	Direction = *dir;
}

void ShadowMapTech::GetDirection(float3* dir)
{
	*dir = Direction;
}

void ShadowMapTech::SetAngleNearFar(const float3* anf)
{
	AngleNearFar = *anf;
}

void ShadowMapTech::GetAngleNearFar(float3* anf)
{
	*anf = AngleNearFar;
}

void ShadowMapTech::SetBias(float bias)
{
	Bias = bias;
}

float ShadowMapTech::GetBias()
{
	return Bias;
}


void ShadowMapTech::SetBlurPixel(float blur_pixel)
{
	BlurPixel = blur_pixel;
}

float ShadowMapTech::GetBlurPixel()
{
	return BlurPixel;
}

void ShadowMapTech::Init()
{
	Frustum = SGCore_CrFrustum();
	
	DepthSurface = 0;
	MLSet::DXDevice->CreateTexture(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,D3DPOOL_DEFAULT, &(DepthMap), NULL);
	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);

	float fOffset = 0.5f + (0.5f/MLSet::SizeTexDepthLocal.x);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset,	fOffset,	fBias,		1.0f);
}

void ShadowMapTech::SetFar(float sfar)
{
	AngleNearFar.z = sfar;
}

float ShadowMapTech::GetFar()
{
	return AngleNearFar.z;
}

void ShadowMapTech::SetNear(float snear)
{
	AngleNearFar.y = snear;
}

float ShadowMapTech::GetNear()
{
	return AngleNearFar.y;
}

void ShadowMapTech::SetAngle(float sangle)
{
	AngleNearFar.x = sangle;
}

float ShadowMapTech::GetAngle()
{
	return AngleNearFar.x;
}

void ShadowMapTech::SetIDArr(long id, long idarr)
{
	if (id < 0)
		return;

	IDArr[id] = idarr;
}

long ShadowMapTech::GetCountIDArrs()
{
	return IDArr.size();
}

long ShadowMapTech::GetIDArr(long id)
{
	if (id < 0 || !(id < IDArr.size()))
		return -2;

	return IDArr[id];
}

void ShadowMapTech::Begin()
{
	MLSet::DXDevice->GetDepthStencilSurface(&OldDepthStencilSurface);
	MLSet::DXDevice->SetDepthStencilSurface(DepthStencilSurface);
	MLSet::DXDevice->GetRenderTarget(0, &OldColorSurface);

	/*MLSet::DXDevice->GetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->GetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->GetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);

	float3 upvec = float3(0, 1, 0);
	
	if (Direction.x == 0 && Direction.y == -1 && Direction.z == 0)
		upvec = SMVector3Transform(Direction, SMMatrixRotationZ(1.57));

	View = SMMatrixLookAtLH(Position, Position + Direction, upvec);
	Proj = SMMatrixPerspectiveFovLH(AngleNearFar.x,MLSet::SizeTexDepthLocal.x / MLSet::SizeTexDepthLocal.y,AngleNearFar.y,AngleNearFar.z);

	/*MLSet::DXDevice->SetTransform(D3DTS_VIEW,&(View.operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&(Proj.operator D3DXMATRIX()));
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&(View * Proj).operator D3DXMATRIX());*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &View);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &Proj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(View * Proj));
	
	Frustum->update(&(View),&(Proj));

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect, "WorldViewProjection", &SMMatrixTranspose(View * Proj));
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomPSSMDirect);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGeomPSSMDirect);

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

	/*MLSet::DXDevice->SetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);
}

void ShadowMapTech::GenShadow2(IDirect3DTexture9* shadowmap)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	shadowmap->GetSurfaceLevel(0, &RenderSurf);
	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, RenderSurf);

	//MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);

	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::DepthScene));

	float4x4 MatrixTexture;

	MLSet::DXDevice->SetTexture(1, DepthMap);

	MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));
	MatrixTexture = View * Proj * ScaleBiasMat;
	MatrixTexture = SMMatrixTranspose(MatrixTexture);

	if (MLSet::IsHalfGenPCFShadowLocal)
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect4, "MatrixTexture", &MatrixTexture);
	else
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect9, "MatrixTexture", &MatrixTexture);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth);

	if (MLSet::IsHalfGenPCFShadowLocal)
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect4);
	else
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect9);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth, "ParamProj", &float3_t(*r_win_width, *r_win_height, *r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth, "NearFar", &float2_t(*r_near, *r_far));

	float3 observerpos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &observerpos);

	if (MLSet::IsHalfGenPCFShadowLocal)
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect4, "PosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect4, "PixelSize", &float2(BlurPixel / MLSet::SizeTexDepthLocal.x, BlurPixel / MLSet::SizeTexDepthLocal.y));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect4, "SizeMapBias", &float3(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, Bias));
		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect4, "NearFar", &float2_t(*r_near, *r_far));
	}
	else
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect9, "PosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect9, "PixelSize", &float2(BlurPixel / MLSet::SizeTexDepthLocal.x, BlurPixel / MLSet::SizeTexDepthLocal.y));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect9, "SizeMapBias", &float3(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.y, Bias));
		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowDirect9, "NearFar", &float2_t(*r_near, *r_far));
	}

	float4x4 camview;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &camview);

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, camview);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth, "ViewInv", &ViewInv);

	SGCore_ScreenQuadDraw();

	MLSet::DXDevice->SetVertexShader(0);
	MLSet::DXDevice->SetPixelShader(0);

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

///////////////////////////////

void ShadowMapCubeTech::OnLostDevice()
{
	mem_release_del(DepthMap);
	mem_release_del(DepthStencilSurface);

	mem_release_del(DepthSurface[0]);
	mem_release_del(DepthSurface[1]);
	mem_release_del(DepthSurface[2]);
	mem_release_del(DepthSurface[3]);
	mem_release_del(DepthSurface[4]);
	mem_release_del(DepthSurface[5]);

	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);
}

void ShadowMapCubeTech::OnResetDevice()
{
	D3DXCreateCubeTexture(MLSet::DXDevice, MLSet::SizeTexDepthLocal.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &DepthMap);
	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.x, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
}

ShadowMapCubeTech::~ShadowMapCubeTech()
{
	for (int i = 0; i < 6; ++i)
	{
		mem_release_del(Frustums[i]);
		mem_release_del(DepthSurface[i]);
	}

	for (int i = 0; i < IDArr.size(); i++)
	{
		mem_delete_a(IDArr[i]);
	}

	mem_release_del(DepthMap);
	mem_release_del(DepthStencilSurface);

	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);
}

ShadowMapCubeTech::ShadowMapCubeTech()
{
	for (int i = 0; i < 6; ++i)
	{
		Frustums[i] = 0;
		DepthSurface[i] = 0;
		//IDArr[i] = -1;
	}

	EnableEdge[0] = EnableEdge[1] = EnableEdge[2] = EnableEdge[3] = EnableEdge[4] = EnableEdge[5] = true;
	EnableEdgeNulled[0] = EnableEdgeNulled[1] = EnableEdgeNulled[2] = EnableEdgeNulled[3] = EnableEdgeNulled[4] = EnableEdgeNulled[5] = false;

	Bias = 0.1;
	BlurPixel = 16;

	DepthMap = 0;
	DepthStencilSurface = 0;
	OldColorSurface = 0;
	OldDepthStencilSurface = 0;
}

void ShadowMapCubeTech::SetEnableCubeEdge(int edge,bool enable)
{
	if (edge >= 0 && edge <= 5)
	{
		if (EnableEdge[edge] && !enable)
			EnableEdgeNulled[edge] = false;
		EnableEdge[edge] = enable;
		
	}
}

bool ShadowMapCubeTech::GetEnableCubeEdge(int edge)
{
		if(edge >= 0 && edge <= 5)
			return EnableEdge[edge];
		return(false);
}

void ShadowMapCubeTech::SetBias(float bias)
{ 
	Bias = bias; 
}

float ShadowMapCubeTech::GetBias()
{ 
	return Bias; 
}

void ShadowMapCubeTech::SetBlurPixel(float blur_pixel)
{ 
	BlurPixel = blur_pixel; 
}

bool ShadowMapCubeTech::GetBlurPixel()
{ 
	return BlurPixel; 
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
	for (int i = 0; i < 6; ++i)
	{
		Frustums[i] = SGCore_CrFrustum();
	}

	DepthSurface[0] = 0;
	DepthSurface[1] = 0;
	DepthSurface[2] = 0;
	DepthSurface[3] = 0;
	DepthSurface[4] = 0;
	DepthSurface[5] = 0;

	D3DXCreateCubeTexture(MLSet::DXDevice, MLSet::SizeTexDepthLocal.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &DepthMap);
	MLSet::DXDevice->CreateDepthStencilSurface(MLSet::SizeTexDepthLocal.x, MLSet::SizeTexDepthLocal.x, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
}

void ShadowMapCubeTech::Begin()
{
	MLSet::DXDevice->GetDepthStencilSurface(&OldDepthStencilSurface);
	MLSet::DXDevice->SetDepthStencilSurface(DepthStencilSurface);
	MLSet::DXDevice->GetRenderTarget(0, &OldColorSurface);

	/*MLSet::DXDevice->GetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->GetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->GetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);
}

void ShadowMapCubeTech::Pre(int cube)
{
	if (!EnableEdge[cube])
	{
		if (!EnableEdgeNulled[cube])
		{
			EnableEdgeNulled[cube] = true;
			mem_release_del(DepthSurface[cube]);

			DepthMap->GetCubeMapSurface((D3DCUBEMAP_FACES)cube, 0, &DepthSurface[cube]);
			MLSet::DXDevice->SetRenderTarget(0, DepthSurface[cube]);

			MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
		}
		return;
	}

	View[cube] = SMMatrixLookAtLH(Position, MLSet::OrientedCube[cube] + Position, MLSet::UpVectorsCube[cube]);

	Proj[cube] = SMMatrixPerspectiveFovLH(SM_PI/2.f,1,NearFar.x,NearFar.y);

	//MLSet::DXDevice->SetTransform(D3DTS_VIEW,&(View[cube].operator D3DXMATRIX()));
	//MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&(Proj[cube].operator D3DXMATRIX()));

	Frustums[cube]->update(&(View[cube]), &(Proj[cube]));

	float4x4 vp = View[cube] * Proj[cube];

	//MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&(vp.operator D3DXMATRIX()));

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &View[cube]);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &Proj[cube]);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &vp);

	vp = SMMatrixTranspose(vp);

	mem_release_del(DepthSurface[cube]);
	
    DepthMap->GetCubeMapSurface( ( D3DCUBEMAP_FACES ) cube, 0, &DepthSurface[cube] );
	MLSet::DXDevice->SetRenderTarget( 0, DepthSurface[cube] );
	
	MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::SMDepthGeomCube);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube, "LightPos", &Position);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::SMDepthGeomCube, "WorldViewProjection", &vp);
}

void ShadowMapCubeTech::Post(int cube)
{
		if(GetAsyncKeyState(VK_NUMPAD5))
		{
			char tmpstr[1024];
			sprintf(tmpstr,"C:\\1\\shadowcube_%d.bmp",cube);
			D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_PNG, DepthSurface[cube], NULL,0);
		}
	mem_release_del(DepthSurface[cube]);
}

void ShadowMapCubeTech::End()
{
	MLSet::DXDevice->SetDepthStencilSurface(OldDepthStencilSurface);
	MLSet::DXDevice->SetRenderTarget(0, OldColorSurface);
	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);

	/*MLSet::DXDevice->SetTransform(D3DTS_VIEW,&OldView);
	MLSet::DXDevice->SetTransform(D3DTS_PROJECTION,&OldProj);
	MLSet::DXDevice->SetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);
}

void ShadowMapCubeTech::GenShadow2(IDirect3DTexture9* shadowmap)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	shadowmap->GetSurfaceLevel(0, &RenderSurf);
	MLSet::DXDevice->GetRenderTarget(0, &BackBuf);
	MLSet::DXDevice->SetRenderTarget(0, RenderSurf);

	//MLSet::DXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, 0);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);

	MLSet::DXDevice->SetTexture(0, SGCore_RTGetTexture(MLSet::IDsRenderTargets::DepthScene));

	MLSet::DXDevice->SetTexture(1, DepthMap);
	MLSet::DXDevice->SetTexture(2, SGCore_LoadTexGetTex(MLSet::IDsTexs::Tex_NoiseTex));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth);
	if (MLSet::IsHalfGenPCFShadowLocal)
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube1);
	else
		SGCore_ShaderBind(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube6);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth, "ParamProj", &float3_t(*r_win_width, *r_win_height, *r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth, "NearFar", &float2_t(*r_near, *r_far));

	float pixel_size = BlurPixel / MLSet::SizeTexDepthLocal.x;
	float3 observerpos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &observerpos);

	if (MLSet::IsHalfGenPCFShadowLocal)
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube1, "PosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube1, "LightPos", &Position);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube1, "SizeMapBias", &float2(MLSet::SizeTexDepthLocal.x, Bias));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube1, "PixelSize", &pixel_size);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube1, "LightPos", &Position);
	}
	else
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube6, "PosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube6, "LightPos", &Position);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube6, "SizeMapBias", &float2(MLSet::SizeTexDepthLocal.x, Bias));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube6, "PixelSize", &pixel_size);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, MLSet::IDsShaders::PS::GenShadowCube6, "LightPos", &Position);
	}

	float4x4 camview;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &camview);

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, camview);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, MLSet::IDsShaders::VS::ResPosDepth, "ViewInv", &ViewInv);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	MLSet::DXDevice->SetRenderTarget(0, BackBuf);
	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

void ShadowMapCubeTech::SetIDArr(long id, int split, long idarr)
{
	if (id < 0 || !(split >= 0 && split < 6))
		return;

	if (id >= IDArr.size())
	{
		long* tmparr = new long[6];
		for (int i = 0; i < 6; ++i)
			tmparr[i] = -1;
		IDArr[id] = tmparr;
	}

	IDArr[id][split] = idarr;
}

long ShadowMapCubeTech::GetCountIDArrs()
{
	return IDArr.size();
}

long ShadowMapCubeTech::GetIDArr(long id, int split)
{
	if (id < 0 || !(split >= 0 && split < 6) || !(id < IDArr.size()))
		return -2;

	return IDArr[id][split];
}