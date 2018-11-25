
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Кнопка
*/

#ifndef __PROP_DEBRIS_H
#define __PROP_DEBRIS_H

#include "PropDynamic.h"

/*! Мусор
\ingroup cbaseanimating
*/
class CPropDebris: public CPropDynamic
{
	DECLARE_CLASS(CPropDebris, CPropDynamic);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	void sheduleRemove();
protected:

	void checkRemove(float fDT);
};

#endif
