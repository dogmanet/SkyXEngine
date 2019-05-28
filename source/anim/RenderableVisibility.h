#ifndef __RENDERABLE_VISIBILITY_H
#define __RENDERABLE_VISIBILITY_H

#include <xcommon/IXRenderable.h>

class CAnimatedModelProvider;
class CDynamicModelProvider;
class CRenderableVisibility: public IXRenderableVisibility
{
public:
	CRenderableVisibility(ID idPlugin, CAnimatedModelProvider *m_pProviderAnimated, CDynamicModelProvider *m_pProviderDynamic);
	~CRenderableVisibility();

	ID getPluginId();

	void setOcclusionCuller(IXOcclusionCuller *pOcclusionCuller);

	void updateForCamera(ICamera *pCamera, const IXRenderableVisibility *pReference = NULL);

	void updateForFrustum(const IFrustum *pFrustum, const IXRenderableVisibility *pReference = NULL);

	ID getVisCalcObjId()
	{
		return(m_idVisCalcObj);
	}

	struct item_s
	{
		bool isVisible = false;
		UINT uLod = 0;
	};

	void setItemCount(UINT uCount);
	item_s *getItem(UINT uIndex);

	void setItemCountDynamic(UINT uCount);
	item_s *getItemDynamic(UINT uIndex);
protected:
	ID m_idPlugin;
	ID m_idVisCalcObj = -1;
	CAnimatedModelProvider *m_pProviderAnimated;
	CDynamicModelProvider *m_pProviderDynamic;

	Array<item_s> m_aItems;
	Array<item_s> m_aItemsDynamic;
};

#endif
