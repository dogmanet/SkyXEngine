
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

	void setOuterAngle(float fAngle);
	float getOuterAngle() const;
	void setInnerAngle(float fAngle);
	float getInnerAngle() const;

	void setOrient(const SMQuaternion &q) override;

protected:
	float m_fOuterAngle = SM_PI * 0.4f;
	float m_fInnerAngle = SM_PI * 0.4f * 0.7f;

	IXLightSpot *m_pLightSpot = NULL;
};

#endif
