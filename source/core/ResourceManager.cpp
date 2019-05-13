#include "ResourceManager.h"
#include <xcommon/IPluginManager.h>

CResourceManager::CResourceManager(IXCore *pCore):
	m_pCore(pCore)
{
	IPluginManager *pPluginManager = m_pCore->getPluginManager();

	IXModelLoader *pLoader;
	UINT ic = 0;
	while((pLoader = (IXModelLoader*)pPluginManager->getInterface(XMODELLOADER_GUID, ic++)))
	{
		if(pLoader->getVersion() == IXMODELLOADER_VERSION)
		{
			for(UINT i = 0, l = pLoader->getExtCount(); i < l; ++i)
			{
				AAString sExt;
				strlwr(const_cast<char*>(sExt.getName()));
				sExt.setName(pLoader->getExt(i));
				m_mapModelLoaders[sExt].push_back(pLoader);
			}
		}
	}
}

bool CResourceManager::getModel(const char *szName, const IXResourceModel **ppOut, bool bForceReload)
{
	return(false);
}
bool CResourceManager::getModelStatic(const char *szName, const IXResourceModelStatic **ppOut, bool bForceReload)
{
	IXResourceModel *pModel = NULL;
	if(getModel(szName, (const IXResourceModel**)&pModel, bForceReload))
	{
		if(pModel->getType() == XMT_STATIC)
		{
			*ppOut = pModel->asStatic();
			return(true);
		}
		mem_release(pModel);
	}
	*ppOut = NULL;
	return(false);
}
bool CResourceManager::getModelAnimated(const char *szName, const IXResourceModelAnimated **ppOut, bool bForceReload)
{
	IXResourceModel *pModel = NULL;
	if(getModel(szName, (const IXResourceModel**)&pModel, bForceReload))
	{
		if(pModel->getType() == XMT_ANIMATED)
		{
			*ppOut = pModel->asAnimated();
			return(true);
		}
		mem_release(pModel);
	}
	*ppOut = NULL;
	return(false);
}
