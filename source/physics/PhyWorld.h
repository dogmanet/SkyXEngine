
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#ifndef __PHY_WORLD_H
#define __PHY_WORLD_H


#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolverMt.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcherMt.h>

#include <common/AssotiativeArray.h>
#include <common/Array.h>
#include <common/file_utils.h>
#include <xcommon/IXRenderable.h>

#include <gdefines.h>

#include <common/Math.h>
#include <mtrl/sxmtrl.h>

#define PHY_MAT_FILE_MAGICK 3630267958475905107

#pragma pack(push,1)
struct PhyMatFile
{
	int64_t i64Magick;
	uint32_t uiGeomFaceCount;
	uint32_t uiMatCount;
	//uint32_t uiGreenObjCount;

	PhyMatFile():
		i64Magick(PHY_MAT_FILE_MAGICK)
	{
	}
};
#pragma pack(pop)

class CPhyWorld
{
public:
	CPhyWorld();
	~CPhyWorld();

	void setThreadNum(int tnum);
	void update(int thread = 0);
	void sync();

	void addShape(btRigidBody * pBody);
	void addShape(btRigidBody * pBody, int group, int mask);
	void removeShape(btRigidBody * pBody);

	void updateSingleAABB(btCollisionObject* colObj);

#if 0
	void loadGeom(const char * file=NULL);
	void unloadGeom();

	bool importGeom(const char * file);
	bool exportGeom(const char * file);
#endif

	void disableSimulation();
	void enableSimulation();

	void render();

	MTLTYPE_PHYSIC getMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);
	
	ID getMtlID(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);

	btDiscreteDynamicsWorldMt * getBtWorld()
	{
		return(m_pDynamicsWorld);
	}

	class CDebugDrawer: public btIDebugDraw
	{
		struct render_point
		{
			float3_t pos;
			float4_t clr;
		};
		int m_iDebugMode = 0;
		render_point m_pDrawData[4096];
		UINT m_uDataSize = 4096;
		UINT m_uDataPointer = 0;

		bool m_bExpectObject = false;

		IGXVertexBuffer *m_pVertexBuffer = NULL;
		IGXVertexDeclaration *m_pVertexDeclaration = NULL;
		IGXRenderBuffer *m_pRenderBuffer = NULL;
		IGXConstantBuffer *m_pVSConstantBuffer = NULL;
		ID m_idShader = -1;
	public:
		CDebugDrawer();
		~CDebugDrawer();

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

		void reportErrorWarning(const char* warningString);

		void draw3dText(const btVector3& location, const char* textString);

		void setDebugMode(int debugMode);

		int getDebugMode() const;

		void begin();
		void render();
		void commit();
	};

	class CRenderable: public IXUnknownImplementation<IXRenderable>
	{
	public:
		CRenderable(CPhyWorld *pWorld):
			m_pWorld(pWorld)
		{
		}

		XIMPLEMENT_VERSION(IXRENDERABLE_VERSION); 

		bool XMETHODCALLTYPE isVisThreadOptimized() override
		{
			return(false);
		}

		X_RENDER_STAGE XMETHODCALLTYPE getStages() override
		{
			return(XRS_GBUFFER);
		}

		UINT XMETHODCALLTYPE getPriorityForStage(X_RENDER_STAGE stage) override
		{
			return(1);
		}

		void XMETHODCALLTYPE renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility) override;

		UINT XMETHODCALLTYPE getTransparentCount(IXRenderableVisibility *pVisibility) override
		{
			return(0);
		}
		void XMETHODCALLTYPE getTransparentObject(IXRenderableVisibility *pVisibility, UINT uIndex, XTransparentObject *pObject) override
		{
		}
		void XMETHODCALLTYPE renderTransparentObject(IXRenderableVisibility *pVisibility, UINT uIndex, UINT uSplitPlanes) override
		{
		}


		void XMETHODCALLTYPE startup(IGXDevice *pDevice, IXMaterialSystem *pMaterialSystem) override;
		void XMETHODCALLTYPE shutdown() override
		{
		}
		void XMETHODCALLTYPE newVisData(IXRenderableVisibility **ppVisibility) override
		{
			*ppVisibility = NULL;
		}

	protected:
		CPhyWorld *m_pWorld;
	};

protected:
	btDefaultCollisionConfiguration * m_pCollisionConfiguration;
	btCollisionDispatcherMt * m_pDispatcher;
	btBroadphaseInterface * m_pBroadphase;
	btSequentialImpulseConstraintSolverMt * m_pSolver;
	btDiscreteDynamicsWorldMt * m_pDynamicsWorld;
	btGhostPairCallback * m_pGHostPairCallback;

	bool m_isUpdating = false;

	const bool * m_bDebugDraw;
	CDebugDrawer * m_pDebugDrawer;

	// физические формы для статической геометрии уровня
	btTriangleMesh * m_pGeomStaticCollideMesh;
	btCollisionShape * m_pGeomStaticCollideShape;
	btRigidBody * m_pGeomStaticRigidBody;
	//MTLTYPE_PHYSIC *m_pGeomMtlTypes;
	ID *m_pGeomMtlIDs;
	int m_iGeomFacesCount;
	int m_iGeomModelCount;

	btCollisionShape ** m_ppGreenStaticCollideShape;
	btRigidBody *** m_pppGreenStaticRigidBody;
	int m_iGreenShapes;
	int * m_piGreenTotal;

	bool m_isRunning;
	int m_iSkipFrames = 3;

	IEventChannel<XEventPhysicsStep> *m_pTickEventChannel = NULL;

	static void TickCallback(btDynamicsWorld *world, btScalar timeStep);
};

#endif
