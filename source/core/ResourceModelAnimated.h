#ifndef __RESOURCEMODELANIMATED_H
#define __RESOURCEMODELANIMATED_H

#include "ResourceModel.h"

class CResourceModelAnimated: public CResourceModel, public virtual IXResourceModelAnimated
{
public:
	~CResourceModelAnimated();

	XMODELTYPE XMETHODCALLTYPE getType() const override;

	UINT XMETHODCALLTYPE getSubsetCount(UINT uLod) const override;
	UINT XMETHODCALLTYPE addLod(UINT uSubsetCount, UINT *puVertexCount, UINT *puIndexCount) override;

	const XResourceModelAnimatedSubset * XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) const override;
	XResourceModelAnimatedSubset * XMETHODCALLTYPE getSubset(UINT uLod, UINT uSubset) override;


	void XMETHODCALLTYPE setBoneCount(UINT uCount) override;
	UINT XMETHODCALLTYPE getBoneCount() const override;

	void XMETHODCALLTYPE setBoneInfo(int iBone, const char *szName, int iParent, const float3 &vTranslation, const SMQuaternion &vRotation) override;
	int XMETHODCALLTYPE getBoneParent(int iBone) const override;
	const char * XMETHODCALLTYPE getBoneName(int iBone) const override;
	float3 XMETHODCALLTYPE getBoneTranslation(int iBone) const override;
	SMQuaternion XMETHODCALLTYPE getBoneRotation(int iBone) const override;


	UINT XMETHODCALLTYPE getSequenceCount() const override;
	const XResourceModelSequence * XMETHODCALLTYPE getSequence(UINT uIndex) const override;

	void XMETHODCALLTYPE setSequenceCount(UINT uCount) override;
	XResourceModelSequence * XMETHODCALLTYPE getSequence(UINT uIndex) override;
	void XMETHODCALLTYPE setSequenceFrameCount(UINT uIndex, UINT uFrameCount) override;


	UINT XMETHODCALLTYPE addActivity(const char *szName) override;
	const char * XMETHODCALLTYPE getActivityName(UINT uIndex) const override;
	UINT XMETHODCALLTYPE getActivitiesCount() const override;


	UINT XMETHODCALLTYPE getControllersCount() const override;
	void XMETHODCALLTYPE setControllersCount(UINT uCount) override;
	void XMETHODCALLTYPE setControllerBoneCount(UINT uIndex, UINT uAffectedBonesCount) override;
	const XResourceModelController * XMETHODCALLTYPE getController(UINT uIndex) const override;
	XResourceModelController * XMETHODCALLTYPE getController(UINT uIndex) override;


	UINT XMETHODCALLTYPE getHitboxCount() const override;
	void XMETHODCALLTYPE setHitboxCount(UINT uCount) override;
	XResourceModelHitbox * XMETHODCALLTYPE getHitbox(UINT uIndex) override;
	const XResourceModelHitbox * XMETHODCALLTYPE getHitbox(UINT uIndex) const override;

	bool XMETHODCALLTYPE validate() const;

protected:

	struct _subset_meta
	{
		UINT uSubsetCount;
		XResourceModelAnimatedSubset *pSubsets;
		XResourceModelAnimatedVertex *pVertices;
		UINT *pIndices;

		~_subset_meta()
		{
			mem_delete_a(pSubsets);
			mem_delete_a(pVertices);
			mem_delete_a(pIndices);
		}
	};
	Array<_subset_meta> m_aLods;

	struct _bone_hierarchy
	{
		int pid;         /*!< Номер родительской кости */
		char szName[MODEL_MAX_NAME];
		XResourceModelBone bindPose;
	};

	_bone_hierarchy *m_pBones = NULL;
	UINT m_uBoneCount = 0;

	XResourceModelSequence *m_pSequences = NULL;
	UINT m_uSequenceCount = 0;

	Array<String> m_asActivities;

	XResourceModelController *m_pControllers = NULL;
	UINT m_uControllersCount = 0;

	XResourceModelHitbox *m_pHitboxes = NULL;
	UINT m_uHitboxCount = 0;
};

#endif
