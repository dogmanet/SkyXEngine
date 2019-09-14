
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "camera.h"

CFrustum::CFrustum()
{
	memset(m_isPointValid, 0, sizeof(m_isPointValid));
}

void CFrustum::update(const float4x4* view, const float4x4* proj)
{
	float4x4 matComb = SMMatrixMultiply(*view, *proj);

	// right (-x)
	m_aFrustumPlanes[0].m_vNormal.x = matComb._14 - matComb._11;
	m_aFrustumPlanes[0].m_vNormal.y = matComb._24 - matComb._21;
	m_aFrustumPlanes[0].m_vNormal.z = matComb._34 - matComb._31;
	m_aFrustumPlanes[0].m_fDistance = matComb._44 - matComb._41;

	// left (+x)
	m_aFrustumPlanes[1].m_vNormal.x = matComb._14 + matComb._11;
	m_aFrustumPlanes[1].m_vNormal.y = matComb._24 + matComb._21;
	m_aFrustumPlanes[1].m_vNormal.z = matComb._34 + matComb._31;
	m_aFrustumPlanes[1].m_fDistance = matComb._44 + matComb._41;

	// top (-y)
	m_aFrustumPlanes[2].m_vNormal.x = matComb._14 - matComb._12;
	m_aFrustumPlanes[2].m_vNormal.y = matComb._24 - matComb._22;
	m_aFrustumPlanes[2].m_vNormal.z = matComb._34 - matComb._32;
	m_aFrustumPlanes[2].m_fDistance = matComb._44 - matComb._42;

	// bottom (+y)
	m_aFrustumPlanes[3].m_vNormal.x = matComb._14 + matComb._12;
	m_aFrustumPlanes[3].m_vNormal.y = matComb._24 + matComb._22;
	m_aFrustumPlanes[3].m_vNormal.z = matComb._34 + matComb._32;
	m_aFrustumPlanes[3].m_fDistance = matComb._44 + matComb._42;
	
	// near (-z)
	m_aFrustumPlanes[4].m_vNormal.x = matComb._14 - matComb._13;
	m_aFrustumPlanes[4].m_vNormal.y = matComb._24 - matComb._23;
	m_aFrustumPlanes[4].m_vNormal.z = matComb._34 - matComb._33;
	m_aFrustumPlanes[4].m_fDistance = matComb._44 - matComb._43;

	// far (+z)
	m_aFrustumPlanes[5].m_vNormal.x = matComb._14 + matComb._13;
	m_aFrustumPlanes[5].m_vNormal.y = matComb._24 + matComb._23;
	m_aFrustumPlanes[5].m_vNormal.z = matComb._34 + matComb._33;
	m_aFrustumPlanes[5].m_fDistance = matComb._44 + matComb._43;

	//Нормализация плоскостей
	for(int i = 0; i < 6; ++i)
		m_aFrustumPlanes[i].normalize();

	memset(m_isPointValid, 0, sizeof(m_isPointValid));
}

void CFrustum::update(const SMPLANE *pPlanes, bool isNormalized)
{
	for(int i = 0; i < 6; ++i)
	{
		m_aFrustumPlanes[i] = CFrustumPlane(pPlanes[i]);
		if(!isNormalized)
		{
			m_aFrustumPlanes[i].normalize();
		}
	}

	memset(m_isPointValid, 0, sizeof(m_isPointValid));
}

bool CFrustum::pointInFrustum(const float3 *point) const
{
	for(int i = 0; i < 6; ++i)
	{
		float tmp = SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, *point) + m_aFrustumPlanes[i].m_fDistance;
		if(int(tmp * 1000.0f) <= 0)
		{
			return false;
		}
	}
	return true;
}

bool CFrustum::polyInFrustum(const float3* p1, const float3* p2, const float3* p3) const
{
	/*if(pointInFrustum(p1) || pointInFrustum(p2) || pointInFrustum(p3))
	return true;*/

	for(int i = 0; i < 6; i++)
	{
		if(int((SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, *p1) + m_aFrustumPlanes[i].m_fDistance) * 1000.f) > 0) continue;
		if(int((SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, *p2) + m_aFrustumPlanes[i].m_fDistance) * 1000.f) > 0) continue;
		if(int((SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, *p3) + m_aFrustumPlanes[i].m_fDistance) * 1000.f) > 0) continue;
		return false;
	}
	return true;
}

bool CFrustum::polyInFrustumAbs(const float3* p1, const float3* p2, const float3* p3) const
{
	if(pointInFrustum(p1) && pointInFrustum(p2) && pointInFrustum(p3))
		return true;

	return false;
}

bool CFrustum::sphereInFrustum(const float3 *point, float radius) const
{
	for(int i = 0; i<6; ++i)
	{
		if(SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, *point) + m_aFrustumPlanes[i].m_fDistance <= -radius)
			return false;
	}
	return true;
}

bool CFrustum::sphereInFrustumAbs(const float3 *point, float radius) const
{
	for(int i = 0; i<6; i++)
	{
		if(SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, *point) + m_aFrustumPlanes[i].m_fDistance > -radius)
			return false;
	}
	return true;
}

bool CFrustum::boxInFrustum(const float3 *min, const float3 *max) const
{
	for(register int p = 0; p < 6; ++p)
	{
		auto &plane = m_aFrustumPlanes[p];
		if(SMVector3Dot(plane.m_vNormal, *min) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(min->x, min->y, max->z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(min->x, max->y, min->z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(min->x, max->y, max->z)) + plane.m_fDistance > 0) continue;

		if(SMVector3Dot(plane.m_vNormal, float3(max->x, min->y, min->z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(max->x, min->y, max->z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(max->x, max->y, min->z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, *max) + plane.m_fDistance > 0) continue;
		return(false);
	}

	return(true);
}

bool CFrustum::frustumInFrustum(const IFrustum *pOther) const
{
	for(register int p = 0; p < 6; ++p)
	{
		auto &plane = m_aFrustumPlanes[p];
		bool isFound = false;
		for(register int j = 0; j < 8; ++j)
		{
			if(SMVector3Dot(plane.m_vNormal, pOther->getPoint(j)) + plane.m_fDistance > 0)
			{
				isFound = true;
				break;
			}
		}
		if(isFound)
		{
			continue;
		}
		return(false);
	}

	return(true);
}

float3 CFrustum::getPoint(int iPoint) const
{
	assert(iPoint >= 0 && iPoint < 8);
	if(iPoint >= 0 && iPoint < 8)
	{
		if(!m_isPointValid[iPoint])
		{
			int p0 = (iPoint & 1);
			int p1 = (iPoint & 2) ? 2 : 3;
			int p2 = (iPoint & 4) ? 4 : 5;

			float fDet = SMMatrix3x3Determinant(SMMATRIX(
				float4(m_aFrustumPlanes[p0].m_vNormal, 0.0f),
				float4(m_aFrustumPlanes[p1].m_vNormal, 0.0f),
				float4(m_aFrustumPlanes[p2].m_vNormal, 0.0f),
				float4(0.0f, 0.0f, 0.0f, 1.0f)
				));
			float3 &vPoint = m_aPoints[iPoint];
			vPoint.x = SMMatrix3x3Determinant(SMMATRIX(
				float4(m_aFrustumPlanes[p0].m_fDistance, m_aFrustumPlanes[p0].m_vNormal.y, m_aFrustumPlanes[p0].m_vNormal.z, 0.0f),
				float4(m_aFrustumPlanes[p1].m_fDistance, m_aFrustumPlanes[p1].m_vNormal.y, m_aFrustumPlanes[p1].m_vNormal.z, 0.0f),
				float4(m_aFrustumPlanes[p2].m_fDistance, m_aFrustumPlanes[p2].m_vNormal.y, m_aFrustumPlanes[p2].m_vNormal.z, 0.0f),
				float4(0.0f, 0.0f, 0.0f, 1.0f)
				));
			vPoint.y = SMMatrix3x3Determinant(SMMATRIX(
				float4(m_aFrustumPlanes[p0].m_vNormal.x, m_aFrustumPlanes[p0].m_fDistance, m_aFrustumPlanes[p0].m_vNormal.z, 0.0f),
				float4(m_aFrustumPlanes[p1].m_vNormal.x, m_aFrustumPlanes[p1].m_fDistance, m_aFrustumPlanes[p1].m_vNormal.z, 0.0f),
				float4(m_aFrustumPlanes[p2].m_vNormal.x, m_aFrustumPlanes[p2].m_fDistance, m_aFrustumPlanes[p2].m_vNormal.z, 0.0f),
				float4(0.0f, 0.0f, 0.0f, 1.0f)
				));
			vPoint.z = SMMatrix3x3Determinant(SMMATRIX(
				float4(m_aFrustumPlanes[p0].m_vNormal.x, m_aFrustumPlanes[p0].m_vNormal.y, m_aFrustumPlanes[p0].m_fDistance, 0.0f),
				float4(m_aFrustumPlanes[p1].m_vNormal.x, m_aFrustumPlanes[p1].m_vNormal.y, m_aFrustumPlanes[p1].m_fDistance, 0.0f),
				float4(m_aFrustumPlanes[p2].m_vNormal.x, m_aFrustumPlanes[p2].m_vNormal.y, m_aFrustumPlanes[p2].m_fDistance, 0.0f),
				float4(0.0f, 0.0f, 0.0f, 1.0f)
				));

			vPoint /= -fDet;
		}
		return(m_aPoints[iPoint]);
	}
	
	return(float3());
}

float3 CFrustum::getCenter() const
{
	assert(!"Not implemented!");
	return m_vCenter;
}

//void CFrustum::setPoint(int iNumPoint, const float3 *pPoint)
//{
//	if(iNumPoint >= 0 && iNumPoint < 8)
//		m_aPoints[iNumPoint] = *pPoint;
//}
//
//void CFrustum::setCenter(const float3 *pCenter)
//{
//	m_vCenter = *pCenter;
//}

//##########################################################################

CCamera::CCamera()
{
	m_vPosition = float3(0.0f, 0.0f, 0.0f);
	m_vRight = float3(1.0f, 0.0f, 0.0f);
	m_vUp = float3(0.0f, 1.0f, 0.0f);
	m_vLook = float3(0.0f, 0.0f, 1.0f);

	m_oFrustum = CFrustum();

	m_vPitchYawRoll = float3_t(0, 0, 0);
}

CCamera::~CCamera()
{

}

void CCamera::posLeftRight(float units)
{
	m_vPosition += float3(m_vRight.x, 0.0f, m_vRight.z) * units;
	//LastVal.x = units;
}

void CCamera::posUpDown(float units)
{
	m_vPosition.y += m_vUp.y * units;
}

void CCamera::posFrontBack(float units)
{
	m_vPosition += float3(m_vLook.x, 0.0f, m_vLook.z) * units;
	//LastVal.z = units;
}


void CCamera::rotUpDown(float angle)
{
	m_vPitchYawRoll.x -= angle;
	if(m_vPitchYawRoll.x > SM_PIDIV2)
	{
		m_vPitchYawRoll.x = SM_PIDIV2;
	}
	else if(m_vPitchYawRoll.x < -SM_PIDIV2)
	{
		m_vPitchYawRoll.x = -SM_PIDIV2;
	}

	updateView();
}

void CCamera::rotRightLeft(float angle)
{
	m_vPitchYawRoll.y -= angle;
	while(m_vPitchYawRoll.y < 0.0f)
	{
		m_vPitchYawRoll.y += SM_2PI;
	}
	while(m_vPitchYawRoll.y > SM_2PI)
	{
		m_vPitchYawRoll.y -= SM_2PI;
	}

	updateView();
}

void CCamera::roll(float angle)
{
	m_vPitchYawRoll.z -= angle;
	updateView();
}

void CCamera::updateView()
{
	SMQuaternion q = SMQuaternion(m_vPitchYawRoll.x, 'x')
		* SMQuaternion(m_vPitchYawRoll.y, 'y')
		* SMQuaternion(m_vPitchYawRoll.z, 'z');

	m_vRight = q * float3(1.0f, 0.0f, 0.0f);
	m_vUp = q * float3(0.0f, 1.0f, 0.0f);
	m_vLook = q * float3(0.0f, 0.0f, 1.0f);
}

void CCamera::setOrientation(const SMQuaternion *q)
{
	m_vPitchYawRoll = SMMatrixToEuler(q->GetMatrix());

	m_vRight = (*q) * float3(1.0f, 0.0f, 0.0f);
	m_vUp = (*q) * float3(0.0f, 1.0f, 0.0f);
	m_vLook = (*q) * float3(0.0f, 0.0f, 1.0f);
}


void CCamera::getViewMatrix(float4x4* view_matrix)
{
	m_mView = SMMatrixLookToLH(m_vPosition, m_vLook, m_vUp);
	*view_matrix = m_mView;// SMMatrixLookToLH(m_vPosition, m_vLook, m_vUp);
}


void CCamera::getPosition(float3* pos) const
{
	*pos = m_vPosition;
}

void CCamera::setPosition(const float3* pos)
{
	m_vPosition = *pos;
}


void CCamera::getDirection(float3* dir) const
{
	*dir = m_vLook;
}

/*void CCamera::setDirection(const float3* dir)
{
m_vLook = *dir;
}*/


void CCamera::getRight(float3* right) const
{
	*right = m_vRight;
}

void CCamera::getUp(float3* up) const
{
	*up = m_vUp;
}

void CCamera::getLook(float3* look) const
{
	*look = m_vLook;
}

void CCamera::getRotation(float3* rot) const
{
	*rot = m_vPitchYawRoll;
}

float CCamera::getRotationX() const
{
	return m_vPitchYawRoll.x;
}

float CCamera::getRotationY() const
{
	return m_vPitchYawRoll.y;
}

float CCamera::getRotationZ() const
{
	return m_vPitchYawRoll.z;
}

void CCamera::setFOV(float fov)
{
	m_fFOV = fov;
}

float CCamera::getFOV() const
{
	return(m_fFOV);
}

void CCamera::updateFrustum(const float4x4 *pProjection)
{
	m_oFrustum.update(&m_mView, pProjection);
}

const IFrustum* CCamera::getFrustum()
{
	return &m_oFrustum;
}