
#ifndef __LIGHTPOINT_H
#define __LIGHTPOINT_H

#include "SXpointEntity.h"

class CLightPoint : public SXpointEntity
{
	DECLARE_CLASS(CLightPoint, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CLightPoint();

	void toggleEnable();

	void setColor(const float3 &vColor) { m_vColor = vColor; };
	const float3& getColor() const { return m_vColor; };

	void setDist(float fDist) { m_fDist = fDist; };
	float getDist() const { return m_fDist; };

	void setShadowDist(float fShadowDist) { m_fShadowDist = fShadowDist; };
	float getShadowDist() const { return m_fShadowDist; };

	void setShadowType(int iShadowType) { m_iShadowType = iShadowType; };
	int getShadowType() const { return m_iShadowType; };

	void setEnable(bool isEnable);
	bool getEnable() const { return m_isEnable; };

protected:
	ID m_idLight;

	float3_t m_vColor;
	float m_fDist;
	float m_fShadowDist;
	int m_iShadowType;
	bool m_isEnable;

	void OnSync();

	void turnOn(inputdata_t * pInputdata);
	void turnOff(inputdata_t * pInputdata);

	output_t m_onTurnOn;
	output_t m_onTurnOff;
};

#endif