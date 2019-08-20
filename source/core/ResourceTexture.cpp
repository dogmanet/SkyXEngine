#include "ResourceTexture.h"

CResourceTexture2D::CResourceTexture2D():
	BaseClass(GXTEXTURE_TYPE_2D)
{
}

CResourceTexture2D::~CResourceTexture2D()
{
	mem_delete_a(pDataBlob);
}

UINT XMETHODCALLTYPE CResourceTexture2D::getWidth() const
{
	return(m_uWidth);
}
UINT XMETHODCALLTYPE CResourceTexture2D::getHeight() const
{
	return(m_uHeight);
}

void XMETHODCALLTYPE CResourceTexture2D::init(UINT uWidth, UINT uHeight, GXFORMAT format, UINT uMipmapCount, UINT uFrameCount)
{
	assert(uWidth);
	assert(uHeight);
	assert(uMipmapCount > 0);
	assert(uFrameCount > 0);

	m_format = format;
	m_uFrameCount = uFrameCount;
	m_uMipmapCount = uMipmapCount;
	m_uWidth = uWidth;
	m_uHeight = uHeight;

	mem_delete_a(pDataBlob);

	if(uMipmapCount == IXRESOURCE_TEXTURE_AUTO_MIPS)
	{
		if(isBlockCompressed(format))
		{
			uMipmapCount = 1;
		}
		else
		{
			UINT uSize = min(uWidth, uHeight);
			uMipmapCount = 1;
			while((uSize >>= 1) >= 4)
			{
				++uMipmapCount;
			}
		}
	}

	size_t sizeData = uFrameCount * (sizeof(void*) + uMipmapCount * sizeof(XImageMip));
	size_t sizeFrame = getTextureBytes(format, uWidth, uHeight);
	size_t sizeTmp = sizeFrame;
	for(UINT i = 0; i < uMipmapCount; ++i)
	{
		sizeData += sizeTmp * uFrameCount;
		sizeTmp >>= 2;
	}

	pDataBlob = new byte[sizeData];

	pppData = (XImageMip**)pDataBlob;
	byte *pTmp = pDataBlob + sizeof(void*) * uFrameCount;
	for(UINT i = 0; i < uFrameCount; ++i)
	{
		pppData[i] = (XImageMip*)pTmp;
		pTmp += sizeof(void*) * uMipmapCount;
	}

	for(UINT i = 0; i < uFrameCount; ++i)
	{
		sizeTmp = sizeFrame;
		for(UINT j = 0; i < uMipmapCount; ++i)
		{
			pppData[i][j].pData = pTmp;
			pppData[i][j].sizeData = sizeTmp;
			pppData[i][j].isWritten = false;
			pTmp += sizeTmp;
			sizeTmp >>= 2;
		}
	}
}

const IXResourceTexture2D* XMETHODCALLTYPE CResourceTexture2D::as2D() const
{
	return(this);
}
IXResourceTexture2D* XMETHODCALLTYPE CResourceTexture2D::as2D()
{
	return(this);
}

//##########################################################################

CResourceTextureCube::CResourceTextureCube():
	BaseClass(GXTEXTURE_TYPE_CUBE)
{
}

CResourceTextureCube::~CResourceTextureCube()
{
	mem_delete_a(pDataBlob);
}

UINT XMETHODCALLTYPE CResourceTextureCube::getSize() const
{
	return(m_uSize);
}

void XMETHODCALLTYPE CResourceTextureCube::init(UINT uSize, GXFORMAT format, UINT uMipmapCount, UINT uFrameCount)
{
	assert(uSize);
	assert(uMipmapCount > 0);

	m_format = format;
	m_uFrameCount = uFrameCount;
	m_uMipmapCount = uMipmapCount;
	m_uSize = uSize;

	mem_delete_a(pDataBlob);

	if(uMipmapCount == IXRESOURCE_TEXTURE_AUTO_MIPS)
	{
		if(isBlockCompressed(format))
		{
			uMipmapCount = 1;
		}
		else
		{
			UINT uTmpSize = uSize;
			uMipmapCount = 1;
			while((uTmpSize >>= 1) >= 4)
			{
				++uMipmapCount;
			}
		}
	}

	size_t sizeData = uFrameCount * (sizeof(void*) + uMipmapCount * sizeof(XImageMip));
	size_t sizeFrame = getTextureBytes(format, uSize, uSize) * 6;
	size_t sizeTmp = sizeFrame;
	for(UINT i = 0; i < uMipmapCount; ++i)
	{
		sizeData += sizeTmp * uFrameCount;
		sizeTmp >>= 2;
	}

	pDataBlob = new byte[sizeData];

	pppData = (XImageMip**)pDataBlob;
	byte *pTmp = pDataBlob + sizeof(void*) * uFrameCount;
	for(UINT i = 0; i < uFrameCount; ++i)
	{
		pppData[i] = (XImageMip*)pTmp;
		pTmp += sizeof(void*) * uMipmapCount;
	}

	for(UINT i = 0; i < uFrameCount; ++i)
	{
		sizeTmp = sizeFrame;
		for(UINT j = 0; i < uMipmapCount; ++i)
		{
			pppData[i][j].pData = pTmp;
			pppData[i][j].sizeData = sizeTmp;
			pppData[i][j].isWritten = false;
			pTmp += sizeTmp;
			sizeTmp >>= 2;
		}
	}
}

const IXResourceTextureCube* XMETHODCALLTYPE CResourceTextureCube::asCube() const
{
	return(this);
}

IXResourceTextureCube* XMETHODCALLTYPE CResourceTextureCube::asCube()
{
	return(this);
}
