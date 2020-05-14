#include <xcommon/IXPlugin.h>
#include "LightSystem.h"


class CLightSystemPlugin: public IXUnknownImplementation<IXPlugin>
{
public:
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
		m_pCore = pCore;
	}
	void XMETHODCALLTYPE shutdown() override
	{
	}

	UINT XMETHODCALLTYPE getInterfaceCount() override
	{
		return(1);
	}
	const XGUID * XMETHODCALLTYPE getInterfaceGUID(UINT id) override
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
	IXUnknown * XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if(guid == IXLIGHTSYSTEM_GUID)
		{
			return(new CLightSystem(m_pCore));
		}
		return(NULL);
	}

protected:
	IXCore *m_pCore;
};

DECLARE_XPLUGIN(CLightSystemPlugin);
