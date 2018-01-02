#include "Camera.h"

Camera::Camera():
m_vPosition(float4(0.0f, 00.0f, -10.0f, 0.0f)),
m_vAt(float4(0.0f, 0.0f, 0.0f, 0.0f)),
m_vUp(float4(0.0f, 1.0f, 0.0f, 0.0f))
{
	m_vCamRef = float3_t(0.0f, 0.0f, 1.0f);
	fYaw = 0.0f;
	fPitch = 0.0f;
	bMove[0] = false;
	bMove[1] = false;
	bMove[2] = false;
	bMove[3] = false;
	UpdateMatrix();
}

SMMATRIX Camera::GetMatrix()
{
	return(m_mResult);
}

void Camera::UpdateMatrix()
{
	m_vEye = m_vPosition;
	SMMATRIX mRotation = SMMatrixRotationY(-fYaw);

	SMVECTOR vTransformedReference = SMVector3Transform(m_vCamRef, mRotation);
	m_vAt = m_vEye + vTransformedReference;
	m_mResult = SMMatrixLookAtLH(m_vEye, m_vAt, m_vUp);
}

void Camera::Rotate(float fDeltaX, float fDeltaY)
{
	fYaw += fDeltaX;
	if(fYaw > SM_2PI)
	{
		fYaw -= SM_2PI;
	}
	if(fYaw < -SM_2PI)
	{
		fYaw += SM_2PI;
	}
	fPitch += fDeltaY;
	if(fPitch >= SM_PIDIV2)
	{
		fPitch = SM_PIDIV2 - 0.001f;
	}
	if(fPitch <= -SM_PIDIV2)
	{
		fPitch = -SM_PIDIV2 + 0.001f;
	}
	m_vCamRef.y = sin(fPitch);
	m_vCamRef.z = cos(fPitch);
	UpdateMatrix();
}

void Camera::RotateX(float fDelta)
{
	Rotate(fDelta, 0.0f);
}

void Camera::RotateY(float fDelta)
{
	Rotate(0.0f, fDelta);
}

void Camera::Forward(float fDelta)
{
	SMMATRIX mForwardMovement = SMMatrixRotationY(-fYaw);
	float4 v = float4(0.0f, m_vCamRef.y * fDelta, m_vCamRef.z * fDelta, 0.0f);
	v = SMVector3Transform(v, mForwardMovement);

	m_vPosition = (float3)(m_vPosition + v);
	UpdateMatrix();
}

void Camera::Strafe(float fDelta)
{
	SMMATRIX mForwardMovement = SMMatrixRotationY(-fYaw);
	float4 v = float4(fDelta, 0.0f, 0.0f, 0.0f);
	v = SMVector3Transform(v, mForwardMovement);

	m_vPosition = (float3)(m_vPosition + v);
	UpdateMatrix();
}

void Camera::move(CAMERA_MOVE m, bool state)
{
	bMove[m] = state;
}

void Camera::advance()
{
	if(bMove[CAMERA_MOVE_FORWARD])
	{
		Forward(0.1f);
	}
	if(bMove[CAMERA_MOVE_BACKWARD])
	{
		Forward(-0.1f);
	}

	if(bMove[CAMERA_MOVE_LEFT])
	{
		Strafe(-0.1f);
	}
	if(bMove[CAMERA_MOVE_RIGHT])
	{
		Strafe(0.1f);
	}
}

float3_t Camera::getPos()
{
	return(m_vPosition);
}

float3_t Camera::GetViewDir()
{
	return(m_vAt - m_vPosition);
}