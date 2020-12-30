
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "sxphysics.h"
#include "PhyWorld.h"
#include "Physics.h"

#include <core/sxcore.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

#include "MutationObserver.h"

#if defined(_DEBUG)
#	pragma comment(lib, "sxcore_d.lib")
#	pragma comment(lib, "sxmtrl_d.lib")
#	pragma comment(lib, "BulletDynamics_vs2010_debug.lib")
#	pragma comment(lib, "BulletCollision_vs2010_debug.lib")
#	pragma comment(lib, "LinearMath_vs2010_debug.lib")
#	pragma comment(lib, "BulletWorldImporter_vs2010_debug.lib")
#	pragma comment(lib, "BulletWorldImporter_vs2010_debug.lib")
#	pragma comment(lib, "BulletFileLoader_vs2010_debug.lib")
#else
#	pragma comment(lib, "sxcore.lib")
#	pragma comment(lib, "sxmtrl.lib")
#	pragma comment(lib, "BulletDynamics_vs2010.lib")
#	pragma comment(lib, "BulletCollision_vs2010.lib")
#	pragma comment(lib, "LinearMath_vs2010.lib")
#	pragma comment(lib, "BulletWorldImporter_vs2010.lib")
#	pragma comment(lib, "BulletFileLoader_vs2010.lib")
#endif

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

CPhyWorld *g_pWorld = NULL;
CPhysics *g_pPhysics = NULL;

#define SP_PRECOND(ret) if(!g_pWorld){LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxphysics is not init", GEN_MSG_LOCATION);return ret;}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

SX_LIB_API void SPhysics_DumpStats()
{
	CProfileManager::dumpAll();
}

SX_LIB_API void SPhysics_0Create()
{
	if(g_pWorld)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxphysics double init", GEN_MSG_LOCATION);
		return;
	}
	Core_SetOutPtr();

	g_pWorld = new CPhyWorld();
	g_pPhysics = new CPhysics();

	Core_0RegisterConcmd("perf_physics", SPhysics_DumpStats);
}
SX_LIB_API void SPhysics_AKill()
{
	SP_PRECOND(_VOID);
	mem_delete(g_pPhysics);
	mem_delete(g_pWorld);
}

SX_LIB_API IXPhysics* SPhysics_GetIXPhysics()
{
	return(g_pPhysics);
}

SX_LIB_API void SPhysics_Update(int thread)
{
	SP_PRECOND(_VOID);
	g_pWorld->update(thread);
}
SX_LIB_API void SPhysics_UpdateSetThreadNum(int num)
{
	SP_PRECOND(_VOID);
	g_pWorld->setThreadNum(num);
}
SX_LIB_API void SPhysics_Sync()
{
	SP_PRECOND(_VOID);
	g_pWorld->sync();
}

SX_LIB_API void SPhysics_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

#if 0
SX_LIB_API void SPhysics_LoadGeom(const char * file)
{
	SP_PRECOND(_VOID);
	g_pWorld->loadGeom(file);
}

SX_LIB_API void SPhysics_UnloadGeom()
{
	SP_PRECOND(_VOID);
	g_pWorld->unloadGeom();
}
#endif

SX_LIB_API void SPhysics_AddShape(btRigidBody * pBody)
{
	SP_PRECOND(_VOID);
	g_pWorld->addShape(pBody);
}

SX_LIB_API void SPhysics_AddShapeEx(btRigidBody * pBody, int group, int mask)
{
	SP_PRECOND(_VOID);
	g_pWorld->addShape(pBody, group, mask);
}

SX_LIB_API void SPhysics_RemoveShape(btRigidBody * pBody)
{
	SP_PRECOND(_VOID);
	g_pWorld->removeShape(pBody);
}

SX_LIB_API btDiscreteDynamicsWorldMt * SPhysics_GetDynWorld()
{
	SP_PRECOND(NULL);
	return(g_pWorld->getBtWorld());
}

SX_LIB_API void SPhysics_UpdateSingleAABB(btCollisionObject* colObj)
{
	SP_PRECOND(_VOID);
	g_pWorld->updateSingleAABB(colObj);
}

#if 0
SX_LIB_API bool SPhysics_ImportGeom(const char * file)
{
	SP_PRECOND(false);
	return(g_pWorld->importGeom(file));
}

SX_LIB_API bool SPhysics_ExportGeom(const char * file)
{
	SP_PRECOND(false);
	return(g_pWorld->exportGeom(file));
}
#endif

SX_LIB_API int SPhysics_GetMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo)
{
	SP_PRECOND(MTLTYPE_PHYSIC_DEFAULT);

	return(g_pWorld->getMtlType(pBody, pShapeInfo));
}

SX_LIB_API ID SPhysics_GetMtlID(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo)
{
	SP_PRECOND(-1);

	return(g_pWorld->getMtlID(pBody, pShapeInfo));
}

SX_LIB_API void SPhysics_EnableSimulation()
{
	SP_PRECOND(_VOID);

	g_pWorld->enableSimulation();
}

SX_LIB_API void SPhysics_DisableSimulation()
{
	SP_PRECOND(_VOID);

	g_pWorld->disableSimulation();
}

SX_LIB_API void SPhysics_BuildHull(btConvexHullShape *pIn, btVector3 **ppOut, int *pNumVertices)
{
	btShapeHull tmpHull(pIn);
	if(tmpHull.buildHull(0))
	{
		*pNumVertices = tmpHull.numVertices();
		*ppOut = new btVector3[*pNumVertices];
		memcpy(*ppOut, tmpHull.getVertexPointer(), sizeof(btVector3)* *pNumVertices);
		//return(new btConvexHullShape((const btScalar*)tmpHull.getVertexPointer(), tmpHull.numVertices(), sizeof(btVector3)));
	}
	else
	{
		*pNumVertices = 0;
		*ppOut = NULL;
	}
}

SX_LIB_API void SPhysics_ReleaseHull(btVector3 *pData, int iNumVertices)
{
	mem_delete_a(pData);
}

SX_LIB_API btCollisionShape* SPhysics_CreateTrimeshShape(const XResourceModelStaticSubset *pSubset)
{
	btTriangleMesh *pMesh = new btTriangleMesh(true, false);
	pMesh->preallocateIndices(pSubset->iIndexCount);
	pMesh->preallocateVertices(pSubset->iVertexCount);
	for(UINT i = 0; i < pSubset->iVertexCount; ++i)
	{
		pMesh->findOrAddVertex(F3_BTVEC(pSubset->pVertices[i].vPos), false);
	}
	for(UINT i = 0; i < pSubset->iIndexCount; i += 3)
	{
		pMesh->addTriangleIndices(pSubset->pIndices[i], pSubset->pIndices[i + 1], pSubset->pIndices[i + 2]);
	}

	return(new btBvhTriangleMeshShape(pMesh, true));
}

SX_LIB_API void SPhysics_ReleaseTrimeshShape(btCollisionShape *pShape)
{
	assert(pShape->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE);

	mem_delete(pShape);
}

SX_LIB_API IXMutationObserver* SPhysics_NewMutationObserver()
{
	return(new CMutationObserver(Core_GetIXCore()));
}
