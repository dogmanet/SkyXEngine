#include "GXRenderBuffer.h"

CGXRenderBuffer::CGXRenderBuffer(CGXContext * pRender, UINT uCountStreams, IGXVertexBuffer **ppVertexBuffers, IGXVertexDeclaration *pVertexDeclaration):
	m_pRender(pRender),
	m_uStreamCount(uCountStreams),
	m_pVertexDeclaration(pVertexDeclaration)
{
	pVertexDeclaration->AddRef();
	for(UINT i = 0; i < uCountStreams; ++i)
	{
		ppVertexBuffers[i]->AddRef();
		m_ppVertexBuffers[i] = ppVertexBuffers[i];
	}
}

void CGXRenderBuffer::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		mem_release(m_pVertexDeclaration);
		for(UINT i = 0; i < m_uStreamCount; ++i)
		{
			mem_release(m_ppVertexBuffers[i]);
		}
		m_pRender->destroyRenderBuffer(this);
	}
}
