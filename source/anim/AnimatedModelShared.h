#ifndef __ANIMATEDMODELSHARED_H
#define __ANIMATEDMODELSHARED_H

#include <xcommon/resource/IXModelProvider.h>
#include <graphix/graphix.h>

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
	const IXResourceModel *getResource(UINT uIndex = 0);


	UINT getPartsCount() const;
	const char *getPartName(UINT uIndex) const;
	UINT getPartIndex(const char *szName);
	XMODEL_PART_FLAGS getPartFlags(UINT uIndex) const;


	UINT getHitboxCount(UINT uPartIndex = 0) const;
	const XResourceModelHitbox *getHitbox(UINT id, UINT uPartIndex = 0) const;


	UINT getBoneId(const char *szName);
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

private:
	void _collectResources(const IXResourceModelAnimated *pResource, Array<const IXResourceModelAnimated*> &aResources);
};

#endif
