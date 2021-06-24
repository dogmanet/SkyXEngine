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
	CRenderableVisibility *pRef = NULL;
	if(pReference)
	{
		assert(((IXRenderableVisibility*)pReference)->getPluginId() == getPluginId());
		pRef = (CRenderableVisibility*)pReference;
	}

	IXFrustum *pFrustum = pCamera->getFrustum();
	m_pProviderAnimated->computeVisibility(pFrustum, this, pRef);
	m_pProviderDynamic->computeVisibility(pFrustum, pCamera->getLook(), this, pRef);
	mem_release(pFrustum);
}

void CRenderableVisibility::updateForFrustum(const IXFrustum *pFrustum, const IXRenderableVisibility *pReference)
{
	CRenderableVisibility *pRef = NULL;
	if(pReference)
	{
		assert(((IXRenderableVisibility*)pReference)->getPluginId() == getPluginId());
		pRef = (CRenderableVisibility*)pReference;
	}

	m_pProviderAnimated->computeVisibility(pFrustum, this, pRef);
	m_pProviderDynamic->computeVisibility(pFrustum, float3(), this, pRef);
}

static void SortRenderList(Array<CDynamicModel*> &aList)
{
	aList.quickSort([](CDynamicModel *pA, CDynamicModel *pB){
		if(pA->getShared() == pB->getShared())
		{
			return(pA->getSkin() < pB->getSkin());
		}
		return(pA->getShared() < pB->getShared());
	});
}

static void MergeArrays(Array<CDynamicModel*> &aTo, const Array<CDynamicModel*> &aFrom)
{
	if(!aFrom.size())
	{
		return;
	}
	if(!aTo.size())
	{
		aTo = aFrom;
		return;
	}

	aTo.append(aFrom);
	aTo.quickSort();

	for(UINT i = 0, l = aTo.size(); i < l - 1; ++i)
	{
		if(aTo[i] == aTo[i + 1])
		{
			if(i + 1 == l - 1)
			{
				aTo.erase(l - 1); --l;
			}
			else
			{
				if(aTo[l - 2] == aTo[l - 1])
				{
					aTo.erase(l - 1); --l;
				}
				aTo[i + 1] = aTo[l - 1];
			}

			++i;
		}
	}
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
	
	MergeArrays(m_aRenderList, pOther->m_aRenderList);
	SortRenderList(m_aRenderList);
	MergeArrays(m_aTransparentList, pOther->m_aTransparentList);
	MergeArrays(m_aSelfillumList, pOther->m_aSelfillumList);
	SortRenderList(m_aSelfillumList);

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

void CRenderableVisibility::setItemTransparentCountDynamic(UINT uCount)
{
	if(uCount > 0 && m_aItemsDynamicTransparent.GetAllocSize() > uCount * 2)
	{
		m_aItemsDynamicTransparent.resize(uCount);
	}
	else
	{
		m_aItemsDynamicTransparent.resizeFast(uCount);
	}
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

static void SetRenderList(Array<CDynamicModel*> &aList, void **ppData, UINT uCount, bool useSort=true)
{
	aList.resizeFast(uCount);
	if(uCount)
	{
		memcpy(&aList[0], ppData, sizeof(void*) * uCount);
	}

	if(useSort)
	{
		SortRenderList(aList);
	}
}

void CRenderableVisibility::setRenderList(void **ppData, UINT uCount)
{
	SetRenderList(m_aRenderList, ppData, uCount);
}

void CRenderableVisibility::setTransparentList(void **ppData, UINT uCount)
{
	SetRenderList(m_aTransparentList, ppData, uCount, false);
}

void CRenderableVisibility::setSelfillumList(void **ppData, UINT uCount)
{
	SetRenderList(m_aSelfillumList, ppData, uCount);
}

Array<CDynamicModel*>& CRenderableVisibility::getRenderList()
{
	return(m_aRenderList);
}

Array<CDynamicModel*>& CRenderableVisibility::getSelfillumList()
{
	return(m_aSelfillumList);
}

Array<CDynamicModel*>& CRenderableVisibility::getTransparentList()
{
	return(m_aTransparentList);
}
