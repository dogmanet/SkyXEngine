
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

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
#include <common/string.h>
#include <common/string_utils.h>
#include <common/file_utils.h>
#include <common/assotiativearray.h>
#include "sxscore.h"

//##########################################################################

/*! проверка допустимости id звука, в случае провала вылет */
#define SOUND_PRECOND(id, retval) \
if ((UINT)id >= m_aSounds.size() || id < 0 || !(m_aSounds[id]))\
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unresolved address to sound %d", GEN_MSG_LOCATION, id); return retval; }

/*! создание имени звука (нужно для разделения между каналами */
#define SOUND_CREATE_NAME(szStr, szFile, idChannel) sprintf(szStr, "%d|%s", idChannel, szFile);

/*! проверка допустимости id канала, в случае провала вылет */
#define SOUND_CHANNEL_PRECOND(id, retval) \
if (!(id >= 0 && id < SOUND_CHANNELS_COUNT)) \
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unresolved id channel %d", GEN_MSG_LOCATION, id); return retval; }

/*! проверка проигрываемости канала */
#define SOUND_CHANNEL_PLAYING(id, retval) \
if (m_aChannels[id] != 1) \
{return retval; }

/*! проверка допустимости id набора звуков, в случае провала вылет */
#define SOUND_SNDKIT_PRECOND(id, retval) \
if (!(id >= 0 && (UINT)id < m_aSoundKits.size() && m_aSoundKits[id])) \
{LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unresolved id sound kit %d", GEN_MSG_LOCATION, id); return retval; }

//##########################################################################

//! просчет громкости звука, пределы [0, 1]
inline float Snd3dComVolume(
	const float3 &vSndPos,		//!< позиция звука
	const float3 &vViewPos,		//!< позиция слушателя
	const float fDistAudible	//!< дистанция слышимости
	)
{
	float fVolume = 1.f - (SMVector3Distance(vSndPos, vViewPos) / fDistAudible);// *(-10000);

	return saturatef(fVolume);
}

//! просчет смещения звука между ушами слушателя
inline int Snd3dComPan(
	const float3 &vSndPos,		//!< позиция звука
	const float3 &vViewPos,		//!< позиция слушателя
	const float3 &vViewDir,		//!< направление взгляда слушателя
	const float fDistAudible,	//!< дистанция слышимости
	const float fShiftPan		//!< коэфициент смещения между ушами слушателя
	)
{
	float fDist = SMVector3Distance(vSndPos, vViewPos);
	float3 fVec = vViewPos + vViewDir;

	float str = (vSndPos.x - vViewPos.x)*(vSndPos.z - fVec.z) - (vSndPos.z - vViewPos.z)*(vSndPos.x - fVec.x);
	return ((str * (fDist / fDistAudible)) * fShiftPan * float(-10000));
}

//! парсинг строки szSrcStr на составляющие элементы (если есть): путь до звука и параметры воспроизведения
inline bool SndGetDataFromStr(const char *szSrcStr, String &sFile, float &fDistAudible, float &fVolume, Array<UINT> &aDelays)
{
	bool existsData = false;

	Array<String> aStrConfig = StrExplode(szSrcStr, " ", false);

	if (aStrConfig[0].length() == 0)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - invalid file", GEN_MSG_LOCATION);
		return false;
	}

	sFile = StrTrim(aStrConfig[0].c_str());

	if (aStrConfig.size() >= 2)
	{
		if (aStrConfig[1][0] == 'd' || aStrConfig[1][0] == 'v')
			existsData = true;

		if (aStrConfig[1][0] == 'd')
			sscanf(aStrConfig[1].c_str() + 1, "%f", &fDistAudible);
		else if (aStrConfig[1][0] == 'v')
			sscanf(aStrConfig[1].c_str() + 1, "%f", &fVolume);
	}

	if (aStrConfig.size() >= 3)
	{
		if (aStrConfig[2][0] == 'd' && fDistAudible > -2.f)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unresolved double instruction for 'distance of audibility'", GEN_MSG_LOCATION);
			return false;
		}

		if (aStrConfig[2][0] == 'v' && fVolume > -2.f)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unresolved double instruction for 'volume'", GEN_MSG_LOCATION);
			return false;
		}

		if (aStrConfig[2][0] == 'd' || aStrConfig[2][0] == 'v')
			existsData = true;

		if (aStrConfig[2][0] == 'd')
			sscanf(aStrConfig[2].c_str() + 1, "%f", &fDistAudible);
		else if (aStrConfig[2][0] == 'v')
			sscanf(aStrConfig[2].c_str() + 1, "%f", &fVolume);
	}

	for (int k = 3, kl = aStrConfig.size(); k < kl; ++k)
	{
		String str = aStrConfig[k];
		aDelays.push_back(aStrConfig[k].toUnsLongInt());
	}

	if (aDelays.size() > 0)
		existsData = true;

	return existsData;
}

//##########################################################################

/*! \name Обработчики ogg
@{*/

//! чтение файла
size_t OggCallbackRead(void *ptr, size_t size, size_t nmemb, void *datasource);

//! закртытие файла
int OggCallbackClose(void* datasource);

//! позиционирование
int OggCallbackSeek(void *datasource, ogg_int64_t offset, int whence);

//! размер файла
long OggCallbackTell(void* datasource);

//!@}

//##########################################################################

//структура для загрузки wave файла
struct CSoundWaveHeader
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

//##########################################################################

//! менеджер звуков
class CSoundManager
{
public:
	CSoundManager();
	~CSoundManager();

	SX_ALIGNED_OP_MEM

	void clear();

	void init(HWND hWnd);

	void setMainVolume(float fVolume);

	//! структура данных для воспроизведения с задержками
	struct CPlayDelay
	{
		CPlayDelay()
		{
			m_iCurrPlayDelay = 0;
			m_uiPlayDelayStart = 0;
		}

		//! структура для хранения данных о текущей настройке проигрывания
		struct CTimeDelay
		{
			CTimeDelay(){ m_uiTime = 0; m_isDelay = false; }
			CTimeDelay(UINT uiTime, bool isDelay)
			{
				m_uiTime = uiTime; m_isDelay = isDelay;
			}

			//! время в млсек
			UINT m_uiTime;

			//! это время задержки в проигрывании или нет?
			bool m_isDelay;
		};

		//! массив настроек прогирывания
		Array<CTimeDelay> m_aPlayDelay;

		//! текущий индекс в #m_aPlayDelay
		int m_iCurrPlayDelay;

		//! стартовое время задержки в млсек (если засекали время)
		UINT m_uiPlayDelayStart;

	};

	//! структура данных базового понятия звука
	struct CSoundBase
	{
		CSoundBase()
		{ 
			m_pSoundBuffer = 0; m_state = SOUND_OBJSTATE_STOP; m_isLooping = false; m_isPlayDelay = false; m_uiBytesPerSec = 0; m_fVolume = 1.f; m_fPan = 0;
		}

		~CSoundBase()
		{
			mem_release(m_pSoundBuffer);
		}

		//! звуковой буфер инстанса
		IDirectSoundBuffer8 *m_pSoundBuffer;

		//! количество байт в секунде
		UINT m_uiBytesPerSec;

		//! состояние воспроизведения
		SOUND_OBJSTATE m_state;

		//! зацикливать ли воспроизведение
		bool m_isLooping;

		//! используется ли воспроизведение с задержками
		bool m_isPlayDelay;

		//! объект с данными проигрывания
		CPlayDelay oPlayDelay;

		//! громкость [0, 1]
		float m_fVolume;

		//! смещение между ушами слушателя [-1, 1]
		float m_fPan;
	};

	//! структура данных инстанса звука
	struct CSoundInstance : public CSoundBase
	{
		CSoundInstance(){ m_pSoundBuffer = 0; m_uiBytesPerSec = 0; m_busy = SOUND_SNDINSTANCE_BUSY_FREE; m_state = SOUND_OBJSTATE_STOP; m_isPlayDelay = false; }
		CSoundInstance(IDirectSoundBuffer8 *pBuffer, UINT uiBytesPerSec, const float3_t *vPos, SOUND_SNDINSTANCE_BUSY busy)
		{
			m_pSoundBuffer = pBuffer; 
			m_uiBytesPerSec = uiBytesPerSec;
			if (vPos) 
				m_vPos = *vPos; 
			m_busy = busy;
			m_state = SOUND_OBJSTATE_STOP;
			m_isPlayDelay = false;
		}

		//! позиция воспроизведения (если надо)
		float3_t m_vPos;

		//! занят ли инстанс в данный момент #SOUND_SNDINSTANCE_BUSY
		SOUND_SNDINSTANCE_BUSY m_busy;
	};

	//! расширенное понятие звука
	struct CSound : public CSoundBase
	{
		CSound();
		~CSound();
		SX_ALIGNED_OP_MEM

		//! путь загруженного звука
		char m_szRPath[SOUND_MAX_SIZE_PATH];

		//! идентификатор (номер в общем массиве звуков)
		ID m_id;

		//! канал
		ID m_idChannel;

		//! 3д воспроизвдеение звука?
		bool m_is3d;

		/*! количество загрузок звука, в основном сделано для инстансовых, если один и тот же звук загружался несколько раз то он будет загружен единожды, но счетчик будет показывать колиество,
		 и звук удалиться только тогда когда последний объект его удалит
		*/
		int m_iCountLoad;

		//! изменение позиционирования звука, на сколько будет смещен звук при поворотах камеры к источнику звука, чем ближе к объекту тем меньше разница в позиционировании при поворотах
		float m_fShiftPan;

		//! дистанция слышимости
		float m_fDistAudible;

		//! разрешено ли создавать инстансы звука (false по дефолту)
		bool m_isInst;

		//! потом чтения файла
		FILE *m_pStream;

		//! поток для декодирования ogg
		OggVorbis_File *m_pVorbisFile;		

		//! массив инстансов
		Array<CSoundInstance> m_aInstances;

		//! формат файла
		SOUND_FILEFORMAT m_format;		

		//! полный размер в байтах (для wav исключая заголовочную структуру)
		UINT m_uiSizeFull;				
		
		//! позиция источника звука (если используется без инстансов, хотя устанавливается главному и при инстансах)
		float3 m_vPosition;	

		//сброс громкости при отдалении на метр, т.е. count_volume = volume - dist * Damping %
		//float Damping;	

		//! оригинальная частота
		UINT m_uiFrecOrigin;	

		//! размер потока в байтах
		UINT m_uiStreamSize;

		//! длина звука в секундах
		int m_iLengthSec;

		//! количество каналов
		int m_iChannelsCount;

		//! частота дискретизации
		int m_iSampleRate;

		//! количество бит в сэмпле
		int m_iBitsPerSample;

		//размеры сплитов потока
		UINT m_uiSplit1Size;
		UINT m_uiSplit2Size;
		UINT m_uiSplit3Size;

		//заглушки для работы апдейта на каждый сплит в отдельности
		bool m_isWork1;
		bool m_isWork2;
		bool m_isWork3;
		bool m_isWork4;

		//! воспроизведение только началось? (для потока)
		bool m_isStarting;

		//! номер текущего активного сплита
		int m_iSplitActive;	

		//! сколько раз был полностью перезагружен поток
		int m_iRePlayCount;	

		//! сколько раз нужно полностью перезагрузить поток чтоб дойти до конца
		int	m_iRePlayEndCount;
	};

	//**********************************************************************

	//! набор звуков, для случайного воспроизведения звуков из массива однотипных звуков
	struct СSoundKit
	{
		СSoundKit();
		~СSoundKit();
		
		//! имя набора
		char m_szName[SOUND_MAX_SIZE_SNDKIT_NAME];

		//! объект набора
		struct CSoundKitObject
		{
			//! идентификатор звука
			ID m_id;

			//! массив задержек
			Array<UINT> m_aDelays;

			//! громкость звука [0,1]
			float m_fVolume;
		};

		//! массив звуков
		Array<CSoundKitObject> m_aSounds;

		//! канал набора звуков
		ID m_idChannel;

		//! 3d (true) или 2d (false) набор?
		bool m_is3D;

		//! общая дистанция слышимости для 3д звуков
		float m_fDistAudible;
	};

	//! создать набор
	ID sndkitCreate(const char *szName, ID idChannel, bool is3D, float fDistAudible = SOUND_DIST_AUDIBLE_DEFAULT);

	ID sndkitCreateFromList(const char *szName, ID idChannel, Array<String> aStrings, bool is3D, float fDistAudible = SOUND_DIST_AUDIBLE_DEFAULT, float fVolume = 1.f);

	//! добавить звук в набор, если fDistAudible < 0 то будут задействованы настройки самого набора
	void sndkitAddSound(ID idSndKit, const char *szFile, float fDistAudible, float fVolume = 1.f, UINT *pArrDelay = 0, int iSizeArrDelay = 0);

	//! получить id по имени набора
	ID sndkitGetID(const char *szName);

	//! получить номер канала
	ID sndkitGetChannel(ID idSndKit);

	//! получить имя по id набора
	void sndkitGetName(ID idSndKit, char *szName);

	//! удалить набор
	void sndkitDelete(ID idSndKit);

	//! удалить все наборы
	void sndkitDeleteAll();

	/*! проиграть случайный звук
	 \note если набор 3д звуков, то позиция обязательна
	 \note id2 должен принимать либо #SOUND_SNDKIT_INSTANCE_BLOCK либо #SOUND_SNDKIT_INSTANCE_NOTBLOCK либо должен быть упакованным значением идентификаторов заблокированного инстанса звука
	  если инстанс воспроизводимого звука заблокирован, то он не будет использоваться другими пока владелец его не разблокирует
	 \return возвращает упакованные значения порядкового номера звука (в массиве набора) в 32 старших битах и id инстанса звука в 32 младших битах
	*/
	uint64_t sndkitPlay(ID idSndKit, uint64_t id2, const float3 *pPos = 0, float fVolume = -1.f, float fPan = 0.f);

	//! остановить проигрывание звуков в наборе
	void sndkitStop(ID idSndKit, uint64_t id2);

	//**********************************************************************

	//! добавить канал
	void channelAdd(ID idChannel, bool isPlaying = false);

	//! существует ил канал
	bool channelExists(ID idChannel);

	//! количество звуков в канале
	int channelGetSndCount(ID idChannel);

	//! проигрывать звуки канала
	void channelPlay(ID idChannel);

	//! проигрывается ли канал?
	bool channelPlaying(ID idChannel);

	//! остановить проигрывание звуков канала
	void channelStop(ID idChannel);

	//! проигрывать звуки только этого канала
	void channelPlayOnly(ID idChannel);

	//! воспроизвести звук канала (используется внутри функций каналов для воспроизведения каналов)
	void channelSndPlay(ID idChannel, ID idSound);

	//! остановить звук канала (используется внутри функций каналов для воспроизведения каналов)
	void channelSndStop(ID idChannel, ID idSound);

	//**********************************************************************

	ID soundCreate2d(const char *szFile, ID idChannel, UINT uiSizeStream = 0);
	ID soundCreate3d(const char *szFile, ID idChannel, UINT uiSizeStream, float fDist);

	ID soundCreate2dInst(const char *szFile, ID idChannel);
	ID soundCreate3dInst(const char *szFile, ID idChannel, float fDist);

	ID soundFind2dInst(const char *szFile, ID idChannel);
	ID soundFind3dInst(const char *szFile, ID idChannel);

	/*! воспроизведение инстанса звука, если нужны задержки то надо их указывать в pArrDelay и iSizeArrDelay, иначе в обоих 0, 
	если нужен 3д звук (если это на самом деле 3д звук) то надо указать pPos, иначе 0 и будет 2д, для 2д по желанию можно указать громкость iVolume и смещение между ушами iPan
	*/
	ID soundInstancePlay(ID idSound, bool isBlocked, bool isLooping, UINT *pArrDelay, int iSizeArrDelay, const float3 *pPos, float fVolume = 1.f, float fPan = 0.f);

	bool soundInstancePlaying(ID idSound, ID idInstance);
	void soundInstanceStop(ID idSound, ID idInstance);
	void soundInstanceFree(ID idSound, ID idInstance);

	bool soundIsInit(ID idSound);
	void soundDelete(ID idSound);

	//! проиграть
	void soundPlay(ID idSound, bool isLooping = false, UINT *pArrDelay = 0, int iSizeArrDelay = 0);

	//! инициализация масисва задержек для воспроизведения звука, если указать iSizeArrDelay = -1 тогда будут задействованы прошлые настройки
	void soundInitPlayDelay(CSoundBase *pSndbase, UINT *pArrDelay, int iSizeArrDelay);

	//! продолжить воспроизведение (с задержками) остановленного звука
	void soundResumePlayDelay(CSoundBase *pSndBase);

	//! приостановить
	void soundPause(ID idSound);

	//! остановить
	void soundStop(ID idSound);

	void soundSetState(ID idSound, SOUND_OBJSTATE state);
	SOUND_OBJSTATE soundGetState(ID idSound);

	//текащая позиция проигрывания
	void soundSetPosPlay(ID idSound, UINT uiPos);
	UINT soundGetPosPlay(ID idSound);

	//громкость
	void soundSetVolume(ID idSound, float fVolume);
	float soundGetVolume(ID idSound);

	//позиционирование между динамиками
	void soundSetPan(ID idSound, float fPan);
	float soundGetPan(ID idSound);

	//частота
	void soundSetFreqCurr(ID idSound, UINT uiFrec);
	UINT soundGetFreqCurr(ID idSound);
	UINT soundGetFreqOrigin(ID idSound);

	void soundSetPosWorld(ID idSound, const float3 *pPos);
	void soundGetPosWorld(ID idSound, float3 *pPos);


	//длина в секундах
	int soundGetLengthSec(ID idSound);

	//байт в секунде
	UINT soundGetBytesPerSec(ID idSound);

	//размер в байтах PCM данных
	UINT soundGetSize(ID idSound);

	//путь до звукового файла
	void soundGetFile(ID idSound, char *szPath);

	float soundGetDistAudible(ID idSound);
	void soundSetDistAudible(ID idSound, float fDistAudible);

	void UpdatePlayDelay(CSoundBase *pSndBase);

	void update(const float3 *pViewPos, const float3 *pViewDir);

	SOUND_FILEFORMAT fileFormat(const char *szFile);

	int soundsGetPlayCount();
	int soundsGetLoadCount();

private:

	void load(CSound *pSnd, const char *szPath, SOUND_FILEFORMAT fmt);

	void loadWAV(CSound *pSnd, const char *szPath);

	void loadOGG(CSound *pSnd, const char *szPath);

	IDirectSoundBuffer8* soundBufferCreate(CSoundWaveHeader *pHeader);

	void soundDataWAVLoad(IDirectSoundBuffer8 *pBuffer, int iLockPos, FILE *pData, int iSize, UINT uiFlag = 0);
	void soundDataOGGLoad(OggVorbis_File *pVorbisFile, IDirectSoundBuffer8 *pBuffer, int iLockPos, int iSize, UINT uiFlag);

	void reLoadSplit(ID id, UINT Pos, UINT Size);

	ID addSound(CSound *pSnd);

	//**********************************************************************

	//! массив со всеми звуковыми объектами
	Array<CSound*> m_aSounds;

	//! ассоциативный массив всех 2д звуков arr[name] = id;
	AssotiativeArray<AAStringNR, ID, false, 16> m_a2dInst;

	//! ассоциативный массив всех 3д звуков arr[name] = id;
	AssotiativeArray<AAStringNR, ID, false, 16> m_a3dInst;

	//! массив наборов звуков
	Array<СSoundKit*> m_aSoundKits;

	//! массив каналов
	int m_aChannels[SOUND_CHANNELS_COUNT];
	
	//! звуковое устройство
	IDirectSound8 *m_pDeviceSound;		

	//! первичный буфер
	IDirectSoundBuffer *m_pPrimaryBuffer;	

	//! количество проигрываемых звуков
	int m_iSoundsPlayCount;

	//! количество загруженных звуков (с учетом как проигрывающихся так и простаивающих)
	int m_iSoundsLoadCount;

	//! предыдущая позиция наблюдателя
	float3 m_vOldViewPos;

	//! предыдущее направление взгляда наблюдателя
	float3 m_vOldViewDir;
};

#endif
