
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __CAMERA_H
#define __CAMERA_H

#include "sxgcore.h"

//класс описывающий фрустум
class CFrustum: public IXUnknownImplementation<IXFrustum>
{
public:
	SX_ALIGNED_OP_MEM();

	CFrustum();

	void update(const float4x4 &mView, const float4x4 &mProj) override;
	void update(const SMPLANE *pPlanes, bool isNormalized = false) override;

	bool pointInFrustum(const float3 &vPoint) const override;
	bool polyInFrustum(const float3 &vPount1, const float3 &vPount2, const float3 &vPount3) const override;
	bool polyInFrustumAbs(const float3 &vPount1, const float3 &vPount2, const float3 &vPount3) const override;

	bool sphereInFrustum(const float3 &vPount, float fRadius) const override;

	bool sphereInFrustumAbs(const float3 &vPount, float fRadius) const override;
	bool boxInFrustum(const float3 &vMin, const float3 &vMax, bool *pIsStrict = NULL) const override;
	bool boxInFrustum(const SMAABB &aabb, bool *pIsStrict = NULL) const override;

	bool frustumInFrustum(const IXFrustum *pOther) const override;

	float3 getPoint(int iNumPoint) const override;
	float3 getCenter() const override;

	UINT getPlaneCount() const override;
	const SMPLANE& getPlaneAt(UINT idx) const override;

	/*void setPoint(int iNumPoint, const float3 *pPoint);
	void setCenter(const float3 *pCenter);*/

private:

	CFrustumPlane m_aFrustumPlanes[6];

	mutable float3 m_aPoints[8];
	float3 m_vCenter;

	mutable bool m_isPointValid[8];
};

//**************************************************************************

class CCamera: public ICamera
{
public:
	CCamera();
	~CCamera();

	void Release() override
	{
		delete this;
	}
	SX_ALIGNED_OP_MEM();

	void setOrientation(const SMQuaternion &qOrientation) override;

	const SMQuaternion& getOrientation() override;

	const SMMATRIX& getViewMatrix() const override;//получаем матрицу вида

	const float3& getPosition() const override;
	void setPosition(const float3 &vPos) override;

	const float3& getRight() const override;
	const float3& getUp() const override;
	const float3& getLook() const override;

	const float3& getRotation() const override;//?

	void setFOV(float fFOV) override;
	float getFOV() const override;

	void updateFrustum(const SMMATRIX &mProjection) override;
	IXFrustum* getFrustum() override;

protected:
	CFrustum *m_pFrustum;	//!< фрустум этой камеры

	float3 m_vRight = float3(1.0f, 0.0f, 0.0f);
	float3 m_vUp = float3(0.0f, 1.0f, 0.0f);
	float3 m_vLook = float3(0.0f, 0.0f, 1.0f);

	float3 m_vPosition = float3(0.0f, 0.0f, 0.0f);

	float3 m_vPitchYawRoll = float3(0.0f, 0.0f, 0.0f);

	SMQuaternion m_qRotation;

	mutable float4x4 m_mView;

	float m_fFOV;
};

#endif
