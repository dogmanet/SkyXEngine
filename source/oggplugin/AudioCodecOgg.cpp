
#include "AudioCodecOgg.h"

//##########################################################################

size_t OggCallbackRead(void *pDataDest, size_t size, size_t nmemb, void *pDataFile)
{
	IFile *pFile = (IFile*)pDataFile;
	return pFile->readBin(pDataDest, size * nmemb);
}

//**************************************************************************

int OggCallbackClose(void *pDataFile)
{
	IFile *pFile = (IFile*)pDataFile;
	//эту функци. вызывает ov_clear, но надо чтобы владалец обьекта сам закрывал файл
	//mem_release(pFile);
	return 0;
}

//**************************************************************************

int OggCallbackSeek(void *pDataFile, ogg_int64_t offset, int whence)
{
	IFile *pFile = (IFile*)pDataFile;
	switch (whence)
	{
	case SEEK_SET:
	{
		if (offset < 0 || offset > pFile->getSize())
			return 1;
		pFile->setPos(offset);
		return 0;
	}
	case SEEK_CUR:
	{
		size_t uPos = pFile->getPos() + offset;
		if (uPos < 0 || uPos > pFile->getSize())
			return 1;

		pFile->setPos(uPos);
		return 0;
	}
	case SEEK_END:
	{
		size_t uPos = pFile->getSize() + offset;
		if (uPos < 0 || uPos > pFile->getSize())
			return 1;

		pFile->setPos(uPos);
		return 0;
	}
	default: 
		return -1;
	}
	return 1;
}

//**************************************************************************

long OggCallbackTell(void *pDataFile)
{
	IFile *pFile = (IFile*)pDataFile;
	return pFile->getPos();
}

//##########################################################################
//##########################################################################
//##########################################################################

CAudioCodecOgg::CAudioCodecOgg(IFileSystem *pFileSystem)
{
	m_pFileSystem = pFileSystem;
	m_oCB.close_func = OggCallbackClose;
	m_oCB.read_func = OggCallbackRead;
	m_oCB.seek_func = OggCallbackSeek;
	m_oCB.tell_func = OggCallbackTell;

	m_aExts.push_back("ogg");
}

//##########################################################################

const char* XMETHODCALLTYPE CAudioCodecOgg::getExt(UINT uIndex) const
{
	if(uIndex >= 0 && uIndex < m_aExts.size())
		return m_aExts[uIndex].c_str();

	return NULL;
}

UINT XMETHODCALLTYPE CAudioCodecOgg::getExtCount() const
{
	return m_aExts.size();
}

//**************************************************************************

bool XMETHODCALLTYPE CAudioCodecOgg::open(const char *szFile, const char *szArg, IXAudioCodecTarget **ppTarget, bool forSave)
{
	IFile *pFile = m_pFileSystem->openFile(szFile, (forSave ? FILE_MODE_WRITE : FILE_MODE_READ));

	if (!pFile || !ppTarget || (forSave && (!szArg && strcasecmp(szArg, getFormat()) != 0)))
		return false;

	OggVorbis_File *pVoFile = NULL;
	AudioRawDesc oDesc;

	if(!forSave)
	{
		pVoFile = new OggVorbis_File();

		int iRet = 0;
		if((iRet = ov_open_callbacks(pFile, pVoFile, 0, 0, m_oCB)) != 0)
			return false;

		vorbis_info *vi = ov_info(pVoFile, -1);

		oDesc.fmtSample = AudioGetSampleFmt(AUDIO_SAMPLE_TYPE_INT, OGG_BYTES_PER_SAMPLE * 8);
		oDesc.u8Channels = vi->channels;
		oDesc.uSampleRate = vi->rate;
		oDesc.uSize = ov_pcm_total(pVoFile, -1) * OGG_BYTES_PER_SAMPLE * vi->channels;
		oDesc.calc();
	}

	if(ppTarget)
	{
		CAudioCodecTargetOgg *pTarget = new CAudioCodecTargetOgg();
		pTarget->init(pFile, pVoFile, &oDesc, forSave);
		*ppTarget = pTarget;
	}

	return true;
}

//**************************************************************************

bool XMETHODCALLTYPE CAudioCodecOgg::canSave(UINT uIndex) const
{
	return true;
}

//**************************************************************************

const char* XMETHODCALLTYPE CAudioCodecOgg::getFormat() const
{
	return "ogg-vorbis";
}

//##########################################################################

CAudioCodecTargetOgg::~CAudioCodecTargetOgg()
{
	if(m_pVoFile)
	{
		ov_clear(m_pVoFile);
		mem_delete(m_pVoFile);
	}

	mem_release(m_pFile);
}

//**************************************************************************

void CAudioCodecTargetOgg::init(IFile *pFile, OggVorbis_File *pVoFile, AudioRawDesc *pDesc, bool forSave)
{
	m_pFile = pFile;
	m_pVoFile = pVoFile;
	if(pDesc)
		m_oDesc = *pDesc;
	m_forSave = forSave;
}

//**************************************************************************

void CAudioCodecTargetOgg::getDesc(AudioRawDesc *pDesc) const
{
	if(!pDesc || !m_pFile || !m_pVoFile)
		return;

	*pDesc = m_oDesc;
}

//**************************************************************************

int64_t XMETHODCALLTYPE CAudioCodecTargetOgg::getPos() const
{
	if (!m_pFile || !m_pVoFile)
		return 0;

	return ov_pcm_tell(m_pVoFile);
}

//**************************************************************************

void XMETHODCALLTYPE CAudioCodecTargetOgg::setPos(int64_t iPos)
{
	if (!m_pFile || !m_pVoFile)
		return;

	ov_pcm_seek(m_pVoFile, iPos);
}

//**************************************************************************

size_t XMETHODCALLTYPE CAudioCodecTargetOgg::decode(int64_t iPos, uint64_t uLen, void **ppData)
{
	if (!ppData || !(*ppData) || uLen == 0 || m_forSave || !m_pVoFile)
		return 0;

	setPos(iPos);

	uint32_t uReaded = 0;
	long lCurrRead = 0;
	int iCurrSection = 0;

	while (uReaded < uLen)
	{
		lCurrRead = ov_read(m_pVoFile, ((char*)*ppData) + uReaded, uLen - uReaded, 0, OGG_BYTES_PER_SAMPLE, 1, &iCurrSection);

		//конец файла
		if (lCurrRead == 0)
			break;

		if (lCurrRead > 0)
			uReaded += lCurrRead;
		else if (lCurrRead == OV_EINVAL)
		{
			//ошибка при декодировании, нужно поставить заглушку
			//LibReport(REPORT_MSG_LEVEL_WARNING,"OV_EINVAL");
			break;
		}
		else
			break;
	}

	//printf("uReaded = %d\n", uReaded);

	return uReaded;
}

//**************************************************************************

bool XMETHODCALLTYPE CAudioCodecTargetOgg::encode(IXBuffer *pBufferPCM, AudioRawDesc *pOutDesc)
{
	if (!m_pFile || !m_forSave)
		return false;

	BYTE *pData = pBufferPCM->get();
	uint64_t uSize = pBufferPCM->size();

	//**************************************************
	// обьявление ogg/vorbis данных кодека

	//логический поток ogg
	ogg_stream_state oOggStream;

	//страница битового потока
	ogg_page oOggPage;

	//пакет сырых данных для декодирования
	ogg_packet oOggPacket;

	//статическая информация (настройки) битового потока
	vorbis_info oVoInfo;

	//комментарии
	vorbis_comment oVoComment;

	//состояние кодека https://xiph.org/vorbis/doc/libvorbis/vorbis_dsp_state.html
	vorbis_dsp_state oVoMainState;

	//блок аудиоданных https://xiph.org/vorbis/doc/libvorbis/vorbis_block.html
	vorbis_block oVoDataBlock;

	//удалось ли дойти до конца логического потока данных
	int iEndOfStream=0;

	int iRetCode = 0;

	//**************************************************
	// первичная инициализация

	//инициализация кодировщика (статических данных) с переменным битрейтом (variable bitrate)
	vorbis_info_init(&oVoInfo);
	if ((iRetCode = vorbis_encode_init_vbr(&oVoInfo, pOutDesc->u8Channels, pOutDesc->uSampleRate, 0.2)))
		return false;

	/* можно заюзать усредненный битрейт (average bitrate):
		iRetCode = vorbis_encode_init(&vi,pOutDesc->u8Channels, pOutDesc->uSampleRate,-1,128000,-1);
		но в данной реализации пусть кодировщик сам разбирается :)
	*/

	//инициализируем комментарий
	vorbis_comment_init(&oVoComment);
	vorbis_comment_add_tag(&oVoComment, "ENCODER", "SkyXEngine plugin [class 'codec_ogg']");

	vorbis_analysis_init(&oVoMainState, &oVoInfo);
	vorbis_block_init(&oVoMainState, &oVoDataBlock);

	//инициализация битового потока и присвоение ему серийного номера
	srand(time(NULL));
	ogg_stream_init(&oOggStream,rand());

	//создание и инициализация заголовков (идентификационного, комментариев, кода)
	ogg_packet oHeaderId, oHeaderComment, oHeaderCode;
	vorbis_analysis_headerout(&oVoMainState, &oVoComment, &oHeaderId, &oHeaderComment, &oHeaderCode);
	ogg_stream_packetin(&oOggStream, &oHeaderId);
	ogg_stream_packetin(&oOggStream, &oHeaderComment);
	ogg_stream_packetin(&oOggStream, &oHeaderCode);

	//**************************************************
	//сброс заголовков в логический поток данных

	while (ogg_stream_flush(&oOggStream, &oOggPage))
	{
		m_pFile->writeBin(oOggPage.header, oOggPage.header_len);
		m_pFile->writeBin(oOggPage.body, oOggPage.body_len);
	}

	//**************************************************
	// инициализация семплов

	
	//количество блоков семплов (количество каналов * байт на семпл)
	int iCountBlocks = uSize/ (pOutDesc->u8BlockAlign);

	//по сколько блоков записывать за один раз
	int iPartBlocks = 1024;

	//количество уже записанных блоков
	int iCountReadedBlocks = 0;

	/*в общем процесс записи сэмлов выглядит как показано в закомментированном коде ниже
		но так делать не надо, потому что внутри vorbis_analysis_wrote выделяется память на стеке по общему количеству блоков
		это может привести к переполнению стека
	*/
	/* но разделять на блоки и передавать таким образом данные тоже нельзя
	*/
	/* получаем выделенный массив (по количеству каналов) массивов (по количеству семплов)
		aaBuffer[iChannel][iSample]
	*/
	/*float **aaBuffer = vorbis_analysis_buffer(&oVoMainState, iCountBlocks);
	int iCurrSample = 0;

	//заполняем выделенный float массив нормализованными данными [-1.0, 1.0]
	for(int i = 0; i < iCountBlocks ; ++i)
	{
		for(int iChannels = 0; iChannels<pOutDesc->u8Channels; ++iChannels)
		{
			iCurrSample = i * pOutDesc->u8Channels + iChannels;
			int16_t i16Sample = ((int16_t*)pData)[iCurrSample];
			aaBuffer[iChannels][i] = float(i16Sample) / 32768.f;
		}
	}

	//сообщаем кодировщику что поступили данные для записи
	vorbis_analysis_wrote(&oVoMainState, iCountBlocks);*/

	//**************************************************
	// запись семплов

	//если еще не дошли до конца битового потока, тогда продолжаем запись
	while(!iEndOfStream)
	{
		//количество прочитанных семплов
		uint32_t uCountReadedSamples = (iCountReadedBlocks*pOutDesc->u8Channels);
		//количество блоков для текущей итерации
		int iCurrBlocks = iPartBlocks;
		if (iCountBlocks - iCountReadedBlocks < iPartBlocks)
			iCurrBlocks = iCountBlocks - iCountReadedBlocks;

		//если есть блоки для записи
		if (iCurrBlocks > 0)
		{
			/* получаем выделенный массив (по количеству каналов) массивов (по количеству семплов)
				aaBuffer[iChannel][iSample]
			*/
			float **aaBuffer = vorbis_analysis_buffer(&oVoMainState, iCurrBlocks);
			int iCurrSample = 0;

			//запись блоков
			for (int i = 0; i < iCurrBlocks; ++i)
			{
				for (int iChannels = 0; iChannels < pOutDesc->u8Channels; ++iChannels)
				{
					iCurrSample = uCountReadedSamples + i * pOutDesc->u8Channels + iChannels;
					int16_t i16Sample = ((int16_t*)pData)[iCurrSample];
					aaBuffer[iChannels][i] = float(i16Sample) / 32768.f;
				}
			}

			//сообщаем кодировщику что поступили данные для записи
			vorbis_analysis_wrote(&oVoMainState, iCurrBlocks);
			iCountReadedBlocks += iCurrBlocks;
		}
		else
		{
			/* сообщаем что данных больше не будет
				если не сообщить об этом, то не все данные будут записаны, не получится дойти до конца битового потока, 
				потому что запись идет постраничная и последняя неполная страница не будет записана
		 */
			vorbis_analysis_wrote(&oVoMainState, 0);
		}

		while (vorbis_analysis_blockout(&oVoMainState, &oVoDataBlock) == 1)
		{
			//поиск режима кодирования и отправка блока на кодировку
			vorbis_analysis(&oVoDataBlock, NULL);
			vorbis_bitrate_addblock(&oVoDataBlock);

			//получение следующего доступного пакета
			while (vorbis_bitrate_flushpacket(&oVoMainState, &oOggPacket))
			{
				//отправка пакета в битовый поток
				ogg_stream_packetin(&oOggStream, &oOggPacket);

				while (!iEndOfStream)
				{
					//формирование пакетов в страницы и отправка в битовый поток
					int result = ogg_stream_pageout(&oOggStream, &oOggPage);
					if (result == 0)
						break;

					m_pFile->writeBin(oOggPage.header, oOggPage.header_len);
					m_pFile->writeBin(oOggPage.body, oOggPage.body_len);

					//если все записано (находимся в конце битового потока), сообщаем о завершении
					if (ogg_page_eos(&oOggPage))
						iEndOfStream = 1;
				}
			}
		}
	}


	ogg_stream_clear(&oOggStream);
	vorbis_block_clear(&oVoDataBlock);
	vorbis_dsp_clear(&oVoMainState);
	vorbis_comment_clear(&oVoComment);
	vorbis_info_clear(&oVoInfo);

	return true;
}

