#ifndef _BaseMag_H_
#define _BaseMag_H_

#include "BaseWeaponAddon.h"

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
