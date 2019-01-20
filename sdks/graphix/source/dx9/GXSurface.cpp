#include "GXSurface.h"

void CGXSurface::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		delete this;
	}
}

CGXSurface::~CGXSurface()
{
	mem_release(m_pSurface);
}

UINT CGXSurface::getWidth()
{
	return(m_uWidth);
}
UINT CGXSurface::getHeight()
{
	return(m_uHeight);
}
GXFORMAT CGXSurface::getFormat()
{
	return(m_format);
}
