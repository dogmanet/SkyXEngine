#include "Renderable.h"

CRenderable::CRenderable(ID idPlugin, CSkyBox *pSkyBox):
	m_idPlugin(idPlugin),
	m_pSkyBox(pSkyBox)
{
}

X_RENDER_STAGE XMETHODCALLTYPE CRenderable::getStages()
{
	return(XRS_GBUFFER | XRS_GI);
}

UINT XMETHODCALLTYPE CRenderable::getPriorityForStage(X_RENDER_STAGE stage)
{
	return(50);
}

void XMETHODCALLTYPE CRenderable::renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility)
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
		m_pSkyBox->render();
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

void XMETHODCALLTYPE CRenderable::startup(IGXDevice *pDevice, IXMaterialSystem *pMaterialSystem)
{
	m_pDevice = pDevice;
	m_pMaterialSystem = pMaterialSystem;

	m_pSkyBox->setDevice(pDevice);
	m_pSkyBox->setMaterialSystem(pMaterialSystem);
}
void XMETHODCALLTYPE CRenderable::shutdown()
{
	m_pSkyBox->setDevice(NULL);
}

void XMETHODCALLTYPE CRenderable::newVisData(IXRenderableVisibility **ppVisibility)
{
	*ppVisibility = NULL;
}

IXMaterialSystem* CRenderable::getMaterialSystem()
{
	return(m_pMaterialSystem);
}
