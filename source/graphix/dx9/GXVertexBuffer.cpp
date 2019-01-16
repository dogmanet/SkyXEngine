#include "GXVertexBuffer.h"

void CGXVertexBuffer::Release()
{
	m_pRender->destroyVertexBuffer(this);
}

bool CGXVertexBuffer::lock(void **ppData, GXBUFFERLOCK mode)
{
	return(!FAILED(m_pBuffer->Lock(0, m_uSize, ppData, mode == GXBL_WRITE ? m_uLockFlagsWrite : D3DLOCK_READONLY)));
}

void CGXVertexBuffer::unlock()
{
	m_pBuffer->Unlock();
}

CGXVertexBuffer::~CGXVertexBuffer()
{
	mem_release(m_pBuffer);
}
