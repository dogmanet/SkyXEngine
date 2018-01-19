
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "Ragdoll.h"

CRagdoll::CRagdoll(IAnimPlayer *pAnimPlayer)
{
/*
	int iShapeCount;
	HITBOX_TYPE * phTypes;
	float3_t ** ppfData;
	int32_t * pDataLen;
	pAnimPlayer->getPhysData(&iShapeCount, &phTypes, &ppfData, &pDataLen);
*/

	enum JointType
	{
		JOINT_CONETWIST,
		JOINT_HINGE
	};
	struct
	{
		const char * szBone;
		float fCapsLength;
		float fCapsRadius;
		float3_t offset;
		float3_t rot;
	}
	bones[] = {
		/*  0 */ {"bip01_head", 0.09956974f, 0.18422470f, float3_t(0.0f, 1.72571533f, -0.00327849f), float3_t(0.0f, 0.0f, 0.0f)},
		/*  1 */ {"bip01_neck", 0.24512114f, 0.21976810f, float3_t(0.0f, 1.51790970f, -0.00327849f), float3_t(0.0f, 0.0f, -SM_PIDIV2)},
		/*  2 */ {"bip01_l_forearm", 0.19536819f, 0.13013379f, float3_t(0.36014973f, 1.49402771f, 0.00f), float3_t(0.0f, 0.0f, SMToRadian(113.364143f))},
		/*  3 */ {"bip01_r_forearm", 0.19536819f, 0.13013379f, float3_t(-0.36014973f, 1.49402771f, 0.00f), float3_t(0.0f, 0.0f, -SMToRadian(113.364143f))},
		/*  4 */ {"bip01_l_hand", 0.15561146f, 0.10492057f, float3_t(0.59173538f, 1.40641129f, 0.03210649f), float3_t(-SMToRadian(13.633236f), SMToRadian(-1.006811f), -SMToRadian(-108.437599f))},
		/*  5 */ {"bip01_r_hand", 0.15561146f, 0.10492057f, float3_t(-0.59173538f, 1.40641129f, 0.03210649f), float3_t(-SMToRadian(13.633236f), -SMToRadian(-1.006811f), SMToRadian(-108.437599f))},
		/*  6 */ {"", 0.11548414f, 0.23943686f, float3_t(0.0f, 1.28031441f, 0.0f), float3_t(SMToRadian(0.0f), SMToRadian(0.0f), SMToRadian(90.0f))},
		/*  7 */ {"bip01_pelvis", 0.17556768f, 0.23326019f, float3_t(0.0f, 1.03345764f, 0.0f), float3_t(SMToRadian(0.0f), SMToRadian(0.0f), SMToRadian(90.0f))},
		/*  8 */ {"bip01_l_calf", 0.20688147f, 0.17573799f, float3_t(0.12117543f, 0.77001144f, 0.01623557f), float3_t(SMToRadian(0.0f), SMToRadian(0.0f), -SMToRadian(9.690015f))},
		/*  9 */ {"bip01_r_calf", 0.20688147f, 0.17573799f, float3_t(-0.12117543f, 0.77001144f, 0.01623557f), float3_t(SMToRadian(0.0f), SMToRadian(0.0f), SMToRadian(9.690015f))},
		/* 10 */ {"bip01_l_foot", 0.33953999f, 0.12518585f, float3_t(0.16363935f, 0.42122353f, 0.0f), float3_t(-SMToRadian(7.451351f), SMToRadian(0.0f), -SMToRadian(5.320535f))},
		/* 11 */ {"bip01_r_foot", 0.33953999f, 0.12518585f, float3_t(-0.16363935f, 0.42122353f, 0.0f), float3_t(-SMToRadian(7.451351f), SMToRadian(0.0f), SMToRadian(5.320535f))},

#if 0
		/*  0 */{"jaw_1", 0.09956974f, 0.18422470f, float3_t(0.0f, 1.72571533f, -0.00327849f), float3_t(0.0f, 0.0f, 0.0f)},
		/*  1 */{"bip01_neck", 0.24512114f, 0.21976810f, float3_t(0.0f, 1.51790970f, -0.00327849f), float3_t(0.0f, 0.0f, -SM_PIDIV2)},
		/*  2 */{"bip01_l_forearm", 0.19536819f, 0.13013379f, float3_t(0.36014973f, 1.49402771f, 0.00f), float3_t(0.0f, 0.0f, SMToRadian(113.364143f))},
		/*  3 */{"bip01_r_forearm", 0.19536819f, 0.13013379f, float3_t(-0.36014973f, 1.49402771f, 0.00f), float3_t(0.0f, 0.0f, -SMToRadian(113.364143f))},
		/*  4 */{"bip01_l_hand", 0.15561146f, 0.10492057f, float3_t(0.59173538f, 1.40641129f, 0.03210649f), float3_t(-SMToRadian(13.633236f), SMToRadian(-1.006811f), -SMToRadian(-108.437599f))},
		/*  5 */{"bip01_r_hand", 0.15561146f, 0.10492057f, float3_t(-0.59173538f, 1.40641129f, 0.03210649f), float3_t(-SMToRadian(13.633236f), -SMToRadian(-1.006811f), SMToRadian(-108.437599f))},
		/*  6 */{"", 0.11548414f, 0.23943686f, float3_t(0.0f, 1.28031441f, 0.0f), float3_t(SMToRadian(0.0f), SMToRadian(0.0f), SMToRadian(90.0f))},
		/*  7 */{"bip01_pelvis", 0.17556768f, 0.23326019f, float3_t(0.0f, 1.03345764f, 0.0f), float3_t(SMToRadian(0.0f), SMToRadian(0.0f), SMToRadian(90.0f))},
		/*  8 */{"bip01_l_calf", 0.20688147f, 0.17573799f, float3_t(0.12117543f, 0.77001144f, 0.01623557f), float3_t(SMToRadian(0.0f), SMToRadian(0.0f), -SMToRadian(9.690015f))},
		/*  9 */{"bip01_r_calf", 0.20688147f, 0.17573799f, float3_t(-0.12117543f, 0.77001144f, 0.01623557f), float3_t(SMToRadian(0.0f), SMToRadian(0.0f), SMToRadian(9.690015f))},
		/* 10 */{"bip01_l_foot", 0.33953999f, 0.12518585f, float3_t(0.16363935f, 0.42122353f, 0.0f), float3_t(-SMToRadian(7.451351f), SMToRadian(0.0f), -SMToRadian(5.320535f))},
		/* 11 */{"bip01_r_foot", 0.33953999f, 0.12518585f, float3_t(-0.16363935f, 0.42122353f, 0.0f), float3_t(-SMToRadian(7.451351f), SMToRadian(0.0f), SMToRadian(5.320535f))},
#endif
		
		/* 12 */// {"bip01_l_toe0", 0.17734697f, 0.13319845f, float3_t(0.17958933f, 0.08906974f, 0.05018244f), float3_t(-SMToRadian(-50.615505f), SMToRadian(0.0f), SMToRadian(0.0f))},
		/* 13 */// {"bip01_r_toe0", 0.17734697f, 0.13319845f, float3_t(-0.17958933f, 0.08906974f, 0.05018244f), float3_t(-SMToRadian(-50.615505f), SMToRadian(0.0f), SMToRadian(0.0f))}
		//{"bip01_spine2", 0.07044751f, 0.30675392f, float3_t(0.0f, 1.34182251f, 0.0f), float3_t(SMToRadian(0.0f), SMToRadian(0.0f), SMToRadian(90.0f))}
	};
	m_iBodiesCount = ARRAYSIZE(bones);
	struct
	{
		const char * szBone;
		int fromBody;
		int toBody;
		JointType joint_type;
		float3_t data;
	}
	joints[] = {
		{"bip01_neck", 1, 0, JOINT_CONETWIST, float3_t(SM_PIDIV4, SM_PIDIV4, SM_PIDIV2)}
	};
	btTransform offset; offset.setIdentity();
	offset.setOrigin(F3_BTVEC(pAnimPlayer->getPos()));

	m_shapes = new btCollisionShape*[m_iBodiesCount];
	m_bodies = new btRigidBody*[m_iBodiesCount];

	m_pidBones = new ID[pAnimPlayer->getBoneCount()];
	for(int i = 0, l = pAnimPlayer->getBoneCount(); i < l; ++i)
	{
		m_pidBones[i] = -1;
	}

	for(int i = 0, l = m_iBodiesCount; i < l; ++i)
	{
		if(bones[i].szBone[0])
		{
			m_pidBones[pAnimPlayer->getBone(bones[i].szBone)] = i;
		}

		m_shapes[i] = new btCapsuleShape(bones[i].fCapsRadius * 0.5f, (bones[i].fCapsLength + bones[i].fCapsRadius) * 0.5f);
		
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(F3_BTVEC(bones[i].offset));
		transform.getBasis().setRotation(Q4_BTQUAT(
			SMQuaternion(bones[i].rot.x, 'x')
			* SMQuaternion(bones[i].rot.y, 'y')
			* SMQuaternion(bones[i].rot.z, 'z')
		));

		m_bodies[i] = localCreateRigidBody(btScalar(10.0f), offset * transform, m_shapes[i]);
	}
	m_iJointsCount = 11;
	m_ppJoints = new btTypedConstraint*[m_iJointsCount];

	/*const ModelHitbox * hb;
	uint32_t iHitboxes = pAnimPlayer->getHitboxCount();
	float fScale = pAnimPlayer->getScale();
	for(uint32_t i = 0; i < iHitboxes; ++i)
	{
		hb = pAnimPlayer->getHitbox(i);
		switch(hb->type)
		{
		case HT_BOX:
			m_shapes[bodypart] = new btBoxShape(F3_BTVEC(hb->lwh * 0.5f * fScale));
			break;
		case HT_CYLINDER:
			assert("Not implemented!");
			break;
		case HT_CAPSULE:
			//assert("Not implemented!");
			m_shapes[bodypart] = new btCapsuleShape(hb->lwh.y * fScale * 0.5f, hb->lwh.z * fScale * 0.5f);
			break;
		case HT_ELIPSOID:
			//new btShape
			m_shapes[bodypart] = new btSphereShape(hb->lwh.x * 0.5f * fScale);
			break;
		case HT_CONVEX:
			assert("Not implemented!");
			break;
		}
		assert(bodypart < BODYPART_COUNT && bodypart >= 0);
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(F3_BTVEC(hb->pos * fScale));
		transform.getBasis().setRotation(Q4_BTQUAT(SMQuaternion(-hb->rot.x, 'x') * SMQuaternion(-hb->rot.y, 'y') * SMQuaternion(-hb->rot.z, 'z')));
	
		m_bodies[bodypart] = localCreateRigidBody(btScalar(1.), offset * transform, m_shapes[bodypart]);
	}*/
	// Setup the geometry
	
	// Setup some damping on the m_bodies
	for(int i = 0, l = m_iBodiesCount; i < l; ++i)
	{
		m_bodies[i]->setDamping(0.05f, 0.85f);
		m_bodies[i]->setDeactivationTime(0.8f);
		m_bodies[i]->setSleepingThresholds(1.6f, 2.5f);

		m_bodies[i]->setActivationState(ISLAND_SLEEPING);

		m_bodies[i]->setCcdMotionThreshold(0.001f);
		m_bodies[i]->setCcdSweptSphereRadius(0.1f);

		//m_bodies[i]->setAngularFactor(0.0f);
		//m_bodies[i]->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
	}
	//m_bodies[1]->setAngularFactor(0.0f);
	//m_bodies[1]->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));
	//UINT bone = pAnimPlayer->getBone("bip01_head");
	{
		btTransform localA, localB;
		btConeTwistConstraint * pCTC;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_neck"));

		localA.setOrigin(F3_BTVEC(pos));
		localA.getBasis().setEulerZYX(SM_PIDIV2, 0, SM_PIDIV2);

		localB = localA;
		localA = m_bodies[0]->getWorldTransform().inverse() * localA;
		localB = m_bodies[1]->getWorldTransform().inverse() * localB;

		pCTC = new btConeTwistConstraint(*m_bodies[0], *m_bodies[1], localA, localB);
		
		pCTC->setLimit(SM_PIDIV4, SM_PIDIV4, SM_PIDIV2);

		m_ppJoints[0] = pCTC;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[0], true);
	}
	{
		btTransform localA, localB;
		btConeTwistConstraint * pCTC;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_l_upperarm"));

		localA.setOrigin(F3_BTVEC(pos));
		localA.getBasis().setIdentity();
		//localA.getBasis().setEulerZYX(SM_PIDIV2, 0, SM_PIDIV2);

		localB = localA;
		localA = m_bodies[2]->getWorldTransform().inverse() * localA;
		localB = m_bodies[1]->getWorldTransform().inverse() * localB;

		pCTC = new btConeTwistConstraint(*m_bodies[2], *m_bodies[1], localA, localB);

		pCTC->setLimit(SM_PIDIV2, SM_PIDIV2, 0);

		m_ppJoints[1] = pCTC;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[1], true);
	}
	{
		btTransform localA, localB;
		btConeTwistConstraint * pCTC;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_r_upperarm"));

		localA.setOrigin(F3_BTVEC(pos));
		localA.getBasis().setIdentity();
		localA.getBasis().setEulerZYX(0, SM_PI, 0);

		localB = localA;
		localA = m_bodies[3]->getWorldTransform().inverse() * localA;
		localB = m_bodies[1]->getWorldTransform().inverse() * localB;

		pCTC = new btConeTwistConstraint(*m_bodies[3], *m_bodies[1], localA, localB);

		pCTC->setLimit(SM_PIDIV2, SM_PIDIV2, 0);

		m_ppJoints[2] = pCTC;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[2], true);
	}

	{
		btTransform localA, localB;
		btHingeConstraint * pHinge;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_l_forearm"));

		localA.setOrigin(F3_BTVEC(pos));
		localA.getBasis().setIdentity();
		localA.getBasis().setEulerZYX(SM_PIDIV2, 0, 0);

		localB = localA;
		localA = m_bodies[4]->getWorldTransform().inverse() * localA;
		localB = m_bodies[2]->getWorldTransform().inverse() * localB;

		pHinge = new btHingeConstraint(*m_bodies[4], *m_bodies[2], localA, localB);

		pHinge->setLimit(0, SM_PI / 6.0f * 5.0f);
		//pCTC->setLimit(SM_PIDIV2, SM_PIDIV2, 0);
		//pHinge->setAngularOnly(true);

		m_ppJoints[3] = pHinge;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[3], true);
	}
	{
		btTransform localA, localB;
		btHingeConstraint * pHinge;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_r_forearm"));

		localA.setOrigin(F3_BTVEC(pos));
		localA.getBasis().setIdentity();
		localA.getBasis().setEulerZYX(SM_PIDIV2, 0, 0);

		localB = localA;
		localA = m_bodies[5]->getWorldTransform().inverse() * localA;
		localB = m_bodies[3]->getWorldTransform().inverse() * localB;

		pHinge = new btHingeConstraint(*m_bodies[5], *m_bodies[3], localA, localB);

		pHinge->setLimit(0, SM_PI / 6.0f * 5.0f);
		//pCTC->setLimit(SM_PIDIV2, SM_PIDIV2, 0);
		//pHinge->setAngularOnly(true);

		m_ppJoints[4] = pHinge;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[4], true);
	}
	{
		btTransform localA, localB;
		btFixedConstraint * joint6DOF;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_spine1"));

		localA.setOrigin(F3_BTVEC(pos));
		localA.getBasis().setIdentity();
		localA.getBasis().setEulerZYX(SM_PIDIV2, 0, 0);

		localB = localA;
		localA = m_bodies[6]->getWorldTransform().inverse() * localA;
		localB = m_bodies[1]->getWorldTransform().inverse() * localB;

		joint6DOF = new btFixedConstraint(*m_bodies[6], *m_bodies[1], localA, localB);
	

		//joint6DOF->setAngularLowerLimit(btVector3(-SIMD_PI*0.2, -SIMD_EPSILON, -SIMD_PI*0.3));
		//joint6DOF->setAngularUpperLimit(btVector3(SIMD_PI*0.2, SIMD_EPSILON, SIMD_PI*0.6));

		m_ppJoints[5] = joint6DOF;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[5], true);
	}
	{
		btTransform localA, localB;
		btGeneric6DofConstraint * joint6DOF;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_spine1"));

		localA.setOrigin(F3_BTVEC(pos));
		localA.getBasis().setIdentity();
		localA.getBasis().setEulerZYX(SM_PIDIV2, 0, 0);

		localB = localA;
		localA = m_bodies[7]->getWorldTransform().inverse() * localA;
		localB = m_bodies[6]->getWorldTransform().inverse() * localB;

		joint6DOF = new btGeneric6DofConstraint(*m_bodies[7], *m_bodies[6], localA, localB, true);


		joint6DOF->setAngularLowerLimit(btVector3(-SIMD_PI*0.2, -SIMD_EPSILON, -SIMD_PI*0.3));
		joint6DOF->setAngularUpperLimit(btVector3(SIMD_PI*0.2, SIMD_EPSILON, SIMD_PI*0.6));

		m_ppJoints[6] = joint6DOF;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[6], true);
	}

	{
		btTransform localA, localB;
		btConeTwistConstraint * pCTC;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_l_thigh"));

		localA.setOrigin(F3_BTVEC(pos));
		//localA.getBasis().setIdentity();
		localA.getBasis().setEulerZYX(0, -SM_PIDIV4, -SM_PIDIV4);

		localB = localA;
		localA = m_bodies[8]->getWorldTransform().inverse() * localA;
		localB = m_bodies[7]->getWorldTransform().inverse() * localB;

		pCTC = new btConeTwistConstraint(*m_bodies[8], *m_bodies[7], localA, localB);

		pCTC->setLimit(SM_PIDIV4, SM_PIDIV4, 0);

		m_ppJoints[7] = pCTC;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[7], true);
	}
	{
		btTransform localA, localB;
		btConeTwistConstraint * pCTC;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_r_thigh"));

		localA.setOrigin(F3_BTVEC(pos));
		//localA.getBasis().setIdentity();
		localA.getBasis().setEulerZYX(0, -SM_PIDIV4 - SM_PIDIV2, SM_PIDIV4);

		localB = localA;
		localA = m_bodies[9]->getWorldTransform().inverse() * localA;
		localB = m_bodies[7]->getWorldTransform().inverse() * localB;

		pCTC = new btConeTwistConstraint(*m_bodies[9], *m_bodies[7], localA, localB);

		pCTC->setLimit(SM_PIDIV4, SM_PIDIV4, 0);

		m_ppJoints[8] = pCTC;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[8], true);
	}
	{
		btTransform localA, localB;
		btHingeConstraint * pHinge;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_l_calf"));

		localA.setOrigin(F3_BTVEC(pos));
		localA.getBasis().setIdentity();
		localA.getBasis().setEulerZYX(SM_PIDIV2, 0, -SM_PIDIV2);

		localB = localA;
		localA = m_bodies[10]->getWorldTransform().inverse() * localA;
		localB = m_bodies[8]->getWorldTransform().inverse() * localB;

		pHinge = new btHingeConstraint(*m_bodies[10], *m_bodies[8], localA, localB);

		pHinge->setLimit(0, SM_PI / 6.0f * 5.0f);
		//pCTC->setLimit(SM_PIDIV2, SM_PIDIV2, 0);
		//pHinge->setAngularOnly(true);

		m_ppJoints[9] = pHinge;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[9], true);
	}
	{
		btTransform localA, localB;
		btHingeConstraint * pHinge;

		float3 pos = pAnimPlayer->getBoneTransformPos(pAnimPlayer->getBone("bip01_r_calf"));

		localA.setOrigin(F3_BTVEC(pos));
		localA.getBasis().setIdentity();
		localA.getBasis().setEulerZYX(SM_PIDIV2, 0, -SM_PIDIV2);

		localB = localA;
		localA = m_bodies[11]->getWorldTransform().inverse() * localA;
		localB = m_bodies[9]->getWorldTransform().inverse() * localB;

		pHinge = new btHingeConstraint(*m_bodies[11], *m_bodies[9], localA, localB);

		pHinge->setLimit(0, SM_PI / 6.0f * 5.0f);
		//pCTC->setLimit(SM_PIDIV2, SM_PIDIV2, 0);
		//pHinge->setAngularOnly(true);

		m_ppJoints[10] = pHinge;
		SXPhysics_GetDynWorld()->addConstraint(m_ppJoints[10], true);
	}



	/*SXPhysics_GetDynWorld()->getDebugDrawer()->setDebugMode(
		SXPhysics_GetDynWorld()->getDebugDrawer()->getDebugMode()
		| btIDebugDraw::DBG_DrawConstraintLimits | btIDebugDraw::DBG_DrawConstraints
	);*/

	
}


CRagdoll::~CRagdoll()
{
	int i;

	// Remove all constraints
	for(i = 0; i < m_iJointsCount; ++i)
	{
		SXPhysics_GetDynWorld()->removeConstraint(m_ppJoints[i]);
		mem_delete(m_ppJoints[i]);
	}
	mem_delete_a(m_ppJoints);

	// Remove all bodies and shapes
	for(i = 0; i < m_iBodiesCount; ++i)
	{
		SXPhysics_GetDynWorld()->removeRigidBody(m_bodies[i]);

		delete m_bodies[i]->getMotionState();

		mem_delete(m_bodies[i]);
		mem_delete(m_shapes[i]);
	}
	mem_delete_a(m_shapes);
	mem_delete_a(m_bodies);
	mem_delete_a(m_pidBones);
}

bool CRagdoll::isBoneAffected(ID id)
{
	return(ID_VALID(m_pidBones[id]));
}
float3 CRagdoll::getBoneOffset(ID id)
{
	assert(ID_VALID(m_pidBones[id]));
	return(BTVEC_F3(m_bodies[m_pidBones[id]]->getWorldTransform().getOrigin()));
}
SMQuaternion CRagdoll::getBoneRotation(ID id)
{
	assert(ID_VALID(m_pidBones[id]));
	//return(SMQuaternion());
	return(BTQUAT_Q4(m_bodies[m_pidBones[id]]->getWorldTransform().getRotation()) );
	// * SMQuaternion(SM_PIDIV2, 'y') * SMQuaternion(-SM_PI, 'z')
	// * SMQuaternion(-SM_PIDIV2, 'x') * SMQuaternion(-SM_PIDIV2, 'z')
}

btRigidBody *CRagdoll::localCreateRigidBody(btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
{
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if(isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	rbInfo.m_additionalDamping = true;
	btRigidBody* body = new btRigidBody(rbInfo);

	SXPhysics_GetDynWorld()->addRigidBody(body);

	return body;
}
