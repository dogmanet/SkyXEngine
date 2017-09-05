#ifndef _SXbaseSupply_H_
#define _SXbaseSupply_H_

#include "SXbaseItem.h"

class SXbaseSupply: public SXbaseItem
{
	DECLARE_CLASS(SXbaseSupply, SXbaseItem);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
