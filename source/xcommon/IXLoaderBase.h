#ifndef __IXLOADER_BASE_H
#define __IXLOADER_BASE_H

#include <gdefines.h>

class IXLoaderBase: public IXUnknown
{
public:
	virtual UINT XMETHODCALLTYPE getExtCount() const = 0;
	virtual const char* XMETHODCALLTYPE getExt(UINT uIndex) const = 0;
	virtual const char* XMETHODCALLTYPE getExtText(UINT uIndex) const = 0;
	virtual const char* XMETHODCALLTYPE getAuthor() const = 0;
	virtual const char* XMETHODCALLTYPE getCopyright() const = 0;
	virtual const char* XMETHODCALLTYPE getDescription() const = 0;
};

#endif
