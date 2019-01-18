
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "shadow.h"
	
CShadowGlobal::CShadowGlobal()
{
	m_fBlurPixel = 2;
	m_useGenerating4Slits = true;

	for(int i=0;i<5;i++)
	{
		m_pDepthSurface = 0;
		m_aIdDepthMap[i] = -1;
		m_aFrustums[i] = 0;
		m_aIsUpdate[i] = 0;
	}

	m_pOldDepthStencilSurface = 0;
	m_pOldColorSurface = 0;
}

CShadowGlobal::~CShadowGlobal()
{
	for(int i=0; i<5; ++i)
	{
		mem_release_del(m_aFrustums[i]);
	}

	mem_release(m_pDepthSurface);

	for (UINT i = 0; i < m_aVisCalcObjs.size(); ++i)
	{
		mem_delete_a(m_aVisCalcObjs[i]);
	}

	m_aVisCalcObjs.clear();

	mem_release_del(m_pOldDepthStencilSurface);
	mem_release_del(m_pOldColorSurface);
}

void CShadowGlobal::setPosition(const float3 *pPos)
{ 
	m_vPosition = *pPos;
}

void CShadowGlobal::getPosition(float3 *pPos)
{ 
	*pPos = m_vPosition;
}

void CShadowGlobal::set4Or3Splits(bool is4)
{ 
	m_useGenerating4Slits = is4; 
}

bool CShadowGlobal::get4Or3Splits()
{ 
	return m_useGenerating4Slits; 
}

void CShadowGlobal::setBlurPixel(float fBlurPixel)
{
	m_fBlurPixel = fBlurPixel;
}

bool CShadowGlobal::getBlurPixel()
{
	return m_fBlurPixel;
}

void CShadowGlobal::setIdVisCalcObj(ID id, int iSplit, ID idVisCalcObj)
{
	if (id < 0 || !(iSplit >= 0 && iSplit < 5))
		return;

	if (id >= (ID)m_aVisCalcObjs.size())
	{
		ID* tmparr = new ID[5];
		for (int i = 0; i < 5; ++i)
			tmparr[i] = -1;
		m_aVisCalcObjs[id] = tmparr;
	}

	m_aVisCalcObjs[id][iSplit] = idVisCalcObj;
}

int CShadowGlobal::getCountVisCalcObj()
{
	return m_aVisCalcObjs.size();
}

ID CShadowGlobal::getIdVisCalcObj(ID id, int iSplit)
{
	if (id < 0 || !(iSplit >= 0 && iSplit < 5) || !(id < (ID)m_aVisCalcObjs.size()))
		return -2;

	return m_aVisCalcObjs[id][iSplit];
}

void CShadowGlobal::onLostDevice()
{
	mem_release(m_pDepthSurface);
}

void CShadowGlobal::onResetDevice()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	for(int i=0; i<5; ++i)
	{
		m_aIsUpdate[i] = 0;
			
		SGCore_RTreCreate(m_aIdDepthMap[i], light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT);
	}

	m_pDepthSurface = 0;

	m_vFovRatio.x = *r_default_fov;
	m_vFovRatio.y = float(*r_win_width) / float(*r_win_height);

	float2 fOffset = float2(0.5, 0.5) - (float2(0.5f, 0.5f) / light_data::vSizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.000f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}

void CShadowGlobal::init()
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	m_vFovRatio.x = *r_default_fov;
	m_vFovRatio.y = float(*r_win_width) / float(*r_win_height);

	for(int i=0; i<5; ++i)
	{
		m_aIsUpdate[i] = 0;
		m_aIdDepthMap[i] = SGCore_RTcreateTexture(light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, (String("gshadow_") + i).c_str(), 0);
		m_aFrustums[i] = SGCore_CrFrustum();
	}

	m_pDepthSurface = 0;

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

	float2 fOffset = float2(0.5,0.5) + (float2(0.5f,0.5)/light_data::vSizeTexDepthGlobal);
	float range = 1.0f;
	float fBias = 0.0f;
	ScaleBiasMat = float4x4(	0.5f,		0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		0.0f,		range,		0.0f,
								fOffset.x,	fOffset.y,	fBias,		1.0f);
}


void CShadowGlobal::updateFrustums(int iSplit, const float3 *pObserverPos, const float3 *pObserverDir)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	m_vFovRatio.x = *r_default_fov;
	m_vFovRatio.y = float(*r_win_width) / float(*r_win_height);

	m_aFrustums[iSplit]->update(&(m_aViews[iSplit]), &(m_aProjections[iSplit]));

	float3 up(0.0f, 1.0f, 0.0f);
	float3 right = SMVector3Normalize(SMVector3Cross((*pObserverDir), up));

	float3 fc = (*pObserverPos) + (*pObserverDir)*m_aNearFar[iSplit].y;
	float3 nc = (*pObserverPos) + (*pObserverDir)*m_aNearFar[iSplit].x;

	up = SMVector3Normalize(SMVector3Cross(right, (*pObserverDir)));

	float near_height = tan(m_vFovRatio.x / 2.f) * m_aNearFar[iSplit].x;
	float near_width = near_height * m_vFovRatio.y;
	float far_height = tan(m_vFovRatio.x / 2.f) * m_aNearFar[iSplit].y;
	float far_width = far_height * m_vFovRatio.y;

	m_aFrustums[iSplit]->setPoint(0, &float3(nc - up*near_height - right*near_width));
	m_aFrustums[iSplit]->setPoint(1, &float3(nc + up*near_height - right*near_width));
	m_aFrustums[iSplit]->setPoint(2, &float3(nc + up*near_height + right*near_width));
	m_aFrustums[iSplit]->setPoint(3, &float3(nc - up*near_height + right*near_width));

	m_aFrustums[iSplit]->setPoint(4, &float3(fc - up*far_height - right*far_width));
	m_aFrustums[iSplit]->setPoint(5, &float3(fc + up*far_height - right*far_width));
	m_aFrustums[iSplit]->setPoint(6, &float3(fc + up*far_height + right*far_width));
	m_aFrustums[iSplit]->setPoint(7, &float3(fc - up*far_height + right*far_width));

	float3 vCenter(0, 0, 0);

	for (int i = 0; i < 8; i++)
		vCenter += m_aFrustums[iSplit]->getPoint(i);
	vCenter /= 8;

	m_aFrustums[iSplit]->setCenter(&vCenter);

	float dist = 1;
	float3 DirL = m_vPosition;
	float3 TarG = float3(m_aFrustums[iSplit]->getCenter());

	float3 LightPos = TarG + DirL*dist;
	float3 LightPos2 = DirL;
	float3 LightTarget = TarG;
	float3	Lightup(0.0f, 1.0f, 0.0f);
	m_aViews[iSplit] = SMMatrixLookAtLH(LightPos, LightTarget, Lightup);

	float minX = 0;
	float minY = 0;
	float minZ = 0;
	float maxX = 0;
	float maxY = 0;
	float maxZ = 0;

	float4 trans0;
	float4 transform0(m_aFrustums[iSplit]->getPoint(0), 1);
	trans0 = SMVector4Transform(transform0, m_aViews[iSplit]);

	minX = trans0.x; maxX = trans0.x;
	minY = trans0.y; maxY = trans0.y;
	maxZ = trans0.z;

	for (int i = 0; i<8; i++)
	{
		float4 trans;
		float4 transform(m_aFrustums[iSplit]->getPoint(i), 1);

		trans = SMVector4Transform(transform, m_aViews[iSplit]);

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

	float3 Diagonal = m_aFrustums[iSplit]->getPoint(0) - m_aFrustums[iSplit]->getPoint(6);
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

	D3DXMATRIX tmpproj;
	D3DXMatrixOrthoOffCenterLH(&tmpproj, OrtMin.x, OrtMax.x, OrtMin.y, OrtMax.y, minZ, maxZ);
	m_aProjections[iSplit] = float4x4(tmpproj);

	m_aViewProj[iSplit] = m_aViews[iSplit] * m_aProjections[iSplit];
	flickering(&m_aViewProj[iSplit], light_data::vSizeTexDepthGlobal.x, light_data::vSizeTexDepthGlobal.y);
}

void CShadowGlobal::preRender(int iSplit)
{
	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_aViews[iSplit]);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_aProjections[iSplit]);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &m_aViewProj[iSplit]);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSM, "g_mWVP", &SMMatrixTranspose(m_aViewProj[iSplit]));
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomPSSM);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomPSSM);

	mem_release(m_pDepthSurface);
	SGCore_RTGetTexture(m_aIdDepthMap[iSplit])->GetSurfaceLevel(0, &m_pDepthSurface);
	
	light_data::pDXDevice->SetRenderTarget(0, m_pDepthSurface);
	
	light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
}

void CShadowGlobal::begin()
{
	light_data::pDXDevice->GetDepthStencilSurface(&m_pOldDepthStencilSurface);
	light_data::pDXDevice->SetDepthStencilSurface(SGCore_RTGetSurface(light_data::texture_id::idSurfaceDepthGlobalShadow));
	light_data::pDXDevice->GetRenderTarget(0, &m_pOldColorSurface);

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &m_mOldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &m_mOldProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &m_mOldViewProj);
}

void CShadowGlobal::end()
{
	light_data::pDXDevice->SetDepthStencilSurface(m_pOldDepthStencilSurface);
	light_data::pDXDevice->SetRenderTarget(0, m_pOldColorSurface);

	mem_release_del(m_pOldDepthStencilSurface);
	mem_release_del(m_pOldColorSurface);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mOldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mOldProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &m_mOldViewProj);

	/*if (GetAsyncKeyState(VK_NUMPAD1))
	{
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow0.png", D3DXIFF_PNG, SGCore_RTGetTexture(m_aIdDepthMap[0]), NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow1.png", D3DXIFF_PNG, SGCore_RTGetTexture(m_aIdDepthMap[1]), NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow2.png", D3DXIFF_PNG, SGCore_RTGetTexture(m_aIdDepthMap[2]), NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow3.png", D3DXIFF_PNG, SGCore_RTGetTexture(m_aIdDepthMap[3]), NULL);
		D3DXSaveTextureToFile("C:\\1\\depthmapshadow4.png", D3DXIFF_PNG, SGCore_RTGetTexture(m_aIdDepthMap[4]), NULL);
	}*/
}

void CShadowGlobal::flickering(float4x4 *matLVP, float size_x, float size_y)
{
	float3 ptOriginShadow(0, 0, 0);
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

void CShadowGlobal::genShadow(IDirect3DTexture9 *pShadowMap)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	LPDIRECT3DSURFACE9 pRenderSurf, pBackBuf;

	pShadowMap->GetSurfaceLevel(0, &pRenderSurf);
	light_data::pDXDevice->GetRenderTarget(0, &pBackBuf);
	light_data::pDXDevice->SetRenderTarget(0, pRenderSurf);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter2(1, 6, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress2(1, 6, D3DTADDRESS_CLAMP);

	light_data::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_DEPTH));
	light_data::pDXDevice->SetTexture(6, SGCore_GbufferGetRT(DS_RT_NORMAL));

	float4x4 aTextures[5];
	float4x4 mTexture;
	
	for (int i = 0; i<5; ++i)
	{
		light_data::pDXDevice->SetTexture(1 + i, SGCore_RTGetTexture(m_aIdDepthMap[i]));
		mTexture = m_aViewProj[i] * ScaleBiasMat;
		mTexture = SMMatrixTranspose(mTexture);
		aTextures[i] = mTexture;
	}

	float4x4 mObserverView, mObserverProjection;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mObserverView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mObserverProjection);

	float4x4 mViewInv = SMMatrixInverse(0, mObserverView);
	mViewInv = SMMatrixTranspose(mViewInv);

	float4x4 mInvVP = SMMatrixTranspose(SMMatrixInverse(0, mObserverView * mObserverProjection));

	float3 vObserverPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);
	
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth);
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vParamProj", &float3_t((float)*r_win_width, (float)*r_win_height, (float)*r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vNearFar", &float2_t(*r_near, *r_far));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_mViewInv", &mViewInv);

	float aDistSplits[4] = { m_aNearFar[0].y, m_aNearFar[1].y, m_aNearFar[2].y, m_aNearFar[3].y };

	ID idShader = (m_useGenerating4Slits ? light_data::shader_id::ps::idPSSM4 : light_data::shader_id::ps::idPSSM3);

	SGCore_ShaderBind(SHADER_TYPE_PIXEL, idShader);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idShader, "g_vPosCam", &vObserverPos);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idShader, "g_vPixelSize", &float2(m_fBlurPixel / light_data::vSizeTexDepthGlobal.x, m_fBlurPixel / light_data::vSizeTexDepthGlobal.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idShader, "g_vNearFar", &float2_t(*r_near, *r_far));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idShader, "g_vDistSplit", &float4(m_aNearFar[0].y, m_aNearFar[1].y, m_aNearFar[2].y, m_aNearFar[3].y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, idShader, "g_aMatrixTexture", &aTextures);

	SGCore_ScreenQuadDraw();

	light_data::pDXDevice->SetVertexShader(0);
	light_data::pDXDevice->SetPixelShader(0);

	light_data::pDXDevice->SetRenderTarget(0, pBackBuf);

	mem_release_del(pRenderSurf);
	mem_release_del(pBackBuf);

	if (GetAsyncKeyState(VK_NUMPAD1))
	{
		D3DXSaveTextureToFile("C:\\1\\pssm.png", D3DXIFF_PNG, pShadowMap, NULL);
	}
}

void CShadowGlobal::genShadowAll(IDirect3DTexture9 *pShadowMap)
{
	LPDIRECT3DSURFACE9 pRenderSurf, pBackBuf;

	pShadowMap->GetSurfaceLevel(0, &pRenderSurf);
	light_data::pDXDevice->GetRenderTarget(0, &pBackBuf);
	light_data::pDXDevice->SetRenderTarget(0, pRenderSurf);

	light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	light_data::pDXDevice->SetRenderTarget(0, pBackBuf);

	mem_release_del(pRenderSurf);
	mem_release_del(pBackBuf);
}

//##########################################################################

void CShadowMap::onLostDevice()
{
	mem_release_del(m_pDepthSurface);
}

void CShadowMap::onResetDevice()
{
	
}

CShadowMap::CShadowMap()
{
	m_fBias = 0.0001f;
	m_fBlurPixel = 1.f;

	m_pFrustum = 0;

	m_idDepth = -1;
	m_pDepthSurface = 0;

	m_pOldDepthStencilSurface = 0;
	m_pOldColorSurface = 0;
}

CShadowMap::~CShadowMap()
{
	mem_release_del(m_pFrustum);

	mem_release_del(m_pDepthSurface);

	mem_release_del(m_pOldDepthStencilSurface);
	mem_release_del(m_pOldColorSurface);
}

void CShadowMap::setPosition(const float3 *pPos)
{
	m_vPosition = *pPos;
}

void CShadowMap::getPosition(float3 *pPos)
{
	*pPos = m_vPosition;
}

void CShadowMap::setDirection(const float3 *vDir)
{
	m_vDirection = *vDir;
}

void CShadowMap::getDirection(float3 *vDir)
{
	*vDir = m_vDirection;
}

void CShadowMap::setAngleNearFar(const float3 *vAngleNearFar)
{
	m_vAngleNearFar = *vAngleNearFar;
}

void CShadowMap::getAngleNearFar(float3 *vAngleNearFar)
{
	*vAngleNearFar = m_vAngleNearFar;
}

void CShadowMap::setBias(float fBias)
{
	m_fBias = fBias;
}

float CShadowMap::getBias()
{
	return m_fBias;
}

void CShadowMap::setBlurPixel(float fBlurPixel)
{
	m_fBlurPixel = fBlurPixel;
}

float CShadowMap::getBlurPixel()
{
	return m_fBlurPixel;
}

void CShadowMap::init()
{
	m_pFrustum = SGCore_CrFrustum();
	
	m_pDepthSurface = 0;
	m_idDepth = SGCore_RTcreateTexture(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.y, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, (String("shadow_dir_") + int(this)).c_str(), -1);
	
	float2 vOffset = float2(0.5f, 0.5f) + (float2(0.5f, 0.5f) / float2(light_data::vSizeTexDepthLocal));
	float fRange = 1.0f;
	float fBias = 0.0f;
	m_mScaleBiasMat = float4x4(	0.5f,		 0.0f,		0.0f,		0.0f,
								0.0f,		-0.5f,		0.0f,		0.0f,
								0.0f,		 0.0f,		fRange,		0.0f,
								vOffset.x,	vOffset.y,	fBias,		1.0f);
}

void CShadowMap::setFar(float sfar)
{
	m_vAngleNearFar.z = sfar;
}

float CShadowMap::getFar()
{
	return m_vAngleNearFar.z;
}

void CShadowMap::setNear(float snear)
{
	m_vAngleNearFar.y = snear;
}

float CShadowMap::getNear()
{
	return m_vAngleNearFar.y;
}

void CShadowMap::setAngle(float fAngle)
{
	m_vAngleNearFar.x = fAngle;
}

float CShadowMap::getAngle()
{
	return m_vAngleNearFar.x;
}

void CShadowMap::setIdVisCalcObj(ID id, ID idVisCalcObj)
{
	if (id < 0)
		return;

	m_aVisCalcObjs[id] = idVisCalcObj;
}

int CShadowMap::getCountVisCalcObj()
{
	return m_aVisCalcObjs.size();
}

ID CShadowMap::getIdVisCalcObj(ID id)
{
	if (id < 0 || !(id < (ID)m_aVisCalcObjs.size()))
		return -2;

	return m_aVisCalcObjs[id];
}

void CShadowMap::begin()
{
	light_data::pDXDevice->GetDepthStencilSurface(&m_pOldDepthStencilSurface);
	light_data::pDXDevice->SetDepthStencilSurface(SGCore_RTGetSurface(light_data::texture_id::idSurfaceDepthDirShadow));
	light_data::pDXDevice->GetRenderTarget(0, &m_pOldColorSurface);

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &m_mOldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &m_mOldProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &m_mOldViewProj);

	float3 vUp = float3(0, 1, 0);
	
	if (m_vDirection.x == 0 && m_vDirection.y == -1 && m_vDirection.z == 0)
		vUp = SMVector3Transform(m_vDirection, SMMatrixRotationZ(1.57f));

	m_mView = SMMatrixLookAtLH(m_vPosition, m_vPosition + m_vDirection, vUp);
	m_mProjection = SMMatrixPerspectiveFovLH(m_vAngleNearFar.x, light_data::vSizeTexDepthLocal.x / light_data::vSizeTexDepthLocal.y, m_vAngleNearFar.y, m_vAngleNearFar.z);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mProjection);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &(m_mView * m_mProjection));
	
	m_pFrustum->update(&(m_mView), &(m_mProjection));

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomDirect, "g_mWVP", &SMMatrixTranspose(m_mView * m_mProjection));
	SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomDirect);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomDirect);

	mem_release_del(m_pDepthSurface);
	SGCore_RTGetTexture(m_idDepth)->GetSurfaceLevel(0, &(m_pDepthSurface));
	
	light_data::pDXDevice->SetRenderTarget(0, m_pDepthSurface);
	
	light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);
}

void CShadowMap::end()
{
	light_data::pDXDevice->SetDepthStencilSurface(m_pOldDepthStencilSurface);
	light_data::pDXDevice->SetRenderTarget(0, m_pOldColorSurface);

	mem_release_del(m_pOldDepthStencilSurface);
	mem_release_del(m_pOldColorSurface);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mOldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mOldProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &m_mOldViewProj);
}

void CShadowMap::genShadow(IDirect3DTexture9 *pShadowMap)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;
	pShadowMap->GetSurfaceLevel(0, &RenderSurf);
	light_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	light_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	//light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);

	light_data::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_DEPTH));

	float4x4 MatrixTexture;

	light_data::pDXDevice->SetTexture(1, SGCore_RTGetTexture(m_idDepth));

	light_data::pDXDevice->SetTexture(2, SGCore_LoadTexGetTex(light_data::texture_id::idNoiseTex));
	MatrixTexture = m_mView * m_mProjection * m_mScaleBiasMat;
	MatrixTexture = SMMatrixTranspose(MatrixTexture);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect, "g_mMatrixTexture", &MatrixTexture);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth);

	SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vParamProj", &float3_t((float)*r_win_width, (float)*r_win_height, (float)*r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vNearFar", &float2_t(*r_near, *r_far));

	float3 observerpos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &observerpos);


	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect, "g_vPosCam", &observerpos);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect, "g_vPixelSize", &float2(m_fBlurPixel / light_data::vSizeTexDepthLocal.x, m_fBlurPixel / light_data::vSizeTexDepthLocal.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect, "g_vSizeMapBias", &float3(light_data::vSizeTexDepthLocal.x, light_data::vSizeTexDepthLocal.y, m_fBias));
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowDirect9, "NearFar", &float2_t(*r_near, *r_far));

	float4x4 mObserverView;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mObserverView);

	float4x4 mViewInv = SMMatrixInverse(0, mObserverView);
	mViewInv = SMMatrixTranspose(mViewInv);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_mViewInv", &mViewInv);

	SGCore_ScreenQuadDraw();

	light_data::pDXDevice->SetVertexShader(0);
	light_data::pDXDevice->SetPixelShader(0);

	light_data::pDXDevice->SetRenderTarget(0, BackBuf);

	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

//##########################################################################

void CShadowMapCube::onLostDevice()
{
	mem_release_del(m_pDepthSurface);
	mem_release_del(m_pOldDepthStencilSurface);
	mem_release_del(m_pOldColorSurface);
}

void CShadowMapCube::onResetDevice()
{
	SGCore_RTreCreateTextureCube(m_idDepthCubeMap, light_data::vSizeTexDepthLocal.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, 0, -1);
}

CShadowMapCube::~CShadowMapCube()
{
	for (int i = 0; i < 6; ++i)
	{
		mem_release_del(m_aFrustums[i]);
		mem_release(m_pDepthSurface);
	}

	for (int i = 0; i < m_aVisCalcObjs.size(); ++i)
	{
		mem_delete_a(m_aVisCalcObjs[i]);
	}

	mem_release_del(m_pOldDepthStencilSurface);
	mem_release_del(m_pOldColorSurface);
}

CShadowMapCube::CShadowMapCube()
{
	for (int i = 0; i < 6; ++i)
	{
		m_aFrustums[i] = 0;
		m_pDepthSurface = 0;
		//m_aVisCalcObjs[i] = -1;
	}

	EnableEdge[0] = EnableEdge[1] = EnableEdge[2] = EnableEdge[3] = EnableEdge[4] = EnableEdge[5] = true;
	EnableEdgeNulled[0] = EnableEdgeNulled[1] = EnableEdgeNulled[2] = EnableEdgeNulled[3] = EnableEdgeNulled[4] = EnableEdgeNulled[5] = false;

	m_fBias = 0.002f;
	m_fBlurPixel = 4.0f;

	m_pOldColorSurface = 0;
	m_pOldDepthStencilSurface = 0;
}

void CShadowMapCube::setEnableCubeEdge(int iEdge, bool isEnable)
{
	if (iEdge >= 0 && iEdge <= 5)
	{
		if (EnableEdge[iEdge] && !isEnable)
			EnableEdgeNulled[iEdge] = false;

		EnableEdge[iEdge] = isEnable;
	}
}

bool CShadowMapCube::getEnableCubeEdge(int iEdge)
{
	if (iEdge >= 0 && iEdge <= 5)
		return EnableEdge[iEdge];

	return(false);
}

void CShadowMapCube::setBias(float fBias)
{ 
	m_fBias = fBias;
}

float CShadowMapCube::getBias()
{ 
	return m_fBias;
}

void CShadowMapCube::setBlurPixel(float fBlurPixel)
{ 
	m_fBlurPixel = fBlurPixel;
}

bool CShadowMapCube::getBlurPixel()
{ 
	return m_fBlurPixel; 
}

void CShadowMapCube::setPosition(const float3 *pPos)
{
	m_vPosition = *pPos;
}

void CShadowMapCube::getPosition(float3 *pPos)
{
	*pPos = m_vPosition;
}

void CShadowMapCube::setNearFar(const float2 *pNearFar)
{
	m_vNearFar = *pNearFar;
}

void CShadowMapCube::getNearFar(float2 *pNearFar)
{
	*pNearFar = m_vNearFar;
}

void CShadowMapCube::setNear(float fNear)
{
	m_vNearFar.x = fNear;
}

float CShadowMapCube::getNear()
{
	return m_vNearFar.x;
}


void CShadowMapCube::setFar(float fFar)
{
	m_vNearFar.y = fFar;
}

float CShadowMapCube::getFar()
{
	return m_vNearFar.y;
}


void CShadowMapCube::init()
{
	for (int i = 0; i < 6; ++i)
	{
		m_aFrustums[i] = SGCore_CrFrustum();
	}

	m_pDepthSurface = 0;

	m_idDepthCubeMap = SGCore_RTcreateTextureCube(light_data::vSizeTexDepthLocal.x, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, (String("shadow_cube_map_") + int(this)).c_str(), -1);
}

void CShadowMapCube::begin()
{
	light_data::pDXDevice->GetDepthStencilSurface(&m_pOldDepthStencilSurface);
	light_data::pDXDevice->SetDepthStencilSurface(SGCore_RTGetSurface(light_data::texture_id::idSurfaceDepthPointShadow));
	light_data::pDXDevice->GetRenderTarget(0, &m_pOldColorSurface);

	Core_RMatrixGet(G_RI_MATRIX_VIEW, &m_mOldView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &m_mOldProj);
	Core_RMatrixGet(G_RI_MATRIX_VIEWPROJ, &m_mOldViewProj);
}

void CShadowMapCube::pre(int iCube)
{
	if (!EnableEdge[iCube])
	{
		if (!EnableEdgeNulled[iCube])
		{
			EnableEdgeNulled[iCube] = true;
			mem_release(m_pDepthSurface);

			SGCore_RTGetTextureCube(m_idDepthCubeMap)->GetCubeMapSurface((D3DCUBEMAP_FACES)iCube, 0, &m_pDepthSurface);
			light_data::pDXDevice->SetRenderTarget(0, m_pDepthSurface);

			light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255), 1.0f, 0);
		}
		return;
	}

	m_aViews[iCube] = SMMatrixLookAtLH(m_vPosition, light_data::vOrientedCube[iCube] + m_vPosition, light_data::vUpVectorsCube[iCube]);

	m_aProjections[iCube] = SMMatrixPerspectiveFovLH(SM_PI / 2.f, 1, m_vNearFar.x, m_vNearFar.y);

	m_aFrustums[iCube]->update(&(m_aViews[iCube]), &(m_aProjections[iCube]));


	float4x4 mVP = m_aViews[iCube] * m_aProjections[iCube];

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_aViews[iCube]);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_aProjections[iCube]);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &mVP);

	//mVP = SMMatrixTranspose(mVP);

	mem_release_del(m_pDepthSurface);
	
	SGCore_RTGetTextureCube(m_idDepthCubeMap)->GetCubeMapSurface((D3DCUBEMAP_FACES)iCube, 0, &m_pDepthSurface);
	light_data::pDXDevice->SetRenderTarget(0, m_pDepthSurface);
	
	light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0);

	/*SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomCube);

	//SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube, "g_vLightPos", &float4_t(m_vPosition.x, m_vPosition.y, m_vPosition.z, m_vNearFar.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idSMDepthGeomCube, "g_vLightPos", &float4_t(m_vPosition.x, m_vPosition.y, m_vPosition.z, m_vNearFar.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idSMDepthGeomCube, "g_mWVP", &mVP);*/
}

void CShadowMapCube::post(int iCube)
{
	if(GetAsyncKeyState(VK_NUMPAD5))
	{
		char tmpstr[1024];
		sprintf(tmpstr, "C:\\1\\shadowcube_%d.bmp", iCube);
		D3DXSaveSurfaceToFile(tmpstr, D3DXIFF_PNG, m_pDepthSurface, NULL, 0);
	}
	mem_release_del(m_pDepthSurface);
}

void CShadowMapCube::end()
{
	light_data::pDXDevice->SetDepthStencilSurface(m_pOldDepthStencilSurface);
	light_data::pDXDevice->SetRenderTarget(0, m_pOldColorSurface);
	mem_release_del(m_pOldDepthStencilSurface);
	mem_release_del(m_pOldColorSurface);

	Core_RMatrixSet(G_RI_MATRIX_VIEW, &m_mOldView);
	Core_RMatrixSet(G_RI_MATRIX_PROJECTION, &m_mOldProj);
	Core_RMatrixSet(G_RI_MATRIX_VIEWPROJ, &m_mOldViewProj);
}

void CShadowMapCube::genShadow(IDirect3DTexture9 *pShadowMap)
{
	static const int *r_win_width = GET_PCVAR_INT("r_win_width");
	static const int *r_win_height = GET_PCVAR_INT("r_win_height");

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	LPDIRECT3DSURFACE9 RenderSurf, BackBuf;

	pShadowMap->GetSurfaceLevel(0, &RenderSurf);
	light_data::pDXDevice->GetRenderTarget(0, &BackBuf);
	light_data::pDXDevice->SetRenderTarget(0, RenderSurf);

	//light_data::pDXDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 0, 0), 1.0f, 0);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(1, D3DTEXF_LINEAR);
	SGCore_SetSamplerAddress(1, D3DTADDRESS_CLAMP);
	SGCore_SetSamplerFilter(2, D3DTEXF_POINT);
	SGCore_SetSamplerAddress(2, D3DTADDRESS_WRAP);

	light_data::pDXDevice->SetTexture(0, SGCore_GbufferGetRT(DS_RT_DEPTH));

	light_data::pDXDevice->SetTexture(1, SGCore_RTGetTextureCube(m_idDepthCubeMap));
	light_data::pDXDevice->SetTexture(2, SGCore_LoadTexGetTex(light_data::texture_id::idNoiseTex));

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vParamProj", &float3_t((float)(float)*r_win_width, *r_win_height, (float)*r_default_fov));
	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_vNearFar", &float2_t(*r_near, *r_far));

	float pixel_size = m_fBlurPixel / light_data::vSizeTexDepthLocal.x;
	float3 observerpos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &observerpos);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube, "g_vPosCam", &observerpos);
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube, "g_vLightPos", &float4_t(m_vPosition.x, m_vPosition.y, m_vPosition.z, m_vNearFar.y));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube, "g_vSizeMapBias", &float2(light_data::vSizeTexDepthLocal.x, m_fBias));
	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube, "g_fPixelSize", &pixel_size);
	//SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, light_data::shader_id::ps::idGenShadowCube6, "g_vLightPos", &m_vPosition);

	float4x4 camview;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &camview);

	float determ = 0;
	float4x4 ViewInv = SMMatrixInverse(&determ, camview);
	ViewInv = SMMatrixTranspose(ViewInv);

	SGCore_ShaderSetVRF(SHADER_TYPE_VERTEX, light_data::shader_id::vs::idResPosDepth, "g_mViewInv", &ViewInv);

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	light_data::pDXDevice->SetRenderTarget(0, BackBuf);
	mem_release_del(RenderSurf);
	mem_release_del(BackBuf);
}

void CShadowMapCube::setIdVisCalcObj(ID id, int iSplit, ID idVisCalcObj)
{
	if (id < 0 || !(iSplit >= 0 && iSplit < 6))
		return;

	if (id >= (long)m_aVisCalcObjs.size())
	{
		ID* tmparr = new ID[6];
		for (int i = 0; i < 6; ++i)
			tmparr[i] = -1;
		m_aVisCalcObjs[id] = tmparr;
	}

	m_aVisCalcObjs[id][iSplit] = idVisCalcObj;
}

ID CShadowMapCube::getCountVisCalcObj()
{
	return m_aVisCalcObjs.size();
}

ID CShadowMapCube::getIdVisCalcObj(ID id, int iSplit)
{
	if (id < 0 || !(iSplit >= 0 && iSplit < 6) || !(id < (long)m_aVisCalcObjs.size()))
		return -2;

	return m_aVisCalcObjs[id][iSplit];
}