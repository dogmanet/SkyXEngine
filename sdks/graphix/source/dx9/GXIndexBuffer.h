#ifndef _IDSRindexBuffer_H_
#define _IDSRindexBuffer_H_

#include "GXContext.h"

class CGXIndexBuffer: public IGXIndexBuffer
{
	friend class CGXContext;

	CGXIndexBuffer(CGXContext * pRender):m_pRender(pRender)
	{
	};
	~CGXIndexBuffer();

	CGXContext * m_pRender;
	IDirect3DIndexBuffer9 *m_pBuffer;
	D3DFORMAT m_format;
	UINT m_uSize;
	UINT m_uLockFlagsWrite = 0;
	bool m_wasReset = true;
	bool m_isResettable = false;

	UINT m_uFlags;

	void onDevLost();
	void onDevRst();

public:
	void Release();

	bool lock(void **ppData, GXBUFFERLOCK mode);
	void unlock();

	bool wasReset();
};

#endif
