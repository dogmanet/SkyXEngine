#ifndef __IXPLUGIN_H
#define __IXPLUGIN_H

#include "IXCore.h"

#define X_PLUGIN_ENTRYPOINT XPluginMain
#define X_PLUGIN_API extern "C" __declspec(dllexport)
#define IXPLUGIN_VERSION 1
#define DECLARE_XPLUGIN(cls) X_PLUGIN_API IXPlugin *X_PLUGIN_ENTRYPOINT(ID id){return(new cls(id));}

class IXPlugin: public IXUnknown
{
public:
	virtual UINT getVersion()
	{
		return(IXPLUGIN_VERSION);
	}
	virtual ID getID() = 0;

	virtual UINT getInterfaceCount() = 0;
	virtual const XGUID *getInterfaceGUID(UINT id) = 0;
	virtual IXUnknown *getInterface(const XGUID &guid) = 0;

	virtual void startup(IXCore *pCore)
	{
	}
	virtual void shutdown()
	{
	}
};

typedef IXPlugin*(*PFNXPLUGINMAIN)(ID id);

#endif
