#ifndef _CGXTexture_H_
#define _CGXTexture_H_

#include "GXContext.h"

class CGXTexture2D: public IGXTexture2D
{
	friend class CGXContext;

protected:
	CGXContext * m_pRender;
	CGXTexture2D(CGXContext * pRender): m_pRender(pRender)
	{
	}
	~CGXTexture2D();
	GXFORMAT m_format;
	bool m_bWasReset;
	bool m_isResettable = false;
	bool m_bAutoResize = false;

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

	GXTEXTURE_TYPE getType();
};

class CGXTextureCube: public IGXTextureCube
{
	friend class CGXContext;

protected:
	CGXContext * m_pRender;
	CGXTextureCube(CGXContext * pRender): m_pRender(pRender)
	{
	}
	~CGXTextureCube();
	GXFORMAT m_format;
	bool m_bWasReset;
	bool m_isResettable = false;
	bool m_bAutoResize = false;

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

	GXTEXTURE_TYPE getType();
};

#endif
