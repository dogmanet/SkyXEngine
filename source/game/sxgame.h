
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

#include <gcore/sxgcore.h>

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

/*! Сохраняет объекты в файл
*/
SX_LIB_API void SXGame_SaveEnts(const char * file);

/*! Помещает игрока в мир
*/
SX_LIB_API void SXGame_PlayerSpawn();

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

/*! Количество всех entity
*/
SX_LIB_API SXbaseEntity * SXGame_EntGet(ID id);

//#############################################################################

/*! defgroup sxgame_ambient_snd Фоновые звуки окружения
 \ingroup sxgame
 \note Звуки проигрывают поочередно
 \note Путь до звука относительно директории со звуками
 @{
*/

//! добавить звук
SX_LIB_API void SGame_AmbientSndAdd(const char* path);

//! возвращает общее количество загруженных звуков
SX_LIB_API UINT SGame_AmbientSndGetCount();

//! в path записывает путь до звука по его id
SX_LIB_API void SGame_AmbientSndGet(ID id, char* path);

//! очистка списка звуков
SX_LIB_API void SGame_AmbientSndClear();

//! начать проигрывание звуков
SX_LIB_API void SGame_AmbientSndPlay();

//! обновление проигрывания
SX_LIB_API void SGame_AmbientSndUpdate();

//! пауза проигрывания
SX_LIB_API void SGame_AmbientSndPause();

//! возвращает "проигрываются ли звуки"
SX_LIB_API bool SGame_AmbientSndIsPlaying();

//!@} sxgame_ambient_snd

//#############################################################################

/*! defgroup sxgame_weather Фоновые звуки окружения
 \ingroup sxgame
 \note Смена погоды происходит на основании игрового времени
 \note При смене погоды для некоторых параметров возможна интерполяция данных от прошлой погоды к текущей на основании игрового времени, поэтому #SGame_WeatherUpdate необходимо вызывать в основном цикле (в рендере) для планой смены

Файл-конфиг состоит из:
 - основной секции (sections) с набором ключей содержащих имена секций по времени, к примеру имя ключа 01:11:12 это значит что ключ содержит имя секции с погодой для 1 часа ночи 11 минут и 12 секунд
 - секции с параметрами погоды (для каждого указанного времени)
 - секции с параметрами рандомных звуков, если таковые имеются \n

Секции погоды состоят из:
 - sky_texture - имя текстуры для скайбокса
 - sky_rotation - угол поворота скайбокса (в радианах)
 - sky_color - цвет неба float4 [0-1] где первые три компоненты это сам цвет, а четвертая компонента на сколько будет применен цвет
 - clouds_texture - текстура облаков
 - clouds_color - цвет облаков float4 [0-1] где первые три компоненты это сам цвет, а четвертая компонента на сколько будет применен цвет
 - clouds_rotate - поворот облаков (в радианах)
 - clouds_transparency - прозрачность облаков [0-1]
 - clouds_speed - скорость движения облаков
 - sun_texture - текстура солнца, если указать 0 то солнце будет выключено
 - sun_color - цвет солнца и освещения от солнца float3 [0-1]
 - sun_pos - позиция сонлца в градусах по оси X и Z
 - far - дальность видимости
 - rain_density плотность дождя [0-1]
 - rain_color - цвет дождя float4 [0-1] где первые три компоненты это сам цвет, а четвертая компонента на сколько будет применен цвет
 - thunderbolt - будет ли молния
 - thunder_period - перидичность звуков ударов молнии
 - fog_color - цвет тумана float3 [0-1]
 - fog_density - плотность тумана
 - env_ambient - имя секции с рандомными звуками для данной погоды \n

Все ключи в секции погоды обязательны, за исключением env_ambient \n
Все компоненты векторов обязательны \n

Секция с рандомными звуками состоит из:
 - period - промежуток (в млсек) между воспроизведением UINT2 (от и до), выбирается случайно
 - volume - громкость (в процентах) воспроизведения UINT2 (от и до), выбирается случайно
 - sounds - список звуков через запятую

@{
*/

#define WEATHER_RAIN_VOL_SND_UPDATE	1000	/*!< промежутки между обновлениями громоксти звука дождя (млсек) */
#define WEATHER_RAIN_VOL_SND_DIST	20.f	/*!< дистанция от наблюдателя, в течении которой будут слышны упавшие капли */
#define WEATHER_RAIN_MIN_Y_OBSERVER	5.f		/*!< на сколько ниже будет минимальная позиция бокса дождя относительно наблюдателя */
#define WEATHER_RAIN_RECREATE_COUNT	500		/*!< количество перерождаемых частиц при полной плотности дождя (должно быть оптимальнымтак как возможно при чрезмерно больших значениях соотношение дождя может быть неправильным) */
#define WEATHER_THUNDERBOLT_LIGHT_TIME	500	/*!< время свечения света при молнии (млсек)*/
#define WEATHER_THUNDERBOLT_WIDTH	100.f	/*!< ширина возможного пространства для молнии */
#define WEATHER_THUNDERBOLT_LENGTH	10.f	/*!< длина возможного пространства для молнии */
#define WEATHER_THUNDERBOLT_HEIGHT	100.f	/*!< высота возможного пространства для молнии */

//! загрузка конфига погоды
SX_LIB_API void SGame_WeatherLoad(const char* path);

//! обновление погоды
SX_LIB_API void SGame_WeatherUpdate();

//! возвращает текущую плотность дождя
SX_LIB_API float SGame_WeatherGetCurrRainDensity();

//! начать воспроизведение звуков погоды
SX_LIB_API void SGame_WeatherSndPlay();

//! приостановить воспроизведение звуков погоды
SX_LIB_API void SGame_WeatherSndPause();

//! возвращает "воспроизводятся ли звуки погоды"
SX_LIB_API bool SGame_WeatherSndIsPlaying();

//!@} sxgame_weather

#endif

/*! @} sxgame*/

