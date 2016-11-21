
#ifndef __sxsound
#define __sxsound

#include <gdefines.h>
#include <sxmath.h>

#define SOUND_PATH_MAX_LEN 1024
#define SOUND_MIN_SIZE_STREAM 1024*64 //минимально возможный размер потока для воспроизведения

#define SOUND_TP_BYTES	0//байты
#define SOUND_TP_SEC	1//секунды
#define SOUND_TP_MLS	2//млсек

#define SOUND_TV_DB		3//децибелы
#define SOUND_TV_PCT	4//проценты

#define SOUND_MAX_SOUNDS 65535//максимально возможно количество инициализированных звуковых объектов

#define SOUND_STREAM		0//потоковая загрузка
#define SOUND_STATIC		1//статичная загрузка

#define SOUND_EFFECT_ON		1			//включение эффекта
#define SOUND_EFFECT_OFF	0			//выключение эффекта
#define SOUND_EFFECT_INVALID_KEY	-1	//ошибка, неправильное значение ключа (к примеру при получении bool значения эффекта)

//форматы звукрвых файлов
#define SOUND_NON_FORMAT	-2	//не известный формат
#define SOUND_WAV_FORMAT	0	//wav формат
#define SOUND_OGG_FORMAT	1	//ogg формат

//сообщения отчета ядра звукового движка
#define SOUND_OK			0//операция успешно завершена
#define SOUND_CORE_ERR_FAILED_INIT		-3//не удалось установить звуковое устройство
#define SOUND_CORE_ERR_FAILED_CL		-4//ошибка установления уровня кооперации
#define SOUND_CORE_ERR_FAILED_PRIM_BUF	-5//ошибка создания первичного звукового буффера
#define SOUND_CORE_ERR_FAILED_SET_FORMAT -6//не удалось установить формат первичному буфферу

#define SOUND_ERR_OPEN			-7	//ошибка открытия, воможно файла не существует
#define SOUND_ERR_FORMAT		-8	//неизвестный формат файла
#define SOUND_ERR_HEADER		-9	//ошибка заголовка файла
#define SOUND_ERR_CREATE_SB		-10	//ошибка создания вторичного звукового буффера
#define SOUND_ERR_UNCPECIFIED	-11	//неопознанная ошибка

//ошибки звукового буффера
#define SOUND_ERR_CONTROLUNAVAIL	-12	//ошибка доступа к управлению буффером, флаг управления должен ставится при создании
#define SOUND_ERR_GENERIC			-13	//неопределенная ошибка в подсистеме Direct Sound
#define SOUND_ERR_INVALIDCALL		-14	//недопустимый вызов функции
#define SOUND_ERR_INVALIDPARAM		-15	//неверный аргумент передан в вызываемую функцию
#define SOUND_ERR_PRIOLEVELNEEDED	-16	//не установлен приоритетный режим
#define SOUND_ERR_NOINTERFACE		-17	//запрошенный интерфейс недоступен
#define SOUND_ERR_EFFECT_NON_SETUP	-18	//не удалось установить эффект
#define SOUND_ERR_NULL_BUFFER		-19	//не инициализирован буффер, возможно он удален

#define SOUND_ERR_OGG_DECODING	-20	//ошибка декодирования
#define SOUND_ERR_LOCK			-21	//ошибка блокирования
#define SOUND_ERR_SIZING		-22	//ошибка размерности/указания размера

//значения эффектов в массиве эффектов
#define SOUND_EFFECT_GARGLE		0
#define SOUND_EFFECT_CHORUS		1
#define SOUND_EFFECT_FLANDER	2
#define SOUND_EFFECT_ECHO		3
#define SOUND_EFFECT_DISTORTION	4
#define SOUND_EFFECT_COMPRESSOR	5
#define SOUND_EFFECT_PARAMEQ	6
#define SOUND_EFFECT_I3DL2REVERB	7
#define SOUND_EFFECT_WAVESREVERB	8

/////////////////

SX_LIB_API long SSound_0GetVersion(); //возвращает версию ядра
SX_LIB_API void SSound_Dbg_Set(report_func rf); //установка вывода в лог
SX_LIB_API void SSound_0Create(const char* name, HWND hwnd, bool is_unic = true); //создание системы звука

SX_LIB_API void SSound_AllUpdate(float3* campos,float3* camdir); //обновление всех звуковых файлов

SX_LIB_API DWORD SSound_Create2d(const char *file, bool looping = 0, int streaming = 0, DWORD size_stream = 0); //загрузка 2д звука
SX_LIB_API DWORD SSound_Create3d(const char *file, bool looping, int streaming, DWORD size_stream, 
	float max_dist, float damping = 1, float shift_pan = 0.1f);//загрузка 3д звука

SX_LIB_API void SSound_Play(DWORD id); //воспроизвести звук с текущей позиции
SX_LIB_API void SSound_Stop(DWORD id); //остановить звук

//громкость
SX_LIB_API void SSound_SetVolume(DWORD id, long volume, int type = SOUND_TV_DB);
SX_LIB_API long SSound_GetVolume(DWORD id, int type = SOUND_TV_DB);

//текущая позиция воспроизведения
SX_LIB_API void SSound_SetCurrentPos(DWORD id, DWORD pos, int	type = SOUND_TP_BYTES);
SX_LIB_API DWORD SSound_GetCurrentPos(DWORD id, int type = SOUND_TP_BYTES);

//смещение звука между ушами
SX_LIB_API void SSound_SetPan(DWORD id, long value, int type = SOUND_TV_DB);
SX_LIB_API long SSound_GetPan(DWORD id, int type = SOUND_TV_DB);

SX_LIB_API void SSound_SetFrequency(DWORD id, long value); //установить частоту звука
SX_LIB_API DWORD SSound_GetFrequency(DWORD id); //получить частоту звука
SX_LIB_API DWORD SSound_GetFrequencyOriginal(DWORD id); //получить оригинальную частоту звука

SX_LIB_API bool SSound_Playing(DWORD id); //воспроизводиться ли звук

//эффекты накладывающиеся на звук
SX_LIB_API int SSound_EffectIsOn(DWORD id, int effect);				//включен ли эффект 
SX_LIB_API int SSound_EffectOnOff(DWORD id, int effect, int switch_);	//включить/отключить эффект

SX_LIB_API int SSound_SetEffectGargle(DWORD id, DWORD RateHz, DWORD WaveShape);
SX_LIB_API int SSound_SetEffectChorus(DWORD id, float	WetDryMix, float Depth, float Feedback, float Frequency, long Waveform, float Delay, long Phase);
SX_LIB_API int SSound_SetEffectFlanger(DWORD id, float	WetDryMix, float Depth, float Feedback, float Frequency, long Waveform, float Delay, long Phase);
SX_LIB_API int SSound_SetEffectEcho(DWORD id, float	WetDryMix, float Feedback, float LeftDelay, float RightDelay, long PanDelay);
SX_LIB_API int SSound_SetEffectDistortion(DWORD id, float	Gain, float Edge, float PostEQCenterFrequency, float PostEQBandwidth, float PreLowpassCutoff);
SX_LIB_API int SSound_SetEffectCompressor(DWORD id, float	Gain, float Attack, float Release, float Threshold, float Ratio, float Predelay);
SX_LIB_API int SSound_SetEffectParameq(DWORD id, float	Center, float Bandwidth, float Gain);
SX_LIB_API int SSound_SetEffectI3DL2Reverb(DWORD id,
	long Room, long RoomHF, float RoomRolloffFactor, float DecayTime, float DecayHFRatio, long Reflections,
	float ReflectionsDelay, long Reverb, float ReverbDelay, float Diffusion, float Density, float HFReference
	);
SX_LIB_API int SSound_SetEffectWavesReverb(DWORD id, float InGain, float ReverbMix, float ReverbTime, float HighFreqRTRatio);

SX_LIB_API float SSound_GetCountSec(DWORD id); //длина в секундах
SX_LIB_API DWORD SSound_GetBytesPerSec(DWORD id); //байт в секунде
SX_LIB_API DWORD SSound_GetSize(DWORD id); //размер в байтах сырых данных
SX_LIB_API void SSound_GetPathFile(DWORD id, char* path); //путь до звукового файла

//максимальная дистанция на которой звук уже не слышен
SX_LIB_API float SSound_GetMaxDist(DWORD id);
SX_LIB_API void SSound_SetMaxDist(DWORD id, float value);

SX_LIB_API void SSound_SetPos(DWORD id, float3* pos); //установить 3д позицию в мире для воспроизведения звука

#endif