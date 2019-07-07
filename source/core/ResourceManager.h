#ifndef __RESOURCEMANAGER_H
#define __RESOURCEMANAGER_H

#include <xcommon/resource/IXResourceManager.h>
#include <xcommon/IXModelLoader.h>
#include <common/string.h>
#include <common/aastring.h>
#include <xcommon/IXCore.h>

class CResourceModel;

class CResourceManager: public IXResourceManager
{
public:
	CResourceManager(IXCore *pCore);

	bool XMETHODCALLTYPE getModel(const char *szName, IXResourceModel **ppOut, bool bForceReload = false) override;
	bool XMETHODCALLTYPE getModelStatic(const char *szName, IXResourceModelStatic **ppOut, bool bForceReload = false) override;
	bool XMETHODCALLTYPE getModelAnimated(const char *szName, IXResourceModelAnimated **ppOut, bool bForceReload = false) override;

	UINT XMETHODCALLTYPE getModelSupportedFormats() override;
	const XFormatName* XMETHODCALLTYPE getModelSupportedFormat(UINT uIndex) override;

	IXResourceModelStatic* XMETHODCALLTYPE newResourceModelStatic() override;
	IXResourceModelAnimated* XMETHODCALLTYPE newResourceModelAnimated() override;
	void XMETHODCALLTYPE addModel(const char *szName, IXResourceModel *pModel) override;

	void onResourceModelRelease(CResourceModel *pResource);

protected:
	IXCore *m_pCore;

	AssotiativeArray<AAString, Array<IXModelLoader*>> m_mapModelLoaders;
	AssotiativeArray<String, IXResourceModel*> m_mpModels;
	
	Array<XFormatName> m_aModelExts;
};

#endif
