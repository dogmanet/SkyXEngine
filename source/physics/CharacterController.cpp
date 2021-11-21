#include "CharacterController.h"
#include "CollisionObject.h"


CCharacterController::CCharacterController(IXGhostObject *pGhostObject, float fStepHeight):
	m_pGhostObject(pGhostObject)
{
	add_ref(pGhostObject);
	btGhostObject *pGhost = ((CGhostObject*)pGhostObject)->getBtGhostObject();
	m_pController = new btKinematicCharacterController((btPairCachingGhostObject*)pGhost, (btConvexShape*)pGhost->getCollisionShape(), fStepHeight, btVector3(0.0f, 1.0f, 0.0f));
}

CCharacterController::~CCharacterController()
{
	unregisterInWorld();

	mem_delete(m_pController);
	mem_release(m_pGhostObject);
}

void XMETHODCALLTYPE CCharacterController::setVelocityForTimeInterval(const float3 &vSpeed, float fTime)
{
	m_pController->setVelocityForTimeInterval(F3_BTVEC(vSpeed), fTime);
}
void XMETHODCALLTYPE CCharacterController::setWalkDirection(const float3 &vSpeed)
{
	m_pController->setWalkDirection(F3_BTVEC(vSpeed));
}

bool XMETHODCALLTYPE CCharacterController::onGround() const
{
	return(m_pController->onGround());
}
bool XMETHODCALLTYPE CCharacterController::canStandUp(float fDelta) const
{
	return(m_pController->canStandUp(fDelta));
}
bool XMETHODCALLTYPE CCharacterController::canJump() const
{
	return(m_pController->canJump());
}
void XMETHODCALLTYPE CCharacterController::jump()
{
	m_pController->jump();
}

float3 XMETHODCALLTYPE CCharacterController::getLinearVelocity() const
{
	btVector3 vec = m_pController->getLinearVelocity();
	return(BTVEC_F3(vec));
}

void XMETHODCALLTYPE CCharacterController::registerInWorld(IXPhysicsWorld *pWorld)
{
	unregisterInWorld();

	m_pWorld = ((CPhyWorld*)pWorld)->getBtWorld();
	m_pWorld->addAction(m_pController);
}
void XMETHODCALLTYPE CCharacterController::unregisterInWorld()
{
	if(m_pWorld)
	{
		m_pWorld->removeAction(m_pController);
	}
}

void XMETHODCALLTYPE CCharacterController::setMaxJumpHeight(float fHeight) const
{
	m_pController->setMaxJumpHeight(fHeight);
}
void XMETHODCALLTYPE CCharacterController::setJumpSpeed(float fSpeed) const
{
	m_pController->setJumpSpeed(fSpeed);
}
void XMETHODCALLTYPE CCharacterController::setGravity(const float3_t &vSpeed) const
{
	m_pController->setGravity(F3_BTVEC(vSpeed));
}
void XMETHODCALLTYPE CCharacterController::setFallSpeed(float fSpeed) const
{
	m_pController->setFallSpeed(fSpeed);
}
void XMETHODCALLTYPE CCharacterController::setMaxPenetrationDepth(float fMaxDepth) const
{
	m_pController->setMaxPenetrationDepth(fMaxDepth);
}
