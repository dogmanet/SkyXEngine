#ifndef __LIGHTSYSTEM_H
#define __LIGHTSYSTEM_H

#include "IXLightSystem.h"
#include <common/memalloc.h>
#include "light.h"

class CLightSystem: public IXLightSystem
{
public:
	~CLightSystem();

	IXLightSun *createSun();
	IXLightSun *getSun();
	void destroySun(IXLightSun *pLight);

	IXLightPoint *createPoint();
	void destroyPoint(IXLightPoint *pLight);
	IXLightSpot *createSpot();
	void destroySpot(IXLightSpot *pLight);

	UINT getCount();
	IXLight *getLight(ID id);

	IMesh *getShapeSphere();
	IMesh *getShapeCone();

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
