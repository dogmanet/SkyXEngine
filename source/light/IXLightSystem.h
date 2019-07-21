#ifndef __IXLIGHTSYSTEM_H
#define __IXLIGHTSYSTEM_H

#include <gdefines.h>
#include "IXLight.h"

// {19EBAD4A-F241-4EA9-A4CD-52096A2C1CE3}
#define IXLIGHTSYSTEM_GUID DEFINE_XGUID(0x19ebad4a, 0xf241, 0x4ea9, 0xa4, 0xcd, 0x52, 0x9, 0x6a, 0x2c, 0x1c, 0xe3)

class ICamera;
class IXLightSystem: public IXUnknown
{
public:
	virtual IXLightSun *createSun() = 0;
	virtual IXLightSun *getSun() = 0;


	virtual IXLightPoint *createPoint() = 0;
	virtual IXLightSpot *createSpot() = 0;

	virtual UINT getCount() = 0;
	virtual IXLight *getLight(ID id) = 0;

	virtual void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax) = 0;
};

#endif
