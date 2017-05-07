#include "PhyWorld.h"
#include <core/sxcore.h>

PhyWorld::PhyWorld()
{
	printf("Initializing physics engine...   ");
	m_pBroadphase = new btDbvtBroadphase();
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
	m_pSolver = new btSequentialImpulseConstraintSolver;
	m_pGHostPairCallback = new btGhostPairCallback();
	m_pBroadphase->getOverlappingPairCache()->setInternalGhostPairCallback(m_pGHostPairCallback);
	m_pDynamicsWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
	
	Core_0RegisterCVarString("phy_world_gravity", "0 -10 0", "World gravity (x y z)");

	
	
	//m_pDebugDrawer = new DebugDrawer();
	//m_pDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	//m_pDebugDrawer->setDebugMode(btIDebugDraw::DBG_FastWireframe);
	//m_pDynamicsWorld->setDebugDrawer(m_pDebugDrawer);

	Core_0RegisterCVarBool("phy_debugdraw", false, "Debug drawing physics shapes");
	m_bDebugDraw = GET_PCVAR_BOOL("phy_debugdraw");
	printf("Done!\n");
}

PhyWorld::~PhyWorld()
{
	mem_delete(m_pDynamicsWorld);
	mem_delete(m_pGHostPairCallback);
	mem_delete(m_pSolver);
	mem_delete(m_pDispatcher);
	mem_delete(m_pCollisionConfiguration);
	mem_delete(m_pBroadphase);
}

void PhyWorld::SetThreadNum(int tnum)
{
}
void PhyWorld::Update(int thread)
{
}
void PhyWorld::Sync()
{
}