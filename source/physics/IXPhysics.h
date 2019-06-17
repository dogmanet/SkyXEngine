#ifndef __IXPHYSICS_H
#define __IXPHYSICS_H

#include "ICollisionShape.h"
#include "ICollisionObject.h"
/*
typedef enum PHY_ScalarType
{
	PHY_FLOAT,
	PHY_DOUBLE,
	PHY_INTEGER,
	PHY_SHORT,
	PHY_FIXEDPOINT88,
	PHY_UCHAR
} PHY_ScalarType;
*/
class IXPhysics: public IXUnknown
{
public:
	virtual IBoxShape* XMETHODCALLTYPE newBoxShape(const float3 &vHalfExtents) = 0;
	virtual ISphereShape* XMETHODCALLTYPE newSphereShape(float fRadius) = 0;
	virtual ICapsuleShape* XMETHODCALLTYPE newCapsuleShape(float fRadius, float fHeight) = 0;
	virtual IStaticPlaneShape* XMETHODCALLTYPE newStaticPlaneShape(const SMPLANE &plane) = 0;
	/*virtual ITerrainShape* XMETHODCALLTYPE newTerrainShape(int heightStickWidth, int heightStickLength,
		const void* heightfieldData, float heightScale,
		float minHeight, float maxHeight,
		char upAxis / * x/y/z * /, PHY_ScalarType heightDataType,
		bool flipQuadEdges) = 0;*/
	virtual IConvexHullShape* XMETHODCALLTYPE newConvexHullShape(UINT uPoints, const float3_t *pPoints, byte u8Stride = sizeof(float3_t), bool bOptimize = true) = 0;
	virtual ITrimeshShape* XMETHODCALLTYPE newTrimeshShape(UINT uVertices, const float3_t *pVertices, UINT uIndices, UINT *pIndices, byte u8Stride = sizeof(float3_t)) = 0;
	virtual ICompoundShape* XMETHODCALLTYPE newCompoundShape(UINT uShapes = 0) = 0;
	
	virtual IRigidBody* XMETHODCALLTYPE newRigidBody(const XRIDIGBODY_DESC *pDesc) = 0;
	virtual IGhostObject* XMETHODCALLTYPE newGhostObject() = 0;
	virtual IGhostObjectPairCaching* XMETHODCALLTYPE newGhostObjectPairCaching() = 0;

	virtual void XMETHODCALLTYPE addCollisionObject(ICollisionObject *pCollisionObject) = 0;
	virtual void XMETHODCALLTYPE removeCollisionObject(ICollisionObject *pCollisionObject) = 0;

	// add/remove action
	// rayTest
	// convexSweepTest
	// add/remove constraint
};

#endif
