#ifndef SXPHYWORLD_H
#define SXPHYWORLD_H


#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <common/AssotiativeArray.h>
#include <common/Array.h>

#include <gdefines.h>

#include <common/SXMath.h>
#include <mtllight/sxmtllight.h>

#define PHY_MAT_FILE_MAGICK 8386069164979148883

#pragma pack(push,1)
struct PhyMatFile
{
	int64_t i64Magick;
	uint32_t uiGeomFaceCount;
	//uint32_t uiGreenObjCount;

	PhyMatFile():
		i64Magick(PHY_MAT_FILE_MAGICK)
	{
	}
};
#pragma pack(pop)

extern report_func reportf;

class PhyWorld
{
public:
	PhyWorld();
	~PhyWorld();

	void SetThreadNum(int tnum);
	void Update(int thread = 0);
	void Sync();

	void AddShape(btRigidBody * pBody);
	void RemoveShape(btRigidBody * pBody);

	void LoadGeom(const char * file=NULL);
	void UnloadGeom();

	bool ImportGeom(const char * file);
	bool ExportGeom(const char * file);

	void Render();

	MTLTYPE_PHYSIC GetMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);

	btDiscreteDynamicsWorld * GetBtWorld()
	{
		return(m_pDynamicsWorld);
	}

	class DebugDrawer: public btIDebugDraw
	{
		struct render_point
		{
			float3_t pos;
			DWORD clr;
		};
		int m_iDebugMode;
		Array<render_point, 16384> m_vDrawData;
	public:
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

		void reportErrorWarning(const char* warningString);

		void draw3dText(const btVector3& location, const char* textString);

		void setDebugMode(int debugMode);

		int getDebugMode() const;

		void Render();
	};

protected:
	btDefaultCollisionConfiguration * m_pCollisionConfiguration;
	btCollisionDispatcher * m_pDispatcher;
	btBroadphaseInterface * m_pBroadphase;
	btSequentialImpulseConstraintSolver * m_pSolver;
	btDiscreteDynamicsWorld * m_pDynamicsWorld;
	btGhostPairCallback * m_pGHostPairCallback;

	const bool * m_bDebugDraw;
	DebugDrawer * m_pDebugDrawer;

	// физические формы для статической геометрии уровня
	btTriangleMesh * m_pGeomStaticCollideMesh;
	btCollisionShape * m_pGeomStaticCollideShape;
	btRigidBody * m_pGeomStaticRigidBody;
	MTLTYPE_PHYSIC *m_pGeomMtlTypes;
	int m_iGeomFacesCount;
	int m_iGeomModelCount;

	btCollisionShape ** m_ppGreenStaticCollideShape;
	btRigidBody *** m_pppGreenStaticRigidBody;
	int m_iGreenShapes;
	int * m_piGreenTotal;
};

#endif
