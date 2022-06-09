#ifndef __IXFRUSTUM_H
#define __IXFRUSTUM_H

#include <gdefines.h>
#include <common/math.h>

//! класс описывающий фрустум
class IXFrustum: public IXUnknown
{
public:
	//! обновление фрустума, на вход матрицы по которым необходимо построить фрустум
	virtual void update(
		const float4x4 &mView,	//<! видовая матрица
		const float4x4 &mProj	//<! проекционная матрица
	) = 0;

	virtual void update(const SMPLANE *pPlanes, bool isNormalized = false) = 0;

	//! находится ли точка во фрустуме
	virtual bool pointInFrustum(const float3 &vPoint) const = 0;

	//! находится ли треугольник во фрутстуме
	virtual bool polyInFrustum(const float3 &vPoint1, const float3 &vPoint2, const float3 &vPoint3) const = 0;

	//! находится ли полигон во фрустуме полностью
	virtual bool polyInFrustumAbs(const float3 &vPoint1, const float3 &vPoint2, const float3 &vPoint3) const = 0;


	//! находится ли полигон во фрустуме
	virtual bool sphereInFrustum(const float3 &vPoint, float fRadius) const = 0;

	//! находится ли сфера во фрустуме полностью
	virtual bool sphereInFrustumAbs(const float3 &vPoint, float fRadius) const = 0;

	//! находится ли параллелепипед (описанный точками экстремума) во фрустуме. isStrict задает строгий режим проверки (AABB должен полностью входить в фрустум) иначе отслеживается частичное пересечение
	virtual bool boxInFrustum(const float3 &vMin, const float3 &vMax, bool *pIsStrict = NULL) const = 0;
	virtual bool boxInFrustum(const SMAABB &aabb, bool *pIsStrict = NULL) const = 0;

	//! находится ли параллелепипед (описанный точками экстремума) во фрустуме
	virtual bool frustumInFrustum(const IXFrustum *pOther) const = 0;


	//! возвращает координаты точки фрустума, iNumPoint = [0,7]
	virtual float3 getPoint(int iNumPoint) const = 0;

	//! возвращает координаты центра фрустума
	virtual float3 getCenter() const = 0;

	virtual UINT getPlaneCount() const = 0;

	virtual const SMPLANE& getPlaneAt(UINT idx) const = 0;

	////! устанавливает координаты точки фрустума, iNumPoint = [0,7]
	//virtual void setPoint(int iNumPoint, const float3 *pPoint) = 0;

	////! устанавливает координаты центра фрустума
	//virtual void setCenter(const float3 *pCenter) = 0;
};

#endif
