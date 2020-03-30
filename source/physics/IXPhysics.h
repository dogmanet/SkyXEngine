#ifndef __IXPHYSICS_H
#define __IXPHYSICS_H

#include "ICollisionShape.h"
#include "ICollisionObject.h"


#define BIT(n) (1 << (n))
enum COLLISION_GROUP
{
	CG_NONE = 0,
	// BEGIN --- Do not change ---
	CG_DEFAULT = BIT(0),
	CG_STATIC = BIT(1),
	CG_KINEMATIC = BIT(2),
	CG_DEBRIS = BIT(3),
	CG_TRIGGER = BIT(4),
	CG_CHARACTER = BIT(5),
	// END --- Do not change ---

	CG_WATER = BIT(6),
	CG_HITBOX = BIT(7),
	CG_BULLETFIRE = BIT(8),
	CG_NPCVIEW = BIT(9),
	CG_DOOR = BIT(10),

	CG_ALL = 0xFFFFFFFF
};

#define CG_STATIC_MASK (CG_ALL ^ (CG_DOOR | CG_HITBOX | CG_STATIC | CG_TRIGGER | CG_WATER))

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
	virtual ICylinderShape* XMETHODCALLTYPE newCylinderShape(float fRadius, float fHeight) = 0;
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
	virtual IGhostObject* XMETHODCALLTYPE newGhostObject(bool isPairCaching = true) = 0;

	virtual void XMETHODCALLTYPE addCollisionObject(ICollisionObject *pCollisionObject, int iCollisionGroup = CG_DEFAULT, int iCollisionMask = CG_ALL) = 0;
	virtual void XMETHODCALLTYPE removeCollisionObject(ICollisionObject *pCollisionObject) = 0;

	// add/remove action
	// rayTest
	// convexSweepTest
	// add/remove constraint
};

#endif
