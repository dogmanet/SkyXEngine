#ifndef __RESOURCEMODELANIMATED_H
#define __RESOURCEMODELANIMATED_H

#include "ResourceModel.h"

class CResourceModelAnimated: public CResourceModel, public virtual IXResourceModelAnimated
{
public:
	XMODELTYPE getType() const override;

	UINT getSubsetCount(UINT uLod) const override;
	UINT addLod(UINT uSubsetCount, UINT *puVertexCount, UINT *puIndexCount) override;

	const XResourceModelAnimatedSubset *getSubset(UINT uLod, UINT uSubset) const override;
	XResourceModelAnimatedSubset *getSubset(UINT uLod, UINT uSubset) override;


	void setBoneCount(UINT uCount) override;
	UINT getBoneCount() const override;

	void setBoneInfo(int iBone, const char *szName, int iParent, const float3 &vTranslation, const SMQuaternion &vRotation) override;
	int getBoneParent(int iBone) const override;
	const char *getBoneName(int iBone) const override;
	float3 getBoneTranslation(int iBone) const override;
	SMQuaternion getBoneRotation(int iBone) const override;


	UINT getSequenceCount() const override;
	const XResourceModelSequence *getSequence(UINT uIndex) const override;

	void setSequenceCount() override;
	XResourceModelSequence *getSequence(UINT uIndex) override;
	void setSequenceName(UINT uIndex, const char *szName) override;
	void setSequenceFrameCount(UINT uIndex, UINT uFrameCount) override;


	UINT addActivity(const char *szName) override;
	const char *getActivityName(UINT uIndex) const override;
	UINT getActivitiesCount() const override;


	UINT getControllersCount() const override;
	void setControllersCount() override;
	void setControllerInfo(UINT uIndex, const char *szName, UINT uAffectedBonesCount) override;
	const XResourceModelController *getController(UINT uIndex) const override;
	XResourceModelController *getController(UINT uIndex) override;


	UINT getHitboxCount() const override;
	void setHitboxCount() override;
	void setHitboxName(const char *szHitbox) override;
	XResourceModelHitbox *getHitbox(UINT uIndex) override;
	const XResourceModelHitbox *getHitbox(UINT uIndex) const override;

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
};

#endif
