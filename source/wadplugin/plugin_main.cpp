#include <xcommon/IXPlugin.h>
#include "TextureLoader.h"
#include "TextureProxy.h"


class CWADPlugin: public IXUnknownImplementation<IXPlugin>
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
		return(2);
	}
	const XGUID* XMETHODCALLTYPE getInterfaceGUID(UINT id) override
	{
		static XGUID s_guid;
		switch(id)
		{
		case 0:
			s_guid = IXTEXTURELOADER_GUID;
			break;
		case 1:
			s_guid = IXTEXTUREPROXY_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown* XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if(guid == IXTEXTURELOADER_GUID)
		{
			return(new CTextureLoader(m_pCore->getFileSystem()));
		}
		if(guid == IXTEXTUREPROXY_GUID)
		{
			return(new CTextureProxy(m_pCore->getFileSystem()));
		}
		return(NULL);
	}

protected:
	IXCore *m_pCore;
};

DECLARE_XPLUGIN(CWADPlugin);
