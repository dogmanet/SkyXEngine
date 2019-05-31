
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Модель
*/

#ifndef __PROP_STATIC_H
#define __PROP_STATIC_H

#include "BaseAnimating.h"

/*! Модель
\ingroup cbaseanimating
*/
class CPropStatic: public CBaseAnimating
{
	DECLARE_CLASS(CPropStatic, CBaseAnimating);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
	~CPropStatic();

	void createPhysBody() override;
	void removePhysBody() override;
protected:
};

#endif
