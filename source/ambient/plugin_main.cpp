#include <xcommon/IXPlugin.h>
#include "Ambient.h"
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

class CAmbientPlugin: public IXUnknownImplementation<IXPlugin>
{
public:	
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
		m_pCore = pCore;

		m_pAmbient = new CAmbient(pCore);
		m_pRenderable = new CRenderable(getID(), m_pAmbient);
		m_pUpdatable = new CUpdatable(m_pAmbient);

	//	m_pEventListener = new CLevelLoadListener(pCore, getID(), m_pSkyBox);
	//	m_pCore->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID)->addListener(m_pEventListener);

	}

	void XMETHODCALLTYPE shutdown() override
	{
	//	m_pCore->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID)->removeListener(m_pEventListener);
		mem_delete(m_pEventListener);
		mem_delete(m_pRenderable);
		mem_delete(m_pUpdatable);
		mem_delete(m_pAmbient);
	}

	UINT XMETHODCALLTYPE getInterfaceCount() override
	{
		return(3);
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
		case 2:
			s_guid = IXAMBIENT_GUID;
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
		if(guid == IXAMBIENT_GUID)
		{
			return(m_pAmbient);
		}
		return(NULL);
	}

protected:
	IXCore *m_pCore = NULL;
	CLevelLoadListener *m_pEventListener = NULL;

	CAmbient *m_pAmbient = NULL;
	CRenderable *m_pRenderable = NULL;
	CUpdatable *m_pUpdatable = NULL;

};

DECLARE_XPLUGIN(CAmbientPlugin);
