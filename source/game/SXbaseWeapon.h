#ifndef _SXbaseWeapon_H_
#define _SXbaseWeapon_H_

#include "SXbaseTool.h"

class SXbaseWeapon: public SXbaseTool
{
	DECLARE_CLASS(SXbaseWeapon, SXbaseTool);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
