#include "GXVertexBuffer.h"

void CGXVertexBuffer::Release()
{
	m_pRender->destroyVertexBuffer(this);
}
