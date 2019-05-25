#include "AnimatedModelShared.h"
#include "AnimatedModelProvider.h"
#include <mtrl/IXMaterialSystem.h>

CAnimatedModelShared::CAnimatedModelShared(CAnimatedModelProvider *pProvider):
	m_pProvider(pProvider)
{}
CAnimatedModelShared::~CAnimatedModelShared()
{
	m_pProvider->onSharedModelRelease(this);

	mem_delete_a(m_pBones);

	for(UINT i = 0; i < m_uSkinCount; ++i)
	{
		for(UINT j = 0; j < m_uMaterialCount; ++j)
		{
			mem_release(m_pppMaterials[i][j]);
		}
	}

	mem_delete_a(m_ppMaterialsBlob);
	m_pppMaterials = NULL;
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
		Array<Array<mtl_node>> aaMaterials;
		UINT uTotalMaterials = 0;
		for(UINT i = 0, l = aResources.size(); i < l; ++i)
		{
			UINT uSkinCount = aResources[i]->getSkinCount();
			aaMaterials.reserve(uSkinCount);
			for(UINT j = aaMaterials.size(); j < uSkinCount; ++j)
			{
				aaMaterials[j].resize(uTotalMaterials);
			}
			for(UINT k = 0, kl = aResources[i]->getMaterialCount(); k < kl; ++k)
			{
				Array<mtl_node> &aFirstSkin = aaMaterials[0];
				const char *szMaterial = aResources[i]->getMaterial(k);
				bool isFound = false;
				for(UINT j = 0, jl = aFirstSkin.size(); j < jl; ++j)
				{
					// same name, test skins
					if(aFirstSkin[j].szName && !fstrcmp(aFirstSkin[j].szName, szMaterial))
					{
						isFound = true;
						// start from 1 as 0 slot already tested
						for(UINT m = 1; m < uSkinCount; ++m)
						{
							const char *szSkin = aResources[i]->getMaterial(k, m);
							if(!((aaMaterials[m][j].szName && !fstrcmp(aaMaterials[m][j].szName, szSkin))
								|| ((!aaMaterials[m][j].szName || !aaMaterials[m][j].szName[0]) && !szSkin[0])))
							{
								isFound = false;
								break;
							}
						}
						if(isFound)
						{
							break;
						}
					}
				}
				if(!isFound)
				{
					for(UINT j = 0; j < uSkinCount; ++j)
					{
						mtl_node node;
						node.szName = aResources[i]->getMaterial(k, j);
						node.uResource = i;
						aaMaterials[j][uTotalMaterials] = node;
					}
					++uTotalMaterials;
				}
			}
		}

		m_uMaterialCount = uTotalMaterials;
		m_uSkinCount = aaMaterials.size();
		m_ppMaterialsBlob = new void*[m_uMaterialCount * m_uSkinCount + m_uSkinCount];
		m_pppMaterials = (IXMaterial***)m_ppMaterialsBlob;
		IXMaterialSystem *pMaterialSystem = m_pProvider->getMaterialSystem();
		XSHADER_DEFAULT_DESC shaderDesc;
		shaderDesc.szFileVS = "mtrlskin_base.vs";
		shaderDesc.szFilePS = "mtrlgeom_base.ps";

		for(UINT i = 0; i < m_uSkinCount; ++i)
		{
			m_pppMaterials[i] = (IXMaterial**)(m_ppMaterialsBlob + m_uSkinCount + m_uMaterialCount * i);

			for(UINT j = 0; j < m_uMaterialCount; ++j)
			{
				if(aaMaterials[i][j].szName && aaMaterials[i][j].szName[0])
				{
					pMaterialSystem->loadMaterial(aaMaterials[i][j].szName, &m_pppMaterials[i][j], &shaderDesc);
				}
				else
				{
					m_pppMaterials[i][j] = m_pppMaterials[0][j];
					m_pppMaterials[i][j]->AddRef();
				}
			}
		}
	}

	// define parts
	Array<const IXResourceModelAnimated*> aPartedResources;
	Array<const IXResourceModelAnimated*> aPart;
	m_aParts[0].szName = "";
	m_aParts[0].flags = XMP_NONE;
	for(UINT i = 0; i < uResourceCount; ++i)
	{
		for(UINT j = 0, jl = ppResources[i]->getPartsCount(); j < jl; ++j)
		{
			const IXResourceModel *pRes = ppResources[i]->getPart(j);
			XMODEL_IMPORT importFlags = ppResources[i]->getPartImportFlags(j);
			//@TODO: Test XMODEL_IMPORT flags
			if(pRes->getType() == XMT_ANIMATED)
			{
				aPart.push_back(pRes->asAnimated());
				_collectResources(pRes->asAnimated(), aPart);


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

				XPT_TOPOLOGY pt = aPart[0]->getPrimitiveTopology();
				for(UINT k = 1, kl = aPart.size(); k < kl; ++k)
				{
					if((importFlags & XMI_MESH) && (aPart[k]->getPrimitiveTopology() != pt))
					{
						pt = XPT_TRIANGLELIST;
						break;
					}
				}
				
				// process part:
				part_s part;
				part.topology = pt;
				part.szName = ppResources[i]->getPartName(j);
				part.flags = ppResources[i]->getPartFlags(j);
				for(UINT k = 0, kl = aPart.size(); k < kl; ++k)
				{
					// merge vertices/indices
					for(UINT m = 0, ml = aPart[k]->getLodCount(); m < ml; ++m)
					{
						for(UINT n = 0, nl = aPart[k]->getSubsetCount(m); n < nl; ++n)
						{
							const XResourceModelAnimatedSubset *pSubset = aPart[k]->getSubset(m, n);
							int uid = pSubset->iIndexCount;
						}
					}

					// merge hitboxes
					for(UINT m = 0, ml = aPart[k]->getHitboxCount(); m < ml; ++m)
					{
						part.aHitboxes.push_back(aPart[k]->getHitbox(m));
					}

					// merge physboxes
					for(UINT m = 0, ml = aPart[k]->getPhysboxCount(); m < ml; ++m)
					{
						int iBone = aPart[k]->getPhysboxBone(m);
						if(iBone >= 0)
						{
							iBone = getBoneId(aPart[k]->getBoneName(iBone));
						}
						part.aPhysboxes.push_back({aPart[k]->getPhysbox(m), iBone});
					}
				}

				m_aParts.push_back(part);

				aPart.clearFast();
			}
		}
	}

	
	// merge sequences
	// merge controllers

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
	assert(uPartIndex < m_aParts.size());

	return(m_aParts[uPartIndex].aPhysboxes.size());
}
const IModelPhysbox *CAnimatedModelShared::getPhysBox(UINT id, UINT uPartIndex) const
{
	assert(uPartIndex < m_aParts.size() && id < m_aParts[uPartIndex].aPhysboxes.size());

	return(m_aParts[uPartIndex].aPhysboxes[id].pPhysbox);
}
const int CAnimatedModelShared::getPhysBoxBone(UINT id, UINT uPartIndex) const
{
	assert(uPartIndex < m_aParts.size() && id < m_aParts[uPartIndex].aPhysboxes.size());

	return(m_aParts[uPartIndex].aPhysboxes[id].iBone);
}
const IXResourceModel *CAnimatedModelShared::getResource(UINT uIndex)
{
	return(0);
}


UINT CAnimatedModelShared::getPartsCount() const
{
	return(m_aParts.size());
}
const char *CAnimatedModelShared::getPartName(UINT uIndex) const
{
	assert(uIndex < m_aParts.size());

	return(m_aParts[uIndex].szName);
}
UINT CAnimatedModelShared::getPartIndex(const char *szName)
{
	for(UINT i = 0, l = m_aParts.size(); i < l; ++i)
	{
		if(!strcasecmp(szName, m_aParts[i].szName))
		{
			return(i);
		}
	}
	return(~0);
}
XMODEL_PART_FLAGS CAnimatedModelShared::getPartFlags(UINT uIndex) const
{
	assert(uIndex < m_aParts.size());

	return(m_aParts[uIndex].flags);
}


UINT CAnimatedModelShared::getHitboxCount(UINT uPartIndex) const
{
	assert(uPartIndex < m_aParts.size());

	return(m_aParts[uPartIndex].aHitboxes.size());
}
const XResourceModelHitbox *CAnimatedModelShared::getHitbox(UINT id, UINT uPartIndex) const
{
	assert(uPartIndex < m_aParts.size() && id < m_aParts[uPartIndex].aHitboxes.size());

	return(m_aParts[uPartIndex].aHitboxes[id]);
}


int CAnimatedModelShared::getBoneId(const char *szName)
{
	for(UINT i = 0; i < m_uBoneCount; ++i)
	{
		if(!strcasecmp(m_pBones[i].szName, szName))
		{
			return(i);
		}
	}
	return(-1);
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
