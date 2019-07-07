
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
			m_aDepthMaps[i] = 0;
			m_aFrustums[i] = 0;
			m_aIsUpdate[i] = 0;
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
		mem_release_del(m_aDepthMaps[i]);
		mem_release_del(DepthSurfaces[i]);
		mem_release_del(m_aFrustums[i]);
	}

	for (UINT i = 0; i < IDArr.size(); i++)
	{
		mem_delete_a(IDArr[i]);
	}

	IDArr.clear();

	mem_release_del(DepthStencilSurface);
	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);
}

void PSSM::setPosition(const float3* pos)
{ 
	Position = *pos; 
}

void PSSM::getPosition(float3* pos)
{ 
	*pos = Position; 
}

void PSSM::set4Or3Splits(bool is4)
{ 
	Generating4Slits = is4; 
}

bool PSSM::get4Or3Splits()
{ 
	return Generating4Slits; 
}

void PSSM::setBlurPixel(float blur_pixel)
{
	BlurPixel = blur_pixel;
}

bool PSSM::getBlurPixel()
{
	return BlurPixel;
}

void PSSM::setIDArr(ID id, int split, ID idarr)
{
	if (id < 0 || !(split >= 0 && split < 5))
		return;

	if (id >= (ID)IDArr.size())
	{
		ID* tmparr = new ID[5];
		for (int i = 0; i < 5; ++i)
			tmparr[i] = -1;
		IDArr[id] = tmparr;
	}

	IDArr[id][split] = idarr;
}

long PSSM::getCountIDArrs()
{
	return IDArr.size();
}

ID PSSM::getIDArr(ID id, int split)
{
	if (id < 0 || !(split >= 0 && split < 5) || !(id < (ID)IDArr.size()))
		return -2;

	return IDArr[id][split];
}

void PSSM::onLostDevice()
{
		for(int i=0;i<5;i++)
		{
	//		mem_release_del(m_aDepthMaps[i]);
	//		mem_release_del(DepthSurfaces[i]);
		}

//	mem_release_del(DepthStencilSurface);
}

void PSSM::onResetDevice()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	for(int i = 0; i < 5; i++)
	{
		m_aIsUpdate[i] = 0;

	//	light_data::pDXDevice->CreateTexture(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &(m_aDepthMaps[i]), NULL);
	//	m_aDepthMaps[i] = light_data::pDXDevice->createTexture2D(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);
		
		DepthSurfaces[i] = 0;
	}

//	light_data::pDXDevice->CreateDepthStencilSurface(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	
	FovRatio.x = *r_default_fov;
	FovRatio.y = float(*r_win_width) / float(*r_win_height);

	float2 fOffset = float2(0.5, 0.5) - (float2(0.5f, 0.5f) / light_data::vSizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.000f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}

void PSSM::init()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	FovRatio.x = *r_default_fov;
	FovRatio.y = float(*r_win_width) / float(*r_win_height);

	for(int i = 0; i < 5; i++)
	{
		m_aIsUpdate[i] = 0;

	//	light_data::pDXDevice->CreateTexture(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &(m_aDepthMaps[i]), NULL);
		m_aDepthMaps[i] = light_data::pDXDevice->createTexture2D(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, 1, GX_TEXFLAG_RENDERTARGET, GXFMT_R32F);

		DepthSurfaces[i] = 0;

		m_aFrustums[i] = SGCore_CrFrustum();
	}

		//m_aMask[]

	m_aNearFar[0].x = *r_near;
	m_aNearFar[0].y = light_data::vDistForPSSM.x;

	m_aNearFar[1].x = light_data::vDistForPSSM.x;
	m_aNearFar[1].y = light_data::vDistForPSSM.y;

	m_aNearFar[2].x = light_data::vDistForPSSM.y;
	m_aNearFar[2].y = light_data::vDistForPSSM.z;

	m_aNearFar[3].x = light_data::vDistForPSSM.z;
	m_aNearFar[3].y = light_data::vDistForPSSM.w;


	m_aNearFar[4].x = *r_near;
	m_aNearFar[4].y = *r_far;

//	light_data::pDXDevice->CreateDepthStencilSurface(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	DepthStencilSurface = light_data::pDXDevice->createDepthStencilSurface(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, GXFMT_D24X8, GXMULTISAMPLE_NONE, true);
	
	float2 fOffset = float2(0.5,0.5) + (float2(0.5f,0.5)/light_data::vSizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}


void PSSM::updateFrustums(int split, const float3* poscam, const float3* dircam)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	FovRatio.x = *r_default_fov;
	FovRatio.y = float(*r_win_width) / float(*r_win_height);

	m_aFrustums[split]->update(&(Views[split]), &(Projs[split]));

	float3 up(0.0f, 1.0f, 0.0f);
	float3 right = SMVector3Normalize(SMVector3Cross((*dircam), up));

	float3 fc = (*poscam) + (*dircam)*m_aNearFar[split].y;
	float3 nc = (*poscam) + (*dircam)*m_aNearFar[split].x;

	up = SMVector3Normalize(SMVector3Cross(right, (*dircam)));

	float near_height = tan(FovRatio.x / 2.f) * m_aNearFar[split].x;
	float near_width = near_height * FovRatio.y;
	float far_height = tan(FovRatio.x / 2.f) * m_aNearFar[split].y;
	float far_width = far_height * FovRatio.y;

	m_aFrustums[split]->setPoint(0, &float3(nc - up*near_height - right*near_width));
	m_aFrustums[split]->setPoint(1, &float3(nc + up*near_height - right*near_width));
	m_aFrustums[split]->setPoint(2, &float3(nc + up*near_height + right*near_width));
	m_aFrustums[split]->setPoint(3, &float3(nc - up*near_height + right*near_width));

	m_aFrustums[split]->setPoint(4, &float3(fc - up*far_height - right*far_width));
	m_aFrustums[split]->setPoint(5, &float3(fc + up*far_height - right*far_width));
	m_aFrustums[split]->setPoint(6, &float3(fc + up*far_height + right*far_width));
	m_aFrustums[split]->setPoint(7, &float3(fc - up*far_height + right*far_width));

	float3 vCenter(0, 0, 0);
	for (int i = 0; i < 8; i++)
		vCenter += m_aFrustums[split]->getPoint(i);
	vCenter /= 8;
	m_aFrustums[split]->setCenter(&vCenter);

	float dist = 1;
	float3 DirL = Position;
	float3 TarG = float3(m_aFrustums[split]->getCenter());

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
	float4 transform0(m_aFrustums[split]->getPoint(0), 1);
	trans0 = SMVector4Transform(transform0, Views[split]);

	minX = trans0.x; maxX = trans0.x;
	minY = trans0.y; maxY = trans0.y;
	maxZ = trans0.z;

	for (int i = 0; i<8; i++)
	{
		float4 trans;
		float4 transform(m_aFrustums[split]->getPoint(i), 1);

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

	float3 Diagonal = m_aFrustums[split]->getPoint(0) - m_aFrustums[split]->getPoint(6);
	float LengthDiagonal = SMVector3Length(Diagonal);

	float2 BoarderOffset = (float2(LengthDiagonal, LengthDiagonal) - (OrtMax - OrtMin)) * 0.5;

	OrtMax += BoarderOffset;
	OrtMin -= BoarderOffset;

	float2 WorldUnitsPerTexel = float2(LengthDiagonal, LengthDiagonal) / float2(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y);

	OrtMin /= WorldUnitsPerTexel;
	OrtMin.x = floor(OrtMin.x);
	OrtMin.y = floor(OrtMin.y);
	OrtMin *= WorldUnitsPerTexel;

	OrtMax /= WorldUnitsPerTexel;
	OrtMax.x = floor(OrtMax.x);
	OrtMax.y = floor(OrtMax.y);
	OrtMax *= WorldUnitsPerTexel;

//	D3DXMATRIX tmpproj;
//	D3DXMatrixOrthoOffCenterLH(&tmpproj, OrtMin.x, OrtMax.x, OrtMin.y, OrtMax.y, minZ, maxZ);
	Projs[split] = SMMatrixOrthographicLH(OrtMax.x - OrtMin.x, OrtMax.y - OrtMin.y, minZ, maxZ);
//	Projs[split] = float4x4(tmpproj);

	ViewProj[split] = Views[split] * Projs[split];
	flickering(&ViewProj[split], light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y);
}

void PSSM::preRender(int split)
{
	/*light_data::pDXDevice->SetTransform(D3DTS_WORLD1,&(ViewProj[split].operator D3DXMATRIX()));
	light_data::pDXDevice->SetTransform(D3DTS_VIEW,&(Views[split].operator D3DXMATRIX()));
	light_data::pDXDevice->SetTransform(D3DTS_PROJECTION,&(Projs[split].operator D3DXMATRIX()));*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &Views[split]);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &Projs[split]);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &ViewProj[split]);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSMDirect, "g_mWVP", &SMMatrixTranspose(ViewProj[split]));
	//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSMDirect, "g_vLigthPos", &Position);
	SGCore_ShaderBind(light_data::shader_id::kit::idSMDepthGeomPSSMDirect);

	mem_release(DepthSurfaces[split]);
	DepthSurfaces[split] = m_aDepthMaps[split]->getMipmap();
	
	light_data::pDXDevice->setColorTarget(DepthSurfaces[split]);
	

	light_data::pDXDevice->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH, GX_COLOR_ARGB(255, 255, 255, 255));
}

void PSSM::begin()
{
	OldDepthStencilSurface = light_data::pDXDevice->getDepthStencilSurface();
	light_data::pDXDevice->setDepthStencilSurface(DepthStencilSurface);
	OldColorSurface = light_data::pDXDevice->getColorTarget();

	/*light_data::pDXDevice->GetTransform(D3DTS_VIEW,&OldView);
	light_data::pDXDevice->GetTransform(D3DTS_PROJECTION,&OldProj);
	light_data::pDXDevice->GetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);
}

void PSSM::end()
{
	light_data::pDXDevice->setDepthStencilSurface(OldDepthStencilSurface);
	light_data::pDXDevice->setColorTarget(OldColorSurface);

	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);

	/*light_data::pDXDevice->SetTransform(D3DTS_VIEW,&OldView);
	light_data::pDXDevice->SetTransform(D3DTS_PROJECTION,&OldProj);
	light_data::pDXDevice->SetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);

	/*if (GetAsyncKeyState(VK_NUMPAD1))
	{
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow0.png", D3DXIFF_PNG, m_aDepthMaps[0], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow1.png", D3DXIFF_PNG, m_aDepthMaps[1], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow2.png", D3DXIFF_PNG, m_aDepthMaps[2], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow3.png", D3DXIFF_PNG, m_aDepthMaps[3], NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow4.png", D3DXIFF_PNG, m_aDepthMaps[4], NULL);
	}*/

	
}

void PSSM::flickering(float4x4 *matLVP,float size_x,float size_y)
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

void PSSM::genShadow(IGXTexture2D* shadowmap)
{
#if 0
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	IGXSurface *RenderSurf, *BackBuf;

	RenderSurf = shadowmap->getMipmap();
	BackBuf = light_data::pDXDevice->getColorTarget();
	light_data::pDXDevice->setColorTarget(RenderSurf);

	//light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	light_data::pDXDevice->setSamplerState(light_data::pSamplerPointClamp, 0);
	for(UINT i = 1; i <= 6; ++i)
	{
		light_data::pDXDevice->setSamplerState(light_data::pSamplerLinearClamp, i);
	}

//	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
//	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
//	SGCore_SetSamplerFilter2(1, 6, D3DTEXF_LINEAR);
//	SGCore_SetSamplerAddress2(1, 6, D3DTADDRESS_CLAMP);

	light_data::pDXDevice->setTexture(SGCore_GbufferGetRT(DS_RT_DEPTH));
	light_data::pDXDevice->setTexture(SGCore_GbufferGetRT(DS_RT_NORMAL), 6);

	float4x4 aMatrixTexture[5];

	float4x4 MatrixTexture;
	//char mattex[16];
	for (int i = 0; i<5; i++)
	{
		light_data::pDXDevice->setTexture(m_aDepthMaps[i], 1 + i);
		MatrixTexture = ViewProj[i] * ScaleBiasMat;
		MatrixTexture = SMMatrixTranspose(MatrixTexture);
		aMatrixTexture[i] = MatrixTexture;
	}

	if(Generating4Slits)
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idPSSM4, "g_aMatrixTexture", &aMatrixTexture);
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthPSSM4);
	}
	else
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idPSSM3, "g_aMatrixTexture", &aMatrixTexture);
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthPSSM3);
	}

	float4x4 camview, mProjection;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &camview);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mProjection);

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, camview);
	ViewInv = SMMatrixTranspose(ViewInv);

	float4x4 mInvVP = SMMatrixTranspose(SMMatrixInverse(&determ, camview * mProjection));

	float3 observerpos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &observerpos);
	
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vParamProj", &float3_t((float)*r_win_width, (float)*r_win_height, (float)*r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vNearFar", &float2_t(*r_near, *r_far));
	
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_mViewInv", &ViewInv);

	float aDistSplits[4] = { m_aNearFar[0].y, m_aNearFar[1].y, m_aNearFar[2].y, m_aNearFar[3].y };

	ID idShader = (Generating4Slits ? light_data::shader_id::ps::idPSSM4 : light_data::shader_id::ps::idPSSM3);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idShader, "g_vPosCam", &observerpos);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idShader, "g_vPixelSize", &float2(BlurPixel / light_data::vSizeTexDepthGlobal.x, BlurPixel / light_data::vSizeTexDepthGlobal.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idShader, "g_vNearFar", &float2_t(*r_near, *r_far));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idShader, "g_vDistSplit", &float4(m_aNearFar[0].y, m_aNearFar[1].y, m_aNearFar[2].y, m_aNearFar[3].y));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	light_data::pDXDevice->setColorTarget(BackBuf);

	mem_release(RenderSurf);
	mem_release(BackBuf);

	if(GetAsyncKeyState(VK_NUMPAD1) < 0)
	{
		light_data::pDXDevice->saveTextureToFile("C:\\1\\pssm.png", shadowmap);
	}
#endif
}

void PSSM::genShadowAll(IGXTexture2D* shadowmap)
{
	IGXSurface *RenderSurf, *BackBuf;

	RenderSurf = shadowmap->getMipmap();
	BackBuf = light_data::pDXDevice->getColorTarget();
	light_data::pDXDevice->setColorTarget(RenderSurf);


	light_data::pDXDevice->clear(GX_CLEAR_COLOR);

	light_data::pDXDevice->setColorTarget(BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

//##########################################################################

void ShadowMapTech::onLostDevice()
{
//	mem_release_del(DepthMap);
//	mem_release_del(DepthStencilSurface);
	mem_release_del(DepthSurface);
}

void ShadowMapTech::onResetDevice()
{
//	light_data::pDXDevice->CreateTexture(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &(DepthMap), NULL);
//	light_data::pDXDevice->CreateDepthStencilSurface(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.y, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
}

ShadowMapTech::ShadowMapTech()
{
	Bias = 0.0001f;
	BlurPixel = 0.5f;

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

void ShadowMapTech::setPosition(const float3* pos)
{
	Position = *pos;
}

void ShadowMapTech::getPosition(float3* pos)
{
	*pos = Position;
}

void ShadowMapTech::setDirection(const float3* dir)
{
	Direction = *dir;
}

void ShadowMapTech::getDirection(float3* dir)
{
	*dir = Direction;
}

void ShadowMapTech::setAngleNearFar(const float3* anf)
{
	AngleNearFar = *anf;
}

void ShadowMapTech::getAngleNearFar(float3* anf)
{
	*anf = AngleNearFar;
}

void ShadowMapTech::setBias(float bias)
{
	Bias = bias;
}

float ShadowMapTech::getBias()
{
	return Bias;
}


void ShadowMapTech::setBlurPixel(float blur_pixel)
{
	BlurPixel = blur_pixel;
}

float ShadowMapTech::getBlurPixel()
{
	return BlurPixel;
}

void ShadowMapTech::init()
{
	Frustum = SGCore_CrFrustum();
	
	DepthSurface = 0;
	DepthMap = light_data::pDXDevice->createTexture2D(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.y, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R32F);
	DepthStencilSurface = light_data::pDXDevice->createDepthStencilSurface(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.y, GXFMT_D24S8, GXMULTISAMPLE_NONE, true);

	float fOffset = 0.5f + (0.5f/light_data::vSizeTexDepthLocal.x);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset,	fOffset,	fBias,		1.0f);
}

void ShadowMapTech::setFar(float sfar)
{
	AngleNearFar.z = sfar;
}

float ShadowMapTech::getFar()
{
	return AngleNearFar.z;
}

void ShadowMapTech::setNear(float snear)
{
	AngleNearFar.y = snear;
}

float ShadowMapTech::getNear()
{
	return AngleNearFar.y;
}

void ShadowMapTech::setAngle(float sangle)
{
	AngleNearFar.x = sangle;
}

float ShadowMapTech::getAngle()
{
	return AngleNearFar.x;
}

void ShadowMapTech::setIDArr(long id, long idarr)
{
	if (id < 0)
		return;

	IDArr[id] = idarr;
}

long ShadowMapTech::getCountIDArrs()
{
	return IDArr.size();
}

long ShadowMapTech::getIDArr(long id)
{
	if (id < 0 || !(id < (ID)IDArr.size()))
		return -2;

	return IDArr[id];
}

void ShadowMapTech::begin()
{
	OldDepthStencilSurface = light_data::pDXDevice->getDepthStencilSurface();
	light_data::pDXDevice->setDepthStencilSurface(DepthStencilSurface);
	OldColorSurface = light_data::pDXDevice->getColorTarget();

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);

	float3 upvec = float3(0, 1, 0);
	
	if (Direction.x == 0 && Direction.y == -1 && Direction.z == 0)
		upvec = SMVector3Transform(Direction, SMMatrixRotationZ(1.57f));

	View = SMMatrixLookAtLH(Position, Position + Direction, upvec);
	Proj = SMMatrixPerspectiveFovLH(AngleNearFar.x,light_data::vSizeTexDepthLocal.x / light_data::vSizeTexDepthLocal.y,AngleNearFar.y,AngleNearFar.z);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &View);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &Proj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(View * Proj));
	
	Frustum->update(&(View),&(Proj));

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSMDirect, "g_mWVP", &SMMatrixTranspose(View * Proj));
	SGCore_ShaderBind(light_data::shader_id::kit::idSMDepthGeomPSSMDirect);

	mem_release_del(DepthSurface);
	DepthSurface = DepthMap->getMipmap();
	
	light_data::pDXDevice->setColorTarget(DepthSurface);
	
	light_data::pDXDevice->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH, GX_COLOR_ARGB(255,255,255,255));
}

void ShadowMapTech::end()
{
	light_data::pDXDevice->setDepthStencilSurface(OldDepthStencilSurface);
	light_data::pDXDevice->setColorTarget(OldColorSurface);

	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);

	/*light_data::pDXDevice->SetTransform(D3DTS_VIEW,&OldView);
	light_data::pDXDevice->SetTransform(D3DTS_PROJECTION,&OldProj);
	light_data::pDXDevice->SetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);
}

void ShadowMapTech::genShadow(IGXTexture2D* shadowmap)
{
#if 0
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	IGXSurface *RenderSurf, *BackBuf;
	RenderSurf = shadowmap->getMipmap(0);
	BackBuf = light_data::pDXDevice->getColorTarget();
	light_data::pDXDevice->setColorTarget(RenderSurf);

	//light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	light_data::pDXDevice->setSamplerState(light_data::pSamplerPointClamp, 0);
	light_data::pDXDevice->setSamplerState(light_data::pSamplerLinearClamp, 1);
	light_data::pDXDevice->setSamplerState(light_data::pSamplerPointWrap, 2);

//	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
//	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
//	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
//	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);
//	SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
//	SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);

	light_data::pDXDevice->setTexture(SGCore_GbufferGetRT(DS_RT_DEPTH));

	float4x4 MatrixTexture;

	light_data::pDXDevice->setTexture(DepthMap, 1);

	light_data::pDXDevice->setTexture(SGCore_LoadTexGetTex(light_data::texture_id::idNoiseTex), 2);
	MatrixTexture = View * Proj * ScaleBiasMat;
	MatrixTexture = SMMatrixTranspose(MatrixTexture);

	
	if(light_data::isHalfGenPCFShadowLocal)
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect4, "g_mMatrixTexture", &MatrixTexture);
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowDirect4);
	}
	else
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "g_mMatrixTexture", &MatrixTexture);
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowDirect9);
	}

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vParamProj", &float3_t((float)*r_win_width, (float)*r_win_height, (float)*r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vNearFar", &float2_t(*r_near, *r_far));

	float3 observerpos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &observerpos);

	if (light_data::isHalfGenPCFShadowLocal)
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect4, "g_vPosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect4, "g_vPixelSize", &float2(BlurPixel / light_data::vSizeTexDepthLocal.x, BlurPixel / light_data::vSizeTexDepthLocal.y));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect4, "g_vSizeMapBias", &float3(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.y, Bias));
		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect4, "NearFar", &float2_t(*r_near, *r_far));
	}
	else
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "g_vPosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "g_vPixelSize", &float2(BlurPixel / light_data::vSizeTexDepthLocal.x, BlurPixel / light_data::vSizeTexDepthLocal.y));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "g_vSizeMapBias", &float3(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.y, Bias));
		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "NearFar", &float2_t(*r_near, *r_far));
	}

	float4x4 camview;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &camview);

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, camview);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_mViewInv", &ViewInv);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	light_data::pDXDevice->setColorTarget(BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
#endif
}

//##########################################################################

void ShadowMapCubeTech::onLostDevice()
{
//	mem_release_del(DepthMap);
//	mem_release_del(DepthStencilSurface);

	mem_release_del(DepthSurface[0]);
	mem_release_del(DepthSurface[1]);
	mem_release_del(DepthSurface[2]);
	mem_release_del(DepthSurface[3]);
	mem_release_del(DepthSurface[4]);
	mem_release_del(DepthSurface[5]);

	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);
}

void ShadowMapCubeTech::onResetDevice()
{
/*	HRESULT hr = D3DXCreateCubeTexture(light_data::pDXDevice, light_data::vSizeTexDepthLocal.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &DepthMap);

	if (hr == D3DERR_INVALIDCALL)
		int qwert = 0;
	else if (hr == D3DERR_NOTAVAILABLE)
		int qwert = 0;
	else if (hr == D3DERR_OUTOFVIDEOMEMORY)
		int qwert = 0;
	else if (hr == E_OUTOFMEMORY)
		int qwert = 0;

	light_data::pDXDevice->CreateDepthStencilSurface(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.x, D3DFMT_D24X8,D3DMULTISAMPLE_NONE, 0, TRUE, &DepthStencilSurface, NULL);
	*/
}

ShadowMapCubeTech::~ShadowMapCubeTech()
{
	for (int i = 0; i < 6; ++i)
	{
		mem_release_del(m_aFrustums[i]);
		mem_release_del(DepthSurface[i]);
	}

	for (UINT i = 0; i < IDArr.size(); i++)
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
		m_aFrustums[i] = 0;
		DepthSurface[i] = 0;
		//IDArr[i] = -1;
	}

	EnableEdge[0] = EnableEdge[1] = EnableEdge[2] = EnableEdge[3] = EnableEdge[4] = EnableEdge[5] = true;
	EnableEdgeNulled[0] = EnableEdgeNulled[1] = EnableEdgeNulled[2] = EnableEdgeNulled[3] = EnableEdgeNulled[4] = EnableEdgeNulled[5] = false;

	Bias = 0.001f;
	BlurPixel = 16.0f;

	DepthMap = 0;
	DepthStencilSurface = 0;
	OldColorSurface = 0;
	OldDepthStencilSurface = 0;
}

void ShadowMapCubeTech::setEnableCubeEdge(int edge,bool enable)
{
	if (edge >= 0 && edge <= 5)
	{
		if (EnableEdge[edge] && !enable)
			EnableEdgeNulled[edge] = false;
		EnableEdge[edge] = enable;
		
	}
}

bool ShadowMapCubeTech::getEnableCubeEdge(int edge)
{
		if(edge >= 0 && edge <= 5)
			return EnableEdge[edge];
		return(false);
}

void ShadowMapCubeTech::setBias(float bias)
{ 
	Bias = bias; 
}

float ShadowMapCubeTech::getBias()
{ 
	return Bias; 
}

void ShadowMapCubeTech::setBlurPixel(float blur_pixel)
{ 
	BlurPixel = blur_pixel; 
}

bool ShadowMapCubeTech::getBlurPixel()
{ 
	return BlurPixel; 
}

void ShadowMapCubeTech::setPosition(float3* pos)
{
	Position = *pos;
}

void ShadowMapCubeTech::getPosition(float3* pos)
{
	*pos = Position;
}

void ShadowMapCubeTech::setNearFar(float2* nf)
{
	m_vNearFar = *nf;
}

void ShadowMapCubeTech::getNearFar(float2* nf)
{
	*nf = m_vNearFar;
}

void ShadowMapCubeTech::setNear(float val)
{
	m_vNearFar.x = val;
}

float ShadowMapCubeTech::getNear()
{
	return m_vNearFar.x;
}


void ShadowMapCubeTech::setFar(float val)
{
	m_vNearFar.y = val;
}

float ShadowMapCubeTech::getFar()
{
	return m_vNearFar.y;
}


void ShadowMapCubeTech::init()
{
	for (int i = 0; i < 6; ++i)
	{
		m_aFrustums[i] = SGCore_CrFrustum();
	}

	DepthSurface[0] = 0;
	DepthSurface[1] = 0;
	DepthSurface[2] = 0;
	DepthSurface[3] = 0;
	DepthSurface[4] = 0;
	DepthSurface[5] = 0;

	DepthMap = light_data::pDXDevice->createTextureCube(light_data::vSizeTexDepthLocal.x, 1, GX_TEXFLAG_RENDERTARGET | GX_TEXFLAG_AUTORESIZE, GXFMT_R32F);
	
	DepthStencilSurface = light_data::pDXDevice->createDepthStencilSurface(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.x, GXFMT_D24S8, GXMULTISAMPLE_NONE, true);
}

void ShadowMapCubeTech::begin()
{
	OldDepthStencilSurface = light_data::pDXDevice->getDepthStencilSurface();
	light_data::pDXDevice->setDepthStencilSurface(DepthStencilSurface);
	OldColorSurface = light_data::pDXDevice->getColorTarget();

	/*light_data::pDXDevice->GetTransform(D3DTS_VIEW,&OldView);
	light_data::pDXDevice->GetTransform(D3DTS_PROJECTION,&OldProj);
	light_data::pDXDevice->GetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);
}

void ShadowMapCubeTech::pre(int cube)
{
	if (!EnableEdge[cube])
	{
		if (!EnableEdgeNulled[cube])
		{
			EnableEdgeNulled[cube] = true;
			mem_release_del(DepthSurface[cube]);

			DepthSurface[cube] = DepthMap->getMipmap((GXCUBEMAP_FACES)cube, 0);
			light_data::pDXDevice->setColorTarget(DepthSurface[cube]);

			light_data::pDXDevice->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH, GX_COLOR_ARGB(255, 255, 255, 255));
		}
		return;
	}

	View[cube] = SMMatrixLookAtLH(Position, light_data::vOrientedCube[cube] + Position, light_data::vUpVectorsCube[cube]);

	Proj[cube] = SMMatrixPerspectiveFovLH(SM_PI / 2.f, 1, m_vNearFar.x, m_vNearFar.y);

	//light_data::pDXDevice->SetTransform(D3DTS_VIEW,&(View[cube].operator D3DXMATRIX()));
	//light_data::pDXDevice->SetTransform(D3DTS_PROJECTION,&(Proj[cube].operator D3DXMATRIX()));

	m_aFrustums[cube]->update(&(View[cube]), &(Proj[cube]));

	float4x4 vp = View[cube] * Proj[cube];

	//light_data::pDXDevice->SetTransform(D3DTS_WORLD1,&(vp.operator D3DXMATRIX()));

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &View[cube]);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &Proj[cube]);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &vp);

	vp = SMMatrixTranspose(vp);

	mem_release_del(DepthSurface[cube]);
	
	DepthSurface[cube] = DepthMap->getMipmap((GXCUBEMAP_FACES)cube, 0);
	light_data::pDXDevice->setColorTarget(DepthSurface[cube]);
	
	light_data::pDXDevice->clear(GX_CLEAR_COLOR | GX_CLEAR_DEPTH, GX_COLOR_ARGB(255,255,255,255));

	SGCore_ShaderBind(light_data::shader_id::kit::idSMDepthGeomCube);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube, "g_vLightPos", &Position);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube, "g_mWVP", &vp);
}

void ShadowMapCubeTech::post(int cube)
{
#ifdef _GRAPHIX_API
		if(GetAsyncKeyState(VK_NUMPAD5))
		{
			char tmpstr[1024];
			sprintf(tmpstr,"C:\\1\\shadowcube_%d.bmp",cube);
			D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_PNG, DepthSurface[cube], NULL,0);
		}
#endif
	mem_release_del(DepthSurface[cube]);
}

void ShadowMapCubeTech::end()
{
	light_data::pDXDevice->setDepthStencilSurface(OldDepthStencilSurface);
	light_data::pDXDevice->setColorTarget(OldColorSurface);
	mem_release_del(OldDepthStencilSurface);
	mem_release_del(OldColorSurface);

	/*light_data::pDXDevice->SetTransform(D3DTS_VIEW,&OldView);
	light_data::pDXDevice->SetTransform(D3DTS_PROJECTION,&OldProj);
	light_data::pDXDevice->SetTransform(D3DTS_WORLD1,&OldViewProj);*/

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &OldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &OldProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &OldViewProj);
}

void ShadowMapCubeTech::genShadow(IGXTexture2D* shadowmap)
{
#if 0
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	IGXSurface *RenderSurf, *BackBuf;

	RenderSurf = shadowmap->getMipmap(0);
	BackBuf = light_data::pDXDevice->getColorTarget();
	light_data::pDXDevice->setColorTarget(RenderSurf);

	//light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, 0);

	light_data::pDXDevice->setSamplerState(light_data::pSamplerPointClamp, 0);
	light_data::pDXDevice->setSamplerState(light_data::pSamplerLinearClamp, 1);
	light_data::pDXDevice->setSamplerState(light_data::pSamplerPointWrap, 2);
//	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
//	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
//	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
//	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);
//	SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
//	SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);

	light_data::pDXDevice->setTexture(SGCore_GbufferGetRT(DS_RT_DEPTH));

	light_data::pDXDevice->setTexture(DepthMap, 1);
	light_data::pDXDevice->setTexture(SGCore_LoadTexGetTex(light_data::texture_id::idNoiseTex), 2);

	if (light_data::isHalfGenPCFShadowLocal)
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowCube1);
	else
		SGCore_ShaderBind(light_data::shader_id::kit::idResPosDepthGenShadowCube6);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vParamProj", &float3_t((float)(float)*r_win_width, *r_win_height, (float)*r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vNearFar", &float2_t(*r_near, *r_far));

	float pixel_size = BlurPixel / light_data::vSizeTexDepthLocal.x;
	float3 observerpos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &observerpos);

	if (light_data::isHalfGenPCFShadowLocal)
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube1, "g_vPosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube1, "g_vLightPos", &Position);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube1, "g_vSizeMapBias", &float2(light_data::vSizeTexDepthLocal.x, Bias));
		//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube1, "g_fPixelSize", &pixel_size);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube1, "g_vLightPos", &Position);
	}
	else
	{
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube6, "g_vPosCam", &observerpos);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube6, "g_vLightPos", &Position);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube6, "g_vSizeMapBias", &float2(light_data::vSizeTexDepthLocal.x, Bias));
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube6, "g_fPixelSize", &pixel_size);
		SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube6, "g_vLightPos", &Position);
	}

	float4x4 camview;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &camview);

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, camview);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_mViewInv", &ViewInv);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	light_data::pDXDevice->setColorTarget(BackBuf);
	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
#endif
}

void ShadowMapCubeTech::setIDArr(long id, int split, long idarr)
{
	if (id < 0 || !(split >= 0 && split < 6))
		return;

	if (id >= (long)IDArr.size())
	{
		long* tmparr = new long[6];
		for (int i = 0; i < 6; ++i)
			tmparr[i] = -1;
		IDArr[id] = tmparr;
	}

	IDArr[id][split] = idarr;
}

long ShadowMapCubeTech::getCountIDArrs()
{
	return IDArr.size();
}

long ShadowMapCubeTech::getIDArr(long id, int split)
{
	if (id < 0 || !(split >= 0 && split < 6) || !(id < (long)IDArr.size()))
		return -2;

	return IDArr[id][split];
}