#include "Renderable.h"

CRenderable::CRenderable(ID idPlugin, CParticleSystem *pParticleSystem):
	m_idPlugin(idPlugin),
	m_pParticleSystem(pParticleSystem)
{
}

X_RENDER_STAGE XMETHODCALLTYPE CRenderable::getStages()
{
	return(XRS_GBUFFER);
}

UINT XMETHODCALLTYPE CRenderable::getPriorityForStage(X_RENDER_STAGE stage)
{
	return(60);
}

void XMETHODCALLTYPE CRenderable::renderStage(X_RENDER_STAGE stage, IXRenderableVisibility *pVisibility)
{
	switch(stage)
	{
	case XRS_PREPARE:
		break;
	case XRS_GBUFFER:
		m_pParticleSystem->render(pVisibility);
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

UINT XMETHODCALLTYPE CRenderable::getTransparentCount(IXRenderableVisibility *pVisibility)
{
	return(0);
}
void XMETHODCALLTYPE CRenderable::getTransparentObject(IXRenderableVisibility *pVisibility, UINT uIndex, XTransparentObject *pObject)
{
}
void XMETHODCALLTYPE CRenderable::renderTransparentObject(IXRenderableVisibility *pVisibility, UINT uIndex, UINT uSplitPlanes)
{
}

void XMETHODCALLTYPE CRenderable::startup(IXRender *pRender, IXMaterialSystem *pMaterialSystem)
{
	m_pDevice = pRender->getDevice();
	m_pMaterialSystem = pMaterialSystem;

	m_pParticleSystem->setDevice(m_pDevice);
	m_pParticleSystem->setMaterialSystem(pMaterialSystem);
}
void XMETHODCALLTYPE CRenderable::shutdown()
{
	m_pParticleSystem->setDevice(NULL);
}

void XMETHODCALLTYPE CRenderable::newVisData(IXRenderableVisibility **ppVisibility)
{
	m_pParticleSystem->newVisData(ppVisibility);
}

IXMaterialSystem* CRenderable::getMaterialSystem()
{
	return(m_pMaterialSystem);
}
