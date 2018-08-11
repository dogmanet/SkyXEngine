
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
 \note Также для полностью загруженных (не потоковое воспроизведение) доступно воспроизвдение инстансов (копий), без выделения дополнительной равноценной памяти, и без дополнительных возможностей управления инстансом.
Однако источник выдачи инстансов должен быть создан одной из функций #SSCore_SndInstancePlay2d #SSCore_SndInstancePlay3d
 \note Идентификация звука происходит на основании его числового идентификатора выдаваемого создаваемыми функциями (#SSCore_SndCreate2d/#SSCore_SndCreate2dInst #SSCore_SndCreate3d/#SSCore_SndCreate3dInst) и является константной, идентификатор является порядковым номером
 \note каждый звук, который проигрывается не потоково, может проигрываться с задержками
 \note каждый звук может загружаться с настройками прямо в строке, при этом настройки в строке будут приоритетными, формат строки следующий:
путь_до_файла_без_пробелов  dдистанция_слышимости_в_метрах vгромкость_от_0_до_1 задержка_перед_проигрыванием_в_млсек задержка2_в_млсек ... \n
данные громкости и дальности должны быть первыми (но между собой у них нет правила), и только после них идут задержки \n
на пример: messages/artefact_lead.ogg d100 v1.0 300 300 300 \n
парсинг строки осуществляется на основании пробелов, то есть в имени файла недопустимы пробелы
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

/*! размер абсолюного пути до директории с файлами */
#define SOUND_MAX_SIZE_STDPATH	256	

/*! размер относительного пути до файла со звуком */
#define SOUND_MAX_SIZE_PATH		128	

/*! размер полного пути до файла со звуком */
#define SOUND_MAX_SIZE_FULLPATH	(SOUND_MAX_SIZE_STDPATH + SOUND_MAX_SIZE_PATH)

/*! минимально возможный размер потока для воспроизведения */
#define SOUND_MIN_SIZE_STREAM	1024*64 

/*! ошибочный идентификатор */
#define SOUND_FAIL_ID	-1


/*! коэффициент смещения между ушами слушателя для 3d звуков */
#define SOUND_SHIFTPAN_3D	0.1f

/*! дистанция слышимости по умолчанию, в метрах */
#define SOUND_DIST_AUDIBLE_DEFAULT	100


/*! максимальное количество каналов */
#define SOUND_CHANNELS_COUNT	100


/*! упаковка двух int32_t в uint64_t */
#define SOUND_ENCODE2ID(id1, id2)( (((uint64_t)((uint32_t)id1))<<32) | (uint32_t)id2 )

/*! извлечение первого int32_t из uint64_t из старших битов */
#define SOUND_DECODE2ID_HI(id)( (int32_t)(uint32_t)(((uint64_t)id) >> 32) )

/*! извлечение первого int32_t из uint64_t из младших битов */
#define SOUND_DECODE2ID_LO(id)( (int32_t)(uint32_t)(((uint64_t)id) & 0xFFFFFFFF) )


/*! масимальная длина имени набора звуков */
#define SOUND_MAX_SIZE_SNDKIT_NAME	128	

/*! блокировать инстанс набора */
#define SOUND_SNDKIT_INSTANCE_BLOCK		-1

/*! не блокировать инстанс набора */
#define SOUND_SNDKIT_INSTANCE_NOTBLOCK	-2

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

/*! количество бит на сэмпл для ogg, возможно 8 или 16 */
#define SOUND_OGG_BITS_PER_SAMPLE	16	

/*! тип количества бит на сэмпл для ogg, 1 - для 8 бит, 2 - для 16 */
#define SOUND_OGG_BITS_PER_SAMPLE_WORD SOUND_OGG_BITS_PER_SAMPLE/8

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

//! остояние занятости инстансов
enum SOUND_SNDINSTANCE_BUSY
{
	SOUND_SNDINSTANCE_BUSY_FREE,	/*!< свободный */
	SOUND_SNDINSTANCE_BUSY_TEMP,	/*!< времено занятый */
	SOUND_SNDINSTANCE_BUSY_LOCKED,	/*!< заблокированный владельцем */
};

//! значения каналов
enum SOUND_CHANNEL
{
	SOUND_CHANNEL_NONE = -1,	/*!< значение не инициализованного канала */
	SOUND_CHANNEL_STOP = 0,		/*!< значение не проигрывающегося в данный момент канала */
	SOUND_CHANNEL_PLAY = 1,		/*!< значение проигрывающегося в данный момент канала */
};

//#############################################################################

//! очистка всего списка звуков (полное их удаление)
SX_LIB_API void SSCore_Clear();	

//! обновление состояний всех звуков
SX_LIB_API void SSCore_Update(
	const float3 *pViewPpos,//!< текущая позиция наблюдателя
	const float3 *pViewDir	//!< текущее направление взгляда
	);

//! установка общей громкости
SX_LIB_API void SSCore_SetMainVolume(float fVolume);

//! количество проигрываемых на данный момент звуков
SX_LIB_API int SSCore_SndsGetCountPlay();

//! количество загруженных на данный момент звуков
SX_LIB_API int SSCore_SndsGetCountLoad();

//##########################################################################

//! добавить канал
SX_LIB_API void SSCore_ChannelAdd(ID idChannel, bool isPlaying = false);

//! существует ли канал
SX_LIB_API bool SSCore_ChannelExists(ID idChannel);

//! количество звуков в канале
SX_LIB_API int SSCore_ChannelGetSndCount(ID idChannel);

//! проигрывать звуки канала
SX_LIB_API void SSCore_ChannelPlay(ID idChannel);

//! проигрывать звуки только этого канала
SX_LIB_API void SSCore_ChannelPlayOnly(ID idChannel);

//! проигрывается ли канал?
SX_LIB_API bool SSCore_ChannelPlaying(ID idChannel);

//! остановить проигрывание звуков канала
SX_LIB_API void SSCore_ChannelStop(ID idChannel);

//###############################################################################

//! загрузка 2d (фонового) звука
SX_LIB_API ID SSCore_SndCreate2d(
	const char *szFile,		//!< путь до файла, относительно стандартного
	ID idChannel,			//!< идентификатор канала
	UINT uiSizeStream = 0	//!< размер потока в байтах для потокового воспроизведения, если 0 тогда полностью грузить в память
	);

//! загрузка 3d звука
SX_LIB_API ID SSCore_SndCreate3d(
	const char *szFile,		//!< путь до файла, относительно стандартного
	ID idChannel,			//!< идентификатор канала
	UINT uiSizeStream,		//!< размер потока в байтах для потокового воспроизведения, если 0 тогда полностью грузить в память
	float fDist				//!< радиус слышимости в метрах
	);

//###############################################################################

//! загрузка 2d звукового файла и пометка его как объект выдающий инстансы, аргументы аналогичны #SSCore_SndCreate2d
SX_LIB_API ID SSCore_SndCreate2dInst(const char *szFile, ID idChannel);

//! загрузка 3d звукового файла и пометка его как объект выдающий инстансы, аргументы аналогичны #SSCore_SndCreate3d
SX_LIB_API ID SSCore_SndCreate3dInst(const char *szFile, ID idChannel, float fDist);

//**************************************************************************

//! поиск 2d звука (выдающего инстансы) по относительному пути загрузки,возвращает его ID (в случае успеха) иначе <0
SX_LIB_API ID SSCore_SndFind2dInst(const char *szFile, ID idChannel);

//! поиск 3d звука (выдающего инстансы) по относительному пути загрузки,возвращает его ID (в случае успеха) иначе <0
SX_LIB_API ID SSCore_SndFind3dInst(const char *szFile, ID idChannel);

//**************************************************************************

/*! воспроизведение инстанса звука, если нужны задержки то надо их указывать в pArrDelay и iSizeArrDelay, иначе в обоих 0, если указать iSizeArrDelay = -1 тогда будут задействованы предыдущие настройки
если нужен 3д звук (если это на самом деле 3д звук) то надо указать pPos, иначе 0 и будет 2д, для 2д по желанию можно указать громкость iVolume и смещение между ушами iPan
*/
SX_LIB_API ID SSCore_SndInstancePlay(
	ID idSound,				//!< идентификатор звука
	bool isBlocked,			//!< блокировать ли файл? если true то вернет id который будет занят до тех пор пока его не освободит владалец через #SSCore_SndInstanceFree
	bool isLooping,			//!< зацикливать ли воспроизведение звука? если да то звук будет заблокирован, а возвращенный id будет приндалежать вызывавшему и именно он несет ответсвенность за остановку и освобождение инстанса
	UINT *pArrDelay,		//!< массив задержек в млсек, первое значение задержка перед воспроизведением
	int iSizeArrDelay,		//!< размер #pArrDelay
	const float3 *pPos,		//!< если это 3д звук то позиция обязательна, если 2д то нужно указать NULL
	float fVolume = 1.f,	//!< громкость [0, 1]
	float fPan = 0.f		//!< смещение между ушами [-1, 1]
	);

//! воспроизведение инстанса 2d звука, аргументы аналогичны #SSCore_SndInstancePlay
#define SSCore_SndInstancePlay2d(idSound, isBlocked, isLooping, iVolume, iPan) (SSCore_SndInstancePlay(idSound, isBlocked, isLooping, 0, 0, 0, iVolume, iPan))

//! воспроизведение инстанса 2d звука с задержками, аргументы аналогичны #SSCore_SndInstancePlay
#define SSCore_SndInstancePlayDelay2d(idSound, isBlocked, isLooping, pArrDelay, iSizeArrDelay, iVolume, iPan) (SSCore_SndInstancePlay(idSound, isBlocked, isLooping, pArrDelay, iSizeArrDelay, 0, iVolume, iPan))

//! воспроизведение инстанса 3d звука, аргументы аналогичны #SSCore_SndInstancePlay
#define SSCore_SndInstancePlay3d(idSound, isBlocked, isLooping, pPos) (SSCore_SndInstancePlay(idSound, isBlocked, isLooping, 0, 0, pPos))

//! воспроизведение инстанса 3d звука с задержками, аргументы аналогичны #SSCore_SndInstancePlay
#define SSCore_SndInstancePlayDelay3d(idSound, isBlocked, isLooping, pArrDelay, iSizeArrDelay, pPos) (SSCore_SndInstancePlay(idSound, isBlocked, isLooping, pArrDelay, iSizeArrDelay, pPos))

//**************************************************************************

//! проигрывается ли инстанс звука с id idInstance
SX_LIB_API bool SSCore_SndInstancePlaying(
	ID idSound,		//!< идентификатор звука
	ID idInstance	//!< идентификатор инстанса звука который был возвращен функцией #SSCore_SndInstancePlay2d или #SSCore_SndInstancePlay3d
	);

//! остановка инстанса звука с id idInstance
SX_LIB_API void SSCore_SndInstanceStop(ID idSound, ID idInstance);

//! освобождает инстанс звука с id idInstance, будет иметь статус занятости #SOUND_SNDINSTANCE_BUSY_TEMP если был #SOUND_SNDINSTANCE_BUSY_LOCKED, а при следующем обновлении если инстанс не проигрывается статус поменяется на #SOUND_SNDINSTANCE_BUSY_FREE
SX_LIB_API void SSCore_SndInstanceFree(ID idSound, ID idInstance);

//###############################################################################

//! инициализирован ли звук с идентификатором id
SX_LIB_API bool SSCore_SndIsInit(ID idSound);

//! удалить звук по его id
SX_LIB_API void SSCore_SndDelete(ID idSound);

//###############################################################################

//! воспроизвести звук, isLooping зацикливать ли воспроизведение
SX_LIB_API void	SSCore_SndPlay(
	ID idSound,				//!< идентификатор звука
	bool isLooping = false,	//!< зацикливать ли воспроизведение?
	UINT *pArrDelay = 0,	//!< массив задержек в млсек, первое значение задержка перед воспроизведением
	int iSizeArrDelay = 0	//!< размер массива pArrDelay, если указать iSizeArrDelay = -1 тогда будут задействованы предыдущие настройки
	);

//! приостановить
SX_LIB_API void	SSCore_SndPause(ID idSound);

//! остановить
SX_LIB_API void	SSCore_SndStop(ID idSound);


//! устанавливает состояние проигрывания звука
SX_LIB_API void SSCore_SndSetState(ID idSound, SOUND_OBJSTATE state);

//! возвращает состояние проигрывания звука на данный момент
SX_LIB_API SOUND_OBJSTATE SSCore_SndGetState(ID idSound);

//**************************************************************************

//! устанавить позицию проигрывания
SX_LIB_API void SSCore_SndSetPosPlay(
	ID idSound,		//!< идентификатор звука
	UINT uiPosMls	//!< значение позиции в млсек
	);	

//! возвращает позицию проигрывания
SX_LIB_API UINT SSCore_SndGetPosPlay(ID idSound);

//! устанавить громкость
SX_LIB_API void SSCore_SndSetVolume(
	ID idSound,		//!< идентификатор звука
	float fVolume	//!< значение громкости [0, 1]
	);

//! возвращает громкость [0, 1]
SX_LIB_API float SSCore_SndGetVolume(ID idSound);

//! установка позиционирования между ушами
SX_LIB_API void SSCore_SndSetPan(
	ID idSound,		//!< идентификатор звука
	float fValue	//!< значение позиционирования [-1, 1]
	);

//! возвращает позиционирование между ушами [-1, 1]
SX_LIB_API float SSCore_SndGetPan(ID idSound);


//! установка частоты воспроизведения
SX_LIB_API void SSCore_SndSetCurrFreq(ID idSound, UINT uiValue);

//! возвращает текущую частоту воспроизведения
SX_LIB_API UINT SSCore_SndGetCurrFreq(ID idSound);

//! возвращает оригинальную частоту воспроизведения
SX_LIB_API UINT SSCore_SndGetOriginFreq(ID idSound);


//! установка мировой позиции звука (только для 3d звуков)
SX_LIB_API void SSCore_SndSetPosWorld(ID idSound, const float3 *pPos);

//! возвращает мировую позицию звука (только для 3d звуков)
SX_LIB_API void SSCore_SndGetPosWorld(ID idSound, float3 *pPos);


//! длина в секундах
SX_LIB_API int SSCore_SndGetLengthSec(ID idSound);

//! количество байт в секунде
SX_LIB_API UINT SSCore_SndGetBytesPerSec(ID idSound);

//! размер в байтах PCM данных
SX_LIB_API UINT SSCore_SndGetSize(ID idSound);

//! относительный путь до звукового файла
SX_LIB_API void SSCore_SndGetFile(ID idSound, char *szPath);


//! возвращает дистанцию слышимости
SX_LIB_API float SSCore_SndGetDistAudible(ID idSound);

//! установка дистанции слышимости в метрах
SX_LIB_API void SSCore_SndSetDistAudible(ID idSound, float fVfalue);

//!@}

//###############################################################################

/*! \name Наборы звуков для воспроизведения случайного звука из набора
 \note Все звуки создаются инстансовыми
@{*/

//! создать набор звуков
SX_LIB_API ID SSCore_SndkitCreate(
	const char *szName,			//!< имя набора
	ID idChannel,				//!< id канала
	bool is3D,					//!< набор 3д звуков?
	float fDistAudible = SOUND_DIST_AUDIBLE_DEFAULT	//!< дистанция слышимости
	);

//! загрузить список звуков (с настройками) из строки либо из строк
SX_LIB_API ID SSCore_SndkitCreateFromList(
	const char *szName,			//!< имя набора
	ID idChannel,				//!< id канала
	bool is3D,					//!< набор 3д звуков?
	float fDistAudible,			//!< дистанция слышимости
	float fVolume,				//!< громкость [0, 1]
	const char *szFileOrList,	//!< строка с первым файлом и настройками (последующие аргументы учитываются), либо строка со всеми файлами (через запятую) и настройками (последующие аргументы не учитываются)
	...							//!< если #szFileOrList один файл тогда здесь должны быть строки с файлами до звуков (с настройками)
	);

//**************************************************************************

//! добавить звук в набор
SX_LIB_API void SSCore_SndkitAddSound(
	ID idSndKit,			//!< id набора
	const char *szFile,		//!< путь к файлу звука
	float fDistAudible,		//!< дистанция слышимости для 3d звуков, для 2d можно просто 0, если < 0 тогда установит то значение которое было передано в #SSCore_SndkitCreate
	float fVolume = 1.f,	//!< громскость [0, 1]
	UINT *pArrDelay = 0,	//!< массив задержек
	int iSizeArrDelay = 0	//!< количество элементов в #pArrDelay
	);

//! добавить в набор 2d звук
#define SSCore_SndkitAddSound2d(idSndKit, szFile, fVolume) (SSCore_SndkitAddSound(idSndKit, szFile, 0, fVolume, 0, 0))

//! добавить в набор 2d звук с задержками при воспроизведении
#define SSCore_SndkitAddSoundDelay2d(idSndKit, szFile, fVolume, pArrDelay, iSizeArrDelay) (SSCore_SndkitAddSound(idSndKit, szFile, 0, fVolume, pArrDelay, iSizeArrDelay))

//! добавить в набор 3d звук
#define SSCore_SndkitAddSound3d(idSndKit, szFile, fDistAudible, fVolume) (SSCore_SndkitAddSound(idSndKit, szFile, fDistAudible, fVolume, 0, 0))

//! добавить в набор 3d звук с задержками при воспроизведении
#define SSCore_SndkitAddSoundDelay3d(idSndKit, szFile, fDistAudible, fVolume, pArrDelay, iSizeArrDelay) (SSCore_SndkitAddSound(idSndKit, szFile, fDistAudible, fVolume, pArrDelay, iSizeArrDelay))

//**************************************************************************

/*! проиграть случайны звук
 \note если набор 3д звуков, то позиция обязательна
 \note id2 должен принимать либо #SOUND_SNDKIT_INSTANCE_BLOCK либо #SOUND_SNDKIT_INSTANCE_NOTBLOCK либо должен быть упакованным значением идентификаторов заблокированного инстанса звука
если инстанс воспроизводимого звука заблокирован, то он не будет использоваться другими пока владелец его не разблокирует
если было передано упакованное значение, и инстанс уже отыграл свое, значит он разблокируется и будет воспроизведен другой инстанс и заблокируется
 \return возвращает упакованные значения порядкового номера звука (в массиве набора) в 32 старших битах и id инстанса звука в 32 младших битах
*/
SX_LIB_API uint64_t SSCore_SndkitPlay(
	ID idSndKit,			//!< id набора 
	uint64_t id2,			//!< либо #SOUND_SNDKIT_INSTANCE_BLOCK либо #SOUND_SNDKIT_INSTANCE_NOTBLOCK либо должен быть упакованным значением идентификаторов заблокированного инстанса звука
	const float3 *pPos = 0, //!< если 3d, позиция звука в пространстве
	float fVolume = -1.f,	//!< громкость [0, 1], если <0 тогда будет использовано ранее заданное значение
	float fPan = 0.f		//!< если 2d, смещение между ушами [-1, 1]
	);

//! проиграть случайны звук из 2d набора, аргументы аналогичны #SSCore_SndkitPlay
#define SSCore_SndkitPlay2d(idSndKit, id2, fVolume, fPan) (SSCore_SndkitPlay(idSndKit, id2, 0, fVolume, fPan))

//! проиграть случайны звук из 3d набора, аргументы аналогичны #SSCore_SndkitPlay
#define SSCore_SndkitPlay3d(idSndKit, id2, pPos) (SSCore_SndkitPlay(idSndKit, id2, pPos))

//! остановить проигрывание звуков в наборе 
SX_LIB_API void SSCore_SndkitStop(
	ID idSndKit,	//!< id набора 
	uint64_t id2	//!< результат SSCore_SndkitPlay если параметр id2 имел значение отличное от #SOUND_SNDKIT_INSTANCE_NOTBLOCK
	);

//**************************************************************************

//! получить id по имени набора
SX_LIB_API ID SSCore_SndkitGetID(const char *szName);

//! получить номер канала
SX_LIB_API ID SSCore_SndkitGetChannel(ID idSndKit);

//! получить имя по id набора
SX_LIB_API void SSCore_SndkitGetName(ID idSndKit, char *szName);

//! удалить набор
SX_LIB_API void SSCore_SndkitDelete(ID idSndKit);

//! удалить все наборы
SX_LIB_API void SSCore_SndkitDeleteAll();

//!@}


#endif

//!@} sxscore
