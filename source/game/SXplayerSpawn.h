#ifndef _SXplayerSpawn_H_
#define _SXplayerSpawn_H_

#include "SXpointEntity.h"

class SXplayerSpawn: public SXpointEntity
{
	DECLARE_CLASS(SXplayerSpawn, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

	int m_iTeam;
};

#endif
