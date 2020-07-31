#ifndef __IXTEXTUREFILTER_H
#define __IXTEXTUREFILTER_H

#include <gdefines.h>
#include "resource/IXResourceTexture.h"

// {2F49E711-445F-4B5E-9F2E-8167953FAAA7}
#define IXTEXTUREFILTER_GUID DEFINE_XGUID(0x2f49e711, 0x445f, 0x4b5e, 0x9f, 0x2e, 0x81, 0x67, 0x95, 0x3f, 0xaa, 0xa7)
#define IXTEXTUREFILTER_VERSION 1

class IXTextureFilter: public IXUnknown
{
public:
	virtual const char* XMETHODCALLTYPE getName() const = 0;
	virtual const char* XMETHODCALLTYPE getDescription() const = 0;

	virtual bool XMETHODCALLTYPE apply(const IXResourceTexture *pResource, const char *szArg, IXResourceTexture **ppOut) = 0;
};

#endif
