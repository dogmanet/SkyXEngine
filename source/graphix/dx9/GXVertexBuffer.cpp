#include "GXVertexBuffer.h"

void CGXVertexBuffer::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pRender->destroyVertexBuffer(this);
	}
}

bool CGXVertexBuffer::lock(void **ppData, GXBUFFERLOCK mode)
{
	if(!FAILED(m_pBuffer->Lock(0, m_uSize, ppData, mode == GXBL_WRITE ? m_uLockFlagsWrite : D3DLOCK_READONLY)))
	{
		m_wasReset = false;
		return(true);
	}
	return(false);
}

void CGXVertexBuffer::unlock()
{
	m_pBuffer->Unlock();
}

CGXVertexBuffer::~CGXVertexBuffer()
{
	mem_release(m_pBuffer);
}

bool CGXVertexBuffer::wasReset()
{
	return(m_wasReset);
}

void CGXVertexBuffer::onDevLost()
{
	mem_release(m_pBuffer);
}

void CGXVertexBuffer::onDevRst()
{
	DX_CALL(m_pRender->getDXDevice()->CreateVertexBuffer(m_uSize, m_uFlags, 0, D3DPOOL_DEFAULT, &m_pBuffer, NULL));
}
