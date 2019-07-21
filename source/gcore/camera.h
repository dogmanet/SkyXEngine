
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __CAMERA_H
#define __CAMERA_H

#include "sxgcore.h"

//класс описывающий фрустум
class CFrustum: public IFrustum
{
public:
	SX_ALIGNED_OP_MEM2();

	CFrustum();

	void update(const float4x4 *pView, const float4x4 *pProj) override;
	void update(const SMPLANE *pPlanes, bool isNormalized = false) override;

	bool pointInFrustum(const float3 *pPoint) const override;
	bool polyInFrustum(const float3 *pPount1, const float3 *pPount2, const float3 *pPount3) const override;
	bool polyInFrustumAbs(const float3 *pPount1, const float3 *pPount2, const float3 *pPount3) const override;

	bool sphereInFrustum(const float3 *pPount, float fRadius) const override;

	bool sphereInFrustumAbs(const float3 *pPount, float fRadius) const override;
	bool boxInFrustum(const float3 *pMin, const float3 *pMax) const override;

	bool frustumInFrustum(const IFrustum *pOther) const override;

	float3 getPoint(int iNumPoint) const override;
	float3 getCenter() const override;

	/*void setPoint(int iNumPoint, const float3 *pPoint);
	void setCenter(const float3 *pCenter);*/

private:

	CFrustumPlane m_aFrustumPlanes[6];

	mutable float3 m_aPoints[8];
	float3 m_vCenter;

	bool m_isPointValid[8];
};

//**************************************************************************

class CCamera: public ICamera
{
public:
	CCamera();
	~CCamera();
	void Release()
	{
		mem_del(this);
	}
	SX_ALIGNED_OP_MEM

		void posLeftRight(float fUnits);//влево/вправо
	void posUpDown(float fUnits);	//вверх/вниз
	void posFrontBack(float fUnits);//вперед/назад

	void rotUpDown(float fAngle);	//вращение вверх/вниз
	void rotRightLeft(float fAngle);//вращение вправо/влево
	void roll(float fAngle);		//крен
	void setOrientation(const SMQuaternion *pQuaternion);

	void getViewMatrix(float4x4 *pMatrix);//получаем матрицу вида

	void getPosition(float3 *pPos) const;
	void setPosition(const float3 *pPos);

	void getDirection(float3 *pDir) const;
	//void setDirection(const float3 *pDir);

	void getRight(float3 *pRight) const;
	void getUp(float3 *pUp) const;
	void getLook(float3 *pLook) const;
	void getRotation(float3 *pRot) const;

	float getRotationX() const;
	float getRotationY() const;
	float getRotationZ() const;

	void setFOV(float fFOV);
	float getFOV() const;

	void updateView();

	void updateFrustum(const float4x4 *pmProjection);
	const IFrustum* getFrustum();

protected:
	CFrustum m_oFrustum;	//!< фрустум этой камеры

	float3 m_vRight;
	float3 m_vUp;
	float3 m_vLook;

	float3 m_vPosition;

	float3_t m_vPitchYawRoll;

	float4x4 m_mView;

	float m_fFOV;
};

#endif
