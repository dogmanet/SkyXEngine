
#include <sound\sxsounds_class.h>

SXMainSound::SXMainSound(const char* name)
{
	sprintf(Name, "%s", name);
}

long SXMainSound::Init(HWND hwnd)
{
		if(FAILED(DirectSoundCreate8(NULL, &DeviceSound, NULL))) 
			return SOUND_CORE_ERR_FAILED_INIT;

		if(FAILED(DeviceSound->SetCooperativeLevel(hwnd,DSSCL_EXCLUSIVE)))
			return SOUND_CORE_ERR_FAILED_CL;

	DSBUFFERDESC       dsbd;
	ZeroMemory(&dsbd,sizeof(DSBUFFERDESC));

	dsbd.dwSize        = sizeof(DSBUFFERDESC);
	dsbd.dwFlags       = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	dsbd.dwBufferBytes = 0;
	dsbd.lpwfxFormat   = NULL;

		if(FAILED(DeviceSound->CreateSoundBuffer(&dsbd, &DSPrimary, NULL))) 
			return SOUND_CORE_ERR_FAILED_PRIM_BUF;

	WAVEFORMATEX wfex;
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));

	wfex.wFormatTag      = WAVE_FORMAT_PCM;
	wfex.nChannels       = 2;
	wfex.nSamplesPerSec  = 48000;
	wfex.wBitsPerSample  = 32;
	wfex.nBlockAlign     = (wfex.wBitsPerSample / 8) * wfex.nChannels;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

		if(FAILED(DSPrimary->SetFormat(&wfex)))
			return SOUND_CORE_ERR_FAILED_SET_FORMAT;

	DSPrimary->SetVolume(0);
	DSPrimary->Play(0, 0, DSBPLAY_LOOPING);

	CountPlaySounds = 0;
	CountLoadSounds = 0;

		for(int i=0;i<SOUND_MAX_SOUNDS;i++)
			ArrSounds[i] = 0;

	CountKeyInArr = SOUND_MAX_SOUNDS;
	GDeviceSound = DeviceSound;
}

void SXMainSound::SetStdPath(const char* path)
{
	sprintf(StdPath, "%s", path);
}

void SXMainSound::GetStdPath(char* path)
{
	sprintf(path, "%s", StdPath);
}

SXMainSound::~SXMainSound()
{
		for(int i=0;i<CountKeyInArr;i++)
		{
				if(ArrSounds[i] != 0)
				{
						if(ArrSounds[i]->Playing())
							ArrSounds[i]->Stop();
					mem_delete(ArrSounds[i]);
				}
		}
}

void SXMainSound::Update(float3* campos, float3* camdir)
{
	DWORD tmpCountPlaySounds = 0;
	DWORD tmpCountLoadSounds = 0;
	
		for(int i=0;i<CountKeyInArr;i++)
		{
				if(ArrSounds[i] != 0)
				{
					tmpCountLoadSounds++;
						if(ArrSounds[i]->Playing())
						{
							ArrSounds[i]->Update(campos, camdir);
							tmpCountPlaySounds++;
						}
				}
		}
	CountPlaySounds = tmpCountPlaySounds;
	CountLoadSounds = tmpCountLoadSounds;
}


DWORD SXMainSound::AddSoundObject(SXSoundObject *Obj)
{
		for(int i=0;i<CountKeyInArr;i++)
		{
				if(ArrSounds[i] == 0)
				{
					ArrSounds[i] = Obj;
					return i;
				}
		}
	ArrSounds[CountKeyInArr++] = Obj;
	return CountKeyInArr;
}

inline SXSoundObject* SXMainSound::GetSoundObject(DWORD id)
{
		if(id < CountKeyInArr && ArrSounds[id])
			return ArrSounds[id];
		/*else if(id >= CountKeyInArr)
			Core::InError("SXMainSound::GetSXSoundObject");
		else if(!ArrSounds[id])
			Core::InError("SXMainSound::GetSXSoundObject");*/
}

void SXMainSound::DeleteSoundObject(DWORD num)
{
		if(num < CountKeyInArr && ArrSounds[num])
			mem_delete(ArrSounds[num]);
}

DWORD SXMainSound::GetCountPlaySounds()
{
	return CountPlaySounds;
}

DWORD SXMainSound::GetCountLoadSounds()
{
	return CountLoadSounds;
}

DWORD SXMainSound::GetCountKey()
{
	return CountKeyInArr;
}




//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//

IDirectSoundBuffer8 *SoundControl::CreateSoundBuffer(WaveHeader *Hdr)
{
	IDirectSoundBuffer  *DSB;
	IDirectSoundBuffer8 *DSBuffer;
	DSBUFFERDESC        dsbd;
	WAVEFORMATEX        wfex;

	// ”станавливаем формат воспроизведени€
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
	wfex.wFormatTag      = WAVE_FORMAT_PCM;
	wfex.nChannels       = Hdr->Channels;
	wfex.nSamplesPerSec  = Hdr->SampleRate;
	wfex.wBitsPerSample  = Hdr->BitsPerSample;
	wfex.nBlockAlign     = wfex.wBitsPerSample / 8 * wfex.nChannels;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

	// —оздаем звуковой буфер, использу€ данные заголовка
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLFX;
	dsbd.dwBufferBytes = Hdr->DataSize;
	dsbd.lpwfxFormat = &wfex;
	if (FAILED(GDeviceSound->CreateSoundBuffer(&dsbd, &DSB, NULL)))
			return NULL;

		if(FAILED(DSB->QueryInterface(IID_IDirectSoundBuffer8,(void**)&DSBuffer))) 
		{
			DSB->Release();
			return NULL;
		}

	return DSBuffer;
}

bool SoundControl::LoadSoundDataWAV(IDirectSoundBuffer8	*DSBuffer,long	LockPos,FILE* data,long	Size,DWORD	flag)
{
	BYTE  *Ptr1, *Ptr2;
	DWORD Size1, Size2;

		if(!Size)
			return false;

		if(FAILED(DSBuffer->Lock(LockPos, Size,(void**)&Ptr1, &Size1,(void**)&Ptr2, &Size2, flag)))
			return false;

	fread(Ptr1, 1, Size1, data);
		if(Ptr2)
			fread(Ptr2, 1, Size2, data);

	DSBuffer->Unlock(Ptr1, Size1, Ptr2, Size2);
	return true;
}

int SoundControl::LoadWAV(const char	*FileName,SoundLoadDataObject** DataObject)
{
	IDirectSoundBuffer8 *DSBuffer;
	WaveHeader Hdr;
	FILE *fp;

		if(!(fp=fopen(FileName, "rb")))
			return SOUND_ERR_OPEN;

	fseek(fp, 0, SEEK_SET);
	fread(&Hdr, 1, sizeof(WaveHeader), fp);
	Hdr.BytesPerSec = ((Hdr.BitsPerSample / 8) * Hdr.Channels) * Hdr.SampleRate;

		if(memcmp(Hdr.RiffSig, "RIFF", 4) || memcmp(Hdr.WaveSig, "WAVE", 4) || memcmp(Hdr.FormatSig, "fmt ", 4) /*|| memcmp(Hdr.DataSig, "data", 4)*/)
			return SOUND_ERR_FORMAT;

		if(!(DSBuffer = CreateSoundBuffer(&Hdr))) 
			return SOUND_ERR_CREATE_SB;

	fseek(fp, sizeof(WaveHeader), SEEK_SET);

	LoadSoundDataWAV(DSBuffer, 0, fp, Hdr.DataSize,0);

	*DataObject = new SoundLoadDataObject();
	(*DataObject)->DSBuffer = DSBuffer;
	(*DataObject)->File = 0;
	(*DataObject)->HowFormatFile = 0;

	(*DataObject)->InfoObject->CountSec = Hdr.DataSize / (Hdr.BytesPerSec);
	(*DataObject)->InfoObject->DataSize = Hdr.DataSize;
	(*DataObject)->InfoObject->CountChannels = Hdr.Channels;
	(*DataObject)->InfoObject->SampleRate = Hdr.SampleRate;
	(*DataObject)->InfoObject->BitsPerSample = Hdr.BitsPerSample;
	(*DataObject)->InfoObject->Path = FileName;
	(*DataObject)->InfoObject->BytesPerSec = Hdr.BytesPerSec;

	(*DataObject)->VorbisFile = 0;

	fclose(fp);
	return SOUND_OK;
}

int SoundControl::LoadSoundDataOGG(OggVorbis_File* VorbisFile,IDirectSoundBuffer8 *DSBuffer,DWORD LockPos,DWORD Size,DWORD flag)
{
	char  *Ptr1, *Ptr2;
	DWORD Size1, Size2;

		if(!Size)
			return SOUND_ERR_SIZING;

		if(FAILED(DSBuffer->Lock(LockPos, Size,(void**)&Ptr1, &Size1,(void**)&Ptr2, &Size2, flag)))
			return SOUND_ERR_LOCK;

	DWORD total_read=0;
	long bites_read;
	int current_section = 0;

		while(total_read < Size1)
		{
			bites_read = ov_read(VorbisFile, Ptr1 + total_read, Size1 - total_read, 0, 2, 1, &current_section);

				//конец файла
				if(bites_read == 0) 
					break;

				if(bites_read > 0)
				{
					total_read += bites_read;
				}
				else if(bites_read == OV_EINVAL)
				{
					return SOUND_ERR_OGG_DECODING;
				}
				else 
					break;
		}


		if(Ptr2)
		{
			total_read = 0;
				while(total_read < Size2)
				{
					bites_read = ov_read(VorbisFile, Ptr2 + total_read, Size2 - total_read, 0, 2, 1, &current_section);

						if(bites_read == 0) 
							break;

						if(bites_read > 0)
						{
							total_read += bites_read;
						}
						else if(bites_read == OV_EINVAL)
						{
							return SOUND_ERR_OGG_DECODING;
						}
						else
							break;
				}
		}

	DSBuffer->Unlock(Ptr1, Size1, Ptr2, Size2);
	return 0;
}

int SoundControl::LoadOGG(const char *FileName,SoundLoadDataObject** DataObject)
{
	OggVorbis_File ogg;
	*DataObject = new SoundLoadDataObject();

	FILE* f = fopen(FileName, "rb");
  
		if(f == NULL)
			return SOUND_ERR_OPEN;

		if(ov_fopen(FileName,&ogg))
			return SOUND_ERR_FORMAT;

	IDirectSoundBuffer8 *DSBuffer;

	long bites_read;
	int current_section = 0;
	long fsize = 0;

	OggVorbis_File* vorbisfile = new OggVorbis_File;

	ov_callbacks cb;
	cb.close_func = close_ogg;
	cb.read_func = read_ogg;
	cb.seek_func = seek_ogg;
	cb.tell_func = tell_ogg;

	ov_open_callbacks(f, vorbisfile, 0, 0, cb);

	vorbis_info *vi = ov_info(&ogg, -1);

		if(!vi)
			return SOUND_ERR_HEADER;

	fsize = ov_pcm_total(vorbisfile,-1)*2*vi->channels;

	(*DataObject)->InfoObject->CountSec = float(ov_pcm_total(vorbisfile,-1)) / float(vi->rate);
	(*DataObject)->InfoObject->DataSize = fsize;
	(*DataObject)->InfoObject->CountChannels = vi->channels;
	(*DataObject)->InfoObject->SampleRate = vi->rate;
	(*DataObject)->InfoObject->BitsPerSample = 16;
	(*DataObject)->InfoObject->Path = FileName;
	(*DataObject)->InfoObject->BytesPerSec = vi->rate * (((*DataObject)->InfoObject->BitsPerSample / 8) * vi->channels);

	WaveHeader hdr;
	hdr.Channels = vi->channels;
	hdr.SampleRate = vi->rate;
	hdr.BitsPerSample = 16;
	hdr.DataSize = fsize;
	DSBuffer = SoundControl::CreateSoundBufferStream(&hdr,fsize);

		if(!DSBuffer)
			return SOUND_ERR_CREATE_SB;

		
	int c_err = SoundControl::LoadSoundDataOGG(vorbisfile, DSBuffer, 0, fsize, 0);
	if (c_err != 0)
		return c_err;

	fclose(f);

	(*DataObject)->DSBuffer = DSBuffer;
	(*DataObject)->File = 0;
	(*DataObject)->HowFormatFile = 1;
	(*DataObject)->VorbisFile = 0;

	ov_clear(&ogg);
	ov_clear(vorbisfile);

	return SOUND_OK;
}


IDirectSoundBuffer8	*SoundControl::CreateSoundBufferStream	(WaveHeader	*Hdr, DWORD size)
{
	IDirectSoundBuffer  *DSB;
	IDirectSoundBuffer8 *DSBuffer;
	DSBUFFERDESC        dsbd;
	WAVEFORMATEX        wfex;

	// ”станавливаем формат воспроизведени€
	ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
	wfex.wFormatTag      = WAVE_FORMAT_PCM;
	wfex.nChannels       = Hdr->Channels;
	wfex.nSamplesPerSec  = Hdr->SampleRate;
	wfex.wBitsPerSample  = Hdr->BitsPerSample;
	wfex.nBlockAlign     = wfex.wBitsPerSample / 8 * wfex.nChannels;
	wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

	// —оздаем звуковой буфер, использу€ данные заголовка
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLFX;
	dsbd.dwBufferBytes = size;
	dsbd.lpwfxFormat = &wfex;
	if (FAILED(GDeviceSound->CreateSoundBuffer(&dsbd, &DSB, NULL)))
		{
			return NULL;
		}

		if(FAILED(DSB->QueryInterface(IID_IDirectSoundBuffer8,(void**)&DSBuffer))) 
		{
			DSB->Release();
			return NULL;
		}

	return DSBuffer;
}

WaveHeader *SoundControl::GetHdrWav(FILE	*fp)
{
	WaveHeader *Hdr;
	fseek(fp, 0, SEEK_SET);
	Hdr = new WaveHeader;
	fread(Hdr, 1, sizeof(WaveHeader), fp);

		if(memcmp(Hdr->RiffSig, "RIFF", 4) || memcmp(Hdr->WaveSig, "WAVE", 4) || memcmp(Hdr->FormatSig, "fmt ", 4)/* || memcmp(Hdr->DataSig, "data", 4)*/)
		{
			delete Hdr;
			return NULL;
		}

	return Hdr;
}


int SoundControl::HowFormat(const char* file)
{
	FILE *fp;

		if(!(fp=fopen(file, "rb")))
			return SOUND_ERR_OPEN;

	WaveHeader *Hdr = SoundControl::GetHdrWav(fp);

		if(Hdr != 0)
		{
			fclose(fp);
			return SOUND_WAV_FORMAT;
		}
	
		if(strcmp(fp->_base,"OggS") == 0)
		{
			fclose(fp);
			return SOUND_OGG_FORMAT;
		}

	fclose(fp);

	return SOUND_NON_FORMAT;
}

//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//--//

long SXSoundObject::Load(const char *file,bool looping, int streaming,DWORD size_stream)
{
	sprintf(File, "%s%s", ObjectMainSound->StdPath, file);

	//MinDist = 0;

	int tmp_info = SOUND_OK;

	VolumeSound = 0;
	Looping = looping;
	IsStreaming = (streaming ? true : false);
	Is3d = false;
	InitEffectBool();

	tmp_info = SoundControl::HowFormat(File);
	if (tmp_info != SOUND_NON_FORMAT)
		return tmp_info;

		if(IsStreaming)
		{
				if(size_stream < SOUND_MIN_SIZE_STREAM)
					size_stream = SOUND_MIN_SIZE_STREAM;
		}


		//статическа€ загрузка
		if(streaming == 0)
		{
			tmp_info = LoadStatic(File, tmp_info);
			if (tmp_info != 0)
				return tmp_info;
		}
		//динамическа€ загрузка
		else
		{
			this->DataObject = new SoundLoadDataObject();
				if(tmp_info == SOUND_OGG_FORMAT)
					tmp_info = LoadDynamicOGG(File,size_stream);
				else if(tmp_info == SOUND_WAV_FORMAT)
					tmp_info = LoadDynamicWAV(File,size_stream);
			
				if (tmp_info != 0)
					return tmp_info;
		}
				
	ID = ObjectMainSound->AddSoundObject(this);
	SetVolume(100,SOUND_TV_PCT);

	BeginFrec = GetFrequency();
	State = 0;
	return 0;
}

long SXSoundObject::Load(const char *file, bool looping, int streaming, DWORD size_stream, float max_dist, float damping, float shift_pan)
{
	sprintf(File, "%s%s", ObjectMainSound->StdPath, file);

	//MinDist = 0;
	MaxDist = max_dist;

	int tmp_info = SOUND_OK;

	VolumeSound = 0;
	Looping = looping;
	IsStreaming = (streaming ? true : false);
	Is3d = true;
	Damping = damping;
	ShiftPan = shift_pan;
	InitEffectBool();

	tmp_info = SoundControl::HowFormat(File);
	if (tmp_info != SOUND_NON_FORMAT)
		return tmp_info;

		if(IsStreaming)
		{
				if(size_stream < SOUND_MIN_SIZE_STREAM)
					size_stream = SOUND_MIN_SIZE_STREAM;
		}

		//статическа€ загрузка
		if(streaming == 0)
		{
			tmp_info = LoadStatic(File, tmp_info);
			if (tmp_info != 0)
				return tmp_info;
		}
		//динамическа€ загрузка
		else
		{
			this->DataObject = new SoundLoadDataObject();
				if(tmp_info == SOUND_OGG_FORMAT)
				{
					tmp_info = LoadDynamicOGG(File,size_stream);
				}
				else if(tmp_info == SOUND_WAV_FORMAT)
				{
					tmp_info = LoadDynamicWAV(File,size_stream);
				}

				if (tmp_info != 0)
					return tmp_info;
		}

	ID = ObjectMainSound->AddSoundObject(this);
	SetVolume(100,SOUND_TV_PCT);

	BeginFrec = GetFrequency();
	State = 0;
	return 0;
}

int SXSoundObject::LoadStatic(const char *file,int format)
{
	this->DataObject = 0;
		
	int ErrorLoad = SOUND_OK;

		if(format == SOUND_OGG_FORMAT)
			ErrorLoad = SoundControl::LoadOGG(file,&this->DataObject);
		else if(format == SOUND_WAV_FORMAT)
			ErrorLoad = SoundControl::LoadWAV(file,&this->DataObject);

		if(ErrorLoad == SOUND_OK)
		{
			DataObject->DSBuffer->SetCurrentPosition(0);
		}
	return ErrorLoad;
}

int SXSoundObject::LoadDynamicWAV(const char *file,DWORD size_stream)
{
	MaxSize = size_stream;

	DataObject = new SoundLoadDataObject();

	WaveHeader Hdr;

	SizeSplit1 = MaxSize / 4;
	SizeSplit2 = SizeSplit1 * 2;
	SizeSplit3 = SizeSplit1 * 3;

	BF1 = false;
	BF2 = false;
	BF3 = false;
	BF4 = false;

	IsStarting = true;

		if(!(DataObject->File=fopen(file, "rb")))
			return SOUND_ERR_OPEN;

	fread(&Hdr, 1, sizeof(WaveHeader), DataObject->File);
	
		if(memcmp(Hdr.RiffSig, "RIFF", 4) || memcmp(Hdr.WaveSig, "WAVE", 4) || memcmp(Hdr.FormatSig, "fmt ", 4) || memcmp(Hdr.DataSig, "data", 4))
		{
			return SOUND_ERR_HEADER;
		}

		if(!(DataObject->DSBuffer = SoundControl::CreateSoundBufferStream(&Hdr,MaxSize)))
		{
			fclose(DataObject->File);
			return SOUND_ERR_CREATE_SB;
		}

	fseek(DataObject->File, sizeof(WaveHeader), SEEK_SET);
	Hdr.BytesPerSec = Hdr.SampleRate * ((Hdr.BitsPerSample / 8) * Hdr.Channels);
	
	SoundControl::LoadSoundDataWAV(DataObject->DSBuffer, 0, DataObject->File, MaxSize,0);

	DataObject->InfoObject->DataSize = Hdr.DataSize;
	DataObject->InfoObject->BytesPerSec = Hdr.BytesPerSec;
	DataObject->HowFormatFile = SOUND_WAV_FORMAT;
	DataObject->InfoObject->CountChannels = Hdr.Channels;
	DataObject->InfoObject->Path = file;
	DataObject->InfoObject->CountSec = Hdr.DataSize / (Hdr.BytesPerSec);
	DataObject->InfoObject->DataSize = Hdr.DataSize;
	DataObject->InfoObject->SampleRate = Hdr.SampleRate;
	DataObject->InfoObject->BitsPerSample = Hdr.BitsPerSample;

	HowRePlay = 0;
	double tmpCRPE = double(DataObject->InfoObject->DataSize) / double(MaxSize);
	float Count = 0;
		while(1)
		{
			if(tmpCRPE > Count && tmpCRPE < Count+1)
			{
				CountRePlayEnd = Count+1;
				break;
			}
			else if(tmpCRPE == Count)
			{
				CountRePlayEnd = Count;
				break;
			}
			Count++;
		}
	return SOUND_OK;
}

int SXSoundObject::LoadDynamicOGG(const char *file,DWORD size_stream)
{
	MaxSize = size_stream;

	WaveHeader Hdr;

	DataObject = new SoundLoadDataObject();

	SizeSplit1 = MaxSize / 4;
	SizeSplit2 = SizeSplit1 * 2;
	SizeSplit3 = SizeSplit1 * 3;

	BF1 = false;
	BF2 = false;
	BF3 = false;
	BF4 = false;

	IsStarting = true;

	OggVorbis_File ogg;
	ov_fopen(file,&ogg);
	
	long bites_read;
	int current_section = 0;
	long fsize = 0;

	DataObject->VorbisFile = new OggVorbis_File;
	ov_callbacks cb;
	cb.close_func = close_ogg;
	cb.read_func = read_ogg;
	cb.seek_func = seek_ogg;
	cb.tell_func = tell_ogg;

	
		if(!(DataObject->File=fopen(file, "rb")))
		{
			return SOUND_ERR_OPEN;
		}

	ov_open_callbacks(DataObject->File, DataObject->VorbisFile, 0, 0, cb);

	vorbis_info *vi = ov_info(&ogg, -1);
	fsize = ov_pcm_total(DataObject->VorbisFile,-1)*2*vi->channels;

	Hdr.DataSize = fsize;
	Hdr.BitsPerSample = 16;
	Hdr.SampleRate = vi->rate;
	Hdr.Channels = vi->channels;
	Hdr.BytesPerSec = ((Hdr.BitsPerSample / 8) * vi->channels) * Hdr.SampleRate;

		if(!(DataObject->DSBuffer = SoundControl::CreateSoundBufferStream(&Hdr,MaxSize)))
		{
			fclose(DataObject->File);
			return SOUND_ERR_CREATE_SB;
		}

	SoundControl::LoadSoundDataOGG(DataObject->VorbisFile,DataObject->DSBuffer, 0, MaxSize,0);

	DataObject->HowFormatFile = SOUND_OGG_FORMAT;
	DataObject->InfoObject->CountChannels = Hdr.Channels;
	DataObject->InfoObject->Path = file;
	DataObject->InfoObject->CountSec = Hdr.DataSize / (Hdr.BytesPerSec);
	DataObject->InfoObject->DataSize = Hdr.DataSize;
	DataObject->InfoObject->SampleRate = Hdr.SampleRate;
	DataObject->InfoObject->BitsPerSample = Hdr.BitsPerSample;
	DataObject->InfoObject->BytesPerSec = Hdr.BytesPerSec;

	HowRePlay = 0;
	double tmpCRPE = double(DataObject->InfoObject->DataSize) / double(MaxSize);
	float Count = 0;

		while(1)
		{
				if(tmpCRPE > Count && tmpCRPE < Count+1)
				{
					CountRePlayEnd = Count+1;
					break;
				}
				else if(tmpCRPE == Count)
				{
					CountRePlayEnd = Count;
					break;
				}
			Count++;
		}
	ov_clear(&ogg);
	return SOUND_OK;
}

inline void SXSoundObject::Play()
{
	//зацикливаем воспроизведение дл€ потокового воспроизведени€
	//остановка произойдет по достижени€ конца если задан флаг
		if(DataObject->DSBuffer)
		{
			DataObject->DSBuffer->Play(0,0,(IsStreaming || Looping ? DSBPLAY_LOOPING : 0));
			State = 1;
		}
}

inline void SXSoundObject::Stop()
{
		if(DataObject->DSBuffer)
		{
			DataObject->DSBuffer->Stop();
			State = 0;
		}
}

void SXSoundObject::Update(float3* campos,float3* camdir)
{
		if(Is3d)
		{
			this->Update3d(campos, camdir);
		}

		if(LastPos == this->GetCurrentPos())
			State = 0;
		else
		{
			State = 1;
			LastPos = this->GetCurrentPos();
		}

		if(IsStreaming && DataObject->DSBuffer != 0)
		{
			DWORD pos;
			DataObject->DSBuffer->GetCurrentPosition(&pos,0);

				if(pos >= SizeSplit1 && pos < SizeSplit2 && !BF2)
				{
					ReLoadSplit(0,SizeSplit1);
					BF2 = true;
					BF1 = false;BF3 = false;BF4 = false;
					ActiveSplit = 2;
				}
				else if(pos >= SizeSplit2 && pos < SizeSplit3 && !BF3)
				{
					BF3 = true;
					BF1 = false;BF2 = false;BF4 = false;
					ReLoadSplit(SizeSplit1,SizeSplit1);
					ActiveSplit = 3;
				}
				else if(pos >= SizeSplit3 && pos < MaxSize && !BF4)
				{
					BF4 = true;
					BF1 = false;BF2 = false;BF3 = false;
					ReLoadSplit(SizeSplit2,SizeSplit1);
					ActiveSplit = 4;
				}
				else if(pos < SizeSplit1 && !BF1)
				{
					BF1 = true;
					BF2 = false;BF3 = false;BF4 = false;
						if(!IsStarting)
						{
							ReLoadSplit(SizeSplit3,SizeSplit1);
							HowRePlay++;
						}
					ActiveSplit = 1;
					IsStarting = false;
				}


				if(HowRePlay+1 == CountRePlayEnd)
				{
						if(DataObject->InfoObject->DataSize <= (MaxSize * HowRePlay + pos))
						{
								if(!Looping)
									Stop();

								//wav
								if(this->DataObject->HowFormatFile == SOUND_WAV_FORMAT)
								{
									fseek(DataObject->File, sizeof(WaveHeader), SEEK_SET);
								}
								//ogg
								else if(this->DataObject->HowFormatFile == SOUND_OGG_FORMAT)
								{
									ov_pcm_seek(DataObject->VorbisFile,0);
								}

							ReLoadSplit(0,MaxSize);
							
							DataObject->DSBuffer->SetCurrentPosition(0);

							HowRePlay = 0;
							IsStarting = true;

							BF1 = false;BF2 = false;BF3 = false;BF4 = false;
						}
				}
		}
}

void SXSoundObject::SetCurrentPos(DWORD pos,int type)
{
	DWORD PosInBytes = 0;
		if(DataObject->DSBuffer)
		{
				if(type == SOUND_TP_BYTES)
				{
					PosInBytes = pos;
				}
				else if(type == SOUND_TP_SEC)
				{
					PosInBytes = pos * DataObject->InfoObject->BytesPerSec;
				}
				else if(type == SOUND_TP_MLS)
				{
					PosInBytes = pos * (DataObject->InfoObject->BytesPerSec / 1000);
				}

				if(IsStreaming)
				{
					WORD HowCountRePlay = 0;
						for(WORD i=0;i <= CountRePlayEnd;i++)
						{
								if((i * MaxSize <= PosInBytes && (i+1) * MaxSize >= PosInBytes) || i * MaxSize == PosInBytes)
								{
									HowCountRePlay = i;
									break;
								}
						}

						//wav
						if(this->DataObject->HowFormatFile == SOUND_WAV_FORMAT)
							fseek(DataObject->File, sizeof(WaveHeader) + (HowCountRePlay  * MaxSize), SEEK_SET);
						//ogg
						else if(this->DataObject->HowFormatFile == SOUND_OGG_FORMAT)
							ov_pcm_seek(DataObject->VorbisFile,(HowCountRePlay  * MaxSize)/(2 * DataObject->InfoObject->CountChannels));

					DWORD SizeCountRePlay = PosInBytes - (MaxSize * HowCountRePlay);

					ReLoadSplit(0 ,MaxSize);

					HowRePlay = HowCountRePlay;
					DataObject->DSBuffer->SetCurrentPosition(SizeCountRePlay);

						for(WORD i=0;i<4;i++)
						{
								if(SizeCountRePlay >= SizeSplit1 * i && SizeCountRePlay < SizeSplit1 * (i+1))
								{
									ActiveSplit = i+1;
									break;
								}
						}

						if(ActiveSplit > 1)
							IsStarting = false;
						else
							IsStarting = true;

						if(ActiveSplit - 2 > 0 && ActiveSplit - 2 < 5)
						{
								for(WORD i=0;i<ActiveSplit-2;i++)
									ReLoadSplit(SizeSplit1*i ,SizeSplit1);
						}

					BF1 = false;BF2 = false;BF3 = false;BF4 = false;
				}
				else
				{
					DataObject->DSBuffer->SetCurrentPosition(PosInBytes);
				}
		}
}

inline DWORD SXSoundObject::GetCurrentPos(int type)
{
	DWORD pos,Bytes,Pos = 0;
		if(DataObject->DSBuffer)
		{
			DataObject->DSBuffer->GetCurrentPosition(&pos,0);
				if(IsStreaming)
					Bytes =  HowRePlay * MaxSize + pos;
				else
					Bytes = pos;

				if(type == SOUND_TP_BYTES)
				{
					Pos = Bytes;
				}
				else if(type == SOUND_TP_SEC)
				{
					Pos = Bytes / DataObject->InfoObject->BytesPerSec;
				}
				else if(type == SOUND_TP_MLS)
				{
					Pos = Bytes / (DataObject->InfoObject->BytesPerSec / 1000);
				}
		}
	return Pos;
}

inline void SXSoundObject::SetVolume(long volume,int type)
{
	long Volume = 0;
		if(DataObject->DSBuffer)
		{
				if(type == SOUND_TV_DB)
				{
					Volume = volume;
				}
				else if(type == SOUND_TV_PCT)
				{
					Volume = -10000+(volume * 100);
				}
				
				if(Is3d)
					VolumeSound = Volume;
				else
					DataObject->DSBuffer->SetVolume(Volume);
		}
}

inline long SXSoundObject::GetVolume(int type)
{
	long volume = 0;
		if(DataObject->DSBuffer)
		{
				if(type == SOUND_TV_DB)
				{
						if(Is3d)
							volume = VolumeSound;
						else
							DataObject->DSBuffer->GetVolume(&volume);
				}
				else if(type == SOUND_TV_PCT)
				{
						if(Is3d)
						{
							double one_percent = 100.0 / (-10000.0);
							volume = double(VolumeSound) * one_percent;
						}
						else
						{
							double one_percent = 100.0 / (-10000.0);
							DataObject->DSBuffer->GetVolume(&volume);
							volume = double(volume) * one_percent;
						}
				}
		}
	return volume;
}

inline void SXSoundObject::SetPan(long value,int type)
{
	long Value;
		if(DataObject->DSBuffer)
		{
				if(type == SOUND_TV_DB || value == 0)
				{
					Value = value;
				}
				else if(type == SOUND_TV_PCT && value != 0)
				{
					Value = (value > 0)?(10000-(value * 100))-10000:10000-(10000+(value * 100));
				}
			DataObject->DSBuffer->SetPan(Value);
		}
}

inline long SXSoundObject::GetPan(int type)
{
	long value = 0;
		if(DataObject->DSBuffer)
		{
				if(type == SOUND_TV_DB)
				{
					DataObject->DSBuffer->GetPan(&value);
				}
				else
				{
					double coef_percent = 100.0 / (10000.0);
					DataObject->DSBuffer->GetPan(&value);
					value = double(value) * coef_percent;
				}
		}
	return value;
}

inline void SXSoundObject::SetFrequency(long value)
{
		if(DataObject->DSBuffer)
			DataObject->DSBuffer->SetFrequency(value);
}

inline DWORD SXSoundObject::GetFrequency()
{
	DWORD value = 0;
		if(DataObject->DSBuffer)
			DataObject->DSBuffer->GetFrequency(&value);
	return value;
}

inline DWORD SXSoundObject::GetFrequencyOriginal()
{
		if(DataObject->DSBuffer)
			return BeginFrec;
			//return DataObject->InfoObject->BitsPerSample;
}

inline void SXSoundObject::ReLoadSplit(DWORD Pos, DWORD Size)
{
		if(DataObject->DSBuffer)
		{
				//wav
				if(this->DataObject->HowFormatFile == SOUND_WAV_FORMAT)
					SoundControl::LoadSoundDataWAV(DataObject->DSBuffer, Pos, DataObject->File, Size,0);
				//ogg
				else if(this->DataObject->HowFormatFile == SOUND_OGG_FORMAT)
				{
					bool err = SoundControl::LoadSoundDataOGG(DataObject->VorbisFile,DataObject->DSBuffer, Pos, Size,0);
				}
		}
}

inline bool SXSoundObject::Playing()
{
	/*DWORD status;
		if(DataObject->DSBuffer)
			DataObject->DSBuffer->GetStatus(&status);

	return (status == DSBSTATUS_PLAYING || status == 5)?true:false;*/
	return State;
}


SXSoundObject::~SXSoundObject()
{
	DataObject->DSBuffer->Release();
	ov_clear(DataObject->VorbisFile);
	delete DataObject;
}


void SXSoundObject::InitEffectBool()
{
	memset(IsInitEffect, 0, sizeof(bool)*9);
}

void SXSoundObject::ReInitEffect()
{
		if(IsInitEffect[SOUND_EFFECT_GARGLE])
		{
			IsInitEffect[SOUND_EFFECT_GARGLE] = false;
			SetEffectGargle(SXGargle.dwRateHz,SXGargle.dwWaveShape);
		}

		if(IsInitEffect[SOUND_EFFECT_CHORUS])
		{
			IsInitEffect[SOUND_EFFECT_CHORUS] = false;
			SetEffectChorus(SXChorus.fWetDryMix,SXChorus.fDepth,SXChorus.fFeedback,SXChorus.fFrequency,SXChorus.lWaveform,SXChorus.fDelay,SXChorus.lPhase);
		}

		if(IsInitEffect[SOUND_EFFECT_FLANDER])
		{
			IsInitEffect[SOUND_EFFECT_FLANDER] = false;
			SetEffectFlanger(SXFlanger.fWetDryMix,SXFlanger.fDepth,SXFlanger.fFeedback,SXFlanger.fFrequency,SXFlanger.lWaveform,SXFlanger.fDelay,SXFlanger.lPhase);
		}

		if(IsInitEffect[SOUND_EFFECT_ECHO])
		{
			IsInitEffect[SOUND_EFFECT_ECHO] = false; 
			SetEffectEcho(SXEcho.fWetDryMix,SXEcho.fFeedback,SXEcho.fLeftDelay,SXEcho.fRightDelay,SXEcho.lPanDelay);
		}

		if(IsInitEffect[SOUND_EFFECT_DISTORTION])
		{
			IsInitEffect[SOUND_EFFECT_DISTORTION] = false;
			SetEffectDistortion(SXDistortion.fGain,SXDistortion.fEdge,SXDistortion.fPostEQCenterFrequency,SXDistortion.fPostEQBandwidth,SXDistortion.fPreLowpassCutoff);
		}

		if(IsInitEffect[SOUND_EFFECT_COMPRESSOR])
		{
			IsInitEffect[SOUND_EFFECT_COMPRESSOR] = false;
			SetEffectCompressor(SXCompressor.fGain,SXCompressor.fAttack,SXCompressor.fRelease,SXCompressor.fThreshold,SXCompressor.fRatio,SXCompressor.fPredelay);
		}

		if(IsInitEffect[SOUND_EFFECT_PARAMEQ])
		{
			IsInitEffect[SOUND_EFFECT_PARAMEQ] = false;
			SetEffectParameq(SXParamEq.fCenter,SXParamEq.fBandwidth,SXParamEq.fGain);
		}

		if(IsInitEffect[SOUND_EFFECT_I3DL2REVERB])
		{
			IsInitEffect[SOUND_EFFECT_I3DL2REVERB] = false;
			SetEffectI3DL2Reverb(	SXI3DL2Reverb.lRoom,SXI3DL2Reverb.lRoomHF,SXI3DL2Reverb.flRoomRolloffFactor,SXI3DL2Reverb.flDecayTime,SXI3DL2Reverb.flDecayHFRatio,
									SXI3DL2Reverb.lReflections,SXI3DL2Reverb.flReflectionsDelay,SXI3DL2Reverb.lReverb,SXI3DL2Reverb.flReverbDelay,
									SXI3DL2Reverb.flDiffusion,SXI3DL2Reverb.flDensity,SXI3DL2Reverb.flHFReference);
		}

		if(IsInitEffect[SOUND_EFFECT_WAVESREVERB])
		{
			IsInitEffect[SOUND_EFFECT_WAVESREVERB] = false;
			SetEffectWavesReverb(SXWavesReverb.fInGain,SXWavesReverb.fReverbMix,SXWavesReverb.fReverbTime,SXWavesReverb.fHighFreqRTRatio);
		}
}

int SXSoundObject::PrecondInitEffect(HRESULT hr)
{
		if(FAILED(hr))
		{
				if(hr == DSERR_CONTROLUNAVAIL)
					return SOUND_ERR_CONTROLUNAVAIL;
				else if(hr == DSERR_GENERIC)
					return SOUND_ERR_GENERIC;
				else if(hr == DSERR_INVALIDCALL)
					return SOUND_ERR_INVALIDCALL;
				else if(hr == DSERR_INVALIDPARAM)
					return SOUND_ERR_INVALIDPARAM;
				else if(hr == DSERR_PRIOLEVELNEEDED)
					return SOUND_ERR_PRIOLEVELNEEDED;
				else if(hr == DSERR_NOINTERFACE)
					return SOUND_ERR_NOINTERFACE;
				else
					return SOUND_ERR_UNCPECIFIED;
		}
		else
			return SOUND_OK;
}



int SXSoundObject::EffectIsOn(int effect)
{
		if(effect >=0 && effect <=8)
			return (IsInitEffect[effect]?SOUND_EFFECT_ON:SOUND_EFFECT_OFF);
		else
			return SOUND_EFFECT_INVALID_KEY;
}

GUID SXSoundObject::GetGuidEffect(int effect)
{
		if(effect == SOUND_EFFECT_GARGLE)
			return GUID_DSFX_STANDARD_GARGLE;
		else if(effect == SOUND_EFFECT_CHORUS)
			return GUID_DSFX_STANDARD_CHORUS;
		else if(effect == SOUND_EFFECT_FLANDER)
			return GUID_DSFX_STANDARD_FLANGER;
		else if(effect == SOUND_EFFECT_ECHO)
			return GUID_DSFX_STANDARD_ECHO;
		else if(effect == SOUND_EFFECT_DISTORTION)
			return GUID_DSFX_STANDARD_DISTORTION;
		else if(effect == SOUND_EFFECT_COMPRESSOR)
			return GUID_DSFX_STANDARD_COMPRESSOR;
		else if(effect == SOUND_EFFECT_PARAMEQ)
			return GUID_DSFX_STANDARD_PARAMEQ;
		else if(effect == SOUND_EFFECT_I3DL2REVERB)
			return GUID_DSFX_STANDARD_I3DL2REVERB;
		else if(effect == SOUND_EFFECT_WAVESREVERB)
			return GUID_DSFX_WAVES_REVERB;
}

int SXSoundObject::EffectOnOff(int effect,int switch_)
{
		if(DataObject->DSBuffer)
			return SOUND_ERR_NULL_BUFFER;

	bool IsPlaying = Playing();
	int info = SOUND_OK;
	HRESULT hr = 0;

		if(IsPlaying)
			Stop();

		//включаем эффект
		if(switch_ == SOUND_EFFECT_ON)
		{
			DWORD arr[1];
			DSEFFECTDESC dsEffect;

			memset(&dsEffect, 0, sizeof(DSEFFECTDESC));
			dsEffect.dwSize		= sizeof(DSEFFECTDESC);
			dsEffect.dwFlags	= 0;
			dsEffect.guidDSFXClass = GetGuidEffect(effect);

			hr = DataObject->DSBuffer->SetFX(1,&dsEffect,arr);

			info = PrecondInitEffect(hr);
			IsInitEffect[effect] = true;
		}

		//выключаем эффект
		else if(switch_ == SOUND_EFFECT_OFF)
		{
			hr = DataObject->DSBuffer->SetFX(0,0,0);

			info = PrecondInitEffect(hr);

			IsInitEffect[effect] = false;
			ReInitEffect();
		}

		if(IsPlaying)
			Play();
	return info;
}

///////////////////////////////////////////////////////////////

int SXSoundObject::SetEffectGargle(DWORD RateHz,DWORD WaveShape)
{
	IDirectSoundFXGargle8 *gargle = 0;
	int info = SOUND_OK;

		if(!IsInitEffect[SOUND_EFFECT_GARGLE])
			info = EffectOnOff(SOUND_EFFECT_GARGLE,SOUND_EFFECT_ON);

		if(info != SOUND_OK)
			return info;

	DataObject->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_GARGLE,0,IID_IDirectSoundFXGargle8,(void**)&gargle);

	DSFXGargle SXGargle;

	gargle->GetAllParameters(&SXGargle);

	SXGargle.dwRateHz		= RateHz;
	SXGargle.dwWaveShape	= WaveShape;

	HRESULT hr = gargle->SetAllParameters(&SXGargle);
		if(FAILED(hr))
			info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;
}

int SXSoundObject::SetEffectChorus(float WetDryMix,float Depth,float Feedback,float Frequency,long Waveform,float Delay,long Phase)
{
	IDirectSoundFXChorus8 *chorus = 0;
	int info = SOUND_OK;

		if(!IsInitEffect[SOUND_EFFECT_CHORUS])
			info = EffectOnOff(SOUND_EFFECT_CHORUS,SOUND_EFFECT_ON);

		if(info != SOUND_OK)
			return info;

	DataObject->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_CHORUS,0,IID_IDirectSoundFXChorus8,(void**)&chorus);

	DSFXChorus SXChorus;

	chorus->GetAllParameters(&SXChorus);

	SXChorus.fDelay		= Delay;
	SXChorus.fDepth		= Depth;
	SXChorus.fFeedback	= Feedback;
	SXChorus.fFrequency	= Frequency;
	SXChorus.lWaveform	= Waveform;
	SXChorus.fWetDryMix	= WetDryMix;
	SXChorus.lPhase		= Phase;

	HRESULT hr = chorus->SetAllParameters(&SXChorus);
		if(FAILED(hr))
			info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;
}

int SXSoundObject::SetEffectFlanger(float WetDryMix,float Depth,float Feedback,float Frequency,long Waveform,float Delay,long Phase)
{
	IDirectSoundFXFlanger8 *flanger = 0;
	int info = SOUND_OK;

		if(!IsInitEffect[SOUND_EFFECT_FLANDER])
			info = EffectOnOff(SOUND_EFFECT_FLANDER,SOUND_EFFECT_ON);

		if(info != SOUND_OK)
			return info;

	DataObject->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_FLANGER,0,IID_IDirectSoundFXFlanger8,(void**)&flanger);

	DSFXFlanger SXFlanger;

	flanger->GetAllParameters(&SXFlanger);

	SXFlanger.fDelay		= Delay;
	SXFlanger.fDepth		= Depth;
	SXFlanger.fFeedback		= Feedback;
	SXFlanger.fFrequency	= Frequency;
	SXFlanger.lWaveform		= Waveform;
	SXFlanger.fWetDryMix	= WetDryMix;
	SXFlanger.lPhase		= Phase;

	HRESULT hr = flanger->SetAllParameters(&SXFlanger);

		if(FAILED(hr))
			info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;
}

int SXSoundObject::SetEffectEcho(float WetDryMix,float Feedback,float LeftDelay,float RightDelay, long PanDelay)
{
	IDirectSoundFXEcho8 *echo = 0;
	int info = SOUND_OK;

		if(!IsInitEffect[SOUND_EFFECT_ECHO])
			info = EffectOnOff(SOUND_EFFECT_ECHO,SOUND_EFFECT_ON);

		if(info != SOUND_OK)
			return info;

	DataObject->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_ECHO,0,IID_IDirectSoundFXEcho8,(void**)&echo);

	DSFXEcho SXEcho;

	echo->GetAllParameters(&SXEcho);

	SXEcho.fWetDryMix	= WetDryMix;
	SXEcho.fFeedback	= Feedback;
	SXEcho.fRightDelay	= RightDelay;
	SXEcho.fLeftDelay	= LeftDelay;
	SXEcho.lPanDelay	= PanDelay;

	HRESULT hr = echo->SetAllParameters(&SXEcho);

		if(FAILED(hr))
			info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;
}

int SXSoundObject::SetEffectDistortion(float Gain,float Edge,float PostEQCenterFrequency,float PostEQBandwidth,float PreLowpassCutoff)
{
	IDirectSoundFXDistortion8 *distortion = 0;
	int info = SOUND_OK;

		if(!IsInitEffect[SOUND_EFFECT_DISTORTION])
			info = EffectOnOff(SOUND_EFFECT_DISTORTION,SOUND_EFFECT_ON);

		if(info != SOUND_OK)
			return info;

	DataObject->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_ECHO,0,IID_IDirectSoundFXDistortion8,(void**)&distortion);

	DSFXDistortion SXDistortion;

	distortion->GetAllParameters(&SXDistortion);

	SXDistortion.fEdge					= Edge;
	SXDistortion.fGain					= Gain;
	SXDistortion.fPostEQBandwidth		= PostEQBandwidth;
	SXDistortion.fPostEQCenterFrequency	= PostEQCenterFrequency;
	SXDistortion.fPreLowpassCutoff		= PreLowpassCutoff;

	HRESULT hr = distortion->SetAllParameters(&SXDistortion);

		if(FAILED(hr))
			info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;
}

int SXSoundObject::SetEffectCompressor(float Gain,float Attack,float Release,float Threshold,float Ratio,float Predelay)
{
	IDirectSoundFXCompressor8 *compressor = 0;
	int info = SOUND_OK;

		if(!IsInitEffect[SOUND_EFFECT_COMPRESSOR])
			info = EffectOnOff(SOUND_EFFECT_COMPRESSOR,SOUND_EFFECT_ON);

		if(info != SOUND_OK)
			return info;

	DataObject->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_COMPRESSOR,0,IID_IDirectSoundFXCompressor8,(void**)&compressor);

	DSFXCompressor SXCompressor;

	compressor->GetAllParameters(&SXCompressor);

	SXCompressor.fAttack	= Attack;
	SXCompressor.fGain		= Gain;
	SXCompressor.fPredelay	= Predelay;
	SXCompressor.fRatio		= Ratio;
	SXCompressor.fThreshold	= Threshold;
	SXCompressor.fRelease	= Release;

	HRESULT hr = compressor->SetAllParameters(&SXCompressor);

		if(FAILED(hr))
			info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;
}

int SXSoundObject::SetEffectParameq(float Center,float Bandwidth,float Gain)
{
	IDirectSoundFXParamEq8 *parameq = 0;
	int info = SOUND_OK;

		if(!IsInitEffect[SOUND_EFFECT_PARAMEQ])
			info = EffectOnOff(SOUND_EFFECT_PARAMEQ,SOUND_EFFECT_ON);

		if(info != SOUND_OK)
			return info;

	DataObject->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_PARAMEQ,0,IID_IDirectSoundFXParamEq8,(void**)&parameq);

	DSFXParamEq SXParamEq;

	parameq->GetAllParameters(&SXParamEq);

	SXParamEq.fBandwidth	= Bandwidth;
	SXParamEq.fCenter		= Center;
	SXParamEq.fGain			= Gain;

	HRESULT hr = parameq->SetAllParameters(&SXParamEq);

		if(FAILED(hr))
			info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;
}

int SXSoundObject::SetEffectI3DL2Reverb	(long Room,long RoomHF,float RoomRolloffFactor,float DecayTime,float DecayHFRatio,long Reflections,
										float ReflectionsDelay,long Reverb,float ReverbDelay,float Diffusion,float Density,float HFReference)
{
	IDirectSoundFXI3DL2Reverb8 *I3DL2Reverb8 = 0;
	int info = SOUND_OK;

		if(!IsInitEffect[SOUND_EFFECT_I3DL2REVERB])
			info = EffectOnOff(SOUND_EFFECT_I3DL2REVERB,SOUND_EFFECT_ON);

		if(info != SOUND_OK)
			return info;

	DataObject->DSBuffer->GetObjectInPath(GUID_DSFX_STANDARD_I3DL2REVERB,0,IID_IDirectSoundFXI3DL2Reverb8,(void**)&I3DL2Reverb8);

	DSFXI3DL2Reverb SXI3DL2Reverb;

	I3DL2Reverb8->GetAllParameters(&SXI3DL2Reverb);

	SXI3DL2Reverb.lRoom					= Room;
	SXI3DL2Reverb.lRoomHF				= RoomHF;
	SXI3DL2Reverb.flRoomRolloffFactor	= RoomRolloffFactor;
	SXI3DL2Reverb.flDecayTime			= DecayTime;
	SXI3DL2Reverb.flDecayHFRatio		= DecayHFRatio;
	SXI3DL2Reverb.lReflections			= Reflections;
	SXI3DL2Reverb.flReflectionsDelay	= ReflectionsDelay;
	SXI3DL2Reverb.lReverb				= Reverb;
	SXI3DL2Reverb.flReverbDelay			= ReverbDelay;
	SXI3DL2Reverb.flDiffusion			= Diffusion;
	SXI3DL2Reverb.flDensity				= Density;
	SXI3DL2Reverb.flHFReference			= HFReference;

	HRESULT hr = I3DL2Reverb8->SetAllParameters(&SXI3DL2Reverb);

		if(FAILED(hr))
			info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;
}

int SXSoundObject::SetEffectWavesReverb(float InGain,float ReverbMix,float ReverbTime,float HighFreqRTRatio)
{
	IDirectSoundFXWavesReverb8 *wavereverb = 0;
	int info = SOUND_OK;

		if(!IsInitEffect[SOUND_EFFECT_WAVESREVERB])
			info = EffectOnOff(SOUND_EFFECT_WAVESREVERB,SOUND_EFFECT_ON);

		if(info != SOUND_OK)
			return info;

	DataObject->DSBuffer->GetObjectInPath(GUID_DSFX_WAVES_REVERB,0,IID_IDirectSoundFXWavesReverb8,(void**)&wavereverb);

	DSFXWavesReverb SXWavesReverb;

	wavereverb->GetAllParameters(&SXWavesReverb);

	SXWavesReverb.fHighFreqRTRatio	= HighFreqRTRatio;
	SXWavesReverb.fInGain			= InGain;
	SXWavesReverb.fReverbMix		= ReverbMix;
	SXWavesReverb.fReverbTime		= ReverbTime;

	HRESULT hr = wavereverb->SetAllParameters(&SXWavesReverb);

		if(FAILED(hr))
			info = SOUND_ERR_EFFECT_NON_SETUP;
	return info;
}

inline float SXSoundObject::GetCountSec()
{
	return DataObject->InfoObject->CountSec;
}

inline DWORD SXSoundObject::GetBytesPerSec()
{
	return DataObject->InfoObject->BytesPerSec;
}

inline DWORD SXSoundObject::GetSize()
{
	return DataObject->InfoObject->DataSize;
}

inline void SXSoundObject::GetPathFile(char* path)
{
		if(path)
			sprintf(path,"%s",this->File);
}

void SXSoundObject::Update3d(float3 *CamPos,float3 *CamLook)
{
		if(Is3d && DataObject->DSBuffer)
		{
			float Dist = SMVector3Distance(Position,*CamPos);
			long tmp_volume = (Dist/MaxDist)*(-10000);

				if(tmp_volume > 0)
					tmp_volume = 0;

				if(tmp_volume < -10000)
					tmp_volume = -10000;

			DataObject->DSBuffer->SetVolume(tmp_volume);

			float3 vec = (*CamPos) + (*CamLook);
			
			float str = (Position.x - CamPos->x)*(Position.z - vec.z)-(Position.z - CamPos->z)*(Position.x - vec.x);
			DataObject->DSBuffer->SetPan((str/Dist) * ShiftPan * (-10000));
		}
}

inline DWORD SXSoundObject::GetID()
{
	return ID;
}

/*inline float SXSoundObject::GetMinDist()
{
	return MinDist;
}

inline void SXSoundObject::SetMinDist(float value)
{
	MinDist = value;
}*/

inline float SXSoundObject::GetMaxDist()
{
	return MaxDist;
}

inline void SXSoundObject::SetMaxDist(float value)
{
	MaxDist = value;
}