#ifndef __IXPROFILER_H
#define __IXPROFILER_H

#ifndef __GDEFINES_H
#error "Must not be included directly!"
#endif

struct XProfilingSectionDesc;
struct XProfilingSection;
struct XProfilingMarker;

#define XPROFILE_SECTION_(szName, szOptionalName) \
	const static XProfilingSectionDesc MACRO_CAT(s_profilingSectionDesc, __LINE__) (__FILE__, szName, (UINT)(__LINE__)); \
	XProfilingSection MACRO_CAT(profilingSection, __LINE__)(&MACRO_CAT(s_profilingSectionDesc, __LINE__), szOptionalName);

#define XPROFILE_FUNCTION()                   XPROFILE_SECTION_(__XMETHOD__, NULL)
#define XPROFILE_FUNCTION_ARG(szOptionalName) XPROFILE_SECTION_(__XMETHOD__, szOptionalName)
#define XPROFILE_SECTION(szName)              XPROFILE_SECTION_(     szName, NULL)

#define XPROFILE_EVENT(szName) \
	const static XProfilingSectionDesc MACRO_CAT(s_profilingSectionDesc, __LINE__) (__FILE__, szName, (UINT)(__LINE__)); \
	XProfilingMarker MACRO_CAT(profilingMarker, __LINE__)(&MACRO_CAT(s_profilingSectionDesc, __LINE__));

//#############################################################################

// {2C683146-4B5D-4224-8A72-773828DD0036}
#define IXPROFILER_GUID DEFINE_XGUID(0x2c683146, 0x4b5d, 0x4224, 0x8a, 0x72, 0x77, 0x38, 0x28, 0xdd, 0x0, 0x36)
#define IXPROFILER_VERSION 1

class IXProfiler: public IXUnknown
{
public:
	virtual void XMETHODCALLTYPE registerSectionDesc(XProfilingSectionDesc *pDesc) = 0;
	virtual void XMETHODCALLTYPE unregisterSectionDesc(XProfilingSectionDesc *pDesc) = 0;

	virtual void XMETHODCALLTYPE startProfilingSection(XProfilingSection *pSection) = 0;
	virtual void XMETHODCALLTYPE endProfilingSection(XProfilingSection *pSection) = 0;

	virtual void XMETHODCALLTYPE insertMarker(XProfilingMarker *pMarker) = 0;
};

//#############################################################################

struct XProfilingSectionDesc
{
	XProfilingSectionDesc(const char *szFilename, const char *szSectionName, UINT uLine):
		szFilename(szFilename),
		szSectionName(szSectionName),
		uLine(uLine)
	{
		extern IXProfiler *g_pProfiler;
		SAFE_CALL(g_pProfiler, registerSectionDesc, this);
	}

	~XProfilingSectionDesc()
	{
		extern IXProfiler *g_pProfiler;
		SAFE_CALL(g_pProfiler, unregisterSectionDesc, this);
	}

	const char *szFilename;
	const char *szSectionName;
	const char *szModule = SX_LIB_NAME;
	void *pCustomData = NULL;
	UINT uLine;
};

//#############################################################################

struct XProfilingSection
{
	XProfilingSection(const XProfilingSectionDesc *pDesc, const char *szOptionalName = NULL):
		pDesc(pDesc),
		szOptionalName(szOptionalName)
	{
		extern IXProfiler *g_pProfiler;
		SAFE_CALL(g_pProfiler, startProfilingSection, this);
	}

	~XProfilingSection()
	{
		extern IXProfiler *g_pProfiler;
		SAFE_CALL(g_pProfiler, endProfilingSection, this);
	}

	const XProfilingSectionDesc *pDesc;
	const char *szOptionalName;
	void *pCustomData = NULL;
};

//#############################################################################

struct XProfilingMarker
{
	XProfilingMarker(const XProfilingSectionDesc *pDesc):
		pDesc(pDesc)
	{
		extern IXProfiler *g_pProfiler;
		SAFE_CALL(g_pProfiler, insertMarker, this);
	}

	const XProfilingSectionDesc *pDesc;
};

#endif
