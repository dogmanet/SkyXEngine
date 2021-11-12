#include <xcommon/IXPlugin.h>
#include "Editable.h"

class CCSGPlugin;
class CLevelLoadListener: public IEventListener<XEventLevel>
{
public:
	CLevelLoadListener(CCSGPlugin *pPlugin):
		m_pPlugin(pPlugin)
	{
	}

	void onEvent(const XEventLevel *pData) override;

protected:
	CCSGPlugin *m_pPlugin;
};


class CCSGPlugin: public IXUnknownImplementation<IXPlugin>
{
public:
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
		m_pCore = pCore;

		m_pEventListener = new CLevelLoadListener(this);

		m_pCore->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID)->addListener(m_pEventListener);
	}

	void XMETHODCALLTYPE shutdown() override
	{
		m_pCore->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID)->removeListener(m_pEventListener);
		mem_delete(m_pEventListener);
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
			return(m_pEditable = new CEditable(m_pCore));
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


	void onLevelEvent(const XEventLevel *pData)
	{
		switch(pData->type)
		{
		case XEventLevel::TYPE_LOAD:
			SAFE_CALL(m_pEditable, load, pData->szLevelName, getID());
			break;
		case XEventLevel::TYPE_UNLOAD:
			SAFE_CALL(m_pEditable, unload);
			break;
		case XEventLevel::TYPE_SAVE:
			SAFE_CALL(m_pEditable, save, pData->szLevelName);
			break;
		}
	}


protected:
	IXCore *m_pCore;
	CLevelLoadListener *m_pEventListener = NULL;
	CEditable *m_pEditable = NULL;
};

DECLARE_XPLUGIN(CCSGPlugin);


void CLevelLoadListener::onEvent(const XEventLevel *pData)
{
	m_pPlugin->onLevelEvent(pData);
}