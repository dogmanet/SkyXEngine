#ifndef __IXTEXTUREPROXY_H
#define __IXTEXTUREPROXY_H

#include <gdefines.h>

// {BD03E0C0-7589-4162-8130-5CD770581EE3}
#define IXTEXTUREPROXY_GUID DEFINE_XGUID(0xbd03e0c0, 0x7589, 0x4162, 0x81, 0x30, 0x5c, 0xd7, 0x70, 0x58, 0x1e, 0xe3)
#define IXTEXTUREPROXY_VERSION 1

class IXTextureProxy: public IXUnknown
{
public:
	virtual const char* XMETHODCALLTYPE getDescription() const = 0;

	virtual bool XMETHODCALLTYPE resolveName(const char *szName, char *szOutput, UINT *puBufSize) = 0;
};

#endif
