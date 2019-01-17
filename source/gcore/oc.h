
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __OC_H
#define __OC_H

#include <gdefines.h>
#include <graphix/graphix.h>
#include <common/SXMath.h>
#include <mutex>

#include <gcore/sxgcore.h>

extern IGXContext *g_pDXDevice;
extern D3DCAPS9 g_dxCaps;
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

#define OC_MAX_MUTEX_COUNT 512

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

	void ensureUpdateDone();

protected:

	class COCReprojection: public IParallelForBody
	{
	public:
		COCReprojection(float4 *pArrWorldPos,
			const float4x4 *mViewProj,
			float _r_near,
			float _r_far,
			int iWidth,
			int iHeight,
			int iCountPixels,
			float *pArrDepthBufferReProjection,
			float *pArrDepthBufferRasterize,
			std::mutex *pArrDepthBufferMutex
		):
		m_pArrWorldPos(pArrWorldPos),
		m_mViewProj(*mViewProj),
		r_near(_r_near),
		r_far(_r_far),
		m_iWidth(iWidth),
		m_iHeight(iHeight),
		m_iCountPixels(iCountPixels),
		m_pArrDepthBufferReProjection(pArrDepthBufferReProjection),
		m_pArrDepthBufferRasterize(pArrDepthBufferRasterize),
		m_pArrDepthBufferMutex(pArrDepthBufferMutex)
		{
		}

		void forLoop(int iStart, int iEnd) const
		{
			Core_PStartSection(PERF_SECTION_OC_REPROJECTION);

			float4 vNewPos;
			const float4x4 &mViewProj = m_mViewProj;
			float2 vNewPos2;

			for(int i = iStart; i < iEnd; ++i)
			{
				vNewPos = SMVector4Transform(m_pArrWorldPos[i], mViewProj);

				vNewPos.x /= abs(vNewPos.w);
				vNewPos.y /= abs(vNewPos.w);
				vNewPos.z = (vNewPos.z + r_near) / r_far;

				vNewPos.x = vNewPos.x * 0.5f + 0.5f;
				vNewPos.y = (vNewPos.y * (-0.5f) + 0.5f);

				//костыль решения проблем округления, без этого будут белые линии

				vNewPos2.x = float(int(vNewPos.x * 10000.f) / 10000.f);
				vNewPos2.y = float(int(vNewPos.y * 10000.f) / 10000.f);

				if(vNewPos2.x == 0.f || vNewPos2.x == 1.f)
					vNewPos.x = vNewPos2.x;

				if(vNewPos2.y == 0.f || vNewPos2.y == 1.f)
					vNewPos.y = vNewPos2.y;

				//******************************************************************

				if((vNewPos.x <= 1.f && vNewPos.x >= 0.f) && (vNewPos.y <= 1.f && vNewPos.y >= 0.f))
				{
					int x = floor(vNewPos.x * float(m_iWidth) + 0.5f);
					int y = floor(vNewPos.y * m_iHeight + 0.5f);
					int iPosPixel = int(y * m_iWidth) + x;

					if(iPosPixel > m_iCountPixels)
						int qwerty = 0;
					else
					{
						//если в буфере репроекции нет записей для текущего пикселя, либо записанная глубина меньше чем новая
						std::lock_guard<std::mutex> lock(m_pArrDepthBufferMutex[iPosPixel % OC_MAX_MUTEX_COUNT]);

						if(m_pArrDepthBufferReProjection[iPosPixel] >= 1.f || vNewPos.z > m_pArrDepthBufferReProjection[iPosPixel])
							m_pArrDepthBufferReProjection[iPosPixel] = vNewPos.z;
					}
				}
			}
			Core_PEndSection(PERF_SECTION_OC_REPROJECTION);
		};

		SX_ALIGNED_OP_MEM;

	protected:
		float4x4 m_mViewProj;
		float4 *m_pArrWorldPos;
		float r_near;
		float r_far;
		int m_iWidth;
		int m_iHeight;
		int m_iCountPixels;
		float *m_pArrDepthBufferReProjection;
		float *m_pArrDepthBufferRasterize;
		std::mutex *m_pArrDepthBufferMutex;
	};

	class COCUpdate: public IParallelForBody
	{
	public:
		COCUpdate(float4 *aWorldRays,
			int iWidth,
			int iHeight,
			float3 *vObserverPos,
			float4 *pArrWorldPos,
			float *pArrDepthBuffer
		):
		m_iWidth(iWidth),
		m_iHeight(iHeight),
		m_vObserverPos(*vObserverPos),
		m_pArrWorldPos(pArrWorldPos),
		m_pArrDepthBuffer(pArrDepthBuffer)
		{
			m_aWorldRays[0] = aWorldRays[0];
			m_aWorldRays[1] = aWorldRays[1];
			m_aWorldRays[2] = aWorldRays[2];
			m_aWorldRays[3] = aWorldRays[3];
		}

		void forLoop(int iStart, int iEnd) const
		{
			Core_PStartSection(PERF_SECTION_OC_UPDATE);

			float4 vWorldRay0, vWorldRay1;
			float4 vWorldPos;
			float4 vEyeRay, vWorldRay;

			for(int x = iStart; x < iEnd; ++x)
			{
				vWorldRay0 = SMVectorLerp(m_aWorldRays[0], m_aWorldRays[1], float(x) / m_iWidth);
				vWorldRay1 = SMVectorLerp(m_aWorldRays[3], m_aWorldRays[2], float(x) / m_iWidth);
				for(int y = 0; y < m_iHeight; ++y)
				{
					int iPosPixel = (y * m_iWidth) + x;
					vWorldRay = SMVectorLerp(vWorldRay1, vWorldRay0, float(y) / m_iHeight);
					vWorldPos = m_vObserverPos + vWorldRay * m_pArrDepthBuffer[iPosPixel];
					vWorldPos.w = 1.f;
					m_pArrWorldPos[iPosPixel] = vWorldPos;
				}
			}
			Core_PEndSection(PERF_SECTION_OC_UPDATE);
		};

		SX_ALIGNED_OP_MEM;

	protected:

		float4 m_aWorldRays[4];
		int m_iWidth;
		int m_iHeight;
		float3 m_vObserverPos;
		float4 *m_pArrWorldPos; 
		float *m_pArrDepthBuffer;
	};

	bool triFrustumCulling(const float3 &vA, const float3 &vB, const float3 &vC);
	bool triRasterize(const float4 &vA, const float4 &vB, const float4 &vC, bool isRasterize, const float3 &vNormal2, const float2_t &vNearFar);

	void ensureReprojectionDone();

	//! массив surfaces для обработки
	IGXSurface *m_pSurfDepthBuffer[3];

	//! массив глубины
	float *m_pArrDepthBuffer = 0;

	//! массив мировых координат
	float4 *m_pArrWorldPos = 0;

	//! массив репроекционной глубины
	float *m_pArrDepthBufferReProjection = 0;

	//! массив растеризации (debug)
	float *m_pArrDepthBufferRasterize = 0;

	//! массив замков синхронизации
	std::mutex *m_pArrDepthBufferMutex = 0;

	COCReprojection *m_pReprojectionCycle = 0;
	ID m_idReprojectionCycle = -1;
	COCUpdate *m_pUpdateCycle = 0;
	ID m_idUpdateCycle = -1;

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