#include "PluginManager.h"

#if defined(_LINUX) || defined(_MAC)
#	include <dlfcn.h>
#endif

void CPluginManager::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		delete this;
	}
}

CPluginManager::~CPluginManager()
{
	for(UINT i = 0, l = m_aPlugins.size(); i < l; ++i)
	{
		m_aPlugins[i]->shutdown();
		mem_release(m_aPlugins[i]);
	}
}

void CPluginManager::invokeStartup(IXCore *pCore)
{
	for(UINT i = 0, l = m_aPlugins.size(); i < l; ++i)
	{
		m_aPlugins[i]->startup(pCore);
	}
}

IXPlugin *CPluginManager::loadPlugin(const char *szPath)
{
	PFNXPLUGINMAIN pfnXPluginMain = NULL;
#if defined(_WINDOWS)
	HMODULE dmod = LoadLibraryA(szPath);
	if(!dmod)
	{
		return(NULL);	
	}
	pfnXPluginMain = (PFNXPLUGINMAIN)GetProcAddress(dmod, MACRO_TEXT(X_PLUGIN_ENTRYPOINT));
#endif
#if defined(_LINUX) || defined(_MAC)
	void *dmod = dlopen(szPath, RTLD_LAZY);
	if(!dmod)
	{
		return(NULL);
	}
	pfnXPluginMain = (PFNXPLUGINMAIN)dlsym(mod, MACRO_TEXT(X_PLUGIN_ENTRYPOINT));
#endif

	if(pfnXPluginMain)
	{
		IXPlugin *pPlugin = pfnXPluginMain();
		if(pPlugin)
		{
			if(pPlugin->getVersion() == IXPLUGIN_VERSION)
			{
				X_PLUGIN_TYPE type = pPlugin->getType();
				IXUnknown *pIFace;
				if(type & XPLUGIN_RENDERABLE)
				{
					pIFace = pPlugin->getIXRenderable();
					if(pIFace)
					{
						m_typedIFaces[XPLUGIN_RENDERABLE].push_back(pIFace);
					}
				}
				if(type & XPLUGIN_LOADER_MODEL)
				{
					pIFace = pPlugin->getIXModelLoader();
					if(pIFace)
					{
						m_typedIFaces[XPLUGIN_LOADER_MODEL].push_back(pIFace);
					}
				}
				if(type & XPLUGIN_LOADER_TEXTURE)
				{
					pIFace = pPlugin->getIXTextureLoader();
					if(pIFace)
					{
						m_typedIFaces[XPLUGIN_LOADER_TEXTURE].push_back(pIFace);
					}
				}
				if(type & XPLUGIN_UPDATABLE)
				{
					pIFace = pPlugin->getIXUpdatable();
					if(pIFace)
					{
						m_typedIFaces[XPLUGIN_UPDATABLE].push_back(pIFace);
					}
				}

				UINT uIFaceCount = pPlugin->getInterfaceCount();
				_iface iface;
				iface.pPlugin = pPlugin;
				for(UINT i = 0; i < uIFaceCount; ++i)
				{
					m_mIFaces[*pPlugin->getInterfaceGUID(i)] = iface;
				}

				m_aPlugins.push_back(pPlugin);

				return(pPlugin);
			}
		}
	}

#if defined(_WINDOWS)
	FreeLibrary(dmod);
#endif
#if defined(_LINUX) || defined(_MAC)
	dlclose(dmod);
#endif
	return(NULL);
}

IXUnknown *CPluginManager::getInterface(const XGUID &guid)
{
	const AssotiativeArray<XGUID, _iface>::Node *pNode;
	if(m_mIFaces.KeyExists(guid, &pNode))
	{
		if(!pNode->Val->pIFace)
		{
			pNode->Val->pIFace = pNode->Val->pPlugin->getInterface(guid);
		}
		return(pNode->Val->pIFace);
	}
	return(NULL);
}

IXUnknown *CPluginManager::getInterfaceByType(X_PLUGIN_TYPE type, UINT uStartFrom)
{
	UINT idx = typeToIdx(type);

	if(uStartFrom >= m_typedIFaces[idx].size())
	{
		return(NULL);
	}
	return(m_typedIFaces[idx][uStartFrom]);
}

void CPluginManager::registerInterface(const XGUID &guid, IXUnknown *pIFace)
{
	_iface iface;
	iface.pIFace = pIFace;
	m_mIFaces[guid] = iface;
}

UINT CPluginManager::typeToIdx(X_PLUGIN_TYPE type)
{
	UINT t = type;
	UINT idx = 0;
	while(t >>= 1)
	{
		++idx;
	}
	return(idx);
}
