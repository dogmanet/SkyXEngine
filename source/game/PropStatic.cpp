
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "PropStatic.h"

/*! \skydocent prop_static
Модель
*/

BEGIN_PROPTABLE(CPropStatic)
	//empty
END_PROPTABLE()

REGISTER_ENTITY(CPropStatic, prop_static);

CPropStatic::~CPropStatic()
{
	removePhysBody();
}

void CPropStatic::createPhysBody()
{
	if(m_pCollideShape)
	{
		btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(Q4_BTQUAT(m_vOrientation), F3_BTVEC(m_vPosition)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			0,                  // mass
			motionState,        // initial position
			m_pCollideShape,    // collision shape of body
			btVector3(0, 0, 0)  // local inertia
			);
		m_pRigidBody = new btRigidBody(rigidBodyCI);
		m_pRigidBody->getInvMass();

		//m_pRigidBody->setFriction(100.0f);
		m_pRigidBody->setUserPointer(this);
		int colGroup = CG_STATIC;
		int colMask = CG_STATIC_MASK;
		SPhysics_AddShapeEx(m_pRigidBody, colGroup, colMask);

		m_pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
		m_pRigidBody->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
	}
}

void CPropStatic::removePhysBody()
{
	SPhysics_RemoveShape(m_pRigidBody);
	mem_delete(m_pRigidBody);
}
