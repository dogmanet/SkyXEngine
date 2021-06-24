#ifndef __IXMATERIALPROXY_H
#define __IXMATERIALPROXY_H

#include <gdefines.h>

class IXMaterialProxyScanList: public IXUnknown
{
public:
	virtual UINT XMETHODCALLTYPE getCount() const = 0;
	virtual const char* XMETHODCALLTYPE getItem(UINT uIdx, const char **pszFileName = NULL) const = 0;
};

// {3EEDEC47-D5DA-488E-BDF0-AD7BE8ECB49D}
#define IXMATERIALPROXY_GUID DEFINE_XGUID(0x3eedec47, 0xd5da, 0x488e, 0xbd, 0xf0, 0xad, 0x7b, 0xe8, 0xec, 0xb4, 0x9d)

class IXMaterialProxy: public IXUnknown
{
public:
	virtual const char* XMETHODCALLTYPE getDescription() const = 0;

	// isForce -- output success even if the file doesn't exists
	virtual bool XMETHODCALLTYPE resolveName(const char *szName, char *szOutput, UINT *puBufSize, bool isForce = false) = 0;

	virtual bool XMETHODCALLTYPE scanForMaterials(IXMaterialProxyScanList **ppOut) = 0;
};

#endif
