
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "camera.h"

CFrustum::CFrustum()
{
	memset(m_isPointValid, 0, sizeof(m_isPointValid));
}

void CFrustum::update(const float4x4 &mView, const float4x4 &mProj)
{
	float4x4 matComb = mView *mProj;

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

	// printf("%.2f, %.2f, %.2f, %.2f\n", m_aFrustumPlanes[4].m_vNormal.x, m_aFrustumPlanes[4].m_vNormal.y, m_aFrustumPlanes[4].m_vNormal.z, m_aFrustumPlanes[4].m_fDistance);

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

bool CFrustum::pointInFrustum(const float3 &vPoint) const
{
	for(int i = 0; i < 6; ++i)
	{
		float tmp = SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, vPoint) + m_aFrustumPlanes[i].m_fDistance;
		if(int(tmp * 1000.0f) <= 0)
		{
			return false;
		}
	}
	return true;
}

bool CFrustum::polyInFrustum(const float3 &p1, const float3 &p2, const float3 &p3) const
{
	/*if(pointInFrustum(p1) || pointInFrustum(p2) || pointInFrustum(p3))
	return true;*/

	for(int i = 0; i < 6; i++)
	{
		if(int((SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, p1) + m_aFrustumPlanes[i].m_fDistance) * 1000.f) > 0) continue;
		if(int((SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, p2) + m_aFrustumPlanes[i].m_fDistance) * 1000.f) > 0) continue;
		if(int((SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, p3) + m_aFrustumPlanes[i].m_fDistance) * 1000.f) > 0) continue;
		return false;
	}
	return true;
}

bool CFrustum::polyInFrustumAbs(const float3 &p1, const float3 &p2, const float3 &p3) const
{
	if(pointInFrustum(p1) && pointInFrustum(p2) && pointInFrustum(p3))
		return true;

	return false;
}

bool CFrustum::sphereInFrustum(const float3 &vPoint, float radius) const
{
	for(int i = 0; i<6; ++i)
	{
		if(SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, vPoint) + m_aFrustumPlanes[i].m_fDistance <= -radius)
			return false;
	}
	return true;
}

bool CFrustum::sphereInFrustumAbs(const float3 &vPoint, float radius) const
{
	for(int i = 0; i<6; i++)
	{
		if(SMVector3Dot(m_aFrustumPlanes[i].m_vNormal, vPoint) + m_aFrustumPlanes[i].m_fDistance > -radius)
			return false;
	}
	return true;
}

bool CFrustum::boxInFrustum(const SMAABB &aabb, bool *pIsStrict) const
{
	return(boxInFrustum(aabb.vMin, aabb.vMax, pIsStrict));
}
#if 0
static bool BoxInFrustum1(const CFrustumPlane *m_aFrustumPlanes, const float3 &vMin, const float3 &vMax)
{
	for(register int p = 0; p < 6; ++p)
	{
		auto &plane = m_aFrustumPlanes[p];
		if(SMVector3Dot(plane.m_vNormal, vMin) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMin.x, vMin.y, vMax.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMin.x, vMax.y, vMin.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMin.x, vMax.y, vMax.z)) + plane.m_fDistance > 0) continue;

		if(SMVector3Dot(plane.m_vNormal, float3(vMax.x, vMin.y, vMin.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMax.x, vMin.y, vMax.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMax.x, vMax.y, vMin.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, vMax) + plane.m_fDistance > 0) continue;
		return(false);
	}
	return(true);
}
static bool BoxInFrustum2(const CFrustumPlane *m_aFrustumPlanes, const float3 &vMin, const float3 &vMax)
{
	bool inside = true;


	for(int p = 0; p < 6; ++p)
	{
		auto &plane = m_aFrustumPlanes[p];
		//находим ближайшую к плоскости вершину
		//проверяем, если она находится за плоскостью, то объект вне врустума
		float d = SMVector3Sum(SMVectorMax(vMin * plane.m_vNormal, vMax * plane.m_vNormal)) + plane.m_fDistance;

		float d2 = max(vMin.x * plane.m_vNormal.x, vMax.x * plane.m_vNormal.x)
			+ max(vMin.y * plane.m_vNormal.y, vMax.y * plane.m_vNormal.y)
			+ max(vMin.z * plane.m_vNormal.z, vMax.z * plane.m_vNormal.z)
			+ plane.m_fDistance;
		inside &= d > 0;

		/*if(d < 0.0f)
		{
		return(false);
		}*/
	}
	return inside;
}


bool CFrustum::boxInFrustum(const float3 &vMin, const float3 &vMax, bool isStrict) const
{
	if(isStrict)
	{
		return(false);
	}

	bool b0 = BoxInFrustum1(m_aFrustumPlanes, vMin, vMax);
	//bool b1 = BoxInFrustum2(m_aFrustumPlanes, vMin, vMax);

	//assert(b0 == b1);

	//return(b0);

	/*for(register int p = 0; p < 6; ++p)
	{
		auto &plane = m_aFrustumPlanes[p];
		if(SMVector3Dot(plane.m_vNormal, vMin) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMin.x, vMin.y, vMax.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMin.x, vMax.y, vMin.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMin.x, vMax.y, vMax.z)) + plane.m_fDistance > 0) continue;

		if(SMVector3Dot(plane.m_vNormal, float3(vMax.x, vMin.y, vMin.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMax.x, vMin.y, vMax.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, float3(vMax.x, vMax.y, vMin.z)) + plane.m_fDistance > 0) continue;
		if(SMVector3Dot(plane.m_vNormal, vMax) + plane.m_fDistance > 0) continue;
		return(false);
	}
	return(true);*/
	auto fn = isStrict ? SMVectorMin : SMVectorMax;

	//bool inside = true;


	for(int p = 0; p < 6; ++p)
	{
		auto &plane = m_aFrustumPlanes[p];
		//находим ближайшую к плоскости вершину
		//проверяем, если она находится за плоскостью, то объект вне врустума
		float d = SMVector3Sum(fn(vMin * plane.m_vNormal, vMax * plane.m_vNormal)) + plane.m_fDistance;

		float d2 = max(vMin.x * plane.m_vNormal.x, vMax.x * plane.m_vNormal.x)
			+ max(vMin.y * plane.m_vNormal.y, vMax.y * plane.m_vNormal.y)
			+ max(vMin.z * plane.m_vNormal.z, vMax.z * plane.m_vNormal.z)
			+ plane.m_fDistance;
		//inside &= d > 0;

		if(d <= 0.0f)
		{
			assert(!b0);
			return(false);
		}
	}
	//assert(inside == b0);
	//return inside;

	assert(b0);
	return(true);
}
#endif
bool CFrustum::boxInFrustum(const float3 &vMin, const float3 &vMax, bool *pIsStrict) const
{
	bool isVisible = true;
	bool isVisibleStrict = true;

	for(int p = 0; p < 6; ++p)
	{
		auto &plane = m_aFrustumPlanes[p];
		//находим ближайшую к плоскости вершину
		//проверяем, если она находится за плоскостью, то объект вне врустума
		float3 vTmpMin = vMin * plane.m_vNormal;
		float3 vTmpMax = vMax * plane.m_vNormal;
		float d = SMVector3Sum(SMVectorMax(vTmpMin, vTmpMax)) + plane.m_fDistance;

		isVisible &= d > 0.0f;

		if(isVisible && pIsStrict)
		{
			d = SMVector3Sum(SMVectorMin(vTmpMin, vTmpMax)) + plane.m_fDistance;
			isVisibleStrict &= d > 0.0f;
		}
	}

	//bool b0 = BoxInFrustum1(m_aFrustumPlanes, vMin, vMax);

	//assert(isVisible == b0);

	if(pIsStrict)
	{
		*pIsStrict = isVisible && isVisibleStrict;
	}

	return(isVisible);
}

bool CFrustum::frustumInFrustum(const IXFrustum *pOther) const
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
				float4(-m_aFrustumPlanes[p0].m_fDistance, m_aFrustumPlanes[p0].m_vNormal.y, m_aFrustumPlanes[p0].m_vNormal.z, 0.0f),
				float4(-m_aFrustumPlanes[p1].m_fDistance, m_aFrustumPlanes[p1].m_vNormal.y, m_aFrustumPlanes[p1].m_vNormal.z, 0.0f),
				float4(-m_aFrustumPlanes[p2].m_fDistance, m_aFrustumPlanes[p2].m_vNormal.y, m_aFrustumPlanes[p2].m_vNormal.z, 0.0f),
				float4(0.0f, 0.0f, 0.0f, 1.0f)
				));
			vPoint.y = SMMatrix3x3Determinant(SMMATRIX(
				float4(m_aFrustumPlanes[p0].m_vNormal.x, -m_aFrustumPlanes[p0].m_fDistance, m_aFrustumPlanes[p0].m_vNormal.z, 0.0f),
				float4(m_aFrustumPlanes[p1].m_vNormal.x, -m_aFrustumPlanes[p1].m_fDistance, m_aFrustumPlanes[p1].m_vNormal.z, 0.0f),
				float4(m_aFrustumPlanes[p2].m_vNormal.x, -m_aFrustumPlanes[p2].m_fDistance, m_aFrustumPlanes[p2].m_vNormal.z, 0.0f),
				float4(0.0f, 0.0f, 0.0f, 1.0f)
				));
			vPoint.z = SMMatrix3x3Determinant(SMMATRIX(
				float4(m_aFrustumPlanes[p0].m_vNormal.x, m_aFrustumPlanes[p0].m_vNormal.y, -m_aFrustumPlanes[p0].m_fDistance, 0.0f),
				float4(m_aFrustumPlanes[p1].m_vNormal.x, m_aFrustumPlanes[p1].m_vNormal.y, -m_aFrustumPlanes[p1].m_fDistance, 0.0f),
				float4(m_aFrustumPlanes[p2].m_vNormal.x, m_aFrustumPlanes[p2].m_vNormal.y, -m_aFrustumPlanes[p2].m_fDistance, 0.0f),
				float4(0.0f, 0.0f, 0.0f, 1.0f)
				));

			vPoint /= fDet;

			m_isPointValid[iPoint] = true;
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

CCamera::CCamera():
	m_pFrustum(new CFrustum())
{}
CCamera::~CCamera()
{
	mem_release(m_pFrustum);
}

void CCamera::setOrientation(const SMQuaternion &q)
{
	m_qRotation = q;
	m_vPitchYawRoll = SMMatrixToEuler(q.GetMatrix());

	m_vRight = q * float3(1.0f, 0.0f, 0.0f);
	m_vUp = q * float3(0.0f, 1.0f, 0.0f);
	m_vLook = q * float3(0.0f, 0.0f, 1.0f);
}

const SMQuaternion& CCamera::getOrientation()
{
	return(m_qRotation);
}

const SMMATRIX& CCamera::getViewMatrix() const
{
	m_mView = SMMatrixLookToLH(m_vPosition, m_vLook, m_vUp);
	return(m_mView);
}


const float3& CCamera::getPosition() const
{
	return(m_vPosition);
}

void CCamera::setPosition(const float3 &vPos)
{
	m_vPosition = vPos;
}

const float3& CCamera::getRight() const
{
	return(m_vRight);
}

const float3& CCamera::getUp() const
{
	return(m_vUp);
}

const float3& CCamera::getLook() const
{
	return(m_vLook);
}

const float3& CCamera::getRotation() const
{
	return(m_vPitchYawRoll);
}

void CCamera::setFOV(float fov)
{
	m_fFOV = fov;
}

float CCamera::getFOV() const
{
	return(m_fFOV);
}

void CCamera::updateFrustum(const SMMATRIX &mProjection)
{
	m_pFrustum->update(getViewMatrix(), mProjection);
}

IXFrustum* CCamera::getFrustum()
{
	m_pFrustum->AddRef();
	return(m_pFrustum);
}
