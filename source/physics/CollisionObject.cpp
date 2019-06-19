#include "CollisionObject.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include "CollisionShape.h"
#include "sxphysics.h"

CRigidBody::CRigidBody(const XRIDIGBODY_DESC *pDesc):
	BaseClass(XCOT_RIGID_BODY)
{
	pDesc->pCollisionShape->AddRef();
	btRigidBody::btRigidBodyConstructionInfo info(pDesc->fMass, NULL, GetCollisionShape(pDesc->pCollisionShape), F3_BTVEC(pDesc->vLocalInertia));
	info.m_startWorldTransform.setOrigin(F3_BTVEC(pDesc->vStartWorldPosition));
	info.m_startWorldTransform.setRotation(Q4_BTQUAT(pDesc->qStartWorldRotation));

	info.m_linearDamping = pDesc->fLinearDamping;
	info.m_angularDamping = pDesc->fAngularDamping;

	info.m_friction = pDesc->fFriction;
	info.m_rollingFriction = pDesc->fRollingFriction;
	info.m_spinningFriction = pDesc->fSpinningFriction;
	info.m_linearSleepingThreshold = pDesc->fLinearSleepingThreshold;
	info.m_angularSleepingThreshold = pDesc->fAngularSleepingThreshold;

	m_pRigidBody = new btRigidBody(info);
	setCollisionObject(m_pRigidBody);
}

void XMETHODCALLTYPE CRigidBody::setDamping(float fLinearDamping, float fAngularDamping)
{
	m_pRigidBody->setDamping(fLinearDamping, fAngularDamping);
}
float XMETHODCALLTYPE CRigidBody::getLinearDamping() const
{
	return(m_pRigidBody->getLinearDamping());
}
float XMETHODCALLTYPE CRigidBody::getAngularDamping() const
{
	return(m_pRigidBody->getAngularDamping());
}

float XMETHODCALLTYPE CRigidBody::getLinearSleepingThreshold() const
{
	return(m_pRigidBody->getLinearSleepingThreshold());
}
float XMETHODCALLTYPE CRigidBody::getAngularSleepingThreshold() const
{
	return(m_pRigidBody->getAngularSleepingThreshold());
}

void XMETHODCALLTYPE CRigidBody::setMass(float fMass, const float3 &vLocalInertia)
{
	m_pRigidBody->setMassProps(fMass, F3_BTVEC(vLocalInertia));
}

float3 XMETHODCALLTYPE CRigidBody::getLinearFactor() const
{
	const btVector3 &v = m_pRigidBody->getLinearFactor();
	return(BTVEC_F3(v));
}
void XMETHODCALLTYPE CRigidBody::setLinearFactor(const float3 &vLinearFactor)
{
	m_pRigidBody->setLinearFactor(F3_BTVEC(vLinearFactor));
}
float3 XMETHODCALLTYPE CRigidBody::getAngularFactor() const
{
	const btVector3 &v = m_pRigidBody->getAngularFactor();
	return(BTVEC_F3(v));
}
void XMETHODCALLTYPE CRigidBody::setAngularFactor(const float3 &vAngularFactor)
{
	m_pRigidBody->setAngularFactor(F3_BTVEC(vAngularFactor));
}

void XMETHODCALLTYPE CRigidBody::applyCentralForce(const float3 &vForce)
{
	m_pRigidBody->applyCentralForce(F3_BTVEC(vForce));
}

void XMETHODCALLTYPE CRigidBody::applyForce(const float3 &vForce, const float3 &vWorldPos)
{
	const btVector3 &vCenterOfMass = m_pRigidBody->getCenterOfMassPosition();
	m_pRigidBody->applyForce(F3_BTVEC(vForce), F3_BTVEC(vWorldPos) - vCenterOfMass);
}
void XMETHODCALLTYPE CRigidBody::applyTorque(const float3 &vTorque)
{
	m_pRigidBody->applyTorque(F3_BTVEC(vTorque));
}

void XMETHODCALLTYPE CRigidBody::applyCentralImpulse(const float3 &vForce)
{
	m_pRigidBody->applyCentralImpulse(F3_BTVEC(vForce));
}
void XMETHODCALLTYPE CRigidBody::applyImpulse(const float3 &vForce, const float3 &vWorldPos)
{
	const btVector3 &vCenterOfMass = m_pRigidBody->getCenterOfMassPosition();
	m_pRigidBody->applyImpulse(F3_BTVEC(vForce), F3_BTVEC(vWorldPos) - vCenterOfMass);
}
void XMETHODCALLTYPE CRigidBody::applyTorqueImpulse(const float3 &vTorque)
{
	m_pRigidBody->applyTorqueImpulse(F3_BTVEC(vTorque));
}

void XMETHODCALLTYPE CRigidBody::setLinearVelocity(const float3 &vLinearVelocity)
{
	m_pRigidBody->setLinearVelocity(F3_BTVEC(vLinearVelocity));
}
void XMETHODCALLTYPE CRigidBody::setAngularVelocity(const float3 &vAngularVelocity)
{
	m_pRigidBody->setAngularVelocity(F3_BTVEC(vAngularVelocity));
}

btRigidBody* CRigidBody::getBtRigidBody()
{
	return(m_pRigidBody);
}

//#############################################################################

float XMETHODCALLTYPE CContactManifoldPoint::getDistance() const
{
	assert(m_pBtPoint);

	return(m_pBtPoint->getDistance());
}

float3 XMETHODCALLTYPE CContactManifoldPoint::getLocalPointA() const
{
	assert(m_pBtPoint);

	return(BTVEC_F3(m_pBtPoint->m_localPointA));
}
float3 XMETHODCALLTYPE CContactManifoldPoint::getLocalPointB() const
{
	assert(m_pBtPoint);

	return(BTVEC_F3(m_pBtPoint->m_localPointB));
}

float3 XMETHODCALLTYPE CContactManifoldPoint::getWorldPointA() const
{
	assert(m_pBtPoint);

	return(BTVEC_F3(m_pBtPoint->m_positionWorldOnA));
}
float3 XMETHODCALLTYPE CContactManifoldPoint::getWorldPointB() const
{
	assert(m_pBtPoint);

	return(BTVEC_F3(m_pBtPoint->m_positionWorldOnB));
}

float3 XMETHODCALLTYPE CContactManifoldPoint::getWorldNormalB() const
{
	assert(m_pBtPoint);

	return(BTVEC_F3(m_pBtPoint->m_normalWorldOnB));
}

void CContactManifoldPoint::setBtPoint(btManifoldPoint *pPoint)
{
	m_pBtPoint = pPoint;
}

//#############################################################################

UINT XMETHODCALLTYPE CContactManifold::getContactCount() const
{
	assert(m_pManifold);

	return(m_pManifold->getNumContacts());
}
IContactManifoldPoint* XMETHODCALLTYPE CContactManifold::getContact(UINT uIndex) const
{
	assert(uIndex < getContactCount());
	if(uIndex >= getContactCount())
	{
		return(NULL);
	}

	m_tempPoint.setBtPoint(&m_pManifold->getContactPoint(uIndex));

	return(&m_tempPoint);
}
void CContactManifold::setBtManifold(btPersistentManifold *pManifold)
{
	m_pManifold = pManifold;
}

//#############################################################################

ICollisionObject* XMETHODCALLTYPE CCollisionPair::getObject0()
{
	return((ICollisionObject*)((btCollisionObject*)m_pPair->m_pProxy0->m_clientObject)->getUserPointer());
}
ICollisionObject* XMETHODCALLTYPE CCollisionPair::getObject1()
{
	return((ICollisionObject*)((btCollisionObject*)m_pPair->m_pProxy1->m_clientObject)->getUserPointer());
}

UINT XMETHODCALLTYPE CCollisionPair::getContactManifoldCount() const
{
	btBroadphasePair *pCollisionPair = SPhysics_GetDynWorld()->getPairCache()->findPair(m_pPair->m_pProxy0, m_pPair->m_pProxy1);
	if(!pCollisionPair || !pCollisionPair->m_algorithm)
	{
		return(0);
	}
	m_manifoldArray.resize(0);
	pCollisionPair->m_algorithm->getAllContactManifolds(m_manifoldArray);

	return(m_manifoldArray.size());
}
IContactManifold* XMETHODCALLTYPE CCollisionPair::getContactManifold(UINT uIndex) const
{
	if((int)uIndex >= m_manifoldArray.size())
	{
		return(NULL);
	}

	m_contactManifold.setBtManifold(m_manifoldArray[uIndex]);

	return(&m_contactManifold);
}

void CCollisionPair::setBtPair(btBroadphasePair *pPair)
{
	m_pPair = pPair;
}

//#############################################################################

CGhostObject::CGhostObject(bool isPairCaching):
	BaseClass(XCOT_GHOST_OBJECT),
	m_isPairCaching(isPairCaching)
{
	if(m_isPairCaching)
	{
		m_pGhostObject = new btPairCachingGhostObject();
	}
	else
	{
		m_pGhostObject = new btGhostObject();
	}
	setCollisionObject(m_pGhostObject);
}

CGhostObject::~CGhostObject()
{
}

UINT XMETHODCALLTYPE CGhostObject::getOverlappingObjectCount() const
{
	return(m_pGhostObject->getNumOverlappingObjects());
}
ICollisionObject* XMETHODCALLTYPE CGhostObject::getOverlappingObject(UINT uIndex) const
{
	ICollisionObject *pObj = (ICollisionObject*)m_pGhostObject->getOverlappingObject(uIndex)->getUserPointer();
	pObj->AddRef();
	return(pObj);
}

UINT XMETHODCALLTYPE CGhostObject::getOverlappingPairCount() const
{
	if(!m_isPairCaching)
	{
		return(0);
	}

	btPairCachingGhostObject *pObj = (btPairCachingGhostObject*)m_pGhostObject;

	btBroadphasePairArray &pairArray = pObj->getOverlappingPairCache()->getOverlappingPairArray();
	return(pairArray.size());
}
ICollisionPair* XMETHODCALLTYPE CGhostObject::getOverlappingPair(UINT uIndex) const
{
	if(!m_isPairCaching)
	{
		return(NULL);
	}

	btPairCachingGhostObject *pObj = (btPairCachingGhostObject*)m_pGhostObject;
	btBroadphasePairArray &pairArray = pObj->getOverlappingPairCache()->getOverlappingPairArray();
	m_collisionPair.setBtPair(&pairArray[uIndex]);

	return(&m_collisionPair);
}

btGhostObject* CGhostObject::getBtGhostObject()
{
	return(m_pGhostObject);
}
