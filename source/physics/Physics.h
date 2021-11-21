#ifndef __XPHYSICS_H
#define __XPHYSICS_H

#include "IXPhysics.h"
//#include "CollisionObject.h"

class CPhyWorld;
class CPhysics: public IXUnknownImplementation<IXPhysics>
{
public:
	CPhysics(CPhyWorld *pDefaultWorld);

	void XMETHODCALLTYPE newBoxShape(const float3 &vHalfExtents, IXBoxShape **ppOut) override;
	void XMETHODCALLTYPE newSphereShape(float fRadius, IXSphereShape **ppOut) override;
	void XMETHODCALLTYPE newCapsuleShape(float fRadius, float fHeight, IXCapsuleShape **ppOut) override;
	void XMETHODCALLTYPE newCylinderShape(float fRadius, float fHeight, IXCylinderShape **ppOut) override;
	void XMETHODCALLTYPE newStaticPlaneShape(const SMPLANE &plane, IXStaticPlaneShape **ppOut) override;
	void XMETHODCALLTYPE newConvexHullShape(UINT uPoints, const float3_t *pPoints, IXConvexHullShape **ppOut, byte u8Stride = sizeof(float3_t), bool bOptimize = true) override;
	void XMETHODCALLTYPE newTrimeshShape(UINT uVertices, const float3_t *pVertices, UINT uIndices, UINT *pIndices, IXTrimeshShape **ppOut, byte u8Stride = sizeof(float3_t)) override;
	void XMETHODCALLTYPE newCompoundShape(IXCompoundShape **ppOut, UINT uShapes = 0) override;
	
	void XMETHODCALLTYPE newRigidBody(const XRIDIGBODY_DESC &pDesc, IXRigidBody **ppOut) override;
	void XMETHODCALLTYPE newGhostObject(IXGhostObject **ppOut, bool isPairCaching = true) override;

	void XMETHODCALLTYPE newCharacterController(IXGhostObject *pGhostObject, float fStepHeight, IXCharacterController **ppOut) override;

	IXPhysicsWorld* XMETHODCALLTYPE getWorld(void *pReserved = NULL) override;

private:
	CPhyWorld *m_pDefaultWorld;
};

#endif
