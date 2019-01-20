#ifndef _IDSGrenderBufferOgl_H_
#define _IDSGrenderBufferOgl_H_

#include "GXContext.h"

class CGXRenderBuffer: public IGXRenderBuffer
{
	friend class CGXContext;

	CGXRenderBuffer(CGXContext * pRender, UINT uCountStreams, IGXVertexBuffer **ppVertexBuffers, IGXVertexDeclaration *pVertexDeclaration);

	CGXContext * m_pRender;

	IGXVertexBuffer *m_ppVertexBuffers[MAXGXVSTREAM];
	IGXVertexDeclaration *m_pVertexDeclaration;

	UINT m_uStreamCount;

public:
	void Release();
};

#endif
