#ifndef __DYNAMICMODELPROVIDER_H
#define __DYNAMICMODELPROVIDER_H

#include <xcommon/resource/IXModelProvider.h>
#include "DynamicModel.h"
#include "DynamicModelShared.h"
#include <xcommon/IXCore.h>
#include <mtrl/IXMaterialSystem.h>
#include "RenderableVisibility.h"
#include <common/ConcurrentQueue.h>
#include <xcommon/XEvents.h>

class CDynamicModelProvider;
class CMaterialChangedEventListener: public IEventListener<XEventMaterialChanged>
{
public:
	CMaterialChangedEventListener(CDynamicModelProvider *pProvider);
	void onEvent(const XEventMaterialChanged *pData) override;

protected:
	CDynamicModelProvider *m_pProvider;
};

class CDynamicModelProvider: public IXDynamicModelProvider
{
public:
	friend class CMaterialChangedEventListener;

	CDynamicModelProvider(IXCore *pCore);
	~CDynamicModelProvider();

	bool XMETHODCALLTYPE createModel(IXResourceModel *pResource, IXDynamicModel **ppModel) override;

	void onSharedModelRelease(CDynamicModelShared *pShared);
	void onModelRelease(CDynamicModel *pModel);
	IXMaterialSystem* getMaterialSystem();
	IGXDevice* getDevice();
	IXCore* getCore();
	void setDevice(IGXDevice *pContext);
	IGXVertexDeclaration *getVertexDeclaration();

	void render(bool isTransparent, CRenderableVisibility *pVisibility = NULL);
	void computeVisibility(const IFrustum *pFrustum, CRenderableVisibility *pVisibility, CRenderableVisibility *pReference=NULL);

	void getLevelSize(const XEventLevelSize *pData);

	void update();

	void scheduleSharedGPUinit(CDynamicModelShared *pShared);
	void scheduleModelGPUinit(CDynamicModel *pModel);

	UINT getTransparentCount(CRenderableVisibility *pVisibility);
	void getTransparentObject(CRenderableVisibility *pVisibility, UINT uIndex, XTransparentObject *pObject);
	void renderTransparentObject(CRenderableVisibility *pVisibility, UINT uIndex, UINT uSplitPlanes);


protected:
	void onMaterialTransparencyChanged(const IXMaterial *pMaterial);

	CMaterialChangedEventListener *m_pMaterialChangedEventListener;

	AssotiativeArray<IXResourceModel*, CDynamicModelShared*> m_mModels;

	Array<CDynamicModel*> m_apModels;

	IGXVertexDeclaration *m_pVertexDeclaration = NULL;

	IXCore *m_pCore;
	IGXDevice *m_pRenderContext = NULL;

	CConcurrentQueue<CDynamicModelShared*> m_queueGPUinitShared;
	CConcurrentQueue<CDynamicModel*> m_queueGPUinitModel;
};

#endif
