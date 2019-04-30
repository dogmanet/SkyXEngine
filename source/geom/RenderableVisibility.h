#ifndef __RENDERABLE_VISIBILITY_H
#define __RENDERABLE_VISIBILITY_H

#include <xcommon/IXRenderable.h>

class CRenderableVisibility: public IXRenderableVisibility
{
public:
	CRenderableVisibility(ID idPlugin);
	~CRenderableVisibility();

	ID getPluginId();

	void setOcclusionCuller(IXOcclusionCuller *pOcclusionCuller);

	void updateForCamera(ICamera *pCamera, const IXRenderableVisibility *pReference = NULL);

	void updateForFrustum(IFrustum *pFrustum, const IXRenderableVisibility *pReference = NULL);

	ID getVisCalcObjId()
	{
		return(m_idVisCalcObj);
	}
protected:
	ID m_idPlugin;
	ID m_idVisCalcObj = -1;
};

#endif
