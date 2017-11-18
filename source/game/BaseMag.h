/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Магазины
*/

#ifndef _BaseMag_H_
#define _BaseMag_H_

#include "BaseWeaponAddon.h"

/*! Магазины
\ingroup cbaseitem
*/
class CBaseMag: public CBaseWeaponAddon
{
	DECLARE_CLASS(CBaseMag, CBaseWeaponAddon);
	DECLARE_PROPTABLE();
public:
	DECLARE_CONSTRUCTOR();

	int getLoad();
	int getCapacity();
	void load(int count);
protected:
	int m_iCapacity;
	int m_iCurrentLoad;
};

#endif
