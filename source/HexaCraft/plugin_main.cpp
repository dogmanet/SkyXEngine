#include <xcommon/IXPlugin.h>
#include "BaseBlock.h"
#include "ChunkManager.h"
#include "Updatable.h"

#if defined(_DEBUG)
#pragma comment(lib, "sxgame_d.lib")
#else
#pragma comment(lib, "sxgame.lib")
#endif


class CObserverChangedEventListener: public IEventListener<XEventObserverChanged>
{
public:
	CObserverChangedEventListener(IXCore *pCore, CChunkManager *pChunkManager):
		m_pChunkManager(pChunkManager)
	{
		m_pObserverChangedEventChannel = pCore->getEventChannel<XEventObserverChanged>(EVENT_OBSERVER_CHANGED_GUID);
		m_pObserverChangedEventChannel->addListener(this);
	}

	~CObserverChangedEventListener()
	{
		m_pObserverChangedEventChannel->removeListener(this);
	}

	void onEvent(const XEventObserverChanged *pData) override
	{
		m_pChunkManager->setCameraPos(pData->pCamera->getPosition());
	}


protected:
	IEventChannel<XEventObserverChanged> *m_pObserverChangedEventChannel = NULL;

	CChunkManager *m_pChunkManager;
};

//##########################################################################

class CLevelEventListener: public IEventListener<XEventLevel>
{
public:
	CLevelEventListener(IXCore *pCore, ID idPlugin, CChunkManager *pChunkManager):
		m_pCore(pCore),
		m_idPlugin(idPlugin),
		m_pChunkManager(pChunkManager)
	{
		m_pLevelEventChannel = m_pCore->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID);
		m_pLevelEventChannel->addListener(this);
	}
	~CLevelEventListener()
	{
		m_pLevelEventChannel->removeListener(this);
	}

	void onEvent(const XEventLevel *pData) override
	{
		//char szPathLevel[1024];

		switch(pData->type)
		{
		case XEventLevel::TYPE_LOAD:
		{
			//sprintf(szPathLevel, "levels/%s/leveldata.xml", pData->szLevelName);

			m_pChunkManager->setLevelName(pData->szLevelName);

			LibReport(REPORT_MSG_LEVEL_NOTICE, "loading level\n");
			IEventChannel<XEventLevelProgress> *pProgressChannel = m_pCore->getEventChannel<XEventLevelProgress>(EVENT_LEVEL_PROGRESS_GUID);
			XEventLevelProgress levelProgress;
			levelProgress.idPlugin = m_idPlugin;
			levelProgress.fProgress = 0.0f;
			levelProgress.szLoadingText = "Загрузка мира";
			levelProgress.type = XEventLevelProgress::TYPE_PROGRESS_BEGIN;
			pProgressChannel->broadcastEvent(&levelProgress);

			levelProgress.type = XEventLevelProgress::TYPE_PROGRESS_VALUE;
			// g_pModels->load(szPathLevel);
			m_pChunkManager->generate(pProgressChannel, &levelProgress);

			levelProgress.fProgress = 1.0f;
			levelProgress.type = XEventLevelProgress::TYPE_PROGRESS_END;
			pProgressChannel->broadcastEvent(&levelProgress);
		}
		break;
		case XEventLevel::TYPE_UNLOAD:
			m_pChunkManager->unload();
			break;
		case XEventLevel::TYPE_SAVE:
			// Not implemented
			break;
		}
	}

protected:
	IXCore *m_pCore;
	IEventChannel<XEventLevel> *m_pLevelEventChannel = NULL;
	ID m_idPlugin;
	CChunkManager *m_pChunkManager;
};

//##########################################################################

class CHexaCraftPlugin: public IXUnknownImplementation<IXPlugin>
{
public:
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
		m_pCore = pCore;

		CBlockManager::Init();

		m_pChunkManager = new CChunkManager(pCore);
		m_pUpdatable = new CUpdatable(m_pChunkManager);

		m_pListener = new CLevelEventListener(pCore, getID(), m_pChunkManager);
		m_pListener2 = new CObserverChangedEventListener(pCore, m_pChunkManager);
		
	}

	void XMETHODCALLTYPE shutdown() override
	{
		mem_delete(m_pListener);
		mem_delete(m_pListener2);

		mem_delete(m_pUpdatable);
		mem_delete(m_pChunkManager);
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
			s_guid = IXUPDATABLE_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown* XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if(guid == IXUPDATABLE_GUID)
		{
			return(m_pUpdatable);
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

	CChunkManager *m_pChunkManager;

	CUpdatable *m_pUpdatable = NULL;
	
	CLevelEventListener *m_pListener = NULL;

	CObserverChangedEventListener *m_pListener2 = NULL;
};

DECLARE_XPLUGIN(CHexaCraftPlugin);
