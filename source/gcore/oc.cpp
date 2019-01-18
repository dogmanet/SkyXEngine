
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "oc.h"

void SwapFloat3(float3 &vA, float3 &vB)
{
	float3 vV;
	vV = vA;
	vA = vB;
	vB = vV;
}

void TriGetSphere(const float3 &vA, const float3 &vB, const float3 &vC, float &fRadius, float2 &vCenter)
{
	float x12 = vA.x - vB.x;
	float x23 = vB.x - vC.x;
	float x31 = vC.x - vA.x;

	float y12 = vA.y - vB.y;
	float y23 = vB.y - vC.y;
	float y31 = vC.y - vA.y;

	float z1 = (vA.x * vA.x) + (vA.y * vA.y);
	float z2 = (vB.x * vB.x) + (vB.y * vB.y);
	float z3 = (vC.x * vC.x) + (vC.y * vC.y);

	float zx = y12 * z3 + y23 * z1 + y31 * z2;
	float zy = x12 * z3 + x23 * z1 + x31 * z2;
	float z = x12 * y31 - y12 * x31;

	vCenter.x = -(zx / (2 * z));
	vCenter.y = zy / (2 * z);

	fRadius = sqrt(pow(vA.x - vCenter.x, 2) + (vA.y - vCenter.y, 2));
}

void QuadGetSphere(const float2 &vMin, const float2 &vMax, float &fRadius, float2 &vCenter)
{
	vCenter = (vMin + vMax) / 2;

	fRadius = SMVector2Length(vCenter - vMin);
}

//##########################################################################

COcclusionCulling::COcclusionCulling()
{
	m_isEnable = true;

	m_pFrustum = 0;
	m_iCountFC = 0;
	m_iCountFCfail = 0;

	m_iWidth = m_iHeight = m_iCountPixels = 0;

	//m_pSurfDepthBuffer[0] = m_pSurfDepthBuffer[1] = m_pSurfDepthBuffer[2];
	m_pSurfDepthBufferID[0] = m_pSurfDepthBufferID[1] = m_pSurfDepthBufferID[2];

	m_pArrDepthBuffer = 0;
	m_pArrDepthBuffer = 0;
	m_pArrWorldPos = 0;
	m_pArrDepthBufferReProjection = 0;
	m_pArrDepthBufferRasterize = 0;
	m_pArrDepthBufferMutex = 0;

	m_aRTdepth[0] = -1;
	m_aRTdepth[1] = -1;

	m_idVS_ScreenOut = -1;
	m_idPS_ScreenOut = -1;
	m_idPS_FindMax9 = -1;

	m_fBiasDepth = 0.0001f;

	m_iCurrRTdepth = 0;
}

COcclusionCulling::~COcclusionCulling()
{
	/*mem_release(m_pSurfDepthBuffer[0]);
	mem_release(m_pSurfDepthBuffer[1]);
	mem_release(m_pSurfDepthBuffer[2]);*/
	mem_delete_a(m_pArrDepthBuffer);
	mem_delete_a(m_pArrDepthBufferMutex);
	mem_delete_a(m_pArrDepthBufferReProjection);
	mem_delete_a(m_pArrWorldPos);
	mem_delete_a(m_pArrDepthBufferRasterize);
}

void COcclusionCulling::init(int iWidth, int iHeight)
{
	m_iWidth = float(iWidth * OC_SIZE_COEF);
	m_iHeight = float(iHeight * OC_SIZE_COEF);
	m_iCountPixels = m_iWidth * m_iHeight;

	/*g_pDXDevice->CreateOffscreenPlainSurface(m_iWidth, m_iHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &m_pSurfDepthBuffer[0], 0);
	g_pDXDevice->CreateOffscreenPlainSurface(m_iWidth, m_iHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &m_pSurfDepthBuffer[1], 0);
	g_pDXDevice->CreateOffscreenPlainSurface(m_iWidth, m_iHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &m_pSurfDepthBuffer[2], 0);*/

	m_pSurfDepthBufferID[0] = SGCore_RTcreateSurfaceOffScreenPlain(m_iWidth, m_iHeight, D3DFMT_R32F, "oc_0", -1);
	m_pSurfDepthBufferID[1] = SGCore_RTcreateSurfaceOffScreenPlain(m_iWidth, m_iHeight, D3DFMT_R32F, "oc_0", -1);
	m_pSurfDepthBufferID[2] = SGCore_RTcreateSurfaceOffScreenPlain(m_iWidth, m_iHeight, D3DFMT_R32F, "oc_0", -1);

	m_pArrDepthBuffer = new float[m_iCountPixels + 1];
	m_pArrWorldPos = new float4[m_iCountPixels + 1];
	m_pArrDepthBufferReProjection = new float[m_iCountPixels + 1];
	m_pArrDepthBufferRasterize = new float[m_iCountPixels + 1];
	m_pArrDepthBufferMutex = new std::mutex[OC_MAX_MUTEX_COUNT];

	m_aRTdepth[0] = SGCore_RTcreateTexture(m_iWidth, m_iHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "depth_oc", 0.25f);
	m_aRTdepth[1] = SGCore_RTcreateTexture(m_iWidth, m_iHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, "depth_oc2", 0.25f);

	m_idVS_ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_VERTEX, "pp_quad_render.vs", "pp_quad_render.vs", SHADER_CHECKDOUBLE_PATH);
	m_idPS_ScreenOut = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_quad_render.ps", "pp_quad_render.ps", SHADER_CHECKDOUBLE_PATH);
	m_idPS_FindMax9 = SGCore_ShaderLoad(SHADER_TYPE_PIXEL, "pp_depth_find_max9.ps", "pp_depth_find_max9.ps", SHADER_CHECKDOUBLE_NAME);
}

void COcclusionCulling::onLostDevice()
{
	/*mem_release(m_pSurfDepthBuffer[0]);
	mem_release(m_pSurfDepthBuffer[1]);
	mem_release(m_pSurfDepthBuffer[2]);*/

	mem_delete_a(m_pArrDepthBuffer);
	mem_delete_a(m_pArrDepthBufferReProjection);
	mem_delete_a(m_pArrWorldPos);
	mem_delete_a(m_pArrDepthBufferRasterize);
}

void COcclusionCulling::onResetDevice(int iWidth, int iHeight)
{
	m_iWidth = float(iWidth * OC_SIZE_COEF);
	m_iHeight = float(iHeight * OC_SIZE_COEF);
	m_iCountPixels = m_iWidth * m_iHeight;

	/*g_pDXDevice->CreateOffscreenPlainSurface(m_iWidth, m_iHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &m_pSurfDepthBuffer[0], 0);
	g_pDXDevice->CreateOffscreenPlainSurface(m_iWidth, m_iHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &m_pSurfDepthBuffer[1], 0);
	g_pDXDevice->CreateOffscreenPlainSurface(m_iWidth, m_iHeight, D3DFMT_R32F, D3DPOOL_SYSTEMMEM, &m_pSurfDepthBuffer[2], 0);*/

	SGCore_RTreCreateSurfaceOffScreenPlain(m_pSurfDepthBufferID[0], m_iWidth, m_iHeight, D3DFMT_R32F, "oc_0", -1);
	SGCore_RTreCreateSurfaceOffScreenPlain(m_pSurfDepthBufferID[1], m_iWidth, m_iHeight, D3DFMT_R32F, "oc_0", -1);
	SGCore_RTreCreateSurfaceOffScreenPlain(m_pSurfDepthBufferID[2], m_iWidth, m_iHeight, D3DFMT_R32F, "oc_0", -1);

	m_pArrDepthBuffer = new float[m_iCountPixels + 1];
	m_pArrWorldPos = new float4[m_iCountPixels + 1];
	m_pArrDepthBufferReProjection = new float[m_iCountPixels + 1];
	m_pArrDepthBufferRasterize = new float[m_iCountPixels + 1];
}


void COcclusionCulling::setEnable(bool isEnable)
{
	m_isEnable = isEnable;
}

bool COcclusionCulling::getEnable()
{
	return m_isEnable;
}

void COcclusionCulling::update(ID idDepthMap, const IFrustum *pFrustum)
{
	if (!m_isEnable)
		return;

	m_pFrustum = pFrustum;
	m_iCountFC = 0;
	m_iCountFCfail = 0;

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	// если нет кваров плоскостей отсечения значит что-то не так
	if (!r_near || !r_far)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - cvar r_near or r_far is not found!", GEN_MSG_LOCATION);
		return;
	}

	static const float *r_default_fov = GET_PCVAR_FLOAT("r_default_fov");

	//если нет квара fov значит что-то не так
	if (!r_default_fov)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - cvar r_default_fov is not found!", GEN_MSG_LOCATION);
		return;
	}

	m_fBiasDepth = 1.f / (*r_far);

	IDirect3DTexture9 *pTexDepth = SGCore_RTGetTexture(idDepthMap);
	IDirect3DTexture9 *pTexDepthOC = SGCore_RTGetTexture(m_aRTdepth[m_iCurrRTdepth]);

	//рисуем глубину с максимальной выборкой в уменьшенный буфер
	g_pDXDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	g_pDXDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);
	g_pDXDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pDXDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	LPDIRECT3DSURFACE9 RenderSurf2, BackBuf2;

	pTexDepthOC->GetSurfaceLevel(0, &RenderSurf2);
	g_pDXDevice->GetRenderTarget(0, &BackBuf2);
	g_pDXDevice->SetRenderTarget(0, RenderSurf2);

	SGCore_SetSamplerFilter(0, D3DTEXF_NONE);
	SGCore_SetSamplerAddress(0, D3DTADDRESS_CLAMP);

	g_pDXDevice->SetTexture(0, pTexDepth);

	SGCore_ShaderBind(SHADER_TYPE_VERTEX, m_idVS_ScreenOut);
	SGCore_ShaderBind(SHADER_TYPE_PIXEL, m_idPS_FindMax9);

	SGCore_ShaderSetVRF(SHADER_TYPE_PIXEL, m_idPS_FindMax9, "g_vPixelSize", (&float2(1.f / float(g_oD3DAPP.BackBufferWidth), 1.f / float(g_oD3DAPP.BackBufferHeight))));

	SGCore_ScreenQuadDraw();

	SGCore_ShaderUnBind();

	g_pDXDevice->SetRenderTarget(0, BackBuf2);

	mem_release(RenderSurf2);
	mem_release(BackBuf2);

	//**********************************************************************

	//копируем отрисованную глубину в массив
	LPDIRECT3DSURFACE9 pDepthSurf;

	int iCurrOld = 1 - m_iCurrRTdepth;

	pTexDepthOC = SGCore_RTGetTexture(m_aRTdepth[m_iCurrRTdepth]);

	pTexDepthOC->GetSurfaceLevel(0, &pDepthSurf);
	g_pDXDevice->GetRenderTargetData(pDepthSurf, SGCore_RTGetSurface(m_pSurfDepthBufferID[m_iCurrRTdepth]));

	D3DLOCKED_RECT  srect;
	SGCore_RTGetSurface(m_pSurfDepthBufferID[iCurrOld])->LockRect(&srect, 0, D3DLOCK_READONLY | D3DLOCK_DONOTWAIT);

	memcpy(m_pArrDepthBuffer, srect.pBits, sizeof(float)* m_iCountPixels);
	SGCore_RTGetSurface(m_pSurfDepthBufferID[iCurrOld])->UnlockRect();

	mem_release(pDepthSurf);

	//**********************************************************************

	float fPosX = 0, fPosY = 0;

	float4 vWorldPos;

	float4x4 mView, mProjection;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mProjection);

	float4x4 mViewProj = m_mOldView * m_mOldProj;// = mView * mProjection;
	float fD = 0.f;
	float4x4 mInvVP = SMMatrixInverse(&fD, mViewProj);

	float3 vObserverPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

	float fTanHalfFOV = tan((*r_default_fov) * 0.5f);
	float fAspectRatio = float(m_iWidth) / float(m_iHeight);
	float fFarY = fTanHalfFOV * (*r_far);
	float fFarX = fFarY * fAspectRatio;

	float4 vEyeRay, vWorldRay;
	float4x4 mInvView = SMMatrixInverse(&fD, m_mOldView);

	float4 aWorldRays[4];
	aWorldRays[0] = SMVector4Transform(float4(-1.f * fFarX, -1.f * fFarY, (*r_far), 0.f), mInvView);
	aWorldRays[1] = SMVector4Transform(float4(1.f * fFarX, -1.f * fFarY, (*r_far), 0.f), mInvView);
	aWorldRays[2] = SMVector4Transform(float4(1.f * fFarX, 1.f * fFarY, (*r_far), 0.f), mInvView);
	aWorldRays[3] = SMVector4Transform(float4(-1.f * fFarX, 1.f * fFarY, (*r_far), 0.f), mInvView);

	float4 vWorldRay0, vWorldRay1;

	ensureReprojectionDone();

	mem_delete(m_pUpdateCycle);
	m_pUpdateCycle = new COCUpdate(aWorldRays, m_iWidth, m_iHeight, &vObserverPos, m_pArrWorldPos, m_pArrDepthBuffer); 
	m_idUpdateCycle = Core_MForLoop(0, m_iWidth, m_pUpdateCycle/*, m_iCountPixels / (Core_MGetThreadCount() * 3)*/);
	/*for (int x = 0; x < m_iWidth; ++x)
	{
		vWorldRay0 = SMVectorLerp(aWorldRays[0], aWorldRays[1], float(x) / m_iWidth);
		vWorldRay1 = SMVectorLerp(aWorldRays[3], aWorldRays[2], float(x) / m_iWidth);
		for (int y = 0; y < m_iHeight; ++y)
		{
			int iPosPixel = (y * m_iWidth) + x;
			vWorldRay = SMVectorLerp(vWorldRay1, vWorldRay0, float(y) / m_iHeight);
			vWorldPos = vObserverPos + vWorldRay * m_pArrDepthBuffer[iPosPixel];
			vWorldPos.w = 1.f;
			m_pArrWorldPos[iPosPixel] = vWorldPos;
		}
	}*/

	m_mOldView = mView;
	m_mOldProj = mProjection;

	++m_iCurrRTdepth;

	if (m_iCurrRTdepth >= 2)
		m_iCurrRTdepth = 0;


	/*if (GetKeyState('M'))
	{
		m_pSurfDepthBuffer[2]->LockRect(&srect, 0, D3DLOCK_READONLY);

		memcpy(srect.pBits, m_pArrDepthBuffer, sizeof(float)* m_iCountPixels);
		m_pSurfDepthBuffer[2]->UnlockRect();

		D3DXSaveSurfaceToFile("C:/1/m_pSurfDepthBuffer.jpg", D3DXIFF_JPG, m_pSurfDepthBuffer[2], NULL, NULL);
	}*/
}

void COcclusionCulling::reprojection()
{
	if (!m_isEnable)
		return;

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	if (!r_near || !r_far)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - cvar r_near or r_far is not found!", GEN_MSG_LOCATION);
		return;
	}

	float4x4 mView, mProjection;
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mProjection);

	float4x4 mViewProj = mView * mProjection;

	float4 vNewPos;

	for (int i = 0; i < m_iCountPixels; ++i)
	{
		m_pArrDepthBufferReProjection[i] = 1.f;
		m_pArrDepthBufferRasterize[i] = 1.f;
	}

	//float3 vObserverPos;
	//Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

	mem_delete(m_pReprojectionCycle);
	m_pReprojectionCycle = new COCReprojection(m_pArrWorldPos, &mViewProj, *r_near, *r_far, m_iWidth, m_iHeight, m_iCountPixels, m_pArrDepthBufferReProjection, m_pArrDepthBufferRasterize, m_pArrDepthBufferMutex);
	m_idReprojectionCycle = Core_MForLoop(0, m_iCountPixels, m_pReprojectionCycle, m_iCountPixels / (Core_MGetThreadCount() * 3));
	
	/*float2 vNewPos2;

	//цикл репроекции каждого пикселя, расчет новой позиции в screen-space и новой глубины
	for (int i = 0; i < m_iCountPixels; ++i)
	{
		vNewPos = SMVector4Transform(m_pArrWorldPos[i], mViewProj);

		vNewPos.x /= abs(vNewPos.w);
		vNewPos.y /= abs(vNewPos.w);
		vNewPos.z = (vNewPos.z + (*r_near)) / (*r_far);

		vNewPos.x = vNewPos.x * 0.5f + 0.5f;
		vNewPos.y = (vNewPos.y * (-0.5f) + 0.5f);

		//костыль решения проблем округления, без этого будут белые линии

		vNewPos2.x = float(int(vNewPos.x * 10000.f) / 10000.f);
		vNewPos2.y = float(int(vNewPos.y * 10000.f) / 10000.f);

		if (vNewPos2.x == 0.f || vNewPos2.x == 1.f)
			vNewPos.x = vNewPos2.x;

		if (vNewPos2.y == 0.f || vNewPos2.y == 1.f)
			vNewPos.y = vNewPos2.y;

		//******************************************************************

		if ((vNewPos.x <= 1.f && vNewPos.x >= 0.f) && (vNewPos.y <= 1.f && vNewPos.y >= 0.f))
		{
			int x = floor(vNewPos.x * float(m_iWidth) + 0.5f);
			int y = floor(vNewPos.y * m_iHeight + 0.5f);
			int iPosPixel = int(y * m_iWidth) + x;

			if (iPosPixel > m_iCountPixels)
				int qwerty = 0;
			else
			{
				//если в буфере репроекции нет записей для текущего пикселя, либо записанная глубина меньше чем новая
				if (m_pArrDepthBufferReProjection[iPosPixel] >= 1.f || vNewPos.z > m_pArrDepthBufferReProjection[iPosPixel])
					m_pArrDepthBufferReProjection[iPosPixel] = vNewPos.z;
			}
		}
	}*/
	//ensureReprojectionDone();

	/*if (GetKeyState('N'))
	{
		D3DLOCKED_RECT  srect;
		g_pOCsurfDepthBuffer[2]->LockRect(&srect, 0, D3DLOCK_READONLY);

		memcpy(srect.pBits, g_pOCarrDepthBufferReProjection, sizeof(float)* g_iOCcountPixels);
		g_pOCsurfDepthBuffer[2]->UnlockRect();
		D3DXSaveSurfaceToFile("C:/1/g_pSurfDepthBufferOCreproj.jpg", D3DXIFF_JPG, g_pOCsurfDepthBuffer[2], NULL, NULL);
	}*/
}

bool COcclusionCulling::comVisible(const float3 *pMax, const float3 *pMin)
{
	if (!pMax || !pMin)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - min or max is null!", GEN_MSG_LOCATION);
		return false;
	}

	if (!m_isEnable)
		return true;
	
	float3 vObserverPos;
	Core_RFloat3Get(G_RI_FLOAT3_OBSERVER_POSITION, &vObserverPos);

	static const float *r_near = GET_PCVAR_FLOAT("r_near");
	static const float *r_far = GET_PCVAR_FLOAT("r_far");

	if (!r_near || !r_far)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - cvar r_near or r_far is not found!", GEN_MSG_LOCATION);
		return false;
	}

	float3 vMax, vMin;
	vMax = *pMax;
	vMin = *pMin;

	//вычисление центра и радиуса по xz чтобы определить дистанцию до наблюдателя
	float3 vCenter = (vMin + vMax) * 0.5f;
	float fRadius = SMVector3Length2(float3(vCenter.x, 0.f, vCenter.z) - float3(vMax.x, 0, vMax.z));

	float fDist = SMVector3Length2(vObserverPos - vCenter);

	//если дистанция до наблюдаеля меньше либо равна радиусу бокса по xz значит наблюдатель в боксе
	if (fDist <= fRadius)
		return true;

	fDist -= fRadius;

	//если бокс в пределах ближнего расстояния неотсечения от наблюдателя, значит он виден
	if (fDist >= 0.f && fDist <= OC_DIST_NEAR_NOT_CULL*OC_DIST_NEAR_NOT_CULL)
		return true;

	//закрывало много багов ложного отсечения, однако на дальних дистанциях не отсекает как надо, возможно просто буфер глубины слишком уменьшенный
	vMax += g_cvOCext;
	vMin -= g_cvOCext;

	float4x4 mWorld, mView, mProjection;
	Core_RMatrixGet(G_RI_MATRIX_WORLD, &mWorld);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_OBSERVER_PROJ, &mProjection);

	float4x4 mWVP = mWorld * mView *mProjection;


	float4 aSSPoints[8];
	float4 aWPoints[8];
	aWPoints[0] = float4(vMax.x, vMax.y, vMax.z, 1.0f);
	aWPoints[1] = float4(vMax.x, vMax.y, vMin.z, 1.0f);
	aWPoints[2] = float4(vMax.x, vMin.y, vMax.z, 1.0f);
	aWPoints[3] = float4(vMin.x, vMax.y, vMax.z, 1.0f);
	aWPoints[4] = float4(vMax.x, vMin.y, vMin.z, 1.0f);
	aWPoints[5] = float4(vMin.x, vMin.y, vMax.z, 1.0f);
	aWPoints[6] = float4(vMin.x, vMax.y, vMin.z, 1.0f);
	aWPoints[7] = float4(vMin.x, vMin.y, vMin.z, 1.0f);

	for (int i = 0; i < 8; ++i)
	{
		aSSPoints[i] = SMVector4Transform(aWPoints[i], mWVP);

		aSSPoints[i].x /= abs(aSSPoints[i].w);
		aSSPoints[i].y /= abs(aSSPoints[i].w);
		aSSPoints[i].z = ((aSSPoints[i].z + (*r_near)) / (*r_far));// *sign(aSSPoints[i].w);

		//просчет линейной глубины из нелинейной
		//float fLinearDepth = (*r_near) / ((*r_far) + (*r_near) - aPoints[i].w * ((*r_far) - (*r_near)));

		aSSPoints[i].x = aSSPoints[i].x * 0.5 + 0.5;
		aSSPoints[i].y = aSSPoints[i].y * (-0.5) + 0.5;

		aSSPoints[i].x *= m_iWidth;
		aSSPoints[i].y *= m_iHeight;

		aSSPoints[i].x = int(aSSPoints[i].x);
		aSSPoints[i].y = int(aSSPoints[i].y);
	}

	float2 vNearFar((*r_near), (*r_far));

	ensureReprojectionDone();

	bool isVisible = (
		(triFrustumCulling(aWPoints[7], aWPoints[6], aWPoints[1]) && triRasterize(aSSPoints[7], aSSPoints[6], aSSPoints[1], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[7], aWPoints[1], aWPoints[4]) && triRasterize(aSSPoints[7], aSSPoints[1], aSSPoints[4], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[0], aWPoints[5], aWPoints[2]) && triRasterize(aSSPoints[0], aSSPoints[5], aSSPoints[2], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[0], aWPoints[3], aWPoints[5]) && triRasterize(aSSPoints[0], aSSPoints[3], aSSPoints[5], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[0], aWPoints[1], aWPoints[3]) && triRasterize(aSSPoints[0], aSSPoints[1], aSSPoints[3], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[1], aWPoints[6], aWPoints[3]) && triRasterize(aSSPoints[1], aSSPoints[6], aSSPoints[3], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[5], aWPoints[4], aWPoints[2]) && triRasterize(aSSPoints[5], aSSPoints[4], aSSPoints[2], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[4], aWPoints[5], aWPoints[7]) && triRasterize(aSSPoints[4], aSSPoints[5], aSSPoints[7], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[0], aWPoints[2], aWPoints[1]) && triRasterize(aSSPoints[0], aSSPoints[2], aSSPoints[1], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[1], aWPoints[2], aWPoints[4]) && triRasterize(aSSPoints[1], aSSPoints[2], aSSPoints[4], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[6], aWPoints[5], aWPoints[3]) && triRasterize(aSSPoints[6], aSSPoints[5], aSSPoints[3], false, aSSPoints[0], vNearFar)) ||
		(triFrustumCulling(aWPoints[5], aWPoints[6], aWPoints[7]) && triRasterize(aSSPoints[5], aSSPoints[6], aSSPoints[7], false, aSSPoints[0], vNearFar))
		);

	return isVisible;
}

void COcclusionCulling::ensureReprojectionDone()
{
	if(ID_VALID(m_idReprojectionCycle))
	{
		Core_MWaitFor(m_idReprojectionCycle);
		mem_delete(m_pReprojectionCycle);
		m_idReprojectionCycle = -1;
	}
}

void COcclusionCulling::ensureUpdateDone()
{
	if(ID_VALID(m_idUpdateCycle))
	{
		Core_MWaitFor(m_idUpdateCycle);
		mem_delete(m_pUpdateCycle);
		m_idUpdateCycle = -1;
	}
}

bool COcclusionCulling::triFrustumCulling(const float3 &vA, const float3 &vB, const float3 &vC)
{
	/*if (GetAsyncKeyState('P'))
		return true;*/

	++m_iCountFC;

	bool isVisible = (!m_pFrustum || m_pFrustum->polyInFrustum(&vA, &vB, &vC));

	if (!isVisible)
		++m_iCountFCfail;

	return isVisible;
}

bool COcclusionCulling::triRasterize(const float4 &vA, const float4 &vB, const float4 &vC, bool isRasterize, const float3 &vNormal2, const float2_t &vNearFar)
{
	if (vA.z < 0.f && vB.z < 0.f && vC.z < 0.f)
		return false;

	float3 vNormal = SMVector3Normalize(SMVector3Cross(vC - vA, vB - vA));

	float3 vPointA = vA;
	float3 vPointB = vB;
	float3 vPointC = vC;

	//для нахождения D достаточно использовать произвольюную точку треугольника
	float fD = -(vNormal.x * vPointA.x + vNormal.y * vPointA.y + vNormal.z * vPointA.z);

	/*
	получить интерполированное значение глубины можно так: -(N.x * x + N.y * y + D) / N.z
	N - нормаль
	x и y - текущие координаты на треугольнике
	*/

	//сортировка точек по убыванию координаты Y
	if (vPointB.y < vPointA.y)
		SwapFloat3(vPointA, vPointB);

	if (vPointC.y < vPointA.y)
		SwapFloat3(vPointA, vPointC);

	if (vPointB.y > vPointC.y)
		SwapFloat3(vPointB, vPointC);


	bool isVisible = false;

	//расширение треугольника, на случай неправильно округления, закрыло много багов
	vPointA.y -= g_fOCextTriangle;
	vPointC.y += g_fOCextTriangle;

	int iTotalHeight = vPointC.y - vPointA.y;

	if (iTotalHeight == 0)
		return false;

	int iSegmentHeight = vPointB.y - vPointA.y + 1;

	if (iSegmentHeight == 0)
		return false;

	int iStartY = clampf(vPointA.y, 0, m_iHeight - 1);
	int iFinishY = clampf(vPointB.y, 0, m_iHeight - 1);

	for (int y = iStartY; y <= iFinishY; ++y)
	{
		float fA = (y - vPointA.y) / iTotalHeight;
		float fB = (y - vPointA.y) / iSegmentHeight;
		fA = vPointA.x + (vPointC.x - vPointA.x) * fA;
		fB = vPointA.x + (vPointB.x - vPointA.x) * fB;

		if (fA > fB)
			std::swap(fA, fB);

		fA = clampf(fA - g_fOCextTriangle, 0, m_iWidth - 1);
		fB = clampf(fB + g_fOCextTriangle, 0, m_iWidth - 1);

		//в оригинале отнимать и прибавлять единицу не надо, но пришлось сделать чтобы закрыть баги отсечения
		for (int x = fA; x <= fB; ++x)
		{
			int iPosPixel = (y * m_iWidth) + x;
			if (iPosPixel < m_iCountPixels)
			{
				float fCurrDepth = (-(vNormal.x * float(x) + vNormal.y * float(y) + fD) / vNormal.z);

				if (fCurrDepth >= 0.f && m_pArrDepthBufferReProjection[iPosPixel] >= (fCurrDepth - m_fBiasDepth))
				{
					return true;
				}
			}
		}
	}


	iSegmentHeight = vPointC.y - vPointB.y + 1;

	if (iSegmentHeight == 0)
		return false;

	iStartY = clampf(vPointB.y, 0, m_iHeight - 1);
	iFinishY = clampf(vPointC.y, 0, m_iHeight - 1);

	for (int y = iStartY; y <= iFinishY; ++y)
	{
		float fA = (y - vPointA.y) / iTotalHeight;
		float fB = (y - vPointB.y) / iSegmentHeight;
		fA = vPointA.x + (vPointC.x - vPointA.x) * fA;
		fB = vPointB.x + (vPointC.x - vPointB.x) * fB;

		if (fA > fB)
			std::swap(fA, fB);

		fA = clampf(fA - g_fOCextTriangle, 0, m_iWidth - 1);
		fB = clampf(fB + g_fOCextTriangle, 0, m_iWidth - 1);

		//в оригинале отнимать и прибавлять единицу не надо, но пришлось сделать чтобы закрыть баги отсечения
		for (int x = fA; x <= fB; ++x)
		{
			int iPosPixel = (y * m_iWidth) + x;
			if (iPosPixel < m_iCountPixels)
			{
				float fCurrDepth = (-(vNormal.x * float(x) + vNormal.y * float(y) + fD) / vNormal.z);

				if (fCurrDepth >= 0.f && m_pArrDepthBufferReProjection[iPosPixel] >= (fCurrDepth - m_fBiasDepth))
				{
					return true;
				}
			}
		}
	}

	return isVisible;
}