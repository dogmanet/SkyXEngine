#include <xcommon/IXPlugin.h>
#include "Editable.h"
#include "Renderable.h"


class CStaticGeomPlugin: public IXPlugin
{
public:
	CStaticGeomPlugin(ID id):
		m_id(id)
	{
	}

	ID getID()
	{
		return(m_id);
	}

	void startup(IXCore *pCore)
	{
		m_pCore = pCore;
	}

	UINT getInterfaceCount()
	{
		return(2);
	}
	const XGUID *getInterfaceGUID(UINT id)
	{
		static XGUID s_guid;
		switch(id)
		{
		case 0:
			s_guid = IXEDITABLE_GUID;
			break;
		case 1:
			s_guid = IXRENDERABLE_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown *getInterface(const XGUID &guid)
	{
		if(guid == IXEDITABLE_GUID)
		{
			return(new CEditable(m_pCore));
		}
		if(guid == IXRENDERABLE_GUID)
		{
			return(new CRenderable());
		}
		return(NULL);
	}

protected:

	IXCore *m_pCore;
	ID m_id;
};

DECLARE_XPLUGIN(CStaticGeomPlugin);
