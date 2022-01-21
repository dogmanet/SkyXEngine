#include "Profiler.h"
#include <xcommon/IPluginManager.h>


CProfiler::CProfiler(IXCore *pCore, const char *szUseProfier, bool bStart)
{
	if(!szUseProfier || !szUseProfier[0])
	{
		return;
	}
	IXProfilerAPI *pAPI;
	UINT uIdx = 0;
	IPluginManager *pPluginManager = pCore->getPluginManager();
	while((pAPI = (IXProfilerAPI*)pPluginManager->getInterface(IXPROFILERAPI_GUID, uIdx++)))
	{
		if(!strcmp(szUseProfier, pAPI->getKeyName()))
		{
			m_pProfiler = pAPI;
			m_pProfiler->startup();
			if(bStart)
			{
				m_pProfiler->enable(true);
			}
			break;
		}
	}
}
CProfiler::~CProfiler()
{
	SAFE_CALL(m_pProfiler, shutdown);
	mem_release(m_pProfiler);
}

void XMETHODCALLTYPE CProfiler::registerSectionDesc(XProfilingSectionDesc *pDesc)
{
	SAFE_CALL(m_pProfiler, registerSectionDesc, pDesc);
}
void XMETHODCALLTYPE CProfiler::unregisterSectionDesc(XProfilingSectionDesc *pDesc)
{
	SAFE_CALL(m_pProfiler, unregisterSectionDesc, pDesc);
}

void XMETHODCALLTYPE CProfiler::startProfilingSection(XProfilingSection *pSection)
{
	SAFE_CALL(m_pProfiler, startProfilingSection, pSection);
}
void XMETHODCALLTYPE CProfiler::endProfilingSection(XProfilingSection *pSection)
{
	SAFE_CALL(m_pProfiler, endProfilingSection, pSection);
}

void XMETHODCALLTYPE CProfiler::insertMarker(XProfilingMarker *pMarker)
{
	SAFE_CALL(m_pProfiler, insertMarker, pMarker);
}

IXProfilerAPI* CProfiler::getAPI()
{
	return(m_pProfiler);
}
