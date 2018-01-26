
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Заголовочный файл sxscore - звукового ядра
*/

/*! \defgroup sxscore sxscore - звуковое ядро

 \note Звуковое ядро работает на технологиях:
 - "DirectInput8"
 - "Ogg Vorbis" (загрузка ogg файлов).
 \note Загружаемые звуки wav и ogg формата (автоматическое определение и загрузка нужных данных).
 \note Поддерживаются 2d и 3d звуки, полная загрузка и потоковое воспроизвдение.
 \note Также для полностью загруженных (не потоковое воспроизведение) доступно воспроизвдение инстансов (копий), без выдеения дополнительной равноценной памяти, и без дополнительных возможностей управления инстансом.
Однако источник выдачи инстансов должен быть создан одной из функций #SSCore_SndInstancePlay2d #SSCore_SndInstancePlay3d
 \note Идентификация звука происходит на основании его числового идентификатора выдаваемого создаваемыми функциями (#SSCore_SndCreate2d/#SSCore_SndCreate2dInst #SSCore_SndCreate3d/#SSCore_SndCreate3dInst) и является константной, идентификатор является порядковым номером
@{*/

#ifndef __SXSCORE_H
#define __SXSCORE_H

#include <gdefines.h>
#include <GRegisterIndex.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif

#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllimport)
#include <core/sxcore.h>

#ifdef SX_DLL
#undef SX_LIB_API
#define SX_LIB_API extern "C" __declspec (dllexport)
#endif

//##########################################################################

/*! \name Стандартные функции библиотеки
@{*/

//! возвращает версию подсистемы геометрии
SX_LIB_API long SSCore_0GetVersion();			

//! установить новую функцию вывода сообщений
SX_LIB_API void SSCore_Dbg_Set(report_func fnFunc);	

//! инициализация подсистемы
SX_LIB_API void SSCore_0Create(	
	const char *szName,	//!< имя
	HWND hWnd,			//!< хэндл окна к которому привязывается звук
	bool isUnic = true	//!< должна ли подсистема быть уникальной по имени
	);

//! уничтожение подсистемы
SX_LIB_API void SSCore_AKill();	

//!@}

//#############################################################################

/*! абсолюный путь до директории с файлами */
#define SOUND_MAX_SIZE_STDPATH	256	

/*! относительный путь до файла со звуком */
#define SOUND_MAX_SIZE_PATH		128	

/*! минимально возможный размер потока для воспроизведения */
#define SOUND_MIN_SIZE_STREAM	1024*64 

//**************************************************************************

/*! \name Данные для первичного буфера
@{*/

/*! количество каналов */
#define SOUND_G_CHANELS_COUNT	2		

/*! количество бит в секунду */
#define SOUND_G_BITS_PER_SEC	48000	

/*! количество бит в семпле */
#define SOUND_G_BITS_PER_SAMPLE	32		

//!@}

//**************************************************************************

/*! количество бит на сэмпл для ogg, возможно 8 или 16 */
#define SOUND_OGG_BITS_PER_SAMPLE	16	

/*! тип количества бит на сэмпл для ogg, 1 - для 8 бит, 2 - для 16 */
#define SOUND_OGG_BITS_PER_SAMPLE_WORD SOUND_OGG_BITS_PER_SAMPLE/8

#define SOUND_POS_BYTES	0	/*!< байты */
#define SOUND_POS_SEC	1	/*!< секунды */
#define SOUND_POS_MLS	2	/*!< млсек */

#define SOUND_VOL_DB	0	/*!< децибелы */
#define SOUND_VOL_PCT	1	/*!< проценты */

#define SOUND_EFF_ON	1	/*!< включение эффекта */
#define SOUND_EFF_OFF	0	/*!< выключение эффекта */
#define SOUND_EFF_INVALID_KEY	-1	/*!< неверно указан ключ при обращении к функциям эффектов */

//#############################################################################

//! форматы звукрвых файлов
enum SOUND_FILEFORMAT
{
	SOUND_FILEFORMAT_UNKNOWN,	//!< не известный формат
	SOUND_FILEFORMAT_WAV,		//!< wave формат
	SOUND_FILEFORMAT_OGG,		//!< ogg формат
};

//! состояния воспроизведения звукового объекта
enum SOUND_OBJSTATE
{
	SOUND_OBJSTATE_STOP,	//!< остановлен
	SOUND_OBJSTATE_PAUSE,	//!< приостановлен
	SOUND_OBJSTATE_PLAY,	//!< проигрывается
};

//#############################################################################

//! очистка всего списка звуков (полное их удаление)
SX_LIB_API void SSCore_Clear();	

//! обновление состояний всех звуков
SX_LIB_API void SSCore_Update(
	const float3 *pViewPpos,//!< текущая позиция наблюдателя
	const float3 *pViewDir	//!< текущее направление взгляда
	);	

//! количество проигрываемых на данный момент звуков
SX_LIB_API int SSCore_SndsPlayCountGet();	

//! количество загруженных на данный момент звуков
SX_LIB_API int SSCore_SndsLoadCountGet();	

//! загрузка 2d (фонового) звука
SX_LIB_API ID SSCore_SndCreate2d(
	const char *szFile,		//!< путь до файла, относительно стандартного
	bool isLooping = false,	//!< зацикливать ли воспроизведение
	UINT uiSizeStream = 0	//!< размер потока в байтах для потокового воспроизведения, если 0 тогда полностью грузить в память
	);

//! загрузка 3d звука
SX_LIB_API ID SSCore_SndCreate3d(
	const char *szFile,		//!< путь до файла, относительно стандартного
	bool isLooping,			//!< зацикливать ли воспроизведение
	UINT uiSizeStream,		//!< размер потока в байтах для потокового воспроизведения, если 0 тогда полностью грузить в память
	float fDist,			//!< радиус слышимости в метрах
	float fShiftPan = 0.1f	//!< изменение позиционирования звука [0-1], на сколько будет смещен звук (между ушами слушателя) при поворотах камеры к источнику звука, чем ближе к объекту тем меньше разница в позиционировании при поворотах
	);

//! воспроизведение инстанса 2d звука
SX_LIB_API void SSCore_SndInstancePlay2d(
	ID idSound,			//!< идентификатор звука
	int iVolume = 100,	//!< громкость [0-100]
	int iPan = 0		//!< смещение между ушами [-100 - 100]
	);

//! воспроизведение инстанса 3d звука
SX_LIB_API void SSCore_SndInstancePlay3d(
	ID idSound,			//!< идентификатор звука
	const float3 *pPos	//!< позиция воспроизведения инстанса
	);

//! загрузка 2d звукового файла и пометка его как объект выдающий инстансы, аргументы аналогичны #SSCore_SndCreate2d
SX_LIB_API ID SSCore_SndCreate2dInst(const char *szFile, bool isLooping = false, UINT uiSizeStream = 0);

//! загрузка 3d звукового файла и пометка его как объект выдающий инстансы, аргументы аналогичны #SSCore_SndCreate3d
SX_LIB_API ID SSCore_SndCreate3dInst(const char *szFile, bool isLooping, UINT uiSizeStream, float fDist, float fShiftPan = 0.1f);

//! поиск 2d звука (выдающего инстансы) по относительному пути загрузки,возвращает его ID (в случае успеха) иначе <0
SX_LIB_API ID SSCore_SndFind2dInst(const char *szFile);

//! поиск 3d звука (выдающего инстансы) по относительному пути загрузки,возвращает его ID (в случае успеха) иначе <0
SX_LIB_API ID SSCore_SndFind3dInst(const char *szFile);


//! инициализирован ли звук с идентификатором id
SX_LIB_API bool SSCore_SndIsInit(ID idSound);

//! удалить звук по его id
SX_LIB_API void SSCore_SndDelete(ID idSound);


//! воспроизвести звук, looping зацикливать ли воспроизведение, 0 - нет, >0 да, <0 не учитывать данное значение
SX_LIB_API void	SSCore_SndPlay(ID idSound, int looping = -1);

//! приостановить
SX_LIB_API void	SSCore_SndPause(ID idSound);

//! остановить
SX_LIB_API void	SSCore_SndStop(ID idSound);


//! устанавливает состояние проигрывания звука
SX_LIB_API void SSCore_SndStateSet(ID idSound, SOUND_OBJSTATE state);

//! возвращает состояние проигрывания звука на данный момент
SX_LIB_API SOUND_OBJSTATE SSCore_SndStateGet(ID idSound);

//! устанавить позицию проигрывания
SX_LIB_API void SSCore_SndPosCurrSet(
	ID idSound,					//!< идентификатор звука
	UINT uiPos,					//!< значение позиции исходя из параметра type
	int iType = SOUND_POS_BYTES	//!< тип значения pos, SOUND_POS_
	);	

//! возвращает позицию проигрывания
SX_LIB_API UINT SSCore_SndPosCurrGet(
	ID idSound,					//!< идентификатор звука
	int iType = SOUND_POS_BYTES	//!< тип возвращаемого значения, SOUND_POS_
	);

//! устанавить громкость
SX_LIB_API void SSCore_SndVolumeSet(
	ID idSound,					//!< идентификатор звука
	int iVolume,				//!< значение громкости исходя из параметра type
	int iType = SOUND_VOL_PCT	//!< тип значения volume, SOUND_VOL_
	);

//! возвращает громкость
SX_LIB_API int SSCore_SndVolumeGet(
	ID idSound,					//!< идентификатор звука
	int iType = SOUND_VOL_PCT	//!< тип возвращаемого значения, SOUND_VOL_
	);

//! установка позиционирования между ушами
SX_LIB_API void SSCore_SndPanSet(
	ID idSound,					//!< идентификатор звука
	int iValue,					//!< значение позиционирования исходя из параметра type
	int iType = SOUND_VOL_PCT	//!< тип значения value, SOUND_VOL_
	);

//! возвращает позиционирование между ушами
SX_LIB_API int SSCore_SndPanGet(
	ID idSound,						//!< идентификатор звука
	int type = SOUND_VOL_PCT	//!< тип возвращаемого значения, SOUND_VOL_
	);


//! установка частоты воспроизведения
SX_LIB_API void SSCore_SndFreqCurrSet(ID idSound, UINT uiValue);

//! возвращает текущую частоту воспроизведения
SX_LIB_API UINT SSCore_SndFreqCurrGet(ID idSound);

//! возвращает оригинальную частоту воспроизведения
SX_LIB_API UINT SSCore_SndFreqOriginGet(ID idSound);


//! установка мировой позиции звука (только для 3d звуков)
SX_LIB_API void SSCore_SndPosWSet(ID idSound, const float3 *pPos);

//! возвращает мировую позицию звука (только для 3d звуков)
SX_LIB_API void SSCore_SndPosWGet(ID idSound, float3 *pPos);


//! длина в секундах
SX_LIB_API int SSCore_SndLengthSecGet(ID idSound);

//! количество байт в секунде
SX_LIB_API UINT SSCore_SndBytesPerSecGet(ID idSound);

//! размер в байтах PCM данных
SX_LIB_API UINT SSCore_SndSizeGet(ID idSound);

//! относительный путь до звукового файла
SX_LIB_API void SSCore_SndFileGet(ID idSound, char *szPath);		


//! возвращает дистанцию слышимости
SX_LIB_API float SSCore_SndDistAudibleGet(ID idSound);

//! установка дистанции слышимости в метрах
SX_LIB_API void SSCore_SndDistAudibleSet(ID idSound, float fVfalue);

//!@}

#endif

//!@} sxscore
