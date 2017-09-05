
#ifndef __SOUND_H
#define __SOUND_H

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "libogg_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")
#include <gdefines.h>
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdint.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <common/aastring.h>
#include <common/array.h>
#include <common/assotiativearray.h>
#include "sxscore.h"

extern report_func g_fnReportf;

struct AAStringNR : public AAString
{
	__forceinline AAStringNR(const char * str)
	{
		tmpName = str;
		Name[0] = 0;
	}

	__forceinline AAStringNR()
	{
		tmpName = NULL;
		Name[0] = 0;
	}

	__forceinline bool operator==(const AAStringNR & str) const
	{
		return(stricmp(tmpName ? tmpName : Name, str.tmpName ? str.tmpName : str.Name) == 0);
	}
};


#define SOUND_PRECOND(id, retval) \
if((UINT)id >= ArrSounds.size() || !(ArrSounds[id]))\
	{g_fnReportf(REPORT_MSG_LEVEL_ERROR, "%s - sxsound - unresolved address to sound %d", gen_msg_location, id); return retval; }

inline long SOUND_3D_COM_VOLUME(const float3 & snd_pos, const float3 & view_pos, const float snd_distaudible)
{
	long vol = (SMVector3Distance(snd_pos, view_pos) / snd_distaudible) * (-10000);

	if (vol > 0)
		vol = 0;

	if (vol < -10000)
		vol = -10000;

	return vol;
}

//чтение файла
size_t ogg_read(void *ptr, size_t size, size_t nmemb, void *datasource);
//закртытие файла
int ogg_close(void* datasource);
//позиционирование
int ogg_seek(void *datasource, ogg_int64_t offset, int whence);
//размер файла
long ogg_tell(void* datasource);

inline long SOUND_3D_COM_PAN(const float3 & snd_pos, const float3 & view_pos, const float3 & view_dir, const float snd_distaudible, const float snd_shiftpan)
{
	float dist = SMVector3Distance(snd_pos, view_pos);
	float3 vec = view_pos + view_dir;

	float str = (snd_pos.x - view_pos.x)*(snd_pos.z - vec.z) - (snd_pos.z - view_pos.z)*(snd_pos.x - vec.x);
	return ((str * (dist / snd_distaudible)) * snd_shiftpan * (-10000));
}

//структура для загрузки wave файла
struct SoundWaveHeader
{
	char	RiffSig[4];
	int32_t	ChunkSize;
	char	Sig[4];
	char	FormatSig[4];
	int32_t	FormatChunkSize;
	int16_t	FormatTag;
	int16_t	Channels;
	int32_t	SampleRate;
	int32_t	BytesPerSec;
	int16_t	BlockAlign;
	int16_t	BitsPerSample;
	char	DataSig[4];
	int32_t	DataSize;
};


class MainSound
{
public:
	MainSound();
	~MainSound();

	SX_ALIGNED_OP_MEM

	void Clear();

	void Init(HWND hwnd);

	struct Sound
	{
		Sound();
		~Sound();
		SX_ALIGNED_OP_MEM

		char RPath[SOUND_MAX_SIZE_PATH];
		ID Id;
		bool IsInst;
		FILE* StreamFile;
		IDirectSoundBuffer8* DSBuffer;	//звуковой буфер
		OggVorbis_File* VorbisFile;		//поток для декодирования ogg

		struct SIData
		{
			SIData(){ sbuffer = 0; busy = false; }
			SIData(IDirectSoundBuffer8* _sbuffer, float3_t* _pos, bool _busy)
			{
				sbuffer = _sbuffer; if (_pos) pos = *_pos; busy = _busy;
			}
			~SIData()
			{
				mem_release(sbuffer);
			}

			IDirectSoundBuffer8* sbuffer;
			float3_t pos;
			bool busy;
		};

		Array<SIData> DataInstances;

		SoundFileFormat Format;		//формат файла
		DWORD SizeFull;				//полный размер в байтах (для wav исключая заголовочную структуру)
		
		float3 Position;	//позиция источника звука
		//float Damping;	//сброс громкости при отдалении на метр, т.е. count_volume = volume - dist * Damping %
		
		//изменение позиционирования звука, на сколько будет смещен звук при поворотах камеры к источнику звука
		//чем ближе к объекту тем меньше разница в позиционировании при поворотах
		float ShiftPan;		

		SoundObjState State;
		DWORD FrecOrigin;	//оригинальная частота

		float DistAudible;	

		DWORD StreamSize;//размер потока в байтах
		int LengthSec;
		int ChannelsCount;
		int RateSample;
		int BitsPerSample;
		DWORD BytesPerSec;
		long Volume;	

		//размеры сплитов потока
		DWORD Split1Size;
		DWORD Split2Size;
		DWORD Split3Size;

		//заглушки для работы апдейта на каждый сплит в отдельности
		bool BF1;
		bool BF2;
		bool BF3;
		bool BF4;

		bool IsLooping;		//зацикливать воспроизведние?
		bool IsStarting;	//воспроизведение только началось? (для потока)
		bool Is3d;

		short SplitActive;	//активный сплит
		int RePlayCount;	//сколько раз был полностью перезагружен поток
		int	RePlayEndCount;	//сколько раз нужно полностью перезагрузить поток чтоб дойти до конца
	};

	ID SoundCreate2d(const char *file, bool looping = false, DWORD size_stream = 0);
	ID SoundCreate3d(const char *file, bool looping, DWORD size_stream, float dist, float shift_pan = 0.1f);

	ID SoundCreate2dInst(const char *file, bool looping = false, DWORD size_stream = 0);
	ID SoundCreate3dInst(const char *file, bool looping, DWORD size_stream, float dist, float shift_pan = 0.1f);

	ID SoundFind2dInst(const char * file);
	ID SoundFind3dInst(const char * file);

	void SoundInstancePlay2d(ID id, int volume=100, int pan = 0);
	void SoundInstancePlay3d(ID id, float3* pos);

	bool SoundIsInit(ID id);
	void SoundDelete(ID id);

	void	SoundPlay(ID id, int looping=-1);	//проиграть
	void	SoundPause(ID id);					//приостановить
	void	SoundStop(ID id);					//остановить

	void SoundStateSet(ID id, SoundObjState state);
	SoundObjState SoundStateGet(ID id);

	//текащая позиция проигрывания
	void SoundPosCurrSet(ID id, DWORD pos, int type = SOUND_POS_BYTES);
	DWORD SoundPosCurrGet(ID id, int type = SOUND_POS_BYTES);

	//громкость
	void SoundVolumeSet(ID id, long volume, int type = SOUND_VOL_PCT);
	long SoundVolumeGet(ID id, int type = SOUND_VOL_PCT);

	//позиционирование между динамиками
	void SoundPanSet(ID id, long value, int type = SOUND_VOL_PCT);
	long SoundPanGet(ID id, int type = SOUND_VOL_PCT);

	//частота
	void SoundFreqCurrSet(ID id, DWORD value);
	DWORD SoundFreqCurrGet(ID id);
	DWORD SoundFreqOriginGet(ID id);

	void SoundPosWSet(ID id, float3* pos);
	void SoundPosWGet(ID id, float3* pos);

	int SoundLengthSecGet(ID id);		//длина в секундах
	DWORD SoundBytesPerSecGet(ID id);	//байт в секунде
	DWORD SoundSizeGet(ID id);			//размер в байтах PCM данных
	void SoundFileGet(ID id, char* path);//путь до звукового файла

	float SoundDistAudibleGet(ID id);
	void SoundDistAudibleSet(ID id, float value);

	void Update(float3* viewpos, float3* viewdir);

	SoundFileFormat FileFormat(const char* file);

	int SoundsPlayCountGet();
	int SoundsLoadCountGet();

private:

	void Load(Sound* snd, const char* fpath, SoundFileFormat fmt);

	void LoadWAV(Sound* snd, const char* fpath);
	void LoadOGG(Sound* snd, const char* fpath);
	IDirectSoundBuffer8* SoundBufferCreate(SoundWaveHeader* hdr);
	void SoundDataWAVLoad(IDirectSoundBuffer8* DSBuffer, long LockPos, FILE* data, long Size, DWORD flag = 0);
	void SoundDataOGGLoad(OggVorbis_File* VorbisFile, IDirectSoundBuffer8 *DSBuffer, long LockPos, long Size, DWORD flag);

	void ReLoadSplit(ID id, DWORD Pos, DWORD Size);

	ID AddSound(Sound* snd);

	Array<Sound*> ArrSounds;	//массив со всеми звуковыми объектами
	AssotiativeArray<AAStringNR, ID, false, 16> AArr2dInst;
	AssotiativeArray<AAStringNR, ID, false, 16> AArr3dInst;
	
	IDirectSound8* DeviceSound;		//звуковое устройство
	IDirectSoundBuffer* DSPrimary;	//первичный буфер
	int SoundsPlayCount;			//количество проигрываемых звуков
	int SoundsLoadCount;			//количество загруженных звуков (с учетом как проигрывающихся так и простаивающих)

	float3 OldViewPos;
	float3 OldViewDir;
};

#endif
