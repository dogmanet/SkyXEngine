#ifndef __IXDOMSTRING_H
#define __IXDOMSTRING_H

#include "IXDesktopStack.h"

class IXDOMString: public IXUnknown
{
public:
	virtual const char* XMETHODCALLTYPE getData() const = 0;
};

#endif
