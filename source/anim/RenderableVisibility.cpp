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

ID CRenderableVisibility::getPluginId() const
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
		assert(((IXRenderableVisibility*)pReference)->getPluginId() == getPluginId());
		pRef = (CRenderableVisibility*)pReference;
	}

	m_pProviderAnimated->computeVisibility(pFrustum, this, pRef);
	m_pProviderDynamic->computeVisibility(pFrustum, this, pRef);
}

void CRenderableVisibility::append(const IXRenderableVisibility *pOther_)
{
	assert(((IXRenderableVisibility*)pOther_)->getPluginId() == getPluginId());
	const CRenderableVisibility *pOther = (const CRenderableVisibility*)pOther_;

	item_s *pItem;
	const item_s *pOtherItem;
	for(UINT i = 0, l = pOther->m_aItems.size(); i < l; ++i)
	{
		pOtherItem = &pOther->m_aItems[i];
		if(pOtherItem->isVisible)
		{
			pItem = &m_aItems[i];
			if(pItem->isVisible)
			{
				if(pOtherItem->uLod < pItem->uLod)
				{
					pItem->uLod = pOtherItem->uLod;
				}
			}
			else
			{
				pItem->isTransparent = pOtherItem->isTransparent;
				pItem->isEmissive = pOtherItem->isEmissive;
				pItem->isVisible = true;
				pItem->uLod = pOtherItem->uLod;
			}
		}
	}

	for(UINT i = 0, l = pOther->m_aItemsDynamic.size(); i < l; ++i)
	{
		pOtherItem = &pOther->m_aItemsDynamic[i];
		if(pOtherItem->isVisible)
		{
			pItem = &m_aItemsDynamic[i];
			if(pItem->isVisible)
			{
				if(pOtherItem->uLod < pItem->uLod)
				{
					pItem->uLod = pOtherItem->uLod;
				}
			}
			else
			{
				pItem->isTransparent = pOtherItem->isTransparent;
				pItem->isVisible = true;
				pItem->uLod = pOtherItem->uLod;
			}
		}
	}

	//! @todo implement for transparency too!
}

void CRenderableVisibility::substract(const IXRenderableVisibility *pOther_)
{
	assert(((IXRenderableVisibility*)pOther_)->getPluginId() == getPluginId());
	const CRenderableVisibility *pOther = (const CRenderableVisibility*)pOther_;

	item_s *pItem;
	const item_s *pOtherItem;
	for(UINT i = 0, l = pOther->m_aItems.size(); i < l; ++i)
	{
		pOtherItem = &pOther->m_aItems[i];
		pItem = &m_aItems[i];
		if(pItem->isVisible && pOtherItem->isVisible)
		{
			pItem->isVisible = false;
		}
	}

	for(UINT i = 0, l = pOther->m_aItemsDynamic.size(); i < l; ++i)
	{
		pOtherItem = &pOther->m_aItemsDynamic[i];
		pItem = &m_aItemsDynamic[i];
		if(pItem->isVisible && pOtherItem->isVisible)
		{
			pItem->isVisible = false;
		}
	}

	//! @todo implement for transparency too!
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

CRenderableVisibility::item_s* CRenderableVisibility::getItem(UINT uIndex)
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

CRenderableVisibility::item_s* CRenderableVisibility::getItemDynamic(UINT uIndex, bool forceCreate)
{
	if(forceCreate || uIndex < m_aItemsDynamic.size())
	{
		return(&m_aItemsDynamic[uIndex]);
	}
	return(NULL);
}

CRenderableVisibility::TransparentModel* CRenderableVisibility::getItemTransparentDynamic(UINT uIndex)
{
	return(&m_aItemsDynamicTransparent[uIndex]);
}

UINT CRenderableVisibility::getItemTransparentDynamicCount()
{
	return(m_aItemsDynamicTransparent.size());
}

void CRenderableVisibility::resetItemTransparentDynamic()
{
	m_aItemsDynamicTransparent.clearFast();
}

void CRenderableVisibility::addItemTransparentDynamic(const TransparentModel &mdl)
{
	m_aItemsDynamicTransparent.push_back(mdl);
}
