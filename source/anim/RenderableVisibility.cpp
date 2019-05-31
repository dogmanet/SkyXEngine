#include "RenderableVisibility.h"
#include <gcore/sxgcore.h>
#include "AnimatedModelProvider.h"
#include "DynamicModelProvider.h"

CRenderableVisibility::CRenderableVisibility(ID idPlugin, CAnimatedModelProvider *pProviderAnimated, CDynamicModelProvider *pProviderDynamic):
	m_idPlugin(idPlugin),
	m_pProviderAnimated(pProviderAnimated),
	m_pProviderDynamic(pProviderDynamic)
{
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
	updateForFrustum(pCamera->getFrustum(), pReference);
}

void CRenderableVisibility::updateForFrustum(const IFrustum *pFrustum, const IXRenderableVisibility *pReference)
{
	CRenderableVisibility *pRef = NULL;
	if(pReference)
	{
		assert(((IXRenderableVisibility*)pReference)->getPluginId() == -1);
		pRef = (CRenderableVisibility*)pReference;
	}

	m_pProviderAnimated->computeVisibility(pFrustum, this, pRef);
	m_pProviderDynamic->computeVisibility(pFrustum, this, pRef);
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

void CRenderableVisibility::setItemCountDynamic(UINT uCount)
{
	if(m_aItemsDynamic.GetAllocSize() > uCount * 2)
	{
		m_aItemsDynamic.resize(uCount);
	}
	else
	{
		m_aItemsDynamic.resizeFast(uCount);
	}
}

CRenderableVisibility::item_s *CRenderableVisibility::getItemDynamic(UINT uIndex)
{
	return(&m_aItemsDynamic[uIndex]);
}
