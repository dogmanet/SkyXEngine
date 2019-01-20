#include "GXDepthStencilSurface.h"

void CGXDepthStencilSurface::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		m_pRender->destroyDepthStencilSurface(this);
	}
}

CGXDepthStencilSurface::~CGXDepthStencilSurface()
{
	mem_release(m_pSurface);
}

void CGXDepthStencilSurface::onDevLost()
{
	mem_release(m_pSurface);
}
void CGXDepthStencilSurface::onDevRst(UINT uScreenWidth, UINT uScreenHeight)
{
	DX_CALL(m_pRender->getDXDevice()->CreateDepthStencilSurface(m_uWidth, m_uHeight, m_format, m_multisampleType, 0, FALSE, &m_pSurface, NULL));
}
