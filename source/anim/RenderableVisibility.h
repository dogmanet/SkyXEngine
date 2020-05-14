#ifndef __RENDERABLE_VISIBILITY_H
#define __RENDERABLE_VISIBILITY_H

#include <xcommon/IXRenderable.h>

class CAnimatedModelProvider;
class CDynamicModelProvider;
class CDynamicModel;
class CRenderableVisibility: public IXUnknownImplementation<IXRenderableVisibility>
{
public:
	CRenderableVisibility(ID idPlugin, CAnimatedModelProvider *m_pProviderAnimated, CDynamicModelProvider *m_pProviderDynamic);

	ID getPluginId() const override;

	void setOcclusionCuller(IXOcclusionCuller *pOcclusionCuller) override;

	void updateForCamera(ICamera *pCamera, const IXRenderableVisibility *pReference = NULL) override;

	void updateForFrustum(const IFrustum *pFrustum, const IXRenderableVisibility *pReference = NULL) override;
	
	void append(const IXRenderableVisibility *pOther) override;

	void substract(const IXRenderableVisibility *pOther) override;

	struct item_s
	{
		bool isVisible = false;
		UINT uLod = 0;
		bool isTransparent = false;
		bool isEmissive = false;
	};

	struct TransparentModel
	{
		CDynamicModel *pReferenceMdl;
		bool hasPSP;
		SMPLANE psp;
		UINT uLod;
		IXMaterial *pMaterial;
	};

	void setItemCount(UINT uCount);
	item_s* getItem(UINT uIndex);

	void setItemCountDynamic(UINT uCount);
	item_s* getItemDynamic(UINT uIndex, bool forceCreate=true);

	void resetItemTransparentDynamic();
	void addItemTransparentDynamic(const TransparentModel &mdl);
	UINT getItemTransparentDynamicCount();
	TransparentModel* getItemTransparentDynamic(UINT uIndex);
protected:
	ID m_idPlugin;
	CAnimatedModelProvider* m_pProviderAnimated;
	CDynamicModelProvider* m_pProviderDynamic;

	Array<item_s> m_aItems;
	Array<item_s> m_aItemsDynamic;
	Array<TransparentModel> m_aItemsDynamicTransparent;
};

#endif
