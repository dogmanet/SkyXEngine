
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "BaseCharacter.h"
#include "GameData.h"
#include "BaseTool.h"
#include "BaseWeapon.h"

#include <aigrid/sxaigrid.h>

/*! \skydocent base_character
Базовый класс персонажа
*/

BEGIN_PROPTABLE(CBaseCharacter)
	// empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseCharacter, base_character);

IEventChannel<XEventPhysicsStep> *CBaseCharacter::m_pTickEventChannel = NULL;

class btKinematicClosestNotMeRayResultCallback: public btCollisionWorld::ClosestRayResultCallback
{
public:
	btKinematicClosestNotMeRayResultCallback(btCollisionObject* me, const btVector3&	rayFromWorld, const btVector3&	rayToWorld): btCollisionWorld::ClosestRayResultCallback(rayFromWorld, rayToWorld)
	{
		m_me = me;
		m_shapeInfo = {-1, -1};
	}

	virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
	{
		if(rayResult.m_collisionObject == m_me)
			return 1.0;
		if(rayResult.m_localShapeInfo)
		{
			m_shapeInfo = *rayResult.m_localShapeInfo;
		}
		return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
	}
	btCollisionWorld::LocalShapeInfo m_shapeInfo;
protected:
	btCollisionObject* m_me;
};

void CCharacterPhysicsTickEventListener::onEvent(const XEventPhysicsStep *pData)
{
	m_pCharacter->onPhysicsStep();
}

CBaseCharacter::CBaseCharacter():
	m_physicsTicker(this)
{
	if(!m_pTickEventChannel)
	{
		m_pTickEventChannel = Core_GetIXCore()->getEventChannel<XEventPhysicsStep>(EVENT_PHYSICS_STEP_GUID);
	}
}

void CBaseCharacter::onPostLoad()
{
	BaseClass::onPostLoad();

	m_pCollideShape = new btCapsuleShape(m_fCapsRadius, m_fCapsHeight - m_fCapsRadius * 2.0f);

	btTransform startTransform;
	startTransform.setIdentity();

	//((btCompoundShape*)m_pCollideShape)->addChildShape(startTransform, m_pCollideShapeBottom);
	//startTransform.setOrigin(btVector3(0.0f, m_fCapsHeight * 0.5f, 0.0f));
	//((btCompoundShape*)m_pCollideShape)->addChildShape(startTransform, m_pCollideShapeTop);

	//btTransform startTransform;
	startTransform.setIdentity();
	float3 vPos = getPos() + float3(0.0f, m_fCapsHeight * 0.5f, 0.0f);
	startTransform.setOrigin(F3_BTVEC(vPos));
	//startTransform.setOrigin(btVector3(0, 12, 10));

	m_pGhostObject = new btPairCachingGhostObject();
	void *p1 = m_pGhostObject;
	void *p2 = &m_pGhostObject->getWorldTransform();
	printf(COLOR_LRED "p1: 0x%p; p2: 0x%p" COLOR_RESET "\n", p1, p2);
	m_pGhostObject->setWorldTransform(startTransform);
	//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	m_pGhostObject->setCollisionShape(m_pCollideShape);
	m_pGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT | btCollisionObject::CF_KINEMATIC_OBJECT);
	m_pGhostObject->setUserPointer(this);

	m_pHeadEnt = (CPointEntity*)CREATE_ENTITY("base_point", m_pMgr);
	m_pHeadEnt->setPos(getPos() + float3(0.0f, m_fCapsHeight - 0.1f, 0.0f));
	m_pHeadEnt->setOrient(getOrient());
	m_pHeadEnt->setParent(this);

	btScalar stepHeight = 0.4f;
	m_pCharacter = new btKinematicCharacterController(m_pGhostObject, (btConvexShape*)m_pCollideShape, stepHeight, btVector3(0.0f, 1.0f, 0.0f));
	m_pCharacter->setMaxJumpHeight(0.60f);
	m_pCharacter->setJumpSpeed(3.50f);
	//m_pCharacter->setJumpSpeed(3.5f);
	m_pCharacter->setGravity(btVector3(0, -10.0f, 0));
	//m_pCharacter->setGravity(1.0f);
	m_pCharacter->setFallSpeed(300.0f);
	//m_pCharacter->setFallSpeed(30.0f);
	m_pCharacter->setMaxPenetrationDepth(0.1f);
	m_pGhostObject->setWorldTransform(startTransform);

	SPhysics_GetDynWorld()->addCollisionObject(m_pGhostObject, CG_CHARACTER, CG_ALL & ~(CG_DEBRIS | CG_HITBOX | CG_WATER));

	//m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	SPhysics_GetDynWorld()->addAction(m_pCharacter);


	m_flashlight = (CLightDirectional*)CREATE_ENTITY("light_directional", m_pMgr);
	//m_flashlight->setPos(getPos() + float3(0.f, 0.1f, 0.f));
	m_flashlight->setPos(m_pHeadEnt->getPos() + float3(0.0f, 0.1f, 0.1f));
	m_flashlight->setOrient(m_pHeadEnt->getOrient() * SMQuaternion(SM_PIDIV2 - SMToRadian(5.0f), 'x'));
	m_flashlight->setParent(m_pHeadEnt);
	m_flashlight->setDist(20.f);
	m_flashlight->setOuterAngle(SMToRadian(30)); // 60
	m_flashlight->setInnerAngle(SMToRadian(10));
	m_flashlight->setColor(float3(1.0f, 0.95f, 0.65f));
	//m_flashlight->setShadowType(-1);
	m_flashlight->setShadowType(1);
	m_flashlight->setFlags(m_flashlight->getFlags() | LIGHT_INITIALLY_DARK);
	m_flashlight->setEnable(false);

	m_idTaskSpread = SET_INTERVAL(updateSpread, 1.0f / 30.0f);

	m_pInventory = new CCharacterInventory(this);

	m_pTickEventChannel->addListener(&m_physicsTicker);
}

CBaseCharacter::~CBaseCharacter()
{
	m_pTickEventChannel->removeListener(&m_physicsTicker);

	CLEAR_INTERVAL(m_idTaskSpread);
	REMOVE_ENTITY(m_pHeadEnt);
	REMOVE_ENTITY(m_flashlight);
	mem_delete(m_pInventory);

	mem_delete(m_pCharacter);
	mem_delete(m_pGhostObject);
	mem_delete(m_pCollideShape);

	mem_release(m_pHandsModelResource);

	if(m_idQuadCurr >= 0)
	{
		//SAIG_QuadSetState(m_idQuadCurr, AIQUAD_STATE_FREE);
	}
}


void CBaseCharacter::attack(BOOL state)
{
	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	if(m_pActiveTool)
	{
		m_pActiveTool->primaryAction(state);
	}
}

void CBaseCharacter::attack2(BOOL state)
{
	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	if(m_pActiveTool)
	{
		m_pActiveTool->secondaryAction(state);
	}
}

void CBaseCharacter::reload()
{
	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	if(m_pActiveTool)
	{
		m_pActiveTool->reload();
	}
}

void CBaseCharacter::toggleFlashlight()
{
	m_flashlight->toggleEnable();
}

void CBaseCharacter::nextFireMode()
{
	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	if(m_pActiveTool && m_pActiveTool->isWeapon())
	{
		((CBaseWeapon*)m_pActiveTool)->nextFireMode();
	}
}

bool CBaseCharacter::onGround()
{
	return(m_pCharacter->onGround());
}

void CBaseCharacter::playFootstepsSound()
{
	if(!(m_uMoveDir & PM_OBSERVER))
	{
		if(onGround())
		{
			float3 start = getPos() + float3(0.0f, 0.5f, 0.0f),
				end = start + float3(0.0f, -2.0f, 0.0f);
			btKinematicClosestNotMeRayResultCallback cb(m_pGhostObject, F3_BTVEC(start), F3_BTVEC(end));
			SPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

			if(cb.hasHit()/* && cb.m_shapeInfo.m_shapePart == 0 && cb.m_shapeInfo.m_triangleIndex >= 0*/)
			{
				MTLTYPE_PHYSIC type = (MTLTYPE_PHYSIC)SPhysics_GetMtlType(cb.m_collisionObject, &cb.m_shapeInfo);
				g_pGameData->playFootstepSound(type, BTVEC_F3(cb.m_hitPointWorld));
			}
		}
	}
}

void CBaseCharacter::setPos(const float3 & pos)
{
	BaseClass::setPos(pos);
	m_pGhostObject->getWorldTransform().setOrigin(F3_BTVEC(pos + float3(0.0f, (m_fCurrentHeight * m_fCapsHeight) * 0.5f, 0.0f)));
}

float CBaseCharacter::getAimRange()
{
	float3 start = getHead()->getPos();
	float3 dir = getHead()->getOrient() * float3(0.0f, 0.0f, 1.0f);
	float3 end = start + dir * 1000.0f;

	btKinematicClosestNotMeRayResultCallback cb(m_pGhostObject, F3_BTVEC(start), F3_BTVEC(end));
	SPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

	if(cb.hasHit())
	{
		return(SMVector3Length(BTVEC_F3(cb.m_hitPointWorld) - start));
	}
	return(-1.0f);
}

float CBaseCharacter::getMomentSpread()
{
	if(!m_pActiveTool || !m_pActiveTool->isWeapon())
	{
		return(0.0f);
	}

	CBaseWeapon *pWpn = (CBaseWeapon*)m_pActiveTool;

	float fBaseSpread = pWpn->getBaseSpread();

	const float fIdleSpreadBase = 1.0f, fIdleSpreadMult = pWpn->getSpreadCoeff(SPREAD_COEFF_IDLE), // стоя
		fCrouchSpreadBase = 1.0f,       fCrouchSpreadMult = pWpn->getSpreadCoeff(SPREAD_COEFF_CROUCH), // пригувшись
		fLaySpreadBase = 1.0f,          fLaySpreadMult = pWpn->getSpreadCoeff(SPREAD_COEFF_CRAWL), // лежа

		fWalkSpreadBase = 1.0f,         fWalkSpreadMult = pWpn->getSpreadCoeff(SPREAD_COEFF_WALK), // в ходьбе
		fRunSpreadBase = 1.0f,          fRunSpreadMult = pWpn->getSpreadCoeff(SPREAD_COEFF_RUN), // в беге
		fAirborneSpreadBase = 1.0f,     fAirborneSpreadMult = pWpn->getSpreadCoeff(SPREAD_COEFF_AIRBORNE), // в прыжке
		fConditionSpreadBase = 1.0f,    fConditionSpreadMult = pWpn->getSpreadCoeff(SPREAD_COEFF_CONDITION), // состояние оружия
		fArmSpreadBase = 1.0f,          fArmSpreadMult = pWpn->getSpreadCoeff(SPREAD_COEFF_ARM), // состояние рук
		fIronSightSpreadBase = 1.0f,    fIronSightSpreadMult = pWpn->getSpreadCoeff(SPREAD_COEFF_IRONSIGHT); // в прицеливании



	float fMomentSpread = fBaseSpread * 
		(fIdleSpreadBase + (!(m_uMoveDir & (PM_CROUCH | PM_CRAWL)) ? fIdleSpreadMult : 0.0f)) *
		(fWalkSpreadBase + (((m_uMoveDir & (PM_FORWARD | PM_BACKWARD | PM_LEFT | PM_RIGHT)) && !(m_uMoveDir & PM_RUN)) ? fWalkSpreadMult : 0.0f)) *
		(fCrouchSpreadBase + ((m_uMoveDir & PM_CROUCH) ? fCrouchSpreadMult : 0.0f)) *
		(fRunSpreadBase + ((m_uMoveDir & PM_RUN) ? fRunSpreadMult : 0.0f)) *
		(fLaySpreadBase + ((m_uMoveDir & PM_CRAWL) ? fLaySpreadMult : 0.0f)) *
		(fAirborneSpreadBase + (!onGround() ? fAirborneSpreadMult : 0.0f)) *
		(fConditionSpreadBase + (fConditionSpreadMult * (1.0f - pWpn->getCondition()))) *
		// (fArmSpreadBase + (fArmSpreadMult * (1.0f - getArmCondition()))) *
		(fIronSightSpreadBase + (pWpn->isIronSight() ? fIronSightSpreadMult : 0.0f))
	;

	//printf("%5.3f => %5.3f\n", fMomentSpread, fBaseSpread);
	if(fMomentSpread < fBaseSpread)
	{
		fMomentSpread = fBaseSpread;
	}
	return(fMomentSpread);
}

void CBaseCharacter::updateSpread(float dt)
{
	float fMomentSpread = getMomentSpread();
	float fWeaponMass = m_pActiveTool ? m_pActiveTool->getWeight() : 0.0f;
	//printf("%5.3f => %5.3f\n", fMomentSpread, m_fCurrentSpread);
	if(m_fCurrentSpread < fMomentSpread)
	{
		m_fCurrentSpread += fMomentSpread * (3.0f * dt);
	}
	else
	{
		float fDivider = 5.0f;
		float fCoeff = (3.0f * dt) * (fDivider / (fWeaponMass + (fDivider / 0.9f)) + 0.1f);
		if(fCoeff > 1.0f)
		{
			fCoeff = 1.0f;
		}
		m_fCurrentSpread -= (m_fCurrentSpread - fMomentSpread) * fCoeff;
	}
}

float CBaseCharacter::getCurrentSpread()
{
	return(m_fCurrentSpread);
}

void CBaseCharacter::initHitboxes()
{
	if(!m_pModel || !m_pModel->asAnimatedModel())
	{
		return;
	}

	auto pAnimatedModel = m_pModel->asAnimatedModel();

	int l = pAnimatedModel->getHitboxCount();
	m_pHitboxBodies = new btRigidBody*[l];

	const XResourceModelHitbox * hb;
	for(int i = 0; i < l; ++i)
	{
		hb = pAnimatedModel->getHitbox(i);
		btCollisionShape *pShape;
		switch(hb->type)
		{
		case XHT_BOX:
			pShape = new btBoxShape(F3_BTVEC(hb->lwh * 0.5f));
			break;
		case XHT_CAPSULE:
			pShape = new btCapsuleShape(hb->lwh.y * 0.5f, hb->lwh.z);
			break;
		case XHT_CYLINDER:
			pShape = new btCylinderShape(F3_BTVEC(hb->lwh * 0.5f));
			break;
		case XHT_SPHERE:
			pShape = new btSphereShape(hb->lwh.x);
			break;
		default:
			assert(!"Not supported here!");
		}
		btVector3 vInertia;
		const float fMass = 1.0f;
		pShape->calculateLocalInertia(fMass, vInertia);

		btDefaultMotionState * motionState = new btDefaultMotionState();

		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			fMass,                  // mass
			motionState,        // initial position
			pShape,    // collision shape of body
			vInertia  // local inertia
			);
		btRigidBody * pRigidBody = new btRigidBody(rigidBodyCI);
		pRigidBody->setUserPointer(this);

		pRigidBody->setAngularFactor(0.0f);
		pRigidBody->setLinearFactor(btVector3(0.0f, 0.0f, 0.0f));

		pRigidBody->setCollisionFlags(pRigidBody->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

		SPhysics_AddShapeEx(pRigidBody, CG_HITBOX, CG_BULLETFIRE);
		m_pHitboxBodies[i] = pRigidBody;
	}

	updateHitboxes();
}

void CBaseCharacter::updateHitboxes()
{
	if(!m_pModel || !m_pHitboxBodies)
	{
		return;
	}

	auto pAnimatedModel = m_pModel->asAnimatedModel();
	if(!pAnimatedModel || !pAnimatedModel->isPlayingAnimations())
	{
		return;
	}
	//@TODO: Reimplement me
#if 0
	const XResourceModelHitbox * hb;
	for(int i = 0, l = pAnimatedModel->getHitboxCount(); i < l; ++i)
	{
		hb = pAnimatedModel->getHitbox(i);
		
		//SMMATRIX mBone = m_pAnimPlayer->getBoneTransformPos(hb->bone_id);

		//@TODO: Cache hitbox transform
		m_pHitboxBodies[i]->getWorldTransform().setFromOpenGLMatrix((btScalar*)&(SMMatrixRotationX(hb->rot.x)
			* SMMatrixRotationY(hb->rot.y)
			* SMMatrixRotationZ(hb->rot.z)
			* SMMatrixTranslation(hb->pos)
			* pAnimatedModel->getBoneTransform(hb->bone_id, true)
			* getWorldTM()
			));
	}
#endif
}

void CBaseCharacter::releaseHitboxes()
{
	if(!m_pModel || !m_pHitboxBodies)
	{
		return;
	}
	auto pAnimatedModel = m_pModel->asAnimatedModel();
	if(!pAnimatedModel)
	{
		return;
	}

	for(int i = 0, l = pAnimatedModel->getHitboxCount(); i < l; ++i)
	{
		SPhysics_RemoveShape(m_pHitboxBodies[i]);

		btMotionState * motionState = m_pHitboxBodies[i]->getMotionState();

		mem_delete(motionState);

		btCollisionShape * pShape = m_pHitboxBodies[i]->getCollisionShape();
		mem_delete(pShape);
		mem_delete(m_pHitboxBodies[i]);
	}

	mem_delete_a(m_pHitboxBodies);
}

float3 CBaseCharacter::getHeadOffset()
{
	float3 vHeadOffset; // = m_pHeadEnt->getOffsetPos();
	vHeadOffset.y = m_fCapsHeight * m_fCurrentHeight - 0.1f;
	return(vHeadOffset);
}

void CBaseCharacter::onPhysicsStep()
{
	updateHitboxes();

	if(m_uMoveDir & PM_OBSERVER)
	{
		return;
	}
	btTransform &trans = m_pGhostObject->getWorldTransform();
	setPos(float3(trans.getOrigin().x(), trans.getOrigin().y() - m_fCapsHeight * m_fCurrentHeight * 0.5f, trans.getOrigin().z()));

	m_pHeadEnt->setOffsetPos(getHeadOffset());

#if 0
	//находим текущий квад аи сетки на котором находится игрок
	ID idq = SAIG_QuadGet(&float3(m_vPosition), true);
	//если нашли
	if(ID_VALID(idq))
	{
		//занимаем этот квад
		SAIG_QuadSetState(idq, AIQUAD_STATE_TEMPBUSY);
		SAIG_QuadSetStateWho(idq, getId());
		m_idQuadLast = idq;
	}

	//если предыдущий и текущие квады не идентичны
	if(m_idQuadCurr != idq)
	{
		//если предыдщий был действительным, убираем занятость
		if(ID_VALID(m_idQuadCurr))
		{
			SAIG_QuadSetState(m_idQuadCurr, AIQUAD_STATE_FREE);
		}

		m_idQuadCurr = idq;
	}
#endif
}

void CBaseCharacter::initPhysics()
{
	initHitboxes();
}

void CBaseCharacter::releasePhysics()
{
	releaseHitboxes();
}

CHUDcontroller * CBaseCharacter::getHUDcontroller()
{
	return(NULL);
}

void CBaseCharacter::onDeath(CBaseEntity *pAttacker, CBaseEntity *pInflictor)
{
	if(m_idQuadCurr >= 0)
	{
		SAIG_QuadSetState(m_idQuadCurr, AIQUAD_STATE_FREE);
		m_idQuadCurr = -1;
	}

	SPhysics_GetDynWorld()->removeCollisionObject(m_pGhostObject);
	SPhysics_GetDynWorld()->removeAction(m_pCharacter);

	cancelNextAnimation();

	BaseClass::onDeath(pAttacker, pInflictor);
}

/*void CBaseCharacter::dispatchDamage(CTakeDamageInfo &takeDamageInfo)
{
	// adjust damage by bodypart
	BaseClass::dispatchDamage(takeDamageInfo);
}*/

CBaseEntity * CBaseCharacter::getHead()
{
	return(m_pHeadEnt);
}

bool CBaseCharacter::isObserver()
{
	return((m_uMoveDir & PM_OBSERVER) != 0);
}

void CBaseCharacter::use(bool start)
{
	if(isObserver())
	{
		return;
	}

	if(start)
	{
		float3 start = getHead()->getPos();
		float3 dir = getHead()->getOrient() * float3(0.0f, 0.0f, 1.0f);
		float3 end = start + dir * 2.0f;

		btKinematicClosestNotMeRayResultCallback cb(m_pGhostObject, F3_BTVEC(start), F3_BTVEC(end));
		SPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

		if(cb.hasHit())
		{
			CBaseEntity *pEnt = (CBaseEntity*)cb.m_collisionObject->getUserPointer();
			if(pEnt)
			{
				pEnt->onUse(this);
			}
		}
	}
}

void CBaseCharacter::onInventoryChanged()
{
	if(m_pActiveTool)
	{
		m_pActiveTool->updateHUDinfo();
	}
}
