#ifndef _SXbasePistol_H_
#define _SXbasePistol_H_

#include "SXbaseWeapon.h"

class SXbasePistol: public SXbaseWeapon
{
	DECLARE_CLASS(SXbasePistol, SXbaseWeapon);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
