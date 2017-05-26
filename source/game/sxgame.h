
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxgame - библиотека игровых объектов
*/

/*! \defgroup sxgame sxgame - библиотека игровых объектов
@{
*/


#ifndef _SXGAME_H_
#define _SXGAME_H_

#include <gdefines.h>

#include <gcore/sxgcore.h>


/*! Инициализирует библиотеку
*/
SX_LIB_API void SXGame_0Create();

/*! Деинициализирует библиотеку
*/
SX_LIB_API void SXGame_0Kill();

/*! Устанавливает функцию сообщений
*/
SX_LIB_API void SXGame_Dbg_Set(report_func rf);

/*! Устанавливает количество потоков обновления
*/
SX_LIB_API void SXGame_UpdateSetThreadNum(int num);

/*! Выполняет обновление
@param[in] thread Номер потока, выполняющего обновление
*/
SX_LIB_API void SXGame_Update(int thread = 0);

/*! Выполняет синхронизацию. Для многопоточного обновления
*/
SX_LIB_API void SXGame_Sync();

/*! Загружает список entity из файла
*/
SX_LIB_API void SXGame_LoadEnts(const char * file);

/*! Сохраняет объекты в файл
*/
SX_LIB_API void SXGame_SaveEnts(const char * file);

/*! Помещает игрока в мир
*/
SX_LIB_API void SXGame_PlayerSpawn();

SX_LIB_API void SXGame_EditorRender();

SX_LIB_API ISXCamera * SXGame_GetActiveCamera();

#endif

/*! @} */

