#include "AnimatedModelShared.h"
#include "AnimatedModelProvider.h"

CAnimatedModelShared::CAnimatedModelShared(CAnimatedModelProvider *pProvider):
	m_pProvider(pProvider)
{}
CAnimatedModelShared::~CAnimatedModelShared()
{
	m_pProvider->onSharedModelRelease(this);

	mem_delete_a(m_pBones);
}
void CAnimatedModelShared::AddRef()
{
	++m_uRefCount;
}
void CAnimatedModelShared::Release()
{
	--m_uRefCount;
	if(!m_uRefCount)
	{
		delete this;
	}
}

void CAnimatedModelShared::_collectResources(const IXResourceModelAnimated *pResource, Array<const IXResourceModelAnimated*> &aResources)
{
	for(UINT i = 0, l = aResources.size(); i < l; ++i)
	{
		if(aResources[i] == pResource)
		{
			return;
		}
	}

	m_apResources.push_back(pResource);

	const IXResourceModel *pRes = NULL;
	for(UINT i = 0, l = pResource->getImportsCount(); i < l; ++i)
	{
		pRes = pResource->getImport(i);
		if(pRes->getType() == XMT_ANIMATED)
		{
			_collectResources(pRes->asAnimated(), aResources);
		}
	}

	for(UINT i = 0, l = pResource->getPartsCount(); i < l; ++i)
	{
		pRes = pResource->getPart(i);
		if(pRes->getType() == XMT_ANIMATED)
		{
			_collectResources(pRes->asAnimated(), aResources);
		}
	}
}

bool CAnimatedModelShared::init(UINT uResourceCount, const IXResourceModelAnimated **ppResources)
{
	for(UINT i = 0; i < uResourceCount; ++i)
	{
		m_apResources.push_back(ppResources[i]);
	}
	m_apResources.quickSort([](const IXResourceModelAnimated *a, const IXResourceModelAnimated *b){
		return(a < b);
	});

	Array<const IXResourceModelAnimated*> aResources;
	for(UINT i = 0; i < uResourceCount; ++i)
	{
		aResources.push_back(ppResources[i]);
		_collectResources(ppResources[i], aResources);
	}

	// merge activities
	for(UINT i = 0, l = aResources.size(); i < l; ++i)
	{
		const IXResourceModelAnimated *pResource = aResources[i];
		for(UINT j = 0, jl = pResource->getActivitiesCount(); j < jl; ++j)
		{
			bool isFound = false;
			const char *szActivityName = pResource->getActivityName(j);
			for(UINT k = 0, kl = m_aszActivities.size(); k < kl; ++k)
			{
				if(!strcasecmp(m_aszActivities[k], szActivityName))
				{
					isFound = true;
					break;
				}
			}
			if(!isFound)
			{
				m_aszActivities.push_back(szActivityName);
			}
		}
	}

	// merge bones hierarchy
	{
		UINT uTotalBones = 0;
		for(UINT i = 0, l = aResources.size(); i < l; ++i)
		{
			uTotalBones += aResources[i]->getBoneCount();
		}

		bone_node *pNodes = (bone_node*)alloca(sizeof(bone_node)* uTotalBones);

		// build chains
		UINT uChainCount = 0;
		bone_node *pStartNode = pNodes;
		for(UINT i = 0, l = aResources.size(); i < l; ++i)
		{
			const IXResourceModelAnimated *pResource = aResources[i];
			for(UINT j = 0, jl = pResource->getBoneCount(); j < jl; ++j)
			{
				int iParent = pResource->getBoneParent(j);
				pStartNode[j] = {i, j, iParent >= 0 ? &pStartNode[iParent] : NULL, pResource->getBoneName(j), false};
				if(iParent < 0)
				{
					++uChainCount;
				}
			}
			pStartNode += pResource->getBoneCount();
		}

		for(UINT i = 0; i < uTotalBones; ++i)
		{
			if(!pNodes[i].pParent && pNodes[i].szName)
			{
				for(UINT j = 0; j < uTotalBones; ++j)
				{
					// same name found
					if(pNodes[j].pParent && pNodes[j].szName && i != j && !strcasecmp(pNodes[i].szName, pNodes[j].szName))
					{
						pNodes[i].pParent = pNodes[j].pParent;
						pNodes[i].bLooseTransform = true;
					}
				}
			}
		}
		_mergeByParent(NULL, pNodes, uTotalBones);

		// validate
		for(UINT i = 0; i < uTotalBones; ++i)
		{
			if(pNodes[i].szName)
			{
				for(UINT j = i + 1; j < uTotalBones; ++j)
				{
					// same name found
					if(pNodes[j].szName && !strcasecmp(pNodes[i].szName, pNodes[j].szName))
					{
						//@TODO: add file names to report!
						LibReport(REPORT_MSG_LEVEL_WARNING, "Unable to merge models, incompatible bones hierarchy!\n");
						return(false);
					}
				}
			}
		}
		//build final list
		m_uBoneCount = 0;
		for(UINT i = 0; i < uTotalBones; ++i)
		{
			if(pNodes[i].szName)
			{
				++m_uBoneCount;
			}
		}
		m_pBones = new bone_s[m_uBoneCount];
		// memset(m_pBones, 254, sizeof(bone_s)* uFinalBones);
		_buildBoneListByParent(NULL, pNodes, uTotalBones, m_pBones, &aResources[0], -1);
	}

	// merge skins/materials
	{
		Array<Array<const char*>> aaMaterials;
		for(UINT i = 0, l = aResources.size(); i < l; ++i)
		{
			for(UINT j = 0, jl = aResources[i]->getSkinCount(); j < jl; ++j)
			{
				for(UINT k = 0, kl = aResources[i]->getMaterialCount(); k < kl; ++k)
				{
					aaMaterials[j][k] = aResources[i]->getMaterial(k, j);
				}
			}
		}
	}

	// define parts
	Array<const IXResourceModelAnimated*> aPartedResources;
	Array<const IXResourceModelAnimated*> aPart;
	for(UINT i = 0; i < uResourceCount; ++i)
	{
		for(UINT j = 0, jl = ppResources[i]->getPartsCount(); j < jl; ++j)
		{
			const IXResourceModel *pRes = ppResources[i]->getPart(i);
			if(pRes->getType() == XMT_ANIMATED)
			{
				_collectResources(pRes->asAnimated(), aPart);
			}

			for(UINT k = 0, kl = aPart.size(); k < kl; ++k)
			{
				bool isFound = false;
				for(UINT m = 0, ml = aPartedResources.size(); m < ml; ++m)
				{
					if(aPartedResources[m] == aPart[k])
					{
						isFound = true;
						aPart.erase(k);
						--k; --kl;
						break;
					}
				}
				if(!isFound)
				{
					aPartedResources.push_back(aPart[k]);
				}
			}

			// process part;

			aPart.clearFast();
		}
	}

	// merge vertices/indices
	// merge sequences
	// merge controllers
	// merge hitboxes
	// merge physboxes

	return(true);
}

int CAnimatedModelShared::_buildBoneListByParent(bone_node *pParent, bone_node *pNodes, UINT uTotalBones, bone_s *pList, const IXResourceModelAnimated **ppResources, int iParent)
{
	int iOffset = 0;
	for(UINT i = 0; i < uTotalBones; ++i)
	{
		if(pNodes[i].pParent == pParent && pNodes[i].szName)
		{
			pList[iOffset].bone.position = ppResources[pNodes[i].uResource]->getBoneTranslation(pNodes[i].uBoneIdx);
			pList[iOffset].bone.orient = ppResources[pNodes[i].uResource]->getBoneRotation(pNodes[i].uBoneIdx);
			pList[iOffset].szName = pNodes[i].szName;
			pList[iOffset].iParent = iParent;
			++iOffset;
			iOffset += _buildBoneListByParent(&pNodes[i], pNodes, uTotalBones, pList + iOffset, ppResources, iParent + iOffset);
		}
	}
	return(iOffset);
}

void CAnimatedModelShared::_mergeByParent(bone_node *pParent, bone_node *pNodes, UINT uTotalBones)
{
	for(UINT i = 0; i < uTotalBones; ++i)
	{
		if(pNodes[i].pParent == pParent && pNodes[i].szName)
		{
			for(UINT j = 0; j < uTotalBones; ++j)
			{
				// same name found
				if(pNodes[j].pParent == pParent && pNodes[j].szName && i != j && !strcasecmp(pNodes[i].szName, pNodes[j].szName))
				{
					UINT uFrom = j, uTo = i;
					if(pNodes[i].bLooseTransform)
					{
						uFrom = i, uTo = j;
					}
					for(UINT k = j + 1; k < uTotalBones; ++k)
					{
						if(pNodes[k].pParent == &pNodes[uFrom])
						{
							pNodes[k].pParent = &pNodes[uTo];
						}
					}
					pNodes[uFrom].szName = NULL;
				}
			}
		}
	}

	//@TODO: can here be endless recursive with broken models?
	for(UINT i = 0; i < uTotalBones; ++i)
	{
		if(pNodes[i].pParent == pParent && pNodes[i].szName)
		{
			_mergeByParent(&pNodes[i], pNodes, uTotalBones);
		}
	}

}

bool CAnimatedModelShared::isSame(UINT uResourceCount, const IXResourceModelAnimated **ppResources)
{
	if(uResourceCount != m_apResources.size())
	{
		return(false);
	}

	for(UINT i = 0; i < uResourceCount; ++i)
	{
		bool isFound = false;
		for(UINT j = 0, jl = m_apResources.size(); j < jl; ++j)
		{
			if((isFound = (m_apResources[j] == ppResources[i])))
			{
				break;
			}
		}
		if(!isFound)
		{
			return(false);
		}
	}

	return(true);
}

const Array<const IXResourceModelAnimated*> &CAnimatedModelShared::getResources()
{
	return(m_apResources);
}


UINT CAnimatedModelShared::getPhysboxCount(UINT uPartIndex) const
{
	return(0);
}
const IModelPhysbox *CAnimatedModelShared::getPhysBox(UINT id, UINT uPartIndex) const
{
	return(0);
}
const IXResourceModel *CAnimatedModelShared::getResource(UINT uIndex)
{
	return(0);
}


UINT CAnimatedModelShared::getPartsCount() const
{
	return(0);
}
const char *CAnimatedModelShared::getPartName(UINT uIndex) const
{
	return(0);
}
UINT CAnimatedModelShared::getPartIndex(const char *szName)
{
	return(0);
}
XMODEL_PART_FLAGS CAnimatedModelShared::getPartFlags(UINT uIndex) const
{
	return(XMP_NONE);
}


UINT CAnimatedModelShared::getHitboxCount(UINT uPartIndex) const
{
	return(0);
}
const XResourceModelHitbox *CAnimatedModelShared::getHitbox(UINT id, UINT uPartIndex) const
{
	return(0);
}


UINT CAnimatedModelShared::getBoneId(const char *szName)
{
	for(UINT i = 0; i < m_uBoneCount; ++i)
	{
		if(!strcasecmp(m_pBones[i].szName, szName))
		{
			return(i);
		}
	}
	return(~0);
}
UINT CAnimatedModelShared::getBoneCount() const
{
	return(m_uBoneCount);
}
const char *CAnimatedModelShared::getBoneName(UINT id) const
{
	assert(id < m_uBoneCount);

	return(m_pBones[id].szName);
}

UINT CAnimatedModelShared::getControllersCount() const
{
	return(0);
}
const char *CAnimatedModelShared::getControllerName(UINT id)
{
	return(0);
}
UINT CAnimatedModelShared::getControllerId(const char *szName)
{
	return(0);
}
