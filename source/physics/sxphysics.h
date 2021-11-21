
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
#include "IXPhysics.h"
#include <xcommon/resource/IXResourceModel.h>
#include <xcommon/IXMutationObserver.h>

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

/*! Выполняет обновление физики
	@param[in] thread Номер потока, выполняющего обновление
*/
SX_LIB_API void SPhysics_Update(int thread = 0);

/*! Выполняет синхронизацию. Для многопоточного обновления
*/
SX_LIB_API void SPhysics_Sync();

SX_LIB_API int SPhysics_GetMtlType(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);

SX_LIB_API ID SPhysics_GetMtlID(const btCollisionObject *pBody, const btCollisionWorld::LocalShapeInfo *pShapeInfo);

SX_LIB_API btDiscreteDynamicsWorldMt* SPhysics_GetDynWorld();

/*! Запускает симуляцию
*/
SX_LIB_API void SPhysics_EnableSimulation();

/*! Останавливает симуляцию
*/
SX_LIB_API void SPhysics_DisableSimulation();

SX_LIB_API void SPhysics_DumpStats();

SX_LIB_API IXMutationObserver* SPhysics_NewMutationObserver();

#endif

/*! @} */

