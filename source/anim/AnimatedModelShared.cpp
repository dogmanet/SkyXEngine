#include "AnimatedModelShared.h"
#include "AnimatedModelProvider.h"

CAnimatedModelShared::CAnimatedModelShared(CAnimatedModelProvider *pProvider):
	m_pProvider(pProvider)
{}
CAnimatedModelShared::~CAnimatedModelShared()
{
	m_pProvider->onSharedModelRelease(this);
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
	assert(uResourceCount > 0 && "test m_apResources sorted ascending");

	Array<const IXResourceModelAnimated*> aResources;
	for(UINT i = 0; i < uResourceCount; ++i)
	{
		aResources.push_back(ppResources[i]);
		_collectResources(ppResources[i], aResources);
	}

	//m_aszActivities

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

	// define parts
	// merge skins/materials
	// merge vertices/indices
	// merge sequences
	// merge controllers
	// merge hitboxes
	// merge physboxes

	return(true);
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
	return(0);
}
UINT CAnimatedModelShared::getBoneCount() const
{
	return(0);
}
const char *CAnimatedModelShared::getBoneName(UINT id) const
{
	return(0);
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
