
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
//#include <mtllight\\sxmtllight.h>

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

/*! Инициализирует библиотеку
*/
SX_LIB_API void SXPhysics_0Create();

/*! Деинициализирует библиотеку
*/
SX_LIB_API void SXPhysics_AKill();

/*! Устанавливает функцию сообщений
*/
SX_LIB_API void SXPhysics_Dbg_Set(report_func rf);

/*! Устанавливает количество потоков обновления физики
*/
SX_LIB_API void SXPhysics_UpdateSetThreadNum(int num);

/*! Выполняет обновление физики
	@param[in] thread Номер потока, выполняющего обновление
*/
SX_LIB_API void SXPhysics_Update(int thread = 0);

/*! Выполняет синхронизацию. Для многопоточного обновления
*/
SX_LIB_API void SXPhysics_Sync();

/*! Загружает информацию о геометрии уровня
*/
SX_LIB_API void SXPhysics_LoadGeom(const char * file = NULL);

/*! Выгружает информацию о геометрии уровня
*/
SX_LIB_API void SXPhysics_UnloadGeom();

/*! Загружает информацию о геометрии уровня
*/
SX_LIB_API bool SXPhysics_ImportGeom(const char * file);

/*! Выгружает информацию о геометрии уровня
*/
SX_LIB_API bool SXPhysics_ExportGeom(const char * file);

/*! Выполняет отрисовку физических объектов
*/
SX_LIB_API void SXPhysics_DebugRender();

/*! Добавляет объект в симуляцию
*/
SX_LIB_API void SXPhysics_AddShape(btRigidBody * pBody);

/*! Удаляет объект из симуляции
*/
SX_LIB_API void SXPhysics_RemoveShape(btRigidBody * pBody);

SX_LIB_API int SXPhysics_GetMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);

SX_LIB_API btDiscreteDynamicsWorld * SXPhysics_GetDynWorld();

#endif

/*! @} */

