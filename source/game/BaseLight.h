
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Базовый свет
*/

#ifndef __BASELIGHT_H
#define __BASELIGHT_H

#include "PointEntity.h"

#define LIGHT_INITIALLY_DARK ENT_FLAG_0

/*! Базовый источник света
\ingroup clight
*/
class CBaseLight: public CPointEntity
{
	DECLARE_CLASS(CBaseLight, CPointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	~CBaseLight();

	void toggleEnable();

	void setColor(const float4 &vColor)
	{
		m_vColor = vColor;
		SAFE_CALL(m_pLight, setColor, float3(m_vColor) * m_vColor.w);
	}
	const float4_t& getColor() const
	{
		return(m_vColor);
	}

	void setDist(float fDist)
	{
		m_fDist = fDist;
	}
	float getDist() const
	{
		return m_fDist;
	}

	/*void setShadowDist(float fShadowDist)
	{
		m_fShadowDist = fShadowDist;
	}
	float getShadowDist() const
	{
		return m_fShadowDist;
	}*/

	void setShadowIntensity(float fShadowIntensity)
	{
		m_fShadowIntensity = fShadowIntensity;
		SAFE_CALL(m_pLight, setShadowIntencity, fShadowIntensity);
	}
	float getShadowIntensity() const
	{
		return m_fShadowIntensity;
	}

	void setShadowType(int iShadowType)
	{
		m_iShadowType = iShadowType;
		SAFE_CALL(m_pLight, setShadowDynamic, iShadowType != 0);
	}
	int getShadowType() const
	{
		return m_iShadowType;
	}

	void setEnable(bool isEnable);
	bool getEnable() const
	{
		return m_isEnable;
	}

	bool getMainColor(float3_t *pOut);

	void updateFlags();

	void setPos(const float3 &pos) override;

protected:
	IXLight *m_pLight = NULL;

	float4_t m_vColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float m_fDist = 10.0f;
	//float m_fShadowDist;
	int m_iShadowType = 1;
	bool m_isEnable = true;
	float m_fShadowIntensity = 1.0f;
	
	void turnOn(inputdata_t * pInputdata);
	void turnOff(inputdata_t * pInputdata);

	output_t m_onTurnOn;
	output_t m_onTurnOff;
};

#endif
