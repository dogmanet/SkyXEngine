#ifndef _SXbaseSnipe_H_
#define _SXbaseSnipe_H_

#include "SXbaseWeapon.h"

class SXbaseSnipe: public SXbaseWeapon
{
	DECLARE_CLASS(SXbaseSnipe, SXbaseWeapon);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
