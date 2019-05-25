#ifndef __ANIMATEDMODELSHARED_H
#define __ANIMATEDMODELSHARED_H

#include <xcommon/resource/IXModelProvider.h>
#include <graphix/graphix.h>
#include <mtrl/IXMaterial.h>

class CAnimatedModelProvider;

class CAnimatedModelShared
{
public:
	CAnimatedModelShared(CAnimatedModelProvider *pProvider);
	~CAnimatedModelShared();
	void AddRef();
	void Release();

	bool init(UINT uResourceCount, const IXResourceModelAnimated **ppResources);
	bool isSame(UINT uResourceCount, const IXResourceModelAnimated **ppResources);

	const Array<const IXResourceModelAnimated*> &getResources();

	UINT getPhysboxCount(UINT uPartIndex = 0) const;
	const IModelPhysbox *getPhysBox(UINT id, UINT uPartIndex = 0) const;
	const int getPhysBoxBone(UINT id, UINT uPartIndex = 0) const;
	const IXResourceModel *getResource(UINT uIndex = 0);


	UINT getPartsCount() const;
	const char *getPartName(UINT uIndex) const;
	UINT getPartIndex(const char *szName);
	XMODEL_PART_FLAGS getPartFlags(UINT uIndex) const;


	UINT getHitboxCount(UINT uPartIndex = 0) const;
	const XResourceModelHitbox *getHitbox(UINT id, UINT uPartIndex = 0) const;


	int getBoneId(const char *szName);
	UINT getBoneCount() const;
	const char *getBoneName(UINT id) const;

	UINT getControllersCount() const;
	const char *getControllerName(UINT id);
	UINT getControllerId(const char *szName);

protected:
	UINT m_uRefCount = 0;
	Array<const IXResourceModelAnimated*> m_apResources;

	CAnimatedModelProvider *m_pProvider;

	IGXRenderBuffer *m_pRenderBuffer = NULL;
	IGXIndexBuffer *m_pIndexBuffer = NULL;

	Array<const char*> m_aszActivities;

	struct bone_s
	{
		const char *szName;
		int iParent;
		XResourceModelBone bone;
	};
	bone_s *m_pBones = NULL;
	UINT m_uBoneCount = 0;

	void **m_ppMaterialsBlob = NULL;
	IXMaterial ***m_pppMaterials = NULL;
	UINT m_uMaterialCount = 0;
	UINT m_uSkinCount = 0;

	struct physbox_s
	{
		const IModelPhysbox *pPhysbox;
		int iBone;
	};

	struct part_s
	{
		const char *szName;
		XMODEL_PART_FLAGS flags;
		Array<const XResourceModelHitbox*> aHitboxes;
		Array<physbox_s> aPhysboxes;
		XPT_TOPOLOGY topology;
	};
	Array<part_s> m_aParts;

private:
	struct bone_node
	{
		UINT uResource;
		UINT uBoneIdx;
		bone_node *pParent;
		const char *szName;
		bool bLooseTransform;
	};

	struct mtl_node
	{
		const char *szName = NULL;
		UINT uResource;
	}; 

	void _collectResources(const IXResourceModelAnimated *pResource, Array<const IXResourceModelAnimated*> &aResources);
	void _mergeByParent(bone_node *pParent, bone_node *pNodes, UINT uTotalBones);
	int _buildBoneListByParent(bone_node *pParent, bone_node *pNodes, UINT uTotalBones, bone_s *pList, const IXResourceModelAnimated **ppResources, int iParent);
};

#endif
