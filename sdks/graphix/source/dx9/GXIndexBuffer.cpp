#include "GXIndexBuffer.h"

void CGXIndexBuffer::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pRender->destroyIndexBuffer(this);
	}
}

bool CGXIndexBuffer::lock(void **ppData, GXBUFFERLOCK mode)
{
	if(!FAILED(m_pBuffer->Lock(0, m_uSize, ppData, mode == GXBL_WRITE ? m_uLockFlagsWrite : D3DLOCK_READONLY)))
	{
		m_wasReset = false;
		return(true);
	}
	return(false);
}

void CGXIndexBuffer::unlock()
{
	m_pBuffer->Unlock();
}

CGXIndexBuffer::~CGXIndexBuffer()
{
	mem_release(m_pBuffer);
}

bool CGXIndexBuffer::wasReset()
{
	return(m_wasReset);
}

void CGXIndexBuffer::onDevLost()
{
	mem_release(m_pBuffer);
}

void CGXIndexBuffer::onDevRst()
{
	DX_CALL(m_pRender->getDXDevice()->CreateIndexBuffer(m_uSize, m_uFlags, m_format, D3DPOOL_DEFAULT, &m_pBuffer, NULL));
}
