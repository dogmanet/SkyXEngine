
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

#ifndef SXGAME_EXPORT
#define SXGAME_EXPORT _declspec(dllimport)
#endif

#include <gdefines.h>

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <gcore/sxgcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

#include "SXbaseEntity.h"


/*! Инициализирует библиотеку
*/
SX_LIB_API void SXGame_0Create();

/*! Деинициализирует библиотеку
*/
SX_LIB_API void SXGame_AKill();

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

/*! Выгрузка игровых объеков уровня */
SX_LIB_API void SXGame_UnloadObjLevel();

/*! Сохраняет объекты в файл
*/
SX_LIB_API void SXGame_SaveEnts(const char * file);

/*! Помещает игрока в мир
*/
SX_LIB_API void SXGame_PlayerSpawn();

SX_LIB_API void SXGame_Render();
SX_LIB_API void SXGame_RenderHUD();

SX_LIB_API void SXGame_EditorRender(ID id, ID id_sel_tex);

SX_LIB_API ISXCamera * SXGame_GetActiveCamera();

/*! Получает количество зарегистрированных классов entity
*/
SX_LIB_API int SXGame_EntGetClassListCount();

/*! Получает список зарегистрированных классов entity.
	Память под массив const char * должна быть выделена заранее
*/
SX_LIB_API void SXGame_EntGetClassList(const char ** pszClassList, int count);

/*! Создает entity заданного класса
*/
SX_LIB_API SXbaseEntity * SXGame_CreateEntity(const char * classname);

/*! Уничтожает entity
*/
SX_LIB_API void SXGame_RemoveEntity(SXbaseEntity * pEnt);

/*! Получает таблицу свойств для заданного класса
*/
SX_LIB_API proptable_t * SXGame_EntGetProptable(const char * classname);

/*! Получает значение свойства по умолчанию для заданного класса
*/
SX_LIB_API const char * SXGame_EntGetDefault(const char * classname, const char * key);

/*! Количество всех entity
*/
SX_LIB_API int SXGame_EntGetCount();

/*! Возвращает entity по его id
*/
SX_LIB_API SXbaseEntity *SXGame_EntGet(ID id);

SX_LIB_API SXbaseEntity *SXGame_EntGetByName(const char *szName, ID idStart = 0);

#endif

/*! @} sxgame*/

