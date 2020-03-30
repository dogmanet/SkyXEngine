#ifndef __IXOCCLUSIONCULLER_H
#define __IXOCCLUSIONCULLER_H

#include <gdefines.h>
#include <common/math.h>

class ICamera;
class IXOcclusionCuller: public IXUnknown
{
public:
	//! обновление
	virtual void XMETHODCALLTYPE update(ICamera *pCamera, float fFOV, float fAspectRatio, float fFarPlane) = 0;

	//! видна ли точка
	virtual bool XMETHODCALLTYPE isPointVisible(const float3 &vPoint) const = 0;

	//! виден ли треугольник
	virtual bool XMETHODCALLTYPE isPolyVisible(const float3 &vPoint1, const float3 &vPoint2, const float3 &vPoint3) const = 0;

	//! видна ли сфера
	virtual bool XMETHODCALLTYPE isSphereVisible(const float3 &vOrigin, float fRadius) const = 0;

	//! находится ли параллелепипед (описанный точками экстремума) во фрустуме
	virtual bool XMETHODCALLTYPE isAABBvisible(const float3 &vMin, const float3 &vMax) const = 0;
};

#endif
