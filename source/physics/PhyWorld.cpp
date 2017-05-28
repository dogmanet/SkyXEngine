#include "PhyWorld.h"
#include <core/sxcore.h>
#include <geom/sxgeom.h>
#include <gcore/sxgcore.h>

PhyWorld::PhyWorld():
	m_pGeomStaticCollideMesh(NULL),
	m_pGeomStaticCollideShape(NULL),
	m_pGeomStaticRigidBody(NULL)
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
	m_pDynamicsWorld->setGravity(btVector3(0, -10, 0));
		
	m_pDebugDrawer = new DebugDrawer();
	m_pDebugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	//m_pDebugDrawer->setDebugMode(btIDebugDraw::DBG_FastWireframe);
	m_pDynamicsWorld->setDebugDrawer(m_pDebugDrawer);

	Core_0RegisterCVarBool("r_physdebug", false, "Debug drawing physics shapes");
	m_bDebugDraw = GET_PCVAR_BOOL("r_physdebug");
	printf("Done!\n");



	{
		m_pCollideShape = new btSphereShape(1.0f);

		btVector3 vInertia;
		const float fMass = 4.0f;
		m_pCollideShape->calculateLocalInertia(fMass, vInertia);

		btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0f, 10.0f, 0.0f)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			fMass,                  // mass
			motionState,        // initial position
			m_pCollideShape,    // collision shape of body
			vInertia  // local inertia
			);
		m_pRigidBody = new btRigidBody(rigidBodyCI);

		m_pRigidBody->setFriction(100.0f);

		AddShape(m_pRigidBody);
	}

	{
		m_pCollideShape = new btSphereShape(0.8f);

		btVector3 vInertia;
		const float fMass = 4.0f;
		m_pCollideShape->calculateLocalInertia(fMass, vInertia);

		btDefaultMotionState * motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0f, 15.0f, 0.0f)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			fMass,                  // mass
			motionState,        // initial position
			m_pCollideShape,    // collision shape of body
			vInertia  // local inertia
			);
		m_pRigidBody = new btRigidBody(rigidBodyCI);

		m_pRigidBody->setFriction(100.0f);

		AddShape(m_pRigidBody);
	}
}

PhyWorld::~PhyWorld()
{
	RemoveShape(m_pGeomStaticRigidBody);
	mem_delete(m_pGeomStaticRigidBody);
	mem_delete(m_pGeomStaticCollideShape);
	mem_delete(m_pGeomStaticCollideMesh);


	mem_delete(m_pDynamicsWorld);
	mem_delete(m_pGHostPairCallback);
	mem_delete(m_pSolver);
	mem_delete(m_pDispatcher);
	mem_delete(m_pCollisionConfiguration);
	mem_delete(m_pBroadphase);
}

void PhyWorld::Render()
{
	if(*m_bDebugDraw)
	{
		m_pDynamicsWorld->debugDrawWorld();
		((DebugDrawer*)(m_pDynamicsWorld->getDebugDrawer()))->Render();
	}
}

void PhyWorld::SetThreadNum(int tnum)
{
}
void PhyWorld::Update(int thread)
{
	static UINT time0 = GetTickCount();
	UINT time1 = GetTickCount();

	if(time1 - time0 > 5000)
	{
		time0 = time1;
	}
	m_pDynamicsWorld->stepSimulation((float)(time1 - time0) / 1000.0f, 0, 1.0f / 60.0f);

	time0 = time1;
}
void PhyWorld::Sync()
{
}

void PhyWorld::AddShape(btRigidBody * pBody)
{
	m_pDynamicsWorld->addRigidBody(pBody);
}

void PhyWorld::RemoveShape(btRigidBody * pBody)
{
	m_pDynamicsWorld->removeRigidBody(pBody);
}

void PhyWorld::LoadGeom()
{
	if(m_pGeomStaticRigidBody)
	{
		RemoveShape(m_pGeomStaticRigidBody);
	}
	mem_delete(m_pGeomStaticRigidBody);
	mem_delete(m_pGeomStaticCollideShape);
	mem_delete(m_pGeomStaticCollideMesh);

	float3_t ** ppVertices;
	int32_t * pVertexCount;
	uint32_t ** ppIndices;
	int32_t * pIndexCount;

	int32_t iModelCount;

	SGeom_ModelsGetArrBuffsGeom(&ppVertices, &pVertexCount, &ppIndices, &pIndexCount, &iModelCount);

	
	m_pGeomStaticCollideMesh = new btTriangleMesh(true, false);

	uint32_t IC = 0, VC = 0;
	for(int32_t tc = 0; tc < iModelCount; ++tc)
	{
		IC += pIndexCount[tc];
		VC += pVertexCount[tc];
	}

	m_pGeomStaticCollideMesh->preallocateIndices(IC);
	m_pGeomStaticCollideMesh->preallocateVertices(VC);
	IC = 0;
	VC = 0;
	
	for(int32_t tc = 0; tc < iModelCount; ++tc)
	{
		for(int i = 0; i < pVertexCount[tc]; ++i)
		{
			m_pGeomStaticCollideMesh->findOrAddVertex(F3_BTVEC(ppVertices[tc][i]), false);
		}
		for(int i = 0; i < pIndexCount[tc]; i += 3)
		{
			m_pGeomStaticCollideMesh->addTriangleIndices(ppIndices[tc][i] + VC, ppIndices[tc][i + 1] + VC, ppIndices[tc][i + 2] + VC);
		}
		IC += pIndexCount[tc];
		VC += pVertexCount[tc];
	}

	if(m_pGeomStaticCollideMesh->getNumTriangles() != 0)
	{
		m_pGeomStaticCollideShape = new btBvhTriangleMeshShape(m_pGeomStaticCollideMesh, true);
		btDefaultMotionState motionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0.0f, 0.0f, 0.0f)));
		btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			0,                  // mass
			&motionState,        // initial position
			m_pGeomStaticCollideShape,    // collision shape of body
			btVector3(0, 0, 0)  // local inertia
			);
		m_pGeomStaticRigidBody = new btRigidBody(rigidBodyCI);
		
		m_pGeomStaticRigidBody->setCollisionFlags(m_pGeomStaticRigidBody->getCollisionFlags() | btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
		m_pGeomStaticRigidBody->setFriction(100.0f);

		AddShape(m_pGeomStaticRigidBody);
	}

	SGeom_ModelsClearArrBuffsGeom(ppVertices, pVertexCount, ppIndices, pIndexCount, iModelCount);
}




//##############################################################

void PhyWorld::DebugDrawer::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	int clr = 0;
	clr += 255;
	clr <<= 8;
	clr += (int)(color.getX() * 255.0f);
	clr <<= 8;
	clr += (int)(color.getY() * 255.0f);
	clr <<= 8;
	clr += (int)(color.getZ() * 255.0f);
	clr <<= 8;

	render_point pt;
	pt.clr = clr;

	pt.pos = float3_t(from.x(), from.y(), from.z());
	m_vDrawData.push_back(pt);

	pt.pos = float3_t(to.x(), to.y(), to.z());
	m_vDrawData.push_back(pt);
}

void PhyWorld::DebugDrawer::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

}

void PhyWorld::DebugDrawer::reportErrorWarning(const char * warningString)
{
	reportf(REPORT_MSG_LEVEL_WARRNING, "%s\n", warningString);
}

void PhyWorld::DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{

}

void PhyWorld::DebugDrawer::setDebugMode(int debugMode)
{
	m_iDebugMode = debugMode;
}

int PhyWorld::DebugDrawer::getDebugMode() const
{
	return(m_iDebugMode);
}

void PhyWorld::DebugDrawer::Render()
{
	if(!m_vDrawData.size())
	{
		return;
	}
	SGCore_ShaderUnBind();

	SMMATRIX mView, mProj;
	Core_RMatrixGet(G_RI_MATRIX_VIEW, &mView);
	Core_RMatrixGet(G_RI_MATRIX_PROJECTION, &mProj);

	SGCore_GetDXDevice()->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&SMMatrixIdentity());
	SGCore_GetDXDevice()->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&mView);
	SGCore_GetDXDevice()->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mProj);

	SGCore_GetDXDevice()->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	SGCore_GetDXDevice()->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFF);

	SGCore_GetDXDevice()->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

	SGCore_GetDXDevice()->SetTexture(0, 0);
	
	SGCore_GetDXDevice()->DrawPrimitiveUP(D3DPT_LINELIST, m_vDrawData.size() / 2, &(m_vDrawData[0]), sizeof(render_point));

	m_vDrawData.clear();
}
