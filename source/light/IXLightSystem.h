#ifndef __IXLIGHTSYSTEM_H
#define __IXLIGHTSYSTEM_H

#include <gdefines.h>
#include "IXLight.h"

// {19EBAD4A-F241-4EA9-A4CD-52096A2C1CE3}
#define IXLIGHTSYSTEM_GUID DEFINE_XGUID(0x19ebad4a, 0xf241, 0x4ea9, 0xa4, 0xcd, 0x52, 0x9, 0x6a, 0x2c, 0x1c, 0xe3)

class ICamera;
class IXRenderPipeline;
class IXLightSystem: public IXUnknown
{
public:
	virtual IXLightSun* XMETHODCALLTYPE newSun() = 0;
	virtual IXLightSun* XMETHODCALLTYPE getSun() = 0;

	virtual IXLightPoint* XMETHODCALLTYPE newPoint() = 0;
	virtual IXLightSpot* XMETHODCALLTYPE newSpot() = 0;

	virtual UINT XMETHODCALLTYPE getCount() = 0;
	virtual IXLight* XMETHODCALLTYPE getLight(ID id) = 0;

	virtual void XMETHODCALLTYPE updateVisibility() = 0;

	virtual void XMETHODCALLTYPE setFrameObserverCamera(ICamera *pMainCamera) = 0;
	
	virtual void XMETHODCALLTYPE setRenderPipeline(IXRenderPipeline *pRenderPipeline) = 0;
	virtual void XMETHODCALLTYPE setGBuffer(IGXTexture2D *pColor, IGXTexture2D *pNormals, IGXTexture2D *pParams, IGXTexture2D *pDepth) = 0;
	virtual void XMETHODCALLTYPE renderGI(IGXTexture2D *pLightTotal, IGXTexture2D *pTempBuffer) = 0;
	virtual void XMETHODCALLTYPE renderToneMapping(IGXTexture2D *pSourceLight) = 0;
	virtual void XMETHODCALLTYPE renderDebug() = 0;
};

#endif
