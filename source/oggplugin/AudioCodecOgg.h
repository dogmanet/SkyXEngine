/*****************************************************
Copyright © DogmaNet Team, 2020
Site: dogmanet.ru
See the license in LICENSE
*****************************************************/

#ifndef __AUDIOCODECOGG_H
#define __AUDIOCODECOGG_H

#include <xcommon/IXAudioCodec.h>

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/vorbisenc.h>

#include <common/string.h>

#include <xcommon/IFileSystem.h>
#include <core/IFile.h>

//! количество байт на семпл
#define OGG_BYTES_PER_SAMPLE 2

//##########################################################################

class CAudioCodecOgg: public IXUnknownImplementation<IXAudioCodec>
{
public:
	CAudioCodecOgg(IFileSystem *pFileSystem);
	~CAudioCodecOgg(){}

	XIMPLEMENT_VERSION(IXAUDIOCODEC_VERSION);

	virtual const char* XMETHODCALLTYPE getFormat() const override;
	virtual const char* XMETHODCALLTYPE getExt(UINT uIndex=0) const override;
	virtual UINT XMETHODCALLTYPE getExtCount() const override;
	virtual bool XMETHODCALLTYPE open(const char *szFile, const char *szArg, IXAudioCodecTarget **ppTarget, bool forSave = false) override;
	virtual bool XMETHODCALLTYPE canSave(UINT uIndex=0) const override;

protected:
	ov_callbacks m_oCB;
	Array<String> m_aExts;
	IFileSystem *m_pFileSystem = NULL;
};

//##########################################################################

class CAudioCodecTargetOgg: public IXUnknownImplementation<IXAudioCodecTarget>
{
public:
	~CAudioCodecTargetOgg();
	virtual void XMETHODCALLTYPE getDesc(AudioRawDesc *pDesc) const override;
	virtual int64_t XMETHODCALLTYPE getPos() const override;
	virtual void XMETHODCALLTYPE setPos(int64_t iPos) override;
	virtual size_t XMETHODCALLTYPE decode(int64_t iPos, uint64_t uLen, void **ppData) override;
	virtual bool XMETHODCALLTYPE encode(IXBuffer *pBufferPCM, AudioRawDesc *pOutDesc) override;

protected:

	friend CAudioCodecOgg;

	void init(IFile *pFile, OggVorbis_File *pVoFile, AudioRawDesc *pDesc, bool forSave);

	IFile *m_pFile = NULL;
	OggVorbis_File *m_pVoFile;
	AudioRawDesc m_oDesc;
	bool m_forSave = false;
};

#endif
