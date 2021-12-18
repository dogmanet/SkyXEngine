#include <xcommon/IXPlugin.h>
#include "ModelLoader.h"
#include "ModelWriter.h"


class CDSEPlugin: public IXUnknownImplementation<IXPlugin>
{
public:
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
	}

	void XMETHODCALLTYPE shutdown() override
	{
	}

	UINT XMETHODCALLTYPE getInterfaceCount() override
	{
		return(2);
	}
	const XGUID* XMETHODCALLTYPE getInterfaceGUID(UINT id) override
	{
		static XGUID s_guid;
		switch(id)
		{
		case 0:
			s_guid = IXMODELLOADER_GUID;
			break;
		case 1:
			s_guid = IXMODELWRITER_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown* XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if(guid == IXMODELLOADER_GUID)
		{
			return(new CModelLoader());
		}
		else if(guid == IXMODELWRITER_GUID)
		{
			return(new CModelWriter());
		}
		return(NULL);
	}
};

DECLARE_XPLUGIN(CDSEPlugin);
