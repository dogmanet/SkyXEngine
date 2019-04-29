#ifndef __RENDERABLE_VISIBILITY_H
#define __RENDERABLE_VISIBILITY_H

#include <xcommon/IXRenderable.h>

class CRenderableVisibility: public IXRenderableVisibility
{
public:
	CRenderableVisibility(IXRenderPipeline *pPipeline);

	ID getPluginId();

	void setOcclusionCuller(IXOcclusionCuller *pOcclusionCuller);

	void updateForCamera(ICamera *pCamera, const IXRenderableVisibility *pReference = NULL);

	void updateForFrustum(IFrustum *pFrustum, const IXRenderableVisibility *pReference = NULL);

	IXRenderableVisibility *getVisibility(ID id);

protected:
	IXRenderableVisibility **m_ppVisibilities;
};

#endif
