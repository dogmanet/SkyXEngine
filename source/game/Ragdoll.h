
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __RAGDOLL_H
#define __RAGDOLL_H

#include "sxgame.h"
#include <anim/sxanim.h>

class CRagdoll: public IAnimRagdoll
{
public:
	CRagdoll(IAnimPlayer *pAnimPlayer);
	~CRagdoll();

	virtual bool isBoneAffected(ID id);
	virtual float3 getBoneOffset(ID id);
	virtual SMQuaternion getBoneRotation(ID id);

protected:

	btCollisionShape **m_shapes;
	btRigidBody **m_bodies;
	btTypedConstraint **m_ppJoints;

	int m_iBodiesCount;
	int m_iJointsCount;
	int *m_pidBones;

	btRigidBody *localCreateRigidBody(btScalar mass, const btTransform &startTransform, btCollisionShape *pShape);
};

#endif
