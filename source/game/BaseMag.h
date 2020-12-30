
/***********************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017, 2018
See the license in LICENSE
***********************************************************/

/*!
\file
Магазины
*/

#ifndef __BASE_MAG_H
#define __BASE_MAG_H

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
	int m_iCapacity = 0;
	int m_iCurrentLoad = 0;
};

#endif
