#ifndef __DYNAMICMODELPROVIDER_H
#define __DYNAMICMODELPROVIDER_H

#include <xcommon/resource/IXModelProvider.h>
#include "DynamicModel.h"
#include "DynamicModelShared.h"
#include <xcommon/IXCore.h>
#include <mtrl/IXMaterialSystem.h>
#include "RenderableVisibility.h"

class CDynamicModelProvider: public IXDynamicModelProvider
{
public:
	CDynamicModelProvider(IXCore *pCore);
	~CDynamicModelProvider();

	bool XMETHODCALLTYPE createModel(IXResourceModel *pResource, IXDynamicModel **ppModel) override;

	void onSharedModelRelease(CDynamicModelShared *pShared);
	void onModelRelease(CDynamicModel *pModel);
	IXMaterialSystem *getMaterialSystem();
	IGXContext *getDevice();
	void setDevice(IGXContext *pContext);
	IGXVertexDeclaration *getVertexDeclaration();

	void render(CRenderableVisibility *pVisibility = NULL);
	void computeVisibility(const IFrustum *pFrustum, CRenderableVisibility *pVisibility, CRenderableVisibility *pReference=NULL);

protected:
	AssotiativeArray<IXResourceModel*, CDynamicModelShared*> m_mModels;

	Array<CDynamicModel*> m_apModels;

	IGXVertexDeclaration *m_pVertexDeclaration = NULL;

	IXCore *m_pCore;
	IGXContext *m_pRenderContext = NULL;
};

#endif
