#include "GXRenderBuffer.h"

CGXRenderBuffer::CGXRenderBuffer(CGXContext * pRender, UINT uCountStreams, IGXVertexBuffer **ppVertexBuffers, IGXVertexDeclaration *pVertexDeclaration):
	m_pRender(pRender),
	m_uStreamCount(uCountStreams),
	m_pVertexDeclaration(pVertexDeclaration)
{
	for(UINT i = 0; i < uCountStreams; ++i)
	{
		m_ppVertexBuffers[i] = ppVertexBuffers[i];
	}
}

void CGXRenderBuffer::Release()
{
	m_pRender->destroyRenderBuffer(this);
}
