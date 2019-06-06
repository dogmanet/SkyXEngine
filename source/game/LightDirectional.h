
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Направленный свет
*/

#ifndef __LIGHTDIRECTIONAL_H
#define __LIGHTDIRECTIONAL_H

#include "BaseLight.h"

/*! Направленный свет
\ingroup clight
*/
class CLightDirectional: public CBaseLight
{
	DECLARE_CLASS(CLightDirectional, CBaseLight);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CLightDirectional();

	void setOuterAngle(float fAngle)
	{
		m_fOuterAngle = fAngle;
	}
	float getOuterAngle() const
	{
		return m_fOuterAngle;
	}
	void setInnerAngle(float fAngle)
	{
		m_fInnerAngle = fAngle;
	}
	float getInnerAngle() const
	{
		return m_fInnerAngle;
	}

	void setRadiusTop(float fRadiusTop) { m_fRadiusTop = fRadiusTop; };
	float getRadiusTop() const { return m_fRadiusTop; };
	
protected:
	void onSync();

	float m_fOuterAngle = SM_PI * 0.4f;
	float m_fInnerAngle = SM_PI * 0.4f * 0.7f;
	float m_fRadiusTop = 0.01f;

	IXLightSpot *m_pLightSpot = NULL;
};

#endif
