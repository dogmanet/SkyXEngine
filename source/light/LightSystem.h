#ifndef __LIGHTSYSTEM_H
#define __LIGHTSYSTEM_H

#include "IXLightSystem.h"
#include <common/memalloc.h>
#include "light.h"

class CLightSystem: public IXLightSystem
{
public:
	~CLightSystem();

	IXLightSun *createSun() override;
	IXLightSun *getSun() override;
	void destroySun(IXLightSun *pLight);

	IXLightPoint *createPoint() override;
	void destroyPoint(IXLightPoint *pLight);
	IXLightSpot *createSpot() override;
	void destroySpot(IXLightSpot *pLight);

	UINT getCount() override;
	IXLight *getLight(ID id) override;

	IMesh *getShapeSphere();
	IMesh *getShapeCone();

	void updateVisibility(ICamera *pMainCamera, const float3 &vLPVmin, const float3 &vLPVmax) override;

protected:
	CXLightSun *m_pSun = NULL;
	MemAlloc<CXLightPoint, 256, 16, 16> m_poolPoint;
	MemAlloc<CXLightSpot, 256, 16, 16> m_poolSpot;

	Array<CXLight*> m_aLights;

	void _deleteLight(CXLight *pLight);

	IMesh *m_pShapeSphere = NULL;
	IMesh *m_pShapeCone = NULL;
};

#endif
