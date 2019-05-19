#include <xcommon/IXPlugin.h>
#include "ModelLoader.h"


class CDSEPlugin: public IXPlugin
{
public:
	CDSEPlugin(ID id):
		m_id(id)
	{
		// Core_SetOutPtr();
	}

	ID getID()
	{
		return(m_id);
	}

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

protected:
	ID m_id;
};

DECLARE_XPLUGIN(CDSEPlugin);
