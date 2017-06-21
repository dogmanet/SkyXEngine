
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Заголовочный файл sxscore - звукового ядра
*/

/*! \defgroup sxscore sxscore - звуковое ядро
@{*/

#ifndef __sxsound
#define __sxsound

#include <gdefines.h>

#if defined(_DEBUG)
#pragma comment(lib, "sxcore_d.lib")
#else
#pragma comment(lib, "sxcore.lib")
#endif
#include <core\\sxcore.h>

/*! \name Стандартные функции библиотеки
@{*/
SX_LIB_API long SSCore_0GetVersion();			//!< возвращает версию подсистемы геометрии
SX_LIB_API void SSCore_Dbg_Set(report_func rf);	//!< установить новую функцию вывода сообщений
SX_LIB_API void SSCore_0Create(	//!< инициализация подсистемы
	const char* name,			//!< имя
	HWND hwnd,
	const char* std_path_sound,	//!< путь до папки с моделями (статическая геометрия и растительность)
	bool is_unic = true			//!< должна ли подсистема быть уникальной по имени
	);

SX_LIB_API void SSCore_AKill();	//!< уничтожение подсистемы
//!@}

#define SOUND_MAX_SIZE_STDPATH	256	/*!< абсолюный путь до директории с файлами */
#define SOUND_MAX_SIZE_PATH		128	/*!< относительный путь до файла со звуком */

#define SOUND_MIN_SIZE_STREAM	1024*64 /*!< минимально возможный размер потока для воспроизведения */

/*! \name Данные для первичного буфера
@{*/

#define SOUND_G_CHANELS_COUNT	2		/*!< количество каналов */
#define SOUND_G_BITS_PER_SEC	48000	/*!< количество бит в секунду */
#define SOUND_G_BITS_PER_SAMPLE	32		/*!< количество бит в семпле */

//!@}

#define SOUND_OGG_BITS_PER_SAMPLE	16	/*!< количество бит на сэмпл для ogg, возможно 8 или 16 */

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

/*! \name Значения эффектов в массиве эффектов
@{*/

#define SOUND_EFFECT_GARGLE			0
#define SOUND_EFFECT_CHORUS			1
#define SOUND_EFFECT_FLANDER		2
#define SOUND_EFFECT_ECHO			3
#define SOUND_EFFECT_DISTORTION		4
#define SOUND_EFFECT_COMPRESSOR		5
#define SOUND_EFFECT_PARAMEQ		6
#define SOUND_EFFECT_I3DL2REVERB	7
#define SOUND_EFFECT_WAVESREVERB	8

//!@}

//! форматы звукрвых файлов
enum SoundFileFormat
{
	sff_unknown,//!< не известный формат
	sff_wav,	//!< wave формат
	sff_ogg,	//!< ogg формат
};

//! состояния воспроизведения звукового объекта
enum SoundObjState
{
	sos_stop,	//!< остановлен
	sos_pause,	//!< приостановлен
	sos_play,	//!< проигрывается
};

SX_LIB_API void SSCore_StdPathSet(const char* path);	//!< установить путь относительно которого будут загружатсья файлы
SX_LIB_API void SSCore_StdPathGet(char* path);			//!< в path записывает текущий путь относительно которого происходит загрузка файлов
SX_LIB_API void SSCore_Clear();	//!< очистка всего списка звуков (полное их удаление)

//! обновление состояний всех звуков
SX_LIB_API void SSCore_Update(
	float3* viewpos,	//!< текущая позиция наблюдателя
	float3* viewdir		//!< текущее направление взгляда
	);	

SX_LIB_API int SSCore_SndsPlayCountGet();	//!< количество проигрываемых на данный момент звуков
SX_LIB_API int SSCore_SndsLoadCountGet();	//!< количество загруженных на данный момент звуков

//! загрузка 2d (фонового) звука
SX_LIB_API ID SSCore_SndCreate2d(
	const char *file,		//!< путь до файла, относительно стандартного
	bool looping = false,	//!< зацикливать ли воспроизведение
	DWORD size_stream = 0	//!< размер потока в байтах для потокового воспроизведения, если 0 тогда полностью грузить в память
	);

//! загрузка 3d звука
SX_LIB_API ID SSCore_SndCreate3d(
	const char *file,		//!< путь до файла, относительно стандартного
	bool looping,			//!< зацикливать ли воспроизведение
	DWORD size_stream,		//!< размер потока в байтах для потокового воспроизведения, если 0 тогда полностью грузить в память
	float dist,				//!< радиус слышимости в метрах
	float shift_pan = 0.1f	//!< изменение позиционирования звука [0-1], на сколько будет смещен звук (между ушами слушателя) при поворотах камеры к источнику звука, чем ближе к объекту тем меньше разница в позиционировании при поворотах
	);

SX_LIB_API void SSCore_SndInstancePlay2d(ID id, int volume = 100, int pan = 0);
SX_LIB_API void SSCore_SndInstancePlay3d(ID id, float3* pos);

SX_LIB_API ID SSCore_SndCreate2dInst(const char *file, bool looping = false, DWORD size_stream = 0);
SX_LIB_API ID SSCore_SndCreate3dInst(const char *file, bool looping, DWORD size_stream, float dist, float shift_pan = 0.1f);

SX_LIB_API ID SSCore_SndFind2dInst(const char * file);
SX_LIB_API ID SSCore_SndFind3dInst(const char * file);

SX_LIB_API bool SSCore_SndIsInit(ID id);	//!< инициализирован ли звук с идентификатором id
SX_LIB_API void SSCore_SndDelete(ID id);	//!< удалить звук по его id

SX_LIB_API void	SSCore_SndPlay(ID id, int looping = -1);	//!< воспроизвести звук, looping зацикливать ли воспроизведение, 0 - нет, >0 да, <0 не учитывать данное значение
SX_LIB_API void	SSCore_SndPause(ID id);						//!< приостановить
SX_LIB_API void	SSCore_SndStop(ID id);						//!< остановить

SX_LIB_API void SSCore_SndStateSet(ID id, SoundObjState state);	//!< устанавливает состояние проигрывания звука
SX_LIB_API SoundObjState SSCore_SndStateGet(ID id);				//!< возвращает состояние проигрывания звука на данный момент

//! устанавить позицию проигрывания
SX_LIB_API void SSCore_SndPosCurrSet(
	ID id,						//!< идентификатор звука
	DWORD pos,					//!< значение позиции исходя из параметра type
	int type = SOUND_POS_BYTES	//!< тип значения pos, SOUND_POS_
	);	

//! возвращает позицию проигрывания
SX_LIB_API DWORD SSCore_SndPosCurrGet(
	ID id,						//!< идентификатор звука
	int type = SOUND_POS_BYTES	//!< тип возвращаемого значения, SOUND_POS_
	);

//! устанавить громкость
SX_LIB_API void SSCore_SndVolumeSet(
	ID id,						//!< идентификатор звука
	long volume,				//!< значение громкости исходя из параметра type
	int type = SOUND_VOL_PCT	//!< тип значения volume, SOUND_VOL_
	);

//! возвращает громкость
SX_LIB_API long SSCore_SndVolumeGet(
	ID id,						//!< идентификатор звука
	int type = SOUND_VOL_PCT	//!< тип возвращаемого значения, SOUND_VOL_
	);

//! установка позиционирования между ушами
SX_LIB_API void SSCore_SndPanSet(
	ID id,						//!< идентификатор звука
	long value,					//!< значение позиционирования исходя из параметра type
	int type = SOUND_VOL_PCT	//!< тип значения value, SOUND_VOL_
	);

//! возвращает позиционирование между ушами
SX_LIB_API long SSCore_SndPanGet(
	ID id,						//!< идентификатор звука
	int type = SOUND_VOL_PCT	//!< тип возвращаемого значения, SOUND_VOL_
	);

SX_LIB_API void SSCore_SndFreqCurrSet(ID id, DWORD value);	//!< установка частоты воспроизведения
SX_LIB_API DWORD SSCore_SndFreqCurrGet(ID id);				//!< возвращает текущую частоту воспроизведения
SX_LIB_API DWORD SSCore_SndFreqOriginGet(ID id);			//!< возвращает оригинальную частоту воспроизведения

SX_LIB_API void SSCore_SndPosWSet(ID id, float3* pos);		//!< установка мировой позиции звука (только для 3d звуков)
SX_LIB_API void SSCore_SndPosWGet(ID id, float3* pos);		//!< возвращает мировую позицию звука (только для 3d звуков)

SX_LIB_API int SSCore_SndLengthSecGet(ID id);				//!< длина в секундах
SX_LIB_API DWORD SSCore_SndBytesPerSecGet(ID id);			//!< количество байт в секунде
SX_LIB_API DWORD SSCore_SndSizeGet(ID id);					//!< размер в байтах PCM данных
SX_LIB_API void SSCore_SndFileGet(ID id, char* path);		//!< относительный путь до звукового файла

SX_LIB_API float SSCore_SndDistAudibleGet(ID id);				//!< возвращает дистанцию слышимости
SX_LIB_API void SSCore_SndDistAudibleSet(ID id, float value);	//!< установка дистанции слышимости в метрах

//!@}

#endif

//!@} sxscore
