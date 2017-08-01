
#ifndef __sxlightpoint_h
#define __sxlightpoint_h

#include "SXpointEntity.h"

class SXLightPoint : public SXpointEntity
{
	DECLARE_CLASS(SXLightPoint, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	SXLightPoint(EntityManager * pMgr);
	~SXLightPoint();

protected:
	ID IDLight;

	float3 m_color;
	float m_dist;
	float m_light_far;
	int m_typeshadow;

	void OnSync();
};

#endif
