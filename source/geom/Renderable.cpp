#include "Renderable.h"
#include "models.h"
#include "RenderableVisibility.h"

extern CModels *g_pModels;
IGXContext *g_pDXDevice = NULL;

X_RENDER_STAGE CRenderable::getStages()
{
	return(XRS_GBUFFER | XRS_SHADOWS | XRS_EDITOR_2D);
}

UINT CRenderable::getPriorityForStage(X_RENDER_STAGE stage)
{
	return(10);
}

void CRenderable::renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility)
{
	ID idVisCalcObj = -1;
	if(pVisibility)
	{
		assert(pVisibility->getPluginId() == 0);

		idVisCalcObj = ((CRenderableVisibility*)pVisibility)->getVisCalcObjId();
	}

	switch(stage)
	{
	case XRS_PREPARE:
		break;
	case XRS_GBUFFER:
		g_pModels->render(0, GEOM_RENDER_TYPE_OPAQUE, idVisCalcObj);
		break;
	case XRS_SHADOWS:
		g_pModels->render(0, GEOM_RENDER_TYPE_OPAQUE, idVisCalcObj);
		break;
	case XRS_GI:
		break;
	case XRS_POSTPROCESS_MAIN:
		break;
	case XRS_TRANSPARENT:
		break;
	case XRS_POSTPROCESS_FINAL:
		break;
	case XRS_EDITOR_2D:
		g_pModels->render(0, GEOM_RENDER_TYPE_OPAQUE, idVisCalcObj);
		break;
	}
}

void CRenderable::startup(IGXContext *pDevice, IXMaterialSystem *pMaterialSystem)
{
	m_pDevice = pDevice;
	m_pMaterialSystem = pMaterialSystem;

	g_pDXDevice = pDevice;
}
void CRenderable::shutdown()
{
}

void CRenderable::newVisData(IXRenderableVisibility **ppVisibility)
{
	*ppVisibility = new CRenderableVisibility(0);
}

IXMaterialSystem *CRenderable::getMaterialSystem()
{
	return(m_pMaterialSystem);
}
