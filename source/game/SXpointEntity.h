#ifndef _SXpointEntity_H_
#define _SXpointEntity_H_

#include "SXbaseEntity.h"

class SXpointEntity: public SXbaseEntity
{
	DECLARE_CLASS(SXpointEntity, SXbaseEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();
};

#endif
