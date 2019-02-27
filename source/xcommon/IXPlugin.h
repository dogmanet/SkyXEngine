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

enum X_PLUGIN_TYPE
{
	XPLUGIN_RENDERABLE = 0x00000001,
	XPLUGIN_LOADER_MODEL = 0x00000002,
	XPLUGIN_LOADER_TEXTURE = 0x00000004,
	XPLUGIN_UPDATABLE = 0x00000008,
};
DEFINE_ENUM_FLAG_OPERATORS(X_PLUGIN_TYPE);

class IXPlugin: public IXUnknown
{
public:
	virtual X_PLUGIN_TYPE getType() = 0;

	virtual IXRenderable *getIXRenderable()
	{
		return(NULL);
	}
	virtual IXModelLoader *getIXModelLoader()
	{
		return(NULL);
	}
	virtual IXTextureLoader *getIXTextureLoader()
	{
		return(NULL);
	}
	virtual IXUpdatable *getIXUpdatable()
	{
		return(NULL);
	}

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
