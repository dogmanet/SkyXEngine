#ifndef __RESOURCEMANAGER_H
#define __RESOURCEMANAGER_H

#include <xcommon/resource/IXResourceManager.h>
#include <xcommon/IXModelLoader.h>
#include <common/string.h>
#include <common/aastring.h>
#include <xcommon/IXCore.h>

class CResourceManager: public IXResourceManager
{
public:
	CResourceManager(IXCore *pCore);

	bool XMETHODCALLTYPE getModel(const char *szName, const IXResourceModel **ppOut, bool bForceReload = false) override;
	bool XMETHODCALLTYPE getModelStatic(const char *szName, const IXResourceModelStatic **ppOut, bool bForceReload = false) override;
	bool XMETHODCALLTYPE getModelAnimated(const char *szName, const IXResourceModelAnimated **ppOut, bool bForceReload = false) override;

protected:
	IXCore *m_pCore;

	AssotiativeArray<AAString, Array<IXModelLoader*>> m_mapModelLoaders;
	AssotiativeArray<String, IXResourceModel*> m_mpModels;
};

#endif
