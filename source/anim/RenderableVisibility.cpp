#include "RenderableVisibility.h"
#include "animated.h"
#include <gcore/sxgcore.h>
#include "AnimatedModelProvider.h"

extern AnimationManager * g_mgr;

CRenderableVisibility::CRenderableVisibility(ID idPlugin, CAnimatedModelProvider *pProvider):
	m_idPlugin(idPlugin),
	m_pProvider(pProvider)
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
	
	float3 vCamPos;
	pCamera->getPosition(&vCamPos);
	g_mgr->computeVis(pCamera->getFrustum(), &vCamPos, m_idVisCalcObj);
	m_pProvider->computeVisibility(pCamera->getFrustum(), vCamPos, this, pRef);
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

void CRenderableVisibility::setItemCount(UINT uCount)
{
	if(m_aItems.GetAllocSize() > uCount * 2)
	{
		m_aItems.resize(uCount);
	}
	else
	{
		m_aItems.resizeFast(uCount);
	}
}

CRenderableVisibility::item_s *CRenderableVisibility::getItem(UINT uIndex)
{
	return(&m_aItems[uIndex]);
}
