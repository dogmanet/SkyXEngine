#include "OcclusionCuller.h"
//#include <physics/sxphysics.h>

#include <gcore/sxgcore.h>

/*struct CAllHitsResultCallback: public btCollisionWorld::RayResultCallback
{
	btAlignedObjectArray<const btCollisionObject*>		m_collisionObjects;

	float3	m_rayToWorld;

	btAlignedObjectArray<btScalar> m_hitFractions;

	btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override
	{
		m_collisionObject = rayResult.m_collisionObject;

		int idx = m_collisionObjects.findLinearSearch2(m_collisionObject);
		if(idx >= 0)
		{
			m_hitFractions[idx] = rayResult.m_hitFraction;
		}
		else
		{
			m_collisionObjects.push_back(rayResult.m_collisionObject);
			m_hitFractions.push_back(rayResult.m_hitFraction);
		}
		return m_closestHitFraction;
	}
};*/

//##########################################################################

COcclusionCuller::COcclusionCuller()
{
	m_pLightSystem = (IXLightSystem*)Core_GetIXCore()->getPluginManager()->getInterface(IXLIGHTSYSTEM_GUID);

	IXPhysics *pPhysics = (IXPhysics*)Core_GetIXCore()->getPluginManager()->getInterface(IXPHYSICS_GUID);
	m_pPhysWorld = pPhysics->getWorld();

	Core_0RegisterConcmdCls("oprint", this, (SXCONCMDCLS)&COcclusionCuller::_print);
}
COcclusionCuller::~COcclusionCuller()
{
	//! @todo uncomment me!
	// mem_release(m_pCamera);
}

void XMETHODCALLTYPE COcclusionCuller::update(ICamera *pCamera, float fFOV, float fAspectRatio, float fFarPlane)
{
	assert(pCamera);

	//! @todo uncomment me!
	// mem_release(m_pCamera);

	m_pCamera = pCamera;

	//! @todo uncomment me!
	// m_pCamera->AddRef();

	m_fFOV = fFOV;
	m_fAspectRatio = fAspectRatio;
	m_fFarPlane = fFarPlane;

	if(m_fFarPlane > 100.0f)
	{
		m_fFarPlane = 100.0f;
	}

	_update();
}

bool XMETHODCALLTYPE COcclusionCuller::isPointVisible(const float3 &vPoint) const
{
	return(classifyPoint(vPoint) >= 0);
}

//bool XMETHODCALLTYPE COcclusionCuller::isPolyVisible(const float3 &vPoint1, const float3 &vPoint2, const float3 &vPoint3) const
//{
//	return(true);
//}
//
//bool XMETHODCALLTYPE COcclusionCuller::isSphereVisible(const float3 &vOrigin, float fRadius) const
//{
//	return(true);
//}

bool XMETHODCALLTYPE COcclusionCuller::isAABBvisible(const SMAABB &aabb, bool *pisFullyVisible) const
{
	return(true);
}

void COcclusionCuller::_update()
{
	for(UINT x = 0; x < OCCLUSION_BUFFER_WIDTH; ++x)
	{
		for(UINT y = 0; y < OCCLUSION_BUFFER_HEIGHT; ++y)
		{
			m_ppfOcclusionBuffer[x][y] = FLT_MAX;
		}
	}

	float3 vStart = m_pCamera->getPosition();
	float3 vDir = SMVector3Normalize(m_pCamera->getLook());
	float3 vRight = m_pCamera->getRight();
	float3 vUp = m_pCamera->getUp();

	float3 vCenter = vStart + vDir * m_fFarPlane;
	float fHalfHeight = tanf(m_fFOV * 0.5f) * m_fFarPlane;
	float fHalfWidth = fHalfHeight * m_fAspectRatio;

//	float3 vUpRight = vCenter + vUp * fHalfHeight + vRight * fHalfWidth;
//	float3 vUpLeft = vCenter + vUp * fHalfHeight - vRight * fHalfWidth;
//	float3 vDnRight = vCenter - vUp * fHalfHeight + vRight * fHalfWidth;
//	float3 vDnLeft = vCenter - vUp * fHalfHeight - vRight * fHalfWidth;
//
//	float3 vEnd = vDnRight;
//	
//	btCollisionWorld::AllHitsRayResultCallback cb(F3_BTVEC(vStart), F3_BTVEC(vEnd));
//	SPhysics_GetDynWorld()->rayTest(F3_BTVEC(vStart), F3_BTVEC(vEnd), cb);

	
	for(UINT x = 0; x <= OCCLUSION_BUFFER_WIDTH; ++x)
	{
		float3 vTmp = vCenter + vRight * lerpf(-fHalfWidth, fHalfWidth, (float)x / OCCLUSION_BUFFER_WIDTH);
		for(UINT y = 0; y <= OCCLUSION_BUFFER_HEIGHT; ++y)
		{
			CAllHitsRayResultCallback &cb = cbs[x][y];
			cb.m_aResults.clearFast();
			cb.m_rayToWorld = vTmp + vUp * lerpf(fHalfHeight, -fHalfHeight, (float)y / OCCLUSION_BUFFER_HEIGHT);

			m_pPhysWorld->rayTest(vStart, cb.m_rayToWorld, &cb);

			if(x != OCCLUSION_BUFFER_WIDTH && y != OCCLUSION_BUFFER_HEIGHT)
			{
				m_ppPolyhedra[x][y].isValid = false;
			}

			if(x != OCCLUSION_BUFFER_WIDTH)
			{
				m_ppPolyhedra[x][y].planes[PLANE_LEFT] = SMPLANE(vStart, cb.m_rayToWorld, cb.m_rayToWorld + vUp);
			}
			if(x != 0)
			{
				m_ppPolyhedra[x - 1][y].planes[PLANE_RIGHT] = -m_ppPolyhedra[x][y].planes[PLANE_LEFT];
			}

			if(x != OCCLUSION_BUFFER_HEIGHT)
			{
				m_ppPolyhedra[x][y].planes[PLANE_TOP] = SMPLANE(vStart, cb.m_rayToWorld, cb.m_rayToWorld + vRight);
			}
			if(y != 0)
			{
				m_ppPolyhedra[x][y - 1].planes[PLANE_BOTTOM] = -m_ppPolyhedra[x][y].planes[PLANE_TOP];
			}

#if 0
			if(m_pLightSystem && false)
			{
				UINT uLightKey = x * OCCLUSION_BUFFER_HEIGHT + y;
				if(m_apTemp.size() <= uLightKey)
				{
					m_apTemp[uLightKey] = m_pLightSystem->newPoint();
					m_apTemp[uLightKey]->setColor(float3(1.0f, 0.0f, 0.0f));
				}

				if(cb.hasHit())
				{
					float fHitFraction = FLT_MAX;
					int idx = -1;
					for(UINT i = 0, l = cb.m_hitFractions.size(); i < l; ++i)
					{
						if(cb.m_hitFractions[i] < fHitFraction)
						{
							fHitFraction = cb.m_hitFractions[i];
							idx = (int)i;
						}
					}
					if(idx >= 0)
					{
						auto lp = BTVEC_F3(cb.m_hitPointWorld[idx]) + SMVector3Normalize(BTVEC_F3(cb.m_hitNormalWorld[idx])) * 0.1f;
						m_apTemp[uLightKey]->setPosition(lp);
						m_apTemp[uLightKey]->setEnabled(true);
					}
				}
				else
				{
					m_apTemp[uLightKey]->setEnabled(false);
				}
			}
#endif
		}
	}

	bool isFound = false;
	for(UINT x = 0; x < OCCLUSION_BUFFER_WIDTH; ++x)
	{
		for(UINT y = 0; y < OCCLUSION_BUFFER_HEIGHT; ++y)
		{
			CAllHitsRayResultCallback &cb0 = cbs[x][y];
			CAllHitsRayResultCallback &cb1 = cbs[x + 1][y];
			CAllHitsRayResultCallback &cb2 = cbs[x][y + 1];
			CAllHitsRayResultCallback &cb3 = cbs[x + 1][y + 1];
			if(!cb0.hasHit() || !cb1.hasHit() || !cb2.hasHit() || !cb3.hasHit())
			{
				continue;
			}

			fora(idx0, cb0.m_aResults)
			{
				auto &obj = cb0.m_aResults[idx0];

				int idx1 = cb1.m_aResults.indexOf(obj, [](const XRayResult &a, const XRayResult &b){
					return(a.pCollisionObject == b.pCollisionObject);
				});
				if(idx1 < 0)
				{
					continue;
				}

				int idx2 = cb2.m_aResults.indexOf(obj, [](const XRayResult &a, const XRayResult &b){
					return(a.pCollisionObject == b.pCollisionObject);
				});
				if(idx2 < 0)
				{
					continue;
				}

				int idx3 = cb3.m_aResults.indexOf(obj, [](const XRayResult &a, const XRayResult &b){
					return(a.pCollisionObject == b.pCollisionObject);
				});
				if(idx3 < 0)
				{
					continue;
				}

				// something found!
				
				float z[] = {
					fabs(SMVector3Dot(vDir, (cb0.m_rayToWorld - vStart) * cb0.m_aResults[idx0].fHitFraction)),
					fabs(SMVector3Dot(vDir, (cb1.m_rayToWorld - vStart) * cb1.m_aResults[idx1].fHitFraction)),
					fabs(SMVector3Dot(vDir, (cb2.m_rayToWorld - vStart) * cb2.m_aResults[idx2].fHitFraction)),
					fabs(SMVector3Dot(vDir, (cb3.m_rayToWorld - vStart) * cb3.m_aResults[idx3].fHitFraction))
				};

				int maxIdx = 0;
				float maxZ = z[maxIdx];
				for(int i = 1; i < 4; ++i)
				{
					if(maxZ < z[i])
					{
						maxZ = z[i];
						maxIdx = i;
					}
				}

				float3_t zPos[] = {
					cb0.m_aResults[idx0].vHitPoint,
					cb1.m_aResults[idx1].vHitPoint,
					cb2.m_aResults[idx2].vHitPoint,
					cb3.m_aResults[idx3].vHitPoint
				};

				if(m_ppfOcclusionBuffer[x][y] > maxZ)
				{
					m_ppfOcclusionBuffer[x][y] = maxZ;
					m_ppPolyhedra[x][y].isValid = true;
					m_ppPolyhedra[x][y].planes[PLANE_FRONT] = SMPLANE(zPos[maxIdx], zPos[maxIdx] + vUp, zPos[maxIdx] + vRight);
				}
			}
		}
	}
}

void COcclusionCuller::_print()
{
	for(UINT y = 0; y < OCCLUSION_BUFFER_HEIGHT; ++y)
	{
		for(UINT x = 0; x < OCCLUSION_BUFFER_WIDTH; ++x)
		{
			if(m_ppfOcclusionBuffer[x][y] > 99.0f)
			{
				printf("...");
			}
			else
			{
				int val = (int)(m_ppfOcclusionBuffer[x][y]);
				if(val > 99)
				{
					val = 99;
				}
				printf("%3d", val);
			}
		}
		printf("\n");
	}
	printf("\n");
}

int COcclusionCuller::classifyPoint(const float3 &vPoint, bool *pisCulledByFrustum) const
{
	IXFrustum *pFrustum = m_pCamera->getFrustum();

	bool isOnSurface = false;
	
	for(UINT i = 0, l = pFrustum->getPlaneCount(); i < l; ++i)
	{
		float f = SMVector4Dot(float4(vPoint, 1.0f), pFrustum->getPlaneAt(i));
		if(SMIsZero(f))
		{
			isOnSurface = true;
			continue;
		}
		if(f < 0.0f)
		{
			if(pisCulledByFrustum)
			{
				*pisCulledByFrustum = true;
			}
			return(-1);
		}
	}
	mem_release(pFrustum);
	if(pisCulledByFrustum)
	{
		*pisCulledByFrustum = false;
	}


	for(UINT x = 0; x < OCCLUSION_BUFFER_WIDTH; ++x)
	{
		for(UINT y = 0; y < OCCLUSION_BUFFER_HEIGHT; ++y)
		{
			auto &ph = m_ppPolyhedra[x][y];
			if(ph.isValid)
			{
				bool isInside = true;
				bool isOnSurfaceLocal = false;
				for(UINT i = 0; i < 5; ++i)
				{
					float f = SMVector4Dot(float4(vPoint, 1.0f), ph.planes[i]);
					if(SMIsZero(f))
					{
						isOnSurfaceLocal = true;
						continue;
					}
					if(f > 0.0f)
					{
						isInside = false;
						break;
					}
				}
				if(isInside)
				{
					if(isOnSurfaceLocal)
					{
						isOnSurface = true;
					}
					else
					{
						return(-1);
					}
				}
			}
		}
	}
	return(isOnSurface ? 0 : 1);
}
