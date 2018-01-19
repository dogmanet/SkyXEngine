
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __CAMERA_H
#define __CAMERA_H

#include <common/SXMath.h>

class Camera
{
	SMMATRIX m_mResult;

	SMVECTOR m_vEye;
	SMVECTOR m_vAt;
	SMVECTOR m_vUp;

	float3_t m_vCamRef;
	float3_t m_vPosition;

	float fYaw;
	float fPitch;

	bool bMove[4];
public:
	enum CAMERA_MOVE
	{
		CAMERA_MOVE_FORWARD,
		CAMERA_MOVE_BACKWARD,
		CAMERA_MOVE_LEFT,
		CAMERA_MOVE_RIGHT
	};
	Camera();

	SMMATRIX GetMatrix();

	void RotateX(float fDelta);
	void RotateY(float fDelta);

	void Rotate(float fDeltaX, float fDeltaY);

	void UpdateMatrix();

	void Forward(float fDelta);
	void Strafe(float fDelta);

	void move(CAMERA_MOVE m, bool state);
	void advance();

	float3_t getPos();

	float3_t GetViewDir();
};

#endif
