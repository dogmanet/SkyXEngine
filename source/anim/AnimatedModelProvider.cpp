#include "AnimatedModelProvider.h"

bool CAnimatedModelProvider::createModel(UINT uResourceCount, const IXResourceModelAnimated **ppResources, IXAnimatedModel **ppModel)
{
	const IXResourceModelAnimated *pMinPtr = ppResources[0];
	for(UINT i = 1; i < uResourceCount; ++i)
	{
		if(ppResources[i] < pMinPtr)
		{
			pMinPtr = ppResources[i];
		}
	}

	auto &aModels = m_mModels[pMinPtr];

	CAnimatedModelShared *pShared = NULL;
	for(UINT i = 0, l = aModels.size(); i < l; ++i)
	{
		if(aModels[i]->isSame(uResourceCount, ppResources))
		{
			pShared = aModels[i];
			break;
		}
	}

	if(!pShared)
	{
		pShared = new CAnimatedModelShared(this);
		aModels.push_back(pShared);
		if(!pShared->init(uResourceCount, ppResources))
		{
			delete pShared;
			return(false);
		}
	}

	CAnimatedModel *pModel = new CAnimatedModel(this, pShared);
	m_apModels.push_back(pModel);

	*ppModel = pModel;
	return(true);
}

void CAnimatedModelProvider::onSharedModelRelease(CAnimatedModelShared *pShared)
{
	auto &aModels = m_mModels[pShared->getResources()[0]];

	for(UINT i = 0, l = aModels.size(); i < l; ++i)
	{
		if(aModels[i] == pShared)
		{
			aModels.erase(i);
			return;
		}
	}

	assert(!"Something wrong! Should never get here!");
}

void CAnimatedModelProvider::onModelRelease(CAnimatedModel *pModel)
{
	for(UINT i = 0, l = m_apModels.size(); i < l; ++i)
	{
		if(m_apModels[i] == pModel)
		{
			m_apModels.erase(i);
			return;
		}
	}

	assert(!"Something wrong! Should never get here!");
}
