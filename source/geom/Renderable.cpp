#include "Renderable.h"
#include "models.h"

extern CModels *g_pModels;

X_RENDER_STAGE CRenderable::getStages()
{
	return(XRS_GBUFFER);
}

UINT CRenderable::getPriorityForStage(X_RENDER_STAGE stage)
{
	return(10);
}

void CRenderable::renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility)
{
	switch(stage)
	{
	case XRS_PREPARE:
		break;
	case XRS_GBUFFER:
		g_pModels->render(0, GEOM_RENDER_TYPE_OPAQUE, 0);
		break;
	case XRS_SHADOWS:
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
		break;
	}
}

void CRenderable::startup(IGXContext *pDevice, IXMaterialSystem *pMaterialSystem)
{
	m_pDevice = pDevice;
	m_pMaterialSystem = pMaterialSystem;
}
void CRenderable::shutdown()
{
}

void CRenderable::newVisData(IXRenderableVisibility **ppVisibility)
{
	*ppVisibility = NULL;
}

IXMaterialSystem *CRenderable::getMaterialSystem()
{
	return(m_pMaterialSystem);
}
