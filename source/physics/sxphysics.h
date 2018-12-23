
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxphysics - библиотека физики
*/

/*! \defgroup sxphysics sxphysics - библиотека физики
@{
*/


#ifndef __SXPHYSICS_H
#define __SXPHYSICS_H

#include <gdefines.h>

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

//#ifdef lerp
//#undef lerp
//#endif
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorldMt.h>

#define BIT(n) (1 << n)
enum COLLISION_GROUP
{
	CG_NONE = 0,
	// BEGIN --- Do not change ---
	CG_DEFAULT = BIT(0),
	CG_STATIC = BIT(1),
	CG_KINEMATIC = BIT(2),
	CG_DEBRIS = BIT(3),
	CG_TRIGGER = BIT(4),
	CG_CHARACTER = BIT(5),
	// END --- Do not change ---

	CG_WATER = BIT(6),
	CG_HITBOX = BIT(7),
	CG_BULLETFIRE = BIT(8),
	CG_NPCVIEW = BIT(9),
	CG_DOOR = BIT(10),

	CG_ALL = 0xFFFFFFFF
};

#define CG_STATIC_MASK (CG_ALL ^ (CG_DOOR | CG_HITBOX | CG_STATIC | CG_TRIGGER | CG_WATER))

//! Описатель физических свойств поверхности
struct SurfaceInfo
{
	ID idMtl; //!< ID материала
	float fHitChance; //!< Шанс столкновения пули
	float fStrength; //!< Прочность
	float fDensity; //!< Плотность
	int mat_type; //!< Тип материала
};

/*! Инициализирует библиотеку
*/
SX_LIB_API void SPhysics_0Create();

/*! Деинициализирует библиотеку
*/
SX_LIB_API void SPhysics_AKill();

/*! Устанавливает функцию сообщений
*/
SX_LIB_API void SPhysics_Dbg_Set(report_func rf);

/*! Устанавливает количество потоков обновления физики
*/
SX_LIB_API void SPhysics_UpdateSetThreadNum(int num);

/*! Выполняет обновление физики
	@param[in] thread Номер потока, выполняющего обновление
*/
SX_LIB_API void SPhysics_Update(int thread = 0);

/*! Выполняет синхронизацию. Для многопоточного обновления
*/
SX_LIB_API void SPhysics_Sync();

/*! Загружает информацию о геометрии уровня
*/
SX_LIB_API void SPhysics_LoadGeom(const char * file = NULL);

/*! Выгружает информацию о геометрии уровня
*/
SX_LIB_API void SPhysics_UnloadGeom();

/*! Загружает информацию о геометрии уровня
*/
SX_LIB_API bool SPhysics_ImportGeom(const char * file);

/*! Выгружает информацию о геометрии уровня
*/
SX_LIB_API bool SPhysics_ExportGeom(const char * file);

/*! Выполняет отрисовку физических объектов
*/
SX_LIB_API void SPhysics_DebugRender();

/*! Добавляет объект в симуляцию
*/
SX_LIB_API void SPhysics_AddShape(btRigidBody * pBody);
SX_LIB_API void SPhysics_AddShapeEx(btRigidBody * pBody, int group, int mask);

/*! Удаляет объект из симуляции
*/
SX_LIB_API void SPhysics_RemoveShape(btRigidBody * pBody);

SX_LIB_API int SPhysics_GetMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);

SX_LIB_API ID SPhysics_GetMtlID(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);

SX_LIB_API btDiscreteDynamicsWorldMt * SPhysics_GetDynWorld();

/*! Запускает симуляцию
*/
SX_LIB_API void SPhysics_EnableSimulation();

/*! Останавливает симуляцию
*/
SX_LIB_API void SPhysics_DisableSimulation();

SX_LIB_API void SPhysics_DumpStats();

SX_LIB_API void SPhysics_BuildHull(btConvexHullShape *pIn, btVector3 **ppOut, int *pNumVertices);

SX_LIB_API void SPhysics_ReleaseHull(btVector3 *pData, int iNumVertices);

#endif

/*! @} */

