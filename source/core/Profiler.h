#ifndef __PROFILER_H
#define __PROFILER_H

#include <xcommon/IXProfilerAPI.h>
#include <xcommon/IXCore.h>

class CProfiler final: public IXUnknownImplementation<IXProfiler>
{
public:
	CProfiler(IXCore *pCore, const char *szUseProfier, bool bStart = false);
	~CProfiler();

	void XMETHODCALLTYPE registerSectionDesc(XProfilingSectionDesc *pDesc) override;
	void XMETHODCALLTYPE unregisterSectionDesc(XProfilingSectionDesc *pDesc) override;

	void XMETHODCALLTYPE startProfilingSection(XProfilingSection *pSection) override;
	void XMETHODCALLTYPE endProfilingSection(XProfilingSection *pSection) override;

	void XMETHODCALLTYPE insertMarker(XProfilingMarker *pMarker) override;

	IXProfilerAPI* getAPI();

protected:
	IXProfilerAPI *m_pProfiler = NULL;
};

#endif
