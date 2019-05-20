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
