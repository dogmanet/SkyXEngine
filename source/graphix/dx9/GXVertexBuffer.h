#ifndef _IDSRvertexBufferOgl_H_
#define _IDSRvertexBufferOgl_H_

#include "GXContext.h"

class CGXVertexBuffer: public IGXVertexBuffer
{
	friend class CGXContext;

	CGXVertexBuffer(CGXContext * pRender):m_pRender(pRender)
	{
	};
	~CGXVertexBuffer();

	CGXContext * m_pRender;
	IDirect3DVertexBuffer9 *m_pBuffer;
	//D3DFORMAT m_format;
	UINT m_uSize;
	UINT m_uLockFlagsWrite = 0;
public:
	void Release();

	bool lock(void **ppData, GXBUFFERLOCK mode);
	void unlock();
};

#endif
