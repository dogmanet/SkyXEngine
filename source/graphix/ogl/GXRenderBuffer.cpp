#include "GXRenderBuffer.h"

CGXRenderBuffer::CGXRenderBuffer(CGXContext * pRender):
	m_pRender(pRender)
{
}

void CGXRenderBuffer::Release()
{
	m_pRender->destroyRenderBuffer(this);
}
