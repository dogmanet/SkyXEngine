
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
#include <xcommon/physics/IXPhysics.h>
#include <xcommon/resource/IXResourceModel.h>

/*! Инициализирует библиотеку
*/
SX_LIB_API void SPhysics_0Create();

/*! Деинициализирует библиотеку
*/
SX_LIB_API void SPhysics_AKill();

/*! Выполняет обновление физики
	@param[in] thread Номер потока, выполняющего обновление
*/
SX_LIB_API void SPhysics_Update();

SX_LIB_API btDiscreteDynamicsWorldMt* SPhysics_GetDynWorld();

SX_LIB_API void SPhysics_DumpStats();

#endif

/*! @} */

