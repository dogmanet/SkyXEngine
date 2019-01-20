#include "GXTexture.h"
#include "GXSurface.h"

CGXTexture2D::~CGXTexture2D()
{
	mem_release(m_pTexture);
}

GXFORMAT CGXTexture2D::getFormat()
{
	return(m_format);
}

bool CGXTexture2D::wasReset()
{
	return(m_bWasReset);
}

UINT CGXTexture2D::getWidth()
{
	return(m_uWidth);
}

UINT CGXTexture2D::getHeight()
{
	return(m_uHeight);
}

void CGXTexture2D::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pRender->destroyTexture2D(this);
	}
}

IGXSurface *CGXTexture2D::getMipmap(UINT i)
{
	IDirect3DSurface9 *pDXSurface;

	if(!SUCCEEDED(DX_CALL(m_pTexture->GetSurfaceLevel(i, &pDXSurface))))
	{
		return(NULL);
	}

	CGXSurface *pSurface = new CGXSurface(m_pRender, max(1, m_uWidth >> i), max(1, m_uHeight >> i), m_format, pDXSurface);
	return(pSurface);
}

bool CGXTexture2D::lock(void **ppData, GXTEXLOCK mode)
{
	CGXContext::debugMessage(GX_LOG_ERROR, "Not implemented: CGXTexture2D::lock");
	return(false);
}

void CGXTexture2D::unlock()
{
	CGXContext::debugMessage(GX_LOG_ERROR, "Not implemented: CGXTexture2D::unlock");
}

IDirect3DBaseTexture9 *CGXTexture2D::getDXTexture()
{
	return(m_pTexture);
}

void CGXTexture2D::onDevLost()
{
	mem_release(m_pTexture);
}
void CGXTexture2D::onDevRst(UINT uScreenWidth, UINT uScreenHeight)
{
	if(m_bAutoResize)
	{
		m_uWidth = (UINT)((float)uScreenWidth * m_fSizeCoeffW);
		m_uHeight = (UINT)((float)uScreenHeight * m_fSizeCoeffH);
	}
	m_bWasReset = true;
	DX_CALL(m_pRender->getDXDevice()->CreateTexture(m_uWidth, m_uHeight, m_uMipLevels, m_uUsage, m_pRender->getDXFormat(m_format), D3DPOOL_DEFAULT, &m_pTexture, NULL));
}

GXTEXTURE_TYPE CGXTexture2D::getType()
{
	return(GXTEXTURE_TYPE_2D);
}

//##########################################################################

CGXTextureCube::~CGXTextureCube()
{
	mem_release(m_pTexture);
}

void CGXTextureCube::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pRender->destroyTextureCube(this);
	}
}

IGXSurface *CGXTextureCube::getMipmap(GXCUBEMAP_FACES face, UINT i)
{
	IDirect3DSurface9 *pDXSurface;
	if(!SUCCEEDED(DX_CALL(m_pTexture->GetCubeMapSurface((D3DCUBEMAP_FACES)face, i, &pDXSurface))))
	{
		return(NULL);
	}

	CGXSurface *pSurface = new CGXSurface(m_pRender, max(1, m_uSize >> i), max(1, m_uSize >> i), m_format, pDXSurface);
	return(pSurface);
}

UINT CGXTextureCube::getSize()
{
	return(m_uSize);
}

IDirect3DBaseTexture9 *CGXTextureCube::getDXTexture()
{
	return(m_pTexture);
}

bool CGXTextureCube::lock(void **ppData, GXCUBEMAP_FACES face, GXTEXLOCK mode)
{
	CGXContext::debugMessage(GX_LOG_ERROR, "Not implemented: CGXTexture2D::lock");
	return(false);
}

void CGXTextureCube::unlock()
{
	CGXContext::debugMessage(GX_LOG_ERROR, "Not implemented: CGXTexture2D::unlock");
}

GXFORMAT CGXTextureCube::getFormat()
{
	return(m_format);
}

bool CGXTextureCube::wasReset()
{
	return(m_bWasReset);
}

void CGXTextureCube::onDevLost()
{
	mem_release(m_pTexture);
}
void CGXTextureCube::onDevRst(UINT uScreenHeight)
{
	if(m_bAutoResize)
	{
		m_uSize = (UINT)((float)uScreenHeight * m_fSizeCoeff);
	}
	m_bWasReset = true;
	DX_CALL(m_pRender->getDXDevice()->CreateCubeTexture(m_uSize, m_uMipLevels, m_uUsage, m_pRender->getDXFormat(m_format), D3DPOOL_DEFAULT, &m_pTexture, NULL));
}

GXTEXTURE_TYPE CGXTextureCube::getType()
{
	return(GXTEXTURE_TYPE_CUBE);
}
