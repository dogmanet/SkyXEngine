#ifndef __DYNAMICMODELSHARED_H
#define __DYNAMICMODELSHARED_H

#include <xcommon/resource/IXModelProvider.h>
#include <graphix/graphix.h>
#include <mtrl/IXMaterial.h>

class CDynamicModelProvider;
class IXMaterialSystem;

class CDynamicModelShared
{
public:
	CDynamicModelShared(CDynamicModelProvider *pProvider);
	~CDynamicModelShared();
	void AddRef();
	void Release();

	bool init(IXResourceModelStatic *pResource);

	IXResourceModelStatic *getResource();

	UINT getPhysboxCount() const;
	const IModelPhysbox *getPhysBox(UINT id) const;

	float3 getLocalBoundMin() const;
	float3 getLocalBoundMax() const;

	void render(UINT uSkin, UINT uLod, const float4_t &vColor);

protected:
	UINT m_uRefCount = 0;
	IXResourceModelStatic *m_pResource;

	IGXRenderBuffer **m_ppRenderBuffer = NULL;
	IGXIndexBuffer **m_ppIndexBuffer = NULL;

	CDynamicModelProvider *m_pProvider;
	IGXContext *m_pDevice;
	IXMaterialSystem *m_pMaterialSystem;

	void **m_ppMaterialsBlob = NULL;
	IXMaterial ***m_pppMaterials = NULL;
	UINT m_uMaterialCount = 0;
	UINT m_uSkinCount = 0;

	XPT_TOPOLOGY m_topology;

	struct physbox_s
	{
		const IModelPhysbox *pPhysbox;
		int iBone;
	};

	float3_t m_vLocalMin;
	float3_t m_vLocalMax;

	struct subset_t
	{
		UINT uStartIndex = 0;
		UINT uIndexCount = 0;
		UINT uStartVertex = 0;
		UINT uVertexCount = 0;
	};

	Array<Array<subset_t>, 1> m_aLods;

	Array<const IModelPhysbox*> m_apPhysboxes;
};

#endif
