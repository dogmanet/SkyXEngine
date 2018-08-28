
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __OC_H
#define __OC_H

#include <gdefines.h>
#include <d3d9.h>
#include <common/SXMath.h>

#include <gcore/sxgcore.h>

extern IDirect3DDevice9 *g_pDXDevice;
extern D3DPRESENT_PARAMETERS g_oD3DAPP;

//! коэффициент размера буфера глубины occlusion culling
#define OC_SIZE_COEF 0.25f

//! дистанция (в метрах) при которой тест occlusion culling всегда будет давать true
#define OC_DIST_NEAR_NOT_CULL 4.f

//! погрешность разниц глубин для теста occlusion culling
//static float g_fOCbiasDepth = 0.0001f;

//! количество пикселей расширения треугольников для 
const float g_fOCextTriangle = 2.f;

//! небольшео расширение бокса для теста occlusion culling
const float3 g_cvOCext(0.05f, 0.05f, 0.05f);

//##########################################################################

/*inline bool OC_RasterizeQuad(const float3 &vA, const float3 &vB, const float3 &vC, const float3 &vD, const float2 &vNearFar)
{
	float3 vNormal = TriGetNormal(vA, vB, vC);
	return (OC_TriangleRasterize(vA, vB, vC, false, vNormal, vNearFar) || OC_TriangleRasterize(vB, vC, vD, false, vNormal, vNearFar));
}*/

void SwapFloat3(float3 &vA, float3 &vB);
void TriGetSphere(const float3 &vA, const float3 &vB, const float3 &vC, float &fRadius, float2 &vCenter);
void QuadGetSphere(const float2 &vMin, const float2 &vMax, float &fRadius, float2 &vCenter);

//##########################################################################

class COcclusionCulling
{
public:

	SX_ALIGNED_OP_MEM

	COcclusionCulling();
	~COcclusionCulling();

	//! инициализация на основании размеров рендера
	void init(int iWidth, int iHeight);

	//! обработка потери устройства
	void onLostDevice();

	//! обработк сброса устройства
	void onResetDevice(int iWidth, int iHeight);

	//! установка включен/выключен тест
	void setEnable(bool isEnable);

	//! возвращает текущее состояния влюченности теста
	bool getEnable();

	/*! обновление данных теста
	 \param idDepthMap - идентификатор rt глубины
	 \param pFrustum - фрустум камеры
	*/
	void update(ID idDepthMap, const IFrustum *pFrustum);

	//! репроекция старого буфера глубины на основании новых матриц
	void reprojection();

	//! просчет видимости бокса
	bool comVisible(const float3 *pMax, const float3 *pMin);

protected:

	bool triFrustumCulling(const float3 &vA, const float3 &vB, const float3 &vC);
	bool triRasterize(const float4 &vA, const float4 &vB, const float4 &vC, bool isRasterize, const float3 &vNormal2, const float2_t &vNearFar);

	//! массив surfaces для обработки
	IDirect3DSurface9 *m_pSurfDepthBuffer[3];

	//! массив глубины
	float *m_pArrDepthBuffer = 0;

	//! массив мировых координат
	float4 *m_pArrWorldPos = 0;

	//! массив репроекционной глубины
	float *m_pArrDepthBufferReProjection = 0;

	//! массив растеризации (debug)
	float *m_pArrDepthBufferRasterize = 0;

	//! включен ли тест
	bool m_isEnable;

	//! прошлая видовая матрица
	float4x4 m_mOldView;

	//! прошлая проекционная матрица
	float4x4 m_mOldProj;

	//! общее количество пикселей в буфере для теста
	int m_iCountPixels;

	//! ширина буфера теста
	int m_iWidth;

	//! высота буфера теста
	int m_iHeight;

	//! массив ке глубин
	ID m_aRTdepth[2];

	//! текущий ключ массива #m_aRTdepth
	int m_iCurrRTdepth;


	ID m_idVS_ScreenOut;
	ID m_idPS_ScreenOut;
	ID m_idPS_FindMax9;

	//! текущий фрустум камеры
	const IFrustum *m_pFrustum;

	int m_iCountFC;
	int m_iCountFCfail;

	float m_fBiasDepth;
};

#endif