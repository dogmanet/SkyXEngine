#ifndef __COLLISIONOBJECT_H
#define __COLLISIONOBJECT_H

#include "IXCollisionObject.h"

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/BroadphaseCollision/btCollisionAlgorithm.h>

#include "Physics.h"
#include "PhyWorld.h"
#include "CollisionShape.h"

template<class T>
class CCollisionObject: public IXUnknownImplementation<T>
{
public:
	typedef CCollisionObject<T> BaseClass;

	CCollisionObject<T>(XCOLLISION_OBJECT_TYPE type)
	{
		m_type = type;
	}

	~CCollisionObject<T>()
	{
		btCollisionShape *pBtShape = m_pCollisionObject->getCollisionShape();
		if(pBtShape)
		{
			((IXCollisionShape*)pBtShape->getUserPointer())->Release();
		}
		mem_delete(m_pCollisionObject);
	}

	void setCollisionObject(btCollisionObject *pObject)
	{
		m_pCollisionObject = pObject;
		m_pCollisionObject->setUserPointer((IXCollisionObject*)this);
		m_pCollisionObject->setUserIndex(2);
	}

	btCollisionObject* getBtCollisionObject()
	{
		return(m_pCollisionObject);
	}

	XCOLLISION_OBJECT_TYPE XMETHODCALLTYPE getType() const override
	{
		return(m_type);
	}

	void XMETHODCALLTYPE setCollisionShape(IXCollisionShape *pCollisionShape) override
	{
		add_ref(pCollisionShape);

		btCollisionShape *pBtShape = m_pCollisionObject->getCollisionShape();
		if(pBtShape)
		{
			((IXCollisionShape*)pBtShape->getUserPointer())->Release();
		}

		pBtShape = GetCollisionShape(pCollisionShape);

		m_pCollisionObject->setCollisionShape(pBtShape);
	}
	IXCollisionShape* XMETHODCALLTYPE getCollisionShape() override
	{
		btCollisionShape *pBtShape = m_pCollisionObject->getCollisionShape();
		if(!pBtShape)
		{
			return(NULL);
		}

		IXCollisionShape *pShape = (IXCollisionShape*)pBtShape->getUserPointer();
		add_ref(pShape);
		return(pShape);
	}

	void XMETHODCALLTYPE setFriction(float fFriction) override
	{
		m_pCollisionObject->setFriction(fFriction);
	}
	float XMETHODCALLTYPE getFriction() const override
	{
		return(m_pCollisionObject->getFriction());
	}

	void XMETHODCALLTYPE setRollingFriction(float fFriction) override
	{
		m_pCollisionObject->setRollingFriction(fFriction);
	}
	float XMETHODCALLTYPE getRollingFriction() const override
	{
		return(m_pCollisionObject->getRollingFriction());
	}

	void XMETHODCALLTYPE setSpinningFriction(float fFriction) override
	{
		m_pCollisionObject->setSpinningFriction(fFriction);
	}
	float XMETHODCALLTYPE getSpinningFriction() const override
	{
		return(m_pCollisionObject->getSpinningFriction());
	}

	void XMETHODCALLTYPE setPosition(const float3 &vPosition) override
	{
		m_pCollisionObject->getWorldTransform().setOrigin(F3_BTVEC(vPosition));
		SAFE_CALL(m_pPhysWorld, updateSingleAABB, this);
	}
	float3 XMETHODCALLTYPE getPosition() const override
	{
		btVector3 &v = m_pCollisionObject->getWorldTransform().getOrigin();
		return(BTVEC_F3(v));
	}

	void XMETHODCALLTYPE setRotation(const SMQuaternion &qRotation) override
	{
		m_pCollisionObject->getWorldTransform().setRotation(Q4_BTQUAT(qRotation));
		SAFE_CALL(m_pPhysWorld, updateSingleAABB, this);
	}
	SMQuaternion XMETHODCALLTYPE getRotation() const override
	{
		btQuaternion &q = m_pCollisionObject->getWorldTransform().getRotation();
		return(BTQUAT_Q4(q));
	}

	void XMETHODCALLTYPE setCollisionFlags(XCOLLISION_FLAGS flags) override
	{
		int f = 0;
		if(flags & XCF_STATIC_OBJECT)
		{
			f |= btCollisionObject::CF_STATIC_OBJECT;
		}
		if(flags & XCF_KINEMATIC_OBJECT)
		{
			f |= btCollisionObject::CF_KINEMATIC_OBJECT;
		}
		if(flags & XCF_NO_CONTACT_RESPONSE)
		{
			f |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
		}
		if(flags & XCF_CHARACTER_OBJECT)
		{
			f |= btCollisionObject::CF_CHARACTER_OBJECT;
		}
		if(flags & XCF_DISABLE_VISUALIZE_OBJECT)
		{
			f |= btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT;
		}
		m_pCollisionObject->setCollisionFlags(f);
	}
	XCOLLISION_FLAGS XMETHODCALLTYPE getCollisionFlags() const override
	{
		int flags = m_pCollisionObject->getCollisionFlags();
		XCOLLISION_FLAGS f = (XCOLLISION_FLAGS)0;
		if(flags & btCollisionObject::CF_STATIC_OBJECT)
		{
			f |= XCF_STATIC_OBJECT;
		}
		if(flags & btCollisionObject::CF_KINEMATIC_OBJECT)
		{
			f |= XCF_KINEMATIC_OBJECT;
		}
		if(flags & btCollisionObject::CF_NO_CONTACT_RESPONSE)
		{
			f |= XCF_NO_CONTACT_RESPONSE;
		}
		if(flags & btCollisionObject::CF_CHARACTER_OBJECT)
		{
			f |= XCF_CHARACTER_OBJECT;
		}
		if(flags & btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT)
		{
			f |= XCF_DISABLE_VISUALIZE_OBJECT;
		}
		return(f);
	}

	bool XMETHODCALLTYPE hasContactResponse() const override
	{
		return(m_pCollisionObject->hasContactResponse());
	}

	bool XMETHODCALLTYPE isStaticOrKinematic() const override
	{
		return(m_pCollisionObject->isStaticOrKinematicObject());
	}

	void XMETHODCALLTYPE setUserPointer(void *pUser) override
	{
		m_pUser = pUser;
	}
	void* XMETHODCALLTYPE getUserPointer() const override
	{
		return(m_pUser);
	}

	void XMETHODCALLTYPE setUserTypeId(int iUser) override
	{
		m_iUserTypeId = iUser;
	}
	int XMETHODCALLTYPE getUserTypeId() const override
	{
		return(m_iUserTypeId);
	}

	void XMETHODCALLTYPE setUserIndex(int iUser) override
	{
		m_iUserIndex = iUser;
	}
	int XMETHODCALLTYPE getUserIndex() const override
	{
		return(m_iUserIndex);
	}

	void XMETHODCALLTYPE setCCDsweptSphereRadius(float fRadius) override
	{
		m_pCollisionObject->setCcdSweptSphereRadius(fRadius);
	}
	float XMETHODCALLTYPE getCCDsweptSphereRadius() const override
	{
		return(m_pCollisionObject->getCcdSweptSphereRadius());
	}

	void XMETHODCALLTYPE setCCDmotionThreshold(float fRadius) override
	{
		m_pCollisionObject->setCcdMotionThreshold(fRadius);
	}
	float XMETHODCALLTYPE getCCDmotionThreshold() const override
	{
		return(m_pCollisionObject->getCcdMotionThreshold());
	}

	SMAABB XMETHODCALLTYPE getAABB() const override
	{
		btVector3 bvMin, bvMax;
		m_pCollisionObject->getCollisionShape()->getAabb(m_pCollisionObject->getWorldTransform(), bvMin, bvMax);
		return(SMAABB(BTVEC_F3(bvMin), BTVEC_F3(bvMax)));
	}

	COLLISION_GROUP XMETHODCALLTYPE getFilterGroup() const override
	{
		btBroadphaseProxy *pProxy = m_pCollisionObject->getBroadphaseHandle();
		if(pProxy)
		{
			return((COLLISION_GROUP)pProxy->m_collisionFilterGroup);
		}
		return(CG_NONE);
	}
	COLLISION_GROUP XMETHODCALLTYPE getFilterMask() const override
	{
		btBroadphaseProxy *pProxy = m_pCollisionObject->getBroadphaseHandle();
		if(pProxy)
		{
			return((COLLISION_GROUP)pProxy->m_collisionFilterMask);
		}
		return(CG_NONE);
	}

	IXRigidBody* XMETHODCALLTYPE asRigidBody() override
	{
		return(NULL);
	}
	IXGhostObject* XMETHODCALLTYPE asGhostObject() override
	{
		return(NULL);
	}

	void setPhysWorld(CPhyWorld *pPhyWorld)
	{
		m_pPhysWorld = pPhyWorld;
	}

	CPhyWorld* getPhysWorld() const
	{
		return(m_pPhysWorld);
	}

private:
	btCollisionObject *m_pCollisionObject = NULL;
	void *m_pUser = NULL;
	CPhyWorld *m_pPhysWorld = NULL;
	XCOLLISION_OBJECT_TYPE m_type;
	int m_iUserTypeId = -1;
	int m_iUserIndex = -1;
};

//#############################################################################

class CRigidBody final: public CCollisionObject<IXRigidBody>
{
public:
	CRigidBody(const XRIDIGBODY_DESC &desc);

	void XMETHODCALLTYPE setDamping(float fLinearDamping, float fAngularDamping) override;
	float XMETHODCALLTYPE getLinearDamping() const override;
	float XMETHODCALLTYPE getAngularDamping() const override;

	float XMETHODCALLTYPE getLinearSleepingThreshold() const override;
	float XMETHODCALLTYPE getAngularSleepingThreshold() const override;

	void XMETHODCALLTYPE setMass(float fMass, const float3 &vLocalInertia) override;

	float3 XMETHODCALLTYPE getLinearFactor() const override;
	void XMETHODCALLTYPE setLinearFactor(const float3 &vLinearFactor) override;
	float3 XMETHODCALLTYPE getAngularFactor() const override;
	void XMETHODCALLTYPE setAngularFactor(const float3 &vAngularFactor) override;

	void XMETHODCALLTYPE applyCentralForce(const float3 &vForce) override;

	void XMETHODCALLTYPE applyForce(const float3 &vForce, const float3 &vWorldPos) override;
	void XMETHODCALLTYPE applyTorque(const float3 &vTorque) override;

	void XMETHODCALLTYPE applyCentralImpulse(const float3 &vForce) override;
	void XMETHODCALLTYPE applyImpulse(const float3 &vForce, const float3 &vWorldPos) override;
	void XMETHODCALLTYPE applyTorqueImpulse(const float3 &vTorque) override;

	void XMETHODCALLTYPE setLinearVelocity(const float3 &vLinearVelocity) override;
	void XMETHODCALLTYPE setAngularVelocity(const float3 &vAngularVelocity) override;

	btRigidBody* getBtRigidBody();

	IXRigidBody* XMETHODCALLTYPE asRigidBody() override
	{
		return(this);
	}

private:
	btRigidBody *m_pRigidBody = NULL;

	struct CMotionState: public btMotionState
	{
	private:
		IXRigidBodyMotionCallback *m_pCallback;
		btTransform m_xform;

	public:
		CMotionState(IXRigidBodyMotionCallback *pCallback, const float3_t &v, const SMQuaternion &q):
			m_pCallback(pCallback),
			m_xform(btTransform(Q4_BTQUAT(q), F3_BTVEC(v)))
		{
		}

		///synchronizes world transform from user to physics
		void getWorldTransform(btTransform &centerOfMassWorldTrans) const override
		{
			centerOfMassWorldTrans = m_xform;
		}

		///synchronizes world transform from physics to user
		///Bullet only calls the update of worldtransform for active objects
		void setWorldTransform(const btTransform& centerOfMassWorldTrans) override
		{
			if(m_pCallback)
			{
				const btVector3 &v = centerOfMassWorldTrans.getOrigin();
				const btQuaternion &q = centerOfMassWorldTrans.getRotation();
				m_pCallback->setWorldTransform(BTVEC_F3(v), BTQUAT_Q4(q));
			}
		}
	};

	CMotionState m_motionState;
};

//#############################################################################

class CContactManifoldPoint: public IXContactManifoldPoint
{
public:
	float XMETHODCALLTYPE getDistance() const override;

	float3 XMETHODCALLTYPE getLocalPointA() const override;
	float3 XMETHODCALLTYPE getLocalPointB() const override;

	float3 XMETHODCALLTYPE getWorldPointA() const override;
	float3 XMETHODCALLTYPE getWorldPointB() const override;

	float3 XMETHODCALLTYPE getWorldNormalB() const override;

	void setBtPoint(btManifoldPoint *pPoint);
protected:

	btManifoldPoint *m_pBtPoint = NULL;
};

//#############################################################################

class CContactManifold: public IXContactManifold
{
public:
	UINT XMETHODCALLTYPE getContactCount() const override;
	IXContactManifoldPoint* XMETHODCALLTYPE getContact(UINT uIndex) const override;

	void setBtManifold(btPersistentManifold *pManifold);

protected:
	btPersistentManifold *m_pManifold = NULL;
	mutable CContactManifoldPoint m_tempPoint;
};

//#############################################################################

class CCollisionPair: public IXCollisionPair
{
public:
	IXCollisionObject* XMETHODCALLTYPE getObject0() override;
	IXCollisionObject* XMETHODCALLTYPE getObject1() override;

	UINT XMETHODCALLTYPE getContactManifoldCount() const override;
	IXContactManifold* XMETHODCALLTYPE getContactManifold(UINT uIndex) const override;

	void setBtPair(btBroadphasePair *pPair);
protected:
	btBroadphasePair *m_pPair = NULL;
	mutable btManifoldArray m_manifoldArray;
	mutable CContactManifold m_contactManifold;
};

//#############################################################################

class CGhostObject: public CCollisionObject<IXGhostObject>
{
public:
	CGhostObject(bool isPairCaching);
	~CGhostObject();

	UINT XMETHODCALLTYPE getOverlappingObjectCount() const override;
	IXCollisionObject* XMETHODCALLTYPE getOverlappingObject(UINT uIndex) const override;

	UINT XMETHODCALLTYPE getOverlappingPairCount() const override;
	IXCollisionPair* XMETHODCALLTYPE getOverlappingPair(UINT uIndex) const override;

	btGhostObject* getBtGhostObject();


	IXGhostObject* XMETHODCALLTYPE asGhostObject() override
	{
		return(this);
	}
protected:
	btGhostObject *m_pGhostObject = NULL;
	bool m_isPairCaching;
	mutable CCollisionPair m_collisionPair;
};

#endif
