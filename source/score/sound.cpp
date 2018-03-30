
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

#include "sound.h"

//############################################################################

size_t OggCallbackRead(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	FILE* f = (FILE*)datasource;
	return fread(ptr, 1, size * nmemb, f);
}

int OggCallbackClose(void* datasource)
{
	FILE* f = (FILE*)datasource;
	fclose(f);
	return 0;
}

int OggCallbackSeek(void *datasource, ogg_int64_t offset, int whence)
{
	FILE* f = (FILE*)datasource;
	switch (whence)
	{
	case SEEK_SET: return fseek(f, offset, SEEK_SET);
	case SEEK_CUR: return fseek(f, offset, SEEK_CUR);
	case SEEK_END: return fseek(f, offset, SEEK_END);
	default: return -1;
	}
	return 1;
}

long OggCallbackTell(void* datasource)
{
	FILE* f = (FILE*)datasource;
	return ftell(f);
}

//############################################################################

CSoundManager::CSoundManager()
{
	m_pDeviceSound = 0;
	m_pPrimaryBuffer = 0;

	for (int i = 0; i < SOUND_CHANNELS_COUNT; ++i)
		m_aChannels[i] = SOUND_CHANNEL_NONE;
}

CSoundManager::~CSoundManager()
{
	clear();
	mem_release_del(m_pPrimaryBuffer);
	mem_release_del(m_pDeviceSound);
}

void CSoundManager::clear()
{
	for(UINT i = 0; i < m_aSounds.size(); ++i)
	{
		mem_delete(m_aSounds[i]);
	}
}

CSoundManager::CSound::CSound()
{
	ZeroMemory(this, sizeof(CSoundManager::CSound));
	m_id = SOUND_FAIL_ID;
	m_fVolume = 1.f;
}

CSoundManager::CSound::~CSound()
{
	if (m_pStream)
		fclose(m_pStream);

	//mem_release_del(m_pSoundBuffer);
	if (m_pVorbisFile)
	{
		ov_clear(m_pVorbisFile);
		mem_delete(m_pVorbisFile);
	}
}

//##########################################################################

CSoundManager::СSoundKit::СSoundKit()
{ 
	m_szName[0] = 0; 
	m_idChannel = SOUND_FAIL_ID; 
	m_is3D = false;
	m_fDistAudible = SOUND_DIST_AUDIBLE_DEFAULT;
}

CSoundManager::СSoundKit::~СSoundKit()
{
	
}

ID CSoundManager::sndkitCreate(const char *szName, ID idChannel, bool is3D, float fDistAudible)
{
	// список звуков подлежащаих загрузке
	СSoundKit *pSndKit = new СSoundKit();
	pSndKit->m_idChannel = idChannel;
	pSndKit->m_is3D = is3D;
	pSndKit->m_fDistAudible = fDistAudible;
	sprintf(pSndKit->m_szName, "%s", szName);

	m_aSoundKits.push_back(pSndKit);

	return m_aSoundKits.size() - 1;
}

ID CSoundManager::sndkitCreateFromList(const char *szName, ID idChannel, Array<String> aStrings, bool is3D, float fDistAudible, float fVolume)
{
	СSoundKit *pSndKit = new СSoundKit();
	pSndKit->m_idChannel = idChannel;
	pSndKit->m_is3D = is3D;
	pSndKit->m_fDistAudible = fDistAudible;
	sprintf(pSndKit->m_szName, "%s", szName);
	m_aSoundKits.push_back(pSndKit);
	ID idSndKit = m_aSoundKits.size() - 1;

	String sFileData;
	float fDistAudibleData = -2.f;
	float fVolumeData = -2.f;
	Array<UINT> aDelaysData;

	for (int i = 0, il = aStrings.size(); i < il; ++i)
	{
		aDelaysData.clearFast();
		fDistAudibleData = -2.f;
		fVolumeData = -2.f;

		if (SndGetDataFromStr(aStrings[i].c_str(), sFileData, fDistAudibleData, fVolumeData, aDelaysData))
		{
			if (fDistAudibleData == -2.f)
				fDistAudibleData = -1.f;

			if (fVolumeData == -2.f)
				fVolumeData = fVolume;

			sndkitAddSound(idSndKit, sFileData.c_str(), fDistAudibleData, fVolumeData, (aDelaysData.size() > 0 ? &(aDelaysData[0]) : 0), aDelaysData.size());
		}
		else
			sndkitAddSound(idSndKit, StrTrim(aStrings[i].c_str()).c_str(), pSndKit->m_fDistAudible, fVolume);
	}

	return idSndKit;
}

void CSoundManager::sndkitAddSound(ID idSndKit, const char *szFile, float fDistAudible, float fVolume, UINT *pArrDelay, int iSizeArrDelay)
{
	SOUND_SNDKIT_PRECOND(idSndKit, _VOID);

	СSoundKit *pSndKit = m_aSoundKits[idSndKit];

	ID idSnd = SOUND_FAIL_ID;

	if (pSndKit->m_is3D)
		idSnd = soundCreate3dInst(szFile, pSndKit->m_idChannel, (fDistAudible > 0 ? fDistAudible : pSndKit->m_fDistAudible));
	else
		idSnd = soundCreate2dInst(szFile, pSndKit->m_idChannel);

	СSoundKit::CSoundKitObject oSndKitObj;
	oSndKitObj.m_id = idSnd;
	oSndKitObj.m_fVolume = fVolume;

	for (int i = 0, il = iSizeArrDelay; i < il; ++i)
	{
		oSndKitObj.m_aDelays.push_back(pArrDelay[i]);
	}

	pSndKit->m_aSounds.push_back(oSndKitObj);
}


ID CSoundManager::sndkitGetID(const char *szName)
{
	for (int i = 0, il = m_aSoundKits.size(); i < il; ++i)
	{
		if (stricmp(m_aSoundKits[i]->m_szName, szName) == 0)
			return i;
	}

	return SOUND_FAIL_ID;
}

ID CSoundManager::sndkitGetChannel(ID idSndKit)
{
	SOUND_SNDKIT_PRECOND(idSndKit, SOUND_FAIL_ID);

	return m_aSoundKits[idSndKit]->m_idChannel;
}


void CSoundManager::sndkitGetName(ID idSndKit, char *szName)
{
	SOUND_SNDKIT_PRECOND(idSndKit, _VOID);

	strcpy(szName, m_aSoundKits[idSndKit]->m_szName);
}


void CSoundManager::sndkitDelete(ID idSndKit)
{
	SOUND_SNDKIT_PRECOND(idSndKit, _VOID);

	СSoundKit *pSndKit = m_aSoundKits[idSndKit];

	for (int i = 0, il = pSndKit->m_aSounds.size(); i < il; ++i)
	{
		soundDelete(pSndKit->m_aSounds[i].m_id);
	}

	mem_delete(m_aSoundKits[idSndKit]);
}


void CSoundManager::sndkitDeleteAll()
{
	for (int i = 0, il = m_aSoundKits.size(); i < il; ++i)
	{
		sndkitDelete(i);
	}
}


uint64_t CSoundManager::sndkitPlay(ID idSndKit, uint64_t id2, const float3 *pPos, float fVolume, float fPan)
{
	SOUND_SNDKIT_PRECOND(idSndKit, SOUND_FAIL_ID);

	СSoundKit *pSndKit = m_aSoundKits[idSndKit];

	// если канал проигрывается
	SOUND_CHANNEL_PLAYING(pSndKit->m_idChannel, ((id2 != SOUND_SNDKIT_INSTANCE_BLOCK && id2 != SOUND_SNDKIT_INSTANCE_NOTBLOCK) ? id2 : SOUND_FAIL_ID));

	// если id2 валиден и инстанс проигрывается
	if ((id2 != SOUND_SNDKIT_INSTANCE_BLOCK && id2 != SOUND_SNDKIT_INSTANCE_NOTBLOCK) && soundInstancePlaying(pSndKit->m_aSounds[SOUND_DECODE2ID_HI(id2)].m_id, SOUND_DECODE2ID_LO(id2)))
		return id2;

	// если id2 валиден то освобождаем инстанс
	if (id2 != SOUND_SNDKIT_INSTANCE_BLOCK && id2 != SOUND_SNDKIT_INSTANCE_NOTBLOCK)
		soundInstanceFree(pSndKit->m_aSounds[SOUND_DECODE2ID_HI(id2)].m_id, SOUND_DECODE2ID_LO(id2));

	ID idRand = rand() % (pSndKit->m_aSounds.size());
	ID idSndInstance = -1;

	UINT *pArr = 0;
	int iSizeArr = pSndKit->m_aSounds[idRand].m_aDelays.size();

	if (iSizeArr > 0)
		pArr = &(pSndKit->m_aSounds[idRand].m_aDelays[0]);

	if (pSndKit->m_is3D)
	{
		if (!pPos)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unresolved call function playing 3d sound without position", GEN_MSG_LOCATION);
			return SOUND_FAIL_ID;
		}

		idSndInstance = soundInstancePlay(pSndKit->m_aSounds[idRand].m_id, id2 != SOUND_SNDKIT_INSTANCE_NOTBLOCK, false, pArr, iSizeArr, pPos, (fVolume < 0 ? pSndKit->m_aSounds[idRand].m_fVolume : fVolume));
	}
	else
		idSndInstance = soundInstancePlay(pSndKit->m_aSounds[idRand].m_id, id2 != SOUND_SNDKIT_INSTANCE_NOTBLOCK, false, pArr, iSizeArr, 0, (fVolume < 0 ? pSndKit->m_aSounds[idRand].m_fVolume : fVolume), fPan);
	
	/*if (idSndInstance > 0)
	{
		CSound* snd = m_aSounds[pSndKit->m_aSounds[SOUND_DECODE2ID_HI(id2)].m_id];
		SOUND_SNDINSTANCE_BUSY busy = snd->m_aInstances[SOUND_DECODE2ID_LO(id2)].m_busy;
		int qwerty = 0;
	}*/
	
	//ibReport(REPORT_MSG_LEVEL_NOTICE, "idSndInstance = %d\n", idSndInstance);
	return SOUND_ENCODE2ID(idRand, idSndInstance);
}


void CSoundManager::sndkitStop(ID idSndKit, uint64_t id2)
{
	SOUND_SNDKIT_PRECOND(idSndKit, _VOID);

	if (id2 == SOUND_SNDKIT_INSTANCE_BLOCK)
		return;

	СSoundKit *pSndKit = m_aSoundKits[idSndKit];

	soundInstanceStop(pSndKit->m_aSounds[SOUND_DECODE2ID_HI(id2)].m_id, SOUND_DECODE2ID_LO(id2));
}

//##########################################################################

void CSoundManager::channelAdd(ID idChannel, bool isPlaying)
{
	SOUND_CHANNEL_PRECOND(idChannel, _VOID);
		
	if (m_aChannels[idChannel] == SOUND_CHANNEL_NONE)
		m_aChannels[idChannel] = isPlaying;
}

bool CSoundManager::channelExists(ID idChannel)
{
	SOUND_CHANNEL_PRECOND(idChannel, false);

	return (m_aChannels[idChannel] != SOUND_CHANNEL_NONE);
}

int CSoundManager::channelGetSndCount(ID idChannel)
{
	SOUND_CHANNEL_PRECOND(idChannel, SOUND_CHANNEL_NONE);

	int iCount = 0;
	for (int i = 0, il = m_aSounds.size(); i < il; ++i)
	{
		if (m_aSounds[i]->m_idChannel == idChannel)
			++iCount;
	}

	return iCount;
}

void CSoundManager::channelPlay(ID idChannel)
{
	SOUND_CHANNEL_PRECOND(idChannel, _VOID);

	for (int i = 0, il = m_aSounds.size(); i < il; ++i)
	{
		if (m_aSounds[i] && m_aSounds[i]->m_idChannel == idChannel)
		{
			channelSndPlay(idChannel, i);
		}
	}

	m_aChannels[idChannel] = SOUND_CHANNEL_PLAY;
}

bool CSoundManager::channelPlaying(ID idChannel)
{
	SOUND_CHANNEL_PRECOND(idChannel, false);

	return m_aChannels[idChannel];
}

void CSoundManager::channelStop(ID idChannel)
{
	SOUND_CHANNEL_PRECOND(idChannel, _VOID);

	for (int i = 0, il = m_aSounds.size(); i < il; ++i)
	{
		if (m_aSounds[i] && m_aSounds[i]->m_idChannel == idChannel)
		{
			channelSndStop(idChannel, i);
		}
	}

	m_aChannels[idChannel] = SOUND_CHANNEL_STOP;
}

void CSoundManager::channelPlayOnly(ID idChannel)
{
	SOUND_CHANNEL_PRECOND(idChannel, _VOID);

	for (int i = 0, il = m_aSounds.size(); i < il; ++i)
	{
		if (m_aSounds[i])
		{
			if (m_aSounds[i]->m_idChannel == idChannel)
			{
				channelSndPlay(idChannel, i);
			}
			else
			{
				channelSndStop(idChannel, i);
			}
		}
	}
}

void CSoundManager::soundResumePlayDelay(CSoundBase *pSndBase)
{
	if (!pSndBase || !(pSndBase->m_isPlayDelay))
		return;

	//если ключ валиден
	if (pSndBase->oPlayDelay.m_iCurrPlayDelay >= 0 && pSndBase->oPlayDelay.m_aPlayDelay.size() > pSndBase->oPlayDelay.m_iCurrPlayDelay)
	{
		//если текущее значение задержка
		if (pSndBase->oPlayDelay.m_aPlayDelay[pSndBase->oPlayDelay.m_iCurrPlayDelay].m_isDelay)
			pSndBase->oPlayDelay.m_uiPlayDelayStart = GetTickCount() - pSndBase->oPlayDelay.m_uiPlayDelayStart;
		else
			pSndBase->m_pSoundBuffer->Play(0, 0, 0);
	}
	//иначе ключ невалиден, просто доигрываем
	else
		pSndBase->m_pSoundBuffer->Play(0, 0, 0);
}

void CSoundManager::channelSndPlay(ID idChannel, ID idSound)
{
	if (idSound < m_aSounds.size() && idSound >= 0 && m_aSounds[idSound] && m_aSounds[idSound]->m_idChannel == idChannel)
	{
		CSound *pSnd = m_aSounds[idSound];
		DWORD dwStatus = 0;

		if (!(pSnd->m_isPlayDelay) && SUCCEEDED(pSnd->m_pSoundBuffer->GetStatus(&dwStatus)) && !(dwStatus & DSBSTATUS_PLAYING) && pSnd->m_state == SOUND_OBJSTATE_PLAY)
			pSnd->m_pSoundBuffer->Play(0, 0, (pSnd->m_uiStreamSize || pSnd->m_isLooping ? DSBPLAY_LOOPING : 0));
		//если воспроизведение с задержками
		else if (pSnd->m_isPlayDelay && pSnd->m_state == SOUND_OBJSTATE_PLAY)			
			soundResumePlayDelay(pSnd);
			

		for (int k = 0, kl = pSnd->m_aInstances.size(); k < kl; ++k)
		{
			//TODO: доделать восстановление проигрывания с задержками для инстансов

			if (pSnd->m_aInstances[k].m_state == SOUND_OBJSTATE_PAUSE)
			{
				pSnd->m_aInstances[k].m_state = SOUND_OBJSTATE_PLAY;
				
				if (!(pSnd->m_isPlayDelay))
					pSnd->m_aInstances[k].m_pSoundBuffer->Play(0, 0, (pSnd->m_aInstances[k].m_isLooping ? DSBPLAY_LOOPING : 0));
				else
					soundResumePlayDelay(&(pSnd->m_aInstances[k]));
			}
		}
	}
}

void CSoundManager::channelSndStop(ID idChannel, ID idSound)
{
	if (idSound < m_aSounds.size() && idSound >= 0 && m_aSounds[idSound] && m_aSounds[idSound]->m_idChannel == idChannel)
	{
		CSound *pSnd = m_aSounds[idSound];

		if (pSnd->m_isPlayDelay)
		{
			if (pSnd->oPlayDelay.m_iCurrPlayDelay >= 0 && pSnd->oPlayDelay.m_aPlayDelay.size() > pSnd->oPlayDelay.m_iCurrPlayDelay && pSnd->oPlayDelay.m_aPlayDelay[pSnd->oPlayDelay.m_iCurrPlayDelay].m_isDelay)
				pSnd->oPlayDelay.m_uiPlayDelayStart = GetTickCount() - pSnd->oPlayDelay.m_uiPlayDelayStart;
		}

		pSnd->m_pSoundBuffer->Stop();
		DWORD dwStatus = 0;
		CSoundInstance *pSndInst = 0;

		for (int k = 0, kl = pSnd->m_aInstances.size(); k < kl; ++k)
		{
			pSndInst = &(pSnd->m_aInstances[k]);
			dwStatus = 0;

			if (pSndInst->m_state == SOUND_OBJSTATE_PLAY && SUCCEEDED(pSndInst->m_pSoundBuffer->GetStatus(&dwStatus)) && (dwStatus & DSBSTATUS_PLAYING))
			{
				pSndInst->m_pSoundBuffer->Stop();
				pSndInst->m_state = SOUND_OBJSTATE_PAUSE;
			}

			if (pSndInst->m_isPlayDelay)
			{
				if (pSndInst->oPlayDelay.m_iCurrPlayDelay >= 0 && pSndInst->oPlayDelay.m_aPlayDelay.size() > pSndInst->oPlayDelay.m_iCurrPlayDelay && pSndInst->oPlayDelay.m_aPlayDelay[pSndInst->oPlayDelay.m_iCurrPlayDelay].m_isDelay)
					pSndInst->oPlayDelay.m_uiPlayDelayStart = GetTickCount() - pSndInst->oPlayDelay.m_uiPlayDelayStart;
			}
		}
	}
}

//##########################################################################

void CSoundManager::init(HWND hwnd)
{
	if (FAILED(DirectSoundCreate8(NULL, &m_pDeviceSound, NULL)))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - could not create sound device", GEN_MSG_LOCATION);
		return;// SOUND_INIT_ERR_INIT;
	}

	if (FAILED(m_pDeviceSound->SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE)))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - could not create cooperative level", GEN_MSG_LOCATION);
		return;// SOUND_INIT_ERR_CL;
	}

	DSBUFFERDESC  dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));

	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = 0;

	if (FAILED(m_pDeviceSound->CreateSoundBuffer(&dsbd, &m_pPrimaryBuffer, NULL)))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - could not create primary buffer", GEN_MSG_LOCATION);
		return;// SOUND_INIT_ERR_PRIM_BUF;
	}

	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));

	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.nChannels = SOUND_G_CHANELS_COUNT;
	wfex.nSamplesPerSec = SOUND_G_BITS_PER_SEC;
	wfex.wBitsPerSample = SOUND_G_BITS_PER_SAMPLE;
	wfex.nBlockAlign = (wfex.wBitsPerSample / 8) * wfex.nChannels;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

	if (FAILED(m_pPrimaryBuffer->SetFormat(&wfex)))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - could not init format", GEN_MSG_LOCATION);
		return;// SOUND_INIT_ERR_SET_FORMAT;
	}

	m_pPrimaryBuffer->SetVolume(0);
	m_pPrimaryBuffer->Play(0, 0, DSBPLAY_LOOPING);
}

//

SOUND_FILEFORMAT CSoundManager::fileFormat(const char* file)
{
	FILE *fp;

	if (!(fp = fopen(file, "rb")))
		return SOUND_FILEFORMAT_UNKNOWN;

	CSoundWaveHeader Hdr;
	fseek(fp, 0, SEEK_SET);
	fread(&Hdr, 1, sizeof(CSoundWaveHeader), fp);

	if (memcmp(Hdr.RiffSig, "RIFF", 4) == 0 || memcmp(Hdr.Sig, "WAVE", 4) == 0 || memcmp(Hdr.FormatSig, "fmt ", 4) == 0)
	{
		fclose(fp);
		return SOUND_FILEFORMAT_WAV;
	}

	if (strcmp(fp->_base, "OggS") == 0)
	{
		fclose(fp);
		return SOUND_FILEFORMAT_OGG;
	}

	fclose(fp);

	return SOUND_FILEFORMAT_UNKNOWN;
}

//#############################################################################

void CSoundManager::load(CSound* snd, const char* fpath, SOUND_FILEFORMAT fmt)
{
	if (fmt == SOUND_FILEFORMAT_OGG)
		loadOGG(snd, fpath);
	else if (fmt == SOUND_FILEFORMAT_WAV)
		loadWAV(snd, fpath);
}

void CSoundManager::loadWAV(CSound* snd, const char* fpath)
{
	CSoundWaveHeader Hdr;
	snd->m_pStream = fopen(fpath, "rb");

	fseek(snd->m_pStream, 0, SEEK_SET);
	fread(&Hdr, 1, sizeof(CSoundWaveHeader), snd->m_pStream);

	snd->m_uiSizeFull = (Hdr.ChunkSize + (sizeof(char)* 4 + sizeof(int32_t))) - sizeof(CSoundWaveHeader);

	if (snd->m_uiStreamSize == 0)
		Hdr.DataSize = snd->m_uiSizeFull;
	else
		Hdr.DataSize = snd->m_uiStreamSize;
	
	if (!(snd->m_pSoundBuffer = soundBufferCreate(&Hdr)))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - could not create sound buffer [%s]", GEN_MSG_LOCATION, snd->m_szRPath);
		return;
	}

	fseek(snd->m_pStream, sizeof(CSoundWaveHeader), SEEK_SET);

	soundDataWAVLoad(snd->m_pSoundBuffer, 0, snd->m_pStream, Hdr.DataSize, 0);

	snd->m_iLengthSec = snd->m_uiStreamSize / (Hdr.BytesPerSec);
	snd->m_iChannelsCount = Hdr.Channels;
	snd->m_iSampleRate = Hdr.SampleRate;
	snd->m_iBitsPerSample = Hdr.BitsPerSample;
	snd->m_uiBytesPerSec = ((snd->m_iBitsPerSample / 8) * snd->m_iChannelsCount) * snd->m_iSampleRate;

	if (snd->m_uiStreamSize == 0)
	{
		fclose(snd->m_pStream);
	}
	else
	{
		snd->m_uiSplit1Size = snd->m_uiStreamSize / 4;
		snd->m_uiSplit2Size = snd->m_uiSplit1Size * 2;
		snd->m_uiSplit3Size = snd->m_uiSplit1Size * 3;

		snd->m_isWork1 = false;
		snd->m_isWork2 = false;
		snd->m_isWork3 = false;
		snd->m_isWork4 = false;

		snd->m_isStarting = true;

		snd->m_iRePlayCount = 0;
		double tmpCRPE = double(snd->m_uiSizeFull) / double(snd->m_uiStreamSize);
		float Count = 0;

		while (1)
		{
			if (tmpCRPE > Count && tmpCRPE < Count + 1)
			{
				snd->m_iRePlayEndCount = Count + 1;
				break;
			}
			else if (tmpCRPE == Count)
			{
				snd->m_iRePlayEndCount = Count;
				break;
			}
			Count++;
		}
	}
}

IDirectSoundBuffer8* CSoundManager::soundBufferCreate(CSoundWaveHeader* hdr)
{
	IDirectSoundBuffer  *DSB;
	IDirectSoundBuffer8 *m_pSoundBuffer;
	DSBUFFERDESC dsbd;
	WAVEFORMATEX wfex;

	//устанавливаем формат воспроизведени§
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
	wfex.wFormatTag = WAVE_FORMAT_PCM;
	wfex.nChannels = hdr->Channels;
	wfex.nSamplesPerSec = hdr->SampleRate;
	wfex.wBitsPerSample = hdr->BitsPerSample;
	wfex.nBlockAlign = wfex.wBitsPerSample / 8 * wfex.nChannels;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

	//создаем звуковой буфер, использу¤ данные заголовка
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE;
	dsbd.dwBufferBytes = hdr->DataSize;
	dsbd.lpwfxFormat = &wfex;
	if (FAILED(m_pDeviceSound->CreateSoundBuffer(&dsbd, &DSB, NULL)))
		return NULL;

	if (FAILED(DSB->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_pSoundBuffer)))
	{
		DSB->Release();
		return NULL;
	}

	return m_pSoundBuffer;
}

void CSoundManager::soundDataWAVLoad(IDirectSoundBuffer8* m_pSoundBuffer, int LockPos, FILE* data, int Size, UINT flag)
{
	BYTE  *Ptr1, *Ptr2;
	DWORD Size1, Size2;

	if (!Size)
		return;

	if (FAILED(m_pSoundBuffer->Lock(LockPos, Size, (void**)&Ptr1, &Size1, (void**)&Ptr2, &Size2, flag)))
		return;

	fread(Ptr1, 1, Size1, data);
	if (Ptr2)
		fread(Ptr2, 1, Size2, data);

	m_pSoundBuffer->Unlock(Ptr1, Size1, Ptr2, Size2);
}

/////////////////////////////////////////////////////////////////////////////

void CSoundManager::loadOGG(CSound* snd, const char* fpath)
{
	OggVorbis_File ogg;

	snd->m_pStream = fopen(fpath, "rb");

	if (snd->m_pStream == NULL)
		return;

	if (ov_fopen(fpath, &ogg))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error reading [%s]", GEN_MSG_LOCATION, snd->m_szRPath);
		return;
	}

	snd->m_pVorbisFile = new OggVorbis_File;

	ov_callbacks cb;
	cb.close_func = OggCallbackClose;
	cb.read_func = OggCallbackRead;
	cb.seek_func = OggCallbackSeek;
	cb.tell_func = OggCallbackTell;

	ov_open_callbacks(snd->m_pStream, snd->m_pVorbisFile, 0, 0, cb);

	vorbis_info *vi = ov_info(&ogg, -1);

	if (!vi)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - error reading (info) [%s]", GEN_MSG_LOCATION, snd->m_szRPath);
		return;
	}

	snd->m_iLengthSec = float(ov_pcm_total(snd->m_pVorbisFile, -1)) / float(vi->rate);
	snd->m_iChannelsCount = vi->channels;
	snd->m_iSampleRate = vi->rate;
	snd->m_iBitsPerSample = SOUND_OGG_BITS_PER_SAMPLE;
	snd->m_uiBytesPerSec = ((snd->m_iBitsPerSample / 8) * snd->m_iChannelsCount) * snd->m_iSampleRate;
	snd->m_uiSizeFull = ov_pcm_total(snd->m_pVorbisFile, -1) * 2 * vi->channels;

	CSoundWaveHeader hdr;
	hdr.Channels = vi->channels;
	hdr.SampleRate = vi->rate;
	hdr.BitsPerSample = SOUND_OGG_BITS_PER_SAMPLE;

	if (snd->m_uiStreamSize == 0)
		hdr.DataSize = snd->m_uiSizeFull;
	else
		hdr.DataSize = snd->m_uiStreamSize;

	snd->m_pSoundBuffer = soundBufferCreate(&hdr);

	if (!snd->m_pSoundBuffer)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - could not create sound buffer [%s]", GEN_MSG_LOCATION, snd->m_szRPath);
		return;
	}

	soundDataOGGLoad(snd->m_pVorbisFile, snd->m_pSoundBuffer, 0, hdr.DataSize, 0);

	if (snd->m_uiStreamSize == 0)
	{
		fclose(snd->m_pStream);
		ov_clear(&ogg);
		ov_clear(snd->m_pVorbisFile);
		mem_delete(snd->m_pVorbisFile);
		snd->m_pVorbisFile = 0;
	}
	else
	{
		snd->m_uiSplit1Size = snd->m_uiStreamSize / 4;
		snd->m_uiSplit2Size = snd->m_uiSplit1Size * 2;
		snd->m_uiSplit3Size = snd->m_uiSplit1Size * 3;

		snd->m_isWork1 = false;
		snd->m_isWork2 = false;
		snd->m_isWork3 = false;
		snd->m_isWork4 = false;

		snd->m_isStarting = true;

		snd->m_iRePlayCount = 0;
		double tmpCRPE = double(snd->m_uiSizeFull) / double(snd->m_uiStreamSize);
		float Count = 0;

		while (1)
		{
			if (tmpCRPE > Count && tmpCRPE < Count + 1)
			{
				snd->m_iRePlayEndCount = Count + 1;
				break;
			}
			else if (tmpCRPE == Count)
			{
				snd->m_iRePlayEndCount = Count;
				break;
			}
			Count++;
		}
	}
	//return SOUND_OK;
}

void CSoundManager::soundDataOGGLoad(OggVorbis_File* VorbisFile, IDirectSoundBuffer8 *m_pSoundBuffer, int LockPos, int Size, UINT flag)
{
	char  *Ptr1, *Ptr2;
	DWORD Size1, Size2;

	if (!Size)
		return;

	if (FAILED(m_pSoundBuffer->Lock(LockPos, Size, (void**)&Ptr1, &Size1, (void**)&Ptr2, &Size2, flag)))
		return;

	DWORD total_read = 0;
	long bites_read;
	int current_section = 0;

	while (total_read < Size1)
	{
		bites_read = ov_read(VorbisFile, Ptr1 + total_read, Size1 - total_read, 0, SOUND_OGG_BITS_PER_SAMPLE_WORD, 1, &current_section);

		//конец файла
		if (bites_read == 0)
			break;

		if (bites_read > 0)
		{
			total_read += bites_read;
		}
		else if (bites_read == OV_EINVAL)
		{
			//ошибка при декодировании, нужно поставить заглушку
			LibReport(REPORT_MSG_LEVEL_WARNING,"OV_EINVAL");
		}
		else
			break;
	}


	if (Ptr2)
	{
		total_read = 0;
		while (total_read < Size2)
		{
			bites_read = ov_read(VorbisFile, Ptr2 + total_read, Size2 - total_read, 0, SOUND_OGG_BITS_PER_SAMPLE_WORD, 1, &current_section);

			if (bites_read == 0)
				break;

			if (bites_read > 0)
			{
				total_read += bites_read;
			}
			else if (bites_read == OV_EINVAL)
			{
				//ошибка при декодировании, нужно поставить заглушку
				LibReport(REPORT_MSG_LEVEL_WARNING, "OV_EINVAL");
			}
			else
				break;
		}
	}

	m_pSoundBuffer->Unlock(Ptr1, Size1, Ptr2, Size2);
}

//#############################################################################

ID CSoundManager::soundCreate2d(const char *szFile, ID idChannel, UINT uiSizeStream)
{
	if (!szFile)
		return SOUND_FAIL_ID;

	String sFileData = szFile;
	float fDistAudibleData = -1.f;
	float fVolumeData = -1.f;
	Array<UINT> aDelaysData;

	SndGetDataFromStr(szFile, sFileData, fDistAudibleData, fVolumeData, aDelaysData);

	char fullpath[SOUND_MAX_SIZE_STDPATH + SOUND_MAX_SIZE_PATH];
	sprintf(fullpath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SOUNDS), sFileData.c_str());

	if (!Core_0FileExists(fullpath))
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - file not found [%s]", GEN_MSG_LOCATION, fullpath);
		return SOUND_FAIL_ID;
	}

	SOUND_FILEFORMAT fmt = fileFormat(fullpath);

	if (fmt == SOUND_FILEFORMAT_UNKNOWN)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unknown format [%s]", GEN_MSG_LOCATION, sFileData.c_str());
		return SOUND_FAIL_ID;
	}

	channelAdd(idChannel);

	CSound *pSnd = new CSound();

	if (fDistAudibleData > 0.f)
		pSnd->m_fDistAudible = fDistAudibleData;

	if (fVolumeData > -1.f)
		pSnd->m_fVolume = fVolumeData;

	strcpy(pSnd->m_szRPath, szFile);
	pSnd->m_format = fmt;

	pSnd->m_uiStreamSize = 0;
	if (uiSizeStream > 0)
	{
		if (uiSizeStream < SOUND_MIN_SIZE_STREAM)
			pSnd->m_uiStreamSize = SOUND_MIN_SIZE_STREAM;
		else
			pSnd->m_uiStreamSize = uiSizeStream;
	}
	
	if (uiSizeStream == 0 && aDelaysData.size() > 0)
	{
		soundInitPlayDelay(pSnd, &(aDelaysData[0]), aDelaysData.size());
	}

	load(pSnd, fullpath, fmt);

	DWORD dwFrec;
	pSnd->m_pSoundBuffer->GetFrequency(&dwFrec);
	pSnd->m_uiFrecOrigin = dwFrec;
	pSnd->m_id = addSound(pSnd);
	pSnd->m_idChannel = idChannel;
	pSnd->m_iCountLoad = 1;

	return pSnd->m_id;
}

ID CSoundManager::soundCreate3d(const char *file, ID idChannel, UINT size_stream, float dist)
{
	ID idSnd = soundCreate2d(file, idChannel, size_stream);
	CSound* snd = 0;

	if (idSnd < 0)
		return idSnd;

	snd = m_aSounds[idSnd];

	if (snd->m_fDistAudible <= 0.f)
		snd->m_fDistAudible = dist;

	snd->m_is3d = true;
	snd->m_fShiftPan = SOUND_SHIFTPAN_3D;
	snd->m_idChannel = idChannel;
	
	return idSnd;
}

ID CSoundManager::soundCreate2dInst(const char *file, ID idChannel)
{
	ID idSnd = soundFind2dInst(file, idChannel);

	if (idSnd >= 0)
	{
		++(m_aSounds[idSnd]->m_iCountLoad);
		return idSnd;
	}

	idSnd = soundCreate2d(file, idChannel);

	char szStr[SOUND_MAX_SIZE_FULLPATH];
	szStr[0] = 0;
	SOUND_CREATE_NAME(szStr, idChannel, file);

	m_a2dInst[szStr] = idSnd;
	m_aSounds[idSnd]->m_isInst = true;
	m_aSounds[idSnd]->m_idChannel = idChannel;
	return idSnd;
}

ID CSoundManager::soundCreate3dInst(const char *file, ID idChannel, float dist)
{
	ID idSnd = soundFind3dInst(file, idChannel);

	if (idSnd >= 0)
	{
		++(m_aSounds[idSnd]->m_iCountLoad);
		return idSnd;
	}

	idSnd = soundCreate3d(file, idChannel, 0, dist);

	char szStr[SOUND_MAX_SIZE_FULLPATH];
	szStr[0] = 0;
	SOUND_CREATE_NAME(szStr, idChannel, file);

	m_a3dInst[szStr] = idSnd;
	m_aSounds[idSnd]->m_isInst = true;
	m_aSounds[idSnd]->m_idChannel = idChannel;

	return idSnd;
}

ID CSoundManager::soundFind2dInst(const char *file, ID idChannel)
{
	char szStr[SOUND_MAX_SIZE_FULLPATH];
	szStr[0] = 0;
	SOUND_CREATE_NAME(szStr, idChannel, file);

	ID id = SOUND_FAIL_ID;
	const AssotiativeArray<AAStringNR, ID, false, 16>::Node* pNode = 0;
	if (m_a2dInst.KeyExists(szStr, &pNode))
		id = *(pNode->Val);

	return id;
}

ID CSoundManager::soundFind3dInst(const char *file, ID idChannel)
{
	char szStr[SOUND_MAX_SIZE_FULLPATH];
	szStr[0] = 0;
	SOUND_CREATE_NAME(szStr, idChannel, file);

	ID id = SOUND_FAIL_ID;
	const AssotiativeArray<AAStringNR, ID, false, 16>::Node* pNode = 0;
	if (m_a3dInst.KeyExists(szStr, &pNode))
		id = *(pNode->Val);

	return id;
}

ID CSoundManager::soundInstancePlay(ID idSound, bool isBlocked, bool isLooping, UINT *pArrDelay, int iSizeArrDelay, const float3 *pPos, float fVolume, float fPan)
{
	SOUND_PRECOND(idSound, SOUND_FAIL_ID);

	CSound *pSnd = m_aSounds[idSound];

	SOUND_CHANNEL_PLAYING(pSnd->m_idChannel, SOUND_FAIL_ID);

	if (pSnd->m_uiStreamSize > 0)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - can not create sound instance for streaming [%s]", GEN_MSG_LOCATION, pSnd->m_szRPath);
		return SOUND_FAIL_ID;
	}

	if (pSnd->m_is3d && !pPos)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - not send position for 3d sound instance [%s]", GEN_MSG_LOCATION, pSnd->m_szRPath);
		return SOUND_FAIL_ID;
	}

	if (!(pSnd->m_is3d) && pPos)
	{
		LibReport(REPORT_MSG_LEVEL_WARNING, "%s - uresolved send position for 2d sound instance [%s]", GEN_MSG_LOCATION, pSnd->m_szRPath);
		return SOUND_FAIL_ID;
	}

	ID idInstance = -1;
	for (int i = 0; i < pSnd->m_aInstances.size(); ++i)
	{
		if (pSnd->m_aInstances[i].m_busy == SOUND_SNDINSTANCE_BUSY_FREE)
		{
			idInstance = i;
			break;
		}
	}

	if (idInstance < 0)
	{
		IDirectSoundBuffer* tsb;
		IDirectSoundBuffer8* tsb8;
		m_pDeviceSound->DuplicateSoundBuffer(pSnd->m_pSoundBuffer, &tsb);
		if (!tsb)
		{
			LibReport(REPORT_MSG_LEVEL_WARNING, "%s - can not create sound instance [%s], this is big problem", GEN_MSG_LOCATION, pSnd->m_szRPath);
			return SOUND_FAIL_ID;
		}
		tsb->QueryInterface(IID_IDirectSoundBuffer8, (void**)&tsb8);
		pSnd->m_aInstances.push_back(CSoundInstance(tsb8, pSnd->m_uiBytesPerSec, (pPos ? &float3_t(*pPos) : 0), SOUND_SNDINSTANCE_BUSY_TEMP));
		idInstance = pSnd->m_aInstances.size() - 1;
	}

	if (idInstance >= 0)
	{
		if (isBlocked || isLooping)
			pSnd->m_aInstances[idInstance].m_busy = SOUND_SNDINSTANCE_BUSY_LOCKED;
		else
			pSnd->m_aInstances[idInstance].m_busy = SOUND_SNDINSTANCE_BUSY_TEMP;

		//если установлены задержки воспроизведения
		if (pArrDelay && iSizeArrDelay > 0 || iSizeArrDelay < 0)
			soundInitPlayDelay(&(pSnd->m_aInstances[idInstance]), pArrDelay, iSizeArrDelay);

		pSnd->m_aInstances[idInstance].m_fVolume = saturatef(fVolume);
		
		//если 3д звук
		if (pSnd->m_is3d && pPos)
		{
			pSnd->m_aInstances[idInstance].m_pSoundBuffer->SetVolume(lerpf(-10000, 0, pSnd->m_aInstances[idInstance].m_fVolume * Snd3dComVolume((*pPos), m_vOldViewPos, pSnd->m_fDistAudible)));
			pSnd->m_aInstances[idInstance].m_pSoundBuffer->SetPan(Snd3dComPan((*pPos), m_vOldViewPos, m_vOldViewDir, pSnd->m_fDistAudible, pSnd->m_fShiftPan));
		}
		//иначе фоновый
		else
		{
			pSnd->m_aInstances[idInstance].m_pSoundBuffer->SetVolume(lerpf(-10000, 0, pSnd->m_aInstances[idInstance].m_fVolume));
			pSnd->m_aInstances[idInstance].m_pSoundBuffer->SetPan((fPan > 0) ? (10000 - (fPan * 100)) - 10000 : 10000 - (10000 + (fPan * 100)));
		}
		
		pSnd->m_aInstances[idInstance].m_pSoundBuffer->SetCurrentPosition(0);
		pSnd->m_aInstances[idInstance].m_isLooping = isLooping;
		
		if (!(pArrDelay && iSizeArrDelay > 0))
			pSnd->m_aInstances[idInstance].m_pSoundBuffer->Play(0, 0, ((pSnd->m_aInstances[idInstance].m_isLooping) ? DSBPLAY_LOOPING : 0));
		
		pSnd->m_aInstances[idInstance].m_state = SOUND_OBJSTATE_PLAY;

		return idInstance;
	}

	return SOUND_FAIL_ID;
}

bool CSoundManager::soundInstancePlaying(ID idSound, ID idInstance)
{
	SOUND_PRECOND(idSound, false);

	CSound* snd = m_aSounds[idSound];

	if (idInstance >= 0)
	{
		/*DWORD dwStatus;
		snd->m_aInstances[idInstance].m_pSoundBuffer->GetStatus(&dwStatus);
		return (dwStatus & DSBSTATUS_PLAYING);*/
		return (snd->m_aInstances[idInstance].m_state != SOUND_OBJSTATE_STOP);
	}

	return false;
}

void CSoundManager::soundInstanceStop(ID idSound, ID idInstance)
{
	SOUND_PRECOND(idSound, _VOID);

	CSound* snd = m_aSounds[idSound];

	if (idInstance >= 0)
	{
		snd->m_aInstances[idInstance].m_isLooping = false;
		snd->m_aInstances[idInstance].m_pSoundBuffer->Stop();
		snd->m_aInstances[idInstance].m_state = SOUND_OBJSTATE_STOP;
	}
}

void CSoundManager::soundInstanceFree(ID idSound, ID idInstance)
{
	SOUND_PRECOND(idSound, _VOID);

	CSound* snd = m_aSounds[idSound];

	if (idInstance >= 0)
	{
		if (snd->m_aInstances[idInstance].m_busy == SOUND_SNDINSTANCE_BUSY_LOCKED)
			snd->m_aInstances[idInstance].m_busy = SOUND_SNDINSTANCE_BUSY_TEMP;
	}
}

ID CSoundManager::addSound(CSound* snd)
{
	for (int i = 0, l = m_aSounds.size(); i < l; ++i)
	{
		if (m_aSounds[i] == 0)
		{
			m_aSounds[i] = snd;
			return i;
		}
	}

	m_aSounds.push_back(snd);
	return m_aSounds.size() - 1;
}

bool CSoundManager::soundIsInit(ID id)
{
	return ((ID)m_aSounds.size() > id && m_aSounds[id] != 0);
}

void CSoundManager::soundDelete(ID id)
{
	SOUND_PRECOND(id, _VOID);

	CSound* snd = m_aSounds[id];
	--(snd->m_iCountLoad);

	if (snd->m_iCountLoad > 0)
		return;

	if (snd->m_isInst)
	{
		char szStr[SOUND_MAX_SIZE_FULLPATH];
		szStr[0] = 0;
		SOUND_CREATE_NAME(szStr, snd->m_idChannel, snd->m_szRPath);

		if (snd->m_is3d)
			m_a3dInst.erase(szStr);
		else
			m_a2dInst.erase(szStr);
	}
	mem_delete(snd);
}

//#############################################################################

void CSoundManager::soundPlay(ID id, bool isLooping, UINT *pArrDelay, int iSizeArrDelay)
{
	SOUND_PRECOND(id, _VOID);

	CSound *pSnd = m_aSounds[id];

	if (pSnd->m_uiStreamSize != 0 && pArrDelay && iSizeArrDelay > 0)
	{
		LibReport(REPORT_MSG_LEVEL_ERROR, "%s - unresolved play delay for streming playing, file %s", GEN_MSG_LOCATION, pSnd->m_szRPath);
		return;
	}

	if (!m_aChannels[pSnd->m_idChannel])
		return;

	if (pArrDelay && iSizeArrDelay > 0 || iSizeArrDelay < 0)
		soundInitPlayDelay(pSnd, pArrDelay, iSizeArrDelay);

	pSnd->m_isLooping = isLooping;

	if (pSnd->m_pSoundBuffer)
	{
		if (pArrDelay && iSizeArrDelay > 0)
			pSnd->m_pSoundBuffer->SetCurrentPosition(0);
		else
			pSnd->m_pSoundBuffer->Play(0, 0, (pSnd->m_uiStreamSize || pSnd->m_isLooping ? DSBPLAY_LOOPING : 0));

		pSnd->m_state = SOUND_OBJSTATE_PLAY;
	}
}

void CSoundManager::soundInitPlayDelay(CSoundBase *pSndbase, UINT *pArrDelay, int iSizeArrDelay)
{
	if (!pSndbase)
		return;

	if (iSizeArrDelay < 0)
	{
		if (pSndbase->oPlayDelay.m_aPlayDelay.size() <= 0)
		{
			LibReport(REPORT_MSG_LEVEL_ERROR, "%s - previous delay data not init", GEN_MSG_LOCATION);
		}

		pSndbase->oPlayDelay.m_iCurrPlayDelay = 0;
		pSndbase->m_isPlayDelay = true;
		return;
	}

	if (pArrDelay == 0 || iSizeArrDelay == 0)
		return;

	pSndbase->oPlayDelay.m_aPlayDelay.clearFast();
	pSndbase->oPlayDelay.m_iCurrPlayDelay = 0;
	pSndbase->m_isPlayDelay = true;
	pSndbase->oPlayDelay.m_uiPlayDelayStart = GetTickCount();

	bool isEven = true;
	for (int i = 0, il = iSizeArrDelay; i < il; ++i)
	{
		pSndbase->oPlayDelay.m_aPlayDelay.push_back(CPlayDelay::CTimeDelay(pArrDelay[i], isEven));
		isEven = !isEven;
	}
}

void CSoundManager::soundPause(ID id)
{
	SOUND_PRECOND(id, _VOID);

	CSound* snd = m_aSounds[id];

	if (!m_aChannels[snd->m_idChannel])
		return;

	if (snd->m_pSoundBuffer)
	{
		snd->m_pSoundBuffer->Stop();
		snd->m_state = SOUND_OBJSTATE_PAUSE;
	}
}

void CSoundManager::soundStop(ID id)
{
	SOUND_PRECOND(id, _VOID);

	CSound* snd = m_aSounds[id];

	if (!m_aChannels[snd->m_idChannel])
		return;

	if (snd->m_pSoundBuffer)
	{
		snd->m_pSoundBuffer->Stop();
		snd->m_pSoundBuffer->SetCurrentPosition(0);
		snd->m_state = SOUND_OBJSTATE_STOP;
		snd->m_isLooping = false;
		snd->m_isPlayDelay = false;
	}
}

void CSoundManager::soundSetState(ID id, SOUND_OBJSTATE state)
{
	SOUND_PRECOND(id, _VOID);

	if (state == SOUND_OBJSTATE_PLAY)
		soundPlay(id);
	else if (state == SOUND_OBJSTATE_PAUSE)
		soundPause(id);
	else if (state == SOUND_OBJSTATE_STOP)
		soundStop(id);
}

SOUND_OBJSTATE CSoundManager::soundGetState(ID id)
{
	SOUND_PRECOND(id, SOUND_OBJSTATE_STOP);

	CSound* snd = m_aSounds[id];
	return snd->m_state;
}

void CSoundManager::soundSetPosPlay(ID id, UINT pos)
{
	SOUND_PRECOND(id, _VOID);

	CSound* snd = m_aSounds[id];

	DWORD PosInBytes = 0;
	if (snd->m_pSoundBuffer)
	{
		PosInBytes = pos * (snd->m_uiBytesPerSec / 1000);

		if (snd->m_uiStreamSize)
		{
			WORD HowCountRePlay = 0;
			for (WORD i = 0; i <= snd->m_iRePlayEndCount; i++)
			{
				if ((i * snd->m_uiStreamSize <= PosInBytes && (i + 1) * snd->m_uiStreamSize >= PosInBytes) || i * snd->m_uiStreamSize == PosInBytes)
				{
					HowCountRePlay = i;
					break;
				}
			}

			//wav
			if (snd->m_format == SOUND_FILEFORMAT_WAV)
				fseek(snd->m_pStream, sizeof(CSoundWaveHeader)+(HowCountRePlay  * snd->m_uiStreamSize), SEEK_SET);
			//ogg
			else if (snd->m_format == SOUND_FILEFORMAT_OGG)
				ov_pcm_seek(snd->m_pVorbisFile, (HowCountRePlay  * snd->m_uiStreamSize) / (2 * snd->m_iChannelsCount));

			DWORD SizeCountRePlay = PosInBytes - (snd->m_uiStreamSize * HowCountRePlay);

			reLoadSplit(id, 0, snd->m_uiStreamSize);

			snd->m_iRePlayCount = HowCountRePlay;
			snd->m_pSoundBuffer->SetCurrentPosition(SizeCountRePlay);

			for (WORD i = 0; i<4; i++)
			{
				if (SizeCountRePlay >= snd->m_uiSplit1Size * i && SizeCountRePlay < snd->m_uiSplit1Size * (i + 1))
				{
					snd->m_iSplitActive = i + 1;
					break;
				}
			}

			if (snd->m_iSplitActive > 1)
				snd->m_isStarting = false;
			else
				snd->m_isStarting = true;

			if (snd->m_iSplitActive - 2 > 0 && snd->m_iSplitActive - 2 < 5)
			{
				for (WORD i = 0; i<snd->m_iSplitActive - 2; i++)
					reLoadSplit(id, snd->m_uiSplit1Size*i, snd->m_uiSplit1Size);
			}

			snd->m_isWork1 = snd->m_isWork2 = snd->m_isWork3 = snd->m_isWork4 = false;
		}
		else
			snd->m_pSoundBuffer->SetCurrentPosition(PosInBytes);
	}
}

UINT CSoundManager::soundGetPosPlay(ID id)
{
	SOUND_PRECOND(id, 0);

	CSound* snd = m_aSounds[id];
	DWORD dwPosBuff, dwBytes, dwPos = 0;
	if (snd->m_pSoundBuffer)
	{
		snd->m_pSoundBuffer->GetCurrentPosition(&dwPosBuff, 0);

		if (snd->m_uiStreamSize)
			dwBytes = snd->m_iRePlayCount * snd->m_uiStreamSize + dwPosBuff;
		else
			dwBytes = dwPosBuff;

		dwPos = dwBytes / (snd->m_uiBytesPerSec / 1000);
	}
	return dwPos;
}

void CSoundManager::soundSetVolume(ID id, float fVolume)
{
	SOUND_PRECOND(id, _VOID);

	CSound *pSnd = m_aSounds[id];
	pSnd->m_fVolume = saturatef(fVolume);

	if (pSnd->m_pSoundBuffer)
	{
		int iVolume = -10000 + (pSnd->m_fVolume * 10000);

		if (!pSnd->m_is3d)
			pSnd->m_pSoundBuffer->SetVolume(iVolume);
	}
}

float CSoundManager::soundGetVolume(ID id)
{
	SOUND_PRECOND(id, 0);

	CSound *pSnd = m_aSounds[id];
	return pSnd->m_fVolume;
}


void CSoundManager::soundSetPan(ID id, float fPan)
{
	SOUND_PRECOND(id, _VOID);

	CSound *pSnd = m_aSounds[id];

	if (pSnd->m_is3d)
		return;

	pSnd->m_fPan = clampf(fPan,-1.f,1.f);

	if (pSnd->m_pSoundBuffer)
	{
		long lValue = (pSnd->m_fPan > 0) ? (10000 - (pSnd->m_fPan * 100)) - 10000 : 10000 - (10000 + (pSnd->m_fPan * 100));

		pSnd->m_pSoundBuffer->SetPan(lValue);
	}
}

float CSoundManager::soundGetPan(ID id)
{
	SOUND_PRECOND(id, 0);

	CSound *pSnd = m_aSounds[id];

	if (pSnd->m_is3d)
		return 0;

	return pSnd->m_fPan;
}


void CSoundManager::soundSetFreqCurr(ID id, UINT value)
{
	SOUND_PRECOND(id, _VOID);

	CSound* snd = m_aSounds[id];
	if (snd->m_pSoundBuffer)
		snd->m_pSoundBuffer->SetFrequency(value);
}

UINT CSoundManager::soundGetFreqCurr(ID id)
{
	SOUND_PRECOND(id, 0);

	CSound* snd = m_aSounds[id];
	static DWORD Value = 0;
	if (snd->m_pSoundBuffer)
		snd->m_pSoundBuffer->GetFrequency(&Value);
	return Value;
}

UINT CSoundManager::soundGetFreqOrigin(ID id)
{
	SOUND_PRECOND(id, 0);

	CSound* snd = m_aSounds[id];
	if (snd->m_pSoundBuffer)
		return snd->m_uiFrecOrigin;

	return 0;
}

void CSoundManager::soundSetPosWorld(ID id, const float3* pos)
{
	SOUND_PRECOND(id, _VOID);
	if (pos)
		m_aSounds[id]->m_vPosition = *pos;
}

void CSoundManager::soundGetPosWorld(ID id, float3* pos)
{
	SOUND_PRECOND(id, _VOID);
	if (pos)
		*pos = m_aSounds[id]->m_vPosition;
}

int CSoundManager::soundGetLengthSec(ID id)
{
	SOUND_PRECOND(id, 0);

	return m_aSounds[id]->m_iLengthSec;
}

UINT CSoundManager::soundGetBytesPerSec(ID id)
{
	SOUND_PRECOND(id, 0);

	return m_aSounds[id]->m_uiBytesPerSec;
}

UINT CSoundManager::soundGetSize(ID id)
{
	SOUND_PRECOND(id, 0);

	return m_aSounds[id]->m_uiSizeFull;
}

void CSoundManager::soundGetFile(ID id, char* path)
{
	SOUND_PRECOND(id, _VOID);

	if(path)
		strcpy(path,m_aSounds[id]->m_szRPath);
}


float CSoundManager::soundGetDistAudible(ID id)
{
	SOUND_PRECOND(id, 0);

	return m_aSounds[id]->m_fDistAudible;
}

void CSoundManager::soundSetDistAudible(ID id, float value)
{
	SOUND_PRECOND(id, _VOID);

	m_aSounds[id]->m_fDistAudible = value;
}

//#############################################################################

void CSoundManager::reLoadSplit(ID id, UINT Pos, UINT Size)
{
	SOUND_PRECOND(id, _VOID);

	CSound* snd = m_aSounds[id];
	if (snd->m_pSoundBuffer)
	{
		//wav
		if (snd->m_format == SOUND_FILEFORMAT_WAV)
			soundDataWAVLoad(snd->m_pSoundBuffer, Pos, snd->m_pStream, Size, 0);
		//ogg
		else if (snd->m_format == SOUND_FILEFORMAT_OGG)
		{
			soundDataOGGLoad(snd->m_pVorbisFile, snd->m_pSoundBuffer, Pos, Size, 0);
		}
	}
}

void CSoundManager::UpdatePlayDelay(CSoundBase *pSndBase)
{
	if (!pSndBase)
		return;

	DWORD dwStatus = 0;

	//если вопроизведение с задержками
	if (pSndBase->m_isPlayDelay)
	{
		if (pSndBase->m_state == SOUND_OBJSTATE_PLAY && pSndBase->m_isLooping && pSndBase->oPlayDelay.m_iCurrPlayDelay == -1 && SUCCEEDED(pSndBase->m_pSoundBuffer->GetStatus(&dwStatus)) && !(dwStatus & DSBSTATUS_PLAYING))
		{
			pSndBase->oPlayDelay.m_iCurrPlayDelay = 0;
			pSndBase->oPlayDelay.m_uiPlayDelayStart = 0;
			pSndBase->m_pSoundBuffer->SetCurrentPosition(0);
			pSndBase->m_pSoundBuffer->Play(0, 0, 0);
		}

		dwStatus = 0;

		DWORD dwCurrPos = 0;

		pSndBase->m_pSoundBuffer->GetCurrentPosition(&dwCurrPos, 0);
		dwCurrPos = dwCurrPos / (pSndBase->m_uiBytesPerSec / 1000);

		//если текущий ключ в массиве задержек валиден
		if (pSndBase->oPlayDelay.m_iCurrPlayDelay >= 0 && pSndBase->oPlayDelay.m_aPlayDelay.size() > pSndBase->oPlayDelay.m_iCurrPlayDelay)
		{
			if (
				//если текущий ключ в массиве это не задержка и текущая время воспроизведения больше либо равно установленному
				(!(pSndBase->oPlayDelay.m_aPlayDelay[pSndBase->oPlayDelay.m_iCurrPlayDelay].m_isDelay) && dwCurrPos >= pSndBase->oPlayDelay.m_aPlayDelay[pSndBase->oPlayDelay.m_iCurrPlayDelay].m_uiTime) ||

				//если текущий ключ в массиве это задержка и время с засечки уже превысило установленное значение либо равно ему
				(pSndBase->oPlayDelay.m_aPlayDelay[pSndBase->oPlayDelay.m_iCurrPlayDelay].m_isDelay && (GetTickCount() - pSndBase->oPlayDelay.m_uiPlayDelayStart) >= pSndBase->oPlayDelay.m_aPlayDelay[pSndBase->oPlayDelay.m_iCurrPlayDelay].m_uiTime)
				)
			{
				//инкрементируем текущий ключ
				++(pSndBase->oPlayDelay.m_iCurrPlayDelay);

				//если следующий ключ валиден
				if (pSndBase->oPlayDelay.m_aPlayDelay.size() > pSndBase->oPlayDelay.m_iCurrPlayDelay)
				{
					//если задержка
					if (pSndBase->oPlayDelay.m_aPlayDelay[pSndBase->oPlayDelay.m_iCurrPlayDelay].m_isDelay)
					{
						//естанавливаем и засекаем время
						pSndBase->m_pSoundBuffer->Stop();
						pSndBase->oPlayDelay.m_uiPlayDelayStart = GetTickCount();
					}
					else
					{
						//воспроизводим и обнуляем засечку
						pSndBase->m_pSoundBuffer->Play(0, 0, 0);
						pSndBase->oPlayDelay.m_uiPlayDelayStart = 0;
					}
				}
				//иначе ключ в массиве задержек невалиден
				else
				{
					//воспроизводим остаток и обнуляем данные настроек
					pSndBase->m_pSoundBuffer->Play(0, 0, 0);

					if (!(pSndBase->m_isLooping))
						pSndBase->m_isPlayDelay = false;

					pSndBase->oPlayDelay.m_iCurrPlayDelay = -1;
					pSndBase->oPlayDelay.m_uiPlayDelayStart = 0;
				}
			}
		}
	}
}

void CSoundManager::update(const float3* viewpos, const float3* viewdir)
{
	int tmpSoundsPlayCount = 0;
	int tmpSoundsLoadCount = 0;

	CSound *pSnd = 0;
	DWORD dwStatus = 0;
	for(UINT i = 0; i < m_aSounds.size(); ++i)
	{
		pSnd = m_aSounds[i];

		//если есть звук и его канал вопроизводится
		if (pSnd && m_aChannels[pSnd->m_idChannel])
		{
			//если вопроизведение с задержками
			if (pSnd->m_state == SOUND_OBJSTATE_PLAY && pSnd->m_uiStreamSize == 0 && pSnd->m_isPlayDelay)
			{
				UpdatePlayDelay((CSoundBase*)pSnd);
			}

			dwStatus = 0;
			
			//если звук воспроизводится без задержек и буфер уже не прогирывается а состояние звука "проигрывается" тогда выключаем звук
			if (!pSnd->m_isPlayDelay && SUCCEEDED(pSnd->m_pSoundBuffer->GetStatus(&dwStatus)) && !(dwStatus & DSBSTATUS_PLAYING) && pSnd->m_state == SOUND_OBJSTATE_PLAY)
				soundSetState(i, SOUND_OBJSTATE_STOP);

			++tmpSoundsLoadCount;
			if(pSnd->m_state == SOUND_OBJSTATE_PLAY)
				++tmpSoundsPlayCount;

			//если анализируемый звук 3d
			if(pSnd->m_is3d && pSnd->m_pSoundBuffer && viewpos && viewdir)
			{
				//устанавливаем ему параметры воспроизведения
				pSnd->m_pSoundBuffer->SetVolume(lerpf(-10000, 0, pSnd->m_fVolume * Snd3dComVolume(pSnd->m_vPosition, (*viewpos), pSnd->m_fDistAudible)));
				pSnd->m_pSoundBuffer->SetPan(Snd3dComPan(pSnd->m_vPosition, (*viewpos), (*viewdir), pSnd->m_fDistAudible, pSnd->m_fShiftPan));

				//если есть инстансы
				if(pSnd->m_aInstances.size() > 0)
				{
					//проходимся по всему массиву инстансов и устанавливаем параметры воспроизведения
					for(int k = 0, l = pSnd->m_aInstances.size(); k < l; ++k)
					{
						pSnd->m_pSoundBuffer->SetVolume(lerpf(-10000, 0, pSnd->m_fVolume * Snd3dComVolume(pSnd->m_aInstances[k].m_vPos, (*viewpos), pSnd->m_fDistAudible)));
						pSnd->m_pSoundBuffer->SetPan(Snd3dComPan(pSnd->m_aInstances[k].m_vPos, (*viewpos), (*viewdir), pSnd->m_fDistAudible, pSnd->m_fShiftPan));
					}
				}
			}

			for(int k = 0, kl = pSnd->m_aInstances.size(); k < kl; ++k)
			{
				//нужно ли это здесь?
				/*if (pSnd->m_aInstances[k].m_state != SOUND_OBJSTATE_PLAY)
					continue;*/

				//если вопроизведение с задержками
				if (pSnd->m_aInstances[k].m_isPlayDelay)
				{
					if (pSnd->m_aInstances[k].m_state == SOUND_OBJSTATE_PLAY)
						UpdatePlayDelay(&(pSnd->m_aInstances[k]));
				}
				//иначе не установлено что звук воспроизводится с задержками
				else
				{
					//обрабатываем состояния
					dwStatus = 0;

					//если буфер не проигрывается и состояние инстанса "временно блокирован", тогда освобождаем инстанс
					if (SUCCEEDED(pSnd->m_aInstances[k].m_pSoundBuffer->GetStatus(&dwStatus)) && !(dwStatus & DSBSTATUS_PLAYING) && pSnd->m_aInstances[k].m_busy != SOUND_SNDINSTANCE_BUSY_LOCKED)
						pSnd->m_aInstances[k].m_busy = SOUND_SNDINSTANCE_BUSY_FREE;

					//если буфер инстанса не проигрывается а состояние инстанса "проигрывается", тогда устанавливаем состояние "непроигрывается"
					if (pSnd->m_aInstances[k].m_state == SOUND_OBJSTATE_PLAY && !(dwStatus & DSBSTATUS_PLAYING))
						pSnd->m_aInstances[k].m_state = SOUND_OBJSTATE_STOP;
				}
			}

			if(pSnd->m_uiStreamSize && pSnd->m_pSoundBuffer != 0)
			{
				DWORD pos;
				pSnd->m_pSoundBuffer->GetCurrentPosition(&pos, 0);

				if (pos >= pSnd->m_uiSplit1Size && pos < pSnd->m_uiSplit2Size && !pSnd->m_isWork2)
				{
					reLoadSplit(i, 0, pSnd->m_uiSplit1Size);
					pSnd->m_isWork2 = true;
					pSnd->m_isWork1 = false; pSnd->m_isWork3 = false; pSnd->m_isWork4 = false;
					pSnd->m_iSplitActive = 2;
				}
				else if (pos >= pSnd->m_uiSplit2Size && pos < pSnd->m_uiSplit3Size && !pSnd->m_isWork3)
				{
					pSnd->m_isWork3 = true;
					pSnd->m_isWork1 = false; pSnd->m_isWork2 = false; pSnd->m_isWork4 = false;
					reLoadSplit(i, pSnd->m_uiSplit1Size, pSnd->m_uiSplit1Size);
					pSnd->m_iSplitActive = 3;
				}
				else if (pos >= pSnd->m_uiSplit3Size && pos < pSnd->m_uiStreamSize && !pSnd->m_isWork4)
				{
					pSnd->m_isWork4 = true;
					pSnd->m_isWork1 = false; pSnd->m_isWork2 = false; pSnd->m_isWork3 = false;
					reLoadSplit(i, pSnd->m_uiSplit2Size, pSnd->m_uiSplit1Size);
					pSnd->m_iSplitActive = 4;
				}
				else if (pos < pSnd->m_uiSplit1Size && !pSnd->m_isWork1)
				{
					pSnd->m_isWork1 = true;
					pSnd->m_isWork2 = false; pSnd->m_isWork3 = false; pSnd->m_isWork4 = false;
					if(!pSnd->m_isStarting)
					{
						reLoadSplit(i, pSnd->m_uiSplit3Size, pSnd->m_uiSplit1Size);
						pSnd->m_iRePlayCount++;
					}
					pSnd->m_iSplitActive = 1;
					pSnd->m_isStarting = false;
				}


				if(pSnd->m_iRePlayCount + 1 == pSnd->m_iRePlayEndCount)
				{
					if(pSnd->m_uiSizeFull <= (pSnd->m_uiStreamSize * pSnd->m_iRePlayCount + pos))
					{
						if(!pSnd->m_isLooping)
							soundStop(i);

						//wav
						if(pSnd->m_format == SOUND_FILEFORMAT_WAV)
							fseek(pSnd->m_pStream, sizeof(CSoundWaveHeader), SEEK_SET);
						//ogg
						else if(pSnd->m_format == SOUND_FILEFORMAT_OGG)
							ov_pcm_seek(pSnd->m_pVorbisFile, 0);

						reLoadSplit(i, 0, pSnd->m_uiStreamSize);

						pSnd->m_pSoundBuffer->SetCurrentPosition(0);

						pSnd->m_iRePlayCount = 0;
						pSnd->m_isStarting = true;

						pSnd->m_isWork1 = false; pSnd->m_isWork2 = false; pSnd->m_isWork3 = false; pSnd->m_isWork4 = false;
					}
				}
			}
		}
	}

	m_iSoundsPlayCount = tmpSoundsPlayCount;
	m_iSoundsLoadCount = tmpSoundsLoadCount;

	m_vOldViewPos = *viewpos;
	m_vOldViewDir = *viewdir;
}

int CSoundManager::soundsGetPlayCount()
{
	return m_iSoundsPlayCount;
}

int CSoundManager::soundsGetLoadCount()
{
	return m_iSoundsLoadCount;
}
