
#include "BaseCharacter.h"
#include "GameData.h"
#include "BaseTool.h"
#include "BaseWeapon.h"

/*! \skydocent base_character
Базовый класс персонажа
*/

BEGIN_PROPTABLE(CBaseCharacter)
	// empty
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CBaseCharacter, base_character);

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

CBaseCharacter::CBaseCharacter(CEntityManager * pMgr):
	BaseClass(pMgr),
	m_uMoveDir(PM_OBSERVER),
	m_vPitchYawRoll(float3_t(0, 0, 0)),
	m_pActiveTool(NULL),
	m_fCurrentSpread(0.0f)
{
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(F3_BTVEC(m_vPosition));
	//startTransform.setOrigin(btVector3(0, 12, 10));

	m_pGhostObject = new btPairCachingGhostObject();
	m_pGhostObject->setWorldTransform(startTransform);
	//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	m_pCollideShape = new btCapsuleShape(0.4f, 1.0f);
	m_pGhostObject->setCollisionShape(m_pCollideShape);
	m_pGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	m_pGhostObject->setUserPointer(this);

	btScalar stepHeight = 0.4f;
	m_pCharacter = new btKinematicCharacterController(m_pGhostObject, (btConvexShape*)m_pCollideShape, stepHeight, btVector3(0.0f, 1.0f, 0.0f));
	m_pCharacter->setMaxJumpHeight(0.60f);
	m_pCharacter->setJumpSpeed(3.50f);
	//m_pCharacter->setJumpSpeed(3.5f);
	m_pCharacter->setGravity(btVector3(0, -10.0f, 0));
	//m_pCharacter->setGravity(1.0f);
	m_pCharacter->setFallSpeed(300.0f);
	//m_pCharacter->setFallSpeed(30.0f);

	SXPhysics_GetDynWorld()->addCollisionObject(m_pGhostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::DebrisFilter);

	m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);

	SXPhysics_GetDynWorld()->addAction(m_pCharacter);


	m_flashlight = (CLightDirectional*)CREATE_ENTITY("light_directional", m_pMgr);
	//m_flashlight->setPos(getPos() + float3(0.f, 0.1f, 0.f));
	m_flashlight->setPos(getPos() + float3(0.f, 0.2f, 0.1f));
	m_flashlight->setOrient(getOrient() * SMQuaternion(SM_PIDIV2, 'x'));
	m_flashlight->setParent(this);
	m_flashlight->setDist(20.f);
	m_flashlight->setAngle(SMToRadian(60));
	m_flashlight->setColor(float3(3.5, 3.5, 3.5));
	//m_flashlight->setShadowType(-1);
	m_flashlight->setShadowType(1);

	m_idTaskSpread = SET_INTERVAL(updateSpread, 1.0f / 30.0f);
}

CBaseCharacter::~CBaseCharacter()
{
	CLEAR_INTERVAL(m_idTaskSpread);
	REMOVE_ENTITY(m_flashlight);
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
	if(m_pActiveTool)
	{
		//@FIXME: Add correct call
		//m_pActiveTool->nextFireMode();
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
			float3 start = getPos(),
				end = start + float3(0.0f, -2.0f, 0.0f);
			btKinematicClosestNotMeRayResultCallback cb(m_pGhostObject, F3_BTVEC(start), F3_BTVEC(end));
			SXPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

			if(cb.hasHit() && cb.m_shapeInfo.m_shapePart == 0 && cb.m_shapeInfo.m_triangleIndex >= 0)
			{
				MTLTYPE_PHYSIC type = (MTLTYPE_PHYSIC)SXPhysics_GetMtlType(cb.m_collisionObject, &cb.m_shapeInfo);
				g_pGameData->playFootstepSound(type, BTVEC_F3(cb.m_hitPointWorld));
			}
		}
	}
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
