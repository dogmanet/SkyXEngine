#include <xcommon/IXPlugin.h>
#include "SkyBox.h"
#include "Renderable.h"
#include "Updatable.h"

class CLevelLoadListener: public IEventListener<XEventLevel>
{
public:
	CLevelLoadListener(IXCore *pCore, ID idPlugin, CSkyBox *pSkyBox):
		m_pCore(pCore),
		m_idPlugin(idPlugin),
		m_pSkyBox(pSkyBox)
	{
		//m_pCore->getRenderPipeline();
		//IXRenderPipeline *pPipeline;
		//pPipeline->
	}
	void onEvent(const XEventLevel *pData) override
	{
		char szPathLevel[1024];

		switch(pData->type)
		{
		case XEventLevel::TYPE_LOAD:
			sprintf(szPathLevel, "levels/%s/%s.env", pData->szLevelName, pData->szLevelName);
			LibReport(REPORT_MSG_LEVEL_NOTICE, "loading level\n");
			//if(FileExistsFile(szPathLevel))
			{
				IEventChannel<XEventLevelProgress> *pProgressChannel = m_pCore->getEventChannel<XEventLevelProgress>(EVENT_LEVEL_PROGRESS_GUID);
				XEventLevelProgress levelProgress;
				levelProgress.idPlugin = m_idPlugin;
				levelProgress.fProgress = 0.0f;
				levelProgress.szLoadingText = "Загрузка окружения";
				levelProgress.type = XEventLevelProgress::TYPE_PROGRESS_BEGIN;
				pProgressChannel->broadcastEvent(&levelProgress);

				m_pSkyBox->setTexture("sky_hdr_hl2");

				levelProgress.fProgress = 1.0f;
				levelProgress.type = XEventLevelProgress::TYPE_PROGRESS_END;
				pProgressChannel->broadcastEvent(&levelProgress);
			}
			break;
		case XEventLevel::TYPE_UNLOAD:
			break;
		case XEventLevel::TYPE_SAVE:
			break;
		}
	}

protected:
	IXCore *m_pCore;
	ID m_idPlugin;
	CSkyBox *m_pSkyBox;
};

class CAmbientPlugin: public IXPlugin
{
public:	
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
		m_pCore = pCore;

		m_pSkyBox = new CSkyBox();
		m_pRenderable = new CRenderable(getID(), m_pSkyBox);
		m_pUpdatable = new CUpdatable(m_pSkyBox);

		m_pEventListener = new CLevelLoadListener(pCore, getID(), m_pSkyBox);
		m_pCore->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID)->addListener(m_pEventListener);

	}

	void XMETHODCALLTYPE shutdown() override
	{
		m_pCore->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID)->removeListener(m_pEventListener);
		mem_delete(m_pEventListener);
		mem_delete(m_pRenderable);
		mem_delete(m_pUpdatable);
		mem_delete(m_pSkyBox);
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
			s_guid = IXUPDATABLE_GUID;
			break;
		case 1:
			s_guid = IXRENDERABLE_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown* XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if(guid == IXRENDERABLE_GUID)
		{
			return(m_pRenderable);
		}
		if(guid == IXUPDATABLE_GUID)
		{
			return(m_pUpdatable);
		}
		return(NULL);
	}

protected:
	IXCore *m_pCore = NULL;
	CLevelLoadListener *m_pEventListener = NULL;

	CSkyBox *m_pSkyBox = NULL;
	CRenderable *m_pRenderable = NULL;
	CUpdatable *m_pUpdatable = NULL;

};

DECLARE_XPLUGIN(CAmbientPlugin);
