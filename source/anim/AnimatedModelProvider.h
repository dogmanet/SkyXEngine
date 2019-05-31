#ifndef __ANIMATEDMODELPROVIDER_H
#define __ANIMATEDMODELPROVIDER_H

#include <xcommon/resource/IXModelProvider.h>
#include "AnimatedModel.h"
#include "AnimatedModelShared.h"
#include <xcommon/IXCore.h>
#include <mtrl/IXMaterialSystem.h>
#include "RenderableVisibility.h"

class CAnimatedModelProvider: public IXAnimatedModelProvider
{
public:
	CAnimatedModelProvider(IXCore *pCore);
	~CAnimatedModelProvider();

	bool XMETHODCALLTYPE createModel(UINT uResourceCount, IXResourceModelAnimated **ppResources, IXAnimatedModel **ppModel) override;

	void onSharedModelRelease(CAnimatedModelShared *pShared);
	void onModelRelease(CAnimatedModel *pModel);
	IXMaterialSystem *getMaterialSystem();
	IGXContext *getDevice();
	void setDevice(IGXContext *pContext);
	IGXVertexDeclaration *getVertexDeclaration();

	void update(float fDT);
	void sync();
	void render(CRenderableVisibility *pVisibility = NULL);
	void computeVisibility(const IFrustum *pFrustum, CRenderableVisibility *pVisibility, CRenderableVisibility *pReference=NULL);

protected:
	AssotiativeArray<IXResourceModelAnimated*, Array<CAnimatedModelShared*>> m_mModels;

	Array<CAnimatedModel*> m_apModels;

	IGXVertexDeclaration *m_pVertexDeclaration = NULL;

	IXCore *m_pCore;
	IGXContext *m_pRenderContext = NULL;
};

#endif
