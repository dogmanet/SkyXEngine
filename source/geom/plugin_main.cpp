#include <xcommon/IXPlugin.h>
#include "Editable.h"
#include "Renderable.h"

#include "models.h"

extern CModels *g_pModels;
CRenderable *g_pRenderable = NULL;

class CStaticGeomPlugin: public IXPlugin
{
public:
	CStaticGeomPlugin()
	{
	}
	
	void XMETHODCALLTYPE startup(IXCore *pCore) override
	{
		m_pCore = pCore;

		g_pModels = m_pModels = new CModels(false);

		IEventChannel<XEventLevelSize> *pLevelSizeChannel = m_pCore->getEventChannel<XEventLevelSize>(EVENT_LEVEL_GET_SIZE_GUID);
		pLevelSizeChannel->addListener([](const XEventLevelSize *pData)
		{
			float3 vMin, vMax;
			g_pModels->getMinMax(&vMin, &vMax);

			if(pData->vMax == pData->vMin)
			{
				pData->vMax = vMax;
				pData->vMin = vMin;
			}
			else
			{
				pData->vMax = SMVectorMax(pData->vMax, vMax);
				pData->vMin = SMVectorMin(pData->vMin, vMin);
			}
		});

		IEventChannel<XEventLevel> *pLevelChannel = m_pCore->getEventChannel<XEventLevel>(EVENT_LEVEL_GUID);

		pLevelChannel->addListener([](const XEventLevel *pData)
		{
			char szPathLevel[1024];

			switch(pData->type)
			{
			case XEventLevel::TYPE_LOAD:
				g_pModels->clear();

				sprintf(szPathLevel, "%s%s/%s.geom", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), pData->szLevelName, pData->szLevelName);
				LibReport(REPORT_MSG_LEVEL_NOTICE, "loading level\n");
				//if(FileExistsFile(szPathLevel))
				{
					IEventChannel<XEventLevelProgress> *pProgressChannel = Core_GetIXCore()->getEventChannel<XEventLevelProgress>(EVENT_LEVEL_PROGRESS_GUID);
					XEventLevelProgress levelProgress;
					//@TODO: fix that value!
					levelProgress.idPlugin = 0;
					levelProgress.fProgress = 0.0f;
					levelProgress.type = XEventLevelProgress::TYPE_PROGRESS_BEGIN;
					pProgressChannel->broadcastEvent(&levelProgress);

					g_pModels->load(szPathLevel);

					levelProgress.fProgress = 1.0f;
					levelProgress.type = XEventLevelProgress::TYPE_PROGRESS_END;
					pProgressChannel->broadcastEvent(&levelProgress);
				}
				break;
			case XEventLevel::TYPE_UNLOAD:
				g_pModels->clear();
				break;
			case XEventLevel::TYPE_SAVE:
				sprintf(szPathLevel, "%s%s/%s.geom", Core_RStringGet(G_RI_STRING_PATH_GS_LEVELS), pData->szLevelName, pData->szLevelName);
				g_pModels->save(szPathLevel);
				break;
			}
		});
	}

	void XMETHODCALLTYPE shutdown() override
	{
		mem_delete(m_pModels);
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
			s_guid = IXRENDERABLE_GUID;
			break;
		case 1:
			s_guid = IXEDITABLE_GUID;
			break;
		default:
			return(NULL);
		}
		return(&s_guid);
	}
	IXUnknown * XMETHODCALLTYPE getInterface(const XGUID &guid) override
	{
		if(guid == IXEDITABLE_GUID)
		{
			return(new CEditable(m_pCore));
		}
		if(guid == IXRENDERABLE_GUID)
		{
			return((g_pRenderable = new CRenderable()));
		}
		return(NULL);
	}

protected:
	IXCore *m_pCore;

	CModels *m_pModels = NULL;
};

DECLARE_XPLUGIN(CStaticGeomPlugin);
