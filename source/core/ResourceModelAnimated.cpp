#include "ResourceModelAnimated.h"

XMODELTYPE CResourceModelAnimated::getType() const
{
	return(XMT_ANIMATED);
}

UINT CResourceModelAnimated::getSubsetCount(UINT uLod) const
{
	assert(uLod < m_aLods.size());

	return(m_aLods[uLod].uSubsetCount);
}
UINT CResourceModelAnimated::addLod(UINT uSubsetCount, UINT *puVertexCount, UINT *puIndexCount)
{
	_subset_meta subset;

	subset.uSubsetCount = uSubsetCount;
	subset.pSubsets = new XResourceModelAnimatedSubset[uSubsetCount];

	UINT uIndexCount = 0;
	UINT uVertexCount = 0;
	for(UINT i = 0; i < uSubsetCount; ++i)
	{
		uVertexCount += puVertexCount[i];
		uIndexCount += puIndexCount[i];
	}
	subset.pVertices = new XResourceModelAnimatedVertex[uVertexCount];
	subset.pIndices = new UINT[uIndexCount];

	uIndexCount = 0;
	uVertexCount = 0;
	for(UINT i = 0; i < uSubsetCount; ++i)
	{
		subset.pSubsets[i].pVertices = subset.pVertices + uVertexCount;
		subset.pSubsets[i].pIndices = subset.pIndices + uIndexCount;
		uVertexCount += puVertexCount[i];
		uIndexCount += puIndexCount[i];
	}

	m_aLods.push_back(subset);
	return(m_aLods.size() - 1);
}

const XResourceModelAnimatedSubset *CResourceModelAnimated::getSubset(UINT uLod, UINT uSubset) const
{
	assert(uLod < m_aLods.size());
	assert(uSubset < m_aLods[uLod].uSubsetCount);

	return(&m_aLods[uLod].pSubsets[uSubset]);
}
XResourceModelAnimatedSubset *CResourceModelAnimated::getSubset(UINT uLod, UINT uSubset)
{
	assert(uLod < m_aLods.size());
	assert(uSubset < m_aLods[uLod].uSubsetCount);

	return(&m_aLods[uLod].pSubsets[uSubset]);
}


void CResourceModelAnimated::setBoneCount(UINT uCount)
{
	
}
UINT CResourceModelAnimated::getBoneCount() const
{
	return(0);
}

void CResourceModelAnimated::setBoneInfo(int iBone, const char *szName, int iParent, const float3 &vTranslation, const SMQuaternion &vRotation)
{
	
}
int CResourceModelAnimated::getBoneParent(int iBone) const
{
	return(0);
}
const char *CResourceModelAnimated::getBoneName(int iBone) const
{
	return(0);
}
float3 CResourceModelAnimated::getBoneTranslation(int iBone) const
{
	return(0);
}
SMQuaternion CResourceModelAnimated::getBoneRotation(int iBone) const
{
	return(SMQuaternion());
}


UINT CResourceModelAnimated::getSequenceCount() const
{
	return(0);
}
const XResourceModelSequence *CResourceModelAnimated::getSequence(UINT uIndex) const
{
	return(0);
}

void CResourceModelAnimated::setSequenceCount()
{
	
}
XResourceModelSequence *CResourceModelAnimated::getSequence(UINT uIndex)
{
	return(0);
}
void CResourceModelAnimated::setSequenceName(UINT uIndex, const char *szName)
{
	
}
void CResourceModelAnimated::setSequenceFrameCount(UINT uIndex, UINT uFrameCount)
{
	
}


UINT CResourceModelAnimated::addActivity(const char *szName)
{
	return(0);
}
const char *CResourceModelAnimated::getActivityName(UINT uIndex) const
{
	return(0);
}
UINT CResourceModelAnimated::getActivitiesCount() const
{
	return(0);
}


UINT CResourceModelAnimated::getControllersCount() const
{
	return(0);
}
void CResourceModelAnimated::setControllersCount()
{
	
}
void CResourceModelAnimated::setControllerInfo(UINT uIndex, const char *szName, UINT uAffectedBonesCount)
{
	
}
const XResourceModelController *CResourceModelAnimated::getController(UINT uIndex) const
{
	return(0);
}
XResourceModelController *CResourceModelAnimated::getController(UINT uIndex)
{
	return(0);
}


UINT CResourceModelAnimated::getHitboxCount() const
{
	return(0);
}
void CResourceModelAnimated::setHitboxCount()
{
	
}
void CResourceModelAnimated::setHitboxName(const char *szHitbox)
{
	
}
XResourceModelHitbox *CResourceModelAnimated::getHitbox(UINT uIndex)
{
	return(0);
}
const XResourceModelHitbox *CResourceModelAnimated::getHitbox(UINT uIndex) const
{
	return(0);
}
