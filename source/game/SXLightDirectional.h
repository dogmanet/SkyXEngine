
#ifndef __sxlightpoint_h
#define __sxlightpoint_h

#include "SXpointEntity.h"

class SXLightDirectional : public SXpointEntity
{
	DECLARE_CLASS(SXLightDirectional, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	SXLightDirectional(EntityManager * pMgr);
	~SXLightDirectional();

	void ToggleEnable();

	float3 m_color;
	float m_dist;
	float m_light_far;
	int m_typeshadow;

	bool m_enable;

	float m_angle;
	float m_topradius;

protected:
	ID IDLight;

	

	void OnSync();
};

#endif
