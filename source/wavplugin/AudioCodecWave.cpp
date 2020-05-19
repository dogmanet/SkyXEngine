
#include "AudioCodecWave.h"

//##########################################################################

CAudioCodecWave::CAudioCodecWave()
{
	m_aExts.push_back("wav");
}

bool XMETHODCALLTYPE CAudioCodecWave::open(const char *szFile, const char *szArg, IXAudioCodecTarget **ppTarget, bool forSave)
{
	FILE *pFile = fopen(szFile, (forSave ? "wb" : "rb"));

	if (!pFile || !ppTarget)
		return false;

	AudioRawDesc oDesc;
	CWaveHeader oHeader;

	if(!forSave)
	{
		uint32_t uCurrPos = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		fread(&oHeader, 1, sizeof(CWaveHeader), pFile);
		fseek(pFile, 0, uCurrPos);

		bool can =
				memcmp(oHeader.aRiff, "RIFF", 4) == 0 &&
				memcmp(oHeader.aFormat, "WAVE", 4) == 0 &&
				memcmp(oHeader.aFormatChunkId, "fmt ", 4) == 0 &&
				(oHeader.i16FormatCode == WAVE_FORMAT_PCM_INT || oHeader.i16FormatCode == WAVE_FORMAT_PCM_FLOAT);

		if(!can)
		{
			fclose(pFile);
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
	fclose(m_pFile);
}

//**************************************************************************

void CAudioCodecTargetWave::init(FILE *pFile, CWaveHeader *pHeader, AudioRawDesc *pDesc, bool forSave)
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

	return ftell(m_pFile) - sizeof(CWaveHeader);
}

//**************************************************************************

void XMETHODCALLTYPE CAudioCodecTargetWave::setPos(int64_t iPos)
{
	if(!m_pFile || m_forSave)
		return;

	fseek(m_pFile, sizeof(CWaveHeader) + iPos, SEEK_SET);
}

//**************************************************************************

size_t XMETHODCALLTYPE CAudioCodecTargetWave::decode(int64_t iPos, uint64_t uLen, void **ppData)
{
	if (!ppData || !(*ppData) || uLen == 0 || m_forSave)
		return 0;

	setPos(iPos);

	size_t sizeRead = fread(*ppData, 1, uLen, m_pFile);

	return sizeRead;
}

//**************************************************************************

bool XMETHODCALLTYPE CAudioCodecTargetWave::encode(IXBuffer *pBufferPCM, AudioRawDesc *pOutDesc)
{
	if (!pBufferPCM || !pOutDesc)
		return false;

	fseek(m_pFile, 0, SEEK_SET);

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

	fwrite(&oOutHeader, sizeof(oOutHeader), 1, m_pFile);
	fwrite(pBufferPCM->get(), 1, pBufferPCM->size(), m_pFile);

	return true;
}

