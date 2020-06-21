/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __AUDIOCODECWAV_H
#define __AUDIOCODECWAV_H

#include <xcommon/IXAudioCodec.h>
#include <common/string.h>
#include <xcommon/IFileSystem.h>
#include <core/IFile.h>

//##########################################################################

enum WAVE_FORMAT
{
	WAVE_FORMAT_PCM_INT = 1,
	WAVE_FORMAT_PCM_FLOAT = 3,
};

struct CWaveHeader
{
	char	aRiff[4];
	uint32_t	uRiffChunkSize;
	char	aFormat[4];
	char	aFormatChunkId[4];
	uint32_t	uFormatChunkSize;
	int16_t	i16FormatCode;
	int16_t	i16Channels;
	int32_t	iSampleRate;
	int32_t	iBytesPerSec;
	int16_t	i16BlockAlign;
	int16_t	i16BitsPerSample;
	char aData[4];
	uint32_t uDataChunkSize;
};

//##########################################################################

class CAudioCodecWave: public IXUnknownImplementation<IXAudioCodec>
{
public:
	CAudioCodecWave(IFileSystem *pFileSystem);

	XIMPLEMENT_VERSION(IXAUDIOCODEC_VERSION);

	virtual const char* XMETHODCALLTYPE getFormat() const override;
	virtual const char* XMETHODCALLTYPE getExt(UINT uIndex=0) const override;
	virtual UINT XMETHODCALLTYPE getExtCount() const override;
	virtual bool XMETHODCALLTYPE open(const char *szFile, const char *szArg, IXAudioCodecTarget **ppTarget, bool forSave = false) override;
	virtual bool XMETHODCALLTYPE canSave(UINT uIndex=0) const override;

protected:
	Array<String> m_aExts;
	IFileSystem *m_pFileSystem = NULL;
};

//##########################################################################

class CAudioCodecTargetWave: public IXUnknownImplementation<IXAudioCodecTarget>
{
public:
	~CAudioCodecTargetWave();
	virtual void XMETHODCALLTYPE getDesc(AudioRawDesc *pDesc) const override;
	virtual int64_t XMETHODCALLTYPE getPos() const override;
	virtual void XMETHODCALLTYPE setPos(int64_t uPos) override;
	virtual size_t XMETHODCALLTYPE decode(int64_t iPos, uint64_t uLen, void **ppData) override;
	virtual bool XMETHODCALLTYPE encode(IXBuffer *pBufferPCM, AudioRawDesc *pOutDesc) override;

protected:

	friend CAudioCodecWave;

	void init(IFile *pFile, CWaveHeader *pHeader, AudioRawDesc *pDesc, bool forSave);

	IFile *m_pFile = NULL;
	CWaveHeader m_oHeader;
	AudioRawDesc m_oDesc;
	bool m_forSave = false;
};

#endif
