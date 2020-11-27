
#include "AudioCodecWave.h"

//##########################################################################

CAudioCodecWave::CAudioCodecWave(IFileSystem *pFileSystem)
{
	m_pFileSystem = pFileSystem;
	m_aExts.push_back("wav");
}

bool XMETHODCALLTYPE CAudioCodecWave::open(const char *szFile, const char *szArg, IXAudioCodecTarget **ppTarget, bool forSave)
{
	IFile *pFile = m_pFileSystem->openFile(szFile, (forSave ? FILE_MODE_WRITE : FILE_MODE_READ));

	if (!pFile || !ppTarget && (forSave && (!szArg && strcasecmp(szArg, getFormat()) != 0)))
		return false;

	AudioRawDesc oDesc;
	CWaveHeader oHeader;

	if(!forSave)
	{
		size_t uCurrPos = pFile->getPos();
		pFile->setPos(0);
		pFile->readBin(&oHeader, sizeof(CWaveHeader));
		pFile->setPos(uCurrPos);

		bool can =
				memcmp(oHeader.aRiff, "RIFF", 4) == 0 &&
				memcmp(oHeader.aFormat, "WAVE", 4) == 0 &&
				memcmp(oHeader.aFormatChunkId, "fmt ", 4) == 0 &&
				(oHeader.i16FormatCode == WAVE_FORMAT_PCM_INT || oHeader.i16FormatCode == WAVE_FORMAT_PCM_FLOAT);

		if(!can)
		{
			mem_release(pFile);
			return false;
		}

		if(can)
		{
			AUDIO_SAMPLE_TYPE typeSample = AUDIO_SAMPLE_TYPE_INT;
			if(oHeader.i16FormatCode == WAVE_FORMAT_PCM_FLOAT)
				typeSample = AUDIO_SAMPLE_TYPE_FLOAT;
			oDesc.fmtSample = AudioGetSampleFmt(typeSample, oHeader.i16BitsPerSample);
			oDesc.u8Channels = oHeader.i16Channels;
			oDesc.uSampleRate = oHeader.iSampleRate;
			oDesc.uSize = oHeader.uDataChunkSize;
			oDesc.calc();
		}
	}

	if(ppTarget)
	{
		CAudioCodecTargetWave *pTarget = new CAudioCodecTargetWave();
		pTarget->init(pFile, &oHeader, &oDesc, forSave);
		*ppTarget = pTarget;
	}

	return true;
}

bool XMETHODCALLTYPE CAudioCodecWave::canSave(UINT uIndex) const
{
	return true;
}

const char* XMETHODCALLTYPE CAudioCodecWave::getFormat() const
{
	return "wav-pcm";
}

const char* XMETHODCALLTYPE CAudioCodecWave::getExt(UINT uIndex) const
{
	if(uIndex >= 0 && uIndex < m_aExts.size())
		return m_aExts[uIndex].c_str();

	return NULL;
}

UINT XMETHODCALLTYPE CAudioCodecWave::getExtCount() const
{
	return m_aExts.size();
}

//##########################################################################

CAudioCodecTargetWave::~CAudioCodecTargetWave()
{
	mem_release(m_pFile);
}

//**************************************************************************

void CAudioCodecTargetWave::init(IFile *pFile, CWaveHeader *pHeader, AudioRawDesc *pDesc, bool forSave)
{
	m_pFile = pFile;

	if(pHeader)
		m_oHeader = *pHeader;

	if(pDesc)
		m_oDesc = *pDesc;

	m_forSave = forSave;
}

//**************************************************************************

void CAudioCodecTargetWave::getDesc(AudioRawDesc *pDesc) const
{
	if(!pDesc || !m_pFile || m_forSave)
		return;

	*pDesc = m_oDesc;
}

//**************************************************************************

int64_t XMETHODCALLTYPE CAudioCodecTargetWave::getPos() const
{
	if(!m_pFile || m_forSave)
		return 0;

	return m_pFile->getPos() - sizeof(CWaveHeader);
}

//**************************************************************************

void XMETHODCALLTYPE CAudioCodecTargetWave::setPos(int64_t iPos)
{
	if(!m_pFile || m_forSave)
		return;

	m_pFile->setPos(sizeof(CWaveHeader) + iPos);
}

//**************************************************************************

size_t XMETHODCALLTYPE CAudioCodecTargetWave::decode(int64_t iPos, uint64_t uLen, void **ppData)
{
	if (!ppData || !(*ppData) || uLen == 0 || m_forSave)
		return 0;

	setPos(iPos);

	size_t sizeRead = m_pFile->readBin(*ppData, uLen);

	return sizeRead;
}

//**************************************************************************

bool XMETHODCALLTYPE CAudioCodecTargetWave::encode(IXBuffer *pBufferPCM, AudioRawDesc *pOutDesc)
{
	if (!pBufferPCM || !pOutDesc)
		return false;

	m_pFile->setPos(0);

	CWaveHeader oOutHeader;
	oOutHeader.iSampleRate = pOutDesc->uSampleRate;
	oOutHeader.uDataChunkSize = pBufferPCM->size();
	oOutHeader.i16Channels = pOutDesc->u8Channels;
	oOutHeader.iBytesPerSec = pOutDesc->uBytesPerSec;
	oOutHeader.i16BlockAlign = pOutDesc->u8BlockAlign;
	oOutHeader.i16BitsPerSample = AudioGetFormatBytes(pOutDesc->fmtSample) * 8;

	memcpy(oOutHeader.aRiff, "RIFF", 4);
	memcpy(oOutHeader.aFormat, "WAVE", 4);
	memcpy(oOutHeader.aFormatChunkId, "fmt ", 4);
	memcpy(oOutHeader.aData, "data", 4);
	oOutHeader.i16FormatCode = (pOutDesc->fmtSample > AUDIO_SAMPLE_FMT_SINT32 ? WAVE_FORMAT_PCM_FLOAT : WAVE_FORMAT_PCM_INT);
	oOutHeader.uFormatChunkSize = 16;

	m_pFile->writeBin(&oOutHeader, sizeof(oOutHeader));
	m_pFile->writeBin(pBufferPCM->get(), pBufferPCM->size());

	return true;
}

