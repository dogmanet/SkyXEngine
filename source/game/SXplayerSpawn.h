
/******************************************************
Copyright © Vitaliy Buturlin, Evgeny Danilovich, 2017
See the license in LICENSE
******************************************************/

/*!
\file
Точка респауна игрока
*/

/*! \ingroup cpointentity
@{
*/

#ifndef _SXplayerSpawn_H_
#define _SXplayerSpawn_H_

#include "SXpointEntity.h"

//! Точка респауна игрока
class SXplayerSpawn: public SXpointEntity
{
	DECLARE_CLASS(SXplayerSpawn, SXpointEntity);
	DECLARE_PROPTABLE();
public:
	DECLARE_TRIVIAL_CONSTRUCTOR();

	int m_iTeam;
};

#endif

//! @}
