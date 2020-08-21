#ifndef __IDESKTOP_H
#define __IDESKTOP_H

#include <gdefines.h>
#include "IXDOMDocument.h"

class IXDesktop: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE loadFromFile(const char *szFile) = 0;

	virtual void XMETHODCALLTYPE setWidth(UINT w) = 0;
	virtual void XMETHODCALLTYPE setHeight(UINT h) = 0;

	virtual void XMETHODCALLTYPE render(float fTimeDelta, bool bPresent = true) = 0;

	virtual void XMETHODCALLTYPE getDocument(IXDOMDocument **ppOut) = 0;
};

#endif
