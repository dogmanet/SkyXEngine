#include <xcommon/IXPlugin.h>
#include "GUI.h"
#include <gui2/CSSLexer.h>

#include "CSSProperty.h""

class CGUIPlugin: public IXUnknownImplementation<IXPlugin>
{
public:
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
		m_pCore = pCore;
		CCSSLexer lexer;
		lexer.read("<>");
	}

	void XMETHODCALLTYPE shutdown() override
	{
	}

	UINT XMETHODCALLTYPE getInterfaceCount() override
	{
		return(0);
	}
	const XGUID* XMETHODCALLTYPE getInterfaceGUID(UINT id) override
	{
		static XGUID s_guid;
		switch(id)
		{
		case 0:
			s_guid = IXGUI_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown* XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if(guid == IXGUI_GUID)
		{
			return(new CGUI());
		}
		return(NULL);
	}

protected:
	IXCore *m_pCore;
};

DECLARE_XPLUGIN(CGUIPlugin);
