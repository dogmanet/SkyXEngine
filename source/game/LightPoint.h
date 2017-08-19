
#ifndef __LIGHTPOINT_H
#define __LIGHTPOINT_H

#include "SXpointEntity.h"

class CLightPoint : public SXpointEntity
{
	DECLARE_CLASS(CLightPoint, SXpointEntity);
	DECLARE_PROPTABLE();
public:

	SX_ALIGNED_OP_MEM

	CLightPoint(EntityManager *pMgr);
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

	void setEnable(bool isEnable) { m_isEnable = isEnable; };
	bool getEnable() const { return m_isEnable; };

protected:
	ID m_idLight;

	float3 m_vColor;
	float m_fDist;
	float m_fShadowDist;
	int m_iShadowType;
	bool m_isEnable;

	void OnSync();
};

#endif
