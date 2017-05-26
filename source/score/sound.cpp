
#include <score/sound.h>

MainSound::MainSound()
{
	DeviceSound = 0;
	DSPrimary = 0;
	StdPath[0] = 0;
}

MainSound::~MainSound()
{
	Clear();
	mem_release_del(DSPrimary);
	mem_release_del(DeviceSound);
}

void MainSound::Clear()
{
	for (int i = 0; i < ArrSounds.size(); ++i)
	{
		mem_delete(ArrSounds[i]);
	}
}

MainSound::Sound::Sound()
{
	ZeroMemory(this, sizeof(MainSound::Sound));
}

MainSound::Sound::~Sound()
{
	if (StreamFile)
		fclose(StreamFile);

	mem_release_del(DSBuffer);
	if (VorbisFile)
	{
		ov_clear(VorbisFile);
		mem_delete(VorbisFile);
	}
}

void MainSound::Init(HWND hwnd)
{
	if (FAILED(DirectSoundCreate8(NULL, &DeviceSound, NULL)))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - could not create sound device", gen_msg_location);
		return;// SOUND_INIT_ERR_INIT;
	}

	if (FAILED(DeviceSound->SetCooperativeLevel(hwnd, DSSCL_EXCLUSIVE)))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - could not create cooperative level", gen_msg_location);
		return;// SOUND_INIT_ERR_CL;
	}

	DSBUFFERDESC       dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));

	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = NULL;

	if (FAILED(DeviceSound->CreateSoundBuffer(&dsbd, &DSPrimary, NULL)))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - could not create primary buffer", gen_msg_location);
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

	if (FAILED(DSPrimary->SetFormat(&wfex)))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - could not init format", gen_msg_location);
		return;// SOUND_INIT_ERR_SET_FORMAT;
	}

	DSPrimary->SetVolume(0);
	DSPrimary->Play(0, 0, DSBPLAY_LOOPING);
}

void MainSound::StdPathSet(const char* path)
{
	if (path)
		strcpy(StdPath, path);
}

void MainSound::StdPathGet(char* path)
{
	if (path)
		strcpy(path, StdPath);
}

//

SoundFileFormat MainSound::FileFormat(const char* file)
{
	FILE *fp;

	if (!(fp = fopen(file, "rb")))
		return SoundFileFormat::sff_unknown;

	SoundWaveHeader Hdr;
	fseek(fp, 0, SEEK_SET);
	fread(&Hdr, 1, sizeof(SoundWaveHeader), fp);

	if (memcmp(Hdr.RiffSig, "RIFF", 4) == 0 || memcmp(Hdr.Sig, "WAVE", 4) == 0 || memcmp(Hdr.FormatSig, "fmt ", 4) == 0)
	{
		fclose(fp);
		return SoundFileFormat::sff_wav;
	}

	if (strcmp(fp->_base, "OggS") == 0)
	{
		fclose(fp);
		return SoundFileFormat::sff_ogg;
	}

	fclose(fp);

	return SoundFileFormat::sff_unknown;
}

//#############################################################################

void MainSound::Load(Sound* snd, const char* fpath, SoundFileFormat fmt)
{
	if (fmt == SoundFileFormat::sff_ogg)
		LoadOGG(snd, fpath);
	else if (fmt == SoundFileFormat::sff_wav)
		LoadWAV(snd, fpath);
}

void MainSound::LoadWAV(Sound* snd, const char* fpath)
{
	SoundWaveHeader Hdr;
	snd->StreamFile = fopen(fpath, "rb");

	fseek(snd->StreamFile, 0, SEEK_SET);
	fread(&Hdr, 1, sizeof(SoundWaveHeader), snd->StreamFile);

	snd->SizeFull = (Hdr.ChunkSize + (sizeof(char)* 4 + sizeof(int32_t))) - sizeof(SoundWaveHeader);

	if (snd->StreamSize == 0)
		Hdr.DataSize = snd->SizeFull;
	else
		Hdr.DataSize = snd->StreamSize;
	
	if (!(snd->DSBuffer = SoundBufferCreate(&Hdr)))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - could not create sound buffer [%s]", gen_msg_location, snd->RPath);
		return;
	}

	fseek(snd->StreamFile, sizeof(SoundWaveHeader), SEEK_SET);

	SoundDataWAVLoad(snd->DSBuffer, 0, snd->StreamFile, Hdr.DataSize, 0);

	snd->LengthSec = snd->StreamSize / (Hdr.BytesPerSec);
	snd->ChannelsCount = Hdr.Channels;
	snd->RateSample = Hdr.SampleRate;
	snd->BitsPerSample = Hdr.BitsPerSample;
	snd->BytesPerSec = ((snd->BitsPerSample / 8) * snd->ChannelsCount) * snd->RateSample;

	if (snd->StreamSize == 0)
	{
		fclose(snd->StreamFile);
	}
	else
	{
		snd->Split1Size = snd->StreamSize / 4;
		snd->Split2Size = snd->Split1Size * 2;
		snd->Split3Size = snd->Split1Size * 3;

		snd->BF1 = false;
		snd->BF2 = false;
		snd->BF3 = false;
		snd->BF4 = false;

		snd->IsStarting = true;

		snd->RePlayCount = 0;
		double tmpCRPE = double(snd->SizeFull) / double(snd->StreamSize);
		float Count = 0;

		while (1)
		{
			if (tmpCRPE > Count && tmpCRPE < Count + 1)
			{
				snd->RePlayEndCount = Count + 1;
				break;
			}
			else if (tmpCRPE == Count)
			{
				snd->RePlayEndCount = Count;
				break;
			}
			Count++;
		}
	}
}

IDirectSoundBuffer8* MainSound::SoundBufferCreate(SoundWaveHeader* hdr)
{
	IDirectSoundBuffer  *DSB;
	IDirectSoundBuffer8 *DSBuffer;
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
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLFX;
	dsbd.dwBufferBytes = hdr->DataSize;
	dsbd.lpwfxFormat = &wfex;
	if (FAILED(DeviceSound->CreateSoundBuffer(&dsbd, &DSB, NULL)))
		return NULL;

	if (FAILED(DSB->QueryInterface(IID_IDirectSoundBuffer8, (void**)&DSBuffer)))
	{
		DSB->Release();
		return NULL;
	}

	return DSBuffer;
}

void MainSound::SoundDataWAVLoad(IDirectSoundBuffer8* DSBuffer, long LockPos, FILE* data, long Size, DWORD flag)
{
	BYTE  *Ptr1, *Ptr2;
	DWORD Size1, Size2;

	if (!Size)
		return;

	if (FAILED(DSBuffer->Lock(LockPos, Size, (void**)&Ptr1, &Size1, (void**)&Ptr2, &Size2, flag)))
		return;

	fread(Ptr1, 1, Size1, data);
	if (Ptr2)
		fread(Ptr2, 1, Size2, data);

	DSBuffer->Unlock(Ptr1, Size1, Ptr2, Size2);
}

/////////////////////////////////////////////////////////////////////////////

void MainSound::LoadOGG(Sound* snd, const char* fpath)
{
	OggVorbis_File ogg;

	snd->StreamFile = fopen(fpath, "rb");

	if (snd->StreamFile == NULL)
		return;

	if (ov_fopen(fpath, &ogg))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - error reading [%s]", gen_msg_location, snd->RPath);
		return;
	}

	snd->VorbisFile = new OggVorbis_File;

	ov_callbacks cb;
	cb.close_func = close_ogg;
	cb.read_func = read_ogg;
	cb.seek_func = seek_ogg;
	cb.tell_func = tell_ogg;

	ov_open_callbacks(snd->StreamFile, snd->VorbisFile, 0, 0, cb);

	vorbis_info *vi = ov_info(&ogg, -1);

	if (!vi)
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - error reading (info) [%s]", gen_msg_location, snd->RPath);
		return;
	}

	snd->LengthSec = float(ov_pcm_total(snd->VorbisFile, -1)) / float(vi->rate);
	snd->ChannelsCount = vi->channels;
	snd->RateSample = vi->rate;
	snd->BitsPerSample = SOUND_OGG_BITS_PER_SAMPLE;
	snd->BytesPerSec = ((snd->BitsPerSample / 8) * snd->ChannelsCount) * snd->RateSample;
	snd->SizeFull = ov_pcm_total(snd->VorbisFile, -1) * 2 * vi->channels;

	SoundWaveHeader hdr;
	hdr.Channels = vi->channels;
	hdr.SampleRate = vi->rate;
	hdr.BitsPerSample = SOUND_OGG_BITS_PER_SAMPLE;

	if (snd->StreamSize == 0)
		hdr.DataSize = snd->SizeFull;
	else
		hdr.DataSize = snd->StreamSize;

	snd->DSBuffer = SoundBufferCreate(&hdr);

	if (!snd->DSBuffer)
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - could not create sound buffer [%s]", gen_msg_location, snd->RPath);
		return;
	}

	SoundDataOGGLoad(snd->VorbisFile, snd->DSBuffer, 0, hdr.DataSize, 0);

	if (snd->StreamSize == 0)
	{
		fclose(snd->StreamFile);
		ov_clear(&ogg);
		ov_clear(snd->VorbisFile);
		mem_delete(snd->VorbisFile);
		snd->VorbisFile = 0;
	}
	else
	{
		snd->Split1Size = snd->StreamSize / 4;
		snd->Split2Size = snd->Split1Size * 2;
		snd->Split3Size = snd->Split1Size * 3;

		snd->BF1 = false;
		snd->BF2 = false;
		snd->BF3 = false;
		snd->BF4 = false;

		snd->IsStarting = true;

		snd->RePlayCount = 0;
		double tmpCRPE = double(snd->SizeFull) / double(snd->StreamSize);
		float Count = 0;

		while (1)
		{
			if (tmpCRPE > Count && tmpCRPE < Count + 1)
			{
				snd->RePlayEndCount = Count + 1;
				break;
			}
			else if (tmpCRPE == Count)
			{
				snd->RePlayEndCount = Count;
				break;
			}
			Count++;
		}
	}
	//return SOUND_OK;
}

void MainSound::SoundDataOGGLoad(OggVorbis_File* VorbisFile, IDirectSoundBuffer8 *DSBuffer, long LockPos, long Size, DWORD flag)
{
	char  *Ptr1, *Ptr2;
	DWORD Size1, Size2;

	if (!Size)
		return;

	if (FAILED(DSBuffer->Lock(LockPos, Size, (void**)&Ptr1, &Size1, (void**)&Ptr2, &Size2, flag)))
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
			reportf(REPORT_MSG_LEVEL_WARRNING,"OV_EINVAL");
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
				reportf(REPORT_MSG_LEVEL_WARRNING, "OV_EINVAL");
			}
			else
				break;
		}
	}

	DSBuffer->Unlock(Ptr1, Size1, Ptr2, Size2);
}

//#############################################################################

ID MainSound::SoundCreate2d(const char *file, bool looping, DWORD size_stream)
{
	if (!file)
		return -1;

	char fullpath[SOUND_MAX_SIZE_STDPATH + SOUND_MAX_SIZE_PATH];
	sprintf(fullpath, "%s%s", StdPath, file);

	if (!Core_0FileExists(fullpath))
		return -1;

	SoundFileFormat fmt = FileFormat(fullpath);

	if (fmt == SoundFileFormat::sff_unknown)
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - unknown format [%s]", gen_msg_location, file);
		return -1;
	}

	Sound* snd = new Sound();
	strcpy(snd->RPath, file);
	snd->Format = fmt;
	snd->IsLooping = looping;

	if (size_stream > 0)
	{
		if (size_stream < SOUND_MIN_SIZE_STREAM)
			snd->StreamSize = SOUND_MIN_SIZE_STREAM;
		else
			snd->StreamSize = size_stream;
	}

	Load(snd, fullpath, fmt);

	snd->DSBuffer->GetFrequency(&snd->FrecOrigin);

	return AddSound(snd);
}

ID MainSound::SoundCreate3d(const char *file, bool looping, DWORD size_stream, float dist, float shift_pan)
{
	ID sndid = SoundCreate2d(file, looping, size_stream);
	Sound* snd = 0;

	if (sndid < 0)
		return sndid;

	snd = ArrSounds[sndid];
	snd->DistAudible = dist;
	snd->Is3d = true;
	//snd->Damping = damping;
	snd->ShiftPan = shift_pan;
	return sndid;
}

ID MainSound::AddSound(Sound* snd)
{
	for (int i = 0, l = ArrSounds.size(); i < l; ++i)
	{
		if (ArrSounds[i] == 0)
		{
			ArrSounds[i] = snd;
			return i;
		}
	}

	ArrSounds.push_back(snd);
	return ArrSounds.size() - 1;
}

bool MainSound::SoundIsInit(ID id)
{
	return (ArrSounds.size() > id && ArrSounds[id] != 0);
}

void MainSound::SoundDelete(ID id)
{
	SOUND_PRECOND(id, _VOID);

	mem_delete(ArrSounds[id]);

}

//#############################################################################

void MainSound::SoundPlay(ID id, int looping)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];

	if (looping >= 0)
		snd->IsLooping = looping;

	if (snd->DSBuffer)
	{
		snd->DSBuffer->Play(0, 0, (snd->StreamSize || snd->IsLooping ? DSBPLAY_LOOPING : 0));
		snd->State = SoundObjState::sos_play;
	}
}

void MainSound::SoundPause(ID id)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];

	if (snd->DSBuffer)
	{
		snd->DSBuffer->Stop();
		snd->State = SoundObjState::sos_pause;
	}
}

void MainSound::SoundStop(ID id)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];

	if (snd->DSBuffer)
	{
		snd->DSBuffer->Stop();
		snd->DSBuffer->SetCurrentPosition(0);
		snd->State = SoundObjState::sos_stop;
	}
}

void MainSound::SoundStateSet(ID id, SoundObjState state)
{
	SOUND_PRECOND(id, _VOID);

	if (state == SoundObjState::sos_play)
		SoundPlay(id);
	else if (state == SoundObjState::sos_pause)
		SoundPause(id);
	else if (state == SoundObjState::sos_stop)
		SoundStop(id);
}

SoundObjState MainSound::SoundStateGet(ID id)
{
	SOUND_PRECOND(id, SoundObjState::sos_stop);

	Sound* snd = ArrSounds[id];
	return snd->State;
}

void MainSound::SoundPosCurrSet(ID id, DWORD pos, int type)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];

	DWORD PosInBytes = 0;
	if (snd->DSBuffer)
	{
		if (type == SOUND_POS_BYTES)
		{
			PosInBytes = pos;
		}
		else if (type == SOUND_POS_SEC)
		{
			PosInBytes = pos * snd->BytesPerSec;
		}
		else if (type == SOUND_POS_MLS)
		{
			PosInBytes = pos * (snd->BytesPerSec / 1000);
		}

		if (snd->StreamSize)
		{
			WORD HowCountRePlay = 0;
			for (WORD i = 0; i <= snd->RePlayEndCount; i++)
			{
				if ((i * snd->StreamSize <= PosInBytes && (i + 1) * snd->StreamSize >= PosInBytes) || i * snd->StreamSize == PosInBytes)
				{
					HowCountRePlay = i;
					break;
				}
			}

			//wav
			if (snd->Format == SoundFileFormat::sff_wav)
				fseek(snd->StreamFile, sizeof(SoundWaveHeader)+(HowCountRePlay  * snd->StreamSize), SEEK_SET);
			//ogg
			else if (snd->Format == SoundFileFormat::sff_ogg)
				ov_pcm_seek(snd->VorbisFile, (HowCountRePlay  * snd->StreamSize) / (2 * snd->ChannelsCount));

			DWORD SizeCountRePlay = PosInBytes - (snd->StreamSize * HowCountRePlay);

			ReLoadSplit(id, 0, snd->StreamSize);

			snd->RePlayCount = HowCountRePlay;
			snd->DSBuffer->SetCurrentPosition(SizeCountRePlay);

			for (WORD i = 0; i<4; i++)
			{
				if (SizeCountRePlay >= snd->Split1Size * i && SizeCountRePlay < snd->Split1Size * (i + 1))
				{
					snd->SplitActive = i + 1;
					break;
				}
			}

			if (snd->SplitActive > 1)
				snd->IsStarting = false;
			else
				snd->IsStarting = true;

			if (snd->SplitActive - 2 > 0 && snd->SplitActive - 2 < 5)
			{
				for (WORD i = 0; i<snd->SplitActive - 2; i++)
					ReLoadSplit(id, snd->Split1Size*i, snd->Split1Size);
			}

			snd->BF1 = snd->BF2 = snd->BF3 = snd->BF4 = false;
		}
		else
			snd->DSBuffer->SetCurrentPosition(PosInBytes);
	}
}

DWORD MainSound::SoundPosCurrGet(ID id, int type)
{
	SOUND_PRECOND(id, 0);

	Sound* snd = ArrSounds[id];
	DWORD posinduff, Bytes, Pos = 0;
	if (snd->DSBuffer)
	{
		snd->DSBuffer->GetCurrentPosition(&posinduff, 0);

		if (snd->StreamSize)
			Bytes = snd->RePlayCount * snd->StreamSize + posinduff;
		else
			Bytes = posinduff;

		if (type == SOUND_POS_BYTES)
			Pos = Bytes;
		else if (type == SOUND_POS_SEC)
			Pos = Bytes / snd->BytesPerSec;
		else if (type == SOUND_POS_MLS)
			Pos = Bytes / (snd->BytesPerSec / 1000);
	}
	return Pos;
}

void MainSound::SoundVolumeSet(ID id, long volume, int type)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];
	if (snd->DSBuffer)
	{
		if (type == SOUND_VOL_DB)
			snd->Volume = volume;
		else if (type == SOUND_VOL_PCT)
			snd->Volume = -10000 + (volume * 100);

		if (!snd->Is3d)
			snd->DSBuffer->SetVolume(snd->Volume);
	}
}

long MainSound::SoundVolumeGet(ID id, int type)
{
	SOUND_PRECOND(id, 0);

	Sound* snd = ArrSounds[id];
	static long volume = 0;
	if (snd->DSBuffer)
	{
		if (type == SOUND_VOL_DB)
		{
			if (snd->Is3d)
				volume = snd->Volume;
			else
				snd->DSBuffer->GetVolume(&volume);
		}
		else if (type == SOUND_VOL_PCT)
		{
			if (snd->Is3d)
			{
				double one_percent = 100.0 / (-10000.0);
				volume = double(snd->Volume) * one_percent;
			}
			else
			{
				double one_percent = 100.0 / (-10000.0);
				snd->DSBuffer->GetVolume(&volume);
				volume = double(volume) * one_percent;
			}
		}
	}
	return volume;
}


void MainSound::SoundPanSet(ID id, long value, int type)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];
	static long Value = 0;
	if (snd->DSBuffer)
	{
		if (type == SOUND_VOL_DB || value == 0)
			Value = value;
		else if (type == SOUND_VOL_PCT && value != 0)
			Value = (value > 0) ? (10000 - (value * 100)) - 10000 : 10000 - (10000 + (value * 100));
	}
}

long MainSound::SoundPanGet(ID id, int type)
{
	SOUND_PRECOND(id, 0);

	Sound* snd = ArrSounds[id];
	static long Value = 0;
	if (snd->DSBuffer)
	{
		if (type == SOUND_VOL_DB)
			snd->DSBuffer->GetPan(&Value);
		else
		{
			double coef_percent = 100.0 / (10000.0);
			snd->DSBuffer->GetPan(&Value);
			Value = double(Value) * coef_percent;
		}
	}
	return Value;
}


void MainSound::SoundFreqCurrSet(ID id, DWORD value)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];
	if (snd->DSBuffer)
		snd->DSBuffer->SetFrequency(value);
}

DWORD MainSound::SoundFreqCurrGet(ID id)
{
	SOUND_PRECOND(id, 0);

	Sound* snd = ArrSounds[id];
	static DWORD Value = 0;
	if (snd->DSBuffer)
		snd->DSBuffer->GetFrequency(&Value);
	return Value;
}

DWORD MainSound::SoundFreqOriginGet(ID id)
{
	SOUND_PRECOND(id, 0);

	Sound* snd = ArrSounds[id];
	if (snd->DSBuffer)
		return snd->FrecOrigin;

	return 0;
}

void MainSound::SoundPosWSet(ID id, float3* pos)
{
	SOUND_PRECOND(id, _VOID);
	if (pos)
		ArrSounds[id]->Position = *pos;
}

void MainSound::SoundPosWGet(ID id, float3* pos)
{
	SOUND_PRECOND(id, _VOID);
	if (pos)
		*pos = ArrSounds[id]->Position;
}

int MainSound::SoundLengthSecGet(ID id)
{
	SOUND_PRECOND(id, 0);

	return ArrSounds[id]->LengthSec;
}

DWORD MainSound::SoundBytesPerSecGet(ID id)
{
	SOUND_PRECOND(id, 0);

	return ArrSounds[id]->BytesPerSec;
}

DWORD MainSound::SoundSizeGet(ID id)
{
	SOUND_PRECOND(id, 0);

	return ArrSounds[id]->SizeFull;
}

void MainSound::SoundFileGet(ID id, char* path)
{
	SOUND_PRECOND(id, _VOID);

	if(path)
		strcpy(path,ArrSounds[id]->RPath);
}


float MainSound::SoundDistAudibleGet(ID id)
{
	SOUND_PRECOND(id, 0);

	return ArrSounds[id]->DistAudible;
}

void MainSound::SoundDistAudibleSet(ID id, float value)
{
	SOUND_PRECOND(id, _VOID);

	ArrSounds[id]->DistAudible = value;
}

//#############################################################################

void MainSound::ReLoadSplit(ID id, DWORD Pos, DWORD Size)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];
	if (snd->DSBuffer)
	{
		//wav
		if (snd->Format == SoundFileFormat::sff_wav)
			SoundDataWAVLoad(snd->DSBuffer, Pos, snd->StreamFile, Size, 0);
		//ogg
		else if (snd->Format == SoundFileFormat::sff_ogg)
		{
			SoundDataOGGLoad(snd->VorbisFile, snd->DSBuffer, Pos, Size, 0);
		}
	}
}

void MainSound::Update(float3* viewpos, float3* viewdir)
{
	int tmpSoundsPlayCount = 0;
	int tmpSoundsLoadCount = 0;

	Sound* snd;
	for (int i = 0; i < ArrSounds.size(); ++i)
	{
		snd = ArrSounds[i];
		if (snd)
		{
			++tmpSoundsLoadCount;
			if (snd->State == SoundObjState::sos_play)
				++tmpSoundsPlayCount;
			if (snd->Is3d && snd->DSBuffer && viewpos && viewdir)
			{
				float Dist = SMVector3Distance(snd->Position, *viewpos);
				float tmp_volume;// = 0 + (Dist/MaxDist)*(-10000 - 0);/*-((float(Dist * Damping)*100.0) + 10000.0 * ((1.0/10000.0) * float((10000.0+VolumeSound))))*/;
				tmp_volume = (Dist) / snd->DistAudible;
				tmp_volume = (tmp_volume)*(-10000.f);

				if (tmp_volume > 0)
					tmp_volume = 0;

				if (tmp_volume < -10000)
					tmp_volume = -10000;

				snd->DSBuffer->SetVolume(tmp_volume);

				float3 vec = (*viewpos) + (*viewdir);

				float str = (snd->Position.x - viewpos->x)*(snd->Position.z - vec.z) - (snd->Position.z - viewpos->z)*(snd->Position.x - vec.x);
				snd->DSBuffer->SetPan((str * (Dist / snd->DistAudible)) * snd->ShiftPan * (-10000));
			}

			if (snd->StreamSize && snd->DSBuffer != 0)
			{
				DWORD pos;
				snd->DSBuffer->GetCurrentPosition(&pos, 0);

				if (pos >= snd->Split1Size && pos < snd->Split2Size && !snd->BF2)
				{
					ReLoadSplit(i, 0, snd->Split1Size);
					snd->BF2 = true;
					snd->BF1 = false; snd->BF3 = false; snd->BF4 = false;
					snd->SplitActive = 2;
				}
				else if (pos >= snd->Split2Size && pos < snd->Split3Size && !snd->BF3)
				{
					snd->BF3 = true;
					snd->BF1 = false; snd->BF2 = false; snd->BF4 = false;
					ReLoadSplit(i, snd->Split1Size, snd->Split1Size);
					snd->SplitActive = 3;
				}
				else if (pos >= snd->Split3Size && pos < snd->StreamSize && !snd->BF4)
				{
					snd->BF4 = true;
					snd->BF1 = false; snd->BF2 = false; snd->BF3 = false;
					ReLoadSplit(i, snd->Split2Size, snd->Split1Size);
					snd->SplitActive = 4;
				}
				else if (pos < snd->Split1Size && !snd->BF1)
				{
					snd->BF1 = true;
					snd->BF2 = false; snd->BF3 = false; snd->BF4 = false;
					if (!snd->IsStarting)
					{
						ReLoadSplit(i, snd->Split3Size, snd->Split1Size);
						snd->RePlayCount++;
					}
					snd->SplitActive = 1;
					snd->IsStarting = false;
				}


				if (snd->RePlayCount + 1 == snd->RePlayEndCount)
				{
					if (snd->SizeFull <= (snd->StreamSize * snd->RePlayCount + pos))
					{
						if (!snd->IsLooping)
							SoundStop(i);

						//wav
						if (snd->Format == SoundFileFormat::sff_wav)
							fseek(snd->StreamFile, sizeof(SoundWaveHeader), SEEK_SET);
						//ogg
						else if (snd->Format == SoundFileFormat::sff_ogg)
							ov_pcm_seek(snd->VorbisFile, 0);

						ReLoadSplit(i, 0, snd->StreamSize);

						snd->DSBuffer->SetCurrentPosition(0);

						snd->RePlayCount = 0;
						snd->IsStarting = true;

						snd->BF1 = false; snd->BF2 = false; snd->BF3 = false; snd->BF4 = false;
					}
				}
			}
		}
	}

	SoundsPlayCount = tmpSoundsPlayCount;
	SoundsLoadCount = tmpSoundsLoadCount;
}

int MainSound::SoundsPlayCountGet()
{
	return SoundsPlayCount;
}

int MainSound::SoundsLoadCountGet()
{
	return SoundsLoadCount;
}

//#############################################################################

void MainSound::EffectInitRe(ID id)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];

	if (snd->EffectInit[SOUND_EFFECT_GARGLE])
	{
		//snd->EffectInit[SOUND_EFF_GARGLE] = false;
		SoundEffectGargleSet(id, snd->EffGargle.dwRateHz, snd->EffGargle.dwWaveShape);
	}

	if (snd->EffectInit[SOUND_EFFECT_CHORUS])
	{
		//snd->EffectInit[SOUND_EFF_CHORUS] = false;
		SoundEffectChorusSet(id, snd->EffChorus.fWetDryMix, snd->EffChorus.fDepth, snd->EffChorus.fFeedback, snd->EffChorus.fFrequency, snd->EffChorus.lWaveform, snd->EffChorus.fDelay, snd->EffChorus.lPhase);
	}

	if (snd->EffectInit[SOUND_EFFECT_FLANDER])
	{
		//snd->EffectInit[SOUND_EFF_FLANDER] = false;
		SoundEffectFlangerSet(id, snd->EffFlanger.fWetDryMix, snd->EffFlanger.fDepth, snd->EffFlanger.fFeedback, snd->EffFlanger.fFrequency, snd->EffFlanger.lWaveform, snd->EffFlanger.fDelay, snd->EffFlanger.lPhase);
	}

	if (snd->EffectInit[SOUND_EFFECT_ECHO])
	{
		//snd->EffectInit[SOUND_EFF_ECHO] = false;
		SoundEffectEchoSet(id, snd->EffEcho.fWetDryMix, snd->EffEcho.fFeedback, snd->EffEcho.fLeftDelay, snd->EffEcho.fRightDelay, snd->EffEcho.lPanDelay);
	}

	if (snd->EffectInit[SOUND_EFFECT_DISTORTION])
	{
		//snd->EffectInit[SOUND_EFF_DISTORTION] = false;
		SoundEffectDistortionSet(id, snd->EffDistortion.fGain, snd->EffDistortion.fEdge, snd->EffDistortion.fPostEQCenterFrequency, snd->EffDistortion.fPostEQBandwidth, snd->EffDistortion.fPreLowpassCutoff);
	}

	if (snd->EffectInit[SOUND_EFFECT_COMPRESSOR])
	{
		//snd->EffectInit[SOUND_EFF_COMPRESSOR] = false;
		SoundEffectCompressorSet(id, snd->EffCompressor.fGain, snd->EffCompressor.fAttack, snd->EffCompressor.fRelease, snd->EffCompressor.fThreshold, snd->EffCompressor.fRatio, snd->EffCompressor.fPredelay);
	}

	if (snd->EffectInit[SOUND_EFFECT_PARAMEQ])
	{
		//snd->EffectInit[SOUND_EFF_PARAMEQ] = false;
		SoundEffectParameqSet(id, snd->EffParamEq.fCenter, snd->EffParamEq.fBandwidth, snd->EffParamEq.fGain);
	}

	if (snd->EffectInit[SOUND_EFFECT_I3DL2REVERB])
	{
		//snd->EffectInit[SOUND_EFF_I3DL2REVERB] = false;
		SoundEffectI3DL2ReverbSet(id, snd->EffI3DL2Reverb.lRoom, snd->EffI3DL2Reverb.lRoomHF, snd->EffI3DL2Reverb.flRoomRolloffFactor, snd->EffI3DL2Reverb.flDecayTime, snd->EffI3DL2Reverb.flDecayHFRatio,
			snd->EffI3DL2Reverb.lReflections, snd->EffI3DL2Reverb.flReflectionsDelay, snd->EffI3DL2Reverb.lReverb, snd->EffI3DL2Reverb.flReverbDelay,
			snd->EffI3DL2Reverb.flDiffusion, snd->EffI3DL2Reverb.flDensity, snd->EffI3DL2Reverb.flHFReference);
	}

	if (snd->EffectInit[SOUND_EFFECT_WAVESREVERB])
	{
		//snd->EffectInit[SOUND_EFF_WAVESREVERB] = false;
		SoundEffectWavesReverbSet(id, snd->EffWavesReverb.fInGain, snd->EffWavesReverb.fReverbMix, snd->EffWavesReverb.fReverbTime, snd->EffWavesReverb.fHighFreqRTRatio);
	}
}

bool MainSound::EffectInitPrecond(HRESULT hr)
{
	if (FAILED(hr))
	{
		if (hr == DSERR_CONTROLUNAVAIL)
			reportf(REPORT_MSG_LEVEL_WARRNING, "SOUND_ERR_CONTROLUNAVAIL");
		else if (hr == DSERR_GENERIC)
			reportf(REPORT_MSG_LEVEL_WARRNING, "SOUND_ERR_GENERIC");
		else if (hr == DSERR_INVALIDCALL)
			reportf(REPORT_MSG_LEVEL_WARRNING, "SOUND_ERR_INVALIDCALL");
		else if (hr == DSERR_INVALIDPARAM)
			reportf(REPORT_MSG_LEVEL_WARRNING, "SOUND_ERR_INVALIDPARAM");
		else if (hr == DSERR_PRIOLEVELNEEDED)
			reportf(REPORT_MSG_LEVEL_WARRNING, "SOUND_ERR_PRIOLEVELNEEDED");
		else if (hr == DSERR_NOINTERFACE)
			reportf(REPORT_MSG_LEVEL_WARRNING, "SOUND_ERR_NOINTERFACE");
		else
			reportf(REPORT_MSG_LEVEL_WARRNING, "SOUND_ERR_UNCPECIFIED");

		return false;
	}
	return true;
}



int MainSound::SoundEffectStateGet(ID id, int effect)
{
	SOUND_PRECOND(id, -1);

	Sound* snd = ArrSounds[id];
	if (effect >= 0 && effect <= 8)
		return (snd->EffectInit[effect]);
	else
		return SOUND_EFF_INVALID_KEY;
}

GUID MainSound::EffectGuidGet(int effect)
{
	if (effect == SOUND_EFFECT_GARGLE)
		return GUID_DSFX_STANDARD_GARGLE;
	else if (effect == SOUND_EFFECT_CHORUS)
		return GUID_DSFX_STANDARD_CHORUS;
	else if (effect == SOUND_EFFECT_FLANDER)
		return GUID_DSFX_STANDARD_FLANGER;
	else if (effect == SOUND_EFFECT_ECHO)
		return GUID_DSFX_STANDARD_ECHO;
	else if (effect == SOUND_EFFECT_DISTORTION)
		return GUID_DSFX_STANDARD_DISTORTION;
	else if (effect == SOUND_EFFECT_COMPRESSOR)
		return GUID_DSFX_STANDARD_COMPRESSOR;
	else if (effect == SOUND_EFFECT_PARAMEQ)
		return GUID_DSFX_STANDARD_PARAMEQ;
	else if (effect == SOUND_EFFECT_I3DL2REVERB)
		return GUID_DSFX_STANDARD_I3DL2REVERB;
	else if (effect == SOUND_EFFECT_WAVESREVERB)
		return GUID_DSFX_WAVES_REVERB;
}

void MainSound::SoundEffectStateSet(ID id, int effect, int state)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];

	bool IsPlaying = snd->State == SoundObjState::sos_play;
	
	if (IsPlaying)
		SoundStop(id);

	//включаем эффект
	if (state == SOUND_EFF_ON)
	{
		DWORD arr;
		DSEFFECTDESC dsEffect;

		memset(&dsEffect, 0, sizeof(DSEFFECTDESC));
		dsEffect.dwSize = sizeof(DSEFFECTDESC);
		dsEffect.dwFlags = 0;
		dsEffect.guidDSFXClass = EffectGuidGet(effect);

		if(EffectInitPrecond(snd->DSBuffer->SetFX(1, &dsEffect, &arr)))
			snd->EffectInit[effect] = true;
	}
	//выключаем эффект
	else if (state == SOUND_EFF_OFF)
	{
		if (EffectInitPrecond(snd->DSBuffer->SetFX(0, 0, 0)))
		{
			snd->EffectInit[effect] = false;
			EffectInitRe(id);
		}
	}

	if (IsPlaying)
		SoundPlay(id);
}

//#############################################################################

bool MainSound::SoundEffectGargleSet(ID id, DWORD RateHz, DWORD WaveShape)
{
	SOUND_PRECOND(id, false);

	Sound* snd = ArrSounds[id];

	IDirectSoundFXGargle8 *gargle = 0;
	
	if (!snd->EffectInit[SOUND_EFFECT_GARGLE])
		SoundEffectStateSet(id, SOUND_EFFECT_GARGLE, SOUND_EFF_ON);

	snd->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_GARGLE, 0, IID_IDirectSoundFXGargle8, (void**)&gargle);

	snd->EffGargle.dwRateHz = RateHz;
	snd->EffGargle.dwWaveShape = WaveShape;

	HRESULT hr = gargle->SetAllParameters(&snd->EffGargle);
	mem_release_del(gargle);
	/*if (FAILED(hr))
		info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;*/

	return true;
}

bool MainSound::SoundEffectChorusSet(ID id, float WetDryMix, float Depth, float Feedback, float Frequency, long Waveform, float Delay, long Phase)
{
	SOUND_PRECOND(id, false);

	Sound* snd = ArrSounds[id];

	IDirectSoundFXChorus8 *chorus = 0;

	if (!snd->EffectInit[SOUND_EFFECT_CHORUS])
		SoundEffectStateSet(id, SOUND_EFFECT_CHORUS, SOUND_EFF_ON);

	snd->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_CHORUS, 0, IID_IDirectSoundFXChorus8, (void**)&chorus);

	snd->EffChorus.fDelay = Delay;
	snd->EffChorus.fDepth = Depth;
	snd->EffChorus.fFeedback = Feedback;
	snd->EffChorus.fFrequency = Frequency;
	snd->EffChorus.lWaveform = Waveform;
	snd->EffChorus.fWetDryMix = WetDryMix;
	snd->EffChorus.lPhase = Phase;

	HRESULT hr = chorus->SetAllParameters(&snd->EffChorus);
	mem_release_del(chorus);
	/*if (FAILED(hr))
		info = SOUND_ERR_EFFECT_NON_SETUP;*/
	return true;
}

bool MainSound::SoundEffectFlangerSet(ID id, float WetDryMix, float Depth, float Feedback, float Frequency, long Waveform, float Delay, long Phase)
{
	SOUND_PRECOND(id, false);

	Sound* snd = ArrSounds[id];

	IDirectSoundFXFlanger8 *flanger = 0;

	if (!snd->EffectInit[SOUND_EFFECT_FLANDER])
		SoundEffectStateSet(id, SOUND_EFFECT_FLANDER, SOUND_EFF_ON);

	snd->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_FLANGER, 0, IID_IDirectSoundFXFlanger8, (void**)&flanger);

	snd->EffFlanger.fDelay = Delay;
	snd->EffFlanger.fDepth = Depth;
	snd->EffFlanger.fFeedback = Feedback;
	snd->EffFlanger.fFrequency = Frequency;
	snd->EffFlanger.lWaveform = Waveform;
	snd->EffFlanger.fWetDryMix = WetDryMix;
	snd->EffFlanger.lPhase = Phase;

	HRESULT hr = flanger->SetAllParameters(&snd->EffFlanger);
	mem_release_del(flanger);
	/*if (FAILED(hr))
		info = SOUND_ERR_EFFECT_NON_SETUP;*/
	return true;
}

bool MainSound::SoundEffectEchoSet(ID id, float WetDryMix, float Feedback, float LeftDelay, float RightDelay, long PanDelay)
{
	SOUND_PRECOND(id, false);

	Sound* snd = ArrSounds[id];

	IDirectSoundFXEcho8 *echo = 0;

	if (!snd->EffectInit[SOUND_EFFECT_ECHO])
		SoundEffectStateSet(id, SOUND_EFFECT_ECHO, SOUND_EFF_ON);

	snd->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_ECHO, 0, IID_IDirectSoundFXEcho8, (void**)&echo);

	snd->EffEcho.fWetDryMix = WetDryMix;
	snd->EffEcho.fFeedback = Feedback;
	snd->EffEcho.fRightDelay = RightDelay;
	snd->EffEcho.fLeftDelay = LeftDelay;
	snd->EffEcho.lPanDelay = PanDelay;

	HRESULT hr = echo->SetAllParameters(&snd->EffEcho);
	mem_release_del(echo);
	/*if (FAILED(hr))
		info = SOUND_ERR_EFFECT_NON_SETUP;*/
	return true;
}

bool MainSound::SoundEffectDistortionSet(ID id, float Gain, float Edge, float PostEQCenterFrequency, float PostEQBandwidth, float PreLowpassCutoff)
{
	SOUND_PRECOND(id, false);

	Sound* snd = ArrSounds[id];

	IDirectSoundFXDistortion8 *distortion = 0;

	if (!snd->EffectInit[SOUND_EFFECT_DISTORTION])
		SoundEffectStateSet(id, SOUND_EFFECT_DISTORTION, SOUND_EFF_ON);

	snd->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_ECHO, 0, IID_IDirectSoundFXDistortion8, (void**)&distortion);

	snd->EffDistortion.fEdge = Edge;
	snd->EffDistortion.fGain = Gain;
	snd->EffDistortion.fPostEQBandwidth = PostEQBandwidth;
	snd->EffDistortion.fPostEQCenterFrequency = PostEQCenterFrequency;
	snd->EffDistortion.fPreLowpassCutoff = PreLowpassCutoff;

	HRESULT hr = distortion->SetAllParameters(&snd->EffDistortion);

	/*if (FAILED(hr))
		info = SOUND_ERR_EFFECT_NON_SETUP;*/
	return true;
}

bool MainSound::SoundEffectCompressorSet(ID id, float Gain, float Attack, float Release, float Threshold, float Ratio, float Predelay)
{
	SOUND_PRECOND(id, false);

	Sound* snd = ArrSounds[id];

	IDirectSoundFXCompressor8 *compressor = 0;

	if (!snd->EffectInit[SOUND_EFFECT_COMPRESSOR])
		SoundEffectStateSet(id, SOUND_EFFECT_COMPRESSOR, SOUND_EFF_ON);

	snd->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_COMPRESSOR, 0, IID_IDirectSoundFXCompressor8, (void**)&compressor);

	snd->EffCompressor.fAttack = Attack;
	snd->EffCompressor.fGain = Gain;
	snd->EffCompressor.fPredelay = Predelay;
	snd->EffCompressor.fRatio = Ratio;
	snd->EffCompressor.fThreshold = Threshold;
	snd->EffCompressor.fRelease = Release;

	HRESULT hr = compressor->SetAllParameters(&snd->EffCompressor);

	/*if (FAILED(hr))
		info = SOUND_ERR_EFFECT_NON_SETUP;*/
	return true;
}

bool MainSound::SoundEffectParameqSet(ID id, float Center, float Bandwidth, float Gain)
{
	SOUND_PRECOND(id, false);

	Sound* snd = ArrSounds[id];

	IDirectSoundFXParamEq8 *parameq = 0;
	
	if (!snd->EffectInit[SOUND_EFFECT_PARAMEQ])
		SoundEffectStateSet(id, SOUND_EFFECT_PARAMEQ, SOUND_EFF_ON);

	snd->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_PARAMEQ, 0, IID_IDirectSoundFXParamEq8, (void**)&parameq);

	snd->EffParamEq.fBandwidth = Bandwidth;
	snd->EffParamEq.fCenter = Center;
	snd->EffParamEq.fGain = Gain;

	HRESULT hr = parameq->SetAllParameters(&snd->EffParamEq);

	/*if (FAILED(hr))
		info = SOUND_ERR_EFFECT_NON_SETUP;*/
	return true;
}

bool MainSound::SoundEffectI3DL2ReverbSet(ID id, long Room, long RoomHF, float RoomRolloffFactor, float DecayTime, float DecayHFRatio, long Reflections,
	float ReflectionsDelay, long Reverb, float ReverbDelay, float Diffusion, float Density, float HFReference)
{
	SOUND_PRECOND(id, false);

	Sound* snd = ArrSounds[id];

	IDirectSoundFXI3DL2Reverb8 *I3DL2Reverb8 = 0;

	if (!snd->EffectInit[SOUND_EFFECT_I3DL2REVERB])
		SoundEffectStateSet(id, SOUND_EFFECT_I3DL2REVERB, SOUND_EFF_ON);

	snd->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_I3DL2REVERB, 0, IID_IDirectSoundFXI3DL2Reverb8, (void**)&I3DL2Reverb8);

	snd->EffI3DL2Reverb.lRoom = Room;
	snd->EffI3DL2Reverb.lRoomHF = RoomHF;
	snd->EffI3DL2Reverb.flRoomRolloffFactor = RoomRolloffFactor;
	snd->EffI3DL2Reverb.flDecayTime = DecayTime;
	snd->EffI3DL2Reverb.flDecayHFRatio = DecayHFRatio;
	snd->EffI3DL2Reverb.lReflections = Reflections;
	snd->EffI3DL2Reverb.flReflectionsDelay = ReflectionsDelay;
	snd->EffI3DL2Reverb.lReverb = Reverb;
	snd->EffI3DL2Reverb.flReverbDelay = ReverbDelay;
	snd->EffI3DL2Reverb.flDiffusion = Diffusion;
	snd->EffI3DL2Reverb.flDensity = Density;
	snd->EffI3DL2Reverb.flHFReference = HFReference;

	HRESULT hr = I3DL2Reverb8->SetAllParameters(&snd->EffI3DL2Reverb);

	/*if (FAILED(hr))
		info = SOUND_ERR_EFFECT_NON_SETUP;*/
	return true;
}

bool MainSound::SoundEffectWavesReverbSet(ID id, float InGain, float ReverbMix, float ReverbTime, float HighFreqRTRatio)
{
	SOUND_PRECOND(id, false);

	Sound* snd = ArrSounds[id];

	IDirectSoundFXWavesReverb8 *wavereverb = 0;

	if (!snd->EffectInit[SOUND_EFFECT_WAVESREVERB])
		SoundEffectStateSet(id, SOUND_EFFECT_WAVESREVERB, SOUND_EFF_ON);

	snd->DSBuffer->GetObjectInPath(GUID_DSFX_WAVES_REVERB, 0, IID_IDirectSoundFXWavesReverb8, (void**)&wavereverb);

	snd->EffWavesReverb.fHighFreqRTRatio = HighFreqRTRatio;
	snd->EffWavesReverb.fInGain = InGain;
	snd->EffWavesReverb.fReverbMix = ReverbMix;
	snd->EffWavesReverb.fReverbTime = ReverbTime;

	HRESULT hr = wavereverb->SetAllParameters(&snd->EffWavesReverb);

	/*if (FAILED(hr))
		info = SOUND_ERR_EFFECT_NON_SETUP;*/
	return true;
}