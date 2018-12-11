
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "sxphysics.h"
#include "PhyWorld.h"

#include <core/sxcore.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

#if defined(_DEBUG)
#	pragma comment(lib, "sxcore_d.lib")
#	pragma comment(lib, "sxgeom_d.lib")
#	pragma comment(lib, "sxgreen_d.lib")
#	pragma comment(lib, "sxlight_d.lib")
#	pragma comment(lib, "sxmtrl_d.lib")
#	pragma comment(lib, "BulletDynamics_vs2010_debug.lib")
#	pragma comment(lib, "BulletCollision_vs2010_debug.lib")
#	pragma comment(lib, "LinearMath_vs2010_debug.lib")
#	pragma comment(lib, "BulletWorldImporter_vs2010_debug.lib")
#else
#	pragma comment(lib, "sxcore.lib")
#	pragma comment(lib, "sxgeom.lib")
#	pragma comment(lib, "sxgreen.lib")
#	pragma comment(lib, "sxlight.lib")
#	pragma comment(lib, "sxmtrl.lib")
#	pragma comment(lib, "BulletDynamics_vs2010.lib")
#	pragma comment(lib, "BulletCollision_vs2010.lib")
#	pragma comment(lib, "LinearMath_vs2010.lib")
#	pragma comment(lib, "BulletWorldImporter_vs2010.lib")
#endif

#if !defined(DEF_STD_REPORT)
#define DEF_STD_REPORT
report_func g_fnReportf = DefReport;
#endif

CPhyWorld * g_pWorld = NULL;

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

SX_LIB_API void SXPhysics_DumpStats()
{
	CProfileManager::dumpAll();
}

SX_LIB_API void SXPhysics_0Create()
{
	if(g_pWorld)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - sxphysics double init", GEN_MSG_LOCATION);
		return;
	}
	Core_SetOutPtr();

	g_pWorld = new CPhyWorld();

	Core_0RegisterConcmd("perf_physics", SXPhysics_DumpStats);
}
SX_LIB_API void SXPhysics_AKill()
{
	SP_PRECOND(_VOID);
	mem_delete(g_pWorld);
}

SX_LIB_API void SXPhysics_Update(int thread)
{
	SP_PRECOND(_VOID);
	g_pWorld->update(thread);
}
SX_LIB_API void SXPhysics_UpdateSetThreadNum(int num)
{
	SP_PRECOND(_VOID);
	g_pWorld->setThreadNum(num);
}
SX_LIB_API void SXPhysics_Sync()
{
	SP_PRECOND(_VOID);
	g_pWorld->sync();
}

SX_LIB_API void SXPhysics_Dbg_Set(report_func rf)
{
	g_fnReportf = rf;
}

SX_LIB_API void SXPhysics_LoadGeom(const char * file)
{
	SP_PRECOND(_VOID);
	g_pWorld->loadGeom(file);
}

SX_LIB_API void SXPhysics_UnloadGeom()
{
	SP_PRECOND(_VOID);
	g_pWorld->unloadGeom();
}

SX_LIB_API void SXPhysics_DebugRender()
{
	SP_PRECOND(_VOID);
	g_pWorld->render();
}

SX_LIB_API void SXPhysics_AddShape(btRigidBody * pBody)
{
	SP_PRECOND(_VOID);
	g_pWorld->addShape(pBody);
}

SX_LIB_API void SXPhysics_AddShapeEx(btRigidBody * pBody, int group, int mask)
{
	SP_PRECOND(_VOID);
	g_pWorld->addShape(pBody, group, mask);
}

SX_LIB_API void SXPhysics_RemoveShape(btRigidBody * pBody)
{
	SP_PRECOND(_VOID);
	g_pWorld->removeShape(pBody);
}

SX_LIB_API btDiscreteDynamicsWorldMt * SXPhysics_GetDynWorld()
{
	SP_PRECOND(NULL);
	return(g_pWorld->getBtWorld());
}

SX_LIB_API bool SXPhysics_ImportGeom(const char * file)
{
	SP_PRECOND(false);
	return(g_pWorld->importGeom(file));
}

SX_LIB_API bool SXPhysics_ExportGeom(const char * file)
{
	SP_PRECOND(false);
	return(g_pWorld->exportGeom(file));
}

SX_LIB_API int SXPhysics_GetMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo)
{
	SP_PRECOND(MTLTYPE_PHYSIC_DEFAULT);

	return(g_pWorld->getMtlType(pBody, pShapeInfo));
}

SX_LIB_API ID SXPhysics_GetMtlID(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo)
{
	SP_PRECOND(-1);

	return(g_pWorld->getMtlID(pBody, pShapeInfo));
}

SX_LIB_API void SXPhysics_EnableSimulation()
{
	SP_PRECOND(_VOID);

	g_pWorld->enableSimulation();
}

SX_LIB_API void SXPhysics_DisableSimulation()
{
	SP_PRECOND(_VOID);

	g_pWorld->disableSimulation();
}

SX_LIB_API void SXPhysics_BuildHull(btConvexHullShape *pIn, btVector3 **ppOut, int *pNumVertices)
{
	btShapeHull tmpHull(pIn);
	tmpHull.buildHull(0);
	*pNumVertices = tmpHull.numVertices();
	*ppOut = new btVector3[*pNumVertices];
	memcpy(*ppOut, tmpHull.getVertexPointer(), sizeof(btVector3)* *pNumVertices);
	//return(new btConvexHullShape((const btScalar*)tmpHull.getVertexPointer(), tmpHull.numVertices(), sizeof(btVector3)));
}

SX_LIB_API void SXPhysics_ReleaseHull(btVector3 *pData, int iNumVertices)
{
	mem_delete_a(pData);
}
