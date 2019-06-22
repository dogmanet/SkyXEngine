#include "Renderable.h"
#include "RenderableVisibility.h"

CRenderable::CRenderable(ID idPlugin, CAnimatedModelProvider *pProviderAnimated, CDynamicModelProvider *pProviderDynamic):
	m_idPlugin(idPlugin),
	m_pAnimatedModelProvider(pProviderAnimated),
	m_pDynamicModelProvider(pProviderDynamic)
{
}

X_RENDER_STAGE CRenderable::getStages()
{
	return(XRS_GBUFFER | XRS_SHADOWS | XRS_EDITOR_2D);
}

UINT CRenderable::getPriorityForStage(X_RENDER_STAGE stage)
{
	return(20);
}

void CRenderable::renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility)
{
	CRenderableVisibility *pVis = NULL;
	if(pVisibility)
	{
		assert(pVisibility->getPluginId() == m_idPlugin);

		pVis = (CRenderableVisibility*)pVisibility;
	}

	switch(stage)
	{
	case XRS_PREPARE:
		break;
	case XRS_GBUFFER:
		m_pAnimatedModelProvider->render(pVis);
		m_pDynamicModelProvider->render(pVis);
		break;
	case XRS_SHADOWS:
		m_pAnimatedModelProvider->render(pVis);
		m_pDynamicModelProvider->render(pVis);
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
		m_pAnimatedModelProvider->render(pVis);
		m_pDynamicModelProvider->render(pVis);
		break;
	}
}

void CRenderable::startup(IGXContext *pDevice, IXMaterialSystem *pMaterialSystem)
{
	m_pDevice = pDevice;
	m_pMaterialSystem = pMaterialSystem;

	m_pAnimatedModelProvider->setDevice(pDevice);
	m_pDynamicModelProvider->setDevice(pDevice);
}
void CRenderable::shutdown()
{
}

void CRenderable::newVisData(IXRenderableVisibility **ppVisibility)
{
	*ppVisibility = new CRenderableVisibility(m_idPlugin, m_pAnimatedModelProvider, m_pDynamicModelProvider);
}

IXMaterialSystem *CRenderable::getMaterialSystem()
{
	return(m_pMaterialSystem);
}
