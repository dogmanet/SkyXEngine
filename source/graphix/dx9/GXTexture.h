#ifndef _CGXTexture_H_
#define _CGXTexture_H_

#include "GXContext.h"

class CGXBaseTexture: public virtual IGXBaseTexture
{					  
	friend class CGXContext;

protected:
	CGXBaseTexture(CGXContext * pRender):
		m_pRender(pRender)
	{
	};

	CGXContext * m_pRender;
	GXFORMAT m_format;
	bool m_bWasReset;
	bool m_bAutoResize = false;
	
public:
	virtual IDirect3DBaseTexture9 *getDXTexture() = 0;
};

class CGXTexture2D: public virtual CGXBaseTexture, public virtual IGXTexture2D
{
	friend class CGXContext;

protected:
	CGXTexture2D(CGXContext * pRender): CGXBaseTexture(pRender)
	{
	}
	~CGXTexture2D();

	UINT m_uWidth;
	UINT m_uHeight;
	IDirect3DTexture9 *m_pTexture;
	float m_fSizeCoeffW = 1.0f;
	float m_fSizeCoeffH = 1.0f;
	UINT m_uMipLevels;
	UINT m_uUsage;

	void onDevLost();
	void onDevRst(UINT uScreenWidth, UINT uScreenHeight);

public:
	void Release();

	IGXSurface *getMipmap(UINT i = 0);

	UINT getWidth();
	UINT getHeight();

	bool lock(void **ppData, GXTEXLOCK mode);
	void unlock();

	GXFORMAT getFormat();
	bool wasReset();

	IDirect3DBaseTexture9 *getDXTexture();
};

class CGXTextureCube: public virtual CGXBaseTexture, public virtual IGXTextureCube
{
	friend class CGXContext;

protected:
	CGXTextureCube(CGXContext * pRender): CGXBaseTexture(pRender)
	{
	}
	~CGXTextureCube();

	UINT m_uSize;
	IDirect3DCubeTexture9 *m_pTexture;
	float m_fSizeCoeff = 1.0f;

	UINT m_uMipLevels;
	UINT m_uUsage;


	void onDevLost();
	void onDevRst(UINT uScreenHeight);
public:
	void Release();

	IGXSurface *getMipmap(GXCUBEMAP_FACES face, UINT i = 0);

	UINT getSize();

	bool lock(void **ppData, GXCUBEMAP_FACES face, GXTEXLOCK mode);
	void unlock();

	GXFORMAT getFormat();
	bool wasReset();

	IDirect3DBaseTexture9 *getDXTexture();
};

#endif
