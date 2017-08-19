
#include <score/sound.h>

MainSound::MainSound()
{
	DeviceSound = 0;
	DSPrimary = 0;
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
	Id = -1;
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

	DSBUFFERDESC  dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));

	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat = 0;

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
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE;
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
	sprintf(fullpath, "%s%s", Core_RStringGet(G_RI_STRING_PATH_GS_SOUNDS), file);

	if (!Core_0FileExists(fullpath))
	{
		reportf(REPORT_MSG_LEVEL_ERROR, "%s - file not found [%s]", gen_msg_location, fullpath);
		return -1;
	}

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

	snd->StreamSize = 0;
	if (size_stream > 0)
	{
		if (size_stream < SOUND_MIN_SIZE_STREAM)
			snd->StreamSize = SOUND_MIN_SIZE_STREAM;
		else
			snd->StreamSize = size_stream;
	}

	Load(snd, fullpath, fmt);

	snd->DSBuffer->GetFrequency(&snd->FrecOrigin);
	snd->Id = AddSound(snd);
	return snd->Id;
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
	snd->ShiftPan = shift_pan;
	
	return sndid;
}

ID MainSound::SoundCreate2dInst(const char *file, bool looping, DWORD size_stream)
{
	ID idsnd = SoundFind2dInst(file);
	if (idsnd >= 0)
		return idsnd;
	idsnd = SoundCreate2d(file, looping, size_stream);
	AArr2dInst[file] = idsnd;
	ArrSounds[idsnd]->IsInst = true;
	return idsnd;
}

ID MainSound::SoundCreate3dInst(const char *file, bool looping, DWORD size_stream, float dist, float shift_pan)
{
	ID idsnd = SoundFind2dInst(file);
	if (idsnd >= 0)
		return idsnd;
	idsnd = SoundCreate3d(file, looping, size_stream, dist, shift_pan);
	AArr3dInst[file] = idsnd;
	ArrSounds[idsnd]->IsInst = true;
	return idsnd;
}

ID MainSound::SoundFind2dInst(const char * file)
{
	ID id = -1;
	const AssotiativeArray<AAStringNR, ID, false, 16>::Node* pNode = 0;
	if (AArr2dInst.KeyExists(file, &pNode))
		id = *(pNode->Val);

	return id;
}

ID MainSound::SoundFind3dInst(const char * file)
{
	ID id = -1;
	const AssotiativeArray<AAStringNR, ID, false, 16>::Node* pNode = 0;
	if (AArr3dInst.KeyExists(file, &pNode))
		id = *(pNode->Val);

	return id;
}

void MainSound::SoundInstancePlay2d(ID id, int volume, int pan)
{
	SOUND_PRECOND(id, _VOID);

	Sound* snd = ArrSounds[id];

	if (snd->StreamSize > 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "%s - can not create sound instance for streaming [%s]", gen_msg_location, snd->RPath);
		return;
	}

	if (snd->Is3d)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "%s - can not create 2d sound instance by 3d [%s]", gen_msg_location, snd->RPath);
		return;
	}

	ID id_instance = -1;
	for (int i = 0; i < snd->DataInstances.size(); ++i)
	{
		if (!snd->DataInstances[i].busy)
		{
			id_instance = i;
			break;
		}
	}

	if (id_instance < 0)
	{
		IDirectSoundBuffer* tsb;
		IDirectSoundBuffer8* tsb8;
		DeviceSound->DuplicateSoundBuffer(snd->DSBuffer, &tsb);
		if (!tsb)
		{
			reportf(REPORT_MSG_LEVEL_WARRNING, "%s - can not create sound instance [%s], this is big problem", gen_msg_location, snd->RPath);
			return;
		}
		tsb->QueryInterface(IID_IDirectSoundBuffer8, (void**)&tsb8);
		snd->DataInstances.push_back(Sound::SIData(tsb8, 0, true));
		id_instance = snd->DataInstances.size() - 1;
	}

	if (id_instance >= 0)
	{
		long tvol;
		snd->DSBuffer->GetVolume(&tvol);
		snd->DataInstances[id_instance].busy = true;
		//IDirectSoundBuffer8* tsb = snd->DataInstances[id_instance].sbuffer;
		snd->DataInstances[id_instance].sbuffer->SetVolume((tvol > -5000 ? tvol - 5000 : tvol + 5000));
		snd->DataInstances[id_instance].sbuffer->SetCurrentPosition(0);
		snd->DataInstances[id_instance].sbuffer->SetVolume(-10000 + (volume * 100));
		snd->DataInstances[id_instance].sbuffer->SetPan((pan > 0) ? (10000 - (pan * 100)) - 10000 : 10000 - (10000 + (pan * 100)));
		snd->DataInstances[id_instance].sbuffer->Play(0, 0, 0);
	}
}

void MainSound::SoundInstancePlay3d(ID id, float3* pos)
{
	SOUND_PRECOND(id, _VOID);

	if (!pos)
		return;

	Sound* snd = ArrSounds[id];

	if (snd->StreamSize > 0)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "%s - can not create sound instance for streaming [%s]", gen_msg_location, snd->RPath);
		return;
	}

	if (!snd->Is3d)
	{
		reportf(REPORT_MSG_LEVEL_WARRNING, "%s - can not create 3d sound instance by 2d[%s]", gen_msg_location, snd->RPath);
		return;
	}

	ID id_instance = -1;
	for (int i = 0; i < snd->DataInstances.size(); ++i)
	{
		if (!snd->DataInstances[i].busy)
		{
			id_instance = i;
			break;
		}
	}

	if (id_instance < 0)
	{
		IDirectSoundBuffer* tsb;
		IDirectSoundBuffer8* tsb8;
		DeviceSound->DuplicateSoundBuffer(snd->DSBuffer, &tsb);
		if (!tsb)
		{
			reportf(REPORT_MSG_LEVEL_WARRNING, "%s - can not create sound instance [%s], this is big problem", gen_msg_location, snd->RPath);
			return;
		}
		tsb->QueryInterface(IID_IDirectSoundBuffer8, (void**)&tsb8);
		snd->DataInstances.push_back(Sound::SIData(tsb8, &float3_t(*pos), true));
		id_instance = snd->DataInstances.size() - 1;
	}

	if (id_instance >= 0)
	{
		long tvol;
		snd->DSBuffer->GetVolume(&tvol);
		snd->DataInstances[id_instance].sbuffer->SetVolume((tvol > -5000 ? tvol - 5000 : tvol + 5000));

		snd->DataInstances[id_instance].sbuffer->SetVolume(SOUND_3D_COM_VOLUME((*pos), OldViewPos, snd->DistAudible));
		snd->DataInstances[id_instance].sbuffer->SetPan(SOUND_3D_COM_PAN((*pos), OldViewPos, OldViewDir, snd->DistAudible,snd->ShiftPan));
		snd->DataInstances[id_instance].sbuffer->SetCurrentPosition(0);
		snd->DataInstances[id_instance].sbuffer->Play(0, 0, 0);
	}
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
	Sound* snd = ArrSounds[id];
	if (snd->IsInst)
	{
		if (snd->Is3d)
			AArr3dInst.erase(snd->RPath);
		else
			AArr2dInst.erase(snd->RPath);
	}
	mem_delete(snd);
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

		snd->DSBuffer->SetPan(Value);
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
	DWORD status = 0;
	for (int i = 0; i < ArrSounds.size(); ++i)
	{
		snd = ArrSounds[i];
		if (snd)
		{
			status = 0;
			snd->DSBuffer->GetStatus(&status);
			if (!(status & DSBSTATUS_PLAYING) && snd->State == SoundObjState::sos_play)
				SoundStateSet(i, SoundObjState::sos_stop);

			++tmpSoundsLoadCount;
			if (snd->State == SoundObjState::sos_play)
				++tmpSoundsPlayCount;
			if (snd->Is3d && snd->DSBuffer && viewpos && viewdir)
			{
				snd->DSBuffer->SetVolume(SOUND_3D_COM_VOLUME(snd->Position, (*viewpos), snd->DistAudible));
				snd->DSBuffer->SetPan(SOUND_3D_COM_PAN(snd->Position, (*viewpos), (*viewdir), snd->DistAudible, snd->ShiftPan));

				if (snd->DataInstances.size() > 0)
				{
					for (int k = 0, l = snd->DataInstances.size(); k < l; ++k)
					{
						snd->DSBuffer->SetVolume(SOUND_3D_COM_VOLUME(snd->DataInstances[k].pos, (*viewpos), snd->DistAudible));
						snd->DSBuffer->SetPan(SOUND_3D_COM_PAN(snd->DataInstances[k].pos, (*viewpos), (*viewdir), snd->DistAudible, snd->ShiftPan));
					}
				}
			}

			for (int k = 0, l = snd->DataInstances.size(); k < l; ++k)
			{
				status = 0;
				if (SUCCEEDED(snd->DataInstances[k].sbuffer->GetStatus(&status)) && !(status & DSBSTATUS_PLAYING))
					snd->DataInstances[k].busy = false;
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

	OldViewPos = *viewpos;
	OldViewDir = *viewdir;
}

int MainSound::SoundsPlayCountGet()
{
	return SoundsPlayCount;
}

int MainSound::SoundsLoadCountGet()
{
	return SoundsLoadCount;
}
