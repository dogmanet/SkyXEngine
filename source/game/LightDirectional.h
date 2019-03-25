
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

	void setAngle(float fAngle) { m_fAngle = fAngle; };
	float getAngle() const { return m_fAngle; };

	void setRadiusTop(float fRadiusTop) { m_fRadiusTop = fRadiusTop; };
	float getRadiusTop() const { return m_fRadiusTop; };
	
protected:
	void onSync();

	float m_fAngle;
	float m_fRadiusTop;

	IXLightSpot *m_pLightSpot = NULL;
};

#endif
