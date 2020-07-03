#ifndef __IXTEXTURELOADER_H
#define __IXTEXTURELOADER_H

#include <xcommon/IXLoaderBase.h>
#include <core/IFile.h>
#include <xcommon/resource/IXResourceTexture.h>

// {95FBCCB6-D40F-4FC6-AFF4-8A52BD75DB3C}
#define IXTEXTURELOADER_GUID DEFINE_XGUID(0x95fbccb6, 0xd40f, 0x4fc6, 0xaf, 0xf4, 0x8a, 0x52, 0xbd, 0x75, 0xdb, 0x3c)
#define IXTEXTURELOADER_VERSION 1

//! возможная информация о текстуре
struct XTextureInfo
{
	//! тип файла
	GXTEXTURE_TYPE type;
	//! Формат изображения
	GXFORMAT format;

	//! Ширина
	UINT uWidth;
	//! Высота
	UINT uHeight;
	//! Глубина
	UINT uDepth;

	//! Количество кадров
	UINT uFrameCount;
	float fFrameTime;

	//! Количество мип-уровней
	UINT uMipCount;
};

class IXTextureLoader: public IXLoaderBase
{
public:
	virtual bool XMETHODCALLTYPE open(const char *szFileName, const char *szArg) = 0;
	virtual GXTEXTURE_TYPE XMETHODCALLTYPE getType() const = 0;
	virtual bool XMETHODCALLTYPE loadAs2D(IXResourceTexture2D *pResource) = 0;
	virtual bool XMETHODCALLTYPE loadAsCube(IXResourceTextureCube *pResource) = 0;
	virtual void XMETHODCALLTYPE getInfo(XTextureInfo *pTextureInfo) = 0;
	virtual void XMETHODCALLTYPE close() = 0;
};

#endif
