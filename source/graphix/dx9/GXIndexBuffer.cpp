#include "GXIndexBuffer.h"

void CGXIndexBuffer::Release()
{
	m_pRender->destroyIndexBuffer(this);
}

bool CGXIndexBuffer::lock(void **ppData, GXBUFFERLOCK mode)
{
	return(!FAILED(m_pBuffer->Lock(0, m_uSize, ppData, mode == GXBL_WRITE ? m_uLockFlagsWrite : D3DLOCK_READONLY)));
}

void CGXIndexBuffer::unlock()
{
	m_pBuffer->Unlock();
}

CGXIndexBuffer::~CGXIndexBuffer()
{
	mem_release(m_pBuffer);
}
