#ifndef __ICOLLISIONOBJECT_H
#define __ICOLLISIONOBJECT_H

#include "ICollisionShape.h"

enum XCOLLISION_FLAGS
{
	XCF_STATIC_OBJECT = 1,
	XCF_KINEMATIC_OBJECT = 2,
	XCF_NO_CONTACT_RESPONSE = 4,
	//XCF_CUSTOM_MATERIAL_CALLBACK = 8,//this allows per-triangle material (friction/restitution)
	XCF_CHARACTER_OBJECT = 16,
	XCF_DISABLE_VISUALIZE_OBJECT = 32 //disable debug drawing
};
DEFINE_ENUM_FLAG_OPERATORS(XCOLLISION_FLAGS);

enum XCOLLISION_OBJECT_TYPE
{
	XCOT_INVALID,
	XCOT_RIGID_BODY,
	XCOT_GHOST_OBJECT,
};

class IRigidBody;
class IGhostObject;

class ICollisionObject: public IXUnknown
{
public:
	virtual XCOLLISION_OBJECT_TYPE XMETHODCALLTYPE getType() const = 0;

	virtual void XMETHODCALLTYPE setCollisionShape(ICollisionShape *pCollisionShape) = 0;
	virtual ICollisionShape* XMETHODCALLTYPE getCollisionShape() = 0;

	virtual void XMETHODCALLTYPE setFriction(float fFriction) = 0;
	virtual float XMETHODCALLTYPE getFriction() const = 0;

	virtual void XMETHODCALLTYPE setRollingFriction(float fFriction) = 0;
	virtual float XMETHODCALLTYPE getRollingFriction() const = 0;

	virtual void XMETHODCALLTYPE setSpinningFriction(float fFriction) = 0;
	virtual float XMETHODCALLTYPE getSpinningFriction() const = 0;

	virtual void XMETHODCALLTYPE setPosition(const float3 &vPosition) = 0;
	virtual float3 XMETHODCALLTYPE getPosition() const = 0;

	virtual void XMETHODCALLTYPE setRotation(const SMQuaternion &qRotation) = 0;
	virtual SMQuaternion XMETHODCALLTYPE getRotation() const = 0;

	virtual void XMETHODCALLTYPE setCollisionFlags(XCOLLISION_FLAGS flags) = 0;
	virtual XCOLLISION_FLAGS XMETHODCALLTYPE getCollisionFlags() const = 0;

	virtual void XMETHODCALLTYPE setUserPointer(void *pUser) = 0;
	virtual void* XMETHODCALLTYPE getUserPointer() const = 0;

	virtual void XMETHODCALLTYPE setCCDsweptSphereRadius(float fRadius) = 0;
	virtual float XMETHODCALLTYPE getCCDsweptSphereRadius() const = 0;

	virtual void XMETHODCALLTYPE setCCDmotionThreshold(float fRadius) = 0;
	virtual float XMETHODCALLTYPE getCCDmotionThreshold() const = 0;

	virtual IRigidBody* XMETHODCALLTYPE asRigidBody() const = 0;
	virtual IGhostObject* XMETHODCALLTYPE asGhostObject() const = 0;
};

struct XRIDIGBODY_DESC
{
	float fMass;
	float3_t vStartWorldPosition;
	SMQuaternion qStartWorldRotation;
	ICollisionShape *pCollisionShape = NULL;
	float3_t vLocalInertia;

	float fLinearDamping = 0.0f;
	float fAngularDamping = 0.0f;

	///best simulation results when friction is non-zero
	float fFriction = 0.5f;
	///the rollingFriction prevents rounded shapes, such as spheres, cylinders and capsules from rolling forever.
	float fRollingFriction = 0.0f;
	float fSpinningFriction = 0.0f; //torsional friction around contact normal
	
	float fLinearSleepingThreshold = 0.8f;
	float fAngularSleepingThreshold = 1.0f;
};

class IRigidBody: public ICollisionObject
{
public:
	virtual void XMETHODCALLTYPE setDamping(float fLinearDamping, float fAngularDamping) = 0;
	virtual float XMETHODCALLTYPE getLinearDamping() const = 0;
	virtual float XMETHODCALLTYPE getAngularDamping() const = 0;

	virtual float XMETHODCALLTYPE getLinearSleepingThreshold() const = 0;
	virtual float XMETHODCALLTYPE getAngularSleepingThreshold() const = 0;

	virtual void XMETHODCALLTYPE setMass(float fMass, const float3 &vLocalInertia) = 0;

	virtual float3 XMETHODCALLTYPE getLinearFactor() const = 0;
	virtual void XMETHODCALLTYPE setLinearFactor(const float3 &vLinearFactor) = 0;
	virtual float3 XMETHODCALLTYPE getAngularFactor() const = 0;
	virtual void XMETHODCALLTYPE setAngularFactor(const float3 &vAngularFactor) = 0;

	virtual void XMETHODCALLTYPE applyCentralForce(const float3 &vForce) = 0;
	// https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=2995
	virtual void XMETHODCALLTYPE applyForce(const float3 &vForce, const float3 &vWorldPos) = 0;
	virtual void XMETHODCALLTYPE applyTorque(const float3 &vTorque) = 0;

	virtual void XMETHODCALLTYPE applyCentralImpulse(const float3 &vForce) = 0;
	virtual void XMETHODCALLTYPE applyImpulse(const float3 &vForce, const float3 &vWorldPos) = 0;
	virtual void XMETHODCALLTYPE applyTorqueImpulse(const float3 &vTorque) = 0;

	virtual void XMETHODCALLTYPE setLinearVelocity(const float3 &vLinearVelocity) = 0;
	virtual void XMETHODCALLTYPE setAngularVelocity(const float3 &vAngularVelocity) = 0;
};


class IContactManifoldPoint
{
public:
	virtual float XMETHODCALLTYPE getDistance() const = 0;

	virtual float3 XMETHODCALLTYPE getLocalPointA() const = 0;
	virtual float3 XMETHODCALLTYPE getLocalPointB() const = 0;

	virtual float3 XMETHODCALLTYPE getWorldPointA() const = 0;
	virtual float3 XMETHODCALLTYPE getWorldPointB() const = 0;

	virtual float3 XMETHODCALLTYPE getWorldNormalB() const = 0;
};

class IContactManifold
{
public:
	virtual UINT XMETHODCALLTYPE getContactCount() const = 0;
	virtual IContactManifoldPoint* XMETHODCALLTYPE getContact(UINT uIndex) const = 0;
};

class ICollisionPair
{
public:
	virtual ICollisionObject* XMETHODCALLTYPE getObject0() = 0;
	virtual ICollisionObject* XMETHODCALLTYPE getObject1() = 0;

	virtual UINT XMETHODCALLTYPE getContactManifoldCount() const = 0;
	virtual IContactManifold* XMETHODCALLTYPE getContactManifold(UINT uIndex) const = 0;
};

class IGhostObject: public ICollisionObject
{
public:
	virtual UINT XMETHODCALLTYPE getOverlappingObjectCount() const = 0;
	virtual ICollisionObject* XMETHODCALLTYPE getOverlappingObject(UINT uIndex) const = 0;

	virtual UINT XMETHODCALLTYPE getOverlappingPairCount() const = 0;
	virtual ICollisionPair* XMETHODCALLTYPE getOverlappingPair(UINT uIndex) const = 0;
};

#endif
