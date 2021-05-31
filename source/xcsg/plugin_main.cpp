#include <xcommon/IXPlugin.h>
#include "Editable.h"

class CCSGPlugin: public IXUnknownImplementation<IXPlugin>
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
	const XGUID* XMETHODCALLTYPE getInterfaceGUID(UINT id) override
	{
		static XGUID s_guid;
		switch(id)
		{
		case 0:
			s_guid = IXEDITABLE_GUID;
			break;
		//case 1:
		//	s_guid = IXMATERIALPROXY_GUID;
		//	break;
		//case 2:
		//	s_guid = IXMATERIALLOADER_GUID;
		//	break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown* XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if(guid == IXEDITABLE_GUID)
		{
			return(new CEditable(m_pCore));
		}
		//if(guid == IXMATERIALPROXY_GUID)
		//{
		//	return(new CMaterialProxy(m_pCore->getFileSystem()));
		//}
		//if(guid == IXMATERIALLOADER_GUID)
		//{
		//	return(new CMaterialLoader(m_pCore));
		//}
		return(NULL);
	}

protected:
	IXCore *m_pCore;
};

DECLARE_XPLUGIN(CCSGPlugin);
