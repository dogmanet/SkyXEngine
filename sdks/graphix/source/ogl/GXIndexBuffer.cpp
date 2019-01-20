#include "GXIndexBuffer.h"

void CGXIndexBuffer::Release()
{
	m_pRender->destroyIndexBuffer(this);
}
