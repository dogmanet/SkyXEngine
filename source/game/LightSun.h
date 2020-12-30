
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Солнце
*/

#ifndef __LIGHTSUN_H
#define __LIGHTSUN_H

#include "BaseLight.h"

/*! Солнце
\ingroup clight
*/
class CLightSun: public CBaseLight
{
	DECLARE_CLASS(CLightSun, CBaseLight);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	void setOrient(const SMQuaternion &q) override;

protected:
	IXLightSun *m_pSun = NULL;
};

#endif
