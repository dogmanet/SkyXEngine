
#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#pragma once

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")

#if defined(_DEBUG)
#pragma comment(lib, "libogg_static_d.lib")
#pragma comment(lib, "libvorbis_static_d.lib")
#pragma comment(lib, "libvorbisfile_static_d.lib")
#elif
#pragma comment(lib, "libogg_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")
#endif

#include <mmsystem.h>
#include <dsound.h>

#include <libvorbis\include\vorbis\codec.h>
#include <libvorbis\include\vorbis\vorbisfile.h>
	
#include <sound\\sxCallBackOgg.cpp>

//структура для загрузки wav файла
struct WaveHeader 
{
	char	RiffSig[4];
	long	WaveformChunkSize;
	char	WaveSig[4];
	char	FormatSig[4];
	long	FormatChunkSize;
	short	FormatTag;
	short	Channels;
	long	SampleRate;
	long	BytesPerSec;
	short	BlockAlign;
	short	BitsPerSample;
	char	DataSig[4];
	long	DataSize;
};

//общая структура для всех звуковых объектов
struct SoundCommonObject
{
	DWORD	DataSize;		//размер PCM данных
	const char*	Path;		//путь до файла со звуком
	float	CountSec;		//длительность звука в секундах

	int		CountChannels;	//каналы
	long	SampleRate;		//rate
	int		BitsPerSample;	//битность сэмпла
	long	BytesPerSec;	//количество байт в сэмпле
};

struct SoundLoadDataObject
{
	SoundLoadDataObject()
	{
		DSBuffer		= 0;
		File			= 0;
		InfoObject		= new SoundCommonObject();
		HowFormatFile	= -1;
		VorbisFile		= 0;
	}

	IDirectSoundBuffer8*	DSBuffer;		//вторичный звуковой буффер
	FILE*					File;			//поток
	SoundCommonObject*		InfoObject;		//информация о загруженном звуковом файле
	int						HowFormatFile;	//формат, 0 - wave, 1 - ogg, -1 - неизвестный формат, все остальные данные в структуре не инициализированы
	OggVorbis_File*			VorbisFile;		//если HowFormatFile == 1, то есть, если загрузили ogg
};


//звуковой объект
class SXSoundObject
{
public:

	SX_ALIGNED_OP_MEM

	SXSoundObject(){}
	//SXSoundObject(const char *file,bool looping,int streaming,DWORD size_stream);	//создание 2d звука
	//SXSoundObject(const char *file,bool looping,int streaming,DWORD size_stream,float max_dist,float damping,float shift_pan);	//создание 3d звука

	long Load(const char *file, bool looping, int streaming, DWORD size_stream);	//создание 2d звука
	long Load(const char *file, bool looping, int streaming, DWORD size_stream, float max_dist, float damping, float shift_pan);
	inline void	Play();		//проиграть
	inline void	Stop();		//остановить
	void	Update(float3* campos = 0, float3* camdir = 0);	//обновление проигрывания потока
	void	Update3d(float3 *CamPos,float3 *CamLook);

	//текащая позиция проигрывания
	void	SetCurrentPos(DWORD	pos,int	type=SOUND_TP_BYTES);
	inline DWORD	GetCurrentPos(int type=SOUND_TP_BYTES);

	//громкость
	inline void	SetVolume		(long volume,int type=SOUND_TV_DB);
	inline long	GetVolume		(int type = SOUND_TV_DB);	

	//позиционирование между динамиками
	inline void	SetPan			(long value,int type=SOUND_TV_DB);	
	inline long	GetPan			(int type = SOUND_TV_DB);

	//частота
	inline void	SetFrequency	(long value);	
	inline DWORD	GetFrequency	();
	inline DWORD	GetFrequencyOriginal();

	inline bool	Playing();	//проигрывается ли звук

	int		EffectIsOn(int effect);					//включен ли эффект 
	int		EffectOnOff(int effect,int switch_);	//включить/отключить эффект

	//установка эффектов
	int SetEffectGargle	(DWORD RateHz,DWORD WaveShape);
	int SetEffectChorus		(float	WetDryMix,	float Depth,	float Feedback,				float Frequency,		long Waveform,	float Delay,long Phase);
	int SetEffectFlanger	(float	WetDryMix,	float Depth,	float Feedback,				float Frequency,		long Waveform,	float Delay,long Phase);
	int SetEffectEcho		(float	WetDryMix,	float Feedback,	float LeftDelay,			float RightDelay,		long PanDelay);
	int SetEffectDistortion	(float	Gain,		float Edge,		float PostEQCenterFrequency,float PostEQBandwidth,	float PreLowpassCutoff);
	int SetEffectCompressor	(float	Gain,		float Attack,	float Release,				float Threshold,		float Ratio,	float Predelay);
	int SetEffectParameq	(float	Center,		float Bandwidth,float Gain);

	int SetEffectI3DL2Reverb	(	
									long Room,long RoomHF,float RoomRolloffFactor,float DecayTime,float DecayHFRatio,long Reflections,
									float ReflectionsDelay,long Reverb,float ReverbDelay,float Diffusion,float Density,float HFReference
								);
	int SetEffectWavesReverb(float InGain,float ReverbMix,float ReverbTime,float HighFreqRTRatio);

	//параметры звукового файла
	inline float GetCountSec();	//длина в секундах
	inline DWORD GetBytesPerSec();	//байт в секунде
	inline DWORD GetSize();		//размер в байтах PCM данных
	inline void GetPathFile(char* path);	//путь до звукового файла
	inline DWORD GetID();

	inline float GetMinDist();
	inline void SetMinDist(float value);

	inline float GetMaxDist();
	inline void SetMaxDist(float value);

	~SXSoundObject();

	float3 Position;	//позиция источника звука
	float Damping;		//сброс громкости при отдалении на метр, т.е. count_volume = volume - dist * Damping %
	float ShiftPan;		//изменение позиционирования звука, на сколько будет смещен звук при поворотах камеры к источнику звука
	//чем ближе к объекту тем меньше разница в позиционировании при поворотах

	int Looping;//зацикливать воспроизведние?

private:

	DWORD LastPos;
	int State;
	DWORD BeginFrec;
	int LoadStatic(const char *file,int format);			
	int LoadDynamicWAV(const char *file,DWORD size_stream);
	int LoadDynamicOGG(const char *file,DWORD size_stream);
	void ReLoadSplit(DWORD Pos, DWORD Size);	//перезагрузка части потока

	//float MinDist;	//радиус в котором звук слышиться на 100%
	float MaxDist;
	
	DWORD	MaxSize;		//размер потока в байтах

	long VolumeSound;

	SoundLoadDataObject* DataObject;
	
	//размеры сплитов потока
	DWORD	SizeSplit1;	
	DWORD	SizeSplit2;
	DWORD	SizeSplit3;

	char File[SOUND_PATH_MAX_LEN];

	//заглушки для работы апдейта на каждый сплит в отдельности
	bool BF1;
	bool BF2;
	bool BF3;
	bool BF4;

	bool	IsStarting;		//воспроизведение только началось? (для потока)
	bool	IsStreaming;	//звук воспроизводится потоково?
	bool	Is3d;
	DWORD	ID;

	WORD	ActiveSplit;	//активный сплит
	DWORD	HowRePlay;		//сколько раз был полностью перезагружен поток
	DWORD	CountRePlayEnd;	//сколько раз нужно полностью перезагрузить поток чтоб дойти до конца

	bool IsInitEffect[9];

	DSFXGargle		SXGargle;
	DSFXChorus		SXChorus;
	DSFXFlanger		SXFlanger;
	DSFXEcho		SXEcho;
	DSFXDistortion	SXDistortion;
	DSFXCompressor	SXCompressor;
	DSFXParamEq		SXParamEq;
	DSFXI3DL2Reverb	SXI3DL2Reverb;
	DSFXWavesReverb	SXWavesReverb;

	void	InitEffectBool();
	void	ReInitEffect();
	int		PrecondInitEffect(HRESULT hr);
	GUID	GetGuidEffect(int effect);
};

//класс инициализации и примитивного управления
class SXMainSound
{
public:
	SXMainSound	(const char* name);
	long Init(HWND hwnd);
	void SetStdPath(const char* path);
	void GetStdPath(char* path);
	~SXMainSound		();
	void	Update(float3* campos, float3* camdir);
	DWORD	AddSoundObject		(SXSoundObject *Obj);
	inline SXSoundObject* GetSoundObject(DWORD id);
	void	DeleteSoundObject	(DWORD num);

	DWORD	GetCountPlaySounds();
	DWORD	GetCountLoadSounds();
	
	DWORD	GetCountKey();
	char StdPath[CORE_NAME_MAX_LEN];
private:
	char Name[CORE_NAME_MAX_LEN];
	
	//все объекты звукового движка
	SXSoundObject *ArrSounds[SOUND_MAX_SOUNDS];	//массив со всеми звуковыми объектами

	IDirectSound8*		DeviceSound;				//звуковое устройство
	IDirectSoundBuffer*	DSPrimary;					//первичный буфер
	DWORD				CountKeyInArr;				//количество ключей в верхнем массиве
	DWORD				CountPlaySounds;			//количество проигрываемых звуков
	DWORD				CountLoadSounds;			//количество загруженных звуков (с учетом как проигрывающихся так и простаивающих)
};

//дополнительнео пространство имен для управления звуком
namespace SoundControl
{
	////////////////////////////////////////////
	inline void ComMsg(int msg,const char* file="",const char* fileerror=__FILE__,int strerror=__LINE__);
	bool LoadSoundDataWAV(IDirectSoundBuffer8 *DSBuffer,long LockPos,FILE* data,long Size,DWORD	flag);	//заполнение буфера
	int LoadWAV(const char *FileName,SoundLoadDataObject** DataObject);									//загрузка wav файла

	WaveHeader*	GetHdrWav(FILE *fp); //возвращает прочитанную структуру WaveHeader

	int LoadSoundDataOGG(OggVorbis_File* VorbisFile,IDirectSoundBuffer8 *DSBuffer,DWORD LockPos,DWORD Size,DWORD flag);//заполнение буфера
	int LoadOGG	(const char	*FileName,SoundLoadDataObject** DataObject);												//загрузка ogg файла

	IDirectSoundBuffer8* CreateSoundBufferStream (WaveHeader *Hdr,DWORD size);	//создание вторичного буфера определенного размера
	IDirectSoundBuffer8* CreateSoundBuffer (WaveHeader	*Hdr);					//создание вторичного буфера
	
	int HowFormat(const char* file);	//возвращает формат файла
};

#endif