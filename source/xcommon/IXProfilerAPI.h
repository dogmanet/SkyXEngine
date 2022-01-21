#ifndef __IXPROFILERAPI_H
#define __IXPROFILERAPI_H

#include <gdefines.h>

// {B6095A6C-C2EB-4322-9774-3206177DCA6D}
#define IXPROFILERAPI_GUID DEFINE_XGUID(0xb6095a6c, 0xc2eb, 0x4322, 0x97, 0x74, 0x32, 0x6, 0x17, 0x7d, 0xca, 0x6d)
#define IXPROFILERAPI_VERSION 1

class IXProfilerAPI: public IXUnknown
{
public:
	virtual const char* XMETHODCALLTYPE getName() const = 0;
	virtual const char* XMETHODCALLTYPE getKeyName() const = 0;

	virtual bool XMETHODCALLTYPE isEnabled() const = 0;
	virtual void XMETHODCALLTYPE enable(bool yesNo) = 0;

	virtual void XMETHODCALLTYPE startup() = 0;
	virtual void XMETHODCALLTYPE shutdown() = 0;

	virtual void XMETHODCALLTYPE onFrameStart() = 0;

	virtual void XMETHODCALLTYPE registerSectionDesc(XProfilingSectionDesc *pDesc) = 0;
	virtual void XMETHODCALLTYPE unregisterSectionDesc(XProfilingSectionDesc *pDesc) = 0;

	virtual void XMETHODCALLTYPE startProfilingSection(XProfilingSection *pSection) = 0;
	virtual void XMETHODCALLTYPE endProfilingSection(XProfilingSection *pSection) = 0;

	virtual void XMETHODCALLTYPE registerThread(const char *szName) = 0;
	virtual void XMETHODCALLTYPE unregisterThread() = 0;

	virtual void XMETHODCALLTYPE insertMarker(XProfilingMarker *pMarker) = 0;
};

#endif
