
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxphysics - библиотека физики
*/

/*! \defgroup sxphysics sxphysics - библиотека физики
@{
*/


#ifndef _SXPHYSICS_H_
#define _SXPHYSICS_H_

#include <gdefines.h>


/*! Инициализирует библиотеку
*/
SX_LIB_API void SXPhysics_0Create();

/*! Деинициализирует библиотеку
*/
SX_LIB_API void SXPhysics_0Kill();

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


#endif

/*! @} */

