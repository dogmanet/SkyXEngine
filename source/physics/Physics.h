#ifndef __XPHYSICS_H
#define __XPHYSICS_H

#include "IXPhysics.h"

class CPhysics: public IXUnknownImplementation<IXPhysics>
{
public:
	IBoxShape* XMETHODCALLTYPE newBoxShape(const float3 &vHalfExtents) override;
	ISphereShape* XMETHODCALLTYPE newSphereShape(float fRadius) override;
	ICapsuleShape* XMETHODCALLTYPE newCapsuleShape(float fRadius, float fHeight) override;
	ICylinderShape* XMETHODCALLTYPE newCylinderShape(float fRadius, float fHeight) override;
	IStaticPlaneShape* XMETHODCALLTYPE newStaticPlaneShape(const SMPLANE &plane) override;
	IConvexHullShape* XMETHODCALLTYPE newConvexHullShape(UINT uPoints, const float3_t *pPoints, byte u8Stride = sizeof(float3_t), bool bOptimize = true) override;
	ITrimeshShape* XMETHODCALLTYPE newTrimeshShape(UINT uVertices, const float3_t *pVertices, UINT uIndices, UINT *pIndices, byte u8Stride = sizeof(float3_t)) override;
	ICompoundShape* XMETHODCALLTYPE newCompoundShape(UINT uShapes = 0) override;
	
	IRigidBody* XMETHODCALLTYPE newRigidBody(const XRIDIGBODY_DESC *pDesc) override;
	IGhostObject* XMETHODCALLTYPE newGhostObject(bool isPairCaching = true) override;

	void XMETHODCALLTYPE addCollisionObject(ICollisionObject *pCollisionObject, int iCollisionGroup = CG_DEFAULT, int iCollisionMask = CG_ALL) override;
	void XMETHODCALLTYPE removeCollisionObject(ICollisionObject *pCollisionObject) override;
};

#endif
