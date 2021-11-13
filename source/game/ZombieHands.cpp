
#include "ZombieHands.h"
#include "BaseCharacter.h"
#include <decals/sxdecals.h>
#include "Tracer.h"

/*! \skydocent wpn_zombie_hands
Руки зомби
*/

BEGIN_PROPTABLE(CZombieHands)
	//! Наносимый урон
	DEFINE_FIELD_FLOAT(m_fDamage, PDFF_NOEDIT | PDFF_NOEXPORT, "damage", "", EDITOR_NONE)
END_PROPTABLE()

REGISTER_ENTITY_NOLISTING(CZombieHands, wpn_zombie_hands);


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

void CZombieHands::taskShoot(float dt)
{
	extern CTracer *g_pTracer;

	CBaseCharacter * pOwner = (CBaseCharacter*)getOwner();

	pOwner->playActivity("ACT_SHOOT", 200);
	pOwner->playActivityNext("ACT_IDLE", 200);

	SAFE_CALL(m_pSndShoot, setWorldPos, getPos());
	SAFE_CALL(m_pSndShoot, play);

	m_idActualShootTimeout = SET_TIMEOUT(actualShoot, 0.6f);
}

void CZombieHands::actualShoot(float dt)
{
	m_idActualShootTimeout = -1;

	CBaseCharacter * pOwner = (CBaseCharacter*)getOwner();

	float3 start = getPos() + float3(0.0f, -0.2f, 0.0f);
	float3 dir = m_pParent->getParent()->getOrient() * float3(0.0f, 0.0f, -1.0f);

	dir = applySpread(dir, SMToRadian(pOwner->getCurrentSpread()));

	float3 end = start + dir * m_fEffectiveDistance;
	btKinematicClosestNotMeRayResultCallback cb(pOwner->getBtCollisionObject(), F3_BTVEC(start), F3_BTVEC(end));
	SPhysics_GetDynWorld()->rayTest(F3_BTVEC(start), F3_BTVEC(end), cb);

	//g_pTracer->begin(start);
	//g_pTracer->lineTo(end);
	//g_pTracer->end();

	if(cb.hasHit())
	{
		SXDecals_ShootDecal(DECAL_TYPE_BLOOD_BIG, BTVEC_F3(cb.m_hitPointWorld), BTVEC_F3(cb.m_hitNormalWorld));

		if(cb.m_collisionObject->getUserPointer() && cb.m_collisionObject->getUserIndex() == 1)
		{
			CTakeDamageInfo takeDamageInfo(pOwner, m_fDamage);
			takeDamageInfo.m_pInflictor = this;

			((CBaseEntity*)cb.m_collisionObject->getUserPointer())->dispatchDamage(takeDamageInfo);
		}
	}
}

void CZombieHands::stopAction()
{
	if(ID_VALID(m_idActualShootTimeout))
	{
		CLEAR_TIMEOUT(m_idActualShootTimeout);
	}
}
