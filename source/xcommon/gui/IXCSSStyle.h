#ifndef __IXCSSSTYLE_H
#define __IXCSSSTYLE_H

#include <gdefines.h>
#include "IXCSSProperty.h"

class IXCSSStyle: public IXUnknown
{
public:
	virtual bool XMETHODCALLTYPE getProperty(const char *szName, IXCSSProperty **ppOut) = 0;
};

#endif
