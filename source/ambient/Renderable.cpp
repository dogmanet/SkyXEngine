#include "Renderable.h"

CRenderable::CRenderable(ID idPlugin, CSkyBox *pSkyBox):
	m_idPlugin(idPlugin),
	m_pSkyBox(pSkyBox)
{
}

X_RENDER_STAGE CRenderable::getStages()
{
	return(XRS_GBUFFER);
}

UINT CRenderable::getPriorityForStage(X_RENDER_STAGE stage)
{
	return(50);
}

void CRenderable::renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility)
{
	UNREFERENCED_PARAMETER(pVisibility);

	switch(stage)
	{
	case XRS_PREPARE:
		break;
	case XRS_GBUFFER:
		m_pSkyBox->render();
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

	m_pSkyBox->setDevice(pDevice);
	m_pSkyBox->setMaterialSystem(pMaterialSystem);
}
void CRenderable::shutdown()
{
	m_pSkyBox->setDevice(NULL);
}

void CRenderable::newVisData(IXRenderableVisibility **ppVisibility)
{
	*ppVisibility = NULL;
}

IXMaterialSystem *CRenderable::getMaterialSystem()
{
	return(m_pMaterialSystem);
}
