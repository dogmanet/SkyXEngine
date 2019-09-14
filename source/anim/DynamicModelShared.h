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

	void render(UINT uSkin, UINT uLod, const float4_t &vColor, bool isTransparent);

	void initGPUresources();

	bool hasTransparentSubsets(UINT uSkin, UINT uLod);
	IXMaterial* getTransparentMaterial(UINT uSkin, UINT uLod);

	void onMaterialTransparencyChanged(const IXMaterial *pMaterial);
	float3 getTransparentBoundMin(UINT uSkin, UINT uLod) const;
	float3 getTransparentBoundMax(UINT uSkin, UINT uLod) const;

	const Array<float4_t>& getPSPs(UINT uSkin, UINT uLod) const;

protected:
	void buildPSPs();

protected:
	UINT m_uRefCount = 0;
	IXResourceModelStatic *m_pResource;

	IGXRenderBuffer **m_ppRenderBuffer = NULL;
	IGXIndexBuffer **m_ppIndexBuffer = NULL;

	UINT **m_ppTempIndices = NULL;
	XResourceModelStaticVertex **m_ppTempVertices = NULL;
	UINT *m_puTempTotalIndices = 0;
	UINT *m_puTempTotalVertices = 0;

	CDynamicModelProvider *m_pProvider;
	IGXDevice *m_pDevice;
	IXMaterialSystem *m_pMaterialSystem;

	void **m_ppMaterialsBlob = NULL;
	IXMaterial ***m_pppMaterials = NULL;
	UINT m_uMaterialCount = 0;
	UINT m_uSkinCount = 0;
	bool *m_isTransparent = NULL; //!< По количеству скинов, истина если есть прозрачные материалы в любом сабсете

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

	Array<Array<Array<float4_t>>, 1> m_aPSPs;

	Array<const IModelPhysbox*> m_apPhysboxes;
};

#endif
