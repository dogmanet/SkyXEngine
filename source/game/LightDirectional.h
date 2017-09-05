
#ifndef __LIGHTDIRECTIONAL_H
#define __LIGHTDIRECTIONAL_H

#include "SXpointEntity.h"

class CLightDirectional: public SXpointEntity
{
	DECLARE_CLASS(CLightDirectional, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CLightDirectional();

	void toggleEnable();

	void setColor(const float3 &vColor) { m_vColor = vColor; };
	const float3& getColor() const { return m_vColor; };

	void setDist(float fDist) { m_fDist = fDist; };
	float getDist() const { return m_fDist; };

	void setShadowDist(float fShadowDist) { m_fShadowDist = fShadowDist; };
	float getShadowDist() const { return m_fShadowDist; };

	void setShadowType(int iShadowType) { m_iShadowType = iShadowType; };
	int getShadowType() const { return m_iShadowType; };

	void setEnable(bool isEnable) { m_isEnable = isEnable; };
	bool getEnable() const { return m_isEnable; };

	void setAngle(float fAngle) { m_fAngle = fAngle; };
	float getAngle() const { return m_fAngle; };

	void setRadiusTop(float fRadiusTop) { m_fRadiusTop = fRadiusTop; };
	float getRadiusTop() const { return m_fRadiusTop; };

protected:

	void OnSync();

	ID m_idLight;

	float3_t m_vColor;
	float m_fDist;
	float m_fShadowDist;
	int m_iShadowType;

	bool m_isEnable;

	float m_fAngle;
	float m_fRadiusTop;
};

#endif
