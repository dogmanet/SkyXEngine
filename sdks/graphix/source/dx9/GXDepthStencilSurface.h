#ifndef _CGXDepthStencilSurface_H_
#define _CGXDepthStencilSurface_H_

#include "GXContext.h"

class CGXDepthStencilSurface: public IGXDepthStencilSurface
{					  
	friend class CGXContext;

	CGXDepthStencilSurface(CGXContext * pRender):m_pRender(pRender)
	{
	};
	~CGXDepthStencilSurface();

	CGXContext * m_pRender;
	IDirect3DSurface9 *m_pSurface = NULL;

	bool m_bAutoResize = false;
	float m_fSizeCoeffW = 1.0f;
	float m_fSizeCoeffH = 1.0f;

	D3DMULTISAMPLE_TYPE m_multisampleType;
	D3DFORMAT m_format;
	UINT m_uWidth;
	UINT m_uHeight;

	void onDevLost();
	void onDevRst(UINT uScreenWidth, UINT uScreenHeight);

public:
	void Release();
};

#endif
