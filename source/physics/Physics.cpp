#include "Physics.h"
#include "CollisionShape.h"
#include "CollisionObject.h"
#include "sxphysics.h"

IBoxShape* XMETHODCALLTYPE CPhysics::newBoxShape(const float3 &vHalfExtents)
{
	return(new CBoxShape(vHalfExtents));
}
ISphereShape* XMETHODCALLTYPE CPhysics::newSphereShape(float fRadius)
{
	return(new CSphereShape(fRadius));
}
ICapsuleShape* XMETHODCALLTYPE CPhysics::newCapsuleShape(float fRadius, float fHeight)
{
	return(new CCapsuleShape(fRadius, fHeight));
}
ICylinderShape* XMETHODCALLTYPE CPhysics::newCylinderShape(float fRadius, float fHeight)
{
	return(new CCylinderShape(fRadius, fHeight));
}
IStaticPlaneShape* XMETHODCALLTYPE CPhysics::newStaticPlaneShape(const SMPLANE &plane)
{
	return(new CStaticPlaneShape(plane));
}
IConvexHullShape* XMETHODCALLTYPE CPhysics::newConvexHullShape(UINT uPoints, const float3_t *pPoints, byte u8Stride, bool bOptimize)
{
	return(new CConvexHullShape(uPoints, pPoints, u8Stride, bOptimize));
}
ITrimeshShape* XMETHODCALLTYPE CPhysics::newTrimeshShape(UINT uVertices, const float3_t *pVertices, UINT uIndices, UINT *pIndices, byte u8Stride)
{
	return(new CTrimeshShape(uVertices, pVertices, uIndices, pIndices, u8Stride));
}
ICompoundShape* XMETHODCALLTYPE CPhysics::newCompoundShape(UINT uShapes)
{
	return(new CCompoundShape(uShapes));
}

IRigidBody* XMETHODCALLTYPE CPhysics::newRigidBody(const XRIDIGBODY_DESC *pDesc)
{
	return(new CRigidBody(pDesc));
}
IGhostObject* XMETHODCALLTYPE CPhysics::newGhostObject(bool isPairCaching)
{
	return(new CGhostObject(isPairCaching));
}

void XMETHODCALLTYPE CPhysics::addCollisionObject(ICollisionObject *pCollisionObject, int iCollisionGroup, int iCollisionMask)
{
	assert(pCollisionObject);

	add_ref(pCollisionObject);

	switch(pCollisionObject->getType())
	{
	case XCOT_INVALID:
		assert(!"Invalid type!");
		break;
	case XCOT_RIGID_BODY:
		{
			CRigidBody *pBody = (CRigidBody*)pCollisionObject->asRigidBody();
			pBody->setPhysWorld(this);
			SPhysics_GetDynWorld()->addRigidBody(pBody->getBtRigidBody(), iCollisionGroup, iCollisionMask);
		}
		break;
	case XCOT_GHOST_OBJECT:
		{
			CGhostObject *pGhost = (CGhostObject*)pCollisionObject->asGhostObject();
			pGhost->setPhysWorld(this);
			SPhysics_GetDynWorld()->addCollisionObject(pGhost->getBtGhostObject(), iCollisionGroup, iCollisionMask);
		}
		break;
	default:
		assert(!"Unknown type!");
	}
}
void XMETHODCALLTYPE CPhysics::removeCollisionObject(ICollisionObject *pCollisionObject)
{
	assert(pCollisionObject);

	switch(pCollisionObject->getType())
	{
	case XCOT_INVALID:
		assert(!"Invalid type!");
		break;
	case XCOT_RIGID_BODY:
		{
			CRigidBody *pBody = (CRigidBody*)pCollisionObject->asRigidBody();
			assert(pBody->getPhysWorld());
			if(pBody->getPhysWorld())
			{
				pBody->setPhysWorld(NULL);
				SPhysics_GetDynWorld()->removeRigidBody(pBody->getBtRigidBody());
			}
		}
		break;
	case XCOT_GHOST_OBJECT:
		{
			CGhostObject *pGhost = (CGhostObject*)pCollisionObject->asGhostObject();
			assert(pGhost->getPhysWorld());
			if(pGhost->getPhysWorld())
			{
				pGhost->setPhysWorld(NULL);
				SPhysics_GetDynWorld()->removeCollisionObject(pGhost->getBtGhostObject());
			}
		}
		break;
	default:
		assert(!"Unknown type!");
	}
	mem_release(pCollisionObject);
}

