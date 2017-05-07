#ifndef SXPHYWORLD_H
#define SXPHYWORLD_H


#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

class PhyWorld
{
public:
	PhyWorld();
	~PhyWorld();

	void SetThreadNum(int tnum);
	void Update(int thread = 0);
	void Sync();


protected:
	btDefaultCollisionConfiguration * m_pCollisionConfiguration;
	btCollisionDispatcher * m_pDispatcher;
	btBroadphaseInterface * m_pBroadphase;
	btSequentialImpulseConstraintSolver * m_pSolver;
	btDiscreteDynamicsWorld * m_pDynamicsWorld;
	btGhostPairCallback * m_pGHostPairCallback;

	const bool * m_bDebugDraw;
};

#endif
