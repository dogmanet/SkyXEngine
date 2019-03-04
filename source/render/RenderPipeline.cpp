#include "RenderPipeline.h"

#include <core/sxcore.h>

CRenderPipeline::CRenderPipeline(IGXContext *pDevice):
	m_pDevice(pDevice)
{
	// load renderables
	IXCore *pCore = Core_GetIXCore();
	IPluginManager *pPluginManager = pCore->getPluginManager();
	IXMaterialSystem *pMaterialSystem = (IXMaterialSystem*)pPluginManager->getInterface(IXMATERIALSYSTEM_GUID);

	IXRenderable *pRenderable;
	UINT ic = 0;
	while((pRenderable = (IXRenderable*)pPluginManager->getInterface(IXRENDERABLE_GUID, ic++)))
	{
		if(pRenderable->getVersion() == IXRENDERABLE_VERSION)
		{
			pRenderable->startup(m_pDevice, pMaterialSystem);
			X_RENDER_STAGE stages = pRenderable->getStages();

			for(UINT idx = 0; idx < sizeof(UINT) * 8; ++idx)
			{
				X_RENDER_STAGE stage = (X_RENDER_STAGE)(1 << idx);

				if(stages & stage)
				{
					_render_sys rs;
					rs.pRenderable = pRenderable;
					rs.uPriority = pRenderable->getPriorityForStage(stage);

					m_apRenderStages[idx].aSystems.push_back(rs);
				}
			}
		}
	}

	for(UINT i = 0, l = m_apRenderStages.size(); i < l; ++i)
	{
		m_apRenderStages[i].aSystems.quickSort([](const _render_sys &a, const _render_sys &b)
		{
			return(a.uPriority < b.uPriority);
		});
	}
}
CRenderPipeline::~CRenderPipeline()
{
}

void CRenderPipeline::renderPrepare()
{
}
void CRenderPipeline::renderGBuffer()
{
	// set required states
	
	auto &list = m_apRenderStages[getIndexForStage(XRS_GBUFFER)].aSystems;

	for(UINT i = 0, l = list.size(); i < l; ++i)
	{
		list[i].pRenderable->renderStage(XRS_GBUFFER, NULL);
	}
}
void CRenderPipeline::renderShadows()
{
}
void CRenderPipeline::renderGI()
{
}
void CRenderPipeline::renderPostprocessMain()
{
}
void CRenderPipeline::renderTransparent()
{
}
void CRenderPipeline::renderPostprocessFinal()
{
}
void CRenderPipeline::renderEditor2D()
{
}

//##########################################################################

UINT CRenderPipeline::getIndexForStage(X_RENDER_STAGE stage)
{
	UINT idx = 0;
	UINT stg = stage;
	while(stg >>= 1)
	{
		idx++;
	}
	return(idx);
}
