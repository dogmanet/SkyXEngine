
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Точечный свет
*/

#ifndef __LIGHTPOINT_H
#define __LIGHTPOINT_H

#include "BaseLight.h"

/*! Точечный источник света
\ingroup clight
*/
class CLightPoint: public CBaseLight
{
	DECLARE_CLASS(CLightPoint, CBaseLight);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();
	~CLightPoint();
};

#endif
