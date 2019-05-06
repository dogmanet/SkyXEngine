#include <xcommon/IXPlugin.h>
#include "LightSystem.h"


class CLightSystemPlugin: public IXPlugin
{
public:
	CLightSystemPlugin(ID id):
		m_id(id)
	{}

	void startup(IXCore *pCore)
	{
		m_pCore = pCore;
	}

	ID getID()
	{
		return(m_id);
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
			s_guid = IXLIGHTSYSTEM_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown *getInterface(const XGUID &guid)
	{
		if(guid == IXLIGHTSYSTEM_GUID)
		{
			return(new CLightSystem());
		}
		return(NULL);
	}

protected:

	IXCore *m_pCore;
	ID m_id;
};

DECLARE_XPLUGIN(CLightSystemPlugin);
