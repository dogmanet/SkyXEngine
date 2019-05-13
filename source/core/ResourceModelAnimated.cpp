#include "ResourceModelAnimated.h"

CResourceModelAnimated::~CResourceModelAnimated()
{
	mem_delete_a(m_pBones);

	for(UINT uIndex = 0; uIndex < m_uSequenceCount; ++uIndex)
	{
		for(UINT i = 0; i < m_pSequences[uIndex].uNumFrames; ++i)
		{
			mem_delete_a(m_pSequences[uIndex].m_ppSequenceData[i]);
		}
		mem_delete_a(m_pSequences[uIndex].m_ppSequenceData);
	}
	mem_delete_a(m_pSequences);

	for(UINT uIndex = 0; uIndex < m_uControllersCount; ++uIndex)
	{
		mem_delete_a(m_pControllers[uIndex].pBones);
	}
	mem_delete_a(m_pControllers);

	mem_delete_a(m_pHitboxes);
}

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
	mem_delete_a(m_pBones);
	m_pBones = new _bone_hierarchy[uCount];
	memset(m_pBones, 0, sizeof(_bone_hierarchy)* uCount);
	m_uBoneCount = uCount;
}
UINT CResourceModelAnimated::getBoneCount() const
{
	return(m_uBoneCount);
}

void CResourceModelAnimated::setBoneInfo(int iBone, const char *szName, int iParent, const float3 &vTranslation, const SMQuaternion &vRotation)
{
	assert(iBone >= 0 && iBone < (int)m_uBoneCount);

	m_pBones[iBone].pid = iParent;
	strncpy(m_pBones[iBone].szName, szName, MODEL_BONE_MAX_NAME);
	m_pBones[iBone].szName[MODEL_BONE_MAX_NAME - 1] = 0;

	m_pBones[iBone].bindPose.position = vTranslation;
	m_pBones[iBone].bindPose.orient = vRotation;
}
int CResourceModelAnimated::getBoneParent(int iBone) const
{
	assert(iBone >= 0 && iBone < (int)m_uBoneCount);

	return(m_pBones[iBone].pid);
}
const char *CResourceModelAnimated::getBoneName(int iBone) const
{
	assert(iBone >= 0 && iBone < (int)m_uBoneCount);

	return(m_pBones[iBone].szName);
}
float3 CResourceModelAnimated::getBoneTranslation(int iBone) const
{
	assert(iBone >= 0 && iBone < (int)m_uBoneCount);

	return(m_pBones[iBone].bindPose.position);
}
SMQuaternion CResourceModelAnimated::getBoneRotation(int iBone) const
{
	assert(iBone >= 0 && iBone < (int)m_uBoneCount);

	return(m_pBones[iBone].bindPose.orient);
}


UINT CResourceModelAnimated::getSequenceCount() const
{
	return(m_uSequenceCount);
}
const XResourceModelSequence *CResourceModelAnimated::getSequence(UINT uIndex) const
{
	assert(uIndex < m_uSequenceCount);
	return(&m_pSequences[uIndex]);
}

void CResourceModelAnimated::setSequenceCount(UINT uCount)
{
	for(UINT uIndex = 0; uIndex < m_uSequenceCount; ++uIndex)
	{
		for(UINT i = 0; i < m_pSequences[uIndex].uNumFrames; ++i)
		{
			mem_delete_a(m_pSequences[uIndex].m_ppSequenceData[i]);
		}
		mem_delete_a(m_pSequences[uIndex].m_ppSequenceData);
	}
	mem_delete_a(m_pSequences);
	m_pSequences = new XResourceModelSequence[uCount];
	memset(m_pSequences, 0, sizeof(XResourceModelSequence)*uCount);
	m_uSequenceCount = uCount;
}
XResourceModelSequence *CResourceModelAnimated::getSequence(UINT uIndex)
{
	assert(uIndex < m_uSequenceCount);
	return(&m_pSequences[uIndex]);
}
void CResourceModelAnimated::setSequenceFrameCount(UINT uIndex, UINT uFrameCount)
{
	assert(uIndex < m_uSequenceCount);

	for(UINT i = 0; i < m_pSequences[uIndex].uNumFrames; ++i)
	{
		mem_delete_a(m_pSequences[uIndex].m_ppSequenceData[i]);
	}
	mem_delete_a(m_pSequences[uIndex].m_ppSequenceData);

	m_pSequences[uIndex].m_ppSequenceData = new XResourceModelBone*[uFrameCount];
	m_pSequences[uIndex].uNumFrames = uFrameCount;

	for(UINT i = 0; i < m_pSequences[uIndex].uNumFrames; ++i)
	{
		m_pSequences[uIndex].m_ppSequenceData[i] = new XResourceModelBone[getBoneCount()];
		memset(m_pSequences[uIndex].m_ppSequenceData[i], 0, sizeof(XResourceModelBone)* getBoneCount());
	}
}


UINT CResourceModelAnimated::addActivity(const char *szName)
{
	m_asActivities.push_back(szName);

	return(m_asActivities.size() - 1);
}
const char *CResourceModelAnimated::getActivityName(UINT uIndex) const
{
	assert(uIndex < m_asActivities.size());

	return(m_asActivities[uIndex].c_str());
}
UINT CResourceModelAnimated::getActivitiesCount() const
{
	return(m_asActivities.size());
}


UINT CResourceModelAnimated::getControllersCount() const
{
	return(m_uControllersCount);
}
void CResourceModelAnimated::setControllersCount(UINT uCount)
{
	for(UINT uIndex = 0; uIndex < m_uControllersCount; ++uIndex)
	{
		mem_delete_a(m_pControllers[uIndex].pBones);
	}
	mem_delete_a(m_pControllers);
	m_pControllers = new XResourceModelController[uCount];
	memset(m_pControllers, 0, sizeof(XResourceModelController)* uCount);
	m_uControllersCount = uCount;
}
void CResourceModelAnimated::setControllerBoneCount(UINT uIndex, UINT uAffectedBonesCount)
{
	assert(uIndex < m_uControllersCount);
	mem_delete_a(m_pControllers[uIndex].pBones);
	m_pControllers[uIndex].pBones = new UINT[uAffectedBonesCount];
	memset(m_pControllers[uIndex].pBones, 0, sizeof(UINT)* uAffectedBonesCount);
	m_pControllers[uIndex].uBoneCount = uAffectedBonesCount;
}
const XResourceModelController *CResourceModelAnimated::getController(UINT uIndex) const
{
	assert(uIndex < m_uControllersCount);
	return(&m_pControllers[uIndex]);
}
XResourceModelController *CResourceModelAnimated::getController(UINT uIndex)
{
	assert(uIndex < m_uControllersCount);
	return(&m_pControllers[uIndex]);
}


UINT CResourceModelAnimated::getHitboxCount() const
{
	return(m_uHitboxCount);
}
void CResourceModelAnimated::setHitboxCount(UINT uCount)
{
	mem_delete_a(m_pHitboxes);
	m_pHitboxes = new XResourceModelHitbox[uCount];
	memset(m_pHitboxes, 0, sizeof(XResourceModelHitbox)* uCount);
	m_uHitboxCount = uCount;
}
XResourceModelHitbox *CResourceModelAnimated::getHitbox(UINT uIndex)
{
	assert(uIndex < m_uHitboxCount);
	return(&m_pHitboxes[uIndex]);
}
const XResourceModelHitbox *CResourceModelAnimated::getHitbox(UINT uIndex) const
{
	assert(uIndex < m_uHitboxCount);
	return(&m_pHitboxes[uIndex]);
}

bool CResourceModelAnimated::validate() const
{
	//@TODO: Implement me!
	return(true);
}
