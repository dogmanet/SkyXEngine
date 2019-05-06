#include "RenderableVisibility.h"
#include "animated.h"
#include <gcore/sxgcore.h>

extern AnimationManager * g_mgr;

CRenderableVisibility::CRenderableVisibility(ID idPlugin):
	m_idPlugin(idPlugin)
{
	m_idVisCalcObj = g_mgr->getNextVisId();
	assert(ID_VALID(m_idVisCalcObj));
}

CRenderableVisibility::~CRenderableVisibility()
{
	g_mgr->freeVisID(m_idVisCalcObj);
}

ID CRenderableVisibility::getPluginId()
{
	return(m_idPlugin);
}

void CRenderableVisibility::setOcclusionCuller(IXOcclusionCuller *pOcclusionCuller)
{
	assert(!"Not implemented!");
}

void CRenderableVisibility::updateForCamera(ICamera *pCamera, const IXRenderableVisibility *pReference)
{
	CRenderableVisibility *pRef = NULL;
	if(pReference)
	{
		assert(((IXRenderableVisibility*)pReference)->getPluginId() == getPluginId());
		pRef = (CRenderableVisibility*)pReference;
	}

	assert(!pRef && "Referencing is not supported!");

	float3 vCamPos;
	pCamera->getPosition(&vCamPos);
	g_mgr->computeVis(pCamera->getFrustum(), &vCamPos, m_idVisCalcObj);
}

void CRenderableVisibility::updateForFrustum(IFrustum *pFrustum, const IXRenderableVisibility *pReference)
{
	CRenderableVisibility *pRef = NULL;
	if(pReference)
	{
		assert(((IXRenderableVisibility*)pReference)->getPluginId() == -1);
		pRef = (CRenderableVisibility*)pReference;
	}

	assert(!pRef && "Referencing is not supported!");

	assert(!"Not implemented!");
}
