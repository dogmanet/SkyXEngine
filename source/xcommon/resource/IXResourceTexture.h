#ifndef __IXRESOURCETEXTURE_H
#define __IXRESOURCETEXTURE_H

#include <gdefines.h>
#include <graphix/graphix.h>

#define IXRESOURCE_TEXTURE_AUTO_MIPS ~((UINT)0)

struct XImageMip
{
	byte *pData;
	size_t sizeData;
	bool isWritten = false;
};

// Implemented in core
class IXResourceTexture2D;
class IXResourceTextureCube;
class IXResourceTexture: public IXUnknown
{
public:
	virtual GXTEXTURE_TYPE XMETHODCALLTYPE getType() const = 0;
	virtual GXFORMAT XMETHODCALLTYPE getFormat() const = 0;
	
	virtual UINT XMETHODCALLTYPE getFrameCount() const = 0;
	virtual float XMETHODCALLTYPE getFrameTime() const = 0;
	virtual void XMETHODCALLTYPE setFrameTime(float fTime) = 0;

	virtual UINT XMETHODCALLTYPE getMipmapCount() const = 0;

	virtual const IXResourceTexture2D* XMETHODCALLTYPE as2D() const = 0;
	virtual const IXResourceTextureCube* XMETHODCALLTYPE asCube() const = 0;
	virtual IXResourceTexture2D* XMETHODCALLTYPE as2D() = 0;
	virtual IXResourceTextureCube* XMETHODCALLTYPE asCube() = 0;
	
	virtual void XMETHODCALLTYPE makeReadOnly() = 0;

	virtual UINT XMETHODCALLTYPE getTextureBytes(GXFORMAT format, UINT uWidth, UINT uHeight) = 0;
};

// Implemented in core
class IXResourceTexture2D: public IXResourceTexture
{
public:
	virtual UINT XMETHODCALLTYPE getWidth() const = 0;
	virtual UINT XMETHODCALLTYPE getHeight() const = 0;

	virtual XImageMip* XMETHODCALLTYPE getMip(UINT uMipmap, UINT uFrame = 0) = 0;
	virtual const XImageMip* XMETHODCALLTYPE getMip(UINT uMipmap, UINT uFrame = 0) const = 0;

	virtual void XMETHODCALLTYPE init(UINT uWidth, UINT uHeight, GXFORMAT format, UINT uMipmapCount = IXRESOURCE_TEXTURE_AUTO_MIPS, UINT uFrameCount = 1) = 0;
};

// Implemented in core
class IXResourceTextureCube: public IXResourceTexture
{
public:
	virtual UINT XMETHODCALLTYPE getSize() const = 0;

	virtual XImageMip* XMETHODCALLTYPE getMip(GXCUBEMAP_FACES face, UINT uMipmap, UINT uFrame = 0) = 0;
	virtual const XImageMip* XMETHODCALLTYPE getMip(GXCUBEMAP_FACES face, UINT uMipmap, UINT uFrame = 0) const = 0;

	virtual void XMETHODCALLTYPE init(UINT uSize, GXFORMAT format, UINT uMipmapCount = IXRESOURCE_TEXTURE_AUTO_MIPS, UINT uFrameCount = 1) = 0;
};


#endif
