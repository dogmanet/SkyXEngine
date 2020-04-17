#ifndef __LIGHTSYSTEM_H
#define __LIGHTSYSTEM_H

#include "IXLightSystem.h"
#include <common/memalloc.h>
#include "light.h"

class CLevelLoadListener;
class CLightSystem: public IXLightSystem
{
public:
	CLightSystem(IXCore *pCore);
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

	void setLevelSize(const float3 &vMin, const float3 &vMax);

protected:
	CXLightSun *m_pSun = NULL;
	MemAlloc<CXLightPoint, 256, 16, 16> m_poolPoint;
	MemAlloc<CXLightSpot, 256, 16, 16> m_poolSpot;

	float3_t m_vLevelMin, m_vLevelMax;
	float m_fLevelDimensions = 0.0f;

	IEventChannel<XEventLevel> *m_pLevelChannel = NULL;
	CLevelLoadListener *m_pLevelListener = NULL;

	Array<CXLight*> m_aLights;

	void _deleteLight(CXLight *pLight);

	IMesh *m_pShapeSphere = NULL;
	IMesh *m_pShapeCone = NULL;
};

#endif
