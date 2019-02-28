#include <xcommon/IXPlugin.h>
#include "ModelLoader.h"


class CDSEPlugin: public IXPlugin
{
public:
	void startup(IXCore *pCore)
	{
	}

	UINT getInterfaceCount()
	{
		return(1);
	}
	const XGUID *getInterfaceGUID(UINT id)
	{
		static XGUID s_guid;
		switch(id)
		{
		case 0:
			s_guid = XMODELLOADER_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown *getInterface(const XGUID &guid)
	{
		if(guid == XMODELLOADER_GUID)
		{
			return(new CModelLoader());
		}
		return(NULL);
	}
};

DECLARE_XPLUGIN(CDSEPlugin);
