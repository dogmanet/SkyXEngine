#ifndef __PLUGINMANAGER_H
#define __PLUGINMANAGER_H

#include "IPluginManager.h"
#include <common/assotiativearray.h>

// Интерфейс для работы с плагинами движка


class CPluginManager: public IPluginManager
{
public:
	~CPluginManager();

	void Release();

	IXPlugin *loadPlugin(const char *szPath);

	IXUnknown *getInterface(const XGUID &guid);

	IXUnknown *getInterfaceByType(X_PLUGIN_TYPE type, UINT uStartFrom = 0);

	void registerInterface(const XGUID &guid, IXUnknown *pIFace);

	void invokeStartup(IXCore *pCore);

protected:

	UINT typeToIdx(X_PLUGIN_TYPE type);

	struct _iface
	{
		IXUnknown *pIFace = NULL;
		IXPlugin *pPlugin = NULL;
	};

	Array<IXPlugin*> m_aPlugins;

	Array<IXUnknown*> m_typedIFaces[sizeof(UINT) * 8];

	AssotiativeArray<XGUID, _iface> m_mIFaces;
};

#endif
