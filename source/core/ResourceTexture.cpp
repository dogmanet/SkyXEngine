#include "ResourceTexture.h"
#include "ResourceManager.h"

CResourceTexture2D::CResourceTexture2D(CResourceManager *pMgr):
	BaseClass(GXTEXTURE_TYPE_2D, pMgr)
{
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
			UINT uSize = max(uWidth, uHeight);
			uMipmapCount = 1;
			while((uSize >>= 1) >= 1)
			{
				++uMipmapCount;
			}
		}
	}

	size_t sizeData = uFrameCount * (sizeof(void*) + uMipmapCount * sizeof(XImageMip));
	UINT uTmpWidth = uWidth;
	UINT uTmpHeight = uHeight;
	for(UINT i = 0; i < uMipmapCount; ++i)
	{
		sizeData += getTextureBytes(format, uTmpWidth, uTmpHeight) * uFrameCount;
		uTmpWidth >>= 1;
		uTmpHeight >>= 1;
		uTmpWidth = max(uTmpWidth, 1);
		uTmpHeight = max(uTmpHeight, 1);
	}

	pDataBlob = new byte[sizeData];

	ppData = (XImageMip**)pDataBlob;
	byte *pTmp = pDataBlob + sizeof(void*) * uFrameCount;
	for(UINT i = 0; i < uFrameCount; ++i)
	{
		ppData[i] = (XImageMip*)pTmp;
		pTmp += sizeof(XImageMip) * uMipmapCount;
	}

	for(UINT i = 0; i < uFrameCount; ++i)
	{
		uTmpWidth = uWidth;
		uTmpHeight = uHeight;

		for(UINT j = 0; j < uMipmapCount; ++j)
		{
			ppData[i][j].pData = pTmp;
			ppData[i][j].sizeData = getTextureBytes(format, uTmpWidth, uTmpHeight);
			ppData[i][j].isWritten = false;
			pTmp += ppData[i][j].sizeData;
			uTmpWidth >>= 1;
			uTmpHeight >>= 1;
			uTmpWidth = max(uTmpWidth, 1);
			uTmpHeight = max(uTmpHeight, 1);
		}
	}
}

XImageMip* XMETHODCALLTYPE CResourceTexture2D::getMip(UINT uMipmap, UINT uFrame)
{
	assert(ppData);
	assert(m_uMipmapCount > uMipmap);
	assert(m_uFrameCount > uFrame);
	if(!ppData || m_uMipmapCount <= uMipmap || m_uFrameCount <= uFrame)
	{
		return(NULL);
	}
	return(&ppData[uFrame][uMipmap]);
}
const XImageMip* XMETHODCALLTYPE CResourceTexture2D::getMip(UINT uMipmap, UINT uFrame) const
{
	assert(ppData);
	assert(m_uMipmapCount < uMipmap);
	assert(m_uFrameCount < uFrame);
	if(!ppData || m_uMipmapCount >= uMipmap || m_uFrameCount >= uFrame)
	{
		return(NULL);
	}
	return(&ppData[uFrame][uMipmap]);
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

CResourceTextureCube::CResourceTextureCube(CResourceManager *pMgr):
	BaseClass(GXTEXTURE_TYPE_CUBE, pMgr)
{
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
			while((uTmpSize >>= 1) >= 1)
			{
				++uMipmapCount;
			}
		}
	}

	size_t sizeData = uFrameCount * (sizeof(void*) + uMipmapCount * (sizeof(void*) + 6 * sizeof(XImageMip)));
	UINT uTmpSize = uSize;
	for(UINT i = 0; i < uMipmapCount; ++i)
	{
		sizeData += getTextureBytes(format, uTmpSize, uTmpSize) * 6 * uFrameCount;
		uTmpSize >>= 1;
		uTmpSize = max(uTmpSize, 1);
	}

	pDataBlob = new byte[sizeData];

	pppData = (XImageMip***)pDataBlob;
	byte *pTmp = pDataBlob + sizeof(void*) * uFrameCount;
	for(UINT i = 0; i < uFrameCount; ++i)
	{
		pppData[i] = (XImageMip**)pTmp;
		pTmp += sizeof(void*) * uMipmapCount;
		for(UINT mip = 0; mip < uMipmapCount; ++mip)
		{
			pppData[i][mip] = (XImageMip*)pTmp;
			pTmp += sizeof(XImageMip) * 6;
		}
	}

	for(UINT i = 0; i < uFrameCount; ++i)
	{
		uTmpSize = uSize;
		for(UINT j = 0; j < uMipmapCount; ++j)
		{
			for(int side = 0; side < 6; ++side)
			{
				pppData[i][j][side].pData = pTmp;
				pppData[i][j][side].sizeData = getTextureBytes(format, uTmpSize, uTmpSize);
				pppData[i][j][side].isWritten = false;
				pTmp += pppData[i][j][side].sizeData;
			}
			uTmpSize >>= 1;
			uTmpSize = max(uTmpSize, 1);
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

XImageMip* XMETHODCALLTYPE CResourceTextureCube::getMip(GXCUBEMAP_FACES face, UINT uMipmap, UINT uFrame)
{
	assert(pppData);
	assert(m_uMipmapCount > uMipmap);
	assert(m_uFrameCount > uFrame);
	if(!pppData || m_uMipmapCount <= uMipmap || m_uFrameCount <= uFrame)
	{
		return(NULL);
	}
	return(&pppData[uFrame][uMipmap][face]);
}
const XImageMip* XMETHODCALLTYPE CResourceTextureCube::getMip(GXCUBEMAP_FACES face, UINT uMipmap, UINT uFrame) const
{
	assert(pppData);
	assert(m_uMipmapCount < uMipmap);
	assert(m_uFrameCount < uFrame);
	if(!pppData || m_uMipmapCount >= uMipmap || m_uFrameCount >= uFrame)
	{
		return(NULL);
	}
	return(&pppData[uFrame][uMipmap][face]);
}
