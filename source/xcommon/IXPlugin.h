#ifndef __IXPLUGIN_H
#define __IXPLUGIN_H

#include <core/IXCore.h>

#include "IXRenderable.h"
#include "IXModelLoader.h"
#include "IXTextureLoader.h"
#include "IXUpdatable.h"

#define X_PLUGIN_ENTRYPOINT XPluginMain
#define X_PLUGIN_API extern "C" __declspec(dllexport)
#define IXPLUGIN_VERSION 1

class IXPlugin: public IXUnknown
{
public:
	virtual UINT getInterfaceCount()
	{
		return(0);
	}
	virtual const XGUID *getInterfaceGUID(UINT id)
	{
		return(NULL);
	}
	virtual IXUnknown *getInterface(const XGUID &guid)
	{
		return(NULL);
	}

	virtual void startup(IXCore *pCore)
	{
	}
	virtual void shutdown()
	{
	}

	virtual UINT getVersion()
	{
		return(IXPLUGIN_VERSION);
	}
};

typedef IXPlugin*(*PFNXPLUGINMAIN)();

#endif
