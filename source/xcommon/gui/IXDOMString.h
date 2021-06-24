#ifndef __IXDOMSTRING_H
#define __IXDOMSTRING_H

#include <gdefines.h>

class IXDOMString: public IXUnknown
{
public:
	virtual const char* XMETHODCALLTYPE getData() const = 0;
};

#endif
